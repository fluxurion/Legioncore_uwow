/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "ItemEnchantmentMgr.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ScriptMgr.h"
#include "ConditionMgr.h"
#include "ItemPackets.h"

void AddItemsSetItem(Player* player, Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);
    if (!set)
        return;

    if (set->RequiredSkill && player->GetSkillValue(set->RequiredSkill) < set->RequiredSkillValue)
        return;

    ItemSetEffect* eff = nullptr;
    for (size_t x = 0; x < player->ItemSetEff.size(); ++x)
    {
        if (player->ItemSetEff[x] && player->ItemSetEff[x]->ItemSetID == setid)
        {
            eff = player->ItemSetEff[x];
            break;
        }
    }

    if (!eff)
    {
        eff = new ItemSetEffect();
        eff->ItemSetID = setid;
        eff->EquippedItemCount = 0;

        size_t x = 0;
        for (; x < player->ItemSetEff.size(); ++x)
            if (!player->ItemSetEff[x])
                break;

        if (x < player->ItemSetEff.size())
            player->ItemSetEff[x] = eff;
        else
            player->ItemSetEff.push_back(eff);
    }

    ++eff->EquippedItemCount;

    DB2Manager::ItemSetSpells& spells = sDB2Manager._itemSetSpells[setid];
    for (ItemSetSpellEntry const* itemSetSpell : spells)
    {
        if (itemSetSpell->Threshold > eff->EquippedItemCount)
            continue;

        if (eff->SetBonuses.count(itemSetSpell))
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itemSetSpell->SpellID);
        if (!spellInfo)
            continue;

        eff->SetBonuses.insert(itemSetSpell);
        if (!itemSetSpell->ChrSpecID || itemSetSpell->ChrSpecID == player->GetSpecializationId(player->GetActiveSpec()))
            player->ApplyEquipSpell(spellInfo, nullptr, true);
    }
}

void RemoveItemsSetItem(Player*player, ItemTemplate const* proto)
{
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);
    if (!set)
        return;

    ItemSetEffect* eff = nullptr;
    size_t setindex = 0;
    for (; setindex < player->ItemSetEff.size(); setindex++)
    {
        if (player->ItemSetEff[setindex] && player->ItemSetEff[setindex]->ItemSetID == setid)
        {
            eff = player->ItemSetEff[setindex];
            break;
        }
    }

    if (!eff)
        return;

    --eff->EquippedItemCount;

    DB2Manager::ItemSetSpells const& spells = sDB2Manager._itemSetSpells[setid];
    for (ItemSetSpellEntry const* itemSetSpell : spells)
    {
        if (itemSetSpell->Threshold <= eff->EquippedItemCount)
            continue;

        if (!eff->SetBonuses.count(itemSetSpell))
            continue;

        player->ApplyEquipSpell(sSpellMgr->AssertSpellInfo(itemSetSpell->SpellID), nullptr, false);
        eff->SetBonuses.erase(itemSetSpell);
    }

    if (!eff->EquippedItemCount)
    {
        ASSERT(eff == player->ItemSetEff[setindex]);
        delete eff;
        player->ItemSetEff[setindex] = nullptr;
    }
}

bool ItemCanGoIntoBag(ItemTemplate const* pProto, ItemTemplate const* pBagProto)
{
    if (!pProto || !pBagProto)
        return false;

    switch (pBagProto->Class)
    {
        case ITEM_CLASS_CONTAINER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_CONTAINER:
                    return true;
                case ITEM_SUBCLASS_SOUL_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_SOUL_SHARDS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_HERB_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_HERBS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENCHANTING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENCHANTING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_MINING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_MINING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENGINEERING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENGINEERING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_GEM_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_GEMS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_LEATHERWORKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_LEATHERWORKING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_INSCRIPTION_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_INSCRIPTION_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_TACKLE_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_FISHING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_COOKING_BAG:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_COOKING_SUPP))
                        return false;
                    return true;
                default:
                    return false;
            }
        case ITEM_CLASS_QUIVER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_QUIVER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ARROWS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_AMMO_POUCH:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_BULLETS))
                        return false;
                    return true;
                default:
                    return false;
            }
    }
    return false;
}

Item::Item() : m_slot(0), uState(ITEM_NEW), uQueuePos(-1), m_container(NULL), m_lootGenerated(false),
mb_in_trade(false), m_in_use(false), m_paidMoney(0), m_paidExtendedCost(0)
{
    m_objectType |= TYPEMASK_ITEM;
    m_objectTypeId = TYPEID_ITEM;

    m_updateFlag = 0;

    m_valuesCount = ITEM_END;
    _dynamicValuesCount = ITEM_DYNAMIC_END;

    m_lastPlayedTimeUpdate = time(NULL);

    m_refundRecipient.Clear();

    memset(_modifiers, 0, sizeof(_modifiers));

    m_scaleLvl = 0;
    memset(&_bonusData, 0, sizeof(_bonusData));
}

bool Item::Create(ObjectGuid::LowType const& guidlow, uint32 itemid, Player const* owner)
{
    Object::_Create(ObjectGuid::Create<HighGuid::Item>(guidlow));

    SetEntry(itemid);
    SetObjectScale(1.0f);

    if (owner)
    {
        SetGuidValue(ITEM_FIELD_OWNER, owner->GetGUID());
        SetGuidValue(ITEM_FIELD_CONTAINED_IN, owner->GetGUID());
    }

    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemid);
    if (!itemProto)
        return false;

    _bonusData.Initialize(itemProto, GetOwner());

    SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, itemProto->MaxDurability);
    SetUInt32Value(ITEM_FIELD_DURABILITY, itemProto->MaxDurability);

    for (uint8 i = 0; i < itemProto->Effects.size() && i < MAX_ITEM_PROTO_SPELLS; ++i)
        SetSpellCharges(i, itemProto->Effects[i]->Charges);

    SetUInt32Value(ITEM_FIELD_EXPIRATION, itemProto->Duration);
    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, 0);

    //ItemLevel = itemProto->ItemLevel;
    //if (ItemUpgradeData const* upgradeData = sDB2Manager.GetItemUpgradeData(itemid))
    //    if (ItemUpgradeEntry const* upgradeEntry = upgradeData->upgrade[0])
    //    {
    //        SetUpgradeId(upgradeEntry->id);
    //        ItemLevel += upgradeEntry->levelBonus;
    //    }

    return true;
}

// Returns true if Item is a bag AND it is not empty.
// Returns false if Item is not a bag OR it is an empty bag.
bool Item::IsNotEmptyBag() const
{
    if (Bag const* bag = ToBag())
        return !bag->IsEmpty();
    return false;
}

void Item::UpdateDuration(Player* owner, uint32 diff)
{
    if (!GetUInt32Value(ITEM_FIELD_EXPIRATION))
        return;

    sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::UpdateDuration Item (Entry: %u Duration %u Diff %u)", GetEntry(), GetUInt32Value(ITEM_FIELD_EXPIRATION), diff);

    if (GetUInt32Value(ITEM_FIELD_EXPIRATION) <= diff)
    {
        sScriptMgr->OnItemExpire(owner, GetTemplate());
        owner->DestroyItem(GetBagSlot(), GetSlot(), true);
        return;
    }

    SetUInt32Value(ITEM_FIELD_EXPIRATION, GetUInt32Value(ITEM_FIELD_EXPIRATION) - diff);
    SetState(ITEM_CHANGED, owner);                          // save new time in database
}

void Item::SaveToDB(SQLTransaction& trans)
{
    bool isInTransaction = bool(trans);
    if (!isInTransaction)
        trans = CharacterDatabase.BeginTransaction();

    ObjectGuid::LowType guid = GetGUIDLow();
    switch (uState)
    {
        case ITEM_NEW:
        case ITEM_CHANGED:
        {
            uint8 index = 0;
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(uState == ITEM_NEW ? CHAR_REP_ITEM_INSTANCE : CHAR_UPD_ITEM_INSTANCE);
            stmt->setUInt32(index, GetEntry());
            stmt->setUInt64(++index, GetOwnerGUID().GetCounter());
            stmt->setUInt64(++index, GetGuidValue(ITEM_FIELD_CREATOR).GetCounter());
            stmt->setUInt64(++index, GetGuidValue(ITEM_FIELD_GIFT_CREATOR).GetCounter());
            stmt->setUInt32(++index, GetCount());
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_EXPIRATION));

            std::ostringstream ssSpells;
            if (ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(GetEntry()))
                for (uint8 i = 0; i < itemProto->Effects.size(); ++i)
                {
                    if (GetEntry() == 38186 && GetSpellCharges(i)) //Efir not have charges
                    {
                        sWorld->BanAccount(BAN_CHARACTER, GetOwner()->GetName(), "-1", "Dupe efir", "System");
                        delete this;
                        return;
                    }
                    ssSpells << GetSpellCharges(i) << ' ';
                }
            stmt->setString(++index, ssSpells.str());

            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));

            std::ostringstream ssEnchants;
            for (uint8 i = 0; i < MAX_ENCHANTMENT_SLOT; ++i)
            {
                ssEnchants << GetEnchantmentId(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentDuration(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentCharges(EnchantmentSlot(i)) << ' ';
            }
            stmt->setString(++index, ssEnchants.str());

            stmt->setInt16(++index, GetItemRandomPropertyId());
            stmt->setUInt16(++index, GetUInt32Value(ITEM_FIELD_DURABILITY));
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME));
            //stmt->setString(++index, m_text);
            stmt->setString(++index, "");
            stmt->setUInt32(++index, GetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID) | (GetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD) << 24));
            stmt->setUInt32(++index, GetModifier(ITEM_MODIFIER_UPGRADE_ID));
            stmt->setUInt32(++index, GetModifier(ITEM_MODIFIER_ENCHANT_ILLUSION));

            std::ostringstream bonusListIDs;
            for (uint32 bonusListID : GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS))
                bonusListIDs << bonusListID << ' ';
            stmt->setString(++index, bonusListIDs.str());
            stmt->setUInt64(++index, guid);

            trans->Append(stmt);

            if ((uState == ITEM_CHANGED) && HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GIFT_OWNER);
                stmt->setUInt64(0, GetOwnerGUID().GetCounter());
                stmt->setUInt64(1, guid);
                trans->Append(stmt);
            }
            break;
        }
        case ITEM_REMOVED:
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
            stmt->setUInt64(0, guid);
            trans->Append(stmt);

            if (HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);
                stmt->setUInt64(0, guid);
                trans->Append(stmt);
            }

            if (!isInTransaction)
                CharacterDatabase.CommitTransaction(trans);

            //CharacterDatabase.PExecute("UPDATE character_donate SET state = 1, deletedate = '%s' WHERE itemguid = '%u'", TimeToTimestampStr(time(NULL)).c_str(), guid);
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_ITEM_DONATE_SET_STATE);
            stmt->setUInt32(0, 1);
            stmt->setString(1, TimeToTimestampStr(time(NULL)).c_str());
            stmt->setUInt64(2, guid);
            trans->Append(stmt);

            delete this;
            return;
        }
        case ITEM_UNCHANGED:
            break;
    }

    SetState(ITEM_UNCHANGED);

    if (!isInTransaction)
        CharacterDatabase.CommitTransaction(trans);
}

bool Item::LoadFromDB(ObjectGuid::LowType const& guid, ObjectGuid const& owner_guid, Field* fields, uint32 entry)
{
    // WARNING! WARNING! WARNING! IF CHANGE FIELD ORDER - DO IT ON GuildMgr::LoadGuilds FOR GB Loading Items
    // WARNING! AND CHAR_SEL_MAILITEMS CHAR_SEL_AUCTION_ITEMS 
    //          0               1             2     3           4       5       6               7               8           9        10         11              12          13              14          15    16    17          18      19       
    //SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, durability, playedTime, text, transmogrification, upgradeId, enchantIllusion, bonusListIDs, bag, slot, ii.guid, itemEntry, item

    // create item before any checks for store correct guid
    // and allow use "FSetState(ITEM_REMOVED); SaveToDB();" for deleting item from DB
    Object::_Create(ObjectGuid::Create<HighGuid::Item>(guid));

    // Set entry, MUST be before proto check
    SetEntry(entry);
    SetObjectScale(1.0f);

    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    _bonusData.Initialize(proto, GetOwner());

    // set owner (not if item is only loaded for gbank/auction/mail
    if (!owner_guid.IsEmpty())
        SetOwnerGUID(owner_guid);

    bool need_save = false;                                 // need explicit save data at load fixes
    if (uint64 creator = fields[0].GetUInt64())
        SetGuidValue(ITEM_FIELD_CREATOR, ObjectGuid::Create<HighGuid::Player>(creator));
    if (uint64 giftCreator = fields[1].GetUInt64())
        SetGuidValue(ITEM_FIELD_GIFT_CREATOR, ObjectGuid::Create<HighGuid::Player>(giftCreator));
    SetCount(fields[2].GetUInt32());

    uint32 duration = fields[3].GetUInt32();
    SetUInt32Value(ITEM_FIELD_EXPIRATION, duration);
    // update duration if need, and remove if not need
    if ((proto->Duration == 0) != (duration == 0))
    {
        SetUInt32Value(ITEM_FIELD_EXPIRATION, proto->Duration);
        need_save = true;
    }

    Tokenizer tokens(fields[4].GetString(), ' ', proto->Effects.size());
    if (tokens.size() == proto->Effects.size())
        for (uint8 i = 0; i < proto->Effects.size(); ++i)
            SetSpellCharges(i, atoi(tokens[i]));

    SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, fields[5].GetUInt32());
    // Remove bind flag for items vs NO_BIND set
    if (IsSoulBound() && proto->Bonding == NO_BIND)
    {
        ApplyModFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_SOULBOUND, false);
        need_save = true;
    }

    std::string enchants = fields[6].GetString();
    _LoadIntoDataField(enchants.c_str(), ITEM_FIELD_ENCHANTMENT, MAX_ENCHANTMENT_SLOT * MAX_ENCHANTMENT_OFFSET);

    if (uint32 transmogEntry = fields[11].GetUInt32())
    {
        SetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD, (transmogEntry >> 24) & 0xFF);
        SetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID, transmogEntry & 0xFFFFFF);
    }
    SetModifier(ITEM_MODIFIER_UPGRADE_ID, fields[12].GetUInt32());
    SetModifier(ITEM_MODIFIER_ENCHANT_ILLUSION, fields[13].GetUInt32());

    Tokenizer bonusListIDs(fields[14].GetString(), ' ');
    for (char const* token : bonusListIDs)
    {
        uint32 bonusListID = atoul(token);
        AddBonuses(bonusListID);
    }

    SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, fields[7].GetInt16());
    // recalculate suffix factor
    if (GetItemRandomPropertyId() < 0)
        UpdateItemSuffixFactor();

    uint32 durability = fields[8].GetUInt16();
    SetUInt32Value(ITEM_FIELD_DURABILITY, durability);
    // update max durability (and durability) if need
    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, proto->MaxDurability);
    if (durability > proto->MaxDurability)
    {
        SetUInt32Value(ITEM_FIELD_DURABILITY, proto->MaxDurability);
        need_save = true;
    }

    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, fields[9].GetUInt32());
    SetText(fields[10].GetString());

    if (need_save)                                           // normal item changed state set not work at loading
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_INSTANCE_ON_LOAD);
        stmt->setUInt32(0, GetUInt32Value(ITEM_FIELD_EXPIRATION));
        stmt->setUInt32(1, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));
        stmt->setUInt32(2, GetUInt32Value(ITEM_FIELD_DURABILITY));
        stmt->setUInt64(3, guid);
        CharacterDatabase.Execute(stmt);
    }

    return true;
}

/*static*/
void Item::DeleteFromDB(SQLTransaction& trans, ObjectGuid::LowType itemGuid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
    stmt->setUInt64(0, itemGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_ITEM_DONATE_SET_STATE);
    stmt->setUInt32(0, 1);
    stmt->setString(1, TimeToTimestampStr(time(NULL)).c_str());
    stmt->setUInt64(2, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromDB(SQLTransaction& trans)
{
    DeleteFromDB(trans, GetGUIDLow());
}

/*static*/
void Item::DeleteFromInventoryDB(SQLTransaction& trans, ObjectGuid::LowType itemGuid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
    stmt->setUInt64(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromInventoryDB(SQLTransaction& trans)
{
    DeleteFromInventoryDB(trans, GetGUIDLow());
}

ItemTemplate const* Item::GetTemplate() const
{
    return sObjectMgr->GetItemTemplate(GetEntry());
}

Player* Item::GetOwner()const
{
    return ObjectAccessor::FindPlayer(GetOwnerGUID());
}

uint32 Item::GetSkill()
{
    const static uint32 item_weapon_skills[MAX_ITEM_SUBCLASS_WEAPON] =
    {
        SKILL_AXES,     SKILL_2H_AXES,  SKILL_BOWS,          SKILL_GUNS,            SKILL_MACES,
        SKILL_2H_MACES, SKILL_POLEARMS, SKILL_SWORDS,        SKILL_2H_SWORDS,       SKILL_WARGLAIVES,
        SKILL_STAVES,   0,              0,                   SKILL_FIST_WEAPONS,    0,
        SKILL_DAGGERS,  SKILL_THROWN,   SKILL_ASSASSINATION, SKILL_CROSSBOWS,       SKILL_WANDS,
        SKILL_FISHING
    };

    const static uint32 item_armor_skills[MAX_ITEM_SUBCLASS_ARMOR] =
    {
        0, SKILL_CLOTH, SKILL_LEATHER, SKILL_MAIL, SKILL_PLATE_MAIL, 0, SKILL_SHIELD, 0, 0, 0, 0
    };

    ItemTemplate const* proto = GetTemplate();

    switch (proto->Class)
    {
        case ITEM_CLASS_WEAPON:
            if (proto->SubClass >= MAX_ITEM_SUBCLASS_WEAPON)
                return 0;
            else
                return item_weapon_skills[proto->SubClass];

        case ITEM_CLASS_ARMOR:
            if (proto->SubClass >= MAX_ITEM_SUBCLASS_ARMOR)
                return 0;
            else
                return item_armor_skills[proto->SubClass];

        default:
            return 0;
    }
}

int32 Item::GenerateItemRandomPropertyId(uint32 item_id, uint32 spec_id)
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(item_id);

    if (!itemProto)
        return 0;

    // item must have one from this field values not null if it can have random enchantments
    if ((!itemProto->RandomProperty) && (!itemProto->RandomSuffix))
        return 0;

    // item can have not null only one from field values
    if ((itemProto->RandomProperty) && (itemProto->RandomSuffix))
    {
        sLog->outError(LOG_FILTER_SQL, "Item template %u have RandomProperty == %u and RandomSuffix == %u, but must have one from field =0", itemProto->ItemId, itemProto->RandomProperty, itemProto->RandomSuffix);
        return 0;
    }

    // RandomProperty case
    if (itemProto->RandomProperty)
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomProperty, ENCHANTMENT_RANDOM_PROPERTY, spec_id);
        ItemRandomPropertiesEntry const* random_id = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in 'ItemRandomProperties.dbc'", randomPropId);
            return 0;
        }

        return random_id->ID;
    }
    // RandomSuffix case
    else
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomSuffix, ENCHANTMENT_RANDOM_SUFFIX, spec_id);
        ItemRandomSuffixEntry const* random_id = sItemRandomSuffixStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in sItemRandomSuffixStore.", randomPropId);
            return 0;
        }

        return -int32(random_id->ID);
    }
}

void Item::SetItemRandomProperties(int32 randomPropId)
{
    if (!randomPropId)
        return;

    if (randomPropId > 0)
    {
        ItemRandomPropertiesEntry const* item_rand = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != int32(item_rand->ID))
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, item_rand->ID);
                SetState(ITEM_CHANGED, GetOwner());
            }
            for (uint32 i = PROP_ENCHANTMENT_SLOT_1; i < PROP_ENCHANTMENT_SLOT_1 + 3; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->Enchantment[i - PROP_ENCHANTMENT_SLOT_1], 0, 0);
        }
    }
    else
    {
        ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != -int32(item_rand->ID) ||
                !GetItemSuffixFactor())
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, -int32(item_rand->ID));
                UpdateItemSuffixFactor();
                SetState(ITEM_CHANGED, GetOwner());
            }

            for (uint32 i = PROP_ENCHANTMENT_SLOT_0; i <= PROP_ENCHANTMENT_SLOT_4; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->Enchantment[i - PROP_ENCHANTMENT_SLOT_0], 0, 0);
        }
    }
}

void Item::UpdateItemSuffixFactor()
{
    ItemTemplate const* proto = GetTemplate();
    uint32 suffixFactor = proto && proto->RandomSuffix ? GenerateEnchSuffixFactor(GetTemplate()) : 0;
    if (GetItemSuffixFactor() == suffixFactor)
        return;
    SetUInt32Value(ITEM_FIELD_PROPERTY_SEED, suffixFactor);
}

void Item::SetState(ItemUpdateState state, Player* forplayer)
{
    if (uState == ITEM_NEW && state == ITEM_REMOVED)
    {
        // pretend the item never existed
        RemoveFromUpdateQueueOf(forplayer);
        forplayer->DeleteRefundReference(GetGUID());
        delete this;
        return;
    }
    if (state != ITEM_UNCHANGED)
    {
        // new items must stay in new state until saved
        if (uState != ITEM_NEW)
            uState = state;

        AddToUpdateQueueOf(forplayer);
    }
    else
    {
        // unset in queue
        // the item must be removed from the queue manually
        uQueuePos = -1;
        uState = ITEM_UNCHANGED;
    }
}

void Item::AddToUpdateQueueOf(Player* player)
{
    if (IsInUpdateQueue())
        return;

    //ASSERT(player != NULL);
    // this come whet we levelup chaar from battlepay
    if (!player)
        return;

    if (player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::AddToUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GetOwnerGUID().GetGUIDLow(), player->GetGUIDLow());
        return;
    }

    if (player->m_itemUpdateQueueBlocked)
        return;

    player->m_itemUpdateQueue.push_back(this);
    uQueuePos = player->m_itemUpdateQueue.size() - 1;
}

void Item::RemoveFromUpdateQueueOf(Player* player)
{
    if (!IsInUpdateQueue())
        return;

    //ASSERT(player != NULL);
    if (!player)
        return;

    if (player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::RemoveFromUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GetOwnerGUID().GetGUIDLow(), player->GetGUIDLow());
        return;
    }

    if (player->m_itemUpdateQueueBlocked)
        return;

    player->m_itemUpdateQueue[uQueuePos] = NULL;
    uQueuePos = -1;
}

uint8 Item::GetBagSlot() const
{
    return m_container ? m_container->GetSlot() : uint8(INVENTORY_SLOT_BAG_0);
}

bool Item::IsEquipped() const
{
    return !IsInBag() && m_slot < EQUIPMENT_SLOT_END;
}

bool Item::CanBeTraded(bool mail, bool trade) const
{
    if (m_lootGenerated)
        return false;

    if ((!mail || !IsBoundAccountWide()) && (IsSoulBound() && (!HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE) || !trade)))
        return false;

    if (IsBag() && (Player::IsBagPos(GetPos()) || !((Bag const*)this)->IsEmpty()))
        return false;

    if (Player* owner = GetOwner())
    {
        if (owner->CanUnequipItem(GetPos(), false) != EQUIP_ERR_OK)
            return false;
        if (owner->GetLootGUID() == GetGUID())
            return false;
    }

    if (IsBoundByEnchant())
        return false;

    return true;
}

bool Item::HasEnchantRequiredSkill(Player const* player) const
{
    for (uint32 i = PERM_ENCHANTMENT_SLOT; i < MAX_ENCHANTMENT_SLOT; ++i)
        if (uint32 id = GetEnchantmentId(EnchantmentSlot(i)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(id))
                if (enchantEntry->RequiredSkillID && player->GetSkillValue(enchantEntry->RequiredSkillID) < enchantEntry->RequiredSkillRank)
                    return false;

    return true;
}

uint32 Item::GetEnchantRequiredLevel() const
{
    uint32 level = 0;

    for (uint32 i = PERM_ENCHANTMENT_SLOT; i < MAX_ENCHANTMENT_SLOT; ++i)
        if (uint32 id = GetEnchantmentId(EnchantmentSlot(i)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(id))
                if (enchantEntry->MinLevel > level)
                    level = enchantEntry->MinLevel;

    return level;
}

bool Item::IsBoundByEnchant() const
{
    for (uint32 i = PERM_ENCHANTMENT_SLOT; i < MAX_ENCHANTMENT_SLOT; ++i)
        if (uint32 id = GetEnchantmentId(EnchantmentSlot(i)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(id))
                if (enchantEntry->Flags & ENCHANTMENT_CAN_SOULBOUND)
                    return true;

    return false;
}

InventoryResult Item::CanBeMergedPartlyWith(ItemTemplate const* proto) const
{
    // not allow merge looting currently items
    if (m_lootGenerated)
        return EQUIP_ERR_LOOT_GONE;

    // check item type
    if (GetEntry() != proto->ItemId)
        return EQUIP_ERR_CANT_STACK;

    // check free space (full stacks can't be target of merge
    if (GetCount() >= proto->GetMaxStackSize())
        return EQUIP_ERR_CANT_STACK;

    return EQUIP_ERR_OK;
}

bool Item::IsFitToSpellRequirements(SpellInfo const* spellInfo) const
{
    ItemTemplate const* proto = GetTemplate();

    if (spellInfo->EquippedItemClass != -1)                 // -1 == any item class
    {
        // Special case - accept vellum for armor/weapon requirements
        if ((spellInfo->EquippedItemClass == ITEM_CLASS_ARMOR ||
            spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON) && proto->IsVellum())
            if (IsPartOfSkillLine(SKILL_ENCHANTING, spellInfo->Id)) // only for enchanting spells
                return true;

        if (spellInfo->EquippedItemClass != int32(proto->Class))
            return false;                                   //  wrong item class

        if (spellInfo->EquippedItemSubClassMask != 0)        // 0 == any subclass
        {
            if ((spellInfo->EquippedItemSubClassMask & (1 << proto->SubClass)) == 0)
                return false;                               // subclass not present in mask
        }
    }

    if (spellInfo->EquippedItemInventoryTypeMask != 0)       // 0 == any inventory type
    {
        // Special case - accept weapon type for main and offhand requirements
        if (proto->GetInventoryType() == INVTYPE_WEAPON &&
            (spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONMAINHAND) ||
            spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONOFFHAND)))
            return true;
        else if ((spellInfo->EquippedItemInventoryTypeMask & (1 << proto->GetInventoryType())) == 0 && spellInfo->Id != 100130) // Hack Fix Wild Strikes
            return false;                                   // inventory type not present in mask
    }

    return true;
}

void Item::SetEnchantment(EnchantmentSlot slot, uint32 id, uint32 duration, uint32 charges, ObjectGuid caster /*= ObjectGuid::Empty*/)
{
    // Better lost small time at check in comparison lost time at item save to DB.
    if ((GetEnchantmentId(slot) == id) && (GetEnchantmentDuration(slot) == duration) && (GetEnchantmentCharges(slot) == charges))
        return;

    Player* owner = GetOwner();
    if (slot < MAX_INSPECTED_ENCHANTMENT_SLOT)
    {
        if (uint32 oldEnchant = GetEnchantmentId(slot))
            SendEnchantmentLog(owner, ObjectGuid::Empty, GetGUID(), GetEntry(), oldEnchant, slot);

        if (id)
            SendEnchantmentLog(owner, caster, GetGUID(), GetEntry(), id, slot);
    }

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot * MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_ID_OFFSET, id);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot * MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot * MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    ForceValuesUpdateAtIndex(ITEM_FIELD_ENCHANTMENT + slot * MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_ID_OFFSET);
    ForceValuesUpdateAtIndex(ITEM_FIELD_ENCHANTMENT + slot * MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET);
    ForceValuesUpdateAtIndex(ITEM_FIELD_ENCHANTMENT + slot * MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET);

    SetState(ITEM_CHANGED, owner);
}

void Item::SendEnchantmentLog(Player* player, ObjectGuid const& caster, ObjectGuid const& item, uint32 itemID, uint32 spellID, EnchantmentSlot slot)
{
    WorldPackets::Item::EnchantmentLog log;
    log.Caster = caster;
    log.Owner = player->GetGUID();
    log.ItemGUID = item;
    log.ItemID = itemID;
    log.EnchantSlot = spellID;
    log.Enchantment = slot;
    player->GetSession()->SendPacket(log.Write());
}

void Item::SendItemEnchantTimeUpdate(Player* player, ObjectGuid const& Itemguid, uint32 slot, uint32 duration)
{
    WorldPackets::Item::ItemEnchantTimeUpdate timeUpdate;
    timeUpdate.ItemGuid = Itemguid;
    timeUpdate.PlayerGuid = player->GetGUID();
    timeUpdate.Duration = duration;
    timeUpdate.Slot = slot;
    player->GetSession()->SendPacket(timeUpdate.Write());
}

void Item::SetEnchantmentDuration(EnchantmentSlot slot, uint32 duration, Player* owner)
{
    if (GetEnchantmentDuration(slot) == duration)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetState(ITEM_CHANGED, owner);
    // Cannot use GetOwner() here, has to be passed as an argument to avoid freeze due to hashtable locking
}

void Item::SetEnchantmentCharges(EnchantmentSlot slot, uint32 charges)
{
    if (GetEnchantmentCharges(slot) == charges)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::ClearEnchantment(EnchantmentSlot slot)
{
    if (!GetEnchantmentId(slot))
        return;

    for (uint8 x = 0; x < MAX_ITEM_ENCHANTMENT_EFFECTS; ++x)
        SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + x, 0);
    SetState(ITEM_CHANGED, GetOwner());
}

bool Item::GemsFitSockets() const
{
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint8 SocketColor = GetTemplate()->Socket[enchant_slot - SOCK_ENCHANTMENT_SLOT].Color;

        if (!SocketColor) // no socket slot
            continue;

        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id) // no gems on this socket
            return false;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry) // invalid gem id on this socket
            return false;

        uint8 GemColor = 0;

        /*uint32 gemid = enchantEntry->SRCItemID;
        if (gemid)
        {
            ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(gemid);
            if (gemProto)
            {
                GemPropertiesEntry const* gemProperty = sGemPropertiesStore.LookupEntry(gemProto->GemProperties);
                if (gemProperty)
                    GemColor = gemProperty->Type;
            }
        }*/

        if (!(GemColor & SocketColor)) // bad gem color on this socket
            return false;
    }
    return true;
}

uint8 Item::GetGemCountWithID(uint32 GemID) const
{
    uint8 count = 0;
    /*for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        if (GemID == enchantEntry->SRCItemID)
            ++count;
    }*/
    return count;
}

uint8 Item::GetGemCountWithLimitCategory(uint32 limitCategory) const
{
    uint8 count = 0;
    /*for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(enchantEntry->SRCItemID);
        if (!gemProto)
            continue;

        if (gemProto->ItemLimitCategory == limitCategory)
            ++count;
    }*/
    return count;
}

bool Item::IsLimitedToAnotherMapOrZone(uint32 cur_mapId, uint32 cur_zoneId) const
{
    ItemTemplate const* proto = GetTemplate();
    return proto && ((proto->Map && proto->Map != cur_mapId) || (proto->Area && proto->Area != cur_zoneId));
}

void Item::SendTimeUpdate(Player* owner)
{
    uint32 duration = GetUInt32Value(ITEM_FIELD_EXPIRATION);
    if (!duration)
        return;

    WorldPackets::Item::ItemTimeUpdate update;
    update.ItemGuid = GetGUID();
    update.DurationLeft = duration;
    owner->GetSession()->SendPacket(update.Write());
}

Item* Item::CreateItem(uint32 item, uint32 count, Player const* player)
{
    if (count < 1)
        return NULL;                                        //don't create item at zero count

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item);
    if (pProto)
    {
        if (count > pProto->GetMaxStackSize())
            count = pProto->GetMaxStackSize();

        ASSERT(count != 0 && "pProto->Stackable == 0 but checked at loading already");

        Item* pItem = NewItemOrBag(pProto);
        if (pItem->Create(sObjectMgr->GetGenerator<HighGuid::Item>()->Generate(), item, player))
        {
            pItem->SetCount(count);
            return pItem;
        }
        else
            delete pItem;
    }
    else
        ASSERT(false);
    return NULL;
}

Item* Item::CloneItem(uint32 count, Player const* player) const
{
    Item* newItem = CreateItem(GetEntry(), count, player);
    if (!newItem)
        return NULL;

    newItem->SetGuidValue(ITEM_FIELD_CREATOR, GetGuidValue(ITEM_FIELD_CREATOR));
    newItem->SetGuidValue(ITEM_FIELD_GIFT_CREATOR, GetGuidValue(ITEM_FIELD_GIFT_CREATOR));
    newItem->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));
    newItem->SetUInt32Value(ITEM_FIELD_EXPIRATION, GetUInt32Value(ITEM_FIELD_EXPIRATION));
    // player CAN be NULL in which case we must not update random properties because that accesses player's item update queue
    if (player)
        newItem->SetItemRandomProperties(GetItemRandomPropertyId());

    memcpy(newItem->_modifiers, _modifiers, sizeof(_modifiers));
    newItem->SetUInt32Value(ITEM_FIELD_MODIFIERS_MASK, GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK));

    return newItem;
}

bool Item::IsBindedNotWith(Player const* player) const
{
    // not binded item
    if (!IsSoulBound())
        return false;

    // own item
    if (GetOwnerGUID() == player->GetGUID())
        return false;

    if (HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE))
        if (allowedGUIDs.find(player->GetGUID()) != allowedGUIDs.end())
            return false;

    // BOA item case
    if (IsBoundAccountWide())
        return false;

    return true;
}

void Item::BuildUpdate(UpdateDataMapType& data_map)
{
    if (Player* owner = GetOwner())
        BuildFieldsUpdate(owner, data_map);
    ClearUpdateMask(false);
}

void Item::BuildDynamicValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    if (!target)
        return;

    ByteBuffer fieldBuffer;
    UpdateMask updateMask;
    updateMask.SetCount(_dynamicValuesCount);

    uint32* flags = nullptr;
    uint32 visibleFlag = GetDynamicUpdateFieldData(target, flags);

    for (uint16 index = 0; index < _dynamicValuesCount; ++index)
    {
        ByteBuffer buffer;
        std::vector<uint32> const& values = _dynamicValues[index];
        if (_fieldNotifyFlags & flags[index] ||
            ((updateType == UPDATETYPE_VALUES ? _dynamicChangesMask.GetBit(index) : !values.empty()) && (flags[index] & visibleFlag)) ||
            (index == ITEM_DYNAMIC_FIELD_MODIFIERS && (updateType == UPDATETYPE_VALUES ? _changedFields[ITEM_FIELD_MODIFIERS_MASK] : GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK))))
        {
            updateMask.SetBit(index);

            UpdateMask arrayMask;
            if (index != ITEM_DYNAMIC_FIELD_MODIFIERS)
            {
                arrayMask.SetCount(values.size());
                for (size_t v = 0; v < values.size(); ++v)
                {
                    if (updateType != UPDATETYPE_VALUES || _dynamicChangesArrayMask[index].GetBit(v))
                    {
                        arrayMask.SetBit(v);
                        buffer << uint32(values[v]);
                    }
                }
            }
            else
            {
                uint32 count = 0;
                arrayMask.SetCount(MAX_ITEM_MODIFIERS);
                for (uint32 v = 0; v < MAX_ITEM_MODIFIERS; ++v)
                {
                    if (uint32 modifier = _modifiers[v])
                    {
                        arrayMask.SetBit(count++);
                        buffer << uint32(modifier);
                    }
                }

            }

            fieldBuffer << uint8(arrayMask.GetBlockCount());
            arrayMask.AppendToPacket(&fieldBuffer);
            fieldBuffer.append(buffer);
        }
    }

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
}

void Item::SaveRefundDataToDB()
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
    stmt->setUInt64(0, GetGUIDLow());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ITEM_REFUND_INSTANCE);
    stmt->setUInt64(0, GetGUIDLow());
    stmt->setUInt64(1, GetRefundRecipient().GetCounter());
    stmt->setUInt32(2, GetPaidMoney());
    stmt->setUInt16(3, uint16(GetPaidExtendedCost()));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void Item::DeleteRefundDataFromDB(SQLTransaction* trans)
{
    if (trans)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
        stmt->setUInt64(0, GetGUIDLow());
        (*trans)->Append(stmt);

    }
}

void Item::SetNotRefundable(Player* owner, bool changestate /*=true*/, SQLTransaction* trans /*=NULL*/)
{
    if (!HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_REFUNDABLE))
        return;

    WorldPackets::Item::ItemExpirePurchaseRefund itemExpirePurchaseRefund;
    itemExpirePurchaseRefund.ItemGUID = GetGUID();
    owner->SendDirectMessage(itemExpirePurchaseRefund.Write());

    RemoveFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_REFUNDABLE);
    // Following is not applicable in the trading procedure
    if (changestate)
        SetState(ITEM_CHANGED, owner);

    SetRefundRecipient(ObjectGuid::Empty);
    SetPaidMoney(0);
    SetPaidExtendedCost(0);
    DeleteRefundDataFromDB(trans);

    owner->DeleteRefundReference(GetGUID());
}

void Item::UpdatePlayedTime(Player* owner)
{
    /*  Here we update our played time
        We simply add a number to the current played time,
        based on the time elapsed since the last update hereof.
    */
    // Get current played time
    uint32 current_playtime = GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME);
    // Calculate time elapsed since last played time update
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    uint32 new_playtime = current_playtime + elapsed;
    // Check if the refund timer has expired yet
    if (new_playtime <= 2 * HOUR)
    {
        // No? Proceed.
        // Update the data field
        SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, new_playtime);
        // Flag as changed to get saved to DB
        SetState(ITEM_CHANGED, owner);
        // Speaks for itself
        m_lastPlayedTimeUpdate = curtime;
        return;
    }
    // Yes
    SetNotRefundable(owner);
}

uint32 Item::GetPlayedTime()
{
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    return GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + elapsed;
}

bool Item::IsRefundExpired()
{
    return (GetPlayedTime() > 2 * HOUR);
}

void Item::SetSoulboundTradeable(GuidSet const& allowedLooters)
{
    SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE);
    allowedGUIDs = allowedLooters;
}

void Item::ClearSoulboundTradeable(Player* currentOwner)
{
    RemoveFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE);
    if (allowedGUIDs.empty())
        return;

    allowedGUIDs.clear();
    SetState(ITEM_CHANGED, currentOwner);
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_BOP_TRADE);
    stmt->setUInt64(0, GetGUIDLow());
    CharacterDatabase.Execute(stmt);
}

bool Item::CheckSoulboundTradeExpire()
{
    // called from owner's update - GetOwner() MUST be valid
    if (GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + 2 * HOUR < GetOwner()->GetTotalPlayedTime())
    {
        ClearSoulboundTradeable(GetOwner());
        return true; // remove from tradeable list
    }

    return false;
}

bool Item::HasStats(WorldPackets::Item::ItemInstance const& itemInstance, BonusData const* bonus)
{
    if (itemInstance.RandomPropertiesID != 0)
        return true;

    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (bonus->ItemStatValue[i] != 0)
            return true;

    return false;
}

bool Item::IsValidTransmogrificationTarget() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    if (proto->GetQuality() == ITEM_QUALITY_LEGENDARY)
        return false;

    if (proto->GetClass() != ITEM_CLASS_ARMOR && proto->GetClass() != ITEM_CLASS_WEAPON)
        return false;

    if (proto->GetClass() == ITEM_CLASS_WEAPON && proto->GetSubClass() == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (proto->GetFlags2() & ITEM_FLAGS_EXTRA_CANNOT_BE_TRANSMOG)
        return false;

    if (!HasStats())
        return false;

    return true;
}

bool Item::IsValidTransmogrificationSource(WorldPackets::Item::ItemInstance const& transmogrifier, BonusData const* bonus)
{
    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(transmogrifier.ItemID);
    if (!proto)
        return false;

    if (proto->GetFlags2() & ITEM_FLAGS_EXTRA_CANNOT_TRANSMOG)
        return false;

    if (proto->GetQuality() == ITEM_QUALITY_LEGENDARY)
        return false;

    if (proto->GetClass() != ITEM_CLASS_ARMOR && proto->GetClass() != ITEM_CLASS_WEAPON)
        return false;

    if (proto->GetClass() == ITEM_CLASS_WEAPON && proto->GetSubClass() == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (proto->GetFlags2() & ITEM_FLAGS_EXTRA_CAN_TRANSMOG)
        return true;

    if (!HasStats(transmogrifier, bonus))
        return false;

    return true;
}

enum class ItemTransmogrificationWeaponCategory : uint8
{
    // Two-handed
    MELEE_2H,
    RANGED,

    // One-handed
    AXE_MACE_SWORD_1H,
    DAGGER,
    FIST,
    WARGLAIVES,

    INVALID
};

static ItemTransmogrificationWeaponCategory GetTransmogrificationWeaponCategory(ItemTemplate const* proto)
{
    if (proto->GetClass() == ITEM_CLASS_WEAPON)
    {
        switch (proto->GetSubClass())
        {
            case ITEM_SUBCLASS_WEAPON_AXE2:
            case ITEM_SUBCLASS_WEAPON_MACE2:
            case ITEM_SUBCLASS_WEAPON_SWORD2:
            case ITEM_SUBCLASS_WEAPON_STAFF:
            case ITEM_SUBCLASS_WEAPON_POLEARM:
                return ItemTransmogrificationWeaponCategory::MELEE_2H;
            case ITEM_SUBCLASS_WEAPON_BOW:
            case ITEM_SUBCLASS_WEAPON_GUN:
            case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                return ItemTransmogrificationWeaponCategory::RANGED;
            case ITEM_SUBCLASS_WEAPON_AXE:
            case ITEM_SUBCLASS_WEAPON_MACE:
            case ITEM_SUBCLASS_WEAPON_SWORD:
                return ItemTransmogrificationWeaponCategory::AXE_MACE_SWORD_1H;
            case ITEM_SUBCLASS_WEAPON_DAGGER:
                return ItemTransmogrificationWeaponCategory::DAGGER;
            case ITEM_SUBCLASS_WEAPON_FIST_WEAPON:
                return ItemTransmogrificationWeaponCategory::FIST;
            case ITEM_SUBCLASS_WEAPON_WARGLAIVES:
                return ItemTransmogrificationWeaponCategory::WARGLAIVES;
            default:
                break;
        }
    }

    return ItemTransmogrificationWeaponCategory::INVALID;
}

int32 const ItemTransmogrificationSlots[MAX_INVTYPE] =
{
    -1,                                                     // INVTYPE_NON_EQUIP
    EQUIPMENT_SLOT_HEAD,                                    // INVTYPE_HEAD
    EQUIPMENT_SLOT_NECK,                                    // INVTYPE_NECK
    EQUIPMENT_SLOT_SHOULDERS,                               // INVTYPE_SHOULDERS
    EQUIPMENT_SLOT_BODY,                                    // INVTYPE_BODY
    EQUIPMENT_SLOT_CHEST,                                   // INVTYPE_CHEST
    EQUIPMENT_SLOT_WAIST,                                   // INVTYPE_WAIST
    EQUIPMENT_SLOT_LEGS,                                    // INVTYPE_LEGS
    EQUIPMENT_SLOT_FEET,                                    // INVTYPE_FEET
    EQUIPMENT_SLOT_WRISTS,                                  // INVTYPE_WRISTS
    EQUIPMENT_SLOT_HANDS,                                   // INVTYPE_HANDS
    -1,                                                     // INVTYPE_FINGER
    -1,                                                     // INVTYPE_TRINKET
    -1,                                                     // INVTYPE_WEAPON
    EQUIPMENT_SLOT_OFFHAND,                                 // INVTYPE_SHIELD
    EQUIPMENT_SLOT_MAINHAND,                                // INVTYPE_RANGED
    EQUIPMENT_SLOT_BACK,                                    // INVTYPE_CLOAK
    -1,                                                     // INVTYPE_2HWEAPON
    -1,                                                     // INVTYPE_BAG
    EQUIPMENT_SLOT_TABARD,                                  // INVTYPE_TABARD
    EQUIPMENT_SLOT_CHEST,                                   // INVTYPE_ROBE
    EQUIPMENT_SLOT_MAINHAND,                                // INVTYPE_WEAPONMAINHAND
    EQUIPMENT_SLOT_OFFHAND,                                 // INVTYPE_WEAPONOFFHAND
    EQUIPMENT_SLOT_OFFHAND,                                 // INVTYPE_HOLDABLE
    -1,                                                     // INVTYPE_AMMO
    EQUIPMENT_SLOT_MAINHAND,                                // INVTYPE_THROWN
    EQUIPMENT_SLOT_MAINHAND,                                // INVTYPE_RANGEDRIGHT
    -1,                                                     // INVTYPE_QUIVER
    -1                                                      // INVTYPE_RELIC
};

bool Item::CanTransmogrifyItemWithItem(Item const* transmogrified, WorldPackets::Item::ItemInstance const& transmogrifier, BonusData const* bonus)
{
    ItemTemplate const* source = sObjectMgr->GetItemTemplate(transmogrifier.ItemID);
    ItemTemplate const* target = transmogrified->GetTemplate();

    if (!source || !target)
        return false;

    if (sDB2Manager.GetItemDisplayId(source->ItemId, bonus->AppearanceModID) == transmogrified->GetDisplayId())
        return false;

    if (!IsValidTransmogrificationSource(transmogrifier, bonus) || !transmogrified->IsValidTransmogrificationTarget())
        return false;

    if (source->GetClass() != target->GetClass())
        return false;

    if (source->GetInventoryType() == INVTYPE_BAG ||
        source->GetInventoryType() == INVTYPE_RELIC ||
        source->GetInventoryType() == INVTYPE_BODY ||
        source->GetInventoryType() == INVTYPE_FINGER ||
        source->GetInventoryType() == INVTYPE_TRINKET ||
        source->GetInventoryType() == INVTYPE_AMMO ||
        source->GetInventoryType() == INVTYPE_QUIVER)
        return false;

    if (source->GetSubClass() != target->GetSubClass())
    {
        if (source->GetClass() != ITEM_CLASS_WEAPON)
            return false;

        if (GetTransmogrificationWeaponCategory(source) != GetTransmogrificationWeaponCategory(target))
            return false;
    }

    if (source->GetInventoryType() != target->GetInventoryType())
    {
        int32 sourceSlot = ItemTransmogrificationSlots[source->GetInventoryType()];
        if (sourceSlot == -1 && source->GetInventoryType() == INVTYPE_WEAPON && (target->GetInventoryType() == INVTYPE_WEAPONMAINHAND || target->GetInventoryType() == INVTYPE_WEAPONOFFHAND))
            sourceSlot = ItemTransmogrificationSlots[target->GetInventoryType()];

        if (sourceSlot != ItemTransmogrificationSlots[target->GetInventoryType()])
            return false;
    }

    return true;
}

bool Item::HasStats() const
{
    if (GetItemRandomPropertyId() != 0)
        return true;

    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatValue != 0)
            return true;

    return false;
}

// used by mail items, transmog cost, stationeryinfo and others
uint32 Item::GetSellPrice(ItemTemplate const* proto, bool& normalSellPrice)
{
    normalSellPrice = true;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_HAS_NORMAL_PRICE)
        return proto->BuyPrice;
    else
    {
        ImportPriceQualityEntry const* qualityPrice = sImportPriceQualityStore.LookupEntry(proto->Quality + 1);
        ItemPriceBaseEntry const* basePrice = sItemPriceBaseStore.LookupEntry(proto->ItemLevel);

        if (!qualityPrice || !basePrice)
            return 0;

        float baseFactor = 0.0f;

        uint32 inventoryType = proto->GetInventoryType();

        if (inventoryType == INVTYPE_WEAPON ||
            inventoryType == INVTYPE_2HWEAPON ||
            inventoryType == INVTYPE_WEAPONMAINHAND ||
            inventoryType == INVTYPE_WEAPONOFFHAND ||
            inventoryType == INVTYPE_RANGED ||
            inventoryType == INVTYPE_THROWN ||
            inventoryType == INVTYPE_RANGEDRIGHT)
            baseFactor = basePrice->Weapon;
        else
            baseFactor = basePrice->Armor;

        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        float typeFactor = 0.0f;
        uint8 wepType = -1;

        switch (inventoryType)
        {
            case INVTYPE_HEAD:
            case INVTYPE_SHOULDERS:
            case INVTYPE_CHEST:
            case INVTYPE_WAIST:
            case INVTYPE_LEGS:
            case INVTYPE_FEET:
            case INVTYPE_WRISTS:
            case INVTYPE_HANDS:
            case INVTYPE_CLOAK:
            {
                ImportPriceArmorEntry const* armorPrice = sImportPriceArmorStore.LookupEntry(inventoryType);
                if (!armorPrice)
                    return 0;

                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_ARMOR_MISCELLANEOUS:
                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                        typeFactor = armorPrice->ClothModifier;
                        break;
                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                        typeFactor = armorPrice->LeatherModifier;
                        break;
                    case ITEM_SUBCLASS_ARMOR_MAIL:
                        typeFactor = armorPrice->ChainModifier;
                        break;
                    case ITEM_SUBCLASS_ARMOR_PLATE:
                        typeFactor = armorPrice->PlateModifier;
                        break;
                    default:
                        return 0;
                }

                break;
            }
            case INVTYPE_SHIELD:
            {
                ImportPriceShieldEntry const* shieldPrice = sImportPriceShieldStore.LookupEntry(1); // it only has two rows, it's unclear which is the one used
                if (!shieldPrice)
                    return 0;

                typeFactor = shieldPrice->Data;
                break;
            }
            case INVTYPE_WEAPONMAINHAND:
                wepType = 0;
                break;
            case INVTYPE_WEAPONOFFHAND:
                wepType = 1;
                break;
            case INVTYPE_WEAPON:
                wepType = 2;
                break;
            case INVTYPE_2HWEAPON:
                wepType = 3;
                break;
            case INVTYPE_RANGED:
            case INVTYPE_RANGEDRIGHT:
            case INVTYPE_RELIC:
            {
                wepType = 4;

                ImportPriceWeaponEntry const* weaponPrice = sImportPriceWeaponStore.LookupEntry(wepType + 1);
                if (!weaponPrice)
                    return 0;

                typeFactor = weaponPrice->Data;
                break;
            }
            default:
                return proto->BuyPrice;
        }

        normalSellPrice = false;
        return (uint32)(qualityPrice->Data * proto->Unk430_2 * proto->Unk430_1 * typeFactor * baseFactor);
    }
}

uint32 Item::GetSpecialPrice(ItemTemplate const* proto, uint32 minimumPrice /*= 10000*/)
{
    uint32 cost = 0;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_HAS_NORMAL_PRICE)
        cost = proto->SellPrice;
    else
    {
        bool normalPrice;
        cost = GetSellPrice(proto, normalPrice);

        if (!normalPrice)
        {
            if (proto->BuyCount <= 1)
            {
                ItemClassEntry const* classEntry = sItemClassStore.LookupEntry(proto->Class);
                if (classEntry)
                    cost *= classEntry->PriceFactor;
                else
                    cost = 0;
            }
            else
                cost /= 4 * proto->BuyCount;
        }
        else
            cost = proto->SellPrice;
    }

    if (cost < minimumPrice)
        cost = minimumPrice;

    return cost;
}

void Item::SetUpgradeId(uint32 value)
{
    SetModifier(ITEM_MODIFIER_UPGRADE_ID, value);
}

uint32 Item::GetUpgradeId() const
{
    return GetModifier(ITEM_MODIFIER_UPGRADE_ID);
}

//!@ For all inventoty items at add to world.
//! TODO: FIX ME.
void Item::SetLevelCap(uint32 cap, bool pvp)
{
    //// Restore levle before cap
    //if (cap == 0)
    //{
    //    // can't add proto lvl as we can have upgrade items.
    //    if (ItemLevelBeforeCap)
    //    {
    //        SetLevel(ItemLevelBeforeCap);
    //        ItemLevelBeforeCap = 0;
    //    }
    //    return;
    //}

    //ItemTemplate const* proto = GetTemplate();
    //if (!proto || proto->ItemLevel <= MIN_ITEM_LEVEL_CUP)
    //    return;

    //// nothing to do
    //if (cap >= GetLevel())
    //    return;

    //// Not for items with pvp power at pvp
    //if (pvp && proto->IsPvPItem())
        //return;

    //ItemLevelBeforeCap = GetLevel();
    //SetLevel(cap);
}

uint32 Item::GetItemLevel() const
{
    ItemTemplate const* stats = GetTemplate();
    if (!stats)
        return MIN_ITEM_LEVEL;

    return std::min(std::max(_bonusData.ItemLevel + GetScaleIlvl(), uint32(MIN_ITEM_LEVEL)), uint32(MAX_ITEM_LEVEL));
}

int32 Item::GetItemStatValue(uint32 index) const
{
    ASSERT(index < MAX_ITEM_PROTO_STATS);

    if (uint32 randomPropPoints = GetRandomPropertyPoints(GetItemLevel(), GetQuality(), GetTemplate()->GetInventoryType(), GetTemplate()->GetSubClass()))
    {
        int32 statAlloc = _bonusData.ItemStatAllocation[index] * randomPropPoints;
        float statValue = statAlloc * 0.0001f;

        if (GtItemSocketCostPerLevelEntry const* gtCost = sGtItemSocketCostPerLevelStore.EvaluateTable(GetItemLevel() - 1, 0))
            statValue -= float(int32(_bonusData.ItemStatSocketCostMultiplier[index] * gtCost->ratio));

        return int32(std::floor(statValue + 0.5f));
    }

    return _bonusData.ItemStatValue[index];
}

uint32 Item::GetDisplayId() const
{
    if (uint32 transmogrification = GetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID))
        return sDB2Manager.GetItemDisplayId(transmogrification, GetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD));

    return sDB2Manager.GetItemDisplayId(GetEntry(), GetAppearanceModId());
}

void Item::SetModifier(ItemModifier modifier, uint32 value)
{
    _modifiers[modifier] = value;
    ApplyModFlag(ITEM_FIELD_MODIFIERS_MASK, /*battlePet ? (8 << modifier) : */(1 << modifier), value != 0);
}

uint32 Item::GetVisibleEntry() const
{
    if (uint32 transmogrification = GetModifier(ITEM_MODIFIER_TRANSMOG_ITEM_ID))
        return transmogrification;

    return GetEntry();
}

uint32 Item::GetVisibleAppearanceModId() const
{
    if (uint32 transmogMod = GetModifier(ITEM_MODIFIER_TRANSMOG_APPEARANCE_MOD))
        return transmogMod;

    return GetAppearanceModId();
}

void Item::AddBonuses(uint32 bonusListID)
{
    if (DB2Manager::ItemBonusContainer const* bonuses = sDB2Manager.GetItemBonusList(bonusListID))
    {
        AddDynamicValue(ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS, bonusListID);
        for (ItemBonusEntry const* bonus : *bonuses)
            _bonusData.AddBonus(bonus->Type, bonus->Value);
    }
}

void BonusData::Initialize(ItemTemplate const* proto, Player const* owner)
{
    ItemLevel = proto->GetBaseItemLevel();

    // scaling ilvl for heilrooms
    if (owner)
    {
        if (ScalingStatDistributionEntry const* ssd = sScalingStatDistributionStore.LookupEntry(proto->GetScalingStatDistribution()))
            if (uint32 HeilroomItemLevel = sDB2Manager.GetHeirloomItemLevel(ssd->ItemLevelCurveID, owner->getLevel()))
                ItemLevel = HeilroomItemLevel;
    }

    Quality = proto->GetQuality();
    RequiredLevel = proto->GetBaseRequiredLevel();

    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        ItemStatType[i] = proto->GetItemStatType(i);

    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        ItemStatValue[i] = proto->GetItemStatValue(i);

    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        ItemStatAllocation[i] = proto->GetItemStatAllocation(i);

    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        ItemStatSocketCostMultiplier[i] = proto->GetItemStatSocketCostMultiplier(i);

    for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS; ++i)
        SocketColor[i] = proto->GetSocketColor(i);

    AppearanceModID = 0;
    StatScalingMod = proto->StatScalingFactor;
}

void BonusData::Initialize(WorldPackets::Item::ItemInstance const& itemInstance)
{
    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemInstance.ItemID);
    if (!proto)
        return;

    Initialize(proto, NULL);

    if (itemInstance.ItemBonus)
        for (uint32 bonusListID : itemInstance.ItemBonus->BonusListIDs)
            if (DB2Manager::ItemBonusContainer const* bonuses = sDB2Manager.GetItemBonusList(bonusListID))
                for (ItemBonusEntry const* bonus : *bonuses)
                    AddBonus(bonus->Type, bonus->Value);
}

void BonusData::AddBonus(uint32 type, int32 const (&values)[2])
{
    switch (type)
    {
        case ITEM_BONUS_ITEM_LEVEL:
            ItemLevel += values[0];
            break;
        case ITEM_BONUS_STAT:
        {
            uint32 statIndex = 0;
            for (statIndex = 0; statIndex < MAX_ITEM_PROTO_STATS; ++statIndex)
                if (ItemStatType[statIndex] == values[0] || ItemStatType[statIndex] == -1)
                    break;

            if (statIndex < MAX_ITEM_PROTO_STATS)
            {
                ItemStatType[statIndex] = values[0];
                ItemStatAllocation[statIndex] += values[1];
            }
            break;
        }
        case ITEM_BONUS_QUALITY:
            if (Quality < static_cast<uint32>(values[0]))
                Quality = static_cast<uint32>(values[0]);
            break;
        case ITEM_BONUS_SOCKET:
        {
            uint32 socketCount = values[0];
            for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS && socketCount; ++i)
            {
                if (!SocketColor[i])
                {
                    SocketColor[i] = values[1];
                    --socketCount;
                }
            }
            break;
        }
        case ITEM_BONUS_APPEARANCE:
            if (AppearanceModID < static_cast<uint32>(values[0]))
                AppearanceModID = static_cast<uint32>(values[0]);
            break;
        case ITEM_BONUS_REQUIRED_LEVEL:
            RequiredLevel += values[0];
            break;
        case ITEM_BONUS_UNK:
            if (values[0] < 0xB)
            {
                if (static_cast<uint32>(values[1]) < UnkField240)
                {
                    UnkField234 = static_cast<uint32>(values[0]);
                    UnkField240 = static_cast<uint32>(values[1]);
                }
            }
            break;
        case ITEM_BONUS_STAT_SCALING_MOD:
            StatScalingMod *= values[0] * 0.0099999998f;
            break;
        case ITEM_BONUS_UNK2:
            UnkField248 = static_cast<uint32>(values[0]);
            break;
    }
}

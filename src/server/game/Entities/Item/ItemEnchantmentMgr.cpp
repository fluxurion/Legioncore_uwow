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

#include <stdlib.h>
#include <functional>
#include "ItemEnchantmentMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectMgr.h"
#include <list>
#include <vector>
#include "Util.h"

struct EnchStoreItem
{
    uint32  ench;
    float   chance;

    EnchStoreItem()
        : ench(0), chance(0) {}

    EnchStoreItem(uint32 _ench, float _chance)
        : ench(_ench), chance(_chance) {}
};

typedef std::vector<EnchStoreItem> EnchStoreList;
typedef std::unordered_map<uint32, EnchStoreList> EnchantmentStore;

static EnchantmentStore RandomPropertyItemEnch;
static EnchantmentStore RandomSuffixItemEnch;

void LoadRandomEnchantmentsTable()
{
    uint32 oldMSTime = getMSTime();

    RandomPropertyItemEnch.clear();                                 // for reload case
    RandomSuffixItemEnch.clear();

    //                                                 0      1      2
    QueryResult result = WorldDatabase.Query("SELECT entry, ench, chance, type FROM item_enchantment_template");

    if (result)
    {
        uint32 count = 0;

        do
        {
            Field* fields = result->Fetch();

            uint32 entry = fields[0].GetUInt32();
            uint32 ench = fields[1].GetUInt32();
            float chance = fields[2].GetFloat();
            uint32 type = fields[3].GetUInt32();

            if (chance > 0.000001f && chance <= 100.0f)
            {
            	if (type == ENCHANTMENT_RANDOM_SUFFIX)
            		RandomSuffixItemEnch[entry].push_back(EnchStoreItem(ench, chance));
            	else if (type == ENCHANTMENT_RANDOM_PROPERTY)
            		RandomPropertyItemEnch[entry].push_back(EnchStoreItem(ench, chance));
            }

            ++count;
        } while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Item Enchantment definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
    else
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Item Enchantment definitions. DB table `item_enchantment_template` is empty.");
}

uint32 GetItemEnchantMod(int32 entry, uint32 type, uint32 SpecID)
{
    if (!entry)
        return 0;

    if (entry == -1)
        return 0;

    EnchantmentStore::const_iterator tab = type == ENCHANTMENT_RANDOM_PROPERTY ? RandomPropertyItemEnch.find(entry) : RandomSuffixItemEnch.find(entry) ;
    if (tab == (type == ENCHANTMENT_RANDOM_PROPERTY ? RandomPropertyItemEnch.end() : RandomSuffixItemEnch.end()))
    {
        sLog->outError(LOG_FILTER_SQL, "Item RandomProperty / RandomSuffix id #%u used in `item_template` but it does not have records in `item_enchantment_template` table.", entry);
        return 0;
    }

    double dRoll = rand_chance();
    float fCount = 0;

    for (EnchStoreList::const_iterator ench_iter = tab->second.begin(); ench_iter != tab->second.end(); ++ench_iter)
    {
        if(SpecID)
        {
            if(!CheckSpecProp(ench_iter->ench, type, SpecID))
                continue;
        }
        fCount += ench_iter->chance;

        if (fCount > dRoll)
            return ench_iter->ench;
    }

    //we could get here only if sum of all enchantment chances is lower than 100%
    dRoll = (irand(0, (int)floor(fCount * 100) + 1)) / 100;
    fCount = 0;

    for (EnchStoreList::const_iterator ench_iter = tab->second.begin(); ench_iter != tab->second.end(); ++ench_iter)
    {
        if(SpecID)
        {
            if(!CheckSpecProp(ench_iter->ench, type, SpecID))
                continue;
        }
        fCount += ench_iter->chance;

        if (fCount > dRoll)
            return ench_iter->ench;
    }

    return 0;
}

uint32 GenerateEnchSuffixFactor(ItemTemplate const* itemProto, uint32 level)
{
    if (!itemProto)
        return 0;

    if (!itemProto->RandomSuffix)
        return 0;

    return GetRandomPropertyPoints(level, itemProto->GetQuality(), itemProto->GetInventoryType(), itemProto->GetSubClass());
}

uint32 GetRandomPropertyPoints(uint32 itemLevel, uint32 quality, uint32 inventoryType, uint32 subClass)
{
    uint32 propIndex;

    switch (inventoryType)
    {
        case INVTYPE_HEAD:
        case INVTYPE_BODY:
        case INVTYPE_CHEST:
        case INVTYPE_LEGS:
        case INVTYPE_RANGED:
        case INVTYPE_2HWEAPON:
        case INVTYPE_ROBE:
        case INVTYPE_THROWN:
            propIndex = 0;
            break;
        case INVTYPE_RANGEDRIGHT:
            if (subClass == ITEM_SUBCLASS_WEAPON_WAND)
                propIndex = 3;
            else
                propIndex = 0;
            break;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            propIndex = 3;
            break;
        case INVTYPE_SHOULDERS:
        case INVTYPE_WAIST:
        case INVTYPE_FEET:
        case INVTYPE_HANDS:
        case INVTYPE_TRINKET:
            propIndex = 1;
            break;
        case INVTYPE_NECK:
        case INVTYPE_WRISTS:
        case INVTYPE_FINGER:
        case INVTYPE_SHIELD:
        case INVTYPE_CLOAK:
        case INVTYPE_HOLDABLE:
            propIndex = 2;
            break;
        case INVTYPE_RELIC:
            propIndex = 4;
        default:
            return 0;
    }

    RandomPropertiesPointsEntry const* randPropPointsEntry = sRandomPropertiesPointsStore.LookupEntry(itemLevel);
    if (!randPropPointsEntry)
        return 0;

    switch (quality)
    {
        case ITEM_QUALITY_UNCOMMON:
            return randPropPointsEntry->UncommonPropertiesPoints[propIndex];
        case ITEM_QUALITY_RARE:
        case ITEM_QUALITY_HEIRLOOM:
            return randPropPointsEntry->RarePropertiesPoints[propIndex];
        case ITEM_QUALITY_EPIC:
        case ITEM_QUALITY_LEGENDARY:
            return randPropPointsEntry->EpicPropertiesPoints[propIndex];
    }

    return 0;
}

bool CheckSpecProp(uint32 ench, uint32 type, uint32 SpecID)
{
    if(type == ENCHANTMENT_RANDOM_PROPERTY)
    {
        if(ItemRandomPropertiesEntry const* random_id = sItemRandomPropertiesStore.LookupEntry(ench))
        {
            for (uint32 slot = 0; slot < MAX_ITEM_ENCHANTS; ++slot)
            {
                if(!random_id->Enchantment[slot])
                    continue;
                if(SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(random_id->Enchantment[slot]))
                {
                    for (int s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                    {
                        uint32 enchant_type = pEnchant->Effect[s];
                        uint32 enchant_stat_id = pEnchant->EffectSpellID[s];
                        if(enchant_type != ITEM_ENCHANTMENT_TYPE_STAT)
                            continue;
                        if(!CheckStatsSpec(enchant_stat_id, SpecID))
                            return false;
                    }
                }
            }
        }
    }
    else
    {
        if(ItemRandomSuffixEntry const* random_id = sItemRandomSuffixStore.LookupEntry(ench))
        {
            for (uint32 slot = 0; slot < MAX_ITEM_ENCHANTS; ++slot)
            {
                if(!random_id->Enchantment[slot])
                    continue;
                if(SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(random_id->Enchantment[slot]))
                {
                    for (int s = 0; s < MAX_ITEM_ENCHANTMENT_EFFECTS; ++s)
                    {
                        uint32 enchant_type = pEnchant->Effect[s];
                        uint32 enchant_stat_id = pEnchant->EffectSpellID[s];
                        if(enchant_type != ITEM_ENCHANTMENT_TYPE_STAT)
                            continue;
                        if(!CheckStatsSpec(enchant_stat_id, SpecID))
                            return false;
                    }
                }
            }
        }
    }
    return true;
}

bool CheckStatsSpec(uint32 StatType, uint32 SpecID)
{
    for (ItemSpecEntry const* itemSpec : sItemSpecStore)
    {
        if (itemSpec->SpecID != SpecID)
            continue;

        switch (StatType)
        {
            case ITEM_MOD_AGILITY:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_AGILITY)
                    return true;
                break;
            case ITEM_MOD_STRENGTH:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_STRENGTH)
                    return true;
                break;
            case ITEM_MOD_INTELLECT:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_INTELLECT)
                    return true;
                break;
            case ITEM_MOD_SPIRIT:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_SPIRIT)
                    return true;
                break;
            case ITEM_MOD_DODGE_RATING:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_DODGE)
                    return true;
                break;
            case ITEM_MOD_PARRY_RATING:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_PARRY)
                    return true;
                break;
            case ITEM_MOD_CRIT_MELEE_RATING:
            case ITEM_MOD_CRIT_RANGED_RATING:
            case ITEM_MOD_CRIT_SPELL_RATING:
            case ITEM_MOD_CRIT_RATING:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_CRIT)
                    return true;
                break;
            case ITEM_MOD_HASTE_MELEE_RATING:
            case ITEM_MOD_HASTE_RANGED_RATING:
            case ITEM_MOD_HASTE_SPELL_RATING:
            case ITEM_MOD_HASTE_RATING:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_HASTE)
                    return true;
                break;
            case ITEM_MOD_HIT_RATING:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_HIT)
                    return true;
                break;
            case ITEM_MOD_EXTRA_ARMOR:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_BONUS_ARMOR)
                    return true;
                break;
            case ITEM_MOD_AGI_STR_INT:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_STRENGTH)
                    return true;
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_STRENGTH)
                    return true;
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_INTELLECT)
                    return true;
                break;
            case ITEM_MOD_AGI_STR:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_AGILITY)
                    return true;
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_STRENGTH)
                    return true;
                break;
            case ITEM_MOD_AGI_INT:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_AGILITY)
                    return true;
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_INTELLECT)
                    return true;
                break;
            case ITEM_MOD_STR_INT:
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_INTELLECT)
                    return true;
                if (itemSpec->SecondaryStat == ITEM_SPEC_STAT_STRENGTH)
                    return true;
                break;
        }
    }
    return false;
}

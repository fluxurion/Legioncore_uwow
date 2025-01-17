/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#include "DBCStores.h"
#include "DB2Stores.h"
#include "World.h"
#include "ItemTemplate.h"


uint32 ItemTemplate::GetArmor(uint32 itemLevel) const
{
    uint32 quality = GetQuality() != ITEM_QUALITY_HEIRLOOM ? GetQuality() : ITEM_QUALITY_RARE;
    if (quality > ITEM_QUALITY_ARTIFACT)
        return 0;

    // all items but shields
    if (GetClass() != ITEM_CLASS_ARMOR || GetSubClass() != ITEM_SUBCLASS_ARMOR_SHIELD)
    {
        ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry(itemLevel);
        ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry(itemLevel);
        if (!armorQuality || !armorTotal)
            return 0;

        uint32 inventoryType = GetInventoryType();
        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        ArmorLocationEntry const* location = sArmorLocationStore.LookupEntry(inventoryType);
        if (!location)
            return 0;

        if (GetSubClass() < ITEM_SUBCLASS_ARMOR_CLOTH || GetSubClass() > ITEM_SUBCLASS_ARMOR_PLATE)
            return 0;

        return uint32(armorQuality->QualityMod[quality] * armorTotal->Value[GetSubClass() - 1] * location->Value[GetSubClass() - 1] + 0.5f);
    }

    // shields
    ItemArmorShieldEntry const* shield = sItemArmorShieldStore.LookupEntry(itemLevel);
    if (!shield)
        return 0;

    return uint32(shield->QualityMod[quality] + 0.5f);
}

DB2Storage<ItemDamageEntry>* ItemTemplate::GetItemDamageStore() const
{
    DB2Storage<ItemDamageEntry>* store = nullptr;

    // get the right store here
    if (GetInventoryType() > INVTYPE_RANGEDRIGHT)
        return store;

    switch (GetInventoryType())
    {
        case INVTYPE_AMMO:
            store = &sItemDamageAmmoStore;
            break;
        case INVTYPE_RANGEDRIGHT:
        case INVTYPE_2HWEAPON:
            if (GetFlags2() & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
                store = &sItemDamageTwoHandCasterStore;
            else
                store = &sItemDamageTwoHandStore;
            break;
        case INVTYPE_RANGED:
        case INVTYPE_THROWN:
        //@TODO:Legion check how this calc now...
            //switch (GetSubClass())
            //{
            //    case ITEM_SUBCLASS_WEAPON_WAND:
            //    case ITEM_SUBCLASS_WEAPON_BOW:
            //    case ITEM_SUBCLASS_WEAPON_GUN:
            //    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
            //    case ITEM_SUBCLASS_WEAPON_THROWN:
            //        break;
            //    default:
            //        return store;
            //}
            //break;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            if (GetFlags2() & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
                store = &sItemDamageOneHandCasterStore;
            else
                store = &sItemDamageOneHandStore;
            break;
        default:
            return store;
    }

    ASSERT(store);
    return store;
}

void ItemTemplate::GetDamage(uint32 itemLevel, float& minDamage, float& maxDamage) const
{
    minDamage = maxDamage = 0.0f;
    uint32 quality = GetQuality() != ITEM_QUALITY_HEIRLOOM ? GetQuality() : ITEM_QUALITY_RARE;
    if (GetClass() != ITEM_CLASS_WEAPON || quality > ITEM_QUALITY_ARTIFACT)
        return;

    DB2Storage<ItemDamageEntry>* store = GetItemDamageStore();
    if (!store)
        return;

    ItemDamageEntry const* damageInfo = store->LookupEntry(itemLevel);
    if (!damageInfo)
        return;

    float dps = damageInfo->Quality[quality];
    float avgDamage = dps * GetDelay() * 0.001f;
    minDamage = (GetStatScalingFactor() * -0.5f + 1.0f) * avgDamage;
    maxDamage = floor(float(avgDamage * (GetStatScalingFactor() * 0.5f + 1.0f) + 0.5f));
}

uint32 ItemTemplate::GetDPS(uint32 itemLevel) const
{
    DB2Storage<ItemDamageEntry>* store = GetItemDamageStore();
    if (!store)
        return 0;

    ItemDamageEntry const* damageInfo = store->LookupEntry(itemLevel);
    if (!damageInfo)
        return 0;

    return damageInfo->Quality[GetQuality() != ITEM_QUALITY_HEIRLOOM ? GetQuality() : ITEM_QUALITY_RARE];
}

bool ItemTemplate::CanWinForPlayer(Player const* player) const
{
    std::unordered_set<uint32> const& specs = Specializations[player->getLevel() > 40];
    if (specs.empty())
        return true;

    uint32 spec = player->GetSpecializationId();
    if (!spec)
        spec = player->GetDefaultSpecId();

    if (!spec)
        return false;

    auto itr = specs.find(spec);
    return itr != specs.end();
}

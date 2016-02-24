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

#ifndef TRINITY_DBCSTRUCTURE_H
#define TRINITY_DBCSTRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Path.h"
#include "Util.h"

#pragma pack(push, 1)

struct GtBarberShopCostBaseEntry
{
    //uint32 level;
    float   cost;
};

struct GtCombatRatingsEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToMeleeCritBaseEntry
{
    //uint32 level;
    float    base;
};

struct GtChanceToMeleeCritEntry
{
    //uint32 level;
    float    ratio;
};

struct GtChanceToSpellCritBaseEntry
{
    //uint32 level;
    float    base;
};

struct GtChanceToSpellCritEntry
{
    //uint32 level;
    float    ratio;
};

struct GtNpcTotalHpEntry
{
    //uint32 level;
    float    HP;
};

struct GtNpcDamageByClassEntry
{
    //uint32 level;
    float    Value;
};

struct GtArmorMitigationByLvlEntry
{
    //uint32 level;
    float    Armor;
};

struct GtOCTLevelExperienceEntry
{
    //uint32 level;
    float    Data;
};

struct GtOCTRegenHPEntry
{
    //uint32 level;
    float    ratio;
};

struct GtOCTHpPerStaminaEntry
{
    //uint32 level;
    float    ratio;
};

struct GtRegenHPPerSptEntry
{
    //uint32 level;
    float    ratio;
};

struct GtRegenMPPerSptEntry
{
    //uint32 level;
    float    ratio;
};

struct GtSpellScalingEntry
{
    //uint32 level;
    float value;
};

struct GtOCTBaseHPByClassEntry
{
    //uint32 level;
    float ratio;
};

struct GtOCTBaseMPByClassEntry
{
    //uint32 level;
    float ratio;
};

struct GtBattlePetTypeDamageModEntry
{
    //uint32 level;
    float value;
};

struct GtItemSocketCostPerLevelEntry
{
    //uint32 level;
    float    ratio;
};

struct GtNPCManaCostScalerEntry
{
    //uint32 level;
    float    ratio;
};

#pragma pack(pop)

float GetCurrencyPrecision(uint32 currencyId);

#endif

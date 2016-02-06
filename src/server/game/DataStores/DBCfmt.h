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

#ifndef TRINITY_DBCSFRM_H
#define TRINITY_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - bool, p - field present in sql dbc, a - field absent in sql dbc

char const BannedAddOnsfmt[] = "nxxxxxxxxxx";
char const BattlemasterListEntryfmt[] = "niiiiiiiiiiiiiiiiixsiiiiiiiixxx";
char const ChrClassesEntryfmt[] = "nixsxxxixiiiiixxxxx";
char const CreatureModelDatafmt[] = "niixxxxxxxxxxxxffxxxxxxxxxxxxxxxxxxx";
char const FactionEntryfmt[] = "niiiiiiiiiiiiiiiiiiffixsxxix";
char const FactionTemplateEntryfmt[] = "niiiiiiiiiiiii";
char const GameTablefmt[] = "xf";
char const GlyphSlotfmt[] = "nii";
char const GtBarberShopCostBasefmt[] = "xf";
char const GtBattlePetTypeDamageModfmt[]= "nf";
char const GtChanceToMeleeCritBasefmt[] = "xf";
char const GtChanceToMeleeCritfmt[] = "xf";
char const GtChanceToSpellCritBasefmt[] = "xf";
char const GtChanceToSpellCritfmt[] = "xf";
char const GtCombatRatingsfmt[] = "xf";
char const GtItemSocketCostPerLevelfmt[] = "xf";
char const GtOCTBaseHPByClassfmt[] = "df";
char const GtOCTBaseMPByClassfmt[] = "df";
char const GtOCTHpPerStaminafmt[] = "df";
char const GtOCTLevelExperiencefmt[] = "xf";
char const GtOCTRegenHPfmt[] = "f";
char const GtRegenMPPerSptfmt[] = "xf";
char const GtSpellScalingfmt[] = "df";
char const LFGDungeonEntryfmt[]= "nsiiiiiiiiiixixixiiiiiiiiiixx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const MapDifficultyEntryfmt[] = "diisiiii";
char const SpellItemEnchantmentfmt[] = "niiiiiiiiiixiiiiiiiiiiifff";
char const TalentEntryfmt[] = "niiiixxxiis";
char const TeamContributionPointsfmt[] = "df";
char const VehicleEntryfmt[] = "niiffffiiiiiiiifffffffffffffffxxxxfifiiii";
char const WMOAreaTableEntryfmt[] = "niiixxxxxiisxxx";
char const WorldMapAreaEntryfmt[] = "xinxffffixxxii";
char const WorldSafeLocsEntryfmt[] = "niffffx";

const std::string CustomMapDifficultyEntryfmt= "pppappaa";
const std::string CustomMapDifficultyEntryIndex = "m_id";

#endif

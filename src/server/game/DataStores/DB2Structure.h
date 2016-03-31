/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Path.h"

#define MAX_BROADCAST_TEXT_EMOTES 3
#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 16
#define MAX_HOLIDAY_FLAGS 10
#define MAX_ITEM_EXT_COST_ITEMS 5
#define MAX_ITEM_EXT_COST_CURRENCIES 5
#define MAX_ITEM_ENCHANTS 5
#define MAX_EFFECT_PROPERTIES 6
#define KEYCHAIN_SIZE 32
#define MAX_OVERRIDE_SPELL 10
#define GO_DBC_DATA_COUNT 8
#define MAX_ITEM_UPDGRADES 5
#define MAX_SPELL_TOTEMS 2
#define MAX_SPELL_REAGENTS 8
#define MAX_OUTFIT_ITEMS 24
#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4
#define MaxAttributes 14
#define MAX_MASTERY_SPELLS 2
#define MAX_ITEM_SET_ITEMS 17
#define MAX_LOCK_CASE 8
#define MAX_SHAPESHIFT_SPELLS 8
#define MAX_FACTION_RELATIONS 4
#define MAX_ITEM_ENCHANTMENT_EFFECTS 3
#define MAX_VEHICLE_SEATS 8

#pragma pack(push, 1)

struct AreaTableEntry
{
    uint32      ID;
    uint32      Flags[2];                                   // 0 - 1
    LocalizedString* ZoneName;                              // 2
    float       AmbientMultiplier;                          // 3
    LocalizedString* AreaName;                              // 4
    uint16      MapID;                                      // 5
    uint16      ParentAreaID;                               // 6
    int16       AreaBit;                                    // 7
    uint16      AmbienceID;                                 // 8
    uint16      ZoneMusic;                                  // 9
    uint16      IntroSound;                                 // 10
    uint16      LiquidTypeID[4];                            // 11 - 14
    uint16      UWIntroMusic;                               // 15
    uint16      UWZoneMusic;                                // 16
    uint16      UWAmbience;                                 // 17
    int8        WorldPvPID;                                 // 18
    int8        WindSettingsID;                             // 19
    uint8       SoundProviderPref;                          // 20
    uint8       SoundProviderPrefUnderwater;                // 21
    int8        ExplorationLevel;                           // 22
    uint8       FactionGroupMask;                           // 23
    uint8       MountFlags;                                 // 24
    uint8       WildBattlePetLevelMin;                      // 25
    uint8       WildBattlePetLevelMax;                      // 26

    bool IsSanctuary() const
    {
        if (MapID == 609)
            return true;

        return (Flags[0] & AREA_FLAG_SANCTUARY) != 0;
    }
};

struct AreaTriggerEntry
{
    uint32      ID;                                         // 0
    DBCPosition3D Pos;                                      // 1 - 3
    float       Radius;                                     // 4
    float       BoxLength;                                  // 5
    float       BoxWidth;                                   // 6
    float       BoxHeight;                                  // 7
    float       BoxYaw;                                     // 8
    uint16      MapID;                                      // 9
    uint16      PhaseID;                                    // 10
    uint16      PhaseGroupID;                               // 11
    uint16      ShapeID;                                    // 12
    uint16      AreaTriggerActionSetID;                     // 13
    uint8       PhaseUseFlags;                              // 14
    uint8       ShapeType;                                  // 16
    uint8       Flags;                                      // 
};

struct AchievementEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Name;                                  // 1
    LocalizedString* Description;                           // 2
    uint32      Flags;                                      // 3
    LocalizedString* Reward;                                // 4
    int16       MapID;                                      // 5
    uint16      ParentAchievement;                          // 6    
    uint16      CategoryID;                                 // 7
    uint16      OrderInCategory;                            // 8
    uint16      IconID;                                     // 9
    uint16      RefAchievement;                             // 10
    uint16      CriteriaTree;                               // 11
    int8        FactionFlag;                                // 12
    uint8       RewardPoints;                               // 13
    uint8       Amount;                                     // 14
};

struct AreaGroupMemberEntry
{
    uint32      ID;
    uint16      AreaGroupID;                                // 0
    uint16      AreaID;                                     // 1
};

struct AuctionHouseEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    uint16      FactionID;                                  // 1
    uint8       DepositRate;                                // 2
    uint8       ConsignmentRate;                            // 3
};

struct ArmorLocationEntry
{
    uint32      ID;
    float       Value[5];                                           // 0 - 4 multiplier for armor types: cloth...plate
};

struct BannedAddonsEntry
{
    uint32      ID;                                         // 0
    uint32      NameMD5[4];                                 // 1
    uint32      VersionMD5[4];                              // 2
    uint32      LastModified;                               // 3
    uint8       Flags;                                      // 4
};

struct BattlemasterListEntry
{
    uint32      ID;
    LocalizedString* NameLang;
    uint32      IconFileDataID;
    LocalizedString* GametypeLang;
    int16       MapID[16];
    uint16      HolidayWorldState;
    uint16      CritreriaID;
    uint8       InstanceType;
    uint8       GroupsAllowed;
    uint8       MaxGroupSize;
    uint8       Minlevel;
    uint8       Maxlevel;
    uint8       RatedPlayers;
    uint8       MinPlayers;
    uint8       MaxPlayers;
    uint8       Flags;
};

struct ChrClassesEntry
{
    uint32      ID;                                         // 0
    LocalizedString* PetNameToken;                          // 1
    LocalizedString* Name;                                  // 2
    LocalizedString* NameFemale;                            // 3
    LocalizedString* NameMale;                              // 4
    LocalizedString* Filename;                              // 5
    uint32      CreateScreenFileDataID;                     // 6
    uint32      SelectScreenFileDataID;                     // 7
    uint32      IconFileDataID;                             // 8
    uint16      Flags;                                      // 9
    uint16      CinematicSequenceID;                        // 10
    uint16      DefaultSpec;                                // 11
    uint8       PowerType;                                  // 12
    uint8       SpellClassSet;                              // 13
    uint8       AttackPowerPerStrength;                     // 14
    uint8       AttackPowerPerAgility;                      // 15
    uint8       RangedAttackPowerPerAgility;                // 16
    uint8       LowResScreenFileDataID;                     // 17
    uint8       LegionUnk;                                  // 18
};

struct CharSectionsEntry
{
    uint32      ID;
    uint32      FileDataID[3];
    uint16      Flags;
    uint8       Race;
    uint8       Gender;
    uint8       GenType;
    uint8       Type;
    uint8       Color;
};

struct CreatureModelDataEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FileDataID;                                 // 2
    float       ModelScale;                                 // 3
    uint32      FootprintTextureID;                         // 4
    float       FootprintTextureLength;                     // 5
    float       FootprintTextureWidth;                      // 6
    float       CollisionWidth;                             // 7
    float       CollisionHeight;                            // 8
    float       MountHeight;                                // 9
    float       GeoBox[6];                                  // 10 - 15
    float       WorldEffectScale;                           // 16
    float       AttachedEffectScale;                        // 17
    float       MissileCollisionRadius;                     // 18
    float       MissileCollisionPush;                       // 19
    float       MissileCollisionRaise;                      // 20
    float       OverrideLootEffectScale;                    // 21
    float       OverrideNameScale;                          // 22
    float       OverrideSelectionRadius;                    // 23
    float       TamedPetBaseScale;                          // 24
    float       HoverHeight;                                // 25
    uint16      SoundID;                                    // 26
    uint16      CreatureGeosetDataID;                       // 27
    uint8       SizeClass;                                  // 28
    uint8       UnkLegion;                                  // 29
    uint8       FootstepShakeSize;                          // 30
    uint8       UnkLegion2;                                 // 31
    uint8       UnkLegion3;                                 // 32
    uint8       UnkLegion4;                                 // 33
    uint8       UnkLegion5;                                 // 34
    uint8       DeathThudShakeSize;                         // 35
};

struct ConversationLineEntry
{
    uint32      ID;
    uint32      tesxID;
    uint16      duration;
    int16       unk;
    uint16      nextID;
    uint8       unk1;
    uint8       unk2;
    uint8       unk3;
    uint8       unk4;
};

struct FactionEntry
{
    uint32      ID;                                         // 0
    uint32      BaseRepRaceMask[4];                         // 1 - 4
    int32       BaseRepValue[4];                            // 5 - 8
    float       SpilloverRate[2];                           // 9 - 10
    LocalizedString* Name;                                  // 11
    LocalizedString* Desc;                                  // 12
    int16       ReputationListID;                           // 13
    uint16      BaseRepClassMask[4];                        // 14 - 17
    uint16      ReputationFlags[4];                         // 18 - 21
    uint16      Team;                                       // 22
    uint8       SpilloverRank[2];                           // 23 - 24
    uint8       Expansion;                                  // 25
    uint8       Flags;                                      // 26
    uint8       FriendshipRepID;                            // 27

    bool CanHaveReputation() const {  return ReputationListID >= 0; }
    bool CanBeLfgBonus() const { return Flags & 0x1; }
};

struct FactionTemplateEntry
{
    uint32      ID;                                         // 0
    uint16      Faction;                                    // 1
    uint16      Flags;                                      // 2
    uint16      Enemies[MAX_FACTION_RELATIONS];             // 3 - 6
    uint16      Friends[MAX_FACTION_RELATIONS];             // 7 - 10
    uint8       Mask;                                       // 11
    uint8       FriendMask;                                 // 12
    uint8       EnemyMask;                                  // 13

    bool IsFriendlyTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return true;

        if (entry.Faction)
        {
            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Enemies[i] == entry.Faction)
                    return false;

            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Friends[i] == entry.Faction)
                    return true;
        }

        return (FriendMask & entry.Mask) || (Mask & entry.FriendMask);
    }

    bool IsHostileTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return false;

        if (entry.Faction)
        {
            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Enemies[i] == entry.Faction)
                    return true;

            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Friends[i] == entry.Faction)
                    return false;
        }

        return (EnemyMask & entry.Mask) != 0;
    }

    bool IsHostileToPlayers() const { return (EnemyMask & FACTION_MASK_PLAYER) != 0; }

    bool IsNeutralToAll() const
    {
        for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
            if (Enemies[i] != 0)
                return false;

        return EnemyMask == 0 && FriendMask == 0;
    }

    bool IsContestedGuardFaction() const { return (Flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

struct GlyphSlotEntry
{
    uint32      ID;
    uint32      TypeFlags;
    uint32      Order;
};

struct LFGDungeonEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Name;                                  // 1
    uint32      Flags;                                      // 2
    LocalizedString* TextureFilename;                       // 3
    LocalizedString* Description;                           // 4
    uint16      MaxLevel;                                   // 5
    uint16      TargetLevelMax;                             // 6
    int16       MapID;                                      // 7
    uint16      RandomID;                                   // 8
    uint16      ScenarioID;                                 // 9
    uint16      LastBossJournalEncounterID;                 // 10
    uint16      BonusReputationAmount;                      // 11
    uint16      TimeWalkerItemLvl;                          // 12
    uint8       MinLevel;                                   // 13
    uint8       TargetLevel;                                // 14
    uint8       TargetLevelMin;                             // 15
    uint8       DifficultyID;                               // 16
    uint8       Type;                                       // 17
    int8        Faction;                                    // 18
    uint8       Expansion;                                  // 19
    uint8       OrderIndex;                                 // 20
    uint8       GroupID;                                    // 21
    uint8       CountTank;                                  // 22
    uint8       CountHealer;                                // 23
    uint8       CountDamage;                                // 24
    uint8       MinCountTank;                               // 25
    uint8       MinCountHealer;                             // 26
    uint8       MinCountDamage;                             // 27
    uint8       SubType;                                    // 28
    uint8       UnkLegion;                                  // 29

    // Helpers
    uint32 Entry() const { return ID + (Type << 24); }
    bool IsScenario() const { return SubType == LFG_SUBTYPE_SCENARIO; }
    bool IsChallenge() const { return DifficultyID == DIFFICULTY_CHALLENGE; }
    bool IsRaidFinder() const { return DifficultyID == DIFFICULTY_LFR; }
    bool IsFlex() const { return DifficultyID == FLEXIBLE_DIFFICULTY; }
    uint32 GetMinGroupSize() const { return MinCountTank + MinCountHealer + MinCountDamage; }
    uint32 GetMaxGroupSize() const { return CountTank + CountHealer + CountDamage; }
    bool IsValid() const
    {
        switch (ID)
        {
            case 540:           // Temple of Kotmogu
            case 541:           // Test Scenario PS
            case 645:           // Greenstone Village
            case 697:           // Scenario: Prototype
            case 699:
                return false;
            default:
                break;
        }

        return true;
    }

    LfgType GetInternalType() const
    {
        switch (SubType)
        {
            case LFG_SUBTYPE_DUNGEON:
                return LFG_TYPE_DUNGEON;
            case LFG_SUBTYPE_RAID:
            case LFG_SUBTYPE_FLEX:
                return LFG_TYPE_RAID;
            case LFG_SUBTYPE_SCENARIO:
                return LFG_TYPE_SCENARIO;
            default:
                break;
        }

        return LFG_TYPE_DUNGEON;
    }

    bool CanBeRewarded() const {  return Type == LFG_TYPE_RANDOM || IsRaidFinder() || IsChallenge() || Flags & LFG_FLAG_SEASONAL; }

    bool FitsTeam(uint32 team) const
    {
        if (Faction == -1)
            return true;
        else if (Faction == 0)
            return team == HORDE;
        else
            return team == ALLIANCE;
    }
};

struct LiquidTypeEntry
{
    uint32      ID;
    LocalizedString* Name;
    uint32      SpellId;
    float       unk_[5];
    LocalizedString* Texture[6];
    uint32      unk2_;
    uint32      unk3_;
    float       unk4_[18];
    uint32      unk5_;
    uint32      unk6_;
    uint32      unk7_;
    uint32      unk8_;
    uint16      unk9_;
    uint16      unk10_;
    uint8       Type;
    uint8       unk11_[10];
};

struct MapDifficultyEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Message;                               // 1
    uint32      RaidDuration;                               // 2
    uint16      MapID;                                      // 3
    uint8       DifficultyID;                               // 4
    uint8       MaxPlayers;                                 // 5
    uint8       LockID;                                     // 6
    uint8       ItemBonusTreeModID;                         // 7
};

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                             // 0
    uint32      EffectSpellID[MAX_ITEM_ENCHANTMENT_EFFECTS];    // 1 - 3
    LocalizedString* Name;                                      // 4
    float       EffectScalingPoints[MAX_ITEM_ENCHANTMENT_EFFECTS];// 5 - 7
    uint32      UnkLegion;                                      // 8
    int16       EffectPointsMin[MAX_ITEM_ENCHANTMENT_EFFECTS];  // 9 - 11
    uint16      ItemVisual;                                     // 12
    uint16      Flags;                                          // 13
    uint16      RequiredSkillID;                                // 14
    uint16      RequiredSkillRank;                              // 15
    uint16      ItemLevel;                                      // 16
    uint8       Charges;                                        // 17
    uint8       Effect[MAX_ITEM_ENCHANTMENT_EFFECTS];           // 18 - 20
    uint8       UnkLegion2;                                     // 21
    uint8       MinLevel;                                       // 22
    uint8       MaxLevel;                                       // 23
    int8        ScalingClass;                                   // 24
    int8        ScalingClassRestricted;                         // 25
    uint8       UnkLegion3;                                     // 26
    uint8       UnkLegion4;                                     // 27
};

struct TalentEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      OverridesSpellID;                           // 2
    LocalizedString* Description;                           // 3
    uint16      SpecID;                                     // 4
    uint8       Row;                                        // 5
    uint8       Column;                                     // 6
    uint8       Flags;                                      // 7
    uint8       CategoryMask[2];                            // 8 - 9
    uint8       ClassID;                                    // 10
};

struct VehicleEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    float       TurnSpeed;                                  // 2
    float       PitchSpeed;                                 // 3
    float       PitchMin;                                   // 4
    float       PitchMax;                                   // 5
    float       MouseLookOffsetPitch;                       // 6
    float       CameraFadeDistScalarMin;                    // 7
    float       CameraFadeDistScalarMax;                    // 8
    float       CameraPitchOffset;                          // 9
    float       FacingLimitRight;                           // 10
    float       FacingLimitLeft;                            // 11
    float       MsslTrgtTurnLingering;                      // 12
    float       MsslTrgtPitchLingering;                     // 13
    float       MsslTrgtMouseLingering;                     // 14
    float       MsslTrgtEndOpacity;                         // 15
    float       MsslTrgtArcSpeed;                           // 16
    float       MsslTrgtArcRepeat;                          // 17
    float       MsslTrgtArcWidth;                           // 18
    float       MsslTrgtImpactRadius[2];                    // 19 - 20
    LocalizedString* MsslTrgtArcTexture;                    // 21
    LocalizedString* MsslTrgtImpactTexture;                 // 22
    LocalizedString* MsslTrgtImpactModel[2];                // 23 - 24
    float       CameraYawOffset;                            // 25
    float       MsslTrgtImpactTexRadius;                    // 26
    uint16      SeatID[MAX_VEHICLE_SEATS];                  // 27 - 34
    uint16      PowerDisplayID[3];                          // 35 - 37
    uint16      FlagsB;                                     // 38
    uint8       UILocomotionType;                           // 39
    uint8       VehicleUIIndicatorID;                       // 40
};

struct WMOAreaTableEntry
{
    uint32      ID;                                         // 0
    int32       WMOGroupID;                                 // 1
    LocalizedString* AreaName;                              // 2
    uint16      WMOID;                                      // 3
    uint16      AmbienceID;                                 // 4
    uint16      ZoneMusic;                                  // 5
    uint16      IntroSound;                                 // 6
    uint16      AreaTableID;                                // 7
    uint16      UWIntroSound;                               // 8
    uint16      UWAmbience;                                 // 9
    uint8       NameSet;                                    // 10
    uint8       SoundProviderPref;                          // 11
    uint8       SoundProviderPrefUnderwater;                // 12
    uint8       Flags;                                      // 13
    uint8       UWZoneMusic;                                // 14
};

struct WorldMapAreaEntry
{
    uint32      ID;                                         // 0
    LocalizedString* AreaName;                              // 1
    float       LocLeft;                                    // 2
    float       LocRight;                                   // 3
    float       LocTop;                                     // 4
    float       LocBottom;                                  // 5
    uint16      MapID;                                      // 6
    uint16      AreaID;                                     // 7
    int16       DisplayMapID;                               // 8
    int16       DefaultDungeonFloor;                        // 9
    uint16      ParentWorldMapID;                           // 10
    uint16      Flags;                                      // 11
    uint8       LevelRangeMin;                              // 12
    uint8       LevelRangeMax;                              // 13
    uint8       Unk;                                        // 14
    uint8       Unk2;                                       // 15
};

struct WorldSafeLocsEntry
{
    uint32          ID;                                     // 0
    DBCPosition4D   Loc;                                    // 1 - 4
    LocalizedString* AreaName;                              // 5
    uint32          MapID;                                  // 6
};

struct BankBagSlotPricesEntry
{
    uint32      ID;
    uint32      Price;
};

struct BarberShopStyleEntry
{
    uint32      ID;
    LocalizedString* Name;                                          // 0
    LocalizedString* Description;                                   // 1
    float       CostMultiplier;                                     // 2
    uint8       Type;                                               // 3
    uint8       Race;                                               // 4
    uint8       Gender;                                             // 5
    uint8       HairID;                                             // 6
};

struct BroadcastTextEntry
{
    uint32      ID;
    LocalizedString* MaleText;
    LocalizedString* FemaleText;
    uint16      EmoteID[MAX_BROADCAST_TEXT_EMOTES];
    uint16      EmoteDelay[MAX_BROADCAST_TEXT_EMOTES];
    uint16      SoundID;
    uint16      UnkEmoteID;
    uint8       Language;
    uint8       Type;
};

struct CharTitlesEntry
{
    uint32      ID;                                         // 0
    LocalizedString* NameMale;                              // 1
    LocalizedString* NameFemale;                            // 2
    uint16      ConditionID;                                // 3
    int16       MaskID;                                     // 4
    int8        Flags;                                      // 5
};

struct CurvePointEntry
{
    uint32      ID;                                         // 0
    DBCPosition2D Pos;                                      // 1 - 2
    uint16      CurveID;                                    // 3
    uint8       Index;                                      // 4
};

struct CreatureFamilyEntry
{
    uint32      ID;                                         // 0
    float       MinScale;                                   // 1
    float       MaxScale;                                   // 2
    LocalizedString* Name;                                  // 3
    LocalizedString* IconFile;                              // 4
    uint16      SkillLine[2];                               // 5 - 6
    uint16      PetFoodMask;                                // 7
    uint8       MaxScaleLevel;                              // 8
    uint8       MinScaleLevel;                              // 9
    uint8       PetTalentType;                              // 10
    uint8       CategoryEnumID;                             // 11
};

struct DifficultyEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Name;                                  // 1
    uint8       FallbackDifficultyID;                       // 2
    uint8       InstanceType;                               // 3
    uint8       MinPlayers;                                 // 4
    uint8       MaxPlayers;                                 // 5
    int8        OldEnumValue;                               // 6
    uint8       Flags;                                      // 7
    uint8       ToggleDifficultyID;                         // 8
    uint8       GroupSizeHealthCurveID;                     // 9
    uint8       GroupSizeDmgCurveID;                        // 10
    uint8       GroupSizeSpellPointsCurveID;                // 11
    uint8       ItemBonusTreeModID;                         // 12
    uint8       OrderIndex;                                 // 13
};

struct DungeonEncounterEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Name;                                  // 1
    uint32      CreatureDisplayID;                          // 2
    uint16      MapID;                                      // 3
    uint16      OrderID;                                    // 4
    uint16      SpellIconID;                                // 5
    uint8       DifficultyID;                               // 6
    uint8       EncounterID;                                // 7
    uint8       Flags;                                      // 8
};

struct EmotesTextEntry
{
    uint32      ID;
    LocalizedString* Name;
    uint16      TextID;
};

struct PhaseEntry
{
    uint32      ID;                                     // 0
    uint16      Flags;                                  // 1
};

struct MapEntry
{
    uint32      ID;                                     // 0
    LocalizedString* Directory;                         // 1
    uint32      Flags[2];                               // 2 - 3
    float       MinimapIconScale;                       // 4
    DBCPosition2D CorpsePos;                            // 5 - 6
    uint32      RaidOffset;                             // 7
    LocalizedString* MapName;                           // 8
    LocalizedString* MapDescription0;                   // 9
    LocalizedString* MapDescription1;                   // 10
    uint16      AreaTableID;                            // 11
    uint16      LoadingScreenID;                        // 12
    int16       CorpseMapID;                            // 13
    uint16      TimeOfDayOverride;                      // 14
    int16       ParentMapID;                            // 15
    int16       CosmeticParentMapID;                    // 16
    uint8       InstanceType;                           // 17
    uint8       MapType;                                // 18
    uint8       ExpansionID;                            // 19
    uint8       MaxPlayers;                             // 20
    uint8       TimeOffset;                             // 21

    bool IsDungeon() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID || InstanceType == MAP_SCENARIO; }
    bool IsNonRaidDungeon() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_SCENARIO; }
    bool Instanceable() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID || InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA || InstanceType == MAP_SCENARIO; }
    bool IsRaid() const { return InstanceType == MAP_RAID; }
    bool IsBattleground() const { return InstanceType == MAP_BATTLEGROUND; }
    bool IsBattleArena() const { return InstanceType == MAP_ARENA; }
    bool IsBattlegroundOrArena() const { return InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA; }
    bool IsWorldMap() const { return InstanceType == MAP_COMMON; }
    bool IsScenario() const { return InstanceType == MAP_SCENARIO; }

    bool GetEntrancePos(int32 &mapid, float &x, float &y) const
    {
        if (CorpseMapID < 0)
            return false;

        mapid = CorpseMapID;
        x = CorpsePos.X;
        y = CorpsePos.Y;
        return true;
    }

    bool IsContinent() const
    {
        return ID == 0 || ID == 1 || ID == 530 || ID == 571 || ID == 870 || ID == 1116;
    }

    bool IsDynamicDifficultyMap() const { return (Flags[0] & MAP_FLAG_CAN_CHANGE_DIFFICULTY) != 0; }
    bool IsGarrison() const { return (Flags[0] & MAP_FLAG_GARRISON) != 0; }

    bool IsDifficultyModeSupported(uint32 difficulty) const;
};

struct MinorTalentEntry
{
    uint32      ID;                                         // 0
    uint32      SpecID;                                     // 1
    uint32      SpellID;                                    // 2
    uint32      OrderIndex;                                 // 3
};

struct CriteriaEntry
{
    uint32      ID;
    int32       Asset;                                      // 0
    uint32      StartAsset;                                 // 1
    uint32      FailAsset;                                  // 2
    uint16      StartTimer;                                 // 3
    uint16      ModifyTree;                                 // 4
    int16       EligibilityWorldStateID;                    // 5
    uint8       Type;                                       // 6
    uint8       StartEvent;                                 // 7
    uint8       FailEvent;                                  // 8
    uint8       Flags;                                      // 9
    int8        EligibilityWorldStateValue;                 // 10
};

struct CriteriaTreeEntry
{
    uint32      ID;
    uint32      Amount;                                     // 0
    LocalizedString* Description;                           // 1
    uint16      CriteriaID;                                 // 2
    uint16      Parent;                                     // 3
    int16       Operator;                                   // 4
    int16       OrderIndex;                                 // 5
    int8        Flags;                                      // 6
};

struct CharacterLoadoutItemEntry
{
    uint32      ID;
    uint32      ItemID;
    uint16      LoadOutID;
};

struct CharacterLoadoutEntry
{
    uint32      ID;
    uint32      Racemask;
    uint8       ClassID;
    uint8       Purpose;
};

struct CharStartOutfitEntry
{
    uint32      ID;
    int32       ItemID[MAX_OUTFIT_ITEMS];                   // 0-23
    int32       PetDisplayID;                               // 24
    uint8       RaceID;                                     // 25
    uint8       ClassID;                                    // 26
    uint8       GenderID;                                   // 27
    uint8       OutfitID;                                   // 28
    uint8       PetFamilyID;                                // 29
};

struct ChrClassesXPowerTypesEntry
{
    uint32      ID;
    uint8       ClassID;                                    // 0
    uint8       PowerType;                                  // 1
};

struct ChrRacesEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    LocalizedString* ClientPrefix;                          // 2
    LocalizedString* ClientFileString;                      // 3
    LocalizedString* Name;                                  // 4
    LocalizedString* NameFemale;                            // 5
    LocalizedString* NameMale;                              // 6
    LocalizedString* FacialHairCustomization[2];            // 7
    LocalizedString* HairCustomization;                     // 8
    uint32      CreateScreenFileDataID;                     // 9
    uint32      SelectScreenFileDataID;                     // 10
    float       MaleCustomizeOffset[3];                     // 11-13
    float       FemaleCustomizeOffset[3];                   // 14-16
    uint32      LowResScreenFileDataID;                     // 17
    uint16      FactionID;                                  // 18
    uint16      ExplorationSoundID;                         // 19
    uint16      MaleDisplayID;                              // 20
    uint16      FemaleDisplayID;                            // 21
    uint16      ResSicknessSpellID;                         // 22
    uint16      SplashSoundID;                              // 23
    uint16      CinematicSequenceID;                        // 24
    uint16      UAMaleCreatureSoundDataID;                  // 25
    uint16      UAFemaleCreatureSoundDataID;                // 26
    uint16      HighResMaleDisplayID;                       // 27
    uint16      HighResFemaleDisplayID;                     // 28
    uint16      Unk;                                        // 29
    uint8       BaseLanguage;                               // 30
    uint8       CreatureType;                               // 31
    uint8       TeamID;                                     // 32
    uint8       RaceRelated;                                // 33
    uint8       UnalteredVisualRaceID;                      // 34
    uint8       CharComponentTexLayoutHiResID;              // 35
    uint8       DefaultClassID;                             // 36
    uint8       NeutralRaceID;                              // 37
    uint8       CharComponentTextureLayoutID;               // 38
    uint8       UnkLegion;                                  // 39
};

struct CreatureDisplayInfoEntry
{
    uint32      ID;                                         // 0
    uint32      ExtendedDisplayInfoID;                      // 1
    float       CreatureModelScale;                         // 2
    float       Unknown620;                                 // 3
    uint32      Unknown701[3];                              // 4 - 6
    LocalizedString* PortraitTextureName;                   // 7
    uint32      PortraitCreatureDisplayInfoID;              // 8
    uint32      Unknown0;                                   // 9
    int32       Unknown1;                                   // 10
    float       UnkLegion2;                                 // 11
    uint16      ModelID;                                    // 12
    uint16      SoundID;                                    // 13
    uint16      NPCSoundID;                                 // 14
    uint16      Unknown2;                                   // 15
    uint16      Unknown3;                                   // 16
    uint16      Unknown5;                                   // 17
    uint8       CreatureModelAlpha;                         // 18
    int8        SizeClass;                                  // 19
    uint8       Unknown6;                                   // 20
    uint8       Unknown7;                                   // 21
    int8        Gender;                                     // 22
    int8        Unknown8;                                   // 23
};

struct CreatureTypeEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    uint8       Flags;                                      // 1
};

struct DestructibleModelDataEntry
{
    uint32      ID;
    uint16      StateDamagedDisplayID;                      // 0
    uint16      StateDestroyedDisplayID;                    // 1
    uint16      StateRebuildingDisplayID;                   // 2
    uint16      StateSmokeDisplayID;                        // 3
    uint16      HealEffectSpeed;                            // 4
    uint8       StateDamagedImpactEffectDoodadSet;          // 5
    uint8       StateDamagedAmbientDoodadSet;               // 6
    uint8       StateDamagedNameSet;                        // 7
    uint8       StateDestroyedDestructionDoodadSet;         // 8
    uint8       StateDestroyedImpactEffectDoodadSet;        // 9
    uint8       StateDestroyedAmbientDoodadSet;             // 10
    uint8       StateDestroyedNameSet;                      // 11
    uint8       StateRebuildingDestructionDoodadSet;        // 13
    uint8       StateRebuildingImpactEffectDoodadSet;       // 14
    uint8       StateRebuildingAmbientDoodadSet;            // 15
    uint8       StateRebuildingNameSet;                     // 16
    uint8       StateSmokeInitDoodadSet;                    // 17
    uint8       StateSmokeAmbientDoodadSet;                 // 18
    uint8       StateSmokeNameSet;                          // 19
    uint8       EjectDirection;                             // 20
    uint8       DoNotHighlight;                             // 21
    uint8       HealEffect;                                 // 22
};

struct DurabilityCostsEntry
{
    uint32      ID;
    uint16      WeaponSubstructCost[21];                    // 0-20
    uint16      ArmorSubstructCost[8];                      // 21-29
};

struct DurabilityQualityEntry
{
    uint32      ID;
    float       Data;                                       // 0
};

struct GameObjectDisplayInfoEntry
{
    uint32      ID;
    uint32      FileDataID;                             // 0
    DBCPosition3D GeoBoxMin;                            // 1-3
    DBCPosition3D GeoBoxMax;                            // 4-6
    float       OverrideLootEffectScale;                // 7
    float       OverrideNameScale;                      // 8
    int16       ObjectEffectPackageID;                  // 19
};

struct ImportPriceArmorEntry
{
    uint32      ID;
    float       ClothModifier;                          // 0
    float       LeatherModifier;                        // 1
    float       ChainModifier;                          // 2
    float       PlateModifier;                          // 3
};

struct ImportPriceQualityEntry
{
    uint32      ID;
    float       Data;                                   // 0
};

struct ImportPriceShieldEntry
{
    uint32      ID;
    float       Data;                                   // 0
};

struct ImportPriceWeaponEntry
{
    uint32      ID;
    float       Data;                                   // 0
};

struct ItemArmorQualityEntry
{
    uint32      ID;
    float       QualityMod[7];                          // 0 - 6
    uint16      ItemLevel;                              // 7
};

struct ItemArmorShieldEntry
{
    uint32      ID;
    float       QualityMod[7];                          // 0 - 6
    uint16      ItemLevel;                              // 7
};

struct ItemArmorTotalEntry
{
    uint32      ID;
    float       Value[4];                               // 0 - 3
    uint32      ItemLevel;                              // 4
};

struct ItemDamageEntry
{
    uint32      ID;
    float       Quality[7];                             // 0 - 6
    uint32      ItemLevel;                              // 7
};

struct ItemClassEntry
{
    uint32      ID;
    uint32      Class;                                  // 0
    float       PriceFactor;                            // 1
    LocalizedString* Name;                              // 2
};

struct ItemDisenchantLootEntry
{
    uint32      ID;
    uint16      MinLevel;                               // 0
    uint16      MaxLevel;                               // 1
    uint16      SkillRequired;                          // 2
    uint8       ItemClass;                              // 3
    int8        ItemSubClass;                           // 4
    uint8       Quality;                                // 5
};

struct ItemLimitCategoryEntry
{
    uint32      ID;
    LocalizedString* NameLang;                          // 0
    uint8       Quantity;                               // 1
    uint8       Flags;                                  // 2
};

struct ItemPriceBaseEntry
{
    uint32      ID;
    float       Armor;                                  // 0
    float       Weapon;                                 // 1
    uint16      ItemLevel;                              // 2
};

struct ItemRandomPropertiesEntry
{
    uint32      ID;
    LocalizedString* Name;                              // 0
    uint32      Enchantment[MAX_ITEM_ENCHANTS];         // 1 - 5
};

struct ItemRandomSuffixEntry
{
    uint32      ID;
    LocalizedString* Name;                              // 0
    LocalizedString* IternalName;                       // 1
    uint32      Enchantment[MAX_ITEM_ENCHANTS];         // 2 - 6
    uint32      AllocationPct[MAX_ITEM_ENCHANTS];       // 7 - 11
};

struct ItemSpecOverrideEntry
{
    uint32      ID;
    uint32      ItemID;                                 // 0
    uint16      SpecID;                                 // 1
};

struct ItemSpecEntry
{
    uint32      ID;
    uint16      SpecID;                                 // 0
    uint8       MinLevel;                               // 1
    uint8       MaxLevel;                               // 2
    uint8       ItemType;                               // 3
    uint8       PrimaryStat;                            // 4
    uint8       SecondaryStat;                          // 5
};

struct ItemSetSpellEntry
{
    uint32      ID; 
    uint32      SpellID;                                // 0
    uint16      ItemSetID;                              // 1
    uint16      ChrSpecID;                              // 2
    uint8       Threshold;                              // 3
};

struct GuildPerkSpellsEntry
{
    uint32      ID;
    uint32      SpellId;
    uint8       Level;
};

struct HeirloomEntry
{
    uint32      ID;                                     // 0
    uint32      ItemID;                                 // 1
    LocalizedString* SourceText;                        // 2
    uint32      OldItem[2];                             // 3 - 4
    uint32      NextDifficultyItemID;                   // 5
    uint32      UpgradeItemID[2];                       // 6 - 7
    uint16      ItemBonusListID[2];                     // 8 - 9
    uint8       Flags;                                  // 10
    uint8       Source;                                 // 11
};

struct HolidaysEntry
{
    uint32      ID;                                         // 0
    uint32      Duration[MAX_HOLIDAY_DURATIONS];            // 1-10
    uint32      Date[MAX_HOLIDAY_DATES];                    // 11-26 (dates in unix time starting at January, 1, 2000)
    uint32      Region;                                     // 27
    uint32      Looping;                                    // 28
    uint32      CalendarFlags[MAX_HOLIDAY_FLAGS];           // 29-38
    uint32      HolidayNameID;                              // 39 HolidayNames.dbc
    uint32      HolidayDescriptionID;                       // 40 HolidayDescriptions.dbc
    LocalizedString* TextureFilename;                       // 41
    uint32      Priority;                                   // 42
    uint32      CalendarFilterType;                         // 43 (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    uint32      Flags;                                      // 44 (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

struct ItemEntry
{
    uint32      ID;                                         // 0
    uint32      FileDataID;                                 // 1
    uint8       Class;                                      // 2
    uint8       SubClass;                                   // 3
    int8        SoundOverrideSubclass;                      // 4
    int8        Material;                                   // 5
    uint8       InventoryType;                              // 6
    uint8       Sheath;                                     // 7
    uint8       GroupSoundsID;                              // 8
};

struct ItemAppearanceEntry
{
    uint32      ID;
    uint32      DisplayID;                                  // 0
    uint32      FileDataID;                                 // 1
    uint32      UnkLegion;                                  // 2
    uint8       UnkLegion2;                                 // 3
};

struct ItemModifiedAppearanceEntry
{
    uint32      ID;                                         // 0
    uint32      ItemID;                                     // 1
    uint16      AppearanceID;                               // 2
    uint8       AppearanceModID;                            // 3
    uint8       VariationID;                                // 4
    int8        UnkLegion;                                  // 5
};

struct MailTemplateEntry
{
    uint32      ID;
    LocalizedString* Content;                               // 0
};

struct ModifierTreeEntry
{
    uint32      ID;
    uint32      Asset;                                      // 0
    uint32      SecondaryAsset;                             // 1
    uint16      Parent;                                     // 2
    uint8       Type;                                       // 3
    uint8       UnkLegion;                                  // 4
    uint8       Operator;                                   // 5
    uint8       Amount;                                     // 6
};

struct MountCapabilityEntry
{
    uint32      ID;
    uint32      RequiredSpell;
    uint32      SpeedModSpell;
    uint16      RequiredRidingSkill;
    uint16      RequiredArea;
    int16      RequiredMap;
    uint8       Flags;
    uint8       RequiredAura;
};

struct MountTypeEntry
{
    uint32      ID;
};

struct MountTypeXCapabilityEntry
{
    uint32      ID;
    uint16      MountTypeID;                                // 0
    uint16      MountCapabilityID;                          // 1
    uint8       OrderIndex;                                 // 2
};

struct NameGenEntry
{
    uint32      ID;
    LocalizedString* Name;
    uint8       RaceID;
    uint8       Gender;
};

struct NamesProfanityEntry
{
    uint32      ID;                                                 // 0
    char const* Name;                                               // 1
    int8        Language;                                           // 2
};

struct NamesReservedEntry
{
    uint32      ID;                                                 // 0
    char const* Name;                                               // 1
};

struct NamesReservedLocaleEntry
{
    uint32      ID;                                                 // 0
    char const* Name;                                               // 1
    uint8       LocaleMask;                                         // 2
};

struct QuestFactionRewEntry
{
    uint32      ID;
    int32       Difficulty[10];
};

struct QuestPOIPointEntry
{
    uint32      ID;                                         // 0
    uint32      QuestPOIBlobID;                             // 1
    int16       X;                                          // 2
    int16       Y;                                          // 3
};

struct QuestSortEntry
{
    uint32      ID;
    LocalizedString* SortName;                              // 0
};

struct QuestV2Entry
{
    uint32      ID;
    uint16      UniqueBitFlag;                              // 0
};

struct QuestXPEntry
{
    uint32      ID;
    uint16      Difficulty[10];
};

struct QuestLineEntry
{
    uint32      ID;
    LocalizedString* LineName;                              // 0
};

struct QuestLineXQuestEntry
{
    uint32      ID;
    uint16      QuestID;                                    // 0
    uint8       LineID;                                     // 1
    uint8       Pos;                                        // 2
};

struct QuestMoneyRewardEntry
{
    uint32      ID;
    uint32      Money[10];                                  // 0
};

struct QuestObjectiveEntry
{
    uint32      ID;
    int32       Amount;                                     // 0
    int32       ObjectID;                                   // 1
    LocalizedString* Description;                           // 2
    uint16      QuestID;                                    // 3
    uint8       Type;                                       // 4
    uint8       StorageIndex;                               // 5
    uint8       UNK;                                        // 6
    uint8       Flags;                                      // 7
};

struct RandPropPointsEntry
{
    uint32      ID;
    uint16      EpicPropertiesPoints[5];                    // 0 - 4
    uint16      RarePropertiesPoints[5];                    // 5 - 9
    uint16      UncommonPropertiesPoints[5];                // 10 - 14
};

struct ResearchBranchEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    LocalizedString* Texture;                               // 1
    uint32      ItemID;                                     // 2
    uint16      CurrencyID;                                 // 3
    uint8       ResearchFieldID;                            // 4
};

struct ResearchProjectEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 1
    LocalizedString* Description;                           // 2
    uint32      Rare;                                       // 3
    uint32      BranchID;                                   // 4
    uint32      SpellID;                                    // 5
    LocalizedString* IconName;                              // 7
    uint32      Complexity;                                 // 6
    uint32      RequiredCurrencyAmount;                     // 8

    bool IsVaid() const
    {
        return BranchID != ARCHAEOLOGY_BRANCH_UNUSED && BranchID != ARCHAEOLOGY_BRANCH_NONE;
    }
};

struct ResearchSiteEntry
{
    uint32      ID;                                           // 0 ID
    uint32      MapID;                                        // 1 MapID
    uint32      POIid;                                        // 2
    LocalizedString* areaName;                              // 3 Research site name
    uint32      flags;                                        // 4 Always 177.

    bool IsValid() const
    {
        return ID != 140 && // template
            ID != 142 &&    // template
            ID != 161 &&    // template
            ID != 471 &&    // vashj'ir
            ID != 473 &&    // vashj'ir
            ID != 475 &&    // vashj'ir
            ID != 949 &&    // template
            ID != 951 &&    // template
            ID != 1023 &&   // not on wowhead
            ID != 1049 &&   // not on wowhead
            ID != 1051 &&   // not on wowhead
            ID != 1284;     // not on wowhead
    }
};

struct ItemBonusEntry
{
    uint32      ID;                                                 // 0
    int32       Value[2];                                           // 1 - 2
    uint16      BonusListID;                                        // 3
    uint8       Type;                                               // 4
    uint8       Index;                                              // 5
};

struct ItemBonusTreeNodeEntry
{
    uint32      ID;                                                      // 0
    uint16      BonusTreeID;                                             // 1
    uint16      SubTreeID;                                               // 2
    uint16      BonusListID;                                             // 3
    uint8       BonusTreeModID;                                          // 4
};

struct ItemCurrencyCostEntry
{
    uint32      ID;                                                 // 0
    uint32      ItemId;                                             // 1
};

struct ItemEffectEntry
{
    uint32      ID;
    uint32      ItemID;                                     // 1
    uint32      SpellID;                                    // 2
    int32       Cooldown;                                   // 3
    int32       CategoryCooldown;                           // 4
    int16       Charges;                                    // 5
    uint16      Category;                                   // 6
    uint8       SpecID;                                     // 7
    uint8       OrderIndex;                                 // 8
    uint8       Trigger;                                    // 9
};

struct ItemSparseEntry
{
    uint32      ID;
    uint32      Flags[MAX_ITEM_PROTO_FLAGS];                        // 0 - 2
    float       UnkFloat1;                                          // 3
    float       UnkFloat2;                                          // 4
    uint32      BuyPrice;                                           // 5
    uint32      SellPrice;                                          // 6
    int32       AllowableClass;                                     // 7
    int32       AllowableRace;                                      // 8
    uint32      RequiredSpell;                                      // 9
    int32       MaxCount;                                           // 10
    int32       Stackable;                                          // 11
    int32       ItemStatAllocation[MAX_ITEM_PROTO_STATS];           // 12 - 21
    float       ItemStatSocketCostMultiplier[MAX_ITEM_PROTO_STATS]; // 22 - 31
    float       RangedModRange;                                     // 32
    LocalizedString* Name;                                          // 33
    LocalizedString* Name2;                                         // 34
    LocalizedString* Name3;                                         // 35
    LocalizedString* Name4;                                         // 36
    LocalizedString* Description;                                   // 37
    uint32      BagFamily;                                          // 38
    float       ArmorDamageModifier;                                // 39
    uint32      Duration;                                           // 40
    float       StatScalingFactor;                                  // 41
    uint16      ItemLevel;                                          // 42
    uint16      RequiredSkill;                                      // 43
    uint16      RequiredSkillRank;                                  // 44
    uint16      RequiredReputationFaction;                          // 45
    int16       ItemStatValue[MAX_ITEM_PROTO_STATS];                // 46 - 55
    uint16      ScalingStatDistribution;                            // 56
    uint16      Delay;                                              // 57
    uint16      PageText;                                           // 58
    uint16      StartQuest;                                         // 59
    uint16      LockID;                                             // 60
    uint16      RandomProperty;                                     // 61
    uint16      RandomSuffix;                                       // 62
    uint16      ItemSet;                                            // 63
    uint16      Area;                                               // 64
    uint16      Map;                                                // 65
    uint16      SocketBonus;                                        // 66
    uint16      GemProperties;                                      // 67
    uint16      ItemLimitCategory;                                  // 68
    uint16      HolidayID;                                          // 69
    uint16      ItemNameDescriptionID;                              // 70
    uint8       Quality;                                            // 71
    uint8       BuyCount;                                           // 72
    uint8       InventoryType;                                      // 73
    int8        RequiredLevel;                                      // 74
    uint8       RequiredHonorRank;                                  // 75 looks unused now
    uint8       RequiredCityRank;                                   // 76 looks unused now
    uint8       RequiredReputationRank;                             // 77
    uint8       ContainerSlots;                                     // 78
    int8        ItemStatType[MAX_ITEM_PROTO_STATS];                 // 79 - 88
    uint8       DamageType;                                         // 89
    uint8       Bonding;                                            // 90
    uint8       LanguageID;                                         // 91
    uint8       PageMaterial;                                       // 92
    int8        Material;                                           // 93
    uint8       Sheath;                                             // 94
    uint8       TotemCategory;                                      // 95
    uint8       SocketColor[MAX_ITEM_PROTO_SOCKETS];                // 96 - 98
    uint8       CurrencySubstitutionID;                             // 99
    uint8       CurrencySubstitutionCount;                          // 100
    uint8       UnkLegion;                                          // 101
    uint8       UnkLegion2;                                         // 102
};

struct ItemExtendedCostEntry
{
    uint32      ID;
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];              // 0 - 4
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];// 5 - 9
    uint32      RequiredMoney;                                      // 10
    uint16      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS];         // 11 - 15
    uint16      RequiredPersonalArenaRating;                        // 12
    uint16      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];     // 17 - 21
    uint8       RequiredArenaSlot;                                  // 22
    uint8       ItemPurchaseGroup;                                  // 23
    uint8       RequiredFactionId;                                  // 24
    uint8       RequirementFlags;                                   // 25
    uint8       RequiredFactionStanding;                            // 26

    bool IsSeasonCurrencyRequirement(uint32 i) const
    {
        if (i > MAX_ITEM_EXT_COST_CURRENCIES)
            return 0;

        return RequirementFlags & (1 << (i + 1));
    }
};

struct BattlePetAbilityEntry
{
    uint32 ID;                  // 0
    uint32 Type;                // 1
    uint32 fileDataEntry;     // 2
    uint32 turnCooldown;        // 3
    uint32 auraAbilityID;       // 4
    uint32 auraDuration;        // 5
    LocalizedString* name;               // 6
    LocalizedString* description;        // 7
};

struct BattlePetAbilityEffectEntry
{
    uint32 ID;                  // 0
    uint32 TurnEntryID;         // 1
    uint32 unk;               // 2
    uint32 AuraId_unk;           // 3
    uint32 propertiesID;        // 4
    uint32 effectIndex;         // 5
    uint32 propertyValues[MAX_EFFECT_PROPERTIES];   // 6 - 11
};

struct BattlePetEffectPropertiesEntry
{
    uint32 ID;                  // 0
    uint32 someFlags;         // 1
    LocalizedString* propertyDescs[MAX_EFFECT_PROPERTIES];     // 2 - 7
    uint32 unk;               // 8
    uint32 unk2;               // 9
    uint32 unk3;               // 10
    uint32 unk4;               // 11
    uint32 unk5;               // 12
    uint32 unk6;               // 13
};

struct BattlePetAbilityTurnEntry
{
    uint32 ID;                  // 0
    uint32 AbilityID;           // 1
    uint32 stateID;           // 2
    uint32 turnIndex;           // 3
    uint32 unk;              // 4
    uint32 procIndex;        // 5
};

struct BattlePetAbilityStateEntry
{
    uint32 ID;                  // 0
    uint32 AbilityID;         // 1
    uint32 stateID;           // 2
    uint32 unk;               // 3
};

struct BattlePetStateEntry
{
    uint32 ID;                  // 0
    LocalizedString* stateName; // 1
    uint16 flags;               // 2
    uint8  unk;                 // 3
};

struct BattlePetSpeciesEntry
{
    uint32 ID;                  // 0
    uint32 CreatureEntry;       // 1
    uint32 IconFileID;          // 2
    uint32 spellId;             // 3
    LocalizedString* SourceText; // 4
    LocalizedString* Description; // 5
    uint16 flags;               // 6
    uint8 petType;             // 7
    int8   source;              // 8
};

struct BattlePetSpeciesStateEntry
{
    uint32 ID;                                                      // 0
    int32 Value;                                                    // 1
    uint16 SpeciesID;                                               // 2
    int8 State;                                                     // 3
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 ID;
    uint16 speciesID;
    uint16 abilityID;
    uint8 requiredLevel;
    uint8 rank;
};

struct BattlePetBreedQualityEntry
{
    uint32 ID;
    float qualityModifier;
    uint8 quality;
};

struct BattlePetBreedStateEntry
{
    uint32 ID;
    uint16 stateModifier;
    uint8 breedID;
    uint8 stateID;
};

struct CharShipmentEntry
{
    uint32 ID;
    uint32 ShipmentConteinerID;
    uint32 CriteriaID;
    uint32 Flags;
    uint32 TimeForShipment;
    uint32 SpellCreation;
    uint32 ShipmentResultItemID;
};

struct CharShipmentConteiner
{
    uint32 ID;
    uint32 Flags;
    uint32 BuildingType;
    //LocalizedString* Name;
    uint32 unt3;
    uint32 unk4;
    uint32 unk5;
    uint32 unk6;
    uint32 unk7;
    uint32 MaxShipments;
    //LocalizedString* Description;
    uint32 enotherID;          //id of other fraction.
    int32 fractionType;        //-1 - for all, 0 - horde, 1 - alliance
};

struct GarrAbilityEntry
{
    uint32 ID;                                                      // 0
    uint32 Flags;                                                   // 1
    LocalizedString* Name;                                          // 2
    LocalizedString* Description;                                   // 3
    uint32 IconFileDataID;                                          // 4
    uint32 OtherFactionGarrAbilityID;                               // 5
    uint32 GarrAbilityCategoryID;                                   // 6
};

struct GarrAbilityEffectEntry
{
    uint32 ID;                  // 0
    uint32 Flags;               // 1
    uint32 GarrAbilityID;       // 2
    uint32 Unk1;                // 3
    uint32 GarrMechanicTypeID;  // 4
    uint32 Unk3;                // 5
    float Amount;               // 6
    float Amount2;              // 7
    float Amount3;              // 8
    uint32 Unk4;                // 9
    uint32 Unk5;                // 10
};

struct GarrBuildingEntry
{
    uint32 ID;                                                      // 0
    uint32 HordeGameObjectID;                                       // 1
    uint32 AllianceGameObjectID;                                    // 2
    uint32 Unknown;                                                 // 3
    uint32 Type;                                                    // 4
    uint32 Level;                                                   // 5
    LocalizedString* NameAlliance;                                  // 6
    LocalizedString* NameHorde;                                     // 7
    LocalizedString* Description;                                   // 8
    LocalizedString* Tooltip;                                       // 9
    uint32 BuildDuration;                                           // 10
    uint32 CostCurrencyID;                                          // 11
    int32  CostCurrencyAmount;                                      // 12
    uint32 HordeTexPrefixKitID;                                     // 13
    uint32 AllianceTexPrefixKitID;                                  // 14
    uint32 IconFileDataID;                                          // 15
    uint32 BonusAmount;                                             // 16
    uint32 Flags;                                                   // 17
    uint32 AllianceActivationScenePackageID;                        // 18
    uint32 HordeActivationScenePackageID;                           // 19
    uint32 MaxShipments;                                            // 20
    uint32 FollowerRequiredGarrAbilityID;                           // 21
    uint32 FollowerGarrAbilityEffectID;                             // 22
    int32  CostMoney;                                               // 23
};

struct GarrBuildingPlotInstEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrBuildingID;                                          // 1
    uint32 UiTextureAtlasMemberID;                                  // 2
    uint32 GarrSiteLevelPlotInstID;                                 // 3
    DBCPosition2D LandmarkOffset;                                   // 4-5
};

struct GarrClassSpecEntry
{
    uint32 ID;                                                      // 0
    LocalizedString* NameMale;                                      // 1
    LocalizedString* NameFemale;                                    // 2
    LocalizedString* NameGenderless;                                // 3
    uint32 ClassAtlasID;                                            // 4 UiTextureAtlasMember.db2 ref
    uint32 GarrFollItemSetID;                                       // 5
};

struct GarrFollowerEntry
{
    uint32 ID;                                                      // 0
    uint32 HordeCreatureID;                                         // 1
    uint32 AllianceCreatureID;                                      // 2
    uint32 HordeUiAnimRaceInfoID;                                   // 3
    uint32 AllianceUiAnimRaceInfoID;                                // 4
    uint32 Quality;                                                 // 5
    uint32 HordeGarrClassSpecID;                                    // 6
    uint32 AllianceGarrClassSpecID;                                 // 7
    uint32 HordeGarrFollItemSetID;                                  // 8
    uint32 AllianceGarrFollItemSetID;                               // 9
    uint32 Level;                                                   // 10
    uint32 ItemLevelWeapon;                                         // 11
    uint32 ItemLevelArmor;                                          // 12
    uint32 Unknown1;                                                // 13
    uint32 Flags;                                                   // 14
    LocalizedString* HordeSourceText;                               // 15
    LocalizedString* AllianceSourceText;                            // 16
    int32 Unknown2;                                                 // 17
    int32 Unknown3;                                                 // 18
    uint32 HordePortraitIconID;                                     // 19
    uint32 AlliancePortraitIconID;                                  // 20
};

struct GarrFollowerLevelXPEntry
{
    uint32 ID;
    uint32 Level;
    uint32 NextLevelXP;
    uint32 Unk;
};

struct GarrFollowerQualityEntry
{
    uint32 ID;
    uint32 Quality;
    uint32 NextQualityXP;
    uint32 Unk;
};

struct GarrFollowerXAbilityEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrFollowerID;                                          // 1
    uint32 GarrAbilityID;                                           // 2
    uint32 FactionIndex;                                            // 3
};

struct GarrEncounterEntry
{
    uint32 ID;                                                      // 0
    uint32 CreatureEntry;                                           // 1
    LocalizedString* Name;                                          // 2
    float Mod;                                                      // 3
    float Mod1;                                                     // 4
    uint32 SomeFileDataID;                                          // 5
};

struct GarrEncounterXMechanicEntry
{
    uint32 ID;                  // 0
    uint32 GarrEncounterID;     // 1
    uint32 GarrMechanicID;      // 2
};

struct GarrMechanicEntry
{
    uint32 ID;                  // 0
    uint32 Type;                // 1 (GarrMechanicTypeID)
    float Mod;                  // 2
};

struct GarrMechanicTypeEntry
{
    uint32 ID;
    uint32 Category;
    LocalizedString* Name;
    LocalizedString* Description;
    uint32 SomeFileDataID;
};

struct GarrMissionEntry
{
    uint32 ID;                    // 0
    uint32 ReqLevel;              // 1
    uint32 ReqFollowersItemLevel; // 2
    uint32 GarrMechanicTypeID;    // 3
    uint32 ReqFollowersCount;     // 4
    uint32 Unk1;                  // 5
    uint32 MissionDuration;       // 6
    uint32 OfferDuration;         // 7
    uint32 Unk2;                  // 8
    uint32 GarrMissionTypeID;     // 9
    LocalizedString* Name;        // 10
    LocalizedString* Description; // 11
    LocalizedString* Location;    // 12
    uint32 CriteriaID;            // 13
    uint32 Unk3;                  // 14
    uint32 ReqResourcesCount;     // 15
    uint32 Unk4;                  // 16
    uint32 BaseXP;                // 17
    uint32 BaseChance;            // 18
};

struct GarrMissionRewardEntry
{
    uint32 ID;                  // 0
    uint32 MissionID;           // 1
    uint32 RewardXP;            // 2
    uint32 RewardItemID;        // 3
    uint32 ItemAmount;          // 4
    uint32 CurrencyID;          // 5
    uint32 CurrencyValue;       // 6
    uint32 Unk2;                // 7
    uint32 Unk3;                // 8
    uint32 Unk4;                // 9

    bool HasItemReward() const { return RewardItemID != 0; }
    bool HasMoneyReward() const { return CurrencyID == 0 && CurrencyValue > 0; }
    bool HasCurrencyReward() const { return CurrencyID != 0 && CurrencyValue > 0; }
    bool HasFollowerXPReward() const { return RewardXP > 0; }
};

struct GarrMissionXEncounterEntry
{
    uint32 ID;                  // 0
    uint32 GarrMissionID;       // 1
    uint32 GarrEncounterID;     // 2
};

struct GarrPlotEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrPlotUICategoryID;                                    // 1
    uint32 PlotType;                                                // 2
    uint32 Flags;                                                   // 3
    LocalizedString* Name;                                          // 4
    uint32 MinCount;                                                // 5
    uint32 MaxCount;                                                // 6
    uint32 AllianceConstructionGameObjectID;                        // 7
    uint32 HordeConstructionGameObjectID;                           // 8
};

struct GarrPlotBuildingEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrPlotID;                                              // 1
    uint32 GarrBuildingID;                                          // 2
};

struct GarrPlotInstanceEntry
{
    uint32 ID;                                                      // 0
    uint32 GarrPlotID;                                              // 1
    LocalizedString* Name;                                          // 2
};

struct GarrSiteLevelEntry
{
    uint32 ID;                                                      // 0
    uint32 Level;                                                   // 1
    uint32 MapID;                                                   // 2
    uint32 SiteID;                                                  // 3
    uint32 UITextureKitID;                                          // 4
    DBCPosition2D TownHall;                                         // 5-6
    uint32 MovieID;                                                 // 7
    uint32 Level2;                                                  // 8
    int32 UpgradeResourceCost;                                      // 9
    int32 UpgradeMoneyCost;                                         // 10
};

struct GarrSiteLevelPlotInstEntry
{
    uint32 ID;                                                       // 0
    uint32 GarrSiteLevelID;                                          // 1
    uint32 GarrPlotInstanceID;                                       // 2
    DBCPosition2D Landmark;                                          // 3-4
    uint32 Unknown;                                                  // 5
};

struct CreatureDifficultyEntry
{
    uint32      ID;
    uint32      CreatureID;
    uint32      Flags[5];
    uint32      UnkLegion;
    uint32      UnkLegion2;
    uint16      FactionID;
    int8        Expansion;
    int8        MinLevel;
    int8        MaxLevel;
};

struct CurrencyTypesEntry
{
    uint32      ID;                                                 // 0
    LocalizedString* Name;                                          // 1
    LocalizedString* InventoryIcon[2];                              // 2 - 3
    uint32      MaxQty;                                             // 4
    uint32      MaxEarnablePerWeek;                                 // 5
    uint32      Flags;                                              // 6
    LocalizedString* Description;                                   // 7
    uint8       CategoryID;                                         // 8
    uint8       SpellWeight;                                        // 9
    uint8       SpellCategory;                                      // 10
    uint8       Quality;                                            // 11
};

struct QuestPackageItemEntry
{
    uint32      ID;
    uint32      ItemID;                                             // 0
    uint16      QuestPackageID;                                     // 1
    uint8       ItemCount;                                          // 2
    uint8       FilterType;                                         // 3
};

struct MountEntry
{
    uint32      ID;
    uint32      SpellID;
    uint32      DisplayID;
    LocalizedString* Name;
    LocalizedString* Description;
    LocalizedString* SourceDescription;
    uint16      MountTypeID;
    uint16      Flags;
    uint16      PlayerConditionId;
    uint8       Source;
};

struct LanguageWordsEntry
{
    uint32      ID;
    LocalizedString* Word;                                      // 0
    uint8       Lang;                                           // 1
};

struct LightEntry
{
    uint32      ID;                                         // 0
    DBCPosition3D Pos;                                      // 1 - 3
    float       FalloffStart;                               // 4
    float       FalloffEnd;                                 // 5
    uint16      MapID;                                      // 6
    uint16      LightParamsID[8];                           // 7 - 14
};

struct KeyChainEntry
{
    uint32      Id;
    uint8       Key[KEYCHAIN_SIZE];
};

struct OverrideSpellDataEntry
{
    uint32 ID;                                                      // 0
    uint32 SpellID[MAX_OVERRIDE_SPELL];                             // 1-10
    uint32 PlayerActionbarFileDataID;                               // 11
    uint8  Flags;                                                   // 12
};

struct SpellAuraRestrictionsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      CasterAuraSpell;                            // 1
    uint32      TargetAuraSpell;                            // 2
    uint32      ExcludeCasterAuraSpell;                     // 3
    uint32      ExcludeTargetAuraSpell;                     // 4
    uint8       DifficultyID;                               // 5
    uint8       CasterAuraState;                            // 6
    uint8       TargetAuraState;                            // 7
    uint8       ExcludeCasterAuraState;                     // 8
    uint8       ExcludeTargetAuraState;                     // 9
};

struct SpellCastingRequirementsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint16      MinFactionID;                               // 1
    int16       RequiredAreasID;                            // 2
    uint16      RequiresSpellFocus;                         // 3
    uint8       FacingCasterFlags;                          // 4
    uint8       MinReputation;                              // 5
    uint8       RequiredAuraVision;                         // 6
};

struct SpellClassOptionsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    flag128     SpellClassMask;                             // 1
    uint16      ModalNextSpell;                             // 2
    uint8       SpellClassSet;                              // 3
};

struct SpellLearnSpellEntry
{
    uint32      ID;                                         // 0
    uint32      LearnSpellID;                               // 1
    uint32      SpellID;                                    // 2
    uint32      OverridesSpellID;                           // 3
};

struct SpellMiscEntry
{
    uint32      ID;                                         // 0
    uint32      Attributes[MaxAttributes];                  // 1 - 14
    float       Speed;                                      // 15
    float       MultistrikeSpeedMod;                        // 16
    uint16      CastingTimeIndex;                           // 17
    uint16      DurationIndex;                              // 18
    uint16      RangeIndex;                                 // 19
    uint16      SpellIconID;                                // 20
    uint16      ActiveIconID;                               // 21
    uint8       SchoolMask;                                 // 22
};

struct SpellMiscDifficultyEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint8       DifficultyID;                               // 2
};

struct SpellPowerEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    int32       PowerCost;                                  // 1
    float       PowerCostPercentage;                        // 2
    float       PowerCostPercentagePerSecond;               // 3
    uint32      RequiredAura;                               // 4
    float       HealthCostPercentage;                       // 5
    uint16      PowerCostPerSecond;                         // 6
    uint16      ManaCostAdditional;                         // 7
    uint16      PowerDisplayID;                             // 8
    uint16      UnitPowerBarID;                             // 9
    uint8       PowerIndex;                                 // 10
    uint8       PowerType;                                  // 11
    uint8       PowerCostPerLevel;                          // 12
};

struct SpellReagentsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    int32       Reagent[MAX_SPELL_REAGENTS];                // 1-8
    uint16      ReagentCount[MAX_SPELL_REAGENTS];           // 9-16
};

struct SpellReagentsCurrencyEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint16      CurrencyID;                                 // 1
    uint16      CurrencyCount;                              // 2
};

struct ToyEntry
{
    uint32      ID;                                         // 0
    uint32      ItemID;                                     // 1
    LocalizedString* Description;                           // 2
    uint8       Flags;                                      // 3
    uint8       CategoryFilter;                             // 4
};

struct PvpItemEntry
{
    uint32      ID;
    uint32      ItemID;                                     // 0
    uint8       BonusIlvl;                                  // 1
};

struct PvpTalentUnlockEntry
{
    uint32      ID;
    uint8       Row;                                        // 0
    uint8       Column;                                     // 1
    uint8       HonorLevel;                                 // 2
};

struct PvpTalentEntry
{
    uint32      ID;                                         // 0
    uint32      Row;                                        // 1
    uint32      Column;                                     // 2
    uint32      SpellID;                                    // 3
    uint32      Unknown4;                                   // 4
    uint32      ClassID;                                    // 5
    uint32      SpecializationID;                           // 6
    uint32      Flags;                                      // 7
    uint32      OverrideSpellID;                            // 8
    LocalizedString* Desc;                                  // 9
};

struct SpellTotemsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      TotemCategory[MAX_SPELL_TOTEMS];            // 1 - 2
    uint8       Totem[MAX_SPELL_TOTEMS];                    // 3 - 4
};

struct SpellXSpellVisualEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    float       TravelSpeed;                                // 2
    uint16      SpellVisualID[2];                           // 3 - 4
    uint16      PlayerConditionID;                          // 5
    uint8       DifficultyID;                               // 6
    uint8       Flags;                                      // 7
};

struct TaxiNodesEntry
{
    uint32          ID;                                     // 0
    DBCPosition3D   Pos;                                    // 1-4
    LocalizedString* Name_lang;                             // 5
    uint32          MountCreatureID[MAX_TEAMS];           // 6-7
    float           MapOffset[MAX_TEAMS];                 // 8-9
    uint16          MapID;                                  // 10
    uint16          ConditionID;                            // 11
    uint16          LearnableIndex;                         // 12 - some kind of index only for learnable nodes
    uint8          Flags;                                  // 13
};

struct TaxiPathEntry
{
    uint32          ID;                                     // 0
    uint16          From;                                   // 1
    uint16          To;                                     // 2
    uint16          Cost;                                   // 3
};

struct TaxiPathNodeEntry
{
    uint32          ID;                                     // 0
    DBCPosition3D   Loc;                                    // 1-3
    uint32          Delay;                                  // 4
    uint16          PathID;                                 // 5
    uint16          MapID;                                  // 6
    uint16          ArrivalEventID;                         // 7
    uint16          DepartureEventID;                       // 8
    uint8           NodeIndex;                              // 9
    uint8           Flags;                                  // 10
};

struct ItemUpgradeEntry
{
    uint32 id;                  // 1 rules id from RuleSetItemUpgradeEntry startUpgrade
    uint32 currencyReqAmt;      // 2 currency count
    uint16 prevUpgradeId;       // 3
    uint16 currencyReqId;       // 4 currency Id
    uint8  itemUpgradePathId;   // 5 extended-cost entry id
    uint8  levelBonus;          // 6 total level bonus related to non-upgraded item
};

struct RuleSetItemUpgradeEntry
{
    uint32 id;                  // 0 m_ID
    uint32 levelUpd;            // 1 level upgrade
    uint32 startUpgrade;        // 2 start update rules for ItemUpgradeEntry
    uint32 itemEntry;           // 3 Item ID
};

struct GameObjectsEntry
{
    uint32      ID;                                                 // 0
    DBCPosition3D Position;                                         // 1 - 3
    DBCPosition4D Rotation;                                         // 4 - 7
    float       Size;                                               // 8
    int32       Data[GO_DBC_DATA_COUNT];                            // 9 - 16
    LocalizedString* Name;                                          // 17
    uint16      MapID;                                              // 18
    uint16      DisplayID;                                          // 19
    uint16      PhaseID;                                            // 20
    uint16      PhaseUseFlags;                                      // 21
    uint8       PhaseGroupID;                                       // 22
    uint8       Type;                                               // 23
};

struct GameTablesEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    uint16      NumRows;                                    // 1
    uint8       NumColumns;                                 // 2
};

struct ItemUpgradeData
{
    ItemUpgradeData() { }

    std::array<ItemUpgradeEntry const*, MAX_ITEM_UPDGRADES> upgrade;
};

struct MapChallengeModeEntry
{
    uint32 id;                  // 0
    uint32 map;                 // 1
    uint32 unk1;                // 2
    uint32 unk2;                // 3
    uint32 season;              // 4
    uint32 bronze;              // 5
    uint32 silver;              // 6
    uint32 gold;                // 7
    uint32 unk3;                // 8
    uint32 unk4;                // 9
};

struct SpellVisualEntry
{
    uint32 ID;                  // 0
    uint32 unk;               // 1
    uint32 unk2;               // 2
    uint32 unk3;              // 3
    uint32 unk4;               // 4
    uint32 unk5;               // 5
    uint32 unk6;               // 6
    uint32 unk7;               // 7
    uint32 unk8;               // 8
    uint32 unk9;               // 9
    uint32 unk10;               // 10
    uint32 unk11;               // 11
    uint32 unk12;               // 12
    uint32 unk13;               // 13
    uint32 unk14;               // 14
    uint32 unk15;               // 15
    uint32 unk16;               // 16
    uint32 unk17;               // 17
    uint32 unk18;               // 18
    float unk19;                // 19
    float unk20;                // 20
    float unk21;                // 21
    float unk22;                // 22
    float unk23;                // 23
    float unk24;                // 24
    uint32 hostileId;           // 25
    uint32 unk25;               // 26
    uint32 unk26;               // 27
    uint32 unk27;               // 28
    uint32 unk28;               // 29
};

struct ItemXBonusTreeEntry
{
    uint32      ID;                                         // 0
    uint32      ItemID;                                     // 1
    uint16      BonusTreeID;                                // 2
};

struct SpellEffectEntry
{
    uint32      ID;                                         // 0
    float       EffectAmplitude;                            // 1
    uint32      EffectAuraPeriod;                           // 2
    uint32      EffectBasePoints;                           // 3
    float       EffectBonusCoefficient;                     // 4
    float       EffectChainAmplitude;                       // 5
    uint32      EffectDieSides;                             // 6
    uint32      EffectItemType;                             // 7
    int32       EffectMiscValue[2];                         // 8 - 9
    float       EffectPointsPerResource;                    // 10
    float       EffectRealPointsPerLevel;                   // 11
    flag128     EffectSpellClassMask;                       // 12 - 15
    uint32      EffectTriggerSpell;                         // 16
    float       EffectPosFacing;                            // 17
    uint32      SpellID;                                    // 18
    uint32      EffectAttributes;                           // 19
    float       BonusCoefficientFromAP;                     // 20
    uint16      EffectAura;                                 // 21
    uint16      EffectChainTargets;                         // 22
    int8        DifficultyID;                               // 23
    uint8       Effect;                                     // 24
    int8        EffectMechanic;                             // 25
    uint8       EffectRadiusIndex[2];                       // 26 - 27
    uint8       ImplicitTarget[2];                          // 28 - 29
    int8        EffectIndex;                                // 30
};

struct SpellShapeshiftFormEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    float       UnkLegion;                                  // 1 seems related to bonus stats
    uint32      Flags;                                      // 2
    uint16      AttackIconID;                               // 3
    uint16      CombatRoundTime;                            // 4
    uint16      CreatureDisplayID[4];                       // 5 - 8
    uint16      PresetSpellID[MAX_SHAPESHIFT_SPELLS];       // 9 - 16
    int8        CreatureType;                               // 17
    uint8       MountTypeID;                                // 18
    uint8       ExitSoundEntriesID;                         // 19
};

struct SpellShapeshiftEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      ShapeshiftExclude[2];                       // 1-2
    uint32      ShapeshiftMask[2];                          // 3-4
    int8        StanceBarOrder;                             // 5
};

struct SpellTargetRestrictionsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    float       ConeAngle;                                  // 1
    float       Width;                                      // 2
    uint32      Targets;                                    // 3
    uint16      MaxTargetLevel;                             // 4
    uint16      TargetCreatureType;                         // 5
    uint8       DifficultyID;                               // 6
    uint8       MaxAffectedTargets;                         // 7
};

struct SpellEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Name;                                  // 1
    LocalizedString* NameSubtext;                           // 2
    LocalizedString* Description;                           // 3
    LocalizedString* AuraDescription;                       // 4
    uint32      SpellMiscDifficultyID;                      // 5
    uint16      DescriptionVariablesID;                     // 6

    SpellEffectEntry const* GetSpellEffect(uint32 eff, uint8 diff = 0) const;
};

struct SpellScalingEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint16      ScalesFromItemLevel;                        // 1
    uint8       ScalingClass;                               // 2
    uint8       MaxScalingLevel;                            // 3
};

struct SpellRangeEntry
{
    uint32      ID;
    float       MinRangeHostile;                            // 0
    float       MinRangeFriend;                             // 1
    float       MaxRangeHostile;                            // 2
    float       MaxRangeFriend;                             // 3
    LocalizedString* DisplayName;                           // 4
    LocalizedString* DisplayNameShort;                      // 5
    uint8       Flags;                                      // 6
};

struct SpellRadiusEntry
{
    uint32      ID;
    float       Radius;                                     // 0
    float       RadiusPerLevel;                             // 1
    float       RadiusMin;                                  // 2
    float       RadiusMax;                                  // 3
};

struct SpellProcsPerMinuteEntry
{
    uint32      ID;
    float       BaseProcRate;                               // 0
    uint8       Flags;                                      // 1
};

struct SpellProcsPerMinuteModEntry
{
    uint32      ID;
    float       PpmRateMod;                                 // 0
    uint16      Type;                                       // 1
    uint8       SpecID;                                     // 2
    uint8       SpellProcsPerMinuteID;                      // 3
};

struct SpellLevelsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint16      BaseLevel;                                  // 1
    uint16      MaxLevel;                                   // 2
    uint16      SpellLevel;                                 // 3
    uint8       DifficultyID;                               // 4
    uint8       Unk701;                                     // 5
};

struct SpellItemEnchantmentConditionEntry
{
    uint32      ID;
    uint8       Color[MAX_ITEM_ENCHANTS];                   // 0 - 4
    uint8       LtOperandType[MAX_ITEM_ENCHANTS];           // 5 - 9
    uint8       Comparator[MAX_ITEM_ENCHANTS];              // 10 - 14
    uint8       CompareColor[MAX_ITEM_ENCHANTS];            // 15 - 19
    uint8       Value[MAX_ITEM_ENCHANTS];                   // 20 - 24
    uint8       Logic[MAX_ITEM_ENCHANTS];                   // 25 - 29
};

struct SpellInterruptsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      AuraInterruptFlags[2];                      // 1 - 2
    uint32      ChannelInterruptFlags[2];                   // 3 - 4
    uint16      InterruptFlags;                             // 5
    uint8       DifficultyID;                               // 6
};

struct SpellFocusObjectEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
};

struct SpellEquippedItemsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    int32       EquippedItemInventoryTypeMask;              // 1
    int32       EquippedItemSubClassMask;                   // 2
    uint8       EquippedItemClass;                          // 3
};

struct SpellEffectScalingEntry
{
    uint32      ID;
    float       Coefficient;                                // 0
    float       Variance;                                   // 1
    float       OtherCoefficient;                           // 2
    uint32      SpellEffectId;                              // 3
};

struct SpellDurationEntry
{
    uint32      ID;
    int32       Duration;                                   // 0
    int32       MaxDuration;                                // 1
    uint16      DurationPerLevel;                           // 2
};

struct SpellCooldownsEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      CategoryRecoveryTime;                       // 1
    uint32      RecoveryTime;                               // 2
    uint32      StartRecoveryTime;                          // 3
    uint8       DifficultyID;                               // 4
};

struct SpellCategoryEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    int32       ChargeRecoveryTime;                         // 1
    int32       UnkLegion;                                  // 2
    uint8       Flags;                                      // 3
    uint8       UsesPerWeek;                                // 4
    uint8       MaxCharges;                                 // 5
};

struct SpellCategoriesEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint16      Category;                                   // 1
    uint16      StartRecoveryCategory;                      // 2
    uint16      ChargeCategory;                             // 3
    uint8       DifficultyID;                               // 4
    uint8       DefenseType;                                // 5
    uint8       DispelType;                                 // 6
    uint8       Mechanic;                                   // 7
    uint8       PreventionType;                             // 8
};

struct SpellCastTimesEntry
{
    uint32      ID;
    int32       Base;                                       // 0
    int32       Minimum;                                    // 1
    int16       PerLevel;                                   // 2
};

struct SpellAuraOptionsEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 0
    uint32      ProcCharges;                                // 1
    uint32      ProcTypeMask;                               // 2
    uint32      ProcCategoryRecovery;                       // 3
    uint16      CumulativeAura;                             // 4
    uint8       DifficultyID;                               // 5
    uint8       ProcChance;                                 // 6
    uint8       SpellProcsPerMinuteID;                      // 7
};

struct SpecializationSpellEntry
{
    uint32      ID;                                         // 0
    uint32      SpellID;                                    // 1
    uint32      OverridesSpellID;                           // 2
    LocalizedString* DescriptionLang;                       // 3
    uint16      SpecID;                                     // 4
    uint8       UnkLegion;                                  // 5 maybe pvp talents related?
};

struct SkillRaceClassInfoEntry
{
    uint32      ID;
    int32       RaceMask;                                   // 0
    uint16      SkillID;                                    // 1
    int16       ClassMask;                                  // 2
    uint16      Flags;                                      // 3
    uint16      SkillTierID;                                // 4
    uint8       Availability;                               // 5
    uint8       MinLevel;                                   // 6
};

struct SkillLineEntry
{
    uint32      ID;
    LocalizedString* DisplayName;                           // 0
    LocalizedString* Description;                           // 1
    LocalizedString* AlternateVerb;                         // 2
    uint16      SpellIconID;                                // 3
    uint16      Flags;                                      // 4
    uint8       CategoryID;                                 // 5
    uint8       CanLink;                                    // 6
    uint8       ParentSkillLineID;                          // 7
};

struct SkillLineAbilityEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      RaceMask;                                   // 1
    uint32      ClassMask;                                  // 2
    uint32      SupercedesSpell;                            // 3
    uint16      SkillLine;                                  // 4
    uint16      MinSkillLineRank;                           // 5
    uint16      TrivialSkillLineRankHigh;                   // 6
    uint16      TrivialSkillLineRankLow;                    // 7
    uint16      UniqueBit;                                  // 8
    uint16      TradeSkillCategoryID;                       // 9
    uint8       AquireMethod;                               // 10
    uint8       NumSkillUps;                                // 11
};

struct ScenarioEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    uint8       Flags;                                      // 1
    uint8       UnkLegion;                                  // 2
    uint8       UnkLegion2;                                 // 3

    bool IsChallenge() const { return Flags & SCENARIO_FLAG_CHALLENGE; }
    bool IsProvingGrounds() const { return Flags & SCENARIO_FLAG_SUPRESS_STAGE_TEXT; }
};

struct ScenarioStepEntry
{
    uint32      ID;
    LocalizedString* DescriptionLang;                       // 0
    LocalizedString* TitleLang;                             // 1
    uint16      Criteriatreeid;                             // 2
    uint16      ScenarioID;                                 // 3
    uint16      Supersedes;                                 // 4
    uint16      RewardQuestID;                              // 5
    uint8       OrderIndex;                                 // 6
    uint8       Flags;                                      // 7
    uint8       RelatedStep;                                // 8

    bool IsBonusObjective() const { return Flags & SCENARIO_STEP_FLAG_BONUS_OBJECTIVE; }
};

struct ScalingStatDistributionEntry
{
    uint32      ID;
    uint16      ItemLevelCurveID;                           // 0
    uint8       MinLevel;                                   // 1
    uint8       MaxLevel;                                   // 2
};

struct TotemCategoryEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0
    uint32      TotemCategoryType;                          // 1
    uint8       TotemCategoryMask;                          // 2
};

struct ItemToBattlePetSpeciesEntry
{
    uint32      ID;                                         // 0
    uint32      BattlePetSpeciesID;                         // 1
};

struct TransportAnimationEntry
{
    uint32      ID;
    uint32      TransportID;                                // 0
    uint32      TimeIndex;                                  // 1
    DBCPosition3D Pos;                                      // 2 - 4
    uint8       SequenceID;                                 // 5
};

struct UnitPowerBarEntry
{
    uint32      ID;
    uint32      MaxPower;                                   // 0
    float       RegenerationPeace;                          // 1
    float       RegenerationCombat;                         // 2
    uint32      FileDataID[6];                              // 3 - 8
    uint32      Color[6];                                   // 9 - 14
    LocalizedString* Name;                                  // 15
    LocalizedString* Cost;                                  // 16
    LocalizedString* OutOfError;                            // 17
    LocalizedString* ToolTip;                               // 18
    float       StartInset;                                 // 19
    float       EndInset;                                   // 20
    uint16      StartPower;                                 // 21
    uint16      Flags;                                      // 22
    uint8       MinPower;                                   // 23
    uint8       CenterPower;                                // 24
    uint8       BarType;                                    // 25
};

struct WorldMapOverlayEntry
{
    uint32      ID;
    LocalizedString* TextureName;                           // 0
    uint16      MapAreaID;                                  // 1
    uint16      AreaID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];     // 2 - 5
    uint16      TextureWidth;                               // 6
    uint16      TextureHeight;                              // 7
    uint16      OffsetX;                                    // 8
    uint16      OffsetY;                                    // 9
    uint16      HitRectTop;                                 // 10
    uint16      HitRectLeft;                                // 11
    uint16      HitRectBottom;                              // 12
    uint16      HitRectRight;                               // 13
    uint16      PlayerConditionID;                          // 14
};

struct WorldMapTransformsEntry
{
    uint32      ID;
    DBCPosition3D RegionMin;                                // 0 - 2
    DBCPosition3D RegionMax;                                // 3 - 5
    DBCPosition2D RegionOffset;                             // 6 - 7
    float       RegionScale;                                // 8
    uint16      MapID;                                      // 9
    uint16      NewMapID;                                   // 10
    uint16      NewDungeonMapID;                            // 11
    uint16      NewAreaID;                                  // 12
    uint8       Flags;                                      // 13
};

struct ChatChannelsEntry
{
    uint32      ID;
    uint32      Flags;                                      // 0
    LocalizedString* Name;                                  // 1
    LocalizedString* Shortcut;                              // 2
    uint8       FactionGroup;                               // 3
};

struct ChrSpecializationEntry
{
    uint32      ID;                                         // 0
    uint32      MasterySpellID[MAX_MASTERY_SPELLS];         // 1 - 2
    uint32      Unknown2;                                   // 3
    uint32      Unknown3;                                   // 4
    LocalizedString* Name;                                  // 5
    LocalizedString* Name2;                                 // 6
    LocalizedString* Description;                           // 7
    LocalizedString* BackgroundFile;                        // 8
    uint16      SpellIconID;                                // 9
    uint8       ClassID;                                    // 10
    uint8       OrderIndex;                                 // 11
    uint8       PetTalentType;                              // 12
    uint8       Role;                                       // 13
    uint8       PrimaryStatOrder2;                          // 14
};

struct EmotesEntry
{
    uint32      ID;
    LocalizedString* EmoteSlashCommand;                     // 0
    uint32      EmoteFlags;                                 // 1
    uint32      SpellVisualKitID;                           // 2
    uint32      UnkLegion;                                  // 3
    uint32      UnkLegion2;                                 // 4
    uint16      AnimID;                                     // 5
    uint16      EmoteSoundID;                               // 6
    uint8       EmoteSpecProc;                              // 7
    uint8       EmoteSpecProcParam;                         // 8
};

struct GemPropertiesEntry
{
    uint32      ID;
    uint32      UnkLegion;                                  // 0
    uint16      Type;                                       // 1
    uint16      EnchantID;                                  // 2
};

struct GlyphPropertiesEntry
{
    uint32      ID;                                         // 0
    uint32      SpellId;                                    // 1
    uint32      TypeFlags;                                  // 2
    uint32      SpellIconID;                                // 3
    uint32      GlyphExclusiveCategoryID;                   // 4
};

struct ItemSetEntry
{
    uint32      ID;
    LocalizedString* Name;                                  // 0ItemSet
    uint32      ItemID[MAX_ITEM_SET_ITEMS];                 // 1 - 16
    uint32      UnkLegion;                                  // 17
    uint16      RequiredSkill;                              // 18
    uint8       RequiredSkillValue;                         // 19
};

struct LockEntry
{
    uint32      ID;                                         // 0
    uint32      Index[MAX_LOCK_CASE];                       // 1-8
    uint16      Skill[MAX_LOCK_CASE];                       // 9-16
    uint8       Type[MAX_LOCK_CASE];                        // 17-24
    uint8       Action[MAX_LOCK_CASE];                      // 25-32
};

struct MovieEntry
{
    uint32      ID;
    uint32      unk1;                                       // 0
    uint32      unk2;                                       // 1
    uint8       unk3;                                       // 2
    uint8       unk4;                                       // 3
};

struct PowerDisplayEntry
{
    uint32      ID;                                         // 0  
    LocalizedString* GlobalStringBaseTag;                   // 2  
    uint8       PowerType;                                  // 1
    uint8       Red;                                        // 3
    uint8       Green;                                      // 4
    uint8       Blue;                                       // 5
};

struct PvPDifficultyEntry
{
    uint32      ID;                                         // 0
    uint16      MapID;                                      // 1
    uint8       BracketID;                                  // 2
    uint8       MinLevel;                                   // 3
    uint8       MaxLevel;                                   // 4

    BattlegroundBracketId GetBracketId() const { return BattlegroundBracketId(BracketID); }
};

struct SummonPropertiesEntry
{
    uint32      ID;                                         // 0
    uint32      Category;                                   // 1
    uint32      Faction;                                    // 2
    uint32      Type;                                       // 3
    int32       Slot;                                       // 4
    uint32      Flags;                                      // 5
};

struct VehicleSeatEntry
{
    uint32      ID;
    uint32      Flags;                                      // 0
    uint32      FlagsB;                                     // 1
    uint32      FlagsC;                                     // 2
    DBCPosition3D AttachmentOffset;                         // 3 - 5
    float       EnterPreDelay;                              // 6
    float       EnterSpeed;                                 // 7
    float       EnterGravity;                               // 8
    float       EnterMinDuration;                           // 9
    float       EnterMaxDuration;                           // 10
    float       EnterMinArcHeight;                          // 11
    float       EnterMaxArcHeight;                          // 12
    float       ExitPreDelay;                               // 13
    float       ExitSpeed;                                  // 14
    float       ExitGravity;                                // 15
    float       ExitMinDuration;                            // 16
    float       ExitMaxDuration;                            // 17
    float       ExitMinArcHeight;                           // 18
    float       ExitMaxArcHeight;                           // 19
    float       PassengerYaw;                               // 20
    float       PassengerPitch;                             // 21
    float       PassengerRoll;                              // 22
    float       VehicleEnterAnimDelay;                      // 23
    float       VehicleExitAnimDelay;                       // 24
    float       CameraEnteringDelay;                        // 25
    float       CameraEnteringDuration;                     // 26
    float       CameraExitingDelay;                         // 27
    float       CameraExitingDuration;                      // 28
    DBCPosition3D CameraOffset;                             // 23 - 31
    float       CameraPosChaseRate;                         // 32
    float       CameraFacingChaseRate;                      // 33
    float       CameraEnteringZoom;                         // 34
    float       CameraSeatZoomMin;                          // 35
    float       CameraSeatZoomMax;                          // 36
    int16       UISkinFileDataID;                           // 37
    int16       EnterAnimStart;                             // 38
    int16       EnterAnimLoop;                              // 39
    int16       RideAnimStart;                              // 40
    int16       RideAnimLoop;                               // 41
    int16       RideUpperAnimStart;                         // 42
    int16       RideUpperAnimLoop;                          // 43
    int16       ExitAnimStart;                              // 44
    int16       ExitAnimLoop;                               // 45
    int16       ExitAnimEnd;                                // 46
    int16       VehicleEnterAnim;                           // 47
    int16       VehicleExitAnim;                            // 48
    int16       VehicleRideAnimLoop;                        // 49
    int16       EnterUISoundID;                             // 50
    int16       ExitUISoundID;                              // 51
    int16       EnterAnimKitID;                             // 52
    int16       RideAnimKitID;                              // 53
    int16       ExitAnimKitID;                              // 54
    int16       VehicleEnterAnimBone;                       // 55
    int16       VehicleExitAnimBone;                        // 56
    int16       VehicleRideAnimLoopBone;                    // 57
    int16       CameraModeID;                               // 58
    int8        AttachmentID;                               // 59
    int8        PassengerAttachmentID;                      // 60
    int8        VehicleAbilityDisplay;                      // 61
    int8        VehicleEnterAnimKitID;                      // 62
    int8        VehicleRideAnimKitID;                       // 63
    int8        VehicleExitAnimKitID;                       // 64

    bool CanEnterOrExit() const { return Flags & VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT; }
    bool CanSwitchFromSeat() const { return Flags & VEHICLE_SEAT_FLAG_CAN_SWITCH; }
    bool IsUsableByOverride() const { return (Flags & VEHICLE_SEAT_FLAG_UNCONTROLLED)
                                    || (FlagsB & (VEHICLE_SEAT_FLAG_B_USABLE_FORCED | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2 |
                                        VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4)); }
    bool IsEjectable() const { return FlagsB & VEHICLE_SEAT_FLAG_B_EJECTABLE; }
};

#pragma pack(pop)

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) { }

    uint32    ID;
    uint32    price;
};

typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

typedef std::vector<TaxiPathNodeEntry const*> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 226
typedef std::array<uint8, TaxiMaskSize> TaxiMask;

#endif
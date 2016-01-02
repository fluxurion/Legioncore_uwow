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
#include "Define.h"
#include "Path.h"

#define MAX_CREATURE_SPELL_DATA_SLOT 4
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

#pragma pack(push, 1)

struct AchievementEntry
{
    uint32      ID;                                         // 0
    LocalizedString* Name;                                  // 1
    LocalizedString* Description;                           // 2
    uint32      Flags;                                      // 3
    LocalizedString* Reward;                                // 4
    uint16      MapID;                                      // 5
    uint16      ParentAchievement;                          // 6    
    uint16      CategoryID;                                 // 7
    uint16      OrderInCategory;                            // 8
    uint16      IconID;                                     // 9
    uint16      RefAchievement;                             // 10
    uint16      CriteriaTree;                               // 11
    uint8       FactionFlag;                                // 12
    uint32      RewardPoints;                               // 13
    uint8       Amount;                                     // 14
};

struct AreaGroupEntry
{
    uint32      ID;                                                 // 0
};

struct AreaGroupMemberEntry
{
    uint32      ID;
    uint16      AreaGroupID;                                // 0
    uint16      AreaID;                                     // 1
};

struct AuctionHouseEntry
{
    LocalizedString* Name;                                  // 0
    uint32      FactionID;                                  // 1
    uint32      DepositRate;                                // 2
    uint32      ConsignmentRate;                            // 3

    uint32      houseId; // temp - for compile
};

struct ArmorLocationEntry
{
    float       Value[5];                                           // 0 - 4 multiplier for armor types: cloth...plate
};

struct BankBagSlotPricesEntry
{
    uint32      price;
};

struct BarberShopStyleEntry
{
    uint32      Type;                                               // 0
    LocalizedString* Name;                                          // 1        
    LocalizedString* Description;                                   // 2        
    float       CostMultiplier;                                     // 3        
    uint8       Race;                                               // 4
    uint8       Gender;                                             // 5
    uint8       HairID;                                             // 6
    uint8       Tato;                                               // 7
};

struct BroadcastTextEntry
{
    uint32 ID;
    int32 Language;
    LocalizedString* MaleText;
    LocalizedString* FemaleText;
    uint32 EmoteID[MAX_BROADCAST_TEXT_EMOTES];
    uint32 EmoteDelay[MAX_BROADCAST_TEXT_EMOTES];
    uint32 SoundID;
    uint32 UnkEmoteID;
    uint32 Type;
};

struct CurvePointEntry
{
    uint32 ID;                                                      // 0
    uint32 CurveID;                                                 // 1
    uint32 Index;                                                   // 2
    float X;                                                        // 3
    float Y;                                                        // 4
};

struct CriteriaEntry
{
    union //int32 Asset;
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE                  = 0
        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE             = 20
        uint32 CreatureID;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                         = 1
        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND          = 15
        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP                   = 16
        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA                      = 32
        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA                     = 33
        // ACHIEVEMENT_CRITERIA_TYPE_INSTANSE_MAP_ID                = 71
        uint32 MapID;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL              = 7
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL              = 40
        uint32 SkillID;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT           = 8
        uint32 AchievementID;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE        = 11
        uint32 ZoneID;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY                       = 12
        uint32 CurrencyID;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON               = 18
        uint32 ManLimit;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID                  = 19
        uint32 RaidSize;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM                    = 26
        uint32 EnviromentalDamageType;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST                 = 27
        uint32 QuestID;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET                = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2               = 69
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL                     = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2                    = 110
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL                    = 34
        uint32 SpellID;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE
        uint32 ObjectiveID;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA         = 31
        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA                   = 43
        uint32 AreaID;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM                       = 36
        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM                       = 41
        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM                      = 42
        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM                     = 57
        uint32 ItemID;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING            = 38
        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING        = 39
        uint32 TeamType;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK                       = 44
        uint32 rank; // TODO: This rank is _NOT_ the index from CharTitles.dbc

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION                = 46
        uint32 FactionID;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM                = 49
        uint32 ItemSlot;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT             = 51
        uint32 RollValue;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS                       = 52
        uint32 ClassID;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                        = 53
        uint32 RaceID;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE                       = 54
        uint32 EmoteID;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT                 = 68
        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT             = 72
        uint32 GoEntry;

        // ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT                   = 73
        // ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT_2                 = 92
        uint32 ScriptDataVal;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS         = 75
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE               = 112
        uint32 SkillLine;

        // ACHIEVEMENT_CRITERIA_TYPE_ADD_BATTLE_PET_JOURNAL         = 96
        uint32 AddPet;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE                      = 109
        uint32 LootType;

        // ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_LEVEL_UP             = 160
        uint32 LevelUP;

        // ACHIEVEMENT_CRITERIA_TYPE_PLACE_GARRISON_BUILDING        = 167
        // ACHIEVEMENT_CRITERIA_TYPE_UPGRADE_GARRISON_BUILDING      = 168
        // ACHIEVEMENT_CRITERIA_TYPE_CONSTRUCT_GARRISON_BUILDING    = 169
        uint32 GarrBuildingID;

        // ACHIEVEMENT_CRITERIA_TYPE_UPGRADE_GARRISON               = 170
        uint32 GarrisonLevel;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_MISSION      = 174
        uint32 GarrMissionID;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_SHIPMENT     = 182
        uint32 CharShipmentContainerID;
    } Asset;
    uint32      StartAsset;                                 // 1
    uint32      FailAsset;                                  // 2
    uint16      StartTimer;                                 // 3
    uint16      ModifyTree;                                 // 4
    uint16      Flags;                                      // 5 
    uint8       Type;                                       // 6
    uint8       StartEvent;                                 // 7
    uint8       FailEvent;                                  // 8
    uint8       EligibilityWorldStateID;                    // 9
    uint8       EligibilityWorldStateValue;                 // 10

    uint32 ID; // temp - for compile
};

struct CriteriaTreeEntry
{
    uint32      Amount;                                     // 0
    LocalizedString* DescriptionLang;                       // 1
    uint16      CriteriaID;                                 // 2
    uint16      Parent;                                     // 3
    uint16      flags2;                                     // 4 Operator/OrderIndex
    uint16      UNK;                                        // 5 Operator/OrderIndex
    uint8       Flags;                                      // 6

    uint32      ID; // temp - for compile
};

struct CharacterLoadoutItemEntry
{
    uint32      LoadOutID;
    uint16      ItemID;
};

struct CharStartOutfitEntry
{
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
    uint8        ClassID;                                    // 0
    uint8        PowerID;                                    // 1
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
};

struct CreatureDisplayInfoExtraEntry
{
    uint32      NPCItemDisplay[10];                         // 0-10
    uint32      FileDataID;                                 // 11
    uint32      Unk;                                        // 12
    uint8       DisplayRaceID;                              // 13
    uint8       DisplaySexID;                               // 14
    uint8       SkinID;                                     // 15
    uint8       FaceID;                                     // 16
    uint8       HairStyleID;                                // 17
    uint8       HairColorID;                                // 18
    uint8       FacialHairID;                               // 19
    uint8       Tato[3];                                    // 20 @TODO:Legion check name
    uint8       Flags;                                      // 21
};

struct CreatureDisplayInfoEntry
{
    uint32      ID;                                         // 0
    uint32      ModelID;                                    // 1
    uint32      SoundID;                                    // 2
    uint32      ExtendedDisplayInfoID;                      // 3
    float       CreatureModelScale;                         // 4
    uint32      CreatureModelAlpha;                         // 5
    LocalizedString* TextureVariation[3];                   // 6-8
    LocalizedString* PortraitTextureName;                   // 9
    uint32      PortraitCreatureDisplayInfoID;              // 10
    uint32      SizeClass;                                  // 11
    uint32      BloodID;                                    // 12
    uint32      NPCSoundID;                                 // 13
    uint32      ParticleColorID;                            // 14
    uint32      CreatureGeosetData;                         // 15
    uint32      ObjectEffectPackageID;                      // 16
    uint32      AnimReplacementSetID;                       // 17
    uint32      Flags;                                      // 18
    int32       Gender;                                     // 19
    uint32      StateSpellVisualKitID;                      // 20
};

struct CreatureTypeEntry
{
    LocalizedString* Name;                                  // 0
    uint8       Flags;                                      // 1
};

struct DestructibleModelDataEntry
{
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
    uint16      WeaponSubstructCost[21];                    // 0-20
    uint16      ArmorSubstructCost[8];                      // 21-29
};

struct DurabilityQualityEntry
{
    float       Data;                                       // 0
};

struct GameObjectDisplayInfoEntry
{
    uint32      FileDataID;                             // 0
    DBCPosition3D GeoBoxMin;                            // 1-3
    DBCPosition3D GeoBoxMax;                            // 4-6
    float       OverrideLootEffectScale;                // 7
    float       OverrideNameScale;                      // 8
    uint16      Sound[10];                              // 9-18
    int16       ObjectEffectPackageID;                  // 19
};

struct ImportPriceArmorEntry
{
    float       ClothModifier;                          // 0
    float       LeatherModifier;                        // 1
    float       ChainModifier;                          // 2
    float       PlateModifier;                          // 3
};

struct ImportPriceQualityEntry
{
    float       Data;                                   // 0
};

struct ImportPriceShieldEntry
{
    float       Data;                                   // 0
};

struct ImportPriceWeaponEntry
{
    float       Data;                                   // 0
};

struct ItemArmorQualityEntry
{
    float       QualityMod[7];                          // 0 - 6
    uint16      ItemLevel;                              // 7
};

struct ItemArmorShieldEntry
{
    float       QualityMod[7];                          // 0 - 6
    uint16      ItemLevel;                              // 7
};

struct ItemArmorTotalEntry
{
    float       Value[4];                               // 0 - 3
    uint32      ItemLevel;                              // 4
};

struct ItemDamageEntry
{
    float       Quality[7];                             // 0 - 6
    uint32      ItemLevel;                              // 7
};

struct ItemClassEntry
{
    uint32      Class;                                  // 0
    float       PriceFactor;                            // 1
    LocalizedString* Name;                              // 2
};

struct ItemDisenchantLootEntry
{
    uint16      MinLevel;                               // 0
    uint16      MaxLevel;                               // 1
    uint16      SkillRequired;                          // 2
    uint16      ItemClass;                              // 3
    int16       ItemSubClass;                           // 4
    uint16      Quality;                                // 5

    uint32 Id; // temp - for compile
};

struct ItemLimitCategoryEntry
{
    LocalizedString* NameLang;                          // 0
    uint8       Quantity;                               // 1
    uint8       Flags;                                  // 2
};

struct ItemPriceBaseEntry
{
    float       Armor;                                  // 0
    float       Weapon;                                 // 1
    uint16      ItemLevel;                              // 2
};

struct ItemRandomPropertiesEntry
{
    LocalizedString* Name;                              // 0
    uint32      Enchantment[MAX_ITEM_ENCHANTS];         // 1 - 5

    uint32 ID; // temp - for compile
};

struct ItemRandomSuffixEntry
{
    LocalizedString* Name;                              // 0
    LocalizedString* IternalName;                       // 1
    uint32      Enchantment[MAX_ITEM_ENCHANTS];         // 2 - 6
    uint32      AllocationPct[MAX_ITEM_ENCHANTS];       // 7 - 11

    uint32 ID; // temp - for compile
};

struct ItemSpecOverrideEntry
{
    uint32      ItemID;                                 // 0
    uint16      SpecID;                                 // 1
};

struct ItemSpecEntry
{
    uint16      SpecID;                                 // 0
    uint8       MinLevel;                               // 1
    uint8       MaxLevel;                               // 2
    uint8       ItemType;                               // 3
    uint8       PrimaryStat;                            // 4
    uint8       SecondaryStat;                          // 5
};

struct ItemSetSpellEntry
{
    uint32      SpellID;                                // 0
    uint16      ItemSetID;                              // 1
    uint16      ChrSpecID;                              // 2
    uint8       Threshold;                              // 3
};

struct GuildPerkSpellsEntry
{
    uint32      SpellId;
    uint8       Level;
};

struct HeirloomEntry
{
    uint32 ID;                                                      // 0
    uint32 ItemID;                                                  // 1
    uint32 Flags;                                                   // 2
    LocalizedString* SourceText;                                    // 3
    uint32 Source;                                                  // 4
    uint32 OldItem[2];                                              // 5-6
    uint32 NextDifficultyItemID;                                    // 7
    uint32 UpgradeItemID[2];                                        // 8-9
    uint32 ItemBonusListID[2];                                      // 10-11
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
    uint32      ID;                                         // 0 (reference to ItemModifiedAppearance.db2?)
    uint32      DisplayID;                                  // 1
    uint32      FileDataID;                                 // 2
};

struct MailTemplateEntry
{
    LocalizedString* Content;                               // 0
};

struct ModifierTreeEntry
{
    uint32      Asset;                                      // 0
    uint32      SecondaryAsset;                             // 1
    uint16      Parent;                                     // 2
    uint8       Type;                                       // 3
    uint8       UnkLegion;                                  // 4
    uint8       Operator;                                   // 5
    uint8       Amount;                                     // 6

    uint32 ID; // temp - for compile
};

struct MountCapabilityEntry
{
    uint32      Unknown0;
    uint32      ReqSpellKnownID;
    uint32      ModSpellAuraID;
    uint16      ReqRidingSkill;
    uint16      ReqAreaID;
    uint16      ReqMapID;
    uint8       Flags;
    uint8       Unknown7;
};

struct MountTypeEntry
{
    uint32      MountTypeID;
};

struct MountTypeXCapabilityEntry
{
    uint16      MountTypeID;                                // 0
    uint16      MountCapabilityID;                          // 1
    uint8       OrderIndex;                                 // 2
};

struct NameGenEntry
{
    LocalizedString* Name;
    uint32      RaceID;
    uint32      Gender;
};

struct QuestFactionRewEntry
{
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
    LocalizedString* SortName;                              // 0
};

struct QuestV2Entry
{
    uint16      UniqueBitFlag;                              // 0
};

struct QuestXPEntry
{
    uint16      Difficulty[10];
};

struct RandPropPointsEntry
{
    uint16      EpicPropertiesPoints[5];                    // 0 - 4
    uint16      RarePropertiesPoints[5];                    // 5 - 9
    uint16      UncommonPropertiesPoints[5];                // 10 - 14
};

struct ResearchBranchEntry
{
    LocalizedString* Name;                                  // 0
    LocalizedString* Texture;                               // 1
    uint32      ItemID;                                     // 2
    uint16      CurrencyID;                                 // 3
    uint8       ResearchFieldID;                            // 4

    uint32 ID; // temp - for compile
};

struct ResearchProjectEntry
{
    LocalizedString* Name;                                  // 1
    LocalizedString* Description;                           // 2
    uint32      rare;                                       // 3
    uint32      branchId;                                   // 4
    uint32      SpellID;                                    // 5
    uint32      Complexity;                                 // 6
    uint32      RequiredCurrencyAmount;                     // 8

    uint32 ID; // temp - for compile

    bool IsVaid() const
    {
        return branchId != ARCHAEOLOGY_BRANCH_UNUSED &&
            branchId != ARCHAEOLOGY_BRANCH_NONE;
    }
};

struct ResearchSiteEntry
{
    uint32    ID;                                           // 0 ID
    uint32    MapID;                                        // 1 MapID
    uint32    POIid;                                        // 2
    LocalizedString* areaName;                              // 3 Research site name
    //uint32  flags;                                        // 4 Always 177.

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
    uint32      BonusListID;                                        // 1
    uint32      Type;                                               // 2
    int32       Value[2];                                           // 3-4
    uint32      Index;                                              // 5
};

struct ItemBonusTreeNodeEntry
{
    uint32 ID;                                                      // 0
    uint32 BonusTreeID;                                             // 1
    uint32 BonusTreeModID;                                          // 2
    uint32 SubTreeID;                                               // 3
    uint32 BonusListID;                                             // 4
};

struct ItemCurrencyCostEntry
{
    uint32      ID;                                                 // 0
    uint32      ItemId;                                             // 1
};

struct ItemEffectEntry
{
    uint32      ID;                                         // 0
    uint32      ItemID;                                     // 1
    uint32      OrderIndex;                                 // 2
    uint32      SpellID;                                    // 3
    uint32      Trigger;                                    // 4
    uint32      Charges;                                    // 5
    int32       Cooldown;                                   // 6
    uint32      Category;                                   // 7
    int32       CategoryCooldown;                           // 8
};

struct ItemModifiedAppearanceEntry
{
    uint32      ID;                                                 // 0
    uint32      ItemID;                                             // 1
    uint32      AppearanceModID;                                    // 2
    uint32      AppearanceID;                                       // 3
    uint32      IconFileDataID;                                     // 4
    uint32      Index;                                              // 5
};

struct ItemSparseEntry
{
    uint32      ID;                                                 // 0
    uint32      Quality;                                            // 1
    uint32      Flags[MAX_ITEM_PROTO_FLAGS];                        // 2-4
    float       Unk1;                                               // 5
    float       Unk2;                                               // 6
    uint32      BuyCount;                                           // 7
    uint32      BuyPrice;                                           // 8
    uint32      SellPrice;                                          // 9
    uint32      InventoryType;                                      // 10
    int32       AllowableClass;                                     // 11
    int32       AllowableRace;                                      // 12
    uint32      ItemLevel;                                          // 13
    int32       RequiredLevel;                                      // 14
    uint32      RequiredSkill;                                      // 15
    uint32      RequiredSkillRank;                                  // 16
    uint32      RequiredSpell;                                      // 17
    uint32      RequiredHonorRank;                                  // 18
    uint32      RequiredCityRank;                                   // 19
    uint32      RequiredReputationFaction;                          // 20
    uint32      RequiredReputationRank;                             // 21
    uint32      MaxCount;                                           // 22
    uint32      Stackable;                                          // 23
    uint32      ContainerSlots;                                     // 24
    int32       ItemStatType[MAX_ITEM_PROTO_STATS];                 // 25 - 34
    int32       ItemStatValue[MAX_ITEM_PROTO_STATS];                // 35 - 44
    int32       ItemStatAllocation[MAX_ITEM_PROTO_STATS];           // 45 - 54
    float       ItemStatSocketCostMultiplier[MAX_ITEM_PROTO_STATS]; // 55 - 64
    uint32      ScalingStatDistribution;                            // 65
    uint32      DamageType;                                         // 66
    uint32      Delay;                                              // 67
    float       RangedModRange;                                     // 68
    uint32      Bonding;                                            // 69
    LocalizedString* Name;                                          // 70
    LocalizedString* Name2;                                         // 71
    LocalizedString* Name3;                                         // 72
    LocalizedString* Name4;                                         // 73
    LocalizedString* Description;                                   // 74
    uint32      PageText;                                           // 75
    uint32      LanguageID;                                         // 76
    uint32      PageMaterial;                                       // 77
    uint32      StartQuest;                                         // 78
    uint32      LockID;                                             // 79
    int32       Material;                                           // 80
    uint32      Sheath;                                             // 81
    uint32      RandomProperty;                                     // 82
    uint32      RandomSuffix;                                       // 83
    uint32      ItemSet;                                            // 84
    uint32      Area;                                               // 85
    uint32      Map;                                                // 86
    uint32      BagFamily;                                          // 87
    uint32      TotemCategory;                                      // 88
    uint32      SocketColor[MAX_ITEM_PROTO_SOCKETS];                // 89-91
    uint32      SocketBonus;                                        // 92
    uint32      GemProperties;                                      // 93
    float       ArmorDamageModifier;                                // 94
    uint32      Duration;                                           // 95
    uint32      ItemLimitCategory;                                  // 96
    uint32      HolidayID;                                          // 97
    float       StatScalingFactor;                                  // 98
    uint32      CurrencySubstitutionID;                             // 99
    uint32      CurrencySubstitutionCount;                          // 100
    uint32      ItemNameDescriptionID;                              // 101
};

struct ItemExtendedCostEntry
{
    uint32      ID;                                                 // 0 extended-cost entry id
    uint32      RequiredArenaSlot;                                  // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];              // 3-6 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS];         // 7-11 required count of 1st item
    uint32      RequiredPersonalArenaRating;                        // 12 required personal arena rating
    uint32      ItemPurchaseGroup;                                  // 13
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];     // 14-18 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];// 19-23 required curency count
    uint32      RequiredFactionId;                                  // 24
    uint32      RequiredFactionStanding;                            // 25
    uint32      RequirementFlags;                                   // 26
    uint32      RequiredAchievement;                                // 27
    uint32      RequiredMoney;                                      // 28

    bool IsSeasonCurrencyRequirement(uint32 i) const
    {
        if (i > MAX_ITEM_EXT_COST_CURRENCIES)
            return 0;

        // start from ITEM_EXTENDED_COST_FLAG_SEASON_IN_INDEX_0
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
    uint32 unk;               // 1
    LocalizedString* stateName;            // 2
    uint32 flags;               // 3
};

struct BattlePetSpeciesEntry
{
    uint32 ID;                  // 0
    uint32 CreatureEntry;       // 1
    uint32 IconFileID;          // 2
    uint32 spellId;             // 3
    uint32 petType;             // 4
    int32  source;              // 5
    uint32 flags;               // 6
    LocalizedString* SourceText; // 7
    LocalizedString* Description; // 8
};

struct BattlePetSpeciesStateEntry
{
    uint32 ID;                                                      // 0
    uint32 SpeciesID;                                               // 1
    uint32 State;                                                   // 2
    int32 Value;                                                    // 3
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 ID;
    uint32 speciesID;
    uint32 abilityID;
    uint32 requiredLevel;
    uint32 rank;
};

struct BattlePetBreedQualityEntry
{
    uint32 ID;
    uint32 quality;
    float qualityModifier;
};

struct BattlePetBreedStateEntry
{
    uint32 ID;
    uint32 breedID;
    uint32 stateID;
    int32 stateModifier;
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
    uint32      FactionID;
    uint32      Expansion;
    uint32      MinLevel;
    uint32      MaxLevel;
    uint32      Flags[5];
};

struct CurrencyTypesEntry
{
    uint32      ID;                                                 // 0
    uint32      CategoryID;                                         // 1
    LocalizedString*       Name_lang;                                          // 2
    LocalizedString*       InventoryIcon[2];                                   // 3-4
    uint32      SpellWeight;                                        // 5
    uint32      SpellCategory;                                      // 6
    uint32      MaxQty;                                             // 7
    uint32      MaxEarnablePerWeek;                                 // 8
    uint32      Flags;                                              // 9
    uint32      Quality;                                            // 10
    LocalizedString*       Description_lang;                                   // 11

    bool HasPrecision() const   { return Flags & CURRENCY_FLAG_HAS_PRECISION; }
    bool HasSeasonCount() const { return Flags & CURRENCY_FLAG_HAS_SEASON_COUNT; }
    float GetPrecision() const  { return HasPrecision() ? CURRENCY_PRECISION : 1.0f; }
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
    uint32 Id;
    uint32 MountTypeId;
    uint32 DisplayId;
    uint32 Flags;
    LocalizedString* Name;
    LocalizedString* Description;
    LocalizedString* SourceDescription;
    uint32 Source;
    uint32 SpellId;
    uint32 PlayerConditionId;
};

struct LanguageWordsEntry
{
    uint32 Id;                                            // 0
    uint32 langId;                                          // 1
    LocalizedString* word;                                             // 2
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
    uint32 Flags;                                                   // 11
    uint32 PlayerActionbarFileDataID;                               // 12
};

struct PhaseGroupEntry
{
    uint32      ID;
    uint32      PhaseID;
    uint32      PhaseGroupID;
};

struct SoundEntriesEntry
{
    uint32      ID;                                                 // 0
    uint32      SoundType;                                          // 1
    LocalizedString* Name;                                          // 2
    uint32      FileDataID[20];                                     // 3-22
    uint32      Freq[20];                                           // 23-42
    float       VolumeFloat;                                        // 43
    uint32      Flags;                                              // 44
    float       MinDistance;                                        // 45
    float       DistanceCutoff;                                     // 46
    uint32      EAXDef;                                             // 47
    uint32      SoundEntriesAdvancedID;                             // 48
    float       VolumeVariationPlus;                                // 49
    float       VolumeVariationMinus;                               // 50
    float       PitchVariationPlus;                                 // 51
    float       PitchVariationMinus;                                // 52
    float       PitchAdjust;                                        // 53
    uint32      DialogType;                                         // 54
    uint32      BusOverwriteID;                                     // 55
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

struct SpellPowerEntry
{
    uint32      ID;
    uint32      SpellID;                                    // 0
    uint32      PowerCost;                                  // 1
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

struct SpellRuneCostEntry
{
    uint32      ID;                                         // 0
    uint32      RuneCost[4];                                // 1-4 (0=blood, 1=frost, 2=unholy)
    uint32      RunePowerGain;                              // 5

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return RunePowerGain == 0; }
};

struct ToyEntry
{
    uint32 ID;                                                      // 0
    uint32 ItemID;                                                  // 1
    uint32 Flags;                                                   // 2
    LocalizedString* Description;                                   // 3
    uint32 CategoryFilter;                                          // 4
};

struct PvpItemEntry
{
    uint32      ID;
    uint32      ItemID;                                     // 0
    uint8       BonusIlvl;                                  // 1
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
    uint32          MapID;                                  // 1
    DBCPosition3D   Pos;                                    // 2-4
    LocalizedString* Name_lang;                             // 5
    uint32          MountCreatureID[MAX_TEAMS];           // 6-7
    uint32          ConditionID;                            // 8
    uint32          LearnableIndex;                         // 9 - some kind of index only for learnable nodes
    uint32          Flags;                                  // 10
    float           MapOffset[MAX_TEAMS];                 // 11-12
};

struct TaxiPathEntry
{
    uint32          ID;                                     // 0
    uint32          From;                                   // 1
    uint32          To;                                     // 2
    uint32          Cost;                                   // 3
};

struct TaxiPathNodeEntry
{
    uint32          ID;                                     // 0
    uint32          PathID;                                 // 1
    uint32          NodeIndex;                              // 2
    uint32          MapID;                                  // 3
    DBCPosition3D   Loc;                                    // 4-6
    uint32          Flags;                                  // 7
    uint32          Delay;                                  // 8
    uint32          ArrivalEventID;                         // 9
    uint32          DepartureEventID;                       // 10
};

struct ItemUpgradeEntry
{
    uint32 id;                  // 0 rules id from RuleSetItemUpgradeEntry startUpgrade
    uint32 itemUpgradePathId;   // 1 extended-cost entry id
    uint32 levelBonus;          // 2 total level bonus related to non-upgraded item
    uint32 prevUpgradeId;       // 3
    uint32 currencyReqId;       // 4 currency Id
    uint32 currencyReqAmt;      // 5 currency count
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
    uint32 ID;                                                      // 0
    uint32 MapID;                                                   // 1
    uint32 DisplayID;                                               // 2
    DBCPosition3D Position;                                         // 3-5
    float RotationX;                                                // 6
    float RotationY;                                                // 7
    float RotationZ;                                                // 8
    float RotationW;                                                // 9
    float Size;                                                     // 10
    uint32 PhaseUseFlags;                                           // 11
    uint32 PhaseID;                                                 // 12
    uint32 PhaseGroupID;                                            // 13
    uint32 Type;                                                    // 14
    uint32 Data[GO_DBC_DATA_COUNT];                                 // 15-22
    LocalizedString* Name;                                          // 23
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
    ItemUpgradeData()
    {
        memset(upgrade, 0, sizeof(upgrade));
    }

    ItemUpgradeEntry const* upgrade[MAX_ITEM_UPDGRADES];
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
    uint32      BonusTreeID;                                // 2
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
    uint32      DescriptionVariablesID;                     // 6

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
    float       ResourceCoefficient;                        // 2
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
    uint8       Flags;                                      // 2
    uint8       UsesPerWeek;                                // 3
    uint8       MaxCharges;                                 // 4
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
    LocalizedString* Name;                                  // 0
    uint8       Flags;                                      // 1
    uint8       UnkLegion;                                  // 2

    bool IsChallenge() const { return Flags & SCENARIO_FLAG_CHALLENGE; }
    bool IsProvingGrounds() const { return Flags & SCENARIO_FLAG_SUPRESS_STAGE_TEXT; }
};

struct ScenarioStepEntry
{
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
    uint16      ItemLevelCurveID;                           // 0
    uint8       MinLevel;                                   // 1
    uint8       MaxLevel;                                   // 2
};

struct TotemCategoryEntry
{
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
    uint32      TransportID;                                // 0
    uint32      TimeIndex;                                  // 1
    DBCPosition3D Pos;                                      // 2 - 4
    uint8       SequenceID;                                 // 5
};

struct TransportRotationEntry
{
    uint32      TransportID;                                // 0
    uint32      TimeIndex;                                  // 1
    DBCPosition4D Rot;                                      // 2 - 5
};

struct UnitPowerBarEntry
{
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

#pragma pack(pop)

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0)
    { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price)
    { }

    uint32    ID;
    uint32    price;
};

typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

typedef std::vector<TaxiPathNodeEntry const*> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 217
typedef std::array<uint8, TaxiMaskSize> TaxiMask;

#endif
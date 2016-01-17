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
#include "Define.h"
#include "Path.h"
#include "Util.h"

#include <map>
#include <set>
#include <vector>

#pragma pack(push, 1)

#define MAX_MAP_DEPTH -5000 // Temporary define until max depth is found somewhere (adt?)
#define MAX_MASTERY_SPELLS 2
#define MAX_PERKS_COUNT 4
#define MAX_FACTION_RELATIONS 4
#define MAX_ITEM_ENCHANTMENT_EFFECTS 3
#define MAX_ITEM_SET_ITEMS 17
#define MAX_ITEM_SET_SPELLS 8
#define MAX_LOCK_CASE 8
#define MAX_MOUNT_CAPABILITIES 24
#define MAX_SPELL_EFFECTS_DIFF 32
#define MAX_EFFECT_MASK 4294967295
#define MAX_POWERS_FOR_SPELL 4
#define MAX_SHAPESHIFT_SPELLS 8
#define MAX_VEHICLE_SEATS 8

struct AreaTableEntry
{
    uint32      ID;                                         // 0
    uint32      mapid;                                      // 1
    uint32      ParentAreaID;                               // 2 if 0 then it's zone, else it's zone id of this area
    uint32      AreaBit;                                    // 3, main index
    uint32      Flags[2];                                   // 4-5,
    //uint32    SoundProviderPref;                          // 6,
    //uint32    SoundProviderPrefUnderwater;                // 7,
    //uint32    AmbienceID;                                 // 8,
    //uint32    ZoneMusic;                                  // 9,
    char*       ZoneName;                                   // 10
    //uint32    IntroSound;                                 // 11
    uint32      ExplorationLevel;                           // 12
    char*       AreaName_lang;                               // 13
    uint32      FactionGroupMask;                           // 14
    uint32      LiquidTypeID[4];                            // 15-18
    //float     AmbientMultiplier;                          // 19
    uint32    MountFlags;                                   // 20
    //uint32    UWIntroMusic;                               // 21
    //uint32    UWZoneMusic;                                // 22
    //uint32    WorldPvPID;                                 // 24 World_PVP_Area.dbc
    //uint32    PvPCombastWorldStateID;                     // 25
    uint32    m_wildBattlePetLevelMin;                      // 26
    uint32    m_wildBattlePetLevelMax;                      // 27
    //uint32    WindSettingsID;                             // 28

    // helpers
    bool IsSanctuary() const
    {
        if (mapid == 609)
            return true;
        return (Flags[0] & AREA_FLAG_SANCTUARY) != 0;
    }
};

struct AreaPOIEntry
{
    uint32 id;              //0
    uint32 icon[11];        //1-11
    float x;                //12
    float y;                //13
    uint32 mapId;           //14  m_continentID
    //uint32 m_flags;       //15  m_flags
    uint32 zoneId;          //16  m_areaID
    //char* name;           //17 - name
    //char* name2;          //18 - name2
    uint32 worldState;      //19
    //uint32 m_playerConditionID;//20
    //uint32 m_worldMapLink //21
    //uint32 m_portLocID;   //22
};

struct AreaTriggerEntry
{
    uint32  id;                                             // 0        m_ID
    uint32  mapid;                                          // 1        m_ContinentID
    float   x;                                              // 2        m_x
    float   y;                                              // 3        m_y
    float   z;                                              // 4        m_z
    //uint32  m_phaseUseFlags                               // 5        m_phaseUseFlags
    //uint32  m_phaseID                                     // 6        m_phaseID
    //uint32  m_phaseGroupID                                // 7        m_phaseGroupID
    float   radius;                                         // 8        m_radius
    float   box_x;                                          // 9        m_box_length
    float   box_y;                                          // 10       m_box_width
    float   box_z;                                          // 11       m_box_heigh
    float   box_orientation;                                // 12       m_box_yaw
    //uint32 m_shapeType                                    // 13       m_shapeType
    //uint32 m_shapeID                                      // 14       m_shapeID
    //uint32 m_areaTriggerActionSetID                       // 15       m_areaTriggerActionSetID
    //uint32 m_flags                                        // 16       m_flags
    //uint32 unk                                                        on 6.0.3 19243
};

struct BannedAddOnsEntry
{
    uint32      ID;                                         // 0
    //uint32    NameMD5[4];                                 // 1
    //uint32    VersionMD5[4];                              // 2
    //uint32    LastModified;                               // 3
    //uint32    Flags;                                      // 4
};

struct BattlemasterListEntry
{
    uint32      ID;
    int32       MapID[16];
    uint32      InstanceType;
    //uint32      GroupsAllowed;
    char*       NameLang;
    uint32      MaxGroupSize;
    uint32      HolidayWorldState;
    uint32      Minlevel;
    uint32      Maxlevel;
    uint32      RatedPlayers;
    uint32      MinPlayers;
    uint32      MaxPlayers;
    uint32      Flags;
    //uint32      IconFileDataID
    //char*       GametypeLang
    //uint32      CritreriaID;
};

struct CharTitlesEntry
{
    uint32      ID;                                         // 0, title ids, for example in Quest::GetRewTitle()
    //uint32    ConditionID;                                // 1
    char*       NameMale_lang;                              // 2 m_name_lang
    char*       NameFemale_lang;                            // 3 m_name1_lang
    uint32      MaskID;                                     // 4 m_mask_ID used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
    //uint32    Flags;                                      // 5
};

struct ChrClassesEntry
{
    uint32      ID;                                         // 0
    uint32      PowerType;                                  // 1
    //char*     PetNameToken                                // 2
    char*       Name_lang;                                  // 3
    //char*     NameFemale_lang;                            // 4
    //char*     NameMale_lang;                              // 5
    //char*     Filename;                                   // 6
    uint32      SpellClassSet;                              // 7
    //uint32    Flags;                                      // 8
    uint32      CinematicSequenceID;                        // 9
    uint32      AttackPowerPerStrength;                     // 10 Attack Power bonus per point of strength
    uint32      AttackPowerPerAgility;                      // 11 Attack Power bonus per point of agility
    uint32      RangedAttackPowerPerAgility;                // 12 Ranged Attack Power bonus per point of agility
    uint32      DefaultSpec;                                  // 13
    //uint32    CreateScreenFileDataID;                     // 14
    //uint32    SelectScreenFileDataID;                     // 15
    //uint32    LowResScreenFileDataID;                     // 16
    //uint32    IconFileDataID;                             // 17
    //uint32    Unk1;                                       // 18
};

struct CinematicSequencesEntry
{
    uint32      Id;                                         // 0 index
    //uint32      unk1;                                     // 1 always 0
    //uint32      cinematicCamera;                          // 2 id in CinematicCamera.dbc
                                                            // 3-9 always 0
};

struct CreatureFamilyEntry
{
    uint32  ID;                                             // 0        m_ID
    float   minScale;                                       // 1        m_minScale
    uint32  minScaleLevel;                                  // 2        m_minScaleLevel
    float   maxScale;                                       // 3        m_maxScale
    uint32  maxScaleLevel;                                  // 4        m_maxScaleLevel
    uint32  skillLine[2];                                   // 5-6      m_skillLine
    uint32  petFoodMask;                                    // 7        m_petFoodMask
    int32   petTalentType;                                  // 8        m_petTalentType
                                                            // 9        m_categoryEnumID
    char* Name;                                             // 10       m_name_lang
                                                            // 11       m_iconFile
};

struct CreatureModelDataEntry
{
    uint32 Id;
    //uint32 Flags;
    //char* ModelPath
    //uint32 Unk1;
    //float Scale;                                             // Used in calculation of unit collision data
    //int32 Unk2
    //int32 Unk3
    //uint32 Unk4
    //uint32 Unk5
    //float Unk6
    //uint32 Unk7
    //float Unk8
    //uint32 Unk9
    //uint32 Unk10
    //float CollisionWidth;
    float CollisionHeight;
    float MountHeight;                                       // Used in calculation of unit collision data when mounted
    //float Unks[13]
};

struct DifficultyEntry
{
    uint32      m_ID;                                       // 0
    uint32      FallbackDifficultyID;                       // 1
    uint32      InstanceType;                               // 2
    uint32      m_minPlayers;                               // 3
    uint32      m_maxPlayers;                               // 4
    //int32     OldEnumValue;                               // 5
    uint32      Flags;                                      // 6
    uint32      ToggleDifficultyID;                         // 7
    //uint32    GroupSizeHealthCurveID;                     // 8
    //uint32    GroupSizeDmgCurveID;                        // 9
    //uint32    GroupSizeSpellPointsCurveID;                // 10
    //char const* NameLang;                                 // 11
    uint32    ItemBonusTreeModID;                           // 12
    //uint32 unkLegion
};

struct DungeonEncounterEntry
{
    uint32 id;                                              // 0        unique id
    uint32 mapId;                                           // 1        map id
    uint32 difficulty;                                      // 2        instance mode
    //uint32 orderIndex;                                    // 3
    uint32 encounterIndex;                                  // 4        encounter index for creating completed mask
    char* encounterName;                                    // 5        encounter name
    uint32 creatureDisplayID;                               // 6
    uint32 spellIconID;                                     // 7
    //uint32 flags;                                         // 8
};

struct EmotesTextEntry
{
    uint32  Id;
    uint32  textid;
};

struct FactionEntry
{
    uint32      ID;                                         // 0        m_ID
    int32       reputationListID;                           // 1        m_reputationIndex
    uint32      BaseRepRaceMask[4];                         // 2-5      m_reputationRaceMask
    uint32      BaseRepClassMask[4];                        // 6-9      m_reputationClassMask
    int32       BaseRepValue[4];                            // 10-13    m_reputationBase
    uint32      ReputationFlags[4];                         // 14-17    m_reputationFlags
    uint32      team;                                       // 18       m_parentFactionID
    float       spilloverRateIn;                            // 19       Faction gains incoming rep * spilloverRateIn
    float       spilloverRateOut;                           // 20       Faction outputs rep * spilloverRateOut as spillover reputation
    uint32      spilloverMaxRankIn;                         // 21       The highest rank the faction will profit from incoming spillover
    //uint32    spilloverRank_unk;                          // 22       It does not seem to be the max standing at which a faction outputs spillover ...so no idea
    char* name;                                             // 23       m_name_lang
    //char*     description;                                // 24       m_description_lang
    //uint32    m_expansion                                 // 25
    uint32      m_flags;                                    // 26       5.4.1
    //uint32    m_friendshipRepID                           // 27       5.4.1

    // helpers
    bool CanHaveReputation() const
    {
        return reputationListID >= 0;
    }

    bool CanBeLfgBonus() const { return m_flags & 0x1; }
};

struct FactionTemplateEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      faction;                                    // 1        m_faction
    uint32      factionFlags;                               // 2        m_flags
    uint32      ourMask;                                    // 3        m_factionGroup
    uint32      friendlyMask;                               // 4        m_friendGroup
    uint32      hostileMask;                                // 5        m_enemyGroup
    uint32      enemyFaction[MAX_FACTION_RELATIONS];        // 6        m_enemies[MAX_FACTION_RELATIONS]
    uint32      friendFaction[MAX_FACTION_RELATIONS];       // 10       m_friend[MAX_FACTION_RELATIONS]
    //-------------------------------------------------------  end structure

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return true;
        if (entry.faction)
        {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (enemyFaction[i] == entry.faction)
                    return false;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (friendFaction[i] == entry.faction)
                    return true;
        }
        return (friendlyMask & entry.ourMask) || (ourMask & entry.friendlyMask);
    }
    bool IsHostileTo(FactionTemplateEntry const& entry) const
    {
        if (ID == entry.ID)
            return false;
        if (entry.faction)
        {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (enemyFaction[i] == entry.faction)
                    return true;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (friendFaction[i] == entry.faction)
                    return false;
        }
        return (hostileMask & entry.ourMask) != 0;
    }
    bool IsHostileToPlayers() const { return (hostileMask & FACTION_MASK_PLAYER) !=0; }
    bool IsNeutralToAll() const
    {
        for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
            if (enemyFaction[i] != 0)
                return false;
        return hostileMask == 0 && friendlyMask == 0;
    }
    bool IsContestedGuardFaction() const { return (factionFlags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

struct GlyphSlotEntry
{
    uint32  Id;
    uint32  TypeFlags;
    uint32  Order;
};

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
    float    base;
};

struct GtChanceToSpellCritEntry
{
    float    ratio;
};

struct GtNpcTotalHpEntry
{
    float    HP;
};

struct GtNpcDamageByClassEntry
{
    float    Value;
};

struct GtArmorMitigationByLvlEntry
{
    float    Armor;
};

struct GtOCTLevelExperienceEntry
{
    float    Data;
};

struct GtOCTRegenHPEntry
{
    float    ratio;
};

struct GtOCTHpPerStaminaEntry
{
    float    ratio;
};

struct GtRegenHPPerSptEntry
{
    float    ratio;
};

struct GtRegenMPPerSptEntry
{
    float    ratio;
};

struct GtSpellScalingEntry
{
    float value;
};

struct GtOCTBaseHPByClassEntry
{
    float ratio;
};

struct GtOCTBaseMPByClassEntry
{
    float ratio;
};

struct GtBattlePetTypeDamageModEntry
{
    uint32 Id;
    float value;
};

struct GtItemSocketCostPerLevelEntry
{
    float    ratio;
};

struct GtNPCManaCostScalerEntry
{
    float    ratio;
};

struct LFGDungeonEntry
{
    uint32  ID;                                             // 0
    char*   name;                                           // 1
    uint32  minlevel;                                       // 2
    uint32  maxlevel;                                       // 3
    uint32  reclevel;                                       // 4
    uint32  recminlevel;                                    // 5
    uint32  recmaxlevel;                                    // 6
    int32   map;                                            // 7
    uint32  difficulty;                                     // 8
    uint32  flags;                                          // 9
    uint32  type;                                           // 10
    int32   faction;                                        // 11
    //char*   textureFileName;                              // 12 Name lite
    uint32  expansion;                                      // 13
    //uint32 orderIndex;                                    // 14
    uint32 groupId;                                         // 15
    //char* descr                                           // 16 Description
    uint32 random_id;                                       // 17 random id
    uint32 tankNeeded;                                      // 18
    uint32 healerNeeded;                                    // 19
    uint32 dpsNeeded;                                       // 20
    uint32 minTankNeeded;                                   // 21
    uint32 minHealerNeeded;                                 // 22
    uint32 minDpsNeeded;                                    // 23   5.4.1
    uint32 scenarioId;                                      // 24   5.4.1
    uint32 subType;                                         // 25   5.4.1
    uint32 bonusRepAmt;                                     // 26   5.4.1
    //uint32 mentorCharLevel;                               // 27   5.4.1
    //uint32 mentorItemLevel;                               // 28   5.4.1

    // Helpers
    uint32 Entry() const { return ID + (type << 24); }
    bool IsScenario() const { return subType == LFG_SUBTYPE_SCENARIO; }
    bool IsChallenge() const { return difficulty == DIFFICULTY_CHALLENGE; }
    bool IsRaidFinder() const { return difficulty == DIFFICULTY_LFR; }
    bool IsFlex() const { return difficulty == FLEXIBLE_DIFFICULTY; }
    uint32 GetMinGroupSize() const { return minTankNeeded + minHealerNeeded + minDpsNeeded; }
    uint32 GetMaxGroupSize() const { return tankNeeded + healerNeeded + dpsNeeded; }
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
        switch (subType)
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

    bool CanBeRewarded() const
    {
        return type == LFG_TYPE_RANDOM || IsRaidFinder() || IsChallenge() || flags & LFG_FLAG_SEASONAL;
    }

    bool FitsTeam(uint32 team) const
    {
        if (faction == -1)
            return true;
        else if (faction == 0)
            return team == HORDE;
        else
            return team == ALLIANCE;
    }
};


struct LiquidTypeEntry
{
    uint32 Id;
    //char*  Name;
    //uint32 Flags;
    uint32 Type;
    //uint32 SoundId;
    uint32 SpellId;
    //float     MaxDarkenDepth;                             // 6
    //float     FogDarkenIntensity;                         // 7
    //float     AmbDarkenIntensity;                         // 8
    //float     DirDarkenIntensity;                         // 9
    //uint32    LightID;                                    // 10
    //float     ParticleScale;                              // 11
    //uint32    ParticleMovement;                           // 12
    //uint32    ParticleTexSlots;                           // 13
    //uint32    MaterialID;                                 // 14
    //char*     Texture[6];                                 // 15-20
    //uint32    Color[2];                                   // 21-23
    //float     Unk1[18];                                   // 24-41
    //uint32    Unk2[4];                                    // 42-45
    //uint32    Unk3[5];                                    // 46-50
};

struct PhaseEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
};

struct MapEntry
{
    uint32  MapID;                                          // 0
    //char*       internalname;                             // 1 unused
    uint32  InstanceType;                                   // 2
    uint32 flags;                                           // 3
    //uint32        MapType;                                // 4
    //uint32 unk4;                                          // 5 4.0.1
    char* name;                                             // 6        m_MapName_lang
    uint32  linked_zone;                                    // 7        m_areaTableID
    //char*     hordeIntro;                                 // 8        m_MapDescription0_lang
    //char*     allianceIntro;                              // 9        m_MapDescription1_lang
    uint32  multimap_id;                                    // 10       m_LoadingScreenID (LoadingScreens.dbc)
    //float   BattlefieldMapIconScale;                      // 11       m_minimapIconScale
    int32   entrance_map;                                   // 12       m_corpseMapID map_id of entrance map in ghost mode (continent always and in most cases = normal entrance)
    float   entrance_x;                                     // 13       m_corpseX entrance x coordinate in ghost mode  (in most cases = normal entrance)
    float   entrance_y;                                     // 14       m_corpseY entrance y coordinate in ghost mode  (in most cases = normal entrance)
    //uint32  timeOfDayOverride;                            // 15       m_timeOfDayOverride
    uint32  addon;                                          // 16       m_expansionID
    uint32 unk_time;                                        // 17       m_raidOffset
    uint32 maxPlayers;                                      // 18       m_maxPlayers
    int32   ParentMapID;                                    // 19
    //uint32        CosmeticParentMapID                     // 20
    //uint32        TimeOffset                              // 21
    // Helpers
    uint32 Expansion() const { return addon; }

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
        if (entrance_map < 0)
            return false;
        mapid = entrance_map;
        x = entrance_x;
        y = entrance_y;
        return true;
    }

    bool IsContinent() const
    {
        return MapID == 0 || MapID == 1 || MapID == 530 || MapID == 571 || MapID == 870 || MapID == 1116;
    }

    bool IsDynamicDifficultyMap() const { return (flags & MAP_FLAG_CAN_CHANGE_DIFFICULTY) != 0; }
    bool IsGarrison() const { return (flags & MAP_FLAG_GARRISON) != 0; }

    bool IsDifficultyModeSupported(uint32 difficulty) const;
};

struct MinorTalentEntry
{
    uint32      ID;                                         // 0
    uint32      SpecID;                                     // 1
    uint32      SpellID;                                    // 2
    uint32      OrderIndex;                                 // 3
};

struct MapDifficultyEntry
{
    //uint32    ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      DifficultyID;                               // 2 (for arenas: arena slot)
    char*       Message_lang;                               // 3 m_message_lang (text showed when transfer to map failed)
    uint32      RaidDuration;                               // 4 m_raidDuration in secs, 0 if no fixed reset time
    uint32      MaxPlayers;                                 // 5 m_maxPlayers some heroic versions have 0 when expected same amount as in normal version
    uint32      LockID;                                     // 6
    uint32      ItemBonusTreeModID;                         // 7
};

struct QuestPOIBlobEntry
{
    uint32    ID;                                           // 0 ID
    uint32    Count;                                        // 1 m_NumPoints
    uint32    MapID;                                        // 2 m_MapID
    //uint32 unk;                                           // 4 m_WorldMapAreaID
};

struct SpellItemEnchantmentEntry
{
    uint32      ID;                                             // 0
    uint32      Charges;                                        // 1
    uint32      Effect[MAX_ITEM_ENCHANTMENT_EFFECTS];           // 2-4
    uint32      EffectPointsMin[MAX_ITEM_ENCHANTMENT_EFFECTS];  // 5-7
    uint32      EffectSpellID[MAX_ITEM_ENCHANTMENT_EFFECTS];    // 8-10
    //char*     Name_lang                                       // 11
    uint32      ItemVisual;                                     // 12
    uint32      Flags;                                          // 13
    uint32      SRCItemID;                                      // 14
    uint32      ConditionID;                                    // 15
    uint32      RequiredSkillID;                                // 16
    uint32      RequiredSkillRank;                              // 17
    uint32      MinLevel;                                       // 18
    uint32      MaxLevel;                                       // 19
    uint32      ItemLevel;                                      // 20
    int32       ScalingClass;                                   // 21
    int32       ScalingClassRestricted;                         // 22
    float       EffectScalingPoints[MAX_ITEM_ENCHANTMENT_EFFECTS];//23-25
};

struct StableSlotPricesEntry
{
    uint32 Slot;
    uint32 Price;
};

struct TalentEntry
{
    uint32  Id;                                     // 0
    uint32  SpecID;                                 // 1
    uint32  row;                                    // 2
    uint32  column;                                 // 3
    uint32  spellId;                                // 4
    //uint32  m_flags;                              // 5
    //uint32  m_categoryMask1;                      // 6
    //uint32  m_categoryMask2;                      // 7
    uint32  classId;                                // 8
    uint32  OverridesSpellID;                       // 9
    char*   description;                            // 10

};

struct TeamContributionPointsEntry
{
    //uint32    entry;                                      // 0
    float     value;                                        // 1 (???)
};

struct VehicleEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FlagsB;                                     // 2
    float       TurnSpeed;                                  // 3
    float       PitchSpeed;                                 // 4
    float       PitchMin;                                   // 5
    float       PitchMax;                                   // 6
    uint32      SeatID[MAX_VEHICLE_SEATS];                  // 7-14
    float       MouseLookOffsetPitch;                       // 15
    float       CameraFadeDistScalarMin;                    // 16
    float       CameraFadeDistScalarMax;                    // 17
    float       CameraPitchOffset;                          // 18
    float       FacingLimitRight;                           // 19
    float       FacingLimitLeft;                            // 20
    float       MsslTrgtTurnLingering;                      // 21
    float       MsslTrgtPitchLingering;                     // 22
    float       MsslTrgtMouseLingering;                     // 23
    float       MsslTrgtEndOpacity;                         // 24
    float       MsslTrgtArcSpeed;                           // 25
    float       MsslTrgtArcRepeat;                          // 26
    float       MsslTrgtArcWidth;                           // 27
    float       MsslTrgtImpactRadius[2];                    // 28-29
    //char*     MsslTrgtArcTexture;                         // 30
    //char*     MsslTrgtImpactTexture;                      // 31
    //char*     MsslTrgtImpactModel;                        // 32-33
    float       CameraYawOffset;                            // 34
    uint32      UILocomotionType;                           // 35
    float       MsslTrgtImpactTexRadius;                    // 36
    uint32      VehicleUIIndicatorID;                       // 37
    uint32      PowerDisplayID[3];                          // 38-40
};

struct WMOAreaTableEntry
{
    uint32      ID;                                         // 0 index
    int32       WMOID;                                      // 1 used in root WMO
    int32       NameSet;                                    // 2 used in adt file
    int32       WMOGroupID;                                 // 3 used in group WMO
    //uint32    SoundProviderPref;                          // 4
    //uint32    SoundProviderPrefUnderwater;                // 5
    //uint32    AmbienceID;                                 // 6
    //uint32    ZoneMusic;                                  // 7
    //uint32    IntroSound;                                 // 8
    uint32      Flags;                                      // 9 used for indoor/outdoor determination
    uint32      AreaTableID;                                // 10 link to AreaTableEntry.ID
    char*       AreaName_lang;                              // 11       m_AreaName_lang
    //uint32    UWIntroSound;                               // 12
    //uint32    UWZoneMusic;                                // 13
    //uint32    UWAmbience;                                 // 14
};

struct WorldMapAreaEntry
{
    //uint32    ID;                                         // 0
    uint32      MapID;                                      // 1
    uint32      AreaID;                                     // 2 index (continent 0 areas ignored)
    //char*     AreaName                                    // 3
    float       LocLeft;                                    // 4
    float       LocRight;                                   // 5
    float       LocTop;                                     // 6
    float       LocBottom;                                  // 7
    int32       DisplayMapID;                               // 8 -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    //int32     DefaultDungeonFloor;                        // 9 pointer to DungeonMap.dbc (owerride loc coordinates)
    //uint32    ParentWorldMapID;                           // 10
    //uint32    Flags;                                      // 11
    uint32    LevelRangeMin;                              // 12 Minimum recommended level displayed on world map
    uint32    LevelRangeMax;                              // 13 Maximum recommended level displayed on world map
};

struct WorldSafeLocsEntry
{
    uint32          ID;                                     // 0
    uint32          MapID;                                  // 1
    DBCPosition4D   Loc;                                    // 2 - 5
    //char*         AreaName_lang;                          // 6
};

#pragma pack(pop)

struct TalentSpellPos
{
    TalentSpellPos() : talent_id(0), rank(0) {}
    TalentSpellPos(uint16 _talent_id, uint8 _rank) : talent_id(_talent_id), rank(_rank) {}

    uint16 talent_id;
    uint8  rank;
};

typedef std::unordered_map<uint32, TalentSpellPos> TalentSpellPosMap;

typedef std::set<uint32> PetFamilySpellsSet;
typedef std::unordered_map<uint32, PetFamilySpellsSet > PetFamilySpellsStore;

float GetCurrencyPrecision(uint32 currencyId);

// artifact point
struct ResearchPOIPoint
{
    ResearchPOIPoint() : x(0), y(0) { }
    ResearchPOIPoint(int32 _x, int32 _y) : x(_x), y(_y) { }

    int32 x;
    int32 y;
};

struct DigSitePosition
{
    DigSitePosition() : x(0.0f), y(0.0f) { }
    DigSitePosition(float _x, float _y) : x(_x), y(_y) { }

    float x;
    float y;
};

typedef std::vector<ResearchPOIPoint> ResearchPOIPointVector;
typedef std::vector<DigSitePosition> DigSitePositionVector;

#endif

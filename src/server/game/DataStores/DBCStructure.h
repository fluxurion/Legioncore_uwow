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

#define MAX_FACTION_RELATIONS 4
#define MAX_ITEM_ENCHANTMENT_EFFECTS 3
#define MAX_VEHICLE_SEATS 8

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

struct CreatureModelDataEntry
{
    uint32      ID;                                         // 0
    uint32      Flags;                                      // 1
    uint32      FileDataID;                                 // 2
    //uint32    SizeClass;                                  // 3
    //float     ModelScale;                                 // 4
    //uint32    BloodID;                                    // 5
    //uint32    FootprintTextureID;                         // 6
    //float     FootprintTextureLength;                     // 7
    //float     FootprintTextureWidth;                      // 8
    //float     FootprintParticleScale;                     // 9
    //uint32    FoleyMaterialID;                            // 10
    //uint32    FootstepShakeSize;                          // 11
    //uint32    DeathThudShakeSize;                         // 12
    //uint32    SoundID;                                    // 13
    //float     CollisionWidth;                             // 14
    float       CollisionHeight;                            // 15
    float       MountHeight;                                // 16
    //float     GeoBoxMin[3];                               // 17-19
    //float     GeoBoxMax[3];                               // 20-22
    //float     WorldEffectScale;                           // 23
    //float     AttachedEffectScale;                        // 24
    //float     MissileCollisionRadius;                     // 25
    //float     MissileCollisionPush;                       // 26
    //float     MissileCollisionRaise;                      // 27
    //float     OverrideLootEffectScale;                    // 28
    //float     OverrideNameScale;                          // 29
    //float     OverrideSelectionRadius;                    // 30
    //float     TamedPetBaseScale;                          // 31
    //uint32    CreatureGeosetDataID;                       // 32
    //float     HoverHeight;                                // 33
    // 34
    // 35
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

struct CharSectionsEntry
{
    uint32      ID;
    uint32      Race;
    uint32      Gender;
    uint32      GenType;
    char*       TexturePath[3];
    uint32      Flags;
    uint32      Type;
    uint32      Color;
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

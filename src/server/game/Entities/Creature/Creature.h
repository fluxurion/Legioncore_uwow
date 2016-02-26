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

#ifndef CREATURE_H
#define CREATURE_H

#include "Common.h"
#include "Unit.h"
#include "UpdateMask.h"
#include "ItemTemplate.h"
#include "LootMgr.h"
#include "DatabaseEnv.h"
#include "Cell.h"

class SpellInfo;

class CreatureAI;
class Quest;
class Player;
class WorldSession;
class CreatureGroup;

enum CreatureFlagsExtra
{
    CREATURE_FLAG_EXTRA_INSTANCE_BIND                   = 0x00000001,       // creature kill bind instance with killer and killer's group
    CREATURE_FLAG_EXTRA_CIVILIAN                        = 0x00000002,       // not aggro (ignore faction/reputation hostility)
    CREATURE_FLAG_EXTRA_NO_PARRY                        = 0x00000004,       // creature can't parry
    CREATURE_FLAG_EXTRA_NO_PARRY_HASTEN                 = 0x00000008,       // creature can't counter-attack at parry
    CREATURE_FLAG_EXTRA_NO_BLOCK                        = 0x00000010,       // creature can't block
    CREATURE_FLAG_EXTRA_NO_CRUSH                        = 0x00000020,       // creature can't do crush attacks
    CREATURE_FLAG_EXTRA_NO_XP_AT_KILL                   = 0x00000040,       // creature kill not provide XP
    CREATURE_FLAG_EXTRA_TRIGGER                         = 0x00000080,       // trigger creature
    CREATURE_FLAG_EXTRA_NO_TAUNT                        = 0x00000100,       // creature is immune to taunt auras and effect attack me
    CREATURE_FLAG_EXTRA_PERSONAL_LOOT                   = 0x00000200,       // Personal loot mobs and increment healths by player
    CREATURE_FLAG_EXTRA_AUTO_LOOT                       = 0x00000400,       // Do not die and send loot auto
    CREATURE_FLAG_EXTRA_WORLDEVENT                      = 0x00004000,       // custom flag for world event creatures (left room for merging)
    CREATURE_FLAG_EXTRA_GUARD                           = 0x00008000,       // Creature is guard
    CREATURE_FLAG_EXTRA_NO_CRIT                         = 0x00020000,       // creature can't do critical strikes
    CREATURE_FLAG_EXTRA_NO_SKILLGAIN                    = 0x00040000,       // creature won't increase weapon skills
    CREATURE_FLAG_EXTRA_TAUNT_DIMINISH                  = 0x00080000,       // Taunt is a subject to diminishing returns on this creautre
    CREATURE_FLAG_EXTRA_ALL_DIMINISH                    = 0x00100000,       // Creature is subject to all diminishing returns as player are
    CREATURE_FLAG_EXTRA_DUNGEON_BOSS                    = 0x10000000,       // creature is a dungeon boss (SET DYNAMICALLY, DO NOT ADD IN DB)
    CREATURE_FLAG_EXTRA_VEHICLE_ATTACKABLE_PASSENGERS   = 0x20000000,       // creature is vehicle, UNIT_STATE_ONVEHICLE will not add to passengers
    CREATURE_FLAG_EXTRA_HP_80_PERC                      = 0x80000000,       // No damage if HP < 80% for target mob
};

#define CREATURE_FLAG_EXTRA_DB_ALLOWED (CREATURE_FLAG_EXTRA_INSTANCE_BIND | CREATURE_FLAG_EXTRA_CIVILIAN | \
    CREATURE_FLAG_EXTRA_NO_PARRY | CREATURE_FLAG_EXTRA_NO_PARRY_HASTEN | CREATURE_FLAG_EXTRA_NO_BLOCK | \
    CREATURE_FLAG_EXTRA_NO_CRUSH | CREATURE_FLAG_EXTRA_NO_XP_AT_KILL | CREATURE_FLAG_EXTRA_TRIGGER | \
    CREATURE_FLAG_EXTRA_NO_TAUNT | CREATURE_FLAG_EXTRA_WORLDEVENT | CREATURE_FLAG_EXTRA_NO_CRIT | \
    CREATURE_FLAG_EXTRA_NO_SKILLGAIN | CREATURE_FLAG_EXTRA_TAUNT_DIMINISH | CREATURE_FLAG_EXTRA_ALL_DIMINISH | \
    CREATURE_FLAG_EXTRA_GUARD | CREATURE_FLAG_EXTRA_HP_80_PERC | CREATURE_FLAG_EXTRA_VEHICLE_ATTACKABLE_PASSENGERS | \
    CREATURE_FLAG_EXTRA_PERSONAL_LOOT | CREATURE_FLAG_EXTRA_AUTO_LOOT)

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

// Creature Pet entries
#define ENTRY_WATER_ELEMENTAL   510
#define ENTRY_FELGUARD          17252
#define ENTRY_GHOUL             26125
#define ENTRY_RUNE_WEAPON       27893

#define MAX_KILL_CREDIT 2
#define MAX_TYPE_FLAGS 2
#define MAX_CREATURE_MODELS 4
#define MAX_CREATURE_QUEST_ITEMS 6
#define MAX_CREATURE_NAMES 4
#define CREATURE_MAX_SPELLS 8

#define CREATURE_REGEN_INTERVAL 5 * IN_MILLISECONDS
#define PET_FOCUS_REGEN_INTERVAL 2 * IN_MILLISECONDS


#define MAX_EQUIPMENT_ITEMS 3

// from `creature_template` table
struct CreatureTemplate
{
    uint32 Entry;
    std::string Name;
    std::string NameAlt;
    std::string Title;
    std::string TitleAlt;
    std::string CursorName;
    uint32 KillCredit[MAX_KILL_CREDIT];
    uint32 Modelid[MAX_CREATURE_MODELS];
    uint32 QuestItem[MAX_CREATURE_QUEST_ITEMS];
    uint32 UnkInt;
    uint32 FlagQuest;
    uint32 VerifiedBuild;
    uint32 Classification;
    uint32 MovementInfoID;
    uint32 Family;                                         // enum CreatureFamily
    uint32  RequiredExpansion;
    uint32 TypeFlags[2];                                   // enum CreatureTypeFlags[0] mask values [1] unk for now
    uint32 Type;                                           // enum CreatureType values
    float HpMulti;
    float PowerMulti;
    bool Leader;
    
    std::string AIName;
    int32 resistance[MAX_SPELL_SCHOOL];
    uint32 AiID;
    uint32 attackpower;
    uint32 baseattacktime;
    uint32 dmgschool;
    uint32 dynamicflags;
    uint32 faction;
    uint32 flags_extra;
    uint32 GetFirstValidModelId() const;
    uint32 GetRandomValidModelId() const;
    uint32 GossipMenuId;
    uint32 InhabitType;
    uint32 lootid;
    uint32 maxgold;
    uint32 MechanicImmuneMask;
    uint32 MeleeID;
    uint32 mingold;
    uint32 MovementIDKit;
    uint32 MovementType;
    uint32 npcflag;
    uint32 npcflag2;
    uint32 personalloot;
    uint32 PetSpellDataId;
    uint32 pickpocketLootId;
    uint32 rangeattacktime;
    uint32 rangedattackpower;
    uint32 ScriptID;
    uint32 SkinLootId;
    uint32 spells[CREATURE_MAX_SPELLS];
    uint32 trainer_class;
    uint32 trainer_race;
    uint32 trainer_spell;
    uint32 trainer_type;
    uint32 unit_class;                                     // enum Classes. Note only 4 classes are known for creatures.
    uint32 unit_flags;                                     // enum UnitFlags mask values
    uint32 unit_flags2;                                    // enum UnitFlags2 mask values
    uint32 unit_flags3;                                    // enum UnitFlags3 mask values
    uint32 VehicleId;
    uint32 VignetteId;
    uint32 WorldEffectID;
    uint8 maxlevel;
    uint8 minlevel;
    float dmg_multiplier;
    float HoverHeight;
    float maxdmg;
    float maxrangedmg;
    float mindmg;
    float minrangedmg;
    float ModArmor;
    float ModManaExtra;                                   // Added in 4.x, this value is usually 2 for a small group of creatures with double mana
    float scale;
    float speed_fly;
    float speed_run;
    float speed_walk;
    bool RegenHealth;

    //Get difficulty from spawnmode
    static uint8 GetDiffFromSpawn(uint8 spawnmode)
    {
        switch (spawnmode)
        {
            case DIFFICULTY_NONE:
            case DIFFICULTY_NORMAL:
            case DIFFICULTY_10_N:
            case DIFFICULTY_40:
            case DIFFICULTY_N_SCENARIO:
            case DIFFICULTY_NORMAL_RAID:
                return 0;
            case DIFFICULTY_CHALLENGE: //ToDo move to 
            case DIFFICULTY_HEROIC:
            case DIFFICULTY_25_N:
            case DIFFICULTY_HC_SCENARIO:
            case DIFFICULTY_HEROIC_RAID:
                return 1;
            case DIFFICULTY_10_HC:
            //case DIFFICULTY_CHALLENGE:
            case DIFFICULTY_MYTHIC_RAID:
            case DIFFICULTY_MYTHIC_DUNGEON:
                return 2;
            case DIFFICULTY_25_HC:
                return 3;
            case DIFFICULTY_LFR:
                return 4;
            /*case FLEXIBLE_DIFFICULTY:
                return 5;*/
            case DIFFICULTY_LFR_NEW:
            case DIFFICULTY_EVENT_RAID:
            case DIFFICULTY_EVENT_DUNGEON:
            case DIFFICULTY_EVENT_SCENARIO:
            default:
                break;
        }

        return 0;
    }

    // helpers
    SkillType GetRequiredLootSkill() const
    {
        if (TypeFlags[0] & CREATURE_TYPEFLAGS_HERBLOOT)
            return SKILL_HERBALISM;
        else if (TypeFlags[0] & CREATURE_TYPEFLAGS_MININGLOOT)
            return SKILL_MINING;
        else if (TypeFlags[0] & CREATURE_TYPEFLAGS_ENGINEERLOOT)
            return SKILL_ENGINEERING;
        else
            return SKILL_SKINNING;                          // normal case
    }

    bool isTameable(bool exotic) const
    {
        if (Type != CREATURE_TYPE_BEAST || Family == 0 || (TypeFlags[0] & CREATURE_TYPEFLAGS_TAMEABLE) == 0)
            return false;

        // if can tame exotic then can tame any temable
        return exotic || (TypeFlags[0] & CREATURE_TYPEFLAGS_EXOTIC) == 0;
    }
};

// from `creature_difficulty_stat` table
struct CreatureDifficultyStat
{
    uint32  Entry;
    uint8   Difficulty;
    float   dmg_multiplier;
    float   ModHealth;
};

// Benchmarked: Faster than std::map (insert/find)
typedef std::unordered_map<ObjectGuid::LowType, CreatureTemplate> CreatureTemplateContainer;
typedef std::unordered_map<uint32, std::vector<CreatureDifficultyStat> > CreatureDifficultyStatContainer;

// Defines base stats for creatures (used to calculate HP/mana/armor).
struct CreatureBaseStats
{
    uint32 BaseHealth[MAX_EXPANSIONS];
    uint32 BaseMana;
    uint32 BaseArmor;
    uint32 AttackPower;
    uint32 RangedAttackPower;
    float BaseDamage[MAX_EXPANSIONS];

    // Helpers

    uint32 GenerateHealth(CreatureTemplate const* info, CreatureDifficultyStat const* diffStats = NULL) const
    {
        if(diffStats)
            return uint32((BaseHealth[info->RequiredExpansion] * diffStats->ModHealth) + 0.5f);
        else
            return uint32((BaseHealth[info->RequiredExpansion] * info->HpMulti) + 0.5f);
    }

    float GenerateBaseDamage(CreatureTemplate const* info) const
    {
        return BaseDamage[info->RequiredExpansion];
    }

    uint32 GenerateMana(CreatureTemplate const* info) const
    {
        // Mana can be 0.
        if (!BaseMana)
            return 0;

        return uint32((BaseMana * info->PowerMulti * info->ModManaExtra) + 0.5f);
    }

    uint32 GenerateArmor(CreatureTemplate const* info) const
    {
        return uint32((BaseArmor * info->ModArmor) + 0.5f);
    }

    static CreatureBaseStats const* GetBaseStats(uint8 level, uint8 unitClass);
};

typedef std::unordered_map<uint16, CreatureBaseStats> CreatureBaseStatsContainer;

struct CreatureLocale
{
    StringVector Name;
    StringVector NameAlt;
    StringVector Title;
    StringVector TitleAlt;
};

struct GossipMenuItemsLocale
{
    StringVector OptionText;
    StringVector BoxText;
};

struct PointOfInterestLocale
{
    StringVector IconName;
};

struct EquipmentInfo
{
    uint32  ItemEntry[MAX_EQUIPMENT_ITEMS];
};

// Benchmarked: Faster than std::map (insert/find)
typedef std::unordered_map<uint8, EquipmentInfo> EquipmentInfoContainerInternal;
typedef std::unordered_map<uint32, EquipmentInfoContainerInternal> EquipmentInfoContainer;

// from `creature` table
struct CreatureData
{
    explicit CreatureData() : dbData(true) {}
    ObjectGuid::LowType guid = 0;
    uint32 id;                                              // entry in creature_template
    uint16 mapid;
    uint16 zoneId = 0;
    uint16 areaId = 0;
    uint32 phaseMask = 1;
    uint32 displayid = 0;
    int8 equipmentId = 0;
    float posX;
    float posY;
    float posZ;
    float orientation;
    uint32 spawntimesecs = 0;
    float spawndist;
    uint32 currentwaypoint = 0;
    uint32 curhealth = 0;
    uint32 curmana = 0;
    uint8 movementType = 0;
    uint32 spawnMask = 1;
    uint32 npcflag = 0;
    uint32 npcflag2 = 0;
    uint32 unit_flags = 0;                                      // enum UnitFlags mask values
    uint32 dynamicflags = 0;
    bool isActive = false;
    bool dbData;
    bool building;                                              // garrison building state
    std::set<uint32> PhaseID;
};

// `creature_addon` table
struct CreatureAddon
{
    uint32 path_id;
    uint32 mount;
    uint32 bytes1;
    uint32 bytes2;
    uint32 emote;
    std::vector<uint32> auras;
};

// `creature_evade_wmo_data` table
struct CreatureEvadeWmoData
{
    uint32 entry;
    float distance;
    int32 wmoId;
    int32 wmoSet;
    int32 wmoGroupId;
};

typedef std::unordered_map<ObjectGuid::LowType, CreatureAddon> CreatureAddonContainer;
typedef std::unordered_map<uint32, CreatureAddon> CreatureTemplateAddonContainer;
typedef std::unordered_map<uint32, CreatureEvadeWmoData> CreatureEvadeWmoDataContainer;
 

struct CreatureModelInfo
{
    float bounding_radius;
    float combat_reach;
    int8 gender;
    uint32 displayId_other_gender;
    uint32 hostileId;
};

// Benchmarked: Faster than std::map (insert/find)
typedef std::unordered_map<uint16, CreatureModelInfo> CreatureModelContainer;

// `creature_ai_instance` table
struct CreatureAIInstance
{
    uint32 entry;
    uint32 bossid;
    uint32 bossidactivete;
};

typedef std::unordered_map<uint32, CreatureAIInstance> CreatureAIInstanceContainer;

// `creature_ai_instance_door` table

enum DoorType
{
    DOOR_TYPE_ROOM          = 0,    // Door can open if encounter is not in progress
    DOOR_TYPE_PASSAGE       = 1,    // Door can open if encounter is done
    DOOR_TYPE_SPAWN_HOLE    = 2,    // Door can open if encounter is in progress, typically used for spawning places
    MAX_DOOR_TYPES
};

struct DoorData
{
    uint32 entry, bossId;
    DoorType type;
    uint32 boundary;
};

struct DoorGoData
{
    uint32 entry, bossId, instanceId;
    DoorType type;
    uint32 boundary;
};

typedef std::map<uint32, std::vector<DoorData> > CreatureAIInstanceDoorContainer;
typedef std::map<uint32, std::vector<DoorGoData> > CreatureAIInstanceGoContainer;


enum BoundaryType
{
    BOUNDARY_NONE = 0,
    BOUNDARY_N,
    BOUNDARY_S,
    BOUNDARY_E,
    BOUNDARY_W,
    BOUNDARY_NE,
    BOUNDARY_NW,
    BOUNDARY_SE,
    BOUNDARY_SW,
    BOUNDARY_MAX_X = BOUNDARY_N,
    BOUNDARY_MIN_X = BOUNDARY_S,
    BOUNDARY_MAX_Y = BOUNDARY_W,
    BOUNDARY_MIN_Y = BOUNDARY_E
};

typedef std::map<BoundaryType, float> BossBoundaryMap;

enum InhabitTypeValues
{
    INHABIT_GROUND = 1,
    INHABIT_WATER  = 2,
    INHABIT_AIR    = 4,
    INHABIT_ANYWHERE = INHABIT_GROUND | INHABIT_WATER | INHABIT_AIR
};

// Enums used by StringTextData::Type (CreatureEventAI)
enum ChatType
{
    CHAT_TYPE_SAY               = 0,
    CHAT_TYPE_YELL              = 1,
    CHAT_TYPE_TEXT_EMOTE        = 2,
    CHAT_TYPE_BOSS_EMOTE        = 3,
    CHAT_TYPE_WHISPER           = 4,
    CHAT_TYPE_BOSS_WHISPER      = 5,
    CHAT_TYPE_ZONE_YELL         = 6,
    CHAT_TYPE_END               = 255
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

// Vendors
struct VendorItem
{
    VendorItem(uint32 _item, int32 _maxcount, uint32 _incrtime, uint32 _ExtendedCost, uint8 _Type)
        : item(_item), maxcount(_maxcount), incrtime(_incrtime), ExtendedCost(_ExtendedCost), Type(_Type) {}

    uint32 item;
    uint32 maxcount;                                        // 0 for infinity item amount
    uint32 incrtime;                                        // time for restore items amount if maxcount != 0
    uint32 ExtendedCost;
    uint8  Type;

    //helpers
    bool IsGoldRequired(ItemTemplate const* pProto) const { return pProto->Flags2 & ITEM_FLAGS_EXTRA_EXT_COST_REQUIRES_GOLD || !ExtendedCost || ExtendedCost > 4999; }
};
typedef std::vector<VendorItem*> VendorItemList;

struct VendorItemData
{
    VendorItemList m_items;

    VendorItem* GetItem(uint32 slot) const
    {
        if (slot >= m_items.size())
            return NULL;

        return m_items[slot];
    }
    bool Empty() const { return m_items.empty(); }
    uint32 GetItemCount() const { return m_items.size(); }
    void AddItem(uint32 item, int32 maxcount, uint32 ptime, uint32 ExtendedCost, uint8 type)
    {
        m_items.push_back(new VendorItem(item, maxcount, ptime, ExtendedCost, type));
    }
    bool RemoveItem(uint32 item_id, uint8 type);
    VendorItem const* FindItemCostPair(uint32 item_id, uint32 extendedCost, uint8 type) const;
    void Clear()
    {
        for (VendorItemList::const_iterator itr = m_items.begin(); itr != m_items.end(); ++itr)
            delete (*itr);
        m_items.clear();
    }
};

struct VendorItemCount
{
    explicit VendorItemCount(uint32 _item, uint32 _count)
        : itemId(_item), count(_count), lastIncrementTime(time(NULL)) {}

    uint32 itemId;
    uint32 count;
    time_t lastIncrementTime;
};

typedef std::list<VendorItemCount> VendorItemCounts;

#define MAX_TRAINERSPELL_ABILITY_REQS 3

struct TrainerSpell
{
    TrainerSpell() : spell(0), spellCost(0), reqSkill(0), reqSkillValue(0), reqLevel(0)
    {
        for (uint8 i = 0; i < MAX_TRAINERSPELL_ABILITY_REQS; ++i)
            learnedSpell[i] = 0;
    }

    uint32 spell;
    uint32 spellCost;
    uint32 reqSkill;
    uint32 reqSkillValue;
    uint32 reqLevel;
    uint32 learnedSpell[MAX_TRAINERSPELL_ABILITY_REQS];

    // helpers
    bool IsCastable() const { return learnedSpell[0] != spell; }
};

typedef std::unordered_map<uint32 /*spellid*/, TrainerSpell> TrainerSpellMap;

struct TrainerSpellData
{
    TrainerSpellData() : trainerType(0) {}
    ~TrainerSpellData() { spellList.clear(); }

    TrainerSpellMap spellList;
    uint32 trainerType;                                     // trainer type based at trainer spells, can be different from creature_template value.
                                                            // req. for correct show non-prof. trainers like weaponmaster, allowed values 0 and 2.
    TrainerSpell const* Find(uint32 spell_id) const;
};

typedef std::map<uint32, time_t> CreatureSpellCooldowns;

enum PetSpellState
{
    PETSPELL_UNCHANGED = 0,
    PETSPELL_CHANGED   = 1,
    PETSPELL_NEW       = 2,
    PETSPELL_REMOVED   = 3
};

enum PetSpellType
{
    PETSPELL_NORMAL = 0,
    PETSPELL_FAMILY = 1,
    PETSPELL_TALENT = 2,
};

enum PetType
{
    SUMMON_PET              = 0,
    HUNTER_PET              = 1,
    MAX_PET_TYPE            = 4,
};

struct PetSpell
{
    ActiveStates active;
    PetSpellState state;
    PetSpellType type;
};

typedef std::unordered_map<uint32, PetSpell> PetSpellMap;
typedef std::vector<uint32> AutoSpellList;

// max different by z coordinate for creature aggro reaction
#define CREATURE_Z_ATTACK_RANGE 3

#define MAX_VENDOR_ITEMS 150                                // Limitation in 4.x.x item count in SMSG_VENDOR_INVENTORY

enum CreatureCellMoveState
{
    CREATURE_CELL_MOVE_NONE, //not in move list
    CREATURE_CELL_MOVE_ACTIVE, //in move list
    CREATURE_CELL_MOVE_INACTIVE, //in move list but should not move
};

class MapCreature
{
    friend class Map; //map for moving creatures
    friend class ObjectGridLoader; //grid loader for loading creatures

protected:
    MapCreature() : _moveState(CREATURE_CELL_MOVE_NONE) {}

private:
    Cell _currentCell;
    Cell const& GetCurrentCell() const { return _currentCell; }
    void SetCurrentCell(Cell const& cell) { _currentCell = cell; }

    CreatureCellMoveState _moveState;
    Position _newPosition;
    void SetNewCellPosition(float x, float y, float z, float o)
    {
        _moveState = CREATURE_CELL_MOVE_ACTIVE;
        _newPosition.Relocate(x, y, z, o);
    }
};

class Creature : public Unit, public GridObject<Creature>, public MapCreature
{
    public:

        explicit Creature(bool isWorldObject = false);
        virtual ~Creature();

        void AddToWorld();
        void RemoveFromWorld();

        void DisappearAndDie();

        bool Create(ObjectGuid::LowType guidlow, Map* map, uint32 phaseMask, uint32 Entry, int32 vehId, uint32 team, float x, float y, float z, float ang, const CreatureData* data = NULL);
        bool LoadCreaturesAddon(bool reload = false);
        void SelectLevel(const CreatureTemplate* cinfo);
        void LoadEquipment(int8 id = 1, bool force=false);

        uint64 GetDBTableGUIDLow() const { return m_DBTableGuid; }

        void Update(uint32 time);                         // overwrited Unit::Update
        void GetRespawnPosition(float &x, float &y, float &z, float* ori = NULL, float* dist =NULL) const;

        void SetCorpseDelay(uint32 delay) { m_corpseDelay = delay; }
        uint32 GetCorpseDelay() const { return m_corpseDelay; }
        bool isRacialLeader() const { return GetCreatureTemplate()->Leader; }
        bool isCivilian() const { return (GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_CIVILIAN) != 0; }
        bool isTrigger() const { return (GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_TRIGGER) != 0; }
        bool isGuard() const { return (GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_GUARD) != 0; }
        bool CanWalk() const { return (GetCreatureTemplate()->InhabitType & INHABIT_GROUND) != 0; }
        bool CanSwim() const { return (GetCreatureTemplate()->InhabitType & INHABIT_WATER) != 0; }
        bool CanFly()  const { return (GetCreatureTemplate()->InhabitType & INHABIT_AIR) != 0; }

        void SetReactState(ReactStates st) { m_reactState = st; }
        ReactStates GetReactState() { return m_reactState; }
        bool HasReactState(ReactStates state) const { return (m_reactState == state); }
        void InitializeReactState()
        {
            if (isTotem() || isTrigger() || GetCreatureType() == CREATURE_TYPE_CRITTER || isSpiritService())
                SetReactState(REACT_PASSIVE);
            else
                SetReactState(REACT_AGGRESSIVE);
            /*else if (isCivilian())
            SetReactState(REACT_DEFENSIVE);*/;
        }

        ///// TODO RENAME THIS!!!!!
        bool isCanTrainingOf(Player* player, bool msg) const;
        bool isCanInteractWithBattleMaster(Player* player, bool msg) const;
        bool isCanTrainingAndResetTalentsOf(Player* player) const;
        bool canCreatureAttack(Unit const* victim, bool force = true) const;
        bool IsImmunedToSpell(SpellInfo const* spellInfo);
                                                            // redefine Unit::IsImmunedToSpell
        bool IsImmunedToSpellEffect(SpellInfo const* spellInfo, uint32 index) const;
                                                            // redefine Unit::IsImmunedToSpellEffect
        bool isElite() const
        {
            if (isPet())
                return false;

            uint32 rank = GetCreatureTemplate()->Classification;
            return rank != CREATURE_ELITE_NORMAL && rank != CREATURE_ELITE_RARE;
        }

        bool isWorldBoss() const
        {
            if (isPet())
                return false;

            return GetCreatureTemplate()->TypeFlags[0] & CREATURE_TYPEFLAGS_BOSS;
        }

        bool IsDungeonBoss() const;
        bool IsPersonalLoot() const;
        bool IsAutoLoot() const;

        uint8 getLevelForTarget(WorldObject const* target) const; // overwrite Unit::getLevelForTarget for boss level support

        bool IsInEvadeMode() const { return HasUnitState(UNIT_STATE_EVADE); }

        bool AIM_Initialize(CreatureAI* ai = NULL);
        void Motion_Initialize();

        void AI_SendMoveToPacket(float x, float y, float z, uint32 time, uint32 MovementFlags, uint8 type);
        inline CreatureAI* AI() const { return (CreatureAI*)i_AI; }

        SpellSchoolMask GetMeleeDamageSchoolMask() const { return m_meleeDamageSchoolMask; }
        void SetMeleeDamageSchool(SpellSchools school) { m_meleeDamageSchoolMask = SpellSchoolMask(1 << school); }

        void ProhibitSpellSchool(SpellSchoolMask idSchoolMask, uint32 unTimeMs);
        void _AddCreatureSpellCooldown(uint32 spell_id, time_t end_time);
        void _AddCreatureCategoryCooldown(uint32 category, time_t apply_time);
        void AddCreatureSpellCooldown(uint32 spellid);
        bool HasSpellCooldown(uint32 spell_id) const;
        bool HasCategoryCooldown(uint32 spell_id) const;
        uint32 _GetSpellCooldownDelay(uint32 spell_id) const
        {
            CreatureSpellCooldowns::const_iterator itr = m_CreatureSpellCooldowns.find(spell_id);
            time_t t = time(NULL);
            return uint32(itr != m_CreatureSpellCooldowns.end() && itr->second > t ? itr->second - t : 0);
        }
        void RemoveCreatureSpellCooldown(uint32 spell_id)
        {
            CreatureSpellCooldowns::iterator itr = m_CreatureSpellCooldowns.find(spell_id);
            if (itr != m_CreatureSpellCooldowns.end())
                m_CreatureSpellCooldowns.erase(itr);
        }

        bool HasSpell(uint32 spellID) const;

        bool UpdateEntry(uint32 entry, uint32 team=ALLIANCE, const CreatureData* data=NULL);
        void UpdateStat();
        bool UpdateStats(Stats stat);
        bool UpdateAllStats();
        void UpdateResistances(uint32 school);
        void UpdateArmor();
        void UpdateMaxHealth();
        void UpdateMaxPower(Powers power);
        void UpdateAttackPowerAndDamage(bool ranged = false);
        void UpdateDamagePhysical(WeaponAttackType attType);
        int8 GetOriginalEquipmentId() const { return m_originalEquipmentId; }
        uint8 GetCurrentEquipmentId() const { return m_equipmentId; }
        void SetCurrentEquipmentId(uint8 id) { m_equipmentId = id; }
        float GetSpellDamageMod(int32 Rank);

        VendorItemData const* GetVendorItems() const;
        uint32 GetVendorItemCurrentCount(VendorItem const* vItem);
        uint32 UpdateVendorItemCurrentCount(VendorItem const* vItem, uint32 used_count);

        TrainerSpellData const* GetTrainerSpells() const;

        CreatureTemplate const* GetCreatureTemplate() const { return m_creatureInfo; }
        CreatureData const* GetCreatureData() const { return m_creatureData; }
        CreatureDifficultyStat const* GetCreatureDiffStat() const { return m_creatureDiffData; }
        CreatureAddon const* GetCreatureAddon() const;

        std::string GetAIName() const;
        std::string GetNPCAIName() const { return AIName; }
        void SetNPCAIName(std::string name) { AIName = name; }
        std::string GetScriptName() const;
        uint32 GetScriptId() const;

        void Say(int32 textId, uint32 language, ObjectGuid TargetGuid) { MonsterSay(textId, language, TargetGuid); }
        void Yell(int32 textId, uint32 language, ObjectGuid TargetGuid) { MonsterYell(textId, language, TargetGuid); }
        void TextEmote(int32 textId, ObjectGuid TargetGuid, bool IsBossEmote = false) { MonsterTextEmote(textId, TargetGuid, IsBossEmote); }
        void Whisper(int32 textId, ObjectGuid receiver, bool IsBossWhisper = false) { MonsterWhisper(textId, receiver, IsBossWhisper); }
        void YellToZone(int32 textId, uint32 language, ObjectGuid TargetGuid) { MonsterYellToZone(textId, language, TargetGuid); }

        // override WorldObject function for proper name localization
        const char* GetNameForLocaleIdx(LocaleConstant locale_idx) const;

        void setDeathState(DeathState s);                   // override virtual Unit::setDeathState

        bool LoadFromDB(ObjectGuid::LowType guid, Map* map) { return LoadCreatureFromDB(guid, map, false); }
        bool LoadCreatureFromDB(ObjectGuid::LowType guid, Map* map, bool addToMap = true);
        void SaveToDB();
                                                            // overriden in Pet
        virtual void SaveToDB(uint32 mapid, uint32 spawnMask, uint32 phaseMask);
        virtual void DeleteFromDB();                        // overriden in Pet

        Loot loot;
        bool lootForPickPocketed;
        bool lootForBody;
        Player* GetLootRecipient() const;
        Group* GetLootRecipientGroup() const;
        Unit* GetOtherRecipient() const;
        bool hasLootRecipient() const { return m_lootRecipient || m_lootRecipientGroup; }
        bool isTappedBy(Player const* player) const;                          // return true if the creature is tapped by the player or a member of his party.

        void SetLootRecipient (Unit* unit);
        void SetOtherLootRecipient(ObjectGuid guid);
        void AllLootRemovedFromCorpse();

        SpellInfo const* reachWithSpellAttack(Unit* victim);
        SpellInfo const* reachWithSpellCure(Unit* victim);

        uint32 m_temlate_spells[CREATURE_MAX_SPELLS];
        CreatureSpellCooldowns m_CreatureSpellCooldowns;
        CreatureSpellCooldowns m_CreatureCategoryCooldowns;

        bool canStartAttack(Unit const* u, bool force) const;
        float GetAttackDistance(Unit const* player) const;

        void SendAIReaction(AiReaction reactionType);

        Unit* SelectNearestTarget(float dist = 0) const;
        Unit* SelectNearestTargetNoCC(float dist = 0) const;
        Unit* SelectNearestTargetInAttackDistance(float dist = 0) const;
        Player* SelectNearestPlayer(float distance = 0) const;
        Player* SelectNearestPlayerNotGM(float distance = 0) const;

        void DoFleeToGetAssistance();
        void CallForHelp(float fRadius);
        void CallAssistance();
        void SetNoCallAssistance(bool val) { m_AlreadyCallAssistance = val; }
        void SetNoSearchAssistance(bool val) { m_AlreadySearchedAssistance = val; }
        bool HasSearchedAssistance() { return m_AlreadySearchedAssistance; }
        bool CanAssistTo(const Unit* u, const Unit* enemy, bool checkfaction = true) const;
        bool _IsTargetAcceptable(const Unit* target) const;

        MovementGeneratorType GetDefaultMovementType() const { return m_defaultMovementType; }
        void SetDefaultMovementType(MovementGeneratorType mgt) { m_defaultMovementType = mgt; }

        void RemoveCorpse(bool setSpawnTime = true);

        void DespawnOrUnsummon(uint32 msTimeToDespawn = 0);

        time_t const& GetRespawnTime() const { return m_respawnTime; }
        time_t GetRespawnTimeEx() const;
        void SetRespawnTime(uint32 respawn) { m_respawnTime = respawn ? time(NULL) + respawn : 0; }
        void Respawn(bool force = false, uint32 timer = 0);
        void SaveRespawnTime();

        uint32 GetRemoveCorpseDelay() const { return m_corpseRemoveTime; }
        void SetRemoveCorpseDelay(uint32 delay) { m_corpseRemoveTime = delay; }

        uint32 GetRespawnDelay() const { return m_respawnDelay; }
        void SetRespawnDelay(uint32 delay) { m_respawnDelay = delay; }

        float GetRespawnRadius() const { return m_respawnradius; }
        void SetRespawnRadius(float dist) { m_respawnradius = dist; }

        uint32 m_groupLootTimer;                            // (msecs)timer used for group loot
        ObjectGuid lootingGroupLowGUID;                     // used to find group which is looting corpse

        void SendZoneUnderAttackMessage(Player* attacker);

        void SetInCombatWithZone();

        bool hasQuest(uint32 quest_id) const;
        bool hasInvolvedQuest(uint32 quest_id)  const;

        bool isRegeneratingHealth() { return m_regenHealth; }
        void setRegeneratingHealth(bool regenHealth) { m_regenHealth = regenHealth; }
        uint8 GetPetAutoSpellSize() const { return m_autospells.size(); }
        uint32 GetPetAutoSpellOnPos(uint8 pos) const
        {
            if (pos >= m_autospells.size())
                return 0;
            else
                return m_autospells[pos];
        }


        uint8 GetPetCastSpellSize() const { return m_castspells.size(); }
        void AddPetCastSpell(uint32 spellid) { m_castspells.push_back(spellid); }
        uint32 GetPetCastSpellOnPos(uint8 pos) const
        {
            if (pos >= m_castspells.size())
                return 0;
            else
                return m_castspells[pos];
        }

        void SetPosition(float x, float y, float z, float o);
        void SetPosition(const Position &pos) { SetPosition(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation()); }

        void SetHomePosition(float x, float y, float z, float o) { m_homePosition.Relocate(x, y, z, o); }
        void SetHomePosition(const Position &pos) { m_homePosition.Relocate(pos); }
        void GetHomePosition(float &x, float &y, float &z, float &ori) const { m_homePosition.GetPosition(x, y, z, ori); }
        Position GetHomePosition() const { return m_homePosition; }

        void SetTransportHomePosition(float x, float y, float z, float o) { m_transportHomePosition.Relocate(x, y, z, o); }
        void SetTransportHomePosition(const Position &pos) { m_transportHomePosition.Relocate(pos); }
        void GetTransportHomePosition(float &x, float &y, float &z, float &ori) { m_transportHomePosition.GetPosition(x, y, z, ori); }
        Position GetTransportHomePosition() { return m_transportHomePosition; }

        uint32 GetWaypointPath(){return m_path_id;}
        void LoadPath(uint32 pathid) { m_path_id = pathid; }

        uint32 GetCurrentWaypointID(){return m_waypointID;}
        void UpdateWaypointID(uint32 wpID){m_waypointID = wpID;}

        void SearchFormation();
        CreatureGroup* GetFormation() {return m_formation;}
        void SetFormation(CreatureGroup* formation) {m_formation = formation;}

        Unit* SelectVictim();

        void SetDisableReputationGain(bool disable) { DisableReputationGain = disable; }
        bool IsReputationGainDisabled() { return DisableReputationGain; }
        bool IsDamageEnoughForLootingAndReward() const { return m_PlayerDamageReq == 0; }
        void LowerPlayerDamageReq(uint32 unDamage)
        {
            if (m_PlayerDamageReq)
                m_PlayerDamageReq > unDamage ? m_PlayerDamageReq -= unDamage : m_PlayerDamageReq = 0;
        }
        void ResetPlayerDamageReq() { m_PlayerDamageReq = GetHealth() / 2; }
        uint32 m_PlayerDamageReq;

        uint32 GetOriginalEntry() const { return m_originalEntry; }
        void SetOriginalEntry(uint32 entry) { m_originalEntry = entry; }

        static float _GetDamageMod(int32 Rank);

        float m_SightDistance, m_CombatDistance;

        void SetGUIDTransport(ObjectGuid::LowType guid) { guid_transport=guid; }
        uint32 GetGUIDTransport() { return guid_transport; }

        void FarTeleportTo(Map* map, float X, float Y, float Z, float O);

        bool m_isTempWorldObject; //true when possessed
        uint32 GetBossId() const { return bossid; }
        uint8 GetMobDifficulty() const { return m_difficulty; }
        uint8 GetSpawnMode() const { return m_spawnMode; }
        uint32 GetPlayerCount() const { return m_playerCount; }

        float GetFollowAngle() const { return m_followAngle; }
        void SetFollowAngle(float angle) { m_followAngle = angle; }

        void ForcedDespawn(uint32 timeMSToDespawn = 0);

        uint32 m_LOSCheckTimer;
        bool m_LOSCheck_creature;
        bool m_LOSCheck_player;
        bool m_Stampeded;
        AutoSpellList   m_autospells;
        AutoSpellList   m_castspells;
        PetSpellMap     m_spells;

        bool onVehicleAccessoryInit() const { return m_onVehicleAccessory; }
        void SetVehicleAccessoryInit(bool r) { m_onVehicleAccessory = r; }

        uint32 GetVignetteId() const { return m_creatureInfo ? m_creatureInfo->VignetteId : 0; }
        uint32 GetPersonalLootId() const { return m_creatureInfo ? m_creatureInfo->personalloot : 0; }

        uint32 getCurrentUpdateAreaID() const { return m_areaUpdateId; }
        uint32 getCurrentUpdateZoneID() const { return m_zoneUpdateId; }

        bool IsDespawn() const { return m_despan; }

        CreatureEvadeWmoData const* GetEvadeWmoData() { return m_evadeWmoData; }

    protected:
        bool m_onVehicleAccessory;

        bool CreateFromProto(ObjectGuid::LowType guidlow, uint32 Entry, int32 vehId, uint32 team, const CreatureData* data = NULL);
        bool InitEntry(uint32 entry, uint32 team=ALLIANCE, const CreatureData* data=NULL);

        // vendor items
        VendorItemCounts m_vendorItemCounts;

        static float _GetHealthMod(int32 Rank);
        float _GetHealthModPersonal(uint32 &count);

        ObjectGuid m_lootRecipient;
        ObjectGuid m_LootOtherRecipient;                        // Pet lotter for example
        ObjectGuid m_lootRecipientGroup;

        /// Timers
        time_t m_corpseRemoveTime;                          // (msecs)timer for death or corpse disappearance
        time_t m_respawnTime;                               // (secs) time of next respawn
        uint32 m_respawnDelay;                              // (secs) delay between corpse disappearance and respawning
        uint32 m_corpseDelay;                               // (secs) delay between death and corpse disappearance
        float m_respawnradius;

        ReactStates m_reactState;                           // for AI, not charmInfo
        void RegenerateMana();
        void RegenerateHealth();
        void Regenerate(Powers power);
        uint32 m_regenTimerCount;
        uint32 m_petregenTimer;
        MovementGeneratorType m_defaultMovementType;
        uint64 m_DBTableGuid;                  ///< For new or temporary creatures is 0 for saved it is lowguid
        uint8 m_equipmentId;
        int8 m_originalEquipmentId; // can be -1

        bool m_AlreadyCallAssistance;
        bool m_AlreadySearchedAssistance;
        bool m_regenHealth;
        bool m_AI_locked;
        std::string AIName;

        SpellSchoolMask m_meleeDamageSchoolMask;
        uint32 m_originalEntry;

        Position m_homePosition;
        Position m_transportHomePosition;

        bool DisableReputationGain;

        CreatureTemplate const* m_creatureInfo;                 // in difficulty mode > 0 can different from sObjectMgr->GetCreatureTemplate(GetEntry())
        CreatureData const* m_creatureData;
        CreatureDifficultyStat const* m_creatureDiffData;

        uint32 guid_transport;
        uint32 bossid;
        uint8 m_difficulty;
        uint8 m_spawnMode;
        uint32 m_playerCount;
        float m_followAngle;
        bool m_despan;

        bool IsInvisibleDueToDespawn() const;
        bool CanAlwaysSee(WorldObject const* obj) const;
        bool IsNeverVisible() const;
    private:

        //WaypointMovementGenerator vars
        uint32 m_waypointID;
        uint32 m_path_id;

        uint16 m_zoneUpdateId;
        uint16 m_areaUpdateId;

        //Formation var
        CreatureGroup* m_formation;
        bool TriggerJustRespawned;
        CreatureEvadeWmoData const* m_evadeWmoData;
};

class AssistDelayEvent : public BasicEvent
{
    public:
        AssistDelayEvent(ObjectGuid victim, Unit& owner) : BasicEvent(), m_victim(victim), m_owner(owner) { }

        bool Execute(uint64 e_time, uint32 p_time);
        void AddAssistant(ObjectGuid guid) { m_assistants.push_back(guid); }
    private:
        AssistDelayEvent();

        ObjectGuid            m_victim;
        GuidList              m_assistants;
        Unit&                 m_owner;
};

class ForcedDespawnDelayEvent : public BasicEvent
{
    public:
        ForcedDespawnDelayEvent(Creature& owner) : BasicEvent(), m_owner(owner) { }
        bool Execute(uint64 e_time, uint32 p_time);

    private:
        Creature& m_owner;
};

class SetPhaseDelayEvent : public BasicEvent
{
    public:
        SetPhaseDelayEvent(Creature& owner, uint32 phase) : BasicEvent(), m_owner(owner), m_phase(phase) { }
        bool Execute(uint64 e_time, uint32 p_time);

    private:
        Creature& m_owner;
        uint32 m_phase;
};

class SetImuneDelayEvent : public BasicEvent
{
    public:
        SetImuneDelayEvent(Creature& owner) : BasicEvent(), m_owner(owner) { }
        bool Execute(uint64 e_time, uint32 p_time);

    private:
        Creature& m_owner;
};

#endif

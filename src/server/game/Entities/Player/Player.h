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

#ifndef _PLAYER_H
#define _PLAYER_H


#include <boost/dynamic_bitset_fwd.hpp>

#include "AchievementMgr.h"
#include "Bag.h"
#include "Battleground.h"
#include "BattlePetMgr.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "GroupReference.h"
#include "Item.h"
#include "ItemTemplate.h"
#include "MapReference.h"
#include "NPCHandler.h"
#include "Object.h"
#include "Opcodes.h"
#include "Packets/VehiclePackets.h"
#include "Pet.h"
#include "PhaseMgr.h"
#include "PlayerTaxi.h"
#include "QuestDef.h"
#include "ReputationMgr.h"
#include "SpellMgr.h"
#include "Unit.h"
#include "Util.h"
#include "WorldSession.h"

struct Mail;
struct ItemExtendedCostEntry;

class Bracket;
class Channel;
class Creature;
class DynamicObject;
class Garrison;
class Group;
class Guild;
class OutdoorPvP;
class Pet;
class PhaseMgr;
class PlayerMenu;
class PlayerSocial;
class SpellCastTargets;
class UpdateMask;

typedef std::deque<Mail*> PlayerMails;

#define PLAYER_MAX_SKILLS           128
#define PLAYER_MAX_DAILY_QUESTS     25
#define PLAYER_EXPLORED_ZONES_SIZE  256
#define MAX_PERKS_COUNT 4

enum SkillFieldOffset
{
    SKILL_ID_OFFSET                 = 0,
    SKILL_STEP_OFFSET               = 0x40,
    SKILL_RANK_OFFSET               = SKILL_STEP_OFFSET + 0x40,
    SUBSKILL_START_RANK_OFFSET      = SKILL_RANK_OFFSET + 0x40,
    SKILL_MAX_RANK_OFFSET           = SUBSKILL_START_RANK_OFFSET + 0x40,
    SKILL_TEMP_BONUS_OFFSET         = SKILL_MAX_RANK_OFFSET + 0x40,
    SKILL_PERM_BONUS_OFFSET         = SKILL_TEMP_BONUS_OFFSET + 0x40
};

// Note: SPELLMOD_* values is aura types in fact
enum SpellModType
{
    SPELLMOD_FLAT         = 107,                            // SPELL_AURA_ADD_FLAT_MODIFIER
    SPELLMOD_PCT          = 108                             // SPELL_AURA_ADD_PCT_MODIFIER
};

// 2^n values, Player::m_isunderwater is a bitmask. These are Trinity internal values, they are never send to any client
enum PlayerUnderwaterState
{
    UNDERWATER_NONE                     = 0x00,
    UNDERWATER_INWATER                  = 0x01,             // terrain type is water and player is afflicted by it
    UNDERWATER_INLAVA                   = 0x02,             // terrain type is lava and player is afflicted by it
    UNDERWATER_INSLIME                  = 0x04,             // terrain type is lava and player is afflicted by it
    UNDERWARER_INDARKWATER              = 0x08,             // terrain type is dark water and player is afflicted by it

    UNDERWATER_EXIST_TIMERS             = 0x10
};

enum BuyBankSlotResult
{
    ERR_BANKSLOT_FAILED_TOO_MANY    = 0,
    ERR_BANKSLOT_INSUFFICIENT_FUNDS = 1,
    ERR_BANKSLOT_NOTBANKER          = 2,
    ERR_BANKSLOT_OK                 = 3
};

enum PlayerSpellState
{
    PLAYERSPELL_UNCHANGED = 0,
    PLAYERSPELL_CHANGED   = 1,
    PLAYERSPELL_NEW       = 2,
    PLAYERSPELL_REMOVED   = 3,
    PLAYERSPELL_TEMPORARY = 4
};

struct PlayerSpell
{
    PlayerSpellState state : 8;
    bool active            : 1;                             // show in spellbook
    bool dependent         : 1;                             // learned as result another spell learn, skill grow, quest reward, etc
    bool disabled          : 1;                             // first rank has been learned in result talent learn but currently talent unlearned, save max learned ranks
    bool mount             : 1;                             // spell summon mount or Companions
    uint32 mountReplace;                                    // if mount for horde replace if exist aliace
};

// Spell modifier (used for modify other spells)
struct SpellModifier
{
    SpellModifier(Aura* _ownerAura = NULL) : op(SPELLMOD_DAMAGE), type(SPELLMOD_FLAT), charges(0), value(0), mask(), spellId(0), ownerAura(_ownerAura) {}
    SpellModOp   op   : 8;
    SpellModType type : 8;
    int16 charges     : 16;
    float value;
    flag128 mask;
    uint32 spellId;
    Aura* const ownerAura;
};

enum PlayerCurrencyFlag
{
    PLAYERCURRENCY_FLAG_NONE                = 0x0,
    PLAYERCURRENCY_FLAG_UNK1                = 0x1,  // unused?
    PLAYERCURRENCY_FLAG_UNK2                = 0x2,  // unused?
    PLAYERCURRENCY_FLAG_SHOW_IN_BACKPACK    = 0x4,
    PLAYERCURRENCY_FLAG_UNUSED              = 0x8,

    PLAYERCURRENCY_MASK_USED_BY_CLIENT =
        PLAYERCURRENCY_FLAG_SHOW_IN_BACKPACK |
        PLAYERCURRENCY_FLAG_UNUSED,
};

enum PlayerCurrencyState
{
   PLAYERCURRENCY_UNCHANGED = 0,
   PLAYERCURRENCY_CHANGED   = 1,
   PLAYERCURRENCY_NEW       = 2,
   PLAYERCURRENCY_REMOVED   = 3     //not removed just set count == 0
};

struct PlayerCurrency
{
    PlayerCurrencyState state;
    uint32 totalCount;
    uint32 weekCount;
    uint32 seasonTotal;
    uint8 flags;
    uint32 curentCap;
    CurrencyTypesEntry const * currencyEntry;
};

enum VignetteState
{
   VIGNETTE_STATE_ADD       = 1,
   VIGNETTE_STATE_UPDATE    = 2,
   VIGNETTE_STATE_IN_GAME   = 3,
   VIGNETTE_STATE_REMOVE    = 4
};

struct PlayerVignette
{
    ObjectGuid guid;
    uint32 vignetteId;
    uint32 zoneId;
    VignetteState state;
    Position position;
};

enum SceneEventStatus
{
    SCENE_NONE          = 0,
    SCENE_LAUNCH        = 1,
    SCENE_TRIGER        = 2,
    SCENE_COMPLETE      = 3,
};

typedef std::unordered_map<uint32, PlayerSpellState> PlayerTalentMap;
typedef std::unordered_map<uint32, PlayerSpell*> PlayerSpellMap;
typedef std::list<SpellModifier*> SpellModList;
typedef std::list<uint32> ItemSpellList;
typedef std::unordered_map<uint32, PlayerCurrency> PlayerCurrenciesMap;
typedef std::unordered_map<ObjectGuid, PlayerVignette> PlayerVignettesMap;

struct SpellCooldown
{
    double end;
    uint32 itemid;
};

struct SpellChargeData
{
    uint8 charges;
    uint8 maxCharges;

    SpellCategoryEntry const* categoryEntry;
    uint32 timer;
    uint32 chargeRegenTime;
};

typedef std::map<uint32, double> RPPMLastSuccessfulProc;
typedef std::map<uint32, double> RPPMLastChanceToProc;
typedef std::map<uint32, SpellCooldown> RPPMSpellCooldowns;
typedef std::map<uint32, SpellCooldown> SpellCooldowns;
typedef std::map<uint32 /*categoryId*/, SpellChargeData> SpellChargeDataMap;
typedef std::unordered_map<uint32 /*instanceId*/, time_t/*releaseTime*/> InstanceTimeMap;

enum TrainerSpellState
{
    TRAINER_SPELL_GRAY  = 0,
    TRAINER_SPELL_GREEN = 1,
    TRAINER_SPELL_RED   = 2,
    TRAINER_SPELL_GREEN_DISABLED = 10                       // custom value, not send to client: formally green but learn not allowed
};

enum ActionButtonUpdateState
{
    ACTIONBUTTON_UNCHANGED = 0,
    ACTIONBUTTON_CHANGED   = 1,
    ACTIONBUTTON_NEW       = 2,
    ACTIONBUTTON_DELETED   = 3
};

enum ActionButtonType
{
    ACTION_BUTTON_SPELL     = 0x00,
    ACTION_BUTTON_C         = 0x01,                         // click?
    ACTION_BUTTON_EQSET     = 0x20,
    ACTION_BUTTON_SUB_BUTTON= 0x30,
    ACTION_BUTTON_MACRO     = 0x40,
    ACTION_BUTTON_PET       = 0x50,
    ACTION_BUTTON_CMACRO    = ACTION_BUTTON_C | ACTION_BUTTON_MACRO,
    ACTION_BUTTON_MOUNT     = 0x60,
    ACTION_BUTTON_ITEM      = 0x80
};

#define ACTION_BUTTON_ACTION(X) (uint32(uint64(X) & 0xFFFFFFFF))
#define ACTION_BUTTON_TYPE(X)   (uint8(uint64(X) >> 56))
#define MAX_ACTION_BUTTON_ACTION_VALUE (0xFFFFFFFF)

struct ActionButton
{
    ActionButton() : packedData(0), uState(ACTIONBUTTON_NEW) { }

    uint64 packedData;
    ActionButtonUpdateState uState;

    // helpers
    ActionButtonType GetType() const { return ActionButtonType(ACTION_BUTTON_TYPE(packedData)); }
    uint32 GetAction() const { return ACTION_BUTTON_ACTION(packedData); }
    void SetActionAndType(uint32 action, ActionButtonType type)
    {
        uint64 newData = action | (uint64(type) << 56);
        if (newData != packedData || uState == ACTIONBUTTON_DELETED)
        {
            packedData = newData;
            if (uState != ACTIONBUTTON_NEW)
                uState = ACTIONBUTTON_CHANGED;
        }
    }
};

#define  MAX_ACTION_BUTTONS 132                             //checked in 5.0.5

typedef std::map<uint8, ActionButton> ActionButtonList;

struct PlayerCreateInfoItem
{
    PlayerCreateInfoItem(uint32 id, uint32 amount) : item_id(id), item_amount(amount) {}

    uint32 item_id;
    uint32 item_amount;
};

typedef std::list<PlayerCreateInfoItem> PlayerCreateInfoItems;

struct PlayerLevelInfo
{
    PlayerLevelInfo()
    {
        for (uint8 i = 0; i < MAX_STATS; ++i)
            stats[i] = 0;
    }

    uint16 stats[MAX_STATS];
};

typedef std::list<uint32> PlayerCreateInfoSpells;

struct PlayerCreateInfoAction
{
    PlayerCreateInfoAction() : button(0), type(0), action(0) {}
    PlayerCreateInfoAction(uint8 _button, uint32 _action, uint8 _type) : button(_button), type(_type), action(_action) {}

    uint8 button;
    uint8 type;
    uint32 action;
};

typedef std::list<PlayerCreateInfoAction> PlayerCreateInfoActions;
typedef std::list<SkillRaceClassInfoEntry const*> PlayerCreateInfoSkills;

struct PlayerInfo
{
                                                            // existence checked by displayId != 0
    PlayerInfo() : displayId_m(0), displayId_f(0), mapId(0), areaId(0), positionX(0.0f), positionY(0.0f), positionZ(0.0f), orientation(0.0f), levelInfo(NULL)
    { }

    uint32 mapId;
    uint32 areaId;
    float positionX;
    float positionY;
    float positionZ;
    float orientation;
    uint16 displayId_m;
    uint16 displayId_f;
    PlayerCreateInfoItems item;
    PlayerCreateInfoSpells spell;
    PlayerCreateInfoActions action;
    PlayerCreateInfoSkills skills;

    PlayerLevelInfo* levelInfo;                             //[level-1] 0..MaxPlayerLevel-1
};

struct PvPInfo
{
    PvPInfo() : inHostileArea(false), inNoPvPArea(false), inFFAPvPArea(false), endTimer(0) {}

    bool inHostileArea;
    bool inNoPvPArea;
    bool inFFAPvPArea;
    time_t endTimer;
};

struct DuelInfo
{
    DuelInfo() : initiator(NULL), opponent(NULL), startTimer(0), startTime(0), outOfBound(0), isMounted(false) {}

    Player* initiator;
    Player* opponent;
    time_t startTimer;
    time_t startTime;
    time_t outOfBound;
    bool isMounted;
};

struct Areas
{
    uint32 areaID;
    uint32 areaFlag;
    float x1;
    float x2;
    float y1;
    float y2;
};

#define MAX_RUNES       6

enum RuneCooldowns
{
    RUNE_BASE_COOLDOWN  = 10000,
    RUNE_MISS_COOLDOWN  = 1500,     // cooldown applied on runes when the spell misses
};

struct RuneInfo
{
    uint32 Cooldown;
};

struct Runes
{
    RuneInfo runes[MAX_RUNES];
    uint8 runeState;                                        // mask of available runes
    float CooldownCoef;

    void SetRuneState(uint8 index, bool set = true)
    {
        if (set)
            runeState |= (1 << index);                      // usable
        else
            runeState &= ~(1 << index);                     // on cooldown
    }
};

struct EnchantDuration
{
    EnchantDuration() : item(NULL), slot(MAX_ENCHANTMENT_SLOT), leftduration(0) {};
    EnchantDuration(Item* _item, EnchantmentSlot _slot, uint32 _leftduration) : item(_item), slot(_slot),
        leftduration(_leftduration){ ASSERT(item); };

    Item* item;
    EnchantmentSlot slot;
    uint32 leftduration;
};

typedef std::list<EnchantDuration> EnchantDurationList;
typedef std::list<Item*> ItemDurationList;

enum DrunkenState
{
    DRUNKEN_SOBER   = 0,
    DRUNKEN_TIPSY   = 1,
    DRUNKEN_DRUNK   = 2,
    DRUNKEN_SMASHED = 3
};

#define MAX_DRUNKEN   4

enum PlayerFlags
{
    PLAYER_FLAGS_GROUP_LEADER           = 0x00000001,
    PLAYER_FLAGS_AFK                    = 0x00000002,
    PLAYER_FLAGS_DND                    = 0x00000004,
    PLAYER_FLAGS_GM                     = 0x00000008,
    PLAYER_FLAGS_GHOST                  = 0x00000010,
    PLAYER_FLAGS_RESTING                = 0x00000020,
    PLAYER_FLAGS_UNK6                   = 0x00000040,
    PLAYER_FLAGS_UNK7                   = 0x00000080,       // pre-3.0.3 PLAYER_FLAGS_FFA_PVP flag for FFA PVP state
    PLAYER_FLAGS_CONTESTED_PVP          = 0x00000100,       // Player has been involved in a PvP combat and will be attacked by contested guards
    PLAYER_FLAGS_IN_PVP                 = 0x00000200,
    PLAYER_FLAGS_HIDE_HELM              = 0x00000400,
    PLAYER_FLAGS_HIDE_CLOAK             = 0x00000800,
    PLAYER_FLAGS_PLAYED_LONG_TIME       = 0x00001000,       // played long time
    PLAYER_FLAGS_PLAYED_TOO_LONG        = 0x00002000,       // played too long time
    PLAYER_FLAGS_IS_OUT_OF_BOUNDS       = 0x00004000,
    PLAYER_FLAGS_DEVELOPER              = 0x00008000,       // <Dev> prefix for something?
    PLAYER_FLAGS_UNK16                  = 0x00010000,       // pre-3.0.3 PLAYER_FLAGS_SANCTUARY flag for player entered sanctuary
    PLAYER_FLAGS_TAXI_BENCHMARK         = 0x00020000,       // taxi benchmark mode (on/off) (2.0.1)
    PLAYER_FLAGS_PVP_TIMER              = 0x00040000,       // 3.0.2, pvp timer active (after you disable pvp manually)
    PLAYER_FLAGS_UBER                   = 0x00080000,
    PLAYER_FLAGS_UNK20                  = 0x00100000,
    PLAYER_FLAGS_UNK21                  = 0x00200000,
    PLAYER_FLAGS_COMMENTATOR2           = 0x00400000,
    PLAYER_ALLOW_ONLY_ABILITY           = 0x00800000,       // used by bladestorm and killing spree, allowed only spells with SPELL_ATTR0_REQ_AMMO, SPELL_EFFECT_ATTACK, checked only for active player
    PLAYER_FLAGS_PET_BATTLES_UNLOCKED   = 0x01000000,       // pet battles
    PLAYER_FLAGS_NO_XP_GAIN             = 0x02000000,
    PLAYER_FLAGS_UNK26                  = 0x04000000,
    PLAYER_FLAGS_AUTO_DECLINE_GUILD     = 0x08000000,       // Automatically declines guild invites
    PLAYER_FLAGS_GUILD_LEVEL_ENABLED    = 0x10000000,       // Lua_GetGuildLevelEnabled() - enables guild leveling related UI
    PLAYER_FLAGS_VOID_UNLOCKED          = 0x20000000,       // void storage
    PLAYER_FLAGS_UNK30                  = 0x40000000, 
};

// used for PLAYER_FIELD_KNOWN_TITLES field (uint64), (1<<bit_index) without (-1)
// can't use enum for uint64 values
#define PLAYER_TITLE_DISABLED              UI64LIT(0x0000000000000000)
#define PLAYER_TITLE_NONE                  UI64LIT(0x0000000000000001)
#define PLAYER_TITLE_PRIVATE               UI64LIT(0x0000000000000002) // 1
#define PLAYER_TITLE_CORPORAL              UI64LIT(0x0000000000000004) // 2
#define PLAYER_TITLE_SERGEANT_A            UI64LIT(0x0000000000000008) // 3
#define PLAYER_TITLE_MASTER_SERGEANT       UI64LIT(0x0000000000000010) // 4
#define PLAYER_TITLE_SERGEANT_MAJOR        UI64LIT(0x0000000000000020) // 5
#define PLAYER_TITLE_KNIGHT                UI64LIT(0x0000000000000040) // 6
#define PLAYER_TITLE_KNIGHT_LIEUTENANT     UI64LIT(0x0000000000000080) // 7
#define PLAYER_TITLE_KNIGHT_CAPTAIN        UI64LIT(0x0000000000000100) // 8
#define PLAYER_TITLE_KNIGHT_CHAMPION       UI64LIT(0x0000000000000200) // 9
#define PLAYER_TITLE_LIEUTENANT_COMMANDER  UI64LIT(0x0000000000000400) // 10
#define PLAYER_TITLE_COMMANDER             UI64LIT(0x0000000000000800) // 11
#define PLAYER_TITLE_MARSHAL               UI64LIT(0x0000000000001000) // 12
#define PLAYER_TITLE_FIELD_MARSHAL         UI64LIT(0x0000000000002000) // 13
#define PLAYER_TITLE_GRAND_MARSHAL         UI64LIT(0x0000000000004000) // 14
#define PLAYER_TITLE_SCOUT                 UI64LIT(0x0000000000008000) // 15
#define PLAYER_TITLE_GRUNT                 UI64LIT(0x0000000000010000) // 16
#define PLAYER_TITLE_SERGEANT_H            UI64LIT(0x0000000000020000) // 17
#define PLAYER_TITLE_SENIOR_SERGEANT       UI64LIT(0x0000000000040000) // 18
#define PLAYER_TITLE_FIRST_SERGEANT        UI64LIT(0x0000000000080000) // 19
#define PLAYER_TITLE_STONE_GUARD           UI64LIT(0x0000000000100000) // 20
#define PLAYER_TITLE_BLOOD_GUARD           UI64LIT(0x0000000000200000) // 21
#define PLAYER_TITLE_LEGIONNAIRE           UI64LIT(0x0000000000400000) // 22
#define PLAYER_TITLE_CENTURION             UI64LIT(0x0000000000800000) // 23
#define PLAYER_TITLE_CHAMPION              UI64LIT(0x0000000001000000) // 24
#define PLAYER_TITLE_LIEUTENANT_GENERAL    UI64LIT(0x0000000002000000) // 25
#define PLAYER_TITLE_GENERAL               UI64LIT(0x0000000004000000) // 26
#define PLAYER_TITLE_WARLORD               UI64LIT(0x0000000008000000) // 27
#define PLAYER_TITLE_HIGH_WARLORD          UI64LIT(0x0000000010000000) // 28
#define PLAYER_TITLE_GLADIATOR             UI64LIT(0x0000000020000000) // 29
#define PLAYER_TITLE_DUELIST               UI64LIT(0x0000000040000000) // 30
#define PLAYER_TITLE_RIVAL                 UI64LIT(0x0000000080000000) // 31
#define PLAYER_TITLE_CHALLENGER            UI64LIT(0x0000000100000000) // 32
#define PLAYER_TITLE_SCARAB_LORD           UI64LIT(0x0000000200000000) // 33
#define PLAYER_TITLE_CONQUEROR             UI64LIT(0x0000000400000000) // 34
#define PLAYER_TITLE_JUSTICAR              UI64LIT(0x0000000800000000) // 35
#define PLAYER_TITLE_CHAMPION_OF_THE_NAARU UI64LIT(0x0000001000000000) // 36
#define PLAYER_TITLE_MERCILESS_GLADIATOR   UI64LIT(0x0000002000000000) // 37
#define PLAYER_TITLE_OF_THE_SHATTERED_SUN  UI64LIT(0x0000004000000000) // 38
#define PLAYER_TITLE_HAND_OF_ADAL          UI64LIT(0x0000008000000000) // 39
#define PLAYER_TITLE_VENGEFUL_GLADIATOR    UI64LIT(0x0000010000000000) // 40

#define KNOWN_TITLES_SIZE   5
#define MAX_TITLE_INDEX     (KNOWN_TITLES_SIZE*64)          // 3 uint64 fields

// used in PLAYER_FIELD_LOCAL_FLAGS values
enum PlayerLocalFlags
{
    PLAYER_LOCAL_FLAG_PET_ACTIVE                    = 0x00000001,
    PLAYER_FIELD_BYTE_TRACK_STEALTHED               = 0x00000002,
    PLAYER_FIELD_BYTE_RELEASE_TIMER                 = 0x00000008,       // Display time till auto release spirit
    PLAYER_FIELD_BYTE_NO_RELEASE_WINDOW             = 0x00000010,       // Display no "release spirit" window at all
    PLAYER_LOCAL_FLAG_NO_PET_BAR                    = 0x00000020,       // CGPetInfo::IsPetBarUsed
    PLAYER_LOCAL_FLAG_OVERRIDE_CAMERA_MIN_HEIGHT    = 0x00000040,
    PLAYER_LOCAL_FLAG_USING_PARTY_GARRISON          = 0x00000100,
    PLAYER_LOCAL_FLAG_CAN_USE_OBJECTS_MOUNTED       = 0x00000200,
    PLAYER_LOCAL_FLAG_CAN_VISIT_PARTY_GARRISON      = 0x00000400
};

// used in PLAYER_FIELD_BYTES2 values
enum PlayerFieldByte2Flags
{
    PLAYER_FIELD_BYTE2_NONE                 = 0x00,
    PLAYER_FIELD_BYTE2_STEALTH              = 0x20,
    PLAYER_FIELD_BYTE2_INVISIBILITY_GLOW    = 0x40
};

enum PlayerBytesOffsets
{
    PLAYER_BYTES_OFFSET_SKIN_ID         = 0,
    PLAYER_BYTES_OFFSET_FACE_ID         = 1,
    PLAYER_BYTES_OFFSET_HAIR_STYLE_ID   = 2,
    PLAYER_BYTES_OFFSET_HAIR_COLOR_ID   = 3
};

enum PlayerBytes2Offsets
{
    PLAYER_BYTES_2_OFFSET_FACIAL_STYLE      = 0,
    PLAYER_BYTES_2_OFFSET_PARTY_TYPE        = 1,
    PLAYER_BYTES_2_OFFSET_BANK_BAG_SLOTS    = 2,
    PLAYER_BYTES_2_OFFSET_BLIND_FOLD        = 3
};

enum PlayerRestStateOffsets
{
    PLAYER_REST_STATE_OFFSET_STATE          = 0,
};

enum PlayerBytes3Offsets
{
    PLAYER_BYTES_3_OFFSET_GENDER        = 0,
    PLAYER_BYTES_3_OFFSET_INEBRIATION   = 1,
    PLAYER_BYTES_3_TATOO                = 2,
    PLAYER_BYTES_3_HORN                 = 3

    //PLAYER_BYTES_3_OFFSET_PVP_TITLE     = 2,
    //PLAYER_BYTES_3_OFFSET_ARENA_FACTION = 3
};

enum PlayerFieldBytesOffsets
{
    PLAYER_FIELD_BYTES_OFFSET_RAF_GRANTABLE_LEVEL   = 0,
    PLAYER_FIELD_BYTES_OFFSET_ACTION_BAR_TOGGLES    = 1,
    PLAYER_FIELD_BYTES_OFFSET_PVP_RANK              = 2,
    PLAYER_FIELD_BYTES_OFFSET_LIFETIME_MAX_PVP_RANK = 3
};

enum PlayerFieldBytes2Offsets
{
    PLAYER_FIELD_BYTES_2_OFFSET_AURA_VISION         = 1,
    PLAYER_FIELD_BYTES_2_OFFSET_OVERRIDE_SPELLS_ID  = 2  // uint16!
};

static_assert((PLAYER_FIELD_BYTES_2_OFFSET_OVERRIDE_SPELLS_ID & 1) == 0, "PLAYER_FIELD_BYTES_2_OFFSET_OVERRIDE_SPELLS_ID must be aligned to 2 byte boundary");

#define PLAYER_BYTES_2_OVERRIDE_SPELLS_UINT16_OFFSET (PLAYER_FIELD_BYTES_2_OFFSET_OVERRIDE_SPELLS_ID / 2)

enum MirrorTimerType : int32
{
    FATIGUE_TIMER      = 0,
    BREATH_TIMER       = 1,
    FIRE_TIMER         = 2 // feign death
};

#define MAX_TIMERS      3
#define DISABLED_MIRROR_TIMER   -1

// 2^n values
enum PlayerExtraFlags
{
    // gm abilities
    PLAYER_EXTRA_GM_ON              = 0x0001,
    PLAYER_EXTRA_ACCEPT_WHISPERS    = 0x0004,
    PLAYER_EXTRA_TAXICHEAT          = 0x0008,
    PLAYER_EXTRA_GM_INVISIBLE       = 0x0010,
    PLAYER_EXTRA_GM_CHAT            = 0x0020,               // Show GM badge in chat messages

    // other states
    PLAYER_EXTRA_PVP_DEATH          = 0x0100                // store PvP death status until corpse creating.
};

// 2^n values
enum AtLoginFlags
{
    AT_LOGIN_NONE                   = 0x000,
    AT_LOGIN_RENAME                 = 0x001,
    AT_LOGIN_RESET_SPELLS           = 0x002,
    AT_LOGIN_RESET_TALENTS          = 0x004,
    AT_LOGIN_CUSTOMIZE              = 0x008,
    AT_LOGIN_RESET_PET_TALENTS      = 0x010,
    AT_LOGIN_FIRST                  = 0x020,
    AT_LOGIN_CHANGE_FACTION         = 0x040,
    AT_LOGIN_CHANGE_RACE            = 0x080,
    AT_LOGIN_UNLOCK                 = 0x100,
    AT_LOGIN_LOCKED_FOR_TRANSFER    = 0x200,
};

typedef std::map<uint32, QuestStatusData> QuestStatusMap;
typedef std::set<uint32> RewardedQuestSet;

enum QuestSaveType
{
    QUEST_DEFAULT_SAVE_TYPE = 0,
    QUEST_DELETE_SAVE_TYPE,
    QUEST_FORCE_DELETE_SAVE_TYPE
};

//               quest,  keep
typedef std::map<uint32, QuestSaveType> QuestStatusSaveMap;

enum QuestSlotOffsets
{
    QUEST_ID_OFFSET     = 0,
    QUEST_STATE_OFFSET  = 1,
    QUEST_COUNTS_OFFSET = 2,
    QUEST_TIME_OFFSET   = 4 // ???
};

#define MAX_QUEST_OFFSET 15

enum QuestSlotStateMask
{
    QUEST_STATE_NONE     = 0x0000,
    QUEST_STATE_COMPLETE = 0x0001,
    QUEST_STATE_FAIL     = 0x0002
};

enum SkillUpdateState
{
    SKILL_UNCHANGED     = 0,
    SKILL_CHANGED       = 1,
    SKILL_NEW           = 2,
    SKILL_DELETED       = 3
};

struct SkillStatusData
{
    SkillStatusData(uint8 _pos, SkillUpdateState _uState) : pos(_pos), uState(_uState)
    {
    }
    uint8 pos;
    SkillUpdateState uState;
};

typedef std::unordered_map<uint32, SkillStatusData> SkillStatusMap;

class Quest;
class Spell;
class Item;
class WorldSession;

enum PlayerSlots
{
    // first slot for item stored (in any way in player m_items data)
    PLAYER_SLOT_START           = 0,
    // last+1 slot for item stored (in any way in player m_items data)
    PLAYER_SLOT_END             = 86,
    PLAYER_SLOTS_COUNT          = (PLAYER_SLOT_END - PLAYER_SLOT_START)
};

#define INVENTORY_SLOT_BAG_0    255

enum EquipmentSlots                                         // 19 slots
{
    EQUIPMENT_SLOT_START        = 0,
    EQUIPMENT_SLOT_HEAD         = 0,
    EQUIPMENT_SLOT_NECK         = 1,
    EQUIPMENT_SLOT_SHOULDERS    = 2,
    EQUIPMENT_SLOT_BODY         = 3,
    EQUIPMENT_SLOT_CHEST        = 4,
    EQUIPMENT_SLOT_WAIST        = 5,
    EQUIPMENT_SLOT_LEGS         = 6,
    EQUIPMENT_SLOT_FEET         = 7,
    EQUIPMENT_SLOT_WRISTS       = 8,
    EQUIPMENT_SLOT_HANDS        = 9,
    EQUIPMENT_SLOT_FINGER1      = 10,
    EQUIPMENT_SLOT_FINGER2      = 11,
    EQUIPMENT_SLOT_TRINKET1     = 12,
    EQUIPMENT_SLOT_TRINKET2     = 13,
    EQUIPMENT_SLOT_BACK         = 14,
    EQUIPMENT_SLOT_MAINHAND     = 15,
    EQUIPMENT_SLOT_OFFHAND      = 16,
    EQUIPMENT_SLOT_RANGED       = 17,
    EQUIPMENT_SLOT_TABARD       = 18,
    EQUIPMENT_SLOT_END          = 19
};

enum InventorySlots                                         // 4 slots
{
    INVENTORY_SLOT_BAG_START    = 19,
    INVENTORY_SLOT_BAG_END      = 23
};

enum InventoryPackSlots                                     // 16 slots
{
    INVENTORY_SLOT_ITEM_START   = 23,
    INVENTORY_SLOT_ITEM_END     = 39
};

enum BankItemSlots                                          // 28 slots
{
    BANK_SLOT_ITEM_START        = 39,
    BANK_SLOT_ITEM_END          = 67
};

enum BankBagSlots                                           // 7 slots
{
    BANK_SLOT_BAG_START         = 67,
    BANK_SLOT_BAG_END           = 74
};

enum BuyBackSlots                                           // 12 slots
{
    // stored in m_buybackitems
    BUYBACK_SLOT_START          = 74,
    BUYBACK_SLOT_END            = 86
};

enum ReagentSlots
{
    REAGENT_SLOT_START          = 87,
    REAGENT_SLOT_END            = 184,
};

enum EquipmentSetUpdateState
{
    EQUIPMENT_SET_UNCHANGED = 0,
    EQUIPMENT_SET_CHANGED   = 1,
    EQUIPMENT_SET_NEW       = 2,
    EQUIPMENT_SET_DELETED   = 3
};

struct EquipmentSetInfo
{
    struct EquipmentSetData
    {
        ObjectGuid Pieces[EQUIPMENT_SLOT_END];
        uint64 Guid = 0;
        uint32 UnkInt2[2] = { };
        uint32 SetID = 0;
        uint32 IgnoreMask = 0;
        uint32 UnkInt = 0;
        std::string SetName;
        std::string SetIcon;
    } Data;

    EquipmentSetUpdateState State = EQUIPMENT_SET_NEW;
};

#define MAX_EQUIPMENT_SET_INDEX 10                          // client limit

typedef std::map<uint32, EquipmentSetInfo> EquipmentSetContainer;

struct ItemPosCount
{
    ItemPosCount(uint16 _pos, uint32 _count) : pos(_pos), count(_count) {}
    bool isContainedIn(std::vector<ItemPosCount> const& vec) const;
    uint16 pos;
    uint32 count;
};
typedef std::vector<ItemPosCount> ItemPosCountVec;

enum TradeSlots
{
    TRADE_SLOT_COUNT            = 7,
    TRADE_SLOT_TRADED_COUNT     = 6,
    TRADE_SLOT_NONTRADED        = 6,
    TRADE_SLOT_INVALID          = -1,
};

enum TransferAbortReason
{
    TRANSFER_ABORT_ERROR                            = 1,
    TRANSFER_ABORT_MAX_PLAYERS                      = 2,      // Transfer Aborted: instance is full
    TRANSFER_ABORT_TOO_MANY_INSTANCES               = 4,      // You have entered too many instances recently.
    TRANSFER_ABORT_ZONE_IN_COMBAT                   = 6,      // Unable to zone in while an encounter is in progress.
    TRANSFER_ABORT_INSUF_EXPAN_LVL                  = 7,      // You must have <TBC, WotLK> expansion installed to access this area.
    TRANSFER_ABORT_DIFFICULTY                       = 8,      // <Normal, Heroic, Epic> difficulty mode is not available for %s.
    TRANSFER_ABORT_UNIQUE_MESSAGE                   = 9,      // Until you've escaped TLK's grasp, you cannot leave this place!
    TRANSFER_ABORT_TOO_MANY_REALM_INSTANCES         = 10,     // Additional instances cannot be launched, please try again later.
    TRANSFER_ABORT_NEED_GROUP                       = 11,     // Transfer Aborted: you must be in a raid group to enter this instance
    TRANSFER_ABORT_NOT_FOUND                        = 14,     // Transfer Aborted: instance not found
    TRANSFER_ABORT_REALM_ONLY                       = 15,     // All players in the party must be from the same realm to enter %s.
    TRANSFER_ABORT_MAP_NOT_ALLOWED                  = 16,     // Map can't be entered at this time.
    TRANSFER_ABORT_LOCKED_TO_DIFFERENT_INSTANCE     = 18,     // You are already locked to %s
    TRANSFER_ABORT_ALREADY_COMPLETED_ENCOUNTER      = 19,     // You are ineligible to participate in at least one encounter in this instance because you are already locked to an instance in which it has been defeated.
    TRANSFER_ABORT_XREALM_ZONE_DOWN                 = 24,     // Transfer Aborted: cross-realm zone is down
    TRANSFER_ABORT_SOLO_PLAYER_SWITCH_DIFFICULTY    = 26,     // This instance is already in progress. You may only switch difficulties from inside the instance.
};

enum InstanceResetWarningType
{
    RAID_INSTANCE_WARNING_HOURS     = 1,                    // WARNING! %s is scheduled to reset in %d hour(s).
    RAID_INSTANCE_WARNING_MIN       = 2,                    // WARNING! %s is scheduled to reset in %d minute(s)!
    RAID_INSTANCE_WARNING_MIN_SOON  = 3,                    // WARNING! %s is scheduled to reset in %d minute(s). Please exit the zone or you will be returned to your bind location!
    RAID_INSTANCE_WELCOME           = 4,                    // Welcome to %s. This raid instance is scheduled to reset in %s.
    RAID_INSTANCE_EXPIRED           = 5
};

// PLAYER_FIELD_PVP_INFO offsets
/// @Posible that where is only 3 arena slots with 8 fields
enum BracketInfoType
{
    //BRACKET_RATING               = 0,
    BRACKET_SEASON_GAMES         = 0,
    BRACKET_SEASON_WIN           = 1,
    BRACKET_WEEK_GAMES           = 2,
    BRACKET_WEEK_WIN             = 3,
    BRACKET_WEEK_BEST            = 4,                       // Best rating on this week
    BRACKET_BEST                 = 5,                       // Best rating on this season
    BRACKET_END                  = 6
};

class InstanceSave;

enum KillStates
{
    KILL_UNCHANGED  = 0,
    KILL_CHANGED    = 1,
    KILL_NEW        = 2
    // no removed state, all kills are flushed at midnight
};

struct KillInfo
{
    uint32 count;
    KillStates state;
    KillInfo() : count(0), state(KILL_NEW) {}
};

typedef std::map<uint64, KillInfo> KillInfoMap;

struct HonorInfo
{
    HonorInfo() : CurrentHonorAtLevel(0), PrestigeLevel(0), HonorLevel(0) { }

    uint16 CurrentHonorAtLevel;
    uint8 PrestigeLevel;
    uint8 HonorLevel;

    static uint8 const MaxHonorLevel = 50;
    static uint8 const MaxPrestigeLevel = 6;
};

typedef std::map<ObjectGuid::LowType, HonorInfo> HonorInfoContainer;

enum RestType
{
    REST_TYPE_NO        = 0,
    REST_TYPE_IN_TAVERN = 1,
    REST_TYPE_IN_CITY   = 2
};

enum DuelCompleteType
{
    DUEL_INTERRUPTED = 0,
    DUEL_WON         = 1,
    DUEL_FLED        = 2
};

enum TeleportToOptions
{
    TELE_TO_GM_MODE             = 0x01,
    TELE_TO_NOT_LEAVE_TRANSPORT = 0x02,
    TELE_TO_NOT_LEAVE_COMBAT    = 0x04,
    TELE_TO_NOT_UNSUMMON_PET    = 0x08,
    TELE_TO_SPELL               = 0x10,
    TELE_TO_SEAMLESS            = 0x20
};

/// Type of environmental damages
enum EnviromentalDamage
{
    DAMAGE_EXHAUSTED = 0,
    DAMAGE_DROWNING  = 1,
    DAMAGE_FALL      = 2,
    DAMAGE_LAVA      = 3,
    DAMAGE_SLIME     = 4,
    DAMAGE_FIRE      = 5,
    DAMAGE_FALL_TO_VOID = 6                                 // custom case for fall without durability loss
};

enum AttackSwingReason
{
    ATTACK_SWING_ERROR_BAD_FACING       = 0,
    ATTACK_SWING_ERROR_NOT_IN_RANGE     = 1,
    ATTACK_SWING_ERROR_DEAD_TARGET      = 2,                // nothing shown
    ATTACK_SWING_ERROR_CANT_ATTACK      = 3                 // interrupted
};

enum PlayerChatTag
{
    CHAT_TAG_NONE       = 0x00,
    CHAT_TAG_AFK        = 0x01,
    CHAT_TAG_DND        = 0x02,
    CHAT_TAG_GM         = 0x04,
    CHAT_TAG_COM        = 0x08, // Commentator
    CHAT_TAG_DEV        = 0x10,
};

enum PlayedTimeIndex
{
    PLAYED_TIME_TOTAL = 0,
    PLAYED_TIME_LEVEL = 1
};

#define MAX_PLAYED_TIME_INDEX 2

// used at player loading query list preparing, and later result selection
enum PlayerLoginQueryIndex
{
    PLAYER_LOGIN_QUERY_LOADFROM                     = 0,
    PLAYER_LOGIN_QUERY_LOADGROUP                    = 1,
    PLAYER_LOGIN_QUERY_LOADBOUNDINSTANCES           = 2,
    PLAYER_LOGIN_QUERY_LOADAURAS                    = 3,
    PLAYER_LOGIN_QUERY_LOADAURAS_EFFECTS            = 4,
    PLAYER_LOGIN_QUERY_LOADSPELLS                   = 5,
    PLAYER_LOGIN_QUERY_LOADQUESTSTATUS              = 6,
    PLAYER_LOGIN_QUERY_LOADDAILYQUESTSTATUS         = 7,
    PLAYER_LOGIN_QUERY_LOADREPUTATION               = 8,
    PLAYER_LOGIN_QUERY_LOADINVENTORY                = 9,
    PLAYER_LOGIN_QUERY_LOADACTIONS                  = 10,
    PLAYER_LOGIN_QUERY_LOADMAILCOUNT                = 11,
    PLAYER_LOGIN_QUERY_LOADMAILDATE                 = 12,
    PLAYER_LOGIN_QUERY_LOADSOCIALLIST               = 13,
    PLAYER_LOGIN_QUERY_LOADHOMEBIND                 = 14,
    PLAYER_LOGIN_QUERY_LOADSPELLCOOLDOWNS           = 15,
    PLAYER_LOGIN_QUERY_LOADDECLINEDNAMES            = 16,
    PLAYER_LOGIN_QUERY_LOADGUILD                    = 17,
    PLAYER_LOGIN_QUERY_LOADACCOUNTMOUNTS            = 18,
    PLAYER_LOGIN_QUERY_LOADACHIEVEMENTS             = 19,
    PLAYER_LOGIN_QUERY_LOADACCOUNTACHIEVEMENTS      = 20,
    PLAYER_LOGIN_QUERY_LOADCRITERIAPROGRESS         = 21,
    PLAYER_LOGIN_QUERY_LOADACCOUNTCRITERIAPROGRESS  = 22,
    PLAYER_LOGIN_QUERY_LOADEQUIPMENTSETS            = 23,
    PLAYER_LOGIN_QUERY_LOADBGDATA                   = 24,
    PLAYER_LOGIN_QUERY_LOADGLYPHS                   = 25,
    PLAYER_LOGIN_QUERY_LOADTALENTS                  = 26,
    PLAYER_LOGIN_QUERY_LOADACCOUNTDATA              = 27,
    PLAYER_LOGIN_QUERY_LOADSKILLS                   = 28,
    PLAYER_LOGIN_QUERY_LOADWEEKLYQUESTSTATUS        = 29,
    PLAYER_LOGIN_QUERY_LOADRANDOMBG                 = 30,
    PLAYER_LOGIN_QUERY_LOADBANNED                   = 31,
    PLAYER_LOGIN_QUERY_LOADQUESTSTATUSREW           = 32,
    PLAYER_LOGIN_QUERY_LOADINSTANCELOCKTIMES        = 33,
    PLAYER_LOGIN_QUERY_LOADSEASONALQUESTSTATUS      = 34,
    PLAYER_LOGIN_QUERY_LOADVOIDSTORAGE              = 35,
    PLAYER_LOGIN_QUERY_LOADCURRENCY                 = 36,
    PLAYER_LOGIN_QUERY_LOAD_CUF_PROFILES            = 37,
    PLAYER_LOGIN_QUERY_LOAD_BATTLE_PETS             = 38,
    PLAYER_LOGIN_QUERY_LOAD_BATTLE_PET_SLOTS        = 39,
    PLAYER_LOGIN_QUERY_LOADARCHAELOGY               = 40,
    PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY_FINDS       = 41,
    PLAYER_LOGIN_QUERY_LOAD_PERSONAL_RATE           = 42,
    PLAYER_LOGIN_QUERY_HONOR                        = 43,
    PLAYER_LOGIN_QUERY_LOAD_VISUAL                  = 44,
    PLAYER_LOGIN_QUERY_LOAD_LOOTCOOLDOWN            = 45,
    PLAYER_LOGIN_QUERY_LOAD_QUEST_STATUS_OBJECTIVES = 46,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON_BLUEPRINTS,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON_BUILDINGS,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON_FOLLOWERS,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON_FOLLOWER_ABILITIES,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON_MISSIONS,
    PLAYER_LOGIN_QUERY_LOAD_GARRISON_SHIPMENTS,
    PLAYER_LOGIN_QUERY_LOAD_TOYS,
    PLAYER_LOGIN_QUERY_LOAD_HEIRLOOMS,
    PLAYER_LOGIN_QUERY_LOAD_TRANSMOGS,
    PLAYER_LOGIN_QUERY_HONOR_INFO,
    PLAYER_LOGIN_QUERY_ARTIFACTS,

    MAX_PLAYER_LOGIN_QUERY
};

enum PlayerDelayedOperations
{
    DELAYED_SAVE_PLAYER         = 0x01,
    DELAYED_RESURRECT_PLAYER    = 0x02,
    DELAYED_SPELL_CAST_DESERTER = 0x04,
    DELAYED_BG_MOUNT_RESTORE    = 0x08,                     ///< Flag to restore mount state after teleport from BG
    DELAYED_BG_TAXI_RESTORE     = 0x10,                     ///< Flag to restore taxi state after teleport from BG
    DELAYED_BG_GROUP_RESTORE    = 0x20,                     ///< Flag to restore group state after teleport from BG
    DELAYED_UPDATE_AFTER_TO_BG  = 0x40,                     ///< Flag to update aura effect after teleport to BG
    DELAYED_END
};

// Player summoning auto-decline time (in secs)
#define MAX_PLAYER_SUMMON_DELAY                   (2*MINUTE)
#define MAX_MONEY_AMOUNT               (UI64LIT(9999999999)) // TODO: Move this restriction to worldserver.conf, default to this value, hardcap at uint64.max

struct InstancePlayerBind
{
    InstanceSave* save;
    bool perm;
    /* permanent PlayerInstanceBinds are created in Raid/Heroic instances for players
       that aren't already permanently bound when they are inside when a boss is killed
       or when they enter an instance that the group leader is permanently bound to. */
    InstancePlayerBind() : save(NULL), perm(false) {}
};

enum DungeonStatusFlag
{
    DUNGEON_STATUSFLAG_NORMAL = 0x01,
    DUNGEON_STATUSFLAG_HEROIC = 0x02,

    RAID_STATUSFLAG_10MAN_NORMAL = 0x01,
    RAID_STATUSFLAG_25MAN_NORMAL = 0x02,
    RAID_STATUSFLAG_10MAN_HEROIC = 0x04,
    RAID_STATUSFLAG_25MAN_HEROIC = 0x08
};

struct AccessRequirement
{
    int32  mapid;
    uint8  difficulty;
    uint16 dungeonId;
    uint8  levelMin;
    uint8  levelMax;
    uint16 item_level;
    uint32 item;
    uint32 item2;
    uint32 quest_A;
    uint32 quest_H;
    uint32 achievement;
    uint32 achievement_A;
    std::string questFailedText;
};

enum CharDeleteMethod
{
    CHAR_DELETE_REMOVE = 0,                      // Completely remove from the database
    CHAR_DELETE_UNLINK = 1                       // The character gets unlinked from the account,
                                                 // the name gets freed up and appears as deleted ingame
};

enum ReferAFriendError
{
    ERR_REFER_A_FRIEND_NONE                          = 0x00,
    ERR_REFER_A_FRIEND_NOT_REFERRED_BY               = 0x01,
    ERR_REFER_A_FRIEND_TARGET_TOO_HIGH               = 0x02,
    ERR_REFER_A_FRIEND_INSUFFICIENT_GRANTABLE_LEVELS = 0x03,
    ERR_REFER_A_FRIEND_TOO_FAR                       = 0x04,
    ERR_REFER_A_FRIEND_DIFFERENT_FACTION             = 0x05,
    ERR_REFER_A_FRIEND_NOT_NOW                       = 0x06,
    ERR_REFER_A_FRIEND_GRANT_LEVEL_MAX_I             = 0x07,
    ERR_REFER_A_FRIEND_NO_TARGET                     = 0x08,
    ERR_REFER_A_FRIEND_NOT_IN_GROUP                  = 0x09,
    ERR_REFER_A_FRIEND_SUMMON_LEVEL_MAX_I            = 0x0A,
    ERR_REFER_A_FRIEND_SUMMON_COOLDOWN               = 0x0B,
    ERR_REFER_A_FRIEND_INSUF_EXPAN_LVL               = 0x0C,
    ERR_REFER_A_FRIEND_SUMMON_OFFLINE_S              = 0x0D,
    ERR_REFER_A_FRIEND_NO_XREALM                     = 0x0E,
};

enum PlayerRestState
{
    REST_STATE_RESTED                                = 0x01,
    REST_STATE_NOT_RAF_LINKED                        = 0x02,
    REST_STATE_RAF_LINKED                            = 0x06
};

enum PlayerCommandStates
{
    CHEAT_NONE          = 0x00,
    CHEAT_GOD           = 0x01,
    CHEAT_CASTTIME      = 0x02,
    CHEAT_COOLDOWN      = 0x04,
    CHEAT_POWER         = 0x08,
    CHEAT_WATERWALK     = 0x10,
    CHEAT_ALL_SPELLS    = 0x20,
};

struct auraEffectData
{
    auraEffectData(uint8 slot, uint8 effect, uint32 amount, uint32 baseamount) : _slot(slot), _effect(effect), _amount(amount), _baseamount(amount)  {}
    uint8 _slot;
    uint8 _effect;
    uint32 _amount;
    uint32 _baseamount;
};

struct playerLootCooldown
{
    uint32 entry;
    uint8 type;
    uint32 difficultyMask;
    uint32 respawnTime;
    bool state;
};

typedef std::unordered_map<uint32, playerLootCooldown> PlayerLootCooldownMap;

enum PlayerLootCooldownType
{
    TYPE_GO          = 0,
    TYPE_CREATURE    = 1,
    TYPE_SPELL       = 2,
};

#define MAX_LOOT_COOLDOWN_TYPE 3

class Player;

/// Holder for Battleground data
struct BGData
{
    BGData() : bgInstanceID(0), bgTypeID(BATTLEGROUND_TYPE_NONE), bgAfkReportedCount(0), bgAfkReportedTimer(0),
        bgTeam(0), mountSpell(0) { bgQueuesJoinedTime.clear(); ClearTaxiPath(); }

    uint32 bgInstanceID;                    ///< This variable is set to bg->m_InstanceID,
                                            ///  when player is teleported to BG - (it is battleground's GUID)
    BattlegroundTypeId bgTypeID;

    std::map<uint32, uint32> bgQueuesJoinedTime;

    std::set<uint32>   bgAfkReporter;
    uint8              bgAfkReportedCount;
    time_t             bgAfkReportedTimer;

    uint32 bgTeam;                          ///< What side the player will be added to

    uint32 mountSpell;
    uint32 taxiPath[MAX_TEAMS];

    WorldLocation joinPos;                  ///< From where player entered BG

    void ClearTaxiPath()     { taxiPath[0] = taxiPath[1] = 0; }
    bool HasTaxiPath() const { return taxiPath[0] && taxiPath[1]; }
};

struct VoidStorageItem
{
    VoidStorageItem() : ItemId(0), ItemEntry(0), ItemRandomPropertyId(0), ItemSuffixFactor(0), ItemUpgradeId(0), change(false), deleted(false) { }
    VoidStorageItem(uint64 id, uint32 entry, ObjectGuid const& creator, uint32 randomPropertyId, uint32 suffixFactor, uint32 upgradeId, std::vector<uint32> const& bonuses, bool _change)
        : ItemId(id), ItemEntry(entry), CreatorGuid(creator), ItemRandomPropertyId(randomPropertyId),
        ItemSuffixFactor(suffixFactor), ItemUpgradeId(upgradeId), change(_change) { BonusListIDs.insert(BonusListIDs.end(), bonuses.begin(), bonuses.end()); }

    VoidStorageItem(uint64 id, uint32 entry, ObjectGuid const& creator, uint32 randomPropertyId, uint32 suffixFactor, bool _change)
        : ItemId(id), ItemEntry(entry), CreatorGuid(creator), ItemRandomPropertyId(randomPropertyId),
        ItemSuffixFactor(suffixFactor), change(_change) { }

    VoidStorageItem(VoidStorageItem&& vsi, bool _change) : ItemId(vsi.ItemId), ItemEntry(vsi.ItemEntry), CreatorGuid(vsi.CreatorGuid), ItemRandomPropertyId(vsi.ItemRandomPropertyId),
        ItemSuffixFactor(vsi.ItemSuffixFactor), ItemUpgradeId(vsi.ItemUpgradeId), BonusListIDs(std::move(vsi.BonusListIDs)), change(_change) { }

    uint64 ItemId;
    uint32 ItemEntry;
    ObjectGuid CreatorGuid;
    uint32 ItemRandomPropertyId;
    uint32 ItemSuffixFactor;
    bool change;
    bool deleted;
    uint32 ItemUpgradeId;
    std::vector<int32> BonusListIDs;
};

#define MAX_CUF_PROFILES 5

enum CUFOptions
{
    CUF_KEEP_GROUPS_TOGETHER,
    CUF_DISPLAY_PETS,
    CUF_DISPLAY_MAIN_TANK_AND_ASSIST,
    CUF_DISPLAY_HEAL_PREDICTION,
    CUF_DISPLAY_AGGRO_HIGHLIGHT,
    CUF_DISPLAY_ONLY_DISPELLABLE_DEBUFFS,
    CUF_DISPLAY_POWER_BAR,
    CUF_DISPLAY_BORDER,
    CUF_USE_CLASS_COLORS,
    CUF_DISPLAY_HORIZONTAL_GROUPS,
    CUF_DISPLAY_NON_BOSS_DEBUFFS,
    CUF_DYNAMIC_POSITION,
    CUF_LOCKED,
    CUF_SHOWN,
    CUF_AUTO_ACTIVATE_2_PLAYERS,
    CUF_AUTO_ACTIVATE_3_PLAYERS,
    CUF_AUTO_ACTIVATE_5_PLAYERS,
    CUF_AUTO_ACTIVATE_10_PLAYERS,
    CUF_AUTO_ACTIVATE_15_PLAYERS,
    CUF_AUTO_ACTIVATE_25_PLAYERS,
    CUF_AUTO_ACTIVATE_40_PLAYERS,
    CUF_AUTO_ACTIVATE_SPEC_1,
    CUF_AUTO_ACTIVATE_SPEC_2,
    CUF_AUTO_ACTIVATE_PVP,
    CUF_AUTO_ACTIVATE_PVE,

    CUF_BOOL_OPTIONS_COUNT
};

struct CUFProfile
{
    CUFProfile() : ProfileName(), BoolOptions()
    {
        FrameHeight = 0;
        FrameWidth = 0;
        SortBy = 0;
        HealthText = 0;
        TopPoint = 0;
        BottomPoint = 0;
        LeftPoint = 0;
        TopOffset = 0;
        BottomOffset = 0;
        LeftOffset = 0;
    }

    CUFProfile(std::string const& name, uint16 frameHeight, uint16 frameWidth, uint8 sortBy, uint8 healthText, uint32 boolOptions,
               uint8 topPoint, uint8 bottomPoint, uint8 leftPoint, uint16 topOffset, uint16 bottomOffset, uint16 leftOffset)
               : ProfileName(name), BoolOptions(int(boolOptions))
    {
        FrameHeight = frameHeight;
        FrameWidth = frameWidth;
        SortBy = sortBy;
        HealthText = healthText;
        TopPoint = topPoint;
        BottomPoint = bottomPoint;
        LeftPoint = leftPoint;
        TopOffset = topOffset;
        BottomOffset = bottomOffset;
        LeftOffset = leftOffset;
    }

    std::string ProfileName;
    uint16 FrameHeight;
    uint16 FrameWidth;
    uint16 TopOffset;
    uint16 BottomOffset;
    uint16 LeftOffset;
    uint8 SortBy;
    uint8 HealthText;
    uint8 TopPoint;
    uint8 BottomPoint;
    uint8 LeftPoint;

    std::bitset<CUF_BOOL_OPTIONS_COUNT> BoolOptions;
};

class TradeData
{
    public:                                                 // constructors
        TradeData(Player* player, Player* trader) :
            m_player(player),  m_trader(trader), m_accepted(false), m_acceptProccess(false),
            m_money(0), m_spell(0), m_spellCastItem(), m_clientStateIndex(1), m_serverStateIndex(1) { memset(m_items, 0, TRADE_SLOT_COUNT * sizeof(uint64)); }

        Player* GetTrader() const { return m_trader; }
        TradeData* GetTraderData() const;

        Item* GetItem(TradeSlots slot) const;
        bool HasItem(ObjectGuid itemGuid) const;
        TradeSlots GetTradeSlotForItem(ObjectGuid itemGuid) const;
        void SetItem(TradeSlots slot, Item* item);

        uint32 GetSpell() const { return m_spell; }
        void SetSpell(uint32 spell_id, Item* castItem = NULL);

        Item*  GetSpellCastItem() const;
        bool HasSpellCastItem() const { return m_spellCastItem; }

        uint64 GetMoney() const { return m_money; }
        void SetMoney(uint64 money);

        bool IsAccepted() const { return m_accepted; }
        void SetAccepted(bool state, bool crosssend = false);

        bool IsInAcceptProcess() const { return m_acceptProccess; }
        void SetInAcceptProcess(bool state) { m_acceptProccess = state; }

        uint32 GetClientStateIndex() const { return m_clientStateIndex; }
        void UpdateClientStateIndex() { ++m_clientStateIndex; }

        uint32 GetServerStateIndex() const { return m_serverStateIndex; }
        void UpdateServerStateIndex() { m_serverStateIndex = rand32(); }

    private:                                                // internal functions

        void Update(bool for_trader = true);

    private:                                                // fields

        Player*    m_player;                                // Player who own of this TradeData
        Player*    m_trader;                                // Player who trade with m_player

        bool       m_accepted;                              // m_player press accept for trade list
        bool       m_acceptProccess;                        // one from player/trader press accept and this processed

        uint64     m_money;                                 // m_player place money to trade

        uint32     m_spell;                                 // m_player apply spell to non-traded slot item
        ObjectGuid m_spellCastItem;                         // applied spell casted by item use

        ObjectGuid m_items[TRADE_SLOT_COUNT];               // traded items from m_player side including non-traded slot

        uint32     m_clientStateIndex;
        uint32     m_serverStateIndex;
};

struct ResurrectionData
{
    ObjectGuid GUID;
    WorldLocation Location;
    uint32 Health;
    uint32 Mana;
    uint32 Aura;
};

static uint8 const DefaultTalentRowLevels[MAX_TALENT_TIERS] = { 15, 30, 45, 60, 75, 90, 100 };
static uint8 const DKTalentRowLevels[MAX_TALENT_TIERS] = { 56, 57, 58, 60, 75, 90, 100 };
static uint8 const DHTalentRowLevels[MAX_TALENT_TIERS] = { 99, 100, 102, 104, 106, 108, 110 };

struct PlayerTalentInfo
{
    PlayerTalentInfo() :
        QuestRewardedTalentCount(0),
        ResetTalentsCost(0), ResetTalentsTime(0), ResetSpecializationCost(0),
        SpecIndex(0), ResetSpecializationTime(0), SpecializationId(0)
    {
        for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i)
        {
            SpecInfo[i].Talents = new PlayerTalentMap();
            memset(SpecInfo[i].Glyphs, 0, MAX_GLYPH_SLOT_INDEX * sizeof(uint32));
        }
    }

    ~PlayerTalentInfo()
    {
        for (uint8 i = 0; i < MAX_TALENT_SPECS; ++i)
            delete SpecInfo[i].Talents;
    }

    struct TalentSpecInfo
    {
        PlayerTalentMap* Talents;
        uint32 Glyphs[MAX_GLYPH_SLOT_INDEX];
    } SpecInfo[MAX_TALENT_SPECS];

    uint32 QuestRewardedTalentCount;
    uint32 ResetTalentsCost;
    time_t ResetTalentsTime;
    uint32 ResetSpecializationCost;
    time_t ResetSpecializationTime;
    uint8 SpecIndex;
    uint32 SpecializationId;

private:
    PlayerTalentInfo(PlayerTalentInfo const&);
};

typedef std::unordered_map<uint8, Bracket*> BracketList;

struct PetData
{
    PetData() : slot(0), petnumber(0), creature(0), lvl(0), modelid(0) {}
    int32 slot;
    uint32 petnumber;
    uint32 creature;
    uint32 lvl;
    uint32 modelid;
    std::string name;
};
typedef std::set<PetData*> PetDataList;
typedef std::vector<uint32/*PetEntry*/> PlayerPetSlotList;

struct DigSite
{
    uint8 count;
    uint16 site_id;
    uint32 find_id;
    float loot_x;
    float loot_y;

    void clear()
    {
        site_id = find_id = 0;
        loot_x = loot_y = 0.0f;
        count = 0;
    }

    bool empty() { return site_id == 0; }
};

struct CompletedProject
{
    CompletedProject() : entry(NULL), count(1), date(time(NULL)) { }
    CompletedProject(ResearchProjectEntry const* _entry) : entry(_entry), count(1), date(time(NULL)) { }

    ResearchProjectEntry const* entry;
    uint32 count;
    uint32 date;
};

typedef std::set<uint32> ResearchSiteSet;
typedef std::list<CompletedProject> CompletedProjectList;
typedef std::set<uint32> ResearchProjectSet;

#define MAX_RESEARCH_SITES 20
#define MAX_DIGSITE_FINDS 6

struct Visuals
{
    Visuals() : m_visHead(1), m_visShoulders(1), m_visChest(1), m_visWaist(1), m_visLegs(1), m_visFeet(1),
    m_visWrists(1), m_visHands(1), m_visBack(1), m_visMainhand(1), m_visOffhand(1), m_visRanged(1), m_visTabard(1), m_visShirt(1), m_altVis(0) { }

    uint32 m_visHead;
    uint32 m_visShoulders;
    uint32 m_visChest;
    uint32 m_visWaist;
    uint32 m_visLegs;
    uint32 m_visFeet;
    uint32 m_visWrists;
    uint32 m_visHands;
    uint32 m_visBack;
    uint32 m_visMainhand;
    uint32 m_visOffhand;
    uint32 m_visRanged;
    uint32 m_visTabard;
    uint32 m_visShirt;

    uint8 m_altVis;
};

typedef std::unordered_map<ObjectGuid/*owner*/, ObjectGuid/*lGUID*/> AoeMap;

class Player : public Unit, public GridObject<Player>
{
    friend class WorldSession;
    friend class BattlePayMgr;
    friend void Item::AddToUpdateQueueOf(Player* player);
    friend void Item::RemoveFromUpdateQueueOf(Player* player);
    public:
        explicit Player (WorldSession* session);
        ~Player();

        void CleanupsBeforeDelete(bool finalCleanup = true);

        void AddToWorld();
        void RemoveFromWorld();

        bool TeleportTo(uint32 mapid, float x, float y, float z, float orientation, uint32 options = 0, uint32 spellID = 0);
        bool TeleportTo(uint32 mapid, Position const* pos, uint32 options = 0, uint32 spellID = 0)
        {
            return TeleportTo(mapid, pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), pos->GetOrientation(), options, spellID);
        }
        bool TeleportTo(WorldLocation const &loc, uint32 options = 0)
        {
            return TeleportTo(loc.GetMapId(), loc.GetPositionX(), loc.GetPositionY(), loc.GetPositionZ(), loc.GetOrientation(), options);
        }
        bool TeleportToBGEntryPoint();
        void TeleportToHomeBind()
        {
            TeleportTo(m_homebindMapId, m_homebindX, m_homebindY, m_homebindZ, GetOrientation());
        }
        bool IsForbiddenMapForLevel(uint32 mapid, uint32 zone);
        bool IsLoXpMap(uint32 map);

        void SetSummonPoint(uint32 mapid, float x, float y, float z)
        {
            m_summon_expire = time(NULL) + MAX_PLAYER_SUMMON_DELAY;
            m_summon_mapid = mapid;
            m_summon_x = x;
            m_summon_y = y;
            m_summon_z = z;
        }
        void SummonIfPossible(bool agree);

        bool Create(ObjectGuid::LowType guidlow, WorldPackets::Character::CharacterCreateInfo* createInfo);

        void Update(uint32 time);

        void SetInWater(bool apply);

        bool IsInWater() const { return m_isInWater; }
        bool IsUnderWater() const;
        bool IsFalling() { return GetPositionZ() < m_lastFallZ; }

        void SendInitialPacketsBeforeAddToMap();
        void SendInitialPacketsAfterAddToMap();
        void SendTransferAborted(uint32 mapid, TransferAbortReason reasonId, uint8 arg = 0);
        void SendInstanceResetWarning(uint32 mapid, Difficulty difficulty, uint32 time);

        bool CanInteractWithQuestGiver(Object* questGiver);
        Creature* GetNPCIfCanInteractWith(ObjectGuid guid, uint32 npcflagmask, uint32 npcflagmask2 = 0);
        GameObject* GetGameObjectIfCanInteractWith(ObjectGuid guid, GameobjectTypes type) const;

        bool ToggleAFK();
        bool ToggleDND();
        bool isAFK() const { return HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AFK); }
        bool isDND() const { return HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_DND); }
        bool IsFFAPvP() const { return HasByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP); }
        uint8 GetChatTag() const;
        std::string afkMsg;
        std::string dndMsg;

        Visuals *m_vis;

        uint32 GetBarberShopCost(uint8 newhairstyle, uint8 newhaircolor, uint8 newfacialhair, BarberShopStyleEntry const* newSkin=NULL);

        PlayerSocial *GetSocial() { return m_social; }

        PlayerTaxi m_taxi;
        void InitTaxiNodesForLevel() { m_taxi.InitTaxiNodesForLevel(getRace(), getClass(), getLevel()); }
        bool ActivateTaxiPathTo(std::vector<uint32> const& nodes, Creature* npc = NULL, uint32 spellid = 0);
        bool ActivateTaxiPathTo(uint32 taxi_path_id, uint32 spellid = 0);
        void CleanupAfterTaxiFlight();
        void ContinueTaxiFlight();
                                                            // mount_id can be used in scripting calls
        bool isAcceptWhispers() const { return (m_ExtraFlags & PLAYER_EXTRA_ACCEPT_WHISPERS) != 0; }
        void SetAcceptWhispers(bool on) { if (on) m_ExtraFlags |= PLAYER_EXTRA_ACCEPT_WHISPERS; else m_ExtraFlags &= ~PLAYER_EXTRA_ACCEPT_WHISPERS; }
        bool isGameMaster() const { return (m_ExtraFlags & PLAYER_EXTRA_GM_ON) != 0; }
        void SetGameMaster(bool on);
        bool isGMChat() const { return (m_ExtraFlags & PLAYER_EXTRA_GM_CHAT) != 0; }
        void SetGMChat(bool on) { if (on) m_ExtraFlags |= PLAYER_EXTRA_GM_CHAT; else m_ExtraFlags &= ~PLAYER_EXTRA_GM_CHAT; }
        bool isTaxiCheater() const { return (m_ExtraFlags & PLAYER_EXTRA_TAXICHEAT) != 0; }
        void SetTaxiCheater(bool on) { if (on) m_ExtraFlags |= PLAYER_EXTRA_TAXICHEAT; else m_ExtraFlags &= ~PLAYER_EXTRA_TAXICHEAT; }
        bool isGMVisible() const { return !(m_ExtraFlags & PLAYER_EXTRA_GM_INVISIBLE); }
        void SetGMVisible(bool on);
        void SetPvPDeath(bool on) { if (on) m_ExtraFlags |= PLAYER_EXTRA_PVP_DEATH; else m_ExtraFlags &= ~PLAYER_EXTRA_PVP_DEATH; }

        void GiveXP(uint32 xp, Unit* victim, float group_rate=1.0f);
        void GiveGatheringXP();
        void GiveLevel(uint8 level);

        void InitStatsForLevel(bool reapplyMods = false);

// Size of client completed quests bit map
#define QUESTS_COMPLETED_BITS_SIZE 625

        // .cheat command related
        bool GetCommandStatus(uint32 command) const { return (_activeCheats & command) != 0; }
        void SetCommandStatusOn(uint32 command) { _activeCheats |= command; }
        void SetCommandStatusOff(uint32 command) { _activeCheats &= ~command; }

        // Played Time Stuff
        time_t m_logintime;
        time_t m_Last_tick;
        uint32 m_Played_time[MAX_PLAYED_TIME_INDEX];
        uint32 GetTotalPlayedTime() { return m_Played_time[PLAYED_TIME_TOTAL]; }
        uint32 GetLevelPlayedTime() { return m_Played_time[PLAYED_TIME_LEVEL]; }

        void setDeathState(DeathState s);                   // overwrite Unit::setDeathState

        void InnEnter (time_t time, uint32 mapid, float x, float y, float z)
        {
            inn_pos_mapid = mapid;
            inn_pos_x = x;
            inn_pos_y = y;
            inn_pos_z = z;
            time_inn_enter = time;
        }

        float GetRestBonus() const { return m_rest_bonus; }
        void SetRestBonus(float rest_bonus_new);

        RestType GetRestType() const { return rest_type; }
        void SetRestType(RestType n_r_type) { rest_type = n_r_type; }

        uint32 GetInnPosMapId() const { return inn_pos_mapid; }
        float GetInnPosX() const { return inn_pos_x; }
        float GetInnPosY() const { return inn_pos_y; }
        float GetInnPosZ() const { return inn_pos_z; }

        time_t GetTimeInnEnter() const { return time_inn_enter; }
        void UpdateInnerTime (time_t time) { time_inn_enter = time; }

        Pet* GetPet() const;
        Pet* SummonPet(uint32 entry, float x, float y, float z, float ang, PetType petType, uint32 despwtime, uint32 spellId = 0, bool stampeded = false);
        void RemovePet(Pet* pet, bool isDelete = false);

        PhaseMgr& GetPhaseMgr() { return phaseMgr; }

        void Say(const std::string& text, const uint32 language);
        void Yell(const std::string& text, const uint32 language);
        void TextEmote(const std::string& text);
        void Whisper(const std::string& text, const uint32 language, ObjectGuid receiver);
        void WhisperAddon(const std::string& text, const std::string& prefix, Player* receiver);
        void BuildPlayerChat(WorldPacket* data, uint8 msgtype, const std::string& text, uint32 language, const char* addonPrefix = NULL) const;
        void BuildPlayerChatData(Trinity::ChatData& c, uint8 msgtype, const std::string& text, uint32 language, const char* addonPrefix = NULL) const;

        /*********************************************************/
        /***                    STORAGE SYSTEM                 ***/
        /*********************************************************/

        void SetVirtualItemSlot(uint8 i, Item* item);
        void SetSheath(SheathState sheathed);             // overwrite Unit version
        uint8 FindEquipSlot(ItemTemplate const* proto, uint32 slot, bool swap) const;
        uint32 GetItemCount(uint32 item, bool inBankAlso = false, Item* skipItem = NULL) const;
        uint32 GetItemCountWithLimitCategory(uint32 limitCategory, Item* skipItem = NULL) const;
        Item* GetItemByGuid(ObjectGuid guid) const;
        Item* GetItemByEntry(uint32 entry) const;
        std::vector<Item*> GetItemListByEntry(uint32 entry, bool inBankAlso = false) const;
        Item* GetItemByPos(uint16 pos) const;
        Item* GetItemByPos(uint8 bag, uint8 slot) const;
        Bag*  GetBagByPos(uint8 slot) const;
        inline Item* GetUseableItemByPos(uint8 bag, uint8 slot) const //Does additional check for disarmed weapons
        {
            if (!CanUseAttackType(GetAttackBySlot(slot)))
                return NULL;
            return GetItemByPos(bag, slot);
        }
        Item* GetWeaponForAttack(WeaponAttackType attackType, bool useable = false) const;
        Item* GetShield(bool useable = false) const;
        static uint8 GetAttackBySlot(uint8 slot);        // MAX_ATTACK if not weapon slot
        std::vector<Item*> &GetItemUpdateQueue() { return m_itemUpdateQueue; }
        static bool IsInventoryPos(uint16 pos) { return IsInventoryPos(pos >> 8, pos & 255); }
        static bool IsInventoryPos(uint8 bag, uint8 slot);
        static bool IsEquipmentPos(uint16 pos) { return IsEquipmentPos(pos >> 8, pos & 255); }
        static bool IsEquipmentPos(uint8 bag, uint8 slot);
        static bool IsBagPos(uint16 pos);
        static bool IsBankPos(uint16 pos) { return IsBankPos(pos >> 8, pos & 255); }
        static bool IsBankPos(uint8 bag, uint8 slot);
        bool IsValidPos(uint16 pos, bool explicit_pos) { return IsValidPos(pos >> 8, pos & 255, explicit_pos); }
        bool IsValidPos(uint8 bag, uint8 slot, bool explicit_pos);
        uint8 GetBankBagSlotCount() const { return GetByteValue(PLAYER_BYTES_2, 2); }
        void SetBankBagSlotCount(uint8 count) { SetByteValue(PLAYER_BYTES_2, 2, count); }
        bool HasItemCount(uint32 item, uint32 count = 1, bool inBankAlso = false) const;
        bool HasItemFitToSpellRequirements(SpellInfo const* spellInfo, Item const* ignoreItem = NULL);
        bool CanNoReagentCast(SpellInfo const* spellInfo) const;
        bool HasItemOrGemWithIdEquipped(uint32 item, uint32 count, uint8 except_slot = NULL_SLOT) const;
        bool HasItemOrGemWithLimitCategoryEquipped(uint32 limitCategory, uint32 count, uint8 except_slot = NULL_SLOT) const;
        InventoryResult CanTakeMoreSimilarItems(Item* pItem) const { return CanTakeMoreSimilarItems(pItem->GetEntry(), pItem->GetCount(), pItem); }
        InventoryResult CanTakeMoreSimilarItems(uint32 entry, uint32 count) const { return CanTakeMoreSimilarItems(entry, count, NULL); }
        InventoryResult CanStoreNewItem(uint8 bag, uint8 slot, ItemPosCountVec& dest, uint32 item, uint32 count, uint32* no_space_count = NULL) const
        {
            return CanStoreItem(bag, slot, dest, item, count, NULL, false, no_space_count);
        }
        InventoryResult CanStoreItem(uint8 bag, uint8 slot, ItemPosCountVec& dest, Item* pItem, bool swap = false) const
        {
            if (!pItem)
                return EQUIP_ERR_ITEM_NOT_FOUND;
            uint32 count = pItem->GetCount();
            return CanStoreItem(bag, slot, dest, pItem->GetEntry(), count, pItem, swap, NULL);

        }
        InventoryResult CanStoreItems(Item** pItem, int count) const;
        InventoryResult CanEquipNewItem(uint8 slot, uint16& dest, uint32 item, bool swap, bool not_loading = true) const;
        InventoryResult CanEquipItem(uint8 slot, uint16& dest, Item* pItem, bool swap, bool not_loading = true) const;

        InventoryResult CanEquipUniqueItem(Item* pItem, uint8 except_slot = NULL_SLOT, uint32 limit_count = 1) const;
        InventoryResult CanEquipUniqueItem(ItemTemplate const* itemProto, uint8 except_slot = NULL_SLOT, uint32 limit_count = 1) const;
        InventoryResult CanUnequipItems(uint32 item, uint32 count) const;
        InventoryResult CanUnequipItem(uint16 src, bool swap) const;
        InventoryResult CanBankItem(uint8 bag, uint8 slot, ItemPosCountVec& dest, Item* pItem, bool swap, bool not_loading = true) const;
        InventoryResult CanUseItem(Item* pItem, bool not_loading = true) const;
        bool HasItemTotemCategory(uint32 TotemCategory) const;
        InventoryResult CanUseItem(ItemTemplate const* pItem) const;
        InventoryResult CanUseAmmo(uint32 item) const;
        InventoryResult CanRollForItemInLFG(ItemTemplate const* item, WorldObject const* lootedObject) const;
        Item* StoreNewItem(ItemPosCountVec const& pos, uint32 item, bool update, int32 randomPropertyId = 0, GuidSet const& allowedLooters = GuidSet(), std::vector<int32> const& bonusListIDs = std::vector<int32>());
        Item* StoreItem(ItemPosCountVec const& pos, Item* pItem, bool update);
        Item* EquipNewItem(uint16 pos, uint32 item, bool update);
        Item* EquipItem(uint16 pos, Item* pItem, bool update);
        void AutoUnequipOffhandIfNeed(bool force = false);
        bool StoreNewItemInBestSlots(uint32 item_id, uint32 item_count, bool not_loading = true);
        bool AutoStoreLoot(uint8 bag, uint8 slot, uint32 loot_id, LootStore const& store, uint32 filterLevel = 0, bool broadcast = true);
        bool AutoStoreLoot(uint32 loot_id, LootStore const& store, uint32 filterLevel = 0, bool broadcast = true) { return AutoStoreLoot(NULL_BAG, NULL_SLOT, loot_id, store, filterLevel, broadcast); }
        void StoreLootItem(uint8 lootSlot, Loot* loot);
        void SortBags();
        Loot personalLoot;

        InventoryResult CanTakeMoreSimilarItems(uint32 entry, uint32 count, Item* pItem, uint32* no_space_count = NULL) const;
        InventoryResult CanStoreItem(uint8 bag, uint8 slot, ItemPosCountVec& dest, uint32 entry, uint32 count, Item* pItem = NULL, bool swap = false, uint32* no_space_count = NULL) const;

        void AddRefundReference(ObjectGuid it);
        void DeleteRefundReference(ObjectGuid it);

        /// send full data about all currencies to client
        void SendCurrencies();
        void SendPvpRewards();
        /// return count of currency witch has plr
        uint32 GetCurrency(uint32 id) const;
        uint32 GetCurrencyOnWeek(uint32 id) const;
        uint32 GetCurrencyOnSeason(uint32 id) const;
        /// return presence related currency
        bool HasCurrency(uint32 id, uint32 count) const;
        bool HasCurrencySeason(uint32 id, uint32 count) const { return GetCurrencyOnSeason(id) >= count; }
        void SetCurrency(uint32 id, uint32 count, bool printLog = true);
        uint32 GetCurrencyWeekCap(uint32 id);
        void ResetCurrencyWeekCap();
        uint32 GetCurrencyWeekCap(CurrencyTypesEntry const* currency);
        void UpdateConquestCurrencyCap(uint32 currency);
        /// modify currency flag
        void ModifyCurrencyFlag(uint32 id, uint8 flag);

        /**
        * @name ModifyCurrency
        * @brief Change specific currency and send result to client

        * @param id currency entry from CurrencyTypes.dbc
        * @param count integer value for adding/removing current currency
        * @param printLog used on SMSG_SET_CURRENCY
        * @param ignore gain multipliers
        */

        void ModifyCurrency(uint32 id, int32 count, bool printLog = true, bool ignoreMultipliers = false, bool modifyWeek = true, bool modifySeason = true, bool sendToast = false);

        /*********************************************************/
        /***                 ARCHAEOLOGY SYSTEM                ***/
        /*********************************************************/

        void _SaveArchaeology(SQLTransaction& trans);
        void _LoadArchaeology(PreparedQueryResult result);
        void _LoadArchaeologyFinds(PreparedQueryResult result);
        bool HasResearchSite(uint32 id) const
        {
            return _researchSites.find(id) != _researchSites.end();
        }

        bool HasResearchProjectOfBranch(uint32 id) const;
        bool HasResearchProject(uint32 id) const;
        void ReplaceResearchProject(uint32 oldId, uint32 newId);

        static float GetRareArtifactChance(uint32 skillValue);

        void ShowResearchSites();
        void GenerateResearchSites();
        void GenerateResearchSiteInMap(uint32 mapId);
        void GenerateResearchProjects();
        bool SolveResearchProject(uint32 spellId, SpellCastTargets& targets);
        void RandomizeSitesInMap(uint32 mapId, uint8 count);
        bool TeleportToDigsiteInMap(uint32 mapId);
        static bool IsPointInZone(ResearchPOIPoint &test, ResearchPOIPointVector &polygon);
        uint16 GetResearchSiteID();
        bool OnSurvey(uint32& entry, float& x, float& y, float& z, float &orientation);
        bool CanResearchWithLevel(uint32 site_id);
        uint8 CanResearchWithSkillLevel(uint32 site_id);
        bool GenerateDigSiteLoot(uint16 zoneid, DigSite &site);
        uint32 AddCompletedProject(ResearchProjectEntry const* entry);
        bool IsCompletedProject(uint32 id, bool onlyRare);
        void SendCompletedProjects();
        void SendSurveyCast(uint32 count, uint32 max, uint32 branchId, bool completed);

        DigSite _digSites[MAX_RESEARCH_SITES];
        ResearchSiteSet _researchSites;
        CompletedProjectList _completedProjects;
        bool _archaeologyChanged;

        // END

        void ApplyEquipCooldown(Item* pItem);
        void QuickEquipItem(uint16 pos, Item* pItem);
        void VisualizeItem(uint8 slot, Item* pItem);
        void SetVisibleItemSlot(uint8 slot, Item* pItem);
        Item* BankItem(ItemPosCountVec const& dest, Item* pItem, bool update)
        {
            return StoreItem(dest, pItem, update);
        }
        Item* BankItem(uint16 pos, Item* pItem, bool update);
        void RemoveItem(uint8 bag, uint8 slot, bool update);
        void RemoveItem(Item* pItem, bool update);
        void MoveItemFromInventory(uint8 bag, uint8 slot, bool update);
        void MoveItemFromInventory(Item* pItem, bool update);
                                                            // in trade, auction, guild bank, mail....
        void MoveItemToInventory(ItemPosCountVec const& dest, Item* pItem, bool update, bool in_characterInventoryDB = false);
                                                            // in trade, guild bank, mail....
        void RemoveItemDependentAurasAndCasts(Item* pItem);
        void DestroyItem(uint8 bag, uint8 slot, bool update);
        void DestroyItemCount(uint32 item, uint32 count, bool update, bool unequip_check = false);
        void DestroyItemCount(Item* item, uint32& count, bool update);
        void DestroyConjuredItems(bool update);
        void DestroyZoneLimitedItem(bool update, uint32 new_zone);
        void SplitItem(uint16 src, uint16 dst, uint32 count);
        void SwapItem(uint16 src, uint16 dst);
        void AddItemToBuyBackSlot(Item* pItem);
        Item* GetItemFromBuyBackSlot(uint32 slot);
        void RemoveItemFromBuyBackSlot(uint32 slot, bool del);
        void TakeExtendedCost(uint32 extendedCostId, uint32 count);
        void SendEquipError(InventoryResult msg, Item* pItem, Item* pItem2 = NULL, uint32 itemid = 0);
        void SendBuyError(BuyResult msg, Creature* creature = nullptr, uint32 item = 0);
        void SendSellError(SellResult msg, Creature* creature, ObjectGuid guid);
        void AddWeaponProficiency(uint32 newflag) { m_WeaponProficiency |= newflag; }
        void AddArmorProficiency(uint32 newflag) { m_ArmorProficiency |= newflag; }
        uint32 GetWeaponProficiency() const { return m_WeaponProficiency; }
        uint32 GetArmorProficiency() const { return m_ArmorProficiency; }
        bool IsUseEquipedWeapon(bool mainhand) const
        {
            // disarm applied only to mainhand weapon
            return !IsInFeralForm() && (!mainhand || !HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED));
        }
        bool IsTwoHandUsed() const
        {
            Item* mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

            if (mainItem && mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_WAND)
                return false;

            return mainItem && ((mainItem->GetTemplate()->GetInventoryType() == INVTYPE_2HWEAPON && !CanTitanGrip()) || mainItem->GetTemplate()->GetInventoryType() == INVTYPE_RANGED || mainItem->GetTemplate()->GetInventoryType() == INVTYPE_THROWN || mainItem->GetTemplate()->GetInventoryType() == INVTYPE_RANGEDRIGHT);
        }
        void SendNewItem(Item* item, uint32 count, bool received, bool created, bool broadcast = false, BattlePetMgr::BattlePet* petInfo = nullptr);
        bool BuyItemFromVendorSlot(ObjectGuid vendorguid, uint32 vendorslot, uint32 item, uint8 count, uint8 bag, uint8 slot);
        bool BuyCurrencyFromVendorSlot(ObjectGuid vendorGuid, uint32 vendorSlot, uint32 currency, uint32 count);
        bool _StoreOrEquipNewItem(uint32 vendorslot, uint32 item, uint8 count, uint8 bag, uint8 slot, int64 price, ItemTemplate const* pProto, Creature* pVendor, VendorItem const* crItem, bool bStore);

        float GetReputationPriceDiscount(Creature const* creature) const;

        Player* GetTrader() const { return m_trade ? m_trade->GetTrader() : NULL; }
        TradeData* GetTradeData() const { return m_trade; }
        void TradeCancel(bool sendback);

        void UpdateEnchantTime(uint32 time);
        void UpdateSoulboundTradeItems();
        void AddTradeableItem(Item* item);
        void RemoveTradeableItem(Item* item);
        void UpdateItemDuration(uint32 time, bool realtimeonly = true);
        void AddEnchantmentDurations(Item* item);
        void RemoveEnchantmentDurations(Item* item);
        void RemoveArenaEnchantments(EnchantmentSlot slot);
        void AddEnchantmentDuration(Item* item, EnchantmentSlot slot, uint32 duration);
        void ApplyEnchantment(Item* item, EnchantmentSlot slot, bool apply, bool apply_dur = true, bool ignore_condition = false);
        void ApplyEnchantment(Item* item, bool apply);
        void UpdateSkillEnchantments(uint16 skill_id, uint16 curr_value, uint16 new_value);
        void SendEnchantmentDurations();
        void AddItemDurations(Item* item);
        void RemoveItemDurations(Item* item);
        void SendItemDurations();
        void LoadCorpse();
        void LoadPet();

        bool AddItem(uint32 itemId, uint32 count, uint32* noSpaceForCount = NULL);

        uint32 m_stableSlots;

        /*********************************************************/
        /***                    GOSSIP SYSTEM                  ***/
        /*********************************************************/

        void PrepareGossipMenu(WorldObject* source, uint32 menuId = 0, bool showQuests = false);
        void SendPreparedGossip(WorldObject* source);
        void OnGossipSelect(WorldObject* source, uint32 gossipListId, uint32 menuId);

        uint32 GetGossipTextId(uint32 menuId, WorldObject* source);
        uint32 GetGossipTextId(WorldObject* source);
        static uint32 GetDefaultGossipMenuForSource(WorldObject* source);

        /*********************************************************/
        /***                    QUEST SYSTEM                   ***/
        /*********************************************************/

        int32 GetQuestLevel(Quest const* quest) const { return quest && (quest->Level > 0) ? quest->Level : getLevel(); }

        void PrepareAreaQuest(uint32 area);
        void PrepareQuestMenu(ObjectGuid guid);
        void SendPreparedQuest(ObjectGuid guid);
        bool IsActiveQuest(uint32 quest_id) const;
        Quest const* GetNextQuest(ObjectGuid guid, Quest const* quest);
        bool CanSeeStartQuest(Quest const* quest);
        bool CanTakeQuest(Quest const* quest, bool msg);
        bool CanAddQuest(Quest const* quest, bool msg);
        bool CanCompleteQuest(uint32 quest_id);
        bool CanCompleteRepeatableQuest(Quest const* quest);
        bool CanRewardQuest(Quest const* quest, bool msg);
        bool CanRewardQuest(Quest const* quest, uint32 reward, bool msg, uint32 packItemId);
        void AddQuest(Quest const* quest, Object* questGiver);
        void CompleteQuest(uint32 quest_id);
        void IncompleteQuest(uint32 quest_id);
        uint32 GetQuestMoneyReward(Quest const* quest) const;
        uint32 GetQuestXPReward(Quest const* quest);
        void RewardQuest(Quest const* quest, uint32 reward, Object* questGiver, bool announce = true, uint32 packItemId = 0);
        void FailQuest(uint32 quest_id);
        bool SatisfyQuestSkill(Quest const* qInfo, bool msg) const;
        bool SatisfyQuestLevel(Quest const* qInfo, bool msg);
        bool SatisfyQuestLog(bool msg);
        bool SatisfyQuestPreviousQuest(Quest const* qInfo, bool msg);
        bool SatisfyQuestClass(Quest const* qInfo, bool msg) const;
        bool SatisfyQuestRace(Quest const* qInfo, bool msg);
        bool SatisfyQuestReputation(Quest const* qInfo, bool msg);
        bool SatisfyQuestStatus(Quest const* qInfo, bool msg);
        bool SatisfyQuestConditions(Quest const* qInfo, bool msg);
        bool SatisfyQuestTimed(Quest const* qInfo, bool msg);
        bool SatisfyQuestExclusiveGroup(Quest const* qInfo, bool msg);
        bool SatisfyQuestNextChain(Quest const* qInfo, bool msg);
        bool SatisfyQuestPrevChain(Quest const* qInfo, bool msg);
        bool SatisfyQuestDay(Quest const* qInfo, bool msg);
        bool SatisfyQuestWeek(Quest const* qInfo, bool msg);
        bool SatisfyQuestSeasonal(Quest const* qInfo, bool msg);
        bool GiveQuestSourceItem(Quest const* quest);
        bool TakeQuestSourceItem(uint32 questId, bool msg);
        bool GetQuestRewardStatus(uint32 quest_id) const;
        QuestStatus GetQuestStatus(uint32 quest_id) const;
        QuestStatus GetDailyQuestStatus(uint32 quest_id) const;
        void SetQuestStatus(uint32 quest_id, QuestStatus status);
        void RemoveActiveQuest(uint32 quest_id);
        void RemoveRewardedQuest(uint32 quest_id);

        void SetDailyQuestStatus(uint32 quest_id);
        void SetWeeklyQuestStatus(uint32 quest_id);
        void SetSeasonalQuestStatus(uint32 quest_id);
        void DailyReset();
        void ResetWeeklyQuestStatus();
        void ResetSeasonalQuestStatus(uint16 event_id);

        uint16 FindQuestSlot(uint32 quest_id) const;
        uint32 GetQuestSlotQuestId(uint16 slot) const;
        uint32 GetQuestSlotState(uint16 slot)   const;
        uint16 GetQuestSlotCounter(uint16 slot, uint8 counter) const;
        uint32 GetQuestSlotTime(uint16 slot)    const;
        void SetQuestSlot(uint16 slot, uint32 quest_id, uint32 timer = 0);
        void SetQuestSlotCounter(uint16 slot, uint8 counter, uint16 count);
        void SetQuestSlotState(uint16 slot, uint32 state);
        void RemoveQuestSlotState(uint16 slot, uint32 state);
        void SetQuestSlotTimer(uint16 slot, uint32 timer);
        void SwapQuestSlot(uint16 slot1, uint16 slot2);
        void SetSpecialCriteriaComplete(uint16 slot, uint8 StorageIndex);

        void SetQuestCompletedBit(uint32 questBit, bool completed);
        uint16 GetReqKillOrCastCurrentCount(uint32 quest_id, int32 entry);
        void AreaExploredOrEventHappens(uint32 questId);
        void GroupEventHappens(uint32 questId, WorldObject const* pEventObject);
        void ItemAddedQuestCheck(uint32 entry, uint32 count);
        void ItemRemovedQuestCheck(uint32 entry, uint32 count);
        void KilledMonster(CreatureTemplate const* cInfo, ObjectGuid guid);
        void KilledMonsterCredit(uint32 entry, ObjectGuid guid = ObjectGuid::Empty);
        void KilledPlayerCredit();
        void KillCreditGO(uint32 entry, ObjectGuid const& guid);
        void AchieveCriteriaCredit(uint32 criteriaID);
        void TalkedToCreature(uint32 entry, ObjectGuid guid);
        void MoneyChanged(uint32 value);
        void ReputationChanged(FactionEntry const* factionEntry);
        bool HasQuestForItem(uint32 itemid) const;
        bool HasQuestForGO(int32 GOId) const;
        void UpdateForQuestWorldObjects();
        bool CanShareQuest(uint32 quest_id) const;

        int32 GetQuestObjectiveData(Quest const* quest, int8 storageIndex) const;
        int32 GetQuestObjectiveData(uint32 QuestID, uint32 ObjectiveID) const;
        void SetQuestObjectiveData(Quest const* quest, QuestObjective const* obj, int32 data);
        void UpdateQuestObjectiveData(Quest const* quest);
        void SendQuestComplete(Quest const* quest);
        void SendQuestReward(Quest const* quest, uint32 XP, Object* questGiver);
        void SendQuestFailed(uint32 questId, InventoryResult reason = EQUIP_ERR_OK);
        void SendQuestTimerFailed(uint32 quest_id);
        void SendCanTakeQuestResponse(uint32 msg, Quest const* qInfo, std::string = "none") const;
        void SendQuestConfirmAccept(Quest const* quest, Player* pReceiver);
        void SendPushToPartyResponse(Player* player, uint8 msg);
        void SendQuestUpdateAddCredit(Quest const* quest, ObjectGuid guid, QuestObjective const& obj, uint16 count);
        void SendQuestUpdateAddPlayer(Quest const* quest, uint16 old_count, uint16 add_count);

        ObjectGuid GetDivider() { return m_divider; }
        void SetDivider(ObjectGuid guid) { m_divider = guid; }

        uint32 GetInGameTime() { return m_ingametime; }

        void SetInGameTime(uint32 time) { m_ingametime = time; }

        void AddTimedQuest(uint32 quest_id) { m_timedquests.insert(quest_id); }
        void RemoveTimedQuest(uint32 quest_id) { m_timedquests.erase(quest_id); }

        void SendMusic(uint32 soundKitID);
        void SendSound(uint32 soundId, ObjectGuid source);
        void SendSoundToAll(uint32 soundId, ObjectGuid source);

        /*********************************************************/
        /***                   LOAD SYSTEM                     ***/
        /*********************************************************/

        bool LoadFromDB(ObjectGuid guid, SQLQueryHolder *holder);
        bool isBeingLoaded() const { return GetSession()->PlayerLoading();}

        void Initialize(ObjectGuid::LowType guid);
        static uint32 GetUInt32ValueFromArray(Tokenizer const& data, uint16 index);
        static float  GetFloatValueFromArray(Tokenizer const& data, uint16 index);
        static uint32 GetZoneIdFromDB(ObjectGuid guid);
        static uint32 GetLevelFromDB(ObjectGuid guid);
        static bool   LoadPositionFromDB(uint32& mapid, float& x, float& y, float& z, float& o, bool& in_flight, ObjectGuid guid);

        static bool IsValidGender(uint8 Gender) { return Gender <= GENDER_FEMALE; }
        static bool IsValidClass(uint8 Class) { return ((1 << (Class - 1)) & CLASSMASK_ALL_PLAYABLE) != 0; }
        static bool IsValidRace(uint8 Race) { return ((1 << (Race - 1)) & RACEMASK_ALL_PLAYABLE) != 0; }
        static bool ValidateAppearance(uint8 race, uint8 class_, uint8 gender, uint8 hairID, uint8 hairColor, uint8 faceID, uint8 facialHair, uint8 skinColor, bool create = false);

        /*********************************************************/
        /***                   SAVE SYSTEM                     ***/
        /*********************************************************/

        void SaveToDB(bool create = false);
        void SaveInventoryAndGoldToDB(SQLTransaction& trans);                    // fast save function for item/money cheating preventing
        void SaveGoldToDB(SQLTransaction& trans);

        static void SetUInt32ValueInArray(Tokenizer& data, uint16 index, uint32 value);
        static void SetFloatValueInArray(Tokenizer& data, uint16 index, float value);
        static void Customize(ObjectGuid::LowType guid, WorldPackets::Character::CharCustomizeInfo* info);
        static void SavePositionInDB(uint32 mapid, float x, float y, float z, float o, uint32 zone, ObjectGuid guid);

        static void DeleteFromDB(ObjectGuid playerguid, uint32 accountId, bool updateRealmChars = true, bool deleteFinally = false);
        static void DeleteOldCharacters();
        static void DeleteOldCharacters(uint32 keepDays);

        bool m_mailsLoaded;
        bool m_mailsUpdated;

        void SetBindPoint(ObjectGuid guid);
        void SendTalentWipeConfirm(ObjectGuid guid, RespecType type = RESPEC_TYPE_TALENTS);
        void CalcRage(uint32 damage, bool attacker);
        void RegenerateAll();
        void Regenerate(Powers power, uint32 saveTimer);
        void RegenerateHealth();
        void setRegenTimerCount(uint32 time) {m_powerRegenTimer[POWER_RAGE] = time;}
        void setWeaponChangeTimer(uint32 time) {m_weaponChangeTimer = time;}

        uint64 GetMoney() const { return GetUInt64Value(PLAYER_FIELD_COINAGE); }
        bool ModifyMoney(int64 d, bool sendError = true);
        bool HasEnoughMoney(uint64 amount) const { return GetMoney() >= amount; }
        bool HasEnoughMoney(int64 amount) const
        {
            if (amount > 0)
                return (GetMoney() >= (uint64)amount);
            return true;
        }

        void SetMoney(uint64 value)
        {
            SetUInt64Value(PLAYER_FIELD_COINAGE, value);
            MoneyChanged(value);
            UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED);
        }

        RewardedQuestSet const& getRewardedQuests() const { return m_RewardedQuests; }
        QuestStatusMap& getQuestStatusMap() { return m_QuestStatus; }

        size_t GetRewardedQuestCount() const { return m_RewardedQuests.size(); }
        bool IsQuestRewarded(uint32 quest_id) const
        {
            return m_RewardedQuests.find(quest_id) != m_RewardedQuests.end();
        }

        ObjectGuid GetSelection() const { return m_curSelection; }
        ObjectGuid GetLastSelection() const { return m_lastSelection; }
        Unit* GetSelectedUnit() const;
        Unit* GetLastSelectedUnit() const;
        Player* GetSelectedPlayer() const;
        void SetSelection(ObjectGuid const& guid) { if (m_curSelection) m_lastSelection = m_curSelection; m_curSelection = guid; SetGuidValue(UNIT_FIELD_TARGET, guid); }

        void SendMailResult(uint32 mailId, MailResponseType mailAction, MailResponseResult mailError, uint32 equipError = 0, ObjectGuid::LowType item_guid = UI64LIT(0), uint32 item_count = 0);
        void SendNewMail();
        void UpdateNextMailTimeAndUnreads();
        void AddNewMailDeliverTime(time_t deliver_time);
        bool IsMailsLoaded() const { return m_mailsLoaded; }

        void RemoveMail(uint32 id);
        void SafeRemoveMailFromIgnored(ObjectGuid const& ignoredPlayerGuid);

        void AddMail(Mail* mail) { m_mail.push_front(mail);}// for call from WorldSession::SendMailTo
        uint32 GetMailSize() { return m_mail.size();}
        Mail* GetMail(uint32 id);

        PlayerMails::iterator GetMailBegin() { return m_mail.begin();}
        PlayerMails::iterator GetMailEnd() { return m_mail.end();}

        /*********************************************************/
        /*** MAILED ITEMS SYSTEM ***/
        /*********************************************************/

        uint8 unReadMails;
        time_t m_nextMailDelivereTime;

        typedef std::unordered_map<ObjectGuid::LowType, Item*> ItemMap;

        ItemMap mMitems;                                    //template defined in objectmgr.cpp

        Item* GetMItem(ObjectGuid::LowType id)
        {
            ItemMap::const_iterator itr = mMitems.find(id);
            return itr != mMitems.end() ? itr->second : NULL;
        }

        void AddMItem(Item* it)
        {
            ASSERT(it);
            //ASSERT deleted, because items can be added before loading
            mMitems[it->GetGUIDLow()] = it;
        }

        bool RemoveMItem(ObjectGuid::LowType id)
        {
            return mMitems.erase(id) ? true : false;
        }

        void SendOnCancelExpectedVehicleRideAura() {  GetSession()->SendPacket(WorldPackets::Vehicle::OnCancelExpectedRideVehicleAura().Write()); }

        void PetSpellInitialize();
        void CharmSpellInitialize();
        void PossessSpellInitialize();
        void VehicleSpellInitialize();
        void SendRemoveControlBar();
        void SendKnownSpells();
        bool HasSpell(uint32 spell) const;
        bool HasActiveSpell(uint32 spell);            // show in spellbook
        TrainerSpellState GetTrainerSpellState(TrainerSpell const* trainer_spell) const;
        bool IsSpellFitByClassAndRace(uint32 spell_id) const;
        bool IsNeedCastPassiveSpellAtLearn(SpellInfo const* spellInfo) const;

        void SendProficiency(ItemClass itemClass, uint32 itemSubclassMask);
        bool addSpell(uint32 spellId, bool active, bool learning, bool dependent, bool disabled, bool loading = false, bool charload = false, uint32 fromSkill = 0);
        void learnSpell(uint32 spell_id, bool dependent, uint32 fromSkill = 0, bool sendMessage = true);
        void removeSpell(uint32 spell_id, bool disabled = false, bool learn_low_rank = true, bool sendMessage = true);
        void resetSpells(bool myClassOnly = false);
        SpellInfo const* GetCastSpellInfo(SpellInfo const* spellInfo) const override;
        void AddOverrideSpell(uint32 overridenSpellId, uint32 newSpellId);
        void RemoveOverrideSpell(uint32 overridenSpellId, uint32 newSpellId);
        void LearnSpecializationSpells();
        void RemoveSpecializationSpells();
        void UpdateSkillsForLevel();
        void LearnDefaultSkills();
        void LearnDefaultSkill(SkillRaceClassInfoEntry const* rcInfo);
        void learnQuestRewardedSpells();
        void learnQuestRewardedSpells(Quest const* quest);
        void learnSpellHighRank(uint32 spellid);
        void AddTemporarySpell(uint32 spellId);
        void RemoveTemporarySpell(uint32 spellId);
        void SetReputation(uint32 factionentry, uint32 value);
        uint32 GetReputation(uint32 factionentry);
        std::string GetGuildName();
        Guild* GetGuild();

        // Talents
        uint32 GetQuestRewardedTalentCount() const { return _talentMgr->QuestRewardedTalentCount; }
        void AddQuestRewardedTalentCount(uint32 points) { _talentMgr->QuestRewardedTalentCount += points; }
        uint32 GetTalentResetCost() const { return _talentMgr->ResetTalentsCost; }
        void SetTalentResetCost(uint32 cost)  { _talentMgr->ResetTalentsCost = cost; }
        uint32 GetSpecializationResetCost() const { return _talentMgr->ResetSpecializationCost; }
        void SetSpecializationResetCost(uint32 cost) { _talentMgr->ResetSpecializationCost = cost; }
        uint32 GetSpecializationResetTime() const { return _talentMgr->ResetSpecializationTime; }
        void SetSpecializationResetTime(time_t time_) { _talentMgr->ResetSpecializationTime = time_; }
        uint32 GetTalentResetTime() const { return _talentMgr->ResetTalentsTime; }
        void SetTalentResetTime(time_t time_)  { _talentMgr->ResetTalentsTime = time_; }
        uint8 GetSpecIndex() const { return _talentMgr->SpecIndex; }
        void SetSpecIndex(uint8 index){ _talentMgr->SpecIndex = index; }
        void SetSpecializationId(uint32 id, bool initial = false);
        uint32 GetSpecializationId() const { return _talentMgr->SpecializationId; }
        uint32 GetRoleForGroup(uint32 specializationId);
        bool isInTankSpec();
        uint32 GetDefaultSpecId() const 
        {
            ChrClassesEntry const* entry = sChrClassesStore.LookupEntry(getClass());
            if (entry)
                return entry->DefaultSpec;
            return 0;
        }

        bool ResetTalents(bool no_cost = false);
        uint32 GetNextResetTalentsCost() const;
        uint32 GetNextResetSpecializationCost() const;
        void InitTalentForLevel();
        void SendTalentsInfoData(bool pet);
        bool LearnTalent(uint32 talentId);
        bool LearnPvpTalent(uint16 talentID);
        bool AddTalent(TalentEntry const* talent, uint8 index, bool learning, bool sendMessage = true);
        bool HasTalent(uint32 spell_id, uint8 index) const;
        void RemoveTalent(TalentEntry const* talent, bool sendMessage = true);
        uint8 CalculateTalentsPoints() const;
        void LearnTalentSpecialization(uint32 talentSpec);
        void ResetTalentSpecialization();

        void ResetSpec();
        void ActivateSpecialization(uint32 specId, uint8 index);

        void InitGlyphsForLevel();
        void SetGlyphSlot(uint8 slot, uint32 slottype) { /*SetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS + slot, slottype)*/; }
        uint32 GetGlyphSlot(uint8 slot) const { return 0/*GetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS + slot)*/; }
        void SetGlyph(uint8 slot, uint32 glyph)
        {
            _talentMgr->SpecInfo[GetSpecIndex()].Glyphs[slot] = glyph;
            //SetUInt32Value(PLAYER_FIELD_GLYPHS + slot, glyph);
        }
        uint32 GetGlyph(uint8 index, uint8 slot) const { return _talentMgr->SpecInfo[index].Glyphs[slot]; }

        PlayerTalentMap const* GetTalentMap(uint8 index) const { return _talentMgr->SpecInfo[index].Talents; }
        PlayerTalentMap* GetTalentMap(uint8 index) { return _talentMgr->SpecInfo[index].Talents; }
        ActionButtonList const& GetActionButtons() const { return m_actionButtons; }

        uint32 GetFreePrimaryProfessionPoints() const { return GetUInt32Value(PLAYER_FIELD_CHARACTER_POINTS); }
        void SetFreePrimaryProfessions(uint16 profs) { SetUInt32Value(PLAYER_FIELD_CHARACTER_POINTS, profs); }
        void InitPrimaryProfessions();

        PlayerSpellMap const& GetSpellMap() const { return m_spells; }
        PlayerSpellMap      & GetSpellMap()       { return m_spells; }

        const ItemSpellList & GetItemSpellList() { return m_itemSpellList; }
        void HandleItemSpellList(uint32 spellId, bool apply)
        {
            if (apply)
                m_itemSpellList.push_back(spellId);
            else
                m_itemSpellList.remove(spellId);
        }

        void AddSpellMountReplacelist(uint32 spellId, uint32 replaseId)
        {
            spellMountReplacelist[replaseId] = spellId;
        }
        uint32 GetSpellIdbyReplace(uint32 replace)
        {
            std::map<uint32, uint32>::const_iterator itr = spellMountReplacelist.find(replace);
            return itr != spellMountReplacelist.end() ? itr->second : NULL;
        }

        SpellCooldowns const& GetSpellCooldownMap() const { return m_spellCooldowns; }

        void AddSpellMod(SpellModifier* mod, bool apply);
        void SendSpellMods();
        bool IsAffectedBySpellmod(SpellInfo const* spellInfo, SpellModifier* mod, Spell* spell = NULL);
        template <class T> T ApplySpellMod(uint32 spellId, SpellModOp op, T &basevalue, Spell* spell = NULL);
        void RemoveSpellMods(Spell* spell, bool casting = false);
        void RestoreSpellMods(Spell* spell, uint32 ownerAuraId = 0, Aura* aura = NULL);
        void RestoreAllSpellMods(uint32 ownerAuraId = 0, Aura* aura = NULL);
        void DropModCharge(SpellModifier* mod, Spell* spell);
        void SetSpellModTakingSpell(Spell* spell, bool apply);
        bool HasInstantCastModForSpell(SpellInfo const* spellInfo);

        static uint32 const infinityCooldownDelay = MONTH;  // used for set "infinity cooldowns" for spells and check
        static uint32 const infinityCooldownDelayCheck = MONTH/2;
        bool HasSpellCooldown(uint32 spell_id) const
        {
            SpellCooldowns::const_iterator itr = m_spellCooldowns.find(spell_id);
            return itr != m_spellCooldowns.end() && itr->second.end > getPreciseTime();
        }
        double GetSpellCooldownDelay(uint32 spell_id) const
        {
            SpellCooldowns::const_iterator itr = m_spellCooldowns.find(spell_id);
            double t = getPreciseTime();
            return uint32(itr != m_spellCooldowns.end() && itr->second.end > t ? itr->second.end - t : 0.0);
        }
        double GetPPPMSpellCooldownDelay(uint32 spell_id) const
        {
            RPPMSpellCooldowns::const_iterator itr = m_rppmspellCooldowns.find(spell_id);
            double t = getPreciseTime();
            return double(itr != m_rppmspellCooldowns.end() && itr->second.end > t ? itr->second.end - t : 0.0);
        }
        double GetLastSuccessfulProc(uint32 spell_id) { return m_rppmLastSuccessfulProc[spell_id]; }
        double GetLastChanceToProc(uint32 spell_id) { return m_rppmLastChanceToProc[spell_id]; }
        void SetLastSuccessfulProc(uint32 spell_id, double time) { m_rppmLastSuccessfulProc[spell_id] = time; }
        void SetLastChanceToProc(uint32 spell_id, double time) { m_rppmLastChanceToProc[spell_id] = time; }
        bool GetRPPMProcChance(double &cooldown, float RPPM, const SpellInfo* spellProto);
        void AddSpellAndCategoryCooldowns(SpellInfo const* spellInfo, uint32 itemId, Spell* spell = NULL, bool infinityCooldown = false);
        void AddSpellCooldown(uint32 spell_id, uint32 itemid, double end_time);
        void AddRPPMSpellCooldown(uint32 spell_id, uint32 itemid, double end_time);
        void SendCooldownEvent(SpellInfo const* spellInfo, uint32 itemId = 0, Spell* spell = NULL, bool setCooldown = true);
        void ProhibitSpellSchool(SpellSchoolMask idSchoolMask, uint32 unTimeMs);
        void RemoveSpellCooldown(uint32 spell_id, bool update = false);
        void RemoveSpellCategoryCooldown(uint32 cat, bool update = false);
        void SendClearCooldown(uint32 spell_id, Unit* target);
        void ModifySpellCooldown(uint32 spell_id, int32 delta);

        GlobalCooldownMgr& GetGlobalCooldownMgr() { return m_GlobalCooldownMgr; }

        void RemoveCategoryCooldown(uint32 cat);
        void RemoveArenaSpellCooldowns(bool removeActivePetCooldowns = false);
        void RemoveAllSpellCooldown();
        void _LoadSpellCooldowns(PreparedQueryResult result);
        void _SaveSpellCooldowns(SQLTransaction& trans);

        bool HasChargesForSpell(SpellInfo const* spellInfo) const;
        uint8 GetMaxSpellCategoryCharges(SpellCategoryEntry const* categoryEntry) const;
        uint32 GetSpellCategoryChargesTimer(SpellCategoryEntry const* categoryEntry) const;
        uint8 GetMaxSpellCategoryCharges(uint32 category) const;
        void TakeSpellCharge(SpellInfo const* spellInfo);
        void UpdateSpellCharges(uint32 diff);
        void RecalculateSpellCategoryCharges(uint32 category);
        void RecalculateSpellCategoryRegenTime(uint32 category);
        void RestoreSpellCategoryCharges(uint32 categoryId = 0);

        void SetLastPotionId(uint32 item_id) { m_lastPotionId = item_id; }
        void UpdatePotionCooldown(Spell* spell = NULL);

        // visible items
        bool HandleChangeSlotModel(uint32 newItem, uint16 pos);
        void HandleAltVisSwitch();

        void SetResurrectRequestData(Unit* caster, uint32 health, uint32 mana, uint32 appliedAura)
        {
            ASSERT(!IsRessurectRequested());
            _resurrectionData = new ResurrectionData();
            _resurrectionData->GUID = caster->GetGUID();
            _resurrectionData->Location.WorldRelocate(*caster);
            _resurrectionData->Health = health;
            _resurrectionData->Mana = mana;
            _resurrectionData->Aura = appliedAura;
        }

        void ClearResurrectRequestData()
        {
            delete _resurrectionData;
            _resurrectionData = NULL;
        }
        bool IsRessurectRequestedBy(ObjectGuid guid) const
        {
            if (!IsRessurectRequested())
                return false;

            return _resurrectionData->GUID == guid;
        }

        bool IsRessurectRequested() const { return _resurrectionData != NULL; }

        void ResurectUsingRequestData();

        uint8 getCinematic()
        {
            return m_cinematic;
        }
        void setCinematic(uint8 cine)
        {
            m_cinematic = cine;
        }

        ActionButton* addActionButton(uint8 button, uint32 action, uint8 type);
        void removeActionButton(uint8 button);
        ActionButton const* GetActionButton(uint8 button);
        void SendInitialActionButtons() const { SendActionButtons(0); }
        void SendActionButtons(uint32 state) const;
        bool IsActionButtonDataValid(uint8 button, uint32 action, uint8 type);

        int8 GetFreeActionButton();

        PvPInfo pvpInfo;
        void UpdatePvPState(bool onlyFFA = false);
        void SetPvP(bool state)
        {
            Unit::SetPvP(state);
            for (ControlList::iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
                (*itr)->SetPvP(state);
        }
        void UpdatePvP(bool state, bool override=false);
        void UpdateZone(uint32 newZone, uint32 newArea);
        void UpdateArea(uint32 newArea);

        void UpdateZoneDependentAuras(uint32 zone_id);    // zones
        void UpdateAreaDependentAuras(uint32 area_id);    // subzones
        void UpdateAreaQuestTasks(uint32 newAreaId, uint32 oldAreaId);

        void UpdateAfkReport(time_t currTime);
        void UpdatePvPFlag(time_t currTime);
        void UpdateContestedPvP(uint32 currTime);
        void SetContestedPvPTimer(uint32 newTime) {m_contestedPvPTimer = newTime;}
        void ResetContestedPvP()
        {
            ClearUnitState(UNIT_STATE_ATTACK_PLAYER);
            RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP);
            m_contestedPvPTimer = 0;
        }

        /** todo: -maybe move UpdateDuelFlag+DuelComplete to independent DuelHandler.. **/
        DuelInfo* duel;
        void UpdateDuelFlag(time_t currTime);
        void CheckDuelDistance(time_t currTime);
        void DuelComplete(DuelCompleteType type);
        void SendDuelCountdown(uint32 counter);

        bool IsGroupVisibleFor(Player const* p) const;
        bool IsInSameGroupWith(Player const* p) const;
        bool IsInSameRaidWith(Player const* p) const { return p == this || (GetGroup() != NULL && GetGroup() == p->GetGroup()); }
        void UninviteFromGroup();
        static void RemoveFromGroup(Group* group, ObjectGuid guid, RemoveMethod method = GROUP_REMOVEMETHOD_DEFAULT, ObjectGuid kicker = ObjectGuid::Empty, const char* reason = NULL);
        void RemoveFromGroup(RemoveMethod method = GROUP_REMOVEMETHOD_DEFAULT) { RemoveFromGroup(GetGroup(), GetGUID(), method); }
        void SendUpdateToOutOfRangeGroupMembers();

        void SetInGuild(ObjectGuid::LowType guildId);
        void SetRank(uint8 rankId) { SetUInt32Value(PLAYER_FIELD_GUILD_RANK_ID, rankId); }
        uint32 GetRank() { return GetUInt32Value(PLAYER_FIELD_GUILD_RANK_ID); }
        void SetGuildLevel(uint32 level) { SetUInt32Value(PLAYER_FIELD_GUILD_LEVEL, level); }
        uint32 GetGuildLevel() { return GetUInt32Value(PLAYER_FIELD_GUILD_LEVEL); }
        void SetGuildIdInvited(uint32 GuildId, ObjectGuid guid = ObjectGuid::Empty) { m_GuildIdInvited = GuildId; m_GuildInviterGuid = guid; }
        ObjectGuid GetGuildInviterGuid() const { return m_GuildInviterGuid; }
        ObjectGuid::LowType GetGuildId() const { return GetGuidValue(OBJECT_FIELD_DATA).GetCounter(); /* return only lower part */ }
        ObjectGuid GetGuildGUID() const { return GetGuidValue(OBJECT_FIELD_DATA); }
        static ObjectGuid::LowType GetGuildIdFromDB(ObjectGuid guid);
        static uint8 GetRankFromDB(ObjectGuid guid);
        int GetGuildIdInvited() { return m_GuildIdInvited; }
        static void RemovePetitionsAndSigns(ObjectGuid guid);


        // Bracket System
        void InitBrackets();

        //! Use for get all data
        uint32 GetBracketInfo(BracketType slot, BracketInfoType type) const { return GetUInt32Value(PLAYER_FIELD_PVP_INFO + (slot * BRACKET_END) + type); }
        void SetBracketInfoField(BracketType slot, BracketInfoType type, uint32 value)
        {
            SetUInt32Value(PLAYER_FIELD_PVP_INFO + (slot * BRACKET_END) + type, value);
        }

        Difficulty GetDifficultyID(MapEntry const* mapEntry) const;
        Difficulty GetDungeonDifficultyID() const { return m_dungeonDifficulty; }
        Difficulty GetRaidDifficultyID() const { return m_raidDifficulty; }
        Difficulty GetLegacyRaidDifficultyID() const { return m_legacyRaidDifficulty; }
        void SetDungeonDifficultyID(Difficulty dungeon_difficulty) { m_dungeonDifficulty = dungeon_difficulty; }
        void SetRaidDifficultyID(Difficulty raid_difficulty) { m_raidDifficulty = raid_difficulty; }
        void SetLegacyRaidDifficultyID(Difficulty raid_difficulty) { m_legacyRaidDifficulty = raid_difficulty; }
        static Difficulty CheckLoadedDungeonDifficultyID(Difficulty difficulty);
        static Difficulty CheckLoadedRaidDifficultyID(Difficulty difficulty);
        static Difficulty CheckLoadedLegacyRaidDifficultyID(Difficulty difficulty);

        bool UpdateSkill(uint32 skill_id, uint32 step);
        bool UpdateSkillPro(uint16 SkillId, int32 Chance, uint32 step);

        bool UpdateCraftSkill(uint32 spellid);
        bool UpdateGatherSkill(uint32 SkillId, uint32 SkillValue, uint32 RedLevel, uint32 Multiplicator = 1);
        bool UpdateFishingSkill();

        uint32 GetSpellByProto(ItemTemplate* proto);

        float GetHealthBonusFromStamina();

        bool UpdateStats(Stats stat);
        bool UpdateAllStats();
        void UpdateResistances(uint32 school);
        void UpdateArmor();
        void UpdateMaxHealth();
        void UpdateMaxPower(Powers power);
        void UpdateAttackPowerAndDamage(bool ranged = false);
        void UpdateDamagePhysical(WeaponAttackType attType);
        void ApplySpellPowerBonus(int32 amount, bool apply);
        void UpdateSpellDamageAndHealingBonus();
        void ApplyRatingMod(CombatRating cr, int32 value, bool apply);
        void UpdateRating(CombatRating cr);
        void UpdateAllRatings();

        void CalculateMinMaxDamage(WeaponAttackType attType, bool normalized, bool addTotalPct, float& min_damage, float& max_damage);

        inline void RecalculateRating(CombatRating cr) { ApplyRatingMod(cr, 0, true);}
        float GetMeleeCritFromAgility();
        void GetDodgeFromAgility(float &diminishing, float &nondiminishing);
        float GetRatingBonusValue(CombatRating cr) const;
        uint32 GetBaseSpellPowerBonus() { return m_baseSpellPower; }
        int32 GetSpellPenetrationItemMod() const { return m_spellPenetrationItemMod; }
        float GetExpertiseDodgeOrParryReduction(WeaponAttackType attType) const;

        void UpdateBlockPercentage();
        void UpdateCritPercentage(WeaponAttackType attType);
        void UpdateAllCritPercentages();
        void UpdateParryPercentage();
        void UpdateDodgePercentage();
        void UpdateMeleeHitChances();
        void UpdateRangedHitChances();
        void UpdateSpellHitChances();
        void UpdateSpellHastDurationRecovery();

        void HandleArenaDeserter();

        void SetNeedToUpdateRunesRegen() { m_needToUpdateRunesRegen = true; }
        void SetNeedToUpdateSpellHastDurationRecovery() { m_needToUpdateSpellHastDurationRecovery = true; }
        void SetNeedUpdateCastHastMods() { m_needUpdateCastHastMods = true; }
        void SetNeedUpdateMeleeHastMod() { m_needUpdateMeleeHastMod = true; }
        void SetNeedUpdateRangeHastMod() { m_needUpdateRangeHastMod = true; }
        void SetNeedUpdateHastMod() { m_needUpdateHastMod = true; }

        void UpdateAllSpellCritChances();
        void UpdateSpellCritChance(uint32 school);
        void UpdateArmorPenetration(int32 amount);
        void UpdateExpertise();
        void ApplyManaRegenBonus(int32 amount, bool apply);
        void ApplyHealthRegenBonus(int32 amount, bool apply);
        void UpdateMasteryAuras();
        void UpdateAllRunesRegen();
        void UpdateVersality(CombatRating cr = CR_NONE);
        void UpdateMultistrike();
        void UpdateMultistrikeDamage();
        void UpdateReadiness();
        void UpdateCRSpeed();
        void UpdateLifesteal();
        void UpdateAvoidance();

        void SetLootSpecID(uint32 spec) { SetUInt32Value(PLAYER_FIELD_LOOT_SPEC_ID, spec); }
        uint32 GetFieldLootSpecID() const { return GetUInt32Value(PLAYER_FIELD_LOOT_SPEC_ID); }
        uint32 GetLootSpecID() const
        {
            if (GetFieldLootSpecID() > 0)
                return GetFieldLootSpecID();
            if (GetSpecializationId() > 0)
                return GetSpecializationId();

            return GetDefaultLootSpecID();
        }
        uint32 GetDefaultLootSpecID() const
        {
            for (ChrSpecializationEntry const* specialization : sChrSpecializationStore)
                if (specialization->ClassID == getClass() && specialization->OrderIndex == 0)
                    return specialization->ID;

            return 0;
        }

        ObjectGuid GetLootGUID() const { return m_lootGuid; }
        void SetLootGUID(ObjectGuid guid) { m_lootGuid = guid; }

        //! AOE loot now used only for looting money. Find fast way for loot money -> LootMgr.
        void ClearAoeLootList() { m_AoelootGuidList.clear(); }
        void AddAoeLootList(ObjectGuid owner, ObjectGuid lGUID) { m_AoelootGuidList[owner] = lGUID; }
        AoeMap* GetAoeLootList() { return &m_AoelootGuidList; }
        void DelAoeLootList(ObjectGuid owner) { m_AoelootGuidList.erase(owner); }

        void RemovedInsignia(Player* looterPlr);

        WorldSession* GetSession() const { return m_session; }

        void BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const;
        void DestroyForPlayer(Player* target, bool onDeath /*= false*/) const override;
        void SendLogXPGain(uint32 GivenXP, Unit* victim, uint32 BonusXP, bool recruitAFriend = false, float group_rate=1.0f);

        // notifiers
        void SendAttackSwingError(AttackSwingReason error);
        void SendCancelAutoRepeat(Unit* target);
        void SendExplorationExperience(uint32 Area, uint32 Experience);

        void SendDungeonDifficulty(int32 forcedDifficulty = -1);
        void SendRaidDifficulty(bool Legacy, int32 forcedDifficulty = -1);
        void ResetInstances(uint8 method, bool isRaid, bool isLegacy);
        void SendResetInstanceSuccess(uint32 MapId);
        void SendResetInstanceFailed(uint32 reason, uint32 MapId);
        void SendResetFailedNotify();

        bool UpdatePosition(float x, float y, float z, float orientation, bool teleport = false, bool stop = false);
        bool UpdatePosition(const Position &pos, bool teleport = false) { return UpdatePosition(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), teleport); }
        void UpdateUnderwaterState(Map* m, float x, float y, float z);

        void SendMessageToSet(WorldPacket const* data, bool self) {SendMessageToSetInRange(data, GetVisibilityRange(), self); };// overwrite Object::SendMessageToSet
        void SendMessageToSetInRange(WorldPacket const* data, float fist, bool self);// overwrite Object::SendMessageToSetInRange
        void SendMessageToSetInRange(WorldPacket const* data, float dist, bool self, bool own_team_only);
        void SendMessageToSet(WorldPacket const* data, Player const* skipped_rcvr);
        void SendChatMessageToSetInRange(Trinity::ChatData& c, float dist, bool self, bool own_team_only);

        Corpse* GetCorpse() const;
        void SpawnCorpseBones();
        void CreateCorpse();
        bool FallGroundAnt();
        void KillPlayer();
        uint32 GetResurrectionSpellId();
        void ResurrectPlayer(float restore_percent, bool applySickness = false);
        void BuildPlayerRepop();
        void RepopAtGraveyard();

        void DurabilityLossAll(double percent, bool inventory, bool withMods);
        void DurabilityLoss(Item* item, double percent);
        void DurabilityPointsLossAll(int32 points, bool inventory);
        void DurabilityPointsLoss(Item* item, int32 points);
        void DurabilityPointLossForEquipSlot(EquipmentSlots slot);
        uint32 DurabilityRepairAll(bool cost, float discountMod, bool guildBank);
        uint32 DurabilityRepair(uint16 pos, bool cost, float discountMod, bool guildBank);

        void UpdateMirrorTimers();
        void StopMirrorTimers()
        {
            StopMirrorTimer(FATIGUE_TIMER);
            StopMirrorTimer(BREATH_TIMER);
            StopMirrorTimer(FIRE_TIMER);
        }
        bool IsMirrorTimerActive(MirrorTimerType type) { return m_MirrorTimer[type] == getMaxTimer(type); }

        bool CanJoinConstantChannelInZone(ChatChannelsEntry const* channel, AreaTableEntry const* zone);

        void JoinedChannel(Channel* c);
        void LeftChannel(Channel* c);
        void CleanupChannels();
        void UpdateLocalChannels(uint32 newZone);
        void LeaveLFGChannel();

        void SetSkill(uint16 id, uint16 step, uint16 currVal, uint16 maxVal);
        uint16 GetMaxSkillValue(uint32 skill) const;        // max + perm. bonus + temp bonus
        uint16 GetPureMaxSkillValue(uint32 skill) const;    // max
        uint16 GetSkillValue(uint32 skill) const;           // skill value + perm. bonus + temp bonus
        uint16 GetBaseSkillValue(uint32 skill) const;       // skill value + perm. bonus
        uint16 GetPureSkillValue(uint32 skill) const;       // skill value
        int16 GetSkillPermBonusValue(uint32 skill) const;
        int16 GetSkillTempBonusValue(uint32 skill) const;
        uint16 GetSkillStep(uint16 skill) const;            // 0...6
        bool HasSkill(uint32 skill) const;
        void learnSkillRewardedSpells(uint32 id, uint32 value);

        WorldLocation& GetTeleportDest() { return m_teleport_dest; }
        bool IsBeingTeleported() const { return mSemaphoreTeleport_Near || mSemaphoreTeleport_Far; }
        bool IsBeingTeleportedNear() const { return mSemaphoreTeleport_Near; }
        bool IsBeingTeleportedFar() const { return mSemaphoreTeleport_Far; }
        bool IsBeingTeleportedSeamlessly() const { return IsBeingTeleportedFar() && m_teleport_options & TELE_TO_SEAMLESS; }
        void SetSemaphoreTeleportNear(bool semphsetting) { mSemaphoreTeleport_Near = semphsetting; }
        void SetSemaphoreTeleportFar(bool semphsetting) { mSemaphoreTeleport_Far = semphsetting; }
        void ProcessDelayedOperations();

        bool HasAreaExplored(uint32 AreaID);
        void CheckAreaExploreAndOutdoor(void);

        static uint32 TeamForRace(uint8 race);
        uint32 GetTeam() const { return m_team; }
        TeamId GetTeamId() const { if (m_team == ALLIANCE) return TEAM_ALLIANCE; if (m_team == HORDE) return TEAM_HORDE; return TEAM_NEUTRAL; }
        void setFactionForRace(uint8 race);

        void InitDisplayIds();

        bool IsAtGroupRewardDistance(WorldObject const* pRewardSource) const;
        bool IsAtRecruitAFriendDistance(WorldObject const* pOther) const;
        void RewardPlayerAndGroupAtKill(Unit* victim, bool isBattleGround);
        void RewardPlayerAndGroupAtEvent(uint32 creature_id, WorldObject* pRewardSource);
        bool isHonorOrXPTarget(Unit* victim);

        float GetsAFriendBonus(bool forXP);
        uint8 GetGrantableLevels() { return m_grantableLevels; }
        void SetGrantableLevels(uint8 val) { m_grantableLevels = val; }

        Expansions GetExpByLevel();

        ReputationMgr&       GetReputationMgr()       { return m_reputationMgr; }
        ReputationMgr const& GetReputationMgr() const { return m_reputationMgr; }
        ReputationRank GetReputationRank(uint32 faction_id) const;
        void RewardReputation(Unit* victim, float rate);
        void RewardReputation(Quest const* quest);
        void RewardGuildReputation(Quest const* quest);

        void ModifySkillBonus(uint32 skillid, int32 val, bool talent);

        void ResetLootCooldown();

        /*********************************************************/
        /***                  PVP SYSTEM                       ***/
        /*********************************************************/
        void UpdateHonorFields(bool loading = false);
        bool RewardHonor(Unit* victim, uint32 groupsize, int32 honor = -1, bool pvptoken = false);
        uint32 GetMaxPersonalArenaRatingRequirement(BracketType minarenaslot) const;
        uint32 GetMaxMMR() const;

        //End of PvP System

        inline SpellCooldowns GetSpellCooldowns() const { return m_spellCooldowns; }

        void SetDrunkValue(uint8 newDrunkValue, uint32 itemId = 0);
        uint8 GetDrunkValue() const { return GetByteValue(PLAYER_BYTES_3, 1); }
        static DrunkenState GetDrunkenstateByValue(uint8 value);

        uint32 GetDeathTimer() const { return m_deathTimer; }
        uint32 GetCorpseReclaimDelay(bool pvp) const;
        void UpdateCorpseReclaimDelay();
        void SendCorpseReclaimDelay(bool load = false);

        uint32 GetBlockPercent() { return GetUInt32Value(PLAYER_FIELD_SHIELD_BLOCK); }
        bool CanParry() const { return m_canParry; }
        void SetCanParry(bool value);
        bool CanBlock() const { return m_canBlock; }
        void SetCanBlock(bool value);
        bool CanTitanGrip() const { return m_canTitanGrip; }
        void SetCanTitanGrip(bool value) { m_canTitanGrip = value; }
        bool CanTameExoticPets() const { return isGameMaster() || HasAuraType(SPELL_AURA_ALLOW_TAME_PET_TYPE); }

        void SetRegularAttackTime();
        void SetBaseModValue(BaseModGroup modGroup, BaseModType modType, float value) { m_auraBaseMod[modGroup][modType] = value; }
        void HandleBaseModValue(BaseModGroup modGroup, BaseModType modType, float amount, bool apply);
        float GetBaseModValue(BaseModGroup modGroup, BaseModType modType) const;
        float GetTotalBaseModValue(BaseModGroup modGroup) const;
        float GetTotalPercentageModValue(BaseModGroup modGroup) const { return m_auraBaseMod[modGroup][FLAT_MOD] + m_auraBaseMod[modGroup][PCT_MOD]; }
        void _ApplyAllStatBonuses();
        void _RemoveAllStatBonuses();

        void ResetAllPowers(bool preparation = false);

        void _ApplyOrRemoveItemEquipDependentAuras(ObjectGuid const& itemGUID = ObjectGuid::Empty, bool apply = true);
        bool CheckItemEquipDependentSpell(SpellInfo const* spellInfo = NULL, ObjectGuid const& itemGUID = ObjectGuid::Empty);

        void _ApplyItemMods(Item* item, uint8 slot, bool apply);
        void _RemoveAllItemMods();
        void _ApplyAllItemMods();
        void _ApplyAllLevelScaleItemMods(bool apply);
        void _ApplyItemBonuses(Item* item, uint8 slot, bool apply);
        void _ApplyWeaponDamage(uint8 slot, Item* item, bool apply);
        bool EnchantmentFitsRequirements(uint32 enchantmentcondition, int8 slot);
        void ToggleMetaGemsActive(uint8 exceptslot, bool apply);
        void CorrectMetaGemEnchants(uint8 slot, bool apply);
        void InitDataForForm(bool reapplyMods = false);

        void ApplyItemEquipSpell(Item* item, bool apply, bool form_change = false);
        void ApplyEquipSpell(SpellInfo const* spellInfo, Item* item, bool apply, bool form_change = false);
        void UpdateEquipSpellsAtFormChange();
        void CastItemCombatSpell(Unit* target, WeaponAttackType attType, uint32 procVictim, uint32 procEx);
        void CastItemUseSpell(Item* item, SpellCastTargets const& targets, int32* misc, ObjectGuid SpellGuid);
        void CastItemCombatSpell(Unit* target, WeaponAttackType attType, uint32 procVictim, uint32 procEx, Item* item, ItemTemplate const* proto);

        void SendEquipmentSetList();
        void SetEquipmentSet(EquipmentSetInfo::EquipmentSetData&& newEqSet);
        void DeleteEquipmentSet(uint64 setGuid);

        void SendInitWorldTimers();
        void SendInitWorldStates(uint32 zone, uint32 area);
        void SendUpdateWorldState(uint32 Field, uint32 Value);
        void SendUpdateWorldState(WorldStates Field, uint32 Value);
        void SendDirectMessage(WorldPacket const* data) const;
        void ScheduleMessageSend(WorldPacket* data, uint32 delay);

        void SendAurasForTarget(Unit* target);
        void SendSpellHistoryData();
        void SendSpellChargeData();
        void SendCategoryCooldownMods();
        void SendModifyCooldown(uint32 spellId, int32 value);

        PlayerMenu* PlayerTalkClass;
        std::vector<ItemSetEffect*> ItemSetEff;

        void SendLoot(ObjectGuid guid, LootType loot_type, bool AoeLoot = false, uint8 pool = 0);
        void SendLootRelease(ObjectGuid guid, ObjectGuid lguid = ObjectGuid::Empty);
        void SendNotifyLootItemRemoved(uint8 lootSlot, Loot* loot);
        void SendNotifyLootMoneyRemoved(Loot* loot);
        void SendLootError(ObjectGuid guid, ObjectGuid lGuid, LootError error);

        /*********************************************************/
        /***               BATTLEGROUND SYSTEM                 ***/
        /*********************************************************/

        bool InBattleground()       const                { return m_bgData.bgInstanceID != 0; }
        bool InArena()              const;
        bool InRBG()              const;
        uint32 GetBattlegroundId()  const                { return m_bgData.bgInstanceID; }
        BattlegroundTypeId GetBattlegroundTypeId() const { return m_bgData.bgTypeID; }
        Battleground* GetBattleground() const;

        uint32 GetBattlegroundQueueJoinTime(uint32 bgTypeId) const
        {
            auto x = m_bgData.bgQueuesJoinedTime.find(bgTypeId);
            if (x == m_bgData.bgQueuesJoinedTime.end())
                return 0;

            return x->second;
        }

        void AddBattlegroundQueueJoinTime(uint32 bgTypeId, uint32 joinTime)
        {
            m_bgData.bgQueuesJoinedTime[bgTypeId] = joinTime;
        }

        void RemoveBattlegroundQueueJoinTime(uint32 bgTypeId)
        {
            m_bgData.bgQueuesJoinedTime.erase(m_bgData.bgQueuesJoinedTime.find(bgTypeId)->second);
        }

        bool InBattlegroundQueue() const
        {
            for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId != BATTLEGROUND_QUEUE_NONE)
                    return true;
            return false;
        }

        BattlegroundQueueTypeId GetBattlegroundQueueTypeId(uint32 index) const { return m_bgBattlegroundQueueID[index].bgQueueTypeId; }
        uint32 GetBattlegroundQueueIndex(BattlegroundQueueTypeId bgQueueTypeId) const
        {
            for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId == bgQueueTypeId)
                    return i;
            return PLAYER_MAX_BATTLEGROUND_QUEUES;
        }
        bool IsInvitedForBattlegroundQueueType(BattlegroundQueueTypeId bgQueueTypeId) const
        {
            for (uint8 i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId == bgQueueTypeId)
                    return m_bgBattlegroundQueueID[i].invitedToInstance != 0;
            return false;
        }
        bool InBattlegroundQueueForBattlegroundQueueType(BattlegroundQueueTypeId bgQueueTypeId) const
        {
            return GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES;
        }

        void SetBattlegroundId(uint32 val, BattlegroundTypeId bgTypeId)
        {
            m_bgData.bgInstanceID = val;
            m_bgData.bgTypeID = bgTypeId;
        }
        uint32 AddBattlegroundQueueId(BattlegroundQueueTypeId val)
        {
            for (uint8 i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
            {
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId == BATTLEGROUND_QUEUE_NONE || m_bgBattlegroundQueueID[i].bgQueueTypeId == val)
                {
                    m_bgBattlegroundQueueID[i].bgQueueTypeId = val;
                    m_bgBattlegroundQueueID[i].invitedToInstance = 0;
                    return i;
                }
            }
            return PLAYER_MAX_BATTLEGROUND_QUEUES;
        }
        bool HasFreeBattlegroundQueueId()
        {
            for (uint8 i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId == BATTLEGROUND_QUEUE_NONE)
                    return true;
            return false;
        }
        void RemoveBattlegroundQueueId(BattlegroundQueueTypeId val)
        {
            for (uint8 i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
            {
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId == val)
                {
                    m_bgBattlegroundQueueID[i].bgQueueTypeId = BATTLEGROUND_QUEUE_NONE;
                    m_bgBattlegroundQueueID[i].invitedToInstance = 0;
                    return;
                }
            }
        }
        void SetInviteForBattlegroundQueueType(BattlegroundQueueTypeId bgQueueTypeId, uint32 instanceId)
        {
            for (uint8 i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
                if (m_bgBattlegroundQueueID[i].bgQueueTypeId == bgQueueTypeId)
                    m_bgBattlegroundQueueID[i].invitedToInstance = instanceId;
        }
        bool IsInvitedForBattlegroundInstance(uint32 instanceId) const
        {
            for (uint8 i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
                if (m_bgBattlegroundQueueID[i].invitedToInstance == instanceId)
                    return true;
            return false;
        }
        WorldLocation const& GetBattlegroundEntryPoint() const { return m_bgData.joinPos; }
        void SetBattlegroundEntryPoint();

        void SetBGTeam(uint32 team) { m_bgData.bgTeam = team; }
        uint32 GetBGTeam() const { return m_bgData.bgTeam ? m_bgData.bgTeam : GetTeam(); }
        TeamId GetBGTeamId() const
        {
            if (GetBGTeam() == ALLIANCE)
                return TEAM_ALLIANCE;

            if (GetBGTeam() == HORDE)
                return TEAM_HORDE;

            return TEAM_NEUTRAL;
        }

        void LeaveBattleground(bool teleportToEntryPoint = true);
        bool CanJoinToBattleground() const;
        bool CanReportAfkDueToLimit();
        void ReportedAfkBy(Player* reporter);
        void ClearAfkReports() { m_bgData.bgAfkReporter.clear(); }

        bool GetBGAccessByLevel(BattlegroundTypeId bgTypeId) const;
        bool isTotalImmunity();
        bool CanUseBattlegroundObject();
        bool isTotalImmune();
        bool CanCaptureTowerPoint();

        bool GetRandomWinner() { return m_IsBGRandomWinner; }
        void SetRandomWinner(bool isWinner);

        /*********************************************************/
        /***               OUTDOOR PVP SYSTEM                  ***/
        /*********************************************************/

        OutdoorPvP* GetOutdoorPvP() const;
        // returns true if the player is in active state for outdoor pvp objective capturing, false otherwise
        bool IsOutdoorPvPActive();

        /*********************************************************/
        /***                    REST SYSTEM                    ***/
        /*********************************************************/

        bool isRested() const { return GetRestTime() >= 10*IN_MILLISECONDS; }
        uint32 GetXPRestBonus(uint32 xp);
        uint32 GetRestTime() const { return m_restTime;}
        void SetRestTime(uint32 v) { m_restTime = v;}

        /*********************************************************/
        /***              ENVIROMENTAL SYSTEM                  ***/
        /*********************************************************/

        bool IsImmuneToEnvironmentalDamage();
        uint32 EnvironmentalDamage(EnviromentalDamage type, uint32 damage);

        /*********************************************************/
        /***               FLOOD FILTER SYSTEM                 ***/
        /*********************************************************/

        void UpdateSpeakTime();
        bool CanSpeak() const;

        /*********************************************************/
        /***                 VARIOUS SYSTEMS                   ***/
        /*********************************************************/
        void UpdateFallInformationIfNeed(MovementInfo const& minfo, uint16 opcode);
        Unit* m_mover;
        WorldObject* m_seer;
        void SetFallInformation(uint32 time, float z)
        {
            m_lastFallTime = time;
            m_lastFallZ = z;
        }
        void HandleFall(MovementInfo const& movementInfo);

        bool IsKnowHowFlyIn(uint32 mapid, uint32 zone) const;

        void SetClientControl(Unit* target, bool allowMove);

        void SetMover(Unit* target);

        void SetSeer(WorldObject* target) { m_seer = target; }
        void SetViewpoint(WorldObject* target, bool apply);
        WorldObject* GetViewpoint() const;
        void StopCastingCharm();
        void StopCastingBindSight();

        void SendPetTameResult(PetTameResult result);

        uint32 GetSaveTimer() const { return m_nextSave; }
        void   SetSaveTimer(uint32 timer) { m_nextSave = timer; }

        // Recall position
        WorldLocation m_recallLoc;
        void   SaveRecallPosition();

        void SetHomebind(WorldLocation const& loc, uint32 area_id);
        void SendBindPointUpdate();

        // Homebind coordinates
        uint32 m_homebindMapId;
        uint16 m_homebindAreaId;
        float m_homebindX;
        float m_homebindY;
        float m_homebindZ;

        WorldLocation GetStartPosition() const;

        // current pet slot
        uint32 m_currentPetNumber;
        PetSlot m_currentSummonedSlot;

        void setPetSlotWithStableMoveOrRealDelete(PetSlot slot, uint32 petID, bool isHanterPet);
        int16 SetOnAnyFreeSlot(uint32 petID);
        void cleanPetSlotForMove(PetSlot slot, uint32 petID);
        void SwapPetSlot(PetSlot oldSlot, PetSlot newSlot);
        uint32 getPetIdBySlot(uint32 slot) const { return m_PetSlots[slot]; }
        const PlayerPetSlotList &GetPetSlotList() { return m_PetSlots; }
        PetSlot getSlotForNewPet(bool full = false);
        PetSlot GetSlotForPetId(uint32 petID);
        PetSlot GetMaxCurentPetSlot() const;

        bool CanSummonPet(uint32 entry) const;
        // currently visible objects at player client
        GuidUnorderedSet  m_clientGUIDs;
        GuidSet m_extraLookList;

        bool HaveAtClient(WorldObject const* u) const { return u == this || m_clientGUIDs.find(u->GetGUID()) != m_clientGUIDs.end(); }
        void AddClient(WorldObject *u) { m_clientGUIDs.insert(u->GetGUID()); }

        void AddVignette(WorldObject *u);
        bool CanSeeVignette(WorldObject *u);
        void RemoveVignette(WorldObject *u, bool update = false);
        void SendVignette(bool force = false);

        ///! Extra look method not alow remove some creatures from player visibility by grid VisibleNotifier
        void AddToExtraLook(WorldObject *u) { m_extraLookList.insert(u->GetGUID()); } 
        void RemoveFromExtraLook(WorldObject *u) { m_extraLookList.erase(u->GetGUID()); } 
        bool HaveExtraLook(ObjectGuid guid) const { return m_extraLookList.find(guid) != m_extraLookList.end(); }

        bool IsNeverVisible(WorldObject const* seer = NULL) const;

        bool IsVisibleGloballyFor(Player const* player) const;

        void SendInitialVisiblePackets(Unit* target);
        void UpdateVisibilityForPlayer();
        void UpdateVisibilityOf(WorldObject* target);
        void UpdateTriggerVisibility();

        template<class T>
        void UpdateVisibilityOf(T* target, UpdateData& data, std::set<Unit*>& visibleNow);

        bool IsPlayerLootCooldown(uint32 entry, uint8 type = 0, uint8 diff = 0) const;
        void AddPlayerLootCooldown(uint32 entry, uint8 type = 0, bool respawn = true, uint8 diff = 0);

        uint8 m_forced_speed_changes[MAX_MOVE_TYPE];

        bool HasAtLoginFlag(AtLoginFlags f) const { return (m_atLoginFlags & f) != 0; }
        void SetAtLoginFlag(AtLoginFlags f) { m_atLoginFlags |= f; }
        void RemoveAtLoginFlag(AtLoginFlags flags, bool persist = false);

        bool isUsingLfg();
        bool inRandomLfgDungeon();
        void SetLfgBonusFaction(uint32 factionId);
        uint32 GetLfgBonusFaction() const;

        typedef std::set<uint32> DFQuestsDoneList;
        DFQuestsDoneList m_DFQuests;

        // Temporarily removed pet cache
        uint32 GetTemporaryUnsummonedPetNumber() const { return m_temporaryUnsummonedPetNumber; }
        void SetTemporaryUnsummonedPetNumber(uint32 petnumber) { m_temporaryUnsummonedPetNumber = petnumber; }
        void UnsummonPetTemporaryIfAny();
        void ResummonPetTemporaryUnSummonedIfAny();
        bool IsPetNeedBeTemporaryUnsummoned() const { return !IsInWorld() || !isAlive() || IsMounted() /*+in flight*/; }

        void SendCinematicStart(uint32 CinematicSequenceId);
        void SendMovieStart(uint32 MovieId);

        uint32 realmTransferid;
        void SetTransferId(uint32 Id) { realmTransferid = Id; }
        uint32 GetTransferId() { return realmTransferid; }

        /*********************************************************/
        /***                 INSTANCE SYSTEM                   ***/
        /*********************************************************/

        typedef std::unordered_map< uint32 /*mapId*/, InstancePlayerBind > BoundInstancesMap;

        void UpdateHomebindTime(uint32 time);

        uint32 m_HomebindTimer;
        bool m_InstanceValid;
        // permanent binds and solo binds by difficulty
        BoundInstancesMap m_boundInstances[MAX_BOUND];
        InstancePlayerBind* GetBoundInstance(uint32 mapid, Difficulty difficulty);
        BoundInstancesMap& GetBoundInstances(Difficulty difficulty);
        InstanceSave* GetInstanceSave(uint32 mapid);
        void UnbindInstance(uint32 mapid, Difficulty difficulty, bool unload = false);
        void UnbindInstance(BoundInstancesMap::iterator &itr, Difficulty difficulty, bool unload = false);
        InstancePlayerBind* BindToInstance(InstanceSave* save, bool permanent, bool load = false);
        void BindToInstance();
        void SetPendingBind(uint32 instanceId, uint32 bindTimer) { _pendingBindId = instanceId; _pendingBindTimer = bindTimer; }
        bool HasPendingBind() const { return _pendingBindId > 0; }
        void SendRaidInfo();
        void SendSavedInstances();
        static void ConvertInstancesToGroup(Player* player, Group* group, bool switchLeader);
        bool Satisfy(AccessRequirement const* ar, uint32 target_map, bool report = false);
        bool CheckInstanceLoginValid();
        bool CheckInstanceCount(uint32 instanceId) const
        {
            if (_instanceResetTimes.size() < sWorld->getIntConfig(CONFIG_MAX_INSTANCES_PER_HOUR))
                return true;
            return _instanceResetTimes.find(instanceId) != _instanceResetTimes.end();
        }

        void AddInstanceEnterTime(uint32 instanceId, time_t enterTime)
        {
            if (_instanceResetTimes.find(instanceId) == _instanceResetTimes.end())
                _instanceResetTimes.insert(InstanceTimeMap::value_type(instanceId, enterTime + HOUR));
        }

        // last used pet number (for BG's)
        uint32 GetLastPetNumber() const { return m_lastpetnumber; }
        void SetLastPetNumber(uint32 petnumber) { m_lastpetnumber = petnumber; }

        uint32 GetLastPetEntry() const { return m_LastPetEntry; }
        void SetLastPetEntry(uint32 entry) { m_LastPetEntry = entry; }

        /*********************************************************/
        /***                   GROUP SYSTEM                    ***/
        /*********************************************************/

        Group* GetGroupInvite() { return m_groupInvite; }
        void SetGroupInvite(Group* group) { m_groupInvite = group; }
        Group* GetGroup() { return m_group.getTarget(); }
        const Group* GetGroup() const { return (const Group*)m_group.getTarget(); }
        GroupReference& GetGroupRef() { return m_group; }
        void SetGroup(Group* group, int8 subgroup = -1);
        uint8 GetSubGroup() const { return m_group.getSubGroup(); }
        uint32 GetGroupUpdateFlag() const { return m_groupUpdateMask; }
        void SetGroupUpdateFlag(uint32 flag) { m_groupUpdateMask |= flag; }
        void RemoveGroupUpdateFlag(uint32 flag) { m_groupUpdateMask &= ~flag; }
        Player* GetNextRandomRaidMember(float radius);
        PartyResult CanUninviteFromGroup() const;

        // Battleground / Battlefield Group System
        void SetBattlegroundOrBattlefieldRaid(Group* group, int8 subgroup = -1);
        void RemoveFromBattlegroundOrBattlefieldRaid();
        Group* GetOriginalGroup() { return m_originalGroup.getTarget(); }
        GroupReference& GetOriginalGroupRef() { return m_originalGroup; }
        uint8 GetOriginalSubGroup() const { return m_originalGroup.getSubGroup(); }
        void SetOriginalGroup(Group* group, int8 subgroup = -1);

        void SetPassOnGroupLoot(bool bPassOnGroupLoot) { m_bPassOnGroupLoot = bPassOnGroupLoot; }
        bool GetPassOnGroupLoot() const { return m_bPassOnGroupLoot; }

        MapReference &GetMapRef() { return m_mapRef; }

        // Set map to player and add reference
        void SetMap(Map* map);
        void ResetMap();

        bool isAllowedToLoot(const Creature* creature);

        DeclinedName const* GetDeclinedNames() const { return m_declinedname; }
        uint8 GetRunesState() const { return m_runes.runeState; }
        uint32 GetRuneCooldown(uint8 index) const { return m_runes.runes[index].Cooldown; }
        float GetRuneCooldownCoef() const { return m_runes.CooldownCoef; }
        void SetRuneCooldownCoef(float Coef) { m_runes.CooldownCoef = Coef; }
        void SetRuneCooldown(uint8 index, uint32 cooldown) { m_runes.runes[index].Cooldown = cooldown; m_runes.SetRuneState(index, (cooldown == 0) ? true : false); }
        void ResyncRunes(uint8 count);
        void AddRunePower(uint8 index);
        void InitRunes();

        AchievementMgr<Player>& GetAchievementMgr() { return m_achievementMgr; }
        AchievementMgr<Player> const& GetAchievementMgr() const { return m_achievementMgr; }
        void UpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 = 0, uint32 miscValue2 = 0, uint32 miscValue3 = 0, Unit* unit = NULL, bool ignoreGroup = false);
        bool HasAchieved(uint32 achievementId) const;
        void CompletedAchievement(AchievementEntry const* entry);
        uint32 GetAchievementPoints() const;
        bool CanUpdateCriteria(uint32 criteriaTreeId, uint32 recursTree = 0) const { return true; }

        bool HasTitle(uint32 bitIndex);
        bool HasTitle(CharTitlesEntry const* title) { return HasTitle(title->MaskID); }
        void SetTitle(CharTitlesEntry const* title, bool lost = false);

        //bool isActiveObject() const { return true; }
        bool canSeeSpellClickOn(Creature const* creature) const;

        uint32 GetChampioningFaction() const { return m_ChampioningFaction; }
        uint32 GetChampioningFactionDungeonLevel() const { return m_ChampioningFactionDungeonLevel; }
        void SetChampioningFaction(uint32 faction, uint32 dungeonLevel = 0)
        {
            m_ChampioningFaction = faction;
            m_ChampioningFactionDungeonLevel = dungeonLevel;
        }
        
        Spell* m_spellModTakingSpell;

        float GetAverageItemLevel();
        bool isDebugAreaTriggers;

        void ClearWhisperWhiteList() { WhisperList.clear(); }
        void AddWhisperWhiteList(ObjectGuid guid) { WhisperList.push_back(guid); }
        bool IsInWhisperWhiteList(ObjectGuid guid);

        void ValidateMovementInfo(MovementInfo* mi);
        void SendMovementForce(AreaTrigger const* at, float x = 0.0f, float y = 0.0f, float z = 0.0f, float magnitude = 0.0f, uint32 type = 0, bool apply = false);
        void SendMovementForceAura(ObjectGuid triggerGuid, G3D::Vector3 wind, G3D::Vector3 center, float magnitude, uint8 type, bool apply);

        /*! These methods send different packets to the client in apply and unapply case.
            These methods are only sent to the current unit.
        */
        void SendMovementSetCanTransitionBetweenSwimAndFly(bool apply);
        void SendMovementSetCollisionHeight(float height);

        bool CanFly() const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_CAN_FLY); }

        //! Return collision height sent to client
        float GetCollisionHeight(bool mounted);

        // Void Storage
        bool IsVoidStorageUnlocked() const { return HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_VOID_UNLOCKED); }
        void UnlockVoidStorage() { SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_VOID_UNLOCKED); }
        void LockVoidStorage() { RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_VOID_UNLOCKED); }
        uint8 GetNextVoidStorageFreeSlot() const;
        uint8 GetNumOfVoidStorageFreeSlots() const;
        uint32 AddVoidStorageItem(VoidStorageItem const& item);
        void DeleteVoidStorageItem(uint8 slot);
        bool SwapVoidStorageItem(uint8 oldSlot, uint8 newSlot);
        VoidStorageItem* GetVoidStorageItem(uint8 slot) const;
        VoidStorageItem* GetVoidStorageItem(uint64 id, uint8& slot) const;

        void SaveCUFProfile(uint8 id, std::nullptr_t) { _CUFProfiles[id] = nullptr; }
        void SaveCUFProfile(uint8 id, std::unique_ptr<CUFProfile> profile) { _CUFProfiles[id] = std::move(profile); }
        CUFProfile* GetCUFProfile(uint8 id) const { return _CUFProfiles[id].get(); }
        uint8 GetCUFProfilesCount() const
        {
            uint8 count = 0;
            for (uint8 i = 0; i < MAX_CUF_PROFILES; ++i)
                if (_CUFProfiles[i])
                    ++count;

            return count;
        }

        uint32 GetLastTargetedGO() { return _lastTargetedGO; }
        void SetLastTargetedGO(uint32 lastTargetedGO) { _lastTargetedGO = lastTargetedGO; }
        void ShowNeutralPlayerFactionSelectUI();

        float GetPersonnalXpRate() { return m_PersonnalXpRate; }
        void SetPersonnalXpRate(float PersonnalXpRate);

        void SetKnockBackTime(uint32 timer) { m_knockBackTimer = timer; }
        uint32 GetKnockBackTime() const { return m_knockBackTimer; }

        void CheckSpellAreaOnQuestStatusChange(uint32 quest_id);

        bool CanSpeakLanguage(uint32 lang_id) const;

        /*********************************************************/
        /***              BATTLE PET SYSTEM                    ***/
        /*********************************************************/

        BattlePetMgr* GetBattlePetMgr() const { return _battlePetMgr.get(); }

        Bracket* getBracket(BracketType slot) const;
        void SendPvpRatedStats();

        /*********************************************************/
        /***              GARRISON SYSTEM                      ***/
        /*********************************************************/
        void CreateGarrison(uint32 garrSiteId);
        void DeleteGarrison();
        Garrison* GetGarrison() { return _garrison.get(); }

        /*********************************************************/
        /***              Collection SYSTEM                    ***/
        /*********************************************************/
        CollectionMgr* GetCollectionMgr() const { return _collectionMgr.get(); }

        /*********************************************************/
        /***              CHALLENGE SYSTEM                     ***/
        /*********************************************************/
        void ChallangeReward(MapChallengeModeEntry const* mode, ChallengeMode medal, uint32 recTime);

        void CheckItemCapLevel(bool hasCap);

        void ScheduleDelayedOperation(uint32 operation)
        {
            if (operation < DELAYED_END)
                m_DelayedOperations |= operation;
        }

        /*********************************************************/
        /***              E.T.C.                               ***/
        /*********************************************************/
        bool isWatchingMovie() const { return m_watching_movie; }
        void setWatchinMovie(bool s) { m_watching_movie = s; }
        uint32 getCurrentUpdateAreaID() const { return m_areaUpdateId; }
        uint32 getCurrentUpdateZoneID() const { return m_zoneUpdateId; }

        //
        AreaTriggerEntry const* GetLastAreaTrigger() { return LastAreaTrigger; }
        void SetLastAreaTrigger(AreaTriggerEntry const* at) { LastAreaTrigger = at; }

        /*********************************************************/
        /***              SCENE SYSTEM                         ***/
        /*********************************************************/
        void SendSpellScene(uint32 miscValue, SpellInfo const* spellInfo, bool apply, Position* pos);
        void SceneCompleted(uint32 sceneID);
        bool HasSceneStatus(uint32 sceneID, SceneEventStatus const& status) const;
        void TrigerScene(uint32 instanceID, std::string const type);

        //Message
        void AddListner(WorldObject* o, bool update = false);
        void RemoveListner(WorldObject* o, bool update = false);

        bool IsAdvancedCombatLoggingEnabled() const { return _advancedCombatLoggingEnabled; }
        void SetAdvancedCombatLogging(bool enabled) { _advancedCombatLoggingEnabled = enabled; }

        // client version server check and helpers
        void SendVersionMismatchWarinings();

        uint32 GetGoVisualQuestData(GameObject* go, uint32 field);

        uint16 GetScenarioId() { return m_scenarioId; }
        void SetScenarioId(uint16 scenarioId) { m_scenarioId = scenarioId; }

    protected:
        // Gamemaster whisper whitelist
        GuidList WhisperList;

        int32 m_powerRegenTimer[MAX_POWERS];
        float m_powerFraction[MAX_POWERS_PER_CLASS];

        uint32 m_contestedPvPTimer;
        uint32 m_statsUpdateTimer;
        bool m_needToUpdateRunesRegen;
        bool m_needToUpdateSpellHastDurationRecovery;
        bool m_needUpdateCastHastMods = false;
        bool m_needUpdateMeleeHastMod = false;
        bool m_needUpdateRangeHastMod = false;
        bool m_needUpdateHastMod = false;
        bool m_duelLock = false;

        std::unordered_map<uint32, uint32> m_sceneInstanceID;
        uint32 sceneInstanceID = 0;

        /*********************************************************/
        /***               BATTLEGROUND SYSTEM                 ***/
        /*********************************************************/

        /*
        this is an array of BG queues (BgTypeIDs) in which is player
        */
        struct BgBattlegroundQueueID_Rec
        {
            BattlegroundQueueTypeId bgQueueTypeId;
            uint32 invitedToInstance;
        };

        BgBattlegroundQueueID_Rec m_bgBattlegroundQueueID[PLAYER_MAX_BATTLEGROUND_QUEUES];
        BGData                    m_bgData;

        bool m_IsBGRandomWinner;

        /*********************************************************/
        /***                    QUEST SYSTEM                   ***/
        /*********************************************************/

        //We allow only one timed quest active at the same time. Below can then be simple value instead of set.
        typedef std::set<uint32> QuestSet;
        typedef std::set<uint32> SeasonalQuestSet;
        typedef std::unordered_map<uint32,SeasonalQuestSet> SeasonalEventQuestMap;
        QuestSet m_timedquests;
        QuestSet m_weeklyquests;
        QuestSet m_dailyquests;
        SeasonalEventQuestMap m_seasonalquests;

        ObjectGuid m_divider;
        uint32 m_ingametime;

        /*********************************************************/
        /***                   LOAD SYSTEM                     ***/
        /*********************************************************/

        void _LoadActions(PreparedQueryResult result);
        void _LoadAuras(PreparedQueryResult result, PreparedQueryResult resultEffect, uint32 timediff);
        void _LoadGlyphAuras();
        void _LoadBoundInstances(PreparedQueryResult result);
        void _LoadArtifacts(PreparedQueryResult result);
        void _LoadInventory(PreparedQueryResult result, uint32 timeDiff);
        void _LoadVoidStorage(PreparedQueryResult result);
        void _LoadMailInit(PreparedQueryResult resultUnread, PreparedQueryResult resultDelivery);
        void _LoadMail();
        void _LoadMailedItems(Mail* mail);
        void _LoadQuestStatus(PreparedQueryResult result);
        void _LoadQuestStatusObjectives(PreparedQueryResult result);
        void _LoadQuestStatusRewarded(PreparedQueryResult result);
        void _LoadDailyQuestStatus(PreparedQueryResult result);
        void _LoadWeeklyQuestStatus(PreparedQueryResult result);
        void _LoadSeasonalQuestStatus(PreparedQueryResult result);
        void _LoadRandomBGStatus(PreparedQueryResult result);
        void _LoadGroup(PreparedQueryResult result);
        void _LoadSkills(PreparedQueryResult result);
        void _LoadSpells(PreparedQueryResult result);
        void _LoadSpellRewards();
        void _LoadAccountSpells(PreparedQueryResult result);
        void _LoadFriendList(PreparedQueryResult result);
        bool _LoadHomeBind(PreparedQueryResult result);
        void _LoadDeclinedNames(PreparedQueryResult result);
        void _LoadEquipmentSets(PreparedQueryResult result);
        void _LoadBGData(PreparedQueryResult result);
        void _LoadGlyphs(PreparedQueryResult result);
        void _LoadTalents(PreparedQueryResult result);
        void _LoadInstanceTimeRestrictions(PreparedQueryResult result);
        void _LoadCurrency(PreparedQueryResult result);
        void _LoadArchaelogy(PreparedQueryResult result);
        void _LoadCUFProfiles(PreparedQueryResult result);
        void _LoadHonor(PreparedQueryResult result, PreparedQueryResult result2);
        void _LoadLootCooldown(PreparedQueryResult result);

        /*********************************************************/
        /***                   SAVE SYSTEM                     ***/
        /*********************************************************/

        void _SaveActions(SQLTransaction& trans);
        void _SaveAuras(SQLTransaction& trans);
        void _SaveInventory(SQLTransaction& trans);
        void _SaveVoidStorage(SQLTransaction& trans);
        void _SaveMail(SQLTransaction& trans);
        void _SaveQuestStatus(SQLTransaction& trans);
        void _SaveDailyQuestStatus(SQLTransaction& trans);
        void _SaveWeeklyQuestStatus(SQLTransaction& trans);
        void _SaveSeasonalQuestStatus(SQLTransaction& trans);
        void _SaveSkills(SQLTransaction& trans);
        void _SaveSpells(SQLTransaction& trans);
        void _SaveEquipmentSets(SQLTransaction& trans);
        void _SaveBGData(SQLTransaction& trans);
        void _SaveGlyphs(SQLTransaction& trans);
        void _SaveTalents(SQLTransaction& trans);
        void _SaveStats(SQLTransaction& trans);
        void _SaveInstanceTimeRestrictions(SQLTransaction& trans);
        void _SaveCurrency(SQLTransaction& trans);
        void _SaveArchaelogy(SQLTransaction& trans);
        void _SaveBrackets(SQLTransaction& trans);
        void _SaveCUFProfiles(SQLTransaction& trans);
        void _SaveHonor();
        void _SaveLootCooldown(SQLTransaction& trans);

        /*********************************************************/
        /***              ENVIRONMENTAL SYSTEM                 ***/
        /*********************************************************/
        void HandleSobering();
        void SendMirrorTimer(MirrorTimerType type, uint32 maxValue, uint32 currentValue, int32 regen);
        void StopMirrorTimer(MirrorTimerType Type);
        void HandleDrowning(uint32 time_diff);
        int32 getMaxTimer(MirrorTimerType timer);

        /*********************************************************/
        /***                  HONOR SYSTEM                     ***/
        /*********************************************************/
        time_t m_lastHonorUpdateTime;

        void outDebugValues() const;
        ObjectGuid m_lootGuid;
        AoeMap m_AoelootGuidList;

        uint32 m_team;
        uint32 m_nextSave;
        time_t m_speakTime;
        uint32 m_speakCount;
        Difficulty m_dungeonDifficulty;
        Difficulty m_raidDifficulty;
        Difficulty m_legacyRaidDifficulty;
        Difficulty m_prevMapDifficulty;

        uint32 m_atLoginFlags;

        Item* m_items[PLAYER_SLOTS_COUNT];
        uint32 m_currentBuybackSlot;

        PlayerCurrenciesMap _currencyStorage;
        PlayerVignettesMap m_vignettes;
        bool _vignetteChanged = false;

        VoidStorageItem* _voidStorageItems[VOID_STORAGE_MAX_SLOT];
        std::array<std::unique_ptr<CUFProfile>, MAX_CUF_PROFILES> _CUFProfiles;

        std::vector<Item*> m_itemUpdateQueue;
        bool m_itemUpdateQueueBlocked;

        uint32 m_ExtraFlags;
        ObjectGuid m_curSelection;
        ObjectGuid m_lastSelection;
        DigSiteInfo m_digsite;

        QuestStatusMap m_QuestStatus;
        QuestStatusSaveMap m_QuestStatusSave;

        RewardedQuestSet m_RewardedQuests;
        QuestStatusSaveMap m_RewardedQuestsSave;

        SkillStatusMap mSkillStatus;

        ObjectGuid::LowType m_GuildIdInvited;
        ObjectGuid m_GuildInviterGuid;

        PlayerMails m_mail;
        PlayerSpellMap m_spells;
        std::unordered_map<uint32 /*overridenSpellId*/, std::unordered_set<uint32> /*newSpellId*/> m_overrideSpells;
        ItemSpellList m_itemSpellList;
        uint32 m_lastPotionId;                              // last used health/mana potion in combat, that block next potion use
        std::map<uint32, uint32> spellMountReplacelist;

        GlobalCooldownMgr m_GlobalCooldownMgr;

        PlayerTalentInfo* _talentMgr;

        ActionButtonList m_actionButtons;

        float m_auraBaseMod[BASEMOD_END][MOD_END];
        uint32 m_baseSpellPower;
        uint32 m_baseManaRegen;
        uint32 m_baseHealthRegen;
        int32 m_spellPenetrationItemMod;

        SpellModList m_spellMods[MAX_SPELLMOD];
        //uint32 m_pad;
//        Spell* m_spellModTakingSpell;  // Spell for which charges are dropped in spell::finish

        EnchantDurationList m_enchantDuration;
        ItemDurationList m_itemDuration;
        ItemDurationList m_itemSoulboundTradeable;

        void ResetTimeSync();
        void SendTimeSync();

        ResurrectionData* _resurrectionData;

        WorldSession* m_session;

        typedef std::list<Channel*> JoinedChannelsList;
        JoinedChannelsList m_channels;

        uint8 m_cinematic;

        TradeData* m_trade;

        bool   m_DailyQuestChanged;
        bool   m_WeeklyQuestChanged;
        bool   m_SeasonalQuestChanged;
        time_t m_lastDailyQuestTime;

        uint32 m_drunkTimer;
        uint32 m_weaponChangeTimer;

        bool m_zoneUpdateAllow;
        Position m_lastZoneUpdPos;
        uint32 m_zoneUpdateId;
        uint32 m_zoneUpdateTimer;
        uint32 m_areaUpdateId;

        uint32 m_deathTimer;
        time_t m_deathExpireTime;

        uint32 m_restTime;

        uint32 m_WeaponProficiency;
        uint32 m_ArmorProficiency;
        bool m_canParry;
        bool m_canBlock;
        bool m_canTitanGrip;
        uint8 m_swingErrorMsg;

        ////////////////////Rest System/////////////////////
        time_t time_inn_enter;
        uint32 inn_pos_mapid;
        float  inn_pos_x;
        float  inn_pos_y;
        float  inn_pos_z;
        float m_rest_bonus;
        RestType rest_type;
        ////////////////////Rest System/////////////////////

        ////////////////////Pet System/////////////////////
        void LoadPetSlot(std::string const &data);
        PetDataList m_PetDataList;
        PlayerPetSlotList m_PetSlots;
        ////////////////////Rest System/////////////////////
    public:
        uint32 GetTimeSync() const { return m_timeSyncServer; }

        void SetInPvPCombat(bool set);
        bool IsInPvPCombat() const { return m_PvPCombat; }
        void UpdatePvP(uint32 diff);
        void SetPvPTimer(uint32 duration) { m_PvPCombatTimer = duration; }
        void OnEnterPvPCombat();
        void OnLeavePvPCombat();
    protected:
        uint32 m_PvPCombatTimer;
        bool m_PvPCombat;

        //kill honor sistem
        KillInfoMap m_killsPerPlayer;
        bool m_flushKills;
        bool m_saveKills;

        HonorInfoContainer m_honorInfo;

        //WorldFilter
        std::string m_sentMsgCache;

        // Social
        PlayerSocial *m_social;

        // Groups
        GroupReference m_group;
        GroupReference m_originalGroup;
        Group* m_groupInvite;
        uint32 m_groupUpdateMask;
        bool m_bPassOnGroupLoot;

        // last used pet number (for BG's)
        uint32 m_lastpetnumber;
        uint32 m_LastPetEntry;

        // Player summoning
        time_t m_summon_expire;
        uint32 m_summon_mapid;
        float  m_summon_x;
        float  m_summon_y;
        float  m_summon_z;

        DeclinedName *m_declinedname;
        Runes m_runes;
        EquipmentSetContainer _equipmentSets;

        bool CanAlwaysSee(WorldObject const* obj) const;

        bool IsAlwaysDetectableFor(WorldObject const* seer) const;

        uint8 m_grantableLevels;

    private:
        // internal common parts for CanStore/StoreItem functions
        InventoryResult CanStoreItem_InSpecificSlot(uint8 bag, uint8 slot, ItemPosCountVec& dest, ItemTemplate const* pProto, uint32& count, bool swap, Item* pSrcItem) const;
        InventoryResult CanStoreItem_InBag(uint8 bag, ItemPosCountVec& dest, ItemTemplate const* pProto, uint32& count, bool merge, bool non_specialized, Item* pSrcItem, uint8 skip_bag, uint8 skip_slot) const;
        InventoryResult CanStoreItem_InInventorySlots(uint8 slot_begin, uint8 slot_end, ItemPosCountVec& dest, ItemTemplate const* pProto, uint32& count, bool merge, Item* pSrcItem, uint8 skip_bag, uint8 skip_slot) const;
        Item* _StoreItem(uint16 pos, Item* pItem, uint32 count, bool clone, bool update);
        Item* _LoadItem(SQLTransaction& trans, uint32 zoneId, uint32 timeDiff, Field* fields);

        GuidSet m_refundableItems;
        void SendRefundInfo(Item* item);
        void RefundItem(Item* item);
        void SendItemRefundResult(Item* item, ItemExtendedCostEntry const* iece, uint8 error);

        int32 CalculateReputationGain(uint32 creatureOrQuestLevel, int32 rep, int32 faction, bool for_quest, bool noQuestBonus = false);
        void AdjustQuestReqItemCount(Quest const* quest);

        bool IsCanDelayTeleport() const { return m_bCanDelayTeleport; }
        void SetCanDelayTeleport(bool setting) { m_bCanDelayTeleport = setting; }
        bool IsHasDelayedTeleport() const { return m_bHasDelayedTeleport; }
        void SetDelayedTeleportFlag(bool setting) { m_bHasDelayedTeleport = setting; }

        MapReference m_mapRef;

        void UpdateCharmedAI();

        uint32 m_lastFallTime;
        float  m_lastFallZ;

        int32 m_MirrorTimer[MAX_TIMERS];
        uint8 m_MirrorTimerFlags;
        uint8 m_MirrorTimerFlagsLast;
        bool m_isInWater;

        // Current teleport data
        WorldLocation m_teleport_dest;
        uint32 m_teleport_options;
        bool mSemaphoreTeleport_Near;
        bool mSemaphoreTeleport_Far;

        uint32 m_DelayedOperations;
        bool m_bCanDelayTeleport;
        bool m_bHasDelayedTeleport;

        // Temporary removed pet cache
        uint32 m_temporaryUnsummonedPetNumber;
        uint32 m_oldpetspell;

        AchievementMgr<Player> m_achievementMgr;
        ReputationMgr  m_reputationMgr;

        RPPMLastChanceToProc m_rppmLastChanceToProc;
        RPPMLastSuccessfulProc m_rppmLastSuccessfulProc;
        RPPMSpellCooldowns m_rppmspellCooldowns;
        SpellCooldowns m_spellCooldowns;
        SpellChargeDataMap m_spellChargeData;

        uint32 m_ChampioningFaction;
        uint32 m_ChampioningFactionDungeonLevel;

        std::queue<uint32> m_timeSyncQueue;
        uint32 m_timeSyncTimer;
        uint32 m_timeSyncClient;
        uint32 m_timeSyncServer;

        InstanceTimeMap _instanceResetTimes;
        uint32 _pendingBindId;
        uint32 _pendingBindTimer;

        uint32 _activeCheats;

        PhaseMgr phaseMgr;

        uint32 _lastTargetedGO;
        float m_PersonnalXpRate;

        uint32 m_knockBackTimer;

        uint32 m_groupUpdateDelay;

        uint32 m_clientCheckDelay;
        uint32 m_clientKickDelay;

        BracketList m_BracketsList;

        PlayerLootCooldownMap m_playerLootCooldown[MAX_LOOT_COOLDOWN_TYPE];

        bool m_watching_movie;
        bool plrUpdate;

        AreaTriggerEntry const *LastAreaTrigger;
        std::unordered_map<uint32, SceneEventStatus> m_sceneStatus;

        bool _advancedCombatLoggingEnabled;

        std::unique_ptr<Garrison> _garrison;
        std::unique_ptr<CollectionMgr> _collectionMgr;
        std::unique_ptr<BattlePetMgr> _battlePetMgr;

        uint16 m_scenarioId = 0;
};

void AddItemsSetItem(Player*player, Item* item);
void RemoveItemsSetItem(Player*player, ItemTemplate const* proto);

// "the bodies of template functions must be made available in a header file"
template <class T> T Player::ApplySpellMod(uint32 spellId, SpellModOp op, T &basevalue, Spell* spell)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return 0;
    float totalmul = 1.0f;
    int32 totalflat = 0;
    T savevalue = basevalue;
    SpellModifier* modCost = NULL;
    SpellModifier* modCast = NULL;

    // Drop charges for triggering spells instead of triggered ones
    if (m_spellModTakingSpell)
        spell = m_spellModTakingSpell;

    for (SpellModList::iterator itr = m_spellMods[op].begin(); itr != m_spellMods[op].end(); ++itr)
    {
        SpellModifier* mod = *itr;

        // Charges can be set only for mods with auras
        if (!mod->ownerAura)
            ASSERT(mod->charges == 0);

        if (!IsAffectedBySpellmod(spellInfo, mod, spell))
            continue;

        SpellInfo const* affectSpell = sSpellMgr->GetSpellInfo(mod->spellId);
        if(!affectSpell)
            continue;

        if((affectSpell->HasAttribute(SPELL_ATTR0_ONLY_STEALTHED)) && !HasStealthAura())
            continue;

        //Don`t moded value if allready max moded
        if(mod->value == 0 || (totalmul <= 0.0f && mod->value < 0) || (totalflat < 0 && ((basevalue + totalflat) <= 0) && mod->value < 0))
            continue;

        if (mod->type == SPELLMOD_FLAT)
            totalflat += mod->value;
        else if (mod->type == SPELLMOD_PCT)
        {
            // skip percent mods for null basevalue (most important for spell mods with charges)
            if (basevalue == T(0))
                continue;

            // special case (skip > 10sec spell casts for instant cast setting)
            if (mod->op == SPELLMOD_CASTING_TIME && basevalue >= T(10000) && mod->value <= -100)
                continue;

            if(spellId == 116858 && mod->spellId == 117828) // Chaos Bolt
            {
                if(mod->ownerAura->GetCharges() < 3)
                    continue;
            }

            if (mod->op == SPELLMOD_CASTING_TIME && mod->value < 0)
            {
                if(!modCast || mod->value <= modCast->value)
                    modCast = mod;
                //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Player::ApplySpellMod SPELLMOD_CASTING_TIME totalflat %i totalmul %f basevalue %i spellId %i mod->spellId %i", totalflat, (totalmul - 1.0f), basevalue, spellId, mod->spellId);
                continue;
            }
            if (mod->op == SPELLMOD_COST && mod->value < 0)
            {
                if(!modCost || mod->value <= modCost->value)
                    modCost = mod;
                //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Player::ApplySpellMod SPELLMOD_COST totalflat %i totalmul %f basevalue %i spellId %i mod->spellId %i", totalflat, (totalmul - 1.0f), basevalue, spellId, mod->spellId);
                continue;
            }

            totalmul += CalculatePct(1.0f, mod->value);
        }

        DropModCharge(mod, spell);
    }

    if(modCast)
    {
        totalmul += CalculatePct(1.0f, modCast->value);
        DropModCharge(modCast, spell);
        //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Player::ApplySpellMod totalflat %i totalmul %f basevalue %i, modCast->spellId %i", totalflat, (totalmul - 1.0f), basevalue, modCast->spellId);
    }
    if(modCost)
    {
        totalmul += CalculatePct(1.0f, modCost->value);
        DropModCharge(modCost, spell);
        //sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "Player::ApplySpellMod totalflat %i totalmul %f basevalue %i, modCost->spellId %i", totalflat, (totalmul - 1.0f), basevalue, modCost->spellId);
    }
    //float diff = (float)basevalue * (totalmul - 1.0f) + (float)totalflat;
    //basevalue = T((float)basevalue + diff);
    basevalue = ((float)basevalue + (float)totalflat) * totalmul;
    float diff = float(savevalue - basevalue);

    return T(diff);
}

class TeleportEvent : public BasicEvent
{
    uint32 mapId, params, m_delay;
    float x, y, z, o;
    bool lfg;
    bool scheduleDelayed;
    Player* m_owner;

public:
    explicit TeleportEvent(Player* owner, uint32 _mapId, float _x, float _y, float _z, float _o, uint32 _params, bool _lfg, bool _scheduleDelayed, uint32 delay) :
        m_owner(owner), mapId(_mapId), x(_x), y(_y), z(_z), o(_o), params(_params), lfg(_lfg), scheduleDelayed(_scheduleDelayed), m_delay(delay) { }

    explicit TeleportEvent(Player* owner, WorldLocation const& loc, uint32 _params, bool _lfg, bool _scheduleDelayed, uint32 delay) :
        m_owner(owner), mapId(loc.GetMapId()), x(loc.GetPositionX()), y(loc.GetPositionY()), z(loc.GetPositionZ()), o(loc.GetOrientation()), params(_params), lfg(_lfg), scheduleDelayed(_scheduleDelayed), m_delay(delay) { }

    ~TeleportEvent() { }

    virtual bool Execute(uint64, uint32);
    bool Schedule();
};
#endif

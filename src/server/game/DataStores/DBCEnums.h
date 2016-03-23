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

#ifndef DBCENUMS_H
#define DBCENUMS_H

#include "ItemTemplate.h"

struct DBCPosition2D
{
    float X;
    float Y;
};

struct DBCPosition3D
{
    float X;
    float Y;
    float Z;
};

struct DBCPosition4D
{
    float X;
    float Y;
    float Z;
    float O;
};

enum LevelLimit
{
    // Client expected level limitation, like as used in DBC item max levels for "until max player level"
    // use as default max player level, must be fit max level for used client
    // also see MAX_LEVEL and STRONG_MAX_LEVEL define
#define DEFAULT_MAX_LEVEL 110
#define DEFAULT_MAX_SKILL_VALUE 600

    // Server side limitation. Base at used code requirements.
    // also see MAX_LEVEL and GT_MAX_LEVEL define
#define STRONG_MAX_LEVEL 255
};

static uint8 const MAX_LEVEL = 110; // client supported max level for player/pets/etc. Avoid overflow or client stability affected. also see GT_MAX_LEVEL define

enum BattlegroundBracketId                                  // bracketId for level ranges
{
    BG_BRACKET_ID_FIRST          = 0,
    BG_BRACKET_ID_LAST           = 16,

    MAX_BATTLEGROUND_BRACKETS
};

enum AreaTeams
{
    AREATEAM_NONE  = 0,
    AREATEAM_ALLY  = 2,
    AREATEAM_HORDE = 4,
    AREATEAM_ANY   = 6
};

enum AchievementFaction
{
    ACHIEVEMENT_FACTION_HORDE           = 0,
    ACHIEVEMENT_FACTION_ALLIANCE        = 1,
    ACHIEVEMENT_FACTION_ANY             = -1
};

enum AchievementFlags
{
    ACHIEVEMENT_FLAG_COUNTER                = 0x00000001,    // Just count statistic (never stop and complete)
    ACHIEVEMENT_FLAG_HIDDEN                 = 0x00000002,    // Not sent to client - internal use only
    ACHIEVEMENT_FLAG_PLAY_NO_VISUAL         = 0x00000004,    // Client does not play achievement earned visual
    ACHIEVEMENT_FLAG_SUMM                   = 0x00000008,    // Use summ criteria value from all requirements (and calculate max value)
    ACHIEVEMENT_FLAG_MAX_USED               = 0x00000010,    // Show max criteria (and calculate max value ??)
    ACHIEVEMENT_FLAG_REQ_COUNT              = 0x00000020,    // Use not zero req count (and calculate max value)
    ACHIEVEMENT_FLAG_AVERAGE                = 0x00000040,    // Show as average value (value / time_in_days) depend from other flag (by def use last criteria value)
    ACHIEVEMENT_FLAG_BAR                    = 0x00000080,    // Show as progress bar (value / max vale) depend from other flag (by def use last criteria value)
    ACHIEVEMENT_FLAG_REALM_FIRST_REACH      = 0x00000100,    //
    ACHIEVEMENT_FLAG_REALM_FIRST_KILL       = 0x00000200,    //
    ACHIEVEMENT_FLAG_UNK3                   = 0x00000400,    // ACHIEVEMENT_FLAG_HIDE_NAME_IN_TIE
    ACHIEVEMENT_FLAG_REALM_FIRST_GUILD      = 0x00000800,    // first guild on realm done something
    ACHIEVEMENT_FLAG_SHOW_IN_GUILD_NEWS     = 0x00001000,    // Shows in guild news
    ACHIEVEMENT_FLAG_SHOW_IN_GUILD_HEADER   = 0x00002000,    // Shows in guild news header
    ACHIEVEMENT_FLAG_GUILD                  = 0x00004000,    //
    ACHIEVEMENT_FLAG_SHOW_GUILD_MEMBERS     = 0x00008000,    //
    ACHIEVEMENT_FLAG_SHOW_CRITERIA_MEMBERS  = 0x00010000,    //
    ACHIEVEMENT_FLAG_ACCOUNT                = 0x00020000     //
};

enum ModifierTreeFlags
{
    MODIFIERTREE_FLAG_UNK                 = 0x00000001,    //
    MODIFIERTREE_FLAG_CONDITION           = 0x00000002,    // Is condition
    MODIFIERTREE_FLAG_MAIN                = 0x00000004,    // Is main and not condition
    MODIFIERTREE_FLAG_PARENT              = 0x00000008,    // Is parent and not condition
};

enum
{
    MAX_CRITERIA_REQUIREMENTS          = 1,
    MAX_ADDITIONAL_CRITERIA_CONDITIONS = 3
};

enum AchievementCriteriaCondition
{
    ACHIEVEMENT_CRITERIA_CONDITION_NONE                 = 0,
    ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH             = 1,    // reset progress on death
    ACHIEVEMENT_CRITERIA_CONDITION_UNK2                 = 2,    // only used in "Complete a daily quest every day for five consecutive days"
    ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP               = 3,    // requires you to be on specific map, reset at change
    ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE              = 4,    // only used in "Win 10 arenas without losing"
    ACHIEVEMENT_CRITERIA_CONDITION_NOT_LOSE_AURA        = 5,    // reset progress on removed aura
    ACHIEVEMENT_CRITERIA_CONDITION_UNK8                 = 8,
    ACHIEVEMENT_CRITERIA_CONDITION_NO_SPELL_HIT         = 9,    // requires the player not to be hit by specific spell
    ACHIEVEMENT_CRITERIA_CONDITION_NOT_IN_GROUP         = 10,   // requires the player not to be in group
    ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE_PET_BATTLE   = 11,   // only used in "Win 10 consecutive pet battles."
    ACHIEVEMENT_CRITERIA_CONDITION_UNK13                = 13    // unk
};

enum AchievementCriteriaAdditionalCondition
{
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_DRUNK_VALUE          = 1,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK2                        = 2,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_ITEM_LEVEL                  = 3,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_CREATURE_ENTRY       = 4,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_MUST_BE_PLAYER       = 5,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_MUST_BE_DEAD         = 6,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_MUST_BE_ENEMY        = 7,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_HAS_AURA             = 8,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_HAS_AURA             = 10,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_MUST_BE_MOUNTED      = 11,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_ITEM_QUALITY_MIN            = 14,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_ITEM_QUALITY_EQUALS         = 15,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK16                       = 16,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_AREA_OR_ZONE         = 17,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_AREA_OR_ZONE         = 18,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_MAP_DIFFICULTY              = 20,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_CREATURE_YIELDS_XP   = 21,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_ARENA_TEAM_SIZE      = 24,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_RACE                 = 25,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_CLASS                = 26,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_RACE                 = 27,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_CLASS                = 28,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_MAX_GROUP_MEMBERS           = 29,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_CREATURE_TYPE        = 30,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_MAP                  = 32,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UPDATE_NEXT_VERSION         = 33, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_LEVEL_IN_SLOT     = 34, // all battlepet level in slot
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_WITHOUT_GROUP               = 35,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_MIN_PERSONAL_RATING         = 37,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TITLE_BIT_INDEX             = 38,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SOURCE_LEVEL                = 39,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_LEVEL                = 40,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_ZONE                 = 41,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_HEALTH_PERCENT_BELOW = 46,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK55                       = 55,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_MIN_ACHIEVEMENT_POINTS      = 56,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_REQUIRES_LFG_GROUP          = 58,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK60                       = 60,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_REQUIRES_GUILD_GROUP        = 61,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GUILD_REPUTATION            = 62,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_RATED_BATTLEGROUND          = 63,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_PROJECT_RARITY              = 65,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_PROJECT_RACE                = 66,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_SPEC_EVENT                  = 67, // While celebrate 10-th wow and etc.
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_DUNGEON_FIFFICULTY          = 68,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK69                       = 69, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_MIN_LEVEL            = 70,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_TARGET_MAX_LEVEL            = 71,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK73                       = 73, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK74                       = 74, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK75                       = 75, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_ACHIEV_POINTS               = 76,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_FEMALY            = 78,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_HP_LOW_THAT       = 79,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK80                       = 80, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_MASTER_PET_TAMER  = 81,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK82                       = 82, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_CHALANGER_RATE              = 83,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLE_PET_ID               = 84,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK86                       = 86, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK87                       = 87, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK88                       = 88, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_QUALITY           = 89,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_WIN_IN_PVP        = 90,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_BATTLEPET_SPECIES           = 91,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_EXPANSION_LESS              = 92, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK93                       = 93, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK94                       = 94, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_REPUTATION                  = 95, // Reputation check
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_ITEM_CLASS_AND_SUBCLASS     = 96,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK99                       = 99, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_UNK112                      = 112, // NYI
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_CURRENCY                    = 121, // Currency
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_DEATH_COUNTER               = 122, // Death instance counter
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_ARENA_SEASON                = 125, // Arena season check
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GARRISON_FOLLOWER_QUALITY   = 145,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GARRISON_FOLLOWER_LEVEL     = 146,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GARRISON_RARE_MISSION       = 147,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GARRISON_BUILDING_LEVEL     = 149,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GARRISON_MISSION_TYPE       = 167,
    ACHIEVEMENT_CRITERIA_ADDITIONAL_CONDITION_GARRISON_FOLLOWER_ILVL      = 184,
};

enum AchievementCriteriaFlags
{
    ACHIEVEMENT_CRITERIA_FLAG_SHOW_PROGRESS_BAR = 0x00000001,         // Show progress as bar
    ACHIEVEMENT_CRITERIA_FLAG_HIDDEN            = 0x00000002,         // Not show criteria in client
    ACHIEVEMENT_CRITERIA_FLAG_FAIL_ACHIEVEMENT  = 0x00000004,         // BG related??
    ACHIEVEMENT_CRITERIA_FLAG_RESET_ON_START    = 0x00000008,         //
    ACHIEVEMENT_CRITERIA_FLAG_IS_DATE           = 0x00000010,         // not used
    ACHIEVEMENT_CRITERIA_FLAG_MONEY_COUNTER     = 0x00000020          // Displays counter as money
};

enum AchievementCriteriaTimedTypes
{
    ACHIEVEMENT_TIMED_TYPE_EVENT            = 1,    // Timer is started by internal event with id in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_UNK2             = 2,    // Unknown
    ACHIEVEMENT_TIMED_TYPE_SPELL_CASTER     = 5,    // Timer is started by casting a spell with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET     = 6,    // Timer is started by being target of spell with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_CREATURE         = 7,    // Timer is started by killing creature with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET2    = 8,    // Timer is started by being target of spell with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_QUEST            = 9,    // Timer is started by accepting quest with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_CREATURE2        = 10,   // Timer is started by killing creature with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_BATTLEPET        = 11,   // Timer is started by win in battlepet to first lose
    ACHIEVEMENT_TIMED_TYPE_ITEM2            = 12,   // Timer is started by using item with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_EVENT2           = 13,   // Timer is started by internal event with id in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_SCENARIO_STAGE   = 14,   // Timer is started by changing stages in a scenario

    ACHIEVEMENT_TIMED_TYPE_MAX
};

enum AchievementCriteriaTypes
{
    ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE                 = 0,
    ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                        = 1,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS = 3, // struct { uint32 itemCount; }
    ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL                   = 5,
    ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL             = 7,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT          = 8,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT          = 9,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY    = 10, // you have to complete a daily quest x times in a row
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE       = 11,
    ACHIEVEMENT_CRITERIA_TYPE_CURRENCY                      = 12,
    ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE                   = 13,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST          = 14,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND         = 15,
    ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP                  = 16,
    ACHIEVEMENT_CRITERIA_TYPE_DEATH                         = 17,
    ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON              = 18,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID                 = 19,
    ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE            = 20,
    ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER              = 23,
    ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING            = 24,
    ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM                   = 26,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST                = 27,
    ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET               = 28,
    ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL                    = 29,
    ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE          = 30,
    ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA        = 31,
    ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA                     = 32,
    ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA                    = 33,
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL                   = 34,
    ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL                = 35,
    ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM                      = 36,
    ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA               = 37,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING           = 38,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING       = 39,
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL             = 40,
    ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM                      = 41,
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM                     = 42,
    ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA                  = 43,
    ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK                      = 44,
    ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT                 = 45,
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION               = 46,
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION       = 47,
    ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP             = 48,
    ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM               = 49,  // TODO: itemlevel is mentioned in text but not present in dbc
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT             = 50,
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT            = 51,
    ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS                      = 52,
    ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                       = 53,
    ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE                      = 54,
    ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE                  = 55,
    ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS             = 56, // TODO: in some cases map not present, and in some cases need do without die
    ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM                    = 57,
    ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS            = 59,
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS        = 60,
    ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS       = 61,
    ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD       = 62,
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING     = 63,
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER          = 65,
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL           = 66,
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY                    = 67,
    ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT                = 68,
    ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2              = 69,
    ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL              = 70,
    ACHIEVEMENT_CRITERIA_TYPE_INSTANSE_MAP_ID               = 71,
    ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT            = 72,
    ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT                  = 73,  // should be thrown by scripts
    ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE              = 74, // TODO: title id is not mentioned in dbc
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS        = 75,
    ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL                      = 76,
    ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL                     = 77,
    ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE            = 78,  // TODO: creature type (demon, undead etc.) is not stored in dbc
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS       = 80,
    ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION                = 82,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID           = 83,
    ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS                  = 84,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD          = 85,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED      = 86,
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION       = 87,
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION       = 88,
    ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS                = 89,
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM                = 90,
    ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM             = 91,
    ACHIEVEMENT_CRITERIA_TYPE_SCRIPT_EVENT_2                = 92,  // should be thrown by scripts
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED                     = 93,
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED                    = 94,
    ACHIEVEMENT_CRITERIA_TYPE_ADD_BATTLE_PET_JOURNAL        = 96,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT             = 101,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED          = 102,
    ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED         = 103,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED           = 104,
    ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED        = 105,
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED      = 106,
    ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED               = 107,
    ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN            = 108,
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE                     = 109,
    ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2                   = 110,  // TODO: target entry is missing
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE              = 112,
    ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL           = 113,
    ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS           = 114,
    ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS       = 115,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETED_LFG_DUNGEONS        = 118, // Number of times the player completed an LFG dungeon
    ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS = 119,
    ACHIEVEMENT_CRITERIA_TYPE_INITIATED_KICK_IN_LFG         = 120, // Number of times the player kicked someone in LFG, initiating the kick
    ACHIEVEMENT_CRITERIA_TYPE_VOTED_KICK_IN_LFG             = 121, // Number of times the player kicked someone in LFG, voting, but not initiating
    ACHIEVEMENT_CRITERIA_TYPE_BEING_KICKED_IN_LFG           = 122, // Number of times the player has been kicked by someone in LFG
    ACHIEVEMENT_CRITERIA_TYPE_ABANDONED_LFG_DUNGEONS        = 123, // Number of times the player abandoned an LFG dungeon
    ACHIEVEMENT_CRITERIA_TYPE_SPENT_GOLD_GUILD_REPAIRS      = 124,
    ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL             = 125,
    ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD             = 126,
    ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL               = 127,
    ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS          = 128,
    ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS = 129,
    ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND        = 130,
    ACHIEVEMENT_CRITERIA_TYPE_REACH_RBG_RATING              = 132,
    ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_EMBLEM              = 133,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD         = 134,
    ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD         = 135,
    ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD      = 136,
    ACHIEVEMENT_CRITERIA_TYPE_UNK137                        = 137, // Number of times the player joined an LFG dungeon with a tank that leaves early
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_DUNGEON_CHALLENGES = 138, //struct { Flag flag; uint32 count; } 1: Guild Dungeon, 2:Guild Challenge, 3:Guild battlefield
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGES     = 139, //struct { uint32 count; } Guild Challenge
    ACHIEVEMENT_CRITERIA_TYPE_UNK140                        = 140, //criteria found but achive not found
    ACHIEVEMENT_CRITERIA_TYPE_UNK141                        = 141, //criteria found but achive not found
    ACHIEVEMENT_CRITERIA_TYPE_UNK142                        = 142, //criteria found but achive not found
    ACHIEVEMENT_CRITERIA_TYPE_UNK143                        = 143, //criteria found but achive not found
    ACHIEVEMENT_CRITERIA_TYPE_UNK144                        = 144,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETED_LFR_DUNGEONS        = 145, // Number of times the player completed an LFR dungeon
    ACHIEVEMENT_CRITERIA_TYPE_ABANDONED_LFR_DUNGEONS        = 146, // Number of times the player abandoned an LFR dungeon
    ACHIEVEMENT_CRITERIA_TYPE_INITIATED_KICK_IN_LFR         = 147, // Number of times the player kicked someone in LFR, initiating the kick
    ACHIEVEMENT_CRITERIA_TYPE_VOTED_KICK_IN_LFR             = 148, // Number of times the player kicked someone in LFR voting, but not initiating
    ACHIEVEMENT_CRITERIA_TYPE_BEING_KICKED_IN_LFR           = 149, // Number of times the player has been kicked by someone in LFR
    ACHIEVEMENT_CRITERIA_TYPE_COUNT_OF_LFR_QUEUE_BOOSTS_BY_TANK = 150,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIOS            = 151,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIOS_SATURDAY   = 152,
    ACHIEVEMENT_CRITERIA_TYPE_REACH_SCENARIO_BOSS           = 153,
    ACHIEVEMENT_CRITERIA_TYPE_UNK154                        = 154, //not found in dbc tree
    ACHIEVEMENT_CRITERIA_TYPE_OBTAIN_BATTLEPET              = 155,
    ACHIEVEMENT_CRITERIA_TYPE_COLLECT_BATTLEPET             = 156,
    ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_PET_IN_BATTLE         = 157,
    ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_WIN                 = 158,
    ACHIEVEMENT_CRITERIA_TYPE_UNK159                        = 159,
    ACHIEVEMENT_CRITERIA_TYPE_BATTLEPET_LEVEL_UP            = 160,
    ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLE_PET_CREDIT     = 161,
    ACHIEVEMENT_CRITERIA_TYPE_LEVEL_BATTLE_PET_CREDIT       = 162, // triggers a quest credit
    ACHIEVEMENT_CRITERIA_TYPE_ENTER_AREA                    = 163, // triggers a quest credit
    ACHIEVEMENT_CRITERIA_TYPE_LEAVE_AREA                    = 164, // triggers a quest credit
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DUNGEON_ENCOUNTER    = 165,
    ACHIEVEMENT_CRITERIA_TYPE_PLACE_GARRISON_BUILDING       = 167,
    ACHIEVEMENT_CRITERIA_TYPE_UPGRADE_GARRISON_BUILDING     = 168,
    ACHIEVEMENT_CRITERIA_TYPE_CONSTRUCT_GARRISON_BUILDING   = 169,
    ACHIEVEMENT_CRITERIA_TYPE_UPGRADE_GARRISON              = 170,
    ACHIEVEMENT_CRITERIA_TYPE_START_GARRISON_MISSION        = 171,
    // 172
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_MISSION_COUNT = 173,
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_MISSION     = 174,
    ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_GARRISON_FOLLOWER_COUNT = 175,
    // 176
    // 177
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_GARRISON_BLUEPRINT_COUNT = 178,
    // 179
    // 180
    // 181
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_SHIPMENT    = 182,
    ACHIEVEMENT_CRITERIA_TYPE_RAISE_GARRISON_FOLLOWER_ITEM_LEVEL = 183,
    ACHIEVEMENT_CRITERIA_TYPE_RAISE_GARRISON_FOLLOWER_LEVEL = 184,
    ACHIEVEMENT_CRITERIA_TYPE_OWN_TOY                       = 185,
    ACHIEVEMENT_CRITERIA_TYPE_OWN_TOY_COUNT                 = 186,
    ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_GARRISON_FOLLOWER     = 187,
    ACHIEVEMENT_CRITERIA_TYPE_OWN_HEIRLOOMS                 = 189,

    ACHIEVEMENT_CRITERIA_TYPE_TOTAL
};

enum AchievementCriteriaTreeOperator
{
    ACHIEVEMENT_CRITERIA_TREE_OPERATOR_ALL  = 4,
    ACHIEVEMENT_CRITERIA_TREE_OPERATOR_ANY  = 8
};

enum ArchaeologyBranches
{
    ARCHAEOLOGY_BRANCH_NONE         = 0,
    ARCHAEOLOGY_BRANCH_DWARF        = 1,
    ARCHAEOLOGY_BRANCH_DRAENEI      = 2,
    ARCHAEOLOGY_BRANCH_FOSSIL       = 3,
    ARCHAEOLOGY_BRANCH_NIGHT_ELF    = 4,
    ARCHAEOLOGY_BRANCH_NERUBIAN     = 5,
    ARCHAEOLOGY_BRANCH_ORC          = 6,
    ARCHAEOLOGY_BRANCH_TOLVIR       = 7,
    ARCHAEOLOGY_BRANCH_TROLL        = 8,
    ARCHAEOLOGY_BRANCH_VRYKUL       = 27,
    ARCHAEOLOGY_BRANCH_MANTID       = 29,
    ARCHAEOLOGY_BRANCH_PANDAREN     = 229,
    ARCHAEOLOGY_BRANCH_MOGU         = 231,
    ARCHAEOLOGY_BRANCH_UNUSED       = 315,
};

enum AreaFlags
{
    AREA_FLAG_SNOW                  = 0x00000001,                // snow (only Dun Morogh, Naxxramas, Razorfen Downs and Winterspring)
    AREA_FLAG_UNK1                  = 0x00000002,                // Razorfen Downs, Naxxramas and Acherus: The Ebon Hold (3.3.5a)
    AREA_FLAG_UNK2                  = 0x00000004,                // Only used for areas on map 571 (development before)
    AREA_FLAG_SLAVE_CAPITAL         = 0x00000008,                // city and city subsones
    AREA_FLAG_UNK3                  = 0x00000010,                // can't find common meaning
    AREA_FLAG_SLAVE_CAPITAL2        = 0x00000020,                // slave capital city flag?
    AREA_FLAG_ALLOW_DUELS           = 0x00000040,                // allow to duel here
    AREA_FLAG_ARENA                 = 0x00000080,                // arena, both instanced and world arenas
    AREA_FLAG_CAPITAL               = 0x00000100,                // main capital city flag
    AREA_FLAG_CITY                  = 0x00000200,                // only for one zone named "City" (where it located?)
    AREA_FLAG_OUTLAND               = 0x00000400,                // expansion zones? (only Eye of the Storm not have this flag, but have 0x00004000 flag)
    AREA_FLAG_SANCTUARY             = 0x00000800,                // sanctuary area (PvP disabled)
    AREA_FLAG_NEED_FLY              = 0x00001000,                // Respawn alive at the graveyard without corpse
    AREA_FLAG_UNUSED1               = 0x00002000,                // Unused in 3.3.5a
    AREA_FLAG_OUTLAND2              = 0x00004000,                // expansion zones? (only Circle of Blood Arena not have this flag, but have 0x00000400 flag)
    AREA_FLAG_OUTDOOR_PVP           = 0x00008000,                // pvp objective area? (Death's Door also has this flag although it's no pvp object area)
    AREA_FLAG_ARENA_INSTANCE        = 0x00010000,                // used by instanced arenas only
    AREA_FLAG_UNUSED2               = 0x00020000,                // Unused in 3.3.5a
    AREA_FLAG_CONTESTED_AREA        = 0x00040000,                // On PvP servers these areas are considered contested, even though the zone it is contained in is a Horde/Alliance territory.
    AREA_FLAG_UNK6                  = 0x00080000,                // Valgarde and Acherus: The Ebon Hold
    AREA_FLAG_LOWLEVEL              = 0x00100000,                // used for some starting areas with area_level <= 15
    AREA_FLAG_TOWN                  = 0x00200000,                // small towns with Inn
    AREA_FLAG_REST_ZONE_HORDE       = 0x00400000,                // Warsong Hold, Acherus: The Ebon Hold, New Agamand Inn, Vengeance Landing Inn, Sunreaver Pavilion (Something to do with team?)
    AREA_FLAG_REST_ZONE_ALLIANCE    = 0x00800000,                // Valgarde, Acherus: The Ebon Hold, Westguard Inn, Silver Covenant Pavilion (Something to do with team?)
    AREA_FLAG_WINTERGRASP           = 0x01000000,                // Wintergrasp and it's subzones
    AREA_FLAG_INSIDE                = 0x02000000,                // used for determinating spell related inside/outside questions in Map::IsOutdoors
    AREA_FLAG_OUTSIDE               = 0x04000000,                // used for determinating spell related inside/outside questions in Map::IsOutdoors
    AREA_FLAG_WINTERGRASP_2         = 0x08000000,                // Can Hearth And Resurrect From Area
    AREA_FLAG_NO_FLY_ZONE           = 0x20000000,                // Marks zones where you cannot fly
    AREA_FLAG_UNK9                  = 0x40000000
};


enum Difficulty : uint8
{
    DIFFICULTY_NONE              = 0,                       // entry
    DIFFICULTY_NORMAL            = 1,                        // entry 
    DIFFICULTY_HEROIC            = 2,                       // difficulty_entry_2 new difficulty_entry_1
    DIFFICULTY_10_N              = 3,                        // entry
    DIFFICULTY_25_N              = 4,                       // difficulty_entry_4 new difficulty_entry_1
    DIFFICULTY_10_HC             = 5,                       // difficulty_entry_5 new difficulty_entry_2
    DIFFICULTY_25_HC             = 6,                       // difficulty_entry_6 new difficulty_entry_3
    DIFFICULTY_LFR               = 7,                       // difficulty_entry_7 new difficulty_entry_4
    DIFFICULTY_CHALLENGE         = 8,                       // difficulty_entry_8 new difficulty_entry_4
    DIFFICULTY_40                = 9,                       // difficulty_entry_9 new difficulty_entry_4
    DIFFICULTY_HC_SCENARIO       = 11,
    DIFFICULTY_N_SCENARIO        = 12,
    FLEXIBLE_DIFFICULTY          = 14,
    DIFFICULTY_NORMAL_RAID       = 14,
    DIFFICULTY_HEROIC_RAID       = 15,
    DIFFICULTY_MYTHIC_RAID       = 16,
    DIFFICULTY_LFR_NEW           = 17,
    DIFFICULTY_EVENT_RAID        = 18,
    DIFFICULTY_EVENT_DUNGEON     = 19,
    DIFFICULTY_EVENT_SCENARIO    = 20,
    DIFFICULTY_MYTHIC_DUNGEON    = 23,

    MAX_DIFFICULTY
};

enum DifficultyFlags
{
    DIFFICULTY_FLAG_HEROIC          = 0x01,
    DIFFICULTY_FLAG_DEFAULT         = 0x02,
    DIFFICULTY_FLAG_CAN_SELECT      = 0x04, // Player can select this difficulty in dropdown menu
    DIFFICULTY_FLAG_CHALLENGE_MODE  = 0x08,

    DIFFICULTY_FLAG_LEGACY          = 0x20,
    DIFFICULTY_FLAG_DISPLAY_HEROIC  = 0x40, // Controls icon displayed on minimap when inside the instance
    DIFFICULTY_FLAG_DISPLAY_MYTHIC  = 0x80  // Controls icon displayed on minimap when inside the instance
};

#define MAX_BOUND                  2

enum ScenarioFlags
{
    SCENARIO_FLAG_CHALLENGE             = 0x1,
    SCENARIO_FLAG_SUPRESS_STAGE_TEXT    = 0x2,
};

enum ScenarioStepFlags
{
    SCENARIO_STEP_FLAG_BONUS_OBJECTIVE  = 0x1,
};

enum SpawnMask
{
    SPAWNMASK_CONTINENT         = (1 << DIFFICULTY_NONE),   // any maps without spawn modes

    SPAWNMASK_DUNGEON_NORMAL    = (1 << DIFFICULTY_NORMAL),
    SPAWNMASK_DUNGEON_HEROIC    = (1 << DIFFICULTY_HEROIC),
    SPAWNMASK_DUNGEON_CHALLENGE = (1 << DIFFICULTY_CHALLENGE),
    SPAWNMASK_DUNGEON_ALL       = (SPAWNMASK_DUNGEON_NORMAL | SPAWNMASK_DUNGEON_HEROIC | SPAWNMASK_DUNGEON_CHALLENGE),
    
    SPAWNMASK_SCENARIO_NORMAL   = (1 << DIFFICULTY_N_SCENARIO),
    SPAWNMASK_SCENARIO_HEROIC   = (1 << DIFFICULTY_HC_SCENARIO),
    SPAWNMASK_SCENARIO_ALL      = (SPAWNMASK_SCENARIO_NORMAL | SPAWNMASK_SCENARIO_HEROIC),

    SPAWNMASK_RAID_10MAN_NORMAL = (1 << DIFFICULTY_10_N ),
    SPAWNMASK_RAID_25MAN_NORMAL = (1 << DIFFICULTY_25_N),
    SPAWNMASK_RAID_40MAN_NORMAL = (1 << DIFFICULTY_40),
    SPAWNMASK_RAID_NORMAL_ALL   = (SPAWNMASK_RAID_10MAN_NORMAL | SPAWNMASK_RAID_25MAN_NORMAL | SPAWNMASK_RAID_40MAN_NORMAL),

    SPAWNMASK_RAID_10MAN_HEROIC = (1 << DIFFICULTY_10_HC),
    SPAWNMASK_RAID_25MAN_HEROIC = (1 << DIFFICULTY_25_HC),
    SPAWNMASK_RAID_HEROIC_ALL   = (SPAWNMASK_RAID_10MAN_HEROIC | SPAWNMASK_RAID_25MAN_HEROIC),

    SPAWNMASK_RAID_RAID_TOOL    = (1 << DIFFICULTY_LFR),

    SPAWNMASK_RAID_ALL          = (SPAWNMASK_RAID_NORMAL_ALL | SPAWNMASK_RAID_HEROIC_ALL | SPAWNMASK_RAID_RAID_TOOL)
};

enum FactionTemplateFlags
{
    FACTION_TEMPLATE_FLAG_PVP               = 0x00000800,   // flagged for PvP
    FACTION_TEMPLATE_FLAG_CONTESTED_GUARD   = 0x00001000,   // faction will attack players that were involved in PvP combats
    FACTION_TEMPLATE_FLAG_HOSTILE_BY_DEFAULT= 0x00002000
};

enum FactionMasks
{
    FACTION_MASK_PLAYER   = 1,                              // any player
    FACTION_MASK_ALLIANCE = 2,                              // player or creature from alliance team
    FACTION_MASK_HORDE    = 4,                              // player or creature from horde team
    FACTION_MASK_MONSTER  = 8                               // aggressive creature from monster team
    // if none flags set then non-aggressive creature
};

enum MapTypes                                               // Lua_IsInInstance
{
    MAP_COMMON          = 0,                                // none
    MAP_INSTANCE        = 1,                                // party
    MAP_RAID            = 2,                                // raid
    MAP_BATTLEGROUND    = 3,                                // pvp
    MAP_ARENA           = 4,                                // arena
    MAP_SCENARIO        = 5                                 // scenario
};

enum MapFlags
{
    MAP_FLAG_CAN_CHANGE_DIFFICULTY              = 0x0000100,

    MAP_FLAG_POST_BC_RAID_INSTANCE              = 0x0008000,

    MAP_FLAG_GARRISON                           = 0x4000000
};

enum SkillRaceClassInfoFlags
{
    SKILL_FLAG_NO_SKILLUP_MESSAGE               = 0x2,
    SKILL_FLAG_ALWAYS_MAX_VALUE                 = 0x10,
    SKILL_FLAG_UNLEARNABLE                      = 0x20,     // Skill can be unlearned
    SKILL_FLAG_INCLUDE_IN_SORT                  = 0x80,     // Spells belonging to a skill with this flag will additionally compare skill ids when sorting spellbook in client
    SKILL_FLAG_NOT_TRAINABLE                    = 0x100,
    SKILL_FLAG_MONO_VALUE                       = 0x400     // Skill always has value 1 - clientside display flag, real value can be different
};

enum AbilytyLearnType
{
    SKILL_LINE_ABILITY_LEARNED_ON_SKILL_VALUE   = 1, // Spell state will update depending on skill value
    SKILL_LINE_ABILITY_LEARNED_ON_SKILL_LEARN   = 2, // Spell will be learned/removed together with entire skill


    ABILITY_LEARNED_ON_GET_PROFESSION_SKILL     = 1,
    ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL  = 2,
    ABILITY_LEARNED_ON_GET_UNK                  = 3,
    ABILITY_LEARNED_ON_GET_UNK2                 = 4,    // DK: Death Gate
};

enum ItemEnchantmentType
{
    ITEM_ENCHANTMENT_TYPE_NONE             = 0,
    ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL     = 1,
    ITEM_ENCHANTMENT_TYPE_DAMAGE           = 2,
    ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL      = 3,
    ITEM_ENCHANTMENT_TYPE_RESISTANCE       = 4,
    ITEM_ENCHANTMENT_TYPE_STAT             = 5,
    ITEM_ENCHANTMENT_TYPE_TOTEM            = 6,
    ITEM_ENCHANTMENT_TYPE_USE_SPELL        = 7,
    ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET = 8
};

enum ItemBonusType
{
    ITEM_BONUS_ITEM_LEVEL                = 1,
    ITEM_BONUS_STAT                      = 2,
    ITEM_BONUS_QUALITY                   = 3,
    ITEM_BONUS_DESCRIPTION               = 4,
    ITEM_BONUS_SUFFIX                    = 5,
    ITEM_BONUS_SOCKET                    = 6,
    ITEM_BONUS_APPEARANCE                = 7,
    ITEM_BONUS_REQUIRED_LEVEL            = 8,
    ITEM_BONUS_DISPLAY_TOAST_METHOD      = 9,
    ITEM_BONUS_REPAIR_COST_MULTIPLIER    = 10,
    ITEM_BONUS_SCALING_STAT_DISTRIBUTION = 11,
    ITEM_BONUS_UNK_12                    = 12
};

enum ItemLimitCategoryMode
{
    ITEM_LIMIT_CATEGORY_MODE_HAVE       = 0,                      // limit applied to amount items in inventory/bank
    ITEM_LIMIT_CATEGORY_MODE_EQUIP      = 1                       // limit applied to amount equipped items (including used gems)
};

enum ItemSpecStat
{
    ITEM_SPEC_STAT_INTELLECT        = 0,
    ITEM_SPEC_STAT_AGILITY          = 1,
    ITEM_SPEC_STAT_STRENGTH         = 2,
    ITEM_SPEC_STAT_SPIRIT           = 3,
    ITEM_SPEC_STAT_HIT              = 4,
    ITEM_SPEC_STAT_DODGE            = 5,
    ITEM_SPEC_STAT_PARRY            = 6,
    ITEM_SPEC_STAT_ONE_HANDED_AXE   = 7,
    ITEM_SPEC_STAT_TWO_HANDED_AXE   = 8,
    ITEM_SPEC_STAT_ONE_HANDED_SWORD = 9,
    ITEM_SPEC_STAT_TWO_HANDED_SWORD = 10,
    ITEM_SPEC_STAT_ONE_HANDED_MACE  = 11,
    ITEM_SPEC_STAT_TWO_HANDED_MACE  = 12,
    ITEM_SPEC_STAT_DAGGER           = 13,
    ITEM_SPEC_STAT_FIST_WEAPON      = 14,
    ITEM_SPEC_STAT_GUN              = 15,
    ITEM_SPEC_STAT_BOW              = 16,
    ITEM_SPEC_STAT_CROSSBOW         = 17,
    ITEM_SPEC_STAT_STAFF            = 18,
    ITEM_SPEC_STAT_POLEARM          = 19,
    ITEM_SPEC_STAT_THROWN           = 20,
    ITEM_SPEC_STAT_WAND             = 21,
    ITEM_SPEC_STAT_SHIELD           = 22,
    ITEM_SPEC_STAT_RELIC            = 23,
    ITEM_SPEC_STAT_CRIT             = 24,
    ITEM_SPEC_STAT_HASTE            = 25,
    ITEM_SPEC_STAT_BONUS_ARMOR      = 26,
    ITEM_SPEC_STAT_CLOAK            = 27,

    ITEM_SPEC_STAT_NONE             = 28
};

enum LfgFlags
{
    LFG_FLAG_SEASONAL                            = 0x0004,
    LFG_FLAG_USER_TELEPORT_NOT_ALLOWED           = 0x0800,
    LFG_FLAG_NON_BACKFILLABLE                    = 0x1000,
};

enum LfgSubType
{
    LFG_SUBTYPE_NONE                             = 0,
    LFG_SUBTYPE_DUNGEON                          = 1,
    LFG_SUBTYPE_RAID                             = 2,
    LFG_SUBTYPE_SCENARIO                         = 3,
    LFG_SUBTYPE_FLEX                             = 4,
};

enum LfgType
{
    LFG_TYPE_NONE                                = 0,       // Internal use only
    LFG_TYPE_DUNGEON                             = 1,
    LFG_TYPE_RAID                                = 2,
    LFG_TYPE_QUEST                               = 3,       // not exist in dbc
    LFG_TYPE_ZONE                                = 4,
    LFG_TYPE_HEROIC                              = 5,       // not exist in dbc
    LFG_TYPE_RANDOM                              = 6,
    LFG_TYPE_SCENARIO                            = 7,       // not exist in dbc
};

enum MountCapabilityFlags
{
    MOUNT_CAPABILITY_FLAG_CAN_PITCH     = 0x4,  // client checks MOVEMENTFLAG2_FULL_SPEED_PITCHING
    MOUNT_CAPABILITY_FLAG_CAN_SWIM      = 0x8,  // client checks MOVEMENTFLAG_SWIMMING
};

enum MountFlags
{
    MOUNT_FLAG_SELF_MOUNT               = 0x02, // Player becomes the mount himself
    MOUNT_FLAG_FACTION_SPECIFIC         = 0x04,
    MOUNT_FLAG_PREFERRED_SWIMMING       = 0x10,
    MOUNT_FLAG_PREFERRED_WATER_WALKING  = 0x20,
    MOUNT_FLAG_HIDE_IF_UNKNOWN          = 0x40
};

enum TotemCategoryType
{
    TOTEM_CATEGORY_TYPE_KNIFE           = 1,
    TOTEM_CATEGORY_TYPE_TOTEM           = 2,
    TOTEM_CATEGORY_TYPE_ROD             = 3,
    TOTEM_CATEGORY_TYPE_PICK            = 21,
    TOTEM_CATEGORY_TYPE_STONE           = 22,
    TOTEM_CATEGORY_TYPE_HAMMER          = 23,
    TOTEM_CATEGORY_TYPE_SPANNER         = 24
};

// SummonProperties.dbc, col 1
enum SummonPropGroup
{
    SUMMON_PROP_GROUP_UNKNOWN1       = 0,                   // 1160 spells in 3.0.3
    SUMMON_PROP_GROUP_UNKNOWN2       = 1,                   // 861 spells in 3.0.3
    SUMMON_PROP_GROUP_PETS           = 2,                   // 52 spells in 3.0.3, pets mostly
    SUMMON_PROP_GROUP_CONTROLLABLE   = 3,                   // 13 spells in 3.0.3, mostly controllable
    SUMMON_PROP_GROUP_UNKNOWN3       = 4                    // 86 spells in 3.0.3, taxi/mounts
};

// SummonProperties.dbc, col 3
enum SummonPropType
{
    SUMMON_PROP_TYPE_UNKNOWN         = 0,                   // different summons, 1330 spells in 3.0.3
    SUMMON_PROP_TYPE_SUMMON          = 1,                   // generic summons, 49 spells in 3.0.3
    SUMMON_PROP_TYPE_GUARDIAN        = 2,                   // summon guardian, 393 spells in 3.0.3
    SUMMON_PROP_TYPE_ARMY            = 3,                   // summon army, 5 spells in 3.0.3
    SUMMON_PROP_TYPE_TOTEM           = 4,                   // summon totem, 169 spells in 3.0.3
    SUMMON_PROP_TYPE_CRITTER         = 5,                   // critter/minipet, 195 spells in 3.0.3
    SUMMON_PROP_TYPE_DK              = 6,                   // summon DRW/Ghoul, 2 spells in 3.0.3
    SUMMON_PROP_TYPE_BOMB            = 7,                   // summon bot/bomb, 4 spells in 3.0.3
    SUMMON_PROP_TYPE_PHASING         = 8,                   // something todo with DK prequest line, 2 spells in 3.0.3
    SUMMON_PROP_TYPE_SIEGE_VEH       = 9,                   // summon different vehicles, 14 spells in 3.0.3
    SUMMON_PROP_TYPE_DRAKE_VEH       = 10,                  // summon drake (vehicle), 3 spells
    SUMMON_PROP_TYPE_LIGHTWELL       = 11,                  // summon lightwell, 6 spells in 3.0.3
    SUMMON_PROP_TYPE_JEEVES          = 12,                  // summon Jeeves, 1 spell in 3.3.5a
    SUMMON_PROP_TYPE_LASHTAIL        = 13,                  // Lashtail Hatchling, 1 spell in 4.2.2
    SUMMON_PROP_TYPE_GATE            = 14,                  // summon gate, 2 spells in 5.4.1
    SUMMON_PROP_TYPE_BANNER          = 18                   // summon Banner, 3 spell in 5.4.1
};

// SummonProperties.dbc, col 5
enum SummonPropFlags
{
    SUMMON_PROP_FLAG_NONE            = 0x00000000,          // 1342 spells in 3.0.3
    SUMMON_PROP_FLAG_UNK1            = 0x00000001,          // 75 spells in 3.0.3, something unfriendly
    SUMMON_PROP_FLAG_UNK2            = 0x00000002,          // 616 spells in 3.0.3, something friendly
    SUMMON_PROP_FLAG_UNK3            = 0x00000004,          // 22 spells in 3.0.3, no idea...
    SUMMON_PROP_FLAG_UNK4            = 0x00000008,          // 49 spells in 3.0.3, some mounts
    SUMMON_PROP_FLAG_UNK5            = 0x00000010,          // 25 spells in 3.0.3, quest related?
    SUMMON_PROP_FLAG_UNK6            = 0x00000020,          // 0 spells in 3.3.5, unused
    SUMMON_PROP_FLAG_UNK7            = 0x00000040,          // 12 spells in 3.0.3, no idea
    SUMMON_PROP_FLAG_UNK8            = 0x00000080,          // 4 spells in 3.0.3, no idea
    SUMMON_PROP_FLAG_UNK9            = 0x00000100,          // 51 spells in 3.0.3, no idea, many quest related
    SUMMON_PROP_FLAG_UNK10           = 0x00000200,          // 51 spells in 3.0.3, something defensive
    SUMMON_PROP_FLAG_UNK11           = 0x00000400,          // 3 spells, requires something near?
    SUMMON_PROP_FLAG_UNK12           = 0x00000800,          // 30 spells in 3.0.3, no idea
    SUMMON_PROP_FLAG_UNK13           = 0x00001000,          // Lightwell, Jeeves, Gnomish Alarm-o-bot, Build vehicles(wintergrasp)
    SUMMON_PROP_FLAG_UNK14           = 0x00002000,          // Guides, player follows
    SUMMON_PROP_FLAG_UNK15           = 0x00004000,          // Force of Nature, Shadowfiend, Feral Spirit, Summon Water Elemental
    SUMMON_PROP_FLAG_UNK16           = 0x00008000,          // Light/Dark Bullet, Soul/Fiery Consumption, Twisted Visage, Twilight Whelp. Phase related?
    SUMMON_PROP_FLAG_UNK17           = 0x00010000,
    SUMMON_PROP_FLAG_UNK18           = 0x00020000,
    SUMMON_PROP_FLAG_UNK19           = 0x00040000,
    SUMMON_PROP_FLAG_UNK20           = 0x00080000,
    SUMMON_PROP_FLAG_UNK21           = 0x00100000           // Totems
};

enum TaxiNodeFlags
{
    TAXI_NODE_FLAG_ALLIANCE         = 0x1,
    TAXI_NODE_FLAG_HORDE            = 0x2
};

enum TaxiPathNodeFlags
{
    TAXI_PATH_NODE_FLAG_TELEPORT    = 0x1,
    TAXI_PATH_NODE_FLAG_STOP        = 0x2
};

enum VehicleSeatFlags
{
    VEHICLE_SEAT_FLAG_HAS_LOWER_ANIM_FOR_ENTER                          = 0x00000001,
    VEHICLE_SEAT_FLAG_HAS_LOWER_ANIM_FOR_RIDE                           = 0x00000002,
    VEHICLE_SEAT_FLAG_UNK1                                              = 0x00000004,
    VEHICLE_SEAT_FLAG_SHOULD_USE_VEH_SEAT_EXIT_ANIM_ON_VOLUNTARY_EXIT   = 0x00000008,
    VEHICLE_SEAT_FLAG_UNK3                                              = 0x00000010,
    VEHICLE_SEAT_FLAG_HIDE_PASSENGER                                    = 0x00000200,           // Passenger is hidden
    VEHICLE_SEAT_FLAG_ALLOW_TURNING                                     = 0x00000400,           // needed for CGCamera__SyncFreeLookFacing
    VEHICLE_SEAT_FLAG_CAN_CONTROL                                       = 0x00000800,           // Lua_UnitInVehicleControlSeat
    VEHICLE_SEAT_FLAG_CAN_CAST_MOUNT_SPELL                              = 0x00001000,           // Can cast spells with SPELL_AURA_MOUNTED from seat (possibly 4.x only, 0 seats on 3.3.5a)
    VEHICLE_SEAT_FLAG_UNCONTROLLED                                      = 0x00002000,           // can override !& VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT
    VEHICLE_SEAT_FLAG_CAN_ATTACK                                        = 0x00004000,           // Can attack, cast spells and use items from vehicle
    VEHICLE_SEAT_FLAG_SHOULD_USE_VEH_SEAT_EXIT_ANIMN_ON_FORCED_EXIT     = 0x00008000,
    VEHICLE_SEAT_FLAG_HAS_VEH_EXIT_ANIM_VOLUNTARY_EXIT                  = 0x00040000,
    VEHICLE_SEAT_FLAG_HAS_VEH_EXIT_ANIM_FORCED_EXIT                     = 0x00080000,
    VEHICLE_SEAT_FLAG_REC_HAS_VEHICLE_ENTER_ANIM                        = 0x00400000,
    VEHICLE_SEAT_FLAG_ENABLE_VEHICLE_ZOOM                               = 0x01000000,
    VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT                                 = 0x02000000,           // Lua_CanExitVehicle - can enter and exit at free will
    VEHICLE_SEAT_FLAG_CAN_SWITCH                                        = 0x04000000,           // Lua_CanSwitchVehicleSeats
    VEHICLE_SEAT_FLAG_HAS_START_WARITING_FOR_VEH_TRANSITION_ANIM_ENTER  = 0x08000000,
    VEHICLE_SEAT_FLAG_HAS_START_WARITING_FOR_VEH_TRANSITION_ANIM_EXIT   = 0x10000000,
    VEHICLE_SEAT_FLAG_CAN_CAST                                          = 0x20000000,           // Lua_UnitHasVehicleUI
    VEHICLE_SEAT_FLAG_UNK2                                              = 0x40000000,           // checked in conjunction with 0x800 in CastSpell2
    VEHICLE_SEAT_FLAG_DISALLOWS_INTERACTION                             = 0x80000000
};

enum VehicleSeatFlagsB
{
    VEHICLE_SEAT_FLAG_B_NONE                     = 0x00000000,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED            = 0x00000002,
    VEHICLE_SEAT_FLAG_B_TARGETS_IN_RAIDUI        = 0x00000008,           // Lua_UnitTargetsVehicleInRaidUI
    VEHICLE_SEAT_FLAG_B_EJECTABLE                = 0x00000020,           // ejectable
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2          = 0x00000040,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3          = 0x00000100,
    VEHICLE_SEAT_FLAG_B_KEEP_PET                 = 0x00020000,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4          = 0x02000000,
    VEHICLE_SEAT_FLAG_B_CAN_SWITCH               = 0x04000000,
    VEHICLE_SEAT_FLAG_B_VEHICLE_PLAYERFRAME_UI   = 0x80000000            // Lua_UnitHasVehiclePlayerFrameUI - actually checked for flagsb &~ 0x80000000
};

// CurrencyTypes.dbc
enum CurrencyTypes
{
    CURRENCY_NONE                                       = 0,
    CURRENCY_TYPE_BADGE_OF_JUSTICE                      = 42,
    CURRENCY_TYPE_DALARAN_JEWELCRAFTER_S_TOKEN          = 61,
    CURRENCY_TYPE_EPICUREAN_S_AWARD                     = 81,
    CURRENCY_TYPE_EMBLEM_OF_HEROISM                     = 101,
    CURRENCY_TYPE_EMBLEM_OF_VALOR                       = 102,
    CURRENCY_TYPE_ARENA_POINTS                          = 103,
    CURRENCY_TYPE_ALTERAC_VALLEY_MARK_OF_HONOR          = 121,
    CURRENCY_TYPE_ARATHI_BASIN_MARK_OF_HONOR            = 122,
    CURRENCY_TYPE_EYE_OF_THE_STORM_MARK_OF_HONOR        = 123,
    CURRENCY_TYPE_STRAND_OF_THE_ANCIENTS_MARK_OF_HONOR  = 124,
    CURRENCY_TYPE_WARSONG_GULCH_MARK_OF_HONOR           = 125,
    CURRENCY_TYPE_WINTERGRASP_MARK_OF_HONOR             = 126,
    CURRENCY_TYPE_STONE_KEEPER_S_SHARD                  = 161,
    CURRENCY_TYPE_HONOR_POINTS_DEPRECATED2              = 181,
    CURRENCY_TYPE_VENTURE_COIN                          = 201,
    CURRENCY_TYPE_EMBLEM_OF_CONQUEST                    = 221,
    CURRENCY_TYPE_CHAMPIONS_SEAL                        = 241,
    CURRENCY_TYPE_EMBLEM_OF_TRIUMPH                     = 301,
    CURRENCY_TYPE_ISLE_OF_CONQUEST_MARK_OF_HONOR        = 321,
    CURRENCY_TYPE_EMBLEM_OF_FROST                       = 341,
    CURRENCY_TYPE_ILLUSTRIOUS_JEWELCRAFTERS_TOKEN       = 361,
    CURRENCY_TYPE_DWARF_ARCHAEOLOGY_FRAGMENT            = 384,
    CURRENCY_TYPE_TROLL_ARCHAEOLOGY_FRAGMENT            = 385,
    CURRENCY_TYPE_CONQUEST_POINTS                       = 390,
    CURRENCY_TYPE_TOL_BARAD_COMMENDATION                = 391,
    CURRENCY_TYPE_HONOR_POINTS                          = 392,
    CURRENCY_TYPE_FOSSIL_ARCHAEOLOGY_FRAGMENT           = 393,
    CURRENCY_TYPE_NIGHT_ELF_ARCHAEOLOGY_FRAGMENT        = 394,
    CURRENCY_TYPE_JUSTICE_POINTS                        = 395,
    CURRENCY_TYPE_VALOR_POINTS                          = 396, // deprecated
    CURRENCY_TYPE_ORC_ARCHAEOLOGY_FRAGMENT              = 397,
    CURRENCY_TYPE_DRAENEI_ARCHAEOLOGY_FRAGMENT          = 398,
    CURRENCY_TYPE_VRYKUL_ARCHAEOLOGY_FRAGMENT           = 399,
    CURRENCY_TYPE_NERUBIAN_ARCHAEOLOGY_FRAGMENT         = 400,
    CURRENCY_TYPE_TOL_VIR_ARCHAEOLOGY_FRAGMENT          = 401,
    CURRENCY_TYPE_IRONPAW_TOKEN                         = 402,
    CURRENCY_TYPE_MARK_OF_THE_WORLD_TREE                = 416,
    CURRENCY_TYPE_CONQUEST_ARENA_META                   = 483,
    CURRENCY_TYPE_CONQUEST_RATED_BG_META                = 484,
    CURRENCY_TYPE_DARKMOON_PRIZE_TICKET                 = 515,
    CURRENCY_TYPE_MOTE_OF_DARKNESS                      = 614,
    CURRENCY_TYPE_ESSENCE_OF_CORRUPTED_DEATHWING        = 615,
    CURRENCY_TYPE_PANDAREN_ARCHAEOLOGY_FRAGMENT         = 676,
    CURRENCY_TYPE_MOGU_ARCHAEOLOGY_FRAGMENT             = 677,
    CURRENCY_TYPE_CONQUEST_RANDOM_BG_META               = 692,
    CURRENCY_TYPE_ELDER_CHARM_OF_GOOD_FORTUNE           = 697,
    CURRENCY_TYPE_ZEN_JEWELCRAFTER_S_TOKEN              = 698,
    CURRENCY_TYPE_LESSER_CHARM_OF_GOOD_FORTUNE          = 738,
    CURRENCY_TYPE_MOGU_RUNE_OF_FATE                     = 752,
    CURRENCY_TYPE_MANTID_ARCHAEOLOGY_FRAGMENT           = 754,
    CURRENCY_TYPE_WARFORGED_SEAL                        = 776,
    CURRENCY_TYPE_TIMELESS_COIN                         = 777,
    CURRENCY_TYPE_BLOODY_COIN                           = 789,
    CURRENCY_TYPE_BLACK_IRON_FRAGMENT                   = 810,
    CURRENCY_TYPE_DRAENOR_CLANS_ARCHAEOLOGY_FRAGMENT    = 821,
    CURRENCY_TYPE_APEXIS_CRYSTAL                        = 823,
    CURRENCY_TYPE_GARRISON_RESOURCES                    = 824,
    CURRENCY_TYPE_OGRE_ARCHAEOLOGY_FRAGMENT             = 828,
    CURRENCY_TYPE_ARAKKOA_ARCHAEOLOGY_FRAGMENT          = 829,
    CURRENCY_TYPE_SECRET_OF_DRAENOR_ALCHEMY             = 910,
    CURRENCY_TYPE_ARTIFACT_FRAGMENT                     = 944,
    CURRENCY_TYPE_DINGY_IRON_COINS                      = 980,
    CURRENCY_TYPE_SEAL_OF_TEMPERED_FATE                 = 994,
    CURRENCY_TYPE_SECRET_OF_DRAENOR_TAILORING           = 999,
    CURRENCY_TYPE_SECRET_OF_DRAENOR_JEWELCRAFTING       = 1008,
    CURRENCY_TYPE_SECRET_OF_DRAENOR_LEATHERWORKING      = 1017,
    CURRENCY_TYPE_SECRET_OF_DRAENOR_BLACKSMITHING       = 1020,
};

#define CURRENCY_PRECISION 100.0f

enum CurrencyCategory
{
    CURRENCY_CATEGORY_ARCHAEOLOGY   = 82,
    CURRENCY_CATEGORY_META_CONQUEST = 89,
};

enum CurrencyFlags
{
    CURRENCY_FLAG_TRADEABLE         = 0x0001,
    CURRENCY_FLAG_HAS_PRECISION     = 0x0008,
    CURRENCY_FLAG_HAS_SEASON_COUNT  = 0x0080,
    CURRENCY_FLAG_UNK               = 0x2000,
};

enum CharSectionFlags
{
    SECTION_FLAG_PLAYER         = 0x01,
    SECTION_FLAG_DEATH_KNIGHT   = 0x04
};

enum CharSectionType
{
    SECTION_TYPE_SKIN           = 0,
    SECTION_TYPE_FACE           = 1,
    SECTION_TYPE_FACIAL_HAIR    = 2,
    SECTION_TYPE_HAIR           = 3,
    SECTION_TYPE_UNDERWEAR      = 4
};

static uint8 const MAX_TALENT_TIERS = 8;
static uint8 const MAX_TALENT_COLUMNS = 3;

enum class DB2Hashes : uint32
{
    Achievement_Category = 588988749, //! MetaFlags: 0
    AdventureJournal = 2398034583, //! MetaFlags: 4
    AdventureMapPOI = 1651896055, //! MetaFlags: 0
    AnimationData = 2628737569, //! MetaFlags: 4
    AnimKit = 3014618826, //! MetaFlags: 4
    AnimKitBoneSet = 2827780344, //! MetaFlags: 4
    AnimKitBoneSetAlias = 931155434, //! MetaFlags: 4
    AnimKitConfig = 337491991, //! MetaFlags: 4
    AnimKitConfigBoneSet = 1570721457, //! MetaFlags: 4
    AnimKitPriority = 1375553837, //! MetaFlags: 4
    AnimKitSegment = 1050181896, //! MetaFlags: 4
    AnimReplacement = 1399499241, //! MetaFlags: 4
    AnimReplacementSet = 3916479355, //! MetaFlags: 4
    AreaAssignment = 3771663260, //! MetaFlags: 4
    AreaGroupMember = 157446066, //! MetaFlags: 4
    AreaPOI = 3745449290, //! MetaFlags: 4
    AreaPOIState = 1240462528, //! MetaFlags: 4
    AreaTable = 1918102339, //! MetaFlags: 4
    AreaTrigger = 441483745, //! MetaFlags: 0
    AreaTriggerActionSet = 1548466975, //! MetaFlags: 4
    AreaTriggerBox = 1225579890, //! MetaFlags: 4
    AreaTriggerCylinder = 2895307949, //! MetaFlags: 4
    AreaTriggerSphere = 2466724645, //! MetaFlags: 4
    ArmorLocation = 2979075396, //! MetaFlags: 4
    Artifact = 23454815, //! MetaFlags: 4
    ArtifactAppearance = 2900396775, //! MetaFlags: 0
    ArtifactAppearanceSet = 3876723617, //! MetaFlags: 0
    ArtifactCategory = 3312628869, //! MetaFlags: 4
    ArtifactPower = 4088109130, //! MetaFlags: 0
    ArtifactPowerLink = 913866359, //! MetaFlags: 4
    ArtifactPowerRank = 367248955, //! MetaFlags: 0
    ArtifactQuestXP = 3392267897, //! MetaFlags: 4
    ArtifactUnlock = 1473366888, //! MetaFlags: 4
    AuctionHouse = 3103351894, //! MetaFlags: 4
    BankBagSlotPrices = 3740031584, //! MetaFlags: 4
    BannedAddOns = 998089029, //! MetaFlags: 4
    BarberShopStyle = 2399777931, //! MetaFlags: 0
    BattlemasterList = 1435332686, //! MetaFlags: 0
    BattlePetAbility = 3416538071, //! MetaFlags: 0
    BattlePetAbilityEffect = 3716901134, //! MetaFlags: 0
    BattlePetAbilityState = 1012231747, //! MetaFlags: 0
    BattlePetAbilityTurn = 3973639388, //! MetaFlags: 0
    BattlePetBreedQuality = 458903206, //! MetaFlags: 4
    BattlePetBreedState = 1794847238, //! MetaFlags: 0
    BattlePetEffectProperties = 1672791226, //! MetaFlags: 4
    BattlePetNPCTeamMember = 4060454394, //! MetaFlags: 4
    BattlePetSpecies = 1821637041, //! MetaFlags: 0
    BattlePetSpeciesState = 366509520, //! MetaFlags: 0
    BattlePetSpeciesXAbility = 1143173908, //! MetaFlags: 0
    BattlePetState = 2403627824, //! MetaFlags: 4
    BattlePetVisual = 3282955075, //! MetaFlags: 4
    Bounty = 1531407228, //! MetaFlags: 4
    BountySet = 3419578634, //! MetaFlags: 0
    BroadcastText = 35137211, //! MetaFlags: 4
    CameraEffect = 1335965597, //! MetaFlags: 4
    CameraEffectEntry = 2638339590, //! MetaFlags: 4
    CameraMode = 4253494973, //! MetaFlags: 4
    CameraShakes = 1220051873, //! MetaFlags: 4
    CastableRaidBuffs = 4147515750, //! MetaFlags: 4
    Cfg_Categories = 3354229117, //! MetaFlags: 0
    Cfg_Configs = 153052569, //! MetaFlags: 4
    Cfg_Regions = 2810085133, //! MetaFlags: 4
    CharacterFaceBoneSet = 1298280559, //! MetaFlags: 4
    CharacterFacialHairStyles = 555942850, //! MetaFlags: 4
    CharacterLoadout = 3758770171, //! MetaFlags: 4
    CharacterLoadoutItem = 1707318183, //! MetaFlags: 4
    CharBaseInfo = 812099832, //! MetaFlags: 4
    CharBaseSection = 1571082092, //! MetaFlags: 4
    CharComponentTextureLayouts = 2809937531, //! MetaFlags: 4
    CharComponentTextureSections = 4205415669, //! MetaFlags: 4
    CharHairGeosets = 299101852, //! MetaFlags: 4
    CharSections = 909133907, //! MetaFlags: 0
    CharShipment = 2956113239, //! MetaFlags: 0
    CharShipmentContainer = 2810793959, //! MetaFlags: 4
    CharStartOutfit = 2067920257, //! MetaFlags: 4
    CharTitles = 2246024846, //! MetaFlags: 4
    ChatChannels = 3439298541, //! MetaFlags: 4
    ChatProfanity = 603412935, //! MetaFlags: 0
    ChrClasses = 4119371148, //! MetaFlags: 0
    ChrClassesXPowerTypes = 3224459983, //! MetaFlags: 4
    ChrClassRaceSex = 836435647, //! MetaFlags: 4
    ChrClassTitle = 3235645495, //! MetaFlags: 4
    ChrClassUIDisplay = 2967626539, //! MetaFlags: 4
    ChrClassVillain = 898992118, //! MetaFlags: 4
    ChrRaces = 1408333884, //! MetaFlags: 0
    ChrSpecialization = 2685374048, //! MetaFlags: 0
    ChrUpgradeBucket = 3720095159, //! MetaFlags: 0
    ChrUpgradeBucketSpell = 3326241424, //! MetaFlags: 4
    ChrUpgradeTier = 2583026706, //! MetaFlags: 0
    CinematicCamera = 3268345337, //! MetaFlags: 4
    CinematicSequences = 1441530096, //! MetaFlags: 4
    CombatCondition = 368401261, //! MetaFlags: 4
    ComponentModelFileData = 3194678566, //! MetaFlags: 4
    ComponentTextureFileData = 2185268109, //! MetaFlags: 4
    ConversationLine = 3348406326, //! MetaFlags: 5
    Creature = 3386291891, //! MetaFlags: 0
    CreatureDifficulty = 3386943305, //! MetaFlags: 0
    CreatureDisplayInfo = 3218799089, //! MetaFlags: 4
    CreatureDisplayInfoCond = 293862385, //! MetaFlags: 0
    CreatureDisplayInfoExtra = 1722140087, //! MetaFlags: 4
    CreatureDisplayInfoTrn = 4245272579, //! MetaFlags: 4
    CreatureDispXUiCamera = 995845379, //! MetaFlags: 4
    CreatureFamily = 916407967, //! MetaFlags: 4
    CreatureImmunities = 555625967, //! MetaFlags: 4
    CreatureModelData = 2137612197, //! MetaFlags: 4
    CreatureMovementInfo = 3474286469, //! MetaFlags: 4
    CreatureSoundData = 2445379776, //! MetaFlags: 4
    CreatureType = 528526655, //! MetaFlags: 4
    Criteria = 4012104832, //! MetaFlags: 4
    CriteriaTree = 1255424668, //! MetaFlags: 4
    CriteriaTreeXEffect = 3898252553, //! MetaFlags: 4
    CurrencyCategory = 3552864641, //! MetaFlags: 4
    CurrencyTypes = 793851790, //! MetaFlags: 0
    Curve = 1272569722, //! MetaFlags: 4
    CurvePoint = 1880017466, //! MetaFlags: 4
    DeathThudLookups = 1110901647, //! MetaFlags: 4
    DecalProperties = 3350717493, //! MetaFlags: 0
    DeclinedWord = 540203103, //! MetaFlags: 0
    DeclinedWordCases = 2756270666, //! MetaFlags: 4
    DestructibleModelData = 2054439517, //! MetaFlags: 4
    DeviceBlacklist = 983446676, //! MetaFlags: 4
    DeviceDefaultSettings = 3490179113, //! MetaFlags: 4
    Difficulty = 3408494138, //! MetaFlags: 0
    DissolveEffect = 2413480372, //! MetaFlags: 4
    DriverBlacklist = 1326512502, //! MetaFlags: 4
    DungeonEncounter = 4218319612, //! MetaFlags: 4
    DungeonMap = 1317353695, //! MetaFlags: 0
    DungeonMapChunk = 2412271734, //! MetaFlags: 0
    DurabilityCosts = 3100520987, //! MetaFlags: 4
    DurabilityQuality = 4152396805, //! MetaFlags: 4
    Emotes = 554245018, //! MetaFlags: 4
    EmotesText = 3981346648, //! MetaFlags: 4
    EmotesTextData = 2003163239, //! MetaFlags: 4
    EmotesTextSound = 490390919, //! MetaFlags: 4
    EnvironmentalDamage = 1851564952, //! MetaFlags: 4
    Exhaustion = 864466243, //! MetaFlags: 0
    Faction = 116794583, //! MetaFlags: 4
    FactionGroup = 2052437341, //! MetaFlags: 4
    FactionTemplate = 270052821, //! MetaFlags: 4
    FileDataComplete = 766838915, //! MetaFlags: 4
    FootprintTextures = 733830049, //! MetaFlags: 0
    FootstepTerrainLookup = 1387041889, //! MetaFlags: 4
    FriendshipRepReaction = 322001913, //! MetaFlags: 4
    FriendshipReputation = 234233465, //! MetaFlags: 0
    FullScreenEffect = 2235570485, //! MetaFlags: 4
    GameObjectArtKit = 2407661, //! MetaFlags: 4
    GameObjectDiffAnimMap = 390211750, //! MetaFlags: 4
    GameObjectDisplayInfo = 1829768651, //! MetaFlags: 4
    GameObjectDisplayInfoXSoundKit = 3676568537, //! MetaFlags: 4
    GameObjects = 331613093, //! MetaFlags: 0
    GameTables = 5504391, //! MetaFlags: 4
    GameTips = 2306351757, //! MetaFlags: 4
    GarrAbility = 3832747704, //! MetaFlags: 0
    GarrAbilityCategory = 852966632, //! MetaFlags: 4
    GarrAbilityEffect = 2753361231, //! MetaFlags: 0
    GarrBuilding = 878382519, //! MetaFlags: 4
    GarrBuildingDoodadSet = 2663146609, //! MetaFlags: 4
    GarrBuildingPlotInst = 2494151198, //! MetaFlags: 0
    GarrClassSpec = 1165344527, //! MetaFlags: 0
    GarrEncounter = 2807278738, //! MetaFlags: 0
    GarrEncounterSetXEncounter = 2666936164, //! MetaFlags: 4
    GarrEncounterXMechanic = 1187330571, //! MetaFlags: 4
    GarrFollItemSetMember = 708957985, //! MetaFlags: 4
    GarrFollower = 2832751941, //! MetaFlags: 0
    GarrFollowerLevelXP = 3727959107, //! MetaFlags: 4
    GarrFollowerQuality = 1107630156, //! MetaFlags: 4
    GarrFollowerSetXFollower = 3253909153, //! MetaFlags: 4
    GarrFollowerType = 3217892214, //! MetaFlags: 4
    GarrFollowerXAbility = 2437774747, //! MetaFlags: 4
    GarrFollSupportSpell = 1992065797, //! MetaFlags: 4
    GarrMechanic = 1672086436, //! MetaFlags: 0
    GarrMechanicType = 2837408936, //! MetaFlags: 0
    GarrMission = 234224168, //! MetaFlags: 0
    GarrMissionReward = 2240070130, //! MetaFlags: 0
    GarrMissionTexture = 939278707, //! MetaFlags: 4
    GarrMissionType = 3638153752, //! MetaFlags: 4
    GarrMissionXEncounter = 1653915679, //! MetaFlags: 0
    GarrMissionXFollower = 3144235570, //! MetaFlags: 4
    GarrMssnBonusAbility = 3275136485, //! MetaFlags: 4
    GarrPlot = 752402434, //! MetaFlags: 4
    GarrPlotBuilding = 1945261867, //! MetaFlags: 4
    GarrPlotInstance = 4093144214, //! MetaFlags: 4
    GarrPlotUICategory = 264699590, //! MetaFlags: 4
    GarrSiteLevel = 583472526, //! MetaFlags: 4
    GarrSiteLevelPlotInst = 1773314988, //! MetaFlags: 4
    GarrSpecialization = 518419645, //! MetaFlags: 4
    GarrTalent = 604014940, //! MetaFlags: 0
    GarrTalentTree = 2640972154, //! MetaFlags: 0
    GarrType = 3513414411, //! MetaFlags: 4
    GarrUiAnimClassInfo = 39598960, //! MetaFlags: 4
    GarrUiAnimRaceInfo = 1280956582, //! MetaFlags: 4
    GemProperties = 2617305709, //! MetaFlags: 4
    GlueScreenEmote = 3562446383, //! MetaFlags: 4
    GlyphBindableSpell = 222011635, //! MetaFlags: 4
    GlyphExclusiveCategory = 3777145859, //! MetaFlags: 4
    GlyphProperties = 4210316457, //! MetaFlags: 4
    GlyphRequiredSpec = 2385530453, //! MetaFlags: 4
    GlyphSlot = 370875876, //! MetaFlags: 4
    GMSurveyAnswers = 1141219786, //! MetaFlags: 4
    GMSurveyCurrentSurvey = 2423343561, //! MetaFlags: 4
    GMSurveyQuestions = 4272931257, //! MetaFlags: 4
    GMSurveySurveys = 202076719, //! MetaFlags: 4
    GroundEffectDoodad = 2777963210, //! MetaFlags: 4
    GroundEffectTexture = 2164840845, //! MetaFlags: 4
    GroupFinderActivity = 2939349194, //! MetaFlags: 4
    GroupFinderActivityGrp = 896541910, //! MetaFlags: 4
    GroupFinderCategory = 803663642, //! MetaFlags: 4
    GuildColorBackground = 2122521976, //! MetaFlags: 4
    GuildColorBorder = 331233397, //! MetaFlags: 4
    GuildColorEmblem = 462112558, //! MetaFlags: 4
    GuildPerkSpells = 26567700, //! MetaFlags: 4
    Heirloom = 779857838, //! MetaFlags: 0
    HelmetAnimScaling = 1015057242, //! MetaFlags: 4
    HelmetGeosetVisData = 940448223, //! MetaFlags: 4
    HighlightColor = 1377377290, //! MetaFlags: 4
    HolidayDescriptions = 690463935, //! MetaFlags: 4
    HolidayNames = 1419319386, //! MetaFlags: 4
    Holidays = 1972272076, //! MetaFlags: 4
    ImportPriceArmor = 1043802212, //! MetaFlags: 4
    ImportPriceQuality = 1013013523, //! MetaFlags: 4
    ImportPriceShield = 735996568, //! MetaFlags: 4
    ImportPriceWeapon = 2540474264, //! MetaFlags: 4
    InvasionClientData = 410588437, //! MetaFlags: 0
    Item = 1344507586, //! MetaFlags: 4
    ItemAppearance = 1109793673, //! MetaFlags: 4
    ItemAppearanceXUiCamera = 2352874699, //! MetaFlags: 4
    ItemArmorQuality = 3108657159, //! MetaFlags: 4
    ItemArmorShield = 4290481602, //! MetaFlags: 4
    ItemArmorTotal = 43864318, //! MetaFlags: 4
    ItemBagFamily = 288365895, //! MetaFlags: 4
    ItemBonus = 2199425034, //! MetaFlags: 0
    ItemBonusListLevelDelta = 962617990, //! MetaFlags: 0
    ItemBonusTreeNode = 2793276977, //! MetaFlags: 4
    ItemChildEquipment = 512648579, //! MetaFlags: 4
    ItemClass = 3111593758, //! MetaFlags: 4
    ItemCurrencyCost = 1876974313, //! MetaFlags: 4
    ItemDamageAmmo = 2542846702, //! MetaFlags: 4
    ItemDamageOneHand = 3769023240, //! MetaFlags: 4
    ItemDamageOneHandCaster = 4069992295, //! MetaFlags: 4
    ItemDamageTwoHand = 3636854083, //! MetaFlags: 4
    ItemDamageTwoHandCaster = 2158076154, //! MetaFlags: 4
    ItemDisenchantLoot = 1722044526, //! MetaFlags: 4
    ItemDisplayInfo = 2557447376, //! MetaFlags: 4
    ItemDisplayInfoMaterialRes = 4082824155, //! MetaFlags: 4
    ItemDisplayXUiCamera = 3851893958, //! MetaFlags: 4
    ItemEffect = 1073915313, //! MetaFlags: 4
    ItemExtendedCost = 3146089301, //! MetaFlags: 4
    ItemGroupSounds = 2671904457, //! MetaFlags: 4
    ItemLimitCategory = 2075795121, //! MetaFlags: 4
    ItemModifiedAppearance = 3834752085, //! MetaFlags: 0
    ItemModifiedAppearanceExtra = 3645674820, //! MetaFlags: 4
    ItemNameDescription = 1891821565, //! MetaFlags: 4
    ItemPetFood = 819861596, //! MetaFlags: 4
    ItemPriceBase = 769695381, //! MetaFlags: 4
    ItemRandomProperties = 79508367, //! MetaFlags: 4
    ItemRandomSuffix = 3867218247, //! MetaFlags: 4
    ItemRangedDisplayInfo = 2297086107, //! MetaFlags: 4
    ItemSearchName = 428746933, //! MetaFlags: 4
    ItemSet = 2389973656, //! MetaFlags: 4
    ItemSetSpell = 2252716921, //! MetaFlags: 4
    Item_sparse = 2442913102, //! MetaFlags: 5
    ItemSpec = 148532778, //! MetaFlags: 4
    ItemSpecOverride = 345681529, //! MetaFlags: 4
    ItemSubClass = 2513986447, //! MetaFlags: 4
    ItemSubClassMask = 3857758069, //! MetaFlags: 4
    ItemUpgrade = 1879459387, //! MetaFlags: 0
    ItemVisualEffects = 2456590174, //! MetaFlags: 4
    ItemVisuals = 2012662885, //! MetaFlags: 4
    ItemXBonusTree = 844464874, //! MetaFlags: 4
    JournalEncounter = 1463296112, //! MetaFlags: 0
    JournalEncounterCreature = 2901650891, //! MetaFlags: 0
    JournalEncounterItem = 4159531103, //! MetaFlags: 0
    JournalEncounterSection = 2035710060, //! MetaFlags: 4
    JournalEncounterXDifficulty = 2822457430, //! MetaFlags: 4
    JournalInstance = 1899895961, //! MetaFlags: 0
    JournalItemXDifficulty = 678313527, //! MetaFlags: 4
    JournalSectionXDifficulty = 184881520, //! MetaFlags: 4
    JournalTier = 651763705, //! MetaFlags: 4
    JournalTierXInstance = 593571724, //! MetaFlags: 4
    KeyChain = 1837770388, //! MetaFlags: 4
    KeystoneAffix = 3269690597, //! MetaFlags: 4
    Languages = 4090750226, //! MetaFlags: 0
    LanguageWords = 3897115516, //! MetaFlags: 4
    LfgDungeonExpansion = 4060071752, //! MetaFlags: 4
    LfgDungeonGroup = 603342232, //! MetaFlags: 4
    LfgDungeons = 2577119682, //! MetaFlags: 0
    LfgDungeonsGroupingMap = 3604091206, //! MetaFlags: 4
    LfgRoleRequirement = 1904724485, //! MetaFlags: 4
    Light = 1556783290, //! MetaFlags: 4
    LightParams = 3330215320, //! MetaFlags: 0
    LightSkybox = 3546600791, //! MetaFlags: 4
    LiquidMaterial = 2352452393, //! MetaFlags: 0
    LiquidObject = 4230614527, //! MetaFlags: 4
    LiquidType = 1712570067, //! MetaFlags: 4
    LoadingScreens = 4020094719, //! MetaFlags: 4
    LoadingScreenTaxiSplines = 3870173959, //! MetaFlags: 0
    Locale = 1065724855, //! MetaFlags: 4
    Location = 961296167, //! MetaFlags: 4
    Lock = 3921595171, //! MetaFlags: 4
    LockType = 2471080772, //! MetaFlags: 0
    LookAtController = 1562431239, //! MetaFlags: 4
    MailTemplate = 3821385681, //! MetaFlags: 4
    ManifestInterfaceActionIcon = 312611201, //! MetaFlags: 0
    ManifestInterfaceData = 4273710460, //! MetaFlags: 4
    ManifestInterfaceItemIcon = 36381654, //! MetaFlags: 0
    ManifestInterfaceTOCData = 3843019564, //! MetaFlags: 4
    ManifestMP3 = 893275916, //! MetaFlags: 0
    Map = 3179597154, //! MetaFlags: 4
    MapChallengeMode = 943410215, //! MetaFlags: 4
    MapDifficulty = 2456155917, //! MetaFlags: 4
    MapDifficultyXCondition = 3080893373, //! MetaFlags: 4
    MarketingPromotionsXLocale = 2715021741, //! MetaFlags: 4
    Material = 2764116073, //! MetaFlags: 4
    MinorTalent = 631345831, //! MetaFlags: 4
    ModelFileData = 2707547180, //! MetaFlags: 0
    ModelManifest = 3324329676, //! MetaFlags: 4
    ModelNameToManifest = 1930364469, //! MetaFlags: 4
    ModifierTree = 2120822102, //! MetaFlags: 4
    Mount = 2524150337, //! MetaFlags: 0
    MountCapability = 4134404214, //! MetaFlags: 0
    MountType = 3317200296, //! MetaFlags: 0
    MountTypeXCapability = 685366953, //! MetaFlags: 4
    Movie = 53344787, //! MetaFlags: 4
    MovieFileData = 3098975037, //! MetaFlags: 4
    MovieVariation = 315695528, //! MetaFlags: 0
    NameGen = 4126835867, //! MetaFlags: 4
    NamesProfanity = 3666008428, //! MetaFlags: 4
    NamesReserved = 633457427, //! MetaFlags: 4
    NamesReservedLocale = 986374917, //! MetaFlags: 4
    NpcModelItemSlotDisplayInfo = 1992314007, //! MetaFlags: 4
    NPCSounds = 1230280159, //! MetaFlags: 4
    ObjectEffect = 1734738485, //! MetaFlags: 4
    ObjectEffectGroup = 1282325123, //! MetaFlags: 4
    ObjectEffectModifier = 1657342891, //! MetaFlags: 4
    ObjectEffectPackage = 1581158180, //! MetaFlags: 4
    ObjectEffectPackageElem = 3408032706, //! MetaFlags: 4
    OutlineEffect = 3107997358, //! MetaFlags: 4
    OverrideSpellData = 3396722460, //! MetaFlags: 4
    PageTextMaterial = 3527639455, //! MetaFlags: 4
    PaperDollItemFrame = 2916845724, //! MetaFlags: 4
    ParticleColor = 3902533594, //! MetaFlags: 4
    Path = 2499044245, //! MetaFlags: 0
    PathNode = 1000230050, //! MetaFlags: 0
    PathNodeProperty = 4263624740, //! MetaFlags: 0
    PathProperty = 149245792, //! MetaFlags: 0
    Phase = 4040235363, //! MetaFlags: 4
    PhaseShiftZoneSounds = 2075223363, //! MetaFlags: 4
    PhaseXPhaseGroup = 634137177, //! MetaFlags: 4
    PlayerCondition = 3108775943, //! MetaFlags: 4
    Positioner = 604866126, //! MetaFlags: 4
    PositionerState = 1623486109, //! MetaFlags: 4
    PositionerStateEntry = 1176493125, //! MetaFlags: 4
    PowerDisplay = 1638210446, //! MetaFlags: 4
    PowerType = 2374605399, //! MetaFlags: 4
    PrestigeLevelInfo = 1362239809, //! MetaFlags: 4
    PvpDifficulty = 3397665861, //! MetaFlags: 4
    PvpItem = 4187421273, //! MetaFlags: 4
    PvpReward = 2333820745, //! MetaFlags: 4
    PvpTalent = 1672384929, //! MetaFlags: 0
    PvpTalentUnlock = 955569378, //! MetaFlags: 4
    QuestFactionReward = 3216535624, //! MetaFlags: 4
    QuestFeedbackEffect = 2043095305, //! MetaFlags: 4
    QuestInfo = 756143922, //! MetaFlags: 4
    QuestLine = 3948389411, //! MetaFlags: 4
    QuestLineXQuest = 2361489494, //! MetaFlags: 4
    QuestMoneyReward = 3473498257, //! MetaFlags: 4
    QuestObjective = 20077728, //! MetaFlags: 4
    QuestPackageItem = 3425666288, //! MetaFlags: 4
    QuestPOIBlob = 2921112328, //! MetaFlags: 4
    QuestPOIPoint = 2202435563, //! MetaFlags: 0
    QuestPOIPointCliTask = 2412384869, //! MetaFlags: 4
    QuestSort = 1417267977, //! MetaFlags: 4
    QuestV2 = 986198281, //! MetaFlags: 4
    QuestV2CliTask = 261693969, //! MetaFlags: 0
    QuestXP = 1032430962, //! MetaFlags: 4
    RacialMounts = 1113597634, //! MetaFlags: 4
    RandPropPoints = 1569517478, //! MetaFlags: 4
    ResearchBranch = 1108318853, //! MetaFlags: 4
    ResearchField = 3722626511, //! MetaFlags: 0
    ResearchProject = 2169900995, //! MetaFlags: 0
    ResearchSite = 2620501600, //! MetaFlags: 4
    Resistances = 2444969254, //! MetaFlags: 4
    RewardPack = 3472385212, //! MetaFlags: 4
    RewardPackXCurrencyType = 3554473599, //! MetaFlags: 4
    RewardPackXItem = 2139595463, //! MetaFlags: 4
    RulesetItemUpgrade = 1840711788, //! MetaFlags: 4
    ScalingStatDistribution = 1171454503, //! MetaFlags: 4
    Scenario = 2198813286, //! MetaFlags: 4
    ScenarioEventEntry = 3247720076, //! MetaFlags: 4
    ScenarioStep = 2523372402, //! MetaFlags: 4
    SceneScript = 3568395212, //! MetaFlags: 4
    SceneScriptPackage = 3905641993, //! MetaFlags: 4
    SceneScriptPackageMember = 3830298396, //! MetaFlags: 0
    ScheduledInterval = 2998606315, //! MetaFlags: 0
    ScheduledWorldState = 945022647, //! MetaFlags: 0
    ScheduledWorldStateGroup = 1473491185, //! MetaFlags: 0
    ScreenEffect = 3007265124, //! MetaFlags: 4
    ScreenLocation = 1824228575, //! MetaFlags: 4
    SeamlessSite = 519658371, //! MetaFlags: 4
    ServerMessages = 3172303717, //! MetaFlags: 4
    ShadowyEffect = 2913784219, //! MetaFlags: 4
    SkillLine = 3040725462, //! MetaFlags: 4
    SkillLineAbility = 4282664694, //! MetaFlags: 4
    SkillRaceClassInfo = 112059424, //! MetaFlags: 4
    SoundAmbience = 905567832, //! MetaFlags: 4
    SoundAmbienceFlavor = 647093267, //! MetaFlags: 4
    SoundBus = 120365402, //! MetaFlags: 0
    SoundBusName = 1257636865, //! MetaFlags: 0
    SoundEmitterPillPoints = 3634485004, //! MetaFlags: 4
    SoundEmitters = 3325606100, //! MetaFlags: 0
    SoundFilter = 250652247, //! MetaFlags: 4
    SoundFilterElem = 2709496350, //! MetaFlags: 4
    SoundKit = 908293937, //! MetaFlags: 0
    SoundKitAdvanced = 3232102598, //! MetaFlags: 4
    SoundKitChild = 736158310, //! MetaFlags: 4
    SoundKitEntry = 3266400455, //! MetaFlags: 4
    SoundKitFallback = 3917681400, //! MetaFlags: 4
    SoundOverride = 2390774732, //! MetaFlags: 4
    SoundProviderPreferences = 3131514536, //! MetaFlags: 4
    SourceInfo = 1010104591, //! MetaFlags: 4
    SpamMessages = 2443936038, //! MetaFlags: 4
    SpecializationSpells = 1337593436, //! MetaFlags: 0
    Spell = 3776013982, //! MetaFlags: 0
    SpellActionBarPref = 2226175447, //! MetaFlags: 4
    SpellActivationOverlay = 4167587713, //! MetaFlags: 4
    SpellAuraOptions = 4096770149, //! MetaFlags: 4
    SpellAuraRestrictions = 3130494798, //! MetaFlags: 4
    SpellAuraVisibility = 3899321669, //! MetaFlags: 0
    SpellAuraVisXChrSpec = 2096181888, //! MetaFlags: 4
    SpellCastingRequirements = 1627543382, //! MetaFlags: 4
    SpellCastTimes = 4256848486, //! MetaFlags: 4
    SpellCategories = 3689412649, //! MetaFlags: 4
    SpellCategory = 3502494094, //! MetaFlags: 4
    SpellChainEffects = 517656225, //! MetaFlags: 4
    SpellClassOptions = 680438657, //! MetaFlags: 4
    SpellCooldowns = 4193483863, //! MetaFlags: 4
    SpellDescriptionVariables = 1019780096, //! MetaFlags: 4
    SpellDispelType = 729546851, //! MetaFlags: 4
    SpellDuration = 3022256762, //! MetaFlags: 4
    SpellEffect = 4030871717, //! MetaFlags: 0
    SpellEffectCameraShakes = 1939361897, //! MetaFlags: 4
    SpellEffectEmission = 3274165704, //! MetaFlags: 4
    SpellEffectGroupSize = 599752208, //! MetaFlags: 4
    SpellEffectScaling = 2645418034, //! MetaFlags: 4
    SpellEquippedItems = 3830102996, //! MetaFlags: 4
    SpellFlyout = 3032234982, //! MetaFlags: 4
    SpellFlyoutItem = 3502171243, //! MetaFlags: 4
    SpellFocusObject = 3114120978, //! MetaFlags: 4
    SpellIcon = 14913004, //! MetaFlags: 4
    SpellInterrupts = 1720692227, //! MetaFlags: 4
    SpellItemEnchantment = 3764045193, //! MetaFlags: 4
    SpellItemEnchantmentCondition = 4182849840, //! MetaFlags: 4
    SpellKeyboundOverride = 3568717261, //! MetaFlags: 4
    SpellLabel = 813076512, //! MetaFlags: 4
    SpellLearnSpell = 3689805315, //! MetaFlags: 4
    SpellLevels = 501138918, //! MetaFlags: 4
    SpellMechanic = 1919757118, //! MetaFlags: 4
    SpellMisc = 3322146344, //! MetaFlags: 4
    SpellMiscDifficulty = 2143331989, //! MetaFlags: 0
    SpellMissile = 1754233351, //! MetaFlags: 4
    SpellMissileMotion = 930182777, //! MetaFlags: 4
    SpellPower = 2712461791, //! MetaFlags: 0
    SpellPowerDifficulty = 2124448848, //! MetaFlags: 0
    SpellProceduralEffect = 4139550175, //! MetaFlags: 0
    SpellProcsPerMinute = 3497430124, //! MetaFlags: 4
    SpellProcsPerMinuteMod = 3945052460, //! MetaFlags: 4
    SpellRadius = 2877179969, //! MetaFlags: 4
    SpellRange = 3763447452, //! MetaFlags: 4
    SpellReagents = 2875640223, //! MetaFlags: 4
    SpellReagentsCurrency = 541701644, //! MetaFlags: 4
    SpellScaling = 1089023333, //! MetaFlags: 4
    SpellShapeshift = 3163679255, //! MetaFlags: 4
    SpellShapeshiftForm = 4182948165, //! MetaFlags: 4
    SpellSpecialUnitEffect = 2947410228, //! MetaFlags: 4
    SpellTargetRestrictions = 3764692828, //! MetaFlags: 4
    SpellTotems = 2769259057, //! MetaFlags: 4
    SpellVisual = 4146370265, //! MetaFlags: 0
    SpellVisualAnim = 859088734, //! MetaFlags: 4
    SpellVisualColorEffect = 1491544390, //! MetaFlags: 4
    SpellVisualEffectName = 48336690, //! MetaFlags: 4
    SpellVisualKit = 4102286043, //! MetaFlags: 0
    SpellVisualKitAreaModel = 545961245, //! MetaFlags: 4
    SpellVisualKitEffect = 1181150530, //! MetaFlags: 4
    SpellVisualKitModelAttach = 4033975491, //! MetaFlags: 0
    SpellVisualMissile = 1369604944, //! MetaFlags: 0
    SpellXSpellVisual = 666345498, //! MetaFlags: 0
    Startup_Strings = 4211496909, //! MetaFlags: 4
    Stationery = 2936914515, //! MetaFlags: 4
    StringLookups = 1247625963, //! MetaFlags: 4
    SummonProperties = 868868520, //! MetaFlags: 4
    TactKey = 3744420815, //! MetaFlags: 0
    TactKeyLookup = 2948698321, //! MetaFlags: 0
    Talent = 4188284511, //! MetaFlags: 4
    TaxiNodes = 1356405368, //! MetaFlags: 0
    TaxiPath = 2872091740, //! MetaFlags: 0
    TaxiPathNode = 3857287356, //! MetaFlags: 0
    TerrainMaterial = 844575557, //! MetaFlags: 4
    TerrainType = 2402854065, //! MetaFlags: 4
    TerrainTypeSounds = 4092287527, //! MetaFlags: 4
    TextureBlendSet = 2512539351, //! MetaFlags: 4
    TextureFileData = 440872980, //! MetaFlags: 0
    TotemCategory = 2812474010, //! MetaFlags: 4
    Toy = 2063803484, //! MetaFlags: 0
    TradeSkillCategory = 624597867, //! MetaFlags: 4
    TradeSkillItem = 3037505077, //! MetaFlags: 4
    TransformMatrix = 173929978, //! MetaFlags: 4
    TransmogSet = 356071576, //! MetaFlags: 0
    TransmogSetItem = 505524148, //! MetaFlags: 0
    TransportAnimation = 1805315045, //! MetaFlags: 4
    TransportPhysics = 3494282461, //! MetaFlags: 4
    TransportRotation = 2521134397, //! MetaFlags: 4
    Trophy = 4047189852, //! MetaFlags: 4
    UiCamera = 4138277569, //! MetaFlags: 4
    UiCameraType = 914907145, //! MetaFlags: 4
    UiCamFbackTransmogChrRace = 1296758631, //! MetaFlags: 4
    UiCamFbackTransmogWeapon = 1100209526, //! MetaFlags: 4
    UiTextureAtlas = 2303540067, //! MetaFlags: 0
    UiTextureAtlasMember = 3039853142, //! MetaFlags: 0
    UiTextureKit = 367806651, //! MetaFlags: 4
    UnitBlood = 3682280824, //! MetaFlags: 4
    UnitBloodLevels = 186245800, //! MetaFlags: 4
    UnitCondition = 240389885, //! MetaFlags: 4
    UnitPowerBar = 1161940423, //! MetaFlags: 4
    Vehicle = 178219711, //! MetaFlags: 4
    VehicleSeat = 3567803627, //! MetaFlags: 4
    VehicleUIIndicator = 2177964088, //! MetaFlags: 4
    VehicleUIIndSeat = 1939779537, //! MetaFlags: 4
    VideoHardware = 1078558972, //! MetaFlags: 0
    Vignette = 4021368146, //! MetaFlags: 4
    VocalUISounds = 1378031962, //! MetaFlags: 4
    WbAccessControlList = 1477136115, //! MetaFlags: 4
    WbCertBlacklist = 3450573023, //! MetaFlags: 4
    WbCertWhitelist = 2287306173, //! MetaFlags: 4
    WbPermissions = 4163366139, //! MetaFlags: 4
    WeaponImpactSounds = 2921579418, //! MetaFlags: 4
    WeaponSwingSounds2 = 3356076557, //! MetaFlags: 4
    WeaponTrail = 2753853165, //! MetaFlags: 4
    WeaponTrailModelDef = 2699506049, //! MetaFlags: 4
    WeaponTrailParam = 1022281925, //! MetaFlags: 4
    Weather = 3271961498, //! MetaFlags: 4
    WindSettings = 1924290945, //! MetaFlags: 4
    WMOAreaTable = 3343891947, //! MetaFlags: 0
    WmoMinimapTexture = 2982519032, //! MetaFlags: 7
    World_PVP_Area = 2950844250, //! MetaFlags: 4
    WorldBossLockout = 4095124812, //! MetaFlags: 4
    WorldChunkSounds = 2572780382, //! MetaFlags: 4
    WorldEffect = 3664936999, //! MetaFlags: 4
    WorldElapsedTimer = 3349771169, //! MetaFlags: 4
    WorldMapArea = 1404082561, //! MetaFlags: 0
    WorldMapContinent = 2345870169, //! MetaFlags: 4
    WorldMapOverlay = 494145138, //! MetaFlags: 4
    WorldMapTransforms = 371324299, //! MetaFlags: 4
    WorldSafeLocs = 2769561834, //! MetaFlags: 4
    WorldState = 3359787322, //! MetaFlags: 0
    WorldStateExpression = 492039028, //! MetaFlags: 4
    WorldStateUI = 825311328, //! MetaFlags: 0
    WorldStateZoneSounds = 1303106555, //! MetaFlags: 4
    ZoneIntroMusicTable = 558463671, //! MetaFlags: 4
    ZoneLight = 1509092176, //! MetaFlags: 4
    ZoneLightPoint = 486089069, //! MetaFlags: 4
    ZoneMusic = 4071081280, //! MetaFlags: 4
};
#endif

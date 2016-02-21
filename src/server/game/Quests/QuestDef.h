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

#ifndef QUEST_H
#define QUEST_H

#include "DatabaseEnv.h"
#include "SharedDefines.h"
#include "WorldPacket.h"

class Player;

class ObjectMgr;

namespace WorldPackets
{
    namespace Quest
    {
        struct QuestRewards;
    }
}

#define MAX_QUEST_LOG_SIZE 25

#define QUEST_OBJECTIVES_COUNT 10
#define QUEST_ITEM_OBJECTIVES_COUNT 10

#define QUEST_ITEM_DROP_COUNT 4
#define QUEST_REWARD_CHOICES_COUNT 6
#define QUEST_REWARD_ITEM_COUNT 4
#define QUEST_DEPLINK_COUNT 10
#define QUEST_REWARD_REPUTATIONS_COUNT 5
#define QUEST_EMOTE_COUNT 4
#define QUEST_PVP_KILL_SLOT 0
#define QUEST_REWARD_CURRENCY_COUNT 4
#define QUEST_REQUIRED_CURRENCY_COUNT 4

enum QuestType
{
    QUEST_TYPE_AUTOCOMPLETE         = 0,
    QUEST_TYPE_DISABLED             = 1,
    QUEST_TYPE_NORMAL               = 2,
    QUEST_TYPE_UNK                  = 3,

    MAX_QUEST_TYPES
};

enum QuestInfo // QuestInfo.db2 7.0.1.20994
{
    QUEST_INFO_GROUP                        = 1,
    QUEST_INFO_CLASS                        = 21,
    QUEST_INFO_PVP                          = 41,
    QUEST_INFO_RAID                         = 62,
    QUEST_INFO_DUNGEON                      = 81,
    QUEST_INFO_WORLD_EVENT                  = 82,
    QUEST_INFO_LEGENDARY                    = 83,
    QUEST_INFO_ESCORT                       = 84,
    QUEST_INFO_HEROIC                       = 85,
    QUEST_INFO_RAID_10                      = 88,
    QUEST_INFO_RAID_25                      = 89,
    QUEST_INFO_SCENARIO                     = 98,
    QUEST_INFO_ACCOUNT                      = 102,
    QUEST_INFO_SIDE_QUEST                   = 104,
    QUEST_INFO_ARTIFACT                     = 107,
    QUEST_INFO_WORLD_QUEST                  = 109,
    QUEST_INFO_RARE_WORLD_QUEST             = 110,
    QUEST_INFO_ELITE_WORLD_QUEST            = 111,
    QUEST_INFO_RARE_ELITE_WORLD_QUEST       = 112,
    QUEST_INFO_PVP_WORLD_QUEST              = 113,
    QUEST_INFO_FIRST_AID_WORLD_QUEST        = 114,
    QUEST_INFO_BATTLE_PET_WORLD_QUEST       = 115,
    QUEST_INFO_BLACKSMITHING_WORLD_QUEST    = 116,
    QUEST_INFO_LEATHERWORKING_WORLD_QUEST   = 117,
    QUEST_INFO_ALCHEMY_WORLD_QUEST          = 118,
    QUEST_INFO_HERBALISM_WORLD_QUEST        = 119,
    QUEST_INFO_MINING_WORLD_QUEST           = 120,
    QUEST_INFO_TAILORING_WORLD_QUEST        = 121,
    QUEST_INFO_ENGINEERING_WORLD_QUEST      = 122,
    QUEST_INFO_ENCHANTING_WORLD_QUEST       = 123,
    QUEST_INFO_SKINNING_WORLD_QUEST         = 124,
    QUEST_INFO_JEWELCRAFTING_WORLD_QUEST    = 125,
    QUEST_INFO_INSCRIPTION_WORLD_QUEST      = 126,
    QUEST_INFO_EMISSARY_QUEST               = 128,
    QUEST_INFO_ARCHAEOLOGY_WORLD_QUEST      = 129,
    QUEST_INFO_FISHING_WORLD_QUEST          = 130,
    QUEST_INFO_COOKING_WORLD_QUEST          = 131,
};

enum QuestObjectiveType // QuestObjective.db2 7.0.1.20994
{
    QUEST_OBJECTIVE_MONSTER                                 = 0,
    QUEST_OBJECTIVE_ITEM                                    = 1,
    QUEST_OBJECTIVE_GAMEOBJECT                              = 2,
    QUEST_OBJECTIVE_TALKTO                                  = 3,
    QUEST_OBJECTIVE_CURRENCY                                = 4,
    QUEST_OBJECTIVE_LEARNSPELL                              = 5,
    QUEST_OBJECTIVE_MIN_REPUTATION                          = 6,
    QUEST_OBJECTIVE_MAX_REPUTATION                          = 7,
    QUEST_OBJECTIVE_MONEY                                   = 8,
    QUEST_OBJECTIVE_PLAYERKILLS                             = 9,
    QUEST_OBJECTIVE_AREATRIGGER                             = 10,
    QUEST_OBJECTIVE_PET_TRAINER_DEFEAT                      = 11,
    QUEST_OBJECTIVE_DEFEATBATTLEPET                         = 12,
    QUEST_OBJECTIVE_PET_BATTLE_VICTORIES                    = 13,
    QUEST_OBJECTIVE_COMPLETE_CRITERIA_TREE                  = 14, // CriteriaTree.dbc
    QUEST_OBJECTIVE_EXCELLENCE_IN_ZONE                      = 15, // kill some npc, use some gobjects etc... and make dynamic quest UI completed
    QUEST_OBJECTIVE_OBTAIN_CURRENCY_AND_UPGRADE_GARRISON    = 16,
    QUEST_OBJECTIVE_OBTAIN_CURRENCY                         = 17,
};

enum QuestFailedReasons
{
    INVALIDREASON_DONT_HAVE_REQ                 = 0,
    INVALIDREASON_QUEST_FAILED_LOW_LEVEL        = 1,        // You are not high enough level for that quest.
    INVALIDREASON_QUEST_FAILED_WRONG_RACE       = 6,        // That quest is not available to your race.
    INVALIDREASON_QUEST_ALREADY_DONE            = 7,        // You have completed that quest.
    INVALIDREASON_QUEST_ONLY_ONE_TIMED          = 12,       // You can only be on one timed quest at a time.
    INVALIDREASON_QUEST_ALREADY_ON              = 13,       // You are already on that quest.
    INVALIDREASON_QUEST_FAILED_EXPANSION        = 16,       // This quest requires an expansion enabled account.
    INVALIDREASON_QUEST_ALREADY_ON2             = 18,       // You are already on that quest.
    INVALIDREASON_QUEST_FAILED_MISSING_ITEMS    = 21,       // You don't have the required items with you. Check storage.
    INVALIDREASON_QUEST_FAILED_NOT_ENOUGH_MONEY = 23,       // You don't have enough money for that quest.
    INVALIDREASON_QUEST_FAILED_CAIS             = 26,       // You cannot complete quests once you have reached tired time.
    INVALIDREASON_DAILY_QUEST_COMPLETED_TODAY   = 28,       // You have completed that daily quest today.
    INVALIDREASON_QUEST_FAILED_SPELL            = 30,
};

enum QuestShareMessages
{
    QUEST_PARTY_MSG_SHARING_QUEST           = 0,
    QUEST_PARTY_MSG_CANT_TAKE_QUEST         = 1,
    QUEST_PARTY_MSG_ACCEPT_QUEST            = 2,
    QUEST_PARTY_MSG_DECLINE_QUEST           = 3,
    QUEST_PARTY_MSG_BUSY                    = 4,
    QUEST_PARTY_MSG_DEAD                    = 5,
    QUEST_PARTY_MSG_LOG_FULL                = 6,
    QUEST_PARTY_MSG_HAVE_QUEST              = 7,
    QUEST_PARTY_MSG_FINISH_QUEST            = 8,
    QUEST_PARTY_MSG_NOT_DAILY               = 9,
    QUEST_PARTY_MSG_SHARING_TIMER_EXPIRED   = 10,
    QUEST_PARTY_MSG_NOT_IN_PARTY            = 11,
    QUEST_PARTY_MSG_DIFFERENT_SERVER_DAILY  = 12,
    QUEST_PARTY_MSG_PUSH_NOT_ALLOWED        = 13,
};

enum __QuestTradeSkill
{
    QUEST_TRSKILL_NONE           = 0,
    QUEST_TRSKILL_ALCHEMY        = 1,
    QUEST_TRSKILL_BLACKSMITHING  = 2,
    QUEST_TRSKILL_COOKING        = 3,
    QUEST_TRSKILL_ENCHANTING     = 4,
    QUEST_TRSKILL_ENGINEERING    = 5,
    QUEST_TRSKILL_FIRSTAID       = 6,
    QUEST_TRSKILL_HERBALISM      = 7,
    QUEST_TRSKILL_LEATHERWORKING = 8,
    QUEST_TRSKILL_POISONS        = 9,
    QUEST_TRSKILL_TAILORING      = 10,
    QUEST_TRSKILL_MINING         = 11,
    QUEST_TRSKILL_FISHING        = 12,
    QUEST_TRSKILL_SKINNING       = 13,
    QUEST_TRSKILL_JEWELCRAFTING  = 14,
};

enum QuestStatus
{
    QUEST_STATUS_NONE           = 0,
    QUEST_STATUS_COMPLETE       = 1,
    //QUEST_STATUS_UNAVAILABLE    = 2,
    QUEST_STATUS_INCOMPLETE     = 3,
    //QUEST_STATUS_AVAILABLE      = 4,
    QUEST_STATUS_FAILED         = 5,
    QUEST_STATUS_REWARDED       = 6,        // Not used in DB
    MAX_QUEST_STATUS
};

enum __QuestGiverStatus
{
    DIALOG_STATUS_NONE                     = 0x000,
    DIALOG_STATUS_UNK                      = 0x001,
    DIALOG_STATUS_UNAVAILABLE              = 0x002,
    DIALOG_STATUS_LOW_LEVEL_AVAILABLE      = 0x004,
    DIALOG_STATUS_LOW_LEVEL_REWARD_REP     = 0x008,
    DIALOG_STATUS_LOW_LEVEL_AVAILABLE_REP  = 0x010,
    DIALOG_STATUS_INCOMPLETE               = 0x020,
    DIALOG_STATUS_REWARD_REP               = 0x040,
    DIALOG_STATUS_AVAILABLE_REP            = 0x080,
    DIALOG_STATUS_AVAILABLE                = 0x100,
    DIALOG_STATUS_REWARD2                  = 0x200,         // no yellow dot on minimap
    DIALOG_STATUS_REWARD                   = 0x400          // yellow dot on minimap
};

enum __QuestFlags
{
    // Flags used at server and sent to client
    QUEST_FLAGS_NONE           = 0x00000000,
    QUEST_FLAGS_STAY_ALIVE     = 0x00000001,                // Not used currently
    QUEST_FLAGS_PARTY_ACCEPT   = 0x00000002,                // Not used currently. If player in party, all players that can accept this quest will receive confirmation box to accept quest CMSG_QUEST_CONFIRM_ACCEPT/SMSG_QUEST_CONFIRM_ACCEPT
    QUEST_FLAGS_EXPLORATION    = 0x00000004,                // Not used currently
    QUEST_FLAGS_SHARABLE       = 0x00000008,                // Can be shared: Player::CanShareQuest()
    //QUEST_FLAGS_NONE2        = 0x00000010,                // Not used currently
    QUEST_FLAGS_EPIC           = 0x00000020,                // Not used currently: Unsure of content
    QUEST_FLAGS_RAID           = 0x00000040,                // Not used currently
    QUEST_FLAGS_TBC            = 0x00000080,                // Not used currently: Available if TBC expansion enabled only
    QUEST_FLAGS_NO_MONEY_FROM_XP        = 0x00000100,   // Not used currently: Experience is not converted to gold at max level
    QUEST_FLAGS_HIDDEN_REWARDS = 0x00000200,                // Items and money rewarded only sent in SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE (not in SMSG_QUESTGIVER_QUEST_DETAILS or in client quest log(SMSG_QUERY_QUEST_INFO_RESPONSE))
    QUEST_FLAGS_AUTO_REWARDED  = 0x00000400,                // These quests are automatically rewarded on quest complete and they will never appear in quest log client side.
    QUEST_FLAGS_TBC_RACES      = 0x00000800,                // Not used currently: Blood elf/Draenei starting zone quests
    QUEST_FLAGS_DAILY          = 0x00001000,                // Used to know quest is Daily one
    QUEST_FLAGS_REPEATABLE     = 0x00002000,                // Used on repeatable quests (3.0.0+)
    QUEST_FLAGS_UNAVAILABLE    = 0x00004000,                // Used on quests that are not generically available
    QUEST_FLAGS_WEEKLY         = 0x00008000,
    QUEST_FLAGS_AUTO_SUBMIT    = 0x00010000,                // Mop541: Quests with this flag player submit automatically by special button in player gui
    QUEST_FLAGS_SPECIAL_ITEM   = 0x00020000,                // has something to do with RequiredItemId and SourceItemId
    QUEST_FLAGS_OBJ_TEXT       = 0x00040000,                // use Objective text as Complete text
    QUEST_FLAGS_AUTO_ACCEPT    = 0x00080000,                // The client recognizes this flag as auto-accept. However, NONE of the current quests (3.3.5a) have this flag. Maybe blizz used to use it, or will use it in the future.
    //QUEST_FLAGS_AUTO_SUBMIT    = 0x00100000,                // Quests with this flag player submit automatically by special button in player gui
    QUEST_FLAGS_AUTO_TAKE      = 0x00200000,                // Automatically suggestion of accepting quest. Not from npc.
    //QUEST_FLAGS_AUTOCOMPLETE
    //QUEST_FLAGS_UNK2           = 0x00400000,
    //QUEST_FLAGS_UNK3           = 0x00800000,                // Found in quest 14069
    //QUEST_FLAGS_UNK4           = 0x01000000,
    QUEST_FLAG_TRACKING_QUEST  = 0x02000000,

    // ... 4.x added flags up to 0x80000000 - all unknown for now
};

// last checked in 19802
enum QuestFlagsEx
{
    QUEST_FLAGS_EX_NONE                                     = 0X000000,
    QUEST_FLAGS_EX_KEEP_ADDITIONAL_ITEMS                    = 0X000001,
    QUEST_FLAGS_EX_SUPPRESS_GOSSIP_COMPLETE                 = 0X000002,
    QUEST_FLAGS_EX_SUPPRESS_GOSSIP_ACCEPT                   = 0X000004,
    QUEST_FLAGS_EX_DISALLOW_PLAYER_AS_QUESTGIVER            = 0X000008,
    QUEST_FLAGS_EX_DISPLAY_CLASS_CHOICE_REWARDS             = 0X000010,
    QUEST_FLAGS_EX_DISPLAY_SPEC_CHOICE_REWARDS              = 0X000020,
    QUEST_FLAGS_EX_REMOVE_FROM_LOG_ON_PERIDOIC_RESET        = 0X000040,
    QUEST_FLAGS_EX_ACCOUNT_LEVEL_QUEST                      = 0X000080,
    QUEST_FLAGS_EX_LEGENDARY_QUEST                          = 0X000100,
    QUEST_FLAGS_EX_NO_GUILD_XP                              = 0X000200,
    QUEST_FLAGS_EX_RESET_CACHE_ON_ACCEPT                    = 0X000400,
    QUEST_FLAGS_EX_NO_ABANDON_ONCE_ANY_OBJECTIVE_COMPLETE   = 0X000800,
    QUEST_FLAGS_EX_RECAST_ACCEPT_SPELL_ON_LOGIN             = 0X001000,
    QUEST_FLAGS_EX_UPDATE_ZONE_AURAS                        = 0X002000,
    QUEST_FLAGS_EX_NO_CREDIT_FOR_PROXY                      = 0X004000,
    QUEST_FLAGS_EX_DISPLAY_AS_DAILY_QUEST                   = 0X008000,
    QUEST_FLAGS_EX_PART_OF_QUEST_LINE                       = 0X010000,
    QUEST_FLAGS_EX_QUEST_FOR_INTERNAL_BUILDS_ONLY           = 0X020000,
    QUEST_FLAGS_EX_SUPPRESS_SPELL_LEARN_TEXT_LINE           = 0X040000,
    QUEST_FLAGS_EX_DISPLAY_HEADER_AS_OBJECTIVE_FOR_TASKS    = 0X080000,
    QUEST_FLAGS_EX_GARRISON_NON_OWNERS_ALLOWED              = 0X100000
};

enum __QuestSpecialFlags
{
    QUEST_SPECIAL_FLAGS_NONE                 = 0x000,
    // Trinity flags for set SpecialFlags in DB if required but used only at server
    QUEST_SPECIAL_FLAGS_REPEATABLE           = 0x001,
    QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT = 0x002, // if required area explore, spell SPELL_EFFECT_QUEST_COMPLETE casting, table `*_script` command SCRIPT_COMMAND_QUEST_EXPLORED use, set from script)
    QUEST_SPECIAL_FLAGS_AUTO_ACCEPT          = 0x004, // quest is to be auto-accepted.
    QUEST_SPECIAL_FLAGS_DF_QUEST             = 0x008, // quest is used by Dungeon Finder.
    QUEST_SPECIAL_FLAGS_NOT_REMOVE_SOURCE    = 0x010, // then we shoul leave at player requared items (exp. q30027)
    QUEST_SPECIAL_FLAGS_CAST                 = 0x020,   // Set by 32 in SpecialFlags in DB if the quest requires RequiredOrNpcGo killcredit but NOT kill (a spell cast)

    // room for more custom flags

    //QUEST_SPECIAL_FLAGS_DB_ALLOWED = QUEST_SPECIAL_FLAGS_REPEATABLE | QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT | QUEST_SPECIAL_FLAGS_AUTO_ACCEPT | QUEST_SPECIAL_FLAGS_DF_QUEST,
    QUEST_SPECIAL_FLAGS_DB_ALLOWED           = 255,

    QUEST_SPECIAL_FLAGS_DELIVER              = 0x080,   // Internal flag computed only
    QUEST_SPECIAL_FLAGS_SPEAKTO              = 0x100,   // Internal flag computed only
    QUEST_SPECIAL_FLAGS_KILL                 = 0x200,   // Internal flag computed only
    QUEST_SPECIAL_FLAGS_TIMED                = 0x400,   // Internal flag computed only
    QUEST_SPECIAL_FLAGS_PLAYER_KILL          = 0x800,   // Internal flag computed only
};

struct QuestTemplateLocale
{
    StringVector LogTitle;
    StringVector LogDescription;
    StringVector QuestDescription;
    StringVector AreaDescription;
    StringVector PortraitGiverText;
    StringVector PortraitGiverName;
    StringVector PortraitTurnInText;
    StringVector PortraitTurnInName;
    StringVector QuestCompletionLog;

    /// @todo: implemente this in new tables
    StringVector OfferRewardText;
    StringVector RequestItemsText;
};

struct QuestObjectivesLocale
{
    StringVector Description;
};

enum QuestObjectiveFlags
{
    QUEST_OBJECTIVE_FLAG_HIDEN = 0x2,
};

struct QuestObjective
{
    uint32 ID           = 0;
    uint8  Type         = 0;
    int8   StorageIndex = 0;
    int32  ObjectID     = 0;
    int32  Amount       = 0;
    uint32 Flags        = 0;
    float  UnkFloat     = 0.0f;
    std::string Description;
    std::vector<int32> VisualEffects;
};

typedef std::vector<QuestObjective> QuestObjectives;

struct QuestLineStruct
{
    uint8 LineID = 0;
    uint8 Pos = 0;
};

typedef std::vector<int32> PrevQuests;
typedef std::vector<uint32> PrevChainQuests;

// This Quest class provides a convenient way to access a few pretotaled (cached) quest details,
// all base quest information, and any utility functions such as generating the amount of
// xp to give
class Quest
{
    friend class ObjectMgr;
    public:
        Quest(Field* questRecord);
        void LoadQuestDetails(Field* fields);
        void LoadQuestRequestItems(Field* fields);
        void LoadQuestOfferReward(Field* fields);
        void LoadQuestTemplateAddon(Field* fields);
        void LoadQuestObjective(Field* fields);
        void LoadQuestObjectiveVisualEffect(Field* fields);

        uint32 XPValue(Player* player) const;

        bool HasFlag(uint32 flag) const { return (Flags & flag) != 0; }
        void SetFlag(uint32 flag) { Flags |= flag; }

        bool HasSpecialFlag(uint32 flag) const { return (SpecialFlags & flag) != 0; }
        void SetSpecialFlag(uint32 flag) { SpecialFlags |= flag; }

        // table data accessors:
        uint32 GetQuestId() const { return Id; }
        QuestObjectives const& GetObjectives() const { return Objectives; };
        uint32 MoneyValue(uint8 playerLVL) const;
        uint32 GetRewMoneyMaxLevel() const; // use in XP calculation at client
        bool   IsRepeatable() const { return (SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE) != 0; }
        bool   IsAutoAccept() const;
        bool   IsAutoComplete() const;
        uint32 GetFlags() const { return Flags; }
        bool   IsDaily() const { return (Flags & QUEST_FLAGS_DAILY) != 0; }
        bool   IsWeekly() const { return (Flags & QUEST_FLAGS_WEEKLY) != 0; }
        bool   IsSeasonal() const { return (QuestSortID == -QUEST_SORT_SEASONAL || QuestSortID == -QUEST_SORT_SPECIAL || QuestSortID == -QUEST_SORT_LUNAR_FESTIVAL || QuestSortID == -QUEST_SORT_MIDSUMMER || QuestSortID == -QUEST_SORT_BREWFEST || QuestSortID == -QUEST_SORT_LOVE_IS_IN_THE_AIR || QuestSortID == -QUEST_SORT_NOBLEGARDEN) && !IsRepeatable(); }
        bool   IsDailyOrWeekly() const { return (Flags & (QUEST_FLAGS_DAILY | QUEST_FLAGS_WEEKLY)) != 0; }
        bool   IsRaidQuest(Difficulty difficulty) const;
        bool   IsAllowedInRaid(Difficulty difficulty) const;
        bool   IsDFQuest() const { return (SpecialFlags & QUEST_SPECIAL_FLAGS_DF_QUEST) != 0; }

        uint32 ItemDrop[QUEST_ITEM_DROP_COUNT] = { };
        uint32 ItemDropQuantity[QUEST_ITEM_DROP_COUNT] = { };
        uint32 RewardChoiceItemId[QUEST_REWARD_CHOICES_COUNT] = { };
        uint32 RewardChoiceItemCount[QUEST_REWARD_CHOICES_COUNT] = { };
        uint32 RewardChoiceItemDisplayId[QUEST_REWARD_CHOICES_COUNT] = { };
        uint32 RewardItemId[QUEST_REWARD_ITEM_COUNT] = { };
        uint32 RewardItemCount[QUEST_REWARD_ITEM_COUNT] = { };
        uint32 RewardFactionId[QUEST_REWARD_REPUTATIONS_COUNT] = { };
        int32 RewardFactionValue[QUEST_REWARD_REPUTATIONS_COUNT] = { };
        int32 RewardFactionOverride[QUEST_REWARD_REPUTATIONS_COUNT] = { };
        uint32 DetailsEmote[QUEST_EMOTE_COUNT] = { };
        uint32 DetailsEmoteDelay[QUEST_EMOTE_COUNT] = { };
        uint32 OfferRewardEmote[QUEST_EMOTE_COUNT] = { };
        uint32 OfferRewardEmoteDelay[QUEST_EMOTE_COUNT] = { };
        uint32 RewardCurrencyId[QUEST_REWARD_CURRENCY_COUNT] = { };
        uint32 RewardCurrencyCount[QUEST_REWARD_CURRENCY_COUNT] = { };

        void BuildQuestRewards(WorldPackets::Quest::QuestRewards& rewards, Player* player) const;

        PrevQuests prevQuests;
        PrevChainQuests prevChainQuests;
        QuestObjectives Objectives;

        //< Table data
        uint32 Id;
        uint32 Type;
        uint32 PackageID;
        int32  QuestSortID;
        uint32 MinLevel;
        int32  Level;
        uint32 QuestInfoID;
        int32  AllowableRaces;
        uint32 SuggestedPlayers;
        uint32 LimitTime;
        uint32 Flags;
        uint32 FlagsEx;
        uint32 RewardTitleId;
        uint32 RewardTalents;
        int32  RewardArenaPoints;
        uint32 NextQuestIdChain;
        uint32 RewardXPDifficulty;
        float  RewardXPMultiplier;
        uint32 SourceItemId;
        std::string LogTitle;
        std::string LogDescription;
        std::string QuestDescription;
        std::string OfferRewardText;
        std::string RequestItemsText;
        std::string AreaDescription;
        std::string QuestCompletionLog;
        uint32 RewardHonor;
        float RewardKillHonor;
        int32  RewardMoney;
        uint32 RewardMoneyDifficulty;
        float  RewardMoneyMultiplier;
        uint32 RewardBonusMoney;
        uint32 RewardDisplaySpell;
        int32  RewardSpell;
        uint32 POIContinent;
        float  POIx;
        float  POIy;
        uint32 POIPriority;
        uint32 StartScript;
        uint32 CompleteScript;
        uint32 RewardSkillId;
        uint32 RewardSkillPoints;
        uint32 RewardFactionFlags;
        uint32 QuestGiverPortrait;
        uint32 QuestTurnInPortrait;
        std::string PortraitGiverText;
        std::string PortraitGiverName;
        std::string PortraitTurnInText;
        std::string PortraitTurnInName;
        uint32 SoundAccept;
        uint32 SoundTurnIn;
        uint32 AreaGroupID;
        uint32 m_rewChoiceItemsCount;
        uint32 m_rewItemsCount;
        uint32 m_rewCurrencyCount;
        uint32 m_reqCurrencyCount;

        // quest_request_items table
        uint32 EmoteOnComplete          = 0;
        uint32 EmoteOnIncomplete        = 0;
        uint32 EmoteOnCompleteDelay     = 0;
        uint32 EmoteOnIncompleteDelay   = 0;

        QuestLineStruct Line;

        // quest_template_addon table (custom data)
        uint32 MaxLevel             = 0;
        int32  AllowableClasses     = 0;
        uint32 SourceSpellID        = 0;
        int32  PrevQuestID          = 0;
        int32  NextQuestID          = 0;
        int32  ExclusiveGroup       = 0;
        uint32 RewardMailTemplateId = 0;
        uint32 RewardMailDelay      = 0;
        uint32 RequiredSkillId      = 0;
        uint32 RequiredSkillPoints  = 0;
        uint32 RequiredMinRepFaction = 0;
        int32  RequiredMinRepValue  = 0;
        uint32 RequiredMaxRepFaction = 0;
        int32  RequiredMaxRepValue  = 0;
        uint32 SourceItemIdCount    = 0;
        uint32 SpecialFlags         = 0; // custom flags, not sniffed/WDB
};

typedef std::map<int32/*idx*/, int32/*data*/> QuestStatusDatas;

struct QuestStatusData
{
    QuestStatusData(): Status(QUEST_STATUS_NONE), Timer(0) { }

    QuestStatus Status;
    uint32 Timer;
    QuestStatusDatas ObjectiveData;
};


#endif

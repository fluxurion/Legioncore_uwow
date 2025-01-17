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

#ifndef GUILD_H
#define GUILD_H

#include "World.h"
#include "Item.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "DBCStore.h"

class Item;

namespace WorldPackets
{
    namespace Guild
    {
        class GuildBankLogQueryResults;
        class GuildEventLogQueryResults;
        class GuildNews;
    }
}

enum GuildMisc
{
    GUILD_BANK_MAX_TABS                 = 8,                    // send by client for money log also
    GUILD_BANK_MAX_SLOTS                = 98,
    GUILD_BANK_MONEY_LOGS_TAB           = 100,                  // used for money log in DB
    GUILD_RANKS_MIN_COUNT               = 5,
    GUILD_RANKS_MAX_COUNT               = 10,
    GUILD_RANK_NONE                     = 0xFF,
    GUILD_WITHDRAW_MONEY_UNLIMITED      = 0xFFFFFFFF,
    GUILD_WITHDRAW_SLOT_UNLIMITED       = 0xFFFFFFFF,
    GUILD_EVENT_LOG_GUID_UNDEFINED      = 0xFFFFFFFF,
    TAB_UNDEFINED                       = 0xFF,
};

enum GuildDefaultRanks
{
    // These ranks can be modified, but they cannot be deleted
    GR_GUILDMASTER  = 0,
    GR_OFFICER      = 1,
    GR_VETERAN      = 2,
    GR_MEMBER       = 3,
    GR_INITIATE     = 4
    // When promoting member server does: rank--
    // When demoting member server does: rank++
};

enum GuildRankRights
{
    GR_RIGHT_EMPTY              = 0x00000040,
    GR_RIGHT_GCHATLISTEN        = GR_RIGHT_EMPTY | 0x00000001,
    GR_RIGHT_GCHATSPEAK         = GR_RIGHT_EMPTY | 0x00000002,
    GR_RIGHT_OFFCHATLISTEN      = GR_RIGHT_EMPTY | 0x00000004,
    GR_RIGHT_OFFCHATSPEAK       = GR_RIGHT_EMPTY | 0x00000008,
    GR_RIGHT_INVITE             = GR_RIGHT_EMPTY | 0x00000010,
    GR_RIGHT_REMOVE             = GR_RIGHT_EMPTY | 0x00000020,
    GR_RIGHT_PROMOTE            = GR_RIGHT_EMPTY | 0x00000080,
    GR_RIGHT_DEMOTE             = GR_RIGHT_EMPTY | 0x00000100,
    GR_RIGHT_SETMOTD            = GR_RIGHT_EMPTY | 0x00001000,
    GR_RIGHT_EPNOTE             = GR_RIGHT_EMPTY | 0x00002000,
    GR_RIGHT_VIEWOFFNOTE        = GR_RIGHT_EMPTY | 0x00004000,
    GR_RIGHT_EOFFNOTE           = GR_RIGHT_EMPTY | 0x00008000,
    GR_RIGHT_MODIFY_GUILD_INFO  = GR_RIGHT_EMPTY | 0x00010000,
    GR_RIGHT_WITHDRAW_GOLD_LOCK = 0x00020000,                   // remove money withdraw capacity
    GR_RIGHT_WITHDRAW_REPAIR    = 0x00040000,                   // withdraw for repair
    GR_RIGHT_WITHDRAW_GOLD      = 0x00080000,                   // withdraw gold
    GR_RIGHT_CREATE_GUILD_EVENT = 0x00100000,                   // wotlk
    GR_RIGHT_ALL                = 0x00DDFFBF
};

enum GuildCommandType
{
    GUILD_CREATE_S   = 0x00,
    GUILD_INVITE_S   = 0x01,
    GUILD_QUIT_S     = 0x03,
    GUILD_PROMOTE_SS = 0x06,
    GUILD_DEMOTE_SS  = 0x07,
    GUILD_FOUNDER_S  = 0x0E,
    GUILD_UNK1       = 0x13,
    GUILD_UNK2       = 0x14
};

enum GuildCommandError
{
    ERR_GUILD_COMMAND_SUCCESS           = 0x00,
    ERR_GUILD_INTERNAL                  = 0x01,
    ERR_ALREADY_IN_GUILD                = 0x02,
    ERR_ALREADY_IN_GUILD_S              = 0x03,
    ERR_INVITED_TO_GUILD                = 0x04,
    ERR_ALREADY_INVITED_TO_GUILD_S      = 0x05,
    ERR_GUILD_NAME_INVALID              = 0x06,
    ERR_GUILD_NAME_EXISTS_S             = 0x07,
    ERR_GUILD_LEADER_LEAVE              = 0x08,
    ERR_GUILD_PERMISSIONS               = 0x08,
    ERR_GUILD_PLAYER_NOT_IN_GUILD       = 0x09,
    ERR_GUILD_PLAYER_NOT_IN_GUILD_S     = 0x0A,
    ERR_GUILD_PLAYER_NOT_FOUND_S        = 0x0B,
    ERR_GUILD_NOT_ALLIED                = 0x0C,
    ERR_GUILD_RANK_TOO_HIGH_S           = 0x0D,
    ERR_GUILD_RANK_TOO_LOW_S            = 0x0E,
    ERR_GUILD_RANKS_LOCKED              = 0x11,
    ERR_GUILD_RANK_IN_USE               = 0x12,
    ERR_GUILD_IGNORING_YOU_S            = 0x13,
    ERR_GUILD_UNK1                      = 0x14, // calls roster update
    ERR_GUILD_WITHDRAW_LIMIT            = 0x19,
    ERR_GUILD_NOT_ENOUGH_MONEY          = 0x1A,
    ERR_GUILD_BANK_FULL                 = 0x1C,
    ERR_GUILD_ITEM_NOT_FOUND            = 0x1D,
    ERR_GUILD_TOO_MUCH_MONEY            = 0x1F,
    ERR_GUILD_BANK_WRONG_TAB            = 0x20,
    ERR_RANK_REQUIRES_AUTHENTICATOR     = 0x22,
    ERR_GUILD_BANK_VOUCHER_FAILED       = 0x23,
    ERR_GUILD_TRIAL_ACCOUNT             = 0x24,
    ERR_GUILD_UNDELETABLE_DUE_TO_LEVEL  = 0x25,
    ERR_GUILD_MOVE_STARTING             = 0x26,
    ERR_GUILD_REP_TOO_LOW               = 0x27,
    ERR_GUILD_INVITE_SELF               = 0x28,
};

enum GuildEvents
{
    GE_PROMOTION                    = 1,    // SMSG_GUILD_SEND_RANK_CHANGE
    GE_DEMOTION                     = 2,    // SMSG_GUILD_SEND_RANK_CHANGE
    GE_MOTD                         = 3,    // SMSG_GUILD_EVENT_MOTD
    GE_JOINED                       = 4,    // SMSG_GUILD_EVENT_PLAYER_JOINED
    GE_LEFT                         = 5,    // SMSG_GUILD_EVENT_PLAYER_LEFT
    GE_REMOVED                      = 6,    // SMSG_GUILD_EVENT_PLAYER_LEFT
    GE_LEADER_IS                    = 7,    // SMSG_GUILD_EVENT_NEW_LEADER
    GE_LEADER_CHANGED               = 8,    // SMSG_GUILD_EVENT_NEW_LEADER
    GE_DISBANDED                    = 9,    // SMSG_GUILD_EVENT_DISBANDED
    GE_TABARDCHANGE                 = 10,   // not exist
    GE_RANK_UPDATED                 = 11,   // SMSG_GUILD_EVENT_RANKS_UPDATED
    GE_RANK_CREATED                 = 12,   // SMSG_GUILD_EVENT_RANKS_UPDATED
    GE_RANK_DELETED                 = 13,   // SMSG_GUILD_EVENT_RANKS_UPDATED
    GE_RANK_ORDER_CHANGED           = 14,   // SMSG_GUILD_EVENT_RANKS_UPDATED
    GE_FOUNDER                      = 15,   // SMSG_GUILD_EVENT_RANKS_UPDATED At guild creation - Set founder
    GE_SIGNED_ON                    = 16,   // SMSG_GUILD_EVENT_PRESENCE_CHANGE
    GE_SIGNED_OFF                   = 17,   // SMSG_GUILD_EVENT_PRESENCE_CHANGE
    GE_GUILDBANKBAGSLOTS_CHANGED    = 18,   // SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED
    GE_BANK_TAB_PURCHASED           = 19,   // SMSG_GUILD_EVENT_TAB_ADDED
    GE_BANK_TAB_UPDATED             = 20,   // SMSG_GUILD_EVENT_TAB_MODIFIED
    GE_BANK_MONEY_UPDATED           = 21,   // SMSG_GUILD_BANK_MONEY_UPDATE
    GE_BANK_MONEY_WITHDRAWN         = 22,   // not exist
    GE_BANK_TEXT_CHANGED            = 23,   // SMSG_GUILD_EVENT_TAB_TEXT_CHANGED
    GE_LEADER_REPLACED              = 24,   // SMSG_GUILD_EVENT_NEW_LEADER
    GE_SIGNED_ON_MOBILE             = 25,   // SMSG_GUILD_EVENT_PRESENCE_CHANGE
    GE_SIGNED_OFF_MOBILE            = 26    // SMSG_GUILD_EVENT_PRESENCE_CHANGE
};

enum PetitionTurns
{
    PETITION_TURN_GUILD_NAME_INVALID    = 0,
    PETITION_TURN_OK                    = 2,
    PETITION_TURN_ALREADY_IN_GUILD      = 3,
    PETITION_TURN_GUILD_PERMISSIONS     = 13,
    PETITION_TURN_NEED_MORE_SIGNATURES  = 14,
};

enum PetitionSigns
{
    PETITION_SIGN_OK                            = 0,
    //PETITION_SIGN_ALREADY_IN_GUILD      = 3,
    //PETITION_SIGN_CANT_SIGN_OWN         = 7,
    //PETITION_SIGN_ALREADY_SIGNED_OTHER  = 8,
    //PETITION_SIGN_FULL                  = 11,
    //PETITION_SIGN_NOT_SERVER            = 12,
    //PETITION_SIGN_RESTRICTED_ACCOUNT    = 13,

    PETITION_SIGN_ALREADY_SIGNED                = 12,
    PETITION_SIGN_ALREADY_INVITED_TO_GUILD_S    = 13, //already invited to enother guild
    PETITION_SIGN_ALREADY_IN_GUILD_S            = 15, //already in guild

};

enum GuildBankRights
{
    GUILD_BANK_RIGHT_VIEW_TAB       = 0x01,
    GUILD_BANK_RIGHT_PUT_ITEM       = 0x02,
    GUILD_BANK_RIGHT_UPDATE_TEXT    = 0x04,

    GUILD_BANK_RIGHT_DEPOSIT_ITEM   = GUILD_BANK_RIGHT_VIEW_TAB | GUILD_BANK_RIGHT_PUT_ITEM,
    GUILD_BANK_RIGHT_FULL           = 0xFF
};

enum GuildBankEventLogTypes
{
    GUILD_BANK_LOG_DEPOSIT_ITEM         = 1,
    GUILD_BANK_LOG_WITHDRAW_ITEM        = 2,
    GUILD_BANK_LOG_MOVE_ITEM            = 3,
    GUILD_BANK_LOG_DEPOSIT_MONEY        = 4,
    GUILD_BANK_LOG_WITHDRAW_MONEY       = 5,
    GUILD_BANK_LOG_REPAIR_MONEY         = 6,
    GUILD_BANK_LOG_MOVE_ITEM2           = 7,
    GUILD_BANK_LOG_WITHDRAW_FOR_TAB     = 8,
    GUILD_BANK_LOG_BUY_SLOT             = 9,
    GUILD_BANK_LOG_CASH_FLOW_DEPOSIT    = 10
};

enum GuildEventLogTypes
{
    GUILD_EVENT_LOG_INVITE_PLAYER     = 1,
    GUILD_EVENT_LOG_JOIN_GUILD        = 2,
    GUILD_EVENT_LOG_PROMOTE_PLAYER    = 3,
    GUILD_EVENT_LOG_DEMOTE_PLAYER     = 4,
    GUILD_EVENT_LOG_UNINVITE_PLAYER   = 5,
    GUILD_EVENT_LOG_LEAVE_GUILD       = 6
};

enum GuildEmblemError
{
    ERR_GUILDEMBLEM_SUCCESS               = 0,
    ERR_GUILDEMBLEM_INVALID_TABARD_COLORS = 1,
    ERR_GUILDEMBLEM_NOGUILD               = 2,
    ERR_GUILDEMBLEM_NOTGUILDMASTER        = 3,
    ERR_GUILDEMBLEM_NOTENOUGHMONEY        = 4,
    ERR_GUILDEMBLEM_INVALIDVENDOR         = 5
};

enum GuildMemberFlags
{
    GUILDMEMBER_STATUS_NONE      = 0x0000,
    GUILDMEMBER_STATUS_ONLINE    = 0x0001,
    GUILDMEMBER_STATUS_AFK       = 0x0002,
    GUILDMEMBER_STATUS_DND       = 0x0004,
    GUILDMEMBER_STATUS_MOBILE    = 0x0008  // remote chat from mobile app
};

enum GuildNews
{
    GUILD_NEWS_GUILD_ACHIEVEMENT    = 0,
    GUILD_NEWS_PLAYER_ACHIEVEMENT   = 1,
    GUILD_NEWS_DUNGEON_ENCOUNTER    = 2,  // @todo Implement
    GUILD_NEWS_ITEM_LOOTED          = 3,
    GUILD_NEWS_ITEM_CRAFTED         = 4,
    GUILD_NEWS_ITEM_PURCHASED       = 5,
    GUILD_NEWS_LEVEL_UP             = 6
};

struct GuildNewsEntry
{
    GuildNews EventType;
    time_t Date;
    ObjectGuid PlayerGuid;
    uint32 Flags;
    uint32 Data;
};

struct GuildReward
{
    uint32 Entry;
    int32 Racemask;
    uint64 Price;
    uint8 Standing;
    std::vector<uint32> AchievementsRequired;
};

uint32 const MinNewsItemLevel[5] = { 61, 90, 200, 353 }; /*@todo: update me for 5.0.5 ?*/

typedef std::map<uint32, GuildNewsEntry> GuildNewsLogMap;

#define GUILD_CHALLENGES_TYPES 6
const uint32 GuildChallengeGoldReward[GUILD_CHALLENGES_TYPES]         = { 0, 250,    1000,    500,   250,   500 };
const uint32 GuildChallengeMaxLevelGoldReward[GUILD_CHALLENGES_TYPES] = { 0, 125,    500,     250,   125,   250 };
const uint32 GuildChallengesMaxCount[GUILD_CHALLENGES_TYPES]          = { 0, 7,      1,       3,     0,     3 };

#define GUILD_WEEKLY_REP_CAP            4375                        // Amount of personal guild rep members can earn per day.

////////////////////////////////////////////////////////////////////////////////////////////
// Emblem info
class EmblemInfo
{
public:
    EmblemInfo() : m_style(0), m_color(0), m_borderStyle(0), m_borderColor(0), m_backgroundColor(0) { }

    void LoadFromDB(Field* fields);
    void SaveToDB(ObjectGuid::LowType guildId) const;
    void ReadPacket(WorldPackets::Guild::SaveGuildEmblem& packet);
    bool ValidateEmblemColors();
    void WritePacket(WorldPacket& data) const;

    uint32 GetStyle() const { return m_style; }
    uint32 GetColor() const { return m_color; }
    uint32 GetBorderStyle() const { return m_borderStyle; }
    uint32 GetBorderColor() const { return m_borderColor; }
    uint32 GetBackgroundColor() const { return m_backgroundColor; }

private:
    uint32 m_style;
    uint32 m_color;
    uint32 m_borderStyle;
    uint32 m_borderColor;
    uint32 m_backgroundColor;
};

// Structure for storing guild bank rights and remaining slots together.
struct GuildBankRightsAndSlots
{
    GuildBankRightsAndSlots() : tabId(TAB_UNDEFINED), rights(0), slots(0) { }
    GuildBankRightsAndSlots(uint8 _tabId) : tabId(_tabId), rights(0), slots(0) { }
    GuildBankRightsAndSlots(uint8 _tabId, uint8 _rights, uint32 _slots) : tabId(_tabId), rights(_rights), slots(_slots) { }

    inline bool IsEqual(GuildBankRightsAndSlots const& rhs) const { return rights == rhs.rights && slots == rhs.slots; }
    void SetGuildMasterValues()
    {
        rights = GUILD_BANK_RIGHT_FULL;
        slots = uint32(GUILD_WITHDRAW_SLOT_UNLIMITED);
    }

    void SetTabId(uint8 _tabId) { tabId = _tabId; }
    void SetSlots(uint32 _slots) { slots = _slots; }
    void SetRights(uint8 _rights) { rights = _rights; }

    int8 GetTabId() const { return tabId; }
    int32 GetSlots() const { return slots; }
    int8 GetRights() const { return rights; }

    uint8  tabId;
    uint32 rights;
    uint32 slots;
};
typedef std::vector <GuildBankRightsAndSlots> GuildBankRightsAndSlotsVec;

typedef std::set <uint8> SlotIds;

#define MAX_GUILD_PROFESSIONS 2
#define KNOW_RECIPES_MASK_SIZE 300
#define MAX_GUILD_NEWS_ENTRIES 100

class Guild
{
private:

    struct KnownRecipes
    {
        KnownRecipes() { Clear(); }

        void Clear() { memset(recipesMask, 0, sizeof(recipesMask)); }
        bool IsEmpty() const;

        void GenerateMask(uint32 skillId, std::set<uint32> const& spells);
        std::string GetMaskForSave() const;
        void LoadFromString(std::string const& str);

        uint8 recipesMask[KNOW_RECIPES_MASK_SIZE];
    };

    // Class representing guild member
    class Member
    {
        friend class Guild;

        struct RemainingValue
        {
            RemainingValue() : value(0), resetTime(0) { }

            uint32 value;
            uint32 resetTime;
        };

        struct ProfessionInfo
        {
            ProfessionInfo(uint32 _skillId, uint32 _skillValue, uint32 _skillRank) :
                skillId(_skillId), skillValue(_skillValue), skillRank(_skillRank) { }

            ProfessionInfo() : skillId(0), skillValue(0), skillRank(0) { }
            uint32 skillId;
            uint32 skillValue;
            uint32 skillRank;

            void GenerateRecipesMask(std::set<uint32> const& spells) { knownRecipes.GenerateMask(skillId, spells); }

            KnownRecipes knownRecipes;
        };

    public:
        Member(ObjectGuid::LowType const& guildId, ObjectGuid guid, uint32 rankId) :
            m_guildId(guildId),
            m_guid(guid),
            m_zoneId(0),
            m_level(0),
            m_class(0),
            m_gender(GENDER_MALE),
            m_flags(GUILDMEMBER_STATUS_NONE),
            m_logoutTime(::time(nullptr)),
            m_accountId(0),
            m_rankId(rankId),
            m_achievementPoints(0),
            m_totalReputation(0)
        {
            memset(m_bankWithdraw, 0, (GUILD_BANK_MAX_TABS + 1) * sizeof(int32));
        }
        void SetStats(Player* player);
        void SaveStatsToDB(SQLTransaction* trans);
        void SetStats(std::string const& name, uint8 level, uint8 _class, uint32 zoneId, uint32 accountId, uint32 reputation, uint8 gender, uint32 achPoints,
            uint32 profId1, uint32 profValue1, uint8 profRank1, std::string const& recipesMask1, uint32 profId2, uint32 profValue2, uint8 profRank2, std::string const& recipesMask2);
        bool CheckStats() const;

        void SetPublicNote(std::string const& publicNote);
        void SetOfficerNote(std::string const& officerNote);

        std::string GetPublicNote() { return m_publicNote; };
        std::string GetOfficerNote() { return m_officerNote; };

        void SetZoneId(uint32 id) { m_zoneId = id; }
        void SetAchievementPoints(uint32 val) { m_achievementPoints = val; }
        void SetLevel(uint8 var) { m_level = var; }

        void AddFlag(uint8 var) { m_flags |= var; }
        void RemFlag(uint8 var) { m_flags &= ~var; }
        void ResetFlags() { m_flags = GUILDMEMBER_STATUS_NONE; }

        bool LoadFromDB(Field* fields);
        void SaveToDB(SQLTransaction& trans) const;

        ObjectGuid GetGUID() const { return m_guid; }
        std::string const& GetName() const { return m_name; }
        uint32 GetAccountId() const { return m_accountId; }
        uint32 GetRankId() const { return m_rankId; }
        uint64 GetLogoutTime() const { return m_logoutTime; }
        std::string GetPublicNote() const { return m_publicNote; }
        std::string GetOfficerNote() const { return m_officerNote; }
        uint8 GetClass() const { return m_class; }
        uint8 GetLevel() const { return m_level; }
        uint8 GetGender() const { return m_gender; }
        uint8 GetFlags() const { return m_flags; }
        uint32 GetZoneId() const { return m_zoneId; }
        uint32 GetAchievementPoints() const { return m_achievementPoints; }
        uint32 GetTotalReputation() const { return m_totalReputation; }
        ProfessionInfo const& GetProfessionInfo(uint32 index) const { return m_professionInfo[index]; }

        bool IsOnline() { return (m_flags & GUILDMEMBER_STATUS_ONLINE); }

        void ChangeRank(uint8 newRank);

        inline void UpdateLogoutTime() { m_logoutTime = ::time(nullptr); }
        inline bool IsRank(uint8 rankId) const { return m_rankId == rankId; }
        inline bool IsRankNotLower(uint8 rankId) const { return m_rankId <= rankId; }
        inline bool IsSamePlayer(ObjectGuid guid) const { return m_guid == guid; }

        void UpdateBankWithdrawValue(SQLTransaction& trans, uint8 tabId, uint32 amount);
        int32 GetBankWithdrawValue(uint8 tabId) const;
        void SendGuildReputationWeeklyCap(WorldSession* session, uint32 reputation) const;
        void ResetValues();

        void DecreaseBankRemainingValue(SQLTransaction& trans, uint8 tabId, uint32 amount);
        uint32 GetBankRemainingValue(uint8 tabId, const Guild* guild) const;

        void ResetTabTimes();
        void ResetMoneyTime();

        inline Player* FindPlayer() const { return ObjectAccessor::FindPlayer(m_guid); }

    private:
        ObjectGuid::LowType m_guildId;
        // Fields from characters table
        ObjectGuid m_guid;
        std::string m_name;
        uint32 m_zoneId;
        uint8 m_level;
        uint8 m_class;
        uint8 m_gender;
        uint8 m_flags;
        uint64 m_logoutTime;
        uint32 m_accountId;
        // Fields from guild_member table
        uint32 m_rankId;
        std::string m_publicNote;
        std::string m_officerNote;

        RemainingValue m_bankRemaining[GUILD_BANK_MAX_TABS + 1];
        int32 m_bankWithdraw[GUILD_BANK_MAX_TABS + 1];
        uint32 m_achievementPoints;
        uint32 m_totalReputation;
        ProfessionInfo m_professionInfo[MAX_GUILD_PROFESSIONS];
    };

    // News Log class
    class GuildNewsLog
    {
    public:
        GuildNewsLog(Guild* guild) : _guild(guild) { }

        void LoadFromDB(PreparedQueryResult result);
        void BuildNewsData(WorldPacket& data);
        void BuildNewsData(uint32 id, GuildNewsEntry& guildNew, WorldPacket& data);
        void AddNewEvent(GuildNews eventType, time_t date, ObjectGuid playerGuid, uint32 flags, uint32 data);
        GuildNewsEntry* GetNewsById(uint32 id)
        {
            GuildNewsLogMap::iterator itr = _newsLog.find(id);
            if (itr != _newsLog.end())
                return &itr->second;
            return nullptr;
        }

        Guild* GetGuild() const { return _guild; }
        void Shrink();
        uint32 GetNewId() const;
        void WritePacket(WorldPackets::Guild::GuildNews& newsPacket) const;

    private:
        Guild* _guild;
        GuildNewsLogMap _newsLog;
    };

    // Base class for event entries
    class LogEntry
    {
    public:
        LogEntry(ObjectGuid::LowType guildId, uint32 guid) : m_guildId(guildId), m_guid(guid), m_timestamp(::time(nullptr)) { }
        LogEntry(ObjectGuid::LowType guildId, uint32 guid, time_t timestamp) : m_guildId(guildId), m_guid(guid), m_timestamp(timestamp) { }
        virtual ~LogEntry() { }

        uint32 GetGUID() const { return m_guid; }
        uint64 GetTimestamp() const { return m_timestamp; }

        virtual void SaveToDB(SQLTransaction& trans) const = 0;

    protected:
        ObjectGuid::LowType m_guildId;
        uint32 m_guid;
        uint64 m_timestamp;
    };

    // Event log entry
    class EventLogEntry : public LogEntry
    {
    public:
        EventLogEntry(ObjectGuid::LowType guildId, uint32 guid, GuildEventLogTypes eventType, ObjectGuid::LowType playerGuid1, ObjectGuid::LowType playerGuid2, uint8 newRank) :
            LogEntry(guildId, guid), m_eventType(eventType), m_playerGuid1(playerGuid1), m_playerGuid2(playerGuid2), m_newRank(newRank) { }

        EventLogEntry(ObjectGuid::LowType guildId, uint32 guid, time_t timestamp, GuildEventLogTypes eventType, ObjectGuid::LowType playerGuid1, ObjectGuid::LowType playerGuid2, uint8 newRank) :
            LogEntry(guildId, guid, timestamp), m_eventType(eventType), m_playerGuid1(playerGuid1), m_playerGuid2(playerGuid2), m_newRank(newRank) { }

        ~EventLogEntry() { }

        void SaveToDB(SQLTransaction& trans) const;
        void WritePacket(WorldPackets::Guild::GuildEventLogQueryResults& packet) const;

    private:
        GuildEventLogTypes m_eventType;
        ObjectGuid::LowType m_playerGuid1;
        ObjectGuid::LowType m_playerGuid2;
        uint8  m_newRank;
    };

    // Bank event log entry
    class BankEventLogEntry : public LogEntry
    {
    public:
        static bool IsMoneyEvent(GuildBankEventLogTypes eventType)
        {
            return
                eventType == GUILD_BANK_LOG_DEPOSIT_MONEY ||
                eventType == GUILD_BANK_LOG_WITHDRAW_MONEY ||
                eventType == GUILD_BANK_LOG_REPAIR_MONEY ||
                eventType == GUILD_BANK_LOG_CASH_FLOW_DEPOSIT;
        }

        bool IsMoneyEvent() const
        {
            return IsMoneyEvent(m_eventType);
        }

        BankEventLogEntry(ObjectGuid::LowType guildId, uint32 guid, GuildBankEventLogTypes eventType, uint8 tabId, ObjectGuid::LowType playerGuid, uint64 itemOrMoney, uint16 itemStackCount, uint8 destTabId) :
            LogEntry(guildId, guid), m_eventType(eventType), m_bankTabId(tabId), m_playerGuid(playerGuid),
            m_itemOrMoney(itemOrMoney), m_itemStackCount(itemStackCount), m_destTabId(destTabId) { }

        BankEventLogEntry(ObjectGuid::LowType guildId, uint32 guid, time_t timestamp, uint8 tabId, GuildBankEventLogTypes eventType, ObjectGuid::LowType playerGuid, uint64 itemOrMoney, uint16 itemStackCount, uint8 destTabId) :
            LogEntry(guildId, guid, timestamp), m_eventType(eventType), m_bankTabId(tabId), m_playerGuid(playerGuid),
            m_itemOrMoney(itemOrMoney), m_itemStackCount(itemStackCount), m_destTabId(destTabId) { }

        ~BankEventLogEntry() { }

        void SaveToDB(SQLTransaction& trans) const;
        void WritePacket(WorldPackets::Guild::GuildBankLogQueryResults& packet) const;

    private:
        GuildBankEventLogTypes m_eventType;
        uint8  m_bankTabId;
        ObjectGuid::LowType m_playerGuid;
        uint64 m_itemOrMoney;
        uint16 m_itemStackCount;
        uint8  m_destTabId;
    };

    // News log entry
    class NewsLogEntry : public LogEntry
    {
    public:
        NewsLogEntry(ObjectGuid::LowType guildId, uint32 guid, GuildNews type, ObjectGuid playerGuid, uint32 flags, uint32 value) :
            LogEntry(guildId, guid), m_type(type), m_playerGuid(playerGuid), m_flags(flags), m_value(value) { }

        NewsLogEntry(ObjectGuid::LowType guildId, uint32 guid, time_t timestamp, GuildNews type, ObjectGuid playerGuid, uint32 flags, uint32 value) :
            LogEntry(guildId, guid, timestamp), m_type(type), m_playerGuid(playerGuid), m_flags(flags), m_value(value) { }

        ~NewsLogEntry() { }

        GuildNews GetType() const { return m_type; }
        ObjectGuid GetPlayerGuid() const { return m_playerGuid; }
        uint32 GetValue() const { return m_value; }
        uint32 GetFlags() const { return m_flags; }
        void SetSticky(bool sticky)
        {
            if (sticky)
                m_flags |= 1;
            else
                m_flags &= ~1;
        }

        void SaveToDB(SQLTransaction& trans) const;
        void WritePacket(WorldPackets::Guild::GuildNews& newsPacket) const;

    private:
        GuildNews m_type;
        ObjectGuid m_playerGuid;
        uint32 m_flags;
        uint32 m_value;
    };

    // Class encapsulating work with events collection
    typedef std::list<LogEntry*> GuildLog;

    class LogHolder
    {
    public:
        LogHolder(uint32 maxRecords) : m_maxRecords(maxRecords), m_nextGUID(uint32(GUILD_EVENT_LOG_GUID_UNDEFINED)) { }
        ~LogHolder();

        uint8 GetSize() const { return uint8(m_log.size()); }
        // Checks if new log entry can be added to holder when loading from DB
        inline bool CanInsert() const { return m_log.size() < m_maxRecords; }
        // Adds event from DB to collection
        void LoadEvent(LogEntry* entry);
        // Adds new event to collection and saves it to DB
        void AddEvent(SQLTransaction& trans, LogEntry* entry);
        uint32 GetNextGUID();
        GuildLog* GetGuildLog() { return &m_log; }

    private:
        GuildLog m_log;
        uint32 m_maxRecords;
        uint32 m_nextGUID;
    };

    // Class encapsulating guild rank data
    class RankInfo
    {
    public:
        RankInfo(ObjectGuid::LowType guildId) : m_guildId(guildId), m_rankId(GUILD_RANK_NONE), m_rights(GR_RIGHT_EMPTY), m_bankMoneyPerDay(0) { }
        RankInfo(ObjectGuid::LowType guildId, uint32 rankId, const std::string& name, uint32 rights, uint64 money) :
            m_guildId(guildId), m_rankId(rankId), m_name(name), m_rights(rights), m_bankMoneyPerDay(money) { }

        void LoadFromDB(Field* fields);
        void SaveToDB(SQLTransaction& trans) const;

        uint32 GetId() const { return m_rankId; }
        void SetId(uint32 id) { m_rankId = id; }
        void UpdateId(uint32 newId);

        std::string GetName() const { return m_name; }
        void SetName(const std::string& name);

        uint32 GetRights() const { return m_rights; }
        void SetRights(uint32 rights);

        bool operator < (const RankInfo& rank) const { return m_rights > rank.GetRights(); }
        bool operator == (const RankInfo& rank) const { return m_rights == rank.GetRights(); }

        uint64 GetBankMoneyPerDay() const { return m_rankId == GR_GUILDMASTER ? GUILD_WITHDRAW_MONEY_UNLIMITED : m_bankMoneyPerDay; }
        void SetBankMoneyPerDay(uint32 money);

        inline uint32 GetBankTabRights(uint8 tabId) const { return tabId < GUILD_BANK_MAX_TABS ? m_bankTabRightsAndSlots[tabId].rights : 0; }
        inline uint32 GetBankTabSlotsPerDay(uint8 tabId) const
        {
            if (tabId < GUILD_BANK_MAX_TABS)
                return m_rankId == GR_GUILDMASTER ? GUILD_WITHDRAW_SLOT_UNLIMITED : m_bankTabRightsAndSlots[tabId].slots;
            return 0;
        }
        void SetBankTabSlotsAndRights(GuildBankRightsAndSlots rightsAndSlots, bool saveToDB);
        void CreateMissingTabsIfNeeded(uint8 ranks, SQLTransaction& trans, bool logOnCreate = false);

    private:
        ObjectGuid::LowType m_guildId;
        uint32 m_rankId;
        std::string m_name;
        uint32 m_rights;
        uint64 m_bankMoneyPerDay;
        GuildBankRightsAndSlots m_bankTabRightsAndSlots[GUILD_BANK_MAX_TABS];
    };

    class BankTab
    {
    public:
        BankTab(ObjectGuid::LowType const& guildId, uint8 tabId) : m_guildId(guildId), m_tabId(tabId)
        {
            memset(m_items, 0, GUILD_BANK_MAX_SLOTS * sizeof(Item*));
        }

        bool LoadFromDB(Field* fields);
        bool LoadItemFromDB(Field* fields);
        void Delete(SQLTransaction& trans, bool removeItemsFromDB = false);

        void SetInfo(std::string const& name, std::string const& icon);
        void SetText(std::string const& text);
        void SendText(Guild const* guild, WorldSession* session) const;

        std::string const& GetName() const { return m_name; }
        std::string const& GetIcon() const { return m_icon; }
        std::string const& GetText() const { return m_text; }

        inline Item* GetItem(uint8 slotId) const { return slotId < GUILD_BANK_MAX_SLOTS ? m_items[slotId] : nullptr; }
        bool SetItem(SQLTransaction& trans, uint8 slotId, Item* item);

    private:
        ObjectGuid::LowType m_guildId;
        uint8 m_tabId;

        Item* m_items[GUILD_BANK_MAX_SLOTS];
        std::string m_name;
        std::string m_icon;
        std::string m_text;
    };

    // Movement data
    class MoveItemData
    {
    public:
        MoveItemData(Guild* guild, Player* player, uint8 container, uint8 slotId) : m_pGuild(guild), m_pPlayer(player),
            m_container(container), m_slotId(slotId), m_pItem(nullptr), m_pClonedItem(nullptr) { }
        virtual ~MoveItemData() { }

        virtual bool IsBank() const = 0;
        // Initializes item pointer. Returns true, if item exists, false otherwise.
        virtual bool InitItem() = 0;
        // Checks splited amount against item. Splited amount cannot be more that number of items in stack.
        virtual bool CheckItem(uint32& splitedAmount);
        // Defines if player has rights to save item in container
        virtual bool HasStoreRights(MoveItemData* /*pOther*/) const { return true; }
        // Defines if player has rights to withdraw item from container
        virtual bool HasWithdrawRights(MoveItemData* /*pOther*/) const { return true; }
        // Checks if container can store specified item
        bool CanStore(Item* pItem, bool swap, bool sendError);
        // Clones stored item
        bool CloneItem(uint32 count);
        // Remove item from container (if splited update items fields)
        virtual void RemoveItem(SQLTransaction& trans, MoveItemData* pOther, uint32 splitedAmount = 0) = 0;
        // Saves item to container
        virtual Item* StoreItem(SQLTransaction& trans, Item* pItem) = 0;
        // Log bank event
        virtual void LogBankEvent(SQLTransaction& trans, MoveItemData* pFrom, uint32 count) const = 0;
        // Log GM action
        virtual void LogAction(MoveItemData* pFrom) const;
        // Copy slots id from position vector
        void CopySlots(SlotIds& ids) const;

        Item* GetItem(bool isCloned = false) const { return isCloned ? m_pClonedItem : m_pItem; }
        uint8 GetContainer() const { return m_container; }
        uint8 GetSlotId() const { return m_slotId; }
    protected:
        virtual InventoryResult CanStore(Item* pItem, bool swap) = 0;

        Guild* m_pGuild;
        Player* m_pPlayer;
        uint8 m_container;
        uint8 m_slotId;
        Item* m_pItem;
        Item* m_pClonedItem;
        ItemPosCountVec m_vec;
    };

    class PlayerMoveItemData : public MoveItemData
    {
    public:
        PlayerMoveItemData(Guild* guild, Player* player, uint8 container, uint8 slotId) :
            MoveItemData(guild, player, container, slotId) { }

        bool IsBank() const { return false; }
        bool InitItem();
        void RemoveItem(SQLTransaction& trans, MoveItemData* pOther, uint32 splitedAmount = 0);
        Item* StoreItem(SQLTransaction& trans, Item* pItem);
        void LogBankEvent(SQLTransaction& trans, MoveItemData* pFrom, uint32 count) const;
    protected:
        InventoryResult CanStore(Item* pItem, bool swap);
    };

    class BankMoveItemData : public MoveItemData
    {
    public:
        BankMoveItemData(Guild* guild, Player* player, uint8 container, uint8 slotId) :
            MoveItemData(guild, player, container, slotId) { }

        bool IsBank() const { return true; }
        bool InitItem();
        bool HasStoreRights(MoveItemData* pOther) const;
        bool HasWithdrawRights(MoveItemData* pOther) const;
        void RemoveItem(SQLTransaction& trans, MoveItemData* pOther, uint32 splitedAmount);
        Item* StoreItem(SQLTransaction& trans, Item* pItem);
        void LogBankEvent(SQLTransaction& trans, MoveItemData* pFrom, uint32 count) const;
        void LogAction(MoveItemData* pFrom) const;

    protected:
        InventoryResult CanStore(Item* pItem, bool swap);

    private:
        Item* _StoreItem(SQLTransaction& trans, BankTab* pTab, Item* pItem, ItemPosCount& pos, bool clone) const;
        bool _ReserveSpace(uint8 slotId, Item* pItem, Item* pItemDest, uint32& count);
        void CanStoreItemInTab(Item* pItem, uint8 skipSlotId, bool merge, uint32& count);
    };

    typedef std::unordered_map<ObjectGuid, Member*> Members;
    typedef std::vector<RankInfo> Ranks;
    typedef std::vector<BankTab*> BankTabs;

public:
    typedef std::unordered_map<uint32, KnownRecipes> KnownRecipesMap;

    static void SendCommandResult(WorldSession* session, GuildCommandType type, GuildCommandError errCode, const std::string& param = "");
    static void SendSaveEmblemResult(WorldSession* session, GuildEmblemError errCode);

    Guild();
    ~Guild();

    bool Create(Player* pLeader, const std::string& name);
    void Disband();

    void SaveToDB(bool withMembers);

    // Getters
    ObjectGuid::LowType GetId() const { return m_id; }
    ObjectGuid GetGUID() const { return ObjectGuid::Create<HighGuid::Guild>(m_id); }
    ObjectGuid GetLeaderGUID() const { return m_leaderGuid; }
    const std::string& GetName() const { return m_name; }
    const std::string& GetMOTD() const { return m_motd; }
    const std::string& GetInfo() const { return m_info; }
    uint32 GetMemberCount() const { return uint32(m_members.size()); }
    time_t GetCreatedDate() const { return m_createdDate; }
    uint64 GetBankMoney() const { return m_bankMoney; }

    // Handle client commands
    void SendRoster(WorldSession* session = nullptr);          // NULL = broadcast
    void SendQueryResponse(WorldSession* session);
    void SendGuildRankInfo(WorldSession* session) const;

    void HandleSetMOTD(WorldSession* session, const std::string& motd);
    void HandleSetInfo(WorldSession* session, const std::string& info);
    void HandleSetEmblem(WorldSession* session, const EmblemInfo& emblemInfo);
    void HandleSetNewGuildMaster(WorldSession* session, std::string const& name);
    void HandleSetBankTabInfo(WorldSession* session, uint8 tabId, const std::string& name, const std::string& icon);
    void HandleSetMemberNote(WorldSession* session, std::string const& note, ObjectGuid guid, bool isPublic);
    void HandleSetRankInfo(WorldSession* session, uint32 rankId, const std::string& name, uint32 rights, uint32 moneyPerDay, GuildBankRightsAndSlotsVec rightsAndSlots);
    void HandleBuyBankTab(WorldSession* session, uint8 tabId);
    void HandleSpellEffectBuyBankTab(WorldSession* session, uint8 tabId);
    void HandleInviteMember(WorldSession* session, const std::string& name);
    void HandleAcceptMember(WorldSession* session);
    void HandleLeaveMember(WorldSession* session);
    void HandleRemoveMember(WorldSession* session, ObjectGuid guid);
    void HandleUpdateMemberRank(WorldSession* session, ObjectGuid targetGuid, bool promote);
    void HandleSetMemberRank(WorldSession* session, ObjectGuid targetGuid, ObjectGuid setterGuid, uint32 rank);
    void HandleAddNewRank(WorldSession* session, const std::string& name);
    void HandleRemoveRank(WorldSession* session, uint32 rankId);
    void HandleChangeNameRank(WorldSession* session, uint32 id, std::string const& name);
    void HandleShiftRank(WorldSession* session, uint32 id, bool up);
    void HandleMemberDepositMoney(WorldSession* session, uint64 amount, bool cashFlow = false);
    bool HandleMemberWithdrawMoney(WorldSession* session, uint64 amount, bool repair = false);
    void HandleMemberLogout(WorldSession* session);
    void HandleDisband(WorldSession* session);
    void HandleGuildPartyRequest(WorldSession* session);
    void HandleNewsSetSticky(WorldSession* session, uint32 newsId, bool sticky);

    // Send info to client
    void SendEventLog(WorldSession* session) const;
    void SendNewsUpdate(WorldSession* session);
    void SendBankLog(WorldSession* session, uint8 tabId) const;
    void SendBankList(WorldSession* session, uint8 tabId, bool fullUpdate) const;
    void SendBankTabText(WorldSession* session, uint8 tabId) const;
    void SendPermissions(WorldSession* session) const;
    void SendMoneyInfo(WorldSession* session) const;
    void SendLoginInfo(WorldSession* session);
    void SendGuildReputationWeeklyCap(WorldSession* session) const;
    void SendGuildXP(WorldSession* session) const;
    void SendGuildChallengesInfo(WorldSession* session) const;

    // send alert frame after complete some type of guild challenge, actually send after finish dungeon, kill raid boss, win rated BG and others...
    void SendGuildChallengeComplete(WorldSession* session) const;

    // Load from DB
    bool LoadFromDB(Field* fields);
    void LoadGuildNewsLogFromDB(Field* fields);
    void LoadRankFromDB(Field* fields);
    bool LoadMemberFromDB(Field* fields);
    bool LoadEventLogFromDB(Field* fields);
    void LoadBankRightFromDB(Field* fields);
    bool LoadBankTabFromDB(Field* fields);
    bool LoadBankEventLogFromDB(Field* fields);
    bool LoadBankItemFromDB(Field* fields);
    bool Validate();

    // Broadcasts
    void BroadcastToGuild(WorldSession* session, bool officerOnly, const std::string& msg, uint32 language = LANG_UNIVERSAL) const;
    void BroadcastAddonToGuild(WorldSession* session, bool officerOnly, const std::string& msg, const std::string& prefix) const;
    void BroadcastPacketToRank(WorldPacket const* packet, uint8 rankId) const;
    void BroadcastPacket(WorldPacket const* packet) const;

    void MassInviteToEvent(WorldSession* session, uint32 minLevel, uint32 maxLevel, uint32 minRank);

    template<class Do>
    void BroadcastWorker(Do& _do, Player* except = nullptr)
    {
        for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (Player* player = itr->second->FindPlayer())
                if (player != except)
                    _do(player);
    }

    // Members
    // Adds member to guild. If rankId == GUILD_RANK_NONE, lowest rank is assigned.
    bool AddMember(ObjectGuid guid, uint8 rankId = GUILD_RANK_NONE);
    void DeleteMember(ObjectGuid guid, bool isDisbanding = false, bool isKicked = false);
    bool ChangeMemberRank(ObjectGuid guid, uint8 newRank);
    bool IsMember(ObjectGuid guid);
    uint32 GetMembersCount() { return m_members.size(); }

    // Bank
    void SwapItems(Player* player, uint8 tabId, uint8 slotId, uint8 destTabId, uint8 destSlotId, uint32 splitedAmount);
    void SwapItemsWithInventory(Player* player, bool toChar, uint8 tabId, uint8 slotId, uint8 playerBag, uint8 playerSlotId, uint32 splitedAmount);

    // Bank tabs
    void SetBankTabText(uint8 tabId, const std::string& text);

    AchievementMgr<Guild>& GetAchievementMgr() { return m_achievementMgr; }
    AchievementMgr<Guild> const& GetAchievementMgr() const { return m_achievementMgr; }

     // Guild leveling
    void ResetWeek();
    void RewardReputation(Player* player, uint32 amount);
    uint8 GetLevel() const { return _level; }

    void AddMemberOnline() { m_members_online++; }
    void RemoveMemberOnline() { if (m_members_online > 0) m_members_online--; }
    uint32 GetMembersOnline() const { return m_members_online; }

    EmblemInfo const& GetEmblemInfo() const { return m_emblemInfo; }

    inline uint8 GetPurchasedTabsSize() const { return uint8(m_bankTabs.size()); }

    void AddGuildNews(uint8 type, ObjectGuid guid, uint32 flags, uint32 value);

    // events
    void SendGuildEventBankContentsChanged();
    void SendEventBankMoneyChanged();
    void SendGuildEventDisbanded();
    void SendGuildEventMOTD(WorldSession* session = nullptr);
    void SendGuildEventNewLeader(Member* newLeader, Member* oldLeader, bool isSelfPromoted = false);
    void SendGuildEventPlayerJoined(ObjectGuid const& guid, std::string name);
    void SendGuildEventPlayerLeft(Member* leaver, Member* remover = nullptr, bool isRemoved = false);
    void SendGuildEventPresenceChanged(ObjectGuid const& guid, std::string name, bool online, WorldSession* session = nullptr);
    void SendGuildEventRankChanged(uint32 rankId);
    void SendGuildEventRanksUpdated();
    void SendGuildEventTabAdded();
    void SendGuildEventTabModified(uint8 tabId, std::string name, std::string icon);
    void SendGuildEventTabTextChanged(uint32 tabId);

    KnownRecipesMap const& GetGuildRecipes() const { return _guildRecipes; }
    KnownRecipes& GetGuildRecipes(uint32 skillId) { return _guildRecipes[skillId]; }
    void UpdateGuildRecipes(uint32 skillId = 0);
    void SendGuildMembersForRecipeResponse(WorldSession* session, uint32 skillId, uint32 spellId);
    void SendGuildMemberRecipesResponse(WorldSession* session, ObjectGuid playerGuid, uint32 skillId);

    bool CanUpdateCriteria(uint32 criteriaTreeId, uint32 recursTree = 0) const { return true; }

protected:
    ObjectGuid::LowType m_id;
    std::string m_name;
    ObjectGuid m_leaderGuid;
    std::string m_motd;
    std::string m_info;
    time_t m_createdDate;
    time_t m_lastSave;

    EmblemInfo m_emblemInfo;
    uint32 m_accountsNumber;
    uint64 m_bankMoney;
    uint32 m_members_online;

    Ranks m_ranks;
    Members m_members;
    BankTabs m_bankTabs;

    // These are actually ordered lists. The first element is the oldest entry.
    LogHolder* m_eventLog;
    LogHolder* m_newsLog;
    LogHolder* m_bankEventLog[GUILD_BANK_MAX_TABS + 1];
    AchievementMgr<Guild> m_achievementMgr;

    uint32 _level;
    KnownRecipesMap _guildRecipes;

private:
    inline uint32 _GetRanksSize() const { return uint32(m_ranks.size()); }
    inline const RankInfo* GetRankInfo(uint32 rankId) const { return rankId < _GetRanksSize() ? &m_ranks[rankId] : nullptr; }
    inline RankInfo* GetRankInfo(uint32 rankId) { return rankId < _GetRanksSize() ? &m_ranks[rankId] : nullptr; }
    inline bool _HasRankRight(Player* player, uint32 right) const { return (_GetRankRights(player->GetRank()) & right) != GR_RIGHT_EMPTY; }
    inline uint32 _GetLowestRankId() const { return uint32(m_ranks.size() - 1); }

    inline BankTab* GetBankTab(uint8 tabId) { return tabId < m_bankTabs.size() ? m_bankTabs[tabId] : nullptr; }
    inline const BankTab* GetBankTab(uint8 tabId) const { return tabId < m_bankTabs.size() ? m_bankTabs[tabId] : nullptr; }

    inline const Member* GetMember(ObjectGuid guid) const
    {
        Members::const_iterator itr = m_members.find(guid);
        return itr != m_members.end() ? itr->second : nullptr;
    }
    inline Member* GetMember(ObjectGuid guid)
    {
        Members::iterator itr = m_members.find(guid);
        return itr != m_members.end() ? itr->second : nullptr;
    }
    inline Member* GetMember(WorldSession* session, const std::string& name)
    {
        for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (itr->second->GetName() == name)
                return itr->second;

        SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD_S, name);
        return nullptr;
    }
    inline void _DeleteMemberFromDB(ObjectGuid::LowType const& lowguid) const
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_MEMBER);
        stmt->setUInt64(0, lowguid);
        CharacterDatabase.Execute(stmt);
    }

    // Creates log holders (either when loading or when creating guild)
    void _CreateLogHolders();
    // Tries to create new bank tab
    bool _CreateNewBankTab();
    // Creates default guild ranks with names in given locale
    void _CreateDefaultGuildRanks(LocaleConstant loc);
    // Creates new rank
    void _CreateRank(const std::string& name, uint32 rights);
    // Update account number when member added/removed from guild
    void _UpdateAccountsNumber();
    bool _IsLeader(Player* player) const;
    void _DeleteBankItems(SQLTransaction& trans, bool removeItemsFromDB = false);
    bool _ModifyBankMoney(SQLTransaction& trans, uint64 amount, bool add);
    void _SetLeaderGUID(Member* pLeader);

    void _SetRankBankMoneyPerDay(uint32 rankId, uint32 moneyPerDay);
    void _SetRankBankTabRightsAndSlots(uint8 rankId, GuildBankRightsAndSlots rightsAndSlots, bool saveToDB = true);
    uint32 _GetRankBankTabRights(uint32 rankId, uint8 tabId) const;
    uint32 _GetRankRights(uint32 rankId) const;
    uint32 _GetRankBankMoneyPerDay(uint32 rankId) const;
    uint32 _GetRankBankTabSlotsPerDay(uint32 rankId, uint8 tabId) const;
    std::string _GetRankName(uint32 rankId) const;

    int32 _GetMemberRemainingSlots(ObjectGuid guid, uint8 tabId) const;
    int32 _GetMemberRemainingMoney(ObjectGuid guid) const;
    void _DecreaseMemberRemainingSlots(SQLTransaction& trans, ObjectGuid guid, uint8 tabId);
    bool _MemberHasTabRights(ObjectGuid guid, uint8 tabId, uint32 rights) const;

    void _LogEvent(GuildEventLogTypes eventType, ObjectGuid::LowType playerGuid1, ObjectGuid::LowType playerGuid2 = UI64LIT(0), uint8 newRank = 0);
    void _LogBankEvent(SQLTransaction& trans, GuildBankEventLogTypes eventType, uint8 tabId, ObjectGuid::LowType playerGuid, uint32 itemOrMoney, uint16 itemStackCount = 0, uint8 destTabId = 0);

    Item* _GetItem(uint8 tabId, uint8 slotId) const;
    void _RemoveItem(SQLTransaction& trans, uint8 tabId, uint8 slotId);
    void _MoveItems(MoveItemData* pSrc, MoveItemData* pDest, uint32 splitedAmount);
    bool _DoItemsMove(MoveItemData* pSrc, MoveItemData* pDest, bool sendError, uint32 splitedAmount = 0);

    void _SendBankContentUpdate(MoveItemData* pSrc, MoveItemData* pDest) const;
    void _SendBankContentUpdate(uint8 tabId, SlotIds slots) const;

    void SendGuildRanksUpdate(ObjectGuid setterGuid, ObjectGuid targetGuid, uint32 rank, bool promote);
};
#endif

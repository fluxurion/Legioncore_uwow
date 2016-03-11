/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __TRINITY_GUILDFINDER_H
#define __TRINITY_GUILDFINDER_H

#include "Common.h"
#include "World.h"
#include "GuildMgr.h"
#include "ObjectGuid.h"

enum GuildFinderPlayStyle
{
    PLAY_STYLE_QUESTING      = 0x01,
    PLAY_STYLE_DUNGEONS      = 0x02,
    PLAY_STYLE_RAIDS         = 0x04,
    PLAY_STYLE_PVP           = 0x08,
    PLAY_STYLE_ROLE_PLAYING  = 0x10,

    ALL_PLAY_STYLES           = PLAY_STYLE_QUESTING | PLAY_STYLE_DUNGEONS | PLAY_STYLE_RAIDS | PLAY_STYLE_PVP | PLAY_STYLE_ROLE_PLAYING
};

enum GuildFinderOptionsAvailability
{
    AVAILABILITY_WEEKDAYS     = 0x1,
    AVAILABILITY_WEEKENDS     = 0x2,
    ALL_WEEK                  = AVAILABILITY_WEEKDAYS | AVAILABILITY_WEEKENDS
};

enum GuildFinderOptionsRoles
{
    GUILDFINDER_ROLE_TANK        = 0x1,
    GUILDFINDER_ROLE_HEALER      = 0x2,
    GUILDFINDER_ROLE_DPS         = 0x4,
    GUILDFINDER_ALL_ROLES        = GUILDFINDER_ROLE_TANK | GUILDFINDER_ROLE_HEALER | GUILDFINDER_ROLE_DPS
};

enum GuildFinderOptionsLevel
{
    ANY_FINDER_LEVEL   = 0x1,
    MAX_FINDER_LEVEL   = 0x2,
    ALL_GUILDFINDER_LEVELS = ANY_FINDER_LEVEL | MAX_FINDER_LEVEL
};

/// Holds all required informations about a membership request
struct MembershipRequest
{
    public:
        MembershipRequest(MembershipRequest const& settings) : _comment(settings.GetComment())
        {
            _availability = settings.GetAvailability();
            _classRoles = settings.GetClassRoles();
            _playStyle = settings.GetPlayStyle();
            _guildId = settings.GetGuildGuid();
            _playerGUID = settings.GetPlayerGUID();
            _time = settings.GetSubmitTime();
        }

        MembershipRequest(ObjectGuid const& playerGUID, ObjectGuid const& guildId, uint32 availability, uint32 classRoles, uint32 playStyle, std::string& comment, time_t submitTime) :
            _playerGUID(playerGUID), _guildId(guildId), _availability(availability), _classRoles(classRoles),
            _playStyle(playStyle), _time(submitTime), _comment(comment) {}
        
        MembershipRequest() : _playerGUID(), _guildId(), _availability(0), _classRoles(0),
            _playStyle(0), _time(time(NULL)) {}

        ObjectGuid GetGuildGuid() const      { return _guildId; }
        ObjectGuid GetPlayerGUID() const   { return _playerGUID; }
        uint8 GetAvailability() const  { return _availability; }
        uint8 GetClassRoles() const    { return _classRoles; }
        uint8 GetPlayStyle() const     { return _playStyle; }

        uint8 GetClass() const
        {
            const CharacterInfo *nameData = sWorld->GetCharacterInfo(_playerGUID);
            return nameData ? nameData->Class : 0;
        }

        uint8 GetLevel() const
        {
            const CharacterInfo *nameData = sWorld->GetCharacterInfo(_playerGUID);
            return nameData ? nameData->Level : 1;
        }

        time_t GetSubmitTime() const   { return _time; }
        time_t GetExpiryTime() const   { return time_t(_time + 30 * 24 * 3600); } // Adding 30 days
        std::string const& GetComment() const { return _comment; }
        std::string const& GetName() const
        {
            const CharacterInfo *nameData = sWorld->GetCharacterInfo(_playerGUID);
            std::string name = "";
            return nameData ? nameData->Name : name;
        }

    private:
        std::string _comment;

        ObjectGuid _guildId;
        ObjectGuid _playerGUID;

        uint8 _availability;
        uint8 _classRoles;
        uint8 _playStyle;

        time_t _time;
};

/// Holds all informations about a player's finder settings. _NOT_ stored in database.
struct LFGuildPlayer
{
    public:
        LFGuildPlayer()
        {
            _guid.Clear();
            _roles = 0;
            _availability = 0;
            _playStyle = 0;
            _level = 0;
        }

        LFGuildPlayer(ObjectGuid const& guid, uint8 role, uint8 availability, uint8 playStyle, uint8 level)
        {
            _guid = guid;
            _roles = role;
            _availability = availability;
            _playStyle = playStyle;
            _level = level;
        }

        LFGuildPlayer(ObjectGuid const& guid, uint8 role, uint8 availability, uint8 playStyle, uint8 level, std::string& comment) : _comment(comment)
        {
            _guid = guid;
            _roles = role;
            _availability = availability;
            _playStyle = playStyle;
            _level = level;
        }

        LFGuildPlayer(LFGuildPlayer const& settings) : _comment(settings.GetComment())
        {
            _guid = settings.GetGUID();
            _roles = settings.GetClassRoles();
            _availability = settings.GetAvailability();
            _playStyle = settings.GetPlayStyle();
            _level = settings.GetLevel();
        }

        ObjectGuid const& GetGUID() const         { return _guid; }
        uint8 GetClassRoles() const    { return _roles; }
        uint8 GetAvailability() const  { return _availability; }
        uint8 GetPlayStyle() const     { return _playStyle; }
        uint8 GetLevel() const         { return _level; }
        std::string const& GetComment() const { return _comment; }

    private:
        std::string _comment;
        ObjectGuid _guid;
        uint8 _roles;
        uint8 _availability;
        uint8 _playStyle;
        uint8 _level;
};

/// Holds settings for a guild in the finder system. Saved to database.
struct LFGuildSettings : public LFGuildPlayer
{
    public:
        LFGuildSettings() : LFGuildPlayer(), _listed(false), _team(TEAM_ALLIANCE) {}

        LFGuildSettings(bool listed, TeamId team) : LFGuildPlayer(), _listed(listed), _team(team) {}

        LFGuildSettings(bool listed, TeamId team, ObjectGuid const& guid, uint8 role, uint8 availability, uint8 playStyle, uint8 level) : _listed(listed),
            LFGuildPlayer(guid, role, availability, playStyle, level), _team(team) {}

        LFGuildSettings(bool listed, TeamId team, ObjectGuid const& guid, uint8 role, uint8 availability, uint8 playStyle, uint8 level, std::string& comment) : _listed(listed),
            LFGuildPlayer(guid, role, availability, playStyle, level, comment), _team(team) {}

        LFGuildSettings(LFGuildSettings const& settings) : _listed(settings.IsListed()), _team(settings.GetTeam()),
            LFGuildPlayer(settings) {}


        bool IsListed() const      { return _listed; }
        void SetListed(bool state) { _listed = state; }

        TeamId GetTeam() const     { return _team; }
    private:
        TeamId _team;
        bool _listed;
};

typedef std::map<ObjectGuid /* guildGuid */, LFGuildSettings> LFGuildStore;
typedef std::map<ObjectGuid /* guildGuid */, std::vector<MembershipRequest> > MembershipRequestStore;

class GuildFinderMgr
{
    private:
        GuildFinderMgr();
        ~GuildFinderMgr();

        LFGuildStore  _guildSettings;

        MembershipRequestStore _membershipRequests;

        void LoadGuildSettings();
        void LoadMembershipRequests();

    public:
        static GuildFinderMgr* instance()
        {
            static GuildFinderMgr instance;
            return &instance;
        }

        void LoadFromDB();

        /**
         * @brief Stores guild settings and begins an asynchronous database insert
         * @param guildGuid The guild's database guid.
         * @param LFGuildSettings The guild's settings storage.
         */
        void SetGuildSettings(ObjectGuid const& guildGuid, LFGuildSettings const& settings);

        /**
         * @brief Returns settings for a guild.
         * @param guildGuid The guild's database guid.
         */
        LFGuildSettings GetGuildSettings(ObjectGuid const& guildGuid)
        {
            return _guildSettings.find(guildGuid) != _guildSettings.end() ? _guildSettings[guildGuid] : LFGuildSettings();
        }

        /**
         * @brief Files a membership request to a guild
         * @param guildGuid The guild's database GUID.
         * @param MembershipRequest An object storing all data related to the request.
         */
        void AddMembershipRequest(ObjectGuid const& guildGuid, MembershipRequest const& request);

        /**
         * @brief Removes all membership request from a player.
         * @param playerId The player's database guid whose application shall be deleted.
         */
        void RemoveAllMembershipRequestsFromPlayer(ObjectGuid const& playerId);

        /**
         * @brief Removes a membership request to a guild.
         * @param playerId The player's database guid whose application shall be deleted.
         * @param guildId  The guild's database guid
         */
        void RemoveMembershipRequest(ObjectGuid const& playerId, ObjectGuid const& guildId);

        /// wipes everything related to a guild. Used when that guild is disbanded
        void DeleteGuild(ObjectGuid const&  guildId);
        
        /**
         * @brief Returns a set of membership requests for a guild
         * @param guildGuid The guild's database guid.
         */
        std::vector<MembershipRequest> GetAllMembershipRequestsForGuild(ObjectGuid const& guildGuid)
        {
            return _membershipRequests.find(guildGuid) != _membershipRequests.end() ?  _membershipRequests[guildGuid] : std::vector<MembershipRequest>();
        }

        /**
         * @brief Returns a list of membership requests for a player.
         * @param playerGuid The player's database guid.
         */
        std::list<MembershipRequest> GetAllMembershipRequestsForPlayer(ObjectGuid const& playerGuid);

        /**
         * @brief Returns a store of guilds matching the settings provided, using bitmask operators.
         * @param settings The player's finder settings
         * @param teamId   The player's faction (TEAM_ALLIANCE or TEAM_HORDE)
         */
        LFGuildStore GetGuildsMatchingSetting(LFGuildPlayer& settings, TeamId faction);

        /// Provided a player DB guid and a guild DB guid, determines if a pending request is filed with these keys.
        bool HasRequest(ObjectGuid const& playerId, ObjectGuid const& guildId);

        /// Counts the amount of pending membership requests, given the player's db guid.
        uint8 CountRequestsFromPlayer(ObjectGuid const& playerId);
 
        void SendApplicantListUpdate(Guild& guild);
        void SendMembershipRequestListUpdate(Player& player);
};

#define sGuildFinderMgr GuildFinderMgr::instance()

#endif // __TRINITY_GUILDFINDER_H
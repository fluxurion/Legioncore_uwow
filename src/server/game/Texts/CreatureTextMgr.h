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

#ifndef TRINITY_CREATURE_TEXT_MGR_H
#define TRINITY_CREATURE_TEXT_MGR_H

#include "Creature.h"
#include "GridNotifiers.h"
#include "ObjectAccessor.h"
#include "SharedDefines.h"

struct CreatureTextEntry
{
    uint32 entry;
    uint32 duration;
    uint32 sound;
    uint32 BroadcastTextID;
    float probability;
    ChatMsg type;
    Language lang;
    Emote emote;
    std::string text;
    uint8 group;
    uint8 id;
};

enum TextRange
{
    TEXT_RANGE_NORMAL   = 0,
    TEXT_RANGE_AREA     = 1,
    TEXT_RANGE_ZONE     = 2,
    TEXT_RANGE_MAP      = 3,
    TEXT_RANGE_WORLD    = 4
};

enum text
{
    TEXT_GENERIC_0                                 = 0,
    TEXT_GENERIC_1                                 = 1,
    TEXT_GENERIC_2                                 = 2,
    TEXT_GENERIC_3                                 = 3,
    TEXT_GENERIC_4                                 = 4,
    TEXT_GENERIC_5                                 = 5,
    TEXT_GENERIC_6                                 = 6,
    TEXT_GENERIC_7                                 = 7,
    TEXT_GENERIC_8                                 = 8,
    TEXT_GENERIC_9                                 = 9,
    TEXT_GENERIC_10                                = 10,
    TEXT_GENERIC_11                                = 11,
    TEXT_GENERIC_12                                = 12,
    TEXT_GENERIC_13                                = 13,
    TEXT_GENERIC_14                                = 14,
    TEXT_GENERIC_15                                = 15,
    TEXT_GENERIC_16                                = 16,
};

struct CreatureTextLocale
{
    StringVector Text;
};

struct CreatureTextId
{
    CreatureTextId(uint32 e, uint32 g, uint32 i) :  entry(e), textGroup(g), textId(i)
    {
    }

    bool operator<(CreatureTextId const& right) const
    {
        return memcmp(this, &right, sizeof(CreatureTextId)) < 0;
    }

    uint32 entry;
    uint32 textGroup;
    uint32 textId;
};

typedef std::vector<CreatureTextEntry> CreatureTextGroup;              //texts in a group
typedef std::unordered_map<uint8, CreatureTextGroup> CreatureTextHolder;    //groups for a creature by groupid
typedef std::unordered_map<uint32, CreatureTextHolder> CreatureTextMap;     //all creatures by entry

typedef std::map<CreatureTextId, CreatureTextLocale> LocaleCreatureTextMap;

//used for handling non-repeatable random texts
typedef std::vector<uint8> CreatureTextRepeatIds;
typedef std::unordered_map<uint8, CreatureTextRepeatIds> CreatureTextRepeatGroup;
typedef std::unordered_map<ObjectGuid::LowType, CreatureTextRepeatGroup> CreatureTextRepeatMap;//guid based

class CreatureTextMgr
{
private:
    CreatureTextMgr() {};
    ~CreatureTextMgr() {};

    public:
        static CreatureTextMgr* instance()
        {
            static CreatureTextMgr instance;
            return &instance;
        }

        void LoadCreatureTexts();
        void LoadCreatureTextLocales();
        CreatureTextMap  const& GetTextMap() const { return mTextMap; }

        void SendSound(Creature* source, uint32 sound, ChatMsg msgType, ObjectGuid whisperGuid, TextRange range, Team team, bool gmOnly);
        void SendEmote(Unit* source, uint32 emote);

        //if sent, returns the 'duration' of the text else 0 if error
        uint32 SendChat(Creature* source, uint8 textGroup, ObjectGuid whisperGuid = ObjectGuid::Empty, ChatMsg msgType = CHAT_MSG_ADDON, Language language = LANG_ADDON, TextRange range = TEXT_RANGE_NORMAL, uint32 sound = 0, Team team = TEAM_OTHER, bool gmOnly = false, Player* srcPlr = NULL);
        bool TextExist(uint32 sourceEntry, uint8 textGroup);
        std::string GetLocalizedChatString(uint32 entry, uint8 gender, uint8 textGroup, uint32 id, LocaleConstant locale) const;

        template<class Builder>
        void SendChatPacket(WorldObject* source, Builder const& builder, ChatMsg msgType, ObjectGuid whisperGuid = ObjectGuid::Empty, TextRange range = TEXT_RANGE_NORMAL, Team team = TEAM_OTHER, bool gmOnly = false) const;
    private:
        CreatureTextRepeatIds GetRepeatGroup(Creature* source, uint8 textGroup);
        void SetRepeatId(Creature* source, uint8 textGroup, uint8 id);

        void SendNonChatPacket(WorldObject* source, WorldPacket const* data, ChatMsg msgType, ObjectGuid whisperGuid, TextRange range, Team team, bool gmOnly) const;
        float GetRangeForChatType(ChatMsg msgType) const;

        CreatureTextMap mTextMap;
        CreatureTextRepeatMap mTextRepeatMap;
        LocaleCreatureTextMap mLocaleTextMap;
};

#define sCreatureTextMgr CreatureTextMgr::instance()

template<class Builder>
class CreatureTextLocalizer
{
    public:
        CreatureTextLocalizer(Builder const& builder, ChatMsg msgType) : _builder(builder), _msgType(msgType)
        {
        }

        ~CreatureTextLocalizer()
        {
        }

        void operator()(Player* player)
        {
            LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

            WorldPacket data;
            _builder(&data, loc_idx, player->GetGUID());

            player->SendDirectMessage(&data);
        }

    private:
        Builder const& _builder;
        ChatMsg _msgType;
};

template<class Builder>
void CreatureTextMgr::SendChatPacket(WorldObject* source, Builder const& builder, ChatMsg msgType, ObjectGuid whisperGuid, TextRange range, Team team, bool gmOnly) const
{
    if (!source)
        return;

    CreatureTextLocalizer<Builder> localizer(builder, msgType);

    switch (msgType)
    {
        case CHAT_MSG_MONSTER_WHISPER:
        case CHAT_MSG_RAID_BOSS_WHISPER:
        {
            if (range == TEXT_RANGE_NORMAL) //ignores team and gmOnly
            {
                Player* player = ObjectAccessor::FindPlayer(whisperGuid);
                if (!player || !player->GetSession())
                    return;

                localizer(player);
                return;
            }
            break;
        }
        default:
            break;
    }

    switch (range)
    {
        case TEXT_RANGE_AREA:
        {
            uint32 areaId = source->GetAreaId();
            Map::PlayerList const& players = source->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (itr->getSource()->GetAreaId() == areaId && (!team || Team(itr->getSource()->GetTeam()) == team) && (!gmOnly || itr->getSource()->isGameMaster()))
                    localizer(itr->getSource());
            return;
        }
        case TEXT_RANGE_ZONE:
        {
            uint32 zoneId = source->GetZoneId();
            Map::PlayerList const& players = source->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (itr->getSource()->GetZoneId() == zoneId && (!team || Team(itr->getSource()->GetTeam()) == team) && (!gmOnly || itr->getSource()->isGameMaster()))
                    localizer(itr->getSource());
            return;
        }
        case TEXT_RANGE_MAP:
        {
            Map::PlayerList const& players = source->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if ((!team || Team(itr->getSource()->GetTeam()) == team) && (!gmOnly || itr->getSource()->isGameMaster()))
                    localizer(itr->getSource());
            return;
        }
        case TEXT_RANGE_WORLD:
        {
            SessionMap const& smap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator iter = smap.begin(); iter != smap.end(); ++iter)
                if (Player* player = iter->second->GetPlayer())
                    if (player->GetSession()  && (!team || Team(player->GetTeam()) == team) && (!gmOnly || player->isGameMaster()))
                        localizer(player);
            return;
        }
        case TEXT_RANGE_NORMAL:
        default:
            break;
    }

    float dist = GetRangeForChatType(msgType);
    Trinity::PlayerDistWorker<CreatureTextLocalizer<Builder> > worker(source, dist, localizer);
    source->VisitNearbyWorldObject(dist, worker);
}

#endif

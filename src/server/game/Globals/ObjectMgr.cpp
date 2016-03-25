﻿/*
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

#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "GroupMgr.h"
#include "SpellMgr.h"
#include "UpdateMask.h"
#include "World.h"
#include "Transport.h"
#include "Language.h"
#include "GameEventMgr.h"
#include "Spell.h"
#include "WordFilterMgr.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "InstanceSaveMgr.h"
#include "SpellAuras.h"
#include "Util.h"
#include "WaypointManager.h"
#include "GossipDef.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "PoolMgr.h"
#include "DB2Stores.h"
#include "Configuration/Config.h"
#include "LFGMgr.h"
#include <openssl/md5.h>
#include "Packets/BattlePayPackets.h"

ScriptMapMap sQuestEndScripts;
ScriptMapMap sQuestStartScripts;
ScriptMapMap sSpellScripts;
ScriptMapMap sGameObjectScripts;
ScriptMapMap sEventScripts;
ScriptMapMap sWaypointScripts;
VisibleDistanceMap sVisibleDistance[TYPE_VISIBLE_MAX];

std::string GetScriptsTableNameByType(ScriptsType type)
{
    std::string res = "";
    switch (type)
    {
        case SCRIPTS_QUEST_END:     res = "quest_end_scripts";  break;
        case SCRIPTS_QUEST_START:   res = "quest_start_scripts";break;
        case SCRIPTS_SPELL:         res = "spell_scripts";      break;
        case SCRIPTS_GAMEOBJECT:    res = "gameobject_scripts"; break;
        case SCRIPTS_EVENT:         res = "event_scripts";      break;
        case SCRIPTS_WAYPOINT:      res = "waypoint_scripts";   break;
        default: break;
    }
    return res;
}

ScriptMapMap* GetScriptsMapByType(ScriptsType type)
{
    ScriptMapMap* res = NULL;
    switch (type)
    {
        case SCRIPTS_QUEST_END:     res = &sQuestEndScripts;    break;
        case SCRIPTS_QUEST_START:   res = &sQuestStartScripts;  break;
        case SCRIPTS_SPELL:         res = &sSpellScripts;       break;
        case SCRIPTS_GAMEOBJECT:    res = &sGameObjectScripts;  break;
        case SCRIPTS_EVENT:         res = &sEventScripts;       break;
        case SCRIPTS_WAYPOINT:      res = &sWaypointScripts;    break;
        default: break;
    }
    return res;
}

std::string GetScriptCommandName(ScriptCommands command)
{
    std::string res = "";
    switch (command)
    {
        case SCRIPT_COMMAND_TALK: res = "SCRIPT_COMMAND_TALK"; break;
        case SCRIPT_COMMAND_EMOTE: res = "SCRIPT_COMMAND_EMOTE"; break;
        case SCRIPT_COMMAND_FIELD_SET: res = "SCRIPT_COMMAND_FIELD_SET"; break;
        case SCRIPT_COMMAND_MOVE_TO: res = "SCRIPT_COMMAND_MOVE_TO"; break;
        case SCRIPT_COMMAND_FLAG_SET: res = "SCRIPT_COMMAND_FLAG_SET"; break;
        case SCRIPT_COMMAND_FLAG_REMOVE: res = "SCRIPT_COMMAND_FLAG_REMOVE"; break;
        case SCRIPT_COMMAND_TELEPORT_TO: res = "SCRIPT_COMMAND_TELEPORT_TO"; break;
        case SCRIPT_COMMAND_QUEST_EXPLORED: res = "SCRIPT_COMMAND_QUEST_EXPLORED"; break;
        case SCRIPT_COMMAND_KILL_CREDIT: res = "SCRIPT_COMMAND_KILL_CREDIT"; break;
        case SCRIPT_COMMAND_RESPAWN_GAMEOBJECT: res = "SCRIPT_COMMAND_RESPAWN_GAMEOBJECT"; break;
        case SCRIPT_COMMAND_TEMP_SUMMON_CREATURE: res = "SCRIPT_COMMAND_TEMP_SUMMON_CREATURE"; break;
        case SCRIPT_COMMAND_OPEN_DOOR: res = "SCRIPT_COMMAND_OPEN_DOOR"; break;
        case SCRIPT_COMMAND_CLOSE_DOOR: res = "SCRIPT_COMMAND_CLOSE_DOOR"; break;
        case SCRIPT_COMMAND_ACTIVATE_OBJECT: res = "SCRIPT_COMMAND_ACTIVATE_OBJECT"; break;
        case SCRIPT_COMMAND_REMOVE_AURA: res = "SCRIPT_COMMAND_REMOVE_AURA"; break;
        case SCRIPT_COMMAND_CAST_SPELL: res = "SCRIPT_COMMAND_CAST_SPELL"; break;
        case SCRIPT_COMMAND_PLAY_SOUND: res = "SCRIPT_COMMAND_PLAY_SOUND"; break;
        case SCRIPT_COMMAND_CREATE_ITEM: res = "SCRIPT_COMMAND_CREATE_ITEM"; break;
        case SCRIPT_COMMAND_DESPAWN_SELF: res = "SCRIPT_COMMAND_DESPAWN_SELF"; break;
        case SCRIPT_COMMAND_LOAD_PATH: res = "SCRIPT_COMMAND_LOAD_PATH"; break;
        case SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT: res = "SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT"; break;
        case SCRIPT_COMMAND_KILL: res = "SCRIPT_COMMAND_KILL"; break;
        case SCRIPT_COMMAND_ORIENTATION: res = "SCRIPT_COMMAND_ORIENTATION"; break;
        case SCRIPT_COMMAND_EQUIP: res = "SCRIPT_COMMAND_EQUIP"; break;
        case SCRIPT_COMMAND_MODEL: res = "SCRIPT_COMMAND_MODEL"; break;
        case SCRIPT_COMMAND_CLOSE_GOSSIP: res = "SCRIPT_COMMAND_CLOSE_GOSSIP"; break;
        case SCRIPT_COMMAND_PLAYMOVIE: res = "SCRIPT_COMMAND_PLAYMOVIE"; break;
        case SCRIPT_COMMAND_PLAYSCENE: res = "SCRIPT_COMMAND_PLAYSCENE"; break;
        case SCRIPT_COMMAND_STOPSCENE: res = "SCRIPT_COMMAND_STOPSCENE"; break;
        default:
        {
            char sz[32];
            sprintf(sz, "Unknown command: %u", command);
            res = sz;
            break;
        }
    }
    return res;
}

std::string ScriptInfo::GetDebugInfo() const
{
    char sz[256];
    sprintf(sz, "%s ('%s' script id: %u)", GetScriptCommandName(command).c_str(), GetScriptsTableNameByType(type).c_str(), id);
    return std::string(sz);
}

bool normalizePlayerName(std::string& name)
{
    if (name.empty())
        return false;

    if (name[0] == -61 && name[1] == -97) // Interdiction d'utiliser ce caractere au debut, il fait planter l'affichage cote client
        return false;

    wchar_t wstr_buf[MAX_INTERNAL_PLAYER_NAME+1];
    size_t wstr_len = MAX_INTERNAL_PLAYER_NAME;

    if (!Utf8toWStr(name, &wstr_buf[0], wstr_len))
        return false;

    wstr_buf[0] = wcharToUpper(wstr_buf[0]);
    for (size_t i = 1; i < wstr_len; ++i)
        wstr_buf[i] = wcharToLower(wstr_buf[i]);

    if (!WStrToUtf8(wstr_buf, wstr_len, name))
        return false;

    return true;
}

LanguageDesc lang_description[LANGUAGE_DESC_COUNT] =
{
    { LANG_ADDON,           0, 0                       },
    { LANG_UNIVERSAL,       0, 0                       },
    { LANG_ORCISH,        669, SKILL_LANG_ORCISH       },
    { LANG_DARNASSIAN,    671, SKILL_LANG_DARNASSIAN   },
    { LANG_TAURAHE,       670, SKILL_LANG_TAURAHE      },
    { LANG_DWARVISH,      672, SKILL_LANG_DWARVEN      },
    { LANG_COMMON,        668, SKILL_LANG_COMMON       },
    { LANG_DEMONIC,       815, SKILL_LANG_DEMON_TONGUE },
    { LANG_TITAN,         816, SKILL_LANG_TITAN        },
    { LANG_THALASSIAN,    813, SKILL_LANG_THALASSIAN   },
    { LANG_DRACONIC,      814, SKILL_LANG_DRACONIC     },
    { LANG_KALIMAG,       817, SKILL_LANG_OLD_TONGUE   },
    { LANG_GNOMISH,      7340, SKILL_LANG_GNOMISH      },
    { LANG_TROLL,        7341, SKILL_LANG_TROLL        },
    { LANG_GUTTERSPEAK, 17737, SKILL_LANG_GUTTERSPEAK  },
    { LANG_DRAENEI,     29932, SKILL_LANG_DRAENEI      },
    { LANG_ZOMBIE,          0, 0                       },
    { LANG_GNOMISH_BINARY,  0, 0                       },
    { LANG_GOBLIN_BINARY,   0, 0                       },
    { LANG_WORGEN,      69270, SKILL_LANG_WORGEN       },
    { LANG_GOBLIN,      69269, SKILL_LANG_GOBLIN       },
    { LANG_PANDAREN_N,  108127,SKILL_LANG_PANDAREN_NEUTRAL },
    { LANG_PANDAREN_H,  108130,SKILL_LANG_PANDAREN_HORDE },
    { LANG_PANDAREN_A,  108131,SKILL_LANG_PANDAREN_ALLIANCE },
};

LanguageDesc const* GetLanguageDescByID(uint32 lang)
{
    for (uint8 i = 0; i < LANGUAGE_DESC_COUNT; ++i)
    {
        if (uint32(lang_description[i].lang_id) == lang)
            return &lang_description[i];
    }

    return NULL;
}

bool SpellClickInfo::IsFitToRequirements(Unit const* clicker, Unit const* clickee) const
{
    Player const* playerClicker = clicker->ToPlayer();
    if (!playerClicker)
        return true;

    Unit const* summoner = NULL;
    // Check summoners for party
    if (clickee->isSummon())
        summoner = clickee->ToTempSummon()->GetSummoner();
    if (!summoner)
        summoner = clickee;

    // This only applies to players
    switch (userType)
    {
        case SPELL_CLICK_USER_FRIEND:
            if (!playerClicker->IsFriendlyTo(summoner))
                return false;
            break;
        case SPELL_CLICK_USER_RAID:
            if (!playerClicker->IsInRaidWith(summoner))
                return false;
            break;
        case SPELL_CLICK_USER_PARTY:
            if (!playerClicker->IsInPartyWith(summoner))
                return false;
            break;
        default:
            break;
    }

    return true;
}

float GetVisibleDistance(uint32 type, uint32 id)
{
    VisibleDistanceMap::const_iterator itr = sVisibleDistance[type].find(id);
    if (itr != sVisibleDistance[type].end())
        return itr->second;

    return 0.0f;
}

template<> ObjectGuidGenerator<HighGuid::Player>* ObjectMgr::GetGenerator() { return &_playerGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Creature>* ObjectMgr::GetGenerator() { return &_creatureGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Pet>* ObjectMgr::GetGenerator() { return &_petGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Vehicle>* ObjectMgr::GetGenerator() { return &_vehicleGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Item>* ObjectMgr::GetGenerator() { return &_itemGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::GameObject>* ObjectMgr::GetGenerator() { return &_gameObjectGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::DynamicObject>* ObjectMgr::GetGenerator() { return &_dynamicObjectGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Corpse>* ObjectMgr::GetGenerator() { return &_corpseGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::AreaTrigger>* ObjectMgr::GetGenerator() { return &_areaTriggerGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::LootObject>* ObjectMgr::GetGenerator() { return &_lootObjectGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Transport>* ObjectMgr::GetGenerator() { return &_moTransportGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::BattlePet>* ObjectMgr::GetGenerator() { return &_BattlePetGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::PetBattle>* ObjectMgr::GetGenerator() { return &_PetBattleGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Conversation>* ObjectMgr::GetGenerator() { return &_conversationGuidGenerator; }
template<> ObjectGuidGenerator<HighGuid::Cast>* ObjectMgr::GetGenerator() { return &_castGuidGenerator; }

template<HighGuid type>
ObjectGuidGenerator<type>* ObjectMgr::GetGenerator()
{
    return nullptr;
}

ObjectMgr::ObjectMgr()
{
    _auctionId = 1;
    _equipmentSetGuid = 1;
    _itemTextId = 1;
    _hiPetNumber = 1;
    _voidItemId = 1;
    _skipUpdateCount = 1;
    _reportComplaintID = 1;
    _supportTicketSubmitBugID = 1;
}

ObjectMgr::~ObjectMgr()
{
    for (QuestMap::iterator i = _questTemplates.begin(); i != _questTemplates.end(); ++i)
        delete i->second;

    for (uint8 race = 0; race < MAX_RACES; ++race)
    {
        for (uint8 class_ = 0; class_ < MAX_CLASSES; ++class_)
        {
            if (_playerInfo[race][class_])
                delete[] _playerInfo[race][class_]->levelInfo;

            delete _playerInfo[race][class_];
        }
    }

    for (CacheVendorItemContainer::iterator itr = _cacheVendorItemStore.begin(); itr != _cacheVendorItemStore.end(); ++itr)
        itr->second.Clear();

    _cacheTrainerSpellStore.clear();

    for (DungeonEncounterContainer::iterator itr =_dungeonEncounterStore.begin(); itr != _dungeonEncounterStore.end(); ++itr)
        for (DungeonEncounterList::iterator encounterItr = itr->second.begin(); encounterItr != itr->second.end(); ++encounterItr)
            delete *encounterItr;
}

std::list<CurrencyLoot> ObjectMgr::GetCurrencyLoot(uint32 entry, uint8 type, uint8 spawnMode)
{
    std::list<CurrencyLoot> temp;
    uint16 diffMask = (1 << (CreatureTemplate::GetDiffFromSpawn(spawnMode)));
    for (CurrencysLoot::iterator itr = _currencysLoot.begin(); itr != _currencysLoot.end(); ++itr)
    {
        if (itr->Entry == entry && itr->Type == type && (itr->lootmode == 0 || (itr->lootmode & diffMask)))
            temp.push_back(*itr);
    }
    return temp;
}

void ObjectMgr::AddLocaleString(std::string const& value, LocaleConstant localeConstant, StringVector& data)
{
    if (value.empty())
        return;

    if (data.size() <= size_t(localeConstant))
        data.resize(localeConstant + 1);

    data[localeConstant] = value;
}

void ObjectMgr::LoadWorldVisibleDistance()
{
    uint32 oldMSTime = getMSTime();

    for (uint8 i = 0; i < TYPE_VISIBLE_MAX; ++i)
        sVisibleDistance[i].clear();

    QueryResult result = WorldDatabase.Query("SELECT `type`, `id`, `distance` FROM `world_visible_distance`");

    if (!result)
        return;

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 type = fields[0].GetUInt32();
        uint32 id = fields[1].GetUInt32();
        float distance = fields[2].GetFloat();
        if(type > TYPE_VISIBLE_MAX)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded world visible distance type %u error", type);
            continue;
        }

        sVisibleDistance[type][id] = distance;

        count++;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u world visible distance in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}


void ObjectMgr::LoadCreatureLocales()
{
    uint32 oldMSTime = getMSTime();

    _creatureLocaleStore.clear();

    //                                               0   1       2      3         4      5      6      7      8         9         10        11
    QueryResult result = WorldDatabase.Query("SELECT ID, Locale, Title, TitleAlt, Name1, Name2, Name3, Name4, NameAlt1, NameAlt2, NameAlt3, NameAlt4 FROM creature_template_wdb_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        LocaleConstant locale = GetLocaleByName(fields[1].GetString());
        if (locale == LOCALE_enUS || locale == LOCALE_none)
            continue;

        CreatureLocale& data = _creatureLocaleStore[fields[0].GetUInt32()];
        AddLocaleString(fields[2].GetString(), locale, data.Title);
        AddLocaleString(fields[3].GetString(), locale, data.TitleAlt);

        for (uint8 i = 0; i < MAX_CREATURE_NAMES; ++i)
            AddLocaleString(fields[4 + i].GetString(), locale, data.Name[i]);

        for (uint8 i = 0; i < MAX_CREATURE_NAMES; ++i)
            AddLocaleString(fields[8 + i].GetString(), locale, data.NameAlt[i]);

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature locale strings in %u ms", static_cast<uint32>(_creatureLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenuItemsLocales()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsLocaleStore.clear();

    //                                               0       1   2       3           4
    QueryResult result = WorldDatabase.Query("SELECT MenuID, ID, Locale, OptionText, BoxText FROM gossip_menu_option_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        GossipMenuItemsLocale& data = _gossipMenuItemsLocaleStore[MAKE_PAIR32(fields[0].GetUInt16(), fields[1].GetUInt16())];
        LocaleConstant locale = GetLocaleByName(fields[2].GetString());
        if (locale == LOCALE_enUS || locale == LOCALE_none)
            continue;

        AddLocaleString(fields[3].GetString(), locale, data.OptionText);
        AddLocaleString(fields[4].GetString(), locale, data.BoxText);

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u gossip_menu_option locale strings in %u ms", static_cast<uint32>(_gossipMenuItemsLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPointOfInterestLocales()
{
    uint32 oldMSTime = getMSTime();

    _pointOfInterestLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, icon_name_loc1, icon_name_loc2, icon_name_loc3, icon_name_loc4, icon_name_loc5, icon_name_loc6, icon_name_loc7, icon_name_loc8, icon_name_loc9, icon_name_loc10 FROM locales_points_of_interest");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        PointOfInterestLocale& data = _pointOfInterestLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.IconName);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu points_of_interest locale strings in %u ms", (unsigned long)_pointOfInterestLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadWDBCreatureTemplates()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query(
    //      0      1      2       3     4       5         6        7         8         9      10        11          12    13         14          15
    "SELECT Entry, Name1, Name2, Name3, Name4, NameAlt1, NameAlt2, NameAlt3, NameAlt4, Title, TitleAlt, CursorName, Type, TypeFlags, TypeFlags2, RequiredExpansion, "
    //16     17              18              19       20          21      22           23           24      25          26          28          29
    "Family, Classification, MovementInfoID, HpMulti, PowerMulti, Leader, KillCredit1, KillCredit2, UnkInt, DisplayId1, DisplayId2, DisplayId3, DisplayId4, "
    //30        31          32          33          34          35          36          37
    "FlagQuest, QuestItem1, QuestItem2, QuestItem3, QuestItem4, QuestItem5, QuestItem6, VerifiedBuild FROM creature_template_wdb;");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature template definitions. DB table `creature_template_wdb` is empty.");
        return;
    }
    
    _creatureTemplateStore.rehash(result->GetRowCount());

    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        uint32 entry = fields[index++].GetUInt32();
        CreatureTemplate& creatureTemplate = _creatureTemplateStore[entry];

        creatureTemplate.Entry = entry;

        for (uint8 i = 0; i < MAX_CREATURE_NAMES; ++i)
            creatureTemplate.Name[i] = fields[index++].GetString();
        for (uint8 i = 0; i < MAX_CREATURE_NAMES; ++i)
            creatureTemplate.NameAlt[i] = fields[index++].GetString();
        creatureTemplate.Title = fields[index++].GetString();
        creatureTemplate.TitleAlt = fields[index++].GetString();
        creatureTemplate.CursorName = fields[index++].GetString();
        creatureTemplate.Type = uint32(fields[index++].GetUInt8());
        for (uint8 i = 0; i < MAX_TYPE_FLAGS; ++i)
            creatureTemplate.TypeFlags[i] = fields[index++].GetUInt32();
        creatureTemplate.RequiredExpansion = uint32(fields[index++].GetInt16());
        creatureTemplate.Family = fields[index++].GetUInt32();
        creatureTemplate.Classification = uint32(fields[index++].GetUInt8());
        creatureTemplate.MovementInfoID = fields[index++].GetUInt32();
        creatureTemplate.HpMulti = fields[index++].GetFloat();
        creatureTemplate.PowerMulti = fields[index++].GetFloat();
        creatureTemplate.Leader = fields[index++].GetBool();
        for (uint8 i = 0; i < MAX_KILL_CREDIT; ++i)
            creatureTemplate.KillCredit[i] = fields[index++].GetUInt32();
        creatureTemplate.UnkInt = fields[index++].GetUInt32();
        for (uint8 i = 0; i < MAX_CREATURE_MODELS; ++i)
            creatureTemplate.Modelid[i] = fields[index++].GetUInt32();
        creatureTemplate.FlagQuest = fields[index++].GetUInt32();
        for (uint8 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            creatureTemplate.QuestItem[i] = fields[index++].GetUInt32();
        creatureTemplate.VerifiedBuild = fields[index++].GetUInt32();
    }
    while (result->NextRow());

    for (CreatureTemplateContainer::const_iterator::value_type itr : _creatureTemplateStore)
        CheckCreatureTemplateWDB(&itr.second);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature WDB templates in %u ms", static_cast<uint32>(_creatureTemplateStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::CheckCreatureTemplateWDB(CreatureTemplate const* cInfo)
{
    if (!cInfo)
        return;

    CreatureDisplayInfoEntry const* displayScaleEntry = nullptr;
    if (cInfo->Modelid[0])
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid[0]);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid[0] id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid[0]);
            const_cast<CreatureTemplate*>(cInfo)->Modelid[0] = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid[0]);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid[0]` = %u listed by creature (Entry: %u).", cInfo->Modelid[0], cInfo->Entry);
    }

    if (cInfo->Modelid[1])
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid[1]);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid[1] id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid[1]);
            const_cast<CreatureTemplate*>(cInfo)->Modelid[1] = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid[1]);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid[1]` = %u listed by creature (Entry: %u).", cInfo->Modelid[1], cInfo->Entry);
    }

    if (cInfo->Modelid[2])
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid[2]);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid[2] id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid[2]);
            const_cast<CreatureTemplate*>(cInfo)->Modelid[2] = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid[2]);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid[2]` = %u listed by creature (Entry: %u).", cInfo->Modelid[2], cInfo->Entry);
    }

    if (cInfo->Modelid[3])
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid[3]);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid[3] id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid[3]);
            const_cast<CreatureTemplate*>(cInfo)->Modelid[3] = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid[3]);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid[3]` = %u listed by creature (Entry: %u).", cInfo->Modelid[3], cInfo->Entry);
    }

    if (!displayScaleEntry)
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) does not have any existing display id in Modelid1/Modelid2/Modelid3/Modelid[3].", cInfo->Entry);

    for (uint8 k = 0; k < MAX_KILL_CREDIT; ++k)
        if (cInfo->KillCredit[k])
            if (!GetCreatureTemplate(cInfo->KillCredit[k]))
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing creature entry %u in `KillCredit%d`.", cInfo->Entry, cInfo->KillCredit[k], k + 1);
                const_cast<CreatureTemplate*>(cInfo)->KillCredit[k] = 0;
            }

    if (cInfo->Type && !sCreatureTypeStore.LookupEntry(cInfo->Type))
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid creature type (%u) in `type`.", cInfo->Entry, cInfo->Type);
        const_cast<CreatureTemplate*>(cInfo)->Type = CREATURE_TYPE_HUMANOID;
    }

    if (cInfo->Family && !sCreatureFamilyStore.LookupEntry(cInfo->Family))
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid creature family (%u) in `family`.", cInfo->Entry, cInfo->Family);
        const_cast<CreatureTemplate*>(cInfo)->Family = 0;
    }

    if (cInfo->RequiredExpansion > (MAX_EXPANSIONS - 1))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `creature_template` lists creature (Entry: %u) with `exp` %u. Ignored and set to 0.", cInfo->Entry, cInfo->RequiredExpansion);
        const_cast<CreatureTemplate*>(cInfo)->RequiredExpansion = 0;
    }
}

void ObjectMgr::LoadCreatureTemplates()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query(
    //      0      1               2         3         4        5        6         7           8          9          10     11      12      13         14
    "SELECT entry, gossip_menu_id, minlevel, maxlevel, faction, npcflag, npcflag2, speed_walk, speed_run, speed_fly, scale, mindmg, maxdmg, dmgschool, attackpower,"
    //15             16              17               18          19          20           21           22            23            24
    "dmg_multiplier, baseattacktime, rangeattacktime, unit_class, unit_flags, unit_flags2, unit_flags3, dynamicflags, trainer_type, trainer_spell, "
    //25            26            27           28           29                 30      31              32        33           34           35
    "trainer_class, trainer_race, minrangedmg, maxrangedmg, rangedattackpower, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, "
    //36          37           38           39      40      41      42      43      44      45      46      47              48         49       50
    "resistance4, resistance5, resistance6, spell1, spell2, spell3, spell4, spell5, spell6, spell7, spell8, PetSpellDataId, VehicleId, mingold, maxgold, "
    //51     52             53          54           55              56         57           58                    59           60          61
    "AIName, MovementType, InhabitType, HoverHeight, Mana_mod_extra, Armor_mod, RegenHealth, mechanic_immune_mask, flags_extra, ScriptName, personalloot, "
    //62         63             64    65             66
    "VignetteId, WorldEffectID, AiID, MovementIDKit, MeleeID FROM creature_template;");

    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();
        uint32 entry = fields[index++].GetUInt32();

        if (!_creatureTemplateStore.count(entry))
            continue;

        CreatureTemplate& creatureTemplate = _creatureTemplateStore[entry];

        creatureTemplate.GossipMenuId      = fields[index++].GetUInt32();
        creatureTemplate.minlevel          = fields[index++].GetUInt8();
        creatureTemplate.maxlevel          = fields[index++].GetUInt8();
        creatureTemplate.faction         = uint32(fields[index++].GetUInt16());
        creatureTemplate.npcflag           = fields[index++].GetUInt32();
        creatureTemplate.npcflag2          = fields[index++].GetUInt32();
        creatureTemplate.speed_walk        = fields[index++].GetFloat();
        creatureTemplate.speed_run         = fields[index++].GetFloat();
        creatureTemplate.speed_fly         = fields[index++].GetFloat();
        creatureTemplate.scale             = fields[index++].GetFloat();
        creatureTemplate.mindmg            = fields[index++].GetFloat();
        creatureTemplate.maxdmg            = fields[index++].GetFloat();
        creatureTemplate.dmgschool         = uint32(fields[index++].GetInt8());
        creatureTemplate.attackpower       = fields[index++].GetUInt32();
        creatureTemplate.dmg_multiplier    = fields[index++].GetFloat();
        creatureTemplate.baseattacktime    = fields[index++].GetUInt32();
        creatureTemplate.rangeattacktime   = fields[index++].GetUInt32();
        creatureTemplate.unit_class        = uint32(fields[index++].GetUInt8());
        creatureTemplate.unit_flags        = fields[index++].GetUInt32();
        creatureTemplate.unit_flags2       = fields[index++].GetUInt32();
        creatureTemplate.unit_flags3       = fields[index++].GetUInt32();
        creatureTemplate.dynamicflags      = fields[index++].GetUInt32();
        creatureTemplate.trainer_type      = uint32(fields[index++].GetUInt8());
        creatureTemplate.trainer_spell     = fields[index++].GetUInt32();
        creatureTemplate.trainer_class     = uint32(fields[index++].GetUInt8());
        creatureTemplate.trainer_race      = uint32(fields[index++].GetUInt8());
        creatureTemplate.minrangedmg       = fields[index++].GetFloat();
        creatureTemplate.maxrangedmg       = fields[index++].GetFloat();
        creatureTemplate.rangedattackpower = uint32(fields[index++].GetUInt16());
        creatureTemplate.lootid            = fields[index++].GetUInt32();
        creatureTemplate.pickpocketLootId  = fields[index++].GetUInt32();
        creatureTemplate.SkinLootId        = fields[index++].GetUInt32();

        for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
            creatureTemplate.resistance[i] = fields[index++].GetInt16();

        for (uint8 i = 0; i < CREATURE_MAX_SPELLS; ++i)
            creatureTemplate.spells[i] = fields[index++].GetUInt32();

        creatureTemplate.PetSpellDataId = fields[index++].GetUInt32();
        creatureTemplate.VehicleId      = fields[index++].GetUInt32();
        creatureTemplate.mingold        = fields[index++].GetUInt32();
        creatureTemplate.maxgold        = fields[index++].GetUInt32();
        creatureTemplate.AIName         = fields[index++].GetString();
        creatureTemplate.MovementType   = uint32(fields[index++].GetUInt8());
        creatureTemplate.InhabitType    = uint32(fields[index++].GetUInt8());
        creatureTemplate.HoverHeight    = fields[index++].GetFloat();
        creatureTemplate.ModManaExtra   = fields[index++].GetFloat();
        creatureTemplate.ModArmor       = fields[index++].GetFloat();
        creatureTemplate.RegenHealth        = fields[index++].GetBool();
        creatureTemplate.MechanicImmuneMask = fields[index++].GetUInt32();
        creatureTemplate.flags_extra        = fields[index++].GetUInt32();
        creatureTemplate.ScriptID           = GetScriptId(fields[index++].GetCString());
        creatureTemplate.personalloot       = fields[index++].GetUInt32();
        creatureTemplate.VignetteId         = fields[index++].GetUInt32();
        creatureTemplate.WorldEffectID      = fields[index++].GetUInt32();
        creatureTemplate.AiID               = fields[index++].GetUInt32();
        creatureTemplate.MovementIDKit      = fields[index++].GetUInt32();
        creatureTemplate.MeleeID            = fields[index++].GetUInt32();

        if(creatureTemplate.TypeFlags[0] & CREATURE_TYPEFLAGS_BOSS)
        {
            //Save loot spell
            if(creatureTemplate.spells[6])
                _creatureSpellBonus[creatureTemplate.spells[6]] = entry;
            //Save bonus loot spell
            if(creatureTemplate.spells[7])
                _creatureSpellBonus[creatureTemplate.spells[7]] = entry;
        }
        ++count;
    }
    while (result->NextRow());

    // Checking needs to be done after loading because of the difficulty self referencing
    for (CreatureTemplateContainer::const_iterator itr = _creatureTemplateStore.begin(); itr != _creatureTemplateStore.end(); ++itr)
        CheckCreatureTemplate(&itr->second);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureEvaseWmoData()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0        1       2      3          4
    QueryResult result = WorldDatabase.Query("SELECT entry, distance, wmoId, wmoSet, wmoGroupId FROM creature_evade_wmo_data;");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature evade data. DB table `creature_evade_wmo_data` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        CreatureEvadeWmoData& creatureEvade = _creatureEvadeWmoDataStore[entry];

        creatureEvade.entry       = entry;
        creatureEvade.distance    = float(fields[1].GetUInt32());
        creatureEvade.wmoId       = fields[2].GetUInt32();
        creatureEvade.wmoSet      = fields[3].GetUInt32();
        creatureEvade.wmoGroupId  = fields[4].GetUInt32();

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature evade data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureDifficultyStat()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0        1             2             3
    QueryResult result = WorldDatabase.Query("SELECT entry, difficulty, dmg_multiplier, HealthModifier FROM creature_difficulty_stat;");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature difficulty stat definitions. DB table `creature_difficulty_stat` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        CreatureDifficultyStat creatureDiffStat;
        creatureDiffStat.Entry           = entry;
        creatureDiffStat.Difficulty      = fields[1].GetUInt8();
        creatureDiffStat.dmg_multiplier  = fields[2].GetFloat();
        creatureDiffStat.ModHealth       = fields[3].GetFloat();

        _creatureDifficultyStatStore[entry].push_back(creatureDiffStat);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature difficulty stat  definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureTemplateAddons()
{
    uint32 oldMSTime = getMSTime();

    //                                                0       1       2      3       4       5      6
    QueryResult result = WorldDatabase.Query("SELECT entry, path_id, mount, bytes1, bytes2, emote, auras FROM creature_template_addon");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature template addon definitions. DB table `creature_template_addon` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Creature template (Entry: %u) does not exist but has a record in `creature_template_addon`", entry);
            continue;
        }

        CreatureAddon& creatureAddon = _creatureTemplateAddonStore[entry];

        creatureAddon.path_id = fields[1].GetUInt32();
        creatureAddon.mount   = fields[2].GetUInt32();
        creatureAddon.bytes1  = fields[3].GetUInt32();
        creatureAddon.bytes2  = fields[4].GetUInt32();
        creatureAddon.emote   = fields[5].GetUInt32();

        Tokenizer tokens(fields[6].GetString(), ' ');
        uint8 i = 0;
        creatureAddon.auras.resize(tokens.size());
        for (Tokenizer::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
        {
            SpellInfo const* AdditionalSpellInfo = sSpellMgr->GetSpellInfo(uint32(atol(*itr)));
            if (!AdditionalSpellInfo || AdditionalSpellInfo->HasAura(SPELL_AURA_CONTROL_VEHICLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong spell %u defined in `auras` field in `creature_template_addon`.", entry, uint32(atol(*itr)));
                continue;
            }
            creatureAddon.auras[i++] = uint32(atol(*itr));
        }

        if (creatureAddon.mount)
        {
            if (!sCreatureDisplayInfoStore.LookupEntry(creatureAddon.mount))
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid displayInfoId (%u) for mount defined in `creature_template_addon`", entry, creatureAddon.mount);
                creatureAddon.mount = 0;
            }
        }

        if (!sEmotesStore.LookupEntry(creatureAddon.emote))
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid emote (%u) defined in `creature_addon`.", entry, creatureAddon.emote);
            creatureAddon.emote = 0;
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature template addons in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::CheckCreatureTemplate(CreatureTemplate const* cInfo)
{
    if (!cInfo)
        return;
   
    if (cInfo->faction)
    {
        FactionTemplateEntry const* factionTemplate = sFactionTemplateStore.LookupEntry(cInfo->faction);
        if (!factionTemplate)
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has non-existing faction template (%u).", cInfo->Entry, cInfo->faction);
    }

    if (!cInfo->unit_class || ((1 << (cInfo->unit_class-1)) & CLASSMASK_ALL_CREATURES) == 0)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid unit_class (%u) in creature_template. Set to 1 (UNIT_CLASS_WARRIOR).", cInfo->Entry, cInfo->unit_class);
        const_cast<CreatureTemplate*>(cInfo)->unit_class = UNIT_CLASS_WARRIOR;
    }

    if (cInfo->dmgschool >= MAX_SPELL_SCHOOL)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid spell school value (%u) in `dmgschool`.", cInfo->Entry, cInfo->dmgschool);
        const_cast<CreatureTemplate*>(cInfo)->dmgschool = SPELL_SCHOOL_NORMAL;
    }

    if (cInfo->baseattacktime == 0)
        const_cast<CreatureTemplate*>(cInfo)->baseattacktime  = BASE_ATTACK_TIME;

    if (cInfo->rangeattacktime == 0)
        const_cast<CreatureTemplate*>(cInfo)->rangeattacktime = BASE_ATTACK_TIME;

    if ((cInfo->npcflag & UNIT_NPC_FLAG_TRAINER) && cInfo->trainer_type >= MAX_TRAINER_TYPE)
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong trainer type %u.", cInfo->Entry, cInfo->trainer_type);

    if (cInfo->dynamicflags & UNIT_DYNFLAG_DISABLE_INTERACTION)
    {
        const_cast<CreatureTemplate*>(cInfo)->dynamicflags = cInfo->dynamicflags &~UNIT_DYNFLAG_DISABLE_INTERACTION;
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has  UNIT_DYNFLAG_DISABLE_INTERACTION (dynamicflags: %u) remove it.", cInfo->Entry, cInfo->dynamicflags);
    }

    if (cInfo->speed_walk == 0.0f)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong value (%f) in speed_walk, set to 1.", cInfo->Entry, cInfo->speed_walk);
        const_cast<CreatureTemplate*>(cInfo)->speed_walk = 1.0f;
    }

    if (cInfo->speed_run == 0.0f)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong value (%f) in speed_run, set to 1.14286.", cInfo->Entry, cInfo->speed_run);
        const_cast<CreatureTemplate*>(cInfo)->speed_run = 1.14286f;
    }

    if (cInfo->Family && !sCreatureFamilyStore.LookupEntry(cInfo->Family) && cInfo->Family != CREATURE_FAMILY_HORSE_CUSTOM)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid creature family (%u) in `family`.", cInfo->Entry, cInfo->Family);
        const_cast<CreatureTemplate*>(cInfo)->Family = 0;
    }

    if (cInfo->InhabitType <= 0 || cInfo->InhabitType > INHABIT_ANYWHERE)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong value (%u) in `InhabitType`, creature will not correctly walk/swim/fly.", cInfo->Entry, cInfo->InhabitType);
        const_cast<CreatureTemplate*>(cInfo)->InhabitType = INHABIT_ANYWHERE;
    }

    if (cInfo->HoverHeight < 0.0f)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong value (%f) in `HoverHeight`", cInfo->Entry, cInfo->HoverHeight);
        const_cast<CreatureTemplate*>(cInfo)->HoverHeight = 1.0f;
    }

    if (cInfo->VehicleId)
    {
        VehicleEntry const* vehId = sVehicleStore.LookupEntry(cInfo->VehicleId);
        if (!vehId)
        {
             sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has a non-existing VehicleId (%u). This *WILL* cause the client to freeze!", cInfo->Entry, cInfo->VehicleId);
             const_cast<CreatureTemplate*>(cInfo)->VehicleId = 0;
        }
    }

    for (uint8 j = 0; j < CREATURE_MAX_SPELLS; ++j)
    {
        if (cInfo->spells[j] && !sSpellMgr->GetSpellInfo(cInfo->spells[j]))
        {
            WorldDatabase.PExecute("UPDATE creature_template SET spell%d = 0 WHERE entry = %u", j+1, cInfo->Entry);
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has non-existing Spell%d (%u), set to 0.", cInfo->Entry, j+1, cInfo->spells[j]);
            const_cast<CreatureTemplate*>(cInfo)->spells[j] = 0;
        }
    }

    if (cInfo->MovementType >= MAX_DB_MOTION_TYPE)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong movement generator type (%u), ignored and set to IDLE.", cInfo->Entry, cInfo->MovementType);
        const_cast<CreatureTemplate*>(cInfo)->MovementType = IDLE_MOTION_TYPE;
    }

    if (uint32 badFlags = (cInfo->flags_extra & ~CREATURE_FLAG_EXTRA_DB_ALLOWED))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `creature_template` lists creature (Entry: %u) with disallowed `flags_extra` %u, removing incorrect flag.", cInfo->Entry, badFlags);
        const_cast<CreatureTemplate*>(cInfo)->flags_extra &= CREATURE_FLAG_EXTRA_DB_ALLOWED;
    }

    if (cInfo->minlevel < 1 || cInfo->minlevel >= STRONG_MAX_LEVEL)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (ID: %u): MinLevel %i is not within [1, 255], value has been set to 1.", cInfo->Entry, cInfo->minlevel);
        const_cast<CreatureTemplate*>(cInfo)->minlevel = 1;
    }

    if (cInfo->maxlevel < 1 || cInfo->maxlevel >= STRONG_MAX_LEVEL)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (ID: %u): MaxLevel %i is not within [1, 255], value has been set to 1.", cInfo->Entry, cInfo->maxlevel);
        const_cast<CreatureTemplate*>(cInfo)->maxlevel = 1;
    }

    if (cInfo->RequiredExpansion == -1)
    {
        if (MAX_LEVEL > cInfo->minlevel)
            const_cast<CreatureTemplate*>(cInfo)->minlevel = MAX_LEVEL;
        if (MAX_LEVEL > cInfo->maxlevel)
            const_cast<CreatureTemplate*>(cInfo)->maxlevel = MAX_LEVEL;
        const_cast<CreatureTemplate*>(cInfo)->RequiredExpansion = CURRENT_EXPANSION;
    }
}

void ObjectMgr::LoadCreatureAddons()
{
    uint32 oldMSTime = getMSTime();

    //                                                0       1       2      3       4       5      6
    QueryResult result = WorldDatabase.Query("SELECT guid, path_id, mount, bytes1, bytes2, emote, auras FROM creature_addon");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature addon definitions. DB table `creature_addon` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        ObjectGuid::LowType guid = fields[0].GetUInt64();

        CreatureData const* creData = GetCreatureData(guid);
        if (!creData)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (GUID: " UI64FMTD ") does not exist but has a record in `creature_addon`", guid);
            continue;
        }

        CreatureAddon& creatureAddon = _creatureAddonStore[guid];

        creatureAddon.path_id = fields[1].GetUInt32();
        if (creData->movementType == WAYPOINT_MOTION_TYPE && !creatureAddon.path_id)
        {
            const_cast<CreatureData*>(creData)->movementType = IDLE_MOTION_TYPE;
            sLog->outError(LOG_FILTER_SQL, "Creature (GUID " UI64FMTD ") has movement type set to WAYPOINT_MOTION_TYPE but no path assigned", guid);
        }

        creatureAddon.mount   = fields[2].GetUInt32();
        creatureAddon.bytes1  = fields[3].GetUInt32();
        creatureAddon.bytes2  = fields[4].GetUInt32();
        creatureAddon.emote   = fields[5].GetUInt32();

        Tokenizer tokens(fields[6].GetString(), ' ');
        uint8 i = 0;
        creatureAddon.auras.resize(tokens.size());
        for (Tokenizer::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
        {
            SpellInfo const* AdditionalSpellInfo = sSpellMgr->GetSpellInfo(uint32(atol(*itr)));
            if (!AdditionalSpellInfo || AdditionalSpellInfo->HasAura(SPELL_AURA_CONTROL_VEHICLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (GUID: " UI64FMTD ") has wrong spell %u defined in `auras` field in `creature_addon`.", guid, uint32(atol(*itr)));
                continue;
            }
            creatureAddon.auras[i++] = uint32(atol(*itr));
        }

        if (creatureAddon.mount)
        {
            if (!sCreatureDisplayInfoStore.LookupEntry(creatureAddon.mount))
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (GUID: " UI64FMTD ") has invalid displayInfoId (%u) for mount defined in `creature_addon`", guid, creatureAddon.mount);
                creatureAddon.mount = 0;
            }
        }

        if (!sEmotesStore.LookupEntry(creatureAddon.emote))
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (GUID: " UI64FMTD ") has invalid emote (%u) defined in `creature_addon`.", guid, creatureAddon.emote);
            creatureAddon.emote = 0;
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature addons in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

CreatureAddon const* ObjectMgr::GetCreatureAddon(ObjectGuid::LowType const& lowguid)
{
    CreatureAddonContainer::const_iterator itr = _creatureAddonStore.find(lowguid);
    if (itr != _creatureAddonStore.end())
        return &(itr->second);

    return NULL;
}

CreatureAddon const* ObjectMgr::GetCreatureTemplateAddon(uint32 entry)
{
    CreatureTemplateAddonContainer::const_iterator itr = _creatureTemplateAddonStore.find(entry);
    if (itr != _creatureTemplateAddonStore.end())
        return &(itr->second);

    return NULL;
}

CreatureEvadeWmoData const* ObjectMgr::GetCreatureEvadeWmoData(uint32 entry)
{
    CreatureEvadeWmoDataContainer::const_iterator itr = _creatureEvadeWmoDataStore.find(entry);
    if (itr != _creatureEvadeWmoDataStore.end())
        return &(itr->second);

    return NULL;
}

EquipmentInfo const* ObjectMgr::GetEquipmentInfo(uint32 entry, int8& id)
{
    EquipmentInfoContainer::const_iterator itr = _equipmentInfoStore.find(entry);
    if (itr == _equipmentInfoStore.end())
        return NULL;

    if (itr->second.empty())
        return NULL;

    if (id == -1) // select a random element
    {
        EquipmentInfoContainerInternal::const_iterator ritr = itr->second.begin();
        std::advance(ritr, urand(0u, itr->second.size() - 1));
        id = std::distance(itr->second.begin(), ritr) + 1;
        return &ritr->second;
    }
    else
    {
        EquipmentInfoContainerInternal::const_iterator itr2 = itr->second.find(id);
        if (itr2 != itr->second.end())
            return &itr2->second;
    }

    return NULL;
}

void ObjectMgr::LoadEquipmentTemplates()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0     1       2           3           4
    QueryResult result = WorldDatabase.Query("SELECT CreatureID, ID, ItemID1, ItemID2, ItemID3 FROM creature_equip_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature equipment templates. DB table `creature_equip_template` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        uint8 id = fields[1].GetUInt8();

        EquipmentInfo& equipmentInfo = _equipmentInfoStore[entry][id];

        equipmentInfo.ItemEntry[0] = fields[2].GetUInt32();
        equipmentInfo.ItemEntry[1] = fields[3].GetUInt32();
        equipmentInfo.ItemEntry[2] = fields[4].GetUInt32();

        for (uint8 i = 0; i < MAX_EQUIPMENT_ITEMS; ++i)
        {
            if (!equipmentInfo.ItemEntry[i])
                continue;

            ItemEntry const* dbcItem = sItemStore.LookupEntry(equipmentInfo.ItemEntry[i]);

            if (!dbcItem)
            {
                sLog->outError(LOG_FILTER_SQL, "Unknown item (entry=%u) in creature_equip_template.itemEntry%u for entry = %u, forced to 0.",
                    equipmentInfo.ItemEntry[i], i+1, entry);
                equipmentInfo.ItemEntry[i] = 0;
                continue;
            }

            if (dbcItem->InventoryType != INVTYPE_WEAPON &&
                dbcItem->InventoryType != INVTYPE_SHIELD &&
                dbcItem->InventoryType != INVTYPE_RANGED &&
                dbcItem->InventoryType != INVTYPE_2HWEAPON &&
                dbcItem->InventoryType != INVTYPE_WEAPONMAINHAND &&
                dbcItem->InventoryType != INVTYPE_WEAPONOFFHAND &&
                dbcItem->InventoryType != INVTYPE_HOLDABLE &&
                dbcItem->InventoryType != INVTYPE_THROWN &&
                dbcItem->InventoryType != INVTYPE_RANGEDRIGHT)
            {
                sLog->outError(LOG_FILTER_SQL, "Item (entry=%u) in creature_equip_template.itemEntry%u for entry = %u is not equipable in a hand, forced to 0.",
                    equipmentInfo.ItemEntry[i], i+1, entry);
                equipmentInfo.ItemEntry[i] = 0;
            }
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u equipment templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

CreatureModelInfo const* ObjectMgr::GetCreatureModelInfo(uint32 modelId)
{
    CreatureModelContainer::const_iterator itr = _creatureModelStore.find(modelId);
    if (itr != _creatureModelStore.end())
        return &(itr->second);

    return NULL;
}

uint32 ObjectMgr::ChooseDisplayId(uint32 /*team*/, const CreatureTemplate* cinfo, const CreatureData* data /*= NULL*/)
{
    // Load creature model (display id)
    uint32 display_id = 0;

    if (!data || data->displayid == 0)
    {
        display_id = cinfo->GetRandomValidModelId();
    }
    else
        return data->displayid;

    return display_id;
}

void ObjectMgr::ChooseCreatureFlags(const CreatureTemplate* cinfo, uint32& npcflag, uint32& npcflag2, uint32& unit_flags, uint32& dynamicflags, const CreatureData* data /*= NULL*/)
{
    npcflag = cinfo->npcflag;
    npcflag2 = cinfo->npcflag2;
    unit_flags = cinfo->unit_flags;
    dynamicflags = cinfo->dynamicflags;

    if (data)
    {
        if (data->npcflag)
            npcflag = data->npcflag;

        if (data->npcflag2)
            npcflag2 = data->npcflag2;

        if (data->unit_flags)
            unit_flags = data->unit_flags;

        if (data->dynamicflags)
            dynamicflags = data->dynamicflags;
    }
}

CreatureModelInfo const* ObjectMgr::GetCreatureModelRandomGender(uint32* displayID)
{
    CreatureModelInfo const* modelInfo = GetCreatureModelInfo(*displayID);
    if (!modelInfo)
        return NULL;

    // If a model for another gender exists, 50% chance to use it
    if (modelInfo->displayId_other_gender != 0 && urand(0, 1) == 0)
    {
        CreatureModelInfo const* minfo_tmp = GetCreatureModelInfo(modelInfo->displayId_other_gender);
        if (!minfo_tmp)
            sLog->outError(LOG_FILTER_SQL, "Model (Entry: %u) has displayId_other_gender %u not found in table `creature_model_info`. ", *displayID, modelInfo->displayId_other_gender);
        else
        {
            // DisplayID changed
            *displayID = modelInfo->displayId_other_gender;
            return minfo_tmp;
        }
    }

    return modelInfo;
}

void ObjectMgr::LoadCreatureModelInfo()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT DisplayID, BoundingRadius, CombatReach, DisplayID_Other_Gender, hostileId FROM creature_model_info");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature model definitions. DB table `creature_model_info` is empty.");
        return;
    }

    _creatureModelStore.rehash(result->GetRowCount());
    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 displayId = fields[0].GetUInt32();

        CreatureDisplayInfoEntry const* creatureDisplay = sCreatureDisplayInfoStore.LookupEntry(displayId);
        if (!creatureDisplay)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has model for not existed display id (%u).", displayId);
            continue;
        }
 
        CreatureModelInfo& modelInfo = _creatureModelStore[displayId];

        modelInfo.bounding_radius      = fields[1].GetFloat();
        modelInfo.combat_reach         = fields[2].GetFloat();
        modelInfo.gender               = creatureDisplay->Gender;
        modelInfo.displayId_other_gender = fields[3].GetUInt32();
        modelInfo.hostileId            = fields[4].GetUInt32();

        // Checks
        if (modelInfo.gender > GENDER_NONE || modelInfo.gender == GENDER_UNKNOWN)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has wrong gender (%u) for display id (%u).", uint32(modelInfo.gender), displayId);
            modelInfo.gender = GENDER_MALE;
        }

        if (modelInfo.displayId_other_gender && !sCreatureDisplayInfoStore.LookupEntry(modelInfo.displayId_other_gender))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has not existed alt.gender model (%u) for existed display id (%u).", modelInfo.displayId_other_gender, displayId);
            modelInfo.displayId_other_gender = 0;
        }

        if (modelInfo.hostileId && !sCreatureDisplayInfoStore.LookupEntry(modelInfo.hostileId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has not existed alt.hostileId model (%u) for existed display id (%u).", modelInfo.hostileId, displayId);
            modelInfo.hostileId = 0;
        }

        if (modelInfo.combat_reach < 0.1f)
            modelInfo.combat_reach = DEFAULT_COMBAT_REACH;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature model based info in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadLinkedRespawn()
{
    uint32 oldMSTime = getMSTime();

    _linkedRespawnStore.clear();
    //                                                 0        1          2
    QueryResult result = WorldDatabase.Query("SELECT guid, linkedGuid, linkType FROM linked_respawn ORDER BY guid ASC");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 linked respawns. DB table `linked_respawn` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        ObjectGuid::LowType guidLow = fields[0].GetUInt64();
        ObjectGuid::LowType linkedGuidLow = fields[1].GetUInt64();
        uint8  linkType = fields[2].GetUInt8();

        ObjectGuid guid, linkedGuid;
        bool error = false;
        switch (linkType)
        {
            case CREATURE_TO_CREATURE:
            {
                const CreatureData* slave = GetCreatureData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow " UI64FMTD "", guidLow);
                    error = true;
                    break;
                }

                const CreatureData* master = GetCreatureData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow " UI64FMTD "", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '" UI64FMTD "' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '" UI64FMTD "' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = ObjectGuid::Create<HighGuid::Creature>(slave->mapid, slave->id, guidLow);
                linkedGuid = ObjectGuid::Create<HighGuid::Creature>(master->mapid, master->id, linkedGuidLow);
                break;
            }
            case CREATURE_TO_GO:
            {
                const CreatureData* slave = GetCreatureData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow " UI64FMTD "", guidLow);
                    error = true;
                    break;
                }

                const GameObjectData* master = GetGOData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow " UI64FMTD "", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '" UI64FMTD "' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '" UI64FMTD "' linking to '" UI64FMTD "' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = ObjectGuid::Create<HighGuid::Creature>(slave->mapid, slave->id, guidLow);
                linkedGuid = ObjectGuid::Create<HighGuid::GameObject>(master->mapid, master->id, linkedGuidLow);
                break;
            }
            case GO_TO_GO:
            {
                const GameObjectData* slave = GetGOData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow " UI64FMTD "", guidLow);
                    error = true;
                    break;
                }

                const GameObjectData* master = GetGOData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow " UI64FMTD "", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '" UI64FMTD "' linking to '" UI64FMTD "' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '" UI64FMTD "' linking to '" UI64FMTD "' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = ObjectGuid::Create<HighGuid::GameObject>(slave->mapid, slave->id, guidLow);
                linkedGuid = ObjectGuid::Create<HighGuid::GameObject>(master->mapid, master->id, linkedGuidLow);
                break;
            }
            case GO_TO_CREATURE:
            {
                const GameObjectData* slave = GetGOData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow " UI64FMTD "", guidLow);
                    error = true;
                    break;
                }

                const CreatureData* master = GetCreatureData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow " UI64FMTD "", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '" UI64FMTD "' linking to '" UI64FMTD "' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '" UI64FMTD "' linking to '" UI64FMTD "' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = ObjectGuid::Create<HighGuid::GameObject>(slave->mapid, slave->id, guidLow);
                linkedGuid = ObjectGuid::Create<HighGuid::Creature>(master->mapid, master->id, linkedGuidLow);
                break;
            }
        }

        if (!error)
            _linkedRespawnStore[guid] = linkedGuid;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded " UI64FMTD " linked respawns in %u ms", uint64(_linkedRespawnStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::SetCreatureLinkedRespawn(ObjectGuid::LowType const& guidLow, ObjectGuid::LowType const& linkedGuidLow)
{
    if (!guidLow)
        return false;

    const CreatureData* master = GetCreatureData(guidLow);
    ObjectGuid guid = ObjectGuid::Create<HighGuid::Creature>(master->mapid, master->id, guidLow);

    if (!linkedGuidLow) // we're removing the linking
    {
        _linkedRespawnStore.erase(guid);
        PreparedStatement *stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_CRELINKED_RESPAWN);
        stmt->setUInt64(0, guidLow);
        WorldDatabase.Execute(stmt);
        return true;
    }

    const CreatureData* slave = GetCreatureData(linkedGuidLow);

    const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
    if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
    {
        sLog->outError(LOG_FILTER_SQL, "Creature '" UI64FMTD "' linking to '" UI64FMTD "' on an unpermitted map.", guidLow, linkedGuidLow);
        return false;
    }

    if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
    {
        sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '" UI64FMTD "' linking to '" UI64FMTD "' with not corresponding spawnMask", guidLow, linkedGuidLow);
        return false;
    }

    ObjectGuid linkedGuid = ObjectGuid::Create<HighGuid::Creature>(slave->mapid, slave->id, linkedGuidLow);

    _linkedRespawnStore[guid] = linkedGuid;
    PreparedStatement *stmt = WorldDatabase.GetPreparedStatement(WORLD_REP_CREATURE_LINKED_RESPAWN);
    stmt->setUInt64(0, guidLow);
    stmt->setUInt64(1, linkedGuidLow);
    WorldDatabase.Execute(stmt);
    return true;
}

void ObjectMgr::LoadTempSummons()
{
    uint32 oldMSTime = getMSTime();

    _tempSummonDataStore.clear();   // needed for reload case

    //                                               0           1             2        3      4           5           6           7              8        9          10         11          12
    QueryResult result = WorldDatabase.Query("SELECT summonerId, summonerType, groupId, entry, position_x, position_y, position_z, orientation, count, actionType, distance, summonType, summonTime FROM creature_summon_groups");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 temp summons. DB table `creature_summon_groups` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 summonerId               = fields[0].GetUInt32();
        SummonerType summonerType       = SummonerType(fields[1].GetUInt8());
        uint8 group                     = fields[2].GetUInt8();

        switch (summonerType)
        {
            case SUMMONER_TYPE_CREATURE:
                if (!GetCreatureTemplate(summonerId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has summoner with non existing entry %u for creature summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            case SUMMONER_TYPE_GAMEOBJECT:
                if (!GetGameObjectTemplate(summonerId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has summoner with non existing entry %u for gameobject summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            case SUMMONER_TYPE_MAP:
                if (!sMapStore.LookupEntry(summonerId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has summoner with non existing entry %u for map summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            default:
                sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has unhandled summoner type %u for summoner %u, skipped.", summonerType, summonerId);
                continue;
        }

        TempSummonData data;
        data.entry                      = fields[3].GetUInt32();

        if (!GetCreatureTemplate(data.entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has creature in group [Summoner ID: %u, Summoner Type: %u, Group ID: %u] with non existing creature entry %u, skipped.", summonerId, summonerType, group, data.entry);
            continue;
        }

        float posX                      = fields[4].GetFloat();
        float posY                      = fields[5].GetFloat();
        float posZ                      = fields[6].GetFloat();
        float orientation               = fields[7].GetFloat();

        data.pos.Relocate(posX, posY, posZ, orientation);

        data.count                      = fields[8].GetUInt8();
        data.actionType                 = fields[9].GetUInt8();
        data.distance                   = fields[10].GetFloat();
        data.sumType                    = TempSummonType(fields[11].GetUInt8());

        if (data.sumType > TEMPSUMMON_MANUAL_DESPAWN)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has unhandled temp summon type %u in group [Summoner ID: %u, Summoner Type: %u, Group ID: %u] for creature entry %u, skipped.", data.sumType, summonerId, summonerType, group, data.entry);
            continue;
        }

        data.time                       = fields[12].GetUInt32();

        TempSummonGroupKey key(summonerId, summonerType, group);
        _tempSummonDataStore[key].push_back(data);

        ++count;

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u temp summons in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

std::vector<TempSummonData> const* ObjectMgr::GetSummonGroup(uint32 summonerId, SummonerType summonerType, uint8 group) const
{
    TempSummonGroupKey groupKey = TempSummonGroupKey(summonerId, summonerType, group);

    TempSummonDataContainer::const_iterator itr = _tempSummonDataStore.find(groupKey);
    if (itr != _tempSummonDataStore.end())
        return &itr->second;

    return NULL;
}

void ObjectMgr::LoadCreatures()
{
    uint32 oldMSTime = getMSTime();

    //                                               0              1   2       3      4       5           6           7           8            9            10            11          12
    QueryResult result = WorldDatabase.Query("SELECT creature.guid, id, map, zoneId, areaId, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, "
    //        13            14         15       16            17         18         19          20          21                22                   23                     24                    25                  26
        "currentwaypoint, curhealth, curmana, MovementType, spawnMask, phaseMask, eventEntry, pool_entry, creature.npcflag, creature.npcflag2, creature.unit_flags, creature.dynamicflags, creature.isActive, creature.PhaseId "
        "FROM creature "
        "LEFT OUTER JOIN game_event_creature ON creature.guid = game_event_creature.guid "
        "LEFT OUTER JOIN pool_creature ON creature.guid = pool_creature.guid "
        "ORDER BY `map` ASC, `guid` ASC");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creatures. DB table `creature` is empty.");
        return;
    }

    // Build single time for check spawnmask
    std::map<uint32, uint32> spawnMasks;
    for (auto& mapDifficultyPair : sDB2Manager._mapDifficulty)
        for (auto& difficultyPair : mapDifficultyPair.second)
            spawnMasks[mapDifficultyPair.first] |= (1 << difficultyPair.first);

    _creatureDataStore.rehash(result->GetRowCount());
    std::map<uint32, CreatureData*> lastEntryCreature;

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        ObjectGuid::LowType guid = fields[index++].GetUInt64();
        uint32 entry        = fields[index++].GetUInt32();

        CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
        if (!cInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` has creature (GUID: " UI64FMTD ") with non existing creature entry %u, skipped.", guid, entry);
            continue;
        }

        CreatureData& data = _creatureDataStore[guid];
        data.guid           = guid;
        data.id             = entry;
        data.mapid          = fields[index++].GetUInt16();
        data.zoneId         = fields[index++].GetUInt16();
        data.areaId         = fields[index++].GetUInt16();
        data.displayid      = fields[index++].GetUInt32();
        data.equipmentId    = fields[index++].GetInt8();
        data.posX           = fields[index++].GetFloat();
        data.posY           = fields[index++].GetFloat();
        data.posZ           = fields[index++].GetFloat();
        data.orientation    = fields[index++].GetFloat();
        data.spawntimesecs  = fields[index++].GetUInt32();
        data.spawndist      = fields[index++].GetFloat();
        data.currentwaypoint= fields[index++].GetUInt32();
        data.curhealth      = fields[index++].GetUInt32();
        data.curmana        = fields[index++].GetUInt32();
        data.movementType   = fields[index++].GetUInt8();
        data.spawnMask      = fields[index++].GetUInt32();
        data.phaseMask      = fields[index++].GetUInt32();
        int16 gameEvent     = fields[index++].GetInt8();
        uint32 PoolId       = fields[index++].GetUInt32();
        data.npcflag        = fields[index++].GetUInt32();
        data.npcflag2       = fields[index++].GetUInt32();
        data.unit_flags     = fields[index++].GetUInt32();
        data.dynamicflags   = fields[index++].GetUInt32();
        data.isActive       = fields[index++].GetBool();

        Tokenizer phasesToken(fields[index++].GetString(), ' ', 100);
        for (Tokenizer::const_iterator::value_type itr : phasesToken)
            if (PhaseEntry const* phase = sPhaseStores.LookupEntry(uint32(strtoull(itr, nullptr, 10))))
                data.PhaseID.insert(phase->ID);

        // check near npc with same entry.
        auto lastCreature = lastEntryCreature.find(entry);
        if (lastCreature != lastEntryCreature.end())
        {
            if (data.mapid == lastCreature->second->mapid)
            {
                float dx1 = lastCreature->second->posX - data.posX;
                float dy1 = lastCreature->second->posY - data.posY;
                float dz1 = lastCreature->second->posZ - data.posZ;

                float distsq1 = dx1*dx1 + dy1*dy1 + dz1*dz1;
                if (distsq1 < 0.5f)
                {
                    // split phaseID
                    for (auto phaseID : data.PhaseID)
                        lastCreature->second->PhaseID.insert(phaseID);

                    lastCreature->second->phaseMask |= data.phaseMask;
                    lastCreature->second->spawnMask |= data.spawnMask;
                    WorldDatabase.PExecute("UPDATE creature SET phaseMask = %u, spawnMask = %u WHERE guid = %u", lastCreature->second->phaseMask, lastCreature->second->spawnMask, lastCreature->second->guid);
                    WorldDatabase.PExecute("DELETE FROM creature WHERE guid = %u", guid);
                    sLog->outError(LOG_FILTER_SQL, "Table `creature` have clone npc %u witch stay too close (dist: %f). original npc guid %u. npc with guid %u will be deleted.", entry, distsq1, lastCreature->second->guid, guid);
                    continue;
                }
            }else
                lastEntryCreature[entry] = &data;

        }else
            lastEntryCreature[entry] = &data;

        MapEntry const* mapEntry = sMapStore.LookupEntry(data.mapid);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD ") that spawned at not existed map (Id: %u), skipped.", guid, data.mapid);
            continue;
        }

        if (data.spawnMask & ~spawnMasks[data.mapid])
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD ") that have wrong spawn mask %u including not supported difficulty modes for map (Id: %u) spawnMasks[data.mapid]: %u.", guid, data.spawnMask, data.mapid, spawnMasks[data.mapid]);
            WorldDatabase.PExecute("UPDATE creature SET spawnMask = %u WHERE guid = %u", spawnMasks[data.mapid], guid);
        }

        // -1 random, 0 no equipment,
        if (data.equipmentId != 0)
        {
            if (!GetEquipmentInfo(data.id, data.equipmentId))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (Entry: %u) with equipment_id %u not found in table `creature_equip_template`, set to no equipment.", data.id, data.equipmentId);
                data.equipmentId = 0;
            }
        }

        if (cInfo->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
        {
            if (!mapEntry || !mapEntry->IsDungeon())
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD " Entry: %u) with `creature_template`.`flags_extra` including CREATURE_FLAG_EXTRA_INSTANCE_BIND but creature are not in instance.", guid, data.id);
        }

        if (data.spawndist < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD " Entry: %u) with `spawndist`< 0, set to 0.", guid, data.id);
            data.spawndist = 0.0f;
        }
        else if (data.movementType == RANDOM_MOTION_TYPE)
        {
            if (data.spawndist == 0.0f)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD " Entry: %u) with `MovementType`=1 (random movement) but with `spawndist`=0, replace by idle movement type (0).", guid, data.id);
                data.movementType = IDLE_MOTION_TYPE;
            }
        }
        else if (data.movementType == IDLE_MOTION_TYPE)
        {
            if (data.spawndist != 0.0f)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD " Entry: %u) with `MovementType`=0 (idle) have `spawndist`<>0, set to 0.", guid, data.id);
                data.spawndist = 0.0f;
            }
        }

        if (data.phaseMask == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: " UI64FMTD " Entry: %u) with `phaseMask`=0 (not visible for anyone), set to 1.", guid, data.id);
            data.phaseMask = 1;
        }

        // Add to grid if not managed by the game event or pool system
        if (gameEvent == 0 && PoolId == 0)
            AddCreatureToGrid(guid, &data);

        if (!data.zoneId || !data.areaId)
        {
            uint32 zoneId = 0;
            uint32 areaId = 0;

            sMapMgr->GetZoneAndAreaId(zoneId, areaId, data.mapid, data.posX, data.posY, data.posZ);
            WorldDatabase.PExecute("UPDATE creature SET zoneId = %u, areaId = %u WHERE guid = %u", zoneId, areaId, guid);
        }

        ++count;

    } while (result->NextRow());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creatures in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureAIInstance()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT entry, bossid, bossidtoactivete FROM creature_ai_instance");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature AI instance. DB table `creature_ai_instance` is empty.");
        return;
    }

    _creatureAIInstance.rehash(result->GetRowCount());
    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        CreatureAIInstance& aiinstance = _creatureAIInstance[entry];

        aiinstance.entry = entry;
        aiinstance.bossid = fields[1].GetUInt32();
        aiinstance.bossidactivete = fields[2].GetUInt32();

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature AI instance in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

    oldMSTime = getMSTime();

    result = WorldDatabase.Query("SELECT instanceId, gobId, bossId, doortype, boundary FROM creature_ai_instance_door");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature AI instance door. DB table `creature_ai_instance_door` is empty.");
        return;
    }

    _creatureAIInstanceGo.clear();
    _creatureAIInstanceDoor.clear();
    count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 instanceId = fields[0].GetUInt32();
        uint32 entry = fields[1].GetUInt32();

        DoorData doorData;
        DoorGoData doorGoData;

        doorData.entry = entry;
        doorData.bossId = fields[2].GetUInt32();
        doorData.type = DoorType(fields[3].GetUInt32());
        doorData.boundary = fields[4].GetUInt32();

        doorGoData.entry = entry;
        doorGoData.instanceId = instanceId;
        doorGoData.bossId = fields[2].GetUInt32();
        doorGoData.type = DoorType(fields[3].GetUInt32());
        doorGoData.boundary = fields[4].GetUInt32();

        _creatureAIInstanceDoor[instanceId].push_back(doorData);
        _creatureAIInstanceGo[entry].push_back(doorGoData);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature AI instance in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPersonalLootTemplate()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT entry, `type`, `chance`, lootspellId, bonusspellId, cooldownid, cooldowntype, respawn  FROM personal_loot_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 personal loot. DB table `personal_loot_template` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();
        uint32 type = fields[1].GetUInt8();
        uint32 chance = fields[2].GetUInt8();
        uint32 lootspellId = fields[3].GetUInt32();
        uint32 bonusspellId = fields[4].GetUInt32();
        uint32 cooldownid = fields[5].GetUInt32();
        uint32 ccooldowntype = fields[6].GetUInt8();
        uint32 respawn = fields[7].GetUInt8();

        PersonalLootData& personalloot = _PersonalLootStore[type][entry];
        personalloot.entry = entry;
        personalloot.type = type;
        personalloot.chance = chance;
        personalloot.lootspellId = lootspellId;
        personalloot.bonusspellId = bonusspellId;
        personalloot.cooldownid = cooldownid;
        personalloot.cooldowntype = ccooldowntype;
        personalloot.respawn = respawn;

        if(lootspellId)
        {
            PersonalLootData& personallootForSpell = _PersonalLootBySpellStore[lootspellId];
            personallootForSpell.entry = entry;
            personallootForSpell.type = type;
            personallootForSpell.chance = chance;
            personallootForSpell.lootspellId = lootspellId;
            personallootForSpell.bonusspellId = bonusspellId;
            personallootForSpell.cooldownid = cooldownid;
            personallootForSpell.cooldowntype = ccooldowntype;
            personallootForSpell.respawn = respawn;
        }
        if(bonusspellId)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(bonusspellId);
            if(!spellInfo || !spellInfo->Effects[0].TriggerSpell)
                continue;

            PersonalLootData& personallootForSpell = _PersonalLootBySpellStore[spellInfo->Effects[0].TriggerSpell];
            personallootForSpell.entry = entry;
            personallootForSpell.type = type;
            personallootForSpell.chance = chance;
            personallootForSpell.lootspellId = lootspellId;
            personallootForSpell.bonusspellId = bonusspellId;
            personallootForSpell.cooldownid = cooldownid;
            personallootForSpell.cooldowntype = ccooldowntype;
            personallootForSpell.respawn = respawn;
        }
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u personal loot in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddCreatureToGrid(ObjectGuid::LowType const& guid, CreatureData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.creatures.insert(guid);
        }
    }
}

void ObjectMgr::RemoveCreatureFromGrid(ObjectGuid::LowType const& guid, CreatureData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.creatures.erase(guid);
        }
    }
}

ObjectGuid::LowType ObjectMgr::AddGOData(uint32 entry, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay, float rotation0, float rotation1, float rotation2, float rotation3, uint32 aid /*= 0*/)
{
    GameObjectTemplate const* goinfo = GetGameObjectTemplate(entry);
    if (!goinfo)
        return UI64LIT(0);

    Map* map = sMapMgr->CreateBaseMap(mapId);
    if (!map)
        return UI64LIT(0);

    ObjectGuid::LowType guid = GetGenerator<HighGuid::GameObject>()->Generate();
    GameObjectData& data = NewGOData(guid);
    data.id             = entry;
    data.mapid          = mapId;
    data.posX           = x;
    data.posY           = y;
    data.posZ           = z;
    data.orientation    = o;
    data.rotation0      = rotation0;
    data.rotation1      = rotation1;
    data.rotation2      = rotation2;
    data.rotation3      = rotation3;
    data.spawntimesecs  = spawntimedelay;
    data.animprogress   = 100;
    data.spawnMask      = 1;
    data.go_state       = GO_STATE_READY;
    data.phaseMask      = PHASEMASK_NORMAL;
    data.artKit         = goinfo->type == GAMEOBJECT_TYPE_CONTROL_ZONE ? 21 : 0;
    data.dbData = false;
    data.AiID = aid;
    AddGameobjectToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    // We use spawn coords to spawn
    if (!map->Instanceable() && map->IsGridLoaded(x, y))
    {
        GameObject* go = new GameObject;
        if (!go->LoadGameObjectFromDB(guid, map))
        {
            sLog->outError(LOG_FILTER_GENERAL, "AddGOData: cannot add gameobject entry %u to map", entry);
            delete go;
            return UI64LIT(0);
        }
    }

    sLog->outDebug(LOG_FILTER_MAPS, "AddGOData: dbguid " UI64FMTD " entry %u map %u x %f y %f z %f o %f", guid, entry, mapId, x, y, z, o);

    return guid;
}

bool ObjectMgr::MoveCreData(ObjectGuid::LowType const& guid, uint32 mapId, Position pos)
{
    CreatureData& data = NewOrExistCreatureData(guid);
    if (!data.id)
        return false;

    RemoveCreatureFromGrid(guid, &data);
    if (data.posX == pos.GetPositionX() && data.posY == pos.GetPositionY() && data.posZ == pos.GetPositionZ())
        return true;
    data.posX = pos.GetPositionX();
    data.posY = pos.GetPositionY();
    data.posZ = pos.GetPositionZ();
    data.orientation = pos.GetOrientation();
    AddCreatureToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    if (Map* map = sMapMgr->CreateBaseMap(mapId))
    {
        // We use spawn coords to spawn
        if (!map->Instanceable() && map->IsGridLoaded(data.posX, data.posY))
        {
            Creature* creature = new Creature;
            if (!creature->LoadCreatureFromDB(guid, map))
            {
                sLog->outError(LOG_FILTER_GENERAL, "AddCreature: cannot add creature entry %u to map", guid);
                delete creature;
                return false;
            }
        }
    }
    return true;
}

ObjectGuid::LowType ObjectMgr::AddCreData(uint32 entry, uint32 /*team*/, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay)
{
    CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
    if (!cInfo)
        return UI64LIT(0);

    uint32 level = cInfo->minlevel == cInfo->maxlevel ? cInfo->minlevel : urand(cInfo->minlevel, cInfo->maxlevel); // Only used for extracting creature base stats
    CreatureBaseStats const* stats = GetCreatureBaseStats(level, cInfo->unit_class);

    ObjectGuid::LowType guid = GetGenerator<HighGuid::Creature>()->Generate();
    CreatureData& data = NewOrExistCreatureData(guid);
    data.id = entry;
    data.mapid = mapId;
    data.displayid = 0;
    data.equipmentId = 0;
    data.posX = x;
    data.posY = y;
    data.posZ = z;
    data.orientation = o;
    data.spawntimesecs = spawntimedelay;
    data.spawndist = 0;
    data.currentwaypoint = 0;
    data.curhealth = stats->GenerateHealth(cInfo);
    data.curmana = stats->GenerateMana(cInfo);
    data.movementType = cInfo->MovementType;
    data.spawnMask = 1;
    data.phaseMask = PHASEMASK_NORMAL;
    data.dbData = false;
    data.npcflag = cInfo->npcflag;
    data.npcflag2 = cInfo->npcflag2;
    data.unit_flags = cInfo->unit_flags;
    data.dynamicflags = cInfo->dynamicflags;

    AddCreatureToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    if (Map* map = sMapMgr->CreateBaseMap(mapId))
    {
        // We use spawn coords to spawn
        if (!map->Instanceable() && !map->IsRemovalGrid(x, y))
        {
            Creature* creature = new Creature;
            if (!creature->LoadCreatureFromDB(guid, map))
            {
                sLog->outError(LOG_FILTER_GENERAL, "AddCreature: cannot add creature entry %u to map", entry);
                delete creature;
                return UI64LIT(0);
            }
        }
    }

    return guid;
}

void ObjectMgr::LoadGameobjects()
{
    uint32 oldMSTime = getMSTime();

    uint32 count = 0;

    //                                                0                1   2    3         4           5           6        7           8
    QueryResult result = WorldDatabase.Query("SELECT gameobject.guid, id, map, zoneId, areaId, position_x, position_y, position_z, orientation, "
    //      9          10         11          12         13          14             15      16         17         18        19          20          21      22
        "rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, isActive, spawnMask, phaseMask, eventEntry, pool_entry, PhaseId, AiID "
        "FROM gameobject LEFT OUTER JOIN game_event_gameobject ON gameobject.guid = game_event_gameobject.guid "
        "LEFT OUTER JOIN pool_gameobject ON gameobject.guid = pool_gameobject.guid ORDER BY `map` ASC, `guid` ASC");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gameobjects. DB table `gameobject` is empty.");

        return;
    }

    // build single time for check spawnmask
    std::map<uint32, uint32> spawnMasks;
    for (auto& mapDifficultyPair : sDB2Manager._mapDifficulty)
        for (auto& difficultyPair : mapDifficultyPair.second)
            spawnMasks[mapDifficultyPair.first] |= (1 << difficultyPair.first);

    std::map<uint32, GameObjectData*> lastEntryGo;
    _gameObjectDataStore.rehash(result->GetRowCount());
    do
    {
        Field* fields = result->Fetch();

        ObjectGuid::LowType guid = fields[0].GetUInt64();
        uint32 entry        = fields[1].GetUInt32();

        GameObjectTemplate const* gInfo = GetGameObjectTemplate(entry);
        if (!gInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD ") with non existing gameobject entry %u, skipped.", guid, entry);
            continue;
        }

        if (!gInfo->displayId)
        {
            switch (gInfo->type)
            {
                case GAMEOBJECT_TYPE_TRAP:
                case GAMEOBJECT_TYPE_SPELL_FOCUS:
                    break;
                default:
                    sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: " UI64FMTD " Entry %u GoType: %u) doesn't have a displayId (%u), not loaded.", guid, entry, gInfo->type, gInfo->displayId);
                    break;
            }
        }

        if (gInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(gInfo->displayId))
        {
            sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: " UI64FMTD " Entry %u GoType: %u) has an invalid displayId (%u), not loaded.", guid, entry, gInfo->type, gInfo->displayId);
            continue;
        }

        if (gInfo->type == GAMEOBJECT_TYPE_GARRISON_BUILDING || gInfo->type ==  GAMEOBJECT_TYPE_GARRISON_PLOT)
        {
            sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: " UI64FMTD " Entry %u GoType: %u) is GAMEOBJECT_TYPE_GARRISON_BUILDING or GAMEOBJECT_TYPE_GARRISON_PLOT, not loaded.", guid, entry, gInfo->type);
            continue;
        }

        GameObjectData& data = _gameObjectDataStore[guid];

        data.id             = entry;
        data.guid           = guid;
        data.mapid          = fields[2].GetUInt16();
        data.zoneId         = fields[3].GetUInt16();
        data.areaId         = fields[4].GetUInt16();
        data.posX           = fields[5].GetFloat();
        data.posY           = fields[6].GetFloat();
        data.posZ           = fields[7].GetFloat();
        data.orientation    = fields[8].GetFloat();
        data.rotation0      = fields[9].GetFloat();
        data.rotation1      = fields[10].GetFloat();
        data.rotation2      = fields[11].GetFloat();
        data.rotation3      = fields[12].GetFloat();
        data.spawntimesecs  = fields[13].GetInt32();
        data.AiID           = fields[22].GetInt32();

        MapEntry const* mapEntry = sMapStore.LookupEntry(data.mapid);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) spawned on a non-existed map (Id: %u), skip", guid, data.id, data.mapid);
            continue;
        }

        if (!data.zoneId || !data.areaId)
        {
            uint32 zoneId = 0;
            uint32 areaId = 0;

            sMapMgr->GetZoneAndAreaId(zoneId, areaId, data.mapid, data.posX, data.posY, data.posZ);
            WorldDatabase.PExecute("UPDATE gameobject SET zoneId = %u, areaId = %u WHERE guid = %u", zoneId, areaId, guid);
        }

        if (data.spawntimesecs == 0 && gInfo->IsDespawnAtAction())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) with `spawntimesecs` (0) value, but the gameobejct is marked as despawnable at action.", guid, data.id);
        }

        data.animprogress   = fields[14].GetUInt8();
        data.artKit         = 0;

        uint32 go_state     = fields[15].GetUInt8();
        if (go_state >= MAX_GO_STATE)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) with invalid `state` (%u) value, skip", guid, data.id, go_state);
            continue;
        }
        data.go_state       = GOState(go_state);

        data.isActive       = fields[16].GetBool();

        data.spawnMask      = fields[17].GetUInt32();

        if (data.spawnMask & ~spawnMasks[data.mapid])
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) that has wrong spawn mask %u including not supported difficulty modes for map (Id: %u), skip", guid, data.id, data.spawnMask, data.mapid);
            WorldDatabase.PExecute("UPDATE gameobject SET spawnMask = %u WHERE guid = %u", spawnMasks[data.mapid], guid);
        }

        data.phaseMask      = fields[18].GetUInt16();
        int16 gameEvent     = fields[19].GetInt8();
        uint32 PoolId        = fields[20].GetUInt32();

        if (data.rotation2 < -1.0f || data.rotation2 > 1.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) with invalid rotation2 (%f) value, skip", guid, data.id, data.rotation2);
            continue;
        }

        if (data.rotation3 < -1.0f || data.rotation3 > 1.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) with invalid rotation3 (%f) value, skip", guid, data.id, data.rotation3);
            continue;
        }

        Tokenizer phasesToken(fields[21].GetString(), ' ', 100);
        for (Tokenizer::const_iterator::value_type itr : phasesToken)
            if (PhaseEntry const* phase = sPhaseStores.LookupEntry(uint32(strtoull(itr, nullptr, 10))))
                data.PhaseID.insert(phase->ID);

        if (data.phaseMask == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: " UI64FMTD " Entry: %u) with `phaseMask`=0 (not visible for anyone), set to 1.", guid, data.id);
            data.phaseMask = 1;
        }

        // check near go with same entry.
        auto lastGo = lastEntryGo.find(entry);
        if (lastGo != lastEntryGo.end())
        {
            if (data.mapid == lastGo->second->mapid)
            {
                float dx1 = lastGo->second->posX - data.posX;
                float dy1 = lastGo->second->posY - data.posY;
                float dz1 = lastGo->second->posZ - data.posZ;

                float distsq1 = dx1*dx1 + dy1*dy1 + dz1*dz1;
                if (distsq1 < 0.5f)
                {
                    // split phaseID
                    for (auto phaseID : data.PhaseID)
                        lastGo->second->PhaseID.insert(phaseID);

                    lastGo->second->phaseMask |= data.phaseMask;
                    lastGo->second->spawnMask |= data.spawnMask;
                    WorldDatabase.PExecute("UPDATE gameobject SET phaseMask = %u, spawnMask = %u WHERE guid = %u", lastGo->second->phaseMask, lastGo->second->spawnMask, lastGo->second->guid);
                    WorldDatabase.PExecute("DELETE FROM gameobject WHERE guid = %u", guid);
                    sLog->outError(LOG_FILTER_SQL, "Table `gameobject` have clone go %u witch stay too close (dist: %f). original go guid %u. go with guid %u will be deleted.", entry, distsq1, lastGo->second->guid, guid);
                    continue;
                }
            }
            else
                lastEntryGo[entry] = &data;

        }
        else
            lastEntryGo[entry] = &data;

        if (gameEvent == 0 && PoolId == 0)                      // if not this is to be managed by GameEvent System or Pool system
            AddGameobjectToGrid(guid, &data);
        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu gameobjects in %u ms", (unsigned long)_gameObjectDataStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddGameobjectToGrid(ObjectGuid::LowType const& guid, GameObjectData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.gameobjects.insert(guid);
        }
    }
}

void ObjectMgr::RemoveGameobjectFromGrid(ObjectGuid::LowType const& guid, GameObjectData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = Trinity::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.gameobjects.erase(guid);
        }
    }
}

Player* ObjectMgr::GetPlayerByLowGUID(ObjectGuid::LowType const& lowguid) const
{
    return ObjectAccessor::FindPlayer(ObjectGuid::Create<HighGuid::Player>(lowguid));
}

// name must be checked to correctness (if received) before call this function
ObjectGuid ObjectMgr::GetPlayerGUIDByName(std::string name)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    stmt->setString(0, name);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
        return ObjectGuid::Create<HighGuid::Player>((*result)[0].GetUInt32());

    return ObjectGuid::Empty;
}

bool ObjectMgr::GetPlayerNameByGUID(ObjectGuid const& guid, std::string& name)
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        name = player->GetName();
        return true;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_NAME);

    stmt->setUInt64(0, guid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        name = (*result)[0].GetString();
        return true;
    }

    return false;
}

uint32 ObjectMgr::GetPlayerTeamByGUID(ObjectGuid const& guid) const
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        return Player::TeamForRace(player->getRace());
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_RACE);

    stmt->setUInt64(0, guid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint8 race = (*result)[0].GetUInt8();
        return Player::TeamForRace(race);
    }

    return 0;
}

uint32 ObjectMgr::GetPlayerAccountIdByGUID(ObjectGuid const& guid)
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        return player->GetSession()->GetAccountId();
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_GUID);

    stmt->setUInt64(0, guid.GetCounter());

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint32 acc = (*result)[0].GetUInt32();
        return acc;
    }

    return 0;
}

uint32 ObjectMgr::GetPlayerAccountIdByPlayerName(std::string const& name)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_NAME);

    stmt->setString(0, name);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint32 acc = (*result)[0].GetUInt32();
        return acc;
    }

    return 0;
}

void ObjectMgr::LoadItemLocales()
{
    uint32 oldMSTime = getMSTime();

    _itemLocaleStore.clear();                                 // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, name_loc1, description_loc1, name_loc2, description_loc2, name_loc3, description_loc3, name_loc4, description_loc4, name_loc5, description_loc5, name_loc6, description_loc6, name_loc7, description_loc7, name_loc8, description_loc8, name_loc9, description_loc9, name_loc10, description_loc10 FROM locales_item");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        ItemLocale& data = _itemLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            AddLocaleString(fields[1 + 2 * (i - 1)].GetString(), locale, data.Name);
            AddLocaleString(fields[1 + 2 * (i - 1) + 1].GetString(), locale, data.Description);
        }
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu Item locale strings in %u ms", (unsigned long)_itemLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

uint32 FillMaxDurability(uint32 itemClass, uint32 itemSubClass, uint32 inventoryType, uint32 quality, uint32 itemLevel)
{
    if (itemClass != ITEM_CLASS_ARMOR && itemClass != ITEM_CLASS_WEAPON)
        return 0;

    static float const qualityMultipliers[MAX_ITEM_QUALITY] =
    {
        1.0f, 1.0f, 1.0f, 1.17f, 1.37f, 1.68f, 0.0f, 0.0f
    };

    static float const armorMultipliers[MAX_INVTYPE] =
    {
        0.00f, // INVTYPE_NON_EQUIP
        0.59f, // INVTYPE_HEAD
        0.00f, // INVTYPE_NECK
        0.59f, // INVTYPE_SHOULDERS
        0.00f, // INVTYPE_BODY
        1.00f, // INVTYPE_CHEST
        0.35f, // INVTYPE_WAIST
        0.75f, // INVTYPE_LEGS
        0.49f, // INVTYPE_FEET
        0.35f, // INVTYPE_WRISTS
        0.35f, // INVTYPE_HANDS
        0.00f, // INVTYPE_FINGER
        0.00f, // INVTYPE_TRINKET
        0.00f, // INVTYPE_WEAPON
        1.00f, // INVTYPE_SHIELD
        0.00f, // INVTYPE_RANGED
        0.00f, // INVTYPE_CLOAK
        0.00f, // INVTYPE_2HWEAPON
        0.00f, // INVTYPE_BAG
        0.00f, // INVTYPE_TABARD
        1.00f, // INVTYPE_ROBE
        0.00f, // INVTYPE_WEAPONMAINHAND
        0.00f, // INVTYPE_WEAPONOFFHAND
        0.00f, // INVTYPE_HOLDABLE
        0.00f, // INVTYPE_AMMO
        0.00f, // INVTYPE_THROWN
        0.00f, // INVTYPE_RANGEDRIGHT
        0.00f, // INVTYPE_QUIVER
        0.00f, // INVTYPE_RELIC
    };

    static float const weaponMultipliers[MAX_ITEM_SUBCLASS_WEAPON] =
    {
        0.89f, // ITEM_SUBCLASS_WEAPON_AXE
        1.03f, // ITEM_SUBCLASS_WEAPON_AXE2
        0.77f, // ITEM_SUBCLASS_WEAPON_BOW
        0.77f, // ITEM_SUBCLASS_WEAPON_GUN
        0.89f, // ITEM_SUBCLASS_WEAPON_MACE
        1.03f, // ITEM_SUBCLASS_WEAPON_MACE2
        1.03f, // ITEM_SUBCLASS_WEAPON_POLEARM
        0.89f, // ITEM_SUBCLASS_WEAPON_SWORD
        1.03f, // ITEM_SUBCLASS_WEAPON_SWORD2
        0.89f, // ITEM_SUBCLASS_WEAPON_WARGLAIVES
        1.03f, // ITEM_SUBCLASS_WEAPON_STAFF
        0.00f, // ITEM_SUBCLASS_WEAPON_EXOTIC
        0.00f, // ITEM_SUBCLASS_WEAPON_EXOTIC2
        0.64f, // ITEM_SUBCLASS_WEAPON_FIST_WEAPON
        0.00f, // ITEM_SUBCLASS_WEAPON_MISCELLANEOUS
        0.64f, // ITEM_SUBCLASS_WEAPON_DAGGER
        0.64f, // ITEM_SUBCLASS_WEAPON_THROWN
        0.00f, // ITEM_SUBCLASS_WEAPON_SPEAR
        0.77f, // ITEM_SUBCLASS_WEAPON_CROSSBOW
        0.64f, // ITEM_SUBCLASS_WEAPON_WAND
        0.64f, // ITEM_SUBCLASS_WEAPON_FISHING_POLE
    };

    float levelPenalty = 1.0f;
    if (itemLevel <= 28)
        levelPenalty = 0.966f - float(28u - itemLevel) / 54.0f;

    if (itemClass == ITEM_CLASS_ARMOR)
    {
        if (inventoryType > INVTYPE_ROBE)
            return 0;

        return 5 * uint32(23.0f * qualityMultipliers[quality] * armorMultipliers[inventoryType] * levelPenalty + 0.5f);
    }

    return 5 * uint32(17.0f * qualityMultipliers[quality] * weaponMultipliers[itemSubClass] * levelPenalty + 0.5f);
};

void FillDisenchantFields(uint32* disenchantID, uint32* requiredDisenchantSkill, ItemTemplate const& itemTemplate)
{
    *disenchantID = 0;
    *(int32*)requiredDisenchantSkill = -1;
    if ((itemTemplate.Flags & (ITEM_PROTO_FLAG_CONJURED | ITEM_PROTO_FLAG_UNK6)) ||
        itemTemplate.Bonding == BIND_QUEST_ITEM || itemTemplate.Area || itemTemplate.Map ||
        itemTemplate.Stackable > 1 ||
        itemTemplate.Quality < ITEM_QUALITY_UNCOMMON || itemTemplate.Quality > ITEM_QUALITY_EPIC ||
        !(itemTemplate.Class == ITEM_CLASS_ARMOR || itemTemplate.Class == ITEM_CLASS_WEAPON) ||
        !(Item::GetSpecialPrice(&itemTemplate) || sItemCurrencyCostStore.LookupEntry(itemTemplate.ItemId)))
        return;

    for (ItemDisenchantLootEntry const* disenchant : sItemDisenchantLootStore)
    {
        if (disenchant->ItemClass == itemTemplate.Class &&
            disenchant->Quality == itemTemplate.Quality &&
            disenchant->MinLevel <= itemTemplate.ItemLevel &&
            disenchant->MaxLevel >= itemTemplate.ItemLevel)
        {
            if (disenchant->ID == 60 || disenchant->ID == 61)   // epic item disenchant ilvl range 66-99 (classic)
            {
                if (itemTemplate.RequiredLevel > 60 || itemTemplate.RequiredSkillRank > 300)
                    continue;                                   // skip to epic item disenchant ilvl range 90-199 (TBC)
            }
            else if (disenchant->ID == 66 || disenchant->ID == 67)  // epic item disenchant ilvl range 90-199 (TBC)
            {
                if (itemTemplate.RequiredLevel <= 60 || (itemTemplate.RequiredSkill && itemTemplate.RequiredSkillRank <= 300))
                    continue;
            }

            *disenchantID = disenchant->ID;
            *requiredDisenchantSkill = disenchant->SkillRequired;
            //return;
        }
    }
}

void ObjectMgr::LoadItemTemplates()
{
    uint32 oldMSTime = getMSTime();
    uint32 sparseCount = 0;
    uint32 dbCount = 0;

    for (ItemSparseEntry const* sparse : sItemSparseStore)
    {
        ItemEntry const* db2Data = sItemStore.LookupEntry(sparse->ID);
        if (!db2Data)
            continue;

        ItemTemplate& itemTemplate = _itemTemplateStore[sparse->ID];

        itemTemplate.ItemId = sparse->ID;
        itemTemplate.Class = db2Data->Class;
        itemTemplate.SubClass = db2Data->SubClass;
        itemTemplate.SoundOverrideSubclass = db2Data->SoundOverrideSubclass;
        itemTemplate.Name1 = sparse->Name->Str[DEFAULT_LOCALE];
        itemTemplate.FileDataID = db2Data->FileDataID;
        itemTemplate.GroupSoundsID = db2Data->GroupSoundsID;
        itemTemplate.Quality = sparse->Quality;
        itemTemplate.Flags = sparse->Flags[0];
        itemTemplate.Flags2 = sparse->Flags[1];
        itemTemplate.Flags3 = sparse->Flags[2];
        itemTemplate.Unk430_1 = sparse->UnkFloat1;
        itemTemplate.Unk430_2 = sparse->UnkFloat2;
        itemTemplate.BuyCount = sparse->BuyCount;
        itemTemplate.BuyPrice = sparse->BuyPrice;
        itemTemplate.SellPrice = sparse->SellPrice;
        itemTemplate._InventoryType = db2Data->InventoryType;
        itemTemplate.AllowableClass = sparse->AllowableClass;
        itemTemplate.AllowableRace = sparse->AllowableRace;
        itemTemplate.ItemLevel = sparse->ItemLevel < 1 ? 1 : sparse->ItemLevel;
        itemTemplate.RequiredLevel = sparse->RequiredLevel;
        itemTemplate.RequiredSkill = sparse->RequiredSkill;
        itemTemplate.RequiredSkillRank = sparse->RequiredSkillRank;
        itemTemplate.RequiredSpell = sparse->RequiredSpell;
        itemTemplate.RequiredHonorRank = sparse->RequiredHonorRank;
        itemTemplate.RequiredCityRank = sparse->RequiredCityRank;
        itemTemplate.RequiredReputationFaction = sparse->RequiredReputationFaction;
        itemTemplate.RequiredReputationRank = sparse->RequiredReputationRank;
        itemTemplate.MaxCount = sparse->MaxCount;
        itemTemplate.Stackable = sparse->Stackable;
        itemTemplate.ContainerSlots = sparse->ContainerSlots;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        {
            itemTemplate.ItemStat[i].ItemStatType = sparse->ItemStatType[i];
            itemTemplate.ItemStat[i].ItemStatValue = sparse->ItemStatValue[i];
            itemTemplate.ItemStat[i].ItemStatAllocation = sparse->ItemStatAllocation[i];
            itemTemplate.ItemStat[i].ItemStatSocketCostMultiplier = sparse->ItemStatSocketCostMultiplier[i];
        }

        itemTemplate.ScalingStatDistribution = sparse->ScalingStatDistribution;
        itemTemplate.DamageType = sparse->DamageType;
        itemTemplate.Delay = sparse->Delay;
        itemTemplate.RangedModRange = sparse->RangedModRange;
        itemTemplate.SpellPPMRate = 0.0f;
        itemTemplate.Bonding = sparse->Bonding;
        itemTemplate.Description = sparse->Description->Str[DEFAULT_LOCALE];
        itemTemplate.PageText = sparse->PageText;
        itemTemplate.LanguageID = sparse->LanguageID;
        itemTemplate.PageMaterial = sparse->PageMaterial;
        itemTemplate.StartQuest = sparse->StartQuest;
        itemTemplate.LockID = sparse->LockID;
        itemTemplate.Material = sparse->Material;
        itemTemplate.Sheath = sparse->Sheath;
        itemTemplate.RandomProperty = sparse->RandomProperty;
        itemTemplate.RandomSuffix = sparse->RandomSuffix;
        itemTemplate.ItemSet = sparse->ItemSet;
        itemTemplate.MaxDurability = FillMaxDurability(db2Data->Class, db2Data->SubClass, sparse->InventoryType, sparse->Quality, sparse->ItemLevel);
        itemTemplate.Area = sparse->Area;
        itemTemplate.Map = sparse->Map;
        itemTemplate.BagFamily = sparse->BagFamily;
        itemTemplate.TotemCategory = sparse->TotemCategory;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS; ++i)
        {
            itemTemplate.Socket[i].Color = sparse->SocketColor[i];
            // TODO: 6.x update/remove this
            itemTemplate.Socket[i].Content = 0;
        }

        itemTemplate.socketBonus = sparse->SocketBonus;
        itemTemplate.GemProperties = sparse->GemProperties;
        FillDisenchantFields(&itemTemplate.DisenchantID, &itemTemplate.RequiredDisenchantSkill, itemTemplate);

        itemTemplate.ArmorDamageModifier = sparse->ArmorDamageModifier;
        itemTemplate.Duration = sparse->Duration;
        itemTemplate.ItemLimitCategory = sparse->ItemLimitCategory;
        itemTemplate.HolidayId = sparse->HolidayID;
        itemTemplate.StatScalingFactor = sparse->StatScalingFactor;
        itemTemplate.CurrencySubstitutionId = sparse->CurrencySubstitutionID;
        itemTemplate.CurrencySubstitutionCount = sparse->CurrencySubstitutionCount;
        itemTemplate.ItemNameDescriptionID = sparse->ItemNameDescriptionID;
        itemTemplate.ScriptId = 0;
        itemTemplate.FoodType = 0;
        itemTemplate.MinMoneyLoot = 0;
        itemTemplate.MaxMoneyLoot = 0;
        itemTemplate.FlagsCu = 0;
        ++sparseCount;
        itemTemplate.ItemSpecExist = sDB2Manager.GetItemSpecsList(sparse->ID).empty() ? 0 : 1;

        if (std::vector<ItemSpecOverrideEntry const*> const* itemSpecOverrides = sDB2Manager.GetItemSpecOverrides(sparse->ID))
        {
            for (ItemSpecOverrideEntry const* itemSpecOverride : *itemSpecOverrides)
                itemTemplate.Specializations[0].insert(itemSpecOverride->SpecID);

            itemTemplate.Specializations[1] = itemTemplate.Specializations[0];
        }
        else
        {
            ItemSpecStats itemSpecStats(db2Data, sparse);

            if (itemSpecStats.ItemSpecStatCount)
            {
                for (ItemSpecEntry const* itemSpec : sItemSpecStore)
                {
                    if (itemSpecStats.ItemType != itemSpec->ItemType)
                        continue;

                    bool hasPrimary = false;
                    bool hasSecondary = itemSpec->SecondaryStat == ITEM_SPEC_STAT_NONE;
                    for (uint32 i = 0; i < itemSpecStats.ItemSpecStatCount; ++i)
                    {
                        if (itemSpecStats.ItemSpecStatTypes[i] == itemSpec->PrimaryStat)
                            hasPrimary = true;
                        if (itemSpecStats.ItemSpecStatTypes[i] == itemSpec->SecondaryStat)
                            hasSecondary = true;
                    }

                    if (!hasPrimary || !hasSecondary)
                        continue;

                    if (ChrSpecializationEntry const* specialization = sChrSpecializationStore.LookupEntry(itemSpec->SpecID))
                        if ((1 << (specialization->ClassID - 1)) & sparse->AllowableClass)
                            itemTemplate.Specializations[itemSpec->MaxLevel > 40].insert(itemSpec->SpecID);
                }
            }
        }

        // Mantid Amber Sliver
        if (itemTemplate.ItemId == 95373)
            if (CurrencyTypesEntry const* curr = sCurrencyTypesStore.LookupEntry(CURRENCY_TYPE_MANTID_ARCHAEOLOGY_FRAGMENT))
                itemTemplate.CurrencySubstitutionId = curr->SpellCategory;
    }

    for (ItemEffectEntry const* effectEntry : sItemEffectStore)
    {
        auto itemItr = _itemTemplateStore.find(effectEntry->ItemID);
        if (itemItr == _itemTemplateStore.end())
            continue;

        itemItr->second.Effects.push_back(effectEntry);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item templates from Item-sparse.db2 and %u from database in %u ms", sparseCount, dbCount, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemTemplateAddon()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT Id, FlagsCu, FoodType, MinMoneyLoot, MaxMoneyLoot, SpellPPMChance FROM item_template_addon");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (!GetItemTemplate(itemId))
            {
                sLog->outError(LOG_FILTER_SQL, "Item %u specified in `item_template_addon` does not exist, skipped.", itemId);
                continue;
            }

            uint32 minMoneyLoot = fields[3].GetUInt32();
            uint32 maxMoneyLoot = fields[4].GetUInt32();
            if (minMoneyLoot > maxMoneyLoot)
            {
                sLog->outError(LOG_FILTER_SQL, "Minimum money loot specified in `item_template_addon` for item %u was greater than maximum amount, swapping.", itemId);
                std::swap(minMoneyLoot, maxMoneyLoot);
            }
            ItemTemplate& itemTemplate = _itemTemplateStore[itemId];
            itemTemplate.FlagsCu = fields[1].GetUInt32();
            itemTemplate.FoodType = fields[2].GetUInt8();
            itemTemplate.MinMoneyLoot = minMoneyLoot;
            itemTemplate.MaxMoneyLoot = maxMoneyLoot;
            itemTemplate.SpellPPMRate = fields[5].GetFloat();
            ++count;
        } while (result->NextRow());
    }
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item addon templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemScriptNames()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT Id, ScriptName FROM item_script_names");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (!GetItemTemplate(itemId))
            {
                sLog->outError(LOG_FILTER_SQL, "Item %u specified in `item_script_names` does not exist, skipped.", itemId);
                continue;
            }

            _itemTemplateStore[itemId].ScriptId = GetScriptId(fields[1].GetCString());
            ++count;
        } while (result->NextRow());
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}
ItemTemplate const* ObjectMgr::GetItemTemplate(uint32 entry)
{
    ItemTemplateContainer::const_iterator itr = _itemTemplateStore.find(entry);
    if (itr != _itemTemplateStore.end())
        return &(itr->second);
    return NULL;
}

void ObjectMgr::LoadVehicleTemplateAccessories()
{
    uint32 oldMSTime = getMSTime();

    _vehicleTemplateAccessoryStore.clear();                           // needed for reload case

    uint32 count = 0;

    //                                                     0              1              2          3           4             5              6          7          8          9
    QueryResult result = WorldDatabase.Query("SELECT `EntryOrAura`, `accessory_entry`, `seat_id`, `minion`, `summontype`, `summontimer`, `offsetX`, `offsetY`, `offsetZ`, `offsetO` FROM `vehicle_template_accessory`");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 vehicle template accessories. DB table `vehicle_template_accessory` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        int32  uiEntryOrAura= fields[0].GetInt32();
        uint32 uiAccessory  = fields[1].GetUInt32();
        int8   uiSeat       = int8(fields[2].GetInt8());
        bool   bMinion      = fields[3].GetBool();
        uint8  uiSummonType = fields[4].GetUInt8();
        uint32 uiSummonTimer= fields[5].GetUInt32();
        float offsetX       = fields[6].GetFloat();
        float offsetY       = fields[7].GetFloat();
        float offsetZ       = fields[8].GetFloat();
        float offsetO       = fields[9].GetFloat();

        if (uiEntryOrAura > 0 && !sObjectMgr->GetCreatureTemplate(uiEntryOrAura))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: creature template entry %u does not exist.", uiEntryOrAura);
            continue;
        }

        if (uiEntryOrAura < 0 && !sSpellMgr->GetSpellInfo(uiEntryOrAura * -1))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: aura %u does not exist.", uiEntryOrAura);
            continue;
        }

        if (!sObjectMgr->GetCreatureTemplate(uiAccessory))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: Accessory %u does not exist.", uiAccessory);
            continue;
        }

        if (uiEntryOrAura > 0 && _spellClickInfoStore.find(uiEntryOrAura) == _spellClickInfoStore.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: creature template entry %u has no data in npc_spellclick_spells", uiEntryOrAura);
            continue;
        }

        _vehicleTemplateAccessoryStore[uiEntryOrAura].push_back(VehicleAccessory(uiAccessory, uiSeat, bMinion, uiSummonType, uiSummonTimer, Position{offsetX, offsetY, offsetZ, offsetO}));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Vehicle Template Accessories in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPetStats()
{
    uint32 oldMSTime = getMSTime();
    _petStatsStore.clear();

    //                                                 0      1     2       3        4          5           6        7            8           9       10      11           12         13
    QueryResult result = WorldDatabase.Query("SELECT entry, `hp`, `ap`, `ap_type`, `spd`, `school_mask`, `state`, `energy`, `energy_type`, `armor`, `type`, `damage`, `maxspdorap`, `haste` FROM pet_stats");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 pet stats. DB table `pet_stats` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Wrong creature id %u in `pet_stats` table, ignoring.", entry);
            continue;
        }

        PetStats stats;
        stats.hp = fields[1].GetFloat();
        stats.ap   = fields[2].GetFloat();
        stats.ap_type  = fields[3].GetInt32();
        stats.spd  = fields[4].GetFloat();
        stats.school_mask  = fields[5].GetInt32();
        stats.state  = fields[6].GetInt32();
        stats.energy  = fields[7].GetInt32();
        stats.energy_type  = fields[8].GetInt32();
        stats.armor  = fields[9].GetFloat();
        stats.type  = fields[10].GetInt32();
        stats.damage  = fields[11].GetFloat();
        stats.maxspdorap  = fields[12].GetInt32();
        stats.haste  = fields[13].GetInt32();
        _petStatsStore[entry] = stats;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pet stats definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

PetStats const* ObjectMgr::GetPetStats(uint32 creature_id) const
{
    PetStatsContainer::const_iterator itr = _petStatsStore.find(creature_id);
    return itr != _petStatsStore.end() ? &(itr->second) : NULL;
}

void ObjectMgr::PlayerCreateInfoAddItemHelper(uint32 race_, uint32 class_, uint32 itemId, int32 count)
{
    if (count > 0)
        _playerInfo[race_][class_]->item.push_back(PlayerCreateInfoItem(itemId, count));
    else
    {
        if (count < -1)
            sLog->outError(LOG_FILTER_SQL, "Invalid count %i specified on item %u be removed from original player create info (use -1)!", count, itemId);

        bool doneOne = false;
        for (CharStartOutfitEntry const* entry : sCharStartOutfitStore)
        {
            if (entry->ClassID == class_ && entry->RaceID == race_)
            {
                bool found = false;
                for (uint8 x = 0; x < MAX_OUTFIT_ITEMS; ++x)
                {
                    if (entry->ItemID[x] > 0 && uint32(entry->ItemID[x]) == itemId)
                    {
                        found = true;
                        const_cast<CharStartOutfitEntry*>(entry)->ItemID[x] = 0;
                        break;
                    }
                }

                if (!found)
                    sLog->outError(LOG_FILTER_SQL, "Item %u specified to be removed from original create info not found in dbc!", itemId);

                if (!doneOne)
                    doneOne = true;
                else
                    break;
            }
        }
    }
}

void ObjectMgr::LoadPlayerInfo()
{
    uint32 oldMSTime = getMSTime();
    {
        // Load playercreate
        //                                                0     1      2    3        4          5           6
        QueryResult result = WorldDatabase.Query("SELECT race, class, map, zone, position_x, position_y, position_z, orientation FROM playercreateinfo");

        if (!result)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 player create definitions. DB table `playercreateinfo` is empty.");
            exit(1);
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();

                uint32 current_race  = fields[0].GetUInt8();
                uint32 current_class = fields[1].GetUInt8();
                uint32 mapId         = fields[2].GetUInt16();
                uint32 areaId        = fields[3].GetUInt32(); // zone
                float  positionX     = fields[4].GetFloat();
                float  positionY     = fields[5].GetFloat();
                float  positionZ     = fields[6].GetFloat();
                float  orientation   = fields[7].GetFloat();

                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo` table, ignoring.", current_race);
                    continue;
                }

                ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(current_race);
                if (!rEntry)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo` table, ignoring.", current_race);
                    continue;
                }

                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo` table, ignoring.", current_class);
                    continue;
                }

                if (!sChrClassesStore.LookupEntry(current_class))
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo` table, ignoring.", current_class);
                    continue;
                }

                // accept DB data only for valid position (and non instanceable)
                if (!MapManager::IsValidMapCoord(mapId, positionX, positionY, positionZ, orientation))
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong home position for class %u race %u pair in `playercreateinfo` table, ignoring.", current_class, current_race);
                    continue;
                }

                if (sMapStore.LookupEntry(mapId)->Instanceable())
                {
                    sLog->outError(LOG_FILTER_SQL, "Home position in instanceable map for class %u race %u pair in `playercreateinfo` table, ignoring.", current_class, current_race);
                    continue;
                }

                PlayerInfo* info = new PlayerInfo();
                info->mapId = mapId;
                info->areaId = areaId;
                info->positionX = positionX;
                info->positionY = positionY;
                info->positionZ = positionZ;
                info->orientation = orientation;
                info->displayId_m = rEntry->MaleDisplayID;
                info->displayId_f = rEntry->FemaleDisplayID;
                _playerInfo[current_race][current_class] = info;

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u player create definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate items
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Items Data...");
    {
        oldMSTime = getMSTime();
        //                                                0     1      2       3
        QueryResult result = WorldDatabase.Query("SELECT race, class, itemid, amount FROM playercreateinfo_item");
        if (!result)
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 custom player create items. DB table `playercreateinfo_item` is empty.");
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo_item` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo_item` table, ignoring.", current_class);
                    continue;
                }

                uint32 item_id = fields[2].GetUInt32();
                if (!GetItemTemplate(item_id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Item id %u (race %u class %u) in `playercreateinfo_item` table but not listed in `item_template`, ignoring.", item_id, current_race, current_class);
                    continue;
                }

                int32 amount   = fields[3].GetUInt32();
                if (!amount)
                {
                    sLog->outError(LOG_FILTER_SQL, "Item id %u (class %u race %u) have amount == 0 in `playercreateinfo_item` table, ignoring.", item_id, current_race, current_class);
                    continue;
                }

                if (!current_race || !current_class)
                {
                    uint32 min_race = current_race ? current_race : 1;
                    uint32 max_race = current_race ? current_race + 1 : MAX_RACES;
                    uint32 min_class = current_class ? current_class : 1;
                    uint32 max_class = current_class ? current_class + 1 : MAX_CLASSES;
                    for (uint32 r = min_race; r < max_race; ++r)
                        for (uint32 c = min_class; c < max_class; ++c)
                            PlayerCreateInfoAddItemHelper(r, c, item_id, amount);
                }
                else
                    PlayerCreateInfoAddItemHelper(current_race, current_class, item_id, amount);

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u custom player create items in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Skill Data...");
    {
        oldMSTime = getMSTime();

        for (SkillRaceClassInfoEntry const* rcInfo : sSkillRaceClassInfoStore)
            if (rcInfo->Availability == 1)
                for (uint32 raceIndex = RACE_HUMAN; raceIndex < MAX_RACES; ++raceIndex)
                    if (rcInfo->RaceMask == -1 || ((1 << (raceIndex - 1)) & rcInfo->RaceMask))
                        for (uint32 classIndex = CLASS_WARRIOR; classIndex < MAX_CLASSES; ++classIndex)
                            if (rcInfo->ClassMask == -1 || ((1 << (classIndex - 1)) & rcInfo->ClassMask))
                                if (PlayerInfo* info = _playerInfo[raceIndex][classIndex])
                                    info->skills.push_back(rcInfo);

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded player create skills in %u s", GetMSTimeDiffToNow(oldMSTime) / IN_MILLISECONDS);
    }

    // Load playercreate actions
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Action Data...");
    {
        oldMSTime = getMSTime();
        //                                                0     1      2       3       4
        QueryResult result = WorldDatabase.Query("SELECT race, class, button, action, type FROM playercreateinfo_action");
        if (!result)
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 player create actions. DB table `playercreateinfo_action` is empty.");
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo_action` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo_action` table, ignoring.", current_class);
                    continue;
                }

                if (PlayerInfo* info = _playerInfo[current_race][current_class])
                    info->action.push_back(PlayerCreateInfoAction(fields[2].GetUInt16(), fields[3].GetUInt32(), fields[4].GetUInt16()));

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u player create actions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Loading levels data (class/race dependent)
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Level Stats Data...");
    {
        oldMSTime = getMSTime();
        //                                                 0     1      2      3    4    5    6    7
        QueryResult result = WorldDatabase.Query("SELECT race, class, level, str, agi, sta, inte, spi FROM player_levelstats");
        if (!result)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 level stats definitions. DB table `player_levelstats` is empty.");
            exit(1);
        }

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();

            uint32 current_race = fields[0].GetUInt8();
            if (current_race >= MAX_RACES)
            {
                sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `player_levelstats` table, ignoring.", current_race);
                continue;
            }

            uint32 current_class = fields[1].GetUInt8();
            if (current_class >= MAX_CLASSES)
            {
                sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `player_levelstats` table, ignoring.", current_class);
                continue;
            }

            uint32 current_level = fields[2].GetUInt8();
            if (current_level > MAX_LEVEL)
            {
                if (current_level > STRONG_MAX_LEVEL)        // hardcoded level maximum
                    sLog->outError(LOG_FILTER_SQL, "Wrong (> %u) level %u in `player_levelstats` table, ignoring.", STRONG_MAX_LEVEL, current_level);
                else
                {
                    sLog->outInfo(LOG_FILTER_GENERAL, "Unused (> MaxPlayerLevel in worldserver.conf) level %u in `player_levelstats` table, ignoring.", current_level);
                    ++count;                                // make result loading percent "expected" correct in case disabled detail mode for example.
                }
                continue;
            }

            if (PlayerInfo* info = _playerInfo[current_race][current_class])
            {
                if (!info->levelInfo)
                    info->levelInfo = new PlayerLevelInfo[MAX_LEVEL];

                PlayerLevelInfo& levelInfo = info->levelInfo[current_level - 1];
                for (uint8 i = 0; i < MAX_STATS; i++)
                    levelInfo.stats[i] = fields[i + 3].GetUInt16();
            }

            ++count;
        }
        while (result->NextRow());

        // Fill gaps and check integrity
        for (uint8 race = 0; race < MAX_RACES; ++race)
        {
            // skip non existed races
            if (!sChrRacesStore.LookupEntry(race))
                continue;

            for (uint8 class_ = 0; class_ < MAX_CLASSES; ++class_)
            {
                if (!sChrClassesStore.LookupEntry(class_))
                    continue;

                PlayerInfo* info = _playerInfo[race][class_];
                if (!info)
                    continue;

                if (CURRENT_EXPANSION < EXPANSION_THE_BURNING_CRUSADE && (race == RACE_BLOODELF || race == RACE_DRAENEI))
                    continue;

                if (CURRENT_EXPANSION < EXPANSION_WRATH_OF_THE_LICH_KING && class_ == CLASS_DEATH_KNIGHT)
                    continue;

                if (CURRENT_EXPANSION < EXPANSION_LEGION && class_ == CLASS_DEMON_HUNTER)
                    continue;

                uint8 level = 1;
                switch (class_)
                {
                    case CLASS_DEATH_KNIGHT:
                        level = START_DK_LEVEL;
                        break;
                    case CLASS_DEMON_HUNTER:
                        level = START_DH_LEVEL;
                        break;
                    default:
                        break;
                }

                if (!info->levelInfo || info->levelInfo[level - 1].stats[0] == 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Race %i Class %i does not have stats data for base lvl (%i)!", race, class_, level - 1);
                    exit(1);
                }

                for (level; level < MAX_LEVEL; ++level)
                {
                    if (info->levelInfo[level].stats[0] == 0)
                    {
                        sLog->outError(LOG_FILTER_SQL, "Race %i Class %i Level %i does not have stats data. Using stats data of level %i.", race, class_, level + 1, level);
                        info->levelInfo[level] = info->levelInfo[level - 1];
                    }
                }
            }
        }

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u level stats definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    // Loading xp per level data
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create XP Data...");
    {
        oldMSTime = getMSTime();

        _playerXPperLevel.resize(sGtOCTLevelExperienceStore.GetTableRowCount() + 1, 0);
        for (uint32 level = 0; level < sGtOCTLevelExperienceStore.GetTableRowCount(); ++level)
            _playerXPperLevel[level + 1] = sGtOCTLevelExperienceStore.EvaluateTable(level)->Value;

        //                                                 0        1
        QueryResult result  = WorldDatabase.Query("SELECT Level, Experience FROM player_xp_for_level");
        uint32 count = 0;
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();

                uint32 current_level = fields[0].GetUInt8();
                uint32 current_xp    = fields[1].GetUInt32();

                if (current_level >= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
                {
                    if (current_level > STRONG_MAX_LEVEL)        // hardcoded level maximum
                        sLog->outError(LOG_FILTER_SQL, "Wrong (> %u) level %u in `player_xp_for_level` table, ignoring.", STRONG_MAX_LEVEL, current_level);
                    else
                    {
                        sLog->outInfo(LOG_FILTER_GENERAL, "Unused (> MaxPlayerLevel in worldserver.conf) level %u in `player_xp_for_levels` table, ignoring.", current_level);
                        ++count;                                // make result loading percent "expected" correct in case disabled detail mode for example.
                    }
                    continue;
                }

                _playerXPperLevel[current_level] = current_xp;
                ++count;
            }
            while (result->NextRow());
        }

        // fill level gaps - only accounting levels > MAX_LEVEL
        for (uint8 level = 1; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
        {
            if (_playerXPperLevel[level] == 0)
            {
                sLog->outInfo(LOG_FILTER_GENERAL, "Level %i does not have XP for level data. Using data of level [%i] + 12000.", level + 1, level);
                _playerXPperLevel[level] = _playerXPperLevel[level - 1] + 12000;
            }
        }

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u xp for level definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
}

void ObjectMgr::GetPlayerClassLevelInfo(uint32 class_, uint8 level, uint32& baseHP, uint32& baseMana) const
{
    if (level < 1 || class_ >= MAX_CLASSES)
        return;

    level = std::min(level, MAX_LEVEL);

    GameTableEntry const* hp = sGtOCTBaseHPByClassStore.EvaluateTable(level - 1, class_ - 1);
    GameTableEntry const* mp = sGtOCTBaseMPByClassStore.EvaluateTable(level - 1, class_ - 1);
    if (!hp || !mp)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Tried to get non-existant Class-Level combination data for base hp/mp. Class %u Level %u", class_, level);
        return;
    }

    baseHP = uint32(hp->Value);
    baseMana = uint32(mp->Value);
}

void ObjectMgr::GetPlayerLevelInfo(uint32 race, uint32 class_, uint8 level, PlayerLevelInfo* info) const
{
    if (level < 1 || race >= MAX_RACES || class_ >= MAX_CLASSES)
        return;

    PlayerInfo const* pInfo = _playerInfo[race][class_];
    if (pInfo->displayId_m == 0 || pInfo->displayId_f == 0)
        return;

    if (level <= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        *info = pInfo->levelInfo[level-1];
    else
        BuildPlayerLevelInfo(race, class_, level, info);
}

void ObjectMgr::BuildPlayerLevelInfo(uint8 race, uint8 _class, uint8 level, PlayerLevelInfo* info) const
{
    // base data (last known level)
    *info = _playerInfo[race][_class]->levelInfo[sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)-1];

    // if conversion from uint32 to uint8 causes unexpected behaviour, change lvl to uint32
    for (uint8 lvl = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)-1; lvl < level; ++lvl)
    {
        switch (_class)
        {
            case CLASS_WARRIOR:
                info->stats[STAT_STRENGTH]  += (lvl > 23 ? 2: (lvl > 1  ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 23 ? 2: (lvl > 1  ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 36 ? 1: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 9 && !(lvl%2) ? 1: 0);
                break;
            case CLASS_PALADIN:
                info->stats[STAT_STRENGTH]  += (lvl > 3  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 33 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 1: (lvl > 7 && !(lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 6 && (lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 7 ? 1: 0);
                break;
            case CLASS_HUNTER:
                info->stats[STAT_STRENGTH]  += (lvl > 4  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 4  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 33 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 8 && (lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 1: (lvl > 9 && !(lvl%2) ? 1: 0));
                break;
            case CLASS_ROGUE:
            case CLASS_DEMON_HUNTER:
                info->stats[STAT_STRENGTH]  += (lvl > 5  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 4  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 16 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 8 && !(lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 1: (lvl > 9 && !(lvl%2) ? 1: 0));
                break;
            case CLASS_PRIEST:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 5  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 1: (lvl > 8 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 22 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 3  ? 1: 0);
                break;
            case CLASS_SHAMAN:
                info->stats[STAT_STRENGTH]  += (lvl > 34 ? 1: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 4 ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 7 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 5 ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 4 ? 1: 0);
                break;
            case CLASS_MAGE:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 5  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 24 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 33 ? 2: (lvl > 2 ? 1: 0));
                break;
            case CLASS_WARLOCK:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 38 ? 2: (lvl > 3 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 33 ? 2: (lvl > 2 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 2: (lvl > 3 ? 1: 0));
                break;
            case CLASS_DRUID:
                info->stats[STAT_STRENGTH]  += (lvl > 38 ? 2: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 32 ? 2: (lvl > 4 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 2: (lvl > 8 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 38 ? 3: (lvl > 4 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 3: (lvl > 5 ? 1: 0));
        }
    }
}

void ObjectMgr::LoadQuests()
{
    for (QuestMap::const_iterator::value_type itr : _questTemplates)
        delete itr.second;

    _questTemplates.clear();
    mExclusiveQuestGroups.clear();

    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT "
        //0  1          2           3               4         5            6            7                  8                9                   10                  11           12
        "ID, QuestType, QuestLevel, QuestPackageID, MinLevel, QuestSortID, QuestInfoID, SuggestedGroupNum, RewardNextQuest, RewardXPDifficulty, RevardXPMultiplier, RewardMoney, RewardMoneyDifficulty, "

        //     20                21               22                   23              24                    25         26            27                  28            29          30                   31            32          33
        "RewardMoneyMultiplier, RewardBonusMoney, RewardDisplaySpell, RewardSpell, RewardHonor, RewardKillHonor, "
        //         34               35               36              37          38         39       40      41 
        "StartItem, Flags, FlagsEx, RewardItem1, RewardAmount1, RewardItem2, RewardAmount2, RewardItem3, RewardAmount3, RewardItem4, RewardAmount4, "
        //      50      51                  52             53          54                55          56                  57
        "ItemDrop1, ItemDropQuantity1, ItemDrop2, ItemDropQuantity2, ItemDrop3, ItemDropQuantity3, ItemDrop4, ItemDropQuantity4, "
        //         58                   59                     60                          61                          62                  63                          64                          65                          66
        "RewardChoiceItemID1, RewardChoiceItemQuantity1, RewardChoiceItemDisplayID1, RewardChoiceItemID2, RewardChoiceItemQuantity2, RewardChoiceItemDisplayID2, RewardChoiceItemID3, RewardChoiceItemQuantity3, RewardChoiceItemDisplayID3, "
        //          67                  68                      69                          70                      71                      72                          73                      74                          75
        "RewardChoiceItemID4, RewardChoiceItemQuantity4, RewardChoiceItemDisplayID4, RewardChoiceItemID5, RewardChoiceItemQuantity5, RewardChoiceItemDisplayID5, RewardChoiceItemID6, RewardChoiceItemQuantity6, RewardChoiceItemDisplayID6, "
        //    76        77    78     79              80          81              82                 83              84                      85          86
        "POIContinent, POIx, POIy, POIPriority, RewardTitle, RewardTalents, RewardArenaPoints, RewardSkillLineID, RewardNumSkillUps, PortraitGiver, PortraitTurnIn, "
        //      87                  88                  89                      90                  91                  92                  93                  94                  95                      96                  97                      98                      99              100                 101
        "RewardFactionID1, RewardFactionValue1, RewardFactionOverride1, RewardFactionID2, RewardFactionValue2, RewardFactionOverride2, RewardFactionID3, RewardFactionValue3, RewardFactionOverride3, RewardFactionID4, RewardFactionValue4, RewardFactionOverride4, RewardFactionID5, RewardFactionValue5, RewardFactionOverride5, RewardFactionFlags, "
        //     168                  169                  170                 171             172                 173                 174                 175 
        "RewardCurrencyID1, RewardCurrencyQty1, RewardCurrencyID2, RewardCurrencyQty2, RewardCurrencyID3, RewardCurrencyQty3, RewardCurrencyID4, RewardCurrencyQty4, "
        //  176                     177              178          179         180         181          
        "AcceptedSoundKitID, CompleteSoundKitID, AreaGroupID, TimeAllowed, AllowableRaces, "
        //  182             183         184             185                 186             187                 188             189                 190                 191                 192
        "LogTitle, LogDescription, QuestDescription, AreaDescription, QuestCompletionLog, PortraitGiverText, PortraitGiverName, PortraitTurnInText, PortraitTurnInName, "
        //   215            216             217
        "StartScript, CompleteScript, VerifiedBuild"
        " FROM quest_template");
    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quests definitions. DB table `quest_template` is empty.");

        return;
    }

    // create multimap previous quest for each existed quest
    // some quests can have many previous maps set by NextQuestID in previous quest
    // for example set of race quests can lead to single not race specific quest
    do
    {
        Field* fields = result->Fetch();

        Quest* newQuest = new Quest(fields);
        _questTemplates[newQuest->GetQuestId()] = newQuest;
    } while (result->NextRow());

    // Load `quest_details` -  SMSG_QUESTGIVER_QUEST_DETAILS
    //                                   0   1       2       3       4       5            6            7            8
    result = WorldDatabase.Query("SELECT ID, Emote1, Emote2, Emote3, Emote4, EmoteDelay1, EmoteDelay2, EmoteDelay3, EmoteDelay4 FROM quest_details");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest details. DB table `quest_details` is empty.");
    }
    else
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 questId = fields[0].GetUInt32();

            auto itr = _questTemplates.find(questId);
            if (itr != _questTemplates.end())
                itr->second->LoadQuestDetails(fields);
            else
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_details` has data for quest %u but such quest does not exist", questId);
        } while (result->NextRow());
    }

    // Load `quest_request_items` - SMSG_QUESTGIVER_REQUEST_ITEMS
    //                                   0   1                2                  3                     4                       5
    result = WorldDatabase.Query("SELECT ID, EmoteOnComplete, EmoteOnIncomplete, EmoteOnCompleteDelay, EmoteOnIncompleteDelay, CompletionText FROM quest_request_items");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest request items. DB table `quest_request_items` is empty.");
    }
    else
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 questId = fields[0].GetUInt32();

            auto itr = _questTemplates.find(questId);
            if (itr != _questTemplates.end())
                itr->second->LoadQuestRequestItems(fields);
            else
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_request_items` has data for quest %u but such quest does not exist", questId);
        } while (result->NextRow());
    }

    // Load `quest_offer_reward` - SMSG_QUESTGIVER_OFFER_REWARD
    //                                   0   1       2       3       4       5            6            7            8            9
    result = WorldDatabase.Query("SELECT ID, Emote1, Emote2, Emote3, Emote4, EmoteDelay1, EmoteDelay2, EmoteDelay3, EmoteDelay4, RewardText FROM quest_offer_reward");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest reward emotes. DB table `quest_offer_reward` is empty.");
    }
    else
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 questId = fields[0].GetUInt32();

            auto itr = _questTemplates.find(questId);
            if (itr != _questTemplates.end())
                itr->second->LoadQuestOfferReward(fields);
            else
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_offer_reward` has data for quest %u but such quest does not exist", questId);
        } while (result->NextRow());
    }

    // Load `quest_template_addon`
    //                                   0   1         2                 3              4            5            6               7                     8
    result = WorldDatabase.Query("SELECT ID, MaxLevel, AllowableClasses, SourceSpellID, PrevQuestID, NextQuestID, ExclusiveGroup, RewardMailTemplateID, RewardMailDelay, "
        //9               10                   11                     12                     13                   14                   15                 16
        "RequiredSkillID, RequiredSkillPoints, RequiredMinRepFaction, RequiredMaxRepFaction, RequiredMinRepValue, RequiredMaxRepValue, ProvidedItemCount, SpecialFlags FROM quest_template_addon");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest template addons. DB table `quest_template_addon` is empty.");
    }
    else
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 questId = fields[0].GetUInt32();

            auto itr = _questTemplates.find(questId);
            if (itr != _questTemplates.end())
                itr->second->LoadQuestTemplateAddon(fields);
            else
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_template_addon` has data for quest %u but such quest does not exist", questId);
        } while (result->NextRow());
    }

    // Load `quest_objectives` order by descending storage index to reduce resizes
    //                                   0   1        2     3             4         5       6      7         8
    result = WorldDatabase.Query("SELECT ID, QuestID, Type, StorageIndex, ObjectID, Amount, Flags, UnkFloat, Description FROM quest_objectives ORDER BY StorageIndex ASC");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest objectives. DB table `quest_objectives` is empty.");
    }
    else
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 questId = fields[1].GetUInt32();

            auto itr = _questTemplates.find(questId);
            if (itr != _questTemplates.end())
                itr->second->LoadQuestObjective(fields);
            else
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_objectives` has objective for quest %u but such quest does not exist", questId);
        } while (result->NextRow());
    }

    // Load `quest_visual_effect` join table with quest_objectives because visual effects are based on objective ID (core stores objectives by their index in quest)
    //                                   0     1     2          3        4
    result = WorldDatabase.Query("SELECT v.ID, o.ID, o.QuestID, v.Index, v.VisualEffect FROM quest_visual_effect AS v LEFT JOIN quest_objectives AS o ON v.ID = o.ID ORDER BY v.Index DESC");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest visual effects. DB table `quest_visual_effect` is empty.");
    }
    else
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 vID = fields[0].GetUInt32();
            uint32 oID = fields[1].GetUInt32();

            if (!vID)
            {
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_visual_effect` has visual effect for null objective id");
                continue;
            }

            // objID will be null if match for table join is not found
            if (vID != oID)
            {
                sLog->outError(LOG_FILTER_SERVER_LOADING, "Table `quest_visual_effect` has visual effect for objective %u but such objective does not exist.", vID);
                continue;
            }

            uint32 questId = fields[2].GetUInt32();

            // Do not throw error here because error for non existing quest is thrown while loading quest objectives. we do not need duplication
            auto itr = _questTemplates.find(questId);
            if (itr != _questTemplates.end())
                itr->second->LoadQuestObjectiveVisualEffect(fields);
        } while (result->NextRow());
    }

    std::map<uint32, uint32> usedMailTemplates;
    for (QuestMap::iterator::value_type iter : _questTemplates)
    {
        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, iter.first, nullptr))
            continue;

        Quest* qinfo = iter.second;

        if (qinfo->Type >= MAX_QUEST_TYPES)
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `Method` = %u, expected values are 0, 1 or 2.", qinfo->GetQuestId(), qinfo->Type);

        if (qinfo->SpecialFlags & ~QUEST_SPECIAL_FLAGS_DB_ALLOWED)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `SpecialFlags` = %u > max allowed value. Correct `SpecialFlags` to value <= %u",
                           qinfo->GetQuestId(), qinfo->SpecialFlags, QUEST_SPECIAL_FLAGS_DB_ALLOWED);
            qinfo->SpecialFlags &= QUEST_SPECIAL_FLAGS_DB_ALLOWED;
        }

        if (qinfo->IsDaily() && qinfo->IsWeekly())
        {
            sLog->outError(LOG_FILTER_SQL, "Weekly Quest %u is marked as daily quest in `Flags`, removed daily flag.", qinfo->GetQuestId());
            qinfo->Flags &= ~QUEST_FLAGS_DAILY;
        }

        if (qinfo->IsDaily() && !qinfo->IsRepeatable())
        {
            sLog->outError(LOG_FILTER_SQL, "Daily Quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
            qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
        }

        if ((qinfo->Flags & QUEST_FLAGS_WEEKLY) && !qinfo->IsRepeatable())
        {
            sLog->outError(LOG_FILTER_SQL, "Weekly Quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
            qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
        }

        if (qinfo->Flags & QUEST_FLAGS_AUTO_REWARDED)
            for (uint8 j = 1; j < QUEST_REWARD_CHOICES_COUNT; ++j)
                if (uint32 id = qinfo->RewardChoiceItemId[j])
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = %u but item from `RewardChoiceItemId%d` can't be rewarded with quest flag QUEST_FLAGS_AUTO_REWARDED.",
                    qinfo->GetQuestId(), j + 1, id, j + 1);

        if (qinfo->MinLevel == uint32(-1) || qinfo->MinLevel > DEFAULT_MAX_LEVEL)
            sLog->outError(LOG_FILTER_SQL, "Quest %u should be disabled because `MinLevel` = %i", qinfo->GetQuestId(), int32(qinfo->MinLevel));

        if (qinfo->QuestSortID > 0)
            if (!sAreaTableStore.LookupEntry(qinfo->QuestSortID))
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `QuestSortID` = %u (zone case) but zone with this id does not exist.",
                qinfo->GetQuestId(), qinfo->QuestSortID);

        if (qinfo->QuestSortID < 0)
        {
            if (!sQuestSortStore.LookupEntry(-int32(qinfo->QuestSortID)))
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `QuestSortID` = %i (sort case) but quest sort with this id does not exist.",
                qinfo->GetQuestId(), qinfo->QuestSortID);

            if (uint32 skill_id = SkillByQuestSort(-int32(qinfo->QuestSortID)))
                if (qinfo->RequiredSkillId != skill_id)
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `QuestSortID` = %i but `RequiredSkillId` does not have a corresponding value (%d).",
                    qinfo->GetQuestId(), qinfo->QuestSortID, skill_id);
        }

        if (qinfo->AllowableClasses)
        {
            if (!(qinfo->AllowableClasses & CLASSMASK_ALL_PLAYABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u does not contain any playable classes in `AllowableClasses` (%u), value set to 0 (all classes).", qinfo->GetQuestId(), qinfo->AllowableClasses);
                qinfo->AllowableClasses = 0;
            }
        }

        if (qinfo->AllowableRaces != -1)
        {
            if (!(qinfo->AllowableRaces & RACEMASK_ALL_PLAYABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u does not contain any playable races in `AllowableRaces` (%u), value set to 0 (all races).", qinfo->GetQuestId(), qinfo->AllowableRaces);
                qinfo->AllowableRaces = -1;
            }
        }

        if (qinfo->RequiredSkillId)
            if (!sSkillLineStore.LookupEntry(qinfo->RequiredSkillId))
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredSkillId` = %u but this skill does not exist",
                qinfo->GetQuestId(), qinfo->RequiredSkillId);

        if (qinfo->RequiredSkillPoints && (qinfo->RequiredSkillPoints > sWorld->GetConfigMaxSkillValue()))
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredSkillPoints` = %u but max possible skill is %u, quest can't be done.",
            qinfo->GetQuestId(), qinfo->RequiredSkillPoints, sWorld->GetConfigMaxSkillValue());

        if (qinfo->RequiredMinRepFaction && !sFactionStore.LookupEntry(qinfo->RequiredMinRepFaction))
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMinRepFaction` = %u but faction template %u does not exist, quest can't be done.",
            qinfo->GetQuestId(), qinfo->RequiredMinRepFaction, qinfo->RequiredMinRepFaction);

        if (qinfo->RequiredMaxRepFaction && !sFactionStore.LookupEntry(qinfo->RequiredMaxRepFaction))
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMaxRepFaction` = %u but faction template %u does not exist, quest can't be done.",
            qinfo->GetQuestId(), qinfo->RequiredMaxRepFaction, qinfo->RequiredMaxRepFaction);

        if (qinfo->RequiredMinRepValue && qinfo->RequiredMinRepValue > ReputationMgr::Reputation_Cap)
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMinRepValue` = %d but max reputation is %u, quest can't be done.",
            qinfo->GetQuestId(), qinfo->RequiredMinRepValue, ReputationMgr::Reputation_Cap);

        if (qinfo->RequiredMinRepValue && qinfo->RequiredMaxRepValue && qinfo->RequiredMaxRepValue <= qinfo->RequiredMinRepValue)
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMaxRepValue` = %d and `RequiredMinRepValue` = %d, quest can't be done.",
            qinfo->GetQuestId(), qinfo->RequiredMaxRepValue, qinfo->RequiredMinRepValue);

        if (!qinfo->RequiredMinRepFaction && qinfo->RequiredMinRepValue != 0)
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMinRepValue` = %d but `RequiredMinRepFaction` is 0, value has no effect",
            qinfo->GetQuestId(), qinfo->RequiredMinRepValue);

        if (!qinfo->RequiredMaxRepFaction && qinfo->RequiredMaxRepValue != 0)
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMaxRepValue` = %d but `RequiredMaxRepFaction` is 0, value has no effect",
            qinfo->GetQuestId(), qinfo->RequiredMaxRepValue);

        if (qinfo->RewardTitleId && !sCharTitlesStore.LookupEntry(qinfo->RewardTitleId))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardTitleId` = %u but CharTitle Id %u does not exist, quest can't be rewarded with title.",
                           qinfo->GetQuestId(), qinfo->RewardTitleId, qinfo->RewardTitleId);
            qinfo->RewardTitleId = 0;
        }

        if (qinfo->SourceItemId)
        {
            if (!sObjectMgr->GetItemTemplate(qinfo->SourceItemId))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceItemId` = %u but item with entry %u does not exist, quest can't be done.",
                               qinfo->GetQuestId(), qinfo->SourceItemId, qinfo->SourceItemId);
                qinfo->SourceItemId = 0;
            }
            else if (qinfo->SourceItemIdCount == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceItemId` = %u but `SourceItemIdCount` = 0, set to 1 but need fix in DB.",
                               qinfo->GetQuestId(), qinfo->SourceItemId);
                qinfo->SourceItemIdCount = 1; // update to 1 for allow quest work for backward compatibility with DB
            }
        }
        else if (qinfo->SourceItemIdCount > 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceItemId` = 0 but `SourceItemIdCount` = %u, useless value.",
                           qinfo->GetQuestId(), qinfo->SourceItemIdCount);
            qinfo->SourceItemIdCount = 0;
        }

        if (qinfo->SourceSpellID)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->SourceSpellID);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceSpellID` = %u but spell %u doesn't exist, quest can't be done.",
                               qinfo->GetQuestId(), qinfo->SourceSpellID, qinfo->SourceSpellID);
                qinfo->SourceSpellID = 0;
            }
            else if (!SpellMgr::IsSpellValid(spellInfo))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceSpellID` = %u but spell %u is broken, quest can't be done.",
                               qinfo->GetQuestId(), qinfo->SourceSpellID, qinfo->SourceSpellID);
                qinfo->SourceSpellID = 0;
            }
        }

        for (uint8 j = 0; j < QUEST_ITEM_DROP_COUNT; ++j)
        {
            if (uint32 id = qinfo->ItemDrop[j])
            {
                if (!sObjectMgr->GetItemTemplate(id))
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `ItemDrop%d` = %u but item with entry %u does not exist, quest can't be done.",
                    qinfo->GetQuestId(), j + 1, id, id);
            }
            else if (qinfo->ItemDropQuantity[j] > 0)
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `ItemDrop%d` = 0 but `ItemDropQuantity%d` = %u.",
                qinfo->GetQuestId(), j + 1, j + 1, qinfo->ItemDropQuantity[j]);
        }

        for (QuestObjective const& obj : qinfo->GetObjectives())
        {
            if (obj.StorageIndex < 0)
            {
                switch (obj.Type)
                {
                    case QUEST_OBJECTIVE_MONSTER:
                    case QUEST_OBJECTIVE_ITEM:
                    case QUEST_OBJECTIVE_GAMEOBJECT:
                    case QUEST_OBJECTIVE_TALKTO:
                    case QUEST_OBJECTIVE_PLAYERKILLS:
                    case QUEST_OBJECTIVE_AREATRIGGER:
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has invalid StorageIndex = %d for objective type %u", qinfo->GetQuestId(), obj.ID, obj.StorageIndex, obj.Type);
                        break;
                    default:
                        break;
                }
            }

            switch (obj.Type)
            {
                case QUEST_OBJECTIVE_ITEM:
                    qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_DELIVER);
                    if (!sObjectMgr->GetItemTemplate(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has non existing item entry %u, quest can't be done.",
                        qinfo->GetQuestId(), obj.ID, obj.ObjectID);
                    break;
                case QUEST_OBJECTIVE_MONSTER:
                    qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_KILL | QUEST_SPECIAL_FLAGS_CAST);
                    if (!sObjectMgr->GetCreatureTemplate(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has non existing creature entry %u, quest can't be done.",
                        qinfo->GetQuestId(), obj.ID, uint32(obj.ObjectID));
                    break;
                case QUEST_OBJECTIVE_GAMEOBJECT:
                    qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_KILL | QUEST_SPECIAL_FLAGS_CAST);
                    if (!sObjectMgr->GetGameObjectTemplate(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has non existing gameobject entry %u, quest can't be done.",
                        qinfo->GetQuestId(), obj.ID, uint32(obj.ObjectID));
                    break;
                case QUEST_OBJECTIVE_TALKTO:
                    qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_CAST | QUEST_SPECIAL_FLAGS_SPEAKTO);
                    break;
                case QUEST_OBJECTIVE_MIN_REPUTATION:
                case QUEST_OBJECTIVE_MAX_REPUTATION:
                    if (!sFactionStore.LookupEntry(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has non existing faction id %u", qinfo->GetQuestId(), obj.ID, obj.ObjectID);
                    break;
                case QUEST_OBJECTIVE_PLAYERKILLS:
                    qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_PLAYER_KILL);
                    if (obj.Amount <= 0)
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has invalid player kills count %d", qinfo->GetQuestId(), obj.ID, obj.Amount);
                    break;
                case QUEST_OBJECTIVE_OBTAIN_CURRENCY:
                case QUEST_OBJECTIVE_OBTAIN_CURRENCY_AND_UPGRADE_GARRISON:
                case QUEST_OBJECTIVE_CURRENCY:
                    if (!sCurrencyTypesStore.LookupEntry(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has non existing currency %u", qinfo->GetQuestId(), obj.ID, obj.ObjectID);
                    if (obj.Amount <= 0)
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has invalid currency amount %d", qinfo->GetQuestId(), obj.ID, obj.Amount);
                    break;
                case QUEST_OBJECTIVE_LEARNSPELL:
                    if (!sSpellMgr->GetSpellInfo(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u has not exist SpellID: %u in ObjectID field ", qinfo->GetQuestId(), obj.ObjectID);
                    break;
                case QUEST_OBJECTIVE_AREATRIGGER:
                    if (!qinfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
                        const_cast<Quest*>(qinfo)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);
                    if (sAreaTriggerStore.LookupEntry(uint32(obj.ObjectID)))
                        _questAreaTriggerStore[obj.ObjectID].insert(qinfo->Id);
                    else if (obj.ObjectID != -1)
                        sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has non existing areatrigger id %d", qinfo->GetQuestId(), obj.ID, obj.ObjectID);
                    break;
                case QUEST_OBJECTIVE_MONEY:
                case QUEST_OBJECTIVE_PET_TRAINER_DEFEAT:
                case QUEST_OBJECTIVE_DEFEATBATTLEPET:
                case QUEST_OBJECTIVE_PET_BATTLE_VICTORIES:
                    break;
                case QUEST_OBJECTIVE_COMPLETE_CRITERIA_TREE:
                    if (!sCriteriaStore.LookupEntry(obj.ObjectID))
                        sLog->outError(LOG_FILTER_SQL, "Quest %u has not exist CriteriaTreeID: %u in ObjectID field ", qinfo->GetQuestId(), obj.ObjectID);
                    break;
                case QUEST_OBJECTIVE_EXCELLENCE_IN_ZONE:
                    break;
                default:
                    sLog->outError(LOG_FILTER_SQL, "Quest %u objective %u has unhandled type %u", qinfo->GetQuestId(), obj.ID, obj.Type);
            }
        }

        for (uint8 j = 0; j < QUEST_REWARD_CHOICES_COUNT; ++j)
        {
            if (uint32 id = qinfo->RewardChoiceItemId[j])
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = %u but item with entry %u does not exist, quest will not reward this item.",
                                   qinfo->GetQuestId(), j + 1, id, id);
                    qinfo->RewardChoiceItemId[j] = 0;
                }

                if (!qinfo->RewardChoiceItemCount[j])
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = %u but `RewardChoiceItemCount%d` = 0, quest can't be done.",
                    qinfo->GetQuestId(), j + 1, id, j + 1);
            }
            else if (qinfo->RewardChoiceItemCount[j]>0)
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = 0 but `RewardChoiceItemCount%d` = %u.",
                qinfo->GetQuestId(), j + 1, j + 1, qinfo->RewardChoiceItemCount[j]);
        }

        for (uint8 j = 0; j < QUEST_REWARD_ITEM_COUNT; ++j)
        {
            if (uint32 id = qinfo->RewardItemId[j])
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardItemId%d` = %u but item with entry %u does not exist, quest will not reward this item.",
                                   qinfo->GetQuestId(), j + 1, id, id);
                    qinfo->RewardItemId[j] = 0;
                }

                if (!qinfo->RewardItemCount[j])
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardItemId%d` = %u but `RewardItemCount%d` = 0, quest will not reward this item.",
                    qinfo->GetQuestId(), j + 1, id, j + 1);
            }
            else if (qinfo->RewardItemCount[j]>0)
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardItemId%d` = 0 but `RewardItemCount%d` = %u.",
                qinfo->GetQuestId(), j + 1, j + 1, qinfo->RewardItemCount[j]);
        }

        for (uint8 j = 0; j < QUEST_REWARD_REPUTATIONS_COUNT; ++j)
        {
            if (qinfo->RewardFactionId[j])
            {
                if (abs(qinfo->RewardFactionValue[j]) > 10)
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has RewardFactionValue%d = %i. That is outside the range of valid values (-9 to 9).", qinfo->GetQuestId(), j + 1, qinfo->RewardFactionValue[j]);

                if (!sFactionStore.LookupEntry(qinfo->RewardFactionId[j]))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardFactionId%d` = %u but raw faction (faction.dbc) %u does not exist, quest will not reward reputation for this faction.", qinfo->GetQuestId(), j + 1, qinfo->RewardFactionId[j], qinfo->RewardFactionId[j]);
                    qinfo->RewardFactionId[j] = 0;
                }
            }

            else if (qinfo->RewardFactionOverride[j] != 0)
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardFactionId%d` = 0 but `RewardFactionOverride%d` = %i.",
                qinfo->GetQuestId(), j + 1, j + 1, qinfo->RewardFactionOverride[j]);
        }

        if (qinfo->RewardDisplaySpell)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->RewardDisplaySpell);

            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardDisplaySpell` = %u but spell %u does not exist, spell removed as display reward.",
                               qinfo->GetQuestId(), qinfo->RewardDisplaySpell, qinfo->RewardDisplaySpell);
                qinfo->RewardDisplaySpell = 0;
            }

            else if (!SpellMgr::IsSpellValid(spellInfo))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardDisplaySpell` = %u but spell %u is broken, quest will not have a spell reward.",
                               qinfo->GetQuestId(), qinfo->RewardDisplaySpell, qinfo->RewardDisplaySpell);
                qinfo->RewardDisplaySpell = 0;
            }
        }

        if (qinfo->RewardSpell > 0)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->RewardSpell);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpell` = %u but spell %u does not exist, quest will not have a spell reward.",
                               qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;
            }

            else if (!SpellMgr::IsSpellValid(spellInfo))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpell` = %u but spell %u is broken, quest will not have a spell reward.",
                               qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;
            }
        }
    
        if (qinfo->RewardMailTemplateId)
        {
            if (!sMailTemplateStore.LookupEntry(qinfo->RewardMailTemplateId))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardMailTemplateId` = %u but mail template  %u does not exist, quest will not have a mail reward.",
                               qinfo->GetQuestId(), qinfo->RewardMailTemplateId, qinfo->RewardMailTemplateId);
                qinfo->RewardMailTemplateId = 0;
                qinfo->RewardMailDelay = 0;
            }
            else if (usedMailTemplates.find(qinfo->RewardMailTemplateId) != usedMailTemplates.end())
            {
                std::map<uint32, uint32>::const_iterator used_mt_itr = usedMailTemplates.find(qinfo->RewardMailTemplateId);
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardMailTemplateId` = %u but mail template  %u already used for quest %u, quest will not have a mail reward.",
                               qinfo->GetQuestId(), qinfo->RewardMailTemplateId, qinfo->RewardMailTemplateId, used_mt_itr->second);
                qinfo->RewardMailTemplateId = 0;
                qinfo->RewardMailDelay = 0;
            }
            else
                usedMailTemplates[qinfo->RewardMailTemplateId] = qinfo->GetQuestId();
        }

        if (qinfo->NextQuestIdChain)
        {
            QuestMap::iterator qNextItr = _questTemplates.find(qinfo->NextQuestIdChain);
            if (qNextItr == _questTemplates.end())
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `NextQuestIdChain` = %u but quest %u does not exist, quest chain will not work.",
                               qinfo->GetQuestId(), qinfo->NextQuestIdChain, qinfo->NextQuestIdChain);
                qinfo->NextQuestIdChain = 0;
            }
            else
                qNextItr->second->prevChainQuests.push_back(qinfo->GetQuestId());
        }

        for (uint8 j = 0; j < QUEST_REWARD_CURRENCY_COUNT; ++j)
        {
            if (qinfo->RewardCurrencyId[j])
            {
                if (qinfo->RewardCurrencyCount[j] == 0)
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardCurrencyId%d` = %u but `RewardCurrencyCount%d` = 0, quest can't be done.",
                    qinfo->GetQuestId(), j + 1, qinfo->RewardCurrencyId[j], j + 1);

                if (!sCurrencyTypesStore.LookupEntry(qinfo->RewardCurrencyId[j]))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardCurrencyId%d` = %u but currency with entry %u does not exist, quest can't be done.",
                                   qinfo->GetQuestId(), j + 1, qinfo->RewardCurrencyId[j], qinfo->RewardCurrencyId[j]);
                    qinfo->RewardCurrencyCount[j] = 0;
                }
            }
            else if (qinfo->RewardCurrencyCount[j] > 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardCurrencyId%d` = 0 but `RewardCurrencyCount%d` = %u, quest can't be done.",
                               qinfo->GetQuestId(), j + 1, j + 1, qinfo->RewardCurrencyCount[j]);
                qinfo->RewardCurrencyCount[j] = 0;
            }
        }

        if (qinfo->RewardSkillId)
        {
            if (!sSkillLineStore.LookupEntry(qinfo->RewardSkillId))
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillId` = %u but this skill does not exist",
                qinfo->GetQuestId(), qinfo->RewardSkillId);

            if (!qinfo->RewardSkillPoints)
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillId` = %u but `RewardSkillPoints` is 0",
                qinfo->GetQuestId(), qinfo->RewardSkillId);
        }

        if (qinfo->RewardSkillPoints)
        {
            if (qinfo->RewardSkillPoints > sWorld->GetConfigMaxSkillValue())
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillPoints` = %u but max possible skill is %u, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RewardSkillPoints, sWorld->GetConfigMaxSkillValue());

            if (!qinfo->RewardSkillId)
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillPoints` = %u but `RewardSkillId` is 0",
                qinfo->GetQuestId(), qinfo->RewardSkillPoints);
        }

        if (qinfo->PrevQuestID)
        {
            if (sDB2Manager.GetQuestLineXQuestData(qinfo->Id))
            {
                if (!qinfo->Line.LineID)
                    sLog->outError(LOG_FILTER_SQL, "QuestID: %d has wrong questLine pos or questLine", qinfo->Id);
                else if (qinfo->Line.Pos >= 1)
                    qinfo->prevQuests.push_back(sDB2Manager.GetQuestIDbyLineAndPos(qinfo->Line.LineID, qinfo->Line.Pos - 1));
            }
            else
            {
                if (_questTemplates.find(abs(qinfo->PrevQuestID)) == _questTemplates.end())
                    sLog->outError(LOG_FILTER_SQL, "Quest %d has PrevQuestID %i, but no such quest", qinfo->GetQuestId(), qinfo->PrevQuestID);
                else
                    qinfo->prevQuests.push_back(qinfo->PrevQuestID);
            }
        }

        if (qinfo->NextQuestID)
        {
            if (sDB2Manager.GetQuestLineXQuestData(qinfo->Id))
            {
                if (!qinfo->Line.Pos || !qinfo->Line.LineID)
                    sLog->outError(LOG_FILTER_SQL, "QuestID: %d has wrong questLine pos or questLine", qinfo->Id);
                else
                    qinfo->prevQuests.push_back(sDB2Manager.GetQuestIDbyLineAndPos(qinfo->Line.LineID, qinfo->Line.Pos + 1));
            }
            else
            {
                QuestMap::iterator qNextItr = _questTemplates.find(abs(qinfo->NextQuestID));
                if (qNextItr == _questTemplates.end())
                    sLog->outError(LOG_FILTER_SQL, "Quest %d has NextQuestID %i, but no such quest", qinfo->GetQuestId(), qinfo->NextQuestID);
                else
                    qNextItr->second->prevQuests.push_back(qinfo->NextQuestID < 0 ? -int32(qinfo->GetQuestId()) : int32(qinfo->GetQuestId()));
            }
        }

        if (qinfo->ExclusiveGroup)
            mExclusiveQuestGroups.insert(std::pair<int32, uint32>(qinfo->ExclusiveGroup, qinfo->GetQuestId()));

        if (qinfo->LimitTime)
            qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED);
    }

    for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(i);
        if (!spellInfo)
            continue;

        for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (spellInfo->Effects[j].Effect != SPELL_EFFECT_QUEST_COMPLETE)
                continue;

            uint32 questID = spellInfo->Effects[j].MiscValue;
            Quest const* quest = GetQuestTemplate(questID);
            if (!quest)
                continue;

            if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
            {
                sLog->outError(LOG_FILTER_SQL, "Spell (id: %u) have SPELL_EFFECT_QUEST_COMPLETE for quest %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT. Quest flags must be fixed, quest modified to enable objective.", spellInfo->Id, questID);
                const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu quests definitions in %u ms", (unsigned long)_questTemplates.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestTemplateLocale()
{
    uint32 oldMSTime = getMSTime();

    _questTemplateLocaleStore.clear(); // need for reload case
    //                                               0     1
    QueryResult result = WorldDatabase.Query("SELECT Id, locale, "
        //      2           3                 4                5                 6                  7                   8                   9                  10
        "LogTitle, LogDescription, QuestDescription, AreaDescription, PortraitGiverText, PortraitGiverName, PortraitTurnInText, PortraitTurnInName, QuestCompletionLog"
        " FROM quest_template_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        LocaleConstant locale = GetLocaleByName(fields[1].GetString());
        if (locale == LOCALE_enUS || locale == LOCALE_none)
            continue;
            
        QuestTemplateLocale& data = _questTemplateLocaleStore[fields[0].GetUInt32()];
        AddLocaleString(fields[2].GetString(), locale, data.LogTitle);
        AddLocaleString(fields[3].GetString(), locale, data.LogDescription);
        AddLocaleString(fields[4].GetString(), locale, data.QuestDescription);
        AddLocaleString(fields[5].GetString(), locale, data.AreaDescription);
        AddLocaleString(fields[6].GetString(), locale, data.PortraitGiverText);
        AddLocaleString(fields[7].GetString(), locale, data.PortraitGiverName);
        AddLocaleString(fields[8].GetString(), locale, data.PortraitTurnInText);
        AddLocaleString(fields[9].GetString(), locale, data.PortraitTurnInName);
        AddLocaleString(fields[10].GetString(), locale, data.QuestCompletionLog);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Quest Tempalate locale strings in %u ms", uint32(_questTemplateLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestObjectivesLocale()
{
    uint32 oldMSTime = getMSTime();

    _questObjectivesLocaleStore.clear(); // need for reload case
    //                                               0     1          2
    QueryResult result = WorldDatabase.Query("SELECT Id, locale, Description FROM quest_objectives_locale");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        LocaleConstant localeConstant = GetLocaleByName(fields[1].GetString());
        if (localeConstant == LOCALE_enUS || localeConstant == LOCALE_none)
            continue;

        QuestObjectivesLocale& data = _questObjectivesLocaleStore[fields[0].GetUInt32()];
        AddLocaleString(fields[2].GetString(), localeConstant, data.Description);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Quest Objectives locale strings in %u ms", uint32(_questObjectivesLocaleStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadScripts(ScriptsType type)
{
    uint32 oldMSTime = getMSTime();

    ScriptMapMap* scripts = GetScriptsMapByType(type);
    if (!scripts)
        return;

    std::string tableName = GetScriptsTableNameByType(type);
    if (tableName.empty())
        return;

    if (sScriptMgr->IsScriptScheduled())                    // function cannot be called when scripts are in use.
        return;

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading %s...", tableName.c_str());

    scripts->clear();                                       // need for reload support

    bool isSpellScriptTable = (type == SCRIPTS_SPELL);
    //                                                 0    1       2         3         4          5    6  7  8  9
    QueryResult result = WorldDatabase.PQuery("SELECT id, delay, command, datalong, datalong2, dataint, x, y, z, o%s FROM %s", isSpellScriptTable ? ", effIndex" : "", tableName.c_str());

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 script definitions. DB table `%s` is empty!", tableName.c_str());
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        ScriptInfo tmp;
        tmp.type      = type;
        tmp.id           = fields[0].GetUInt32();
        if (isSpellScriptTable)
            tmp.id      |= fields[10].GetUInt8() << 24;
        tmp.delay        = fields[1].GetUInt32();
        tmp.command      = ScriptCommands(fields[2].GetUInt32());
        tmp.Raw.nData[0] = fields[3].GetUInt32();
        tmp.Raw.nData[1] = fields[4].GetUInt32();
        tmp.Raw.nData[2] = fields[5].GetInt32();
        tmp.Raw.fData[0] = fields[6].GetFloat();
        tmp.Raw.fData[1] = fields[7].GetFloat();
        tmp.Raw.fData[2] = fields[8].GetFloat();
        tmp.Raw.fData[3] = fields[9].GetFloat();

        // generic command args check
        switch (tmp.command)
        {
            case SCRIPT_COMMAND_TALK:
            {
                if (tmp.Talk.ChatType > CHAT_TYPE_WHISPER && tmp.Talk.ChatType != CHAT_MSG_RAID_BOSS_WHISPER)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid talk type (datalong = %u) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName.c_str(), tmp.Talk.ChatType, tmp.id);
                    continue;
                }
                if (!tmp.Talk.TextID)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid talk text id (dataint = %i) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName.c_str(), tmp.Talk.TextID, tmp.id);
                    continue;
                }
                if (tmp.Talk.TextID < MIN_DB_SCRIPT_STRING_ID || tmp.Talk.TextID >= MAX_DB_SCRIPT_STRING_ID)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has out of range text id (dataint = %i expected %u-%u) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName.c_str(), tmp.Talk.TextID, MIN_DB_SCRIPT_STRING_ID, MAX_DB_SCRIPT_STRING_ID, tmp.id);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_EMOTE:
            {
                if (!sEmotesStore.LookupEntry(tmp.Emote.EmoteID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid emote id (datalong = %u) in SCRIPT_COMMAND_EMOTE for script id %u",
                        tableName.c_str(), tmp.Emote.EmoteID, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_TELEPORT_TO:
            {
                if (!sMapStore.LookupEntry(tmp.TeleportTo.MapID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid map (Id: %u) in SCRIPT_COMMAND_TELEPORT_TO for script id %u",
                        tableName.c_str(), tmp.TeleportTo.MapID, tmp.id);
                    continue;
                }

                if (!Trinity::IsValidMapCoord(tmp.TeleportTo.DestX, tmp.TeleportTo.DestY, tmp.TeleportTo.DestZ, tmp.TeleportTo.Orientation))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid coordinates (X: %f Y: %f Z: %f O: %f) in SCRIPT_COMMAND_TELEPORT_TO for script id %u",
                        tableName.c_str(), tmp.TeleportTo.DestX, tmp.TeleportTo.DestY, tmp.TeleportTo.DestZ, tmp.TeleportTo.Orientation, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_QUEST_EXPLORED:
            {
                Quest const* quest = GetQuestTemplate(tmp.QuestExplored.QuestID);
                if (!quest)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid quest (ID: %u) in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u",
                        tableName.c_str(), tmp.QuestExplored.QuestID, tmp.id);
                    continue;
                }

                if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has quest (ID: %u) in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT in quest flags. Script command or quest flags wrong. Quest modified to require objective.",
                        tableName.c_str(), tmp.QuestExplored.QuestID, tmp.id);

                    // this will prevent quest completing without objective
                    const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);

                    // continue; - quest objective requirement set and command can be allowed
                }

                if (float(tmp.QuestExplored.Distance) > DEFAULT_VISIBILITY_DISTANCE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has too large distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u",
                        tableName.c_str(), tmp.QuestExplored.Distance, tmp.id);
                    continue;
                }

                if (tmp.QuestExplored.Distance && float(tmp.QuestExplored.Distance) > DEFAULT_VISIBILITY_DISTANCE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has too large distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, max distance is %f or 0 for disable distance check",
                        tableName.c_str(), tmp.QuestExplored.Distance, tmp.id, DEFAULT_VISIBILITY_DISTANCE);
                    continue;
                }

                if (tmp.QuestExplored.Distance && float(tmp.QuestExplored.Distance) < INTERACTION_DISTANCE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has too small distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, min distance is %f or 0 for disable distance check",
                        tableName.c_str(), tmp.QuestExplored.Distance, tmp.id, INTERACTION_DISTANCE);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_KILL_CREDIT:
            {
                if (!GetCreatureTemplate(tmp.KillCredit.CreatureEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid creature (Entry: %u) in SCRIPT_COMMAND_KILL_CREDIT for script id %u",
                        tableName.c_str(), tmp.KillCredit.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_RESPAWN_GAMEOBJECT:
            {
                GameObjectData const* data = GetGOData(tmp.RespawnGameobject.GOGuid);
                if (!data)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid gameobject (GUID: %u) in SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName.c_str(), tmp.RespawnGameobject.GOGuid, tmp.id);
                    continue;
                }

                GameObjectTemplate const* info = GetGameObjectTemplate(data->id);
                if (!info)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has gameobject with invalid entry (GUID: %u Entry: %u) in SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName.c_str(), tmp.RespawnGameobject.GOGuid, data->id, tmp.id);
                    continue;
                }

                if (info->type == GAMEOBJECT_TYPE_FISHINGNODE ||
                    info->type == GAMEOBJECT_TYPE_FISHINGHOLE ||
                    info->type == GAMEOBJECT_TYPE_DOOR        ||
                    info->type == GAMEOBJECT_TYPE_BUTTON      ||
                    info->type == GAMEOBJECT_TYPE_TRAP)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` have gameobject type (%u) unsupported by command SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName.c_str(), info->entry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_TEMP_SUMMON_CREATURE:
            {
                if (!Trinity::IsValidMapCoord(tmp.TempSummonCreature.PosX, tmp.TempSummonCreature.PosY, tmp.TempSummonCreature.PosZ, tmp.TempSummonCreature.Orientation))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid coordinates (X: %f Y: %f Z: %f O: %f) in SCRIPT_COMMAND_TEMP_SUMMON_CREATURE for script id %u",
                        tableName.c_str(), tmp.TempSummonCreature.PosX, tmp.TempSummonCreature.PosY, tmp.TempSummonCreature.PosZ, tmp.TempSummonCreature.Orientation, tmp.id);
                    continue;
                }

                if (!GetCreatureTemplate(tmp.TempSummonCreature.CreatureEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid creature (Entry: %u) in SCRIPT_COMMAND_TEMP_SUMMON_CREATURE for script id %u",
                        tableName.c_str(), tmp.TempSummonCreature.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_OPEN_DOOR:
            case SCRIPT_COMMAND_CLOSE_DOOR:
            {
                GameObjectData const* data = GetGOData(tmp.ToggleDoor.GOGuid);
                if (!data)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid gameobject (GUID: %u) in %s for script id %u",
                        tableName.c_str(), tmp.ToggleDoor.GOGuid, GetScriptCommandName(tmp.command).c_str(), tmp.id);
                    continue;
                }

                GameObjectTemplate const* info = GetGameObjectTemplate(data->id);
                if (!info)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has gameobject with invalid entry (GUID: %u Entry: %u) in %s for script id %u",
                        tableName.c_str(), tmp.ToggleDoor.GOGuid, data->id, GetScriptCommandName(tmp.command).c_str(), tmp.id);
                    continue;
                }

                if (info->type != GAMEOBJECT_TYPE_DOOR)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has gameobject type (%u) non supported by command %s for script id %u",
                        tableName.c_str(), info->entry, GetScriptCommandName(tmp.command).c_str(), tmp.id);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_REMOVE_AURA:
            {
                if (!sSpellMgr->GetSpellInfo(tmp.RemoveAura.SpellID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using non-existent spell (id: %u) in SCRIPT_COMMAND_REMOVE_AURA for script id %u",
                        tableName.c_str(), tmp.RemoveAura.SpellID, tmp.id);
                    continue;
                }
                if (tmp.RemoveAura.Flags & ~0x1)                    // 1 bits (0, 1)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using unknown flags in datalong2 (%u) in SCRIPT_COMMAND_REMOVE_AURA for script id %u",
                        tableName.c_str(), tmp.RemoveAura.Flags, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_CAST_SPELL:
            {
                if (!sSpellMgr->GetSpellInfo(tmp.CastSpell.SpellID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using non-existent spell (id: %u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.SpellID, tmp.id);
                    continue;
                }
                if (tmp.CastSpell.Flags > 4)                      // targeting type
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using unknown target in datalong2 (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.Flags, tmp.id);
                    continue;
                }
                if (tmp.CastSpell.Flags != 4 && tmp.CastSpell.CreatureEntry & ~0x1)                      // 1 bit (0, 1)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using unknown flags in dataint (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.CreatureEntry, tmp.id);
                    continue;
                }
                else if (tmp.CastSpell.Flags == 4 && !GetCreatureTemplate(tmp.CastSpell.CreatureEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using invalid creature entry in dataint (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_CREATE_ITEM:
            {
                if (!GetItemTemplate(tmp.CreateItem.ItemEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has nonexistent item (entry: %u) in SCRIPT_COMMAND_CREATE_ITEM for script id %u",
                        tableName.c_str(), tmp.CreateItem.ItemEntry, tmp.id);
                    continue;
                }
                if (!tmp.CreateItem.Amount)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` SCRIPT_COMMAND_CREATE_ITEM but amount is %u for script id %u",
                        tableName.c_str(), tmp.CreateItem.Amount, tmp.id);
                    continue;
                }
                break;
            }
            default:
                break;
        }

        if (scripts->find(tmp.id) == scripts->end())
        {
            ScriptMap emptyMap;
            (*scripts)[tmp.id] = emptyMap;
        }
        (*scripts)[tmp.id].insert(std::pair<uint32, ScriptInfo>(tmp.delay, tmp));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u script definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameObjectScripts()
{
    LoadScripts(SCRIPTS_GAMEOBJECT);

    // check ids
    for (ScriptMapMap::const_iterator itr = sGameObjectScripts.begin(); itr != sGameObjectScripts.end(); ++itr)
    {
        if (!GetGOData(itr->first))
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_scripts` has not existing gameobject (GUID: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadQuestEndScripts()
{
    LoadScripts(SCRIPTS_QUEST_END);

    // check ids
    for (ScriptMapMap::const_iterator itr = sQuestEndScripts.begin(); itr != sQuestEndScripts.end(); ++itr)
    {
        if (!GetQuestTemplate(itr->first))
            sLog->outError(LOG_FILTER_SQL, "Table `quest_end_scripts` has not existing quest (Id: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadQuestStartScripts()
{
    LoadScripts(SCRIPTS_QUEST_START);

    // check ids
    for (ScriptMapMap::const_iterator itr = sQuestStartScripts.begin(); itr != sQuestStartScripts.end(); ++itr)
    {
        if (!GetQuestTemplate(itr->first))
            sLog->outError(LOG_FILTER_SQL, "Table `quest_start_scripts` has not existing quest (Id: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadSpellScripts()
{
    LoadScripts(SCRIPTS_SPELL);

    // check ids
    for (ScriptMapMap::const_iterator itr = sSpellScripts.begin(); itr != sSpellScripts.end(); ++itr)
    {
        uint32 spellId = uint32(itr->first) & 0x00FFFFFF;
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `spell_scripts` has not existing spell (Id: %u) as script id", spellId);
            continue;
        }

        uint8 i = (uint8)((uint32(itr->first) >> 24) & 0x000000FF);
        //check for correct spellEffect
        if (!spellInfo->Effects[i].Effect || (spellInfo->Effects[i].Effect != SPELL_EFFECT_SCRIPT_EFFECT && spellInfo->Effects[i].Effect != SPELL_EFFECT_DUMMY))
            sLog->outError(LOG_FILTER_SQL, "Table `spell_scripts` - spell %u effect %u is not SPELL_EFFECT_SCRIPT_EFFECT or SPELL_EFFECT_DUMMY", spellId, i);
    }
}

void ObjectMgr::LoadEventScripts()
{
    LoadScripts(SCRIPTS_EVENT);

    std::set<uint32> evt_scripts;
    // Load all possible script entries from gameobjects
    GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
    for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
        if (uint32 eventId = itr->second.GetEventScriptId())
            evt_scripts.insert(eventId);

    // Load all possible script entries from spells
    for (uint32 i = 1; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
        if (SpellInfo const* spell = sSpellMgr->GetSpellInfo(i))
            for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
                if (spell->Effects[j].Effect == SPELL_EFFECT_SEND_EVENT)
                    if (spell->Effects[j].MiscValue)
                        evt_scripts.insert(spell->Effects[j].MiscValue);

    for (size_t path_idx = 0; path_idx < sTaxiPathNodesByPath.size(); ++path_idx)
    {
        for (size_t node_idx = 0; node_idx < sTaxiPathNodesByPath[path_idx].size(); ++node_idx)
        {
            TaxiPathNodeEntry const* node = sTaxiPathNodesByPath[path_idx][node_idx];

            if (node->ArrivalEventID)
                evt_scripts.insert(node->ArrivalEventID);

            if (node->DepartureEventID)
                evt_scripts.insert(node->DepartureEventID);
        }
    }

    // Then check if all scripts are in above list of possible script entries
    for (ScriptMapMap::const_iterator itr = sEventScripts.begin(); itr != sEventScripts.end(); ++itr)
    {
        std::set<uint32>::const_iterator itr2 = evt_scripts.find(itr->first);
        if (itr2 == evt_scripts.end())
            sLog->outError(LOG_FILTER_SQL, "Table `event_scripts` has script (Id: %u) not referring to any gameobject_template type 10 data2 field, type 3 data6 field, type 13 data 2 field or any spell effect %u",
                itr->first, SPELL_EFFECT_SEND_EVENT);
    }
}

//Load WP Scripts
void ObjectMgr::LoadWaypointScripts()
{
    LoadScripts(SCRIPTS_WAYPOINT);

    std::set<uint32> actionSet;

    for (ScriptMapMap::const_iterator itr = sWaypointScripts.begin(); itr != sWaypointScripts.end(); ++itr)
        actionSet.insert(itr->first);

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WOLRD_SEL_WAYPOINT_DATA_ACTION);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 action = fields[0].GetUInt32();

            actionSet.erase(action);
        }
        while (result->NextRow());
    }

    for (std::set<uint32>::iterator itr = actionSet.begin(); itr != actionSet.end(); ++itr)
        sLog->outError(LOG_FILTER_SQL, "There is no waypoint which links to the waypoint script %u", *itr);
}

void ObjectMgr::LoadSpellScriptNames()
{
    uint32 oldMSTime = getMSTime();

    _spellScriptsStore.clear();                            // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT spell_id, ScriptName FROM spell_script_names");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell script names. DB table `spell_script_names` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {

        Field* fields = result->Fetch();

        int32 spellId          = fields[0].GetInt32();
        const char *scriptName = fields[1].GetCString();

        bool allRanks = false;
        if (spellId <= 0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Scriptname:`%s` spell (spell_id:%d) does not exist in `Spell.dbc`.", scriptName, fields[0].GetInt32());
            WorldDatabase.PExecute("DELETE FROM `spell_script_names` WHERE spell_id = %i", fields[0].GetInt32());
            continue;
        }

        if (allRanks)
        {
            if (sSpellMgr->GetFirstSpellInChain(spellId) != uint32(spellId))
            {
                sLog->outError(LOG_FILTER_SQL, "Scriptname:`%s` spell (spell_id:%d) is not first rank of spell.", scriptName, fields[0].GetInt32());
                continue;
            }
            while (spellInfo)
            {
                _spellScriptsStore.insert(SpellScriptsContainer::value_type(spellInfo->Id, GetScriptId(scriptName)));
                spellInfo = sSpellMgr->GetSpellInfo(spellInfo->Id)->GetNextRankSpell();
            }
        }
        else
            _spellScriptsStore.insert(SpellScriptsContainer::value_type(spellInfo->Id, GetScriptId(scriptName)));
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::ValidateSpellScripts()
{
    uint32 oldMSTime = getMSTime();

    if (_spellScriptsStore.empty())
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Validated 0 scripts.");
        return;
    }

    uint32 count = 0;

    for (SpellScriptsContainer::iterator itr = _spellScriptsStore.begin(); itr != _spellScriptsStore.end();)
    {
        SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(itr->first);
        std::vector<std::pair<SpellScriptLoader *, SpellScriptsContainer::iterator> > SpellScriptLoaders;
        sScriptMgr->CreateSpellScriptLoaders(itr->first, SpellScriptLoaders);
        itr = _spellScriptsStore.upper_bound(itr->first);

        for (std::vector<std::pair<SpellScriptLoader *, SpellScriptsContainer::iterator> >::iterator sitr = SpellScriptLoaders.begin(); sitr != SpellScriptLoaders.end(); ++sitr)
        {
            SpellScript* spellScript = sitr->first->GetSpellScript();
            AuraScript* auraScript = sitr->first->GetAuraScript();
            bool valid = true;
            if (!spellScript && !auraScript)
            {
                sLog->outError(LOG_FILTER_TSCR, "Functions GetSpellScript() and GetAuraScript() of script `%s` do not return objects - script skipped",  GetScriptName(sitr->second->second));
                valid = false;
            }
            if (spellScript)
            {
                spellScript->_Init(&sitr->first->GetName(), spellEntry->Id);
                spellScript->_Register();
                if (!spellScript->_Validate(spellEntry))
                    valid = false;
                delete spellScript;
            }
            if (auraScript)
            {
                auraScript->_Init(&sitr->first->GetName(), spellEntry->Id);
                auraScript->_Register();
                if (!auraScript->_Validate(spellEntry))
                    valid = false;
                delete auraScript;
            }
            if (!valid)
            {
                _spellScriptsStore.erase(sitr->second);
            }
        }
        ++count;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Validated %u scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPageTexts()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0      1       2
    QueryResult result = WorldDatabase.Query("SELECT ID, Text, NextPageID FROM page_text");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 page texts. DB table `page_text` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        PageText& pageText = _pageTextStore[fields[0].GetUInt32()];
        pageText.Text = fields[1].GetString();
        pageText.NextPage = fields[2].GetUInt32();

        ++count;
    }
    while (result->NextRow());

    for (PageTextContainer::const_iterator::value_type const& itr : _pageTextStore)
        if (itr.second.NextPage)
        {
            PageTextContainer::const_iterator itr2 = _pageTextStore.find(itr.second.NextPage);
            if (itr2 == _pageTextStore.end())
                sLog->outError(LOG_FILTER_SQL, "Page text (Id: %u) has not existing next page (Id: %u)", itr.first, itr.second.NextPage);
        }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u page texts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

PageText const* ObjectMgr::GetPageText(uint32 pageEntry)
{
    PageTextContainer::const_iterator itr = _pageTextStore.find(pageEntry);
    if (itr != _pageTextStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadPageTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _pageTextLocaleStore.clear();                             // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, text_loc1, text_loc2, text_loc3, text_loc4, text_loc5, text_loc6, text_loc7, text_loc8, text_loc9, text_loc10 FROM locales_page_text");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        PageTextLocale& data = _pageTextLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.Text);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu PageText locale strings in %u ms", (unsigned long)_pageTextLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadInstanceTemplate()
{
    uint32 oldMSTime = getMSTime();

    //                                                0     1       2        4             5
    QueryResult result = WorldDatabase.Query("SELECT map, parent, script, allowMount, bonusChance FROM instance_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 instance templates. DB table `page_text` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint16 mapID = fields[0].GetUInt16();

        if (!MapManager::IsValidMAP(mapID, true))
        {
            sLog->outError(LOG_FILTER_SQL, "ObjectMgr::LoadInstanceTemplate: bad mapid %d for template!", mapID);
            continue;
        }

        InstanceTemplate instanceTemplate;

        instanceTemplate.AllowMount     = fields[3].GetBool();
        instanceTemplate.Parent         = uint32(fields[1].GetUInt16());
        instanceTemplate.ScriptId       = sObjectMgr->GetScriptId(fields[2].GetCString());
        instanceTemplate.bonusChance    = fields[3].GetUInt32();

        _instanceTemplateStore[mapID] = instanceTemplate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u instance templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

InstanceTemplate const* ObjectMgr::GetInstanceTemplate(uint32 mapID)
{
    InstanceTemplateContainer::const_iterator itr = _instanceTemplateStore.find(uint16(mapID));
    if (itr != _instanceTemplateStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadInstanceEncounters()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0         1            2                3                4
    QueryResult result = WorldDatabase.Query("SELECT entry, creditType, creditEntry, lastEncounterDungeon, difficulty FROM instance_encounters");
    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 instance encounters, table is empty!");

        return;
    }

    uint32 count = 0;
    std::map<uint32, DungeonEncounterEntry const*> dungeonLastBosses;
    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();
        uint8 creditType = fields[1].GetUInt8();
        uint32 creditEntry = fields[2].GetUInt32();
        uint32 lastEncounterDungeon = fields[3].GetUInt16();
        int32 DungeonDifficult = fields[4].GetInt32();

        DungeonEncounterEntry const* dungeonEncounter = sDungeonEncounterStore.LookupEntry(entry);
        if (!dungeonEncounter)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid encounter id %u, skipped!", entry);
            continue;
        }

        if (lastEncounterDungeon && !sLFGMgr->GetLFGDungeonEntry(lastEncounterDungeon))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an encounter %u (%s) marked as final for invalid dungeon id %u, skipped!", entry, dungeonEncounter->Name, lastEncounterDungeon);
            continue;
        }

        std::map<uint32, DungeonEncounterEntry const*>::const_iterator itr = dungeonLastBosses.find(lastEncounterDungeon);
        if (lastEncounterDungeon)
        {
            if (itr != dungeonLastBosses.end())
            {
                sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` specified encounter %u (%s) as last encounter but %u (%s) is already marked as one, skipped!", entry, dungeonEncounter->Name, itr->second->ID, itr->second->Name);
                continue;
            }

            dungeonLastBosses[lastEncounterDungeon] = dungeonEncounter;
        }

        switch (creditType)
        {
            case ENCOUNTER_CREDIT_KILL_CREATURE:
            {
                CreatureTemplate const* creatureInfo = GetCreatureTemplate(creditEntry);
                if (!creatureInfo)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid creature (entry %u) linked to the encounter %u (%s), skipped!", creditEntry, entry, dungeonEncounter->Name);
                    continue;
                }
                const_cast<CreatureTemplate*>(creatureInfo)->flags_extra |= CREATURE_FLAG_EXTRA_DUNGEON_BOSS;
                break;
            }
            case ENCOUNTER_CREDIT_CAST_SPELL:
                if (!sSpellMgr->GetSpellInfo(creditEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid spell (entry %u) linked to the encounter %u (%s), skipped!", creditEntry, entry, dungeonEncounter->Name);
                    continue;
                }
                break;
            default:
                sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid credit type (%u) for encounter %u (%s), skipped!", creditType, entry, dungeonEncounter->Name);
                continue;
        }

        if (dungeonEncounter->DifficultyID > 0 && dungeonEncounter->DifficultyID != DungeonDifficult)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` overvrite dificult from dbc for encounter %u. DBC %i, DB %i", entry, dungeonEncounter->DifficultyID, DungeonDifficult);
            DungeonDifficult = dungeonEncounter->DifficultyID;
        }

        if (DungeonDifficult > 0)
        {
            DungeonEncounterList& encounters = _dungeonEncounterStore[MAKE_PAIR32(dungeonEncounter->MapID, DungeonDifficult)];
            encounters.push_back(new DungeonEncounter(dungeonEncounter, EncounterCreditType(creditType), creditEntry, lastEncounterDungeon));
        }
        else
            for (int32 i = 0; i < MAX_DIFFICULTY; ++i)
            {
                DungeonEncounterList& encounters = _dungeonEncounterStore[MAKE_PAIR32(dungeonEncounter->MapID, Difficulty(i))];
                encounters.push_back(new DungeonEncounter(dungeonEncounter, EncounterCreditType(creditType), creditEntry, lastEncounterDungeon));
            }

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u instance encounters in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

NpcText const* ObjectMgr::GetNpcText(uint32 textID) const
{
    NpcTextContainer::const_iterator itr = _npcTextStore.find(textID);
    if (itr != _npcTextStore.end())
        return &itr->second;
    return NULL;
}

void ObjectMgr::LoadNPCText()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT ID, "
        "Probability0, Probability1, Probability2, Probability3, Probability4, Probability5, Probability6, Probability7, "
        "BroadcastTextID0, BroadcastTextID1, BroadcastTextID2, BroadcastTextID3, BroadcastTextID4, BroadcastTextID5, BroadcastTextID6, BroadcastTextID7"
        " FROM npc_text");

    int count = 0;
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u npc texts", count);
        return;
    }
    _npcTextStore.rehash(result->GetRowCount());

    int cic;

    do
    {
        ++count;
        cic = 0;

        Field* fields = result->Fetch();

        uint32 textID = fields[cic++].GetUInt32();
        if (!textID)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `npc_text` has record wit reserved id 0, ignore.");
            continue;
        }

        NpcText& npcText = _npcTextStore[textID];

        for (uint8 i = 0; i < MAX_NPC_TEXT_OPTIONS; ++i)
        {
            npcText.Data[i].Probability      = fields[1 + i].GetFloat();
            npcText.Data[i].BroadcastTextID  = fields[9 + i].GetUInt32();
        }
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u npc texts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

//not very fast function but it is called only once a day, or on starting-up
void ObjectMgr::ReturnOrDeleteOldMails(bool serverUp)
{
    uint32 oldMSTime = getMSTime();

    time_t curTime = time(NULL);
    tm lt;
    localtime_r(&curTime, &lt);
    uint64 basetime(curTime);
    sLog->outInfo(LOG_FILTER_GENERAL, "Returning mails current time: hour: %d, minute: %d, second: %d ", lt.tm_hour, lt.tm_min, lt.tm_sec);

    // Delete all old mails without item and without body immediately, if starting server
    if (!serverUp)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EMPTY_EXPIRED_MAIL);
        stmt->setUInt64(0, basetime);
        CharacterDatabase.Execute(stmt);
    }
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_MAIL);
    stmt->setUInt64(0, basetime);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> No expired mails found.");
        return;                                             // any mails need to be returned or deleted
    }

    std::map<uint32 /*messageId*/, MailItemInfoVec> itemsCache;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_MAIL_ITEMS);
    stmt->setUInt32(0, (uint32)basetime);
    if (PreparedQueryResult items = CharacterDatabase.Query(stmt))
    {
        MailItemInfo item;
        do
        {
            Field* fields = items->Fetch();
            item.item_guid = fields[0].GetUInt64();
            item.item_template = fields[1].GetUInt32();
            uint32 mailId = fields[2].GetUInt32();
            itemsCache[mailId].push_back(item);
        } while (items->NextRow());
    }

    uint32 deletedCount = 0;
    uint32 returnedCount = 0;
    do
    {
        Field* fields = result->Fetch();
        Mail* m = new Mail;
        m->messageID      = fields[0].GetUInt32();
        m->messageType    = fields[1].GetUInt8();
        m->sender         = fields[2].GetUInt64();
        m->receiver       = fields[3].GetUInt64();
        bool has_items    = fields[4].GetBool();
        m->expire_time    = time_t(fields[5].GetUInt32());
        m->deliver_time   = 0;
        m->COD            = fields[6].GetUInt64();
        m->checked        = fields[7].GetUInt8();
        m->mailTemplateId = fields[8].GetInt16();

        Player* player = NULL;
        if (serverUp)
            player = ObjectAccessor::FindPlayer(ObjectGuid::Create<HighGuid::Player>(m->receiver));

        if (player && player->m_mailsLoaded)
        {                                                   // this code will run very improbably (the time is between 4 and 5 am, in game is online a player, who has old mail
            // his in mailbox and he has already listed his mails)
            delete m;
            continue;
        }

        // Delete or return mail
        if (has_items)
        {
            // read items from cache
            m->items.swap(itemsCache[m->messageID]);

            // if it is mail from non-player, or if it's already return mail, it shouldn't be returned, but deleted
            if (m->messageType != MAIL_NORMAL || (m->checked & (MAIL_CHECK_MASK_COD_PAYMENT | MAIL_CHECK_MASK_RETURNED)))
            {
                // mail open and then not returned
                for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
                {
                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
                    stmt->setUInt64(0, itr2->item_guid);
                    CharacterDatabase.Execute(stmt);
                }
            }
            else
            {
                // Mail will be returned
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MAIL_RETURNED);
                stmt->setUInt64(0, m->receiver);
                stmt->setUInt64(1, m->sender);
                stmt->setUInt32(2, basetime + 30 * DAY);
                stmt->setUInt32(3, basetime);
                stmt->setUInt8 (4, uint8(MAIL_CHECK_MASK_RETURNED));
                stmt->setUInt32(5, m->messageID);
                CharacterDatabase.Execute(stmt);
                for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
                {
                    // Update receiver in mail items for its proper delivery, and in instance_item for avoid lost item at sender delete
                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MAIL_ITEM_RECEIVER);
                    stmt->setUInt64(0, m->sender);
                    stmt->setUInt64(1, itr2->item_guid);
                    CharacterDatabase.Execute(stmt);

                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_OWNER);
                    stmt->setUInt64(0, m->sender);
                    stmt->setUInt64(1, itr2->item_guid);
                    CharacterDatabase.Execute(stmt);
                }
                delete m;
                ++returnedCount;
                continue;
            }
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
        stmt->setUInt32(0, m->messageID);
        CharacterDatabase.Execute(stmt);
        delete m;
        ++deletedCount;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Processed %u expired mails: %u deleted and %u returned in %u ms", deletedCount + returnedCount, deletedCount, returnedCount, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestAreaTriggers()
{
    uint32 oldMSTime = getMSTime();

    _questAreaTriggerStore.clear();                           // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT id, quest FROM areatrigger_involvedrelation");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest trigger points. DB table `areatrigger_involvedrelation` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 trigger_ID = fields[0].GetUInt32();
        uint32 quest_ID   = fields[1].GetUInt32();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", trigger_ID);
            continue;
        }

        Quest const* quest = GetQuestTemplate(quest_ID);
        if (!quest)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `areatrigger_involvedrelation` has record (id: %u) for not existing quest %u", trigger_ID, quest_ID);
            continue;
        }

        if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `areatrigger_involvedrelation` has record (id: %u) for not quest %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT. Trigger or quest flags must be fixed, quest modified to require objective.", trigger_ID, quest_ID);
            const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT); // this will prevent quest completing without objective
        }

        _questAreaTriggerStore[trigger_ID].insert(quest_ID);

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u quest trigger points in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadTavernAreaTriggers()
{
    uint32 oldMSTime = getMSTime();

    _tavernAreaTriggerStore.clear();                          // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT id FROM areatrigger_tavern");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 tavern triggers. DB table `areatrigger_tavern` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 Trigger_ID      = fields[0].GetUInt32();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }

        _tavernAreaTriggerStore.insert(Trigger_ID);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u tavern triggers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAreaTriggerScripts()
{
    uint32 oldMSTime = getMSTime();

    _areaTriggerScriptStore.clear();                            // need for reload case
    QueryResult result = WorldDatabase.Query("SELECT entry, ScriptName FROM areatrigger_scripts");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 areatrigger scripts. DB table `areatrigger_scripts` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 Trigger_ID      = fields[0].GetUInt32();
        const char *scriptName = fields[1].GetCString();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }
        _areaTriggerScriptStore[Trigger_ID] = GetScriptId(scriptName);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u areatrigger scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetNearestTaxiNode(float x, float y, float z, uint32 mapid, uint32 team)
{
    bool found = false;
    float dist = 10000;
    uint32 id = 0;

    uint32 requireFlag = (team == ALLIANCE) ? TAXI_NODE_FLAG_ALLIANCE : TAXI_NODE_FLAG_HORDE;
    for (TaxiNodesEntry const* node : sTaxiNodesStore)
    {
        if (!node || node->MapID != mapid || !(node->Flags & requireFlag))
            continue;

        uint8  field   = (uint8)((node->ID - 1) / 8);
        uint32 submask = 1 << ((node->ID - 1) % 8);

        // skip not taxi network nodes
        if ((sTaxiNodesMask[field] & submask) == 0)
            continue;

        float dist2 = (node->Pos.X - x)*(node->Pos.X - x) + (node->Pos.Y - y)*(node->Pos.Y - y) + (node->Pos.Z - z)*(node->Pos.Z - z);
        if (found)
        {
            if (dist2 < dist)
            {
                dist = dist2;
                id = node->ID;
            }
        }
        else
        {
            found = true;
            dist = dist2;
            id = node->ID;
        }
    }

    return id;
}

void ObjectMgr::GetTaxiPath(uint32 source, uint32 destination, uint32 &path, uint32 &cost)
{
    TaxiPathSetBySource::iterator src_i = sTaxiPathSetBySource.find(source);
    if (src_i == sTaxiPathSetBySource.end())
    {
        path = 0;
        cost = 0;
        return;
    }

    TaxiPathSetForSource& pathSet = src_i->second;

    TaxiPathSetForSource::iterator dest_i = pathSet.find(destination);
    if (dest_i == pathSet.end())
    {
        path = 0;
        cost = 0;
        return;
    }

    cost = dest_i->second.price;
    path = dest_i->second.ID;
}

uint32 ObjectMgr::GetTaxiMountDisplayId(uint32 id, uint32 team, bool allowed_alt_team /* = false */)
{
    uint32 mount_id = 0;

    // select mount creature id
    TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(id);
    if (node)
    {
        uint32 mount_entry = 0;
        if (team == ALLIANCE)
            mount_entry = node->MountCreatureID[1];
        else
            mount_entry = node->MountCreatureID[0];

        // Fix for Alliance not being able to use Acherus taxi
        // only one mount type for both sides
        if (mount_entry == 0 && allowed_alt_team)
        {
            // Simply reverse the selection. At least one team in theory should have a valid mount ID to choose.
            mount_entry = team == ALLIANCE ? node->MountCreatureID[0] : node->MountCreatureID[1];
        }

        CreatureTemplate const* mount_info = GetCreatureTemplate(mount_entry);
        if (mount_info)
        {
            mount_id = mount_info->GetRandomValidModelId();
            if (!mount_id)
            {
                sLog->outError(LOG_FILTER_SQL, "No displayid found for the taxi mount with the entry %u! Can't load it!", mount_entry);
                return 0;
            }
        }
    }

    // minfo is not actually used but the mount_id was updated
    GetCreatureModelRandomGender(&mount_id);

    return mount_id;
}

void ObjectMgr::LoadGraveyardZones()
{
    uint32 oldMSTime = getMSTime();

    GraveYardStore.clear();                                  // need for reload case

    //                                                0       1         2
    QueryResult result = WorldDatabase.Query("SELECT id, ghost_zone, faction FROM game_graveyard_zone");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 graveyard-zone links. DB table `game_graveyard_zone` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 safeLocId = fields[0].GetUInt32();
        uint32 zoneId = fields[1].GetUInt32();
        uint32 team   = fields[2].GetUInt16();

        if (!sWorldSafeLocsStore.LookupEntry(safeLocId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for not existing graveyard (WorldSafeLocs.dbc id) %u, skipped.", safeLocId);
            continue;
        }

        if (!sAreaTableStore.LookupEntry(zoneId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for not existing zone id (%u), skipped.", zoneId);
            continue;
        }

        if (team != 0 && team != HORDE && team != ALLIANCE)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for non player faction (%u), skipped.", team);
            continue;
        }

        if (!AddGraveYardLink(safeLocId, zoneId, team, false))
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a duplicate record for Graveyard (ID: %u) and Zone (ID: %u), skipped.", safeLocId, zoneId);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u graveyard-zone links in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

WorldSafeLocsEntry const* ObjectMgr::GetDefaultGraveYard(uint32 team)
{
    enum DefaultGraveyard
    {
        HORDE_GRAVEYARD = 10, // Crossroads
        ALLIANCE_GRAVEYARD = 4, // Westfall
    };

    if (team == HORDE)
        return sWorldSafeLocsStore.LookupEntry(HORDE_GRAVEYARD);
    else if (team == ALLIANCE)
        return sWorldSafeLocsStore.LookupEntry(ALLIANCE_GRAVEYARD);
    else return NULL;
}

WorldSafeLocsEntry const* ObjectMgr::GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team)
{
    // search for zone associated closest graveyard
    uint32 zoneId = sMapMgr->GetZoneId(MapId, x, y, z);

    if (!zoneId)
    {
        if (z > -500)
        {
            sLog->outError(LOG_FILTER_GENERAL, "ZoneId not found for map %u coords (%f, %f, %f)", MapId, x, y, z);
            return GetDefaultGraveYard(team);
        }
    }

    // Simulate std. algorithm:
    //   found some graveyard associated to (ghost_zone, ghost_map)
    //
    //   if mapId == graveyard.mapId (ghost in plain zone or city or battleground) and search graveyard at same map
    //     then check faction
    //   if mapId != graveyard.mapId (ghost in instance) and search any graveyard associated
    //     then check faction
    GraveYardContainer::const_iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::const_iterator graveUp   = GraveYardStore.upper_bound(zoneId);
    MapEntry const* map = sMapStore.LookupEntry(MapId);
    // not need to check validity of map object; MapId _MUST_ be valid here

    if (graveLow == graveUp && !map->IsBattleArena())
    {
        sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` incomplete: Zone %u Team %u does not have a linked graveyard.", zoneId, team);
        return NULL;
    }

    // at corpse map
    bool foundNear = false;
    float distNear = 10000;
    WorldSafeLocsEntry const* entryNear = NULL;

    // at entrance map for corpse map
    bool foundEntr = false;
    float distEntr = 10000;
    WorldSafeLocsEntry const* entryEntr = NULL;

    // some where other
    WorldSafeLocsEntry const* entryFar = NULL;

    MapEntry const* mapEntry = sMapStore.LookupEntry(MapId);

    for (GraveYardContainer::const_iterator itr = graveLow; itr != graveUp; ++itr)
    {
        GraveYardData const& data = itr->second;

        WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(data.safeLocId);
        if (!entry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has record for not existing graveyard (WorldSafeLocs.dbc id) %u, skipped.", data.safeLocId);
            continue;
        }

        // skip enemy faction graveyard
        // team == 0 case can be at call from .neargrave
        if (data.team != 0 && team != 0 && data.team != team)
            continue;

        // find now nearest graveyard at other map
        if (MapId != entry->MapID)
        {
            // if find graveyard at different map from where entrance placed (or no entrance data), use any first
            if (!mapEntry
                || mapEntry->CorpseMapID < 0
                || uint32(mapEntry->CorpseMapID) != entry->MapID
                || (mapEntry->CorpsePos.X == 0.0f && mapEntry->CorpsePos.Y == 0.0f))
            {
                // not have any corrdinates for check distance anyway
                entryFar = entry;
                continue;
            }

            // at entrance map calculate distance (2D);
            float dist2 = (entry->Loc.X - mapEntry->CorpsePos.X)*(entry->Loc.X - mapEntry->CorpsePos.X)
                + (entry->Loc.Y - mapEntry->CorpsePos.Y)*(entry->Loc.Y - mapEntry->CorpsePos.Y);
            if (foundEntr)
            {
                if (dist2 < distEntr)
                {
                    distEntr = dist2;
                    entryEntr = entry;
                }
            }
            else
            {
                foundEntr = true;
                distEntr = dist2;
                entryEntr = entry;
            }
        }
        // find now nearest graveyard at same map
        else
        {
            float dist2 = (entry->Loc.X - x)*(entry->Loc.X - x) + (entry->Loc.Y - y)*(entry->Loc.Y - y) + (entry->Loc.Z - z)*(entry->Loc.Z - z);
            if (foundNear)
            {
                if (dist2 < distNear)
                {
                    distNear = dist2;
                    entryNear = entry;
                }
            }
            else
            {
                foundNear = true;
                distNear = dist2;
                entryNear = entry;
            }
        }
    }

    if (entryNear)
        return entryNear;

    if (entryEntr)
        return entryEntr;

    return entryFar;
}

GraveYardData const* ObjectMgr::FindGraveYardData(uint32 id, uint32 zoneId)
{
    GraveYardContainer::const_iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::const_iterator graveUp   = GraveYardStore.upper_bound(zoneId);

    for (GraveYardContainer::const_iterator itr = graveLow; itr != graveUp; ++itr)
    {
        if (itr->second.safeLocId == id)
            return &itr->second;
    }

    return NULL;
}

bool ObjectMgr::AddGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist /*= true*/)
{
    if (FindGraveYardData(id, zoneId))
        return false;

    // add link to loaded data
    GraveYardData data;
    data.safeLocId = id;
    data.team = team;

    GraveYardStore.insert(GraveYardContainer::value_type(zoneId, data));

    // add link to DB
    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GRAVEYARD_ZONE);

        stmt->setUInt32(0, id);
        stmt->setUInt32(1, zoneId);
        stmt->setUInt16(2, uint16(team));

        WorldDatabase.Execute(stmt);
    }

    return true;
}

void ObjectMgr::RemoveGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist /*= false*/)
{
    GraveYardContainer::iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::iterator graveUp   = GraveYardStore.upper_bound(zoneId);
    if (graveLow == graveUp)
    {
        //sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` incomplete: Zone %u Team %u does not have a linked graveyard.", zoneId, team);
        return;
    }

    bool found = false;

    GraveYardContainer::iterator itr;

    for (itr = graveLow; itr != graveUp; ++itr)
    {
        GraveYardData & data = itr->second;

        // skip not matching safezone id
        if (data.safeLocId != id)
            continue;

        // skip enemy faction graveyard at same map (normal area, city, or battleground)
        // team == 0 case can be at call from .neargrave
        if (data.team != 0 && team != 0 && data.team != team)
            continue;

        found = true;
        break;
    }

    // no match, return
    if (!found)
        return;

    // remove from links
    GraveYardStore.erase(itr);

    // remove link from DB
    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GRAVEYARD_ZONE);

        stmt->setUInt32(0, id);
        stmt->setUInt32(1, zoneId);
        stmt->setUInt16(2, uint16(team));

        WorldDatabase.Execute(stmt);
    }
}

void ObjectMgr::LoadAreaTriggerTeleports()
{
    uint32 oldMSTime = getMSTime();

    _areaTriggerStore.clear();                                  // need for reload case
    _instanceGraveYardStore.clear();                            // need for reload case

    //                                                        0            1                  2                  3                  4                   5
    QueryResult result = WorldDatabase.Query("SELECT id,  target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM areatrigger_teleport");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 area trigger teleport definitions. DB table `areatrigger_teleport` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ++count;

        uint32 Trigger_ID = fields[0].GetUInt32();

        AreaTriggerStruct at;

        at.target_mapId             = fields[1].GetUInt16();
        at.target_X                 = fields[2].GetFloat();
        at.target_Y                 = fields[3].GetFloat();
        at.target_Z                 = fields[4].GetFloat();
        at.target_Orientation       = fields[5].GetFloat();

        WorldLocation loc = WorldLocation(at.target_mapId, at.target_X, at.target_Y, at.target_Z, at.target_Orientation);
        _instanceGraveYardStore[at.target_mapId].push_back(loc);

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }

        MapEntry const* mapEntry = sMapStore.LookupEntry(at.target_mapId);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) target map (ID: %u) does not exist in `Map.dbc`.", Trigger_ID, at.target_mapId);
            continue;
        }

        if (at.target_X == 0 && at.target_Y == 0 && at.target_Z == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) target coordinates not provided.", Trigger_ID);
            continue;
        }

        _areaTriggerStore[Trigger_ID] = at;

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u area trigger teleport definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAccessRequirements()
{
    uint32 oldMSTime = getMSTime();

    _accessRequirementStore.clear();                                  // need for reload case

    //                                               0      1           2          3          4           5     6      7             8             9                      10                        11
    QueryResult result = WorldDatabase.Query("SELECT mapid, difficulty, level_min, level_max, item_level, item, item2, quest_done_A, quest_done_H, completed_achievement, quest_failed_text, completed_achievement_A FROM access_requirement");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 access requirement definitions. DB table `access_requirement` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ++count;

        uint32 mapid = fields[0].GetUInt32();
        uint8 difficulty = fields[1].GetUInt8();
        uint32 requirement_ID = MAKE_PAIR32(mapid, difficulty);

        AccessRequirement ar;

        ar.levelMin                 = fields[2].GetUInt8();
        ar.levelMax                 = fields[3].GetUInt8();
        ar.item_level               = fields[4].GetUInt16();
        ar.item                     = fields[5].GetUInt32();
        ar.item2                    = fields[6].GetUInt32();
        ar.quest_A                  = fields[7].GetUInt32();
        ar.quest_H                  = fields[8].GetUInt32();
        ar.achievement              = fields[9].GetUInt32();
        ar.questFailedText          = fields[10].GetString();
        ar.achievement_A            = fields[11].GetUInt32();

        if (ar.item)
        {
            ItemTemplate const* pProto = GetItemTemplate(ar.item);
            if (!pProto)
            {
                sLog->outError(LOG_FILTER_GENERAL, "Key item %u does not exist for map %u difficulty %u, removing key requirement.", ar.item, mapid, difficulty);
                ar.item = 0;
            }
        }

        if (ar.item2)
        {
            ItemTemplate const* pProto = GetItemTemplate(ar.item2);
            if (!pProto)
            {
                sLog->outError(LOG_FILTER_GENERAL, "Second item %u does not exist for map %u difficulty %u, removing key requirement.", ar.item2, mapid, difficulty);
                ar.item2 = 0;
            }
        }

        if (ar.quest_A)
        {
            if (!GetQuestTemplate(ar.quest_A))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Alliance Quest %u not exist for map %u difficulty %u, remove quest done requirement.", ar.quest_A, mapid, difficulty);
                ar.quest_A = 0;
            }
        }

        if (ar.quest_H)
        {
            if (!GetQuestTemplate(ar.quest_H))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Horde Quest %u not exist for map %u difficulty %u, remove quest done requirement.", ar.quest_H, mapid, difficulty);
                ar.quest_H = 0;
            }
        }

        if (ar.achievement)
        {
            if (!sAchievementStore.LookupEntry(ar.achievement))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Achievement %u not exist for map %u difficulty %u, remove quest done requirement.", ar.achievement, mapid, difficulty);
                ar.achievement = 0;
            }
        }

        if (ar.achievement_A)
        {
            if (!sAchievementStore.LookupEntry(ar.achievement_A))
            {
                sLog->outError(LOG_FILTER_SQL, "Required achievement_A %u not exist for map %u difficulty %u, remove quest done requirement.", ar.achievement_A, mapid, difficulty);
                ar.achievement_A = 0;
            }
        }

        _accessRequirementStore[requirement_ID] = ar;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u access requirement definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

/*
 * Searches for the areatrigger which teleports players out of the given map with instance_template.parent field support
 */
AreaTriggerStruct const* ObjectMgr::GetGoBackTrigger(uint32 Map) const
{
    bool useParentDbValue = false;
    uint32 parentId = 0;
    const MapEntry* mapEntry = sMapStore.LookupEntry(Map);
    if (!mapEntry || mapEntry->CorpseMapID < 0)
        return NULL;

    if (mapEntry->IsDungeon())
    {
        const InstanceTemplate* iTemplate = sObjectMgr->GetInstanceTemplate(Map);

        if (!iTemplate)
            return NULL;

        parentId = iTemplate->Parent;
        useParentDbValue = true;
    }

    uint32 corpseMapID = uint32(mapEntry->CorpseMapID);
    for (AreaTriggerContainer::const_iterator itr = _areaTriggerStore.begin(); itr != _areaTriggerStore.end(); ++itr)
        if ((!useParentDbValue && itr->second.target_mapId == corpseMapID) || (useParentDbValue && itr->second.target_mapId == parentId))
        {
            AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(itr->first);
            if (atEntry && atEntry->MapID == Map)
                return &itr->second;
        }
    return NULL;
}

/**
 * Searches for the areatrigger which teleports players to the given map
 */
AreaTriggerStruct const* ObjectMgr::GetMapEntranceTrigger(uint32 Map) const
{
    for (AreaTriggerContainer::const_iterator itr = _areaTriggerStore.begin(); itr != _areaTriggerStore.end(); ++itr)
    {
        if (itr->second.target_mapId == Map)
        {
            AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(itr->first);
            if (atEntry)
                return &itr->second;
        }
    }
    return NULL;
}

void ObjectMgr::SetHighestGuids()
{
    QueryResult result = CharacterDatabase.Query("SELECT MAX(guid) FROM characters");
    if (result)
        _playerGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    result = WorldDatabase.Query("SELECT MAX(guid) FROM creature");
    if (result)
        _creatureGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM item_instance");
    if (result)
        _itemGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    // Cleanup other tables from not existed guids ( >= _hiItemGuid)
    CharacterDatabase.PExecute("DELETE FROM character_inventory WHERE item >= '%u'", _itemGuidGenerator.GetNextAfterMaxUsed());      // One-time query
    CharacterDatabase.PExecute("DELETE FROM mail_items WHERE item_guid >= '%u'", _itemGuidGenerator.GetNextAfterMaxUsed());          // One-time query
    CharacterDatabase.PExecute("DELETE FROM auctionhouse WHERE itemguid >= '%u'", _itemGuidGenerator.GetNextAfterMaxUsed());         // One-time query
    CharacterDatabase.PExecute("DELETE FROM guild_bank_item WHERE item_guid >= '%u'", _itemGuidGenerator.GetNextAfterMaxUsed());     // One-time query

    result = WorldDatabase.Query("SELECT MAX(guid) FROM gameobject");
    if (result)
        _gameObjectGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    result = WorldDatabase.Query("SELECT MAX(guid) FROM transports");
    if (result)
        _moTransportGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    result = CharacterDatabase.Query("SELECT MAX(id) FROM auctionhouse");
    if (result)
        _auctionId = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(id) FROM mail");
    if (result)
        _mailId = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(corpseGuid) FROM corpse");
    if (result)
        _corpseGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    result = CharacterDatabase.Query("SELECT MAX(setguid) FROM character_equipmentsets");
    if (result)
        _equipmentSetGuid = (*result)[0].GetUInt64()+1;

    result = CharacterDatabase.Query("SELECT MAX(guildId) FROM guild");
    if (result)
        sGuildMgr->SetNextGuildId((*result)[0].GetUInt64()+1);

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM groups");
    if (result)
        sGroupMgr->SetGroupDbStoreSize((*result)[0].GetUInt32()+1);

    result = CharacterDatabase.Query("SELECT MAX(itemId) from character_void_storage");
    if (result)
        _voidItemId = (*result)[0].GetUInt64()+1;

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM account_battle_pet_journal");
    if (result)
        _BattlePetGuidGenerator.Set((*result)[0].GetUInt64() + 1);

    //@TODO make table to save PetBattles
    result = CharacterDatabase.Query("SELECT MAX(guid) FROM account_battle_pet_journal");
    if (result)
        _PetBattleGuidGenerator.Set((*result)[0].GetUInt64() + 1);
}

uint64 ObjectMgr::GenerateReportComplaintID()
{
    if (QueryResult result = CharacterDatabase.Query("SELECT MAX(ID) FROM report_complaints"))
        _reportComplaintID = (*result)[0].GetUInt64();

    if (_reportComplaintID >= std::numeric_limits<uint64>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "_reportComplaintID overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _reportComplaintID++;
}

uint64 ObjectMgr::GenerateSupportTicketSubmitBugID()
{
    if (QueryResult result = CharacterDatabase.Query("SELECT MAX(ID) FROM report_bugreport"))
        _supportTicketSubmitBugID = (*result)[0].GetUInt64();

    if (_supportTicketSubmitBugID >= std::numeric_limits<uint64>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "_reportComplaintID overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _supportTicketSubmitBugID++;
}

uint32 ObjectMgr::GenerateAuctionID()
{
    if (_auctionId >= std::numeric_limits<uint32>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auctions ids overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _auctionId++;
}

uint64 ObjectMgr::GenerateEquipmentSetGuid()
{
    if (_equipmentSetGuid >= std::numeric_limits<uint64>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "EquipmentSetInfo guid overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _equipmentSetGuid++;
}

uint32 ObjectMgr::GenerateMailID()
{
    if (_mailId >= std::numeric_limits<uint32>::max())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Mail ids overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _mailId++;
}

void ObjectMgr::LoadGameObjectLocales()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectLocaleStore.clear();                           // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "name_loc1, name_loc2, name_loc3, name_loc4, name_loc5, name_loc6, name_loc7, name_loc8, name_loc9, name_loc10, "
        "castbarcaption_loc1, castbarcaption_loc2, castbarcaption_loc3, castbarcaption_loc4, "
        "castbarcaption_loc5, castbarcaption_loc6, castbarcaption_loc7, castbarcaption_loc8, castbarcaption_loc9, castbarcaption_loc10 FROM locales_gameobject");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        GameObjectLocale& data = _gameObjectLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.Name);

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + (TOTAL_LOCALES - 1)].GetString(), LocaleConstant(i), data.CastBarCaption);
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu gameobject locale strings in %u ms", (unsigned long)_gameObjectLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

inline void CheckGOLockId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (sLockStore.LookupEntry(dataN))
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but lock (Id: %u) not found.",
        goInfo->entry, goInfo->type, N, goInfo->GetLockId(), goInfo->GetLockId());
}

inline void CheckGOLinkedTrapId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (GameObjectTemplate const* trapInfo = sObjectMgr->GetGameObjectTemplate(dataN))
    {
        if (trapInfo->type != GAMEOBJECT_TYPE_TRAP)
            sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but GO (Entry %u) have not GAMEOBJECT_TYPE_TRAP (%u) type.",
            goInfo->entry, goInfo->type, N, dataN, dataN, GAMEOBJECT_TYPE_TRAP);
    }
}

inline void CheckGOSpellId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (sSpellMgr->GetSpellInfo(dataN))
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but Spell (Entry %u) not exist.",
        goInfo->entry, goInfo->type, N, dataN, dataN);
}

inline void CheckAndFixGOChairHeightId(GameObjectTemplate const* goInfo, uint32 const& dataN, uint32 N)
{
    if (dataN <= (UNIT_STAND_STATE_SIT_HIGH_CHAIR-UNIT_STAND_STATE_SIT_LOW_CHAIR))
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but correct chair height in range 0..%i.",
        goInfo->entry, goInfo->type, N, dataN, UNIT_STAND_STATE_SIT_HIGH_CHAIR-UNIT_STAND_STATE_SIT_LOW_CHAIR);

    // prevent client and server unexpected work
    const_cast<uint32&>(dataN) = 0;
}

inline void CheckGONoDamageImmuneId(GameObjectTemplate* goTemplate, uint32 dataN, uint32 N)
{
    // 0/1 correct values
    if (dataN <= 1)
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but expected boolean (0/1) noDamageImmune field value.", goTemplate->entry, goTemplate->type, N, dataN);
}

inline void CheckGOConsumable(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    // 0/1 correct values
    if (dataN <= 1)
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but expected boolean (0/1) consumable field value.",
        goInfo->entry, goInfo->type, N, dataN);
}

void ObjectMgr::LoadGameObjectTemplate()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0      1      2        3       4             5          6      7       8     9        10         11          12
    QueryResult result = WorldDatabase.Query("SELECT entry, type, displayId, name, IconName, castBarCaption, unk1, faction, flags, size, questItem1, questItem2, questItem3, "
    //                                            13          14          15       16     17     18     19     20     21     22     23     24     25      26      27      28
                                             "questItem4, questItem5, questItem6, Data0, Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8, Data9, Data10, Data11, Data12, "
    //                                          29      30      31      32      33      34      35      36      37      38      39      40      41      42      43      44
                                             "Data13, Data14, Data15, Data16, Data17, Data18, Data19, Data20, Data21, Data22, Data23, Data24, Data25, Data26, Data27, Data28, "
    //                                          45      46      47      48       49       50        51          52              53                 54                55                  56                  57
                                             "Data29, Data30, Data31, Data32, unkInt32, AIName, ScriptName, WorldEffectID, SpellVisualID, SpellStateVisualID, SpellStateAnimID, SpellStateAnimKitID, StateWorldEffectID  "
                                             "FROM gameobject_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gameobject definitions. DB table `gameobject_template` is empty.");
        return;
    }

    std::list<uint32> tempList = sDB2Manager.GetGameObjectsList();

    _gameObjectTemplateStore.rehash(result->GetRowCount() + tempList.size());

    for (uint32 const& itr : tempList)
    {
        if (GameObjectsEntry const* goe = sGameObjectsStore.LookupEntry(itr))
        {
            GameObjectTemplate& got = _gameObjectTemplateStore[goe->ID];

            got.entry          = goe->ID;
            got.type           = goe->Type;
            got.displayId      = goe->DisplayID;
            got.name           = goe->Name->Str[DEFAULT_LOCALE];
            got.IconName       = "";
            got.castBarCaption = "";
            got.unk1           = "";
            got.faction        = 0;
            got.flags          = 0;
            got.size           = goe->Size;

            for (uint8 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
                got.questItems[i] = 0;

            for (uint8 i = 0; i < 8; ++i)
                got.raw.data[i] = goe->Data[i];

            got.unkInt32 = 0;
            got.AIName = "";
            got.ScriptId = GetScriptId("");

            //++count;
        }
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        GameObjectTemplate& got = _gameObjectTemplateStore[entry];
        got.entry          = entry;
        got.type           = uint32(fields[1].GetUInt8());
        got.displayId      = fields[2].GetUInt32();
        got.name           = fields[3].GetString();
        got.IconName       = fields[4].GetString();
        got.castBarCaption = fields[5].GetString();
        got.unk1           = fields[6].GetString();
        got.faction        = uint32(fields[7].GetUInt16());
        got.flags          = fields[8].GetUInt32();
        got.size           = fields[9].GetFloat();

        for (uint8 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            got.questItems[i] = fields[10 + i].GetUInt32();

        for (uint8 i = 0; i < MAX_GAMEOBJECT_DATA; ++i)
            got.raw.data[i] = fields[16 + i].GetInt32();

        got.unkInt32 = fields[49].GetInt32();
        got.AIName = fields[50].GetString();
        got.ScriptId = GetScriptId(fields[51].GetCString());
        got.visualQuestID = 0;
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].WorldEffectID = fields[52].GetInt32();
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellVisualID = fields[53].GetInt32();
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID = fields[54].GetInt32();
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateAnimID = fields[55].GetInt32();
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateAnimKitID = fields[56].GetInt32();
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID = fields[57].GetInt32();

        // Checks

        switch (got.type)
        {
            case GAMEOBJECT_TYPE_DOOR:                      //0
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 1);
                CheckGONoDamageImmuneId(&got, got.door.noDamageImmune, 3);
                break;
            case GAMEOBJECT_TYPE_BUTTON:                    //1
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 1);
                CheckGONoDamageImmuneId(&got, got.button.noDamageImmune, 4);
                break;
            case GAMEOBJECT_TYPE_QUESTGIVER:                //2
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);
                CheckGONoDamageImmuneId(&got, got.questgiver.noDamageImmune, 5);
                break;
            case GAMEOBJECT_TYPE_CHEST:                     //3
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);

                CheckGOConsumable(&got, got.chest.consumable, 3);

                if (got.chest.linkedTrap)              // linked trap
                    CheckGOLinkedTrapId(&got, got.chest.linkedTrap, 7);
                break;
            case GAMEOBJECT_TYPE_TRAP:                      //6
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);
                break;
            case GAMEOBJECT_TYPE_CHAIR:                     //7
                CheckAndFixGOChairHeightId(&got, got.chair.chairheight, 1);
                break;
            case GAMEOBJECT_TYPE_SPELL_FOCUS:               //8
                if (got.spellFocus.spellFocusType)
                    if (!sSpellFocusObjectStore.LookupEntry(got.spellFocus.spellFocusType))
                        sLog->outError(LOG_FILTER_SQL, "GameObject (Entry: %u GoType: %u) have data0=%u but SpellFocus (Id: %u) not exist.",
                        entry, got.type, got.spellFocus.spellFocusType, got.spellFocus.spellFocusType);

                if (got.spellFocus.linkedTrap)        // linked trap
                    CheckGOLinkedTrapId(&got, got.spellFocus.linkedTrap, 2);
                break;
            case GAMEOBJECT_TYPE_GOOBER:                    //10
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);

                CheckGOConsumable(&got, got.goober.consumable, 3);

                if (got.goober.pageID)                  // pageId
                    if (!GetPageText(got.goober.pageID))
                        sLog->outError(LOG_FILTER_SQL, "GameObject (Entry: %u GoType: %u) have data7=%u but PageText (Entry %u) not exist.",
                        entry, got.type, got.goober.pageID, got.goober.pageID);

                CheckGONoDamageImmuneId(&got, got.goober.noDamageImmune, 11);
                if (got.goober.linkedTrap)            // linked trap
                    CheckGOLinkedTrapId(&got, got.goober.linkedTrap, 12);
                break;
            case GAMEOBJECT_TYPE_TRANSPORT:                 // 11
            {
                auto const& itr = sDB2Manager._transportAnimationsByEntry.find(got.entry);
                if (itr == sDB2Manager._transportAnimationsByEntry.end())
                {
                    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) is transport by does not have entries in TransportAnimation.dbc! Gameobject is obsolete.",
                        got.entry, got.type);
                    break;
                }

                //ToDo:: Timeto5thfloor is it old frames???????
                if (uint32 frame = got.transport.Timeto2ndfloor)
                    if (itr->second.find(frame) == itr->second.end())
                        sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) has data0=%u but this frame is not in TransportAnimation.dbc!",
                            got.entry, got.type, frame);

                if (uint32 frame = got.transport.Timeto3rdfloor)
                    if (itr->second.find(frame) == itr->second.end())
                        sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) has data6=%u but this frame is not in TransportAnimation.dbc!",
                            got.entry, got.type, frame);

                if (uint32 frame = got.transport.Timeto4thfloor)
                    if (itr->second.find(frame) == itr->second.end())
                        sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) has data8=%u but this frame is not in TransportAnimation.dbc!",
                            got.entry, got.type, frame);

                if (uint32 frame = got.transport.Timeto5thfloor)
                    if (itr->second.find(frame) == itr->second.end())
                        sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) has data10=%u but this frame is not in TransportAnimation.dbc!",
                            got.entry, got.type, frame);
                break;
            }
            case GAMEOBJECT_TYPE_AREADAMAGE:                //12
            case GAMEOBJECT_TYPE_CAMERA:                    //13
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);
                break;
            case GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT:              //15
                if (got.moTransport.taxiPathID)
                    if (got.moTransport.taxiPathID >= sTaxiPathNodesByPath.size() || sTaxiPathNodesByPath[got.moTransport.taxiPathID].empty())
                        sLog->outError(LOG_FILTER_SQL, "GameObject (Entry: %u GoType: %u) have data0=%u but TaxiPath (Id: %u) not exist.",
                        entry, got.type, got.moTransport.taxiPathID, got.moTransport.taxiPathID);
                break;
            case GAMEOBJECT_TYPE_RITUAL:          //18
                break;
            case GAMEOBJECT_TYPE_SPELLCASTER:               //22
                CheckGOSpellId(&got, got.spellCaster.spell, 0);
                break;
            case GAMEOBJECT_TYPE_FLAGSTAND:                 //24
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);
                CheckGONoDamageImmuneId(&got, got.flagStand.noDamageImmune, 5);
                break;
            case GAMEOBJECT_TYPE_FISHINGHOLE:               //25
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 4);
                break;
            case GAMEOBJECT_TYPE_FLAGDROP:                  //26
                if (got.GetLockId())
                    CheckGOLockId(&got, got.GetLockId(), 0);
                CheckGONoDamageImmuneId(&got, got.flagDrop.noDamageImmune, 3);
                break;
            case GAMEOBJECT_TYPE_BARBER_CHAIR:              //32
                CheckAndFixGOChairHeightId(&got, got.barberChair.chairheight, 0);
                break;
            case GAMEOBJECT_TYPE_GARRISON_BUILDING:
                //if (uint32 transportMap = got.garrisonBuilding.mapID)
                //    _transportMaps.insert(transportMap);
                break;
        }

       ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u game object templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameObjectQuestVisual()
{
    uint32 oldMSTime = getMSTime();

    //                                                  0       1           2                                   3                               4                               5
    QueryResult result = WorldDatabase.Query("SELECT `goID`, `questID`, `incomplete_state_spell_visual`, `incomplete_state_world_effect`, `complete_state_spell_visual`, `complete_state_world_effect` FROM gameobject_quest_visual");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gameobject quest visual. DB table `gameobject_quest_visual` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 goID = fields[0].GetUInt32();
        uint32 questID = fields[1].GetUInt32();
        if (!GetGameObjectTemplate(goID))
        {
            sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u) not exist on table `gameobject_quest_visual`", goID);
            continue;
        }
        if (questID && !GetQuestTemplate(questID))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest (ID: %u) not exist on table `gameobject_quest_visual`", questID);
            continue;
        }
        GameObjectTemplate& got = _gameObjectTemplateStore[goID];
        got.visualQuestID = questID;
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID = fields[2].GetUInt32();
        got.visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID = fields[3].GetUInt32();
        got.visualData[GO_VISUAL_AFTER_COMPLETEQUEST].SpellStateVisualID = fields[4].GetUInt32();
        got.visualData[GO_VISUAL_AFTER_COMPLETEQUEST].StateWorldEffectID = fields[5].GetUInt32();
        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u game object quest visual in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadExplorationBaseXP()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT level, basexp FROM exploration_basexp");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 BaseXP definitions. DB table `exploration_basexp` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint8 level  = fields[0].GetUInt8();
        uint32 basexp = fields[1].GetInt32();
        _baseXPTable[level] = basexp;
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u BaseXP definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetBaseXP(uint8 level)
{
    return _baseXPTable[level] ? _baseXPTable[level] : 0;
}

uint32 ObjectMgr::GetXPForLevel(uint8 level) const
{
    if (level < _playerXPperLevel.size())
        return _playerXPperLevel[level];
    return 0;
}

void ObjectMgr::LoadPetNames()
{
    uint32 oldMSTime = getMSTime();
    //                                                0     1      2
    QueryResult result = WorldDatabase.Query("SELECT word, entry, half FROM pet_name_generation");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 pet name parts. DB table `pet_name_generation` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        std::string word = fields[0].GetString();
        uint32 entry     = fields[1].GetUInt32();
        bool   half      = fields[2].GetBool();
        if (half)
            _petHalfName1[entry].push_back(word);
        else
            _petHalfName0[entry].push_back(word);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pet name parts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPetNumber()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT MAX(id) FROM character_pet");
    if (result)
    {
        Field* fields = result->Fetch();
        _hiPetNumber = fields[0].GetUInt32()+1;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded the max pet number: %d in %u ms", _hiPetNumber-1, GetMSTimeDiffToNow(oldMSTime));
}

std::string ObjectMgr::GeneratePetName(uint32 entry)
{
    StringVector & list0 = _petHalfName0[entry];
    StringVector & list1 = _petHalfName1[entry];

    if (list0.empty() || list1.empty())
    {
        CreatureTemplate const* cinfo = GetCreatureTemplate(entry);
        const char* petname = sDB2Manager.GetPetName(cinfo->Family, sWorld->GetDefaultDbcLocale());
        if (!petname)
            return cinfo->Name[0];

        return std::string(petname);
    }

    return *(list0.begin()+urand(0, list0.size()-1)) + *(list1.begin()+urand(0, list1.size()-1));
}

uint32 ObjectMgr::GeneratePetNumber()
{
    return ++_hiPetNumber;
}

uint64 ObjectMgr::GenerateVoidStorageItemId()
{
    return ++_voidItemId;
}

void ObjectMgr::LoadCurrencysLoot()
{
    //                                                  0       1       2           3                   4               5       6
    QueryResult result = WorldDatabase.PQuery("SELECT entry, type, currencyId, currencyAmount, currencyMaxAmount, lootmode, chance FROM currency_loot");
    if (!result)
        return;

    uint32 count = 0;
    do
    {
        Field* field = result->Fetch();

        uint32 entry = field[0].GetUInt32();
        uint8 type = field[1].GetInt8();
        uint32 currencyId = field[2].GetUInt32();
        uint32 currencyAmount = field[3].GetUInt32();
        uint32 currencyMaxAmount = field[4].GetUInt32();
        uint32 lootmode = field[5].GetUInt32();
        float chance = field[6].GetFloat();

        if (type < 1)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Currency 'type' can not be < 1 (entry = %u type = %i)", entry, type);
            continue;
        }
        else if (type > 3)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Currency 'type' can not be > 3 (entry = %u type = %i)", entry, type);
            continue;
        }

        if (chance > 100.0f || !chance)
            chance = 100.0f;

        CurrencyLoot loot = CurrencyLoot(entry, type, currencyId, currencyAmount, currencyMaxAmount, lootmode, chance);
        _currencysLoot.push_back(loot);
        ++count;
    }
    while (result->NextRow());

    if (count)
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loaded %u currency loot definition", count);
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loaded 0 currency loot definition. Table is empty!");
}

void ObjectMgr::LoadCorpses()
{
    //        0     1     2     3            4      5          6          7       8       9      10        11    12          13          14          15         16
    // SELECT posX, posY, posZ, orientation, mapId, displayId, itemCache, bytes1, bytes2, flags, dynFlags, time, corpseType, instanceId, phaseMask, corpseGuid, guid FROM corpse WHERE corpseType <> 0

    uint32 oldMSTime = getMSTime();

    PreparedQueryResult result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_CORPSES));
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 corpses. DB table `corpse` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        ObjectGuid::LowType guid = fields[16].GetUInt64();
        CorpseType type = CorpseType(fields[12].GetUInt8());
        if (type >= MAX_CORPSE_TYPE)
        {
            sLog->outError(LOG_FILTER_GENERAL, "Corpse (guid: %u) have wrong corpse type (%u), not loading.", guid, type);
            continue;
        }

        Corpse* corpse = new Corpse(type);
        if (!corpse->LoadCorpseFromDB(guid, fields))
        {
            delete corpse;
            continue;
        }

        sObjectAccessor->AddCorpse(corpse);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u corpses in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationRewardRate()
{
    uint32 oldMSTime = getMSTime();

    _repRewardRateStore.clear();                             // for reload case

    uint32 count = 0; //                                0          1            2             3
    QueryResult result = WorldDatabase.Query("SELECT faction, quest_rate, creature_rate, spell_rate FROM reputation_reward_rate");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SQL, ">> Loaded `reputation_reward_rate`, table is empty!");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 factionId        = fields[0].GetUInt32();

        RepRewardRate repRate;

        repRate.quest_rate      = fields[1].GetFloat();
        repRate.creature_rate   = fields[2].GetFloat();
        repRate.spell_rate      = fields[3].GetFloat();

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);
        if (!factionEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_reward_rate`", factionId);
            continue;
        }

        if (repRate.quest_rate < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table reputation_reward_rate has quest_rate with invalid rate %f, skipping data for faction %u", repRate.quest_rate, factionId);
            continue;
        }

        if (repRate.creature_rate < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table reputation_reward_rate has creature_rate with invalid rate %f, skipping data for faction %u", repRate.creature_rate, factionId);
            continue;
        }

        if (repRate.spell_rate < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table reputation_reward_rate has spell_rate with invalid rate %f, skipping data for faction %u", repRate.spell_rate, factionId);
            continue;
        }

        _repRewardRateStore[factionId] = repRate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u reputation_reward_rate in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationOnKill()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    _repOnKillStore.clear();

    uint32 count = 0;

    //                                                0            1                     2
    QueryResult result = WorldDatabase.Query("SELECT creature_id, RewOnKillRepFaction1, RewOnKillRepFaction2, "
    //   3             4             5                   6             7             8                   9
        "IsTeamAward1, MaxStanding1, RewOnKillRepValue1, IsTeamAward2, MaxStanding2, RewOnKillRepValue2, TeamDependent "
        "FROM creature_onkill_reputation");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature award reputation definitions. DB table `creature_onkill_reputation` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 creature_id = fields[0].GetUInt32();

        ReputationOnKillEntry repOnKill;
        repOnKill.RepFaction1          = fields[1].GetInt16();
        repOnKill.RepFaction2          = fields[2].GetInt16();
        repOnKill.IsTeamAward1        = fields[3].GetBool();
        repOnKill.ReputationMaxCap1  = fields[4].GetUInt8();
        repOnKill.RepValue1            = fields[5].GetInt32();
        repOnKill.IsTeamAward2        = fields[6].GetBool();
        repOnKill.ReputationMaxCap2  = fields[7].GetUInt8();
        repOnKill.RepValue2            = fields[8].GetInt32();
        repOnKill.TeamDependent       = fields[9].GetBool();

        if (!GetCreatureTemplate(creature_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_onkill_reputation` have data for not existed creature entry (%u), skipped", creature_id);
            continue;
        }

        if (repOnKill.RepFaction1)
        {
            FactionEntry const* factionEntry1 = sFactionStore.LookupEntry(repOnKill.RepFaction1);
            if (!factionEntry1)
            {
                sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `creature_onkill_reputation`", repOnKill.RepFaction1);
                continue;
            }
        }

        if (repOnKill.RepFaction2)
        {
            FactionEntry const* factionEntry2 = sFactionStore.LookupEntry(repOnKill.RepFaction2);
            if (!factionEntry2)
            {
                sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `creature_onkill_reputation`", repOnKill.RepFaction2);
                continue;
            }
        }

        _repOnKillStore[creature_id] = repOnKill;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature award reputation definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationSpilloverTemplate()
{
    uint32 oldMSTime = getMSTime();

    _repSpilloverTemplateStore.clear();                      // for reload case

    uint32 count = 0; //                                0         1        2       3        4       5       6         7        8      9        10       11     12        13       14     15
    QueryResult result = WorldDatabase.Query("SELECT faction, faction1, rate_1, rank_1, faction2, rate_2, rank_2, faction3, rate_3, rank_3, faction4, rate_4, rank_4, faction5, rate_5, rank_5 FROM reputation_spillover_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded `reputation_spillover_template`, table is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 factionId                = fields[0].GetUInt16();

        RepSpilloverTemplate repTemplate;

        repTemplate.faction[0]          = fields[1].GetUInt16();
        repTemplate.faction_rate[0]     = fields[2].GetFloat();
        repTemplate.faction_rank[0]     = fields[3].GetUInt8();
        repTemplate.faction[1]          = fields[4].GetUInt16();
        repTemplate.faction_rate[1]     = fields[5].GetFloat();
        repTemplate.faction_rank[1]     = fields[6].GetUInt8();
        repTemplate.faction[2]          = fields[7].GetUInt16();
        repTemplate.faction_rate[2]     = fields[8].GetFloat();
        repTemplate.faction_rank[2]     = fields[9].GetUInt8();
        repTemplate.faction[3]          = fields[10].GetUInt16();
        repTemplate.faction_rate[3]     = fields[11].GetFloat();
        repTemplate.faction_rank[3]     = fields[12].GetUInt8();
        repTemplate.faction[4]          = fields[13].GetUInt16();
        repTemplate.faction_rate[4]     = fields[14].GetFloat();
        repTemplate.faction_rank[4]     = fields[15].GetUInt8();

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);

        if (!factionEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", factionId);
            continue;
        }

        if (factionEntry->Team == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u in `reputation_spillover_template` does not belong to any team, skipping", factionId);
            continue;
        }

        for (uint32 i = 0; i < MAX_SPILLOVER_FACTIONS; ++i)
        {
            if (repTemplate.faction[i])
            {
                FactionEntry const* factionSpillover = sFactionStore.LookupEntry(repTemplate.faction[i]);

                if (!factionSpillover)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spillover faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template` for faction %u, skipping", repTemplate.faction[i], factionId);
                    continue;
                }

                if (factionSpillover->ReputationListID < 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spillover faction (faction.dbc) %u for faction %u in `reputation_spillover_template` can not be listed for client, and then useless, skipping", repTemplate.faction[i], factionId);
                    continue;
                }

                if (repTemplate.faction_rank[i] >= MAX_REPUTATION_RANK)
                {
                    sLog->outError(LOG_FILTER_SQL, "Rank %u used in `reputation_spillover_template` for spillover faction %u is not valid, skipping", repTemplate.faction_rank[i], repTemplate.faction[i]);
                    continue;
                }
            }
        }

        FactionEntry const* factionEntry0 = sFactionStore.LookupEntry(repTemplate.faction[0]);
        if (repTemplate.faction[0] && !factionEntry0)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[0]);
            continue;
        }
        FactionEntry const* factionEntry1 = sFactionStore.LookupEntry(repTemplate.faction[1]);
        if (repTemplate.faction[1] && !factionEntry1)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[1]);
            continue;
        }
        FactionEntry const* factionEntry2 = sFactionStore.LookupEntry(repTemplate.faction[2]);
        if (repTemplate.faction[2] && !factionEntry2)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[2]);
            continue;
        }
        FactionEntry const* factionEntry3 = sFactionStore.LookupEntry(repTemplate.faction[3]);
        if (repTemplate.faction[3] && !factionEntry3)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[3]);
            continue;
        }
        FactionEntry const* factionEntry4 = sFactionStore.LookupEntry(repTemplate.faction[4]);
        if (repTemplate.faction[4] && !factionEntry4)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[4]);
            continue;
        }

        _repSpilloverTemplateStore[factionId] = repTemplate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u reputation_spillover_template in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPointsOfInterest()
{
    uint32 oldMSTime = getMSTime();

    _pointsOfInterestStore.clear();                              // need for reload case

    uint32 count = 0;

    //                                                  0   1  2   3      4     5       6
    QueryResult result = WorldDatabase.Query("SELECT entry, x, y, icon, flags, data, icon_name FROM points_of_interest");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Points of Interest definitions. DB table `points_of_interest` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 point_id = fields[0].GetUInt32();

        PointOfInterest POI;
        POI.x = fields[1].GetFloat();
        POI.y = fields[2].GetFloat();
        POI.icon = fields[3].GetUInt32();
        POI.flags = fields[4].GetUInt32();
        POI.data = fields[5].GetUInt32();
        POI.icon_name = fields[6].GetString();

        if (!Trinity::IsValidMapCoord(POI.x, POI.y))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `points_of_interest` (Entry: %u) have invalid coordinates (X: %f Y: %f), ignored.", point_id, POI.x, POI.y);
            continue;
        }

        _pointsOfInterestStore[point_id] = POI;

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Points of Interest definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestPOI()
{
    uint32 oldMSTime = getMSTime();

    _questPOIStore.clear();                              // need for reload case

    uint32 count = 0;

    //                                                   0        1        2            3           4                 5           6         7            8       9       10         11              12             13
    QueryResult result = WorldDatabase.Query("SELECT QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, WorldMapAreaId, Floor, Priority, Flags, WorldEffectID, PlayerConditionID, WoDUnk1 FROM quest_poi order by QuestID, Idx1");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest POI definitions. DB table `quest_poi` is empty.");

        return;
    }

    //                                                0        1    2  3
    QueryResult points = WorldDatabase.Query("SELECT QuestID, Idx1, X, Y FROM quest_poi_points ORDER BY QuestID DESC, Idx1, Idx2");

    std::vector<std::vector<std::vector<QuestPOIPoint>>> POIs;

    if (points)
    {
        // The first result should have the highest questId
        Field* fields = points->Fetch();
        uint32 questIdMax = fields[0].GetInt32();
        POIs.resize(questIdMax + 1);

        do
        {
            fields = points->Fetch();

            int32 QuestID             = fields[0].GetInt32();
            int32 Idx1                = fields[1].GetInt32();
            int32 X                   = fields[2].GetInt32();
            int32 Y                   = fields[3].GetInt32();

            if (int32(POIs[QuestID].size()) <= Idx1 + 1)
                POIs[QuestID].resize(Idx1 + 10);

            QuestPOIPoint point(X, Y);
            POIs[QuestID][Idx1].push_back(point);
        } while (points->NextRow());
    }

    do
    {
        Field* fields = result->Fetch();

        int32 QuestID               = fields[0].GetInt32();
        int32 BlobIndex             = fields[1].GetInt32();
        int32 Idx1                  = fields[2].GetInt32();
        int32 ObjectiveIndex        = fields[3].GetInt32();
        int32 QuestObjectiveID      = fields[4].GetInt32();
        int32 QuestObjectID         = fields[5].GetInt32();
        int32 MapID                 = fields[6].GetInt32();
        int32 WorldMapAreaId        = fields[7].GetInt32();
        int32 Floor                 = fields[8].GetInt32();
        int32 Priority              = fields[9].GetInt32();
        int32 Flags                 = fields[10].GetInt32();
        int32 WorldEffectID         = fields[11].GetInt32();
        int32 PlayerConditionID     = fields[12].GetInt32();
        int32 WoDUnk1               = fields[13].GetInt32();

        QuestPOI POI(BlobIndex, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, WorldMapAreaId, Floor, Priority, Flags, WorldEffectID, PlayerConditionID, WoDUnk1);
        if (QuestID < int32(POIs.size()) && Idx1 < int32(POIs[QuestID].size()))
        {
            POI.points = POIs[QuestID][Idx1];
            _questPOIStore[QuestID].push_back(POI);
        }
        else
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Table quest_poi references unknown quest points for quest %i POI id %i", QuestID, BlobIndex);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u quest POI definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadScenarioPOI()
{
    uint32 oldMSTime = getMSTime();

    _scenarioPOIStore.clear();                          // need for reload case

    uint32 count = 0;
    uint32 criteriaTreeIdMax = 0;

    //                                                      0           1      2          3            4         5       6          7                8
    QueryResult result = WorldDatabase.Query("SELECT criteriaTreeId, BlobID, MapID, WorldMapAreaID, `Floor`, Priority, Flags, WorldEffectID, PlayerConditionID FROM scenario_poi order by criteriaTreeId");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 scenario POI definitions. DB table `scenario_poi` is empty.");
        return;
    }

    //                                               0               1   2  3
    QueryResult points = WorldDatabase.Query("SELECT criteriaTreeId, id, x, y FROM scenario_poi_points ORDER BY criteriaTreeId DESC, idx");

    std::vector<std::vector<std::vector<ScenarioPOIPoint> > > POIs;

    if (points)
    {
        // The first result should have the highest questId
        Field* fields = points->Fetch();
        criteriaTreeIdMax = fields[0].GetUInt32();
        POIs.resize(criteriaTreeIdMax + 1);

        do
        {
            fields = points->Fetch();

            uint32 criteriaTreeId     = fields[0].GetUInt32();
            uint32 id                 = fields[1].GetUInt32();
            int32  x                  = fields[2].GetInt32();
            int32  y                  = fields[3].GetInt32();

            if (POIs[criteriaTreeId].size() <= id + 1)
                POIs[criteriaTreeId].resize(id + 10);

            ScenarioPOIPoint point(x, y);
            POIs[criteriaTreeId][id].push_back(point);
        } while (points->NextRow());
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 criteriaTreeId     = fields[0].GetUInt32();
        uint32 BlobID             = fields[1].GetUInt32();
        uint32 MapID              = fields[2].GetUInt32();
        uint32 WorldMapAreaID     = fields[3].GetUInt32();
        uint32 Floor              = fields[4].GetUInt32();
        uint32 Priority           = fields[5].GetUInt32();
        uint32 Flags              = fields[6].GetUInt32();
        uint32 WorldEffectID      = fields[7].GetUInt32();
        uint32 PlayerConditionID  = fields[8].GetUInt32();

        ScenarioPOI POI(BlobID, MapID, WorldMapAreaID, Floor, Priority, Flags, WorldEffectID, PlayerConditionID);

        if(criteriaTreeId <= criteriaTreeIdMax && POIs[criteriaTreeId].size() > 0)
            POI.points = POIs[criteriaTreeId][BlobID];

        _scenarioPOIStore[criteriaTreeId].push_back(POI);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u scenario POI definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadNPCSpellClickSpells()
{
    uint32 oldMSTime = getMSTime();

    _spellClickInfoStore.clear();
    //                                                0          1         2            3
    QueryResult result = WorldDatabase.Query("SELECT npc_entry, spell_id, cast_flags, user_type FROM npc_spellclick_spells");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spellclick spells. DB table `npc_spellclick_spells` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 npc_entry = fields[0].GetUInt32();
        CreatureTemplate const* cInfo = GetCreatureTemplate(npc_entry);
        if (!cInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table npc_spellclick_spells references unknown creature_template %u. Skipping entry.", npc_entry);
            continue;
        }

        uint32 spellid = fields[1].GetUInt32();
        SpellInfo const* spellinfo = sSpellMgr->GetSpellInfo(spellid);
        if (!spellinfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table npc_spellclick_spells references unknown spellid %u. Skipping entry.", spellid);
            continue;
        }

        uint8 userType = fields[3].GetUInt16();
        if (userType >= SPELL_CLICK_USER_MAX)
            sLog->outError(LOG_FILTER_SQL, "Table npc_spellclick_spells references unknown user type %u. Skipping entry.", uint32(userType));

        uint8 castFlags = fields[2].GetUInt8();
        SpellClickInfo info;
        info.spellId = spellid;
        info.castFlags = castFlags;
        info.userType = SpellClickUserTypes(userType);
        _spellClickInfoStore.insert(SpellClickInfoContainer::value_type(npc_entry, info));

        auto data = _creatureTemplateStore.find(npc_entry);
        if (data != _creatureTemplateStore.end())
            data->second.npcflag |= UNIT_NPC_FLAG_SPELLCLICK;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spellclick definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::DeleteCreatureData(ObjectGuid::LowType const& guid)
{
    // remove mapid*cellid -> guid_set map
    CreatureData const* data = GetCreatureData(guid);
    if (data)
        RemoveCreatureFromGrid(guid, data);

    _creatureDataStore.erase(guid);
}

void ObjectMgr::DeleteGOData(ObjectGuid::LowType const& guid)
{
    // remove mapid*cellid -> guid_set map
    GameObjectData const* data = GetGOData(guid);
    if (data)
        RemoveGameobjectFromGrid(guid, data);

    _gameObjectDataStore.erase(guid);
}

void ObjectMgr::AddCorpseCellData(uint32 mapid, uint32 cellid, ObjectGuid player_guid, uint32 instance)
{
    // corpses are always added to spawn mode 0 and they are spawned by their instance id
    CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(mapid, 0)][cellid];
    cell_guids.corpses[player_guid] = instance;
}

void ObjectMgr::DeleteCorpseCellData(uint32 mapid, uint32 cellid, ObjectGuid player_guid)
{
    // corpses are always added to spawn mode 0 and they are spawned by their instance id
    CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(mapid, 0)][cellid];
    cell_guids.corpses.erase(player_guid);
}

void ObjectMgr::LoadQuestRelationsHelper(QuestRelations& map, QuestStarter& _map, std::string const& table, bool starter, bool go)
{
    uint32 oldMSTime = getMSTime();

    map.clear();                                            // need for reload case

    if (starter)
        _map.clear();

    uint32 count = 0;

    QueryResult result = WorldDatabase.PQuery("SELECT id, quest, pool_entry FROM %s qr LEFT JOIN pool_quest pq ON qr.quest = pq.entry", table.c_str());

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest relations from `%s`, table is empty.", table.c_str());

        return;
    }

    PooledQuestRelation* poolRelationMap = go ? &sPoolMgr->mQuestGORelation : &sPoolMgr->mQuestCreatureRelation;
    if (starter)
        poolRelationMap->clear();

    do
    {
        uint32 id     = result->Fetch()[0].GetUInt32();
        uint32 quest  = result->Fetch()[1].GetUInt32();
        uint32 poolId = result->Fetch()[2].GetUInt32();

        if (_questTemplates.find(quest) == _questTemplates.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s`: Quest %u listed for entry %u does not exist.", table.c_str(), quest, id);
            continue;
        }

        if (!poolId || !starter)
            map.insert(QuestRelations::value_type(id, quest));
        else if (starter)
            poolRelationMap->insert(PooledQuestRelation::value_type(quest, id));

        if (starter)
            _map[quest].insert(id);

        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u quest relations from %s in %u ms", count, table.c_str(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameobjectQuestRelations()
{
    LoadQuestRelationsHelper(_goQuestRelations, _goQuestStarter, "gameobject_questrelation", true, true);

    for (QuestRelations::iterator itr = _goQuestRelations.begin(); itr != _goQuestRelations.end(); ++itr)
    {
        GameObjectTemplate const* goInfo = GetGameObjectTemplate(itr->first);
        if (!goInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_questrelation` have data for not existed gameobject entry (%u) and existed quest %u", itr->first, itr->second);
        else if (goInfo->type != GAMEOBJECT_TYPE_QUESTGIVER)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_questrelation` have data gameobject entry (%u) for quest %u, but GO is not GAMEOBJECT_TYPE_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadGameobjectInvolvedRelations()
{
    LoadQuestRelationsHelper(_goQuestInvolvedRelations, _goQuestStarter, "gameobject_involvedrelation", false, true);

    for (QuestRelations::iterator itr = _goQuestInvolvedRelations.begin(); itr != _goQuestInvolvedRelations.end(); ++itr)
    {
        GameObjectTemplate const* goInfo = GetGameObjectTemplate(itr->first);
        if (!goInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_involvedrelation` have data for not existed gameobject entry (%u) and existed quest %u", itr->first, itr->second);
        else if (goInfo->type != GAMEOBJECT_TYPE_QUESTGIVER)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_involvedrelation` have data gameobject entry (%u) for quest %u, but GO is not GAMEOBJECT_TYPE_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadCreatureQuestRelations()
{
    LoadQuestRelationsHelper(_creatureQuestRelations, _creatureQuestStarter, "creature_questrelation", true, false);

    for (QuestRelations::iterator itr = _creatureQuestRelations.begin(); itr != _creatureQuestRelations.end(); ++itr)
    {
        CreatureTemplate const* cInfo = GetCreatureTemplate(itr->first);
        if (!cInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `creature_questrelation` have data for not existed creature entry (%u) and existed quest %u", itr->first, itr->second);
        else if (!(cInfo->npcflag & UNIT_NPC_FLAG_QUESTGIVER))
            sLog->outError(LOG_FILTER_SQL, "Table `creature_questrelation` has creature entry (%u) for quest %u, but npcflag does not include UNIT_NPC_FLAG_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadAreaQuestRelations()
{
    LoadQuestRelationsHelper(_areaQuestRelations, _areaQuestStarter, "area_queststart", false, false);

    for (QuestRelations::iterator itr = _areaQuestRelations.begin(); itr != _areaQuestRelations.end(); ++itr)
        if (!sAreaTableStore.LookupEntry(itr->first))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `area_questrelation` have data for not existed area entry (%u) and existed quest %u", itr->first, itr->second);
            continue;
        }
}

void ObjectMgr::LoadCreatureInvolvedRelations()
{
    LoadQuestRelationsHelper(_creatureQuestInvolvedRelations, _creatureQuestStarter, "creature_involvedrelation", false, false);

    for (QuestRelations::iterator itr = _creatureQuestInvolvedRelations.begin(); itr != _creatureQuestInvolvedRelations.end(); ++itr)
    {
        CreatureTemplate const* cInfo = GetCreatureTemplate(itr->first);
        if (!cInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `creature_involvedrelation` have data for not existed creature entry (%u) and existed quest %u", itr->first, itr->second);
        else if (!(cInfo->npcflag & UNIT_NPC_FLAG_QUESTGIVER))
            sLog->outError(LOG_FILTER_SQL, "Table `creature_involvedrelation` has creature entry (%u) for quest %u, but npcflag does not include UNIT_NPC_FLAG_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadReservedPlayersNames()
{
    uint32 oldMSTime = getMSTime();

    _reservedNamesStore.clear();                                // need for reload case

    QueryResult result = CharacterDatabase.Query("SELECT name FROM reserved_name");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 reserved player names. DB table `reserved_name` is empty!");
        return;
    }

    uint32 count = 0;

    Field* fields;
    do
    {
        fields = result->Fetch();
        std::string name= fields[0].GetString();

        std::wstring wstr;
        if (!Utf8toWStr (name, wstr))
        {
            sLog->outError(LOG_FILTER_GENERAL, "Table `reserved_name` have invalid name: %s", name.c_str());
            continue;
        }

        wstrToLower(wstr);

        _reservedNamesStore.insert(wstr);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u reserved player names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::IsReservedName(const std::string& name) const
{
    std::wstring wstr;
    if (!Utf8toWStr (name, wstr))
        return false;

    wstrToLower(wstr);

    return _reservedNamesStore.find(wstr) != _reservedNamesStore.end();
}

enum LanguageType
{
    LT_BASIC_LATIN    = 0x0000,
    LT_EXTENDEN_LATIN = 0x0001,
    LT_CYRILLIC       = 0x0002,
    LT_EAST_ASIA      = 0x0004,
    LT_ANY            = 0xFFFF
};

static LanguageType GetRealmLanguageType(bool create)
{
    switch (sWorld->getIntConfig(CONFIG_REALM_ZONE))
    {
        case REALM_ZONE_UNKNOWN:                            // any language
        case REALM_ZONE_DEVELOPMENT:
        case REALM_ZONE_TEST_SERVER:
        case REALM_ZONE_QA_SERVER:
            return LT_ANY;
        case REALM_ZONE_UNITED_STATES:                      // extended-Latin
        case REALM_ZONE_OCEANIC:
        case REALM_ZONE_LATIN_AMERICA:
        case REALM_ZONE_ENGLISH:
        case REALM_ZONE_GERMAN:
        case REALM_ZONE_FRENCH:
        case REALM_ZONE_SPANISH:
            return LT_EXTENDEN_LATIN;
        case REALM_ZONE_KOREA:                              // East-Asian
        case REALM_ZONE_TAIWAN:
        case REALM_ZONE_CHINA:
            return LT_EAST_ASIA;
        case REALM_ZONE_RUSSIAN:                            // Cyrillic
            return LT_CYRILLIC;
        default:
            return create ? LT_BASIC_LATIN : LT_ANY;        // basic-Latin at create, any at login
    }
}

bool isValidString(std::wstring wstr, uint32 strictMask, bool numericOrSpace, bool create = false)
{
    if (strictMask == 0)                                       // any language, ignore realm
    {
        if (isExtendedLatinString(wstr, numericOrSpace))
            return true;
        if (isCyrillicString(wstr, numericOrSpace))
            return true;
        if (isEastAsianString(wstr, numericOrSpace))
            return true;
        return false;
    }

    if (strictMask & 0x2)                                    // realm zone specific
    {
        LanguageType lt = GetRealmLanguageType(create);
        if (lt & LT_EXTENDEN_LATIN)
            if (isExtendedLatinString(wstr, numericOrSpace))
                return true;
        if (lt & LT_CYRILLIC)
            if (isCyrillicString(wstr, numericOrSpace))
                return true;
        if (lt & LT_EAST_ASIA)
            if (isEastAsianString(wstr, numericOrSpace))
                return true;
    }

    if (strictMask & 0x1)                                    // basic Latin
    {
        if (isBasicLatinString(wstr, numericOrSpace))
            return true;
    }

    return false;
}

ResponseCodes ObjectMgr::CheckPlayerName(std::string const& name, LocaleConstant locale, bool create /*= false*/)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return CHAR_NAME_INVALID_CHARACTER;

    if (wname.size() > MAX_PLAYER_NAME)
        return CHAR_NAME_TOO_LONG;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_PLAYER_NAME);
    if (wname.size() < minName)
        return CHAR_NAME_TOO_SHORT;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_PLAYER_NAMES);
    if (!isValidString(wname, strictMask, false, create))
        return CHAR_NAME_MIXED_LANGUAGES;

    wstrToLower(wname);
    for (size_t i = 2; i < wname.size(); ++i)
        if (wname[i] == wname[i-1] && wname[i] == wname[i-2])
            return CHAR_NAME_THREE_CONSECUTIVE;

    if (sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE) && !sWordFilterMgr->FindBadWord(name).empty())
        return CHAR_NAME_PROFANE;

    return sDB2Manager.ValidateName(name, locale);
}

bool ObjectMgr::IsValidCharterName(const std::string& name)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    if (wname.size() > MAX_CHARTER_NAME)
        return false;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_CHARTER_NAME);
    if (wname.size() < minName)
        return false;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_CHARTER_NAMES);

    return isValidString(wname, strictMask, true);
}

PetNameInvalidReason ObjectMgr::CheckPetName(const std::string& name)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return PET_NAME_INVALID;

    if (wname.size() > MAX_PET_NAME)
        return PET_NAME_TOO_LONG;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_PET_NAME);
    if (wname.size() < minName)
        return PET_NAME_TOO_SHORT;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_PET_NAMES);
    if (!isValidString(wname, strictMask, false))
        return PET_NAME_MIXED_LANGUAGES;

    if(sWorld->getBoolConfig(CONFIG_WORD_FILTER_ENABLE) && !sWordFilterMgr->FindBadWord(name).empty())
        return PET_NAME_PROFANE;

    return PET_NAME_SUCCESS;
}

void ObjectMgr::LoadGameObjectForQuests()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectForQuestStore.clear();                         // need for reload case

    if (sObjectMgr->GetGameObjectTemplates()->empty())
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 GameObjects for quests");
        return;
    }

    uint32 count = 0;

    // collect GO entries for GO that must activated
    GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
    for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
    {
        switch (itr->second.type)
        {
            // scan GO chest with loot including quest items
            case GAMEOBJECT_TYPE_CHEST:
            {
                uint32 loot_id = (itr->second.entry);

                // find quest loot for GO
                if (itr->second.chest.questID || LootTemplates_Gameobject.HaveQuestLootFor(loot_id))
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    ++count;
                }
                break;
            }
            case GAMEOBJECT_TYPE_GENERIC:
            {
                if (itr->second.generic.questID > 0)            //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            case GAMEOBJECT_TYPE_GOOBER:
            {
                if (itr->second.goober.questID > 0)              //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            case GAMEOBJECT_TYPE_SPELL_FOCUS:
            {
                //
                if (itr->second.spellFocus.questID > 0)              //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            default:
                break;
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u GameObjects for quests in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::LoadTrinityStrings(const char* table, int32 min_value, int32 max_value)
{
    uint32 oldMSTime = getMSTime();

    int32 start_value = min_value;
    int32 end_value   = max_value;
    // some string can have negative indexes range
    if (start_value < 0)
    {
        if (end_value >= start_value)
        {
            sLog->outError(LOG_FILTER_SQL, "Table '%s' attempt loaded with invalid range (%d - %d), strings not loaded.", table, min_value, max_value);
            return false;
        }

        // real range (max+1, min+1) exaple: (-10, -1000) -> -999...-10+1
        std::swap(start_value, end_value);
        ++start_value;
        ++end_value;
    }
    else
    {
        if (start_value >= end_value)
        {
            sLog->outError(LOG_FILTER_SQL, "Table '%s' attempt loaded with invalid range (%d - %d), strings not loaded.", table, min_value, max_value);
            return false;
        }
    }

    // cleanup affected map part for reloading case
    for (TrinityStringLocaleContainer::iterator itr = _trinityStringLocaleStore.begin(); itr != _trinityStringLocaleStore.end();)
    {
        if (itr->first >= start_value && itr->first < end_value)
            _trinityStringLocaleStore.erase(itr++);
        else
            ++itr;
    }

    QueryResult result = WorldDatabase.PQuery("SELECT entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8, content_loc9, content_loc10 FROM %s", table);

    if (!result)
    {
        if (min_value == MIN_TRINITY_STRING_ID)              // error only in case internal strings
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 trinity strings. DB table `%s` is empty. Cannot continue.", table);
        else
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 string templates. DB table `%s` is empty.", table);

        return false;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        int32 entry = fields[0].GetInt32();

        if (entry == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s` contain reserved entry 0, ignored.", table);
            continue;
        }
        else if (entry < start_value || entry >= end_value)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s` contain entry %i out of allowed range (%d - %d), ignored.", table, entry, min_value, max_value);
            continue;
        }

        TrinityStringLocale& data = _trinityStringLocaleStore[entry];

        if (!data.Content.empty())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s` contain data for already loaded entry  %i (from another table?), ignored.", table, entry);
            continue;
        }

        data.Content.resize(1);
        ++count;

        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + 1].GetString(), LocaleConstant(i), data.Content);
    } while (result->NextRow());

    if (min_value == MIN_TRINITY_STRING_ID)
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Trinity strings from table %s in %u ms", count, table, GetMSTimeDiffToNow(oldMSTime));
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u string templates from %s in %u ms", count, table, GetMSTimeDiffToNow(oldMSTime));

    return true;
}

const char *ObjectMgr::GetTrinityString(int32 entry, LocaleConstant locale_idx) const
{
    if (TrinityStringLocale const* msl = GetTrinityStringLocale(entry))
    {
        if (msl->Content.size() > size_t(locale_idx) && !msl->Content[locale_idx].empty())
            return msl->Content[locale_idx].c_str();

        return msl->Content[DEFAULT_LOCALE].c_str();
    }

    if (entry > 0)
        sLog->outError(LOG_FILTER_SQL, "Entry %i not found in `trinity_string` table.", entry);
    else
        sLog->outError(LOG_FILTER_SQL, "Trinity string entry %i not found in DB.", entry);
    return "<error>";
}

void ObjectMgr::LoadFishingBaseSkillLevel()
{
    uint32 oldMSTime = getMSTime();

    _fishingBaseForAreaStore.clear();                            // for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, skill FROM skill_fishing_base_level");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 areas for fishing base skill level. DB table `skill_fishing_base_level` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 entry  = fields[0].GetUInt32();
        int32 skill   = fields[1].GetInt16();

        if (!sAreaTableStore.LookupEntry(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "AreaId %u defined in `skill_fishing_base_level` does not exist", entry);
            continue;
        }

        _fishingBaseForAreaStore[entry] = skill;
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u areas for fishing base skill level in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::CheckDeclinedNames(std::wstring w_ownname, DeclinedName const& names)
{
    // get main part of the name
    std::wstring mainpart = GetMainPartOfName(w_ownname, 0);
    // prepare flags
    bool x = true;
    bool y = true;

    // check declined names
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        std::wstring wname;
        if (!Utf8toWStr(names.name[i], wname))
            return false;

        if (mainpart != GetMainPartOfName(wname, i+1))
            x = false;

        if (w_ownname != wname)
            y = false;
    }
    return (x || y);
}

uint32 ObjectMgr::GetAreaTriggerScriptId(uint32 trigger_id)
{
    AreaTriggerScriptContainer::const_iterator i = _areaTriggerScriptStore.find(trigger_id);
    if (i!= _areaTriggerScriptStore.end())
        return i->second;
    return 0;
}

SpellScriptsBounds ObjectMgr::GetSpellScriptsBounds(uint32 spell_id)
{
    return SpellScriptsBounds(_spellScriptsStore.lower_bound(spell_id), _spellScriptsStore.upper_bound(spell_id));
}

SkillRangeType GetSkillRangeType(SkillRaceClassInfoEntry const* rcEntry)
{
    SkillLineEntry const* skill = sSkillLineStore.LookupEntry(rcEntry->SkillID);
    if (!skill)
        return SKILL_RANGE_NONE;

    if (sObjectMgr->GetSkillTier(rcEntry->SkillTierID))
        return SKILL_RANGE_RANK;

    if (rcEntry->SkillID == SKILL_RUNEFORGING || rcEntry->SkillID == SKILL_RUNEFORGING_2)
        return SKILL_RANGE_MONO;

    switch (skill->CategoryID)
    {
        case SKILL_CATEGORY_ARMOR:
            return SKILL_RANGE_MONO;
        case SKILL_CATEGORY_LANGUAGES:
            return SKILL_RANGE_LANGUAGE;
    }

    return SKILL_RANGE_LEVEL;
}

void ObjectMgr::LoadGameTele()
{
    uint32 oldMSTime = getMSTime();

    _gameTeleStore.clear();                                  // for reload case

    //                                                0       1           2           3           4        5     6
    QueryResult result = WorldDatabase.Query("SELECT id, position_x, position_y, position_z, orientation, map, name FROM game_tele");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 GameTeleports. DB table `game_tele` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 id         = fields[0].GetUInt32();

        GameTele gt;

        gt.position_x     = fields[1].GetFloat();
        gt.position_y     = fields[2].GetFloat();
        gt.position_z     = fields[3].GetFloat();
        gt.orientation    = fields[4].GetFloat();
        gt.mapId          = fields[5].GetUInt16();
        gt.name           = fields[6].GetString();

        if (!MapManager::IsValidMapCoord(gt.mapId, gt.position_x, gt.position_y, gt.position_z, gt.orientation))
        {
            sLog->outError(LOG_FILTER_SQL, "Wrong position for id %u (name: %s) in `game_tele` table, ignoring.", id, gt.name.c_str());
            continue;
        }

        if (!Utf8toWStr(gt.name, gt.wnameLow))
        {
            sLog->outError(LOG_FILTER_SQL, "Wrong UTF8 name for id %u in `game_tele` table, ignoring.", id);
            continue;
        }

        wstrToLower(gt.wnameLow);

        _gameTeleStore[id] = gt;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u GameTeleports in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

GameTele const* ObjectMgr::GetGameTele(const std::string& name) const
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return NULL;

    // converting string that we try to find to lower case
    wstrToLower(wname);

    // Alternative first GameTele what contains wnameLow as substring in case no GameTele location found
    const GameTele* alt = NULL;
    for (GameTeleContainer::const_iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
    {
        if (itr->second.wnameLow == wname)
            return &itr->second;
        else if (alt == NULL && itr->second.wnameLow.find(wname) != std::wstring::npos)
            alt = &itr->second;
    }

    return alt;
}

bool ObjectMgr::AddGameTele(GameTele& tele)
{
    // find max id
    uint32 new_id = 0;
    for (GameTeleContainer::const_iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
        if (itr->first > new_id)
            new_id = itr->first;

    // use next
    ++new_id;

    if (!Utf8toWStr(tele.name, tele.wnameLow))
        return false;

    wstrToLower(tele.wnameLow);

    _gameTeleStore[new_id] = tele;

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GAME_TELE);

    stmt->setUInt32(0, new_id);
    stmt->setFloat(1, tele.position_x);
    stmt->setFloat(2, tele.position_y);
    stmt->setFloat(3, tele.position_z);
    stmt->setFloat(4, tele.orientation);
    stmt->setUInt16(5, uint16(tele.mapId));
    stmt->setString(6, tele.name);

    WorldDatabase.Execute(stmt);

    return true;
}

bool ObjectMgr::DeleteGameTele(const std::string& name)
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    // converting string that we try to find to lower case
    wstrToLower(wname);

    for (GameTeleContainer::iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
    {
        if (itr->second.wnameLow == wname)
        {
            PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAME_TELE);

            stmt->setString(0, itr->second.name);

            WorldDatabase.Execute(stmt);

            _gameTeleStore.erase(itr);
            return true;
        }
    }

    return false;
}

void ObjectMgr::LoadMailLevelRewards()
{
    uint32 oldMSTime = getMSTime();

    _mailLevelRewardStore.clear();                           // for reload case

    //                                                 0        1             2            3
    QueryResult result = WorldDatabase.Query("SELECT level, raceMask, mailTemplateId, senderEntry FROM mail_level_reward");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 level dependent mail rewards. DB table `mail_level_reward` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint8 level           = fields[0].GetUInt8();
        uint32 raceMask       = fields[1].GetUInt32();
        uint32 mailTemplateId = fields[2].GetUInt32();
        uint32 senderEntry    = fields[3].GetUInt32();

        if (level > MAX_LEVEL)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have data for level %u that more supported by client (%u), ignoring.", level, MAX_LEVEL);
            continue;
        }

        if (!(raceMask & RACEMASK_ALL_PLAYABLE))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have raceMask (%u) for level %u that not include any player races, ignoring.", raceMask, level);
            continue;
        }

        if (!sMailTemplateStore.LookupEntry(mailTemplateId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have invalid mailTemplateId (%u) for level %u that invalid not include any player races, ignoring.", mailTemplateId, level);
            continue;
        }

        if (!GetCreatureTemplate(senderEntry))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have not existed sender creature entry (%u) for level %u that invalid not include any player races, ignoring.", senderEntry, level);
            continue;
        }

        _mailLevelRewardStore[level].push_back(MailLevelReward(raceMask, mailTemplateId, senderEntry));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u level dependent mail rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddSpellToTrainer(uint32 entry, uint32 spell, uint32 spellCost, uint32 reqSkill, uint32 reqSkillValue, uint32 reqLevel)
{
    if (entry >= TRINITY_TRAINER_START_REF)
        return;

    CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
    if (!cInfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry for a non-existing creature template (Entry: %u), ignoring", entry);
        return;
    }

    if (!(cInfo->npcflag & UNIT_NPC_FLAG_TRAINER))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry for a creature template (Entry: %u) without trainer flag, ignoring", entry);
        return;
    }

    SpellInfo const* spellinfo = sSpellMgr->GetSpellInfo(spell);
    if (!spellinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry (Entry: %u) for a non-existing spell (Spell: %u), ignoring", entry, spell);
        return;
    }

    if (uint32 learnSpell = sDB2Manager.GetLearnSpell(spell))
    {
        if(SpellInfo const* spellinfoNew = sSpellMgr->GetSpellInfo(learnSpell))
        {
            spell = learnSpell;
            spellinfo = spellinfoNew;

            // Hack. The recipe does not have to be studied by the test spell
            if (spellinfo->Id == 125761)
                spell = 104298;
        }
    }

    if (!SpellMgr::IsSpellValid(spellinfo))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry (Entry: %u) for a broken spell (Spell: %u), ignoring", entry, spell);
        return;
    }

    TrainerSpellData& data = _cacheTrainerSpellStore[entry];

    TrainerSpell& trainerSpell = data.spellList[spell];
    trainerSpell.spell         = spell;
    trainerSpell.spellCost     = spellCost;
    trainerSpell.reqSkill      = reqSkill;
    trainerSpell.reqSkillValue = reqSkillValue;
    trainerSpell.reqLevel      = reqLevel;

    if (!trainerSpell.reqLevel)
        trainerSpell.reqLevel = spellinfo->SpellLevel;

    // calculate learned spell for profession case when stored cast-spell
    trainerSpell.learnedSpell[0] = spell;
    for (uint8 i = 0; i < MAX_TRAINERSPELL_ABILITY_REQS; ++i)
    {
        if (spellinfo->Effects[i].Effect != SPELL_EFFECT_LEARN_SPELL)
            continue;
        if (trainerSpell.learnedSpell[0] == spell)
            trainerSpell.learnedSpell[0] = 0;
        // player must be able to cast spell on himself
        if (spellinfo->Effects[i].TargetA.GetTarget() != 0 && spellinfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_TARGET_ALLY
            && spellinfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_TARGET_ANY && spellinfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_CASTER)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` has spell %u for trainer entry %u with learn effect which has incorrect target type, ignoring learn effect!", spell, entry);
            continue;
        }

        trainerSpell.learnedSpell[i] = spellinfo->Effects[i].TriggerSpell;

        if (trainerSpell.learnedSpell[i])
        {
            SpellInfo const* learnedSpellInfo = sSpellMgr->GetSpellInfo(trainerSpell.learnedSpell[i]);
            if (learnedSpellInfo && learnedSpellInfo->IsProfession())
                data.trainerType = 2;
        }
    }

    return;
}

void ObjectMgr::LoadTrainerSpell()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    _cacheTrainerSpellStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT b.entry, a.spell, a.spellcost, a.reqskill, a.reqskillvalue, a.reqlevel FROM npc_trainer AS a "
                                             "INNER JOIN npc_trainer AS b ON a.entry = -(b.spell) "
                                             "UNION SELECT * FROM npc_trainer WHERE spell > 0");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 Trainers. DB table `npc_trainer` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry         = fields[0].GetUInt32();
        uint32 spell         = fields[1].GetUInt32();
        uint32 spellCost     = fields[2].GetUInt32();
        uint32 reqSkill      = fields[3].GetUInt16();
        uint32 reqSkillValue = fields[4].GetUInt16();
        uint32 reqLevel      = fields[5].GetUInt8();

        AddSpellToTrainer(entry, spell, spellCost, reqSkill, reqSkillValue, reqLevel);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %d Trainers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

int ObjectMgr::LoadReferenceVendor(int32 vendor, int32 item, uint8 type, std::set<uint32> *skip_vendors)
{
    // find all items from the reference vendor
    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_NPC_VENDOR_REF);
    stmt->setUInt32(0, uint32(item));
    stmt->setUInt8(1, type);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (!result)
        return 0;

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 item_id = fields[0].GetInt32();

        // if item is a negative, its a reference
        if (item_id < 0)
            count += LoadReferenceVendor(vendor, -item_id, type, skip_vendors);
        else
        {
            int32  maxcount     = fields[1].GetUInt32();
            uint32 incrtime     = fields[2].GetUInt32();
            uint32 ExtendedCost = fields[3].GetUInt32();
            uint8  type         = fields[4].GetUInt8();

            if (!IsVendorItemValid(vendor, item_id, maxcount, incrtime, ExtendedCost, type, NULL, skip_vendors))
                continue;

            VendorItemData& vList = _cacheVendorItemStore[vendor];

            vList.AddItem(item_id, maxcount, incrtime, ExtendedCost, type);
            ++count;
        }
    } while (result->NextRow());

    return count;
}

void ObjectMgr::LoadVendors()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    for (CacheVendorItemContainer::iterator itr = _cacheVendorItemStore.begin(); itr != _cacheVendorItemStore.end(); ++itr)
        itr->second.Clear();
    _cacheVendorItemStore.clear();

    std::set<uint32> skip_vendors;

    QueryResult result = WorldDatabase.Query("SELECT entry, item, maxcount, incrtime, ExtendedCost, type FROM npc_vendor ORDER BY entry, slot ASC");
    if (!result)
    {

        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 Vendors. DB table `npc_vendor` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry        = fields[0].GetUInt32();
        int32 item_id      = fields[1].GetInt32();

        // if item is a negative, its a reference
        if (item_id < 0)
            count += LoadReferenceVendor(entry, -item_id, 0, &skip_vendors);
        else
        {
            uint32 maxcount     = fields[2].GetUInt32();
            uint32 incrtime     = fields[3].GetUInt32();
            uint32 ExtendedCost = fields[4].GetUInt32();
            uint8  type         = fields[5].GetUInt8();

            if (!IsVendorItemValid(entry, item_id, maxcount, incrtime, ExtendedCost, type, NULL, &skip_vendors))
                continue;

            VendorItemData& vList = _cacheVendorItemStore[entry];

            vList.AddItem(item_id, maxcount, incrtime, ExtendedCost, type);
            ++count;
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %d Vendors in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenu()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenusStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT entry, text_id FROM gossip_menu");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0  gossip_menu entries. DB table `gossip_menu` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        GossipMenus gMenu;

        gMenu.entry             = fields[0].GetUInt32();
        gMenu.text_id           = fields[1].GetUInt32();

        if (!GetNpcText(gMenu.text_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu entry %u are using non-existing text_id %u", gMenu.entry, gMenu.text_id);
            continue;
        }

        _gossipMenusStore.insert(GossipMenusContainer::value_type(gMenu.entry, gMenu));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u gossip_menu entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenuItems()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsStore.clear();

    QueryResult result = WorldDatabase.Query(
        //      0        1   2            3            4          5                   6               7              8          9          10
        "SELECT menu_id, id, option_icon, option_text, option_id, npc_option_npcflag, action_menu_id, action_poi_id, box_coded, box_money, box_text, "
        //11                    12
        "OptionBroadcastTextID, BoxBroadcastTextID FROM gossip_menu_option ORDER BY menu_id, id");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gossip_menu_option entries. DB table `gossip_menu_option` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        GossipMenuItems gMenuItem;
        gMenuItem.MenuId                = fields[0].GetUInt32();
        gMenuItem.OptionIndex           = fields[1].GetUInt16();
        gMenuItem.OptionIcon            = fields[2].GetUInt32();
        gMenuItem.OptionText            = fields[3].GetString();
        gMenuItem.OptionType            = fields[4].GetUInt8();
        gMenuItem.OptionNpcflag         = fields[5].GetUInt32();
        gMenuItem.ActionMenuId          = fields[6].GetUInt32();
        gMenuItem.ActionPoiId           = fields[7].GetUInt32();
        gMenuItem.BoxCoded              = fields[8].GetBool();
        gMenuItem.BoxMoney              = fields[9].GetUInt32();
        gMenuItem.BoxText               = fields[10].GetString();
        gMenuItem.OptionBroadcastTextID = fields[11].GetUInt32();
        gMenuItem.BoxBroadcastTextID    = fields[12].GetUInt32();

        if (gMenuItem.OptionIcon >= GOSSIP_ICON_MAX)
        {
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu_option for menu %u, id %u has unknown icon id %u. Replacing with GOSSIP_ICON_CHAT", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionIcon);
            gMenuItem.OptionIcon = GOSSIP_ICON_CHAT;
        }

        if (gMenuItem.OptionType >= GOSSIP_OPTION_MAX)
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu_option for menu %u, id %u has unknown option id %u. Option will not be used", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionType);

        if (gMenuItem.ActionPoiId && !GetPointOfInterest(gMenuItem.ActionPoiId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu_option for menu %u, id %u use non-existing action_poi_id %u, ignoring", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.ActionPoiId);
            gMenuItem.ActionPoiId = 0;
        }

        if (gMenuItem.OptionBroadcastTextID)
            if (!sBroadcastTextStore.LookupEntry(gMenuItem.OptionBroadcastTextID))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `gossip_menu_option` for menu %u, id %u has non-existing or incompatible OptionBroadcastTextId %u, ignoring.", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionBroadcastTextID);
                gMenuItem.OptionBroadcastTextID = 0;
            }

        if (gMenuItem.BoxBroadcastTextID)
            if (!sBroadcastTextStore.LookupEntry(gMenuItem.BoxBroadcastTextID))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `gossip_menu_option` for menu %u, id %u has non-existing or incompatible BoxBroadcastTextID %u, ignoring.", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.BoxBroadcastTextID);
                gMenuItem.BoxBroadcastTextID = 0;
            }

        _gossipMenuItemsStore.insert(GossipMenuItemsContainer::value_type(gMenuItem.MenuId, gMenuItem));
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u gossip_menu_option entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddVendorItem(uint32 entry, uint32 item, int32 maxcount, uint32 incrtime, uint32 extendedCost, uint8 type, bool persist /*= true*/)
{
    VendorItemData& vList = _cacheVendorItemStore[entry];
    vList.AddItem(item, maxcount, incrtime, extendedCost, type);

    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_NPC_VENDOR);

        stmt->setUInt32(0, entry);
        stmt->setUInt32(1, item);
        stmt->setUInt8(2, maxcount);
        stmt->setUInt32(3, incrtime);
        stmt->setUInt32(4, extendedCost);
        stmt->setUInt8(5, type);

        WorldDatabase.Execute(stmt);
    }
}

bool ObjectMgr::RemoveVendorItem(uint32 entry, uint32 item, uint8 type, bool persist /*= true*/)
{
    CacheVendorItemContainer::iterator  iter = _cacheVendorItemStore.find(entry);
    if (iter == _cacheVendorItemStore.end())
        return false;

    if (!iter->second.RemoveItem(item, type))
        return false;

    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_NPC_VENDOR);

        stmt->setUInt32(0, entry);
        stmt->setUInt32(1, item);
        stmt->setUInt8(2, type);

        WorldDatabase.Execute(stmt);
    }

    return true;
}

bool ObjectMgr::IsVendorItemValid(uint32 vendor_entry, uint32 id, int32 maxcount, uint32 incrtime, uint32 ExtendedCost, uint8 type, Player* player, std::set<uint32>* skip_vendors, uint32 ORnpcflag) const
{
    CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(vendor_entry);
    if (!cInfo)
    {
        if (player)
            ChatHandler(player).SendSysMessage(LANG_COMMAND_VENDORSELECTION);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have data for not existed creature template (Entry: %u), ignore", vendor_entry);
        return false;
    }

    if (!((cInfo->npcflag | ORnpcflag) & UNIT_NPC_FLAG_VENDOR))
    {
        if (!skip_vendors || skip_vendors->count(vendor_entry) == 0)
        {
            if (player)
                ChatHandler(player).SendSysMessage(LANG_COMMAND_VENDORSELECTION);
            else
                sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have data for not creature template (Entry: %u) without vendor flag, ignore", vendor_entry);

            if (skip_vendors)
                skip_vendors->insert(vendor_entry);
        }
        return false;
    }

    if ((type == ITEM_VENDOR_TYPE_ITEM && !sObjectMgr->GetItemTemplate(id)) ||
        (type == ITEM_VENDOR_TYPE_CURRENCY && !sCurrencyTypesStore.LookupEntry(id)))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_ITEM_NOT_FOUND, id, type);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` for Vendor (Entry: %u) have in item list non-existed item (%u, type %u), ignore", vendor_entry, id, type);
        return false;
    }

    if (ExtendedCost && !sItemExtendedCostStore.LookupEntry(ExtendedCost))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_EXTENDED_COST_NOT_EXIST, ExtendedCost);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have Item (Entry: %u) with wrong ExtendedCost (%u) for vendor (%u), ignore", id, ExtendedCost, vendor_entry);
        return false;
    }

    if (type == ITEM_VENDOR_TYPE_ITEM) // not applicable to currencies
    {
        if (maxcount > 0 && incrtime == 0)
        {
            if (player)
                ChatHandler(player).PSendSysMessage("MaxCount != 0 (%u) but IncrTime == 0", maxcount);
            else
                sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` has `maxcount` (%u) for item %u of vendor (Entry: %u) but `incrtime`=0, ignore", maxcount, id, vendor_entry);
            return false;
        }
        else if (maxcount == 0 && incrtime > 0)
        {
            if (player)
                ChatHandler(player).PSendSysMessage("MaxCount == 0 but IncrTime<>= 0");
            else
                sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` has `maxcount`=0 for item %u of vendor (Entry: %u) but `incrtime`<>0, ignore", id, vendor_entry);
            return false;
        }
    }

    VendorItemData const* vItems = GetNpcVendorItemList(vendor_entry);
    if (!vItems)
        return true;                                        // later checks for non-empty lists

    if (vItems->FindItemCostPair(id, ExtendedCost, type))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_ITEM_ALREADY_IN_LIST, id, ExtendedCost, type);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `npc_vendor` has duplicate items %u (with extended cost %u, type %u) for vendor (Entry: %u), ignoring", id, ExtendedCost, type, vendor_entry);
        return false;
    }

    return true;
}

void ObjectMgr::LoadScriptNames()
{
    uint32 oldMSTime = getMSTime();

    _scriptNamesStore.push_back("");
    QueryResult result = WorldDatabase.Query(
      "SELECT DISTINCT(ScriptName) FROM achievement_criteria_data WHERE ScriptName <> '' AND type = 11 "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM achievement_reward WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM battleground_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM creature_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM gameobject_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM item_script_names WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM areatrigger_scripts WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM spell_script_names WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM transports WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM game_weather WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM conditions WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM outdoorpvp_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM spell_scene WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(script) FROM instance_template WHERE script <> ''");

    if (!result)
    {

        sLog->outError(LOG_FILTER_SQL, ">> Loaded empty set of Script Names!");
        return;
    }

    uint32 count = 1;

    do
    {
        _scriptNamesStore.push_back((*result)[0].GetString());
        ++count;
    }
    while (result->NextRow());

    std::sort(_scriptNamesStore.begin(), _scriptNamesStore.end());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %d Script Names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetScriptId(const char *name)
{
    // use binary search to find the script name in the sorted vector
    // assume "" is the first element
    if (!name)
        return 0;

    ScriptNameContainer::const_iterator itr = std::lower_bound(_scriptNamesStore.begin(), _scriptNamesStore.end(), name);
    if (itr == _scriptNamesStore.end() || *itr != name)
        return 0;

    return uint32(itr - _scriptNamesStore.begin());
}

void ObjectMgr::CheckScripts(ScriptsType type, std::set<int32>& ids)
{
    ScriptMapMap* scripts = GetScriptsMapByType(type);
    if (!scripts)
        return;

    for (ScriptMapMap::const_iterator itrMM = scripts->begin(); itrMM != scripts->end(); ++itrMM)
    {
        for (ScriptMap::const_iterator itrM = itrMM->second.begin(); itrM != itrMM->second.end(); ++itrM)
        {
            switch (itrM->second.command)
            {
                case SCRIPT_COMMAND_TALK:
                {
                    if (!GetTrinityStringLocale (itrM->second.Talk.TextID))
                        sLog->outError(LOG_FILTER_SQL, "Table `%s` references invalid text id %u from `db_script_string`, script id: %u.", GetScriptsTableNameByType(type).c_str(), itrM->second.Talk.TextID, itrMM->first);

                    if (ids.find(itrM->second.Talk.TextID) != ids.end())
                        ids.erase(itrM->second.Talk.TextID);
                }
                default:
                    break;
            }
        }
    }
}

void ObjectMgr::LoadDbScriptStrings()
{
    LoadTrinityStrings("db_script_string", MIN_DB_SCRIPT_STRING_ID, MAX_DB_SCRIPT_STRING_ID);

    std::set<int32> ids;

    for (int32 i = MIN_DB_SCRIPT_STRING_ID; i < MAX_DB_SCRIPT_STRING_ID; ++i)
        if (GetTrinityStringLocale(i))
            ids.insert(i);

    for (int type = SCRIPTS_FIRST; type < SCRIPTS_LAST; ++type)
        CheckScripts(ScriptsType(type), ids);

    for (std::set<int32>::const_iterator itr = ids.begin(); itr != ids.end(); ++itr)
        sLog->outError(LOG_FILTER_SQL, "Table `db_script_string` has unused string id  %u", *itr);
}

bool LoadTrinityStrings(const char* table, int32 start_value, int32 end_value)
{
    // MAX_DB_SCRIPT_STRING_ID is max allowed negative value for scripts (scrpts can use only more deep negative values
    // start/end reversed for negative values
    if (start_value > MAX_DB_SCRIPT_STRING_ID || end_value >= start_value)
    {
        sLog->outError(LOG_FILTER_SQL, "Table '%s' load attempted with range (%d - %d) reserved by Trinity, strings not loaded.", table, start_value, end_value+1);
        return false;
    }

    return sObjectMgr->LoadTrinityStrings(table, start_value, end_value);
}

CreatureBaseStats const* ObjectMgr::GetCreatureBaseStats(uint8 level, uint8 unitClass)
{
    CreatureBaseStatsContainer::const_iterator it = _creatureBaseStatsStore.find(MAKE_PAIR16(level, unitClass));

    if (it != _creatureBaseStatsStore.end())
        return &(it->second);

    struct DefaultCreatureBaseStats : public CreatureBaseStats
    {
        DefaultCreatureBaseStats()
        {
            BaseArmor = 1;
            AttackPower = 1;
            RangedAttackPower = 1;
            for (uint8 j = 0; j < MAX_EXPANSIONS; ++j)
            {
                BaseHealth[j] = 1;
                BaseDamage[j] = 1;
            }
            BaseMana = 0;
        }
    };
    static const DefaultCreatureBaseStats def_stats;
    return &def_stats;
}

void ObjectMgr::LoadCreatureClassLevelStats()
{
    uint32 oldMSTime = getMSTime();
    //                                                  0     1       2        3            4
    QueryResult result = WorldDatabase.Query("SELECT level, class, basemana, attackpower, rangedattackpower FROM creature_classlevelstats");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature base stats. DB table `creature_classlevelstats` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        uint8 Level = fields[0].GetUInt8();
        uint8 Class = fields[1].GetUInt8();

        GameTable<GameTableEntry>* hpTables[] = { &sGtNpcTotalHpStore, &sGtNpcTotalHpExp1Store, &sGtNpcTotalHpExp2Store, &sGtNpcTotalHpExp3Store, &sGtNpcTotalHpExp4Store, &sGtNpcTotalHpExp5Store, &sGtNpcTotalHpExp6Store, nullptr };
        GameTable<GameTableEntry>* dmgTables[] = { &sGtNpcDamageByClassStore, &sGtNpcDamageByClassStoreExp1, &sGtNpcDamageByClassStoreExp2, &sGtNpcDamageByClassStoreExp3, &sGtNpcDamageByClassStoreExp4, &sGtNpcDamageByClassStoreExp5, &sGtNpcDamageByClassStoreExp6, nullptr };
        GameTableEntry const* armor = sGtArmorMitigationByLvlStore.EvaluateTable(Level - 1);

        CreatureBaseStats stats;
        stats.BaseMana = fields[2].GetUInt32();
        stats.BaseArmor = armor ? uint32(armor->Value) : 1; //@TODO:Legion - recheck
        stats.AttackPower = fields[3].GetUInt16();
        stats.RangedAttackPower = fields[4].GetUInt16();

        if (!Class || ((1 << (Class - 1)) & CLASSMASK_ALL_CREATURES) == 0)
            sLog->outError(LOG_FILTER_SQL, "Creature base stats for level %u has invalid class %u", Level, Class);

        for (uint8 i = 0; hpTables[i]; ++i)
            if (hpTables[i]->HasEntry(Level - 1, Class - 1))
                stats.BaseHealth[i] = hpTables[i]->EvaluateTable(Level - 1, Class - 1)->Value;

        for (uint8 i = 0; dmgTables[i]; ++i)
            if (dmgTables[i]->HasEntry(Level - 1, Class - 1))
                stats.BaseDamage[i] = dmgTables[i]->EvaluateTable(Level - 1, Class - 1)->Value;

        _creatureBaseStatsStore[MAKE_PAIR16(Level, Class)] = stats;

        ++count;
    }
    while (result->NextRow());

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
        for (uint16 lvl = itr->second.minlevel; lvl <= itr->second.maxlevel; ++lvl)
            if (_creatureBaseStatsStore.find(MAKE_PAIR16(lvl, itr->second.unit_class)) == _creatureBaseStatsStore.end())
                sLog->outError(LOG_FILTER_SQL, "Missing base stats for creature class %u level %u", itr->second.unit_class, lvl);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature base stats in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeAchievements()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_achievement");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change achievement pairs. DB table `player_factionchange_achievement` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sAchievementStore.LookupEntry(alliance))
            sLog->outError(LOG_FILTER_SQL, "Achievement %u referenced in `player_factionchange_achievement` does not exist, pair skipped!", alliance);
        else if (!sAchievementStore.LookupEntry(horde))
            sLog->outError(LOG_FILTER_SQL, "Achievement %u referenced in `player_factionchange_achievement` does not exist, pair skipped!", horde);
        else
            FactionChange_Achievements[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change achievement pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeItems()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_items");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change item pairs. DB table `player_factionchange_items` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!GetItemTemplate(alliance))
            sLog->outError(LOG_FILTER_SQL, "Item %u referenced in `player_factionchange_items` does not exist, pair skipped!", alliance);
        else if (!GetItemTemplate(horde))
            sLog->outError(LOG_FILTER_SQL, "Item %u referenced in `player_factionchange_items` does not exist, pair skipped!", horde);
        else
            FactionChange_Items[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change item pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeSpells()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_spells");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change spell pairs. DB table `player_factionchange_spells` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sSpellMgr->GetSpellInfo(alliance))
            sLog->outError(LOG_FILTER_SQL, "Spell %u referenced in `player_factionchange_spells` does not exist, pair skipped!", alliance);
        else if (!sSpellMgr->GetSpellInfo(horde))
            sLog->outError(LOG_FILTER_SQL, "Spell %u referenced in `player_factionchange_spells` does not exist, pair skipped!", horde);
        else
            FactionChange_Spells[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change spell pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeReputations()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_reputations");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change reputation pairs. DB table `player_factionchange_reputations` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sFactionStore.LookupEntry(alliance))
            sLog->outError(LOG_FILTER_SQL, "Reputation %u referenced in `player_factionchange_reputations` does not exist, pair skipped!", alliance);
        else if (!sFactionStore.LookupEntry(horde))
            sLog->outError(LOG_FILTER_SQL, "Reputation %u referenced in `player_factionchange_reputations` does not exist, pair skipped!", horde);
        else
            FactionChange_Reputation[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change reputation pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeTitles()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_titles");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change title pairs. DB table `player_factionchange_title` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sCharTitlesStore.LookupEntry(alliance))
            sLog->outError(LOG_FILTER_SQL, "Title %u referenced in `player_factionchange_title` does not exist, pair skipped!", alliance);
        else if (!sCharTitlesStore.LookupEntry(horde))
            sLog->outError(LOG_FILTER_SQL, "Title %u referenced in `player_factionchange_title` does not exist, pair skipped!", horde);
        else
            FactionChange_Titles[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change title pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPhaseDefinitions()
{
    _PhaseDefinitionStore.clear();

    uint32 oldMSTime = getMSTime();

    //                                                 0       1       2         3            4           5       6
    QueryResult result = WorldDatabase.Query("SELECT zoneId, entry, phasemask, phaseId, PreloadMapID, VisibleMapID, flags FROM `phase_definitions` ORDER BY `entry` ASC");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 phasing definitions. DB table `phase_definitions` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field *fields = result->Fetch();

        PhaseDefinition pd;

        pd.zoneId                = fields[0].GetUInt32();
        pd.entry                 = fields[1].GetUInt16();
        pd.phasemask             = fields[2].GetUInt64();
        pd.terrainswapmap        = fields[4].GetUInt16();
        pd.wmAreaId              = fields[5].GetUInt16();
        pd.flags                 = fields[6].GetUInt8();

        Tokenizer phasesToken(fields[3].GetString(), ' ', 100);
        for (Tokenizer::const_iterator::value_type itr : phasesToken)
            if (PhaseEntry const* phase = sPhaseStores.LookupEntry(uint32(strtoull(itr, nullptr, 10))))
                pd.phaseId.push_back(phase->ID);

        // Checks
        if ((pd.flags & PHASE_FLAG_OVERWRITE_EXISTING) && (pd.flags & PHASE_FLAG_NEGATE_PHASE))
        {
            sLog->outError(LOG_FILTER_SQL, "Flags defined in phase_definitions in zoneId %d and entry %u does contain PHASE_FLAG_OVERWRITE_EXISTING and PHASE_FLAG_NEGATE_PHASE. Setting flags to PHASE_FLAG_OVERWRITE_EXISTING", pd.zoneId, pd.entry);
            pd.flags &= ~PHASE_FLAG_NEGATE_PHASE;
        }

        if (pd.terrainswapmap > 0)
        {
            const MapEntry* const map = sMapStore.LookupEntry(pd.terrainswapmap);
            if (!map)
            {
                sLog->outError(LOG_FILTER_SQL, "DB table `phase_definitions` has not existen terrainswapmap %u", pd.terrainswapmap);
                continue;
            }
        }

        _PhaseDefinitionStore[pd.zoneId].push_back(pd);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u phasing definitions in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadSpellPhaseInfo()
{
    _SpellPhaseStore.clear();

    uint32 oldMSTime = getMSTime();

    //                                               0       1            2            3
    QueryResult result = WorldDatabase.Query("SELECT id, phasemask, terrainswapmap, phaseId FROM `spell_phase`");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell dbc infos. DB table `spell_phase` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field *fields = result->Fetch();

        SpellPhaseInfo spellPhaseInfo;
        spellPhaseInfo.spellId                = fields[0].GetUInt32();

        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellPhaseInfo.spellId);
        if (!spell)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u defined in `spell_phase` does not exists, skipped.", spellPhaseInfo.spellId);
            continue;
        }

        spellPhaseInfo.phasemask              = fields[1].GetUInt32();
        spellPhaseInfo.terrainswapmap         = fields[2].GetUInt32();

        _SpellPhaseStore[spellPhaseInfo.spellId] = spellPhaseInfo;

        ++count;
    }
    while (result->NextRow());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell dbc infos in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}


GameObjectTemplate const* ObjectMgr::GetGameObjectTemplate(uint32 entry)
{
    GameObjectTemplateContainer::const_iterator itr = _gameObjectTemplateStore.find(entry);
    if (itr != _gameObjectTemplateStore.end())
        return &(itr->second);

    return NULL;
}

const std::vector<CreatureDifficultyStat>* ObjectMgr::GetDifficultyStat(uint32 entry) const
{
    CreatureDifficultyStatContainer::const_iterator itr = _creatureDifficultyStatStore.find(entry);
    return itr != _creatureDifficultyStatStore.end() ? &(itr->second) : NULL;
}

CreatureDifficultyStat const* ObjectMgr::GetCreatureDifficultyStat(uint32 entry, uint8 diff) const
{
    uint8 statId = 0;
    switch (diff)
    {
        case DIFFICULTY_HEROIC:
        case DIFFICULTY_10_N:
        case DIFFICULTY_CHALLENGE:
        case DIFFICULTY_40:
        case DIFFICULTY_HC_SCENARIO:
        case DIFFICULTY_N_SCENARIO:
        case DIFFICULTY_NORMAL_RAID:
            statId = 0;
            break;
        case DIFFICULTY_NORMAL:
        case DIFFICULTY_25_N:
        case DIFFICULTY_HEROIC_RAID:
            statId = 1;
            break;
        case DIFFICULTY_10_HC:
        case DIFFICULTY_MYTHIC_RAID:
        case DIFFICULTY_MYTHIC_DUNGEON:
            statId = 2;
            break;
        case DIFFICULTY_25_HC:
        case DIFFICULTY_LFR_NEW:
            statId = 3;
            break;
        case DIFFICULTY_LFR:
            statId = 4;
            break;
    }

    if (std::vector<CreatureDifficultyStat> const* diffStat = GetDifficultyStat(entry))
        for (std::vector<CreatureDifficultyStat>::const_iterator itr = diffStat->begin(); itr != diffStat->end(); ++itr)
            if(itr->Difficulty == statId)
                return &(*itr);

    return NULL;
}

CreatureTemplate const* ObjectMgr::GetCreatureTemplate(uint32 entry)
{
    CreatureTemplateContainer::const_iterator itr = _creatureTemplateStore.find(entry);
    if (itr != _creatureTemplateStore.end())
        return &(itr->second);

    return NULL;
}

VehicleAccessoryList const* ObjectMgr::GetVehicleAccessoryList(Vehicle* veh) const
{
    if (Creature* cre = veh->GetBase()->ToCreature())
    {
        // Give preference to GUID-based accessories
        VehicleAccessoryContainer::const_iterator itr = _vehicleAccessoryStore.find(cre->GetDBTableGUIDLow());
        if (itr != _vehicleAccessoryStore.end())
            return &itr->second;
    }

    // Otherwise return entry-based
    VehicleAccessoryTemplateContainer::const_iterator itr = _vehicleTemplateAccessoryStore.find(veh->GetCreatureEntry());
    if (itr != _vehicleTemplateAccessoryStore.end())
        return &itr->second;

    // For Player witch come vehicle by 296 aura
    if (veh->GetRecAura())
    {
        VehicleAccessoryTemplateContainer::const_iterator itr = _vehicleTemplateAccessoryStore.find(veh->GetRecAura() * -1);
        if (itr != _vehicleTemplateAccessoryStore.end())
            return &itr->second;
    }
    
    return NULL;
}

void ObjectMgr::RestructCreatureGUID()
{
    QueryResult result = WorldDatabase.Query("SELECT guid FROM creature ORDER BY guid;");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "RestructCreatureGUID failed");
        return;
    }

    uint32 newGUID = 1;
    do
    {
        Field *fields = result->Fetch();
        uint32 oldGUID = fields[0].GetUInt32();

        SQLTransaction worldTrans = WorldDatabase.BeginTransaction();

        if (oldGUID == newGUID)
        {
            newGUID++;
            continue;
        }
        // World Database
        worldTrans->PAppend("UPDATE creature SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE creature_addon SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE creature_addon SET path_id = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE waypoint_data SET id = %u WHERE id = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE creature_formations SET leaderGUID = %u WHERE leaderGUID = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE creature_formations SET memberGUID = %u WHERE memberGUID = %u;", newGUID, oldGUID);
        //worldTrans->PAppend("UPDATE creature_transport SET guid = %u WHERE guid = %u;", newGUID, oldGUID); not the same guid it has.
        worldTrans->PAppend("UPDATE game_event_creature SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE pool_creature SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE smart_scripts SET target_param1 = %u WHERE `target_type` = 10 AND target_param1 = %u;", newGUID, oldGUID);

        WorldDatabase.CommitTransaction(worldTrans);
        WorldDatabase.WaitExecution();
        newGUID++;
    } while (result->NextRow());

    WorldDatabase.PExecute("ALTER TABLE creature AUTO_INCREMENT = %u;", newGUID + 1);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "RestructCreatureGUID: Last guid %u", newGUID);
}

void ObjectMgr::RestructGameObjectGUID()
{
    QueryResult result = WorldDatabase.Query("SELECT guid FROM gameobject ORDER BY guid;");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "RestructGameObjectGUID failed");
        return;
    }

    uint32 newGUID = 1;
    do
    {
        Field *fields = result->Fetch();
        uint32 oldGUID = fields[0].GetUInt32();

        if (oldGUID == newGUID)
        {
            newGUID++;
            continue;
        }

        SQLTransaction worldTrans = WorldDatabase.BeginTransaction();
        // World Database
        worldTrans->PAppend("UPDATE gameobject SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE game_event_gameobject SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE pool_gameobject SET guid = %u WHERE guid = %u;", newGUID, oldGUID);
        worldTrans->PAppend("UPDATE smart_scripts SET target_param1 = %u WHERE `target_type` = 14 AND target_param1 = %u;", newGUID, oldGUID);

        WorldDatabase.CommitTransaction(worldTrans);
        WorldDatabase.WaitExecution();

        newGUID++;
    } while (result->NextRow());

    WorldDatabase.PExecute("ALTER TABLE gameobject AUTO_INCREMENT = %u;", newGUID + 1);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "RestructGameObjectGUID: Last guid %u", newGUID);
}

void ObjectMgr::LoadResearchSiteToZoneData()
{
    QueryResult result = WorldDatabase.Query("SELECT site_id, zone_id, branch_id FROM archaeology_zones");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 archaeology zones. DB table `archaeology_zones` is empty.");
        return;
    }

    uint32 counter = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 site_id = fields[0].GetUInt32();
        uint32 zone_id = fields[1].GetUInt32();
        uint32 branch_id = fields[2].GetUInt32();

        auto itr = sDB2Manager._researchSiteDataMap.find(site_id);
        if (itr == sDB2Manager._researchSiteDataMap.end())
        {
            sLog->outError(LOG_FILTER_SQL, "DB table `archaeology_zones` has data for nonexistant site id %u", site_id);
            continue;
        }

        ResearchSiteData& data = itr->second;
        data.zone = zone_id;
        data.branch_id = branch_id;

        for (AreaTableEntry const* area : sAreaTableStore)
            if (area->ParentAreaID == zone_id)
            {
                data.level = area->ExplorationLevel;
                break;
            }

        ++counter;
    }
    while (result->NextRow());

    // recheck all research sites
    for (auto const& itr : sDB2Manager._researchSiteDataMap)
    {
        if (itr.second.zone == 0 || itr.second.level == 0xFF || itr.second.branch_id == 0)
            sLog->outError(LOG_FILTER_SQL, "DB table `archaeology_zones` has not full or does not have data for site id %u: "
            "zone %u level %u branch_id %u",
            itr.second.entry->areaName, itr.second.zone, itr.second.level, itr.second.branch_id);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u archaeology zones.", counter);
}

void ObjectMgr::LoadDigSitePositions()
{
    QueryResult result = WorldDatabase.Query("SELECT idx, map, x, y FROM archaeology_digsites");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 dig site positions. DB table `archaeology_digsites` is empty.");
        return;
    }

    uint32 counter = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 idx = fields[0].GetUInt32();
        uint32 map = fields[1].GetUInt32();
        float x = fields[2].GetFloat();
        float y = fields[3].GetFloat();

        bool added = false;
        for (auto itr : sDB2Manager._researchSiteDataMap)
        {
            ResearchSiteData& data = itr.second;

            if (data.entry->MapID != map)
                continue;

            ResearchPOIPoint p;
            p.x = int32(x);
            p.y = int32(y);

            if (Player::IsPointInZone(p, data.points))
            {
                data.digSites.push_back(DigSitePosition(x, y));
                added = true;
            }
        }

        if (!added)
        {
            sLog->outError(LOG_FILTER_SQL, "DB table `archaeology_digsites` has data for point idx:%u x:%f y:%f at map %u that does not belong to any digsite!",
                idx, x, y, map);
            continue;
        }

        ++counter;
    }
    while (result->NextRow());

    std::set<uint32> toRemove;
    for (auto itr : sDB2Manager._researchSiteDataMap)
    {
        ResearchSiteData& data = itr.second;

        uint32 siteCount = data.digSites.size();
        if (siteCount < MAX_DIGSITE_FINDS)
        {
            sLog->outError(LOG_FILTER_SQL, "Archaeology research site %u has less that %u dig site positions! (%u)", data.entry->ID, MAX_DIGSITE_FINDS, siteCount);
            if (!siteCount)
                toRemove.insert(itr.first);
            else
            {
                while (data.digSites.size() < MAX_DIGSITE_FINDS)
                    data.digSites.push_back(data.digSites[urand(0, data.digSites.size() - 1)]);
            }
        }
    }

    for (std::set<uint32>::const_iterator itr = toRemove.begin(); itr != toRemove.end(); ++itr)
        sDB2Manager._researchSiteDataMap.erase(*itr);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u dig site positions.", counter);
}

void ObjectMgr::LoadBannedAddons()
{
    QueryResult result = CharacterDatabase.Query("SELECT Id, Name, Version, UNIX_TIMESTAMP(Timestamp) FROM banned_addons");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 banned addons. DB table `banned_addons` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 dbcMaxBannedAddon = sBannedAddOnsStore.GetNumRows();

    do
    {
        Field* fields = result->Fetch();

        std::string name = fields[1].GetString();
        std::string version = fields[2].GetString();
        uint32 timestamp = uint32(fields[3].GetUInt64());

        BannedAddon ba;
        ba.Id = fields[0].GetUInt32() + dbcMaxBannedAddon;
        MD5(reinterpret_cast<uint8 const*>(name.c_str()), name.length(), ba.MD5_name);
        MD5(reinterpret_cast<uint8 const*>(version.c_str()), version.length(), ba.MD5_version);
        ba.timestamp = timestamp;

        sBannedAddonDataMap.push_back(ba);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u banned addons.", count);
}
// Battle Pet System
void ObjectMgr::LoadBattlePetXPForLevel()
{
    // Loading xp per level data
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battle Pet XP Data...");
    
    _battlePetXPperLevel.resize(25);
    
    // clear container
    for (uint8 level = 0; level < 25; ++level)
        _battlePetXPperLevel[level] = 0;

    QueryResult result  = WorldDatabase.Query("SELECT lvl, xp_for_next_level FROM battle_pet_xp_for_level");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 xp for battle pet level definitions. DB table `battle_pet_xp_for_level` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 cur_level = fields[0].GetUInt8();
        uint32 cur_xp    = fields[1].GetUInt32();

        _battlePetXPperLevel[cur_level] = cur_xp;
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battle pet xp for level definitions.", count);
}

void ObjectMgr::LoadBattlePetBreedsToSpecies()
{
    // Loading xp per level data
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battle Pet Breeds For Species Data...");

    // clear container
    _battlePetPossibleBreedsToSpecies.clear();

    QueryResult result = WorldDatabase.Query("SELECT speciesID, possibleBreedMask FROM battle_pet_breed2species");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 possible breeds for species definitions. DB table `battle_pet_breed2species` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 speciesID = fields[0].GetUInt32();
        uint32 possibleBreedMask = fields[1].GetUInt32();

        std::vector<uint32> breeds;

        // fill breed data
        for (uint8 i = 3; i < 13; ++i)
        {
            if ((possibleBreedMask & (1 << i)) != 0)
                breeds.push_back(i);
        }

        _battlePetPossibleBreedsToSpecies[speciesID] = breeds;
        ++count;

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u possible breeds for species definitions.", count);
}

const std::vector<uint32>* ObjectMgr::GetPossibleBreedsForSpecies(uint32 speciesID) const
{
    BattlePetPossibleBreedsToSpecies::const_iterator itr = _battlePetPossibleBreedsToSpecies.find(speciesID);
    return itr != _battlePetPossibleBreedsToSpecies.end() ? &(itr->second) : NULL;
}

void ObjectMgr::LoadBattlePetNpcTeamMember()
{
    // Loading xp per level data
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battle Pet Npc Team Members Data...");

    // clear container
    _battlePetNpcTeamMembers.clear();

    QueryResult result = WorldDatabase.Query("SELECT npcEntry, slot0, slot1, slot2 FROM battle_pet_npc_team_member");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 possible npc team members definitions. DB table `battle_pet_npc_team_member` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 npcEntry = fields[0].GetUInt32();
        std::vector<uint32> slots;

        for (uint8 i = 0; i < MAX_ACTIVE_BATTLE_PETS; ++i)
        {
            uint32 slot = fields[i+1].GetUInt32();
            slots.push_back(slot);
        }

        _battlePetNpcTeamMembers[npcEntry] = slots;
        ++count;

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u possible npc team members definitions.", count);
}

const std::vector<uint32>* ObjectMgr::GetBattlePetTeamMembers(uint32 creatureEntry) const
{
    BattlePetNpcTeamMembers::const_iterator itr = _battlePetNpcTeamMembers.find(creatureEntry);
    return itr != _battlePetNpcTeamMembers.end() ? &(itr->second) : NULL;
}

void ObjectMgr::LoadAreaTriggerActionsAndData()
{
    _areaTriggerData.clear();

    //                                                  0         1            2                3            4            5            6            7            8          9
    QueryResult result = WorldDatabase.Query("SELECT `entry`, `spellId`, `customEntry`, `customVisualId`, `Radius`, `RadiusTarget`, `Height`, `HeightTarget`, `Float4`, `Float5`,"
    //    9          10        11            12                13           14         15           16             17           18      19       20         21           22         23
    "`isMoving`, `moveType`, `speed`, `activationDelay`, `updateDelay`, `maxCount`, `hitType`, `MoveCurveID`, `ElapsedTime`, `windX`, `windY`, `windZ`, `windSpeed`, `windType`, `polygon`,"
    "`MorphCurveID`, `FacingCurveID`, `ScaleCurveID`, `HasFollowsTerrain`, `HasAttached`, `HasAbsoluteOrientation`, `HasDynamicShape`, `HasFaceMovementDir`, `waitTime`,"
    "`hasAreaTriggerBox`, `RollPitchYaw1X`, `RollPitchYaw1Y`, `RollPitchYaw1Z`, `TargetRollPitchYawX`, `TargetRollPitchYawY`, `TargetRollPitchYawZ` FROM areatrigger_data");

    if (result)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 i = 0;
            uint32 id = fields[i++].GetUInt32();
            AreaTriggerInfo& info = _areaTriggerData[id];
            info.spellId = fields[i++].GetUInt32();
            info.customEntry = fields[i++].GetUInt32();
            info.visualId = fields[i++].GetUInt32();
            info.Radius = fields[i++].GetFloat();
            info.RadiusTarget = fields[i++].GetFloat();
            info.Height = fields[i++].GetFloat();
            info.HeightTarget = fields[i++].GetFloat();
            info.Float4 = fields[i++].GetFloat();
            info.Float5 = fields[i++].GetFloat();
            info.isMoving = fields[i++].GetBool();
            info.moveType = fields[i++].GetUInt32();
            info.speed = fields[i++].GetFloat();
            info.activationDelay = fields[i++].GetUInt32();
            info.updateDelay = fields[i++].GetUInt32();
            info.maxCount = fields[i++].GetUInt8();
            info.hitType = fields[i++].GetUInt32();
            info.MoveCurveID = fields[i++].GetUInt32();
            info.ElapsedTime = fields[i++].GetUInt32();
            info.windX = fields[i++].GetFloat();
            info.windY = fields[i++].GetFloat();
            info.windZ = fields[i++].GetFloat();
            info.windSpeed = fields[i++].GetFloat();
            info.windType = fields[i++].GetUInt32();
            info.polygon = fields[i++].GetUInt32();
            info.MorphCurveID = fields[i++].GetUInt32();
            info.FacingCurveID = fields[i++].GetUInt32();
            info.ScaleCurveID = fields[i++].GetUInt32();
            info.HasFollowsTerrain = fields[i++].GetUInt32();
            info.HasAttached = fields[i++].GetUInt32();
            info.HasAbsoluteOrientation = fields[i++].GetUInt32();
            info.HasDynamicShape = fields[i++].GetUInt32();
            info.HasFaceMovementDir = fields[i++].GetUInt32();
            info.waitTime = fields[i++].GetUInt32();
            info.hasAreaTriggerBox = fields[i++].GetUInt32();
            info.RollPitchYaw1X = fields[i++].GetFloat();
            info.RollPitchYaw1Y = fields[i++].GetFloat();
            info.RollPitchYaw1Z = fields[i++].GetFloat();
            info.TargetRollPitchYawX = fields[i++].GetFloat();
            info.TargetRollPitchYawY = fields[i++].GetFloat();
            info.TargetRollPitchYawZ = fields[i++].GetFloat();
            if(info.polygon && info.customEntry)
            {
                QueryResult resultPolygon = WorldDatabase.PQuery("SELECT `id`, `x`, `y` FROM areatrigger_polygon WHERE `entry` = '%u' AND `spellId` = '%u' AND `type` = 1", info.customEntry, info.spellId);
                if (resultPolygon)
                {
                    do
                    {
                        Field* fieldP = resultPolygon->Fetch();
                        PolygonPOI polygonPOI;
                        polygonPOI.id = fieldP[0].GetUInt32();
                        polygonPOI.x = fieldP[1].GetFloat();
                        polygonPOI.y = fieldP[2].GetFloat();
                        info.verticesPoints[polygonPOI.id] = polygonPOI;
                    }
                    while (resultPolygon->NextRow());
                }
                QueryResult resultPolygonTarget = WorldDatabase.PQuery("SELECT `id`, `x`, `y` FROM areatrigger_polygon WHERE `entry` = '%u' AND `spellId` = '%u' AND `type` = 2", info.customEntry, info.spellId);
                if (resultPolygonTarget)
                {
                    do
                    {
                        Field* fieldP = resultPolygonTarget->Fetch();
                        PolygonPOI polygonPOI;
                        polygonPOI.id = fieldP[0].GetUInt32();
                        polygonPOI.x = fieldP[1].GetFloat();
                        polygonPOI.y = fieldP[2].GetFloat();
                        info.verticesTargetPoints[polygonPOI.id] = polygonPOI;
                    }
                    while (resultPolygonTarget->NextRow());
                }
            }
            if(info.windSpeed)
            {
                AreaTriggerAction actionEnter;
                actionEnter.id = 0;
                actionEnter.moment = AT_ACTION_MOMENT_ENTER;
                actionEnter.actionType = AT_ACTION_TYPE_APPLY_MOVEMENT_FORCE;
                actionEnter.targetFlags = AreaTriggerTargetFlags(AT_TARGET_FLAG_PLAYER | AT_TARGET_FLAG_HOSTILE);
                actionEnter.spellId = info.spellId;
                actionEnter.maxCharges = 0;
                actionEnter.chargeRecoveryTime = 0;
                actionEnter.aura = 0;
                actionEnter.hasspell = 0;
                actionEnter.scale = 0;
                actionEnter.hitMaxCount = 0;
                info.actions.push_back(actionEnter);

                AreaTriggerAction actionLeave;
                actionLeave.id = 1;
                actionLeave.moment = AreaTriggerActionMoment(AT_ACTION_MOMENT_LEAVE | AT_ACTION_MOMENT_DESPAWN | AT_ACTION_MOMENT_REMOVE);
                actionLeave.actionType = AT_ACTION_TYPE_REMOVE_MOVEMENT_FORCE;
                actionLeave.targetFlags = AreaTriggerTargetFlags(0);
                actionLeave.spellId = info.spellId;
                actionLeave.maxCharges = 0;
                actionLeave.chargeRecoveryTime = 0;
                actionLeave.aura = 0;
                actionLeave.hasspell = 0;
                actionLeave.scale = 0;
                actionLeave.hitMaxCount = 0;
                info.actions.push_back(actionLeave);
            }
            ++counter;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u areatrigger data.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 areatrigger data. DB table `areatrigger_data` is empty.");

    //                                                0      1     2         3           4           5         6                7           8       9       10        11         12
    QueryResult result2 = WorldDatabase.Query("SELECT entry, id, moment, actionType, targetFlags, spellId, maxCharges, chargeRecoveryTime, aura, hasspell, scale, hitMaxCount, amount FROM areatrigger_actions");
    if (result2)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result2->Fetch();

            uint8 i = 0;
            AreaTriggerAction action;
            uint32 entry = fields[i++].GetUInt32();
            action.id = fields[i++].GetUInt32();
            action.moment = (AreaTriggerActionMoment)fields[i++].GetUInt32();
            action.actionType = (AreaTriggerActionType)fields[i++].GetUInt32();
            action.targetFlags = (AreaTriggerTargetFlags)fields[i++].GetUInt32();
            action.spellId = fields[i++].GetUInt32();
            action.maxCharges = fields[i++].GetInt8();
            action.chargeRecoveryTime = fields[i++].GetUInt32();
            action.aura = fields[i++].GetInt32();
            action.hasspell = fields[i++].GetInt32();
            action.scale = fields[i++].GetFloat();
            action.hitMaxCount = fields[i++].GetInt32();
            action.amount = fields[i++].GetInt32();

            if (action.actionType >= AT_ACTION_TYPE_MAX)
            {
                sLog->outError(LOG_FILTER_SQL, "DB table `areatrigger_actions` has invalid action type '%u' for areatrigger entry %u",
                    action.actionType, entry);
                continue;
            }

            if (action.actionType == AT_ACTION_TYPE_CHANGE_AMOUNT_FROM_HEALT && !(action.targetFlags & AT_TARGET_FLAG_NOT_FULL_HP))
                action.targetFlags = AreaTriggerTargetFlags(action.targetFlags | AT_TARGET_FLAG_NOT_FULL_HP);

            if (!sSpellMgr->GetSpellInfo(action.spellId))
            {
                sLog->outError(LOG_FILTER_SQL, "DB table `areatrigger_actions` has non-existant spell id '%u' for areatrigger entry %u",
                    action.spellId, entry);
                continue;
            }

            AreaTriggerInfo& info = _areaTriggerData[entry];
            info.actions.push_back(action);
            ++counter;
        }
        while (result2->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u areatrigger actions.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 areatrigger actions. DB table `areatrigger_actions` is empty.");
}

void ObjectMgr::LoadScenarioData()
{
    _scenarioData.clear();
    _scenarioDataList.clear();

    //                                                     0          1            2              3            4             5                    6                         7
    QueryResult result = WorldDatabase.Query("SELECT `ScenarioID`, `mapId`, `DifficultyID`, `WaveCurrent`, `WaveMax`, `TimerDuration`, `CriteriaProgressCount`, `BonusObjectiveDataCount` FROM `scenario_data`");
    if (result)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 i = 0;
            uint32 ScenarioID = fields[i++].GetUInt32();
            ScenarioData& data = _scenarioData[ScenarioID];
            data.mapId = fields[i++].GetUInt32();
            data.DifficultyID = fields[i++].GetUInt32();
            data.WaveCurrent = fields[i++].GetUInt32();
            data.WaveMax = fields[i++].GetUInt32();
            data.TimerDuration = fields[i++].GetUInt32();
            data.CriteriaProgressCount = fields[i++].GetUInt32();
            data.BonusObjectiveDataCount = fields[i++].GetUInt32();
            data.ScenarioID = ScenarioID;
            _scenarioDataList[data.mapId].push_back(&data);
            ++counter;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Scenario data.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Scenario data. DB table `scenario_data` is empty.");
}

void ObjectMgr::LoadConversationData()
{
    _conversationDataList.clear();
    _conversationCreatureList.clear();
    _conversationActorList.clear();

    //                                                  0       1       2       3       4
    QueryResult result = WorldDatabase.Query("SELECT `entry`, `id`, `textId`, `unk1`, `unk2` FROM `conversation_data` ORDER BY idx");
    if (result)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 i = 0;
            ConversationData data;
            data.entry = fields[i++].GetUInt32();
            data.id = fields[i++].GetUInt32();
            data.textId = fields[i++].GetUInt32();
            data.unk1 = fields[i++].GetUInt32();
            data.unk2 = fields[i++].GetUInt32();
            _conversationDataList[data.entry].push_back(data);
            ++counter;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u conversation data.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 conversation data. DB table `conversation_data` is empty.");

    //                                      0      1         2               3          4       5         6
    result = WorldDatabase.Query("SELECT `entry`, `id`, `creatureId`, `creatureGuid`, `unk1`, `unk2`, `duration` FROM `conversation_creature` ORDER BY id");
    if (result)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 i = 0;
            ConversationCreature data;
            data.entry = fields[i++].GetUInt32();
            data.id = fields[i++].GetUInt32();
            data.creatureId = fields[i++].GetUInt32();
            data.creatureGuid = fields[i++].GetUInt32();
            data.unk1 = fields[i++].GetUInt32();
            data.unk2 = fields[i++].GetUInt32();
            data.duration = fields[i++].GetUInt32();
            _conversationCreatureList[data.entry].push_back(data);
            ++counter;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u conversation creature data.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 conversation creature data. DB table `conversation_creature` is empty.");


    //                                      0      1        2           3             4         5       6       7         8
    result = WorldDatabase.Query("SELECT `entry`, `id`, `actorId`, `creatureId`, `displayId`, `unk1`, `unk2`, `unk3`, `duration` FROM `conversation_actor` ORDER BY id");
    if (result)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 i = 0;
            ConversationActor data;
            data.entry = fields[i++].GetUInt32();
            data.id = fields[i++].GetUInt32();
            data.actorId = fields[i++].GetUInt32();
            data.creatureId = fields[i++].GetUInt32();
            data.displayId = fields[i++].GetUInt32();
            data.unk1 = fields[i++].GetUInt32();
            data.unk2 = fields[i++].GetUInt32();
            data.unk3 = fields[i++].GetUInt32();
            data.duration = fields[i++].GetUInt32();
            _conversationActorList[data.entry].push_back(data);
            ++counter;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u conversation actor data.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 conversation actor data. DB table `conversation_actor` is empty.");
}


void ObjectMgr::LoadCreatureActionData()
{
    for (uint8 i = 0; i < 2; ++i)
        for (uint8 j = 0; j < 2; ++j)
            _creatureActionDataList[i][j].clear();

    //                                                  0         1        2        3         4
    QueryResult result = WorldDatabase.Query("SELECT `entry`, `target`, `type`, `spellId`, `action` FROM `creature_action`");
    if (result)
    {
        uint32 counter = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 i = 0;
            CreatureActionData data;
            data.entry = fields[i++].GetUInt32();
            data.target = fields[i++].GetUInt32();
            data.type = fields[i++].GetUInt32();
            data.spellId = fields[i++].GetUInt32();
            data.action = fields[i++].GetUInt32();
            _creatureActionDataList[data.type][data.action][data.entry].push_back(data);
            ++counter;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature_action data.", counter);
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature_action data. DB table `creature_action` is empty.");
}

const ScenarioData* ObjectMgr::GetScenarioOnMap(uint32 mapId, uint32 scenarioId) const
{
    ScenarioDataListMap::const_iterator itr = _scenarioDataList.find(mapId);
    if(itr == _scenarioDataList.end())
        return NULL;

    for (std::list<ScenarioData* >::const_iterator iter = itr->second.begin(); iter != itr->second.end(); ++iter)
    {
        if(scenarioId)
        {
            if((*iter)->ScenarioID == scenarioId)
                return (*iter);
        }
        else
            return (*iter);
    }

    return NULL;
}

const std::vector<ConversationData>* ObjectMgr::GetConversationData(uint32 entry) const
{
    ConversationDataMap::const_iterator itr = _conversationDataList.find(entry);
    return itr != _conversationDataList.end() ? &(itr->second) : NULL;
}

const std::vector<ConversationCreature>* ObjectMgr::GetConversationCreature(uint32 entry) const
{
    ConversationCreatureMap::const_iterator itr = _conversationCreatureList.find(entry);
    return itr != _conversationCreatureList.end() ? &(itr->second) : NULL;
}

const std::vector<ConversationActor>* ObjectMgr::GetConversationActor(uint32 entry) const
{
    ConversationActorMap::const_iterator itr = _conversationActorList.find(entry);
    return itr != _conversationActorList.end() ? &(itr->second) : NULL;
}

const std::vector<CreatureActionData>* ObjectMgr::GetCreatureActionData(uint32 entry, uint8 type, uint8 action) const
{
    CreatureActionDataMap::const_iterator itr = _creatureActionDataList[type][action].find(entry);
    return itr != _creatureActionDataList[type][action].end() ? &(itr->second) : NULL;
}

AreaTriggerInfo const* ObjectMgr::GetAreaTriggerInfo(uint32 entry)
{
    AreaTriggerInfoMap::const_iterator itr = _areaTriggerData.find(entry);
    return itr != _areaTriggerData.end() ? &itr->second : NULL;
}

void ObjectMgr::LoadRaceAndClassExpansionRequirements()
{
    uint32 oldMSTime = getMSTime();
    _raceExpansionRequirementStore.clear();

    //                                               0       1
    QueryResult result = WorldDatabase.Query("SELECT raceID, expansion FROM `race_expansion_requirement`");

    if (result)
    {
        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 raceID = fields[0].GetInt8();
            uint8 expansion = fields[1].GetInt8();

            ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(raceID);
            if (!raceEntry)
            {
                sLog->outError(LOG_FILTER_SQL, "Race %u defined in `race_expansion_requirement` does not exists, skipped.", raceID);
                continue;
            }

            if (expansion >= MAX_EXPANSIONS)
            {
                sLog->outError(LOG_FILTER_SQL, "Race %u defined in `race_expansion_requirement` has incorrect expansion %u, skipped.", raceID, expansion);
                continue;
            }

            _raceExpansionRequirementStore[raceID] = expansion;

            ++count;
        }
        while (result->NextRow());
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u race expansion requirements in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 race expansion requirements. DB table `race_expansion_requirement` is empty.");

    oldMSTime = getMSTime();
    _classExpansionRequirementStore.clear();

    //                                   0        1
    result = WorldDatabase.Query("SELECT classID, expansion FROM `class_expansion_requirement`");

    if (result)
    {
        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();

            uint8 classID = fields[0].GetInt8();
            uint8 expansion = fields[1].GetInt8();

            ChrClassesEntry const* classEntry = sChrClassesStore.LookupEntry(classID);
            if (!classEntry)
            {
                sLog->outError(LOG_FILTER_SQL, "Class %u defined in `class_expansion_requirement` does not exists, skipped.", classID);
                continue;
            }

            if (expansion >= MAX_EXPANSIONS)
            {
                sLog->outError(LOG_FILTER_SQL, "Class %u defined in `class_expansion_requirement` has incorrect expansion %u, skipped.", classID, expansion);
                continue;
            }

            _classExpansionRequirementStore[classID] = expansion;

            ++count;
        }
        while (result->NextRow());
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u class expansion requirements in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
    }
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 class expansion requirements. DB table `class_expansion_requirement` is empty.");
}

void ObjectMgr::LoadCharacterTemplates()
{
    uint32 oldMSTime = getMSTime();
    _characterTemplateStore.clear();

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CHARACTER_TEMPLATES);
    PreparedQueryResult templates = WorldDatabase.Query(stmt);

    if (!templates)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 character templates. DB table `character_template` is empty.");
        return;
    }

    PreparedQueryResult classes;
    uint32 count[2] = {0, 0};

    do
    {
        Field* fields = templates->Fetch();

        uint32 templateSetID = fields[0].GetUInt32();

        stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CHARACTER_TEMPLATE_CLASSES);
        stmt->setUInt32(0, templateSetID);
        classes = WorldDatabase.Query(stmt);

        if (classes)
        {
            CharacterTemplate templ;
            templ.TemplateSetID = templateSetID;
            templ.Name = fields[1].GetString();
            templ.Description = fields[2].GetString();

            do
            {
                fields = classes->Fetch();

                uint8 factionGroup = fields[0].GetUInt8();
                if (!((factionGroup & (FACTION_MASK_PLAYER | FACTION_MASK_ALLIANCE)) == (FACTION_MASK_PLAYER | FACTION_MASK_ALLIANCE)) &&
                    !((factionGroup & (FACTION_MASK_PLAYER | FACTION_MASK_HORDE)) == (FACTION_MASK_PLAYER | FACTION_MASK_HORDE)))
                    continue;

                uint8 classID = fields[1].GetUInt8();
                if (!sChrClassesStore.LookupEntry(classID))
                    continue;

                ++count[1];
                templ.Classes.emplace_back(factionGroup, classID);
                templ.Level = fields[2].GetUInt8();
                templ.X = fields[3].GetFloat();
                templ.Y = fields[4].GetFloat();
                templ.Z = fields[5].GetFloat();
                templ.O = fields[6].GetFloat();
                templ.MapID = fields[7].GetUInt16();
                templ.Money = fields[8].GetUInt32();
            }
            while (classes->NextRow());

            if (!templ.Classes.empty())
            {
                _characterTemplateStore[templateSetID] = templ;
                ++count[0];
            }
        }
        else
            continue;
    }
    while (templates->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u character templates with character_template for %u classes in %u ms.", count[0], count[1], GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadRealmNames()
{
    uint32 oldMSTime = getMSTime();
    _realmNameStore.clear();

    //                                               0   1
    QueryResult result = LoginDatabase.Query("SELECT id, name FROM `realmlist`");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 realm names. DB table `realmlist` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 realm = fields[0].GetUInt32();
        std::string realmName = fields[1].GetString();

        _realmNameStore[realm] = realmName;

        ++count;
    }
    while (result->NextRow());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u realm names in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

std::string ObjectMgr::GetRealmName(uint32 realm) const
{
    RealmNameContainer::const_iterator iter = _realmNameStore.find(realm);
    return iter != _realmNameStore.end() ? iter->second : "";
}

void ObjectMgr::LoadBattlePay()
{
    uint32 oldMSTime = getMSTime();

    //                                                  0            1              2               3           4
    QueryResult result = WorldDatabase.Query("SELECT `GroupID`, `IconFileDataID`, `DisplayType`, `Ordering`, `Name` FROM `battlepay_group`");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            WorldPackets::BattlePay::ProductGroup group;
            group.GroupID = fields[0].GetInt32();
            group.IconFileDataID = fields[1].GetInt32();
            group.DisplayType = fields[2].GetInt8();
            group.Ordering =  fields[3].GetInt32();
            group.Name = fields[4].GetString();
            productList.productGroup.push_back(group);
        }
        while (result->NextRow());
    }

    //                                                  0            1                          2                           3         4       5              6                          7                   8               9               10              11          12              13          14                  15              16
    result = WorldDatabase.Query("SELECT `ProductID`, `NormalPriceFixedPoint`, `CurrentPriceFixedPoint`, `ChoiceType`, `Type`, `Flags`, `dInfo_CreatureDisplayInfoID`, `dInfo_FileDataID`, `dInfo_Name1`, `dInfo_Name2`, `dInfo_Name3`, `dInfo_Flags`, `pItem_ID`, `pItem_ItemID`, `pItem_Quantity`, `pItem_HasPet`, `pItem_PetResult` FROM `battlepay_product`");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            WorldPackets::BattlePay::Product product;
            product.ProductID = fields[0].GetInt32();
            product.NormalPriceFixedPoint = fields[1].GetInt64();
            product.CurrentPriceFixedPoint = fields[2].GetInt64();
            product.ChoiceType = fields[3].GetInt8();
            product.Type = fields[4].GetInt8();
            product.Flags = fields[5].GetInt32();

            WorldPackets::BattlePay::DisplayInfo displayInfo;
            if (fields[6].GetInt32())
                displayInfo.CreatureDisplayInfoID = fields[6].GetInt32();
            if (fields[7].GetInt32())
                displayInfo.FileDataID = fields[7].GetInt32();

            displayInfo.Name1 = fields[8].GetString();
            displayInfo.Name2 = fields[9].GetString();
            displayInfo.Name3 = fields[10].GetString();

            if (fields[11].GetInt32())
                displayInfo.Flags = fields[11].GetInt32();
            product.displayInfo = displayInfo;

            if (fields[12].GetInt32() > 0)
            {
                WorldPackets::BattlePay::ProductItem pItem;
                pItem.ID = fields[12].GetInt32();
                pItem.ItemID = fields[13].GetInt32();
                pItem.Quantity = fields[14].GetInt32();
                pItem.HasPet = fields[15].GetBool();
                if (fields[16].GetInt8())
                    pItem.PetResult = fields[16].GetInt8();

                //! Little hack for WoW
                if (pItem.ItemID == 122270)
                    pItem.displayInfo = displayInfo;

                product.battlePayProduct.push_back(pItem);

            }

            if (product.ProductID !=107) //WowToken kill game.
                productList.product.push_back(product);
            BattlePayProductMap[product.ProductID] = product;
        }
        while (result->NextRow());
    }

    //                                                  0            1           2          3           4       5                   6                           7                   8           9               10              11
    result = WorldDatabase.Query("SELECT `EntryID`, `GroupID`, `ProductID`, `Ordering`, `Flags`, `BannerType`, `dInfo_CreatureDisplayInfoID`, `dInfo_FileDataID`, `dInfo_Name1`, `dInfo_Name2`, `dInfo_Name3`, `dInfo_Flags` FROM `battlepay_shopentry`");

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            WorldPackets::BattlePay::ShopEntry shop;
            shop.EntryID = fields[0].GetInt32();
            shop.GroupID = fields[1].GetInt32();
            shop.ProductID = fields[2].GetInt32();
            shop.Ordering = fields[3].GetInt32();
            shop.Flags = fields[4].GetInt32();
            shop.BannerType = fields[5].GetInt32();

            WorldPackets::BattlePay::DisplayInfo info;

            if (fields[6].GetInt32())
                info.CreatureDisplayInfoID = fields[6].GetInt32();
            if (fields[7].GetInt32())
                info.FileDataID = fields[7].GetInt32();

            if (info.FileDataID || info.CreatureDisplayInfoID)
            {
                info.Name1 = fields[8].GetString();
                info.Name2 = fields[9].GetString();
                info.Name3 = fields[10].GetString();
                if (fields[11].GetInt32())
                    info.Flags = fields[11].GetInt32();
                shop.displayInfo = info;
            }

            if (shop.ProductID !=107) //WowToken kill game.
                productList.shop.push_back(shop);
        }
        while (result->NextRow());
    }
}

void ObjectMgr::AddCharToDupeLog(ObjectGuid const& guid)
{
    m_dupeLogMap.insert(guid);
}

bool ObjectMgr::IsPlayerInLogList(Player *player)
{
    if(m_dupeLogMap.empty())
        return false;

    ObjectGuid guid = player->GetGUID();
    auto itr = m_dupeLogMap.find(guid);
    if (itr != m_dupeLogMap.end())
        return true;

    return false;
}

void ObjectMgr::RemoveCharFromDupeList(ObjectGuid const& guid)
{
    m_dupeLogMap.erase(guid);
}

void ObjectMgr::DumpDupeConstant(Player *player)
{
    sLog->outDebug(LOG_FILTER_DUPE, "Name: %s. Items: %u; Pos: map - %u; %f; %f; %f;", player->GetName(), player->GetItemCount(38186, true),
                  player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());
}

void ObjectMgr::LoadSkillTiers()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT ID, Value1, Value2, Value3, Value4, Value5, Value6, Value7, Value8, Value9, Value10, "
                                             " Value11, Value12, Value13, Value14, Value15, Value16 FROM skill_tiers");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        SkillTiersEntry& tier = _skillTiers[fields[0].GetUInt32()];
        for (uint32 i = 0; i < MAX_SKILL_STEP; ++i)
            tier.Value[i] = fields[1 + i].GetUInt32();

    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u skill max values in %u ms", uint32(_skillTiers.size()), GetMSTimeDiffToNow(oldMSTime));
}

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

/* ScriptData
Name: achievement_commandscript
%Complete: 100
Comment: All achievement related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "AchievementMgr.h"

class achievement_commandscript : public CommandScript
{
public:
    achievement_commandscript() : CommandScript("achievement_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand achievementCommandTable[] =
        {
            { "add",            SEC_ADMINISTRATOR,  false,  &HandleAchievementAddCommand,      "", NULL },
            { "criteria",       SEC_ADMINISTRATOR,  false,  &HandleAchievementCriteriaCommand, "", NULL },
            { "info",           SEC_ADMINISTRATOR,  false,  &HandleAchievementInfoCommand,     "", NULL },
            { NULL,             0,                  false,  NULL,                              "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "achievement",    SEC_ADMINISTRATOR,  false, NULL,            "", achievementCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    static bool HandleAchievementAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 achievementId = atoi((char*)args);
        if (!achievementId)
        {
            if (char* id = handler->extractKeyFromLink((char*)args, "Hachievement"))
                achievementId = atoi(id);
            if (!achievementId)
                return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievementId))
            target->CompletedAchievement(achievementEntry);

        return true;
    }

    static bool HandleAchievementCriteriaCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* criteriatr = strtok((char*)args, " ");
        char* miscValueStr1 = strtok(NULL, " ");
        char* miscValueStr2 = strtok(NULL, " ");
        char* miscValueStr3 = strtok(NULL, " ");

        uint32 criteriaType = criteriatr ? atoi(criteriatr) : 0;
        uint32 miscValue1 = miscValueStr1 ? atoi(miscValueStr1) : 0;
        uint32 miscValue2 = miscValueStr2 ? atoi(miscValueStr2) : 0;
        uint32 miscValue3 = miscValueStr3 ? atoi(miscValueStr2) : 0;
        sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "HandleAchievementCriteriaCommand criteriaType %i miscValue1 %i miscValue2 %i, miscValue3 %i", criteriaType, miscValue1, miscValue2, miscValue3);

        if(Player* player = handler->GetSession()->GetPlayer())
        {
            if(!criteriaType)
                player->KilledMonsterCredit(miscValue1);
            else
                player->UpdateAchievementCriteria(AchievementCriteriaTypes(criteriaType), miscValue1, miscValue2, miscValue3, handler->getSelectedUnit());
        }

        return true;
    }

    static bool HandleAchievementInfoCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 achievementId = atoi((char*)args);
        if (!achievementId)
        {
            if (char* id = handler->extractKeyFromLink((char*)args, "Hachievement"))
                achievementId = atoi(id);
            if (!achievementId)
                return false;
        }

        if (AchievementEntry const* achievement = sAchievementStore.LookupEntry(achievementId))
        {
            sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo achievement %u", achievement->ID);

            if(Player* player = handler->GetSession()->GetPlayer())
            {
                sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo player %u", achievement->ID);
                if(CriteriaProgressTree* treeProgress = player->GetAchievementMgr().GetCriteriaTreeProgressMap(achievement->CriteriaTree))
                {
                    sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo criteriaTree %u, achievement %u ChildrenTree %u ChildrenCriteria %u", achievement->CriteriaTree, achievement->ID, treeProgress->ChildrenTree.size(), treeProgress->ChildrenCriteria.size());

                    for (std::vector<CriteriaProgressTree*>::iterator itr = treeProgress->ChildrenTree.begin(); itr != treeProgress->ChildrenTree.end(); ++itr)
                        sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo ChildrenTree criteriaTree %u parent %u", (*itr)->criteriaTree->ID, (*itr)->criteriaTree->Parent);

                    for (std::vector<CriteriaTreeProgress const*>::const_iterator itr = treeProgress->ChildrenCriteria.begin(); itr != treeProgress->ChildrenCriteria.end(); ++itr)
                    {
                        CriteriaTreeProgress const* progress = *itr;
                        if(!progress)
                        {
                            sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo ChildrenCriteria error progress achievement %u", achievement->ID);
                            continue;
                        }
                        CriteriaEntry const* criteria = progress->criteria;
                        if(!criteria)
                        {
                            sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo ChildrenCriteria error criteria achievement %u criteriaTree %u parent %u", achievement->ID, progress->criteriaTree->ID, progress->parent->ID);
                            continue;
                        }
                        sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo ChildrenCriteria criteria achievement %u criteriaTree %u parent %u criteria %u completed %i deactiveted %i",
                        achievement->ID, progress->criteriaTree ? progress->criteriaTree->ID : 0, progress->parent ? progress->parent->ID : 0, criteria->ID, progress->completed, progress->deactiveted);
                    }
                }
            }
        }
        else
            sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementInfo error achievement %u not found", achievementId);

        return true;
    }
};

void AddSC_achievement_commandscript()
{
    new achievement_commandscript();
}

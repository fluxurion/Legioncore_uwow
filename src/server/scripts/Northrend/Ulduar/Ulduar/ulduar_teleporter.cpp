/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "ulduar.h"

/*
The teleporter appears to be active and stable.

- Expedition Base Camp
- Formation Grounds
- Colossal Forge
- Scrapyard
- Antechamber of Ulduar
- Shattered Walkway
- Conservatory of Life
- Spark of Imagination
- Descent into the Madness
*/

enum Locations
{
    BASE_CAMP                                   = 200,
    GROUNDS                                     = 201,
    FORGE                                       = 202,
    SCRAPYARD                                   = 203,
    ANTECHAMBER                                 = 204,
    WALKWAY                                     = 205,
    CONSERVATORY                                = 206,
    SPARK_IMAGINATION                           = 207,
    DESCENT_MADNESS                             = 208
};

class ulduar_teleporter : public GameObjectScript
{
public:
    ulduar_teleporter() : GameObjectScript("ulduar_teleporter") { }

    bool OnGossipSelect(Player *pPlayer, GameObject * /*pGO*/, uint32 sender, uint32 action)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN)
            return false;
        if (!pPlayer->getAttackers().empty())
            return false;

        switch(action)
        {
            case BASE_CAMP:
                pPlayer->TeleportTo(603, -706.122f, -92.6024f, 429.876f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case GROUNDS:
                pPlayer->TeleportTo(603, 131.248f, -35.3802f, 409.804f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case FORGE:
                pPlayer->TeleportTo(603, 553.233f, -12.3247f, 409.679f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case SCRAPYARD:
                pPlayer->TeleportTo(603, 926.292f, -11.4635f, 418.595f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case ANTECHAMBER:
                pPlayer->TeleportTo(603, 1498.09f, -24.246f, 420.967f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case WALKWAY:
                pPlayer->TeleportTo(603, 1859.45f, -24.1f, 448.9f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case CONSERVATORY:
                pPlayer->TeleportTo(603, 2086.27f, -24.3134f, 421.239f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case SPARK_IMAGINATION:
                pPlayer->TeleportTo(603, 2518.16f, 2569.03f, 412.299f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
            case DESCENT_MADNESS:
                pPlayer->TeleportTo(603, 1854.82f, -11.5608f, 334.175f, 0);
                pPlayer->CLOSE_GOSSIP_MENU(); break;
        }

        return true;
    }

    bool OnGossipHello(Player *pPlayer, GameObject *pGO)
    {
        InstanceScript* instance = pGO->GetInstanceScript();
        if (!instance)
            return false;

        pPlayer->ADD_GOSSIP_ITEM(0, "Expedition Base Camp", GOSSIP_SENDER_MAIN, BASE_CAMP);
        //pPlayer->ADD_GOSSIP_ITEM(0, "Formation Grounds", GOSSIP_SENDER_MAIN, GROUNDS);
        if (instance->GetBossState(BOSS_LEVIATHAN) == DONE || pPlayer->isGameMaster())
        {
            pPlayer->ADD_GOSSIP_ITEM(0, "Colossal Forge", GOSSIP_SENDER_MAIN, FORGE);
            if (instance->GetBossState(BOSS_XT002) == DONE || pPlayer->isGameMaster())
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "Scrapyard", GOSSIP_SENDER_MAIN, SCRAPYARD);
                pPlayer->ADD_GOSSIP_ITEM(0, "Antechamber of Ulduar", GOSSIP_SENDER_MAIN, ANTECHAMBER);
                if (instance->GetBossState(BOSS_KOLOGARN) == DONE || pPlayer->isGameMaster())
                {
                    pPlayer->ADD_GOSSIP_ITEM(0, "Shattered Walkway", GOSSIP_SENDER_MAIN, WALKWAY);
                    if (instance->GetBossState(BOSS_AURIAYA) == DONE || pPlayer->isGameMaster())
                    {
                        pPlayer->ADD_GOSSIP_ITEM(0, "Conservatory of Life", GOSSIP_SENDER_MAIN, CONSERVATORY);
                        if (instance->GetBossState(BOSS_MIMIRON) == FAIL || instance->GetBossState(BOSS_MIMIRON) == DONE || pPlayer->isGameMaster())
                        {
                            pPlayer->ADD_GOSSIP_ITEM(0, "Spark of Imagination", GOSSIP_SENDER_MAIN, SPARK_IMAGINATION);
                            if (instance->GetBossState(BOSS_VEZAX) == DONE || pPlayer->isGameMaster())
                                pPlayer->ADD_GOSSIP_ITEM(0, "Descent into Madness", GOSSIP_SENDER_MAIN, DESCENT_MADNESS);
                        }
                    }
                }
            }
        }
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pGO->GetGUID());
        return true;
    }

};

void AddSC_ulduar_teleporter()
{
    new ulduar_teleporter();
}
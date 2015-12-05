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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "CreatureTextMgr.h"
#include "culling_of_stratholme.h"

#define MAX_ENCOUNTER 5

/* Culling of Stratholme encounters:
0 - Meathook
1 - Salramm the Fleshcrafter
2 - Chrono-Lord Epoch
3 - Mal'Ganis
4 - Infinite Corruptor (Heroic only)
*/

enum Texts
{
    SAY_CRATES_COMPLETED    = 0,
};

Position const ChromieSummonPos = {1813.298f, 1283.578f, 142.3258f, 3.878161f};

class instance_culling_of_stratholme : public InstanceMapScript
{
    public:
        instance_culling_of_stratholme() : InstanceMapScript("instance_culling_of_stratholme", 595) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_culling_of_stratholme_InstanceMapScript(map);
        }

        struct instance_culling_of_stratholme_InstanceMapScript : public InstanceScript
        {
            instance_culling_of_stratholme_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                _arthasGUID.Clear();
                _meathookGUID.Clear();
                _salrammGUID.Clear();
                _epochGUID.Clear();
                _malGanisGUID.Clear();
                _infiniteGUID.Clear();
                _shkafGateGUID.Clear();
                _malGanisGate1GUID.Clear();
                _malGanisGate2GUID.Clear();
                _exitGateGUID.Clear();
                _malGanisChestGUID.Clear();
                _genericBunnyGUID.Clear();
                memset(&_encounterState[0], 0, sizeof(uint32) * MAX_ENCOUNTER);
                _crateCount = 0;
            }

            bool IsEncounterInProgress() const
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (_encounterState[i] == IN_PROGRESS)
                        return true;

                return false;
            }

            void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
            {
                packet.Worldstates.emplace_back(WorldStates::WORLDSTATE_SHOW_CRATES, 1);
                packet.Worldstates.emplace_back(WorldStates::WORLDSTATE_CRATES_REVEALED, _crateCount);
                packet.Worldstates.emplace_back(WorldStates::WORLDSTATE_WAVE_COUNT, 0);
                packet.Worldstates.emplace_back(WorldStates::WORLDSTATE_TIME_GUARDIAN, 25);
                packet.Worldstates.emplace_back(WorldStates::WORLDSTATE_TIME_GUARDIAN_SHOW, 0);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_ARTHAS:
                        _arthasGUID = creature->GetGUID();
                        break;
                    case NPC_MEATHOOK:
                        _meathookGUID = creature->GetGUID();
                        break;
                    case NPC_SALRAMM:
                        _salrammGUID = creature->GetGUID();
                        break;
                    case NPC_EPOCH:
                        _epochGUID = creature->GetGUID();
                        break;
                    case NPC_MAL_GANIS:
                        _malGanisGUID = creature->GetGUID();
                        break;
                    case NPC_INFINITE:
                        _infiniteGUID = creature->GetGUID();
                        break;
                    case NPC_GENERIC_BUNNY:
                        _genericBunnyGUID = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GO_SHKAF_GATE:
                        _shkafGateGUID = go->GetGUID();
                        break;
                    case GO_MALGANIS_GATE_1:
                        _malGanisGate1GUID = go->GetGUID();
                        break;
                    case GO_MALGANIS_GATE_2:
                        _malGanisGate2GUID = go->GetGUID();
                        break;
                    case GO_EXIT_GATE:
                        _exitGateGUID = go->GetGUID();
                        if (_encounterState[3] == DONE)
                            HandleGameObject(_exitGateGUID, true);
                        break;
                    case GO_MALGANIS_CHEST_N:
                    case GO_MALGANIS_CHEST_H:
                        _malGanisChestGUID = go->GetGUID();
                        if (_encounterState[3] == DONE)
                            go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                        break;
                }
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_MEATHOOK_EVENT:
                        _encounterState[0] = data;
                        break;
                    case DATA_SALRAMM_EVENT:
                        _encounterState[1] = data;
                        break;
                    case DATA_EPOCH_EVENT:
                        _encounterState[2] = data;
                        break;
                    case DATA_MAL_GANIS_EVENT:
                        _encounterState[3] = data;

                        switch (_encounterState[3])
                        {
                            case NOT_STARTED:
                                HandleGameObject(_malGanisGate2GUID, true);
                                break;
                            case IN_PROGRESS:
                                HandleGameObject(_malGanisGate2GUID, false);
                                break;
                            case DONE:
                                HandleGameObject(_exitGateGUID, true);
                                if (GameObject* go = instance->GetGameObject(_malGanisChestGUID))
                                    go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                                break;
                        }
                        break;
                    case DATA_INFINITE_EVENT:
                        _encounterState[4] = data;
                        break;
                    case DATA_CRATE_COUNT:
                        _crateCount = data;
                        if (_crateCount == 5)
                        {
                            if (Creature* bunny = instance->GetCreature(_genericBunnyGUID))
                                bunny->CastSpell(bunny, SPELL_CRATES_CREDIT, true);

                            // Summon Chromie and global whisper
                            if (Creature* chromie = instance->SummonCreature(NPC_CHROMIE_2, ChromieSummonPos))
                                if (!instance->GetPlayers().isEmpty())
                                    if (Player* player = instance->GetPlayers().getFirst()->getSource())
                                        sCreatureTextMgr->SendChat(chromie, SAY_CRATES_COMPLETED, player->GetGUID(), CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_MAP);
                        }
                        DoUpdateWorldState(WorldStates::WORLDSTATE_CRATES_REVEALED, _crateCount);
                        break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_MEATHOOK_EVENT:
                        return _encounterState[0];
                    case DATA_SALRAMM_EVENT:
                        return _encounterState[1];
                    case DATA_EPOCH_EVENT:
                        return _encounterState[2];
                    case DATA_MAL_GANIS_EVENT:
                        return _encounterState[3];
                    case DATA_INFINITE_EVENT:
                        return _encounterState[4];
                    case DATA_CRATE_COUNT:
                        return _crateCount;
                }
                return 0;
            }

            ObjectGuid GetGuidData(uint32 identifier) const
            {
                switch (identifier)
                {
                    case DATA_ARTHAS:
                        return _arthasGUID;
                    case DATA_MEATHOOK:
                        return _meathookGUID;
                    case DATA_SALRAMM:
                        return _salrammGUID;
                    case DATA_EPOCH:
                        return _epochGUID;
                    case DATA_MAL_GANIS:
                        return _malGanisGUID;
                    case DATA_INFINITE:
                        return _infiniteGUID;
                    case DATA_SHKAF_GATE:
                        return _shkafGateGUID;
                    case DATA_MAL_GANIS_GATE_1:
                        return _malGanisGate1GUID;
                    case DATA_MAL_GANIS_GATE_2:
                        return _malGanisGate2GUID;
                    case DATA_EXIT_GATE:
                        return _exitGateGUID;
                    case DATA_MAL_GANIS_CHEST:
                        return _malGanisChestGUID;
                }
                return ObjectGuid::Empty;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "C S " << _encounterState[0] << ' ' << _encounterState[1] << ' '
                    << _encounterState[2] << ' ' << _encounterState[3] << ' ' << _encounterState[4];

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* in)
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;
                uint16 data0, data1, data2, data3, data4;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4;

                if (dataHead1 == 'C' && dataHead2 == 'S')
                {
                    _encounterState[0] = data0;
                    _encounterState[1] = data1;
                    _encounterState[2] = data2;
                    _encounterState[3] = data3;
                    _encounterState[4] = data4;

                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                        if (_encounterState[i] == IN_PROGRESS)
                            _encounterState[i] = NOT_STARTED;

                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            ObjectGuid _arthasGUID;
            ObjectGuid _meathookGUID;
            ObjectGuid _salrammGUID;
            ObjectGuid _epochGUID;
            ObjectGuid _malGanisGUID;
            ObjectGuid _infiniteGUID;
            ObjectGuid _shkafGateGUID;
            ObjectGuid _malGanisGate1GUID;
            ObjectGuid _malGanisGate2GUID;
            ObjectGuid _exitGateGUID;
            ObjectGuid _malGanisChestGUID;
            ObjectGuid _genericBunnyGUID;
            uint32 _encounterState[MAX_ENCOUNTER];
            uint32 _crateCount;
        };
};

void AddSC_instance_culling_of_stratholme()
{
    new instance_culling_of_stratholme();
}

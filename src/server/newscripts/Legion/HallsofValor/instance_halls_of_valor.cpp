/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

DoorData const doorData[] =
{
    {GO_HYMDALL_ENTER_DOOR,         DATA_HYMDALL,         DOOR_TYPE_ROOM,       BOUNDARY_NONE},
    {GO_HYMDALL_EXIT_DOOR,          DATA_HYMDALL,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {GO_HYRJA_DOOR,                 DATA_HYRJA,           DOOR_TYPE_ROOM,       BOUNDARY_NONE},
    {GO_GATES_OF_GLORY_DOOR,        DATA_HYRJA,           DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {GO_GATES_OF_GLORY_DOOR,        DATA_FENRYR,          DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
};

class instance_halls_of_valor : public InstanceMapScript
{
public:
    instance_halls_of_valor() : InstanceMapScript("instance_halls_of_valor", 1477) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_halls_of_valor_InstanceMapScript(map);
    }

    struct instance_halls_of_valor_InstanceMapScript : public InstanceScript
    {
        instance_halls_of_valor_InstanceMapScript(Map* map) : InstanceScript(map) {}

        ObjectGuid feryrGUID;
        ObjectGuid skovaldGUID;
        ObjectGuid aegisGUID;
        ObjectGuid OdynGUID;
        ObjectGuid OdynChestGUID;

        std::map<uint32, ObjectGuid> runicBrandGUIDconteiner;
        
        bool onInitEnterState;

        void Initialize()
        {
            SetBossNumber(MAX_ENCOUNTER);
            LoadDoorData(doorData);

            feryrGUID.Clear();
            OdynGUID.Clear();
            aegisGUID.Clear();
            OdynChestGUID.Clear();

            onInitEnterState = false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_BOSS_FENRYR:    
                    feryrGUID = creature->GetGUID();
                    break;
                case NPC_GOD_KING_SKOVALD:
                    skovaldGUID = creature->GetGUID();
                    break;
                case NPC_AEGIS_OF_AGGRAMAR:
                    aegisGUID = creature->GetGUID();
                    break;
                case NPC_ODYN:
                    OdynGUID = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_HYMDALL_ENTER_DOOR:
                case GO_HYMDALL_EXIT_DOOR:
                case GO_HYRJA_DOOR:
                case GO_GATES_OF_GLORY_DOOR:
                    AddDoor(go, true);
                    break;
                case GO_ODYN_CHEST:
                    OdynChestGUID = go->GetGUID();
                    break;
                case GO_RUNIC_BRAND_PURE:
                    GoRunicColour[0] = go->GetEntry();
                    runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                    break;
                case GO_RUNIC_BRAND_RED:
                    GoRunicColour[1] = go->GetEntry();
                    runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                    break;
                case GO_RUNIC_BRAND_YELLOW:
                    GoRunicColour[2] = go->GetEntry();
                    runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                    break;
                case GO_RUNIC_BRAND_BLUE:
                    GoRunicColour[3] = go->GetEntry();
                    runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                    break;
                case GO_RUNIC_BRAND_GREEN:
                    GoRunicColour[4] = go->GetEntry();
                    runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                    break;
                default:
                    break;
            }
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
                case DATA_FENRYR_EVENT:
                {
                    if (state == DONE)
                        if (Creature* fen = instance->GetCreature(feryrGUID))
                        {
                            fen->SetVisible(true);
                            fen->SetReactState(REACT_AGGRESSIVE);
                        }
                    break;
                }
                case DATA_SKOVALD:
                {
                    if (state != IN_PROGRESS)
                        if (Creature* aegis = instance->GetCreature(aegisGUID))
                            aegis->DespawnOrUnsummon();
                    if (state == DONE)
                        DoEventCreatures();
                    break;
                }
                case DATA_ODYN:
                {
                    if (state != IN_PROGRESS)
                        for (uint8 i = 0; i < 5; i++)
                            if (GameObject* rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[i]]))
                                rune->SetGoState(GO_STATE_READY);
                    if (state == DONE)
                    {
                        if (GameObject* chest = instance->GetGameObject(OdynChestGUID))
                            chest->SetRespawnTime(86400);

                        instance->SummonCreature(NPC_SPOILS_CHEST_VISUAL, spoilsPos);

                        Map::PlayerList const& players = instance->GetPlayers();
                        if (!players.isEmpty())
                        {
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* pPlayer = itr->getSource())
                                    instance->ToInstanceMap()->PermBindAllPlayers(pPlayer);
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_RUNES_ACTIVATED:
                    if (GameObject* rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[data]]))
                        rune->SetGoState(GO_STATE_ACTIVE);
                    break;
                case DATA_RUNES_DEACTIVATED:
                    if (GameObject* rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[data]]))
                        rune->SetGoState(GO_STATE_READY);
                    if (Creature* odyn = instance->GetCreature(OdynGUID))
                        odyn->AI()->SetData(1, data);
                    break;
                default:
                    break;
            }
        }

        ObjectGuid GetGuidData(uint32 type) const
        {
            switch (type)
            {
                case DATA_SKOVALD:
                    return skovaldGUID;
                case DATA_ODYN:   
                    return OdynGUID;
            }
            return ObjectGuid::Empty;
        }

        uint32 GetData(uint32 type) const
        {
            return 0;
        }

        void OnPlayerEnter(Player* player)
        {
            if (onInitEnterState)
                return;

            onInitEnterState = true;

            DoEventCreatures();
        }

        void DoEventCreatures()
        {
            if (GetBossState(DATA_SKOVALD) == DONE && GetBossState(DATA_ODYN) != DONE)
                if (Creature* odyn = instance->GetCreature(OdynGUID))
                    odyn->AI()->DoAction(true);
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "H V " << GetBossSaveData();

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

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2;

            if (dataHead1 == 'H' && dataHead2 == 'V')
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
                    SetBossState(i, EncounterState(tmpState));
                }

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        uint32 GoRunicColour[5];
        
        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_halls_of_valor()
{
    new instance_halls_of_valor();
}
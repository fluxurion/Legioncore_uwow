/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "vault_of_the_wardens.h"

DoorData const doorData[] =
{
    {GO_SALTHERIL_DOOR_1,       DATA_SALTHERIL,        DOOR_TYPE_ROOM,          BOUNDARY_NONE},
    {GO_SALTHERIL_DOOR_2,       DATA_SALTHERIL,        DOOR_TYPE_PASSAGE,       BOUNDARY_NONE},
    {GO_SALTHERIL_DOOR_3,       DATA_SALTHERIL,        DOOR_TYPE_PASSAGE,       BOUNDARY_NONE},
    {GO_SALTHERIL_DOOR_4,       DATA_SALTHERIL,        DOOR_TYPE_PASSAGE,       BOUNDARY_NONE},
    {GO_SALTHERIL_DOOR_5,       DATA_SALTHERIL,        DOOR_TYPE_PASSAGE,       BOUNDARY_NONE},
    {GO_TORMENTORUM_DOOR_1,     DATA_TORMENTORUM,      DOOR_TYPE_PASSAGE,       BOUNDARY_NONE},
    {GO_ASHGOLM_PLATFORM,       DATA_ASHGOLM,          DOOR_TYPE_ROOM,          BOUNDARY_NONE},
    {GO_ASHGOLM_BRIDGE,         DATA_ASHGOLM,          DOOR_TYPE_SPAWN_HOLE,    BOUNDARY_NONE},
    {GO_GLAZER_BRIDGE,          DATA_GLAZER,           DOOR_TYPE_SPAWN_HOLE,    BOUNDARY_NONE},
    {GO_CORDANA_DOOR,           DATA_CORDANA,          DOOR_TYPE_ROOM,          BOUNDARY_NONE},
};

class instance_vault_of_the_wardens : public InstanceMapScript
{
public:
    instance_vault_of_the_wardens() : InstanceMapScript("instance_vault_of_the_wardens", 1493) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_vault_of_the_wardens_InstanceMapScript(map);
    }

    struct instance_vault_of_the_wardens_InstanceMapScript : public InstanceScript
    {
        instance_vault_of_the_wardens_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        std::map<uint32, ObjectGuid> tormCageContainer;
        std::list<ObjectGuid> ashSysContainer;

        void Initialize()
        {
            LoadDoorData(doorData);
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_COUNTERMEASURES:
                    ashSysContainer.push_back(creature->GetGUID());
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_SALTHERIL_DOOR_1:
                case GO_SALTHERIL_DOOR_2:
                case GO_SALTHERIL_DOOR_3:
                case GO_SALTHERIL_DOOR_4:
                case GO_SALTHERIL_DOOR_5:
                case GO_TORMENTORUM_DOOR_1:
                case GO_ASHGOLM_PLATFORM:
                case GO_ASHGOLM_BRIDGE:
                case GO_GLAZER_BRIDGE:
                case GO_CORDANA_DOOR:
                    AddDoor(go, true);
                    break;
                default:
                    break;
            }

            if (go->GetEntry() >= GO_CAGE_1 && go->GetEntry() <= GO_CAGE_12)
                tormCageContainer[go->GetEntry()] = go->GetGUID();
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
                case DATA_TORMENTORUM:
                {
                    if (state == NOT_STARTED)
                    {
                        for (std::map<uint32, ObjectGuid>::iterator itr = tormCageContainer.begin(); itr != tormCageContainer.end(); ++itr)
                            if (GameObject* cage = instance->GetGameObject(itr->second))
                                cage->SetGoState(GO_STATE_READY);
                    }
                    break;
                }
            }
            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_ASHGOLM_SYSTEM:
                {
                    if (data == IN_PROGRESS)
                    {
                        for (std::list<ObjectGuid>::iterator itr = ashSysContainer.begin(); itr != ashSysContainer.end(); ++itr)
                            if (Creature* system = instance->GetCreature(*itr))
                                system->AI()->DoAction(true);
                    }
                    break;
                }
                default:
                    break;
            }
        }

        /* ObjectGuid GetGuidData(uint32 type) const
        {
            switch (type)
            {
                case NPC_:   
                    return GUID;
            }
            return ObjectGuid::Empty;
        } */

        uint32 GetData(uint32 type) const
        {
            /* switch (type)
            {
                case 0:
                    return 0;
            } */
            return 0;
        }

        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_vault_of_the_wardens()
{
    new instance_vault_of_the_wardens();
}
/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

DoorData const doorData[] =
{
    {GO_HARBARON_DOOR,      DATA_HARBARON,       DOOR_TYPE_PASSAGE,       BOUNDARY_NONE},
};

class instance_maw_of_souls : public InstanceMapScript
{
public:
    instance_maw_of_souls() : InstanceMapScript("instance_maw_of_souls", 1492) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_maw_of_souls_InstanceMapScript(map);
    }

    struct instance_maw_of_souls_InstanceMapScript : public InstanceScript
    {
        instance_maw_of_souls_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        ObjectGuid YmironGornGUID;
        ObjectGuid helyaGUID;
        ObjectGuid shipGUID;
        ObjectGuid helyaChestGUID;

        void Initialize()
        {
            LoadDoorData(doorData);

            YmironGornGUID.Clear();
            helyaGUID.Clear();
            shipGUID.Clear();
            helyaChestGUID.Clear();
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_HELYA:    
                    helyaGUID = creature->GetGUID(); 
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_YMIRON_GORN:
                    YmironGornGUID = go->GetGUID();
                    break;
                case GO_SHIP:
                    shipGUID = go->GetGUID();
                    break;
                case GO_HELYA_CHEST:
                    helyaChestGUID = go->GetGUID();
                case GO_HARBARON_DOOR:
                    AddDoor(go, true);
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
                case DATA_HELYA:
                {
                    if (state == DONE)
                        if (GameObject* chest = instance->GetGameObject(helyaChestGUID))
                            chest->SetRespawnTime(86400);
                    break;
                }
                default:
                    break;
            }
            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            /*switch (type)
            {
                default:
                    break;
            }*/
        }

        ObjectGuid GetGuidData(uint32 type) const
        {
            switch (type)
            {
                case DATA_YMIRON_GORN:   
                    return YmironGornGUID;
                case DATA_HELYA:
                    return helyaGUID;
                case DATA_SHIP:
                    return shipGUID;
            }
            return ObjectGuid::Empty;
        }

        uint32 GetData(uint32 type) const
        {
            return 0;
        }

        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_maw_of_souls()
{
    new instance_maw_of_souls();
}
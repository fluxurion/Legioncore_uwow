/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {0,                   0,                  DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
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
        instance_halls_of_valor_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        void Initialize()
        {
            LoadDoorData(doorData);
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;
            
            return true;
        }

        void OnGameObjectCreate(GameObject* go)
        {
            /* switch (go->GetEntry())
            {
                case GO_DOOR:
                    AddDoor(go, true);
                    break;
                default:
                    break;
            } */
        }

        void OnCreatureCreate(Creature* creature)
        {
            /* switch (creature->GetEntry())
            {
                case NPC_:    
                    GUID = creature->GetGUID(); 
                    break;
            } */
        }

        void SetData(uint32 type, uint32 data)
        {
            /*switch (type)
            {
                default:
                    break;
            }*/
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
            return 0;
        }

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
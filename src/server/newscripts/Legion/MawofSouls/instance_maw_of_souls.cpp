/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "maw_of_souls.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {0,                   0,                  DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
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

void AddSC_instance_maw_of_souls()
{
    new instance_maw_of_souls();
}
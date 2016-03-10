/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "eye_of_azshara.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {0,                   0,                  DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
};

class instance_eye_of_azshara : public InstanceMapScript
{
public:
    instance_eye_of_azshara() : InstanceMapScript("instance_eye_of_azshara", 1456) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_eye_of_azshara_InstanceMapScript(map);
    }

    struct instance_eye_of_azshara_InstanceMapScript : public InstanceScript
    {
        instance_eye_of_azshara_InstanceMapScript(Map* map) : InstanceScript(map) 
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

void AddSC_instance_eye_of_azshara()
{
    new instance_eye_of_azshara();
}
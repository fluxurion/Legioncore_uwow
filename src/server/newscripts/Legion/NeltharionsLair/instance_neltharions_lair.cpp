/*
    http://uwow.biz
    Dungeon : Neltharions Lair 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "neltharions_lair.h"

DoorData const doorData[] =
{
    {GO_ROKMORA_DOOR,           DATA_ROKMORA,       DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_ULAROGG_DOOR,           DATA_ULAROGG,       DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_NARAXAS_DOOR,           DATA_NARAXAS,       DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_NARAXAS_EXIT_DOOR_1,    DATA_NARAXAS,       DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_NARAXAS_EXIT_DOOR_2,    DATA_NARAXAS,       DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
};

class instance_neltharions_lair : public InstanceMapScript
{
public:
    instance_neltharions_lair() : InstanceMapScript("instance_neltharions_lair", 1458) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_neltharions_lair_InstanceMapScript(map);
    }

    struct instance_neltharions_lair_InstanceMapScript : public InstanceScript
    {
        instance_neltharions_lair_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        void Initialize()
        {
            LoadDoorData(doorData);
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

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_ROKMORA_DOOR:
                case GO_ULAROGG_DOOR:
                case GO_NARAXAS_DOOR:
                case GO_NARAXAS_EXIT_DOOR_1:
                case GO_NARAXAS_EXIT_DOOR_2:
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

void AddSC_instance_neltharions_lair()
{
    new instance_neltharions_lair();
}
/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "darkheart_thicket.h"

DoorData const doorData[] =
{
    //{GO_,       DATA_,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {GO_GLAIDALIS_FIRE_DOOR,     DATA_GLAIDALIS,       DOOR_TYPE_ROOM,       BOUNDARY_NONE},
    {GO_GLAIDALIS_INVIS_DOOR,    DATA_GLAIDALIS,       DOOR_TYPE_ROOM,       BOUNDARY_NONE},
};

class instance_darkheart_thicket : public InstanceMapScript
{
public:
    instance_darkheart_thicket() : InstanceMapScript("instance_darkheart_thicket", 1466) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_darkheart_thicket_InstanceMapScript(map);
    }

    struct instance_darkheart_thicket_InstanceMapScript : public InstanceScript
    {
        instance_darkheart_thicket_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        ObjectGuid MalfurionGUID;
        ObjectGuid MalfurionCageGUID;

        void Initialize()
        {
            LoadDoorData(doorData);

            MalfurionGUID.Clear();
            MalfurionCageGUID.Clear();
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;
            
            return true;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_MALFURION_STORMRAGE:    
                    MalfurionGUID = creature->GetGUID();
                    break;
                case NPC_NIGHTMARE_BINDINGS:
                    MalfurionCageGUID = creature->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_GLAIDALIS_FIRE_DOOR:
                case GO_GLAIDALIS_INVIS_DOOR:
                    AddDoor(go, true);
                    break;
                default:
                    break;
            }
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
                case NPC_MALFURION_STORMRAGE:   
                    return MalfurionGUID;
                case NPC_NIGHTMARE_BINDINGS:
                    return MalfurionCageGUID;
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

void AddSC_instance_darkheart_thicket()
{
    new instance_darkheart_thicket();
}
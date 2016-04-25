/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_arcway.h"

DoorData const doorData[] =
{
    {GO_CORSTILAX_DOOR_2,       DATA_CORSTILAX,       DOOR_TYPE_ROOM,       BOUNDARY_NONE},
};

class instance_the_arcway : public InstanceMapScript
{
public:
    instance_the_arcway() : InstanceMapScript("instance_the_arcway", 1516) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_the_arcway_InstanceMapScript(map);
    }

    struct instance_the_arcway_InstanceMapScript : public InstanceScript
    {
        instance_the_arcway_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        ObjectGuid VandrosGUID;

        bool onInitEnterState;
        uint8 RandTeleport;

        void Initialize()
        {
            LoadDoorData(doorData);

            VandrosGUID.Clear();

            onInitEnterState = false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_ADVISOR_VANDROS:    
                    VandrosGUID = creature->GetGUID(); 
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_CORSTILAX_DOOR_1:
                case GO_CORSTILAX_DOOR_2:
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

            DoEventCreatures();

            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_RAND_TELEPORT:
                    RandTeleport = data;
                    break;
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
            switch (type)
            {
                case DATA_RAND_TELEPORT:
                    return RandTeleport;
            }
            return 0;
        }

        void OnPlayerEnter(Player* player)
        {
            if (onInitEnterState)
                return;

            onInitEnterState = true;

            DoEventCreatures();
        }

        bool checkBossDone()
        {
            for (uint8 i = 0; i < DATA_VANDROS; i++)
                if (GetBossState(i) != DONE)
                    return false;

            return true;
        }

        void DoEventCreatures()
        {
            if (!checkBossDone())
                return;

            if (Creature* vandros = instance->GetCreature(VandrosGUID))
            {
                //vandros->AI()->ZoneTalk();
                vandros->SetReactState(REACT_AGGRESSIVE);
                vandros->SetVisible(true);
            }
        }

        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_the_arcway()
{
    new instance_the_arcway();
}
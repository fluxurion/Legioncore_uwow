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
        instance_halls_of_valor_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        ObjectGuid feryrGUID;
        ObjectGuid OdynGUID;

        void Initialize()
        {
            LoadDoorData(doorData);

            feryrGUID.Clear();
            OdynGUID.Clear();
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_BOSS_FENRYR:    
                    feryrGUID = creature->GetGUID(); 
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
                case DATA_ODYN:   
                    return OdynGUID;
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

void AddSC_instance_halls_of_valor()
{
    new instance_halls_of_valor();
}
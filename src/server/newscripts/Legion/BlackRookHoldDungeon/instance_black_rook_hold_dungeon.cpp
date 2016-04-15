/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_rook_hold_dungeon.h"

DoorData const doorData[] =
{
    {GO_AMALGAM_DOOR_EXIT,      DATA_AMALGAM,       DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_AMALGAM_DOOR_1,         DATA_AMALGAM,       DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_AMALGAM_DOOR_2,         DATA_AMALGAM,       DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_AMALGAM_DOOR_3,         DATA_AMALGAM,       DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_AMALGAM_DOOR_4,         DATA_AMALGAM,       DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_ILLYSANNA_DOOR_1,       DATA_ILLYSANNA,     DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_ILLYSANNA_DOOR_2,       DATA_ILLYSANNA,     DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_ILLYSANNA_DOOR_3,       DATA_ILLYSANNA,     DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_SMASHSPITE_DOOR,        DATA_SMASHSPITE,    DOOR_TYPE_ROOM,         BOUNDARY_NONE},
    {GO_SMASH_KURT_DOOR_1,      DATA_SMASHSPITE,    DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_SMASH_KURT_DOOR_2,      DATA_SMASHSPITE,    DOOR_TYPE_PASSAGE,      BOUNDARY_NONE},
    {GO_KURTALOS_DOOR,          DATA_KURTALOS,      DOOR_TYPE_ROOM,         BOUNDARY_NONE},
};

class instance_black_rook_hold_dungeon : public InstanceMapScript
{
public:
    instance_black_rook_hold_dungeon() : InstanceMapScript("instance_black_rook_hold_dungeon", 1501) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_black_rook_hold_dungeon_InstanceMapScript(map);
    }

    struct instance_black_rook_hold_dungeon_InstanceMapScript : public InstanceScript
    {
        instance_black_rook_hold_dungeon_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_ENCOUNTER);
        }

        uint8 KurtalosState;

        void Initialize()
        {
            LoadDoorData(doorData);
            KurtalosState = 0;
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
                case GO_AMALGAM_DOOR_1:
                case GO_AMALGAM_DOOR_2:
                case GO_AMALGAM_DOOR_3:
                case GO_AMALGAM_DOOR_4:
                case GO_AMALGAM_DOOR_EXIT:
                case GO_ILLYSANNA_DOOR_1:
                case GO_ILLYSANNA_DOOR_2:
                case GO_ILLYSANNA_DOOR_3:
                case GO_SMASHSPITE_DOOR:
                case GO_SMASH_KURT_DOOR_1:
                case GO_SMASH_KURT_DOOR_2:
                case GO_KURTALOS_DOOR:
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
            switch (type)
            {
                case DATA_KURTALOS_STATE:
                    KurtalosState = data;
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
                case DATA_KURTALOS_STATE:
                    return KurtalosState;
            }
            return 0;
        }

        /* void Update(uint32 diff) 
        {
            // Challenge
            InstanceScript::Update(diff);
        } */
    };
};

void AddSC_instance_black_rook_hold_dungeon()
{
    new instance_black_rook_hold_dungeon();
}
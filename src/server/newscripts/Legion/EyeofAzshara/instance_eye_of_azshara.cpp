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

        ObjectGuid AzsharaGUID;
        ObjectGuid NagasContainerGUID[4];

        uint8 NagasCount;
        uint32 WindsTimer;

        bool onInitEnterState;

        void Initialize()
        {
            LoadDoorData(doorData);

            AzsharaGUID.Clear();

            for (uint8 i = 0; i < 4; i++)
                NagasContainerGUID[i].Clear();

            NagasCount = 0;
            WindsTimer = urand(1, 90) * IN_MILLISECONDS;
            onInitEnterState = false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_WRATH_OF_AZSHARA:    
                    AzsharaGUID = creature->GetGUID(); 
                    break;
                case NPC_MYSTIC_SSAVEH:
                case NPC_RITUALIST_LESHA:
                case NPC_CHANNELER_VARISZ:
                case NPC_BINDER_ASHIOI:
                    NagasContainerGUID[NagasCount] = creature->GetGUID();
                    NagasCount++;
                    break;
            }
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

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            DoEventCreatures();

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

        bool checkBossDone()
        {
            for (uint8 i = 0; i < DATA_WRATH_OF_AZSHARA; i++)
                if (GetBossState(i) != DONE)
                    return false;

            return true;
        }

        void DoEventCreatures()
        {
            if (!checkBossDone())
                return;

            for (uint8 i = 0; i < 4; i++)
                if (Creature* naga = instance->GetCreature(NagasContainerGUID[i]))
                    naga->AI()->DoAction(true);
        }

        void OnPlayerEnter(Player* player)
        {
            if (onInitEnterState)
                return;

            onInitEnterState = true;

            DoEventCreatures();
        }

        void Update(uint32 diff) 
        {
            // Challenge
            //InstanceScript::Update(diff);
            
            if (WindsTimer <= diff)
            {
                Map::PlayerList const& players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                        player->CastSpell(player, 191797, true); //Violent Winds
                }
                WindsTimer = 90 * IN_MILLISECONDS;
            }
            else WindsTimer -= diff;
        }
    };
};

void AddSC_instance_eye_of_azshara()
{
    new instance_eye_of_azshara();
}
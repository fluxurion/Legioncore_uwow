/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
*/

#ifndef VAULT_OF_THE_WARDENS_H_
#define VAULT_OF_THE_WARDENS_H_

enum eData
{
    DATA_SALTHERIL          = 0,
    DATA_TORMENTORUM        = 1,
    DATA_ASHGOLM            = 2,
    DATA_GLAZER             = 3,
    DATA_CORDANA            = 4,
    MAX_ENCOUNTER,

    DATA_ASHGOLM_SYSTEM,
};

enum eCreatures
{
    //Inquisitor
    NPC_SHADOWMOON_WARLOCK      = 99704, //Orks
    NPC_FELGUARD_ANNIHILATOR    = 99644, //Orks
    NPC_SHADOWMOON_TECHNICIAN   = 99645, //Orks
    NPC_ENORMOUS_STONE_QUILEN   = 99675, //Mogu
    NPC_MOGUSHAN_SECRET_KEEPER  = 99676, //Mogu
    NPC_VOID_TOUCHED_JUGGERNAUT = 99655, //Void
    NPC_DERANGED_MINDFLAYER     = 99657, //Void
    NPC_FACELESS_VOIDCASTER     = 99651, //Void
    NPC_LINGERING_CORRUPTION    = 99678, //Corruption

    //Ashgolm
    NPC_EMBER                   = 99233,
    NPC_COUNTERMEASURES         = 99240,

    //Glazer
    NPC_GLAZER                  = 95887,
    NPC_GLAZER_BEAM             = 102310,
    NPC_LENS                    = 98082,
    NPC_BEAM_TARGET_TRIGGER     = 98118,

    //Cordana
    NPC_GLOWING_SENTRY          = 100525,
    NPC_AVATAR_OF_SHADOW        = 104293,
};

enum eGameObjects
{
    GO_SALTHERIL_DOOR_1         = 246055,
    GO_SALTHERIL_DOOR_2         = 246054,
    GO_SALTHERIL_DOOR_3         = 246053,
    GO_SALTHERIL_DOOR_4         = 247081,
    GO_SALTHERIL_DOOR_5         = 247082,
    GO_TORMENTORUM_DOOR_1       = 246112,
    GO_ASHGOLM_PLATFORM         = 246418,
    GO_ASHGOLM_BRIDGE           = 246057,
    GO_GLAZER_BRIDGE            = 246727,
    GO_CORDANA_DOOR             = 246049,

    //Ashgolm
    GO_ASHGOLM_VOLCANO          = 247010,

    //Torment cages
    GO_CAGE_1                   = 246506, //Orks
    GO_CAGE_2                   = 246507,
    GO_CAGE_3                   = 246508,
    GO_CAGE_4                   = 246509, //Mogu
    GO_CAGE_5                   = 246510,
    GO_CAGE_6                   = 246511,
    GO_CAGE_7                   = 246512, //Void
    GO_CAGE_8                   = 246513,
    GO_CAGE_9                   = 246514,
    GO_CAGE_10                  = 246515, //Corruptions
    GO_CAGE_11                  = 246516,
    GO_CAGE_12                  = 246517,
};

#endif
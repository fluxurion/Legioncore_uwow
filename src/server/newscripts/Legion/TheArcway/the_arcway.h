/*
    http://uwow.biz
    Dungeon : The Arcway 100-110
*/

#ifndef THE_ARCWAY_H_
#define THE_ARCWAY_H_

enum eData
{
    DATA_IVANYR         = 0,
    DATA_CORSTILAX      = 1,
    DATA_XAKAL          = 2,
    DATA_NALTIRA        = 3,
    DATA_VANDROS        = 4,
    MAX_ENCOUNTER,

    DATA_RAND_TELEPORT,
};

enum eCreatures
{
    //Corstilax
    NPC_SUPPRESSION_PROTOCOL    = 99628,
    NPC_QUARANTINE              = 99560,
    NPC_PIPE_STALKER            = 99446,
    //Xakal
    NPC_FISSURE                 = 100342,
    NPC_DREAD_FELBAT            = 100393,
    //Vandros
    NPC_ADVISOR_VANDROS         = 98208, //102849
};

enum eGameObjects
{
    GO_CORSTILAX_DOOR_1         = 245968,
    GO_CORSTILAX_DOOR_2         = 246024,
    GO_XAKAL_FISSURE            = 245606,
};

#endif
/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
*/

#ifndef BLACK_ROOK_HOLD_DUNGEON_H_
#define BLACK_ROOK_HOLD_DUNGEON_H_

enum eData
{
    DATA_AMALGAM        = 0,
    DATA_ILLYSANNA      = 1,
    DATA_SMASHSPITE     = 2,
    DATA_KURTALOS       = 3,
    MAX_ENCOUNTER,
};

enum eCreatures
{
    NPC_SOUL_ECHOES_STALKER     = 99090,
    //Illysanna
    NPC_EYE_BEAM_STALKER        = 100436,
    NPC_RISEN_ARCANIST          = 98280,
    NPC_SOUL_TORN_VANGUARD      = 100485,
    //Smashspite
    NPC_FEL_BAT                 = 100759,
};

enum eGameObjects
{
    GO_AMALGAM_DOOR_1           = 247403,
    GO_AMALGAM_DOOR_2           = 247404,
    GO_AMALGAM_DOOR_3           = 247405,
    GO_AMALGAM_DOOR_4           = 247406,
    GO_AMALGAM_DOOR_EXIT        = 247407,
    GO_ILLYSANNA_DOOR_1         = 247401,
    GO_ILLYSANNA_DOOR_2         = 247399,
    GO_ILLYSANNA_DOOR_3         = 247400,
    GO_SMASHSPITE_DOOR          = 247402,
};

#endif
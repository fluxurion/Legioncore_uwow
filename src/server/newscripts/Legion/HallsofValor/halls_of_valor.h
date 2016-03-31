/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
*/

#ifndef HALLS_OF_VALOR_H_
#define HALLS_OF_VALOR_H_

enum eData
{
    DATA_HYMDALL        = 0,
    DATA_HYRJA          = 1,
    DATA_FENRYR         = 2,
    DATA_SKOVALD        = 3,
    DATA_ODYN           = 4,
    MAX_ENCOUNTER,

    DATA_HYRJA_EVENT_COMPLETE,
    DATA_FENRYR_EVENT,
};

enum eCreatures
{
    //Hymdall summons
    NPC_STORM_DRAKE             = 97788,
    NPC_STATIC_FIELD            = 97795,
    NPC_DANCING_BLADE           = 97960,
    //Hyrja
    NPC_OLMYR_THE_ENLIGHTENED   = 97202,
    NPC_SOLSTEN                 = 97219,
    //Fenryr
    NPC_BOSS_FENRYR             = 99868,
    NPC_FENRYR                  = 95674,
    //Skovald
    NPC_KING_HALDOR             = 95843,
    NPC_KING_BJORN              = 97081,
    NPC_KING_RANULF             = 97083,
    NPC_KING_TOR                = 97084,
    //Odyn
    NPC_ODYN                    = 95676,
};

enum eGameObjects
{
    GO_HYMDALL_ENTER_DOOR   = 245700,
    GO_HYMDALL_EXIT_DOOR    = 245701,
    GO_HYRJA_DOOR           = 245702,
    GO_TRACES_OF_FENRIR     = 246272,
    GO_GATES_OF_GLORY_DOOR  = 244664,
    GO_BRIDGE_OF_GLORY      = 246144,
};

#endif
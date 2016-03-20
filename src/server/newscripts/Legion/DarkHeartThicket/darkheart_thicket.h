/*
    http://uwow.biz
    Dungeon : Dark Heart Thicket 100-110
*/

#ifndef DARKHEART_THICKET_H_
#define DARKHEART_THICKET_H_

enum eData
{
    DATA_GLAIDALIS         = 0,
    DATA_OAKHEART          = 1,
    DATA_DRESARON          = 2,
    DATA_XAVIUS            = 3,
    MAX_ENCOUNTER,
};

enum eCreatures
{
    //Xavius summons
    NPC_NIGHTMARE_BINDINGS      = 101329, //Malfurion cage
    NPC_MALFURION_STORMRAGE     = 100652,
    
    //Pre-event Glaydalis
    NPC_DRUIDIC_PRESERVER       = 100403,
};

enum eGameObjects
{
    GO_GLAIDALIS_FIRE_DOOR      = 246910,
    GO_GLAIDALIS_INVIS_DOOR     = 246905,
};

#endif
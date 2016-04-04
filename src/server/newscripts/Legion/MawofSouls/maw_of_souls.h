/*
    http://uwow.biz
    Dungeon : Maw of Souls 100-110
*/

#ifndef MAW_OF_SOULS_H_
#define MAW_OF_SOULS_H_

enum eData
{
    DATA_YMIRON         = 0,
    DATA_HARBARON       = 1,
    DATA_HELYA          = 2,
    MAX_ENCOUNTER,

    DATA_YMIRON_GORN,
    DATA_SHIP,
};

enum eCreatures
{
    //Harbaron summons
    NPC_SOUL_FRAGMENT           = 98761,
    NPC_COSMIC_SCYTHE           = 98989,
    NPC_COSMIC_SCYTHE_2         = 100839,
    //Helya
    NPC_HELYA                   = 96759,
    //Helya summons
    NPC_DESTRUCTOR_TENTACLE     = 99803,  //veh - 99801
    NPC_GRASPING_TENTACLE_1     = 99800,  //veh right 1 98363,
    NPC_GRASPING_TENTACLE_2     = 100361, //veh left 1  100362,
    NPC_GRASPING_TENTACLE_3     = 100359, //veh left 2  100360,
    NPC_GRASPING_TENTACLE_4     = 100353, //veh right 2 100354,
    NPC_SWIRLING_POOL_VEH       = 97099,  //veh - создает АТ и и суммонит тентаклю.
    NPC_PIERCING_TENTACLE       = 100188, //Появляется, бьет 197112 и ломает пол, исчезает.
    NPC_SWIRLING_POOL_TRIG      = 102306, //Плюеется из отверстий водой
    
    NPC_YMIRON                  = 96756,
    //Ymiron trash
    NPC_BIRD                    = 97163,
    NPC_SHIELDMAIDEN            = 102104,
    NPC_RUNECARVER              = 102375,
};

enum eGameObjects
{
    GO_YMIRON_GORN      = 247041,
    GO_HARBARON_DOOR    = 246947,
    GO_SHIP             = 246919,
    GO_HELYA_CHEST      = 246036,

    GO_SWIRLING_POOL_1 = 246834,
    GO_SWIRLING_POOL_2 = 246835,
    GO_SWIRLING_POOL_3 = 246838,
    GO_SWIRLING_POOL_4 = 246839,
    GO_SWIRLING_POOL_5 = 246840,
    GO_SWIRLING_POOL_6 = 246842,
    GO_SWIRLING_POOL_7 = 246843,
    GO_SWIRLING_POOL_8 = 246844, 
};

enum eStateShip
{
    STATE_DESTROY       = 0,
    STATE_REBUILDING    = 1,
};

#endif
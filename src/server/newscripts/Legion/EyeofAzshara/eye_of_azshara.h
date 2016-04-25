/*
    http://uwow.biz
    Dungeon : Eye of Azshara 100-110
*/

#ifndef EYE_OF_AZSHARA_H_
#define EYE_OF_AZSHARA_H_

enum eData
{
    DATA_PARJESH                = 0,
    DATA_HATECOIL               = 1,
    DATA_DEEPBEARD              = 2,
    DATA_SERPENTRIX             = 3,
    DATA_WRATH_OF_AZSHARA       = 4,
    MAX_ENCOUNTER,
};

enum eCreatures
{
    //Hatecoil
    NPC_SAND_DUNE               = 97853,
    //Deepbeard
    NPC_QUAKE                   = 97916,
    //Serpentrix
    NPC_BLAZING_HYDRA_SPAWN     = 97259,
    //Wrath of Azshara
    NPC_WRATH_OF_AZSHARA        = 96028,
    NPC_WEATHERMAN              = 97063,
    NPC_LIGHTNING_STALKER       = 97713,
    NPC_MYSTIC_SSAVEH           = 98173, 
    NPC_RITUALIST_LESHA         = 100248,
    NPC_CHANNELER_VARISZ        = 100249,
    NPC_BINDER_ASHIOI           = 100250,
    NPC_ARCANE_BOMB             = 97691,
};

enum eGameObjects
{
    GO_SAND_DUNE                = 244690,
};

#endif
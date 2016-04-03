/*
* Copyright (C) 2012-2016 Uwow <https://uwow.biz/>
* Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PetBattle_h__
#define PetBattle_h__

#include "BattlePetPackets.h"

enum MoveType
{
    MOVE_TYPE_NONE                  = 0,  // forfeit, sent together with CMSG_PET_BATTLE_QUIT_NOTIFY
    MOVE_TYPE_USE_ABILITY           = 1,
    MOVE_TYPE_PET_SWAP              = 2,
    MOVE_TYPE_USE_TRAP              = 3,
    MOVE_TYPE_REJECTION_FROM_BATTLE = 4,
};

#endif // PetBattle_h__

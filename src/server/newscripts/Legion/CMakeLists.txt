# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

if (USE_SCRIPTPCH)
  set(Legion_STAT_PCH_HDR PrecompiledHeaders/LegionPCH.h)
  set(Legion_STAT_PCH_SRC PrecompiledHeaders/LegionPCH.cpp)
endif ()

set(Legion_STAT_SRCS
  ${Legion_STAT_SRCS}
  mardum.cpp
  warden_prison.cpp
  
  BlackRookHoldDungeon/instance_black_rook_hold_dungeon.cpp
  BlackRookHoldDungeon/boss_the_amalgam_of_souls.cpp
  BlackRookHoldDungeon/boss_illysanna_ravencrest.cpp
  BlackRookHoldDungeon/boss_smashspite_the_hateful.cpp
  BlackRookHoldDungeon/boss_lord_kurtalos_ravencrest.cpp

  DarkHeartThicket/instance_darkheart_thicket.cpp
  DarkHeartThicket/boss_arch_druid_glaidalis.cpp
  DarkHeartThicket/boss_oakheart.cpp
  DarkHeartThicket/boss_dresaron.cpp
  DarkHeartThicket/boss_shade_of_xavius.cpp

  EyeofAzshara/instance_eye_of_azshara.cpp
  EyeofAzshara/boss_warlord_parjesh.cpp
  EyeofAzshara/boss_lady_hatecoil.cpp
  EyeofAzshara/boss_king_deepbeard.cpp
  EyeofAzshara/boss_serpentrix.cpp
  EyeofAzshara/boss_wrath_of_azshara.cpp

  HallsofValor/instance_halls_of_valor.cpp
  HallsofValor/boss_odyn.cpp
  HallsofValor/boss_hyrja.cpp
  HallsofValor/boss_hymdall.cpp
  HallsofValor/boss_fenryr.cpp
  HallsofValor/boss_god_king_skovald.cpp

  MawofSouls/instance_maw_of_souls.cpp
  MawofSouls/boss_ymiron_the_fallen_king.cpp
  MawofSouls/boss_harbaron.cpp
  MawofSouls/boss_helya.cpp

  NeltharionsLair/instance_neltharions_lair.cpp
  NeltharionsLair/boss_naraxas.cpp
  NeltharionsLair/boss_rokmora.cpp
  NeltharionsLair/boss_ularogg_cragshaper.cpp
  NeltharionsLair/boss_dargrul_the_underking.cpp

  TheArcway/instance_the_arcway.cpp
  TheArcway/boss_ivanyr.cpp
  TheArcway/boss_corstilax.cpp
  TheArcway/boss_general_xakal.cpp
  TheArcway/boss_naltira.cpp
  TheArcway/boss_advisor_vandros.cpp

  VaultoftheWardens/instance_vault_of_the_wardens.cpp
  VaultoftheWardens/boss_tirathon_saltheril.cpp
  VaultoftheWardens/boss_inquisitor_tormentorum.cpp
  VaultoftheWardens/boss_ashgolm.cpp
  VaultoftheWardens/boss_glazer.cpp
  VaultoftheWardens/boss_cordana.cpp

  VioletHoldLegion/instance_violet_hold_legion.cpp
  VioletHoldLegion/boss_mindflayer_kaahrj.cpp
  VioletHoldLegion/boss_millificent_manastorm.cpp
  VioletHoldLegion/boss_festerface.cpp
  VioletHoldLegion/boss_shivermaw.cpp
  VioletHoldLegion/boss_anubesset.cpp
  VioletHoldLegion/boss_saelorn.cpp
  VioletHoldLegion/boss_blood_princess_thalena.cpp
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/cppformat
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour/Include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast/Include
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/ipc
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Management
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/game/Accounts
  ${CMAKE_SOURCE_DIR}/src/server/game/Anticheat
  ${CMAKE_SOURCE_DIR}/src/server/game/Achievements
  ${CMAKE_SOURCE_DIR}/src/server/game/Addons
  ${CMAKE_SOURCE_DIR}/src/server/game/AI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/CoreAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/ScriptedAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/SmartScripts
  ${CMAKE_SOURCE_DIR}/src/server/game/AuctionHouse
  ${CMAKE_SOURCE_DIR}/src/server/game/Battlefield
  ${CMAKE_SOURCE_DIR}/src/server/game/Battlefield/Zones
  ${CMAKE_SOURCE_DIR}/src/server/game/Battlegrounds
  ${CMAKE_SOURCE_DIR}/src/server/game/Battlegrounds/Zones
  ${CMAKE_SOURCE_DIR}/src/server/game/BattlePet
  ${CMAKE_SOURCE_DIR}/src/server/game/BattlePay
  ${CMAKE_SOURCE_DIR}/src/server/game/Calendar
  ${CMAKE_SOURCE_DIR}/src/server/game/Challenge
  ${CMAKE_SOURCE_DIR}/src/server/game/Chat
  ${CMAKE_SOURCE_DIR}/src/server/game/Chat/Channels
  ${CMAKE_SOURCE_DIR}/src/server/game/Conditions
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/game/Combat
  ${CMAKE_SOURCE_DIR}/src/server/game/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/game/DungeonFinding
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/AreaTrigger
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Conversation
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Corpse
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Creature
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/DynamicObject
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Item
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Item/Container
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/GameObject
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object/Updates
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Pet
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Player
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Transport
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Unit
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Vehicle
  ${CMAKE_SOURCE_DIR}/src/server/game/Events
  ${CMAKE_SOURCE_DIR}/src/server/game/Globals
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids/Cells
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids/Notifiers
  ${CMAKE_SOURCE_DIR}/src/server/game/Groups
  ${CMAKE_SOURCE_DIR}/src/server/game/Guilds
  ${CMAKE_SOURCE_DIR}/src/server/game/Handlers
  ${CMAKE_SOURCE_DIR}/src/server/game/Instances
  ${CMAKE_SOURCE_DIR}/src/server/game/LookingForGroup
  ${CMAKE_SOURCE_DIR}/src/server/game/Loot
  ${CMAKE_SOURCE_DIR}/src/server/game/Mails
  ${CMAKE_SOURCE_DIR}/src/server/game/Miscellaneous
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/MovementGenerators
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/Spline
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/Waypoints
  ${CMAKE_SOURCE_DIR}/src/server/game/Opcodes
  ${CMAKE_SOURCE_DIR}/src/server/game/OutdoorPvP
  ${CMAKE_SOURCE_DIR}/src/server/game/Pools
  ${CMAKE_SOURCE_DIR}/src/server/game/PrecompiledHeaders
  ${CMAKE_SOURCE_DIR}/src/server/game/Quests
  ${CMAKE_SOURCE_DIR}/src/server/game/Reputation
  ${CMAKE_SOURCE_DIR}/src/server/game/Scenarios  
  ${CMAKE_SOURCE_DIR}/src/server/game/Scripting
  ${CMAKE_SOURCE_DIR}/src/server/game/Server
  ${CMAKE_SOURCE_DIR}/src/server/game/Server/Protocol
  ${CMAKE_SOURCE_DIR}/src/server/game/Skills
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/Auras
  ${CMAKE_SOURCE_DIR}/src/server/game/Texts
  ${CMAKE_SOURCE_DIR}/src/server/game/Tickets
  ${CMAKE_SOURCE_DIR}/src/server/game/Tools
  ${CMAKE_SOURCE_DIR}/src/server/game/Warden
  ${CMAKE_SOURCE_DIR}/src/server/game/Warden/Modules
  ${CMAKE_SOURCE_DIR}/src/server/game/Weather
  ${CMAKE_SOURCE_DIR}/src/server/game/World
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_library(Legion STATIC
  ${Legion_STAT_SRCS}
  ${Legion_STAT_PCH_SRC}
)

message("  -> Prepared: Legion")

# Generate precompiled header
if (USE_SCRIPTPCH)
  add_cxx_pch(Legion ${Legion_STAT_PCH_HDR} ${Legion_STAT_PCH_SRC})
endif()

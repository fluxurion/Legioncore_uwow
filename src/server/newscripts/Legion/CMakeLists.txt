# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Legion/mardum.cpp
  
  Legion/BlackRookHoldDungeon/instance_black_rook_hold_dungeon.cpp
  Legion/BlackRookHoldDungeon/boss_the_amalgam_of_souls.cpp
  Legion/BlackRookHoldDungeon/boss_illysanna_ravencrest.cpp
  Legion/BlackRookHoldDungeon/boss_smashspite_the_hateful.cpp
  Legion/BlackRookHoldDungeon/boss_lord_kurtalos_ravencrest.cpp

  Legion/DarkHeartThicket/instance_darkheart_thicket.cpp
  Legion/DarkHeartThicket/boss_arch_druid_glaidalis.cpp
  Legion/DarkHeartThicket/boss_oakheart.cpp
  Legion/DarkHeartThicket/boss_dresaron.cpp
  Legion/DarkHeartThicket/boss_shade_of_xavius.cpp

  Legion/EyeofAzshara/instance_eye_of_azshara.cpp
  Legion/EyeofAzshara/boss_warlord_parjesh.cpp
  Legion/EyeofAzshara/boss_lady_hatecoil.cpp
  Legion/EyeofAzshara/boss_king_deepbeard.cpp
  Legion/EyeofAzshara/boss_serpentrix.cpp
  Legion/EyeofAzshara/boss_wrath_of_azshara.cpp

  Legion/HallsofValor/instance_halls_of_valor.cpp
  Legion/HallsofValor/boss_odyn.cpp
  Legion/HallsofValor/boss_hyrja.cpp
  Legion/HallsofValor/boss_hymdall.cpp
  Legion/HallsofValor/boss_fenryr.cpp
  Legion/HallsofValor/boss_god_king_skovald.cpp

  Legion/MawofSouls/instance_maw_of_souls.cpp
  Legion/MawofSouls/boss_ymiron_the_fallen_king.cpp
  Legion/MawofSouls/boss_harbaron.cpp
  Legion/MawofSouls/boss_helya.cpp

  Legion/NeltharionsLair/instance_neltharions_lair.cpp
  Legion/NeltharionsLair/boss_naraxas.cpp
  Legion/NeltharionsLair/boss_rokmora.cpp
  Legion/NeltharionsLair/boss_ularogg_cragshaper.cpp
  Legion/NeltharionsLair/boss_dargrul_the_underking.cpp

  Legion/TheArcway/instance_the_arcway.cpp
  Legion/TheArcway/boss_ivanyr.cpp
  Legion/TheArcway/boss_corstilax.cpp
  Legion/TheArcway/boss_general_xakal.cpp
  Legion/TheArcway/boss_naltira.cpp
  Legion/TheArcway/boss_advisor_vandros.cpp

  Legion/VaultoftheWardens/instance_vault_of_the_wardens.cpp
  Legion/VaultoftheWardens/boss_tirathon_saltheril.cpp
  Legion/VaultoftheWardens/boss_inquisitor_tormentorum.cpp
  Legion/VaultoftheWardens/boss_ashgolm.cpp
  Legion/VaultoftheWardens/boss_glazer.cpp
  Legion/VaultoftheWardens/boss_cordana.cpp

  Legion/VioletHoldLegion/instance_violet_hold_legion.cpp
  Legion/VioletHoldLegion/boss_mindflayer_kaahrj.cpp
  Legion/VioletHoldLegion/boss_millificent_manastorm.cpp
  Legion/VioletHoldLegion/boss_festerface.cpp
  Legion/VioletHoldLegion/boss_shivermaw.cpp
  Legion/VioletHoldLegion/boss_anubesset.cpp
  Legion/VioletHoldLegion/boss_saelorn.cpp
  Legion/VioletHoldLegion/boss_blood_princess_thalena.cpp
)

message("  -> Prepared: Legion")

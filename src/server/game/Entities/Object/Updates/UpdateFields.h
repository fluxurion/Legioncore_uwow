/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef _UPDATEFIELDS_H
#define _UPDATEFIELDS_H

#define DYNAMICOBJECT_FIELD_BYTES DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID

#define UNIT_FIELD_BYTES_0 UNIT_FIELD_SEX
#define UNIT_FIELD_BYTES_1 UNIT_FIELD_ANIM_TIER
#define UNIT_FIELD_BYTES_2 UNIT_FIELD_SHAPESHIFT_FORM

#define PLAYER_FIELD_BYTES PLAYER_FIELD_MAX_ARTIFACT_POWER_RANKS // can be wrong
#define PLAYER_FIELD_BYTES2 PLAYER_FIELD_OVERRIDE_SPELLS_ID
#define PLAYER_BYTES PLAYER_FIELD_HAIR_COLOR_ID
#define PLAYER_BYTES_2 PLAYER_FIELD_CUSTOM_DISPLAY_OPTION
#define PLAYER_BYTES_3 PLAYER_FIELD_INEBRIATION

#define GAMEOBJECT_FIELD_BYTES_1 GAMEOBJECT_FIELD_PERCENT_HEALTH
#define GAMEOBJECT_DYNAMIC GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID

#define CORPSE_FIELD_BYTES_1 CORPSE_FIELD_SKIN_ID
#define CORPSE_FIELD_BYTES_2 CORPSE_FIELD_FACIAL_HAIR_STYLE_ID

// Updatefields auto generated for version 6, 1, 2, 19802
enum EObjectFields
{
    OBJECT_FIELD_GUID = 0x0, // Size: 4 Flags: Public
    OBJECT_FIELD_DATA = 0x4, // Size: 4 Flags: Public
    OBJECT_FIELD_TYPE = 0x8, // Size: 1 Flags: Public
    OBJECT_FIELD_ENTRY_ID = 0x9, // Size: 1 Flags: Dynamic
    OBJECT_FIELD_DYNAMIC_FLAGS = 0xA, // Size: 1 Flags: Dynamic, Urgent
    OBJECT_FIELD_SCALE = 0xB, // Size: 1 Flags: Public
    OBJECT_END = 0xC
};

enum EObjectDynamicFields
{
    OBJECT_DYNAMIC_END = 0x0,
};

enum EUnitFields
{
    UNIT_FIELD_CHARM = OBJECT_END + 0x0, // Size: 4 Flags: Public
    UNIT_FIELD_SUMMON = OBJECT_END + 0x4, // Size: 4 Flags: Public
    UNIT_FIELD_CRITTER = OBJECT_END + 0x8, // Size: 4 Flags: Private
    UNIT_FIELD_CHARMED_BY = OBJECT_END + 0xC, // Size: 4 Flags: Public
    UNIT_FIELD_SUMMONED_BY = OBJECT_END + 0x10, // Size: 4 Flags: Public
    UNIT_FIELD_CREATED_BY = OBJECT_END + 0x14, // Size: 4 Flags: Public
    UNIT_FIELD_DEMON_CREATOR = OBJECT_END + 0x18, // Size: 4 Flags: Public
    UNIT_FIELD_TARGET = OBJECT_END + 0x1C, // Size: 4 Flags: Public
    UNIT_FIELD_BATTLE_PET_COMPANION_GUID = OBJECT_END + 0x20, // Size: 4 Flags: Public
    UNIT_FIELD_BATTLE_PET_DBID = OBJECT_END + 0x24, // Size: 2 Flags: Public
    UNIT_FIELD_CHANNEL_OBJECT = OBJECT_END + 0x26, // Size: 4 Flags: Public, Urgent
    UNIT_FIELD_CHANNEL_SPELL = OBJECT_END + 0x2A, // Size: 1 Flags: Public, Urgent
    UNIT_FIELD_CHANNEL_SPELL_XSPELL_VISUAL = OBJECT_END + 0x2B, // Size: 1 Flags: Public, Urgent
    UNIT_FIELD_SUMMONED_BY_HOME_REALM = OBJECT_END + 0x2C, // Size: 1 Flags: Public
    UNIT_FIELD_SEX = OBJECT_END + 0x2D, // Size: 1 Flags: Public
    UNIT_FIELD_DISPLAY_POWER = OBJECT_END + 0x2E, // Size: 1 Flags: Public
    UNIT_FIELD_OVERRIDE_DISPLAY_POWER_ID = OBJECT_END + 0x2F, // Size: 1 Flags: Public
    UNIT_FIELD_HEALTH = OBJECT_END + 0x30, // Size: 1 Flags: Public
    UNIT_FIELD_POWER = OBJECT_END + 0x31, // Size: 6 Flags: Public, UrgentSelfOnly
    UNIT_FIELD_MAX_HEALTH = OBJECT_END + 0x37, // Size: 1 Flags: Public
    UNIT_FIELD_MAX_POWER = OBJECT_END + 0x38, // Size: 6 Flags: Public
    UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER = OBJECT_END + 0x3E, // Size: 6 Flags: Private, Owner, UnitAll
    UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER = OBJECT_END + 0x44, // Size: 6 Flags: Private, Owner, UnitAll
    UNIT_FIELD_LEVEL = OBJECT_END + 0x4A, // Size: 1 Flags: Public
    UNIT_FIELD_EFFECTIVE_LEVEL = OBJECT_END + 0x4B, // Size: 1 Flags: Public
    UNIT_FIELD_SCALING_LEVEL_MIN = OBJECT_END + 0x4C, // Size: 1 Flags: Public
    UNIT_FIELD_SCALING_LEVEL_MAX = OBJECT_END + 0x4D, // Size: 1 Flags: Public
    UNIT_FIELD_SCALING_LEVEL_DELTA = OBJECT_END + 0x4E, // Size: 1 Flags: Public
    UNIT_FIELD_FACTION_TEMPLATE = OBJECT_END + 0x4F, // Size: 1 Flags: Public
    UNIT_FIELD_VIRTUAL_ITEM_ID = OBJECT_END + 0x50, // Size: 6 Flags: Public
    UNIT_FIELD_FLAGS = OBJECT_END + 0x56, // Size: 1 Flags: Public
    UNIT_FIELD_FLAGS_2 = OBJECT_END + 0x57, // Size: 1 Flags: Public
    UNIT_FIELD_FLAGS_3 = OBJECT_END + 0x58, // Size: 1 Flags: Public
    UNIT_FIELD_AURA_STATE = OBJECT_END + 0x59, // Size: 1 Flags: Public
    UNIT_FIELD_ATTACK_ROUND_BASE_TIME = OBJECT_END + 0x5A, // Size: 2 Flags: Public
    UNIT_FIELD_RANGED_ATTACK_ROUND_BASE_TIME = OBJECT_END + 0x5C, // Size: 1 Flags: Private
    UNIT_FIELD_BOUNDING_RADIUS = OBJECT_END + 0x5D, // Size: 1 Flags: Public
    UNIT_FIELD_COMBAT_REACH = OBJECT_END + 0x5E, // Size: 1 Flags: Public
    UNIT_FIELD_DISPLAY_ID = OBJECT_END + 0x5F, // Size: 1 Flags: Dynamic, Urgent
    UNIT_FIELD_NATIVE_DISPLAY_ID = OBJECT_END + 0x60, // Size: 1 Flags: Public, Urgent
    UNIT_FIELD_MOUNT_DISPLAY_ID = OBJECT_END + 0x61, // Size: 1 Flags: Public, Urgent
    UNIT_FIELD_MIN_DAMAGE = OBJECT_END + 0x62, // Size: 1 Flags: Private, Owner, SpecialInfo
    UNIT_FIELD_MAX_DAMAGE = OBJECT_END + 0x63, // Size: 1 Flags: Private, Owner, SpecialInfo
    UNIT_FIELD_MIN_OFF_HAND_DAMAGE = OBJECT_END + 0x64, // Size: 1 Flags: Private, Owner, SpecialInfo
    UNIT_FIELD_MAX_OFF_HAND_DAMAGE = OBJECT_END + 0x65, // Size: 1 Flags: Private, Owner, SpecialInfo
    UNIT_FIELD_ANIM_TIER = OBJECT_END + 0x66, // Size: 1 Flags: Public
    UNIT_FIELD_PET_NUMBER = OBJECT_END + 0x67, // Size: 1 Flags: Public
    UNIT_FIELD_PET_NAME_TIMESTAMP = OBJECT_END + 0x68, // Size: 1 Flags: Public
    UNIT_FIELD_PET_EXPERIENCE = OBJECT_END + 0x69, // Size: 1 Flags: Owner
    UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE = OBJECT_END + 0x6A, // Size: 1 Flags: Owner
    UNIT_FIELD_MOD_CASTING_SPEED = OBJECT_END + 0x6B, // Size: 1 Flags: Public
    UNIT_FIELD_MOD_SPELL_HASTE = OBJECT_END + 0x6C, // Size: 1 Flags: Public
    UNIT_FIELD_MOD_HASTE = OBJECT_END + 0x6D, // Size: 1 Flags: Public
    UNIT_FIELD_MOD_RANGED_HASTE = OBJECT_END + 0x6E, // Size: 1 Flags: Public
    UNIT_FIELD_MOD_HASTE_REGEN = OBJECT_END + 0x6F, // Size: 1 Flags: Public
    UNIT_FIELD_MOD_GLOBAL_COOLDOWN_RATE = OBJECT_END + 0x70, // Size: 1 Flags: Public
    UNIT_FIELD_CREATED_BY_SPELL = OBJECT_END + 0x71, // Size: 1 Flags: Public
    UNIT_FIELD_NPC_FLAGS = OBJECT_END + 0x72, // Size: 2 Flags: Public, Dynamic
    UNIT_FIELD_NPC_FLAGS2 = UNIT_FIELD_NPC_FLAGS + 1,
    UNIT_NPC_EMOTESTATE = OBJECT_END + 0x74, // Size: 1 Flags: Public
    UNIT_FIELD_STATS = OBJECT_END + 0x75, // Size: 5 Flags: Private, Owner
    UNIT_FIELD_STAT_POS_BUFF = OBJECT_END + 0x7A, // Size: 5 Flags: Private, Owner
    UNIT_FIELD_STAT_NEG_BUFF = OBJECT_END + 0x7F, // Size: 5 Flags: Private, Owner
    UNIT_FIELD_RESISTANCES = OBJECT_END + 0x84, // Size: 7 Flags: Private, Owner, SpecialInfo
    UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE = OBJECT_END + 0x8B, // Size: 7 Flags: Private, Owner
    UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE = OBJECT_END + 0x92, // Size: 7 Flags: Private, Owner
    UNIT_FIELD_MOD_BONUS_ARMOR = OBJECT_END + 0x99, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_BASE_MANA = OBJECT_END + 0x9A, // Size: 1 Flags: Public
    UNIT_FIELD_BASE_HEALTH = OBJECT_END + 0x9B, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_SHAPESHIFT_FORM = OBJECT_END + 0x9C, // Size: 1 Flags: Public
    UNIT_FIELD_ATTACK_POWER = OBJECT_END + 0x9D, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_ATTACK_POWER_MOD_POS = OBJECT_END + 0x9E, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_ATTACK_POWER_MOD_NEG = OBJECT_END + 0x9F, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_ATTACK_POWER_MULTIPLIER = OBJECT_END + 0xA0, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_RANGED_ATTACK_POWER = OBJECT_END + 0xA1, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS = OBJECT_END + 0xA2, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG = OBJECT_END + 0xA3, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER = OBJECT_END + 0xA4, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_MIN_RANGED_DAMAGE = OBJECT_END + 0xA5, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_MAX_RANGED_DAMAGE = OBJECT_END + 0xA6, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_POWER_COST_MODIFIER = OBJECT_END + 0xA7, // Size: 7 Flags: Private, Owner
    UNIT_FIELD_POWER_COST_MULTIPLIER = OBJECT_END + 0xAE, // Size: 7 Flags: Private, Owner
    UNIT_FIELD_MAX_HEALTH_MODIFIER = OBJECT_END + 0xB5, // Size: 1 Flags: Private, Owner
    UNIT_FIELD_HOVER_HEIGHT = OBJECT_END + 0xB6, // Size: 1 Flags: Public
    UNIT_FIELD_MIN_ITEM_LEVEL_CUTOFF = OBJECT_END + 0xB7, // Size: 1 Flags: Public
    UNIT_FIELD_MIN_ITEM_LEVEL = OBJECT_END + 0xB8, // Size: 1 Flags: Public
    UNIT_FIELD_MAX_ITEM_LEVEL = OBJECT_END + 0xB9, // Size: 1 Flags: Public
    UNIT_FIELD_WILD_BATTLE_PET_LEVEL = OBJECT_END + 0xBA, // Size: 1 Flags: Public
    UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP = OBJECT_END + 0xBB, // Size: 1 Flags: Public
    UNIT_FIELD_INTERACT_SPELL_ID = OBJECT_END + 0xBC, // Size: 1 Flags: Public
    UNIT_FIELD_STATE_SPELL_VISUAL_ID = OBJECT_END + 0xBD, // Size: 1 Flags: Dynamic, Urgent
    UNIT_FIELD_STATE_ANIM_ID = OBJECT_END + 0xBE, // Size: 1 Flags: Dynamic, Urgent
    UNIT_FIELD_STATE_ANIM_KIT_ID = OBJECT_END + 0xBF, // Size: 1 Flags: Dynamic, Urgent
    UNIT_FIELD_STATE_WORLD_EFFECT_ID = OBJECT_END + 0xC0, // Size: 4 Flags: Dynamic, Urgent
    UNIT_FIELD_SCALE_DURATION = OBJECT_END + 0xC4, // Size: 1 Flags: Public
    UNIT_FIELD_LOOKS_LIKE_MOUNT_ID = OBJECT_END + 0xC5, // Size: 1 Flags: Public
    UNIT_FIELD_LOOKS_LIKE_CREATURE_ID = OBJECT_END + 0xC6, // Size: 1 Flags: Public
    UNIT_FIELD_LOOK_AT_CONTROLLER_ID = OBJECT_END + 0xC7, // Size: 1 Flags: Public
    UNIT_FIELD_LOOK_AT_CONTROLLER_TARGET = OBJECT_END + 0xC8, // Size: 4 Flags: Public
    UNIT_END = OBJECT_END + 0xCC
};

enum EItemFields
{
    ITEM_FIELD_OWNER = OBJECT_END + 0x0, // Size: 4 Flags: Public
    ITEM_FIELD_CONTAINED_IN = OBJECT_END + 0x4, // Size: 4 Flags: Public
    ITEM_FIELD_CREATOR = OBJECT_END + 0x8, // Size: 4 Flags: Public
    ITEM_FIELD_GIFT_CREATOR = OBJECT_END + 0xC, // Size: 4 Flags: Public
    ITEM_FIELD_STACK_COUNT = OBJECT_END + 0x10, // Size: 1 Flags: Owner
    ITEM_FIELD_EXPIRATION = OBJECT_END + 0x11, // Size: 1 Flags: Owner
    ITEM_FIELD_SPELL_CHARGES = OBJECT_END + 0x12, // Size: 5 Flags: Owner
    ITEM_FIELD_DYNAMIC_FLAGS = OBJECT_END + 0x17, // Size: 1 Flags: Public
    ITEM_FIELD_ENCHANTMENT = OBJECT_END + 0x18, // Size: 39 Flags: Public
    ITEM_FIELD_PROPERTY_SEED = OBJECT_END + 0x3F, // Size: 1 Flags: Public
    ITEM_FIELD_RANDOM_PROPERTIES_ID = OBJECT_END + 0x40, // Size: 1 Flags: Public
    ITEM_FIELD_DURABILITY = OBJECT_END + 0x41, // Size: 1 Flags: Owner
    ITEM_FIELD_MAX_DURABILITY = OBJECT_END + 0x42, // Size: 1 Flags: Owner
    ITEM_FIELD_CREATE_PLAYED_TIME = OBJECT_END + 0x43, // Size: 1 Flags: Public
    ITEM_FIELD_MODIFIERS_MASK = OBJECT_END + 0x44, // Size: 1 Flags: Owner
    ITEM_FIELD_CONTEXT = OBJECT_END + 0x45, // Size: 1 Flags: Public
    ITEM_FIELD_ARTIFACT_XP = OBJECT_END + 0x46, // Size: 1 Flags: Owner
    ITEM_FIELD_ITEM_APPEARANCE_MOD_ID = OBJECT_END + 0x47, // Size: 1 Flags: Owner
    ITEM_END = OBJECT_END + 0x48
};

enum EItemDynamicFields
{
    ITEM_DYNAMIC_FIELD_MODIFIERS = OBJECT_DYNAMIC_END + 0x0, // Size: 1 Flags: Owner
    ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS = OBJECT_DYNAMIC_END + 0x1, // Size: 1 Flags: Owner, Unk0x100
    ITEM_DYNAMIC_FIELD_ARTIFACT_POWERS = OBJECT_DYNAMIC_END + 0x2, // Size: 1 Flags: Owner
    ITEM_DYNAMIC_FIELD_GEMS = OBJECT_DYNAMIC_END + 0x3, // Size: 1 Flags: Owner
    ITEM_DYNAMIC_END = OBJECT_DYNAMIC_END + 0x4
};

enum EPlayerFields
{
    PLAYER_FIELD_DUEL_ARBITER = UNIT_END + 0x0, // Size: 4 Flags: Public
    PLAYER_FIELD_WOW_ACCOUNT = UNIT_END + 0x4, // Size: 4 Flags: Public
    PLAYER_FIELD_LOOT_TARGET_GUID = UNIT_END + 0x8, // Size: 4 Flags: Public
    PLAYER_FIELD_PLAYER_FLAGS = UNIT_END + 0xC, // Size: 1 Flags: Public
    PLAYER_FIELD_PLAYER_FLAGS_EX = UNIT_END + 0xD, // Size: 1 Flags: Public
    PLAYER_FIELD_GUILD_RANK_ID = UNIT_END + 0xE, // Size: 1 Flags: Public
    PLAYER_FIELD_GUILD_DELETE_DATE = UNIT_END + 0xF, // Size: 1 Flags: Public
    PLAYER_FIELD_GUILD_LEVEL = UNIT_END + 0x10, // Size: 1 Flags: Public
    PLAYER_FIELD_HAIR_COLOR_ID = UNIT_END + 0x11, // Size: 1 Flags: Public
    PLAYER_FIELD_CUSTOM_DISPLAY_OPTION = UNIT_END + 0x12, // Size: 1 Flags: Public
    PLAYER_FIELD_INEBRIATION = UNIT_END + 0x13, // Size: 1 Flags: Public
    PLAYER_FIELD_ARENA_FACTION = UNIT_END + 0x14, // Size: 1 Flags: Public
    PLAYER_FIELD_DUEL_TEAM = UNIT_END + 0x15, // Size: 1 Flags: Public
    PLAYER_FIELD_GUILD_TIME_STAMP = UNIT_END + 0x16, // Size: 1 Flags: Public
    PLAYER_FIELD_QUEST_LOG = UNIT_END + 0x17, // Size: 750 Flags: PartyMember
    PLAYER_FIELD_VISIBLE_ITEMS = UNIT_END + 0x305, // Size: 38 Flags: Public
    PLAYER_FIELD_PLAYER_TITLE = UNIT_END + 0x32B, // Size: 1 Flags: Public
    PLAYER_FIELD_FAKE_INEBRIATION = UNIT_END + 0x32C, // Size: 1 Flags: Public
    PLAYER_FIELD_VIRTUAL_PLAYER_REALM = UNIT_END + 0x32D, // Size: 1 Flags: Public
    PLAYER_FIELD_CURRENT_SPEC_ID = UNIT_END + 0x32E, // Size: 1 Flags: Public
    PLAYER_FIELD_TAXI_MOUNT_ANIM_KIT_ID = UNIT_END + 0x32F, // Size: 1 Flags: Public
    PLAYER_FIELD_AVG_ITEM_LEVEL = UNIT_END + 0x330, // Size: 4 Flags: Public
    PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY = UNIT_END + 0x334, // Size: 1 Flags: Public
    PLAYER_FIELD_PRESTIGE = UNIT_END + 0x335, // Size: 1 Flags: Public
    PLAYER_FIELD_HONOR_LEVEL = UNIT_END + 0x336, // Size: 1 Flags: Public
    PLAYER_FIELD_INV_SLOTS = UNIT_END + 0x337, // Size: 748 Flags: Private
    PLAYER_END_NOT_SELF = PLAYER_FIELD_INV_SLOTS,
    PLAYER_FIELD_FARSIGHT_OBJECT = UNIT_END + 0x623, // Size: 4 Flags: Private
    PLAYER_FIELD_KNOWN_TITLES = UNIT_END + 0x627, // Size: 12 Flags: Private
    PLAYER_FIELD_COINAGE = UNIT_END + 0x633, // Size: 2 Flags: Private
    PLAYER_FIELD_XP = UNIT_END + 0x635, // Size: 1 Flags: Private
    PLAYER_FIELD_NEXT_LEVEL_XP = UNIT_END + 0x636, // Size: 1 Flags: Private
    PLAYER_FIELD_SKILL = UNIT_END + 0x637, // Size: 448 Flags: Private
    PLAYER_FIELD_CHARACTER_POINTS = UNIT_END + 0x7F7, // Size: 1 Flags: Private
    PLAYER_FIELD_MAX_TALENT_TIERS = UNIT_END + 0x7F8, // Size: 1 Flags: Private
    PLAYER_FIELD_TRACK_CREATURE_MASK = UNIT_END + 0x7F9, // Size: 1 Flags: Private
    PLAYER_FIELD_TRACK_RESOURCE_MASK = UNIT_END + 0x7FA, // Size: 1 Flags: Private
    PLAYER_FIELD_MAINHAND_EXPERTISE = UNIT_END + 0x7FB, // Size: 1 Flags: Private
    PLAYER_FIELD_OFFHAND_EXPERTISE = UNIT_END + 0x7FC, // Size: 1 Flags: Private
    PLAYER_FIELD_RANGED_EXPERTISE = UNIT_END + 0x7FD, // Size: 1 Flags: Private
    PLAYER_FIELD_COMBAT_RATING_EXPERTISE = UNIT_END + 0x7FE, // Size: 1 Flags: Private
    PLAYER_FIELD_BLOCK_PERCENTAGE = UNIT_END + 0x7FF, // Size: 1 Flags: Private
    PLAYER_FIELD_DODGE_PERCENTAGE = UNIT_END + 0x800, // Size: 1 Flags: Private
    PLAYER_FIELD_PARRY_PERCENTAGE = UNIT_END + 0x801, // Size: 1 Flags: Private
    PLAYER_FIELD_CRIT_PERCENTAGE = UNIT_END + 0x802, // Size: 1 Flags: Private
    PLAYER_FIELD_RANGED_CRIT_PERCENTAGE = UNIT_END + 0x803, // Size: 1 Flags: Private
    PLAYER_FIELD_OFFHAND_CRIT_PERCENTAGE = UNIT_END + 0x804, // Size: 1 Flags: Private
    PLAYER_FIELD_SPELL_CRIT_PERCENTAGE = UNIT_END + 0x805, // Size: 1 Flags: Private
    PLAYER_FIELD_SHIELD_BLOCK = UNIT_END + 0x806, // Size: 1 Flags: Private
    PLAYER_FIELD_SHIELD_BLOCK_CRIT_PERCENTAGE = UNIT_END + 0x807, // Size: 1 Flags: Private
    PLAYER_FIELD_MASTERY = UNIT_END + 0x808, // Size: 1 Flags: Private
    PLAYER_FIELD_SPEED = UNIT_END + 0x809, // Size: 1 Flags: Private
    PLAYER_FIELD_LIFESTEAL = UNIT_END + 0x80A, // Size: 1 Flags: Private
    PLAYER_FIELD_AVOIDANCE = UNIT_END + 0x80B, // Size: 1 Flags: Private
    PLAYER_FIELD_STURDINESS = UNIT_END + 0x80C, // Size: 1 Flags: Private
    PLAYER_FIELD_VERSATILITY = UNIT_END + 0x80D, // Size: 1 Flags: Private
    PLAYER_FIELD_VERSATILITY_BONUS = UNIT_END + 0x80E, // Size: 1 Flags: Private
    PLAYER_FIELD_PVP_POWER_DAMAGE = UNIT_END + 0x80F, // Size: 1 Flags: Private
    PLAYER_FIELD_PVP_POWER_HEALING = UNIT_END + 0x810, // Size: 1 Flags: Private
    PLAYER_FIELD_EXPLORED_ZONES = UNIT_END + 0x811, // Size: 256 Flags: Private
    PLAYER_FIELD_REST_INFO = UNIT_END + 0x911, // Size: 4 Flags: Private
    PLAYER_FIELD_MOD_DAMAGE_DONE_POS = UNIT_END + 0x915, // Size: 7 Flags: Private
    PLAYER_FIELD_MOD_DAMAGE_DONE_NEG = UNIT_END + 0x91C, // Size: 7 Flags: Private
    PLAYER_FIELD_MOD_DAMAGE_DONE_PERCENT = UNIT_END + 0x923, // Size: 7 Flags: Private
    PLAYER_FIELD_MOD_HEALING_DONE_POS = UNIT_END + 0x92A, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_HEALING_PERCENT = UNIT_END + 0x92B, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_HEALING_DONE_PERCENT = UNIT_END + 0x92C, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_PERIODIC_HEALING_DONE_PERCENT = UNIT_END + 0x92D, // Size: 1 Flags: Private
    PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS = UNIT_END + 0x92E, // Size: 3 Flags: Private
    PLAYER_FIELD_WEAPON_ATK_SPEED_MULTIPLIERS = UNIT_END + 0x931, // Size: 3 Flags: Private
    PLAYER_FIELD_MOD_SPELL_POWER_PERCENT = UNIT_END + 0x934, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_RESILIENCE_PERCENT = UNIT_END + 0x935, // Size: 1 Flags: Private
    PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_APPERCENT = UNIT_END + 0x936, // Size: 1 Flags: Private
    PLAYER_FIELD_OVERRIDE_APBY_SPELL_POWER_PERCENT = UNIT_END + 0x937, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_TARGET_RESISTANCE = UNIT_END + 0x938, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE = UNIT_END + 0x939, // Size: 1 Flags: Private
    PLAYER_FIELD_LOCAL_FLAGS = UNIT_END + 0x93A, // Size: 1 Flags: Private
    PLAYER_FIELD_MAX_ARTIFACT_POWER_RANKS = UNIT_END + 0x93B, // Size: 1 Flags: Private
    PLAYER_FIELD_SELF_RES_SPELL = UNIT_END + 0x93C, // Size: 1 Flags: Private
    PLAYER_FIELD_PVP_MEDALS = UNIT_END + 0x93D, // Size: 1 Flags: Private
    PLAYER_FIELD_BUYBACK_PRICE = UNIT_END + 0x93E, // Size: 12 Flags: Private
    PLAYER_FIELD_BUYBACK_TIMESTAMP = UNIT_END + 0x94A, // Size: 12 Flags: Private
    PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS = UNIT_END + 0x956, // Size: 1 Flags: Private
    PLAYER_FIELD_LIFETIME_HONORABLE_KILLS = UNIT_END + 0x957, // Size: 1 Flags: Private
    PLAYER_FIELD_WATCHED_FACTION_INDEX = UNIT_END + 0x958, // Size: 1 Flags: Private
    PLAYER_FIELD_COMBAT_RATINGS = UNIT_END + 0x959, // Size: 32 Flags: Private
    PLAYER_FIELD_PVP_INFO = UNIT_END + 0x979, // Size: 36 Flags: Private
    PLAYER_FIELD_MAX_LEVEL = UNIT_END + 0x99D, // Size: 1 Flags: Private
    PLAYER_FIELD_SCALING_PLAYER_LEVEL_DELTA = UNIT_END + 0x99E, // Size: 1 Flags: Private
    PLAYER_FIELD_NO_REAGENT_COST_MASK = UNIT_END + 0x99F, // Size: 4 Flags: Private
    PLAYER_FIELD_PET_SPELL_POWER = UNIT_END + 0x9A3, // Size: 1 Flags: Private
    PLAYER_FIELD_RESEARCHING = UNIT_END + 0x9A4, // Size: 10 Flags: Private
    PLAYER_FIELD_PROFESSION_SKILL_LINE = UNIT_END + 0x9AE, // Size: 2 Flags: Private
    PLAYER_FIELD_UI_HIT_MODIFIER = UNIT_END + 0x9B0, // Size: 1 Flags: Private
    PLAYER_FIELD_UI_SPELL_HIT_MODIFIER = UNIT_END + 0x9B1, // Size: 1 Flags: Private
    PLAYER_FIELD_HOME_REALM_TIME_OFFSET = UNIT_END + 0x9B2, // Size: 1 Flags: Private
    PLAYER_FIELD_MOD_PET_HASTE = UNIT_END + 0x9B3, // Size: 1 Flags: Private
    PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID = UNIT_END + 0x9B4, // Size: 4 Flags: Private
    PLAYER_FIELD_OVERRIDE_SPELLS_ID = UNIT_END + 0x9B8, // Size: 1 Flags: Private, UrgentSelfOnly
    PLAYER_FIELD_LFG_BONUS_FACTION_ID = UNIT_END + 0x9B9, // Size: 1 Flags: Private
    PLAYER_FIELD_LOOT_SPEC_ID = UNIT_END + 0x9BA, // Size: 1 Flags: Private
    PLAYER_FIELD_OVERRIDE_ZONE_PVPTYPE = UNIT_END + 0x9BB, // Size: 1 Flags: Private, UrgentSelfOnly
    PLAYER_FIELD_BAG_SLOT_FLAGS = UNIT_END + 0x9BC, // Size: 4 Flags: Private
    PLAYER_FIELD_BANK_BAG_SLOT_FLAGS = UNIT_END + 0x9C0, // Size: 7 Flags: Private
    PLAYER_FIELD_INSERT_ITEMS_LEFT_TO_RIGHT = UNIT_END + 0x9C7, // Size: 1 Flags: Private
    PLAYER_FIELD_QUEST_COMPLETED = UNIT_END + 0x9C8, // Size: 875 Flags: Private
    PLAYER_FIELD_HONOR = UNIT_END + 0xD33, // Size: 1 Flags: Private
    PLAYER_FIELD_HONOR_NEXT_LEVEL = UNIT_END + 0xD34, // Size: 1 Flags: Private
    PLAYER_END = UNIT_END + 0xD35
};

enum EContainerFields
{
    CONTAINER_FIELD_SLOT_1 = ITEM_END + 0x0, // Size: 144 Flags: Public
    CONTAINER_FIELD_NUM_SLOTS = ITEM_END + 0x90, // Size: 1 Flags: Public
    CONTAINER_END = ITEM_END + 0x91
};

enum ContainerDynamicFields
{
    CONTAINER_DYNAMIC_END = ITEM_DYNAMIC_END + 0x000,
};

enum EGameObjectFields
{
    GAMEOBJECT_FIELD_CREATED_BY = OBJECT_END + 0x0, // Size: 4 Flags: Public
    GAMEOBJECT_FIELD_DISPLAY_ID = OBJECT_END + 0x4, // Size: 1 Flags: Dynamic, Urgent
    GAMEOBJECT_FIELD_FLAGS = OBJECT_END + 0x5, // Size: 1 Flags: Public, Urgent
    GAMEOBJECT_FIELD_PARENT_ROTATION = OBJECT_END + 0x6, // Size: 4 Flags: Public
    GAMEOBJECT_FIELD_FACTION_TEMPLATE = OBJECT_END + 0xA, // Size: 1 Flags: Public
    GAMEOBJECT_FIELD_LEVEL = OBJECT_END + 0xB, // Size: 1 Flags: Public
    GAMEOBJECT_FIELD_PERCENT_HEALTH = OBJECT_END + 0xC, // Size: 1 Flags: Public, Urgent
    GAMEOBJECT_FIELD_SPELL_VISUAL_ID = OBJECT_END + 0xD, // Size: 1 Flags: Public, Dynamic, Urgent
    GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID = OBJECT_END + 0xE, // Size: 1 Flags: Dynamic, Urgent
    GAMEOBJECT_FIELD_STATE_ANIM_ID = OBJECT_END + 0xF, // Size: 1 Flags: Dynamic, Urgent
    GAMEOBJECT_FIELD_STATE_ANIM_KIT_ID = OBJECT_END + 0x10, // Size: 1 Flags: Dynamic, Urgent
    GAMEOBJECT_FIELD_STATE_WORLD_EFFECT_ID = OBJECT_END + 0x11, // Size: 4 Flags: Dynamic, Urgent
    GAMEOBJECT_END = OBJECT_END + 0x15
};

enum GameObjectDynamicFields
{
    GAMEOBJECT_DYNAMIC_END                                 = OBJECT_DYNAMIC_END + 0x000,
};

enum EDynamicObjectFields
{
    DYNAMICOBJECT_FIELD_CASTER = OBJECT_END + 0x0, // Size: 4 Flags: Public
    DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID = OBJECT_END + 0x4, // Size: 1 Flags: Dynamic
    DYNAMICOBJECT_FIELD_SPELL_ID = OBJECT_END + 0x5, // Size: 1 Flags: Public
    DYNAMICOBJECT_FIELD_RADIUS = OBJECT_END + 0x6, // Size: 1 Flags: Public
    DYNAMICOBJECT_FIELD_CAST_TIME = OBJECT_END + 0x7, // Size: 1 Flags: Public
    DYNAMICOBJECT_END = OBJECT_END + 0x8
};

enum DynamicObjectDynamicFields
{
    DYNAMICOBJECT_DYNAMIC_END                              = OBJECT_DYNAMIC_END + 0x000,
};

enum ECorpseFields
{
    CORPSE_FIELD_OWNER = OBJECT_END + 0x0, // Size: 4 Flags: Public
    CORPSE_FIELD_PARTY_GUID = OBJECT_END + 0x4, // Size: 4 Flags: Public
    CORPSE_FIELD_DISPLAY_ID = OBJECT_END + 0x8, // Size: 1 Flags: Public
    CORPSE_FIELD_ITEMS = OBJECT_END + 0x9, // Size: 19 Flags: Public
    CORPSE_FIELD_SKIN_ID = OBJECT_END + 0x1C, // Size: 1 Flags: Public
    CORPSE_FIELD_FACIAL_HAIR_STYLE_ID = OBJECT_END + 0x1D, // Size: 1 Flags: Public
    CORPSE_FIELD_FLAGS = OBJECT_END + 0x1E, // Size: 1 Flags: Public
    CORPSE_FIELD_DYNAMIC_FLAGS = OBJECT_END + 0x1F, // Size: 1 Flags: Dynamic
    CORPSE_FIELD_FACTION_TEMPLATE = OBJECT_END + 0x20, // Size: 1 Flags: Public
    CORPSE_FIELD_CUSTOM_DISPLAY_OPTION = OBJECT_END + 0x21, // Size: 1 Flags: Public
    CORPSE_END = OBJECT_END + 0x22
};

enum CorpseDynamicFields
{
    CORPSE_DYNAMIC_END                                     = OBJECT_DYNAMIC_END + 0x000,
};

enum EAreaTriggerFields
{
    AREATRIGGER_FIELD_OVERRIDE_SCALE_CURVE = OBJECT_END + 0x0, // Size: 7 Flags: Public, Urgent
    AREATRIGGER_FIELD_EXTRA_SCALE_CURVE = OBJECT_END + 0x7, // Size: 7 Flags: Public, Urgent
    AREATRIGGER_FIELD_CASTER = OBJECT_END + 0xE, // Size: 4 Flags: Public
    AREATRIGGER_FIELD_DURATION = OBJECT_END + 0x12, // Size: 1 Flags: Public
    AREATRIGGER_FIELD_TIME_TO_TARGET = OBJECT_END + 0x13, // Size: 1 Flags: Public, Urgent
    AREATRIGGER_FIELD_TIME_TO_TARGET_SCALE = OBJECT_END + 0x14, // Size: 1 Flags: Public, Urgent
    AREATRIGGER_FIELD_TIME_TO_TARGET_EXTRA_SCALE = OBJECT_END + 0x15, // Size: 1 Flags: Public, Urgent
    AREATRIGGER_FIELD_SPELL_ID = OBJECT_END + 0x16, // Size: 1 Flags: Public
    AREATRIGGER_FIELD_SPELL_VISUAL_ID = OBJECT_END + 0x17, // Size: 1 Flags: Dynamic
    AREATRIGGER_FIELD_BOUNDS_RADIUS_2_D = OBJECT_END + 0x18, // Size: 1 Flags: Dynamic, Urgent
    AREATRIGGER_FIELD_DECAL_PROPERTIES_ID = OBJECT_END + 0x19, // Size: 1 Flags: Public
    AREATRIGGER_END = OBJECT_END + 0x1A
};

enum AreaTriggerDynamicFields
{
    AREATRIGGER_DYNAMIC_END                                = OBJECT_DYNAMIC_END + 0x000,
};

enum ESceneObjectFields
{
    SCENEOBJECT_FIELD_SCRIPT_PACKAGE_ID = OBJECT_END + 0x0, // Size: 1 Flags: Public
    SCENEOBJECT_FIELD_RND_SEED_VAL = OBJECT_END + 0x1, // Size: 1 Flags: Public
    SCENEOBJECT_FIELD_CREATED_BY = OBJECT_END + 0x2, // Size: 4 Flags: Public
    SCENEOBJECT_FIELD_SCENE_TYPE = OBJECT_END + 0x6, // Size: 1 Flags: Public
    SCENEOBJECT_END = OBJECT_END + 0x7
};

enum EConversationFields
{
    CONVERSATION_FIELD_LAST_LINE_DURATION = OBJECT_END + 0x0, // Size: 1 Flags: Dynamic
    CONVERSATION_END = OBJECT_END + 0x1
};

enum EUnitDynamicFields
{
    UNIT_DYNAMIC_FIELD_PASSIVE_SPELLS = OBJECT_DYNAMIC_END + 0x0, // Size: 1 Flags: Public, Urgent
    UNIT_DYNAMIC_FIELD_WORLD_EFFECTS = OBJECT_DYNAMIC_END + 0x1, // Size: 1 Flags: Public, Urgent
    UNIT_DYNAMIC_END = OBJECT_DYNAMIC_END + 0x2
};

enum EPlayerDynamicFields
{
    PLAYER_DYNAMIC_FIELD_RESEARCH_SITES = UNIT_DYNAMIC_END + 0x0, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_RESEARCH_SITE_PROGRESS = UNIT_DYNAMIC_END + 0x1, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_DAILY_QUESTS_COMPLETED = UNIT_DYNAMIC_END + 0x2, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_AVAILABLE_QUEST_LINE_XQUEST_IDS = UNIT_DYNAMIC_END + 0x3, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_HEIRLOOMS = UNIT_DYNAMIC_END + 0x4, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_HEIRLOOM_FLAGS = UNIT_DYNAMIC_END + 0x5, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_TOYS = UNIT_DYNAMIC_END + 0x6, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_TRANSMOG = UNIT_DYNAMIC_END + 0x7, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_FIELD_CONDITIONAL_TRANSMOG = UNIT_DYNAMIC_END + 0x8, // Size: 1 Flags: Private
    PLAYER_DYNAMIC_END = UNIT_DYNAMIC_END + 0x9
};

enum EGameObjectDynamicFields
{
    GAMEOBJECTDYNAMIC_FIELD_ENABLE_DOODAD_SETS = OBJECT_DYNAMIC_END + 0x0, // Size: 1 Flags: Public
    GAME_OBJECT_DYNAMIC_END = OBJECT_DYNAMIC_END + 0x1
};

enum EConversationDynamicFields
{
    CONVERSATION_DYNAMIC_FIELD_ACTORS = OBJECT_DYNAMIC_END + 0x0, // Size: 1 Flags: Public
    CONVERSATION_DYNAMIC_FIELD_LINES = OBJECT_DYNAMIC_END + 0x1, // Size: 1 Flags: Unk0x100
    CONVERSATION_DYNAMIC_END = OBJECT_DYNAMIC_END + 0x2
};

#endif // _UPDATEFIELDS_H

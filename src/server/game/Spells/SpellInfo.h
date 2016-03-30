/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef _SPELLINFO_H
#define _SPELLINFO_H

#include "SharedDefines.h"
#include "Util.h"
#include "DBCStructure.h"
#include "DB2Structure.h"
#include "Object.h"

class Unit;
class Player;
class Item;
class Spell;
class SpellInfo;
struct SpellChainNode;
struct SpellTargetPosition;
struct SpellDurationEntry;
struct SpellModifier;
struct SpellRangeEntry;
struct SpellRadiusEntry;
struct SpellEntry;
struct SpellCastTimesEntry;
struct Condition;

#define MAX_SPELL_EFFECTS_DIFF 32
#define MAX_EFFECT_MASK 4294967295
#define MAX_POWERS_FOR_SPELL 4
#define MAX_CREATURE_SPELL_DATA_SLOT 4

enum SpellCastTargetFlags
{
    TARGET_FLAG_NONE            = 0x00000000,
    TARGET_FLAG_UNUSED_1        = 0x00000001,               // not used
    TARGET_FLAG_UNIT            = 0x00000002,               // pguid
    TARGET_FLAG_UNIT_RAID       = 0x00000004,               // not sent, used to validate target (if raid member)
    TARGET_FLAG_UNIT_PARTY      = 0x00000008,               // not sent, used to validate target (if party member)
    TARGET_FLAG_ITEM            = 0x00000010,               // pguid
    TARGET_FLAG_SOURCE_LOCATION = 0x00000020,               // pguid, 3 float
    TARGET_FLAG_DEST_LOCATION   = 0x00000040,               // pguid, 3 float
    TARGET_FLAG_UNIT_ENEMY      = 0x00000080,               // not sent, used to validate target (if enemy)
    TARGET_FLAG_UNIT_ALLY       = 0x00000100,               // not sent, used to validate target (if ally)
    TARGET_FLAG_CORPSE_ENEMY    = 0x00000200,               // pguid
    TARGET_FLAG_UNIT_DEAD       = 0x00000400,               // not sent, used to validate target (if dead creature)
    TARGET_FLAG_GAMEOBJECT      = 0x00000800,               // pguid, used with TARGET_GAMEOBJECT_TARGET
    TARGET_FLAG_TRADE_ITEM      = 0x00001000,               // pguid
    TARGET_FLAG_STRING          = 0x00002000,               // string
    TARGET_FLAG_GAMEOBJECT_ITEM = 0x00004000,               // not sent, used with TARGET_GAMEOBJECT_ITEM_TARGET
    TARGET_FLAG_CORPSE_ALLY     = 0x00008000,               // pguid
    TARGET_FLAG_UNIT_MINIPET    = 0x00010000,               // pguid, used to validate target (if non combat pet)
    TARGET_FLAG_GLYPH_SLOT      = 0x00020000,               // used in glyph spells
    TARGET_FLAG_DEST_TARGET     = 0x00040000,               // sometimes appears with DEST_TARGET spells (may appear or not for a given spell)
    TARGET_FLAG_EXTRA_TARGETS   = 0x00080000,               // uint32 counter, loop { vec3 - screen position (?), guid }, not used so far
    TARGET_FLAG_FOLLOWER        = 0x00100000,               // select follower
    TARGET_FLAG_ITEM_HEIRLOOM   = 0x00200000,               // item
    TARGET_FLAG_GARRISON_MISSION= 0x00400000,               // garrison mission
    TARGET_FLAG_UNIT_PASSENGER  = 0x10000000,               // guessed, used to validate target (if vehicle passenger) flags is custom

    TARGET_FLAG_UNIT_MASK = TARGET_FLAG_UNIT | TARGET_FLAG_UNIT_RAID | TARGET_FLAG_UNIT_PARTY
        | TARGET_FLAG_UNIT_ENEMY | TARGET_FLAG_UNIT_ALLY | TARGET_FLAG_UNIT_DEAD | TARGET_FLAG_UNIT_MINIPET | TARGET_FLAG_UNIT_PASSENGER,
    TARGET_FLAG_GAMEOBJECT_MASK = TARGET_FLAG_GAMEOBJECT | TARGET_FLAG_GAMEOBJECT_ITEM,
    TARGET_FLAG_CORPSE_MASK = TARGET_FLAG_CORPSE_ALLY | TARGET_FLAG_CORPSE_ENEMY,
    TARGET_FLAG_ITEM_MASK = TARGET_FLAG_TRADE_ITEM | TARGET_FLAG_ITEM | TARGET_FLAG_GAMEOBJECT_ITEM,
};

enum SpellTargetSelectionCategories
{
    TARGET_SELECT_CATEGORY_NYI,
    TARGET_SELECT_CATEGORY_DEFAULT,
    TARGET_SELECT_CATEGORY_CHANNEL,
    TARGET_SELECT_CATEGORY_NEARBY,
    TARGET_SELECT_CATEGORY_CONE,
    TARGET_SELECT_CATEGORY_AREA,
    TARGET_SELECT_CATEGORY_BETWEEN,
    TARGET_SELECT_CATEGORY_GOTOMOVE,
    TARGET_SELECT_CATEGORY_THREAD,
};

enum SpellTargetReferenceTypes
{
    TARGET_REFERENCE_TYPE_NONE,
    TARGET_REFERENCE_TYPE_CASTER,
    TARGET_REFERENCE_TYPE_TARGET,
    TARGET_REFERENCE_TYPE_LAST,
    TARGET_REFERENCE_TYPE_SRC,
    TARGET_REFERENCE_TYPE_DEST,
    TARGET_REFERENCE_TYPE_NEAR_DEST,
};

enum SpellTargetObjectTypes
{
    TARGET_OBJECT_TYPE_NONE = 0,
    TARGET_OBJECT_TYPE_SRC,
    TARGET_OBJECT_TYPE_DEST,
    TARGET_OBJECT_TYPE_UNIT,
    TARGET_OBJECT_TYPE_UNIT_AND_DEST,
    TARGET_OBJECT_TYPE_OBJ_AND_DEST,
    TARGET_OBJECT_TYPE_GOBJ,
    TARGET_OBJECT_TYPE_GOBJ_ITEM,
    TARGET_OBJECT_TYPE_ITEM,
    TARGET_OBJECT_TYPE_CORPSE,
    // only for effect target type
    TARGET_OBJECT_TYPE_CORPSE_ENEMY,
    TARGET_OBJECT_TYPE_CORPSE_ALLY,
};

enum SpellTargetCheckTypes
{
    TARGET_CHECK_DEFAULT,
    TARGET_CHECK_ENTRY,
    TARGET_CHECK_ENEMY,
    TARGET_CHECK_ALLY,
    TARGET_CHECK_PARTY,
    TARGET_CHECK_RAID,
    TARGET_CHECK_RAID_CLASS,
    TARGET_CHECK_PASSENGER,
    TARGET_CHECK_SUMMON,
};

enum SpellTargetDirectionTypes
{
    TARGET_DIR_NONE,
    TARGET_DIR_FRONT,
    TARGET_DIR_BACK,
    TARGET_DIR_RIGHT,
    TARGET_DIR_LEFT,
    TARGET_DIR_FRONT_RIGHT,
    TARGET_DIR_BACK_RIGHT,
    TARGET_DIR_BACK_LEFT,
    TARGET_DIR_FRONT_LEFT,
    TARGET_DIR_RANDOM,
    TARGET_DIR_ENTRY,
};

enum SpellEffectImplicitTargetTypes
{
    EFFECT_IMPLICIT_TARGET_NONE = 0,
    EFFECT_IMPLICIT_TARGET_EXPLICIT,
    EFFECT_IMPLICIT_TARGET_CASTER,
};

// Spell clasification
enum SpellSpecificType
{
    SPELL_SPECIFIC_NORMAL                        = 0,
    SPELL_SPECIFIC_SEAL                          = 1,
    SPELL_SPECIFIC_BLESSING                      = 2,
    SPELL_SPECIFIC_AURA                          = 3,
    SPELL_SPECIFIC_STING                         = 4,
    SPELL_SPECIFIC_CURSE                         = 5,
    SPELL_SPECIFIC_ASPECT                        = 6,
    SPELL_SPECIFIC_TRACKER                       = 7,
    SPELL_SPECIFIC_WARLOCK_ARMOR                 = 8,
    SPELL_SPECIFIC_MAGE_ARMOR                    = 9,
    SPELL_SPECIFIC_ELEMENTAL_SHIELD              = 10,
    SPELL_SPECIFIC_MAGE_POLYMORPH                = 11,
    SPELL_SPECIFIC_JUDGEMENT                     = 13,
    SPELL_SPECIFIC_WARLOCK_CORRUPTION            = 17,
    SPELL_SPECIFIC_WELL_FED                      = 18,
    SPELL_SPECIFIC_FOOD                          = 19,
    SPELL_SPECIFIC_DRINK                         = 20,
    SPELL_SPECIFIC_FOOD_AND_DRINK                = 21,
    SPELL_SPECIFIC_PRESENCE                      = 22,
    SPELL_SPECIFIC_CHARM                         = 23,
    SPELL_SPECIFIC_SCROLL                        = 24,
    SPELL_SPECIFIC_MAGE_ARCANE_BRILLANCE         = 25,
    SPELL_SPECIFIC_WARRIOR_ENRAGE                = 26,
    SPELL_SPECIFIC_PRIEST_DIVINE_SPIRIT          = 27,
    SPELL_SPECIFIC_HAND                          = 28,
    SPELL_SPECIFIC_PHASE                         = 29,
    SPELL_SPECIFIC_BANE                          = 30,
    SPELL_SPECIFIC_CHAKRA                        = 31,
};

enum SpellCustomAttributes
{
    SPELL_ATTR0_CU_ENCHANT_PROC                  = 0x00000001,
    SPELL_ATTR0_CU_CONE_BACK                     = 0x00000002,
    SPELL_ATTR0_CU_CONE_LINE                     = 0x00000004,
    SPELL_ATTR0_CU_SHARE_DAMAGE                  = 0x00000008,
    SPELL_ATTR0_CU_NO_INITIAL_THREAT             = 0x00000010,
    SPELL_ATTR0_CU_POSITIVE_FOR_CASTER           = 0x00000020,
    SPELL_ATTR0_CU_AURA_CC                       = 0x00000040,
    SPELL_ATTR0_CU_DIRECT_DAMAGE                 = 0x00000100,
    SPELL_ATTR0_CU_CHARGE                        = 0x00000200,
    SPELL_ATTR0_CU_PICKPOCKET                    = 0x00000400,
    SPELL_ATTR0_CU_NEGATIVE_EFF0                 = 0x00001000,
    SPELL_ATTR0_CU_NEGATIVE_EFF1                 = 0x00002000,
    SPELL_ATTR0_CU_NEGATIVE_EFF2                 = 0x00004000,
    SPELL_ATTR0_CU_IGNORE_ARMOR                  = 0x00008000,
    SPELL_ATTR0_CU_REQ_TARGET_FACING_CASTER      = 0x00010000,
    SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET      = 0x00020000,
    SPELL_ATTR0_CU_NEGATIVE_EFF3                 = 0x00040000,
    SPELL_ATTR0_CU_NEGATIVE_EFF4                 = 0x00080000,
    SPELL_ATTR0_CU_REQ_CASTER_NOT_FRONT_TARGET   = 0x00100000,

    SPELL_ATTR0_CU_NEGATIVE                      = SPELL_ATTR0_CU_NEGATIVE_EFF0 | SPELL_ATTR0_CU_NEGATIVE_EFF1 | SPELL_ATTR0_CU_NEGATIVE_EFF2,
};

enum SpellTypes
{
    SPELL_TYPE_MELEE                       = 0x00000001,
    SPELL_TYPE_RANGE                       = 0x00000002,
    SPELL_TYPE_AOE                         = 0x00000004,
    SPELL_TYPE_CHANELED                    = 0x00000008,
    SPELL_TYPE_AUTOREPEATE                 = 0x00000010,
};

enum SpellAuraDummyType
{
    SPELL_DUMMY_DEFAULT                         = 0,
    SPELL_DUMMY_DAMAGE                          = 1,
    SPELL_DUMMY_CRIT                            = 2,
    SPELL_DUMMY_TIME                            = 3,
    SPELL_DUMMY_PROC                            = 4,
    SPELL_DUMMY_DURATION                        = 5,
};

typedef std::vector<SpellPowerEntry const*> SpellPowerData;
typedef std::unordered_map<int32, int32> SpellPowerCost;

uint32 GetTargetFlagMask(SpellTargetObjectTypes objType);

class SpellImplicitTargetInfo
{
private:
    Targets _target;
public:
    SpellImplicitTargetInfo() {}
    SpellImplicitTargetInfo(uint32 target);

    bool IsArea() const;
    SpellTargetSelectionCategories GetSelectionCategory() const;
    SpellTargetReferenceTypes GetReferenceType() const;
    SpellTargetObjectTypes GetObjectType() const;
    SpellTargetCheckTypes GetCheckType() const;
    SpellTargetDirectionTypes GetDirectionType() const;
    float CalcDirectionAngle() const;

    Targets GetTarget() const;
    uint32 GetExplicitTargetMask(bool& srcSet, bool& dstSet) const;

private:
    struct StaticData
    {
        SpellTargetObjectTypes ObjectType;    // type of object returned by target type
        SpellTargetReferenceTypes ReferenceType; // defines which object is used as a reference when selecting target
        SpellTargetSelectionCategories SelectionCategory;
        SpellTargetCheckTypes SelectionCheckType; // defines selection criteria
        SpellTargetDirectionTypes DirectionType; // direction for cone and dest targets
    };
    static StaticData _data[TOTAL_SPELL_TARGETS];
};

class SpellEffectInfo
{
    SpellInfo const* _spellInfo;
    uint8 EffectIndex;
public:
    uint32    Effect;
    uint32    ApplyAuraName;
    uint32    ApplyAuraPeriod;
    int32     DieSides;
    float     RealPointsPerLevel;
    int32     BasePoints;
    float     PointsPerResource;
    float     Amplitude;
    float     ChainAmplitude;
    float     BonusCoefficient;
    int32     MiscValue;
    int32     MiscValueB;
    Mechanics Mechanic;
    SpellImplicitTargetInfo TargetA;
    SpellImplicitTargetInfo TargetB;
    SpellRadiusEntry const* RadiusEntry;
    uint32    ChainTargets;
    uint32    ItemType;
    uint32    TriggerSpell;
    flag128   SpellClassMask;
    std::list<Condition*>* ImplicitTargetConditions;
    float     SpellAPBonusMultiplier;

    struct ScalingInfo
    {
        float Coefficient;
        float Variance;
        float ResourceCoefficient;
    } Scaling;

    SpellEffectInfo() {}
    SpellEffectInfo(SpellEntry const* spellEntry, SpellInfo const* spellInfo, uint8 effIndex, SpellEffectEntry const* _effect);

    bool IsEffect() const;
    bool IsEffect(SpellEffects effectName) const;
    bool IsAura() const;
    bool IsAura(AuraType aura) const;
    bool IsTargetingArea() const;
    bool IsAreaAuraEffect() const;
    bool IsFarUnitTargetEffect() const;
    bool IsFarDestTargetEffect() const;
    bool IsUnitOwnedAuraEffect() const;

    int32 CalcValue(Unit const* caster = NULL, int32 const* basePoints = NULL, Unit const* target = NULL, Item* m_castitem = NULL, bool lockBasePoints = false, float* variance = nullptr) const;
    int32 CalcBaseValue(int32 value) const;
    float CalcValueMultiplier(Unit* caster, Spell* spell = NULL) const;
    float CalcDamageMultiplier(Unit* caster, Spell* spell = NULL) const;

    bool HasRadius() const;
    float CalcRadius(Unit* caster = NULL, Spell* = NULL) const;

    uint32 GetProvidedTargetMask() const;
    uint32 GetMissingTargetMask(bool srcSet = false, bool destSet = false, uint32 mask = 0) const;

    SpellEffectImplicitTargetTypes GetImplicitTargetType() const;
    SpellTargetObjectTypes GetUsedTargetObjectType() const;

private:
    struct StaticData
    {
        SpellEffectImplicitTargetTypes ImplicitTargetType; // defines what target can be added to effect target list if there's no valid target type provided for effect
        SpellTargetObjectTypes UsedTargetObjectType; // defines valid target object type for spell effect
    };
    static StaticData _data[TOTAL_SPELL_EFFECTS];
};

typedef std::unordered_map<uint16, SpellEffectInfo> SpellEffectInfoMap;
typedef std::unordered_map<uint16, SpellTargetRestrictionsEntry const*> SpellTargetRestrictionsMap;

typedef std::vector<SpellXSpellVisualEntry const*> SpellVisualVector;
typedef std::unordered_map<uint32, SpellVisualVector> SpellVisualMap;

class SpellInfo
{
public:
    uint32 Id;
    uint32 AttributesCu;
    uint32 GeneralTargets;      //old Targets.
    uint32 GeneralTargetCreatureType;  //old TargetCreatureType
    uint32 InterruptFlags;
    uint32 AuraInterruptFlags;
    uint32 ChannelInterruptFlags;
    uint32 MaxLevel;
    uint32 BaseLevel;
    uint32 SpellLevel;
    uint32 RuneCostID;
    SpellRangeEntry const* RangeEntry;
    int32  EquippedItemClass;
    int32  EquippedItemSubClassMask;
    int32  EquippedItemInventoryTypeMask;
    char const* SpellName;
    uint32 MaxTargetLevel;
    uint32 CustomMaxAffectedTargets;      //only if not exist on dbc use it.
    uint32 SpellScalingId;
    uint32 SpellPowerId;
    uint32 ResearchProject;

    struct SpellCooldowns
    {
        uint32 CategoryRecoveryTime;
        uint32 RecoveryTime;
        uint32 StartRecoveryTime;
    } Cooldowns;

    struct SpellClassOptions
    {
        flag128 SpellClassMask;
        uint16 ModalNextSpell;
        uint8 SpellClassSet;
    } ClassOptions;

    struct ScalingInfo
    {
        int32 Class;
        uint32 MaxScalingLevel;
        uint32 ScalesFromItemLevel;
    } Scaling;

    struct SpellCastTimes
    {
        int32 Base;
        int32 Minimum;
        int16 PerLevel;
    } CastTimes;

    struct SpellMisc
    {
        uint32 Attributes[MaxAttributes];
        float Speed;
        float MultistrikeSpeedMod;
        uint16 CastingTimeIndex;
        uint16 DurationIndex;
        uint16 RangeIndex;
        uint16 SpellIconID;
        uint16 ActiveIconID;
        uint8 SchoolMask;
    } Misc;

    struct SpellDuration
    {
        int32 Duration;
        int32 MaxDuration;
        uint16 DurationPerLevel;
    } Duration;

    struct SpellAuraOptions
    {
        uint32 ProcCharges;
        uint32 ProcTypeMask;
        uint32 ProcCategoryRecovery;
        uint16 CumulativeAura;
        uint8 ProcChance;
        uint8 SpellProcsPerMinuteID;
    } AuraOptions;

    struct SpellAuraRescrictions
    {
        uint32 CasterAuraSpell;
        uint32 TargetAuraSpell;
        uint32 ExcludeCasterAuraSpell;
        uint32 ExcludeTargetAuraSpell;
        uint8 CasterAuraState;
        uint8 TargetAuraState;
        uint8 ExcludeCasterAuraState;
        uint8 ExcludeTargetAuraState;
    } AuraRestrictions;

    struct SpellCastingRequirements
    {
        uint16 MinFactionID;
        int16 RequiredAreasID;
        uint16 RequiresSpellFocus;
        uint8 FacingCasterFlags;
        uint8 MinReputation;
        uint8 RequiredAuraVision;
    } CastingReq;

    struct SpellCategories
    {
        uint16 Category;
        uint16 StartRecoveryCategory;
        uint16 ChargeCategory;
        uint8 DefenseType;
        uint8 DispelType;
        uint8 Mechanic;
        uint8 PreventionType;
    } Categories;

    struct SpellCategory
    {
        int32 ChargeRecoveryTime;
        uint8 Flags;
        uint8 UsesPerWeek;
        uint8 MaxCharges;
    } Category;

    struct SpellTotems
    {
        uint32 Totem[MAX_SPELL_TOTEMS];
        uint8 TotemCategory[MAX_SPELL_TOTEMS];
    } Totems;

    struct SpellReagents
    {
        int32 Reagent[MAX_SPELL_REAGENTS];
        uint16 ReagentCount[MAX_SPELL_REAGENTS];
        uint16 CurrencyID;
        uint16 CurrencyCount;
    } Reagents;
    
    struct SpellPowers
    {
        int32 PowerCost;
        float PowerCostPercentage;
        float PowerCostPercentagePerSecond;
        uint32 RequiredAura;
        float HealthCostPercentage;
        uint16 PowerCostPerSecond;
        uint16 ManaCostAdditional;
        uint16 PowerDisplayID;
        uint16 UnitPowerBarID;
        uint8 PowerIndex;
        uint8 PowerType;
        uint8 PowerCostPerLevel;
    } Power;

    struct SpellShapeshifts
    {
        uint64 ShapeshiftMask;
        uint64 ShapeshiftExclude;
    } Shapeshift;

    SpellEffectInfo Effects[MAX_SPELL_EFFECTS];
    SpellEffectInfoMap EffectsMap;
    SpellTargetRestrictionsMap RestrrictionsMap;
    uint32 ExplicitTargetMask;
    SpellChainNode const* ChainEntry;
    SpellPowerEntry spellPower[MAX_POWERS_FOR_SPELL];

    // TalentInfo
    uint32 talentId;

    SpellInfo(SpellEntry const* spellEntry, SpellVisualMap&& visuals);
    ~SpellInfo();

    SpellEffectInfo const* GetEffect(uint8 effect, uint8 difficulty = 0) const;
    bool HasEffect(SpellEffects effect) const;
    bool HasAura(AuraType aura) const;
    bool HasAreaAuraEffect() const;
    bool HasDynAuraEffect() const;
    bool IsMountOrCompanions() const;
    bool IsNotProcSpell() const;
    uint32 GetBattlePetEntry() const;

    inline bool HasAttribute(SpellAttr0 attribute) const { return !!(Misc.Attributes[0] & attribute); }
    inline bool HasAttribute(SpellAttr1 attribute) const { return !!(Misc.Attributes[1] & attribute); }
    inline bool HasAttribute(SpellAttr2 attribute) const { return !!(Misc.Attributes[2] & attribute); }
    inline bool HasAttribute(SpellAttr3 attribute) const { return !!(Misc.Attributes[3] & attribute); }
    inline bool HasAttribute(SpellAttr4 attribute) const { return !!(Misc.Attributes[4] & attribute); }
    inline bool HasAttribute(SpellAttr5 attribute) const { return !!(Misc.Attributes[5] & attribute); }
    inline bool HasAttribute(SpellAttr6 attribute) const { return !!(Misc.Attributes[6] & attribute); }
    inline bool HasAttribute(SpellAttr7 attribute) const { return !!(Misc.Attributes[7] & attribute); }
    inline bool HasAttribute(SpellAttr8 attribute) const { return !!(Misc.Attributes[8] & attribute); }
    inline bool HasAttribute(SpellAttr9 attribute) const { return !!(Misc.Attributes[9] & attribute); }
    inline bool HasAttribute(SpellAttr10 attribute) const { return !!(Misc.Attributes[10] & attribute); }
    inline bool HasAttribute(SpellAttr11 attribute) const { return !!(Misc.Attributes[11] & attribute); }
    inline bool HasAttribute(SpellAttr12 attribute) const { return !!(Misc.Attributes[12] & attribute); }
    inline bool HasAttribute(SpellAttr13 attribute) const { return !!(Misc.Attributes[13] & attribute); }
    inline bool HasAttribute(SpellCustomAttributes customAttribute) const { return !!(AttributesCu & customAttribute); }

    bool IsExplicitDiscovery() const;
    bool IsLootCrafting() const;
    bool IsArchaeologyCraftingSpell() const;
    bool IsQuestTame() const;
    bool IsProfessionOrRiding() const;
    bool IsProfession() const;
    bool IsPrimaryProfession() const;
    bool IsPrimaryProfessionFirstRank() const;
    bool IsAbilityLearnedWithProfession() const;

    bool IsAffectingArea() const;
    bool IsTargetingArea() const;
    bool IsTargetingAreaCast(uint8 effect = 0) const;
    bool NeedsExplicitUnitTarget() const;
    bool NeedsToBeTriggeredByCaster(SpellInfo const* triggeringSpell, uint32 difficulty) const;

    bool IsPassive() const;
    bool IsAutocastable() const;
    bool IsStackableWithRanks() const;
    bool IsPassiveStackableWithRanks() const;
    bool IsMultiSlotAura() const;
    bool IsDeathPersistent() const;
    bool IsRequiringDeadTarget() const;
    bool IsAllowingDeadTarget() const;
    bool CanBeUsedInCombat() const;
    bool IsPositive() const;
    bool IsPositiveEffect(uint8 effIndex, bool caster = false) const;
    bool IsChanneled() const;
    bool NeedsComboPoints() const;
    bool IsBreakingStealth() const;
    bool IsRangedWeaponSpell() const;
    bool IsRangedSpell() const;
    bool IsAutoRepeatRangedSpell() const;
    bool IsNonNeedDelay() const;

    bool IsAffectedBySpellMods() const;
    bool IsAffectedBySpellMod(SpellModifier* mod) const;

    bool CanPierceImmuneAura(SpellInfo const* aura) const;
    bool CanDispelAura(SpellInfo const* aura) const;

    bool IsNeedDelayForSpell() const;

    bool IsSingleTarget(Unit const* caster) const;
    bool IsAuraExclusiveBySpecificWith(SpellInfo const* spellInfo, bool sameCaster = false) const;
    bool IsAuraExclusiveBySpecificPerCasterWith(SpellInfo const* spellInfo) const;

    SpellCastResult CheckShapeshift(uint32 form) const;
    SpellCastResult CheckLocation(uint32 map_id, uint32 zone_id, uint32 area_id, Player const* player = NULL) const;
    SpellCastResult CheckTarget(Unit const* caster, WorldObject const* target, bool implicit = true) const;
    SpellCastResult CheckExplicitTarget(Unit const* caster, WorldObject const* target, Item const* itemTarget = NULL) const;
    bool CheckTargetCreatureType(Unit const* target) const;

    SpellSchoolMask GetSchoolMask() const;
    uint32 GetAllEffectsMechanicMask() const;
    uint32 GetEffectMechanicMask(uint8 effIndex) const;
    uint32 GetSpellMechanicMaskByEffectMask(uint32 effectMask) const;
    Mechanics GetEffectMechanic(uint8 effIndex) const;
    bool HasAnyEffectMechanic() const;
    uint32 GetDispelMask() const;
    static uint32 GetDispelMask(DispelType type);
    uint32 GetMechanicMask(uint32 miscVal) const;
    uint32 GetSimilarEffectsMiscValueMask(SpellEffects effectName, Unit* caster = NULL) const;
    uint32 GetExplicitTargetMask() const;
    uint32 GetSpellTypeMask() const;

    AuraStateType GetAuraState() const;
    SpellSpecificType GetSpellSpecific() const;

    uint32 GetMaxAffectedTargets(uint16 diff) const;
    uint32 GetTargets(uint16 diff) const;
    uint32 GetTargetCreatureType(uint16 diff) const;

    float GetMinRange(bool positive = false) const;
    float GetMaxRange(bool positive = false, Unit* caster = NULL, Spell* spell = NULL) const;

    int32 GetDuration() const;
    int32 GetMaxDuration() const;

    uint32 GetMaxTicks() const;

    uint32 CalcCastTime(Unit* caster = NULL, Spell* spell = NULL) const;
    uint32 GetRecoveryTime() const;

    SpellPowerCost CalcPowerCost(Unit const* caster, SpellSchoolMask schoolMask) const;

    bool IsRanked() const;
    uint8 GetRank() const;
    SpellInfo const* GetFirstRankSpell() const;
    SpellInfo const* GetLastRankSpell() const;
    SpellInfo const* GetNextRankSpell() const;
    SpellInfo const* GetPrevRankSpell() const;
    SpellInfo const* GetAuraRankForLevel(uint8 level) const;
    bool IsRankOf(SpellInfo const* spellInfo) const;
    bool IsDifferentRankOf(SpellInfo const* spellInfo) const;
    bool IsHighRankOf(SpellInfo const* spellInfo) const;
    bool IsAfflictionPeriodicDamage() const;
    
    uint32 GetSpellXSpellVisualId(Difficulty difficulty) const;
    uint32 GetSpellVisual(Difficulty difficulty, Player* forPlayer = nullptr) const;

    // helpers for breaking by damage spells
    bool IsBreakingCamouflage() const;
    bool IsBreakingCamouflageAfterHit() const;
    bool CanTriggerBladeFlurry() const;

    // loading helpers
    uint32 _GetExplicitTargetMask() const;
    bool _IsPositiveEffect(uint8 effIndex, bool deep) const;
    bool _IsPositiveSpell() const;
    static bool _IsPositiveTarget(uint32 targetA, uint32 targetB);

    void SetRangeIndex(uint32 index);

    // unloading helpers
    void _UnloadImplicitTargetConditionLists();
    bool IsSealSpell() const;

    bool AddPowerData(SpellPowerEntry const * power);
    bool IsPowerActive(uint8 powerIndex) const;
    SpellPowerEntry const* GetPowerInfo(uint8 powerIndex) const;
    bool GetSpellPowerByCasterPower(Unit const * caster, SpellPowerData& power) const;
    bool HasPower(Powers power) const;
    bool NoPower() const;
    bool CanNonFacing(Unit const * caster) const;

private:
    SpellTargetRestrictionsEntry const* GetSpellTargetRestrictions(uint16 diff) const;

    SpellVisualMap _visuals;
};

#endif // _SPELLINFO_H

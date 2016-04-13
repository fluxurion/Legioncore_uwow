/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
    Encounter: Odyn
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

enum Says
{
    SAY_FIRST           = 0,
    SAY_INTRO_1         = 2,
    SAY_INTRO_2         = 3,
    SAY_INTRO_3         = 4,
    SAY_RADIANT         = 5,
    SAY_SPEARS          = 6,
    SAY_RUNIC           = 7,
    SAY_RUNIC_1         = 8,
    SAY_DEATH           = 9,
};

enum Spells
{
    SPELL_HONOR_BOUND           = 198187, //? 01:44
    SPELL_SPEAR_OF_LIGHT        = 198072,
    SPELL_RADIANT_TEMPEST       = 201006,
    SPELL_SHATTER_SPEARS        = 198077,
    SPELL_GLOWING_FRAGMENT      = 198078,
    SPELL_RUNIC_BRAND           = 197961,

    SPELL_RUNIC_BRAND_PURE      = 197963,
    SPELL_RUNIC_BRAND_RED       = 197964,
    SPELL_RUNIC_BRAND_YELLOW    = 197965,
    SPELL_RUNIC_BRAND_BLUE      = 197966,
    SPELL_RUNIC_BRAND_GREEN     = 197967,
    
    SPELL_RUNIC_BRAND_PURE_AT   = 197968,
    SPELL_RUNIC_BRAND_RED_AT    = 197971,
    SPELL_RUNIC_BRAND_YELLOW_AT = 197972,
    SPELL_RUNIC_BRAND_BLUE_AT   = 197975,
    SPELL_RUNIC_BRAND_GREEN_AT  = 197977,
};

enum eEvents
{
    EVENT_SKOVALD_DONE_1        = 1,
    EVENT_SKOVALD_DONE_2        = 2,
    EVENT_SPEAR_OF_LIGHT        = 3,
    EVENT_RADIANT_TEMPEST       = 4,
    EVENT_SHATTER_SPEARS        = 5,
    EVENT_RUNIC_BRAND           = 6,
};

uint32 SpellsRunicColour[5] =
{
    SPELL_RUNIC_BRAND_PURE,
    SPELL_RUNIC_BRAND_RED,
    SPELL_RUNIC_BRAND_YELLOW,
    SPELL_RUNIC_BRAND_BLUE,
    SPELL_RUNIC_BRAND_GREEN
};

uint32 SpellsRunicAt[5] =
{
    SPELL_RUNIC_BRAND_PURE_AT,
    SPELL_RUNIC_BRAND_RED_AT,
    SPELL_RUNIC_BRAND_YELLOW_AT,
    SPELL_RUNIC_BRAND_BLUE_AT,
    SPELL_RUNIC_BRAND_GREEN_AT 
};

class boss_odyn : public CreatureScript
{
public:
    boss_odyn() : CreatureScript("boss_odyn") { }

    struct boss_odynAI : public BossAI
    {
        boss_odynAI(Creature* creature) : BossAI(creature, DATA_ODYN) 
        {
            intro = true;
            encounterComplete = false;
            _firstSay = false;
        }

        bool intro;
        bool _firstSay;
        bool encounterComplete;
        uint8 runicBrandCount;

        void Reset()
        {
            if (instance->GetBossState(DATA_ODYN) == DONE)
            {
                me->DespawnOrUnsummon();
                return;
            }
            events.Reset();
            _Reset();

            for (uint8 i = 0; i < 5; i++)
                instance->DoRemoveAurasDueToSpellOnPlayers(SpellsRunicColour[i]);
        }
        void MoveInLineOfSight(Unit* who)
        {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!_firstSay && me->IsWithinDistInMap(who, 150.0f))
             {
                _firstSay = true;
                Talk(SAY_FIRST);
             }
        }

        void EnterCombat(Unit* /*who*/) //01:44
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_SPEAR_OF_LIGHT, 8000);   //01:52, 02:00, 02:20, 02:48
            events.ScheduleEvent(EVENT_RADIANT_TEMPEST, 24000); //02:08, 02:56, 03:52, 04:48
            events.ScheduleEvent(EVENT_SHATTER_SPEARS, 40000);  //02:24, 03:20, 04:16, 05:12
            events.ScheduleEvent(EVENT_RUNIC_BRAND, 44000);     //02:28, 03:24, 04:20, 05:16
        }

        void DoAction(int32 const action)
        {
            Talk(SAY_INTRO_1); //Most impressive! I never thought I would meet anyone who could match the Valarjar's strength... and yet here you stand.
            events.ScheduleEvent(EVENT_SKOVALD_DONE_1, 5000);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_SPEAR_OF_LIGHT)
                DoCast(me, 198284, true); //spear of light searcher
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_SHATTER_SPEARS)
                if (target->GetEntry() == NPC_SPEAR_OF_LIGHT)
                {
                    Position pos;
                    float angle;
                    for (uint8 i = 0; i < 3; i++)
                    {
                        angle = 0.0f + (i * 2);
                        target->GetNearPosition(pos, 1.0f, angle);
                        me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_GLOWING_FRAGMENT, true);
                    }
                    target->ToCreature()->DespawnOrUnsummon(1000);
                }
            if (spell->Id == SPELL_RUNIC_BRAND)
            {
                DoCast(target, SpellsRunicColour[runicBrandCount], true);
                DoCast(me, SpellsRunicAt[runicBrandCount], true);
                Talk(SAY_RUNIC_1);
                instance->SetData(DATA_RUNES_ACTIVATED, runicBrandCount);
                runicBrandCount++;
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            me->RemoveAreaObject(SpellsRunicAt[data]);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(81) && !encounterComplete)
            {
                encounterComplete = true;
                DoStopAttack();
                Talk(SAY_DEATH);
                _JustDied();
                me->setFaction(35);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_ATTACKABLE_1);
                me->CombatStop();
                me->GetMotionMaster()->MoveTargetedHome();
                me->DespawnOrUnsummon(5000);
            }
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() && !intro)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SKOVALD_DONE_1:
                        Talk(SAY_INTRO_2); //There is one final trial that awaits......
                        me->GetMotionMaster()->MoveJump(2399.30f, 528.85f, 749.0f, 25.0f, 15.0f, 1);
                        events.ScheduleEvent(EVENT_SKOVALD_DONE_2, 10000);
                        break;
                    case EVENT_SKOVALD_DONE_2:
                        intro = false;
                        me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        Talk(SAY_INTRO_3); //It has been ages since I faced a worthy opponent. Let the battle begin!
                        me->setFaction(16);
                        break;
                    case EVENT_SPEAR_OF_LIGHT:
                        DoCast(SPELL_SPEAR_OF_LIGHT);
                        events.ScheduleEvent(EVENT_SPEAR_OF_LIGHT, 10000);
                        break;
                    case EVENT_RADIANT_TEMPEST:
                        DoCast(SPELL_RADIANT_TEMPEST);
                        Talk(SAY_RADIANT);
                        events.ScheduleEvent(EVENT_RADIANT_TEMPEST, 48000);
                        break;
                    case EVENT_SHATTER_SPEARS:
                        DoCast(SPELL_SHATTER_SPEARS);
                        Talk(SAY_SPEARS);
                        events.ScheduleEvent(EVENT_SHATTER_SPEARS, 56000);
                        break;
                    case EVENT_RUNIC_BRAND:
                        runicBrandCount = 0;
                        DoCast(SPELL_RUNIC_BRAND);
                        Talk(SAY_RUNIC);
                        events.ScheduleEvent(EVENT_RUNIC_BRAND, 56000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_odynAI (creature);
    }
};

//201006
class spell_odyn_radiant_tempest : public SpellScriptLoader
{
public:
    spell_odyn_radiant_tempest() : SpellScriptLoader("spell_odyn_radiant_tempest") { }

    class spell_odyn_radiant_tempest_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_odyn_radiant_tempest_SpellScript);

        void HandleOnHit()
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetHitUnit() != GetCaster())
                GetHitUnit()->CastSpell(GetCaster(), GetSpellInfo()->Effects[EFFECT_0].BasePoints, true);
        }

        void HandleOnCast()
        {
            if (!GetCaster())
                return;
            
            GetCaster()->CastSpell(GetCaster(), 198263, false);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_odyn_radiant_tempest_SpellScript::HandleOnHit);
            OnCast += SpellCastFn(spell_odyn_radiant_tempest_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_odyn_radiant_tempest_SpellScript();
    }
};

//197963, 197964, 197965, 197966, 197967
class spell_odyn_runic_brand : public SpellScriptLoader
{
public:
    spell_odyn_runic_brand() : SpellScriptLoader("spell_odyn_runic_brand") { }

    class spell_odyn_runic_brand_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_odyn_runic_brand_AuraScript);

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (!target)
                return;

            InstanceScript* instance = target->GetInstanceScript();
            if (!instance)
                return;

            for (uint8 i = 0; i < 5; i++)
                if (GetId() == SpellsRunicColour[i])
                    instance->SetData(DATA_RUNES_DEACTIVATED, i);
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_odyn_runic_brand_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_odyn_runic_brand_AuraScript();
    }
};

void AddSC_boss_odyn()
{
    new boss_odyn();
    new spell_odyn_radiant_tempest();
    new spell_odyn_runic_brand();
}
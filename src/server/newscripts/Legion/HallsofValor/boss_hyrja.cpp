/*
    http://uwow.biz
    Dungeon : Halls of Valor 100-110
    Encounter: Hyrja
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_valor.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_EXPEL_LIGHT     = 1,
    SAY_DEATH           = 2,
};

enum Spells
{
    SPELL_ASCENDANCE            = 191475,
    SPELL_ASCENDANCE_JUMP       = 191478,
    SPELL_EMPOWERMENT_TRACKER   = 192130,
    SPELL_EXPEL_LIGHT           = 192044, //192095 - 29:16?
    SPELL_EXPEL_LIGHT_DMG       = 192067,
    SPELL_EXPEL_LIGHT_CONFUSE   = 192070,
    SPELL_SHIELD_OF_LIGHT       = 192018,
    //Special Ability
    SPELL_EYE_OF_THE_STORM      = 192304, //Поиск ближайшего триггера (нужен фикс выбора таргетов и развешивания аур) (вырубил у босса и у моба)
    SPELL_EYE_OF_THE_STORM_DMG  = 200901,
    SPELL_EYE_OF_THE_STORM_AT   = 203955,
    SPELL_SANCTIFY_TICK         = 192158,
    SPELL_SANCTIFY_AT           = 192163,

    //Defender
    SPELL_OLMYR_VISUAL          = 191899,
    SPELL_SOLSTEN_VISUAL        = 192147,
    SPELL_OLMYR_LIGHT           = 192288,
    SPELL_SOLSTEN_ARCING        = 191976,
       
    SPELL_CONVERSATION          = 198226,
};

enum eEvents
{
    EVENT_EXPEL_LIGHT           = 1,
    EVENT_SHIELD_OF_LIGHT       = 2,
    EVENT_SPECIAL_ABILITY       = 3,
};

class boss_hyrja : public CreatureScript
{
public:
    boss_hyrja() : CreatureScript("boss_hyrja") { }

    struct boss_hyrjaAI : public BossAI
    {
        boss_hyrjaAI(Creature* creature) : BossAI(creature, DATA_HYRJA), summons(me)
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
            eventComplete = false;
            _introDone = false;
            defenderComplete = 0;
        }

        SummonList summons;
        uint8 defenderComplete;
        ObjectGuid defGUID[2];
        bool eventComplete;
        bool _introDone;

        void Reset()
        {
            summons.DespawnAll();
            events.Reset();
            _Reset();
            SummonDefenders();
            me->RemoveAurasDueToSpell(SPELL_EMPOWERMENT_TRACKER);
            if (eventComplete)
                DoCast(me, SPELL_ASCENDANCE, true);
        }

        void EnterCombat(Unit* /*who*/) //29:11
        {
            Talk(SAY_AGGRO);
            _EnterCombat();
            DoCast(me, SPELL_EMPOWERMENT_TRACKER, true);

            if (Creature* olmyr = me->GetCreature(*me, defGUID[0]))
                olmyr->CastSpell(olmyr, 192152, true);

            if (Creature* solsten = me->GetCreature(*me, defGUID[1]))
                solsten->CastSpell(solsten, 192150, true);

            events.ScheduleEvent(EVENT_EXPEL_LIGHT, 3000);      //29:14, 29:36, 30:12, 30:39, 31:10, 31:40
            events.ScheduleEvent(EVENT_SHIELD_OF_LIGHT, 10000); //29:21, 29:40, 30:09, 30:43
            events.ScheduleEvent(EVENT_SPECIAL_ABILITY, 20000);
        }

        void EnterEvadeMode()
        {
            BossAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
            summons.DespawnAll();
        }

        void SummonDefenders()
        {
            me->SummonCreature(NPC_OLMYR_THE_ENLIGHTENED, 3141.80f, 362.52f, 655.27f, 4.66f);
            me->SummonCreature(NPC_SOLSTEN, 3187.97f, 316.82f, 655.27f, 3.16f);
        }

        void JustSummoned(Creature* sum)
        {
            summons.Summon(sum);

            if (sum->GetEntry() == NPC_OLMYR_THE_ENLIGHTENED)
            {
                defGUID[0].Clear();
                defGUID[0] = sum->GetGUID();
            }

            if (sum->GetEntry() == NPC_SOLSTEN)
            {
                defGUID[1].Clear();
                defGUID[1] = sum->GetGUID();
            }
        }
        
        void MoveInLineOfSight(Unit* who)
         {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!_introDone && me->IsWithinDistInMap(who, 45.0f))
             {
                _introDone = true;
                DoCast(SPELL_CONVERSATION);
             }
         }

        void DoAction(int32 const action)
        {
            if (eventComplete)
                return;

            defenderComplete++;

            if (defenderComplete == 2)
                DoCast(SPELL_ASCENDANCE);
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_ASCENDANCE)
                DoCast(me, SPELL_ASCENDANCE_JUMP, true);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != EFFECT_MOTION_TYPE)
                return;

            if (id == SPELL_ASCENDANCE_JUMP)
            {
                eventComplete = true;
                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
            }
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
                case DATA_HYRJA_EVENT_COMPLETE:
                    return eventComplete;
            }
            return 0;
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_EXPEL_LIGHT:
                        DoCast(SPELL_EXPEL_LIGHT);
                        Talk(SAY_EXPEL_LIGHT);
                        events.ScheduleEvent(EVENT_EXPEL_LIGHT, 22000);
                        break;
                    case EVENT_SHIELD_OF_LIGHT:
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_SHIELD_OF_LIGHT);
                        events.ScheduleEvent(EVENT_SHIELD_OF_LIGHT, 20000);
                        break;
                    case EVENT_SPECIAL_ABILITY:
                    {
                        Creature* olmyr = me->GetCreature(*me, defGUID[0]);
                        Creature* solsten = me->GetCreature(*me, defGUID[1]);
                        if (!olmyr || !solsten)
                            return;

                        if (me->GetDistance(solsten) < me->GetDistance(olmyr))
                        {
                            /*DoCast(me, SPELL_EYE_OF_THE_STORM, true);
                            DoCast(me, SPELL_EYE_OF_THE_STORM_AT, true);
                            DoCast(SPELL_EYE_OF_THE_STORM_DMG); 
                            просто безумно багается и лагает из-за выбора таргетов */
                        }
                        else
                        {
                            DoCast(SPELL_SANCTIFY_TICK);
                        }
                        events.ScheduleEvent(EVENT_SPECIAL_ABILITY, 30000);
                        break;
                    }
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_hyrjaAI (creature);
    }
};

//97202, 97219
class npc_hyrja_defenders : public CreatureScript
{
public:
    npc_hyrja_defenders() : CreatureScript("npc_hyrja_defenders") { }

    struct npc_hyrja_defendersAI : public ScriptedAI
    {
        npc_hyrja_defendersAI(Creature* creature) : ScriptedAI(creature) 
        {
            ascendance = false;
        }

        EventMap events;
        bool ascendance;

        void Reset()
        {
            events.Reset();
            
            if (Unit* summoner = me->GetAnyOwner())
                if (summoner->GetAI()->GetData(DATA_HYRJA_EVENT_COMPLETE) == 1)
                    SpiritForm();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (damage >= me->GetHealth())
            {
                if (!ascendance)
                {
                    ascendance = true;
                    if (Unit* summoner = me->GetAnyOwner())
                        summoner->ToCreature()->AI()->DoAction(true);

                    SpiritForm();
                    me->GetMotionMaster()->MoveTargetedHome();
                    me->CombatStop();
                }
                damage = 0;
            }
        }
        
        void EnterCombat(Unit* /*who*/)
        {
            Talk(0);
            events.ScheduleEvent(EVENT_2, 30000);
            events.ScheduleEvent(EVENT_1, 5000);
        }

        void SpiritForm()
        {
            Talk(1);
            DoStopAttack();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);

            if (me->GetEntry() == NPC_OLMYR_THE_ENLIGHTENED)
                DoCast(me, SPELL_OLMYR_VISUAL, true);
            else
                DoCast(me, SPELL_SOLSTEN_VISUAL, true);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->GetDistance(me->GetHomePosition()) >= 30.0f)
            {
                EnterEvadeMode();
                return;
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        if (me->GetEntry() == NPC_OLMYR_THE_ENLIGHTENED)
                            DoCast(SPELL_OLMYR_LIGHT);
                        else
                            DoCast(SPELL_SOLSTEN_ARCING);   
                     events.ScheduleEvent(EVENT_1, 7000);
                        break;                    
                    case EVENT_2:
                        if (me->GetEntry() == NPC_OLMYR_THE_ENLIGHTENED)
                            DoCast(me, SPELL_SANCTIFY_TICK);
                       /* else
                        {
                            DoCast(me, SPELL_EYE_OF_THE_STORM, true);
                            DoCast(me, SPELL_EYE_OF_THE_STORM_AT, true);
                            DoCast(SPELL_EYE_OF_THE_STORM_DMG);
                        } тоже багается */
                     events.ScheduleEvent(EVENT_2, 30000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_hyrja_defendersAI(creature);
    }
};

//192130
class spell_hyrja_empowerment_tracker : public SpellScriptLoader
{
public:                                                      
    spell_hyrja_empowerment_tracker() : SpellScriptLoader("spell_hyrja_empowerment_tracker") { }

    class spell_hyrja_empowerment_tracker_AuraScript : public AuraScript 
    {
        PrepareAuraScript(spell_hyrja_empowerment_tracker_AuraScript) 

        uint8 tickCount;

        bool load()
        {
            tickCount = 0;
        }

        void OnPereodic(AuraEffect const* /*aurEff*/) 
        {
            tickCount++;

            if (GetTarget() && tickCount == 2)
            {
                tickCount = 0;

                //Thunder
                if (GetTarget()->HasAura(192008))
                    GetTarget()->CastSpell(GetTarget(), 192132, true);

                //Holy
                if (GetTarget()->HasAura(191924))
                    GetTarget()->CastSpell(GetTarget(), 192133, true);
            }
        }

        void Register() 
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hyrja_empowerment_tracker_AuraScript::OnPereodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hyrja_empowerment_tracker_AuraScript();
    }
};

//192048
class spell_hyrja_expel_light : public SpellScriptLoader
{
    public:
        spell_hyrja_expel_light() : SpellScriptLoader("spell_hyrja_expel_light") { }

        class spell_hyrja_expel_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hyrja_expel_light_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Unit* target = GetTarget();
                if (!target)
                    return;

                caster->CastSpell(target, SPELL_EXPEL_LIGHT_DMG, true);

                if (caster->GetMap()->IsHeroic())
                    caster->CastSpell(caster, SPELL_EXPEL_LIGHT_CONFUSE, true);
            }
            
            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_hyrja_expel_light_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hyrja_expel_light_AuraScript();
        }
};

//192158
class spell_hyrja_sanctify : public SpellScriptLoader
{
public:                                                      
    spell_hyrja_sanctify() : SpellScriptLoader("spell_hyrja_sanctify") { }

    class spell_hyrja_sanctify_AuraScript : public AuraScript 
    {
        PrepareAuraScript(spell_hyrja_sanctify_AuraScript) 

        void OnPereodic(AuraEffect const* /*aurEff*/) 
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            for (uint8 i = 0; i < 6; i++)
                caster->CastSpell(caster, SPELL_SANCTIFY_AT, true);
        }

        void Register() 
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_hyrja_sanctify_AuraScript::OnPereodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_hyrja_sanctify_AuraScript();
    }
};
//4885
class at_sanctify : public AreaTriggerScript
{
    public:
        at_sanctify() : AreaTriggerScript("at_sanctify") {}

       bool OnTrigger(Player* player, const AreaTriggerEntry* /*at*/, bool /*enter*/)
        {
                player->CastSpell(player, 192206, true);
            return true;
        }
};

void AddSC_boss_hyrja()
{
    new boss_hyrja();
    new npc_hyrja_defenders();
    new spell_hyrja_empowerment_tracker();
    new spell_hyrja_expel_light();
    new spell_hyrja_sanctify();
    new at_sanctify();
}
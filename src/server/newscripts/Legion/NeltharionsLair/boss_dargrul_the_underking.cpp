/*
    http://uwow.biz
    Dungeon : Neltharions Lair 100-110
    Encounter: Dargrul the Underking
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "neltharions_lair.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_EMOTE           = 1,
    SAY_DEATH           = 2,
};

enum Spells
{
    SPELL_GAIN_ENERGY           = 201444,
    SPELL_CONVERSATION_01       = 201665, //SPELL_MAGMA_SCULPTOR
    SPELL_CONVERSATION_02       = 201666, //SPELL_MAGMA_SCULPTOR
    SPELL_MAGMA_SCULPTOR        = 200637, 
    SPELL_LANDSLIDE             = 200700,
    SPELL_LANDSLIDE_MISSILE     = 200722,
    SPELL_CRYSTAL_WALL          = 200551,
    SPELL_MAGMA_WAVE            = 200404,
    SPELL_MAGMA_WAVE_AT         = 200418,

    SPELL_CONVERSATION_03       = 201663, //
    SPELL_CONVERSATION_04       = 201659, //
    SPELL_CONVERSATION_05       = 201661, //SPELL_MAGMA_WAVE

    SPELL_SUM_PILLAR_CREATION   = 213589,
    //Trash
    SPELL_FIXATE_PLR            = 200154,
    SPELL_COLLISION_AT          = 188197,
    SPELL_MAGMA_BREAKER         = 209926,
    SPELL_MAGMA_BREAKER_AURA    = 209920,

    SPELL_CRYSTAL_BLOCKER_AT    = 200367,
    SPELL_CRYSTAL_WALL_SUM_GO   = 209964, //GO DOOR
};

enum eEvents
{
    EVENT_MAGMA_SCULPTOR        = 1,
    EVENT_LANDSLIDE             = 2,
    EVENT_CRYSTAL_WALL          = 3,
};

class boss_dargrul_the_underking : public CreatureScript
{
public:
    boss_dargrul_the_underking() : CreatureScript("boss_dargrul_the_underking") {}

    struct boss_dargrul_the_underkingAI : public BossAI
    {
        boss_dargrul_the_underkingAI(Creature* creature) : BossAI(creature, DATA_DARGRUL) 
        {
            me->SetMaxPower(POWER_MANA, 100);
            introDone = false;
        }

        uint8 rand;
        bool introDone;

        void Reset()
        {
            _Reset();
            me->RemoveAurasDueToSpell(SPELL_GAIN_ENERGY);
            me->SetPower(POWER_MANA, 0);
        }

        void EnterCombat(Unit* /*who*/) //27:51
        {
            Talk(SAY_AGGRO); //Yes. TRY to take the hammer from me!
            _EnterCombat();
            DoCast(me, SPELL_GAIN_ENERGY, true);

            events.ScheduleEvent(EVENT_MAGMA_SCULPTOR, 7000); //27:58, 29:10, 30:26
            events.ScheduleEvent(EVENT_LANDSLIDE, 16000);     //28:07, 28:25, 28:41
            events.ScheduleEvent(EVENT_CRYSTAL_WALL, 22000);  //28:13, 28:35, 29:00
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);
            _JustDied();
        }
        
        void MoveInLineOfSight(Unit* who)
        {  
 
            if (!(who->GetTypeId() == TYPEID_PLAYER))
               return;
          
             if (!introDone && me->IsWithinDistInMap(who, 115.0f))
             {
                who->CastSpell(who, 209698, true);
                introDone = true;
             }
        } 

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_LANDSLIDE)
            {
                Position pos;
                float dist;
                float angle;
                for(uint16 i = 0; i < 100; i++)
                {
                    angle = frand(-0.17f, 0.17f);
                    me->GetNearPosition(pos, dist+i, angle);
                    me->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_LANDSLIDE_MISSILE, true);
                }
            }
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
                    case EVENT_MAGMA_SCULPTOR:
                        rand = urand(0, 1);
                        DoCast(me, rand ? SPELL_CONVERSATION_01 : SPELL_CONVERSATION_02, true);
                        DoCast(SPELL_MAGMA_SCULPTOR);
                        events.ScheduleEvent(EVENT_MAGMA_SCULPTOR, 70000);
                        break;
                    case EVENT_LANDSLIDE:
                        DoCast(SPELL_LANDSLIDE);
                        events.ScheduleEvent(EVENT_LANDSLIDE, 16000);
                        break;
                    case EVENT_CRYSTAL_WALL:
                        DoCast(SPELL_CRYSTAL_WALL);
                        events.ScheduleEvent(EVENT_CRYSTAL_WALL, 22000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_dargrul_the_underkingAI (creature);
    }
};

//101476
class npc_dargrul_molten_charskin : public CreatureScript
{
public:
    npc_dargrul_molten_charskin() : CreatureScript("npc_dargrul_molten_charskin") {}

    struct npc_dargrul_molten_charskinAI : public ScriptedAI
    {
        npc_dargrul_molten_charskinAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;
        ObjectGuid playerGuid;

        void Reset()
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* summoner)
        {
            DoZoneInCombat(me, 100.0f);
            DoCast(me, SPELL_MAGMA_BREAKER_AURA, true);
            events.ScheduleEvent(EVENT_1, 2000);
        }

        bool checkPlayers()
        {
            std::list<Player*> playerList;
            std::list<Player*> playerTempList;
            GetPlayerListInGrid(playerList, me, 80.0f);
            for (auto const& player : playerList)
                if (!playerList.empty())
                    if (player->isAlive() && player->isInCombat())
                        playerTempList.push_back(player);

            if (playerTempList.size() > 1)
                return true;

            return false;
        }            

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_1:
                        if (!me->getVictim() || !me->getVictim()->HasAura(SPELL_FIXATE_PLR))
                        {
                            if (Creature* summoner = me->GetAnyOwner()->ToCreature())
                                if (Unit* pTarget = summoner->AI()->SelectTarget(SELECT_TARGET_RANDOM, checkPlayers(), 80.0f, true, -SPELL_FIXATE_PLR))
                                {
                                    DoStopAttack();
                                    AttackStart(pTarget);
                                    me->CastSpell(pTarget, SPELL_FIXATE_PLR);
                                    me->ClearUnitState(UNIT_STATE_CASTING);
                                }
                        }
                        events.ScheduleEvent(EVENT_1, 2000);
                        break;
                    default:
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_dargrul_molten_charskinAI(creature);
    }
};

//101593
class npc_dargrul_crystal_wall : public CreatureScript
{
public:
    npc_dargrul_crystal_wall() : CreatureScript("npc_dargrul_crystal_wall") {}

    struct npc_dargrul_crystal_wallAI : public ScriptedAI
    {
        npc_dargrul_crystal_wallAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        ObjectGuid WallGUID;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_CRYSTAL_BLOCKER_AT, true);
            DoCast(me, SPELL_CRYSTAL_WALL_SUM_GO, true);
        }

        void JustSummonedGO(GameObject* go)
        {
            WallGUID = go->GetGUID();
        }

        void OnAreaTriggerCast(Unit* caster, Unit* target, uint32 spellId)
        {
            if (spellId == 200672) //Stun
                DespawnMe();
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == 200721 || spell->Id == 209947)
                DespawnMe();
        }
        
        void DespawnMe()
        {
            if (GameObject* go = me->GetMap()->GetGameObject(WallGUID))
                go->Delete();

            me->DespawnOrUnsummon();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            damage = 0;
        }

        void UpdateAI(uint32 diff) {}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_dargrul_crystal_wallAI(creature);
    }
};


//201444
class spell_dargrul_gain_energy : public SpellScriptLoader
{
public:
    spell_dargrul_gain_energy() : SpellScriptLoader("spell_dargrul_gain_energy") { }

    class spell_dargrul_gain_energy_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dargrul_gain_energy_AuraScript);

        uint8 checkTick = 0;
        uint8 amount = 0;

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (checkTick < 2)
            {
                checkTick++;
                amount = 2;
            }
            else
            {
                checkTick = 0;
                amount = 1;
            }
            const_cast<AuraEffect*>(aurEff)->ChangeAmount(amount);

            if (caster->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (caster->GetPower(POWER_MANA) >= 100)
            {
                caster->CastSpell(caster, SPELL_CONVERSATION_05, true);
                caster->CastSpell(caster, SPELL_MAGMA_WAVE_AT, true);
                caster->CastSpell(caster, SPELL_MAGMA_WAVE);
                if (Creature* target = caster->FindNearestCreature(91007, 50, true))
                        target->AI()->Talk(SAY_EMOTE);
                caster->SetPower(POWER_MANA, 0);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dargrul_gain_energy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_ENERGIZE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dargrul_gain_energy_AuraScript();
    }
};

//209920
class spell_dargrul_magma_breaker : public SpellScriptLoader
{
public:
    spell_dargrul_magma_breaker() : SpellScriptLoader("spell_dargrul_magma_breaker") { }

    class spell_dargrul_magma_breaker_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dargrul_magma_breaker_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->CastSpell(caster, SPELL_MAGMA_BREAKER, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dargrul_magma_breaker_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dargrul_magma_breaker_AuraScript();
    }
};

//200404
class spell_dargrul_magma_wave_filter : public SpellScriptLoader
{
public:
    spell_dargrul_magma_wave_filter() : SpellScriptLoader("spell_dargrul_magma_wave_filter") { }

    class spell_dargrul_magma_wave_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_dargrul_magma_wave_filter_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            if (targets.empty())
                return;

            std::list<GameObject*> goList;
            GetCaster()->GetGameObjectListWithEntryInGrid(goList, GO_CRYSTAL_WALL_COLLISION, 100.0f);
            if (!goList.empty())
                for (std::list<GameObject*>::const_iterator itr = goList.begin(); itr != goList.end(); ++itr)
                    targets.remove_if(WallCheck(GetCaster(), (*itr)));

        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dargrul_magma_wave_filter_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }

    private:
        class WallCheck
        {
            public:
                WallCheck(Unit* searcher, GameObject* go) : _searcher(searcher), _go(go) {}
        
                bool operator()(WorldObject* unit)
                {
                    return (_go->IsInBetween(_searcher, unit, 4.0f));
                }

            private:
                Unit* _searcher;
                GameObject* _go;
        };
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_dargrul_magma_wave_filter_SpellScript();
    }
};
// 102295 trash
class npc_emberhusk_dominator : public CreatureScript
{
public:
    npc_emberhusk_dominator() : CreatureScript("npc_emberhusk_dominator") {}

    struct npc_emberhusk_dominatorAI : public ScriptedAI
    {
        npc_emberhusk_dominatorAI(Creature* creature) : ScriptedAI(creature) {}
        
        uint32 SpellTimer;
        uint32 SayTimer;
         void Reset()
         {
            SpellTimer = urand (3000, 4000);
            SayTimer = urand(30000, 45000);
         }
        
        void UpdateAI(uint32 diff)
        {
             if (!UpdateVictim())
                 return;

             if (me->HasUnitState(UNIT_STATE_CASTING))
                 return;
              
             if (SpellTimer <= diff)
             {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                     me->CastSpell(target, 201959, TriggerCastFlags(TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE));
                SpellTimer = urand (3000, 4000);
             } else SpellTimer -= diff;
             
             if (SayTimer <= diff)
             {
                SayTimer = urand(30000, 45000);
                Talk(0);
             } else SayTimer -= diff;
        }
        
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_emberhusk_dominatorAI(creature);
    }
};

void AddSC_boss_dargrul_the_underking()
{
    new boss_dargrul_the_underking();
    new npc_dargrul_molten_charskin();
    new npc_dargrul_crystal_wall();
    new spell_dargrul_gain_energy();
    new spell_dargrul_magma_breaker();
    new spell_dargrul_magma_wave_filter();
    //trash
    new npc_emberhusk_dominator();
}
/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "siege_of_orgrimmar.h"

enum eSpells
{
    //Phase 1
    SPELL_FEARSOME_ROAR      = 143426, 
    SPELL_DEAFENING_SCREECH  = 143343,
    SPELL_TAIL_LASH          = 143428, 
    SPELL_SHOCK_BLAST        = 143707,
    SPELL_BLOODIED           = 143452,
    SPELL_POWER_REGEN        = 143345,
    SPELL_ACCELERATION       = 143411,
    //
    SPELL_ACID_BREATH        = 143780,
    SPELL_CORROSIVE_BLOOD    = 143791,
    //
    SPELL_FREEZING_BREATH    = 143773,
    SPELL_ICY_BLOOD          = 143800,
    SPELL_SUMMON_ICE_TOMB    = 136929,
    SPELL_FROZEN_SOLID       = 143777,
    //
    SPELL_SCORCHING_BREATH   = 143767,
    SPELL_BURNING_BLOOD      = 143783,
    SPELL_BURNING_BLOOD_ADMG = 143784,

    //Phase 2
    SPELL_BLOOD_FRENZY       = 143440,
    SPELL_BLOOD_FRENZY_TE    = 143442,
    SPELL_BLOOD_FRENZY_KB    = 144067, 
    SPELL_FIXATE_PL          = 143445,
    SPELL_FIXATE_IM          = 146540,
    SPELL_FIXATE_PR          = 146581,

    SPELL_ENRAGE_KJ          = 145974,
    SPELL_UNLOCKING          = 146589, 

    SPELL_ENRAGE             = 26662,

    SPELL_VAMPIRIC_FRENZY    = 147978,

    SPELL_CANNON_BALL        = 147906,
    SPELL_CANNON_BALL_ATDMG  = 147607,
    SPELL_CANNON_BALL_AT_A   = 147609,
    SPELL_CANNON_BALL_DESTD  = 147662,
    SPELL_FLAME_COATING      = 144115,
    SPELL_R_WATERS           = 144117,
};

enum Events
{
    //Default events
    EVENT_FEARSOME_ROAR      = 1,
    EVENT_TAIL_LASH          = 2,
    EVENT_SHOCK_BLAST        = 3,
    //Extra events
    EVENT_ACID_BREATH        = 4,
    EVENT_CORROSIVE_BLOOD    = 5,
    //
    EVENT_FREEZING_BREATH    = 6,
    EVENT_ICY_BLOOD          = 7,
    //
    EVENT_SCORCHING_BREATH   = 8,
    EVENT_BURNING_BLOOD      = 9,
    //Special events
    EVENT_GO_TO_PRISONER     = 10,
    EVENT_FIXATE             = 11,
    EVENT_KILL_PRISONER      = 12,
    EVENT_MOVING             = 13,

    //Summon events
    EVENT_ENRAGE_KJ          = 14,
    EVENT_MOVE_TO_CENTER     = 15,
    EVENT_MOVE_TO_THOK       = 16,
    EVENT_CHECK_TPLAYER      = 17,
    EVENT_Y_CHARGE           = 18,
    EVENT_PRE_Y_CHARGE       = 19,
    EVENT_VAMPIRIC_FRENZY    = 20,
    EVENT_R_WATERS           = 21,
};

enum Action
{
    ACTION_PHASE_TWO         = 1,
    ACTION_PHASE_ONE_ACID    = 2,
    ACTION_PHASE_ONE_FROST   = 3,
    ACTION_PHASE_ONE_FIRE    = 4,
    ACTION_FIXATE            = 5,
    ACTION_START_FIXATE      = 6,
    ACTION_FREEDOM           = 7,
};

uint32 prisonersentry[3] =
{
    NPC_AKOLIK, 
    NPC_MONTAK, 
    NPC_WATERSPEAKER_GORAI, 
};

Position fpos[3] =
{
    {1273.30f, -5123.47f, -290.4582f, 2.9432f},
    {1138.35f, -5100.04f, -290.4619f, 6.1209f},
    {1220.41f, -5045.36f, -290.4579f, 4.5030f},
};

Position kjspawnpos = {1285.03f, -5059.10f, -290.9505f, 3.6988f}; 
Position cpos = {1208.61f, -5106.27f, -289.939f, 0.526631f};

Position ccbatspawnpos[7] =
{
    {1257.04f, -5169.60f, -280.0894f, 2.2238f},
    {1251.81f, -5162.37f, -280.0894f, 2.2238f},
    {1251.15f, -5174.29f, -280.0894f, 2.2238f},
    {1245.83f, -5166.56f, -280.0894f, 2.2238f},
    {1238.66f, -5181.14f, -280.0894f, 2.2238f},
    {1233.69f, -5172.25f, -280.0894f, 2.2238f},
    {1244.47f, -5158.36f, -280.0894f, 2.2238f},
};

Position sumyetipos[4] = 
{
    {1217.50f, -5041.30f, -290.4328f, 4.4818f},
    {1272.52f, -5124.51f, -290.4575f, 2.8089f},
    {1218.18f, -5181.76f, -290.4609f, 1.7683f},
    {1135.86f, -5098.79f, -290.4617f, 6.0448f},
};

enum CreatureText
{
    SAY_PULL
};

class boss_thok_the_bloodthirsty : public CreatureScript
{
    public:
        boss_thok_the_bloodthirsty() : CreatureScript("boss_thok_the_bloodthirsty") {}

        struct boss_thok_the_bloodthirstyAI : public BossAI
        {
            boss_thok_the_bloodthirstyAI(Creature* creature) : BossAI(creature, DATA_THOK)
            {
                instance = creature->GetInstanceScript();
                me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_CORROSIVE_BLOOD, true);
            }
            
            InstanceScript* instance;
            ObjectGuid fplGuid, jGuid, pGuid;
            uint32 enrage;
            uint32 findtargets;
            uint8 phasecount;
            bool phasetwo;

            void Reset()
            {
                _Reset();
                DespawnObjects();
                me->SetReactState(REACT_DEFENSIVE);
                me->RemoveAurasDueToSpell(SPELL_POWER_REGEN);
                me->RemoveAurasDueToSpell(SPELL_ACCELERATION);
                me->RemoveAurasDueToSpell(SPELL_FIXATE_PL);
                me->RemoveAurasDueToSpell(SPELL_BLOOD_FRENZY);
                me->RemoveAurasDueToSpell(SPELL_BLOOD_FRENZY_TE);
                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);
                findtargets = 0;
                fplGuid.Clear();  
                jGuid.Clear();
                pGuid.Clear();
                phasecount = 0;
                phasetwo = false;
                enrage = 0;
                if (instance)
                {
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIXATE_PL);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOODIED);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNLOCKING);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BURNING_BLOOD_ADMG);
                }
            }

            void DespawnObjects()
            {
                std::list<AreaTrigger*> atlist;
                atlist.clear();
                me->GetAreaTriggersWithEntryInRange(atlist, 4890, me->GetGUID(), 200.0f);
                if (!atlist.empty())
                {
                    for (std::list<AreaTrigger*>::const_iterator itr = atlist.begin(); itr != atlist.end(); itr++)
                        (*itr)->RemoveFromWorld();
                }
            }

            /*Debug (for testing)
            void SpellHit(Unit* caster, SpellInfo const *spell)
            {
                if (spell->Id == SPELL_BLOODIED && me->HasAura(SPELL_POWER_REGEN))
                {
                    me->RemoveAurasDueToSpell(SPELL_BLOODIED);
                    me->RemoveAurasDueToSpell(SPELL_POWER_REGEN);
                    me->ToCreature()->AI()->DoAction(ACTION_PHASE_TWO);
                }
            }*/

            void EnterCombat(Unit* who)
            {
                _EnterCombat();
                enrage = 600000;
                DoCast(me, SPELL_POWER_REGEN, true);
                events.ScheduleEvent(EVENT_SHOCK_BLAST, 4000);
                events.ScheduleEvent(EVENT_TAIL_LASH, 12000);
                events.ScheduleEvent(EVENT_FEARSOME_ROAR, 15000);
            }

            void SetGUID(ObjectGuid const& Guid, int32 type) override
            {
                if (type == 2 && instance)
                {
                    findtargets = 0;
                    pGuid = Guid;
                    me->InterruptNonMeleeSpells(true);
                    me->RemoveAurasDueToSpell(SPELL_FIXATE_PL);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIXATE_PL);
                    if (Creature* prisoner = me->GetCreature(*me, pGuid))
                        DoCast(prisoner, SPELL_FIXATE_PR, true);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->StopMoving();
                    me->GetMotionMaster()->Clear(false);
                    me->getThreatManager().resetAllAggro();
                    me->GetMotionMaster()->MoveCharge(cpos.GetPositionX(), cpos.GetPositionY(), cpos.GetPositionZ(), 15.0f, 0);
                }
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                case ACTION_PHASE_ONE_ACID: 
                case ACTION_PHASE_ONE_FROST: 
                case ACTION_PHASE_ONE_FIRE:
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                    events.Reset();
                    phasetwo = false;
                    me->StopMoving();
                    me->GetMotionMaster()->Clear(false);
                    me->getThreatManager().resetAllAggro();
                    me->RemoveAurasDueToSpell(SPELL_BLOOD_FRENZY);
                    me->RemoveAurasDueToSpell(SPELL_BLOOD_FRENZY_TE);
                    me->RemoveAurasDueToSpell(SPELL_FIXATE_PL);
                    if (instance)
                        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIXATE_PL);
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoCast(me, SPELL_POWER_REGEN, true);
                    DoZoneInCombat(me, 150.0f);
                    if (me->getVictim())
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    else
                    {
                        EnterEvadeMode();
                        return;
                    }
                    events.ScheduleEvent(EVENT_TAIL_LASH, 12000);
                    switch (action)
                    {
                    case ACTION_PHASE_ONE_ACID: 
                        events.ScheduleEvent(EVENT_ACID_BREATH, 15000);
                        events.ScheduleEvent(EVENT_CORROSIVE_BLOOD, 4000);
                        break;
                    case ACTION_PHASE_ONE_FROST: 
                        events.ScheduleEvent(EVENT_FREEZING_BREATH, 15000);
                        events.ScheduleEvent(EVENT_ICY_BLOOD, 4000);
                        break;
                    case ACTION_PHASE_ONE_FIRE:
                        DoCastAOE(SPELL_FLAME_COATING, true);
                        events.ScheduleEvent(EVENT_SCORCHING_BREATH, 15000);
                        events.ScheduleEvent(EVENT_BURNING_BLOOD, 4000);
                        break;
                    }
                    if (me->GetMap()->IsHeroic())
                    {
                        phasecount++;
                        me->SetHealth(me->GetHealth() + me->CountPctFromMaxHealth(8));
                        switch (phasecount)
                        {
                        case 1:
                            for (uint8 n = 0; n < 7; n++)
                                if (Creature* bat = me->SummonCreature(NPC_CAPTIVE_CAVE_BAT, ccbatspawnpos[n]))
                                    bat->AI()->DoZoneInCombat(bat, 200.0f);
                            break;
                        case 2:
                            me->SummonCreature(NPC_STARVED_YETI, sumyetipos[urand(0, 3)]);
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                case ACTION_PHASE_TWO:
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    phasetwo = true;
                    events.Reset();
                    events.ScheduleEvent(EVENT_SHOCK_BLAST, 3000);
                    if (instance)
                        instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOODIED);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->StopMoving();
                    me->GetMotionMaster()->Clear(false);
                    me->getThreatManager().resetAllAggro();
                    me->RemoveAurasDueToSpell(SPELL_POWER_REGEN);
                    me->RemoveAurasDueToSpell(SPELL_ACCELERATION);
                    me->SetPower(POWER_ENERGY, 0);
                    DoCast(me, SPELL_BLOOD_FRENZY_KB, true);
                    DoCast(me, SPELL_BLOOD_FRENZY, true);
                    if (Creature* kj = me->SummonCreature(NPC_KORKRON_JAILER, kjspawnpos))
                    {
                        kj->AI()->DoZoneInCombat(kj, 250.0f);
                        jGuid = kj->GetGUID();
                    }
                    events.ScheduleEvent(EVENT_FIXATE, 2000);
                    break;
                //Special actions
                case ACTION_FIXATE:
                    findtargets = 0;
                    events.ScheduleEvent(EVENT_FIXATE, 1000);
                    break;
                case ACTION_START_FIXATE:
                    events.ScheduleEvent(EVENT_MOVING, 2000);
                    break;
                }
            }

            void MovementInform(uint32 type, uint32 pointId)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (pointId)
                    {
                    case 0:
                        events.ScheduleEvent(EVENT_GO_TO_PRISONER, 500);
                        break;
                    case 1:
                        events.ScheduleEvent(EVENT_KILL_PRISONER, 3000);
                        break;
                    }       
                }
            }

            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (findtargets)
                {
                    if (findtargets <= diff)
                    {
                        std::list<Player*> plist;
                        plist.clear();
                        GetPlayerListInGrid(plist, me, 20.0f);
                        if (!plist.empty())
                        {
                            for (std::list<Player*>::const_iterator itr = plist.begin(); itr != plist.end(); itr++)
                                if (me->isInFront(*itr, 2.0f) && me->GetDistance(*itr) <= 8.0f)
                                    (*itr)->Kill(*itr, true);
                        }
                        findtargets = 700;
                    }
                    else
                        findtargets -= diff;
                }

                if (enrage)
                {
                    if (enrage <= diff)
                    {
                        DoCast(me, SPELL_ENRAGE, true);
                        enrage = 0;
                    }
                    else
                        enrage -= diff;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    //Default events
                    case EVENT_SHOCK_BLAST:
                        DoCastAOE(SPELL_SHOCK_BLAST, true);
                        events.ScheduleEvent(EVENT_SHOCK_BLAST, 4000);
                        break;
                    case EVENT_TAIL_LASH:
                        DoCast(me, SPELL_TAIL_LASH, true);
                        events.ScheduleEvent(EVENT_TAIL_LASH, 12000);
                        break;
                    case EVENT_FEARSOME_ROAR:
                        DoCast(me, SPELL_FEARSOME_ROAR, true);
                        events.ScheduleEvent(EVENT_FEARSOME_ROAR, 15000);
                        break;
                    case EVENT_GO_TO_PRISONER:
                        if (Creature* pr = me->GetCreature(*me, pGuid))
                            me->GetMotionMaster()->MoveCharge(pr->GetPositionX(), pr->GetPositionY(), pr->GetPositionZ(), 15.0f, 1);
                        break;
                    //Extra events
                    //
                    case EVENT_ACID_BREATH:
                        DoCast(me, SPELL_ACID_BREATH, true);
                        events.ScheduleEvent(EVENT_ACID_BREATH, 15000);
                        break;
                    case EVENT_CORROSIVE_BLOOD:
                        DoCastAOE(SPELL_CORROSIVE_BLOOD, true);
                        events.ScheduleEvent(EVENT_CORROSIVE_BLOOD, 4000);
                        break;
                    //
                    case EVENT_FREEZING_BREATH:
                        DoCast(me, SPELL_FREEZING_BREATH, true);
                        events.ScheduleEvent(EVENT_FREEZING_BREATH, 15000);
                        break;
                    case EVENT_ICY_BLOOD:
                    {
                        std::list<HostileReference*> tlist = me->getThreatManager().getThreatList();
                        if (!tlist.empty())
                        {
                            uint8 num = 0;
                            uint8 maxnum = me->GetMap()->Is25ManRaid() ? 8 : 3;
                            for (std::list<HostileReference*>::const_iterator itr = tlist.begin(); itr != tlist.end(); itr++)
                            {
                                if (itr != tlist.begin())
                                {
                                    if (Player* pl = me->GetPlayer(*me, (*itr)->getUnitGuid()))
                                    {
                                        if (!pl->HasAura(SPELL_FROZEN_SOLID))
                                        {
                                            pl->AddAura(SPELL_ICY_BLOOD, pl);
                                            num++;
                                            if (num == maxnum)
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_ICY_BLOOD, 4000);
                        break;
                    }
                    //
                    case EVENT_SCORCHING_BREATH:
                        DoCast(me, SPELL_SCORCHING_BREATH, true);
                        events.ScheduleEvent(EVENT_SCORCHING_BREATH, 15000);
                        break;
                    case EVENT_BURNING_BLOOD:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 150.0f, true))
                            DoCast(target, SPELL_BURNING_BLOOD);
                        events.ScheduleEvent(EVENT_BURNING_BLOOD, 4000);
                        break; 
                    //
                    //Special events
                    case EVENT_KILL_PRISONER:
                        if (Creature* pr = me->GetCreature(*me, pGuid))
                        {
                            me->RemoveAurasDueToSpell(SPELL_FIXATE_PR);
                            pr->Kill(pr, true);
                            switch (pr->GetEntry())
                            {
                            case NPC_AKOLIK:
                                DoAction(ACTION_PHASE_ONE_ACID);
                                break;
                            case NPC_MONTAK:
                                DoAction(ACTION_PHASE_ONE_FIRE);
                                break;
                            case NPC_WATERSPEAKER_GORAI:
                                DoAction(ACTION_PHASE_ONE_FROST);
                                break;
                            }
                            pGuid.Clear();
                        }
                        break;
                    case EVENT_FIXATE:
                        me->InterruptNonMeleeSpells(true);
                        if (Player* pl = me->GetPlayer(*me, GetFixateTargetGuid()))
                        {
                            DoCast(pl, SPELL_FIXATE_PL);
                            fplGuid = pl->GetGUID();
                        }
                        else
                            EnterEvadeMode();
                        break;
                    case EVENT_MOVING:
                        if (Player* pl = me->GetPlayer(*me, fplGuid))
                        {
                            me->AddThreat(pl, 50000000.0f);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->Attack(pl, true);
                            me->GetMotionMaster()->MoveChase(pl);
                            findtargets = 700;
                        }
                        else
                            EnterEvadeMode();
                        break;
                    }
                }
                if (!phasetwo)
                    DoMeleeAttackIfReady();
            }

            ObjectGuid GetJailerVictimGuid()
            {
                if (Creature* kj = me->GetCreature(*me, jGuid))
                    if (kj->isAlive() && kj->isInCombat())
                        return kj->getVictim() ? kj->getVictim()->GetGUID() : ObjectGuid::Empty;

                return ObjectGuid::Empty;
            }

            ObjectGuid GetFixateTargetGuid()
            {
                std::list<Player*> pllist;
                std::list<Player*> fpllist;
                pllist.clear();
                fpllist.clear();
                GetPlayerListInGrid(pllist, me, 150.0f);
                ObjectGuid jvGuid = GetJailerVictimGuid();
                if (!pllist.empty())
                {
                    for (std::list<Player*>::const_iterator itr = pllist.begin(); itr != pllist.end(); itr++)
                        if (!(*itr)->HasAura(SPELL_UNLOCKING))
                            if ((*itr)->GetGUID() != jvGuid)
                                fpllist.push_back(*itr);

                    if (!fpllist.empty())
                    {
                        std::list<Player*>::const_iterator Itr = fpllist.begin();
                        std::advance(Itr, urand(0, fpllist.size() - 1));
                        return (*Itr)->GetGUID();
                    }
                }
                return ObjectGuid::Empty;
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                DespawnObjects();
                if (instance)
                {
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FIXATE_PL);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOODIED);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_UNLOCKING);
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BURNING_BLOOD_ADMG);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_thok_the_bloodthirstyAI(creature);
        }
};

//71658
class npc_korkron_jailer : public CreatureScript
{
public:
    npc_korkron_jailer() : CreatureScript("npc_korkron_jailer") {}

    struct npc_korkron_jailerAI : public ScriptedAI
    {
        npc_korkron_jailerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset(){}
        
        void EnterCombat(Unit* who)
        {
            Talk(SAY_PULL);
            events.ScheduleEvent(EVENT_ENRAGE_KJ, 1000);
        }

        void OnSpellClick(Unit* clicker)
        {
            clicker->CastSpell(clicker, SPELL_UNLOCKING);
            me->DespawnOrUnsummon();
        }

        void JustDied(Unit* killer)
        {
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_ENRAGE_KJ)
                {
                    DoCast(me, SPELL_ENRAGE_KJ, true);
                    events.ScheduleEvent(EVENT_ENRAGE_KJ, urand(15000, 20000));
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_korkron_jailerAI(creature);
    }
};

//71742, 71763, 71749
class npc_generic_prisoner : public CreatureScript
{
public:
    npc_generic_prisoner() : CreatureScript("npc_generic_prisoner") {}

    struct npc_generic_prisonerAI : public ScriptedAI
    {
        npc_generic_prisonerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            if (me->GetEntry() == NPC_WATERSPEAKER_GORAI)
                me->setFaction(35);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        InstanceScript* instance;
        EventMap events;

        void Reset(){}

        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_FREEDOM:
                if (Creature* thok = me->GetCreature(*me, instance->GetGuidData(NPC_THOK)))
                    thok->AI()->SetGUID(me->GetGUID(), 2);
                if (me->GetEntry() == NPC_WATERSPEAKER_GORAI)
                {
                    DoCastAOE(SPELL_R_WATERS);
                    events.ScheduleEvent(EVENT_R_WATERS, 11000);
                }
                break;
            case ACTION_RESET:
                me->InterruptNonMeleeSpells(true);
                events.Reset();
                break;
            }
        }

        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_R_WATERS)
                {
                    DoCastAOE(SPELL_R_WATERS);
                    events.ScheduleEvent(EVENT_R_WATERS, 11000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_generic_prisonerAI(creature);
    }
};

//69398
class npc_thok_ice_tomb : public CreatureScript
{
public:
    npc_thok_ice_tomb() : CreatureScript("npc_thok_ice_tomb") {}

    struct npc_thok_ice_tombAI : public ScriptedAI
    {
        npc_thok_ice_tombAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        InstanceScript* instance;
        EventMap events;
        ObjectGuid sGuid;

        void Reset()
        {
            events.Reset();
            sGuid.Clear();
        }

        void IsSummonedBy(Unit* summoner)
        {
            sGuid = summoner->GetGUID();
            events.ScheduleEvent(EVENT_CHECK_TPLAYER, 1000);
        }

        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void JustDied(Unit* killer)
        {
            if (Player* pl = me->GetPlayer(*me, sGuid))
            {
                pl->RemoveAurasDueToSpell(SPELL_FROZEN_SOLID);
                if (GameObject* it = me->FindNearestGameObject(GO_ICE_TOMB, 3.0f))
                    it->Delete();
            }
            me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_CHECK_TPLAYER)
                {
                    if (Player* pl = me->GetPlayer(*me, sGuid))
                    {
                        if (!pl->isAlive())
                        {
                            if (GameObject* it = me->FindNearestGameObject(GO_ICE_TOMB, 3.0f))
                                it->Delete();
                            me->DespawnOrUnsummon();
                        }
                        else
                            events.ScheduleEvent(EVENT_CHECK_TPLAYER, 1000);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_thok_ice_tombAI(creature);
    }
};

//73522
class npc_captive_cave_bat : public CreatureScript
{
public:
    npc_captive_cave_bat() : CreatureScript("npc_captive_cave_bat") {}

    struct npc_captive_cave_batAI : public ScriptedAI
    {
        npc_captive_cave_batAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        InstanceScript* instance;
        EventMap events;

        void Reset(){}

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_VAMPIRIC_FRENZY, urand(10000, 20000));
        }

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_VAMPIRIC_FRENZY)
                {
                    DoCastAOE(SPELL_VAMPIRIC_FRENZY, true);
                    events.ScheduleEvent(EVENT_VAMPIRIC_FRENZY, urand(10000, 20000));
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_captive_cave_batAI(creature);
    }
};

//71787
class npc_body_stalker : public CreatureScript
{
public:
    npc_body_stalker() : CreatureScript("npc_body_stalker") {}

    struct npc_body_stalkerAI : public ScriptedAI
    {
        npc_body_stalkerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetDisplayId(11686);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE |UNIT_FLAG_DISABLE_MOVE);
        }

        InstanceScript* instance;

        void Reset(){}
        
        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff){}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_body_stalkerAI(creature);
    }
};

//71645
class npc_shock_collar : public CreatureScript
{
public:
    npc_shock_collar() : CreatureScript("npc_shock_collar") {}

    struct npc_shock_collarAI : public ScriptedAI
    {
        npc_shock_collarAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetDisplayId(11686);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);
            me->AddAura(SPELL_CANNON_BALL_DESTD, me);
        }

        InstanceScript* instance;

        void Reset(){}

        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff){}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shock_collarAI(creature);
    }
};

//73184
class npc_starved_yeti : public CreatureScript
{
public:
    npc_starved_yeti() : CreatureScript("npc_starved_yeti") {}

    struct npc_starved_yetiAI : public ScriptedAI
    {
        npc_starved_yetiAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        }

        InstanceScript* instance;
        EventMap events;
        SummonList summons;
        float x, y;

        void Reset(){}

        void JustSummoned(Creature* sum)
        {
            summons.Summon(sum);
            events.ScheduleEvent(EVENT_Y_CHARGE, 2000);
        }

        void MovementInform(uint32 type, uint32 pointId)
        {
            if (type == POINT_MOTION_TYPE)
            {
                if (pointId == 1)
                {
                    me->RemoveAurasDueToSpell(SPELL_SWIRL_SEARCHER);
                    summons.DespawnAll();
                    DoCast(me, SPELL_CANNON_BALL, true);
                    if (Creature* bs = me->FindNearestCreature(NPC_BODY_STALKER, 100.0f, true))
                    {
                        float ang1 = me->GetAngle(bs);
                        float ang = ang1 + GetAngleMod();
                        me->SetFacingTo(ang);
                        GetPositionWithDistInOrientation(me, 135.0f, ang, x, y);
                        events.ScheduleEvent(EVENT_PRE_Y_CHARGE, 15000);
                    }
                }
            }
        }

        float GetAngleMod()
        {
            float mod = float(urand(0, 1));
            mod = !mod ? -1 : 1;
            float mod2 = float(urand(1, 5))/10;
            float modangle = mod*mod2;
            return modangle;
        }

        void IsSummonedBy(Unit* summoner)
        {
            GetPositionWithDistInOrientation(me, 135.0f, me->GetOrientation(), x, y);
            me->SummonCreature(NPC_SHOCK_COLLAR, x, y, me->GetPositionZ());
        }

        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_PRE_Y_CHARGE:
                    me->SummonCreature(NPC_SHOCK_COLLAR, x, y, me->GetPositionZ());
                    break;
                case EVENT_Y_CHARGE:
                    if (Creature* sc = me->FindNearestCreature(NPC_SHOCK_COLLAR, 135.0f, true))
                        me->SetFacingToObject(sc);
                    me->GetMotionMaster()->MoveCharge(x, y, me->GetPositionZ(), 30.0f, 1);
                    DoCast(me, SPELL_SWIRL_SEARCHER, true);
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_starved_yetiAI(creature);
    }
};

//143345
class spell_power_regen : public SpellScriptLoader
{
public:
    spell_power_regen() : SpellScriptLoader("spell_power_regen") { }

    class spell_power_regen_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_power_regen_AuraScript);

        void OnPeriodic(AuraEffect const* aurEff)
        {
            if (GetCaster())
            {
                if (GetCaster()->GetPower(POWER_ENERGY) == 100)
                    if (!GetCaster()->HasUnitState(UNIT_STATE_CASTING))
                        GetCaster()->CastSpell(GetCaster(), SPELL_DEAFENING_SCREECH);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_power_regen_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_power_regen_AuraScript();
    }
};

//143430
class spell_clump_check : public SpellScriptLoader
{
public:
    spell_clump_check() : SpellScriptLoader("spell_clump_check") { }

    class spell_clump_check_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_clump_check_SpellScript);

        void HandleOnHit()
        {
            if (GetHitUnit() && GetCaster()->ToCreature())
            {
                if (!GetHitUnit()->HasAura(SPELL_BLOODIED))
                {
                    if (GetHitUnit()->HealthBelowPct(50))
                        GetHitUnit()->AddAura(SPELL_BLOODIED, GetHitUnit());
                }
                else
                {
                    if (GetCaster()->HasAura(SPELL_POWER_REGEN)) //for safe
                    {
                        std::list<Player*> pllist;
                        pllist.clear();
                        GetPlayerListInGrid(pllist, GetHitUnit(), 10.0f);
                        if (!pllist.empty())
                        {
                            uint8 maxcount = GetCaster()->GetMap()->Is25ManRaid() ? 15 : 5;
                            uint8 count = 0;
                            for (std::list<Player*>::const_iterator itr = pllist.begin(); itr != pllist.end(); itr++)
                            {
                                if ((*itr)->HasAura(SPELL_BLOODIED))
                                    count++;

                                if (count >= maxcount)
                                {
                                    if (GetCaster()->HasAura(SPELL_POWER_REGEN))
                                    {
                                        GetCaster()->RemoveAurasDueToSpell(SPELL_POWER_REGEN);
                                        GetCaster()->ToCreature()->AI()->DoAction(ACTION_PHASE_TWO);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_clump_check_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_clump_check_SpellScript();
    }
};

//143445
class spell_fixate : public SpellScriptLoader
{
public:
    spell_fixate() : SpellScriptLoader("spell_fixate") { }

    class spell_fixate_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_fixate_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetCaster() && GetCaster()->ToCreature())
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_FIXATE_IM, true);
                GetCaster()->ClearUnitState(UNIT_STATE_CASTING);
                GetCaster()->ToCreature()->AI()->DoAction(ACTION_START_FIXATE);
            }
        }

        void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes mode)
        {
            if (GetTarget() && GetCaster())
            {
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                { 
                    if (GetCaster()->isAlive() && GetCaster()->HasAura(SPELL_BLOOD_FRENZY) && !GetCaster()->HasAura(SPELL_FIXATE_PR))
                    {
                        GetCaster()->ToCreature()->SetReactState(REACT_PASSIVE);
                        GetCaster()->AttackStop();
                        GetCaster()->StopMoving();
                        GetCaster()->GetMotionMaster()->Clear(false);
                        GetCaster()->getThreatManager().resetAllAggro();
                        GetCaster()->ToCreature()->AI()->DoAction(ACTION_FIXATE);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_fixate_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_fixate_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_fixate_AuraScript();
    }
};

//146589
class spell_unlocking : public SpellScriptLoader
{
public:
    spell_unlocking() : SpellScriptLoader("spell_unlocking") { }

    class spell_unlocking_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_unlocking_AuraScript);

        void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes mode)
        {
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_DEATH)
            {
                for (uint8 n = 0; n < 3; n++)
                {
                    if (Creature* p = GetTarget()->FindNearestCreature(prisonersentry[n], 40.0f, true))
                    {
                        p->NearTeleportTo(fpos[n].GetPositionX(), fpos[n].GetPositionY(), fpos[n].GetPositionZ(), fpos[n].GetOrientation());
                        p->AI()->DoAction(ACTION_FREEDOM);
                        break;
                    }
                }
            }
        }

        void Register()
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_unlocking_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_unlocking_AuraScript();
    }
};

//143800
class spell_icy_blood : public SpellScriptLoader
{
public:
    spell_icy_blood() : SpellScriptLoader("spell_icy_blood") { }

    class spell_icy_blood_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_icy_blood_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (GetTarget() && GetTarget()->HasAura(SPELL_ICY_BLOOD))
            {
                if (GetTarget()->GetAura(SPELL_ICY_BLOOD)->GetStackAmount() >= 5 && !GetTarget()->HasAura(SPELL_FROZEN_SOLID))
                {
                    GetTarget()->AddAura(SPELL_FROZEN_SOLID, GetTarget());
                    GetTarget()->RemoveAurasDueToSpell(SPELL_ICY_BLOOD);
                    GetTarget()->CastSpell(GetTarget(), SPELL_SUMMON_ICE_TOMB, true);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_icy_blood_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_icy_blood_AuraScript();
    }
};

//143773
class spell_freezing_breath : public SpellScriptLoader
{
public:
    spell_freezing_breath() : SpellScriptLoader("spell_freezing_breath") { }

    class spell_freezing_breath_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_freezing_breath_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {
            if (GetTarget() && GetTarget()->HasAura(SPELL_FREEZING_BREATH))
            {
                if (GetTarget()->GetAura(SPELL_FREEZING_BREATH)->GetStackAmount() >= 5 && !GetTarget()->HasAura(SPELL_FROZEN_SOLID))
                {
                    GetTarget()->AddAura(SPELL_FROZEN_SOLID, GetTarget());
                    GetTarget()->RemoveAurasDueToSpell(SPELL_FREEZING_BREATH);
                    GetTarget()->CastSpell(GetTarget(), SPELL_SUMMON_ICE_TOMB, true);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_freezing_breath_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_freezing_breath_AuraScript();
    }
};


void AddSC_boss_thok_the_bloodthirsty()
{
    new boss_thok_the_bloodthirsty();
    new npc_korkron_jailer();
    new npc_generic_prisoner();
    new npc_thok_ice_tomb();
    new npc_captive_cave_bat();
    new npc_body_stalker();
    new npc_shock_collar();
    new npc_starved_yeti();
    new spell_power_regen();
    new spell_clump_check();
    new spell_fixate();
    new spell_unlocking();
    new spell_icy_blood();
    new spell_freezing_breath();
}

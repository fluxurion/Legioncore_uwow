/*
    http://uwow.biz
    Dungeon : Black Rook Hold Dungeon 100-110
    Encounter: The Amalgam of Souls
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_rook_hold_dungeon.h"

enum Says
{
    SAY_AGGRO           = 0,
    SAY_SCYTHE          = 1,
    SAY_SOUL            = 2,
    SAY_REAP            = 3,
    SAY_DEATH           = 4,
};

enum Spells
{
    SPELL_SWIRLING_SCYTHE       = 195254,
    SPELL_SOUL_ECHOES           = 194966,
    SPELL_REAP_SOUL             = 194956,

    SPELL_SUMMON_MINIBOSS_A     = 196619, //Суммонятся что бы открыть двери, после смерти босса
    SPELL_SUMMON_MINIBOSS_B     = 196620,
    SPELL_SUMMON_MINIBOSS_C     = 196646,
    SPELL_SUMMON_CONVERSATION   = 197075,

    SPELL_SOUL_ECHOES_CLONE_PLR = 194981,
    SPELL_SOUL_ECHOES_DMG       = 194960,
};

enum eEvents
{
    EVENT_SWIRLING_SCYTHE       = 1,
    EVENT_SOUL_ECHOES           = 2,
    EVENT_REAP_SOUL             = 3,
};

enum eAnim //SMSG_PLAY_ONE_SHOT_ANIM_KIT
{
    STALKER_ANIM_1 = 9038,
    STALKER_ANIM_2 = 9039,
    STALKER_ANIM_3 = 9176
};

uint32 AnimRand[3] =
{
    STALKER_ANIM_1,
    STALKER_ANIM_2,
    STALKER_ANIM_3
};

class boss_the_amalgam_of_souls : public CreatureScript
{
public:
    boss_the_amalgam_of_souls() : CreatureScript("boss_the_amalgam_of_souls")  {}

    struct boss_the_amalgam_of_soulsAI : public BossAI
    {
        boss_the_amalgam_of_soulsAI(Creature* creature) : BossAI(creature, DATA_AMALGAM) 
        {
          if (me->isAlive())
            instance->SetData(DATA_AMALGAM_OUTRO, IN_PROGRESS);   
        }
        

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* /*who*/) //58:07
        {
            Talk(SAY_AGGRO); //Consume! Devour!
            _EnterCombat();

            events.ScheduleEvent(EVENT_SWIRLING_SCYTHE, 8000); //58:15, 58:36, 58:59
            events.ScheduleEvent(EVENT_SOUL_ECHOES, 14000); //58:22, 58:50, 59:18, 59:45
            events.ScheduleEvent(EVENT_REAP_SOUL, 20000); //58:27, 58:40, 58:54, 59:07
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
            DoCast(SPELL_SUMMON_MINIBOSS_C);
            DoCast(SPELL_SUMMON_MINIBOSS_B);
            DoCast(SPELL_SUMMON_MINIBOSS_A);
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
                    case EVENT_SWIRLING_SCYTHE:
                        Talk(SAY_SCYTHE);
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(pTarget, SPELL_SWIRLING_SCYTHE);
                        events.ScheduleEvent(EVENT_SWIRLING_SCYTHE, 20000);
                        break;
                    case EVENT_SOUL_ECHOES:
                        Talk(SAY_SOUL);
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                            DoCast(pTarget, SPELL_SOUL_ECHOES);
                        events.ScheduleEvent(EVENT_SOUL_ECHOES, 28000);
                        break;
                    case EVENT_REAP_SOUL:
                        Talk(SAY_REAP);
                        if (me->getVictim())
                            DoCast(me->getVictim(), SPELL_REAP_SOUL);
                        events.ScheduleEvent(EVENT_REAP_SOUL, 13000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_the_amalgam_of_soulsAI(creature);
    }
};

//99090
class npc_soul_echoes_stalker : public CreatureScript
{
public:
    npc_soul_echoes_stalker() : CreatureScript("npc_soul_echoes_stalker") {}

    struct npc_soul_echoes_stalkerAI : public ScriptedAI
    {
        npc_soul_echoes_stalkerAI(Creature* creature) : ScriptedAI(creature) 
        {
            me->SetReactState(REACT_PASSIVE);
        }

        EventMap events;

        void Reset() {}

        void IsSummonedBy(Unit* summoner)
        {
            summoner->CastSpell(me, SPELL_SOUL_ECHOES_CLONE_PLR, true);
            me->PlayOneShotAnimKit(AnimRand[urand(STALKER_ANIM_1, STALKER_ANIM_3)]);
            events.ScheduleEvent(EVENT_1, 4000);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_1)
                    DoCast(me, SPELL_SOUL_ECHOES_DMG, true);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_soul_echoes_stalkerAI(creature);
    }
};

class npc_soul_echoes_outro : public CreatureScript
{
public:
    npc_soul_echoes_outro() : CreatureScript("npc_soul_echoes_outro") {}

    struct npc_soul_echoes_outroAI : public ScriptedAI
    {
        npc_soul_echoes_outroAI(Creature* creature) : ScriptedAI(creature) 
        {
           first = false;
           two = false;
           three = false;
           instance = me->GetInstanceScript();
           instance->SetData(DATA_AMALGAM_OUTRO, IN_PROGRESS);
           outro = false;
        }
         EventMap events;
         InstanceScript* instance;
         bool first;
         bool two;
         bool three;
         bool outro;
         
         void IsSummonedBy(Unit* summoner)
         {
           events.ScheduleEvent(EVENT_3, 10);
         }
         void MovementInform(uint32 type, uint32 id)
         {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    case 1:
                     instance->SetData(DATA_AMALGAM_OUTRO, 0);
                     Talk(0);
                     break;
                    case 2:
                     instance->SetData(DATA_AMALGAM_OUTRO, 0);
                     break;
                    case 3:
                     instance->SetData(DATA_AMALGAM_OUTRO, 0);
                     break;
                }
         }
         
        void UpdateAI(uint32 diff)
        {
            events.Update(diff);
            
            if (instance->GetData(DATA_AMALGAM_OUTRO) >= 3 && !outro)
            {
               outro = true;
               if (me->GetEntry() == 99857)
                     me->CastSpell(me, 205212);              
               if (me->GetEntry() == 99426)
                     me->CastSpell(me, 205210);
               if (me->GetEntry() == 99858)
                  {
                      std::list<Creature*> list;
                      list.clear();
                      me->GetCreatureListWithEntryInGrid(list, 103662, 30.0f);
                      if (!list.empty())
                          for (std::list<Creature*>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                             if (!(*itr)->HasAura(205210) && !(*itr)->HasAura(205212))
                                me->CastSpell((*itr), 205211, true);
                  }
               events.ScheduleEvent(EVENT_2, 5000);
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                   case EVENT_1:
                     if (me->GetEntry() == 99857)
                        me->GetMotionMaster()->MovePoint(1, 3226.46f, 7553.97f, 15.26f);                     
                     if (me->GetEntry() == 99858)
                        me->GetMotionMaster()->MovePoint(2, 3228.56f, 7548.91f, 14.85f);                     
                     if (me->GetEntry() == 99426)
                        me->GetMotionMaster()->MovePoint(3, 3233.58f, 7548.09f, 15.04f);
                     break;
                   case EVENT_2:
                     me->CastStop();
                     instance->SetData(DATA_AMALGAM_OUTRO, DONE);
                     me->DespawnOrUnsummon(2000);
                     break;          
                   case EVENT_3:
                     if (me->GetEntry() == 99857)
                       DoCast(SPELL_SUMMON_CONVERSATION);        
                     events.ScheduleEvent(EVENT_1, 7000);
                     break;
                }
                   
            }
        }         
  
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_soul_echoes_outroAI(creature);
    }
};

void AddSC_boss_the_amalgam_of_souls()
{
    new boss_the_amalgam_of_souls();
    new npc_soul_echoes_stalker();
    new npc_soul_echoes_outro();
}
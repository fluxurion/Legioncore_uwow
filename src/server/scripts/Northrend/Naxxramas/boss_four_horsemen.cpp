/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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

#include "naxxramas.h"

enum Horsemen
{
    HORSEMEN_THANE,
    HORSEMEN_LADY,
    HORSEMEN_BARON,
    HORSEMEN_SIR,
};

enum Events
{
    EVENT_NONE,
    EVENT_MARK,
    EVENT_CAST,
    EVENT_BERSERK,
};

const Position WaypointPositions[12] =
{
    // Thane waypoints
    {2542.3f, -2984.1f, 241.49f, 5.362f},
    {2547.6f, -2999.4f, 241.34f, 5.049f},
    {2542.9f, -3015.0f, 241.35f, 4.654f},
    // Lady waypoints
    {2498.3f, -2961.8f, 241.28f, 3.267f},
    {2487.7f, -2959.2f, 241.28f, 2.890f},
    {2469.4f, -2947.6f, 241.28f, 2.576f},
    // Baron waypoints
    {2553.8f, -2968.4f, 241.33f, 5.757f},
    {2564.3f, -2972.5f, 241.33f, 5.890f},
    {2583.9f, -2971.67f, 241.35f, 0.008f},
    // Sir waypoints
    {2534.5f, -2921.7f, 241.53f, 1.363f},
    {2523.5f, -2902.8f, 241.28f, 2.095f},
    {2517.8f, -2896.6f, 241.28f, 2.315f},
};

const uint32 MOB_HORSEMEN[]     =   {16064, 16065, 30549, 16063};
const uint32 SPELL_MARK[]       =   {28832, 28833, 28834, 28835};
#define SPELL_PRIMARY(i)            RAID_MODE(SPELL_PRIMARY_N[i], SPELL_PRIMARY_H[i])
const uint32 SPELL_PRIMARY_N[]  =   {28884, 28863, 28882, 28883};
const uint32 SPELL_PRIMARY_H[]  =   {57467, 57463, 57369, 57466};
#define SPELL_SECONDARY(i)          RAID_MODE(SPELL_SECONDARY_N[i], SPELL_SECONDARY_H[i])
const uint32 SPELL_SECONDARY_N[]=   {0, 57374, 0, 57376};
const uint32 SPELL_SECONDARY_H[]=   {0, 57464, 0, 57465};
const uint32 SPELL_PUNISH[]     =   {0, 57381, 0, 57377};
#define SPELL_BERSERK               26662

// used by 16063, 16064, 16065, 30549, but signed for 16063
const int32 SAY_AGGRO[]     =   {-1533051, -1533044, -1533065, -1533058};
const int32 SAY_TAUNT[3][4] ={  {-1533052, -1533045, -1533071, -1533059},
                                {-1533053, -1533046, -1533072, -1533060},
                                {-1533054, -1533047, -1533073, -1533061}, };
const int32 SAY_SPECIAL[]   =   {-1533055, -1533048, -1533070, -1533062};
const int32 SAY_SLAY[]      =   {-1533056, -1533049, -1533068, -1533063};
const int32 SAY_DEATH[]     =   {-1533057, -1533050, -1533074, -1533064};

#define SAY_BARON_AGGRO     RAND(-1533065, -1533066, -1533067)
#define SAY_BARON_SLAY      RAND(-1533068, -1533069)

class boss_four_horsemen : public CreatureScript
{
public:
    boss_four_horsemen() : CreatureScript("boss_four_horsemen") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_four_horsemenAI (creature);
    }

    struct boss_four_horsemenAI : public BossAI
    {
        boss_four_horsemenAI(Creature* c) : BossAI(c, BOSS_HORSEMEN)
        {
            id = Horsemen(0);
            for (uint8 i = 0; i < 4; ++i)
                if (me->GetEntry() == MOB_HORSEMEN[i])
                    id = Horsemen(i);
            caster = (id == HORSEMEN_LADY || id == HORSEMEN_SIR);
        }

        Horsemen id;
        ObjectGuid uiEventStarterGUID;
        uint8 nextWP;
        uint32 punishTimer;
        bool caster;
        bool nextMovementStarted;
        bool movementCompleted;
        bool movementStarted;
        bool encounterActionAttack;
        bool encounterActionReset;
        bool doDelayPunish;

        void Reset()
        {
            if (!encounterActionReset)
                DoEncounterAction(NULL, false, true, false);

            if (instance)
                instance->SetData(DATA_HORSEMEN0 + id, NOT_STARTED);

            me->SetReactState(REACT_AGGRESSIVE);
            uiEventStarterGUID.Clear();
            nextWP = 0;
            punishTimer = 10000;
            nextMovementStarted = false;
            movementCompleted = false;
            movementStarted = false;
            encounterActionAttack = false;
            encounterActionReset = false;
            doDelayPunish = false;
            _Reset();
        }

        bool DoEncounterAction(Unit* who, bool attack, bool reset, bool checkAllDead)
        {
            if (!instance)
                return false;

            Creature* Thane = Unit::GetCreature(*me, instance->GetGuidData(DATA_THANE));
            Creature* Lady = Unit::GetCreature(*me, instance->GetGuidData(DATA_LADY));
            Creature* Baron = Unit::GetCreature(*me, instance->GetGuidData(DATA_BARON));
            Creature* Sir = Unit::GetCreature(*me, instance->GetGuidData(DATA_SIR));

            if (Thane && Lady && Baron && Sir)
            {
                if (attack && who)
                {
                    if (boss_four_horsemen::boss_four_horsemenAI* ThaneAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Thane->GetAI()))
                    {
                        ThaneAI->encounterActionAttack = true;
                        ThaneAI->AttackStart(who);
                    }
                    if (boss_four_horsemen::boss_four_horsemenAI* LadyAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Lady->GetAI()))
                    {
                        LadyAI->encounterActionAttack = true;
                        LadyAI->AttackStart(who);
                    }
                    if (boss_four_horsemen::boss_four_horsemenAI* BaronAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Baron->GetAI()))
                    {
                        BaronAI->encounterActionAttack = true;
                        BaronAI->AttackStart(who);
                    }
                    if (boss_four_horsemen::boss_four_horsemenAI* SirAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Sir->GetAI()))
                    {
                        SirAI->encounterActionAttack = true;
                        SirAI->AttackStart(who);
                    }
                }

                if (reset)
                {
                    if (instance->GetBossState(BOSS_HORSEMEN) != NOT_STARTED)
                    {
                        if (!Thane->isAlive())
                            Thane->Respawn();

                        if (!Lady->isAlive())
                            Lady->Respawn();

                        if (!Baron->isAlive())
                            Baron->Respawn();

                        if (!Sir->isAlive())
                            Sir->Respawn();

                        if (boss_four_horsemen::boss_four_horsemenAI* ThaneAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Thane->GetAI()))
                        {
                            ThaneAI->encounterActionReset = true;
                            ThaneAI->EnterEvadeMode();
                        }
                        if (boss_four_horsemen::boss_four_horsemenAI* LadyAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Lady->GetAI()))
                        {
                            LadyAI->encounterActionReset = true;
                            LadyAI->EnterEvadeMode();
                        }
                        if (boss_four_horsemen::boss_four_horsemenAI* BaronAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Baron->GetAI()))
                        {
                            BaronAI->encounterActionReset = true;
                            BaronAI->EnterEvadeMode();
                        }
                        if (boss_four_horsemen::boss_four_horsemenAI* SirAI = CAST_AI(boss_four_horsemen::boss_four_horsemenAI, Sir->GetAI()))
                        {
                            SirAI->encounterActionReset = true;
                            SirAI->EnterEvadeMode();
                        }
                    }
                }

                if (checkAllDead)
                    return !Thane->isAlive() && !Lady->isAlive() && !Baron->isAlive() && !Sir->isAlive();
            }
            return false;
        }

        void BeginFourHorsemenMovement()
        {
            movementStarted = true;
            me->SetReactState(REACT_PASSIVE);
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            me->SetSpeed(MOVE_RUN, me->GetSpeedRate(MOVE_RUN), true);

            switch (id)
            {
                case HORSEMEN_THANE:
                    me->GetMotionMaster()->MovePoint(0, WaypointPositions[0]);
                    break;
                case HORSEMEN_LADY:
                    me->GetMotionMaster()->MovePoint(3, WaypointPositions[3]);
                    break;
                case HORSEMEN_BARON:
                    me->GetMotionMaster()->MovePoint(6, WaypointPositions[6]);
                    break;
                case HORSEMEN_SIR:
                    me->GetMotionMaster()->MovePoint(9, WaypointPositions[9]);
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 2 || id == 5 || id == 8 || id == 11)
            {
                movementCompleted = true;
                me->SetReactState(REACT_AGGRESSIVE);

                Unit* eventStarter = Unit::GetUnit(*me, uiEventStarterGUID);

                if (eventStarter)
                    AttackStart(eventStarter);
                else if (!UpdateVictim())
                {
                    EnterEvadeMode();
                    return;
                }

                if (caster)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveIdle();
                }

                return;
            }

            nextMovementStarted = false;
            nextWP = id + 1;
        }

        // switch to "who" if nearer than current target.
        void SelectNearestTarget(Unit* who)
        {
            if (me->getVictim() && me->GetDistanceOrder(who, me->getVictim()))
            {
                me->getThreatManager().modifyThreatPercent(me->getVictim(), -100);
                me->AddThreat(who, 1000000.0f);
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            BossAI::MoveInLineOfSight(who);
            if (caster)
                SelectNearestTarget(who);
        }

        void AttackStart(Unit* who)
        {
            if (!movementCompleted && !movementStarted)
            {
                uiEventStarterGUID = who->GetGUID();
                BeginFourHorsemenMovement();

                if (!encounterActionAttack)
                    DoEncounterAction(who, true, false, false);
            }
            else if (movementCompleted && movementStarted)
            {
                if (caster)
                    me->Attack(who, false);
                else
                    BossAI::AttackStart(who);
            }
        }

        void KilledUnit(Unit* /*victim*/)
        {
            if (!(rand()%5))
            {
                if (id == HORSEMEN_BARON)
                    DoScriptText(SAY_BARON_SLAY, me);
                else
                    DoScriptText(SAY_SLAY[id], me);
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            events.Reset();
            summons.DespawnAll();

            if (instance)
                instance->SetData(DATA_HORSEMEN0 + id, DONE);

            if (instance && DoEncounterAction(NULL, false, false, true))
            {
                instance->SetBossState(BOSS_HORSEMEN, DONE);
                instance->SaveToDB();

                // Achievements related to the 4-horsemen are given through spell 59450 which does not exist.
                // There is thus no way it can be given by casting the spell on the players.
                instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 59450);
            }

            DoScriptText(SAY_DEATH[id], me);
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            if (id == HORSEMEN_BARON)
                DoScriptText(SAY_BARON_AGGRO, me);
            else
                DoScriptText(SAY_AGGRO[id], me);

            events.ScheduleEvent(EVENT_MARK, 15000);
            events.ScheduleEvent(EVENT_CAST, 20000+rand()%5000);
            events.ScheduleEvent(EVENT_BERSERK, 15*100*1000);
        }

        void UpdateAI(uint32 diff)
        {
            if (nextWP && movementStarted && !movementCompleted && !nextMovementStarted)
            {
                nextMovementStarted = true;
                me->GetMotionMaster()->MovePoint(nextWP, WaypointPositions[nextWP]);
            }

            if (!UpdateVictim() || !CheckInRoom() || !movementCompleted)
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MARK:
                        if (!(rand()%5))
                            DoScriptText(SAY_SPECIAL[id], me);
                        DoCastAOE(SPELL_MARK[id]);
                        events.ScheduleEvent(EVENT_MARK, 15000);
                        break;
                    case EVENT_CAST:
                        if (!(rand()%5))
                            DoScriptText(SAY_TAUNT[rand()%3][id], me);

                        if (caster)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 45.0f))
                                DoCast(target, SPELL_PRIMARY(id));
                        }
                        else
                            DoCast(me->getVictim(), SPELL_PRIMARY(id));

                        events.ScheduleEvent(EVENT_CAST, 15000);
                        break;
                    case EVENT_BERSERK:
                        DoScriptText(SAY_SPECIAL[id], me);
                        DoCast(me, SPELL_BERSERK);
                        break;
                }
            }

            if (punishTimer <= diff)
            {
                if (doDelayPunish)
                {
                    // try to find a new target
                    Unit *pTemp = NULL;

                    std::list<Unit *> playerList;
                    SelectTargetList(playerList, 10, SELECT_TARGET_NEAREST, 45);
                    for (std::list<Unit*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                    {
                        pTemp = (*itr);
                        if (me->IsWithinLOSInMap(pTemp) && !pTemp->IsImmunedToDamage(SPELL_SCHOOL_MASK_ALL))
                        {
                            SelectNearestTarget(pTemp);
                            break;
                        }
                        pTemp = NULL;
                    }

                    if (!pTemp)
                        DoCastAOE(SPELL_PUNISH[id], true);

                    doDelayPunish = false;
                }
                punishTimer = 2000;
            } else punishTimer -= diff;

            if (!caster)
                DoMeleeAttackIfReady();
            else if ((!DoSpellAttackIfReady(SPELL_SECONDARY(id)) || !me->IsWithinLOSInMap(me->getVictim())) && movementCompleted && !doDelayPunish)
                doDelayPunish = true;
        }
    };

};

void AddSC_boss_four_horsemen()
{
    new boss_four_horsemen();
}

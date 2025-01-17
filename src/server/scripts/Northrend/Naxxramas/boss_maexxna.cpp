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

enum Emotes
{
    EMOTE_SPIN_WEB          = -1533146,
    EMOTE_SPIDERLING        = -1533147,
    EMOTE_SPRAY             = -1533148
};
enum Spells
{
    SPELL_WEB_WRAP              = 28622,
    SPELL_WEB_SPRAY_10          = 29484,
    SPELL_WEB_SPRAY_25          = 54125,
    SPELL_POISON_SHOCK_10       = 28741,
    SPELL_POISON_SHOCK_25       = 54122,
    SPELL_NECROTIC_POISON_10    = 54121,
    SPELL_NECROTIC_POISON_25    = 28776,
    SPELL_FRENZY_10             = 54123,
    SPELL_FRENZY_25             = 54124,
};

enum Creatures
{
    MOB_WEB_WRAP                = 16486,
    MOB_SPIDERLING              = 17055,
};

#define MAX_POS_WRAP            3
const Position PosWrap[MAX_POS_WRAP] =
{
    {3546.796f, -3869.082f, 296.450f, 0.0f},
    {3531.271f, -3847.424f, 299.450f, 0.0f},
    {3497.067f, -3843.384f, 302.384f, 0.0f},
};

enum Events
{
    EVENT_NONE,
    EVENT_SPRAY,
    EVENT_SHOCK,
    EVENT_POISON,
    EVENT_WRAP,
    EVENT_SUMMON,
    EVENT_FRENZY,
};

class boss_maexxna : public CreatureScript
{
public:
    boss_maexxna() : CreatureScript("boss_maexxna") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_maexxnaAI (pCreature);
    }

    struct boss_maexxnaAI : public BossAI
    {
        boss_maexxnaAI(Creature *c) : BossAI(c, BOSS_MAEXXNA) {}

        bool enraged;

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
            enraged = false;
            events.ScheduleEvent(EVENT_WRAP, 20000);
            events.ScheduleEvent(EVENT_SPRAY, 40000);
            events.ScheduleEvent(EVENT_SHOCK, urand(5000,10000));
            events.ScheduleEvent(EVENT_POISON, urand(10000,15000));
            events.ScheduleEvent(EVENT_SUMMON, 30000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() || !CheckInRoom())
                return;

            if (!enraged && HealthBelowPct(30))
            {
                enraged = true;
                events.ScheduleEvent(EVENT_FRENZY, 0); // will be cast immediately
            }

            if (me->IsNonMeleeSpellCasted(false))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WRAP:
                        DoScriptText(EMOTE_SPIN_WEB, me);
                        for (uint8 i = 0; i < RAID_MODE(1,2); ++i)
                        {
                            if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 0, true, -SPELL_WEB_WRAP))
                            {
                                pTarget->RemoveAura(RAID_MODE(SPELL_WEB_SPRAY_10,SPELL_WEB_SPRAY_25));
                                uint8 pos = rand()%MAX_POS_WRAP;
                                pTarget->GetMotionMaster()->MoveJump(PosWrap[pos].GetPositionX(), PosWrap[pos].GetPositionY(), PosWrap[pos].GetPositionZ(), 20, 20);
                                if (Creature *wrap = DoSummon(MOB_WEB_WRAP, PosWrap[pos], 0, TEMPSUMMON_CORPSE_DESPAWN))
                                    wrap->AI()->SetGUID(pTarget->GetGUID());
                            }
                        }
                        events.ScheduleEvent(EVENT_WRAP, 40000);
                        break;
                    case EVENT_SPRAY:
                        DoScriptText(EMOTE_SPRAY, me);
                        DoCastAOE(RAID_MODE(SPELL_WEB_SPRAY_10,SPELL_WEB_SPRAY_25));
                        events.ScheduleEvent(EVENT_SPRAY, 40000);
                        break;
                    case EVENT_SHOCK:
                        DoCastAOE(RAID_MODE(SPELL_POISON_SHOCK_10,SPELL_POISON_SHOCK_25));
                        events.ScheduleEvent(EVENT_SHOCK, urand(10000,20000));
                        break;
                    case EVENT_POISON:
                        DoCast(me->getVictim(), RAID_MODE(SPELL_NECROTIC_POISON_10,SPELL_NECROTIC_POISON_25));
                        events.ScheduleEvent(EVENT_POISON, urand(10000, 20000));
                        break;
                    case EVENT_FRENZY:
                        DoCast(me, RAID_MODE(SPELL_FRENZY_10,SPELL_FRENZY_25), true);
                        events.ScheduleEvent(EVENT_FRENZY, 600000);
                        break;
                    case EVENT_SUMMON:
                        DoScriptText(EMOTE_SPIDERLING, me);
                        uint8 amount = urand(8,10);
                        for (uint8 i = 0; i < amount; ++i)
                            DoSummon(MOB_SPIDERLING, me, 0, TEMPSUMMON_CORPSE_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON, 40000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class mob_webwrap : public CreatureScript
{
public:
    mob_webwrap() : CreatureScript("mob_webwrap") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_webwrapAI (pCreature);
    }

    struct mob_webwrapAI : public NullCreatureAI
    {
        mob_webwrapAI(Creature *c) : NullCreatureAI(c), victimGUID() {}

        ObjectGuid victimGUID;

        void SetGUID(ObjectGuid const& guid, int32 /*param*/)
        {
            victimGUID = guid;
            if (me->m_temlate_spells[0] && victimGUID)
                if (Unit* victim = Unit::GetUnit(*me, victimGUID))
                    victim->CastSpell(victim, me->m_temlate_spells[0], true, NULL, NULL, me->GetGUID());
        }

        void JustDied(Unit* /*killer*/)
        {
            if (me->m_temlate_spells[0] && victimGUID)
                if (Unit* victim = Unit::GetUnit(*me, victimGUID))
                    victim->RemoveAurasDueToSpell(me->m_temlate_spells[0], me->GetGUID());
        }
    };

};

void AddSC_boss_maexxna()
{
    new boss_maexxna();
    new mob_webwrap();
}

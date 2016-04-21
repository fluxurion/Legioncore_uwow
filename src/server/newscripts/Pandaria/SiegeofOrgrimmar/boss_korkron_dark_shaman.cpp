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
    SPELL_BERSERK               = 26662,

    //Shamans
    SPELL_BLOODLUST             = 144302,
    SPELL_POISONMIST_TOTEM      = 144288,
    SPELL_FOULSTREAM_TOTEM      = 144289,
    SPELL_ASHFLARE_TOTEM        = 144290,

    //Kardris
    SPELL_FROSTSTORM_BOLT       = 144214,
    //95pct HM
    SPELL_IRON_PRISON           = 144330, 
    //85pct
    SPELL_TOXIC_STORM_SUM       = 144005,
    SPELL_TOXIC_STORM_TR_AURA   = 144006,
    SPELL_TOXIC_TORNADO_SUM     = 144019,
    SPELL_TOXIC_TORNADO_TR_AURA = 144029,
    //65pct
    SPELL_FOUL_GEYSER           = 143990,
    //50pct
    SPELL_FALLING_ASH_SUM       = 143973,
    SPELL_FALLING_ASH_DMG       = 143987,
    SPELL_FALLING_ASH_COSMETIC  = 143986,

    //Haromm
    SPELL_FROSTSTORM_STRIKE     = 144215,
    //95pct HM
    SPELL_IRON_TOMB_SUM         = 144329, 
    SPELL_IRON_TOMB_DMG         = 144334,
    //85pct
    SPELL_TOXIC_MIST            = 144089,
    //65pct
    SPELL_FOUL_STREAM           = 144090,
    //50pct
    SPELL_ASH_ELEMENTAL_SPAWN   = 144222,
    SPELL_ASHEN_WALL            = 144071,

    //Mount
    SPELL_SWIPE                 = 144303,
    SPELL_REND                  = 144304,

    //Other Creature
    SPELL_FOULNESS              = 144064,
};

enum CreatureText
{
    SAY_PULL                    = 0,
    SAY_FIRSTATTACK             = 1,
    SAY_POISONMIST_TOTEM        = 2,
    SAY_ASHFLARE_TOTEM          = 3,
    SAY_BLOODLUST               = 4,
    SAY_FOULSTREAM_TOTEM        = 5,
};

enum sEvents
{
    //Shamans
    //Kardris
    EVENT_FROSTSTORM_BOLT       = 1,
    EVENT_TOXIC_STORM           = 2,
    EVENT_FOUL_GEYSER           = 3,
    EVENT_FALLING_ASH           = 4,
    //HM
    EVENT_IRON_PRISON           = 5,

    //Haromm
    EVENT_FROSTSTORM_STRIKE     = 6,
    EVENT_TOXIC_MIST            = 7,
    EVENT_FOUL_STREAM           = 8,
    EVENT_ASHEN_WALL            = 9,
    //HM
    EVENT_IRON_TOMB             = 10,

    //Mounts
    EVENT_SWIPE                 = 11,
    EVENT_REND                  = 12,

    EVENT_SUMMON_TORNADO        = 13,
    EVENT_ACTIVE                = 14,
    EVENT_DESPAWN               = 15,

    EVENT_BERSERK               = 16,
};

enum Data
{
    DATA_GET_PULL_STATE         = 1,
};

Position const spawnpos[4] =
{
    {1668.15f, -4354.63f, 26.3788f, 2.87635f},
    {1658.76f, -4337.2f,  26.3985f, 4.43536f},
    {1605.02f, -4384.74f, 20.4198f,  3.6096f},
    {1598.41f, -4372.17f, 21.1200f,  3.6896f},
};

struct  SpecialModifier
{
    float dist, ang;
};

static SpecialModifier mod[] =
{
    { 5.0f,  1.570796326795f },
    { 10.0f, 1.570796326795f },
    { 15.0f, 1.570796326795f },
    { 20.0f, 1.570796326795f },
    { 5.0f,  M_PI + 1.570796326795f },
    { 10.0f, M_PI + 1.570796326795f },
    { 15.0f, M_PI + 1.570796326795f },
    { 20.0f, M_PI + 1.570796326795f },
};

class boss_korkron_dark_shaman : public CreatureScript
{
public:
    boss_korkron_dark_shaman() : CreatureScript("boss_korkron_dark_shaman") {}

    struct boss_korkron_dark_shamanAI : public ScriptedAI
    {
        boss_korkron_dark_shamanAI(Creature* creature) : ScriptedAI(creature), summon(me)
        {
            instance = creature->GetInstanceScript();
            firstpull = true;
        }

        InstanceScript* instance;
        SummonList summon;
        EventMap events;
        uint8 nextpct;
        bool firstpull, firstattack;
        
        void Reset()
        {
            if (instance)
            {
                events.Reset();
                summon.DespawnAll();
                DespawnAllSummons();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                instance->SetBossState(DATA_KORKRON_D_SHAMAN, NOT_STARTED);
                if (firstpull)
                    SummonAndSeatOnMount(me->GetEntry());
                if (me->GetMap()->IsHeroic())
                    nextpct = 95;
                else
                    nextpct = 85;
                firstattack = false;
            }
        }

        Creature* GetOtherShaman()
        {
            if (instance)
            { 
                if (Creature* oshaman = me->GetCreature(*me, instance->GetGuidData(me->GetEntry() == NPC_WAVEBINDER_KARDRIS ? NPC_EARTHBREAKER_HAROMM : NPC_WAVEBINDER_KARDRIS)))
                {
                    if (oshaman->isAlive())
                        return oshaman;
                }
            }
            return NULL;
        }

        void DespawnAllSummons() //Despawn special creature and gameobject(summons from triggers)
        {
            std::list<Creature*> list;
            list.clear();
            me->GetCreatureListWithEntryInGrid(list, 71827, 200.0f); //ash elemental
            me->GetCreatureListWithEntryInGrid(list, 71817, 200.0f); //toxic tornado
            if (!list.empty())
            {
                for (std::list<Creature*>::const_iterator itr = list.begin(); itr != list.end(); itr++)
                    (*itr)->DespawnOrUnsummon();
            }

            if (me->GetMap()->IsHeroic())
            {
                std::list<GameObject*>glist;
                glist.clear();
                me->GetGameObjectListWithEntryInGrid(glist, 220864, 200.0f); //iron tomb
                if (!glist.empty())
                {
                    for (std::list<GameObject*>::const_iterator itr = glist.begin(); itr != glist.end(); itr++)
                        (*itr)->Delete();
                }
            }
        }

        void JustReachedHome()
        {
            if (!firstpull)
                SummonAndSeatOnMount(me->GetEntry());
        }

        void SummonAndSeatOnMount(uint32 entry)
        {
            uint32 mauntentry = 0;
            switch (entry)
            {
            case NPC_WAVEBINDER_KARDRIS:
                mauntentry = NPC_BLOODCLAW;
                break;
            case NPC_EARTHBREAKER_HAROMM:
                mauntentry = NPC_DARKFANG;
                break;
            }
            if (Creature* mount = me->SummonCreature(mauntentry, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
                me->EnterVehicle(mount->ToUnit(), 0);
        }
        
        void EnterCombat(Unit* who)
        {
            if (instance)
                instance->SetBossState(DATA_KORKRON_D_SHAMAN, IN_PROGRESS);

            if (firstpull)
            {
                firstpull = false;
                switch (me->GetEntry())
                {
                case NPC_WAVEBINDER_KARDRIS:
                    me->SetHomePosition(spawnpos[2]);
                    break;
                case NPC_EARTHBREAKER_HAROMM:
                    me->SetHomePosition(spawnpos[3]);
                    break;
                }
            }
            Talk(SAY_PULL);
            events.ScheduleEvent(EVENT_BERSERK, 600000);
            events.ScheduleEvent(me->GetEntry() == NPC_WAVEBINDER_KARDRIS ? EVENT_FROSTSTORM_BOLT : EVENT_FROSTSTORM_STRIKE, 6000);
        }

        void EnterEvadeMode()
        {
            ScriptedAI::EnterEvadeMode();
            if (Creature* oshaman = GetOtherShaman())
            {
                if (oshaman->isInCombat())
                    oshaman->AI()->EnterEvadeMode();
            }
        }
        
        void DamageTaken(Unit* attacker, uint32 &damage)
        {
            if (Creature* oshaman = GetOtherShaman())
            {
                if (damage >= me->GetHealth())
                    oshaman->Kill(oshaman, true);
                else
                    oshaman->SetHealth(oshaman->GetHealth() - damage);
            }
        }
        
        void SetExtraEvents(uint8 phase)
        {
            switch (phase)
            {
            case 85: //95pct HM
            {
                switch (me->GetEntry())
                {
                case NPC_WAVEBINDER_KARDRIS:
                    events.ScheduleEvent(EVENT_IRON_PRISON, 2000);
                    break;
                case NPC_EARTHBREAKER_HAROMM:
                    events.ScheduleEvent(EVENT_IRON_TOMB, 2000);
                    break;
                }
            }
            break;
            case 65: //85pct
            {
                switch (me->GetEntry())
                {
                case NPC_WAVEBINDER_KARDRIS:
                    Talk(SAY_POISONMIST_TOTEM);
                    DoCast(me, SPELL_POISONMIST_TOTEM, true);
                    events.ScheduleEvent(EVENT_TOXIC_STORM, 5000);
                    break;
                case NPC_EARTHBREAKER_HAROMM:
                    events.ScheduleEvent(EVENT_TOXIC_MIST, 2000);
                    break;
                }
            }
            break;
            case 50: //65pct
            {
                switch (me->GetEntry())
                {
                case NPC_WAVEBINDER_KARDRIS:
                    DoCast(me, SPELL_FOULSTREAM_TOTEM, true);
                    events.ScheduleEvent(EVENT_FOUL_GEYSER, 2000);
                    break;
                case NPC_EARTHBREAKER_HAROMM:
                    Talk(SAY_FOULSTREAM_TOTEM);
                    events.ScheduleEvent(EVENT_FOUL_STREAM, 1000);
                    break;
                }
            }
            break;
            case 25: //50pct
            {
                switch (me->GetEntry())
                {
                case NPC_WAVEBINDER_KARDRIS:
                    Talk(SAY_ASHFLARE_TOTEM);
                    DoCast(me, SPELL_ASHFLARE_TOTEM, true);
                    //events.ScheduleEvent(EVENT_FALLING_ASH, 20000); not works
                    break;
                case NPC_EARTHBREAKER_HAROMM:
                    events.ScheduleEvent(EVENT_ASHEN_WALL, 2000);
                    break;
                }
            }
            break;
            case 0: //25pct
                Talk(SAY_BLOODLUST);
                me->AddAura(SPELL_BLOODLUST, me);
                break;
            }
        }
        
        void JustDied(Unit* killer)
        {
            summon.DespawnAll();
            DespawnAllSummons();
            if (killer == me && instance)
            {
                me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                instance->SetBossState(DATA_KORKRON_D_SHAMAN, DONE);
            }
        }
        
        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (HealthBelowPct(nextpct))
            {
                switch (nextpct)
                {
                case 95:
                    nextpct = 85;
                    break;
                case 85:
                    nextpct = 65;
                    break;
                case 65:
                    nextpct = 50;
                    break;
                case 50:
                    nextpct = 25;
                    break;
                case 25:
                    nextpct = NULL;
                    break;
                }
                SetExtraEvents(nextpct);
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                //Base Events
                //Kardris
                case EVENT_FROSTSTORM_BOLT:
                    if (me->getVictim())
                    {
                        if (!firstattack)
                        {
                            firstattack = true;
                            Talk(SAY_FIRSTATTACK);
                        }
                        DoCastVictim(SPELL_FROSTSTORM_BOLT);
                    }
                    events.ScheduleEvent(EVENT_FROSTSTORM_BOLT, 6000);
                    break;
                //Haromm
                case EVENT_FROSTSTORM_STRIKE:
                    if (me->getVictim())
                    {
                        if (!firstattack)
                        {
                            firstattack = true;
                            Talk(SAY_FIRSTATTACK);
                        }
                        DoCastVictim(SPELL_FROSTSTORM_STRIKE);
                    }
                    events.ScheduleEvent(EVENT_FROSTSTORM_STRIKE, 6000);
                    break;
                //Extra Events 95 pct HM
                //Kardris
                case EVENT_IRON_PRISON:
                {
                    std::list<HostileReference*> tlist = me->getThreatManager().getThreatList();
                    if (!tlist.empty())
                    {
                        uint8 num = 0;
                        for (std::list<HostileReference*>::const_iterator itr = tlist.begin(); itr != tlist.end(); itr++)
                        {
                            if (itr != tlist.begin())
                            {
                                if (Player* pl = me->GetPlayer(*me, (*itr)->getUnitGuid()))
                                {
                                    if (!pl->HasAura(SPELL_IRON_PRISON))
                                    {
                                        pl->AddAura(SPELL_IRON_PRISON, pl);
                                        num++;
                                        if (num == 2)
                                            break;
                                    }
                                }
                            }
                        }
                    }
                    events.ScheduleEvent(EVENT_IRON_PRISON, 31500);
                    break;
                }
                //Haromm
                case EVENT_IRON_TOMB:
                {
                    std::list<HostileReference*> tlist = me->getThreatManager().getThreatList();
                    if (!tlist.empty())
                    {
                        uint8 num = 0;
                        for (std::list<HostileReference*>::const_iterator itr = tlist.begin(); itr != tlist.end(); itr++)
                        {
                            if (itr != tlist.begin())
                            {
                                if (Player* pl = me->GetPlayer(*me, (*itr)->getUnitGuid()))
                                {
                                    DoCast(pl, SPELL_IRON_TOMB_SUM);
                                    num++;
                                    if (num == 3)
                                        break;
                                }
                            }
                        }
                    }
                    events.ScheduleEvent(EVENT_IRON_TOMB, 31500);
                    break;
                }
                //Extra Events 85 pct
                //Kardris
                case EVENT_TOXIC_STORM:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                        DoCast(target, SPELL_TOXIC_STORM_SUM);
                    events.ScheduleEvent(EVENT_TOXIC_STORM, 25000);
                    break;
                //Haromm
                case EVENT_TOXIC_MIST:
                {
                    std::list<HostileReference*> tlist = me->getThreatManager().getThreatList();
                    if (!tlist.empty())
                    {
                        uint8 num = 0;
                        for (std::list<HostileReference*>::const_iterator itr = tlist.begin(); itr != tlist.end(); itr++)
                        {
                            if (itr != tlist.begin())
                            {
                                if (Player* pl = me->GetPlayer(*me, (*itr)->getUnitGuid()))
                                {
                                    if (!pl->HasAura(SPELL_TOXIC_MIST))
                                    {
                                        pl->AddAura(SPELL_TOXIC_MIST, pl);
                                        num++;

                                        if (num == 2)
                                            break;
                                    }
                                }
                            }
                        }
                    }
                    events.ScheduleEvent(EVENT_TOXIC_MIST, 30000);
                    break;
                }
                //Extra events 65pct
                //Kardris
                case EVENT_FOUL_GEYSER:
                    if (me->getVictim())
                        DoCastVictim(SPELL_FOUL_GEYSER);
                    events.ScheduleEvent(EVENT_FOUL_GEYSER, 30000);
                    break;
                //Haromm
                case EVENT_FOUL_STREAM:
                    if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 1, 50.0f, true))
                        DoCast(target, SPELL_FOUL_STREAM);
                    events.ScheduleEvent(EVENT_FOUL_STREAM, 30000);
                    break;
                //Extra events 50pct
                //Kardris
                case EVENT_FALLING_ASH:
                    //TODO - Not found visual spell id 
                    break;
                //Haromm
                case EVENT_ASHEN_WALL:
                    if (me->getVictim())
                        me->getVictim()->SummonCreature(NPC_ASH_ELEMENTAL, me->getVictim()->GetPositionX(), me->getVictim()->GetPositionY(), me->getVictim()->GetPositionZ(), me->getVictim()->GetOrientation());
                    events.ScheduleEvent(EVENT_ASHEN_WALL, 30000);
                    break;
                case EVENT_BERSERK:
                    DoCast(me, SPELL_BERSERK);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature* sum)
        {
            summon.Summon(sum);
            switch (sum->GetEntry())
            {
            //Totems
            case 71915:
            case 71916:
            case 71917:
                sum->SetReactState(REACT_PASSIVE);
                sum->AttackStop();
                break;
            }
        }

        uint32 GetData(uint32 type) const override
        {
            if (type == DATA_GET_PULL_STATE)
                return firstpull ? 1 : 0;

            return NULL;
        }
    };
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_korkron_dark_shamanAI(creature);
    }
};

class npc_wolf_maunt : public CreatureScript
{
public:
    npc_wolf_maunt() : CreatureScript("npc_wolf_maunt") {}

    struct npc_wolf_mauntAI : public ScriptedAI
    {
        npc_wolf_mauntAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            SetHomePos();
            me->GetMotionMaster()->MoveTargetedHome();
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            if (instance)
            {
                if (Creature* omount = me->GetCreature(*me, instance->GetGuidData(me->GetEntry() == NPC_BLOODCLAW ? NPC_DARKFANG : NPC_BLOODCLAW)))
                {
                    if (!omount->isInCombat())
                        omount->AI()->DoZoneInCombat(omount, 150.0f);
                }
                CallAndDismountShamans();
                events.ScheduleEvent(EVENT_SWIPE, 7000);
                events.ScheduleEvent(EVENT_REND,  13000);
            }
        }

        void CallAndDismountShamans()
        {
            if (Vehicle* _me = me->GetVehicleKit())
            {
                if (Unit* p = _me->GetPassenger(0))
                {
                    p->_ExitVehicle();
                    p->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    p->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                    p->ToCreature()->AI()->DoZoneInCombat(p->ToCreature(), 150.0f);
                }
            }
        }

        void SetHomePos()
        {
            if (me->ToTempSummon() && me->ToTempSummon()->GetSummoner())
            {
                if (Creature* ds = me->ToTempSummon()->GetSummoner()->ToCreature())
                {
                    switch (me->GetEntry())
                    {
                    case NPC_BLOODCLAW:
                        if (ds->AI()->GetData(DATA_GET_PULL_STATE))
                            me->SetHomePosition(spawnpos[0]);
                        else
                            me->SetHomePosition(spawnpos[2]);
                        break;
                    case NPC_DARKFANG:
                        if (ds->AI()->GetData(DATA_GET_PULL_STATE))
                            me->SetHomePosition(spawnpos[1]);
                        else
                            me->SetHomePosition(spawnpos[3]);
                        break;
                    }
                }
            }
        }
        
        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SWIPE:
                    if (me->getVictim())
                        DoCastVictim(SPELL_SWIPE, true);
                    events.ScheduleEvent(EVENT_SWIPE, 7000);
                    break;
                case EVENT_REND:
                    if (me->getVictim())
                        DoCastVictim(SPELL_REND, true);
                    events.ScheduleEvent(EVENT_REND, 4000);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wolf_mauntAI(creature);
    }
}; 

//71801
class npc_toxic_storm : public CreatureScript
{
public:
    npc_toxic_storm() : CreatureScript("npc_toxic_storm") {}

    struct npc_toxic_stormAI : public ScriptedAI
    {
        npc_toxic_stormAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            DoCast(me, SPELL_TOXIC_STORM_TR_AURA, true);
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            events.ScheduleEvent(EVENT_SUMMON_TORNADO, 8000);
        }

        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_SUMMON_TORNADO)
                {
                    float ang = float(urand(0, 6));
                    me->SetOrientation(ang);
                    DoCast(me, SPELL_TOXIC_TORNADO_SUM);
                    events.ScheduleEvent(EVENT_SUMMON_TORNADO, 8000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_toxic_stormAI(creature);
    }
};

//71817
class npc_toxic_tornado : public CreatureScript
{
public:
    npc_toxic_tornado() : CreatureScript("npc_toxic_tornado") {}

    struct npc_toxic_tornadoAI : public ScriptedAI
    {
        npc_toxic_tornadoAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            DoCast(me, SPELL_TOXIC_TORNADO_TR_AURA, true);
        }

        InstanceScript* instance;

        void Reset()
        {
            float x = 0, y = 0;
            GetPositionWithDistInOrientation(me, 75.0f, me->GetOrientation(), x, y);
            me->GetMotionMaster()->MoveCharge(x, y, me->GetPositionZ(), 3.0f);
        }

        void EnterCombat(Unit* who){}

        void EnterEvadeMode(){}

        void UpdateAI(uint32 diff){}
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_toxic_tornadoAI(creature);
    }
};

//71825
class npc_foul_slime : public CreatureScript
{
public:
    npc_foul_slime() : CreatureScript("npc_foul_slime") {}

    struct npc_foul_slimeAI : public ScriptedAI
    {
        npc_foul_slimeAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset(){}
        
        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_FOULNESS);
            DoZoneInCombat(me, 150.0f);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_foul_slimeAI(creature);
    }
};

//71827
class npc_ash_elemental : public CreatureScript
{
public:
    npc_ash_elemental() : CreatureScript("npc_ash_elemental") {}

    struct npc_ash_elementalAI : public ScriptedAI
    {
        npc_ash_elementalAI(Creature* creature) : ScriptedAI(creature), summon(me)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            me->SetVisible(false);
        }

        InstanceScript* instance;
        SummonList summon;
        EventMap events;

        void Reset(){}

        void IsSummonedBy(Unit* summoner)
        {
            if (!summoner->ToCreature())
            {
                float x, y;
                for (uint8 n = 0; n < 8; n++)
                {
                    x = 0, y = 0;
                    float ang = me->GetOrientation() + mod[n].ang;
                    GetPositionWithDistInOrientation(me, mod[n].dist, ang, x, y);
                    me->SummonCreature(NPC_ASH_ELEMENTAL, x, y, me->GetPositionZ() + 5.0f, me->GetOrientation());
                }
                events.ScheduleEvent(EVENT_DESPAWN, 60000);
            }
            else
                me->GetMotionMaster()->MoveFall();
            events.ScheduleEvent(EVENT_ACTIVE, 1250);
        }

        void JustSummoned(Creature* sum)
        {
            summon.Summon(sum);
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ACTIVE:
                    me->SetVisible(true);
                    DoCast(me, SPELL_ASH_ELEMENTAL_SPAWN, true);
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoZoneInCombat(me, 150.0f);
                    break;
                case EVENT_DESPAWN:
                    summon.DespawnAll();
                    me->DespawnOrUnsummon();
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ash_elementalAI(creature);
    }
};

//71941
class npc_iron_tomb : public CreatureScript
{
public:
    npc_iron_tomb() : CreatureScript("npc_iron_tomb") {}

    struct npc_iron_tombAI : public ScriptedAI
    {
        npc_iron_tombAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetDisplayId(11686);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
        }

        InstanceScript* instance;

        void Reset(){}

        void IsSummonedBy(Unit* summoner)
        {
            DoCast(me, SPELL_IRON_TOMB_DMG);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_iron_tombAI(creature);
    }
};


void AddSC_boss_korkron_dark_shaman()
{
    new boss_korkron_dark_shaman();
    new npc_wolf_maunt();
    new npc_toxic_storm();
    new npc_toxic_tornado();
    new npc_foul_slime();
    new npc_ash_elemental();
    new npc_iron_tomb();
}

/*
    http://uwow.biz
    Dungeon : Vault of the Wardens 100-110
    Encounter: Inquisitor Tormentorum
    TODO: Нужно скриптануть суммоны!
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "vault_of_the_wardens.h"

/* enum Says
{
    SAY_AGGRO           = ,
    SAY_DEATH           = ,
}; */

enum Spells
{
    SPELL_SAP_SOUL              = 200905,
    SPELL_TELEPORT              = 200898,
    SPELL_OPEN_PRISONS_AURA     = 196821,
    SPELL_OPEN_PRISONS_CENTR    = 196993,
    SPELL_OPEN_PRISONS_LEFT     = 197009,
    SPELL_OPEN_PRISONS_RIGHT    = 197010,

    //Other
    SPELL_TORMENT               = 206470,
};

enum eEvents
{
    EVENT_SAP_SOUL              = 1,
    EVENT_TELEPORT              = 2,
    EVENT_OPEN_PRISON           = 3,
    EVENT_SUM_PRISON_ADDS       = 4,
};

Position const tpPos[4] =
{
    {4423.45f, -423.79f, 119.87f, 2.33f}, //Orks
    {4478.04f, -423.82f, 119.87f, 0.75f}, //Mogu
    {4478.52f, -478.88f, 119.87f, 5.44f}, //Void
    {4423.03f, -478.81f, 119.87f, 3.90f}, //Corruption
};

Position const sumPos[12] =
{
    {4417.47f, -417.87f, 120.39f, 5.50f}, //Orks centr - 99704
    {4405.09f, -418.98f, 120.15f, 4.65f}, //Orks left  - 99644
    {4418.89f, -405.60f, 120.23f, 0.08f}, //Orks right - 99645
    {4483.31f, -418.44f, 119.96f, 3.97f}, //Mogu centr - 99675
    {4481.22f, -405.33f, 119.96f, 3.13f}, //Mogu left  - 99676
    {4496.58f, -419.27f, 120.39f, 4.91f}, //Mogu right - 99676
    {4484.36f, -484.62f, 120.31f, 2.36f}, //Void centr - 99655
    {4496.40f, -482.99f, 120.15f, 1.47f}, //Void left  - 99657
    {4483.17f, -496.78f, 120.15f, 3.25f}, //Void right - 99651
    {4417.06f, -484.70f, 120.31f, 0.79f}, //Corruption centr - 99678
    {4418.96f, -496.95f, 120.25f, 0.02f}, //Corruption left  - 99678
    {4405.20f, -483.58f, 120.25f, 1.68f}, //Corruption right - 99678
};

class boss_inquisitor_tormentorum : public CreatureScript
{
public:
    boss_inquisitor_tormentorum() : CreatureScript("boss_inquisitor_tormentorum") {}

    struct boss_inquisitor_tormentorumAI : public BossAI
    {
        boss_inquisitor_tormentorumAI(Creature* creature) : BossAI(creature, DATA_TORMENTORUM) 
        {
            CoordSelection();
        }

        uint8 healthPct;
        uint8 tpRand[2];
        uint8 tpCount;

        void Reset()
        {
            _Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            DoCast(me, SPELL_TORMENT, true);
            healthPct = 71;
        }

        void EnterCombat(Unit* /*who*/) //07:12
        {
            //Talk(SAY_AGGRO);
            _EnterCombat();
            tpCount = 0;

            events.ScheduleEvent(EVENT_SAP_SOUL, 15000); //07:27, 07:48
        }

        void JustDied(Unit* /*killer*/)
        {
            //Talk(SAY_DEATH);
            _JustDied();
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage)
        {
            if (me->HealthBelowPct(healthPct) && healthPct > 40)
            {
                healthPct -= 30;
                DoStopAttack();
                me->StopMoving();
                events.ScheduleEvent(EVENT_TELEPORT, 500);
            }
        }

        void CoordSelection()
        {
            std::list<uint8> randList;
            for (uint8 i = 0; i < 4; i++)
                randList.push_back(i);

            Trinity::Containers::RandomResizeList(randList, 2);

            for (std::list<uint8>::iterator itr = randList.begin(); itr != randList.end(); ++itr)
            {
                tpRand[tpCount] = *itr;
                tpCount++;
            }
        }

        void OpenPrisonSumAdds()
        {
            std::list<GameObject*> goList;
            for (uint32 i = GO_CAGE_1; i < GO_CAGE_12 + 1; i++)
            {
                me->GetGameObjectListWithEntryInGrid(goList, i, 30);
            }
            for (std::list<GameObject*>::iterator itr = goList.begin(); itr != goList.end(); ++itr)
            {
                (*itr)->SetGoState(GO_STATE_ACTIVE);
            }

            switch (tpRand[tpCount])
            {
                case 0:
                    me->SummonCreature(NPC_SHADOWMOON_WARLOCK, sumPos[0]);
                    me->SummonCreature(NPC_FELGUARD_ANNIHILATOR, sumPos[1]);
                    me->SummonCreature(NPC_SHADOWMOON_TECHNICIAN, sumPos[2]);
                    break;
                case 1:
                    me->SummonCreature(NPC_ENORMOUS_STONE_QUILEN, sumPos[3]);
                    me->SummonCreature(NPC_MOGUSHAN_SECRET_KEEPER, sumPos[4]);
                    me->SummonCreature(NPC_MOGUSHAN_SECRET_KEEPER, sumPos[5]);
                    break;
                case 2:
                    me->SummonCreature(NPC_VOID_TOUCHED_JUGGERNAUT, sumPos[6]);
                    me->SummonCreature(NPC_DERANGED_MINDFLAYER, sumPos[7]);
                    me->SummonCreature(NPC_FACELESS_VOIDCASTER, sumPos[8]);
                    break;
                case 3:
                    for (uint8 i = 9; i < 12; i++)
                        me->SummonCreature(NPC_LINGERING_CORRUPTION, sumPos[i]);
                    break;
            }
            tpCount++;
            me->SetReactState(REACT_AGGRESSIVE);
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
                    case EVENT_SAP_SOUL:
                        DoCast(SPELL_SAP_SOUL);
                        me->AddUnitState(UNIT_STATE_CASTING);
                        me->StopMoving();
                        events.ScheduleEvent(EVENT_SAP_SOUL, 21000);
                        break;
                    case EVENT_TELEPORT:
                        me->CastSpell(tpPos[tpRand[tpCount]].GetPositionX(), tpPos[tpRand[tpCount]].GetPositionY(), tpPos[tpRand[tpCount]].GetPositionZ(), SPELL_TELEPORT);
                        events.ScheduleEvent(EVENT_OPEN_PRISON, 2500);
                        break;
                    case EVENT_OPEN_PRISON:
                        DoCast(SPELL_OPEN_PRISONS_AURA);
                        me->SetFacingTo(tpPos[tpRand[tpCount]].GetOrientation());
                        events.ScheduleEvent(EVENT_SUM_PRISON_ADDS, 5000);
                        break;
                    case EVENT_SUM_PRISON_ADDS:
                        OpenPrisonSumAdds();
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_inquisitor_tormentorumAI (creature);
    }
};

//200904
class spell_tormentorum_sap_soul : public SpellScriptLoader
{
public:
    spell_tormentorum_sap_soul() : SpellScriptLoader("spell_tormentorum_sap_soul") { }

    class spell_tormentorum_sap_soul_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_tormentorum_sap_soul_AuraScript);

        void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();

            Player* player = GetTarget()->ToPlayer();
            if (!player)
                return;

            SpellInfo const* spellInfo = eventInfo.GetDamageInfo()->GetSpellInfo();
            player->ModifySpellCooldown(spellInfo->Id, aurEff->GetSpellInfo()->Effects[EFFECT_1].BasePoints * IN_MILLISECONDS);
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_tormentorum_sap_soul_AuraScript::OnProc, EFFECT_1, SPELL_AURA_MOD_COOLDOWN);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_tormentorum_sap_soul_AuraScript();
    }
};

//196992
class spell_tormentorum_open_prisons : public SpellScriptLoader
{
public:
    spell_tormentorum_open_prisons() : SpellScriptLoader("spell_tormentorum_open_prisons") { }

    class spell_tormentorum_open_prisons_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_tormentorum_open_prisons_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster() || !GetHitGObj())
                return;

            switch (GetHitGObj()->GetEntry())
            {
                case GO_CAGE_2:
                case GO_CAGE_5:
                case GO_CAGE_8:
                case GO_CAGE_11:
                    GetCaster()->CastSpell(GetHitGObj(), SPELL_OPEN_PRISONS_CENTR, true);
                    break;
                case GO_CAGE_1:
                case GO_CAGE_4:
                case GO_CAGE_7:
                case GO_CAGE_12:
                    GetCaster()->CastSpell(GetHitGObj(), SPELL_OPEN_PRISONS_LEFT, true);
                    break;
                case GO_CAGE_3:
                case GO_CAGE_6:
                case GO_CAGE_10:
                case GO_CAGE_9:
                    GetCaster()->CastSpell(GetHitGObj(), SPELL_OPEN_PRISONS_RIGHT, true);
                    break;
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_tormentorum_open_prisons_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_tormentorum_open_prisons_SpellScript();
    }
};

void AddSC_boss_inquisitor_tormentorum()
{
    new boss_inquisitor_tormentorum();
    new spell_tormentorum_sap_soul();
    new spell_tormentorum_open_prisons();
}
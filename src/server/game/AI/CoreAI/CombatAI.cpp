/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "CombatAI.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "Vehicle.h"
#include "ObjectAccessor.h"
#include "Spell.h"
#include "Group.h"

int AggressorAI::Permissible(const Creature* creature)
{
    // have some hostile factions, it will be selected by IsHostileTo check at MoveInLineOfSight
    if (!creature->isCivilian() && !creature->IsNeutralToAll())
        return PERMIT_BASE_PROACTIVE;

    return PERMIT_BASE_NO;
}

void AggressorAI::UpdateAI(uint32 diff)
{
    if (me->isDead())
        return;

    if (!UpdateVictim())
    {
        if (m_checkTimer <= diff)
        {
            if (me->HasUnitState(UNIT_STATE_CASTING) || !me->isAlive())
            {
                m_checkTimer = 5000;
                return;
            }

            //Test code, searching target, info get from sniff
            if (me->m_actionData[0] && !me->m_actionData[0]->empty())
            {
                for (std::vector<CreatureActionData>::const_iterator itr = me->m_actionData[0]->begin(); itr != me->m_actionData[0]->end(); ++itr)
                {
                    if (Creature* target = me->FindNearestCreature(itr->target, 7.0f))
                        if (target->isAlive())
                            AttackStart(target);
                }
            }
            else if (me->m_actionData[1] && !me->m_actionData[1]->empty())
            {
                for (std::vector<CreatureActionData>::const_iterator itr = me->m_actionData[1]->begin(); itr != me->m_actionData[1]->end(); ++itr)
                {
                    if (Creature* target = me->FindNearestCreature(itr->target, 7.0f))
                        if (target->isAlive())
                            me->CastSpell(target, itr->spellId, false);
                }
            }
            m_checkTimer = 1000;
        }
        else
            m_checkTimer -= diff;
        return;
    }

    DoMeleeAttackIfReady();
}

// some day we will delete these useless things
int CombatAI::Permissible(const Creature* /*creature*/)
{
    return PERMIT_BASE_NO;
}

int ArcherAI::Permissible(const Creature* /*creature*/)
{
    return PERMIT_BASE_NO;
}

int TurretAI::Permissible(const Creature* /*creature*/)
{
    return PERMIT_BASE_NO;
}

int VehicleAI::Permissible(const Creature* /*creature*/)
{
    return PERMIT_BASE_NO;
}

void CombatAI::InitializeAI()
{
    for (uint32 i = 0; i < CREATURE_MAX_SPELLS; ++i)
        if (me->m_temlate_spells[i] && sSpellMgr->GetSpellInfo(me->m_temlate_spells[i]))
            spells.push_back(me->m_temlate_spells[i]);

    CreatureAI::InitializeAI();
}

void CombatAI::Reset()
{
    events.Reset();
}

void CombatAI::JustDied(Unit* killer)
{
    for (SpellVct::iterator i = spells.begin(); i != spells.end(); ++i)
        if (AISpellInfo[*i].condition == AICOND_DIE)
            me->CastSpell(killer, *i, true);
}

void CombatAI::EnterCombat(Unit* who)
{
    for (SpellVct::iterator i = spells.begin(); i != spells.end(); ++i)
    {
        if (AISpellInfo[*i].condition == AICOND_AGGRO)
            me->CastSpell(who, *i, false);
        else if (AISpellInfo[*i].condition == AICOND_COMBAT)
            events.ScheduleEvent(*i, AISpellInfo[*i].cooldown + rand()%AISpellInfo[*i].cooldown);
    }
}

void CombatAI::UpdateAI(uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->getVictim() && me->getVictim()->HasCrowdControlAura(me))
    {
        me->InterruptNonMeleeSpells(false);
        return;
    }

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 spellId = events.ExecuteEvent())
    {
        DoCast(spellId);
        events.ScheduleEvent(spellId, AISpellInfo[spellId].cooldown + rand()%AISpellInfo[spellId].cooldown);
    }
    else
        DoMeleeAttackIfReady();
}

/////////////////
//CasterAI
/////////////////

void CasterAI::InitializeAI()
{
    CombatAI::InitializeAI();

    m_attackDist = MAGIC_RANGE;
    for (SpellVct::iterator itr = spells.begin(); itr != spells.end(); ++itr)
        if (AISpellInfo[*itr].condition == AICOND_COMBAT && m_attackDist > GetAISpellInfo(*itr)->maxRange)
            m_attackDist = GetAISpellInfo(*itr)->maxRange;
    if (m_attackDist < MAGIC_RANGE)
        m_attackDist = MAGIC_RANGE;
}

void CasterAI::EnterCombat(Unit* who)
{
    if (spells.empty())
        return;

    uint32 spell = rand()%spells.size();
    uint32 count = 0;
    for (SpellVct::iterator itr = spells.begin(); itr != spells.end(); ++itr, ++count)
    {
        if (AISpellInfo[*itr].condition == AICOND_AGGRO)
            me->CastSpell(who, *itr, false);
        else if (AISpellInfo[*itr].condition == AICOND_COMBAT)
        {
            uint32 cooldown = GetAISpellInfo(*itr)->realCooldown;
            if (count == spell)
            {
                DoCast(spells[spell]);
                cooldown += me->GetCurrentSpellCastTime(*itr);
            }
            events.ScheduleEvent(*itr, cooldown);
        }
    }
}

void CasterAI::UpdateAI(uint32 diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->getVictim()->HasCrowdControlAura(me))
    {
        me->InterruptNonMeleeSpells(false);
        return;
    }

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (events.GetEvent())
        if (!me->IsWithinLOSInMap(me->getVictim()))
            if (MotionMaster* mMaster = me->GetMotionMaster())
            {
                mMaster->MoveChase(me->getVictim(), 0.001f);
                return;
            }

    if (uint32 spellId = events.ExecuteEvent())
    {
        DoCast(spellId);
        uint32 casttime = me->GetCurrentSpellCastTime(spellId);
        events.ScheduleEvent(spellId, (casttime ? casttime : 500) + GetAISpellInfo(spellId)->realCooldown);
    }
}

//////////////
//ArcherAI
//////////////

ArcherAI::ArcherAI(Creature* c) : CreatureAI(c)
{
    if (!me->m_temlate_spells[0])
        sLog->outError(LOG_FILTER_GENERAL, "ArcherAI set for creature (entry = %u) with spell1=0. AI will do nothing", me->GetEntry());

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(me->m_temlate_spells[0]);
    m_minRange = spellInfo ? spellInfo->GetMinRange(false) : 0;

    if (!m_minRange)
        m_minRange = MELEE_RANGE;
    me->m_CombatDistance = spellInfo ? spellInfo->GetMaxRange(false) : 0;
    me->m_SightDistance = me->m_CombatDistance;
}

void ArcherAI::AttackStart(Unit* who)
{
    if (!who)
        return;

    if (me->IsWithinCombatRange(who, m_minRange))
    {
        if (me->Attack(who, true) && !who->IsFlying())
            me->GetMotionMaster()->MoveChase(who);
    }
    else
    {
        if (me->Attack(who, false) && !who->IsFlying())
            me->GetMotionMaster()->MoveChase(who, me->m_CombatDistance);
    }

    if (who->IsFlying())
        me->GetMotionMaster()->MoveIdle();
}

void ArcherAI::UpdateAI(uint32 /*diff*/)
{
    if (!UpdateVictim())
        return;

    if (!me->IsWithinCombatRange(me->getVictim(), m_minRange))
        DoSpellAttackIfReady(me->m_temlate_spells[0]);
    else
        DoMeleeAttackIfReady();
}

//////////////
//TurretAI
//////////////

TurretAI::TurretAI(Creature* c) : CreatureAI(c)
{
    if (!me->m_temlate_spells[0])
        sLog->outError(LOG_FILTER_GENERAL, "TurretAI set for creature (entry = %u) with spell1=0. AI will do nothing", me->GetEntry());

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(me->m_temlate_spells[0]);
    m_minRange = spellInfo ? spellInfo->GetMinRange(false) : 0;
    me->m_CombatDistance = spellInfo ? spellInfo->GetMaxRange(false) : 0;
    me->m_SightDistance = me->m_CombatDistance;
}

bool TurretAI::CanAIAttack(const Unit* /*who*/) const
{
    // TODO: use one function to replace it
    if (!me->IsWithinCombatRange(me->getVictim(), me->m_CombatDistance)
        || (m_minRange && me->IsWithinCombatRange(me->getVictim(), m_minRange)))
        return false;
    return true;
}

void TurretAI::AttackStart(Unit* who)
{
    if (who)
        me->Attack(who, false);
}

void TurretAI::UpdateAI(uint32 /*diff*/)
{
    if (!UpdateVictim())
        return;

    DoSpellAttackIfReady(me->m_temlate_spells[0]);
}

//////////////
//VehicleAI
//////////////

VehicleAI::VehicleAI(Creature* c) : CreatureAI(c), m_vehicle(c->GetVehicleKit()), m_IsVehicleInUse(false), m_ConditionsTimer(VEHICLE_CONDITION_CHECK_TIME)
{
    LoadConditions();
    m_DoDismiss = false;
    m_DismissTimer = VEHICLE_DISMISS_TIME;
}

//NOTE: VehicleAI::UpdateAI runs even while the vehicle is mounted
void VehicleAI::UpdateAI(uint32 diff)
{
    CheckConditions(diff);

    if (m_DoDismiss)
    {
        if (m_DismissTimer < diff)
        {
            m_DoDismiss = false;
            me->SetVisible(false);
            me->DespawnOrUnsummon();
        }else m_DismissTimer -= diff;
    }
}

void VehicleAI::Reset()
{
    me->SetVisible(true);
}

void VehicleAI::OnCharmed(bool apply)
{
    if (m_IsVehicleInUse && !apply && !conditions.empty())//was used and has conditions
    {
        m_DoDismiss = true;//needs reset
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    }
    else if (apply)
        m_DoDismiss = false;//in use again
    m_DismissTimer = VEHICLE_DISMISS_TIME;//reset timer
    m_IsVehicleInUse = apply;
}

void VehicleAI::LoadConditions()
{
    conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_CREATURE_TEMPLATE_VEHICLE, me->GetEntry());
    if (!conditions.empty())
        sLog->outDebug(LOG_FILTER_CONDITIONSYS, "VehicleAI::LoadConditions: loaded %u conditions", uint32(conditions.size()));
}

void VehicleAI::CheckConditions(const uint32 diff)
{
    if (m_ConditionsTimer < diff)
    {
        if (!conditions.empty())
        {
            if (Vehicle* vehicleKit = me->GetVehicleKit())
                for (SeatMap::iterator itr = vehicleKit->Seats.begin(); itr != vehicleKit->Seats.end(); ++itr)
                    if (Unit* passenger = ObjectAccessor::GetUnit(*me, itr->second.Passenger))
                    {
                        if (Player* player = passenger->ToPlayer())
                        {
                            if (!sConditionMgr->IsObjectMeetToConditions(player, me, conditions))
                            {
                                player->ExitVehicle();
                                return;//check other pessanger in next tick
                            }
                        }
                    }
        }
        m_ConditionsTimer = VEHICLE_CONDITION_CHECK_TIME;
    } else m_ConditionsTimer -= diff;
}

int AnyPetAI::Permissible(const Creature* creature)
{
    // have some hostile factions, it will be selected by IsHostileTo check at MoveInLineOfSight
    if (!creature->isCivilian() && !creature->IsNeutralToAll())
        return PERMIT_BASE_PROACTIVE;

    return PERMIT_BASE_NO;
}

void AnyPetAI::InitializeAI()
{
    CreatureAI::InitializeAI();

    if(PetStats const* pStats = sObjectMgr->GetPetStats(me->GetEntry()))
    {
        if(pStats->state)
            me->SetReactState(ReactStates(pStats->state));
    }
    if(me->GetReactState() == REACT_AGGRESSIVE)
    {
        if(Unit* victim = me->GetTargetUnit())
        {
            if (!me->IsFriendlyTo(victim))
            {
                Unit* owner = me->GetCharmerOrOwner();
                if (owner && !owner->isInCombat())
                    owner->SetInCombatWith(victim);

                if(me->GetCasterPet())
                    AttackStartCaster(victim, me->GetAttackDist() - 0.5f);
                else
                    AttackStart(victim);
            }
        }
    }

    // Update speed as needed to prevent dropping too far behind and despawning
    me->UpdateSpeed(MOVE_RUN, true);
    me->UpdateSpeed(MOVE_WALK, true);
    me->UpdateSpeed(MOVE_FLIGHT, true);
}

void AnyPetAI::UpdateAI(uint32 diff)
{
    if (!me->isAlive())
        return;

    if (m_updateAlliesTimer <= diff)
        // UpdateAllies self set update timer
        UpdateAllies();
    else
        m_updateAlliesTimer -= diff;

    Unit* owner = me->GetCharmerOrOwner();
    Unit* target = me->getAttackerForHelper();
    Unit* targetOwner = NULL;
    if(!me->HasReactState(REACT_PASSIVE))
    {
        if (owner)
            targetOwner = owner->getAttackerForHelper();

        if(targetOwner != NULL && targetOwner != target)
        {
            if(me->GetCasterPet())
                AttackStartCaster(targetOwner, me->GetAttackDist() - 0.5f);
            else
                AttackStart(targetOwner);

            //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::UpdateAI AttackStart");
        }
        else if (me->getVictim() && !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED))
        {
            // is only necessary to stop casting, the pet must not exit combat
            if (me->getVictim()->HasCrowdControlAura(me))
            {
                me->InterruptNonMeleeSpells(false);
                return;
            }

            if (owner && !owner->isInCombat())
                owner->SetInCombatWith(me->getVictim());

            if(!me->GetCasterPet())
                DoMeleeAttackIfReady();
        }
        else if (owner && !me->HasUnitState(UNIT_STATE_FOLLOW))
        {
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
            //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::UpdateAI PET_FOLLOW_DIST");
        }
    }

    // Autocast (casted only in combat or persistent spells in any state)
    if (!me->HasUnitState(UNIT_STATE_CASTING))
    {
        typedef std::vector<std::pair<Unit*, Spell*> > TargetSpellList;
        TargetSpellList targetSpellStore;
        //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::UpdateAI GetPetCastSpellSize %i", me->GetPetCastSpellSize());

        for (uint8 i = 0; i < me->GetPetCastSpellSize(); ++i)
        {
            uint32 spellID = me->GetPetCastSpellOnPos(i);
            if (!spellID)
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
            if (!spellInfo)
                continue;

            //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::UpdateAI spellID %i, Cooldown %i IsPositive %i CanBeUsedInCombat %i GUID %u",
            //spellID, me->HasSpellCooldown(spellID), spellInfo->IsPositive(), spellInfo->CanBeUsedInCombat(), me->GetGUIDLow());

            if (me->HasSpellCooldown(spellID))
                continue;

            if (spellInfo->IsPositive())
            {
                if (spellInfo->CanBeUsedInCombat())
                {
                    // Check if we're in combat
                    if (!me->isInCombat())
                        continue;
                }

                TriggerCastData triggerData;
                Spell* spell = new Spell(me, spellInfo, triggerData);
                bool spellUsed = false;

                if (target)
                {
                    if (me->IsWithinMeleeRange(target, me->GetAttackDist()) && spell->CanAutoCast(target))
                    {
                        targetSpellStore.push_back(std::make_pair(target, spell));
                        spellUsed = true;
                    }
                }

                // No enemy, check friendly
                if (!spellUsed)
                {
                    for (GuidSet::const_iterator tar = m_AllySet.begin(); tar != m_AllySet.end(); ++tar)
                    {
                        Unit* ally = ObjectAccessor::GetUnit(*me, *tar);

                        //only buff targets that are in combat, unless the spell can only be cast while out of combat
                        if (!ally)
                            continue;

                        if (spell->CanAutoCast(ally))
                        {
                            targetSpellStore.push_back(std::make_pair(ally, spell));
                            spellUsed = true;
                            break;
                        }
                    }
                }

                // No valid targets at all
                if (!spellUsed)
                    delete spell;
            }
            else if(spellInfo->IsTargetingAreaCast())
            {
                if(target)
                    me->CastSpell(target, spellInfo, false);
                else
                    me->CastSpell(me, spellInfo, false);
            }
            else if (target/* && me->IsWithinMeleeRange(target, me->GetAttackDist())*/ && spellInfo->CanBeUsedInCombat())
            {
                TriggerCastData triggerData;
                Spell* spell = new Spell(me, spellInfo, triggerData);
                if (spell->CanAutoCast(target))
                    targetSpellStore.push_back(std::make_pair(target, spell));
                else
                    delete spell;
            }
            //else
                //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::UpdateAI not cast spellID %i", spellID);
        }

        //found units to cast on to
        if (!targetSpellStore.empty())
        {
            uint32 index = urand(0, targetSpellStore.size() - 1);

            Spell* spell  = targetSpellStore[index].second;
            Unit* targetSpell = targetSpellStore[index].first;

            targetSpellStore.erase(targetSpellStore.begin() + index);

            SpellCastTargets targets;
            targets.SetUnitTarget(targetSpell);

            if (!me->HasInArc(M_PI, targetSpell))
            {
                me->SetInFront(targetSpell);
                if (targetSpell && targetSpell->GetTypeId() == TYPEID_PLAYER)
                    me->SendUpdateToPlayer(targetSpell->ToPlayer());

                if (owner && owner->GetTypeId() == TYPEID_PLAYER)
                    me->SendUpdateToPlayer(owner->ToPlayer());
            }

            me->AddCreatureSpellCooldown(spell->m_spellInfo->Id);

            spell->prepare(&targets);
        }
        //else
            //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::UpdateAI targetSpellStore is empty");

        // deleted cached Spell objects
        for (TargetSpellList::const_iterator itr = targetSpellStore.begin(); itr != targetSpellStore.end(); ++itr)
            delete itr->second;
    }

    if(!me->GetCasterPet())
        DoMeleeAttackIfReady();
}

void AnyPetAI::UpdateAllies()
{
    Unit* owner = me->GetCharmerOrOwner();
    Group* group = NULL;

    m_updateAlliesTimer = 10*IN_MILLISECONDS;                //update friendly targets every 10 seconds, lesser checks increase performance

    if (!owner)
        return;
    else if (owner->GetTypeId() == TYPEID_PLAYER)
        group = owner->ToPlayer()->GetGroup();

    //only pet and owner/not in group->ok
    if (m_AllySet.size() == 2 && !group)
        return;
    //owner is in group; group members filled in already (no raid -> subgroupcount = whole count)
    if (group && !group->isRaidGroup() && m_AllySet.size() == (group->GetMembersCount() + 2))
        return;

    m_AllySet.clear();
    m_AllySet.insert(me->GetGUID());
    if (group)                                              //add group
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* Target = itr->getSource();
            if (!Target || !group->SameSubGroup((Player*)owner, Target))
                continue;

            if (Target->GetGUID() == owner->GetGUID())
                continue;

            m_AllySet.insert(Target->GetGUID());
        }
    }
    else                                                    //remove group
        m_AllySet.insert(owner->GetGUID());
}

void AnyPetAI::MovementInform(uint32 moveType, uint32 data)
{
    //sLog->outDebug(LOG_FILTER_PETS, "AnyPetAI::MovementInform Pet %u moveType %i data %i", me->GetEntry(), moveType, data);
    // Receives notification when pet reaches stay or follow owner
    switch (moveType)
    {
        case POINT_MOTION_TYPE:
        {
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();
            if(me->getVictim() && me->isInCombat())
                me->GetMotionMaster()->MoveChase(me->getVictim(), me->GetAttackDist() - 0.5f);
            if(me->GetCharmerOrOwner() && (!me->getVictim() || !me->isInCombat()))
                me->GetMotionMaster()->MoveFollow(me->GetCharmerOrOwner(), PET_FOLLOW_DIST, me->GetFollowAngle());
            break;
        }
        default:
            break;
    }
}

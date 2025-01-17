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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "Util.h"
#include "Pet.h"
#include "World.h"
#include "Group.h"
#include "SpellInfo.h"
#include "SpellAuraEffects.h"
#include "SpellPackets.h"
#include "QueryPackets.h"
#include "PetPackets.h"

void WorldSession::HandleDismissCritter(WorldPackets::PetPackets::DismissCritter& packet)
{
    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.CritterGUID);
    if (!pet)
        return;

    if (_player->GetCritterGUID() == pet->GetGUID())
    {
         if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isSummon())
             pet->ToTempSummon()->UnSummon();
    }
}

void WorldSession::HandlePetAction(WorldPackets::PetPackets::PetAction& packet)
{
    uint32 spellID = UNIT_ACTION_BUTTON_ACTION(packet.Action);
    uint8 flag = UNIT_ACTION_BUTTON_TYPE(packet.Action); //delete = 0x07 CastSpell = C1

    sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet %u - flag: %u, spellID: %u, target: %u.", packet.PetGUID.GetGUIDLow(), uint32(flag), spellID, packet.TargetGUID.GetGUIDLow());
    
    Unit* pet = ObjectAccessor::GetUnit(*_player, packet.PetGUID);
    if (!pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) doesn't exist for player '%s'", packet.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (pet != GetPlayer()->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) does not belong to player '%s'", packet.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    if (!pet->isAlive())
    {
        SpellInfo const* spell = (flag == ACT_ENABLED || flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(spellID) : nullptr;
        if (!spell)
            return;

        if (!(spell->HasAttribute(SPELL_ATTR0_CASTABLE_WHILE_DEAD)))
            return;
    }

    //TODO: allow control charmed player?
    if (pet->GetTypeId() == TYPEID_PLAYER && !(flag == ACT_COMMAND && spellID == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pet, packet.PetGUID, spellID, flag, packet.TargetGUID, packet.ActionPosition);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for (Unit::ControlList::const_iterator::value_type const& itr : GetPlayer()->m_Controlled)
            if (itr->GetEntry() == pet->GetEntry() && itr->isAlive())
            {
                if (itr->ToCreature())
                {
                    if (!itr->ToCreature()->m_Stampeded && itr->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
                        controlled.push_back(itr);
                }
                else
                    controlled.push_back(itr);
            }

        for (std::vector<Unit*>::const_iterator::value_type const& itr : controlled)
            HandlePetActionHelper(itr, packet.PetGUID, spellID, flag, packet.TargetGUID, packet.ActionPosition);
    }
}

void WorldSession::HandleStopAttack(WorldPackets::PetPackets::StopAttack& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, packet.PetGUID);
    if (!pet)
        return;

    if (pet != player->GetPet() && pet != player->GetCharm())
        return;

    if (!pet->isAlive())
        return;

    pet->AttackStop();
}

void WorldSession::HandleQueryPetName(WorldPackets::Query::QueryPetName& packet)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    WorldPackets::Query::QueryPetNameResponse response;
    response.UnitGUID = packet.UnitGUID;

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*player, packet.UnitGUID);
    if (!pet)
    {
        player->GetSession()->SendPacket(response.Write());
        return;
    }

    response.Allow = pet->isPet() ? 1 : 0;
    response.Name = pet->GetName();

    if (Pet* playerPet = pet->ToPet())
    {
        DeclinedName const* declinedNames = playerPet->GetDeclinedNames();
        if (declinedNames)
        {
            response.HasDeclined = true;
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                if (declinedNames->name[i].size())
                    response.DeclinedNames.name[i] = declinedNames->name[i];
        }
        else
            response.HasDeclined = false;

        response.Timestamp = playerPet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);
    }

    player->GetSession()->SendPacket(response.Write());
}

void WorldSession::HandlePetSetAction(WorldPackets::PetPackets::PetSetAction& packet)
{
    Unit* pet = ObjectAccessor::GetUnit(*_player, packet.PetGUID);
    if (!pet || pet != _player->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", packet.PetGUID.GetGUIDLow(), _player->GetGUIDLow());
        return;
    }

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (packet.Index >= MAX_UNIT_ACTION_BAR_INDEX)
        return;
        
    uint32 spellID = UNIT_ACTION_BUTTON_ACTION(packet.Action);
    uint8 actState = UNIT_ACTION_BUTTON_TYPE(packet.Action);

    if (actState == ACT_DECIDE && !charmInfo->GetActionBarEntry(packet.Index))
        return;

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X HasSpell %i",
                  _player->GetName(), packet.Index, spellID, uint32(actState), pet->HasSpell(spellID));

    //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
    if (!((actState == ACT_ENABLED || actState == ACT_DISABLED || actState == ACT_PASSIVE) && spellID && !pet->HasSpell(spellID)))
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID))
        {
            if (actState == ACT_ENABLED)
            {
                if (pet->GetCharmInfo())
                    static_cast<Pet*>(pet)->ToggleAutocast(spellInfo, true);
                else
                    for (Unit::ControlList::const_iterator::value_type const& itr : GetPlayer()->m_Controlled)
                        if (itr->GetEntry() == pet->GetEntry())
                            itr->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, true);
            }
            else if (actState == ACT_DISABLED)
            {
                if (pet->GetCharmInfo())
                    static_cast<Pet*>(pet)->ToggleAutocast(spellInfo, false);
                else
                    for (Unit::ControlList::const_iterator::value_type const& itr : GetPlayer()->m_Controlled)
                        if (itr->GetEntry() == pet->GetEntry())
                            itr->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, false);
            }
        }

        charmInfo->SetActionBar(packet.Index, spellID, ActiveStates(actState));
    }
}

void WorldSession::HandlePetRename(WorldPackets::PetPackets::PetRename& packet)
{
    Pet* pet = _player->GetPet();
    if (!pet)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename pet not found");
        return;
    }

    // check it!
    if (!pet->isPet() || static_cast<Pet*>(pet)->getPetType() != HUNTER_PET ||
        !pet->HasByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED) ||
        pet->GetOwnerGUID() != _player->GetGUID() || !pet->GetCharmInfo())
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename error pet");
        return;
    }

    if (pet->GetCharmInfo()->GetPetNumber() != packet.RenameData.PetNumber ||
        pet->GetGUID() != packet.RenameData.PetGUID)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename petnumber not correct");
        return;
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    //if (namepart)
    {
        PetNameInvalidReason res = ObjectMgr::CheckPetName(packet.RenameData.NewName);
        if (res != PET_NAME_SUCCESS)
        {
            SendPetNameInvalid(res, packet.RenameData.PetGUID, packet.RenameData.NewName, nullptr);
            sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetRename CheckPetName res %i", res);
            return;
        }

        if (sObjectMgr->IsReservedName(packet.RenameData.NewName))
        {
            SendPetNameInvalid(PET_NAME_RESERVED, packet.RenameData.PetGUID, packet.RenameData.NewName, nullptr);
            return;
        }

        pet->SetName(packet.RenameData.NewName);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_NAME);
        stmt->setString(0, packet.RenameData.NewName);
        stmt->setUInt64(1, _player->GetGUIDLow());
        stmt->setUInt32(2, pet->GetCharmInfo()->GetPetNumber());
        trans->Append(stmt);
    }

    if (packet.RenameData.HasDeclinedNames)
    {
        if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
        {
            std::wstring wname;
            Utf8toWStr(packet.RenameData.NewName, wname);
            if (!ObjectMgr::CheckDeclinedNames(wname, packet.RenameData.DeclinedNames))
            {
                SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, packet.RenameData.PetGUID, packet.RenameData.NewName, &packet.RenameData.DeclinedNames);
                return;
            }

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_DECLINEDNAME);
            stmt->setUInt32(0, pet->GetCharmInfo()->GetPetNumber());
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_ADD_CHAR_PET_DECLINEDNAME);
            stmt->setUInt64(0, _player->GetGUIDLow());

            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
                stmt->setString(i + 1, packet.RenameData.DeclinedNames.name[i]);

            trans->Append(stmt);
        }
    }

    CharacterDatabase.CommitTransaction(trans);

    Unit* owner = pet->GetOwner();
    if (owner && (owner->GetTypeId() == TYPEID_PLAYER) && owner->ToPlayer()->GetGroup())
        owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    pet->RemoveByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);
    pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(nullptr))); // cast can't be helped
}

void WorldSession::HandlePetAbandon(WorldPackets::PetPackets::PetAbandon& packet)
{
    if (!_player->IsInWorld())
        return;

    if (Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.Pet))
    {
        if (pet->isPet())
        {
            _player->RemovePet(static_cast<Pet*>(pet), true);
            _player->GetSession()->SendStablePet();
        }
        else if (pet->GetGUID() == _player->GetCharmGUID())
            _player->StopCastingCharm();
    }
}

void WorldSession::HandlePetSpellAutocast(WorldPackets::PetPackets::PetSpellAutocast& packet)
{
    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    if (ObjectAccessor::FindPlayer(packet.PetGUID))
        return;

    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*_player, packet.PetGUID);
    if (!pet || (pet != _player->GetGuardianPet() && pet != _player->GetCharm()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetSpellAutocast.Pet %u isn't pet of player %s .", packet.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(packet.SpellID);
    if (!pet->HasSpell(packet.SpellID) || spellInfo->IsAutocastable())
        return;

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetSpellAutocastOpcod: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    if (pet->isPet())
        static_cast<Pet*>(pet)->ToggleAutocast(spellInfo, packet.AutocastEnabled);
    else
        pet->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, packet.AutocastEnabled);

    charmInfo->SetSpellAutocast(spellInfo, packet.AutocastEnabled);
}

void WorldSession::HandlePetCastSpellOpcode(WorldPackets::Spells::PetCastSpell& cast)
{
    if (!_player->GetGuardianPet() && !_player->GetCharm())
        return;

    Unit* caster = ObjectAccessor::GetUnit(*_player, cast.PetGUID);

    if (!caster || (caster != _player->GetGuardianPet() && caster != _player->GetCharm()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s .", cast.PetGUID.GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    SpellCastTargets targets(caster, cast.Cast);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(cast.Cast.SpellID);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", cast.Cast.SpellID);
        return;
    }

    bool triggered = false;
    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_TRAJ || spellInfo->Effects[i].TargetB.GetTarget() == TARGET_DEST_TRAJ || spellInfo->Effects[i].Effect == SPELL_EFFECT_TRIGGER_MISSILE)
            triggered = true;
    }

    // do not cast not learned spells
    if (!triggered && (!caster->HasSpell(cast.Cast.SpellID) || spellInfo->IsPassive()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: !HasSpell or IsPassive");
        return;
    }

    caster->ClearUnitState(UNIT_STATE_FOLLOW);

    uint32 triggeredCastFlags = triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE;
    triggeredCastFlags &= ~TRIGGERED_IGNORE_POWER_AND_REAGENT_COST;

    TriggerCastData triggerData;
    triggerData.triggerFlags = TriggerCastFlags(triggeredCastFlags);
    triggerData.miscData0 = cast.Cast.Misc[0];
    triggerData.miscData1 = cast.Cast.Misc[1];
    triggerData.spellGuid = cast.Cast.SpellGuid;

    Spell* spell = new Spell(caster, spellInfo, triggerData);
    spell->m_targets = targets;

    if (spellInfo->Categories.StartRecoveryCategory) // Check if spell is affected by GCD
        if (caster->GetTypeId() == TYPEID_UNIT && caster->GetCharmInfo() && caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: Check if spell is affected by GCD");
            spell->SendCastResult(GetPlayer(), spellInfo, SPELL_FAILED_NOT_READY, SPELL_CUSTOM_ERROR_NONE, 0, true);
            spell->finish(false);
            delete spell;
            return;
        }

    // TODO: need to check victim?
    SpellCastResult result;
    if (caster->m_movedPlayer)
        result = spell->CheckPetCast(caster->m_movedPlayer->GetSelectedUnit());
    else
        result = spell->CheckPetCast(NULL);
    if (result == SPELL_CAST_OK)
    {
        if (caster->GetTypeId() == TYPEID_UNIT)
        {
            Creature* pet = caster->ToCreature();
            pet->AddCreatureSpellCooldown(cast.Cast.SpellID);
            if (pet->isPet())
            {
                Pet* p = (Pet*)pet;
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (p->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(cast.PetGUID);
            }
        }

        spell->prepare(&(spell->m_targets));
    }
    else
    {
        Creature* pet = caster->ToCreature();
        bool sendPet = !pet || !(pet->isPossessed() || pet->IsVehicle());
        spell->SendCastResult(GetPlayer(), spellInfo, result, SPELL_CUSTOM_ERROR_NONE, 0, sendPet);

        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!caster->ToPlayer()->HasSpellCooldown(cast.Cast.SpellID))
                GetPlayer()->SendClearCooldown(cast.Cast.SpellID, caster);
        }
        else
        {
            if (!caster->ToCreature()->HasSpellCooldown(cast.Cast.SpellID))
                GetPlayer()->SendClearCooldown(cast.Cast.SpellID, caster);
        }

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::HandleLearnPetSpecializationGroup(WorldPackets::PetPackets::LearnPetSpecializationGroup& packet)
{
    if (_player->isInCombat() || _player->getClass() != CLASS_HUNTER)
        return;

    uint32 specializationId = 0;
    bool adaptation = false;

    if (_player->HasAura(152244)) //Adaptation Spec
        adaptation = true;

    switch(packet.SpecGroupId)
    {
        case 0:
            if(adaptation)
                specializationId = SPEC_PET_ADAPTATION_FEROCITY;
            else
                specializationId = SPEC_PET_FEROCITY;
            break;
        case 1:
            if(adaptation)
                specializationId = SPEC_PET_ADAPTATION_TENACITY;
            else
                specializationId = SPEC_PET_TENACITY;
            break;
        case 2:
            if(adaptation)
                specializationId = SPEC_PET_ADAPTATION_CUNNING;
            else
                specializationId = SPEC_PET_CUNNING;
            break;
        default:
            break;
    }

    if (!specializationId)
        return;

    Pet* pet = _player->GetPet();
    if (!pet || !pet->isAlive())
        return;

    if (pet->GetSpecializationId())
        pet->UnlearnSpecializationSpell();

    pet->SetSpecializationId(specializationId);
    pet->LearnSpecializationSpell();
    _player->SendTalentsInfoData(true);
}

void WorldSession::HanleSetPetSlot(WorldPackets::PetPackets::SetPetSlot& packet)
{
    if (!CheckStableMaster(packet.NpcGUID))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (packet.NewSlot > MAX_PET_STABLES)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Pet* pet = _player->GetPet();
    if (pet && pet->GetCharmInfo() && pet->GetCharmInfo()->GetPetNumber() == packet.PetIndex)
        _player->RemovePet(pet);

    PetSlot curentSlot = GetPlayer()->GetSlotForPetId(GetPlayer()->m_currentPetNumber);
    if (pet && curentSlot == packet.NewSlot)
        _player->RemovePet(pet);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_BY_ID);
    stmt->setUInt64(0, _player->GetGUIDLow());
    stmt->setUInt32(1, packet.PetIndex);
    _stableChangeSlotCallback.SetParam(packet.NewSlot);
    _stableChangeSlotCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleStableChangeSlotCallback(PreparedQueryResult result, uint8 new_slot)
{
    if (!GetPlayer())
        return;

    if (!result)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Field *fields = result->Fetch();

    uint32 pet_entry = fields[0].GetUInt32();
    uint32 pet_number  = fields[1].GetUInt32();
    bool isHunter = fields[2].GetUInt8() == HUNTER_PET;

    PetSlot slot = GetPlayer()->GetSlotForPetId(pet_number);

    if (!pet_entry)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(pet_entry);
    if (!creatureInfo || !creatureInfo->isTameable(_player->CanTameExoticPets()))
    {
        // if problem in exotic pet
        if (creatureInfo && creatureInfo->isTameable(true))
            SendStableResult(STABLE_ERR_EXOTIC);
        else
            SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // Update if its a Hunter pet
    if (new_slot != 100)
    {
        // We need to remove and add the new pet to there diffrent slots
        GetPlayer()->SwapPetSlot(slot, (PetSlot)new_slot);
    }

    SendStableResult(STABLE_SUCCESS_STABLE);
}

void WorldSession::HandleStableRevivePet(WorldPacket & recvData )
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleStableRevivePet: Not implemented");
}

bool WorldSession::CheckStableMaster(ObjectGuid const& guid)
{
    // spell case or GM
    if (guid == GetPlayer()->GetGUID())
    {
        if (!GetPlayer()->isGameMaster() && !GetPlayer()->HasAuraType(SPELL_AURA_OPEN_STABLE))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Player (GUID:%u) attempt open stable in cheating way.", guid.GetGUIDLow());
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Stablemaster (GUID:%u) not found or you can't interact with him.", guid.GetGUIDLow());
            return false;
        }
    }
    return true;
}

void WorldSession::SendStableResult(StableResultCode res)
{
    WorldPackets::PetPackets::StableResult stableResult;
    stableResult.Result = res;
    SendPacket(stableResult.Write());
}

void WorldSession::HandlePetActionHelper(Unit* pet, ObjectGuid petGuid, uint32 spellid, uint16 flag, ObjectGuid targetGuid, Position pos)
{
    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetAction(petGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!",
            petGuid, targetGuid, spellid, flag, pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    switch (flag)
    {
        case ACT_COMMAND:                                   //0x07
            switch (spellid)
            {
                case COMMAND_STAY:                          //flat=1792  //STAY
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MoveIdle();
                    charmInfo->SetCommandState(COMMAND_STAY);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                case COMMAND_FOLLOW:                        //spellid=1792  //FOLLOW
                    pet->AttackStop();
                    pet->InterruptNonMeleeSpells(false);
                    pet->GetMotionMaster()->MoveFollow(_player, PET_FOLLOW_DIST, pet->GetFollowAngle());
                    charmInfo->SetCommandState(COMMAND_FOLLOW);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(false);
                    charmInfo->SetIsReturning(true);
                    charmInfo->SetIsFollowing(false);
                    break;
                case COMMAND_ATTACK:                        //spellid=1792  //ATTACK
                {
                    // Can't attack if owner is pacified
                    if (_player->HasAuraType(SPELL_AURA_MOD_PACIFY))
                    {
                        //pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                        //TODO: Send proper error message to client
                        return;
                    }

                    // only place where pet can be player
                    Unit* TargetUnit = ObjectAccessor::GetUnit(*_player, targetGuid);
                    if (!TargetUnit)
                        return;

                    if (Unit* owner = pet->GetOwner())
                        if (!owner->IsValidAttackTarget(TargetUnit))
                            return;

                    // Not let attack through obstructions
                    if (sWorld->getBoolConfig(CONFIG_PET_LOS))
                    {
                        if (!pet->IsWithinLOSInMap(TargetUnit))
                            return;
                    }

                    pet->ClearUnitState(UNIT_STATE_FOLLOW);
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != TargetUnit || (pet->getVictim() == TargetUnit && !pet->GetCharmInfo()->IsCommandAttack()))
                    {
                        if (pet->getVictim())
                            pet->AttackStop();

                        if (pet->GetTypeId() != TYPEID_PLAYER && pet->ToCreature()->IsAIEnabled)
                        {
                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->ToCreature()->AI()->AttackStart(TargetUnit);

                            //10% chance to play special pet attack talk, else growl
                            if (pet->ToCreature()->isPet() && ((Pet*)pet)->getPetType() == SUMMON_PET && pet != TargetUnit && urand(0, 100) < 10)
                                pet->SendPetTalk((uint32)PET_TALK_ATTACK);
                            else
                                pet->SendPetAIReaction(petGuid); // 90% chance for pet and 100% chance for charmed creature
                        }
                        else                                // charmed player
                        {
                            if (pet->getVictim() && pet->getVictim() != TargetUnit)
                                pet->AttackStop();

                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->Attack(TargetUnit, true);
                            pet->SendPetAIReaction(petGuid);
                        }
                    }
                    break;
                }
                case COMMAND_ABANDON:                       // abandon (hunter pet) or dismiss (summoned pet)
                    if (pet->GetCharmerGUID() == GetPlayer()->GetGUID())
                        _player->StopCastingCharm();
                    else if (pet->GetOwnerGUID() == GetPlayer()->GetGUID())
                    {
                        ASSERT(pet->GetTypeId() == TYPEID_UNIT);
                        if (pet->isPet())
                            GetPlayer()->RemovePet((Pet*)pet);
                        else if (pet->HasUnitTypeMask(UNIT_MASK_MINION))
                        {
                            ((Minion*)pet)->UnSummon();
                        }
                    }
                    break;
                case COMMAND_MOVE_TO:
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MovePoint(0, pos);
                    charmInfo->SetCommandState(COMMAND_MOVE_TO);
                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;

                default:
                    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
            }
            break;
        case ACT_REACTION:                                  // 0x6
            switch (spellid)
            {
                case REACT_PASSIVE:                         //passive
                    pet->AttackStop();
                    //pet->GetMotionMaster()->Clear();
                    pet->GetMotionMaster()->MoveFollow(_player, PET_FOLLOW_DIST, pet->GetFollowAngle());
                    charmInfo->SetIsReturning(true);
                case REACT_DEFENSIVE:                       //recovery
                case REACT_HELPER:
                case REACT_AGGRESSIVE:
                    if (pet->GetTypeId() == TYPEID_UNIT)
                        pet->ToCreature()->SetReactState(ReactStates(spellid));
                    break;
                default:
                    break;
            }
            break;
        case ACT_DISABLED:                                  // 0x81    spell (disabled), ignore
        case ACT_PASSIVE:                                   // 0x01
        case ACT_ENABLED:                                   // 0xC1    spell
        {
            Unit* unit_target = nullptr;

            if (targetGuid)
                unit_target = ObjectAccessor::GetUnit(*_player, targetGuid);

            // do not cast unknown spells
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", spellid);
                return;
            }

            if (spellInfo->Categories.StartRecoveryCategory)
                if (pet->GetCharmInfo() && pet->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                    return;

            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                    return;
            }

            // do not cast not learned spells
            if (!pet->HasSpell(spellid) || spellInfo->IsPassive())
                return;

            //  Clear the flags as if owner clicked 'attack'. AI will reset them
            //  after AttackStart, even if spell failed
            if (pet->GetCharmInfo())
            {
                pet->GetCharmInfo()->SetIsAtStay(false);
                pet->GetCharmInfo()->SetIsCommandAttack(true);
                pet->GetCharmInfo()->SetIsReturning(false);
                pet->GetCharmInfo()->SetIsFollowing(false);
            }

            TriggerCastData triggerData;
            Spell* spell = new Spell(pet, spellInfo, triggerData);

            SpellCastResult result = spell->CheckPetCast(unit_target);

            //auto turn to target unless possessed
            if (result == SPELL_FAILED_UNIT_NOT_INFRONT && !pet->isPossessed() && !pet->IsVehicle())
            {
                if (unit_target)
                {
                    pet->SetInFront(unit_target);
                    if (unit_target->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer((Player*)unit_target);
                }
                else if (Unit* unit_target2 = spell->m_targets.GetUnitTarget())
                {
                    pet->SetInFront(unit_target2);
                    if (unit_target2->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer((Player*)unit_target2);
                }

                if (Unit* powner = pet->GetCharmerOrOwner())
                    if (powner->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer(powner->ToPlayer());

                result = SPELL_CAST_OK;
            }

            if (result == SPELL_CAST_OK)
            {
                pet->ToCreature()->AddCreatureSpellCooldown(spellid);

                unit_target = spell->m_targets.GetUnitTarget();

                //10% chance to play special pet attack talk, else growl
                //actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (pet->ToCreature()->isPet() && (((Pet*)pet)->getPetType() == SUMMON_PET) && (pet != unit_target) && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(petGuid);

                if (unit_target && !GetPlayer()->IsFriendlyTo(unit_target) && !pet->isPossessed() && !pet->IsVehicle())
                {
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != unit_target)
                    {
                        if (pet->getVictim())
                            pet->AttackStop();
                        pet->GetMotionMaster()->Clear();
                        if (pet->ToCreature()->IsAIEnabled)
                            pet->ToCreature()->AI()->AttackStart(unit_target);
                    }
                }

                spell->prepare(&(spell->m_targets));
            }
            else
            {
                spell->SendCastResult(GetPlayer(), spellInfo, result, SPELL_CUSTOM_ERROR_NONE, 0, !(pet->isPossessed() || pet->IsVehicle()));
                if (!pet->ToCreature()->HasSpellCooldown(spellid))
                    GetPlayer()->SendClearCooldown(spellid, pet);

                spell->finish(false);
                delete spell;

                // reset specific flags in case of spell fail. AI will reset other flags
                if (pet->GetCharmInfo())
                    pet->GetCharmInfo()->SetIsCommandAttack(false);
            }
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

void WorldSession::SendPetNameInvalid(uint32 error, ObjectGuid const& guid, const std::string& name, DeclinedName *declinedName)
{
    WorldPackets::PetPackets::PetNameInvalid petNameInvalid;
    petNameInvalid.Result = error;
    petNameInvalid.RenameData.NewName = name;
    petNameInvalid.RenameData.PetGUID = guid;
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
        petNameInvalid.RenameData.DeclinedNames.name[i] = declinedName->name[i];

    SendPacket(petNameInvalid.Write());
}

void WorldSession::SendStablePet(ObjectGuid const& guid /*= ObjectGuid::Empty*/)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_DETAIL);
    stmt->setUInt64(0, _player->GetGUIDLow());
    _sendStabledPetCallback.SetParam(guid);
    _sendStabledPetCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::SendStablePetCallback(PreparedQueryResult result, ObjectGuid const& guid)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    WorldPackets::PetPackets::StableList list;
    ObjectGuid StableMaster = guid;

    std::set<uint32> stableNumber;
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            uint32 petNumber = fields[0].GetUInt32();
            PetSlot petSlot = player->GetSlotForPetId(petNumber);
            stableNumber.insert(petNumber);

            if (petSlot > PET_SLOT_STABLE_LAST)
                continue;

            if (petSlot == PET_SLOT_FULL_LIST)
                petSlot = (PetSlot)player->SetOnAnyFreeSlot(petNumber);

            if (petSlot >= PET_SLOT_HUNTER_FIRST &&  petSlot < PET_SLOT_STABLE_LAST)
            {
                WorldPackets::PetPackets::StableInfo info;
                info.PetSlot = petSlot;
                info.PetNumber = petNumber;
                info.CreatureID = fields[1].GetUInt32();
                info.DisplayID = fields[4].GetUInt32();
                info.ExperienceLevel = fields[2].GetUInt16();
                info.PetFlags = petSlot < PET_SLOT_STABLE_FIRST ? 1 : 3;
                info.PetName = fields[3].GetString();
                list.Stables.push_back(info);
            }
        }
        while (result->NextRow());
    }

    if (player->getClass() == CLASS_HUNTER)
    {
        SendPacket(list.Write());
        SendStableResult(STABLE_ERR_NONE);
    }

    PlayerPetSlotList const& petSlots = player->GetPetSlotList();
    for (uint32 i = uint32(PET_SLOT_HUNTER_FIRST); i < uint32(PET_SLOT_STABLE_LAST); ++i)
    {
        if (!petSlots[i])
            continue;

        std::set<uint32>::iterator find = stableNumber.find(petSlots[i]);
        if (find == stableNumber.end())
            player->cleanPetSlotForMove(PetSlot(i), petSlots[i]);
    }
}
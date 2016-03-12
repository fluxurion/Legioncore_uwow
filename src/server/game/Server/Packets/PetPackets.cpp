/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#include "PetPackets.h"

WorldPacket const* WorldPackets::PetPackets::Spells::Write()
{
    _worldPacket << PetGUID;
    _worldPacket << CreatureFamily;
    _worldPacket << Specialization;
    _worldPacket << TimeLimit;
    _worldPacket << PetModeAndOrders;

    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; i++)
        _worldPacket << Buttons[i];

    _worldPacket << static_cast<uint32>(Actions.size());
    _worldPacket << static_cast<uint32>(Cooldowns.size());
    _worldPacket << static_cast<uint32>(Historys.size());

    for (auto const& map : Actions)
        _worldPacket << map;

    for (auto const& map : Cooldowns)
    {
        _worldPacket << map.SpellID;
        _worldPacket << map.Duration;
        _worldPacket << map.CategoryDuration;
        _worldPacket << map.Category;
    }

    for (auto const& map : Historys)
    {
        _worldPacket << map.CategoryID;
        _worldPacket << map.RecoveryTime;
        _worldPacket << map.ConsumedCharges;
    }

    return &_worldPacket;
}

void WorldPackets::PetPackets::Action::Read()
{
    uint16 a1 = 0;
    uint8 a2 = 0;

    _worldPacket >> PetGUID;
    _worldPacket >> a1 >> a2;
    ActionData = a1 + (a2 << 16);
    _worldPacket >> Slot;
    _worldPacket >> TargetGUID;
    _worldPacket >> ActionPosition.x;
    _worldPacket >> ActionPosition.y;
    _worldPacket >> ActionPosition.z;
}

WorldPacket const* WorldPackets::PetPackets::SetPetSpecialization::Write()
{
    _worldPacket << SpecializationID;

    return &_worldPacket;
}

void WorldPackets::PetPackets::LearnPetSpecializationGroup::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> SpecGroupId;
}

WorldPacket const* WorldPackets::PetPackets::LearnedRemovedSpells::Write()
{
    _worldPacket << static_cast<uint32>(SpellIDs.size());
    for (auto const& map : SpellIDs)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::PetPackets::Guids::Write()
{
    _worldPacket << static_cast<uint32>(PetGUIDs.size());
    for (ObjectGuid const& map : PetGUIDs)
        _worldPacket << map;

    return &_worldPacket;
}

void WorldPackets::PetPackets::DismissCritter::Read()
{
    _worldPacket >> CritterGUID;
}

WorldPacket const* WorldPackets::PetPackets::Sound::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket << Action;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::PetPackets::Mode::Write()
{
    _worldPacket << PetGUID;

    //! custom code - jam data looks like: x >> 0 + x2 >> 8 + x2 >> 16
    _worldPacket << static_cast<uint8>(_reactState);
    _worldPacket << static_cast<uint8>(_commandState);
    _worldPacket << static_cast<uint16>(_flag);

    return &_worldPacket;
}

void WorldPackets::PetPackets::StopAttack::Read()
{
    _worldPacket >> PetGUID;
}

void WorldPackets::PetPackets::SetAction::Read()
{
    uint16 a1 = 0;
    uint8 a2 = 0;

    _worldPacket >> PetGUID;
    _worldPacket >> Index;
    _worldPacket >> a1 >> a2;
    Action = a1 + (a2 << 16);
    _worldPacket >> Unk;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::PetPackets::StableInfo const& info)
{
    data << info.PetSlot;
    data << info.PetNumber;
    data << info.CreatureID;
    data << info.DisplayID;
    data << info.ExperienceLevel;
    data << info.PetFlags;

    data.WriteBits(info.PetName.length(), 8);
    data.WriteString(info.PetName);

    return data;
}

WorldPacket const* WorldPackets::PetPackets::StableList::Write()
{
    _worldPacket << StableMaster;
    _worldPacket << static_cast<uint32>(Stables.size());
    for (auto const& map : Stables)
        _worldPacket << map;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::PetPackets::Added::Write()
{
    _worldPacket << Stable;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::PetPackets::ActionFeedback::Write()
{
    _worldPacket << SpellID;
    _worldPacket << Response;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::PetPackets::StableResult::Write()
{
    _worldPacket << Result;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::PetPackets::TameFailure::Write()
{
    _worldPacket << Result;

    return &_worldPacket;
}

void WorldPackets::PetPackets::PetCancelAura::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> SpellID;
}

void WorldPackets::PetPackets::PetAbandon::Read()
{
    _worldPacket >> Pet;
}

void WorldPackets::PetPackets::PetAction::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> Action;
    _worldPacket >> TargetGUID;
    _worldPacket >> ActionPosition.PositionXYZStream();
}

void WorldPackets::PetPackets::PetRename::Read()
{
    _worldPacket >> RenameData.PetGUID;
    _worldPacket >> RenameData.PetNumber;

    int8 nameLen = 0;
    _worldPacket >> nameLen;

    RenameData.HasDeclinedNames = _worldPacket.ReadBit();
    if (RenameData.HasDeclinedNames)
    {
        int32 count[MAX_DECLINED_NAME_CASES] = { };
        for (int32 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
            count[i] = _worldPacket.ReadBits(7);

        for (int32 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
            RenameData.DeclinedNames.name[i] = _worldPacket.ReadString(count[i]);
    }

    _worldPacket.ReadString(nameLen, RenameData.NewName);
}

void WorldPackets::PetPackets::PetSetAction::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> Index;
    _worldPacket >> Action;
}

void WorldPackets::PetPackets::PetSpellAutocast::Read()
{
    _worldPacket >> PetGUID;
    _worldPacket >> SpellID;
    AutocastEnabled = _worldPacket.ReadBit();
}

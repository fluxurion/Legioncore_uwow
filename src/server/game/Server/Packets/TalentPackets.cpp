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

#include "TalentPackets.h"

WorldPacket const* WorldPackets::Talent::UpdateTalentData::Write()
{
    _worldPacket << Info.ActiveGroup;
    _worldPacket << Info.ActiveSpecID;
    _worldPacket << static_cast<uint32>(Info.TalentGroups.size());

    for (auto const& talentGroupInfo : Info.TalentGroups)
    {
        _worldPacket << talentGroupInfo.SpecID;

        _worldPacket << static_cast<uint32>(talentGroupInfo.TalentIDs.size());
        _worldPacket << static_cast<uint32>(talentGroupInfo.PvPTalentIDs.size());

        for (uint16 talentID : talentGroupInfo.TalentIDs)
            _worldPacket << talentID;
            
        for (uint16 talentID : talentGroupInfo.PvPTalentIDs)
            _worldPacket << talentID;
    }

    return &_worldPacket;
}

void WorldPackets::Talent::SetSpecialization::Read()
{
    _worldPacket >> SpecGroupIndex;
}

void WorldPackets::Talent::LearnTalent::Read()
{
    Talents.resize(_worldPacket.read<uint32>());
    for (uint16& talent : Talents)
        _worldPacket >> talent;
}

void WorldPackets::Talent::LearnPvpTalents::Read()
{
    TalentIDs.resize(_worldPacket.read<uint32>());
    for (uint16& talent : TalentIDs)
        _worldPacket >> talent;
}

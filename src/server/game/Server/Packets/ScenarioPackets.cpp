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

#include "ScenarioPackets.h"

void WorldPackets::Scenario::QueryScenarioPOI::Read()
{
    MissingScenarioPOITreeIDs.resize(_worldPacket.read<uint32>());
    for (size_t i = 0; i < MissingScenarioPOITreeIDs.size(); i++)
        MissingScenarioPOITreeIDs.push_back(_worldPacket.read<uint32>());

    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WorldPackets::Scenario::QueryScenarioPOI size %u", MissingScenarioPOITreeIDs.size());
}

WorldPacket const* WorldPackets::Scenario::ScenarioPOIs::Write()
{
    sLog->outDebug(LOG_FILTER_SPELLS_AURAS, "WorldPackets::Scenario::ScenarioPOIs size %u", PoiInfos.size());

    _worldPacket << static_cast<uint32>(PoiInfos.size());
    for (auto const& map : PoiInfos)
    {
        _worldPacket << map.CriteriaTreeID;
        _worldPacket << static_cast<uint32>(map.BlobDatas.size());
        for (auto const& blob : map.BlobDatas)
        {
            _worldPacket << blob.BlobID;
            _worldPacket << blob.MapID;
            _worldPacket << blob.WorldMapAreaID;
            _worldPacket << blob.Floor;
            _worldPacket << blob.Priority;
            _worldPacket << blob.Flags;
            _worldPacket << blob.WorldEffectID;
            _worldPacket << blob.PlayerConditionID;
            _worldPacket << static_cast<uint32>(blob.Points.size());
            for (auto const& point : blob.Points)
            {
                _worldPacket << point.X;
                _worldPacket << point.Y;
            }
        }
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Scenario::ScenarioProgressUpdate::Write()
{
    _worldPacket << Progress;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Scenario::ScenarioState::Write()
{
    _worldPacket << ScenarioID;
    _worldPacket << CurrentStep;
    _worldPacket << DifficultyID;
    _worldPacket << WaveCurrent;
    _worldPacket << WaveMax;
    _worldPacket << TimerDuration;
    _worldPacket << static_cast<uint32>(Progress.size());
    _worldPacket << static_cast<uint32>(Objectives.size());
    _worldPacket << static_cast<uint32>(ActiveSteps.size());
    _worldPacket << static_cast<uint32>(UnkDatas.size());

    for (auto const& map : Progress)
        _worldPacket << map;

    for (auto const& obj : Objectives)
    {
        _worldPacket << obj.BonusObjectiveID;
        _worldPacket.WriteBit(obj.ObjectiveComplete);
        _worldPacket.FlushBits();
    }
    
    for (auto const& v : ActiveSteps)
        _worldPacket << v;

    for (auto const& obj : UnkDatas)
    {
        _worldPacket << obj.UnkInt;
        _worldPacket.WriteBit(obj.UnkBool);
        _worldPacket.FlushBits();
    }

    _worldPacket.WriteBit(ScenarioComplete);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Scenario::ScenarioCompleted::Write()
{
    _worldPacket << ScenarioID;

    return &_worldPacket;
}

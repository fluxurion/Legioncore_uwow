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

#include "MiscPackets.h"
#include "PacketUtilities.h"

WorldPacket const* WorldPackets::Misc::BindPointUpdate::Write()
{
    _worldPacket << BindPosition;
    _worldPacket << uint32(BindMapID);
    _worldPacket << uint32(BindAreaID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::InvalidatePlayer::Write()
{
    _worldPacket << Guid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::LoginSetTimeSpeed::Write()
{
    _worldPacket.AppendPackedTime(ServerTime);
    _worldPacket.AppendPackedTime(GameTime);
    _worldPacket << float(NewSpeed);
    _worldPacket << uint32(ServerTimeHolidayOffset);
    _worldPacket << uint32(GameTimeHolidayOffset);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetCurrency::Write()
{
    _worldPacket << uint32(Type);
    _worldPacket << uint32(Quantity);
    _worldPacket << uint32(Flags);
    _worldPacket.WriteBit(WeeklyQuantity.is_initialized());
    _worldPacket.WriteBit(TrackedQuantity.is_initialized());
    _worldPacket.WriteBit(SuppressChatLog);
    _worldPacket.FlushBits();

    if (WeeklyQuantity)
        _worldPacket << *WeeklyQuantity;

    if (TrackedQuantity)
        _worldPacket << *TrackedQuantity;

    return &_worldPacket;
}

void WorldPackets::Misc::SetSelection::Read()
{
    _worldPacket >> Selection;
}

WorldPacket const* WorldPackets::Misc::SetupCurrency::Write()
{
    _worldPacket << static_cast<uint32>(Data.size());

    for (Record const& data : Data)
    {
        _worldPacket << uint32(data.Type);
        _worldPacket << uint32(data.Quantity);

        _worldPacket.WriteBit(data.WeeklyQuantity.is_initialized());
        _worldPacket.WriteBit(data.MaxWeeklyQuantity.is_initialized());
        _worldPacket.WriteBit(data.TrackedQuantity.is_initialized());
        _worldPacket.WriteBit(data.Unk.is_initialized());
        _worldPacket.WriteBit(data.Unk2.is_initialized());
        _worldPacket.WriteBits(data.Flags, 5);
        _worldPacket.FlushBits();

        if (data.WeeklyQuantity)
            _worldPacket << *data.WeeklyQuantity;

        if (data.MaxWeeklyQuantity)
            _worldPacket << *data.MaxWeeklyQuantity;

        if (data.TrackedQuantity)
            _worldPacket << *data.TrackedQuantity;

        if (data.Unk)
            _worldPacket << *data.Unk;

        if (data.Unk2)
            _worldPacket << *data.Unk2;
    }

    return &_worldPacket;
}

void WorldPackets::Misc::ViolenceLevel::Read()
{
    _worldPacket >> ViolenceLvl;
}

WorldPacket const* WorldPackets::Misc::TimeSyncRequest::Write()
{
    _worldPacket << SequenceIndex;

    return &_worldPacket;
}

void WorldPackets::Misc::TimeSyncResponse::Read()
{
    _worldPacket >> SequenceIndex;
    _worldPacket >> ClientTime;
}

WorldPacket const* WorldPackets::Misc::UITime::Write()
{
    _worldPacket << Time;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::TriggerMovie::Write()
{
    _worldPacket << uint32(MovieID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::TriggerCinematic::Write()
{
    _worldPacket << uint32(CinematicID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::TutorialFlags::Write()
{
    _worldPacket.append(TutorialData, MAX_ACCOUNT_TUTORIAL_VALUES);

    return &_worldPacket;
}

void WorldPackets::Misc::TutorialSetFlag::Read()
{
    Action = _worldPacket.ReadBits(2);
    if (Action == 1)
        _worldPacket >> TutorialBit;
}

WorldPacket const* WorldPackets::Misc::WorldServerInfo::Write()
{
    _worldPacket << uint32(DifficultyID);
    _worldPacket << uint8(IsTournamentRealm);
    _worldPacket << uint32(WeeklyReset);
    _worldPacket.WriteBit(RestrictedAccountMaxLevel.is_initialized());
    _worldPacket.WriteBit(RestrictedAccountMaxMoney.is_initialized());
    _worldPacket.WriteBit(IneligibleForLootMask.is_initialized());
    _worldPacket.WriteBit(InstanceGroupSize.is_initialized());
    _worldPacket.FlushBits();

    if (RestrictedAccountMaxLevel)
        _worldPacket << *RestrictedAccountMaxLevel;

    if (RestrictedAccountMaxMoney)
        _worldPacket << *RestrictedAccountMaxMoney;

    if (IneligibleForLootMask)
        _worldPacket << *IneligibleForLootMask;

    if (InstanceGroupSize)
        _worldPacket << *InstanceGroupSize;

    return &_worldPacket;
}

void WorldPackets::Misc::AreaTrigger::Read()
{
    _worldPacket >> AreaTriggerID;
    Entered = _worldPacket.ReadBit();
    FromClient = _worldPacket.ReadBit();
}

void WorldPackets::Misc::SetDungeonDifficulty::Read()
{
    _worldPacket >> DifficultyID;
}

void WorldPackets::Misc::SetRaidDifficulty::Read()
{
    _worldPacket >> DifficultyID;
    _worldPacket >> Legacy;
}

WorldPacket const* WorldPackets::Misc::DungeonDifficultySet::Write()
{
    _worldPacket << int32(DifficultyID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::RaidDifficultySet::Write()
{
    _worldPacket << int32(DifficultyID);
    _worldPacket << uint8(Legacy);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::CorpseReclaimDelay::Write()
{
    _worldPacket << Remaining;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DeathReleaseLoc::Write()
{
    _worldPacket << MapID;
    _worldPacket << Loc;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PreRessurect::Write()
{
    _worldPacket << PlayerGUID;

    return &_worldPacket;
}

void WorldPackets::Misc::ReclaimCorpse::Read()
{
    _worldPacket >> CorpseGUID;
}

void WorldPackets::Misc::RepopRequest::Read()
{
    CheckInstance = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Misc::RequestCemeteryListResponse::Write()
{
    _worldPacket.WriteBit(IsGossipTriggered);
    _worldPacket.FlushBits();

    _worldPacket << static_cast<uint32>(CemeteryID.size());
    for (uint32 const& cemetery : CemeteryID)
        _worldPacket << cemetery;

    return &_worldPacket;
}

void WorldPackets::Misc::ResurrectResponse::Read()
{
    _worldPacket >> Resurrecter;
    _worldPacket >> Response;
}

WorldPackets::Misc::Weather::Weather() : ServerPacket(SMSG_WEATHER, 4 + 4 + 1) { }

WorldPackets::Misc::Weather::Weather(WeatherState weatherID, float intensity /*= 0.0f*/, bool abrupt /*= false*/)
    : ServerPacket(SMSG_WEATHER, 4 + 4 + 1), Abrupt(abrupt), Intensity(intensity), WeatherID(weatherID)
{ }

WorldPacket const* WorldPackets::Misc::Weather::Write()
{
    _worldPacket << uint32(WeatherID);
    _worldPacket << float(Intensity);
    _worldPacket.WriteBit(Abrupt);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Misc::StandStateChange::Read()
{
    uint32 state;
    _worldPacket >> state;

    StandState = UnitStandStateType(state);
}

WorldPacket const* WorldPackets::Misc::StandStateUpdate::Write()
{
    _worldPacket << AnimKitID;
    _worldPacket << uint8(State);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ArchaeologySurveryCast::Write()
{
    _worldPacket << uint32(ResearchBranchID);
    _worldPacket << uint32(TotalFinds);
    _worldPacket << uint32(NumFindsCompleted);
    _worldPacket.WriteBit(SuccessfulFind);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayerBound::Write()
{
    _worldPacket << BinderID;
    _worldPacket << uint32(AreaID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::BinderConfirm::Write()
{
    _worldPacket << Unit;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StartMirrorTimer::Write()
{
    _worldPacket << int32(Timer);
    _worldPacket << int32(Value);
    _worldPacket << int32(MaxValue);
    _worldPacket << int32(Scale);
    _worldPacket << int32(SpellID);
    _worldPacket.WriteBit(Paused);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PauseMirrorTimer::Write()
{
    _worldPacket << int32(Timer);
    _worldPacket.WriteBit(Paused);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StopMirrorTimer::Write()
{
    _worldPacket << int32(Timer);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ExplorationExperience::Write()
{
    _worldPacket << int32(AreaID);
    _worldPacket << int32(Experience);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::LevelUpInfo::Write()
{
    _worldPacket << int32(Level);
    _worldPacket << int32(HealthDelta);

    for (int32 const& power : PowerDelta)
        _worldPacket << power;

    for (int32 const& stat : StatDelta)
        _worldPacket << stat;

    _worldPacket << int32(Cp);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayMusic::Write()
{
    _worldPacket << uint32(SoundKitID);

    return &_worldPacket;
}

void WorldPackets::Misc::RandomRollClient::Read()
{
    _worldPacket >> Min;
    _worldPacket >> Max;
    _worldPacket >> PartyIndex;
}

WorldPacket const* WorldPackets::Misc::RandomRoll::Write()
{
    _worldPacket << Roller;
    _worldPacket << RollerWowAccount;
    _worldPacket << int32(Min);
    _worldPacket << int32(Max);
    _worldPacket << int32(Result);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PhaseShift::Write()
{
    _worldPacket << ClientGUID;                                 // CLientGUID
    _worldPacket << static_cast<uint32>(PhaseShifts.size() ? 0 : 8);         // PhaseShiftFlags
    _worldPacket << static_cast<uint32>(PhaseShifts.size());                 // PhaseShiftCount
    _worldPacket << PersonalGUID;                               // PersonalGUID
    for (uint32 const& phase : PhaseShifts)
    {
        _worldPacket << uint16(1);                              // PhaseFlags
        _worldPacket << uint16(phase);                          // PhaseID
    }

    _worldPacket << static_cast<uint32>(VisibleMapIDs.size() * 2);           // Active terrain swaps size
    for (uint32 const& map : VisibleMapIDs)
        _worldPacket << uint16(map);                            // Active terrain swap map id

    _worldPacket << static_cast<uint32>(PreloadMapIDs.size() * 2);           // Inactive terrain swaps size
    for (uint32 const& map : PreloadMapIDs)
        _worldPacket << uint16(map);                            // Inactive terrain swap map id

    _worldPacket << static_cast<uint32>(UiWorldMapAreaIDSwaps.size() * 2);   // UI map swaps size
    for (uint32 const& map : UiWorldMapAreaIDSwaps)
        _worldPacket << uint16(map);                            // UI map id, WorldMapArea.dbc, controls map display

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ZoneUnderAttack::Write()
{
    _worldPacket << int32(AreaID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DurabilityDamageDeath::Write()
{
    _worldPacket << int32(Percent);

    return &_worldPacket;
}

void WorldPackets::Misc::ObjectUpdateFailed::Read()
{
    _worldPacket >> ObjectGUID;
}

void WorldPackets::Misc::ObjectUpdateRescued::Read()
{
    _worldPacket >> ObjectGUID;
}

WorldPacket const* WorldPackets::Misc::PlaySound::Write()
{
    _worldPacket << int32(SoundKitID);
    _worldPacket << SourceObjectGuid;

    return &_worldPacket;
}

void WorldPackets::Misc::FarSight::Read()
{
    Enable = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Misc::Dismount::Write()
{
    _worldPacket << Guid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetAIAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetMovementAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetMeleeAnimKit::Write()
{
    _worldPacket << Unit;
    _worldPacket << uint16(AnimKitID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetPlayHoverAnim::Write()
{
    _worldPacket << UnitGUID;
    _worldPacket.WriteBit(PlayHoverAnim);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Misc::SetPvP::Read()
{
    EnablePVP = _worldPacket.ReadBit();
}

void WorldPackets::Misc::WorldTeleport::Read()
{
    _worldPacket >> MapID;
    _worldPacket >> TransportGUID;
    _worldPacket >> Pos;
    _worldPacket >> Facing;
}

WorldPacket const* WorldPackets::Misc::SummonRequest::Write()
{
    _worldPacket << SummonerGUID;
    _worldPacket << SummonerVirtualRealmAddress;
    _worldPacket << AreaID;
    _worldPacket.WriteBit(ConfirmSummon);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SpecialMountAnim::Write()
{
    _worldPacket << UnitGUID;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Misc::TaskProgress const& progress)
{
    data << progress.TaskID;
    data << uint32(progress.FailureTime);
    data << progress.Flags;
    for (uint16 const& x : progress.Counts)
        data << x;

    return data;
}

WorldPacket const* WorldPackets::Misc::UpdateTaskProgress::Write()
{
    _worldPacket << static_cast<uint32>(Progress.size());
    for (auto const& x : Progress)
        _worldPacket << x;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::SetAllTaskProgress::Write()
{
    _worldPacket << static_cast<uint32>(Progress.size());
    for (auto const& x : Progress)
        _worldPacket << x;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StreamingMovie::Write()
{
    _worldPacket << static_cast<uint32>(MovieIDs.size());
    for (auto const& v : MovieIDs)
        _worldPacket << v;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StopElapsedTimer::Write()
{
    _worldPacket << TimerID;
    _worldPacket.WriteBit(KeepTimer);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::StartElapsedTimer::Write()
{
    _worldPacket << TimerID;
    _worldPacket << CurrentDuration;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::ShowTradeSkillResponse::Write()
{
    _worldPacket << PlayerGUID;
    _worldPacket << SpellId;
    _worldPacket << static_cast<uint32>(SkillLineIDs.size());
    _worldPacket << static_cast<uint32>(SkillRanks.size());
    _worldPacket << static_cast<uint32>(SkillMaxRanks.size());
    _worldPacket << static_cast<uint32>(KnownAbilitySpellIDs.size());

    for (auto const& v : SkillLineIDs)
        _worldPacket << v;

    for (auto const& c : SkillRanks)
        _worldPacket << c;

    for (auto const& z : SkillMaxRanks)
        _worldPacket << z;

    for (auto const& t : KnownAbilitySpellIDs)
        _worldPacket << t;

    return &_worldPacket;
}

void WorldPackets::Misc::ShowTradeSkill::Read()
{
    _worldPacket >> PlayerGUID;
    _worldPacket >> SpellID;
    _worldPacket >> SkillLineID;
}

WorldPacket const* WorldPackets::Misc::SetTaskComplete::Write()
{
    _worldPacket << TaskID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlayerSkinned::Write()
{
    _worldPacket.WriteBit(FreeRepop);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::PlaySpeakerbotSound::Write()
{
    _worldPacket << SourceObjectGUID;
    _worldPacket << SoundID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::MountResult::Write()
{
    _worldPacket << uint32(Result);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DisplayGameError::Write()
{
    _worldPacket << Error;
    _worldPacket.WriteBit(Arg.is_initialized());
    _worldPacket.WriteBit(Arg2.is_initialized());

    if (Arg)
        _worldPacket << *Arg;

    if (Arg2)
        _worldPacket << *Arg2;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DismountResult::Write()
{
    _worldPacket << Result;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::DisenchantCredit::Write()
{
    _worldPacket << Disenchanter;
    _worldPacket << Item;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::CustomLoadScreen::Write()
{
    _worldPacket << TeleportSpellID;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::RespecWipeConfirm::Write()
{
    _worldPacket << int8(respecType);
    _worldPacket << Cost;
    _worldPacket << RespecMaster;

    return &_worldPacket;
}

void WorldPackets::Misc::ConfirmRespecWipe::Read()
{
    _worldPacket >> RespecMaster;
    respecType = static_cast<RespecType>(_worldPacket.read<uint8>());
}

WorldPacket const* WorldPackets::Misc::CrossedInebriationThreshold::Write()
{
    _worldPacket << Guid;
    _worldPacket << Threshold;
    _worldPacket << ItemID;

    return &_worldPacket;
}

void WorldPackets::Misc::SaveCUFProfiles::Read()
{
    uint32 count = 0;
    _worldPacket >> count;

    for (uint8 i = 0; i < count && i < MAX_CUF_PROFILES; i++)
    {
        std::unique_ptr<CUFProfile> cufProfile = Trinity::make_unique<CUFProfile>();

        uint8 strLen = _worldPacket.ReadBits(7);
        for (uint8 option = 0; option < CUF_BOOL_OPTIONS_COUNT; option++)
            cufProfile->BoolOptions.set(option, _worldPacket.ReadBit());

        _worldPacket >> cufProfile->FrameHeight;
        _worldPacket >> cufProfile->FrameWidth;
        _worldPacket >> cufProfile->SortBy;
        _worldPacket >> cufProfile->HealthText;
        _worldPacket >> cufProfile->TopPoint;
        _worldPacket >> cufProfile->BottomPoint;
        _worldPacket >> cufProfile->LeftPoint;
        _worldPacket >> cufProfile->TopOffset;
        _worldPacket >> cufProfile->BottomOffset;
        _worldPacket >> cufProfile->LeftOffset;
        _worldPacket.ReadString(strLen, cufProfile->ProfileName);

        CUFProfiles.push_back(std::move(cufProfile));
    }
}

WorldPacket const* WorldPackets::Misc::LoadCUFProfiles::Write()
{
    _worldPacket << uint32(CUFProfiles.size());

    for (CUFProfile const* cufProfile : CUFProfiles)
    {
        _worldPacket.WriteBits(cufProfile->ProfileName.size(), 7);

        for (uint8 option = 0; option < CUF_BOOL_OPTIONS_COUNT; option++)
            _worldPacket.WriteBit(cufProfile->BoolOptions[option]);

        _worldPacket << cufProfile->FrameHeight;
        _worldPacket << cufProfile->FrameWidth;
        _worldPacket << cufProfile->SortBy;
        _worldPacket << cufProfile->HealthText;
        _worldPacket << cufProfile->TopPoint;
        _worldPacket << cufProfile->BottomPoint;
        _worldPacket << cufProfile->LeftPoint;
        _worldPacket << cufProfile->TopOffset;
        _worldPacket << cufProfile->BottomOffset;
        _worldPacket << cufProfile->LeftOffset;

        _worldPacket.WriteString(cufProfile->ProfileName);
    }

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Misc::OverrideLight::Write()
{
    _worldPacket << int32(AreaLightID);
    _worldPacket << int32(OverrideLightID);
    _worldPacket << int32(TransitionMilliseconds);

    return &_worldPacket;
}

void WorldPackets::Misc::WardenData::Read()
{
    if (uint32 size = _worldPacket.read<uint32>())
    {
        Data.resize(size);
        _worldPacket.read(Data.contents(), size);
    }
}

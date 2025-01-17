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

#include "CombatLogPackets.h"
#include "SpellPackets.h"

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::CombatLog::PlayerStatsInfo const& info)
{
    data.WriteBits(info.UnkShort1, 2);
    data.FlushBits();

    data << info.UnkShort2;

    data << info.UnkByte1;
    data << info.UnkByte2;
    data << info.UnkByte3;
    data << info.UnkByte4;
    data << info.UnkByte5;

    data << info.UnkSByte;

    return data;
}

WorldPacket const* WorldPackets::CombatLog::SpellNonMeleeDamageLog::Write()
{
    *this << Me;
    *this << CasterGUID;
    *this << CastGuid;
    *this << int32(SpellID);
    *this << int32(Damage);
    *this << int32(Overkill);
    *this << uint8(SchoolMask);
    *this << int32(ShieldBlock);
    *this << int32(Resisted);
    *this << int32(Absorbed);
    FlushBits();
    WriteBit(Periodic);
    WriteBits(Flags, 7);
    WriteBit(false); // HasDebugInfo
    WriteLogDataBit();
    WriteBit(PlayerStats.is_initialized());
    WriteLogDataBit();
    FlushBits();

    if (PlayerStats)
        *this << *PlayerStats;

    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::EnvironmentalDamageLog::Write()
{
    *this << Victim;
    *this << uint8(Type);
    *this << int32(Amount);
    *this << int32(Resisted);
    *this << int32(Absorbed);
    WriteLogDataBit();
    FlushBits();
    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellExecuteLog::Write()
{
    *this << Caster;
    *this << SpellID;
    *this << static_cast<uint32>(Effects.size());

    for (SpellLogEffect const& effect : Effects)
    {
        *this << effect.Effect;

        *this << static_cast<uint32>(effect.PowerDrainTargets.size());
        *this << static_cast<uint32>(effect.ExtraAttacksTargets.size());
        *this << static_cast<uint32>(effect.DurabilityDamageTargets.size());
        *this << static_cast<uint32>(effect.GenericVictimTargets.size());
        *this << static_cast<uint32>(effect.TradeSkillTargets.size());
        *this << static_cast<uint32>(effect.FeedPetTargets.size());

        for (SpellLogEffectPowerDrainParams const& powerDrainTarget : effect.PowerDrainTargets)
        {
            *this << powerDrainTarget.Victim;
            *this << powerDrainTarget.Points;
            *this << powerDrainTarget.PowerType;
            *this << powerDrainTarget.Amplitude;
        }

        for (SpellLogEffectExtraAttacksParams const& extraAttacksTarget : effect.ExtraAttacksTargets)
        {
            *this << extraAttacksTarget.Victim;
            *this << extraAttacksTarget.NumAttacks;
        }

        for (SpellLogEffectDurabilityDamageParams const& durabilityDamageTarget : effect.DurabilityDamageTargets)
        {
            *this << durabilityDamageTarget.Victim;
            *this << durabilityDamageTarget.ItemID;
            *this << durabilityDamageTarget.Amount;
        }

        for (SpellLogEffectGenericVictimParams const& genericVictimTarget : effect.GenericVictimTargets)
            *this << genericVictimTarget.Victim;

        for (SpellLogEffectTradeSkillItemParams const& tradeSkillTarget : effect.TradeSkillTargets)
            *this << tradeSkillTarget.ItemID;

        for (SpellLogEffectFeedPetParams const& feedPetTarget : effect.FeedPetTargets)
            *this << feedPetTarget.ItemID;
    }

    WriteLogDataBit();
    FlushBits();
    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellHealLog::Write()
{
    *this << TargetGUID;
    *this << CasterGUID;
    *this << int32(SpellID);
    *this << int32(Health);
    *this << int32(OverHeal);
    *this << int32(Absorbed);
    WriteBit(Crit);
    WriteBit(CritRollMade.is_initialized());
    WriteBit(CritRollNeeded.is_initialized());
    WriteLogDataBit();
    WriteBit(false); // HasUnkData
    FlushBits();

    if (CritRollMade)
        *this << *CritRollMade;

    if (CritRollNeeded)
        *this << *CritRollNeeded;

    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellPeriodicAuraLog::Write()
{
    *this << TargetGUID;
    *this << CasterGUID;
    *this << SpellID;
    *this << static_cast<uint32>(Effects.size());

    for (SpellLogEffect const& effect : Effects)
    {
        *this << effect.Effect;
        *this << int32(effect.Amount);
        *this << int32(effect.OverHealOrKill);
        *this << int32(effect.AbsorbedOrAmplitude);
        *this << int32(effect.SchoolMaskOrPower);
        *this << int32(effect.Resisted);
        WriteBit(effect.Crit);
        WriteBit(effect.DebugInfo.is_initialized());
        WriteBit(effect.PlayerStats.is_initialized());

        if (effect.DebugInfo)
        {
            *this << float(effect.DebugInfo->CritRollMade);
            *this << float(effect.DebugInfo->CritRollNeeded);
        }

        if (effect.PlayerStats)
            *this << *effect.PlayerStats;

        FlushBits();
    }

    WriteLogDataBit();
    FlushBits();
    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellInterruptLog::Write()
{
    _worldPacket << Caster;
    _worldPacket << Victim;
    _worldPacket << int32(InterruptedSpellID);
    _worldPacket << int32(SpellID);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellEnergizeLog::Write()
{
    *this << CasterGUID;
    *this << TargetGUID;
    *this << int32(SpellID);
    *this << int32(Type);
    *this << int32(Amount);
    WriteBit(false); // Unk Legion - related to new log system?

    WriteLogDataBit();
    FlushBits();
    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellInstakillLog::Write()
{
    _worldPacket << Target;
    _worldPacket << Caster;
    _worldPacket << int32(SpellID);

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& buffer, WorldPackets::CombatLog::SpellLogMissDebug const& missDebug)
{
    buffer << float(missDebug.HitRoll);
    buffer << float(missDebug.HitRollNeeded);
    return buffer;
}

ByteBuffer& operator<<(ByteBuffer& buffer, WorldPackets::CombatLog::SpellLogMissEntry const& missEntry)
{
    buffer << missEntry.Victim;
    buffer << uint8(missEntry.MissReason);
    if (buffer.WriteBit(missEntry.Debug.is_initialized()))
        buffer << *missEntry.Debug;

    buffer.FlushBits();
    return buffer;
}

WorldPacket const* WorldPackets::CombatLog::SpellMissLog::Write()
{
    _worldPacket << int32(SpellID);
    _worldPacket << Caster;
    _worldPacket << static_cast<uint32>(Entries.size());
    for (SpellLogMissEntry const& missEntry : Entries)
        _worldPacket << missEntry;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::ProcResist::Write()
{
    _worldPacket << Caster;
    _worldPacket << Target;
    _worldPacket << int32(SpellID);
    _worldPacket.WriteBit(Rolled.is_initialized());
    _worldPacket.WriteBit(Needed.is_initialized());
    _worldPacket.FlushBits();

    if (Rolled)
        _worldPacket << *Rolled;

    if (Needed)
        _worldPacket << *Needed;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellOrDamageImmune::Write()
{
    _worldPacket << CasterGUID;
    _worldPacket << VictimGUID;
    _worldPacket << uint32(SpellID);
    _worldPacket.WriteBit(IsPeriodic);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::SpellDamageShield::Write()
{
    *this << Attacker;
    *this << Defender;
    *this << int32(SpellID);
    *this << int32(TotalDamage);
    *this << int32(OverKill);
    *this << int32(SchoolMask);
    *this << int32(LogAbsorbed);
    WriteBit(false); // Unk Legion - related to new log system?
    WriteLogDataBit();
    FlushBits();
    WriteLogData();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::AttackerStateUpdate::Write()
{
    ByteBuffer attackRoundInfo;
    attackRoundInfo << HitInfo;
    attackRoundInfo << AttackerGUID;
    attackRoundInfo << VictimGUID;
    attackRoundInfo << Damage;
    attackRoundInfo << OverDamage;

    if (attackRoundInfo.WriteBit(SubDmg.is_initialized()))
    {
        attackRoundInfo << SubDmg->SchoolMask;
        attackRoundInfo << SubDmg->FDamage;
        attackRoundInfo << SubDmg->Damage;
        if (HitInfo & (HITINFO_FULL_ABSORB | HITINFO_PARTIAL_ABSORB))
            attackRoundInfo << SubDmg->Absorbed;
        if (HitInfo & (HITINFO_FULL_RESIST | HITINFO_PARTIAL_RESIST))
            attackRoundInfo << SubDmg->Resisted;
    }

    attackRoundInfo << VictimState;
    attackRoundInfo << AttackerState;
    attackRoundInfo << MeleeSpellID;
    if (HitInfo & HITINFO_BLOCK)
        attackRoundInfo << BlockAmount;

    if (HitInfo & HITINFO_RAGE_GAIN)
        attackRoundInfo << RageGained;

    if (HitInfo & HITINFO_UNK1)
    {
        attackRoundInfo << UnkState.State1;
        attackRoundInfo << UnkState.State2;
        attackRoundInfo << UnkState.State3;
        attackRoundInfo << UnkState.State4;
        attackRoundInfo << UnkState.State5;
        attackRoundInfo << UnkState.State6;
        attackRoundInfo << UnkState.State7;
        attackRoundInfo << UnkState.State8;
        attackRoundInfo << UnkState.State9;
        attackRoundInfo << UnkState.State10;
        attackRoundInfo << UnkState.State11;
        attackRoundInfo << UnkState.State12;
    }
    if (HitInfo & (HITINFO_BLOCK | HITINFO_UNK12))
        attackRoundInfo << Unk;

    WriteLogDataBit();
    FlushBits();
    WriteLogData();

    *this << static_cast<uint32>(attackRoundInfo.size());
    _worldPacket.append(attackRoundInfo);
    _fullLogPacket.append(attackRoundInfo);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::CombatLog::AttackEventFailed::Write()
{
    _worldPacket << Attacker;
    _worldPacket << Victim;

    return &_worldPacket;
}

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

#ifndef CombatPackets_h__
#define CombatPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "Player.h"
#include "SpellPackets.h"

namespace WorldPackets
{
    namespace Combat
    {
        class AttackSwingError final : public ServerPacket
        {
        public:
            AttackSwingError(AttackSwingReason reason) : ServerPacket(SMSG_ATTACK_SWING_ERROR, 4), Reason(reason) { }

            WorldPacket const* Write() override;

            AttackSwingReason Reason = ATTACK_SWING_ERROR_CANT_ATTACK;
        };

        class AttackSwing final : public ClientPacket
        {
        public:
            AttackSwing(WorldPacket&& packet) : ClientPacket(CMSG_ATTACK_SWING, std::move(packet)) { }

            void Read() override;

            ObjectGuid Victim;
        };

        class AttackStop final : public ClientPacket
        {
        public:
            AttackStop(WorldPacket&& packet) : ClientPacket(CMSG_ATTACK_STOP, std::move(packet)) { }

            void Read() override { }
        };

        class AttackStart final : public ServerPacket
        {
        public:
            AttackStart() : ServerPacket(SMSG_ATTACK_START, 16 * 2) { }

            WorldPacket const* Write() override;

            ObjectGuid Attacker;
            ObjectGuid Victim;
        };

        class SAttackStop final : public ServerPacket
        {
        public:
            SAttackStop() : ServerPacket(SMSG_ATTACK_STOP, 16 * 2 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid Attacker;
            ObjectGuid Victim;
            bool Dead = false;
        };

        struct ThreatInfo
        {
            ObjectGuid UnitGUID;
            int32 Threat = 0;
        };

        class ThreatUpdate final : public ServerPacket
        {
        public:
            ThreatUpdate() : ServerPacket(SMSG_THREAT_UPDATE, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            std::vector<ThreatInfo> ThreatList;
        };

        class HighestThreatUpdate final : public ServerPacket
        {
        public:
            HighestThreatUpdate() : ServerPacket(SMSG_HIGHEST_THREAT_UPDATE, 16 * 2 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            std::vector<ThreatInfo> ThreatList;
            ObjectGuid HighestThreatGUID;
        };

        class ThreatRemove final : public ServerPacket
        {
        public:
            ThreatRemove() : ServerPacket(SMSG_THREAT_REMOVE, 32) { }

            WorldPacket const* Write() override;

            ObjectGuid AboutGUID;
            ObjectGuid UnitGUID;
        };

        class AIReaction final : public ServerPacket
        {
        public:
            AIReaction() : ServerPacket(SMSG_AI_REACTION, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
            uint32 Reaction = 0;
        };

        class CancelCombat final : public ServerPacket
        {
        public:
            CancelCombat() : ServerPacket(SMSG_CANCEL_COMBAT, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        struct PowerUpdatePower
        {
            PowerUpdatePower(int32 power, uint8 powerType) : Power(power), PowerType(powerType) { }

            int32 Power = 0;
            uint8 PowerType = 0;
        };

        class PowerUpdate final : public ServerPacket
        {
        public:
            PowerUpdate() : ServerPacket(SMSG_POWER_UPDATE, 16 + 4 + 1) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
            std::vector<PowerUpdatePower> Powers;
        };

        class SetSheathed final : public ClientPacket
        {
        public:
            SetSheathed(WorldPacket&& packet) : ClientPacket(CMSG_SET_SHEATHED, std::move(packet)) { }

            void Read() override;

            int32 CurrentSheathState = 0;
            bool Animate = true;
        };

        class CancelAutoRepeat final : public ServerPacket
        {
        public:
            CancelAutoRepeat() : ServerPacket(SMSG_CANCEL_AUTO_REPEAT, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class HealthUpdate final : public ServerPacket
        {
        public:
            HealthUpdate() : ServerPacket(SMSG_HEALTH_UPDATE, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
            int32 Health = 0;
        };

        class ThreatClear final : public ServerPacket
        {
        public:
            ThreatClear() : ServerPacket(SMSG_THREAT_CLEAR, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
        };

        class PvPCredit final : public ServerPacket
        {
        public:
            PvPCredit() : ServerPacket(SMSG_PVP_CREDIT, 4 + 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid Target;
            int32 Honor = 0;
            int32 Rank = 0;
        };

        class BreakTarget final : public ServerPacket
        {
        public:
            BreakTarget() : ServerPacket(SMSG_BREAK_TARGET, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid UnitGUID;
        };
    }
}

#endif // CombatPackets_h__

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

#include "CharacterPackets.h"
#include "ObjectMgr.h"
#include "World.h"

WorldPackets::Character::EnumCharactersResult::CharacterInfo::CharacterInfo(Field* fields)
{
    enum fieldenum
    {
        f_guid, f_name, f_race, f_class, f_playerBytes, f_playerBytes2, f_playerBytes3, f_level,
        f_zone, f_map, f_position_x, f_position_y, f_position_z, f_guildid, f_playerFlags,
        f_at_login, f_entry, f_modelid, f_pet_level, f_equipmentCache, f_guid_lock, f_slot
    };
    
    Guid = ObjectGuid::Create<HighGuid::Player>(fields[f_guid].GetUInt64());
    Name = fields[f_name].GetString();
    Race = fields[f_race].GetUInt8();
    Class = fields[f_class].GetUInt8();
    Skin = uint8(fields[f_playerBytes].GetUInt32() & 0xFF);
    Face = uint8((fields[f_playerBytes].GetUInt32() >> 8) & 0xFF);
    HairStyle = uint8((fields[f_playerBytes].GetUInt32() >> 16) & 0xFF);
    HairColor = uint8((fields[f_playerBytes].GetUInt32() >> 24) & 0xFF);
    FacialHair = uint8(fields[f_playerBytes2].GetUInt32() & 0xFF);
    Blindfold = uint8((fields[f_playerBytes2].GetUInt32() >> 24) & 0xFF);
    Sex = uint8(fields[f_playerBytes3].GetUInt32() & 0xFF);
    Tattoo = uint8((fields[f_playerBytes3].GetUInt32() >> 16) & 0xFF);;
    Horn = uint8((fields[f_playerBytes3].GetUInt32() >> 24) & 0xFF);
    Level = fields[f_level].GetUInt8();
    ZoneId = int32(fields[f_zone].GetUInt16());
    MapId = int32(fields[f_map].GetUInt16());
    PreLoadPosition.x = fields[f_position_x].GetFloat();
    PreLoadPosition.y = fields[f_position_y].GetFloat();
    PreLoadPosition.z = fields[f_position_z].GetFloat();

    if (uint32 guildId = fields[f_guildid].GetUInt32())
        GuildGuid = ObjectGuid::Create<HighGuid::Guild>(guildId);

    uint32 playerFlags = fields[f_playerFlags].GetUInt32();
    uint32 atLoginFlags = fields[f_at_login].GetUInt16();

    if (playerFlags & PLAYER_FLAGS_HIDE_HELM)
        Flags |= CHARACTER_FLAG_HIDE_HELM;

    if (playerFlags & PLAYER_FLAGS_HIDE_CLOAK)
        Flags |= CHARACTER_FLAG_HIDE_CLOAK;

    if (playerFlags & PLAYER_FLAGS_GHOST)
        Flags |= CHARACTER_FLAG_GHOST;

    if (atLoginFlags & AT_LOGIN_RENAME)
        Flags |= CHARACTER_FLAG_RENAME;

    if (fields[f_guid_lock].GetUInt32())
        Flags |= CHARACTER_FLAG_LOCKED_BY_BILLING;

    //if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    //{
    //    if (!fields[idx++].GetString().empty())
    //        Flags |= CHARACTER_FLAG_DECLINED;
    //}else
    Flags |= CHARACTER_FLAG_DECLINED;

    if (atLoginFlags & AT_LOGIN_CUSTOMIZE)
        CustomizationFlag = CHAR_CUSTOMIZE_FLAG_CUSTOMIZE;
    else if (atLoginFlags & AT_LOGIN_CHANGE_FACTION)
        CustomizationFlag = CHAR_CUSTOMIZE_FLAG_FACTION;
    else if (atLoginFlags & AT_LOGIN_CHANGE_RACE)
        CustomizationFlag = CHAR_CUSTOMIZE_FLAG_RACE;

    Flags3 = 0;
    FirstLogin = (atLoginFlags & AT_LOGIN_FIRST) != 0;

    // show pet at selection character in character list only for non-ghost character
    if (!(playerFlags & PLAYER_FLAGS_GHOST) && (Class == CLASS_WARLOCK || Class == CLASS_HUNTER || Class == CLASS_DEATH_KNIGHT))
    {
        if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(fields[f_entry].GetUInt32()))
        {
            Pet.CreatureDisplayId = fields[f_modelid].GetUInt32();
            Pet.Level = fields[f_pet_level].GetUInt16();
            Pet.CreatureFamily = creatureInfo->Family;
        }
    }

    BoostInProgress  = false;
    ProfessionIds[0] = 0;
    ProfessionIds[1] = 0;

    Tokenizer equipment(fields[f_equipmentCache].GetString(), ' ');

    ListPosition = fields[f_slot].GetUInt8();

    for (uint8 slot = 0; slot < INVENTORY_SLOT_BAG_END; ++slot)
    {
        uint32 visualBase = slot * 3;
        VisualItems[slot].InventoryType = Player::GetUInt32ValueFromArray(equipment, visualBase);
        VisualItems[slot].DisplayId = Player::GetUInt32ValueFromArray(equipment, visualBase + 1);
        VisualItems[slot].DisplayEnchantId = Player::GetUInt32ValueFromArray(equipment, visualBase + 2);
    }
}

WorldPacket const* WorldPackets::Character::EnumCharactersResult::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.WriteBit(IsDeletedCharacters);
    _worldPacket.WriteBit(UnkBit1);
    _worldPacket.WriteBit(UnkBit2);
    _worldPacket.WriteBit(UnkBit3);
    _worldPacket.WriteBit(UnkBit4);
    _worldPacket << static_cast<uint32>(Characters.size());
    _worldPacket << static_cast<uint32>(FactionChangeRestrictions.size());
    //if (_worldPacket.WriteBit(true/*UnkInt.is_initialized()*/))
    //    _worldPacket << uint32(2147479552)/**UnkInt*/;

    for (CharacterInfo const& charInfo : Characters)
    {
        _worldPacket << charInfo.Guid;
        _worldPacket << uint8(charInfo.ListPosition);
        _worldPacket << uint8(charInfo.Race);
        _worldPacket << uint8(charInfo.Class);
        _worldPacket << uint8(charInfo.Sex);
        _worldPacket << uint8(charInfo.Skin);
        _worldPacket << uint8(charInfo.Face);
        _worldPacket << uint8(charInfo.HairStyle);
        _worldPacket << uint8(charInfo.HairColor);
        _worldPacket << uint8(charInfo.FacialHair);
        _worldPacket << uint8(charInfo.Tattoo);
        _worldPacket << uint8(charInfo.Horn);
        _worldPacket << uint8(charInfo.Blindfold);
        _worldPacket << uint8(charInfo.Level);
        _worldPacket << int32(charInfo.ZoneId);
        _worldPacket << int32(charInfo.MapId);
        _worldPacket << float(charInfo.PreLoadPosition.x);
        _worldPacket << float(charInfo.PreLoadPosition.y);
        _worldPacket << float(charInfo.PreLoadPosition.z);
        _worldPacket << charInfo.GuildGuid;
        _worldPacket << uint32(charInfo.Flags);
        _worldPacket << uint32(charInfo.CustomizationFlag);
        _worldPacket << uint32(charInfo.Flags3);
        _worldPacket << uint32(charInfo.Pet.CreatureDisplayId);
        _worldPacket << uint32(charInfo.Pet.Level);
        _worldPacket << uint32(charInfo.Pet.CreatureFamily);
        _worldPacket << uint32(charInfo.ProfessionIds[0]);
        _worldPacket << uint32(charInfo.ProfessionIds[1]);

        for (uint8 slot = 0; slot < INVENTORY_SLOT_BAG_END; ++slot)
        {
            _worldPacket << uint32(charInfo.VisualItems[slot].DisplayId);
            _worldPacket << uint32(charInfo.VisualItems[slot].DisplayEnchantId);
            _worldPacket << uint8(charInfo.VisualItems[slot].InventoryType);
        }

        _worldPacket.WriteBits(charInfo.Name.length(), 6);
        _worldPacket.WriteBit(charInfo.FirstLogin);
        _worldPacket.WriteBit(charInfo.BoostInProgress);
        _worldPacket.WriteBits(charInfo.unkWod61x, 5);
        _worldPacket.WriteString(charInfo.Name);
    }

    for (RestrictedFactionChangeRuleInfo const& rule : FactionChangeRestrictions)
    {
        _worldPacket << int32(rule.Mask);
        _worldPacket << uint8(rule.Race);
    }

    return &_worldPacket;
}

void WorldPackets::Character::CreateChar::Read()
{
    CreateInfo.reset(new CharacterCreateInfo());
    uint32 nameLength = _worldPacket.ReadBits(6);
    bool const hasTemplateSet = _worldPacket.ReadBit();
    _worldPacket >> CreateInfo->Race;
    _worldPacket >> CreateInfo->Class;
    _worldPacket >> CreateInfo->Sex;
    _worldPacket >> CreateInfo->Skin;
    _worldPacket >> CreateInfo->Face;
    _worldPacket >> CreateInfo->HairStyle;
    _worldPacket >> CreateInfo->HairColor;
    _worldPacket >> CreateInfo->FacialHairStyle;
    _worldPacket >> CreateInfo->OutfitId;
    _worldPacket >> CreateInfo->Tattoo;
    _worldPacket >> CreateInfo->Horn;
    _worldPacket >> CreateInfo->Blindfold;
    CreateInfo->Name = _worldPacket.ReadString(nameLength);
    if (hasTemplateSet)
        CreateInfo->TemplateSet = _worldPacket.read<int32>();
}

WorldPacket const* WorldPackets::Character::CharacterCreateResponse::Write()
{
    _worldPacket << uint8(Code);

    return &_worldPacket;
}

void WorldPackets::Character::DeleteChar::Read()
{
    _worldPacket >> Guid;
}

WorldPacket const* WorldPackets::Character::CharacterDeleteResponse::Write()
{
    _worldPacket << uint8(Code);

    return &_worldPacket;
}

void WorldPackets::Character::CharacterRenameRequest::Read()
{
    RenameInfo.reset(new CharacterRenameInfo());
    _worldPacket >> RenameInfo->Guid;
    RenameInfo->NewName = _worldPacket.ReadString(_worldPacket.ReadBits(6));
}

WorldPacket const* WorldPackets::Character::CharacterRenameResult::Write()
{
    _worldPacket << uint8(Result);
    _worldPacket.WriteBit(Guid.is_initialized());
    _worldPacket.WriteBits(Name.length(), 6);

    if (Guid)
        _worldPacket << *Guid;

    _worldPacket.WriteString(Name);

    return &_worldPacket;
}

void WorldPackets::Character::CharRaceOrFactionChange::Read()
{
    RaceOrFactionChangeInfo.reset(new CharRaceOrFactionChangeInfo());

    RaceOrFactionChangeInfo->FactionChange = _worldPacket.ReadBit();

    uint32 nameLength = _worldPacket.ReadBits(6);

    bool const hasSkinID = _worldPacket.ReadBit();
    bool const hasHairColorID = _worldPacket.ReadBit();
    bool const hasHairStyleID = _worldPacket.ReadBit();
    bool const hasFacialHairStyleID = _worldPacket.ReadBit();
    bool const hasFaceID = _worldPacket.ReadBit();

    _worldPacket >> RaceOrFactionChangeInfo->Guid;
    _worldPacket >> RaceOrFactionChangeInfo->SexID;
    _worldPacket >> RaceOrFactionChangeInfo->RaceID;

    RaceOrFactionChangeInfo->Name = _worldPacket.ReadString(nameLength);

    if (hasSkinID)
        RaceOrFactionChangeInfo->SkinID = _worldPacket.read<uint8>();

    if (hasHairColorID)
        RaceOrFactionChangeInfo->HairColorID = _worldPacket.read<uint8>();

    if (hasHairStyleID)
        RaceOrFactionChangeInfo->HairStyleID = _worldPacket.read<uint8>();

    if (hasFacialHairStyleID)
        RaceOrFactionChangeInfo->FacialHairStyleID = _worldPacket.read<uint8>();

    if (hasFaceID)
        RaceOrFactionChangeInfo->FaceID = _worldPacket.read<uint8>();
}

WorldPacket const* WorldPackets::Character::CharFactionChangeResult::Write()
{
    _worldPacket << uint8(Result);
    _worldPacket << Guid;
    _worldPacket.WriteBit(Display.is_initialized());
    _worldPacket.FlushBits();

    if (Display)
    {
        _worldPacket.WriteBits(Display->Name.length(), 6);
        _worldPacket << uint8(Display->SexID);
        _worldPacket << uint8(Display->SkinID);
        _worldPacket << uint8(Display->HairColorID);
        _worldPacket << uint8(Display->HairStyleID);
        _worldPacket << uint8(Display->FacialHairStyleID);
        _worldPacket << uint8(Display->FaceID);
        _worldPacket << uint8(Display->RaceID);
        _worldPacket.WriteString(Display->Name);
    }

    return &_worldPacket;
}

void WorldPackets::Character::GenerateRandomCharacterName::Read()
{
    _worldPacket >> Race;
    _worldPacket >> Sex;
}

WorldPacket const* WorldPackets::Character::GenerateRandomCharacterNameResult::Write()
{
    _worldPacket.WriteBit(Success);
    _worldPacket.WriteBits(Name.length(), 6);
    _worldPacket.WriteString(Name);

    return &_worldPacket;
}

WorldPackets::Character::ReorderCharacters::ReorderCharacters(WorldPacket&& packet) : ClientPacket(CMSG_REORDER_CHARACTERS, std::move(packet)),
    Entries(sWorld->getIntConfig(CONFIG_CHARACTERS_PER_REALM)) { }

void WorldPackets::Character::ReorderCharacters::Read()
{
    Entries.resize(_worldPacket.ReadBits(9));
    for (ReorderInfo& reorderInfo : Entries)
    {
        _worldPacket >> reorderInfo.PlayerGUID;
        _worldPacket >> reorderInfo.NewPosition;
    }
}

void WorldPackets::Character::UndeleteCharacter::Read()
{
    UndeleteInfo.reset(new CharacterUndeleteInfo());
    _worldPacket >> UndeleteInfo->ClientToken;
    _worldPacket >> UndeleteInfo->CharacterGuid;
}

WorldPacket const* WorldPackets::Character::UndeleteCharacterResponse::Write()
{
    ASSERT(UndeleteInfo);

    _worldPacket << int32(UndeleteInfo->ClientToken);
    _worldPacket << uint32(Result);
    _worldPacket << UndeleteInfo->CharacterGuid;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::UndeleteCooldownStatusResponse::Write()
{
    _worldPacket.WriteBit(OnCooldown);
    _worldPacket.FlushBits();

    _worldPacket << uint32(MaxCooldown);
    _worldPacket << uint32(CurrentCooldown);

    return &_worldPacket;
}

void WorldPackets::Character::PlayerLogin::Read()
{
    _worldPacket >> Guid;
    _worldPacket >> FarClip;
}

WorldPacket const* WorldPackets::Character::LoginVerifyWorld::Write()
{
    _worldPacket << int32(MapID);
    _worldPacket << Pos.PositionXYZOStream();
    _worldPacket << uint32(Reason);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::CharacterLoginFailed::Write()
{
    _worldPacket << uint8(Code);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::LogoutResponse::Write()
{
    _worldPacket << int32(LogoutResult);
    _worldPacket.WriteBit(Instant);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::LogoutComplete::Write()
{
    _worldPacket << SwitchToCharacter;

    return &_worldPacket;
}

void WorldPackets::Character::LogoutInstant::Read()
{
    if (_worldPacket.ReadBit())
        _worldPacket >> *Reason;
}

void WorldPackets::Character::LoadingScreenNotify::Read()
{
    _worldPacket >> MapID;
    Showing = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Character::InitialSetup::Write()
{
    _worldPacket << uint8(ServerExpansionLevel);
    _worldPacket << uint8(ServerExpansionTier);
    _worldPacket << int32(ServerRegionID);
    _worldPacket << uint32(RaidOrigin);

    return &_worldPacket;
}

void WorldPackets::Character::SetActionBarToggles::Read()
{
    _worldPacket >> Mask;
}

void WorldPackets::Character::RequestPlayedTime::Read()
{
    TriggerScriptEvent = _worldPacket.ReadBit();
}

WorldPacket const* WorldPackets::Character::PlayedTime::Write()
{
    _worldPacket << int32(TotalTime);
    _worldPacket << int32(LevelTime);
    _worldPacket.WriteBit(TriggerEvent);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Character::ShowingCloak::Read()
{
    ShowCloak = _worldPacket.ReadBit();
}

void WorldPackets::Character::ShowingHelm::Read()
{
    ShowHelm = _worldPacket.ReadBit();
}

void WorldPackets::Character::SetTitle::Read()
{
    _worldPacket >> TitleID;
}

void WorldPackets::Character::AlterApperance::Read()
{
    _worldPacket >> NewHairStyle;
    _worldPacket >> NewHairColor;
    _worldPacket >> NewFacialHair;
    _worldPacket >> NewSkinColor;
    _worldPacket >> NewFace;
}

WorldPacket const* WorldPackets::Character::BarberShopResultServer::Write()
{
    _worldPacket << int32(Result);
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::LogXPGain::Write()
{
    _worldPacket << Victim;
    _worldPacket << int32(Original);
    _worldPacket << uint8(Reason);
    _worldPacket << int32(Amount);
    _worldPacket << float(GroupBonus);
    _worldPacket.WriteBit(ReferAFriend);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::TitleEarned::Write()
{
    _worldPacket << uint32(Index);

    return &_worldPacket;
}

void WorldPackets::Character::SetFactionAtWar::Read()
{
    _worldPacket >> FactionIndex;
}

void WorldPackets::Character::SetFactionNotAtWar::Read()
{
    _worldPacket >> FactionIndex;
}

void WorldPackets::Character::SetFactionInactive::Read()
{
    _worldPacket >> Index;
    State = _worldPacket.ReadBit();
}

void WorldPackets::Character::SetWatchedFaction::Read()
{
    _worldPacket >> FactionIndex;
}

WorldPacket const* WorldPackets::Character::SetFactionVisible::Write()
{
    _worldPacket << FactionIndex;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::XPGainAborted::Write()
{
    _worldPacket << Victim;
    _worldPacket << XpToAdd;
    _worldPacket << XpGainReason;
    _worldPacket << XpAbortReason;

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::NeutralPlayerFactionSelectResult::Write()
{
    _worldPacket << NewRaceID;
    _worldPacket.WriteBit(Success);
    _worldPacket.FlushBits();

    return &_worldPacket;
}

void WorldPackets::Character::SetPlayerDeclinedNames::Read()
{
    _worldPacket >> Player;

    uint8 stringLengths[MAX_DECLINED_NAME_CASES] = { };
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        stringLengths[i] = _worldPacket.ReadBits(7);

    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        DeclinedNames.name[i] = _worldPacket.ReadString(stringLengths[i]);
}

WorldPacket const * WorldPackets::Character::SetPlayerDeclinedNamesResult::Write()
{
    _worldPacket << ResultCode;
    _worldPacket << Player;

    return &_worldPacket;
}

void WorldPackets::Character::CharCustomize::Read()
{
    CustomizeInfo.reset(new CharCustomizeInfo());
    _worldPacket >> CustomizeInfo->CharGUID;
    _worldPacket >> CustomizeInfo->SexID;
    _worldPacket >> CustomizeInfo->SkinID;
    _worldPacket >> CustomizeInfo->HairColorID;
    _worldPacket >> CustomizeInfo->HairStyleID;
    _worldPacket >> CustomizeInfo->FacialHairStyleID;
    _worldPacket >> CustomizeInfo->FaceID;
    _worldPacket >> CustomizeInfo->Tattoo;
    _worldPacket >> CustomizeInfo->Horn;
    _worldPacket >> CustomizeInfo->Blindfold;
    _worldPacket.ReadString(_worldPacket.ReadBits(6), CustomizeInfo->CharName);
}

WorldPackets::Character::CharCustomizeResponse::CharCustomizeResponse(WorldPackets::Character::CharCustomizeInfo const* info)
    : ServerPacket(SMSG_CHAR_CUSTOMIZE, 16 + 9 + 2)
{
    CharGUID = info->CharGUID;
    SexID = info->SexID;
    SkinID = info->SkinID;
    HairColorID = info->HairColorID;
    HairStyleID = info->HairStyleID;
    FacialHairStyleID = info->FacialHairStyleID;
    FaceID = info->FaceID;
    CharName = info->CharName;
    Tattoo = info->Tattoo;
    Horn = info->Horn;
    Blindfold = info->Blindfold;
}

WorldPacket const* WorldPackets::Character::CharCustomizeResponse::Write()
{
    _worldPacket << CharGUID;
    _worldPacket << SexID;
    _worldPacket << SkinID;
    _worldPacket << HairColorID;
    _worldPacket << HairStyleID;
    _worldPacket << FacialHairStyleID;
    _worldPacket << FaceID;
    _worldPacket << Tattoo;
    _worldPacket << Horn;
    _worldPacket << Blindfold;
    _worldPacket.WriteBits(CharName.length(), 6);
    _worldPacket.FlushBits();
    _worldPacket.WriteString(CharName);

    return &_worldPacket;
}

WorldPacket const* WorldPackets::Character::CharCustomizeFailed::Write()
{
    _worldPacket << uint8(Result);
    _worldPacket << CharGUID;

    return &_worldPacket;
}

void WorldPackets::Character::NeutralPlayerSelectFaction::Read()
{
    _worldPacket >> Faction;
}

void WorldPackets::Character::SetCurrencyFlags::Read()
{
    _worldPacket >> CurrencyID;
    _worldPacket >> Flags;
}

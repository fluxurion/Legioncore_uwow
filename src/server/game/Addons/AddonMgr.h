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

#ifndef _ADDONMGR_H
#define _ADDONMGR_H
#include <openssl/md5.h>

struct AddonInfo
{
    AddonInfo(const std::string& name, uint8 _hasPubKey, uint32 crc, uint8 state, bool crcOrPubKey)
        : Name(name), hasPubKey(_hasPubKey), CRC(crc), State(state), UsePublicKeyOrCRC(crcOrPubKey) {}

    std::string Name;
    uint8 hasPubKey;
    uint32 CRC;
    uint8 State;
    bool UsePublicKeyOrCRC;
};

struct SavedAddon
{
    SavedAddon(const std::string& name, uint32 crc)
    {
        Name = name;
        CRC = crc;
    }

    std::string Name;
    uint32 CRC;
};

struct BannedAddon
{
    uint32 Id;
    uint8 MD5_name[MD5_DIGEST_LENGTH];
    uint8 MD5_version[MD5_DIGEST_LENGTH];
    uint32 timestamp;
};

typedef std::list<BannedAddon> BannedAddonDataMap;
extern BannedAddonDataMap                        sBannedAddonDataMap;

#define STANDARD_ADDON_CRC 0x4c1c776d

namespace AddonMgr
{
    void LoadFromDB();
    void SaveAddon(AddonInfo const& addon);
    SavedAddon const* GetAddonInfo(const std::string& name);
}

#endif


/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
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

#define _CRT_SECURE_NO_DEPRECATE

#include "dbcfile.h"

DBCFile::DBCFile(char const* file, bool isDBC)
{
    _file = file;
    _isDBC = isDBC;
    recordTable = nullptr;
    stringTable = nullptr;

    header.RecordSize = 0;
    header.RecordCount = 0;
    header.FieldCount = 0;
    header.BlockValue = 0;
    header.Signature = 0;
    header.Hash = 0;
    header.Build = 0;
    header.TimeStamp = 0;
    header.Min = 0;
    header.Max = 0;
    header.Locale = 0;
    header.ReferenceDataSize = 0;
    header.MetaFlags = 0;
}

bool DBCFile::open()
{
    if (recordTable)
    {
        delete[] recordTable;
        recordTable = nullptr;
    }

    FILE* f = fopen(_file, "rb");
    if (!f)
        return false;

    if (fread(&header.Signature, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&header.RecordCount, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&header.FieldCount, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&header.RecordSize, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&header.BlockValue, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (!_isDBC)
    {
        fread(&header.Hash, sizeof(uint32), 1, f);
        fread(&header.Build, sizeof(uint32), 1, f);
        fread(&header.TimeStamp, sizeof(uint32), 1, f);
        fread(&header.Min, sizeof(uint32), 1, f);
        fread(&header.Max, sizeof(uint32), 1, f);
        fread(&header.Locale, sizeof(uint32), 1, f);
        fread(&header.ReferenceDataSize, sizeof(uint32), 1, f);
        fread(&header.MetaFlags, sizeof(uint32), 1, f);
    }

    recordTable = new unsigned char[header.RecordSize * header.RecordCount + header.BlockValue];
    stringTable = recordTable + header.RecordSize * header.RecordCount;

    if (fread(recordTable, header.RecordSize * header.RecordCount + header.BlockValue, 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}

DBCFile::~DBCFile()
{
    delete[] recordTable;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
    assert(recordTable);
    return Record(*this, recordTable + id * header.RecordSize);
}

size_t DBCFile::getMaxId()
{
    assert(recordTable);

    size_t maxId = 0;
    for (size_t i = 0; i < getRecordCount(); ++i)
        if (maxId < getRecord(i).getUInt(0))
            maxId = getRecord(i).getUInt(0);

    return maxId;
}

DBCFile::Iterator DBCFile::begin()
{
    assert(recordTable);
    return Iterator(*this, recordTable);
}

DBCFile::Iterator DBCFile::end()
{
    assert(recordTable);
    return Iterator(*this, stringTable);
}

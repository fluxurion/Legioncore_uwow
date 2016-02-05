/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

DBCFile::DBCFile(HANDLE file) :
_file(file), _recordSize(0), _recordCount(0), _fieldCount(0),
_stringSize(0), _data(nullptr), _stringTable(nullptr)
{ }

bool DBCFile::open(FILE* f, char const* fmt)
{
    uint32 signature;
    if (fread(&signature, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&_recordCount, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&_fieldCount, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&_recordSize, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    if (fread(&_stringSize, sizeof(uint32), 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    fieldsOffset = new uint32[_fieldCount];
    fieldsOffset[0] = 0;
    for (size_t i = 1; i < _fieldCount; i++)
    {
        fieldsOffset[i] = fieldsOffset[i - 1];
        switch (fmt[i - 1])
        {
            case 'i':
            case 'n':
                fieldsOffset[i] += 4;
                break;
            case 't':
                fieldsOffset[i] += 2;
                break;
            case 'b':
                fieldsOffset[i] += 1;
                break;
        }
    }

    _data = new uint8[_recordSize * _recordCount + _stringSize];
    _stringTable = _data + _recordSize * _recordCount;

    return true;
}

DBCFile::~DBCFile()
{
    delete[] _data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
    assert(_data);
    return Record(*this, _data + id * _recordSize);
}

float DBCFile::Record::getFloat(size_t field) const
{
    assert(field < file._fieldCount);
    return *reinterpret_cast<float*>(offset + file.GetOffset(field));
}

uint32 DBCFile::Record::getUInt(size_t field) const
{
    assert(field < file._fieldCount);
    return *reinterpret_cast<uint32*>(offset + file.GetOffset(field));
}

uint8 DBCFile::Record::getUInt8(size_t field) const
{
    assert(field < file._fieldCount);
    return *reinterpret_cast<uint8*>(offset + file.GetOffset(field));
}

uint16 DBCFile::Record::getUInt16(size_t field) const
{
    assert(field < file._fieldCount);
    return *reinterpret_cast<uint16*>(offset + file.GetOffset(field));
}

uint64 DBCFile::Record::getUInt64(size_t field) const
{
    assert(field < file._fieldCount);
    return *reinterpret_cast<uint64*>(offset + file.GetOffset(field));
}

char const* DBCFile::Record::getString(size_t field) const
{
    assert(field < file._fieldCount);
    return reinterpret_cast<char*>(file._stringTable + getUInt(field));
}

size_t DBCFile::getMaxId()
{
    assert(_data);

    size_t maxId = 0;
    for (size_t i = 0; i < getRecordCount(); ++i)
        if (maxId < getRecord(i).getUInt(0))
            maxId = getRecord(i).getUInt(0);

    return maxId;
}

DBCFile::Iterator DBCFile::begin()
{
    assert(_data);
    return Iterator(*this, _data);
}

DBCFile::Iterator DBCFile::end()
{
    assert(_data);
    return Iterator(*this, _stringTable);
}

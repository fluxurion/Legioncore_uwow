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

#ifndef DB2_FILE_LOADER_H
#define DB2_FILE_LOADER_H

#include "Define.h"
#include "Utilities/ByteConverter.h"
#include "Implementation/HotfixDatabase.h"
#include <cassert>
#include <list>

static const int32 HeaderSize = 48;
static const uint32 DB3FmtSig = 0x33424457;
static const uint32 DB2FmtSig = 0x32424457;

class DB2FileLoader
{
    public:
    DB2FileLoader();
    ~DB2FileLoader();

    bool Load(const char *filename, const char *fmt);

    class Record
    {
    public:
        float getFloat(size_t field) const
        {
            assert(field < file.header.FieldCount);
            float val = *reinterpret_cast<float*>(offset + file.GetOffset(field));
            EndianConvert(val);
            return val;
        }
        uint32 getUInt(size_t field) const
        {
            assert(field < file.header.FieldCount);
            uint32 val = *reinterpret_cast<uint32*>(offset + file.GetOffset(field));
            EndianConvert(val);
            return val;
        }
        uint8 getUInt8(size_t field) const
        {
            assert(field < file.header.FieldCount);
            return *reinterpret_cast<uint8*>(offset + file.GetOffset(field));
        }
        uint16 getUInt16(size_t field) const
        {
            assert(field < file.header.FieldCount);
            return *reinterpret_cast<uint16*>(offset + file.GetOffset(field));
        }
        uint64 getUInt64(size_t field) const
        {
            assert(field < file.header.FieldCount);
            uint64 val = *reinterpret_cast<uint64*>(offset + file.GetOffset(field));
            EndianConvert(val);
            return val;
        }
        const char *getString(size_t field) const
        {
            assert(field < file.header.FieldCount);
            size_t stringOffset = getUInt(field);
            //assert(stringOffset < file.stringSize); //Crash on windows on debug mode
            return reinterpret_cast<char*>(file.stringTable + stringOffset);
        }

    private:
        Record(DB2FileLoader &file_, unsigned char *offset_): offset(offset_), file(file_) {}
        unsigned char *offset;
        DB2FileLoader &file;

        friend class DB2FileLoader;
    };

    // Get record by id
    Record getRecord(size_t id);
    /// Get begin iterator over records

    uint32 GetNumRows() const { return header.RecordCount;}
    uint32 GetCols() const { return header.FieldCount; }
    uint32 GetOffset(size_t id) const { return (fieldsOffset != nullptr && id < header.FieldCount) ? fieldsOffset[id] : 0; }
    uint32 GetHash() const { return header.Hash; }
    bool IsLoaded() const { return (recordTable != nullptr); }
    char* AutoProduceData(const char* fmt, uint32& count, char**& indexTable);
    char* AutoProduceStringsArrayHolders(const char* fmt, char* dataTable);
    char* AutoProduceStrings(const char* fmt, char* dataTable, uint32 locale);
    static uint32 GetFormatRecordSize(const char * format, int32 * index_pos = nullptr);
    static uint32 GetFormatStringFieldCount(const char * format);
    static uint32 GetFormatLocalizedStringFieldCount(const char * format);
private:
    char const* fileName;

    uint32* fieldsOffset;
    unsigned char* recordTable;
    unsigned char* stringTable;

    struct
    {
        uint32 Signature;
        uint32 RecordCount;
        uint32 FieldCount;
        uint32 RecordSize;
        uint32 StringBlockSize;

        uint32 Hash;
        uint32 Build;
        int32 Unknown;
        int32 Min;
        int32 Max;
        int32 Locale;
        int32 ReferenceDataSize;
        int32 MetaFlags;
    } header;
};

class DB2DatabaseLoader
{
public:
    explicit DB2DatabaseLoader(std::string const& storageName) : _storageName(storageName) { }

    char* Load(const char* format, HotfixDatabaseStatements preparedStatement, uint32& records, char**& indexTable, char*& stringHolders, std::list<char*>& stringPool);
    void LoadStrings(const char* format, HotfixDatabaseStatements preparedStatement, uint32 locale, char**& indexTable, std::list<char*>& stringPool);
    static char* AddString(char const** holder, std::string const& value);

private:
    std::string _storageName;
};

#endif

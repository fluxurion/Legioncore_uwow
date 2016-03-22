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

#include <cassert>
#include <list>

#include "Define.h"
#include "Utilities/ByteConverter.h"
#include "Implementation/HotfixDatabase.h"

static const int32 HeaderSize = 48;
static const uint32 DB3FmtSig = 0x33424457;
static const uint32 DB2FmtSig = 0x32424457;

enum DBFilesFlags
{
    None       = 0,
    DataOffset = 1,
    UnkFlag    = 2, // Some Index data stuff?!
    Index      = 4
};

class DB2FileLoader
{
    public:
    DB2FileLoader();
    ~DB2FileLoader();

    bool Load(char const* filename, char const* fmt);

    class Record
    {
    public:
        float getFloat(size_t field) const;
        uint32 getUInt(size_t field) const;
        uint8 getUInt8(size_t field) const;
        uint16 getUInt16(size_t field) const;
        uint64 getUInt64(size_t field) const;
        char const* getString(size_t field) const;

    private:
        Record(DB2FileLoader &file_, unsigned char *offset_): offset(offset_), file(file_) {}
        unsigned char *offset;
        DB2FileLoader &file;

        friend class DB2FileLoader;
    };

    Record getRecord(size_t id);

    uint32 GetNumRows() const { return header.RecordCount;}
    uint32 GetCols() const { return header.FieldCount; }
    uint32 GetOffset(size_t id) const { return (fieldsOffset != nullptr && id < header.FieldCount) ? fieldsOffset[id] : 0; }
    uint32 GetHash() const { return header.Hash; }
    bool IsLoaded() const { return (recordTable != nullptr); }
    bool HasIndex() const { return _hasIndex; }
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
        uint32 BlockValue;

        uint32 Hash;
        uint32 Build;
        uint32 Unknown;
        uint32 Min;
        uint32 Max;
        uint32 Locale;
        uint32 ReferenceDataSize;
        uint32 MetaFlags;
    } header;

    bool _hasIndex;
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

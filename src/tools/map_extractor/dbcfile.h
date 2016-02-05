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

#ifndef DBCFILE_H
#define DBCFILE_H

#include <cassert>
#include <string>

#include "CascLib.h"
#include "Define.h"

class DBCFile
{
    public:
        DBCFile(HANDLE file);
        ~DBCFile();

        // Open database. It must be openened before it can be used.
        bool open(FILE* f, char const* fmt);

        // Database exceptions
        class Exception
        {
            public:
                Exception(const std::string &message) : message(message) { }
                virtual ~Exception() { }
                const std::string &getMessage() { return message; }
            private:
                std::string message;
        };

        class NotFound: public Exception
        {
            public:
                NotFound(): Exception("Key was not found") { }
        };

        // Iteration over database
        class Iterator;
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
                Record(DBCFile& file, uint8* offset): file(file), offset(offset) {}
                DBCFile& file;
                uint8* offset;

                friend class DBCFile;
                friend class DBCFile::Iterator;

                Record& operator=(Record const& right);
        };
        /** Iterator that iterates over records
        */
        class Iterator
        {
            public:
                Iterator(DBCFile &file, uint8* offset) : record(file, offset) { }

                /// Advance (prefix only)
                Iterator& operator++()
                {
                    record.offset += record.file._recordSize;
                    return *this;
                }

                /// Return address of current instance
                Record const& operator*() const { return record; }
                Record const* operator->() const { return &record; }

                /// Comparison
                bool operator==(Iterator const& b) const
                {
                    return record.offset == b.record.offset;
                }

                bool operator!=(Iterator const& b) const
                {
                    return record.offset != b.record.offset;
                }

            private:
                Record record;

                Iterator& operator=(Iterator const& right);
        };

        // Get record by id
        Record getRecord(size_t id);
        /// Get begin iterator over records
        Iterator begin();
        /// Get begin iterator over records
        Iterator end();
        /// Trivial
        size_t getRecordCount() const { return _recordCount; }
        size_t getFieldCount() const { return _fieldCount; }
        size_t getMaxId();
        uint32 GetOffset(size_t id) const { return (fieldsOffset != nullptr && id < _fieldCount) ? fieldsOffset[id] : 0; }


    private:
        HANDLE _file;
        size_t _recordSize;
        size_t _recordCount;
        size_t _fieldCount;
        size_t _stringSize;
        uint8* _data;
        uint8* _stringTable;
        uint32* fieldsOffset;
};

#endif

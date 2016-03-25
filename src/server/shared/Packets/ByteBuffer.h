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

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "Define.h"
#include "Errors.h"
#include "ByteConverter.h"
#include "Util.h"

#include <exception>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <time.h>
#include "Util.h"

#include <math.h>
#include <type_traits>
#include <boost/asio/buffer.hpp>
#include <cmath>

class MessageBuffer;

#define BITS_1 uint8 _1
#define BITS_2 BITS_1, uint8 _2
#define BITS_3 BITS_2, uint8 _3
#define BITS_4 BITS_3, uint8 _4
#define BITS_5 BITS_4, uint8 _5
#define BITS_6 BITS_5, uint8 _6
#define BITS_7 BITS_6, uint8 _7
#define BITS_8 BITS_7, uint8 _8

#define BIT_VALS_1 _1
#define BIT_VALS_2 BIT_VALS_1, _2
#define BIT_VALS_3 BIT_VALS_2, _3
#define BIT_VALS_4 BIT_VALS_3, _4
#define BIT_VALS_5 BIT_VALS_4, _5
#define BIT_VALS_6 BIT_VALS_5, _6
#define BIT_VALS_7 BIT_VALS_6, _7
#define BIT_VALS_8 BIT_VALS_7, _8

#define DEFINE_READGUIDMASK(T1, T2) template <T1> \
    void ReadGuidMask(ObjectGuidSteam& guid) \
    { \
        uint8 maskArr[] = { T2 }; \
        for (uint8 i = 0; i < sizeof(maskArr) / sizeof(maskArr[0]) ; ++i) \
            guid[maskArr[i]] = ReadBit(); \
    }

#define DEFINE_WRITEGUIDMASK(T1, T2) template <T1> \
    void WriteGuidMask(ObjectGuidSteam guid) \
    { \
        uint8 maskArr[] = { T2 }; \
        for (uint8 i = 0; i < sizeof(maskArr) / sizeof(maskArr[0]); ++i) \
            WriteBit(guid[maskArr[i]]); \
    }

#define DEFINE_READGUIDBYTES(T1, T2) template <T1> \
    void ReadGuidBytes(ObjectGuidSteam& guid) \
    { \
        uint8 maskArr[] = { T2 }; \
        for (uint8 i = 0; i < sizeof(maskArr) / sizeof(maskArr[0]); ++i) \
            if (guid[maskArr[i]] != 0) \
                guid[maskArr[i]] ^= read<uint8>(); \
    }

#define DEFINE_WRITEGUIDBYTES(T1, T2) template <T1> \
    void WriteGuidBytes(ObjectGuidSteam guid) \
    { \
        uint8 maskArr[] = { T2 }; \
        for (uint8 i = 0; i < sizeof(maskArr) / sizeof(maskArr[0]); ++i) \
            if (guid[maskArr[i]] != 0) \
                (*this) << uint8(guid[maskArr[i]] ^ 1); \
    }

//! Structure to ease conversions from single 64 bit integer guid into individual bytes, for packet sending purposes
//! Nuke this out when porting ObjectGuidSteam from MaNGOS, but preserve the per-byte storage
struct ObjectGuidSteam
{
    public:
        ObjectGuidSteam() { _data.u64 = 0LL; }
        ObjectGuidSteam(uint64 guid) { _data.u64 = guid; }
        ObjectGuidSteam(ObjectGuidSteam const& other) { _data.u64 = other._data.u64; }

        uint8& operator[](uint32 index)
        {
            ASSERT(index < sizeof(uint64));

#if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
            return _data.byte[index];
#else
            return _data.byte[7 - index];
#endif
        }

        uint8 const& operator[](uint32 index) const
        {
            ASSERT(index < sizeof(uint64));

#if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
            return _data.byte[index];
#else
            return _data.byte[7 - index];
#endif
        }

        operator uint64()
        {
            return _data.u64;
        }

        ObjectGuidSteam& operator=(uint64 guid)
        {
            _data.u64 = guid;
            return *this;
        }

        ObjectGuidSteam& operator=(ObjectGuidSteam const& other)
        {
            _data.u64 = other._data.u64;
            return *this;
        }

        void Clear()
        {
            _data.u64 = 0LL;
        }

        bool IsEmpty() const
        {
            // AMARU TODO: dafuq?
            //return bool(_data.u64);
            return _data.u64 == 0;
        }

    private:
        union
        {
            uint64 u64;
            uint8 byte[8];
        } _data;
};

class ByteBufferException : public std::exception
{
public:
    ~ByteBufferException() throw() { }

    char const* what() const throw() override { return msg_.c_str(); }

protected:
    std::string & message() throw() { return msg_; }

private:
    std::string msg_;
};

// Root of ByteBuffer exception hierarchy
class ByteBufferPositionException : public ByteBufferException
{
public:
    ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize);

    ~ByteBufferPositionException() throw() { }
};

class ByteBufferSourceException : public ByteBufferException
{
public:
    ByteBufferSourceException(size_t pos, size_t size, size_t valueSize);

    ~ByteBufferSourceException() throw() { }
};

template<class T>
struct Unused
{
    Unused() {}
};

class ByteBuffer
{
    public:
        static size_t const DEFAULT_SIZE = 0x1000;
        static uint8 const InitialBitPos = 8;

        // constructor
        ByteBuffer() : _rpos(0), _wpos(0), _bitpos(InitialBitPos), _curbitval(0)
        {
            _storage.reserve(DEFAULT_SIZE);
        }

        ByteBuffer(size_t reserve) : _rpos(0), _wpos(0), _bitpos(InitialBitPos), _curbitval(0)
        {
            _storage.reserve(reserve);
        }

        // copy constructor
        ByteBuffer(ByteBuffer&& buf) : _rpos(buf._rpos), _wpos(buf._wpos),
            _bitpos(buf._bitpos), _curbitval(buf._curbitval), _storage(buf.Move()) { }

        ByteBuffer(ByteBuffer const& right) : _rpos(right._rpos), _wpos(right._wpos),
            _bitpos(right._bitpos), _curbitval(right._curbitval), _storage(right._storage) { }

        ByteBuffer(MessageBuffer&& buffer);

        std::vector<uint8>&& Move()
        {
            _rpos = 0;
            _wpos = 0;
            _bitpos = InitialBitPos;
            _curbitval = 0;
            return std::move(_storage);
        }

        ByteBuffer& operator=(ByteBuffer const& right)
        {
            if (this != &right)
            {
                _rpos = right._rpos;
                _wpos = right._wpos;
                _bitpos = right._bitpos;
                _curbitval = right._curbitval;
                _storage = right._storage;
            }

            return *this;
        }

        ByteBuffer& operator=(ByteBuffer&& right)
        {
            if (this != &right)
            {
                _rpos = right._rpos;
                _wpos = right._wpos;
                _bitpos = right._bitpos;
                _curbitval = right._curbitval;
                _storage = right.Move();
            }

            return *this;
        }

        virtual ~ByteBuffer() { }

        void clear()
        {
            _storage.clear();
            _rpos = _wpos = 0;
            _bitpos = 8;
            _curbitval = 0;
        }

        template <typename T> void append(T value)
        {
            static_assert(std::is_fundamental<T>::value, "append(compound)");
            EndianConvert(value);
            append((uint8 *)&value, sizeof(value));
        }

        void FlushBits()
        {
            if (_bitpos == 8)
                return;

            _bitpos = 8;    //should be before append for priventing deadloop
            append((uint8 *)&_curbitval, sizeof(uint8));
            _curbitval = 0;
        }

        void ResetBitPos()
        {
            if (_bitpos > 7)
                return;

            _bitpos = 8;
            _curbitval = 0;
        }

        void ResetBitReader()
        {
            if (_bitpos == 8)
                return;

            _curbitval = 0;
            _bitpos = 8;
        }

        void WriteBitInOrder(ObjectGuidSteam guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                WriteBit(guid[order[i]]);
        }

        bool WriteBit(uint32 bit)
        {
            --_bitpos;
            if (bit)
                _curbitval |= (1 << (_bitpos));

            if (_bitpos == 0)
            {
                _bitpos = 8;
                append((uint8 *)&_curbitval, sizeof(_curbitval));
                _curbitval = 0;
            }

            return (bit != 0);
        }

        void ReadBitInOrder(ObjectGuidSteam& guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                guid[order[i]] = ReadBit();
        }

        bool ReadBit()
        {
            ++_bitpos;
            if (_bitpos > 7)
            {
                _curbitval = read<uint8>();
                _bitpos = 0;
            }

            return ((_curbitval >> (7-_bitpos)) & 1) != 0;
        }

        void ReadBit(bool& val)
        {
            ++_bitpos;
            if (_bitpos > 7)
            {
                _curbitval = read<uint8>();
                _bitpos = 0;
            }

            val = ((_curbitval >> (7-_bitpos)) & 1) != 0;
        }

        template <typename T> void WriteBits(T value, size_t bits)
        {
            for (int32 i = bits-1; i >= 0; --i)
                WriteBit((value >> i) & 1);
        }

        uint32 ReadBits(size_t bits)
        {
            uint32 value = 0;
            for (int32 i = bits-1; i >= 0; --i)
                if (ReadBit())
                    value |= (1 << (i));

            return value;
        }

        void ReadBytesSeq(ObjectGuidSteam& guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                ReadByteSeq(guid[order[i]]);
        }

        // Reads a byte (if needed) in-place
        void ReadByteSeq(uint8& b)
        {
            if (b != 0)
                b ^= read<uint8>();
        }

        void WriteBytesSeq(ObjectGuidSteam guid, uint8 order[8])
        {
            for (uint8 i = 0; i < 8; ++i)
                WriteByteSeq(guid[order[i]]);
        }

        void WriteByteSeq(uint8 b)
        {
            if (b != 0)
                append<uint8>(b ^ 1);
        }

        template <typename T> void put(size_t pos, T value)
        {
            static_assert(std::is_fundamental<T>::value, "append(compound)");
            EndianConvert(value);
            put(pos, (uint8 *)&value, sizeof(value));
        }

        /**
          * @name   PutBits
          * @brief  Places specified amount of bits of value at specified position in packet.
          *         To ensure all bits are correctly written, only call this method after
          *         bit flush has been performed

          * @param  pos Position to place the value at, in bits. The entire value must fit in the packet
          *             It is advised to obtain the position using bitwpos() function.

          * @param  value Data to write.
          * @param  bitCount Number of bits to store the value on.
        */
        template <typename T> void PutBits(size_t pos, T value, uint32 bitCount)
        {
            if (!bitCount)
                throw ByteBufferSourceException((pos + bitCount) / 8, size(), 0);

            if (pos + bitCount > size() * 8)
                throw ByteBufferPositionException(false, (pos + bitCount) / 8, size(), (bitCount - 1) / 8 + 1);

            for (uint32 i = 0; i < bitCount; ++i)
            {
                size_t wp = (pos + i) / 8;
                size_t bit = (pos + i) % 8;
                if ((value >> (bitCount - i - 1)) & 1)
                    _storage[wp] |= 1 << (7 - bit);
                else
                    _storage[wp] &= ~(1 << (7 - bit));
            }
        }

        ByteBuffer &operator<<(uint8 value)
        {
            append<uint8>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint16 value)
        {
            append<uint16>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint32 value)
        {
            append<uint32>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint64 value)
        {
            append<uint64>(value);
            return *this;
        }

        ByteBuffer &operator<<(Milliseconds value)
        {
            append<uint32>(value.count());
            return *this;
        }

        ByteBuffer &operator<<(Seconds value)
        {
            append<uint32>(value.count());
            return *this;
        }

        // signed as in 2e complement
        ByteBuffer &operator<<(int8 value)
        {
            append<int8>(value);
            return *this;
        }

        ByteBuffer &operator<<(int16 value)
        {
            append<int16>(value);
            return *this;
        }

        ByteBuffer &operator<<(int32 value)
        {
            append<int32>(value);
            return *this;
        }

        ByteBuffer &operator<<(int64 value)
        {
            append<int64>(value);
            return *this;
        }

        // floating points
        ByteBuffer &operator<<(float value)
        {
            append<float>(value);
            return *this;
        }

        ByteBuffer &operator<<(double value)
        {
            append<double>(value);
            return *this;
        }

        ByteBuffer &operator<<(const std::string &value)
        {
            if (size_t len = value.length())
                append((uint8 const*)value.c_str(), len);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator<<(const char *str)
        {
            if (size_t len = (str ? strlen(str) : 0))
                append((uint8 const*)str, len);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator>>(bool &value)
        {
            value = read<char>() > 0 ? true : false;
            return *this;
        }

        ByteBuffer &operator>>(uint8 &value)
        {
            value = read<uint8>();
            return *this;
        }

        ByteBuffer &operator>>(uint16 &value)
        {
            value = read<uint16>();
            return *this;
        }

        ByteBuffer &operator>>(uint32 &value)
        {
            value = read<uint32>();
            return *this;
        }

        ByteBuffer &operator>>(uint64 &value)
        {
            value = read<uint64>();
            return *this;
        }

        //signed as in 2e complement
        ByteBuffer &operator>>(int8 &value)
        {
            value = read<int8>();
            return *this;
        }

        ByteBuffer &operator>>(int16 &value)
        {
            value = read<int16>();
            return *this;
        }

        ByteBuffer &operator>>(int32 &value)
        {
            value = read<int32>();
            return *this;
        }

        ByteBuffer &operator>>(int64 &value)
        {
            value = read<int64>();
            return *this;
        }

        ByteBuffer &operator>>(float &value)
        {
            value = read<float>();
            if (isNanOrInf<float>(value))
                throw ByteBufferPositionException(false, _rpos, sizeof(float), size());
            return *this;
        }

        ByteBuffer &operator>>(double &value)
        {
            value = read<double>();
            if (isNanOrInf<double>(value))
                throw ByteBufferPositionException(false, _rpos, sizeof(double), size());
            return *this;
        }

        ByteBuffer &operator>>(std::string& value)
        {
            value.clear();
            while (rpos() < size())                         // prevent crash at wrong string format in packet
            {
                char c = read<char>();
                if (c == 0)
                    break;
                value += c;
            }
            return *this;
        }

        template<class T>
        ByteBuffer &operator>>(Unused<T> const&)
        {
            read_skip<T>();
            return *this;
        }

        uint8& operator[](size_t const pos)
        {
            if (pos >= size())
                throw ByteBufferPositionException(false, pos, 1, size());
            return _storage[pos];
        }

        uint8 const& operator[](size_t const pos) const
        {
            if (pos >= size())
                throw ByteBufferPositionException(false, pos, 1, size());
            return _storage[pos];
        }

        size_t rpos() const { return _rpos; }

        size_t rpos(size_t rpos_)
        {
            _rpos = rpos_;
            return _rpos;
        }

        void rfinish()
        {
            _rpos = wpos();
        }

        size_t wpos() const { return _wpos; }

        size_t wpos(size_t wpos_)
        {
            _wpos = wpos_;
            return _wpos;
        }

        /// Returns position of last written bit
        size_t bitwpos() const { return _wpos * 8 + 8 - _bitpos; }

        size_t bitwpos(size_t newPos)
        {
            _wpos = newPos / 8;
            _bitpos = 8 - (newPos % 8);
            return _wpos * 8 + 8 - _bitpos;
        }

        template<typename T>
        void read_skip() { read_skip(sizeof(T)); }

        void read_skip(size_t skip)
        {
            if (_rpos + skip > size())
                throw ByteBufferPositionException(false, _rpos, skip, size());

            ResetBitPos();
            _rpos += skip;
        }

        template <typename T> T read()
        {
            ResetBitPos();
            T r = read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        }

        template <typename T> T read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
                throw ByteBufferPositionException(false, pos, sizeof(T), size());
            T val = *((T const*)&_storage[pos]);
            EndianConvert(val);
            return val;
        }

        void read(uint8 *dest, size_t len)
        {
            if (_rpos  + len > size())
               throw ByteBufferPositionException(false, _rpos, len, size());

            ResetBitPos();
            std::memcpy(dest, &_storage[_rpos], len);
            _rpos += len;
        }

        void ReadPackedUInt64(uint64& guid)
        {
            guid = 0;
            ReadPackedUInt64(read<uint8>(), guid);
        }

        void ReadPackedUInt64(uint8 mask, uint64& value)
        {
            for (uint32 i = 0; i < 8; ++i)
                if (mask & (uint8(1) << i))
                    value |= (uint64(read<uint8>()) << (i * 8));
        }

        std::string ReadString(uint32 length)
        {
            if (_rpos + length > size())
                throw ByteBufferPositionException(false, _rpos, length, size());

            if (!length)
                return std::string();

            ResetBitPos();
            std::string str((char const*)&_storage[_rpos], length);
            _rpos += length;
            return str;
        }

        void ReadString(uint32 length, std::string& string)
        {
            if (_rpos + length > size())
                throw ByteBufferPositionException(false, _rpos, length, size());

            if (!length)
            {
                string.clear();
                return;
            }

            ResetBitPos();
            std::string str((char const*)&_storage[_rpos], length);
            _rpos += length;
            string = str;
            return;
        }

        //! Method for writing strings that have their length sent separately in packet
        //! without null-terminating the string
        void WriteString(std::string const& str)
        {
            FlushBits();
            if (size_t len = str.length())
                append(str.c_str(), len);
        }

        uint32 ReadPackedTime()
        {
            uint32 packedDate = read<uint32>();
            tm lt = tm();

            lt.tm_min = packedDate & 0x3F;
            lt.tm_hour = (packedDate >> 6) & 0x1F;
            lt.tm_mday = ((packedDate >> 14) & 0x3F) + 1;
            lt.tm_mon = (packedDate >> 20) & 0xF;
            lt.tm_year = ((packedDate >> 24) & 0x1F) + 100;

            return uint32(mktime(&lt));
        }

        ByteBuffer& ReadPackedTime(uint32& time)
        {
            time = ReadPackedTime();
            return *this;
        }

        uint8* contents()
        {
            if (_storage.empty())
                throw ByteBufferException();
            return _storage.data();
        }

        uint8 const* contents() const
        {
            if (_storage.empty())
                throw ByteBufferException();
            return _storage.data();
        }

        size_t size() const { return _storage.size(); }
        bool empty() const { return _storage.empty(); }

        void resize(size_t newsize)
        {
            _storage.resize(newsize, 0);
            _rpos = 0;
            _wpos = size();
        }

        void reserve(size_t ressize)
        {
            if (ressize > size())
                _storage.reserve(ressize);
        }

        void append(const char *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt);
        }

        template<class T> void append(const T *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt * sizeof(T));
        }

        void append(const uint8 *src, size_t cnt)
        {
            if (!cnt)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            if (!src)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            ASSERT(size() < 10000000);

            FlushBits();

            if (_storage.size() < _wpos + cnt)
                _storage.resize(_wpos + cnt);
            std::memcpy(&_storage[_wpos], src, cnt);
            _wpos += cnt;
        }

        void append(const ByteBuffer& buffer)
        {
            if (buffer.wpos())
                append(buffer.contents(), buffer.wpos());
        }

        // can be used in SMSG_ON_MONSTER_MOVE opcode
        void appendPackXYZ(float x, float y, float z)
        {
            uint32 packed = 0;
            packed |= ((int)(x / 0.25f) & 0x7FF);
            packed |= ((int)(y / 0.25f) & 0x7FF) << 11;
            packed |= ((int)(z / 0.25f) & 0x3FF) << 22;
            *this << packed;
        }

        void AppendPackedUInt64(uint64 guid)
        {
            uint8 mask = 0;
            size_t pos = wpos();
            *this << uint8(mask);

            uint8 packed[8];
            if (size_t packedSize = PackUInt64(guid, &mask, packed))
                append(packed, packedSize);

            put<uint8>(pos, mask);
        }

        static size_t PackUInt64(uint64 value, uint8* mask, uint8* result)
        {
            size_t resultSize = 0;
            *mask = 0;
            memset(result, 0, 8);

            for (uint8 i = 0; value != 0; ++i)
            {
                if (value & 0xFF)
                {
                    *mask |= uint8(1 << i);
                    result[resultSize++] = uint8(value & 0xFF);
                }

                value >>= 8;
            }

            return resultSize;
        }

        void AppendPackedTime(time_t time)
        {
            tm lt;
            localtime_r(&time, &lt);
            append<uint32>((lt.tm_year - 100) << 24 | lt.tm_mon << 20 | (lt.tm_mday - 1) << 14 | lt.tm_wday << 11 | lt.tm_hour << 6 | lt.tm_min);
        }

        void appendPackGUID(uint64 guid)
        {
            uint8 packGUID[8+1];
            packGUID[0] = 0;
            size_t size = 1;
            for (uint8 i = 0;guid != 0;++i)
            {
                if (guid & 0xFF)
                {
                    packGUID[0] |= uint8(1 << i);
                    packGUID[size] =  uint8(guid & 0xFF);
                    ++size;
                }

                guid >>= 8;
            }
            append(packGUID, size);
        }

        void put(size_t pos, const uint8 *src, size_t cnt)
        {
            if (pos + cnt > size())
                throw ByteBufferPositionException(true, pos, cnt, size());

            if (!src)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            std::memcpy(&_storage[pos], src, cnt);
        }

        void print_storage() const;

        void textlike() const;

        void hexlike() const;

        size_t GetBitPos() const
        {
            return _bitpos;
        }

        std::vector<uint8>& _GetStorage() { return _storage; }

        DEFINE_READGUIDMASK(BITS_1, BIT_VALS_1)
        DEFINE_READGUIDMASK(BITS_2, BIT_VALS_2)
        DEFINE_READGUIDMASK(BITS_3, BIT_VALS_3)
        DEFINE_READGUIDMASK(BITS_4, BIT_VALS_4)
        DEFINE_READGUIDMASK(BITS_5, BIT_VALS_5)
        DEFINE_READGUIDMASK(BITS_6, BIT_VALS_6)
        DEFINE_READGUIDMASK(BITS_7, BIT_VALS_7)
        DEFINE_READGUIDMASK(BITS_8, BIT_VALS_8)

        DEFINE_WRITEGUIDMASK(BITS_1, BIT_VALS_1)
        DEFINE_WRITEGUIDMASK(BITS_2, BIT_VALS_2)
        DEFINE_WRITEGUIDMASK(BITS_3, BIT_VALS_3)
        DEFINE_WRITEGUIDMASK(BITS_4, BIT_VALS_4)
        DEFINE_WRITEGUIDMASK(BITS_5, BIT_VALS_5)
        DEFINE_WRITEGUIDMASK(BITS_6, BIT_VALS_6)
        DEFINE_WRITEGUIDMASK(BITS_7, BIT_VALS_7)
        DEFINE_WRITEGUIDMASK(BITS_8, BIT_VALS_8)

        DEFINE_READGUIDBYTES(BITS_1, BIT_VALS_1)
        DEFINE_READGUIDBYTES(BITS_2, BIT_VALS_2)
        DEFINE_READGUIDBYTES(BITS_3, BIT_VALS_3)
        DEFINE_READGUIDBYTES(BITS_4, BIT_VALS_4)
        DEFINE_READGUIDBYTES(BITS_5, BIT_VALS_5)
        DEFINE_READGUIDBYTES(BITS_6, BIT_VALS_6)
        DEFINE_READGUIDBYTES(BITS_7, BIT_VALS_7)
        DEFINE_READGUIDBYTES(BITS_8, BIT_VALS_8)

        DEFINE_WRITEGUIDBYTES(BITS_1, BIT_VALS_1)
        DEFINE_WRITEGUIDBYTES(BITS_2, BIT_VALS_2)
        DEFINE_WRITEGUIDBYTES(BITS_3, BIT_VALS_3)
        DEFINE_WRITEGUIDBYTES(BITS_4, BIT_VALS_4)
        DEFINE_WRITEGUIDBYTES(BITS_5, BIT_VALS_5)
        DEFINE_WRITEGUIDBYTES(BITS_6, BIT_VALS_6)
        DEFINE_WRITEGUIDBYTES(BITS_7, BIT_VALS_7)
        DEFINE_WRITEGUIDBYTES(BITS_8, BIT_VALS_8)

    protected:
        size_t _rpos, _wpos, _bitpos;
        uint8 _curbitval;
        std::vector<uint8> _storage;
};

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i)
    {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while (msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

template<> inline std::string ByteBuffer::read<std::string>()
{
    std::string tmp;
    *this >> tmp;
    return tmp;
}

template<>
inline void ByteBuffer::read_skip<char*>()
{
    std::string temp;
    *this >> temp;
}

template<>
inline void ByteBuffer::read_skip<char const*>()
{
    read_skip<char*>();
}

template<>
inline void ByteBuffer::read_skip<std::string>()
{
    read_skip<char*>();
}

namespace boost
{
    namespace asio
    {
        inline const_buffers_1 buffer(ByteBuffer const& packet)
        {
            return buffer(packet.contents(), packet.size());
        }
    }
}

#endif

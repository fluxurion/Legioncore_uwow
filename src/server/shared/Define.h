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

#ifndef TRINITY_DEFINE_H
#define TRINITY_DEFINE_H

#include "CompilerDefs.h"

#if COMPILER == COMPILER_GNU
#  if !defined(__STDC_FORMAT_MACROS)
#    define __STDC_FORMAT_MACROS
#  endif
#endif

#include <cstddef>
#include <cinttypes>

#define TRINITY_LITTLEENDIAN 0
#define TRINITY_BIGENDIAN    1

#if !defined(TRINITY_ENDIAN)
#  if defined (BOOST_BIG_ENDIAN)
#    define TRINITY_ENDIAN TRINITY_BIGENDIAN
#  else
#    define TRINITY_ENDIAN TRINITY_LITTLEENDIAN
#  endif
#endif

#if PLATFORM == PLATFORM_WINDOWS
#  define TRINITY_PATH_MAX MAX_PATH
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#  ifndef DECLSPEC_DEPRECATED
#    define DECLSPEC_DEPRECATED __declspec(deprecated)
#  endif //DECLSPEC_DEPRECATED
#else //PLATFORM != PLATFORM_WINDOWS
#  define TRINITY_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#  define DECLSPEC_DEPRECATED
#endif //PLATFORM

#if !defined(COREDEBUG)
#  define TRINITY_INLINE inline
#else //COREDEBUG
#  if !defined(TRINITY_DEBUG)
#    define TRINITY_DEBUG
#  endif //TRINITY_DEBUG
#  define TRINITY_INLINE
#endif //!COREDEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F, V) __attribute__ ((format (printf, F, V)))
#  define ATTR_DEPRECATED __attribute__((deprecated))
#  define TRINITY_CONSTEXPR constexpr
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED
#if _MSC_VER >= 1900
#  define TRINITY_CONSTEXPR constexpr
#else
#  define TRINITY_CONSTEXPR
#endif
#endif //COMPILER == COMPILER_GNU

#define UI64FMTD "%" PRIu64
#define UI64LIT(N) UINT64_C(N)

#define SI64FMTD "%" PRId64
#define SI64LIT(N) INT64_C(N)

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

enum DBCFormer
{
    FT_NA='x',                                              //not used or unknown, 4 byte size
    FT_NA_BYTE='X',                                         //not used or unknown, byte
    FT_STRING='s',                                          //char*
    FT_STRING_NOT_LOCALIZED = 'S',                          //char* but without locale in DB2
    FT_FLOAT='f',                                           //float
    FT_LONG = 'l',                                          //uint64
    FT_INT='i',                                             //uint32
    FT_SHORT='t',                                           //uint16
    FT_BYTE='b',                                            //uint8
    FT_SORT='d',                                            //sorted by this field, field is not included
    FT_IND='n',                                             //the same, but parsed to data
    FT_SQL_PRESENT='p',                                     //Used in sql format to mark column present in sql dbc
    FT_SQL_ABSENT='a'                                       //Used in sql format to mark column absent in sql dbc
};
#endif //TRINITY_DEFINE_H

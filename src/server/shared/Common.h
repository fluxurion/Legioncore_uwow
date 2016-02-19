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

#ifndef COMMON_H
#define COMMON_H

#include "Define.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#if PLATFORM == PLATFORM_WINDOWS
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#include <set>
#include <unordered_set>
#include <list>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <memory>
#include <vector>
#include <array>

#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/functional/hash.hpp>

#include "Debugging/Errors.h"

#include "Threading/LockedQueue.h"

#if PLATFORM == PLATFORM_WINDOWS
#  include <ws2tcpip.h>

#  if defined(__INTEL_COMPILER)
#    if !defined(BOOST_ASIO_HAS_MOVE)
#      define BOOST_ASIO_HAS_MOVE
#    endif // !defined(BOOST_ASIO_HAS_MOVE)
#  endif // if defined(__INTEL_COMPILER)

#else
#  include <sys/types.h>
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
#  include <netdb.h>
#endif

#if COMPILER == COMPILER_MICROSOFT

#include <float.h>

#define I32FMT "%08I32X"
#define I64FMT "%016I64X"
#define snprintf _snprintf
#define atoll _atoi64
#define vsnprintf _vsnprintf
#define finite(X) _finite(X)
#define llabs _abs64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I32FMT "%08X"
#define I64FMT "%016llX"

#endif

inline float finiteAlways(float f) { return finite(f) ? f : 0.0f; }

inline unsigned long atoul(char const* str) { return strtoul(str, nullptr, 10); }
inline unsigned long long atoull(char const* str) { return strtoull(str, nullptr, 10); }

#define STRINGIZE(a) #a

enum TimeConstants
{
    ONE_SECOND      = 1,
    THREE_SECONDS   = 3,
    FIVE_SECONDS    = 5,
    HALF_MINUTE     = 30,
    MINUTE          = 60,
    HOUR            = MINUTE*60,
    DAY             = HOUR*24,
    WEEK            = DAY*7,
    MONTH           = DAY*30,
    YEAR            = MONTH*12,
    IN_MILLISECONDS = 1000
};

enum AccountTypes
{
    SEC_PLAYER                = 0,
    SEC_MODERATOR             = 1,
    SEC_GAMEMASTER            = 2,
    SEC_CONFIRMED_GAMEMASTER  = 3,
    SEC_REALM_LEADER          = 4,
    SEC_GM_LEADER             = 5,
    SEC_ADMINISTRATOR         = 6,
    SEC_CONSOLE               = 7                                  // must be always last in list, accounts must have less security level always also
};

enum LocaleConstant
{
    LOCALE_enUS =  0,
    LOCALE_koKR =  1,
    LOCALE_frFR =  2,
    LOCALE_deDE =  3,
    LOCALE_zhCN =  4,
    LOCALE_zhTW =  5,
    LOCALE_esES =  6,
    LOCALE_esMX =  7,
    LOCALE_ruRU =  8,
    LOCALE_ptPT =  9,
    LOCALE_itIT = 10,
    LOCALE_none = 11,

    MAX_LOCALES
};

const uint8 TOTAL_LOCALES = 11;
const LocaleConstant DEFAULT_LOCALE = LOCALE_enUS;

#define MAX_ACCOUNT_TUTORIAL_VALUES 8

extern char const* localeNames[TOTAL_LOCALES];

LocaleConstant GetLocaleByName(const std::string& name);

typedef std::vector<std::string> StringVector;

struct LocalizedString
{
    char const* Str[TOTAL_LOCALES];
};

enum GM_COMMAND_TAB
{
    GM,
    PLAYER
};

struct GmCommand
{
    uint32 accountID[2];
    std::string accountName[2];
    uint32 characterID[2];
    std::string characterName[2];
    std::string command;
};

struct GmChat
{
    uint32 type;
    uint32 accountID[2];
    std::string accountName[2];
    uint32 characterID[2];
    std::string characterName[2];
    std::string message;
};

struct ArenaLog
{
    uint32 timestamp;
    std::string str;
};

extern LockedQueue<GmCommand*> GmLogQueue;
extern LockedQueue<GmChat*> GmChatLogQueue;
extern LockedQueue<ArenaLog*> ArenaLogQueue;

// we always use stdlibc++ std::max/std::min, undefine some not C++ standard defines (Win API and some other platforms)
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846f
#endif

#ifndef M_RAD
#define M_RAD           57.295779513082320876846364344191f
#endif

#define MAX_QUERY_LEN 32*1024

//! Optional helper class to wrap optional values within.
template <typename T>
using Optional = boost::optional<T>;

namespace Trinity
{
    //! std::make_unique implementation (TODO: remove this once C++14 is supported)
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&& ...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

//! Hash implementation for std::pair to allow using pairs in unordered_set or as key for unordered_map
//! Individual types used in pair must be hashable by boost::hash
namespace std
{
    template<class K, class V>
    struct hash<std::pair<K, V>>
    {
    public:
        size_t operator()(std::pair<K, V> const& key) const
        {
            return boost::hash_value(key);
        }
    };
}

#endif

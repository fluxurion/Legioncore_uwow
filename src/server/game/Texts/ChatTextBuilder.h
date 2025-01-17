/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __CHATTEXT_BUILDER_H
#define __CHATTEXT_BUILDER_H

#include "Chat.h"
#include "ObjectMgr.h"
#include "Packets/ChatPackets.h"

namespace Trinity
{
    class BroadcastTextBuilder
    {
        public:
            BroadcastTextBuilder(Unit const* obj, ChatMsg msgType, uint32 textId, WorldObject const* target = nullptr, uint32 achievementId = 0)
                : _source(obj), _msgType(msgType), _textId(textId), _target(target), _achievementId(achievementId) { }

            WorldPackets::Chat::Chat* operator()(LocaleConstant locale) const
            {
                BroadcastTextEntry const* bct = sBroadcastTextStore.LookupEntry(_textId);
                WorldPackets::Chat::Chat* chat = new WorldPackets::Chat::Chat();
                chat->Initalize(_msgType, bct ? Language(bct->Language) : LANG_UNIVERSAL, _source, _target, bct ? DB2Manager::GetBroadcastTextValue(bct, locale, _source->getGender()) : "", _achievementId, "", locale);
                return chat;
            }

        private:
            Unit const* _source;
            ChatMsg _msgType;
            uint32 _textId;
            WorldObject const* _target;
            uint32 _achievementId;
    };
}
// namespace Trinity

#endif // __CHATTEXT_BUILDER_H

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

#include "Common.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "Conversation.h"
#include "ObjectMgr.h"

Conversation::Conversation() : WorldObject(false), _caster(NULL), _duration(0), _spellId(0)
{
    m_objectType |= TYPEMASK_CONVERSATION;
    m_objectTypeId = TYPEID_CONVERSATION;

    m_updateFlag = UPDATEFLAG_STATIONARY_POSITION;

    m_valuesCount = CONVERSATION_END;
    _dynamicValuesCount = CONVERSATION_DYNAMIC_END;

    _fieldNotifyFlags = UF_FLAG_PUBLIC | UF_FLAG_DYNAMIC | UF_FLAG_UNK0X100;
    updateMask.SetCount(_dynamicValuesCount);
}

Conversation::~Conversation()
{
}

void Conversation::AddToWorld()
{
    ///- Register the Conversation for guid lookup and for caster
    if (!IsInWorld())
    {
        sObjectAccessor->AddObject(this);
        WorldObject::AddToWorld();
        BindToCaster();
    }
}

void Conversation::RemoveFromWorld()
{
    ///- Remove the Conversation from the accessor and from all lists of objects in world
    if (IsInWorld())
    {
        // dynobj could get removed in Aura::RemoveAura
        if (!IsInWorld())
            return;

        UnbindFromCaster();
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool Conversation::CreateConversation(ObjectGuid::LowType guidlow, uint32 triggerEntry, Unit* caster, SpellInfo const* info, Position const& pos)
{
    std::vector<ConversationData> const* conversationData = sObjectMgr->GetConversationData(triggerEntry);
    std::vector<ConversationCreature> const* conversationCreature = sObjectMgr->GetConversationCreature(triggerEntry);
    std::vector<ConversationActor> const* conversationActor = sObjectMgr->GetConversationActor(triggerEntry);

    bool isActor = conversationActor && !conversationActor->empty();
    bool isCreature = conversationCreature && !conversationCreature->empty();
    bool hasData = conversationData && !conversationData->empty();

    if(!hasData || (!isActor && !isCreature))
        return false;

    SetMap(caster->GetMap());
    Relocate(pos);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Conversation (spell %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)", info ? info->Id : 0, GetPositionX(), GetPositionY());
        return false;
    }

    uint32 duration = 30000;

    Object::_Create(ObjectGuid::Create<HighGuid::Conversation>(GetMapId(), triggerEntry, guidlow));
    SetPhaseMask(caster->GetPhaseMask(), false);
    SetPhaseId(caster->GetPhases(), false);

    SetEntry(triggerEntry);
    SetObjectScale(1.0f);
    SetUInt32Value(CONVERSATION_FIELD_LAST_LINE_DURATION, duration);
    casterGUID = caster->GetGUID();

    if (Player* player = caster->ToPlayer())
    {
        if (Group* group = player->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                if (Player* member = itr->getSource())
                    AddPlayerInPersonnalVisibilityList(member->GetGUID());
            }
        }
        else
            AddPlayerInPersonnalVisibilityList(caster->GetGUID());
    }

    for (uint16 index = 0; index < _dynamicValuesCount; ++index)
    {
        ByteBuffer buffer;
        if (_fieldNotifyFlags & ConversationDynamicFieldFlags[index])
        {
            updateMask.SetBit(index);

            UpdateMask arrayMask;
            if (index == CONVERSATION_DYNAMIC_FIELD_ACTORS)
            {
                uint32 count = 0;
                if (isActor)
                {
                    arrayMask.SetCount(conversationActor->size() * 6);
                    for (std::vector<ConversationActor>::const_iterator itr = conversationActor->begin(); itr != conversationActor->end(); ++itr)
                    {
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->actorId);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->creatureId);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->displayId);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk1);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk2);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk3);

                        if (itr->duration)
                            duration = itr->duration;
                    }
                }
                if(isCreature)
                {
                    arrayMask.SetCount(conversationCreature->size() * 6);
                    for (std::vector<ConversationCreature>::const_iterator itr = conversationCreature->begin(); itr != conversationCreature->end(); ++itr)
                    {
                        ObjectGuid guid = ObjectGuid::Create<HighGuid::Player>(0xFFFFFFFFFF);

                        if (itr->creatureId)
                        {
                            Creature* creature = caster->FindNearestCreature(itr->creatureId, caster->GetVisibilityRange());
                            if (!creature)
                                return false;

                            guid = creature->GetGUID();
                        }

                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_LOPART(guid.GetLowPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_HIPART(guid.GetLowPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_LOPART(guid.GetHighPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_HIPART(guid.GetHighPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk1);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk2);

                        if (itr->duration)
                            duration = itr->duration;
                    }
                }
            }
            if (index == CONVERSATION_DYNAMIC_FIELD_LINES)
            {
                uint32 count = 0;
                arrayMask.SetCount(conversationData->size() * 4);
                for (std::vector<ConversationData>::const_iterator itr = conversationData->begin(); itr != conversationData->end(); ++itr)
                {
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->id);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->textId);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->unk1);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->unk2);
                }
            }

            fieldBuffer << uint16(arrayMask.GetBlockCount());
            arrayMask.AppendToPacket(&fieldBuffer);
            fieldBuffer.append(buffer);
        }
    }

    SetUInt32Value(CONVERSATION_FIELD_LAST_LINE_DURATION, duration);
    SetDuration(duration);
    setActive(true);

    if (!GetMap()->AddToMap(this))
        return false;

    return true;
}

void Conversation::Update(uint32 p_time)
{
    bool expired = false;

    if (GetDuration())
    {
        if (GetDuration() > int32(p_time))
            _duration -= p_time;
        else
            expired = true;

        if (expired)
            Remove();
    }
}

void Conversation::Remove()
{
    if (IsInWorld())
    {
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

void Conversation::BindToCaster()
{
    if (!GetCasterGUID())
        return;
    ASSERT(!_caster);
    _caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());
    ASSERT(_caster);
    ASSERT(_caster->GetMap() == GetMap());
}

void Conversation::UnbindFromCaster()
{
    if (!GetCasterGUID())
        return;
    ASSERT(_caster);
    _caster = NULL;
}

void Conversation::BuildDynamicValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    UpdateMask updateMaskTemp;
    updateMaskTemp = updateMask;

    *data << uint8(updateMaskTemp.GetBlockCount());
    updateMaskTemp.AppendToPacket(data);
    data->append(fieldBuffer);
}

bool Conversation::LoadConversationFromDB(ObjectGuid::LowType guid, Map* map, bool addToMap)
{
    ConversationSpawnData const* data = sObjectMgr->GetConversationData(guid);
    if (!data)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (GUID: %u) not found in table `creature`, can't load. ", guid);
        return false;
    }

    if (map->GetInstanceId() == 0)
    {
        /*if (map->GetConversation(ObjectGuid::Create<HighGuid::Conversation>(data->mapid, data->id, guid)))
            return false;*/
    }
    else
        guid = sObjectMgr->GetGenerator<HighGuid::Conversation>()->Generate();

    std::vector<ConversationData> const* conversationData = sObjectMgr->GetConversationData(data->id);
    std::vector<ConversationCreature> const* conversationCreature = sObjectMgr->GetConversationCreature(data->id);
    std::vector<ConversationActor> const* conversationActor = sObjectMgr->GetConversationActor(data->id);

    bool isActor = conversationActor && !conversationActor->empty();
    bool isCreature = conversationCreature && !conversationCreature->empty();
    bool hasData = conversationData && !conversationData->empty();

    if (!hasData || (!isActor && !isCreature))
        return false;

    SetMap(map);
    Relocate(data->posX, data->posY, data->posZ, data->orientation);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Conversation (conversation %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)", data->id, GetPositionX(), GetPositionY());
        return false;
    }

    uint32 duration = 30000;

    Object::_Create(ObjectGuid::Create<HighGuid::Conversation>(GetMapId(), data->id, guid));
    SetPhaseMask(data->phaseMask, false);
    SetPhaseId(data->PhaseID, false);

    SetEntry(data->id);
    SetObjectScale(1.0f);
    SetUInt32Value(CONVERSATION_FIELD_LAST_LINE_DURATION, duration);


    for (uint16 index = 0; index < _dynamicValuesCount; ++index)
    {
        ByteBuffer buffer;
        if (_fieldNotifyFlags & ConversationDynamicFieldFlags[index])
        {
            updateMask.SetBit(index);

            UpdateMask arrayMask;
            if (index == CONVERSATION_DYNAMIC_FIELD_ACTORS)
            {
                uint32 count = 0;
                if (isActor)
                {
                    arrayMask.SetCount(conversationActor->size() * 6);
                    for (std::vector<ConversationActor>::const_iterator itr = conversationActor->begin(); itr != conversationActor->end(); ++itr)
                    {
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->actorId);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->creatureId);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->displayId);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk1);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk2);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk3);

                        if (itr->duration)
                            duration = itr->duration;
                    }
                }
                if (isCreature)
                {
                    arrayMask.SetCount(conversationCreature->size() * 6);
                    for (std::vector<ConversationCreature>::const_iterator itr = conversationCreature->begin(); itr != conversationCreature->end(); ++itr)
                    {
                        ObjectGuid guid = ObjectGuid::Create<HighGuid::Player>(0xFFFFFFFFFF);

                        if (itr->creatureId)
                        {
                            Creature* creature = FindNearestCreature(itr->creatureId, GetVisibilityRange());
                            if (!creature)
                            {
                                sLog->outError(LOG_FILTER_GENERAL, "Conversation (conversation %u) not created. Can't fine creature %u", data->id, itr->creatureId);
                                return false;
                            }

                            guid = creature->GetGUID();
                        }

                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_LOPART(guid.GetLowPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_HIPART(guid.GetLowPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_LOPART(guid.GetHighPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(PAIR64_HIPART(guid.GetHighPart()));
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk1);
                        arrayMask.SetBit(count++);
                        buffer << uint32(itr->unk2);

                        if (itr->duration)
                            duration = itr->duration;
                    }
                }
            }
            if (index == CONVERSATION_DYNAMIC_FIELD_LINES)
            {
                uint32 count = 0;
                arrayMask.SetCount(conversationData->size() * 4);
                for (std::vector<ConversationData>::const_iterator itr = conversationData->begin(); itr != conversationData->end(); ++itr)
                {
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->id);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->textId);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->unk1);
                    arrayMask.SetBit(count++);
                    buffer << uint32(itr->unk2);
                }
            }

            fieldBuffer << uint16(arrayMask.GetBlockCount());
            arrayMask.AppendToPacket(&fieldBuffer);
            fieldBuffer.append(buffer);
        }
    }

    SetUInt32Value(CONVERSATION_FIELD_LAST_LINE_DURATION, duration);
    //SetDuration(duration);
    setActive(true);

    if (addToMap && !GetMap()->AddToMap(this))
        return false;

    return true;
}

bool Conversation::CanNeverSee2(WorldObject const* seer) const
{
    if (seer->GetTypeId() != TYPEID_PLAYER)
        return true;

    auto data = playing.find(seer->GetGUID());
    if (data == playing.end())
    {
        const_cast<Conversation*>(this)->playing[seer->GetGUID()] = getMSTime();
        return true;
    }
    uint32 const dur = getMSTime() - data->second;
    if (dur > GetUInt32Value(CONVERSATION_FIELD_LAST_LINE_DURATION))
        return true;
    return false;
}
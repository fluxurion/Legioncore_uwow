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

#include "GameObjectAI.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "PoolMgr.h"
#include "SpellMgr.h"
#include "World.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "OutdoorPvPMgr.h"
#include "BattlegroundAV.h"
#include "ScriptMgr.h"
#include "CreatureAISelector.h"
#include "Group.h"
#include "MapManager.h"
#include "GameObjectModel.h"
#include "DynamicTree.h"
#include "SpellAuraEffects.h"
#include "GameObjectPackets.h"
#include "MiscPackets.h"

GameObject::GameObject() : WorldObject(false), m_model(NULL), m_goValue(new GameObjectValue), m_AI(NULL), 
    m_manual_anim(false), m_respawnTime(0), m_respawnDelayTime(300),
    m_lootState(GO_NOT_READY), m_spawnedByDefault(true), m_usetimes(0), m_spellId(0), m_cooldownTime(0), 
    m_goInfo(NULL), m_ritualOwner(NULL), m_goData(NULL), m_DBTableGuid(0), m_rotation(0), m_lootRecipient(),
    m_lootRecipientGroup(), m_groupLootTimer(0), lootingGroupLowGUID(), m_isDynActive(false)
{
    m_valuesCount = GAMEOBJECT_END;
    _dynamicValuesCount = GAMEOBJECT_DYNAMIC_END;
    m_objectType |= TYPEMASK_GAMEOBJECT;
    m_objectTypeId = TYPEID_GAMEOBJECT;

    m_updateFlag = (UPDATEFLAG_STATIONARY_POSITION | UPDATEFLAG_ROTATION);
}

GameObject::~GameObject()
{
    delete m_goValue;
    delete m_AI;
    delete m_model;
    //if (m_uint32Values)                                      // field array can be not exist if GameOBject not loaded
    //    CleanupsBeforeDelete();
}

bool GameObject::AIM_Initialize()
{
    if (m_AI)
        delete m_AI;

    m_AI = FactorySelector::SelectGameObjectAI(this);

    if (!m_AI)
        return false;

    m_AI->InitializeAI();
    return true;
}

std::string GameObject::GetAIName() const
{
    return sObjectMgr->GetGameObjectTemplate(GetEntry())->AIName;
}

void GameObject::CleanupsBeforeDelete(bool /*finalCleanup*/)
{
    if (IsInWorld())
        RemoveFromWorld();

    if (m_uint32Values)                                      // field array can be not exist if GameOBject not loaded
        RemoveFromOwner();
}

void GameObject::RemoveFromOwner()
{
    ObjectGuid ownerGUID = GetOwnerGUID();
    if (ownerGUID.IsEmpty())
        return;

    if (Unit* owner = ObjectAccessor::GetUnit(*this, ownerGUID))
    {
        owner->RemoveGameObject(this, false);
        ASSERT(!GetOwnerGUID());
        return;
    }

    const char * ownerType = "creature";
    if (ownerGUID.IsPlayer())
        ownerType = "player";
    else if (ownerGUID.IsPet())
        ownerType = "pet";

    sLog->outFatal(LOG_FILTER_GENERAL, "Delete GameObject (GUID: %u Entry: %u SpellId %u LinkedGO %u) that lost references to owner (GUID %u Type '%s') GO list. Crash possible later.",
        GetGUIDLow(), GetGOInfo()->entry, m_spellId, GetGOInfo()->GetLinkedGameObjectEntry(), ownerGUID.GetCounter(), ownerType);
    SetOwnerGUID(ObjectGuid::Empty);
}

void GameObject::AddToWorld()
{
    ///- Register the gameobject for guid lookup
    if (!IsInWorld())
    {
        if (m_zoneScript)
            m_zoneScript->OnGameObjectCreate(this);

        sObjectAccessor->AddObject(this);
        // The state can be changed after GameObject::Create but before GameObject::AddToWorld
        bool toggledState = GetGoType() == GAMEOBJECT_TYPE_CHEST ? getLootState() == GO_READY : GetGoState() == GO_STATE_READY;
        if (m_model)
            GetMap()->InsertGameObjectModel(*m_model);

        EnableCollision(toggledState);
        WorldObject::AddToWorld();
    }
}

void GameObject::RemoveFromWorld()
{
    ///- Remove the gameobject from the accessor
    if (IsInWorld())
    {
        volatile uint32 entry = GetEntry();
        if (m_zoneScript)
            m_zoneScript->OnGameObjectRemove(this);

        RemoveFromOwner();
        if (m_model)
            if (GetMap()->ContainsGameObjectModel(*m_model))
                GetMap()->RemoveGameObjectModel(*m_model);
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool GameObject::Create(ObjectGuid::LowType guidlow, uint32 name_id, Map* map, uint32 phaseMask, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 animprogress, GOState go_state, uint32 artKit, uint32 aid /*= 0*/)
{
    ASSERT(map);
    SetMap(map);

    Relocate(x, y, z, ang);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Gameobject (GUID: %u Entry: %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)", guidlow, name_id, x, y);
        return false;
    }

    SetPhaseMask(phaseMask, false);

    SetZoneScript();
    if (m_zoneScript)
    {
        name_id = m_zoneScript->GetGameObjectEntry(guidlow, name_id);
        if (!name_id)
            return false;
    }

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(name_id);
    if (!goinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u Entry: %u) not created: non-existing entry in `gameobject_template`. Map: %u (X: %f Y: %f Z: %f)", guidlow, name_id, map->GetId(), x, y, z);
        return false;
    }

    m_goInfo = goinfo;

    if (IsTransport())
        Object::_Create(ObjectGuid::Create<HighGuid::Transport>(guidlow));
    else
        Object::_Create(ObjectGuid::Create<HighGuid::GameObject>(map->GetId(), goinfo->entry, guidlow));


    if (goinfo->type >= MAX_GAMEOBJECT_TYPE)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u Entry: %u) not created: non-existing GO type '%u' in `gameobject_template`. It will crash client if created.", guidlow, name_id, goinfo->type);
        return false;
    }

    if (GameObjectData const* data = sObjectMgr->GetGOData(guidlow))
    {
        SetPhaseId(data->PhaseID, false);
    }

    SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+0, rotation0);
    SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+1, rotation1);

    UpdateRotationFields(rotation2, rotation3);              // GAMEOBJECT_FACING, GAMEOBJECT_ROTATION, GAMEOBJECT_FIELD_PARENT_ROTATION+2/3

    SetObjectScale(goinfo->size);

    SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, goinfo->faction);
    SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, goinfo->flags);

    SetEntry(goinfo->entry);

    // set name for logs usage, doesn't affect anything ingame
    SetName(goinfo->name);

    SetDisplayId(goinfo->displayId);

    // GAMEOBJECT_FIELD_BYTES_1, index at 0, 1, 2 and 3
    SetGoType(GameobjectTypes(goinfo->type));
    SetGoState(go_state);
    SetGoArtKit(artKit);
    SetAIAnimKitId(aid);

    if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].WorldEffectID)
        m_updateFlag |= UPDATEFLAG_HAS_WORLDEFFECTID;

    if (!m_goInfo->visualQuestID)
    {  
        if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID)
            SetUInt32Value(GAMEOBJECT_FIELD_STATE_WORLD_EFFECT_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID);

        if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID)
            SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID);
    }

    if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellVisualID)
        SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellVisualID);

    if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateAnimID)
        SetUInt32Value(GAMEOBJECT_FIELD_STATE_ANIM_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateAnimID);

    if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateAnimKitID)
        SetUInt32Value(GAMEOBJECT_FIELD_STATE_ANIM_KIT_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateAnimKitID);



    switch (goinfo->type)
    {
        case GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING:
            // TODO: Get the values somehow, no longer in gameobject_template
            m_goValue->Building.Health = 20000/*goinfo->destructibleBuilding.intactNumHits + goinfo->destructibleBuilding.damagedNumHits*/;
            m_goValue->Building.MaxHealth = m_goValue->Building.Health;
            SetGoAnimProgress(255);
            SetUInt32Value(GAMEOBJECT_FIELD_PARENT_ROTATION, m_goInfo->destructibleBuilding.DestructibleModelRec);
            break;
        case GAMEOBJECT_TYPE_TRANSPORT:
            SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, getMSTime());
            if (goinfo->transport.startOpen)
                SetGoState(GO_STATE_ACTIVE);
            SetGoAnimProgress(animprogress);
            break;
        case GAMEOBJECT_TYPE_FISHINGNODE:
            SetGoAnimProgress(0);
            break;
        case GAMEOBJECT_TYPE_TRAP:
            if (GetGOInfo()->trap.stealthed)
            {
                m_stealth.AddFlag(STEALTH_TRAP);
                m_stealth.AddValue(STEALTH_TRAP, 70);
            }

            if (GetGOInfo()->trap.stealthAffected)
            {
                m_invisibility.AddFlag(INVISIBILITY_TRAP);
                m_invisibility.AddValue(INVISIBILITY_TRAP, 300);
            }
            break;
        default:
            SetGoAnimProgress(animprogress);
            break;
    }
    LastUsedScriptID = GetGOInfo()->ScriptId;
    AIM_Initialize();

    m_actionVector = sObjectMgr->GetGameObjectActionData(name_id);
    if (m_actionVector && !m_actionVector->empty())
    {
        m_actionActive = true;
        for (std::vector<GameObjectActionData>::const_iterator itr = m_actionVector->begin(); itr != m_actionVector->end(); ++itr)
        {
            if (m_maxActionDistance < itr->Distance)
                m_maxActionDistance = itr->Distance;
        }
    }

    return true;
}

void GameObject::Update(uint32 diff)
{
    if (!AI())
    {
        if (!AIM_Initialize())
            sLog->outError(LOG_FILTER_GENERAL, "Could not initialize GameObjectAI");
    } else
        AI()->UpdateAI(diff);

    if (GetGUID().IsMOTransport())
    {
        //((Transport*)this)->Update(p_time);
        return;
    }

    if (m_actionTimeCheck <= diff)
    {
        m_actionTimeCheck = 2500;
        GameObjectAction();
    } else m_actionTimeCheck -= diff;

    switch (m_lootState)
    {
        case GO_NOT_READY:
        {
            switch (GetGoType())
            {
                case GAMEOBJECT_TYPE_TRAP:
                {
                    // Arming Time for GAMEOBJECT_TYPE_TRAP (6)
                    GameObjectTemplate const* goInfo = GetGOInfo();
                    // Bombs
                    if (goInfo->trap.charges == 2)
                        m_cooldownTime = time(NULL) + 10;   // Hardcoded tooltip value
                    else if (Unit* owner = GetOwner())
                    {
                        if (owner->isInCombat())
                            m_cooldownTime = time(NULL) + 1;
                    }
                    m_lootState = GO_READY;
                    break;
                }
                case GAMEOBJECT_TYPE_FISHINGNODE:
                {
                    // fishing code (bobber ready)
                    if (time(NULL) > m_respawnTime - FISHING_BOBBER_READY_TIME)
                    {
                        // splash bobber (bobber ready now)
                        Unit* caster = GetOwner();
                        if (caster && caster->GetTypeId() == TYPEID_PLAYER)
                        {
                            SetGoState(GO_STATE_ACTIVE);
                            SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NODESPAWN);

                            UpdateData udata(caster->GetMapId());
                            WorldPacket packet;
                            BuildValuesUpdateBlockForPlayer(&udata, caster->ToPlayer());
                            udata.BuildPacket(&packet);
                            caster->ToPlayer()->GetSession()->SendPacket(&packet);

                            SendCustomAnim(GetGoAnimProgress());
                        }

                        m_lootState = GO_READY;                 // can be successfully open with some chance
                    }
                    return;
                }
                default:
                    m_lootState = GO_READY;                         // for other GOis same switched without delay to GO_READY
                    break;
            }
            // NO BREAK for switch (m_lootState)
        }
        case GO_READY:
        {
            if (m_respawnTime > 0)                          // timer on
            {
                time_t now = time(NULL);
                if (m_respawnTime <= now)            // timer expired
                {
                    ObjectGuid dbtableHighGuid = ObjectGuid::Create<HighGuid::GameObject>(GetMapId(), GetEntry(), m_DBTableGuid);
                    time_t linkedRespawntime = GetMap()->GetLinkedRespawnTime(dbtableHighGuid);
                    if (linkedRespawntime)             // Can't respawn, the master is dead
                    {
                        ObjectGuid targetGuid = sObjectMgr->GetLinkedRespawnGuid(dbtableHighGuid);
                        if (targetGuid == dbtableHighGuid) // if linking self, never respawn (check delayed to next day)
                            SetRespawnTime(DAY);
                        else
                            m_respawnTime = (now > linkedRespawntime ? now : linkedRespawntime)+urand(5, MINUTE); // else copy time from master and add a little
                        SaveRespawnTime(); // also save to DB immediately
                        return;
                    }

                    m_respawnTime = 0;
                    m_SkillupList.clear();
                    m_usetimes = 0;

                    switch (GetGoType())
                    {
                        case GAMEOBJECT_TYPE_FISHINGNODE:   //  can't fish now
                        {
                            Unit* caster = GetOwner();
                            if (caster && caster->GetTypeId() == TYPEID_PLAYER)
                            {
                                caster->FinishSpell(CURRENT_CHANNELED_SPELL);
                                caster->ToPlayer()->GetSession()->SendPacket(WorldPackets::GameObject::FishNotHooked().Write());
                            }
                            // can be delete
                            m_lootState = GO_JUST_DEACTIVATED;
                            return;
                        }
                        case GAMEOBJECT_TYPE_DOOR:
                        case GAMEOBJECT_TYPE_BUTTON:
                            //we need to open doors if they are closed (add there another condition if this code breaks some usage, but it need to be here for battlegrounds)
                            if (GetGoState() != GO_STATE_READY)
                                ResetDoorOrButton();
                            //flags in AB are type_button and we need to add them here so no break!
                        default:
                            if (!m_spawnedByDefault)        // despawn timer
                            {
                                                            // can be despawned or destroyed
                                SetLootState(GO_JUST_DEACTIVATED);
                                return;
                            }
                                                            // respawn timer
                            uint32 poolid = GetDBTableGUIDLow() ? sPoolMgr->IsPartOfAPool<GameObject>(GetDBTableGUIDLow()) : 0;
                            if (poolid)
                                sPoolMgr->UpdatePool<GameObject>(poolid, GetDBTableGUIDLow());
                            else
                                GetMap()->AddToMap(this);
                            break;
                    }
                }
            }

            if (isSpawned())
            {
                // traps can have time and can not have
                GameObjectTemplate const* goInfo = GetGOInfo();
                if (goInfo->type == GAMEOBJECT_TYPE_TRAP)
                {
                    if (m_cooldownTime >= time(NULL))
                        return;

                    // Type 2 - Bomb (will go away after casting it's spell)
                    if (goInfo->trap.charges == 2)
                    {
                        if (goInfo->trap.spell)
                            CastSpell(NULL, goInfo->trap.spell);  // FIXME: null target won't work for target type 1
                        SetLootState(GO_JUST_DEACTIVATED);
                        break;
                    }
                    // Type 0 and 1 - trap (type 0 will not get removed after casting a spell)
                    Unit* owner = GetOwner();
                    Unit* ok = NULL;                            // pointer to appropriate target if found any

                    bool IsBattlegroundTrap = false;
                    //FIXME: this is activation radius (in different casting radius that must be selected from spell data)
                    //TODO: move activated state code (cast itself) to GO_ACTIVATED, in this place only check activating and set state
                    float radius = (float)(goInfo->trap.radius)/3*2; // TODO rename radius to diameter (goInfo->trap.radius) should be (goInfo->trap.diameter)
                    if (!radius)
                    {
                        if (goInfo->trap.cooldown != 3)            // cast in other case (at some triggering/linked go/etc explicit call)
                            return;
                        else
                        {
                            if (m_respawnTime > 0)
                                break;

                            radius = (float)goInfo->trap.cooldown;       // battlegrounds gameobjects has data2 == 0 && data5 == 3
                            IsBattlegroundTrap = true;

                            if (!radius)
                                return;
                        }
                    }

                    // Note: this hack with search required until GO casting not implemented
                    // search unfriendly creature
                    if (owner)                    // hunter trap
                    {
                        Trinity::AnyUnfriendlyNoTotemUnitInObjectRangeCheck checker(this, owner, radius);
                        Trinity::UnitSearcher<Trinity::AnyUnfriendlyNoTotemUnitInObjectRangeCheck> searcher(this, ok, checker);
                        VisitNearbyGridObject(radius, searcher);
                        if (!ok) VisitNearbyWorldObject(radius, searcher);
                    }
                    else                                        // environmental trap
                    {
                        // environmental damage spells already have around enemies targeting but this not help in case not existed GO casting support
                        // affect only players
                        Player* player = NULL;
                        Trinity::AnyPlayerInObjectRangeCheck checker(this, radius);
                        Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(this, player, checker);
                        VisitNearbyWorldObject(radius, searcher);
                        ok = player;
                    }

                    if (ok)
                    {
                        // some traps do not have spell but should be triggered
                        if (goInfo->trap.spell)
                            CastSpell(ok, goInfo->trap.spell);

                        m_cooldownTime = time(NULL) + (goInfo->trap.cooldown ? goInfo->trap.cooldown :  uint32(4));   // template or 4 seconds

                        if (goInfo->trap.charges == 1)
                            SetLootState(GO_JUST_DEACTIVATED);

                        if (IsBattlegroundTrap && ok->GetTypeId() == TYPEID_PLAYER)
                        {
                            //Battleground gameobjects case
                            if (ok->ToPlayer()->InBattleground())
                                if (Battleground* bg = ok->ToPlayer()->GetBattleground())
                                    bg->HandleTriggerBuff(GetGUID());
                        }
                    }
                }
                else if (uint32 max_charges = goInfo->GetCharges())
                {
                    if (m_usetimes >= max_charges)
                    {
                        m_usetimes = 0;
                        SetLootState(GO_JUST_DEACTIVATED);      // can be despawned or destroyed
                    }
                }
            }

            break;
        }
        case GO_ACTIVATED:
        {
            switch (GetGoType())
            {
                case GAMEOBJECT_TYPE_DOOR:
                case GAMEOBJECT_TYPE_BUTTON:
                    if (GetGOInfo()->GetAutoCloseTime() && (m_cooldownTime < time(NULL)))
                        ResetDoorOrButton();
                    break;
                case GAMEOBJECT_TYPE_GOOBER:
                    if (m_cooldownTime < time(NULL))
                    {
                        RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);

                        SetLootState(GO_JUST_DEACTIVATED);
                        m_cooldownTime = 0;
                    }
                    break;
                case GAMEOBJECT_TYPE_CHEST:
                    if (m_groupLootTimer)
                    {
                        if (m_groupLootTimer <= diff)
                        {
                            Group* group = sGroupMgr->GetGroupByGUID(lootingGroupLowGUID);
                            if (group)
                                group->EndRoll(&loot);
                            m_groupLootTimer = 0;
                            lootingGroupLowGUID.Clear();
                        }
                        else m_groupLootTimer -= diff;
                    }
                default:
                    break;
            }
            break;
        }
        case GO_JUST_DEACTIVATED:
        {
            //if Gameobject should cast spell, then this, but some GOs (type = 10) should be destroyed
            if (GetGoType() == GAMEOBJECT_TYPE_GOOBER)
            {
                uint32 spellId = GetGOInfo()->goober.spell;

                if (spellId)
                {
                    for (GuidSet::const_iterator it = m_unique_users.begin(); it != m_unique_users.end(); ++it)
                        // m_unique_users can contain only player GUIDs
                        if (Player* owner = ObjectAccessor::GetPlayer(*this, *it))
                            owner->CastSpell(owner, spellId, false);

                    m_unique_users.clear();
                    m_usetimes = 0;
                }

                //any return here in case battleground traps
                if (GetGOInfo()->flags & GO_FLAG_NODESPAWN)
                {
                    // Test: This falg on Mop perfome despoan go with some animation
                    // Should despawn some go with animations.
                    if (GetGOInfo()->IsDespawnAtAction() && m_respawnDelayTime > 0)
                    {
                        SendObjectDeSpawnAnim(GetGUID());
                        m_respawnTime = time(NULL) + m_respawnDelayTime;
                        UpdateObjectVisibility();
                        loot.clear();
                    }
                    //CD for using. not need as we set this state after CD launched. So just set ready state.
                    SetGoState(GO_STATE_READY);
                    SetLootState(GO_READY);
                    return;
                }

                SetGoState(GO_STATE_READY);
            }

            loot.clear();

            //! If this is summoned by a spell with ie. SPELL_EFFECT_SUMMON_OBJECT_WILD, with or without owner, we check respawn criteria based on spell
            //! The GetOwnerGUID() check is mostly for compatibility with hacky scripts - 99% of the time summoning should be done trough spells.
            //! If object is tmp spawn no need collect him all time in depawn mode.
            if (GetSpellId() || GetOwnerGUID() || !m_spawnedByDefault)
            {
                SetRespawnTime(0);
                Delete();
                return;
            }

            SetLootState(GO_READY);

            //burning flags in some battlegrounds, if you find better condition, just add it
            if (GetGOInfo()->IsDespawnAtAction() || GetGoAnimProgress() > 0)
            {
                SendObjectDeSpawnAnim(GetGUID());
                //reset flags
                SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GetGOInfo()->flags);
            }

            //! depr. see above
            if (!m_spawnedByDefault)
            {
                m_respawnTime = 0;
                UpdateObjectVisibility();
                return;
            }

            m_respawnTime = time(NULL) + m_respawnDelayTime;

            // if option not set then object will be saved at grid unload
            if (sWorld->getBoolConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY))
                SaveRespawnTime();

            UpdateObjectVisibility();

            break;
        }
    }
    sScriptMgr->OnGameObjectUpdate(this, diff);
}

void GameObject::Refresh()
{
    // not refresh despawned not casted GO (despawned casted GO destroyed in all cases anyway)
    if (m_respawnTime > 0 && m_spawnedByDefault)
        return;

    if (isSpawned())
        GetMap()->AddToMap(this);
}

void GameObject::AddUniqueUse(Player* player)
{
    AddUse();
    m_unique_users.insert(player->GetGUID());
}

void GameObject::Delete()
{
    if(!this)
        return;

    SetLootState(GO_NOT_READY);
    RemoveFromOwner();

    SendObjectDeSpawnAnim(GetGUID());

    SetGoState(GO_STATE_READY);
    SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GetGOInfo()->flags);

    uint32 poolid = GetDBTableGUIDLow() ? sPoolMgr->IsPartOfAPool<GameObject>(GetDBTableGUIDLow()) : 0;
    if (poolid)
        sPoolMgr->UpdatePool<GameObject>(poolid, GetDBTableGUIDLow());
    else
        AddObjectToRemoveList();
}

void GameObject::getFishLoot(Loot* fishloot, Player* loot_owner)
{
    fishloot->clear();

    uint32 zone, subzone;
    GetZoneAndAreaId(zone, subzone);
    fishloot->objGuid = GetGUID();

    // if subzone loot exist use it
    if (!fishloot->FillLoot(subzone, LootTemplates_Fishing, loot_owner, true, true, this))
        // else use zone loot (must exist in like case)
        fishloot->FillLoot(zone, LootTemplates_Fishing, loot_owner, true, false, this);
}

void GameObject::SaveToDB()
{
    // this should only be used when the gameobject has already been loaded
    // preferably after adding to map, because mapid may not be valid otherwise
    GameObjectData const* data = sObjectMgr->GetGOData(m_DBTableGuid);
    if (!data)
    {
        sLog->outError(LOG_FILTER_GENERAL, "GameObject::SaveToDB failed, cannot get gameobject data!");
        return;
    }

    SaveToDB(GetMapId(), data->spawnMask, data->phaseMask);
}

void GameObject::SaveToDB(uint32 mapid, uint32 spawnMask, uint32 phaseMask)
{
    const GameObjectTemplate* goI = GetGOInfo();

    if (!goI)
        return;

    if (!m_DBTableGuid)
        m_DBTableGuid = GetGUID().GetGUIDLow();
    // update in loaded data (changing data only in this place)
    GameObjectData& data = sObjectMgr->NewGOData(m_DBTableGuid);

    uint32 zoneId = 0;
    uint32 areaId = 0;
    sMapMgr->GetZoneAndAreaId(zoneId, areaId, mapid, GetPositionX(), GetPositionY(), GetPositionZ());

    // data->guid = guid must not be updated at save
    data.id = GetEntry();
    data.mapid = mapid;
    data.zoneId = zoneId;
    data.areaId = areaId;
    data.phaseMask = phaseMask;
    data.posX = GetPositionX();
    data.posY = GetPositionY();
    data.posZ = GetPositionZ();
    data.orientation = GetOrientation();
    data.rotation0 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+0);
    data.rotation1 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+1);
    data.rotation2 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+2);
    data.rotation3 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+3);
    data.spawntimesecs = m_spawnedByDefault ? m_respawnDelayTime : -(int32)m_respawnDelayTime;
    data.animprogress = GetGoAnimProgress();
    data.go_state = GetGoState();
    data.spawnMask = spawnMask;
    data.artKit = GetGoArtKit();

    // Update in DB
    SQLTransaction trans = WorldDatabase.BeginTransaction();

    uint8 index = 0;

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAMEOBJECT);
    stmt->setUInt64(0, m_DBTableGuid);
    trans->Append(stmt);

    stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GAMEOBJECT);
    stmt->setUInt64(index++, m_DBTableGuid);
    stmt->setUInt32(index++, GetEntry());
    stmt->setUInt16(index++, uint16(mapid));
    stmt->setUInt16(index++, zoneId);
    stmt->setUInt16(index++, areaId);
    stmt->setUInt32(index++, spawnMask);
    stmt->setUInt16(index++, uint16(GetPhaseMask()));
    stmt->setFloat(index++, GetPositionX());
    stmt->setFloat(index++, GetPositionY());
    stmt->setFloat(index++, GetPositionZ());
    stmt->setFloat(index++, GetOrientation());
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION));
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+1));
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+2));
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+3));
    stmt->setInt32(index++, int32(m_respawnDelayTime));
    stmt->setUInt8(index++, GetGoAnimProgress());
    stmt->setUInt8(index++, uint8(GetGoState()));
    stmt->setUInt8(index++, uint8(isActiveObject()));
    trans->Append(stmt);

    WorldDatabase.CommitTransaction(trans);
}

bool GameObject::LoadGameObjectFromDB(ObjectGuid::LowType guid, Map* map, bool addToMap)
{
    GameObjectData const* data = sObjectMgr->GetGOData(guid);

    if (!data)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u) not found in table `gameobject`, can't load. ", guid);
        return false;
    }

    uint32 entry = data->id;
    //uint32 map_id = data->mapid;                          // already used before call

    m_DBTableGuid = guid;
    if (map->GetInstanceId() != 0) guid = sObjectMgr->GetGenerator<HighGuid::GameObject>()->Generate();

    if (!Create(guid, entry, map, data->phaseMask,
        data->posX, data->posY, data->posZ, data->orientation,
        data->rotation0, data->rotation1, data->rotation2, data->rotation3,
        data->animprogress, (GOState)data->go_state, data->artKit, data->AiID))
        return false;

    if (data->spawntimesecs >= 0)
    {
        m_spawnedByDefault = true;

        if (!GetGOInfo()->GetDespawnPossibility() && !GetGOInfo()->IsDespawnAtAction())
        {
            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NODESPAWN);
            m_respawnDelayTime = 0;
            m_respawnTime = 0;
        }
        else
        {
            m_respawnDelayTime = data->spawntimesecs;
            m_respawnTime = GetMap()->GetGORespawnTime(m_DBTableGuid);

            // ready to respawn
            if (m_respawnTime && m_respawnTime <= time(NULL))
            {
                m_respawnTime = 0;
                GetMap()->RemoveGORespawnTime(m_DBTableGuid);
            }
        }
    }
    else
    {
        m_spawnedByDefault = false;
        m_respawnDelayTime = -data->spawntimesecs;
        m_respawnTime = 0;
    }

    m_goData = data;

    setDynActive(data->isActive);

    loot.SetGUID(ObjectGuid::Create<HighGuid::LootObject>(data->mapid, data->id, sObjectMgr->GetGenerator<HighGuid::LootObject>()->Generate()));

    if (addToMap && !GetMap()->AddToMap(this))
        return false;

    return true;
}

void GameObject::DeleteFromDB()
{
    GetMap()->RemoveGORespawnTime(m_DBTableGuid);
    sObjectMgr->DeleteGOData(m_DBTableGuid);

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAMEOBJECT);

    stmt->setUInt64(0, m_DBTableGuid);

    WorldDatabase.Execute(stmt);

    stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_EVENT_GAMEOBJECT);

    stmt->setUInt64(0, m_DBTableGuid);

    WorldDatabase.Execute(stmt);
}

GameObject* GameObject::GetGameObject(WorldObject& object, ObjectGuid guid)
{
    return object.GetMap()->GetGameObject(guid);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/
bool GameObject::hasQuest(uint32 quest_id) const
{
    QuestRelationBounds qr = sObjectMgr->GetGOQuestRelationBounds(GetEntry());
    for (QuestRelations::const_iterator itr = qr.first; itr != qr.second; ++itr)
    {
        if (itr->second == quest_id)
            return true;
    }
    return false;
}

bool GameObject::hasInvolvedQuest(uint32 quest_id) const
{
    QuestRelationBounds qir = sObjectMgr->GetGOQuestInvolvedRelationBounds(GetEntry());
    for (QuestRelations::const_iterator itr = qir.first; itr != qir.second; ++itr)
    {
        if (itr->second == quest_id)
            return true;
    }
    return false;
}

bool GameObject::IsTransport() const
{
    // If something is marked as a transport, don't transmit an out of range packet for it.
    GameObjectTemplate const* gInfo = GetGOInfo();
    if (!gInfo)
        return false;

    return gInfo->type == GAMEOBJECT_TYPE_TRANSPORT || gInfo->type == GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT;
}

// is Dynamic transport = non-stop Transport
bool GameObject::IsDynTransport() const
{
    // If something is marked as a transport, don't transmit an out of range packet for it.
    GameObjectTemplate const* gInfo = GetGOInfo();
    if (!gInfo)
        return false;

    return gInfo->type == GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT || (gInfo->type == GAMEOBJECT_TYPE_TRANSPORT && !gInfo->moTransport.taxiPathID);
}

bool GameObject::IsDestructibleBuilding() const
{
    GameObjectTemplate const* gInfo = GetGOInfo();
    if (!gInfo) return false;
    return gInfo->type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING;
}

Unit* GameObject::GetOwner() const
{
    return ObjectAccessor::GetUnit(*this, GetOwnerGUID());
}

void GameObject::SaveRespawnTime()
{
    if (m_goData && m_goData->dbData && m_respawnTime > time(NULL) && m_spawnedByDefault)
        GetMap()->SaveGORespawnTime(m_DBTableGuid, m_respawnTime);
}

bool GameObject::IsAlwaysVisibleFor(WorldObject const* seer) const
{
    if (WorldObject::IsAlwaysVisibleFor(seer))
        return true;

    if (IsTransport() || IsDestructibleBuilding())
        return true;

    if (!seer)
        return false;

    if (GetGoType() == GAMEOBJECT_TYPE_TRAP)
    {
        GameObjectTemplate const* goInfo = GetGOInfo();
        if (goInfo->trap.charges == 2)
            return true;
    }

    // Always seen by owner and friendly units
    ObjectGuid guid = GetOwnerGUID();
    if (!guid.IsEmpty())
    {
        if (seer->GetGUID() == guid)
            return true;

        Unit* owner = GetOwner();
        if (owner && seer->isType(TYPEMASK_UNIT) && owner->IsFriendlyTo(((Unit*)seer)))
            return true;
    }

    return false;
}

bool GameObject::IsNeverVisible(WorldObject const* obj) const
{
    auto last = m_lastUser.find(obj->GetGUID());
    if (last != m_lastUser.end() && time(nullptr) < last->second)
        return true;

    const GameObjectData* data = GetGOData();
    if (data && data->spawnMask == 256)  // only challenge go check. for hiding them after start.
    {
        if (GetMap()->GetSpawnMode() == DIFFICULTY_HEROIC)
            return true;
    }
    return WorldObject::IsNeverVisible();
}

bool GameObject::IsInvisibleDueToDespawn() const
{
    if (WorldObject::IsInvisibleDueToDespawn())
        return true;

    // Despawned
    if (!isSpawned())
        return true;

    return false;
}

void GameObject::Respawn()
{
    if (m_spawnedByDefault && m_respawnTime > 0)
    {
        m_respawnTime = time(NULL);
        GetMap()->RemoveGORespawnTime(m_DBTableGuid);
    }
}

bool GameObject::ActivateToQuest(Player* target) const
{
    if (target->HasQuestForGO(GetEntry()))
        return true;

    for (auto i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
    {
        if (!GetGOInfo()->questItems[i])
            break;
        if (target->HasQuestForItem(GetGOInfo()->questItems[i]))
            return true;
    }

    if (!sObjectMgr->IsGameObjectForQuests(GetEntry()))
        return false;

    switch (GetGoType())
    {
        // scan GO chest with loot including quest items
        case GAMEOBJECT_TYPE_CHEST:
        {
            if (LootTemplates_Gameobject.HaveQuestLootForPlayer(GetEntry(), target))
            {
                //TODO: fix this hack
                //look for battlegroundAV for some objects which are only activated after mine gots captured by own team
                if (GetEntry() == BG_AV_OBJECTID_MINE_N || GetEntry() == BG_AV_OBJECTID_MINE_S)
                    if (Battleground* bg = target->GetBattleground())
                        if (bg->GetTypeID(true) == BATTLEGROUND_AV && !(((BattlegroundAV*)bg)->PlayerCanDoMineQuest(GetEntry(), target->GetTeam())))
                            return false;
                return true;
            }
            break;
        }
        case GAMEOBJECT_TYPE_GENERIC:
        {
            if (GetGOInfo()->generic.questID == -1 || target->GetQuestStatus(GetGOInfo()->generic.questID) == QUEST_STATUS_INCOMPLETE)
                return true;
            break;
        }
        case GAMEOBJECT_TYPE_GOOBER:
        {
            if (GetGOInfo()->goober.questID == -1 || target->GetQuestStatus(GetGOInfo()->goober.questID) == QUEST_STATUS_INCOMPLETE)
                return true;
            break;
        }
        case GAMEOBJECT_TYPE_SPELL_FOCUS:
        {
            if (GetGOInfo()->spellFocus.questID == -1 || target->GetQuestStatus(GetGOInfo()->spellFocus.questID) == QUEST_STATUS_INCOMPLETE)
                return true;
            break;
        }
        default:
            break;
    }

    return false;
}

void GameObject::TriggeringLinkedGameObject(uint32 trapEntry, Unit* target)
{
    GameObjectTemplate const* trapInfo = sObjectMgr->GetGameObjectTemplate(trapEntry);
    if (!trapInfo || trapInfo->type != GAMEOBJECT_TYPE_TRAP)
        return;

    SpellInfo const* trapSpell = sSpellMgr->GetSpellInfo(trapInfo->trap.spell);
    if (!trapSpell)                                          // checked at load already
        return;

    float range = float(target->GetSpellMaxRangeForTarget(GetOwner(), trapSpell));

    // search nearest linked GO
    GameObject* trapGO = NULL;
    {
        // using original GO distance
        CellCoord p(Trinity::ComputeCellCoord(GetPositionX(), GetPositionY()));
        Cell cell(p);

        Trinity::NearestGameObjectEntryInObjectRangeCheck go_check(*target, trapEntry, range);
        Trinity::GameObjectLastSearcher<Trinity::NearestGameObjectEntryInObjectRangeCheck> checker(this, trapGO, go_check);

        TypeContainerVisitor<Trinity::GameObjectLastSearcher<Trinity::NearestGameObjectEntryInObjectRangeCheck>, GridTypeMapContainer > object_checker(checker);
        cell.Visit(p, object_checker, *GetMap(), *target, range);
    }

    // found correct GO
    if (trapGO)
        trapGO->CastSpell(target, trapInfo->trap.spell);
}

GameObject* GameObject::LookupFishingHoleAround(float range)
{
    GameObject* ok = NULL;

    CellCoord p(Trinity::ComputeCellCoord(GetPositionX(), GetPositionY()));
    Cell cell(p);
    Trinity::NearestGameObjectFishingHole u_check(*this, range);
    Trinity::GameObjectSearcher<Trinity::NearestGameObjectFishingHole> checker(this, ok, u_check);

    TypeContainerVisitor<Trinity::GameObjectSearcher<Trinity::NearestGameObjectFishingHole>, GridTypeMapContainer > grid_object_checker(checker);
    cell.Visit(p, grid_object_checker, *GetMap(), *this, range);

    return ok;
}

void GameObject::ResetDoorOrButton()
{
    if (m_lootState == GO_READY || m_lootState == GO_JUST_DEACTIVATED)
        return;

    SwitchDoorOrButton(false);
    SetLootState(GO_JUST_DEACTIVATED);
    m_cooldownTime = 0;
}

void GameObject::UseDoorOrButton(uint32 time_to_restore, bool alternative /* = false */, Unit* user /*=NULL*/)
{
    if (m_lootState != GO_READY)
        return;

    if (!time_to_restore)
        time_to_restore = GetGOInfo()->GetAutoCloseTime();

    SwitchDoorOrButton(true, alternative);
    SetLootState(GO_ACTIVATED, user);

    m_cooldownTime = time(NULL) + time_to_restore;
}

void GameObject::SetGoArtKit(uint8 kit)
{
    SetByteValue(GAMEOBJECT_FIELD_BYTES_1, 2, kit);
    GameObjectData* data = const_cast<GameObjectData*>(sObjectMgr->GetGOData(m_DBTableGuid));
    if (data)
        data->artKit = kit;
}

void GameObject::SetGoArtKit(uint8 artkit, GameObject* go, ObjectGuid::LowType lowguid)
{
    const GameObjectData* data = NULL;
    if (go)
    {
        go->SetGoArtKit(artkit);
        data = go->GetGOData();
    }
    else if (lowguid)
        data = sObjectMgr->GetGOData(lowguid);

    if (data)
        const_cast<GameObjectData*>(data)->artKit = artkit;
}

void GameObject::SwitchDoorOrButton(bool activate, bool alternative /* = false */)
{
    if (activate)
        SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
    else
        RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);

    if (GetGoState() == GO_STATE_READY)                      //if closed -> open
        SetGoState(alternative ? GO_STATE_ACTIVE_ALTERNATIVE : GO_STATE_ACTIVE);
    else                                                    //if open -> close
        SetGoState(GO_STATE_READY);
}

//! Analog SwitchDoorOrButton but with force set state at enable/disable state
void GameObject::EnableOrDisableGo(bool enable, bool alternative)
{
    if (enable)
    {
        SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
        SetGoState(alternative ? GO_STATE_ACTIVE_ALTERNATIVE : GO_STATE_ACTIVE);    //if closed -> open
    }
    else
    {
        RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
        SetGoState(GO_STATE_READY);                                                 //if open -> close
    }
}

void GameObject::Use(Unit* user)
{
    // by default spell caster is user
    Unit* spellCaster = user;
    uint32 spellId = 0;
    bool triggered = false;

    if (Player* playerUser = user->ToPlayer())
    {
        if (sScriptMgr->OnGossipHello(playerUser, this))
            return;

        if (AI()->GossipUse(playerUser))
            return;

        // We do not allow players to use the hidden objects.
        if (playerUser && (!isSpawned() || HasFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE | GO_FLAG_NOT_SELECTABLE)))
            return;
    }

    // If cooldown data present in template
    if (uint32 cooldown = GetGOInfo()->GetCooldown())
    {
        if (m_cooldownTime > sWorld->GetGameTime())
            return;

        m_cooldownTime = sWorld->GetGameTime() + cooldown;
    }

    switch (GetGoType())
    {
        case GAMEOBJECT_TYPE_DOOR:                          //0
        case GAMEOBJECT_TYPE_BUTTON:                        //1
            //doors/buttons never really despawn, only reset to default state/flags
            UseDoorOrButton(0, false, user);

            if (Player* player = user->ToPlayer())
                if (Battleground* bg = player->GetBattleground())
                    bg->EventPlayerUsedGO(player, this);

            // activate script
            GetMap()->ScriptsStart(sGameObjectScripts, GetDBTableGUIDLow(), spellCaster, this);
            return;

        case GAMEOBJECT_TYPE_QUESTGIVER:                    //2
        {
            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            player->PrepareGossipMenu(this, GetGOInfo()->questgiver.gossipID);
            player->SendPreparedGossip(this);
            return;
        }
        case GAMEOBJECT_TYPE_TRAP:                          //6
            {
                GameObjectTemplate const* goInfo = GetGOInfo();
                if (goInfo->trap.spell)
                    CastSpell(user, goInfo->trap.spell);

                m_cooldownTime = time(NULL) + (goInfo->trap.cooldown ? goInfo->trap.cooldown :  uint32(4));   // template or 4 seconds

                if (goInfo->trap.charges == 1)         // Deactivate after trigger
                    SetLootState(GO_JUST_DEACTIVATED);

                return;
            }
        //Sitting: Wooden bench, chairs enzz
        case GAMEOBJECT_TYPE_CHAIR:                         //7
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            if (ChairListSlots.empty())        // this is called once at first chair use to make list of available slots
            {
                if (info->chair.chairslots > 0)     // sometimes chairs in DB have error in fields and we dont know number of slots
                    for (uint32 i = 0; i < info->chair.chairslots; ++i)
                        ChairListSlots[i].Clear(); // Last user of current slot set to 0 (none sit here yet)
                else
                    ChairListSlots[0].Clear();     // error in DB, make one default slot
            }

            Player* player = user->ToPlayer();

            // a chair may have n slots. we have to calculate their positions and teleport the player to the nearest one

            float lowestDist = DEFAULT_VISIBILITY_DISTANCE;

            uint32 nearest_slot = 0;
            float x_lowest = GetPositionX();
            float y_lowest = GetPositionY();

            // the object orientation + 1/2 pi
            // every slot will be on that straight line
            float orthogonalOrientation = GetOrientation()+M_PI*0.5f;
            // find nearest slot
            bool found_free_slot = false;
            for (ChairSlotAndUser::iterator itr = ChairListSlots.begin(); itr != ChairListSlots.end(); ++itr)
            {
                // the distance between this slot and the center of the go - imagine a 1D space
                float relativeDistance = (info->size*itr->first) - (info->size*(info->chair.chairslots - 1) / 2.0f);

                float x_i = GetPositionX() + relativeDistance * std::cos(orthogonalOrientation);
                float y_i = GetPositionY() + relativeDistance * std::sin(orthogonalOrientation);

                if (itr->second)
                {
                    if (Player* ChairUser = ObjectAccessor::FindPlayer(itr->second))
                        if (ChairUser->IsSitState() && ChairUser->getStandState() != UNIT_STAND_STATE_SIT && ChairUser->GetExactDist2d(x_i, y_i) < 0.1f)
                            continue;        // This seat is already occupied by ChairUser. NOTE: Not sure if the ChairUser->getStandState() != UNIT_STAND_STATE_SIT check is required.
                        else
                            itr->second.Clear(); // This seat is unoccupied.
                    else
                        itr->second.Clear();     // The seat may of had an occupant, but they're offline.
                }

                found_free_slot = true;

                // calculate the distance between the player and this slot
                float thisDistance = player->GetDistance2d(x_i, y_i);

                /* debug code. It will spawn a npc on each slot to visualize them.
                Creature* helper = player->SummonCreature(14496, x_i, y_i, GetPositionZ(), GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                std::ostringstream output;
                output << i << ": thisDist: " << thisDistance;
                helper->MonsterSay(output.str().c_str(), LANG_UNIVERSAL, 0);
                */

                if (thisDistance <= lowestDist)
                {
                    nearest_slot = itr->first;
                    lowestDist = thisDistance;
                    x_lowest = x_i;
                    y_lowest = y_i;
                }
            }

            if (found_free_slot)
            {
                ChairSlotAndUser::iterator itr = ChairListSlots.find(nearest_slot);
                if (itr != ChairListSlots.end())
                {
                    itr->second = player->GetGUID(); //this slot in now used by player
                    player->TeleportTo(GetMapId(), x_lowest, y_lowest, GetPositionZ(), GetOrientation(), TELE_TO_NOT_LEAVE_TRANSPORT | TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET);
                    player->SetStandState(UNIT_STAND_STATE_SIT_LOW_CHAIR + info->chair.chairheight);
                    return;
                }
            }
            //else
                //player->GetSession()->SendNotification("There's nowhere left for you to sit.");

            return;
        }
        //big gun, its a spell/aura
        case GAMEOBJECT_TYPE_GOOBER:                        //10
        {
            GameObjectTemplate const* info = GetGOInfo();

            if (user->GetTypeId() == TYPEID_PLAYER)
            {
                Player* player = user->ToPlayer();

                if (info->goober.pageID)                    // show page...
                    player->GetSession()->SendPacket(WorldPackets::GameObject::PageText(GetGUID()).Write());
                else if (info->goober.gossipID)
                {
                    player->PrepareGossipMenu(this, info->goober.gossipID);
                    player->SendPreparedGossip(this);
                }

                if (info->goober.eventID)
                {
                    sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "Goober ScriptStart id %u for GO entry %u (GUID %u).", info->goober.eventID, GetEntry(), GetDBTableGUIDLow());
                    GetMap()->ScriptsStart(sEventScripts, info->goober.eventID, player, this);
                    EventInform(info->goober.eventID);
                }

                // possible quest objective for active quests
                if (info->goober.questID && sObjectMgr->GetQuestTemplate(info->goober.questID))
                {
                    //Quest require to be active for GO using
                    if (player->GetQuestStatus(info->goober.questID) != QUEST_STATUS_INCOMPLETE)
                        break;
                }

                if (Battleground* bg = player->GetBattleground())
                    bg->EventPlayerUsedGO(player, this);

                player->KillCreditGO(info->entry, GetGUID());

                GetMap()->ScriptsStart(sGameObjectScripts, GetDBTableGUIDLow(), player, this);
            }

            if (uint32 trapEntry = info->goober.linkedTrap)
                TriggeringLinkedGameObject(trapEntry, user);

            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
            SetLootState(GO_ACTIVATED, user);

            // this appear to be ok, however others exist in addition to this that should have custom (ex: 190510, 188692, 187389)
            if (info->goober.customAnim)
                SendCustomAnim(GetGoAnimProgress());
            else
                SetGoState(GO_STATE_ACTIVE);

            m_cooldownTime = time(NULL) + (info->GetAutoCloseTime() ? info->GetAutoCloseTime() : 60);

            // cast this spell later if provided
            spellId = info->goober.spell;
            spellCaster = NULL;

            break;
        }
        case GAMEOBJECT_TYPE_CAMERA:                        //13
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            if (info->camera.camera)
                player->SendCinematicStart(info->camera.camera);

            if (info->camera.eventID)
                GetMap()->ScriptsStart(sEventScripts, info->camera.eventID, player, this);

            return;
        }
        //fishing bobber
        case GAMEOBJECT_TYPE_FISHINGNODE:                   //17
        {
            Player* player = user->ToPlayer();
            if (!player)
                return;

            if (player->GetGUID() != GetOwnerGUID())
                return;

            switch (getLootState())
            {
                case GO_READY:                              // ready for loot
                {
                    uint32 zone, subzone;
                    GetZoneAndAreaId(zone, subzone);

                    int32 zone_skill = sObjectMgr->GetFishingBaseSkillLevel(subzone);
                    if (!zone_skill)
                        zone_skill = sObjectMgr->GetFishingBaseSkillLevel(zone);

                    //provide error, no fishable zone or area should be 0
                    if (!zone_skill)
                        sLog->outError(LOG_FILTER_SQL, "Fishable areaId %u are not properly defined in `skill_fishing_base_level`.", subzone);

                    int32 skill = player->GetSkillValue(SKILL_FISHING);

                    int32 chance;
                    if (skill < zone_skill)
                    {
                        chance = int32(pow((double)skill/zone_skill, 2) * 100);
                        if (chance < 1)
                            chance = 1;
                    }
                    else
                        chance = 100;

                    int32 roll = irand(1, 100);

                    sLog->outDebug(LOG_FILTER_GENERAL, "Fishing check (skill: %i zone min skill: %i chance %i roll: %i", skill, zone_skill, chance, roll);

                    // but you will likely cause junk in areas that require a high fishing skill (not yet implemented)
                    if (chance >= roll)
                    {
                        player->UpdateFishingSkill();

                        //TODO: I do not understand this hack. Need some explanation.
                        // prevent removing GO at spell cancel
                        RemoveFromOwner();
                        SetOwnerGUID(player->GetGUID());

                        //TODO: find reasonable value for fishing hole search
                        GameObject* ok = LookupFishingHoleAround(20.0f + CONTACT_DISTANCE);
                        if (ok)
                        {
                            ok->Use(player);
                            SetLootState(GO_JUST_DEACTIVATED);
                        }
                        else
                            player->SendLoot(GetGUID(), LOOT_FISHING);
                    }
                    else // else: junk
                        player->SendLoot(GetGUID(), LOOT_FISHING_JUNK);
                    break;
                }
                case GO_JUST_DEACTIVATED:                   // nothing to do, will be deleted at next update
                    break;
                default:
                {
                    SetLootState(GO_JUST_DEACTIVATED);
                    player->GetSession()->SendPacket(WorldPackets::GameObject::FishNotHooked().Write());
                    break;
                }
            }

            player->FinishSpell(CURRENT_CHANNELED_SPELL);
            return;
        }

        case GAMEOBJECT_TYPE_RITUAL:              //18
        {
            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            Unit* owner = GetOwner();

            GameObjectTemplate const* info = GetGOInfo();

            // ritual owner is set for GO's without owner (not summoned)
            if (!m_ritualOwner && !owner)
                m_ritualOwner = player;

            if (owner)
            {
                if (owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                // accept only use by player from same group as owner, excluding owner itself (unique use already added in spell effect)
                if (player == owner->ToPlayer() || (info->ritual.castersGrouped && !player->IsInSameRaidWith(owner->ToPlayer())))
                    return;

                // expect owner to already be channeling, so if not...
                if (!owner->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    return;

                // in case summoning ritual caster is GO creator
                spellCaster = owner;
            }
            else
            {
                if (player != m_ritualOwner && (info->ritual.castersGrouped && !player->IsInSameRaidWith(m_ritualOwner)))
                    return;

                spellCaster = player;
            }

            AddUniqueUse(player);

            if (info->ritual.animSpell)
            {
                player->CastSpell(player, info->ritual.animSpell, true);

                // for this case, ritual.spellId is always triggered
                triggered = true;
            }

            // full amount unique participants including original summoner
            if (GetUniqueUseCount() == info->ritual.casters)
            {
                spellCaster = m_ritualOwner ? m_ritualOwner : spellCaster;
                if (m_ritualOwner)
                    spellCaster = m_ritualOwner;

                spellId = info->ritual.spell;

                if (spellId == 62330)                       // GO store nonexistent spell, replace by expected
                {
                    // spell have reagent and mana cost but it not expected use its
                    // it triggered spell in fact casted at currently channeled GO
                    spellId = 61993;
                    triggered = true;
                }

                // Cast casterTargetSpell at a random GO user
                // on the current DB there is only one gameobject that uses this (Ritual of Doom)
                // and its required target number is 1 (outter for loop will run once)
                if (info->ritual.casterTargetSpell && info->ritual.casterTargetSpell != 1) // No idea why this field is a bool in some cases
                    for (uint32 i = 0; i < info->ritual.casterTargetSpellTargets; i++)
                        // m_unique_users can contain only player GUIDs
                        if (Player* target = ObjectAccessor::GetPlayer(*this, Trinity::Containers::SelectRandomContainerElement(m_unique_users)))
                            spellCaster->CastSpell(target, info->ritual.casterTargetSpell, true);

                // finish owners spell
                if (owner)
                    owner->FinishSpell(CURRENT_CHANNELED_SPELL);

                // can be deleted now, if
                if (!info->ritual.ritualPersistent)
                    SetLootState(GO_JUST_DEACTIVATED);
                else
                {
                    // reset ritual for this GO
                    m_ritualOwner = NULL;
                    m_unique_users.clear();
                    m_usetimes = 0;
                }
            }
            else
                return;

            // go to end function to spell casting
            break;
        }
        case GAMEOBJECT_TYPE_SPELLCASTER:                   //22
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (info->spellCaster.partyOnly)
            {
                Unit* caster = GetOwner();
                if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (user->GetTypeId() != TYPEID_PLAYER || !user->ToPlayer()->IsInSameRaidWith(caster->ToPlayer()))
                    return;
            }

            user->RemoveAurasByType(SPELL_AURA_MOUNTED);
            spellId = info->spellCaster.spell;

            AddUse();
            break;
        }
        case GAMEOBJECT_TYPE_MEETINGSTONE:                  //23
        {
            GameObjectTemplate const* info = GetGOInfo();

            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            Player* targetPlayer = ObjectAccessor::FindPlayer(player->GetSelection());

            // accept only use by player from same raid as caster, except caster itself
            if (!targetPlayer || targetPlayer == player || !targetPlayer->IsInSameRaidWith(player))
                return;

            //required lvl checks!
            uint8 level = player->getLevel();
            if (level < info->meetingStone.minLevel)
                return;
            level = targetPlayer->getLevel();
            if (level < info->meetingStone.minLevel)
                return;

            if (info->entry == 194097)
                spellId = 61994;                            // Ritual of Summoning
            else
                spellId = 59782;                            // Summoning Stone Effect

            break;
        }

        case GAMEOBJECT_TYPE_FLAGSTAND:                     // 24
        {
            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            if (player->CanUseBattlegroundObject())
            {
                // in battleground check
                Battleground* bg = player->GetBattleground();
                if (!bg)
                    return;
                if (player->GetVehicle())
                    return;

                player->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                player->RemoveAurasByType(SPELL_AURA_MOD_INVISIBILITY);
                player->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
                // BG flag click
                // AB:
                // 15001
                // 15002
                // 15003
                // 15004
                // 15005
                bg->EventPlayerClickedOnFlag(player, this);
                return;                                     //we don;t need to delete flag ... it is despawned!
            }
            break;
        }

        case GAMEOBJECT_TYPE_FISHINGHOLE:                   // 25
        {
            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            player->SendLoot(GetGUID(), LOOT_FISHINGHOLE);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT, GetGOInfo()->entry);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL, GetGOInfo()->entry);
            return;
        }

        case GAMEOBJECT_TYPE_FLAGDROP:                      // 26
        {
            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            if (player->CanUseBattlegroundObject())
            {
                // in battleground check
                Battleground* bg = player->GetBattleground();
                if (!bg)
                    return;
                if (player->GetVehicle())
                    return;
                // BG flag dropped
                // WS:
                // 179785 - Silverwing Flag
                // 179786 - Warsong Flag
                // EotS:
                // 184142 - Netherstorm Flag
                GameObjectTemplate const* info = GetGOInfo();
                if (info)
                {
                    switch (info->entry)
                    {
                        case 179785:                        // Silverwing Flag
                        case 179786:                        // Warsong Flag
                            if (bg->GetTypeID(true) == BATTLEGROUND_WS || bg->GetTypeID(true) == BATTLEGROUND_TP)
                                bg->EventPlayerClickedOnFlag(player, this);
                            break;
                        case 184142:                        // Netherstorm Flag
                            if (bg->GetTypeID(true) == BATTLEGROUND_EY || bg->GetTypeID(true) == BATTLEGROUND_EY_RATED)
                                bg->EventPlayerClickedOnFlag(player, this);
                            break;
                    }
                }
                //this cause to call return, all flags must be deleted here!!
                spellId = 0;
                Delete();
            }
            break;
        }
        case GAMEOBJECT_TYPE_BARBER_CHAIR:                  //32
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (user->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = user->ToPlayer();

            // fallback, will always work
            player->TeleportTo(GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation(), TELE_TO_NOT_LEAVE_TRANSPORT | TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET);
            player->GetSession()->SendPacket(WorldPackets::Misc::EnableBarberShop().Write());
            player->SetStandState(UNIT_STAND_STATE_SIT_LOW_CHAIR+info->barberChair.chairheight);
            return;
        }
        case GAMEOBJECT_TYPE_NEW_FLAG:     //36
        {
            GameObjectTemplate const* goInfo = GetGOInfo();
            if (goInfo->newflag.pickupSpell)
                CastSpell(user, goInfo->newflag.pickupSpell);

            if (user->GetTypeId() == TYPEID_PLAYER)
            {
                Player* player = user->ToPlayer();
                if (Battleground* bg = player->GetBattleground())
                {
                    SetLootState(GO_JUST_DEACTIVATED);
                    bg->EventPlayerUsedGO(player, this);
                }
            }

            return;
        }
        default:
            if (GetGoType() >= MAX_GAMEOBJECT_TYPE)
                sLog->outError(LOG_FILTER_GENERAL, "GameObject::Use(): unit (type: %u, guid: %u, name: %s) tries to use object (guid: %u, entry: %u, name: %s) of unknown type (%u)",
                    user->GetTypeId(), user->GetGUIDLow(), user->GetName(), GetGUIDLow(), GetEntry(), GetGOInfo()->name.c_str(), GetGoType());
            break;
    }

    if (!spellId)
        return;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        if (user->GetTypeId() != TYPEID_PLAYER || !sOutdoorPvPMgr->HandleCustomSpell(user->ToPlayer(), spellId, this))
            sLog->outError(LOG_FILTER_GENERAL, "WORLD: unknown spell id %u at use action for gameobject (Entry: %u GoType: %u)", spellId, GetEntry(), GetGoType());
        else
            sLog->outDebug(LOG_FILTER_OUTDOORPVP, "WORLD: %u non-dbc spell was handled by OutdoorPvP", spellId);
        return;
    }

    if (spellCaster)
        spellCaster->CastSpell(user, spellInfo, triggered);
    else
        CastSpell(user, spellId);
}

void GameObject::CastSpell(Unit* target, uint32 spellId)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    bool self = false;

    GameObjectTemplate const* goInfo = GetGOInfo();
    if (goInfo->type != GAMEOBJECT_TYPE_TRAP && target && target->HasAuraType(SPELL_AURA_MOD_STEALTH))
        self = true;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_CASTER)
        {
            self = true;
            break;
        }
    }

    if (self)
    {
        if (target)
            target->CastSpell(target, spellInfo, true);
        return;
    }

    if (goInfo->type == GAMEOBJECT_TYPE_TRAP && GetOwner() && target && target->HasAuraType(SPELL_AURA_MOD_STEALTH))
    {
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
            {
                self = true;
                break;
            }
        }

        if (spellInfo->ClassOptions.SpellClassSet == SPELLFAMILY_HUNTER && spellInfo->Misc.SpellIconID != 27)
            target->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
    }

    //summon world trigger
    Creature* trigger = SummonTrigger(GetPositionX(), GetPositionY(), GetPositionZ(), 0, spellInfo->CalcCastTime() + 100);
    if (!trigger)
        return;

    if (Unit* owner = GetOwner())
    {
        trigger->setFaction(owner->getFaction());
        // needed for GO casts for proper target validation checks
        trigger->SetGuidValue(UNIT_FIELD_SUMMONED_BY, owner->GetGUID());
        trigger->CastSpell(target ? target : trigger, spellInfo, true, 0, NULL, owner->GetGUID());
    }
    else
    {
        if (target)
            trigger->SetPhaseMask(target->GetPhaseMask(), true);
        trigger->setFaction(14);
        // Set owner guid for target if no owner available - needed by trigger auras
        // - trigger gets despawned and there's no caster avalible (see AuraEffect::TriggerSpell())
        trigger->CastSpell(target ? target : trigger, spellInfo, true, 0, NULL, target ? target->GetGUID() : ObjectGuid::Empty);
    }
}

void GameObject::SendCustomAnim(uint32 animID, bool playAsDespawn /*= false*/)
{
    WorldPackets::GameObject::GoCustomAnim customAnim;
    customAnim.ObjectGUID = GetGUID();
    customAnim.CustomAnim = animID;
    customAnim.PlayAsDespawn = playAsDespawn;
    SendMessageToSet(customAnim.Write(), true);
}

void GameObject::SendGOPlaySpellVisual(uint32 spellVisualID, ObjectGuid activatorGuid /*= ObjectGuid::Empty*/)
{
    WorldPackets::GameObject::GameObjectPlaySpellVisual objectSpellVisual;
    objectSpellVisual.ActivatorGUID = activatorGuid;
    objectSpellVisual.ObjectGUID = GetGUID();
    objectSpellVisual.SpellVisualID = spellVisualID;
    SendMessageToSet(objectSpellVisual.Write(), true);
}

void GameObject::SendGameObjectActivateAnimKit(uint32 animKitID, bool maintain)
{
    WorldPackets::GameObject::GameObjectActivateAnimKit animKit;
    animKit.ObjectGUID = GetGUID();
    animKit.AnimKitID = animKitID;
    animKit.Maintain = maintain;
    SendMessageToSet(animKit.Write(), true);
}

bool GameObject::IsInRange(float x, float y, float z, float radius) const
{
    GameObjectDisplayInfoEntry const* info = sGameObjectDisplayInfoStore.LookupEntry(m_goInfo->displayId);
    if (!info)
        return IsWithinDist3d(x, y, z, radius);

    float sinA = std::sin(GetOrientation());
    float cosA = std::cos(GetOrientation());
    float dx = x - GetPositionX();
    float dy = y - GetPositionY();
    float dz = z - GetPositionZ();
    float dist = sqrt(dx * dx + dy * dy);
    //! Check if the distance between the 2 objects is 0, can happen if both objects are on the same position.
    //! The code below this check wont crash if dist is 0 because 0/0 in float operations is valid, and returns infinite
    if (G3D::fuzzyEq(dist, 0.0f))
        return true;

    float sinB = dx / dist;
    float cosB = dy / dist;
    dx = dist * (cosA * cosB + sinA * sinB);
    dy = dist * (cosA * sinB - sinA * cosB);
    return dx < info->GeoBoxMax.X + radius && dx > info->GeoBoxMin.X - radius
        && dy < info->GeoBoxMax.Y + radius && dy > info->GeoBoxMin.Y - radius
        && dz < info->GeoBoxMax.Z + radius && dz > info->GeoBoxMin.Z - radius;
}

void GameObject::EventInform(uint32 eventId)
{
    if (!eventId)
        return;

    if (AI())
        AI()->EventInform(eventId);

    if (m_zoneScript)
        m_zoneScript->ProcessEvent(this, eventId);
}

// overwrite WorldObject function for proper name localization
const char* GameObject::GetNameForLocaleIdx(LocaleConstant loc_idx) const
{
    if (loc_idx != DEFAULT_LOCALE)
    {
        uint8 uloc_idx = uint8(loc_idx);
        if (GameObjectLocale const* cl = sObjectMgr->GetGameObjectLocale(GetEntry()))
            if (cl->Name.size() > uloc_idx && !cl->Name[uloc_idx].empty())
                return cl->Name[uloc_idx].c_str();
    }

    return GetName();
}

void GameObject::UpdateRotationFields(float rotation2 /*=0.0f*/, float rotation3 /*=0.0f*/)
{
    static double const atan_pow = atan(pow(2.0f, -20.0f));

    double f_rot1 = std::sin(GetOrientation() / 2.0f);
    double f_rot2 = std::cos(GetOrientation() / 2.0f);

    int64 i_rot1 = int64(f_rot1 / atan_pow *(f_rot2 >= 0 ? 1.0f : -1.0f));
    int64 rotation = (i_rot1 << 43 >> 43) & 0x00000000001FFFFF;

    //float f_rot2 = std::sin(0.0f / 2.0f);
    //int64 i_rot2 = f_rot2 / atan(pow(2.0f, -20.0f));
    //rotation |= (((i_rot2 << 22) >> 32) >> 11) & 0x000003FFFFE00000;

    //float f_rot3 = std::sin(0.0f / 2.0f);
    //int64 i_rot3 = f_rot3 / atan(pow(2.0f, -21.0f));
    //rotation |= (i_rot3 >> 42) & 0x7FFFFC0000000000;

    m_rotation = rotation;

    if (rotation2 == 0.0f && rotation3 == 0.0f)
    {
        rotation2 = (float)f_rot1;
        rotation3 = (float)f_rot2;
    }

    SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+2, rotation2);
    SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+3, rotation3);
}

void GameObject::ModifyHealth(int32 change, Unit* attackerOrHealer /*= NULL*/, uint32 spellId /*= 0*/)
{
    if (!GetGOValue()->Building.MaxHealth || !change)
        return;

    // prevent double destructions of the same object
    if (change < 0 && !GetGOValue()->Building.Health)
        return;

    if (int32(GetGOValue()->Building.Health) + change <= 0)
        GetGOValue()->Building.Health = 0;
    else if (int32(GetGOValue()->Building.Health) + change >= int32(GetGOValue()->Building.MaxHealth))
        GetGOValue()->Building.Health = GetGOValue()->Building.MaxHealth;
    else
        GetGOValue()->Building.Health += change;

    // Set the health bar, value = 255 * healthPct;
    SetGoAnimProgress(GetGOValue()->Building.Health * 255 / GetGOValue()->Building.MaxHealth);

    Player* player = attackerOrHealer ? attackerOrHealer->GetCharmerOrOwnerPlayerOrPlayerItself(): NULL;

    // dealing damage, send packet
    // TODO: is there any packet for healing?
    if (change < 0 && player)
    {
        WorldPackets::GameObject::DestructibleBuildingDamage buildingDamage;
        buildingDamage.Target = GetGUID();
        buildingDamage.Caster = attackerOrHealer->GetGUID();
        buildingDamage.Owner = GetOwnerGUID();
        buildingDamage.Damage = -change;
        buildingDamage.SpellID = spellId;
        player->GetSession()->SendPacket(buildingDamage.Write());
    }

    GameObjectDestructibleState newState = GetDestructibleState();

    if (!GetGOValue()->Building.Health)
        newState = GO_DESTRUCTIBLE_DESTROYED;
    else if (GetGOValue()->Building.Health <= 10000/*GetGOInfo()->destructibleBuilding.damagedNumHits*/) // TODO: Get health somewhere
        newState = GO_DESTRUCTIBLE_DAMAGED;
    else if (GetGOValue()->Building.Health == GetGOValue()->Building.MaxHealth)
        newState = GO_DESTRUCTIBLE_INTACT;

    if (newState == GetDestructibleState())
        return;

    SetDestructibleState(newState, player, false);
}

void GameObject::SetDestructibleState(GameObjectDestructibleState state, Player* eventInvoker /*= NULL*/, bool setHealth /*= false*/)
{
    // the user calling this must know he is already operating on destructible gameobject
    ASSERT(GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING);

    switch (state)
    {
        case GO_DESTRUCTIBLE_INTACT:
            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_DESTROYED);
            SetDisplayId(m_goInfo->displayId);
            if (setHealth)
            {
                m_goValue->Building.Health = m_goValue->Building.MaxHealth;
                SetGoAnimProgress(255);
            }
            EnableCollision(true);
            break;
        case GO_DESTRUCTIBLE_DAMAGED:
        {
            EventInform(m_goInfo->destructibleBuilding.DamagedEvent);
            sScriptMgr->OnGameObjectDamaged(this, eventInvoker);
            if (eventInvoker)
                if (Battleground* bg = eventInvoker->GetBattleground())
                    bg->EventPlayerDamagedGO(eventInvoker, this, m_goInfo->destructibleBuilding.DamagedEvent);

            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DESTROYED);
            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED);

            uint32 modelId = m_goInfo->destructibleBuilding.DestructibleModelRec;
            if (DestructibleModelDataEntry const* modelData = sDestructibleModelDataStore.LookupEntry(m_goInfo->destructibleBuilding.DestructibleModelRec))
                if (modelData->StateDamagedDisplayID)
                    modelId = modelData->StateDamagedDisplayID;
            SetDisplayId(modelId);

            if (setHealth)
            {
                m_goValue->Building.Health = 10000/*m_goInfo->destructibleBuilding.damagedNumHits*/;
                uint32 maxHealth = m_goValue->Building.MaxHealth;
                // in this case current health is 0 anyway so just prevent crashing here
                if (!maxHealth)
                    maxHealth = 1;
                SetGoAnimProgress(m_goValue->Building.Health * 255 / maxHealth);
            }
            break;
        }
        case GO_DESTRUCTIBLE_DESTROYED:
        {
            sScriptMgr->OnGameObjectDestroyed(this, eventInvoker);
            EventInform(m_goInfo->destructibleBuilding.DestroyedEvent);
            if (eventInvoker)
            {
                if (Battleground* bg = eventInvoker->GetBattleground())
                {
                    bg->EventPlayerDamagedGO(eventInvoker, this, m_goInfo->destructibleBuilding.DestroyedEvent);
                    bg->DestroyGate(eventInvoker, this);
                }
            }

            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED);
            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DESTROYED);

            uint32 modelId = m_goInfo->displayId;
            if (DestructibleModelDataEntry const* modelData = sDestructibleModelDataStore.LookupEntry(m_goInfo->destructibleBuilding.DestructibleModelRec))
                if (modelData->StateDestroyedDisplayID)
                    modelId = modelData->StateDestroyedDisplayID;
            SetDisplayId(modelId);

            if (setHealth)
            {
                m_goValue->Building.Health = 0;
                SetGoAnimProgress(0);
            }
            EnableCollision(false);
            break;
        }
        case GO_DESTRUCTIBLE_REBUILDING:
        {
            EventInform(m_goInfo->destructibleBuilding.RebuildingEvent);
            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_DESTROYED);

            uint32 modelId = m_goInfo->displayId;
            if (DestructibleModelDataEntry const* modelData = sDestructibleModelDataStore.LookupEntry(m_goInfo->destructibleBuilding.DestructibleModelRec))
                if (modelData->StateRebuildingDisplayID)
                    modelId = modelData->StateRebuildingDisplayID;
            SetDisplayId(modelId);

            // restores to full health
            if (setHealth)
            {
                m_goValue->Building.Health = m_goValue->Building.MaxHealth;
                SetGoAnimProgress(255);
            }
            EnableCollision(true);
            break;
        }
    }

    UpdateObjectVisibility();
}

void GameObject::SetLootState(LootState state, Unit* unit)
{
    m_lootState = state;
    AI()->OnStateChanged(state, unit);
    sScriptMgr->OnGameObjectLootStateChanged(this, state, unit);
    if (m_model)
    {
        bool collision = false;

        // Use the current go state
        if ((GetGoState() != GO_STATE_READY && (state == GO_ACTIVATED || state == GO_JUST_DEACTIVATED)) || state == GO_READY)
            collision = !collision;

        EnableCollision(collision);
    }
}

void GameObject::SetGoState(GOState state)
{
    GOState oldState = GetGoState();
    SetByteValue(GAMEOBJECT_FIELD_BYTES_1, 0, state);
    if (oldState != state && HasManualAnim())
        SetUInt32Value(GAMEOBJECT_FIELD_LEVEL, getMSTime() + CalculateAnimDuration(oldState, state));

    sScriptMgr->OnGameObjectStateChanged(this, state);
    if (m_model)
    {
        if (!IsInWorld())
            return;

        // startOpen determines whether we are going to add or remove the LoS on activation
        bool collision = false;
        if (state == GO_STATE_READY)
            collision = !collision;

        EnableCollision(collision);
    }
    switch (state)
    {
        case GO_STATE_READY:
            if (!m_goInfo->visualQuestID)
            {
                if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID)
                    SetUInt32Value(GAMEOBJECT_FIELD_STATE_WORLD_EFFECT_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID);
                if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID)
                    SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID);
            }

            if (GetGOInfo()->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellVisualID)
                SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, GetGOInfo()->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellVisualID);

            break;
        default:
            if (!m_goInfo->visualQuestID)
            {
                if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].StateWorldEffectID)
                    SetUInt32Value(GAMEOBJECT_FIELD_STATE_WORLD_EFFECT_ID, 0);
                if (m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID)
                    SetUInt32Value(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, m_goInfo->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellStateVisualID);
            }
            if (GetGOInfo()->visualData[GO_VISUAL_BEFORE_COMPLETE_QUEST].SpellVisualID)
                SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, 0);

            break;
    }
}

void GameObject::SetDisplayId(uint32 displayid)
{
    SetUInt32Value(GAMEOBJECT_FIELD_DISPLAY_ID, displayid);
    UpdateModel();
}

void GameObject::SetPhaseMask(uint32 newPhaseMask, bool update)
{
    WorldObject::SetPhaseMask(newPhaseMask, update);
    if (m_model && m_model->isEnabled())
        EnableCollision(true);
}

void GameObject::EnableCollision(bool enable)
{
    if (!m_model)
        return;

    /*if (enable && !GetMap()->ContainsGameObjectModel(*m_model))
        GetMap()->InsertGameObjectModel(*m_model);*/

    m_model->enable(enable ? GetPhaseMask() : 0);
}

void GameObject::UpdateModel()
{
    if (!IsInWorld())
        return;
    if (m_model)
        if (GetMap()->ContainsGameObjectModel(*m_model))
            GetMap()->RemoveGameObjectModel(*m_model);
    delete m_model;
    m_model = CreateModel();
    if (m_model)
        GetMap()->InsertGameObjectModel(*m_model);
}

Player* GameObject::GetLootRecipient() const
{
    if (!m_lootRecipient)
        return NULL;
    return ObjectAccessor::FindPlayer(m_lootRecipient);
}

Group* GameObject::GetLootRecipientGroup() const
{
    if (!m_lootRecipientGroup)
        return NULL;
    return sGroupMgr->GetGroupByGUID(m_lootRecipientGroup);
}

void GameObject::SetLootRecipient(Unit* unit)
{
    // set the player whose group should receive the right
    // to loot the creature after it dies
    // should be set to NULL after the loot disappears

    if (!unit)
    {
        m_lootRecipient.Clear();
        m_lootRecipientGroup.Clear();
        return;
    }

    if (unit->GetTypeId() != TYPEID_PLAYER && !unit->IsVehicle())
        return;

    Player* player = unit->GetCharmerOrOwnerPlayerOrPlayerItself();
    if (!player)                                             // normal creature, no player involved
        return;

    m_lootRecipient = player->GetGUID();
    if (Group* group = player->GetGroup())
        m_lootRecipientGroup = group->GetGUID();
}

bool GameObject::IsLootAllowedFor(Player const* player) const
{
    if (!m_lootRecipient && !m_lootRecipientGroup)
        return true;

    if (player->GetGUID() == m_lootRecipient)
        return true;

    Group const* playerGroup = player->GetGroup();
    if (!playerGroup || playerGroup != GetLootRecipientGroup()) // if we dont have a group we arent the recipient
        return false;                                           // if go doesnt have group bound it means it was solo killed by someone else

    return true;
}

uint32 GameObject::CalculateAnimDuration(GOState oldState, GOState newState) const
{
    if (oldState == newState || oldState >= MAX_GO_STATE || newState >= MAX_GO_STATE)
        return 0;

    auto const& itr = sDB2Manager._transportAnimationsByEntry.find(GetEntry());
    if (itr == sDB2Manager._transportAnimationsByEntry.end())
        return 0;

    uint32 frameByState[MAX_GO_STATE] = { 0, m_goInfo->transport.Timeto2ndfloor, m_goInfo->transport.Timeto3rdfloor };
    if (oldState == GO_STATE_ACTIVE)
        return frameByState[newState];

    if (newState == GO_STATE_ACTIVE)
        return frameByState[oldState];

    return uint32(std::abs(int32(frameByState[oldState]) - int32(frameByState[newState])));
}

bool GameObject::IsPersonalLoot() const
{
    if(GetGOInfo()->chest.chestPersonalLoot)
        if(sObjectMgr->GetPersonalLootData(GetEntry()))
            return true;

    return false;
}

void GameObject::setVisibilityCDForPlayer(ObjectGuid const& guid, uint32 sec/* = 300*/)
{
    // By default set 5 min.
    m_lastUser[guid] = time(nullptr) + sec;
}


void GameObject::GameObjectAction()
{
    if (!m_actionActive)
        return;

    std::list<Player*> playerList;
    GetPlayerListInGrid(playerList, m_maxActionDistance);

    if (playerList.empty())
        return;

    for (std::vector<GameObjectActionData>::const_iterator itr = m_actionVector->begin(); itr != m_actionVector->end(); ++itr)
    {
        for (auto player : playerList)
        {
            if (player->GetDistance2d(this) > float(itr->Distance))
                continue;

            if (itr->SpellID)
                player->CastSpell(player, itr->SpellID, true);

            if (itr->WorldSafeLocID)
            {
                if (WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(itr->WorldSafeLocID))
                {
                    float Orientation = entry->Loc.O * 3.14159265358979323846f / 180.0f;
                    player->TeleportTo(entry->MapID, entry->Loc.X, entry->Loc.Y, entry->Loc.Z, Orientation);
                }
            }
            else if(itr->X != 0.0f || itr->Y != 0.0f || itr->Z != 0.0f)
                player->TeleportTo(itr->MapID, itr->X, itr->Y, itr->Z, itr->O);
        }
    }
}

class GameObjectModelOwnerImpl : public GameObjectModelOwnerBase
{
public:
    explicit GameObjectModelOwnerImpl(GameObject const* owner) : _owner(owner) { }

    virtual bool IsSpawned() const override { return _owner->isSpawned(); }
    virtual uint32 GetDisplayId() const override { return _owner->GetDisplayId(); }
    virtual uint32 GetPhaseMask() const override { return _owner->GetPhaseMask(); }
    virtual G3D::Vector3 GetPosition() const override { return G3D::Vector3(_owner->GetPositionX(), _owner->GetPositionY(), _owner->GetPositionZ()); }
    virtual float GetOrientation() const override { return _owner->GetOrientation(); }
    virtual float GetScale() const override { return _owner->GetObjectScale(); }
    virtual void DebugVisualizeCorner(G3D::Vector3 const& corner) const override
    {
        Position pos;
        pos.Relocate(corner.x, corner.y, corner.z, 0.0f);
        _owner->SummonCreature(1, pos, TEMPSUMMON_MANUAL_DESPAWN);
    }

private:
    GameObject const* _owner;
};

GameObjectModel* GameObject::CreateModel()
{
    return GameObjectModel::Create(Trinity::make_unique<GameObjectModelOwnerImpl>(this), sWorld->GetDataPath());
}

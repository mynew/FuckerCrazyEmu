/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: faction_champions
SD%Complete: ??%
SDComment: Scripts by Selector, modified by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

// Known bugs:
// All - untested

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "trial_of_the_crusader.h"

enum Yells
{
    SAY_KILL_PLAYER     = 6,
};

enum eAIs
{
    AI_MELEE    = 0,
    AI_RANGED   = 1,
    AI_HEALER   = 2,
    AI_PET      = 3,
};

enum eSpells
{
    SPELL_ANTI_AOE      = 68595,
    SPELL_PVP_TRINKET   = 65547,
};

class boss_toc_champion_controller : public CreatureScript
{
public:
    boss_toc_champion_controller() : CreatureScript("boss_toc_champion_controller") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_toc_champion_controllerAI (creature);
    }

    struct boss_toc_champion_controllerAI : public ScriptedAI
    {
        boss_toc_champion_controllerAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        SummonList Summons;
        uint32 m_uiChampionsNotStarted;
        uint32 m_uiChampionsFailed;
        uint32 m_uiChampionsKilled;
        bool   m_bInProgress;

        void Reset()
        {
            m_uiChampionsNotStarted = 0;
            m_uiChampionsFailed = 0;
            m_uiChampionsKilled = 0;
            m_bInProgress = false;
        }

        std::vector<uint32> SelectChampions(Team playerTeam)
        {
            std::vector<uint32> vHealersEntries;
            vHealersEntries.clear();
            vHealersEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_DRUID_RESTORATION : NPC_ALLIANCE_DRUID_RESTORATION);
            vHealersEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_PALADIN_HOLY : NPC_ALLIANCE_PALADIN_HOLY);
            vHealersEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_PRIEST_DISCIPLINE : NPC_ALLIANCE_PRIEST_DISCIPLINE);
            vHealersEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_SHAMAN_RESTORATION : NPC_ALLIANCE_SHAMAN_RESTORATION);

            std::vector<uint32> vOtherEntries;
            vOtherEntries.clear();
            vOtherEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_DEATH_KNIGHT : NPC_ALLIANCE_DEATH_KNIGHT);
            vOtherEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_HUNTER : NPC_ALLIANCE_HUNTER);
            vOtherEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_MAGE : NPC_ALLIANCE_MAGE);
            vOtherEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_ROGUE : NPC_ALLIANCE_ROGUE);
            vOtherEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_WARLOCK : NPC_ALLIANCE_WARLOCK);
            vOtherEntries.push_back(playerTeam == ALLIANCE ? NPC_HORDE_WARRIOR : NPC_ALLIANCE_WARRIOR);

            uint8 healersSubtracted = 2;
            if (instance->instance->GetSpawnMode() == RAID_DIFFICULTY_25MAN_NORMAL || instance->instance->GetSpawnMode() == RAID_DIFFICULTY_25MAN_HEROIC)
                healersSubtracted = 1;
            for (uint8 i = 0; i < healersSubtracted; ++i)
            {
                uint8 pos = urand(0, vHealersEntries.size()-1);
                switch (vHealersEntries[pos])
                {
                    case NPC_ALLIANCE_DRUID_RESTORATION:
                        vOtherEntries.push_back(NPC_ALLIANCE_DRUID_BALANCE);
                        break;
                    case NPC_HORDE_DRUID_RESTORATION:
                        vOtherEntries.push_back(NPC_HORDE_DRUID_BALANCE);
                        break;
                    case NPC_ALLIANCE_PALADIN_HOLY:
                        vOtherEntries.push_back(NPC_ALLIANCE_PALADIN_RETRIBUTION);
                        break;
                    case NPC_HORDE_PALADIN_HOLY:
                        vOtherEntries.push_back(NPC_HORDE_PALADIN_RETRIBUTION);
                        break;
                    case NPC_ALLIANCE_PRIEST_DISCIPLINE:
                        vOtherEntries.push_back(NPC_ALLIANCE_PRIEST_SHADOW);
                        break;
                    case NPC_HORDE_PRIEST_DISCIPLINE:
                        vOtherEntries.push_back(NPC_HORDE_PRIEST_SHADOW);
                        break;
                    case NPC_ALLIANCE_SHAMAN_RESTORATION:
                        vOtherEntries.push_back(NPC_ALLIANCE_SHAMAN_ENHANCEMENT);
                        break;
                    case NPC_HORDE_SHAMAN_RESTORATION:
                        vOtherEntries.push_back(NPC_HORDE_SHAMAN_ENHANCEMENT);
                        break;
                }
                vHealersEntries.erase(vHealersEntries.begin()+pos);
            }

            if (instance->instance->GetSpawnMode() == RAID_DIFFICULTY_10MAN_NORMAL || instance->instance->GetSpawnMode() == RAID_DIFFICULTY_10MAN_HEROIC)
                for (uint8 i = 0; i < 4; ++i)
                    vOtherEntries.erase(vOtherEntries.begin()+urand(0, vOtherEntries.size()-1));

            std::vector<uint32> vChampionEntries;
            vChampionEntries.clear();
            for (uint8 i = 0; i < vHealersEntries.size(); ++i)
                vChampionEntries.push_back(vHealersEntries[i]);
            for (uint8 i = 0; i < vOtherEntries.size(); ++i)
                vChampionEntries.push_back(vOtherEntries[i]);

            return vChampionEntries;
        }

        void SummonChampions(Team playerTeam)
        {
            std::vector<Position> vChampionJumpOrigin;
            if (playerTeam == ALLIANCE)
                for (uint8 i = 0; i < 5; i++)
                    vChampionJumpOrigin.push_back(FactionChampionLoc[i]);
            else
                for (uint8 i = 5; i < 10; i++)
                    vChampionJumpOrigin.push_back(FactionChampionLoc[i]);

            std::vector<Position> vChampionJumpTarget;
            for (uint8 i = 10; i < 20; i++)
                vChampionJumpTarget.push_back(FactionChampionLoc[i]);
            std::vector<uint32> vChampionEntries = SelectChampions(playerTeam);

            for (uint8 i = 0; i < vChampionEntries.size(); ++i)
            {
                uint8 pos = urand(0, vChampionJumpTarget.size()-1);
                if (Creature* temp = me->SummonCreature(vChampionEntries[i], vChampionJumpOrigin[urand(0, vChampionJumpOrigin.size()-1)], TEMPSUMMON_MANUAL_DESPAWN))
                {
                    Summons.Summon(temp);
                    temp->SetReactState(REACT_PASSIVE);
                    temp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    if (playerTeam == ALLIANCE)
                    {
                        temp->SetHomePosition(vChampionJumpTarget[pos].GetPositionX(), vChampionJumpTarget[pos].GetPositionY(), vChampionJumpTarget[pos].GetPositionZ(), 0);
                        temp->GetMotionMaster()->MoveJump(vChampionJumpTarget[pos].GetPositionX(), vChampionJumpTarget[pos].GetPositionY(), vChampionJumpTarget[pos].GetPositionZ(), 20.0f, 20.0f);
                        temp->SetOrientation(0);
                    }
                    else
                    {
                        temp->SetHomePosition((ToCCommonLoc[1].GetPositionX()*2)-vChampionJumpTarget[pos].GetPositionX(), vChampionJumpTarget[pos].GetPositionY(), vChampionJumpTarget[pos].GetPositionZ(), 3);
                        temp->GetMotionMaster()->MoveJump((ToCCommonLoc[1].GetPositionX()*2)-vChampionJumpTarget[pos].GetPositionX(), vChampionJumpTarget[pos].GetPositionY(), vChampionJumpTarget[pos].GetPositionZ(), 20.0f, 20.0f);
                        temp->SetOrientation(3);
                    }
                }
                vChampionJumpTarget.erase(vChampionJumpTarget.begin()+pos);
            }
        }

        void SetData(uint32 uiType, uint32 uiData)
        {
            switch (uiType)
            {
                case 0:
                    SummonChampions((Team)uiData);
                    break;
                case 1:
                    for (std::list<uint64>::iterator i = Summons.begin(); i != Summons.end(); ++i)
                    {
                        if (Creature* temp = Unit::GetCreature(*me, *i))
                        {
                            temp->SetReactState(REACT_AGGRESSIVE);
                            temp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                        }
                    }
                    break;
                case 2:
                    switch (uiData)
                    {
                        case FAIL:
                            m_uiChampionsFailed++;
                            if (m_uiChampionsFailed + m_uiChampionsKilled >= Summons.size())
                            {
                                instance->SetData(TYPE_CRUSADERS, FAIL);
                                Summons.DespawnAll();
                                me->DespawnOrUnsummon();
                            }
                            break;
                        case IN_PROGRESS:
                            if (!m_bInProgress)
                            {
                                m_uiChampionsNotStarted = 0;
                                m_uiChampionsFailed = 0;
                                m_uiChampionsKilled = 0;
                                m_bInProgress = true;
                                Summons.DoZoneInCombat();
                                instance->SetData(TYPE_CRUSADERS, IN_PROGRESS);
                            }
                            break;
                        case DONE:
                            m_uiChampionsKilled++;
                            if (m_uiChampionsKilled == 1)
                                instance->SetData(TYPE_CRUSADERS, SPECIAL);
                            else if (m_uiChampionsKilled >= Summons.size())
                            {
                                instance->SetData(TYPE_CRUSADERS, DONE);
                                Summons.DespawnAll();
                                me->DespawnOrUnsummon();
                            }
                            break;
                    }
                    break;
            }
        }
    };

};

struct boss_faction_championsAI : public ScriptedAI
{
    boss_faction_championsAI(Creature* creature, uint32 aitype) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
        mAIType = aitype;
    }

    InstanceScript* instance;

    uint64 championControllerGUID;
    uint32 mAIType;
    uint32 ThreatTimer;
    uint32 CCTimer;

    void Reset()
    {
        championControllerGUID = 0;
        CCTimer = rand()%10000;
        ThreatTimer = 5000;
    }

    void JustReachedHome()
    {
        if (instance)
            if (Creature* pChampionController = Unit::GetCreature((*me), instance->GetData64(NPC_CHAMPIONS_CONTROLLER)))
                pChampionController->AI()->SetData(2, FAIL);
        me->DespawnOrUnsummon();
    }

    float CalculateThreat(float distance, float armor, uint32 health)
    {
        float dist_mod = (mAIType == AI_MELEE || mAIType == AI_PET) ? 15.0f/(15.0f + distance) : 1.0f;
        float armor_mod = (mAIType == AI_MELEE || mAIType == AI_PET) ? armor / 16635.0f : 0.0f;
        float eh = (health+1) * (1.0f + armor_mod);
        return dist_mod * 30000.0f / eh;
    }

    void UpdateThreat()
    {
        std::list<HostileReference*> const& tList = me->getThreatManager().getThreatList();
        for (std::list<HostileReference*>::const_iterator itr = tList.begin(); itr != tList.end(); ++itr)
        {
            Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid());
            if (unit && me->getThreatManager().getThreat(unit))
            {
                if (unit->GetTypeId()==TYPEID_PLAYER)
                {
                    float threat = CalculateThreat(me->GetDistance2d(unit), (float)unit->GetArmor(), unit->GetHealth());
                    me->getThreatManager().modifyThreatPercent(unit, -100);
                    me->AddThreat(unit, 1000000.0f * threat);
                }
            }
        }
    }

    void UpdatePower()
    {
        if (me->getPowerType() == POWER_MANA)
            me->ModifyPower(POWER_MANA, me->GetMaxPower(POWER_MANA) / 3);
        //else if (me->getPowerType() == POWER_ENERGY)
        //    me->ModifyPower(POWER_ENERGY, 100);
    }

    void RemoveCC()
    {
        me->RemoveAurasByType(SPELL_AURA_MOD_STUN);
        me->RemoveAurasByType(SPELL_AURA_MOD_FEAR);
        me->RemoveAurasByType(SPELL_AURA_MOD_ROOT);
        me->RemoveAurasByType(SPELL_AURA_MOD_PACIFY);
        me->RemoveAurasByType(SPELL_AURA_MOD_CONFUSE);
        //DoCast(me, SPELL_PVP_TRINKET);
    }

    void JustDied(Unit* /*killer*/)
    {
        if (mAIType != AI_PET)
            if (instance)
                if (Creature* pChampionController = Unit::GetCreature((*me), instance->GetData64(NPC_CHAMPIONS_CONTROLLER)))
                    pChampionController->AI()->SetData(2, DONE);
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoCast(me, SPELL_ANTI_AOE, true);
        me->SetInCombatWithZone();
        if (instance)
            if (Creature* pChampionController = Unit::GetCreature((*me), instance->GetData64(NPC_CHAMPIONS_CONTROLLER)))
                pChampionController->AI()->SetData(2, IN_PROGRESS);
    }

    void KilledUnit(Unit* who)
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            Map::PlayerList const &players = me->GetMap()->GetPlayers();
            uint32 TeamInInstance = 0;

            if (!players.isEmpty())
                if (Player* player = players.begin()->getSource())
                    TeamInInstance = player->GetTeam();

            if (instance)
            {
                if (TeamInInstance == ALLIANCE)
                {
                    if (Creature* temp = Unit::GetCreature(*me, instance->GetData64(NPC_VARIAN)))
                        temp->AI()->Talk(SAY_KILL_PLAYER);
                }
                else
                    if (Creature* temp = Unit::GetCreature(*me, instance->GetData64(NPC_GARROSH)))
                        temp->AI()->Talk(SAY_KILL_PLAYER);


                instance->SetData(DATA_TRIBUTE_TO_IMMORTALITY_ELEGIBLE, 0);
            }
        }
    }

    Creature* SelectRandomFriendlyMissingBuff(uint32 spell)
    {
        std::list<Creature*> lst = DoFindFriendlyMissingBuff(40.0f, spell);
        std::list<Creature*>::const_iterator itr = lst.begin();
        if (lst.empty())
            return NULL;
        advance(itr, rand()%lst.size());
        return (*itr);
    }

    Unit* SelectEnemyCaster(bool /*casting*/)
    {
        std::list<HostileReference*> const& tList = me->getThreatManager().getThreatList();
        std::list<HostileReference*>::const_iterator iter;
        Unit* target;
        for (iter = tList.begin(); iter!=tList.end(); ++iter)
        {
            target = Unit::GetUnit(*me, (*iter)->getUnitGuid());
            if (target && target->getPowerType() == POWER_MANA)
                return target;
        }
        return NULL;
    }

    uint32 EnemiesInRange(float distance)
    {
        std::list<HostileReference*> const& tList = me->getThreatManager().getThreatList();
        std::list<HostileReference*>::const_iterator iter;
        uint32 count = 0;
        Unit* target;
        for (iter = tList.begin(); iter!=tList.end(); ++iter)
        {
            target = Unit::GetUnit(*me, (*iter)->getUnitGuid());
                if (target && me->GetDistance2d(target) < distance)
                    ++count;
        }
        return count;
    }

    void AttackStart(Unit* who)
    {
        if (!who)
            return;

        if (me->Attack(who, true))
        {
            me->AddThreat(who, 10.0f);
            me->SetInCombatWith(who);
            who->SetInCombatWith(me);

            if (mAIType == AI_MELEE || mAIType == AI_PET)
                DoStartMovement(who);
            else
                DoStartMovement(who, 20.0f);
            SetCombatMovement(true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (ThreatTimer < uiDiff)
        {
            UpdatePower();
            UpdateThreat();
            ThreatTimer = 4000;
        }
        else ThreatTimer -= uiDiff;

        if (mAIType != AI_PET)
        {
            if (CCTimer < uiDiff)
            {
                RemoveCC();
                CCTimer = 8000+rand()%2000;
            }
            else CCTimer -= uiDiff;
        }

        if (mAIType == AI_MELEE || mAIType == AI_PET) DoMeleeAttackIfReady();
    }
};

/********************************************************************
                            HEALERS
********************************************************************/
enum eDruidSpells
{
    SPELL_LIFEBLOOM         = 66093,
    SPELL_NOURISH           = 66066,
    SPELL_REGROWTH          = 66067,
    SPELL_REJUVENATION      = 66065,
    SPELL_TRANQUILITY       = 66086,
    SPELL_BARKSKIN          = 65860, //1 min cd
    SPELL_THORNS            = 66068,
    SPELL_NATURE_GRASP      = 66071, //1 min cd, self buff
};

class mob_toc_druid : public CreatureScript
{
    enum
    {
        EVENT_LIFEBLOOM = 1,
        EVENT_NOURISH,
        EVENT_REGROWTH,
        EVENT_REJUVENATION,
        EVENT_TRANQUILITY,
        EVENT_BARKSKIN,
        EVENT_THORNS,
        EVENT_NATURE_GRASP,
    };

    public:
        mob_toc_druid() : CreatureScript("mob_toc_druid") { }

        struct mob_toc_druidAI : public boss_faction_championsAI
        {
            mob_toc_druidAI(Creature* creature) : boss_faction_championsAI(creature, AI_HEALER) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_LIFEBLOOM, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_NOURISH, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_REGROWTH, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_REJUVENATION, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_TRANQUILITY, urand(5*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BARKSKIN, urand(15*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_THORNS, 2*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_NATURE_GRASP, urand(3*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                SetEquipmentSlots(false, 51799, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_LIFEBLOOM:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_LIFEBLOOM);
                            events.ScheduleEvent(EVENT_LIFEBLOOM, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_NOURISH:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_NOURISH);
                            events.ScheduleEvent(EVENT_NOURISH, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_REGROWTH:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_REGROWTH);
                            events.ScheduleEvent(EVENT_REGROWTH, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_REJUVENATION:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_REJUVENATION);
                            events.ScheduleEvent(EVENT_REJUVENATION, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_TRANQUILITY:
                            DoCastAOE(SPELL_TRANQUILITY);
                            events.ScheduleEvent(EVENT_TRANQUILITY, urand(25*IN_MILLISECONDS, 50*IN_MILLISECONDS));
                            return;
                        case EVENT_BARKSKIN:
                            if (HealthBelowPct(30))
                            {
                                DoCast(me, SPELL_BARKSKIN);
                                events.ScheduleEvent(EVENT_BARKSKIN, 60*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_BARKSKIN, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_THORNS:
                            if (Creature* target = SelectRandomFriendlyMissingBuff(SPELL_THORNS))
                                DoCast(target, SPELL_THORNS);
                            events.ScheduleEvent(EVENT_THORNS, urand(25*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                            return;
                        case EVENT_NATURE_GRASP:
                            DoCast(me, SPELL_NATURE_GRASP);
                            events.ScheduleEvent(EVENT_NATURE_GRASP, 60*IN_MILLISECONDS);
                            return;
                    }
                }
            }

            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_druidAI (creature);
        }
};

enum eShamanSpells
{
    SPELL_HEALING_WAVE          = 66055,
    SPELL_RIPTIDE               = 66053,
    SPELL_SPIRIT_CLEANSE        = 66056, //friendly only
    SPELL_HEROISM               = 65983,
    SPELL_BLOODLUST             = 65980,
    SPELL_HEX                   = 66054,
    SPELL_EARTH_SHIELD          = 66063,
    SPELL_EARTH_SHOCK           = 65973,
    AURA_EXHAUSTION             = 57723,
    AURA_SATED                  = 57724,
};

class mob_toc_shaman : public CreatureScript
{
    enum
    {
        EVENT_HEALING_WAVE = 1,
        EVENT_RIPTIDE,
        EVENT_SPIRIT_CLEANSE,
        EVENT_BLOODLUST_HEROISM,
        EVENT_HEX,
        EVENT_EARTH_SHIELD,
        EVENT_EARTH_SHOCK,
    };

    public:
        mob_toc_shaman() : CreatureScript("mob_toc_shaman") { }

        struct mob_toc_shamanAI : public boss_faction_championsAI
        {
            mob_toc_shamanAI(Creature* creature) : boss_faction_championsAI(creature, AI_HEALER) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_HEALING_WAVE, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_RIPTIDE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SPIRIT_CLEANSE, urand(15*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BLOODLUST_HEROISM, 20*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_HEX, urand(5*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_EARTH_SHIELD, 1*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_EARTH_SHOCK, urand(5*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                SetEquipmentSlots(false, 49992, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_HEALING_WAVE:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_HEALING_WAVE);
                            events.ScheduleEvent(EVENT_HEALING_WAVE, urand(3*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                            return;
                        case EVENT_RIPTIDE:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_RIPTIDE);
                            events.ScheduleEvent(EVENT_RIPTIDE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_SPIRIT_CLEANSE:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_SPIRIT_CLEANSE);
                            events.ScheduleEvent(EVENT_SPIRIT_CLEANSE, urand(15*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_BLOODLUST_HEROISM:
                            if (me->getFaction()) //Am i alliance?
                            {
                                if (!me->HasAura(AURA_EXHAUSTION))
                                    DoCastAOE(SPELL_HEROISM);
                            }
                            else
                            {
                                if (!me->HasAura(AURA_SATED))
                                    DoCastAOE(SPELL_BLOODLUST);
                            }
                            events.ScheduleEvent(EVENT_BLOODLUST_HEROISM, 300*IN_MILLISECONDS);
                            return;
                        case EVENT_HEX:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 20.f))
                                DoCast(target, SPELL_HEX);
                            events.ScheduleEvent(EVENT_HEX, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_EARTH_SHIELD:
                            if (Creature* target = SelectRandomFriendlyMissingBuff(SPELL_EARTH_SHIELD))
                                DoCast(target, SPELL_EARTH_SHIELD);
                            events.ScheduleEvent(EVENT_EARTH_SHIELD, urand(35*IN_MILLISECONDS, 60*IN_MILLISECONDS));
                            return;
                        case EVENT_EARTH_SHOCK:
                            if (Unit* target = SelectEnemyCaster(true))
                                DoCast(target, SPELL_EARTH_SHOCK);
                            events.ScheduleEvent(EVENT_EARTH_SHOCK, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_shamanAI (creature);
        }
};

enum ePaladinSpells
{
    SPELL_HAND_OF_FREEDOM     = 68757, //25 sec cd
    SPELL_DIVINE_SHIELD       = 66010, //5 min cd
    SPELL_CLEANSE             = 66116,
    SPELL_FLASH_OF_LIGHT      = 66113,
    SPELL_HOLY_LIGHT          = 66112,
    SPELL_HOLY_SHOCK          = 66114,
    SPELL_HAND_OF_PROTECTION  = 66009,
    SPELL_HAMMER_OF_JUSTICE   = 66613,
    SPELL_FORBEARANCE         = 25771,
};

class mob_toc_paladin : public CreatureScript
{
    enum
    {
        EVENT_HAND_OF_FREEDOM = 1,
        EVENT_DIVINE_SHIELD,
        EVENT_CLEANSE,
        EVENT_FLASH_OF_LIGHT,
        EVENT_HOLY_LIGHT,
        EVENT_HOLY_SHOCK,
        EVENT_HAND_OF_PROTECTION,
        EVENT_HAMMER_OF_JUSTICE,
    };

    public:
        mob_toc_paladin() : CreatureScript("mob_toc_paladin") { }

        struct mob_toc_paladinAI : public boss_faction_championsAI
        {
            mob_toc_paladinAI(Creature* creature) : boss_faction_championsAI(creature, AI_HEALER) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_HAND_OF_FREEDOM, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DIVINE_SHIELD, 20*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_CLEANSE, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FLASH_OF_LIGHT, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HOLY_LIGHT, urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HOLY_SHOCK, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, urand(30*IN_MILLISECONDS, 60*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HAMMER_OF_JUSTICE, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                SetEquipmentSlots(false, 50771, 47079, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_HAND_OF_FREEDOM:
                            if (Unit* target = SelectRandomFriendlyMissingBuff(SPELL_HAND_OF_FREEDOM))
                                DoCast(target, SPELL_HAND_OF_FREEDOM);
                            events.ScheduleEvent(EVENT_HAND_OF_FREEDOM, urand(15*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_DIVINE_SHIELD:
                            if (HealthBelowPct(30) && !me->HasAura(SPELL_FORBEARANCE))
                            {
                                DoCast(me, SPELL_DIVINE_SHIELD);
                                events.ScheduleEvent(EVENT_DIVINE_SHIELD, 300*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_DIVINE_SHIELD, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_CLEANSE:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(me, SPELL_CLEANSE);
                            events.ScheduleEvent(EVENT_CLEANSE, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_FLASH_OF_LIGHT:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_FLASH_OF_LIGHT);
                            events.ScheduleEvent(EVENT_FLASH_OF_LIGHT, urand(3*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                            return;
                        case EVENT_HOLY_LIGHT:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_HOLY_LIGHT);
                            events.ScheduleEvent(EVENT_HOLY_LIGHT, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_HOLY_SHOCK:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_HOLY_SHOCK);
                            events.ScheduleEvent(EVENT_HOLY_SHOCK, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_HAND_OF_PROTECTION:
                            if (Unit* target = DoSelectLowestHpFriendly(30.0f))
                            {
                                if (!target->HasAura(SPELL_FORBEARANCE))
                                {
                                    DoCast(target, SPELL_HAND_OF_PROTECTION);
                                    events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 300*IN_MILLISECONDS);
                                }
                                else
                                    events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 10*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_HAMMER_OF_JUSTICE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 15.f))
                                DoCast(target, SPELL_HAMMER_OF_JUSTICE);
                            events.ScheduleEvent(EVENT_HAMMER_OF_JUSTICE, 40*IN_MILLISECONDS);
                            return;
                    }
                }
            }

            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_paladinAI (creature);
        }
};

enum ePriestSpells
{
    SPELL_RENEW             = 66177,
    SPELL_SHIELD            = 66099,
    SPELL_FLASH_HEAL        = 66104,
    SPELL_DISPEL            = 65546,
    SPELL_PSYCHIC_SCREAM    = 65543,
    SPELL_MANA_BURN         = 66100,
    SPELL_PENANCE           = 66097,
};

class mob_toc_priest : public CreatureScript
{
    enum
    {
        EVENT_RENEW = 1,
        EVENT_SHIELD,
        EVENT_FLASH_HEAL,
        EVENT_DISPEL,
        EVENT_PSYCHIC_SCREAM,
        EVENT_MANA_BURN,
        EVENT_PENANCE,
    };

    public:
        mob_toc_priest() : CreatureScript("mob_toc_priest") { }

        struct mob_toc_priestAI : public boss_faction_championsAI
        {
            mob_toc_priestAI(Creature* creature) : boss_faction_championsAI(creature, AI_HEALER) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_RENEW, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHIELD, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FLASH_HEAL, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DISPEL, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_MANA_BURN, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_PENANCE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                SetEquipmentSlots(false, 49992, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_RENEW:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_RENEW);
                            events.ScheduleEvent(EVENT_RENEW, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_SHIELD:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_SHIELD);
                            events.ScheduleEvent(EVENT_SHIELD, urand(15*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_FLASH_HEAL:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_FLASH_HEAL);
                            events.ScheduleEvent(EVENT_FLASH_HEAL, urand(3*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                            return;
                        case EVENT_DISPEL:
                            if (Unit* target = urand(0, 1) ? SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f) : DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_DISPEL);
                            events.ScheduleEvent(EVENT_DISPEL, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_PSYCHIC_SCREAM:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_PSYCHIC_SCREAM);
                            events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_MANA_BURN:
                            if (Unit* target = SelectEnemyCaster(false))
                                DoCast(target, SPELL_MANA_BURN);
                            events.ScheduleEvent(EVENT_MANA_BURN, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_PENANCE:
                            if (Unit* target = DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_PENANCE);
                            events.ScheduleEvent(EVENT_PENANCE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_priestAI (creature);
        }
};

/********************************************************************
                            RANGED
********************************************************************/
enum eShadowPriestSpells
{
    SPELL_SILENCE           = 65542,
    SPELL_VAMPIRIC_TOUCH    = 65490,
    SPELL_SW_PAIN           = 65541,
    SPELL_MIND_FLAY         = 65488,
    SPELL_MIND_BLAST        = 65492,
    SPELL_HORROR            = 65545,
    SPELL_DISPERSION        = 65544,
    SPELL_SHADOWFORM        = 16592,
};

class mob_toc_shadow_priest : public CreatureScript
{
    enum
    {
        EVENT_SILENCE = 1,
        EVENT_VAMPIRIC_TOUCH,
        EVENT_SW_PAIN,
        EVENT_MIND_BLAST,
        EVENT_HORROR,
        EVENT_DISPERSION,
        EVENT_DISPEL,
        EVENT_PSYCHIC_SCREAM,
    };

    public:
        mob_toc_shadow_priest() : CreatureScript("mob_toc_shadow_priest") { }

        struct mob_toc_shadow_priestAI : public boss_faction_championsAI
        {
            mob_toc_shadow_priestAI(Creature* creature) : boss_faction_championsAI(creature, AI_RANGED) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_SILENCE, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_VAMPIRIC_TOUCH, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SW_PAIN, urand(3*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_MIND_BLAST, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HORROR, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DISPERSION, urand(20*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DISPEL, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                SetEquipmentSlots(false, 50040, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                DoCast(me, SPELL_SHADOWFORM);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_SILENCE:
                            if (Unit* target = SelectEnemyCaster(true))
                                DoCast(target, SPELL_SILENCE);
                            events.ScheduleEvent(EVENT_SILENCE, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_VAMPIRIC_TOUCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f))
                                DoCast(target, SPELL_VAMPIRIC_TOUCH);
                            events.ScheduleEvent(EVENT_VAMPIRIC_TOUCH, urand(10*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_SW_PAIN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.f))
                                DoCast(target, SPELL_SW_PAIN);
                            events.ScheduleEvent(EVENT_SW_PAIN, urand(10*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_MIND_BLAST:
                            DoCastVictim(SPELL_MIND_BLAST);
                            events.ScheduleEvent(EVENT_MIND_BLAST, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_HORROR:
                            DoCastVictim(SPELL_HORROR);
                            events.ScheduleEvent(EVENT_HORROR, urand(15*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_DISPERSION:
                            if (HealthBelowPct(40))
                            {
                                DoCast(me, SPELL_DISPERSION);
                                events.ScheduleEvent(EVENT_DISPERSION, 180*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_DISPERSION, 20*IN_MILLISECONDS);
                            return;
                        case EVENT_DISPEL:
                            if (Unit* target = urand(0, 1) ? SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f) : DoSelectLowestHpFriendly(40.0f))
                                DoCast(target, SPELL_DISPEL);
                            events.ScheduleEvent(EVENT_DISPEL, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_PSYCHIC_SCREAM:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_PSYCHIC_SCREAM);
                            events.ScheduleEvent(EVENT_PSYCHIC_SCREAM, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                    }
                }
                DoSpellAttackIfReady(SPELL_MIND_FLAY);
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_shadow_priestAI (creature);
        }
};

enum WarlockSpells
{
    SPELL_HELLFIRE                   = 65816,
    SPELL_CORRUPTION                 = 65810,
    SPELL_CURSE_OF_AGONY             = 65814,
    SPELL_CURSE_OF_EXHAUSTION        = 65815,
    SPELL_FEAR                       = 65809, // 8s
    SPELL_SEARING_PAIN               = 65819,
    SPELL_SHADOW_BOLT                = 65821,
    SPELL_UNSTABLE_AFFLICTION        = 65812, // 15s
    SPELL_UNSTABLE_AFFLICTION_DISPEL = 65813,
    SPELL_SUMMON_FELHUNTER           = 67514,
};

class mob_toc_warlock : public CreatureScript
{
    enum
    {
        EVENT_HELLFIRE = 1,
        EVENT_CORRUPTION,
        EVENT_CURSE_OF_AGONY,
        EVENT_CURSE_OF_EXHAUSTION,
        EVENT_FEAR,
        EVENT_SEARING_PAIN,
        EVENT_UNSTABLE_AFFLICTION,
    };

    public:
        mob_toc_warlock() : CreatureScript("mob_toc_warlock") { }

        struct mob_toc_warlockAI : public boss_faction_championsAI
        {
            mob_toc_warlockAI(Creature* creature) : boss_faction_championsAI(creature, AI_RANGED), Summons(me) {}

            SummonList Summons;

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_HELLFIRE, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CORRUPTION, urand(2*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CURSE_OF_AGONY, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CURSE_OF_EXHAUSTION, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FEAR, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SEARING_PAIN, urand(5*IN_MILLISECONDS, 12*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_UNSTABLE_AFFLICTION, urand(7*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                SetEquipmentSlots(false, 49992, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* who)
            {
                boss_faction_championsAI::EnterCombat(who);
                DoCast(SPELL_SUMMON_FELHUNTER);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_HELLFIRE:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_HELLFIRE);
                            events.ScheduleEvent(EVENT_HELLFIRE, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_CORRUPTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f))
                                DoCast(target, SPELL_CORRUPTION);
                            events.ScheduleEvent(EVENT_CORRUPTION, urand(15*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_CURSE_OF_AGONY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f))
                                DoCast(target, SPELL_CURSE_OF_AGONY);
                            events.ScheduleEvent(EVENT_CURSE_OF_AGONY, urand(20*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_CURSE_OF_EXHAUSTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f))
                                DoCast(target, SPELL_CURSE_OF_EXHAUSTION);
                            events.ScheduleEvent(EVENT_CURSE_OF_EXHAUSTION, urand(20*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_FEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 20.f))
                                DoCast(target, SPELL_FEAR);
                            events.ScheduleEvent(EVENT_FEAR, urand(5*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_SEARING_PAIN:
                            DoCastVictim(SPELL_SEARING_PAIN);
                            events.ScheduleEvent(EVENT_SEARING_PAIN, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_UNSTABLE_AFFLICTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f))
                                DoCast(target, SPELL_UNSTABLE_AFFLICTION);
                            events.ScheduleEvent(EVENT_UNSTABLE_AFFLICTION, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                    }
                }
                DoSpellAttackIfReady(SPELL_SHADOW_BOLT);
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_warlockAI (creature);
        }
};

enum eMageSpells
{
    SPELL_ARCANE_BARRAGE    = 65799, //3s
    SPELL_ARCANE_BLAST      = 65791,
    SPELL_ARCANE_EXPLOSION  = 65800,
    SPELL_BLINK             = 65793, //15s
    SPELL_COUNTERSPELL      = 65790, //24s
    SPELL_FROST_NOVA        = 65792, //25s
    SPELL_FROSTBOLT         = 65807,
    SPELL_ICE_BLOCK         = 65802, //5min
    SPELL_POLYMORPH         = 65801, //15s
};

class mob_toc_mage : public CreatureScript
{
    enum
    {
        EVENT_ARCANE_BARRAGE = 1,
        EVENT_ARCANE_BLAST,
        EVENT_ARCANE_EXPLOSION,
        EVENT_BLINK,
        EVENT_COUNTERSPELL,
        EVENT_FROST_NOVA,
        EVENT_ICE_BLOCK,
        EVENT_POLYMORPH,
    };

    public:
        mob_toc_mage() : CreatureScript("mob_toc_mage") { }

        struct mob_toc_mageAI : public boss_faction_championsAI
        {
            mob_toc_mageAI(Creature* creature) : boss_faction_championsAI(creature, AI_RANGED) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_ARCANE_BARRAGE, urand(1*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ARCANE_BLAST, urand(3*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BLINK, urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_COUNTERSPELL, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FROST_NOVA, urand(5*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ICE_BLOCK, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_POLYMORPH, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                SetEquipmentSlots(false, 47524, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_ARCANE_BARRAGE:
                            DoCastVictim(SPELL_ARCANE_BARRAGE);
                            events.ScheduleEvent(EVENT_ARCANE_BARRAGE, urand(5*IN_MILLISECONDS, 7*IN_MILLISECONDS));
                            return;
                        case EVENT_ARCANE_BLAST:
                            DoCastVictim(SPELL_ARCANE_BLAST);
                            events.ScheduleEvent(EVENT_ARCANE_BLAST, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_ARCANE_EXPLOSION:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_ARCANE_EXPLOSION);
                            events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_BLINK:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCast(SPELL_BLINK);
                            events.ScheduleEvent(EVENT_BLINK, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_COUNTERSPELL:
                            if (Unit* target = SelectEnemyCaster(true))
                                DoCast(target, SPELL_COUNTERSPELL);
                            events.ScheduleEvent(EVENT_COUNTERSPELL, 24*IN_MILLISECONDS);
                            return;
                        case EVENT_FROST_NOVA:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_FROST_NOVA);
                            events.ScheduleEvent(EVENT_FROST_NOVA, 25*IN_MILLISECONDS);
                            return;
                        case EVENT_ICE_BLOCK:
                            if (HealthBelowPct(30))
                            {
                                DoCast(SPELL_ICE_BLOCK);
                                events.ScheduleEvent(EVENT_ICE_BLOCK, 300*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_ICE_BLOCK, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_POLYMORPH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 30.f))
                                DoCastAOE(SPELL_POLYMORPH);
                            events.ScheduleEvent(EVENT_POLYMORPH, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                    }
                }
                DoSpellAttackIfReady(SPELL_FROSTBOLT);
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_mageAI (creature);
        }
};

enum eHunterSpells
{
    SPELL_AIMED_SHOT        = 65883,
    SPELL_DETERRENCE        = 65871, //90s
    SPELL_DISENGAGE         = 65869, //30s
    SPELL_EXPLOSIVE_SHOT    = 65866,
    SPELL_FROST_TRAP        = 65880, //30s
    SPELL_SHOOT             = 65868, //1.7s
    SPELL_STEADY_SHOT       = 65867, //3s
    SPELL_WING_CLIP         = 66207, //6s
    SPELL_WYVERN_STING      = 65877, //60s
    SPELL_CALL_PET          = 67777,
};

class mob_toc_hunter : public CreatureScript
{
    enum
    {
        EVENT_AIMED_SHOT = 1,
        EVENT_DETERRENCE,
        EVENT_DISENGAGE,
        EVENT_EXPLOSIVE_SHOT,
        EVENT_FROST_TRAP,
        EVENT_STEADY_SHOT,
        EVENT_WING_CLIP,
        EVENT_WYVERN_STING,
    };

    public:
        mob_toc_hunter() : CreatureScript("mob_toc_hunter") { }

        struct mob_toc_hunterAI : public boss_faction_championsAI
        {
            mob_toc_hunterAI(Creature* creature) : boss_faction_championsAI(creature, AI_RANGED), Summons(me) {}

            SummonList Summons;

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_AIMED_SHOT, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DETERRENCE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DISENGAGE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_EXPLOSIVE_SHOT, urand(3*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FROST_TRAP, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_STEADY_SHOT, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_WING_CLIP, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_WYVERN_STING, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                SetEquipmentSlots(false, 47156, EQUIP_NO_CHANGE, 48711);
            }

            void EnterCombat(Unit* who)
            {
                boss_faction_championsAI::EnterCombat(who);
                DoCast(SPELL_CALL_PET);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_AIMED_SHOT:
                            DoCastVictim(SPELL_AIMED_SHOT);
                            events.ScheduleEvent(EVENT_AIMED_SHOT, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_DETERRENCE:
                            if (HealthBelowPct(30))
                            {
                                DoCast(SPELL_DETERRENCE);
                                events.ScheduleEvent(EVENT_DETERRENCE, 150*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_DETERRENCE, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_DISENGAGE:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCast(SPELL_DISENGAGE);
                            events.ScheduleEvent(EVENT_DISENGAGE, 30*IN_MILLISECONDS);
                            return;
                        case EVENT_EXPLOSIVE_SHOT:
                            DoCastVictim(SPELL_EXPLOSIVE_SHOT);
                            events.ScheduleEvent(EVENT_EXPLOSIVE_SHOT, urand(6*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                            return;
                        case EVENT_FROST_TRAP:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_FROST_TRAP);
                            events.ScheduleEvent(EVENT_FROST_TRAP, 30*IN_MILLISECONDS);
                            return;
                        case EVENT_STEADY_SHOT:
                            DoCastVictim(SPELL_STEADY_SHOT);
                            events.ScheduleEvent(EVENT_STEADY_SHOT, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_WING_CLIP:
                            if (me->GetDistance2d(me->getVictim()) < 6.0f)
                                DoCastVictim(SPELL_WING_CLIP);
                            events.ScheduleEvent(EVENT_WING_CLIP, urand(15*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_WYVERN_STING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 35.f))
                                DoCast(target, SPELL_WYVERN_STING);
                            events.ScheduleEvent(EVENT_WYVERN_STING, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                    }
                }
                DoSpellAttackIfReady(SPELL_SHOOT);
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_hunterAI (creature);
        }
};

enum eBoomkinSpells
{
    SPELL_CYCLONE           = 65859, //6s
    SPELL_ENTANGLING_ROOTS  = 65857, //10s
    SPELL_FAERIE_FIRE       = 65863,
    SPELL_FORCE_OF_NATURE   = 65861, //180s
    SPELL_INSECT_SWARM      = 65855,
    SPELL_MOONFIRE          = 65856, //5s
    SPELL_STARFIRE          = 65854,
    SPELL_WRATH             = 65862,
};

class mob_toc_boomkin : public CreatureScript
{
    enum
    {
        EVENT_CYCLONE = 1,
        EVENT_ENTANGLING_ROOTS,
        EVENT_FAERIE_FIRE,
        EVENT_FORCE_OF_NATURE,
        EVENT_INSECT_SWARM,
        EVENT_MOONFIRE,
        EVENT_STARFIRE,
        EVENT_BARKSKIN,
    };

    public:
        mob_toc_boomkin() : CreatureScript("mob_toc_boomkin") { }

        struct mob_toc_boomkinAI : public boss_faction_championsAI
        {
            mob_toc_boomkinAI(Creature* creature) : boss_faction_championsAI(creature, AI_RANGED) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_CYCLONE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ENTANGLING_ROOTS, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FAERIE_FIRE, urand(2*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FORCE_OF_NATURE, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_INSECT_SWARM, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_MOONFIRE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_STARFIRE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BARKSKIN, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));

                SetEquipmentSlots(false, 50966, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_CYCLONE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 20.f))
                                DoCast(target, SPELL_CYCLONE);
                            events.ScheduleEvent(EVENT_CYCLONE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_ENTANGLING_ROOTS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.f))
                                DoCast(SPELL_ENTANGLING_ROOTS);
                            events.ScheduleEvent(EVENT_ENTANGLING_ROOTS, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_FAERIE_FIRE:
                            DoCastVictim(SPELL_FAERIE_FIRE);
                            events.ScheduleEvent(EVENT_FAERIE_FIRE, urand(30*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                            return;
                        case EVENT_FORCE_OF_NATURE:
                            DoCastVictim(SPELL_FORCE_OF_NATURE);
                            events.ScheduleEvent(EVENT_FORCE_OF_NATURE, 180*IN_MILLISECONDS);
                            return;
                        case EVENT_INSECT_SWARM:
                            DoCastVictim(SPELL_INSECT_SWARM);
                            events.ScheduleEvent(EVENT_INSECT_SWARM, urand(15*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_MOONFIRE:
                            DoCastVictim(SPELL_MOONFIRE);
                            events.ScheduleEvent(EVENT_MOONFIRE, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_STARFIRE:
                            DoCastVictim(SPELL_STARFIRE);
                            events.ScheduleEvent(EVENT_STARFIRE, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_BARKSKIN:
                            if (HealthBelowPct(30))
                            {
                                DoCast(me, SPELL_BARKSKIN);
                                events.ScheduleEvent(EVENT_BARKSKIN, 60*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_BARKSKIN, 10*IN_MILLISECONDS);
                            return;
                    }
                }
                DoSpellAttackIfReady(SPELL_WRATH);
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_boomkinAI (creature);
        }
};

/********************************************************************
                            MELEE
********************************************************************/
enum eWarriorSpells
{
    SPELL_BLADESTORM            = 65947,
    SPELL_INTIMIDATING_SHOUT    = 65930,
    SPELL_MORTAL_STRIKE         = 65926,
    SPELL_CHARGE                = 68764,
    SPELL_DISARM                = 65935,
    SPELL_OVERPOWER             = 65924,
    SPELL_SUNDER_ARMOR          = 65936,
    SPELL_SHATTERING_THROW      = 65940,
    SPELL_RETALIATION           = 65932,
};

class mob_toc_warrior : public CreatureScript
{
    enum
    {
        EVENT_BLADESTORM = 1,
        EVENT_INTIMIDATING_SHOUT,
        EVENT_MORTAL_STRIKE,
        EVENT_CHARGE,
        EVENT_DISARM,
        EVENT_OVERPOWER,
        EVENT_SUNDER_ARMOR,
        EVENT_SHATTERING_THROW,
        EVENT_RETALIATION,
    };

    public:
        mob_toc_warrior() : CreatureScript("mob_toc_warrior") { }

        struct mob_toc_warriorAI : public boss_faction_championsAI
        {
            mob_toc_warriorAI(Creature* creature) : boss_faction_championsAI(creature, AI_MELEE) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_BLADESTORM, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_INTIMIDATING_SHOUT, urand(20*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(5*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CHARGE, 1*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_DISARM, urand(5*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_OVERPOWER, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SUNDER_ARMOR, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHATTERING_THROW, urand(20*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_RETALIATION, urand(5*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                SetEquipmentSlots(false, 47427, 46964, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_BLADESTORM:
                            DoCastVictim(SPELL_BLADESTORM);
                            events.ScheduleEvent(EVENT_BLADESTORM, 150*IN_MILLISECONDS);
                            return;
                        case EVENT_INTIMIDATING_SHOUT:
                            DoCastAOE(SPELL_INTIMIDATING_SHOUT);
                            events.ScheduleEvent(EVENT_INTIMIDATING_SHOUT, 120*IN_MILLISECONDS);
                            return;
                        case EVENT_MORTAL_STRIKE:
                            DoCastVictim(SPELL_MORTAL_STRIKE);
                            events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_CHARGE:
                            DoCastVictim(SPELL_CHARGE);
                            events.ScheduleEvent(EVENT_CHARGE, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_DISARM:
                            DoCastVictim(SPELL_DISARM);
                            events.ScheduleEvent(EVENT_DISARM, urand(15*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                            return;
                        case EVENT_OVERPOWER:
                            DoCastVictim(SPELL_OVERPOWER);
                            events.ScheduleEvent(EVENT_OVERPOWER, urand(20*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                            return;
                        case EVENT_SUNDER_ARMOR:
                            DoCastVictim(SPELL_SUNDER_ARMOR);
                            events.ScheduleEvent(EVENT_SUNDER_ARMOR, urand(2*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                            return;
                        case EVENT_SHATTERING_THROW:
                            if (me->getVictim()->HasAuraWithMechanic(1<<MECHANIC_IMMUNE_SHIELD))
                            {
                                DoCastVictim(SPELL_SHATTERING_THROW);
                                events.ScheduleEvent(EVENT_SHATTERING_THROW, 300*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_SHATTERING_THROW, 3*IN_MILLISECONDS);
                            return;
                        case EVENT_RETALIATION:
                            if (HealthBelowPct(50))
                            {
                                DoCast(SPELL_RETALIATION);
                                events.ScheduleEvent(EVENT_RETALIATION, 300*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_RETALIATION, 10*IN_MILLISECONDS);
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_warriorAI (creature);
        }
};

enum eDeathKnightSpells
{
    SPELL_CHAINS_OF_ICE       = 66020, //8sec
    SPELL_DEATH_COIL          = 66019, //5sec
    SPELL_DEATH_GRIP          = 66017, //35sec
    SPELL_FROST_STRIKE        = 66047, //6sec
    SPELL_ICEBOUND_FORTITUDE  = 66023, //1min
    SPELL_ICY_TOUCH           = 66021, //8sec
    SPELL_STRANGULATE         = 66018, //2min
};

class mob_toc_dk : public CreatureScript
{
    enum
    {
        EVENT_CHAINS_OF_ICE = 1,
        EVENT_DEATH_COIL,
        EVENT_DEATH_GRIP,
        EVENT_FROST_STRIKE,
        EVENT_ICEBOUND_FORTITUDE,
        EVENT_ICY_TOUCH,
        EVENT_STRANGULATE,
    };

    public:
        mob_toc_dk() : CreatureScript("mob_toc_dk") { }

        struct mob_toc_dkAI : public boss_faction_championsAI
        {
            mob_toc_dkAI(Creature* creature) : boss_faction_championsAI(creature, AI_MELEE) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_CHAINS_OF_ICE, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DEATH_COIL, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DEATH_GRIP, urand(15*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_FROST_STRIKE, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ICEBOUND_FORTITUDE, urand(25*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ICY_TOUCH, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_STRANGULATE, urand(5*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                SetEquipmentSlots(false, 47518, 51021, EQUIP_NO_CHANGE);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_CHAINS_OF_ICE:
                            DoCastVictim(SPELL_CHAINS_OF_ICE);
                            events.ScheduleEvent(EVENT_CHAINS_OF_ICE, urand(15*IN_MILLISECONDS, 25*IN_MILLISECONDS));
                            return;
                        case EVENT_DEATH_COIL:
                            DoCastVictim(SPELL_DEATH_COIL);
                            events.ScheduleEvent(EVENT_DEATH_COIL, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_DEATH_GRIP:
                            if (me->IsInRange(me->getVictim(), 5.0f, 30.0f, false))
                            {
                                DoCast(me->getVictim(), SPELL_DEATH_GRIP);
                                events.ScheduleEvent(EVENT_DEATH_GRIP, 35*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_DEATH_GRIP, 3*IN_MILLISECONDS);
                            return;
                        case EVENT_FROST_STRIKE:
                            DoCastVictim(SPELL_FROST_STRIKE);
                            events.ScheduleEvent(EVENT_FROST_STRIKE, urand(6*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                            return;
                        case EVENT_ICEBOUND_FORTITUDE:
                            if (HealthBelowPct(50))
                            {
                                DoCast(SPELL_ICEBOUND_FORTITUDE);
                                events.ScheduleEvent(EVENT_ICEBOUND_FORTITUDE, 60*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_ICEBOUND_FORTITUDE, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_ICY_TOUCH:
                            DoCastVictim(SPELL_ICY_TOUCH);
                            events.ScheduleEvent(EVENT_ICY_TOUCH, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_STRANGULATE:
                            if (Unit* target = SelectEnemyCaster(false))
                            {
                                DoCast(target, SPELL_STRANGULATE);
                                events.ScheduleEvent(EVENT_STRANGULATE, 120*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_STRANGULATE, 10*IN_MILLISECONDS);
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_dkAI (creature);
        }

};

enum eRogueSpells
{
    SPELL_FAN_OF_KNIVES         = 65955, //2sec
    SPELL_BLIND                 = 65960, //2min
    SPELL_CLOAK                 = 65961, //90sec
    SPELL_BLADE_FLURRY          = 65956, //2min
    SPELL_SHADOWSTEP            = 66178, //30sec
    SPELL_HEMORRHAGE            = 65954,
    SPELL_EVISCERATE            = 65957,
    SPELL_WOUND_POISON          = 65962,
};

class mob_toc_rogue : public CreatureScript
{
    enum
    {
        EVENT_FAN_OF_KNIVES = 1,
        EVENT_BLIND,
        EVENT_CLOAK,
        EVENT_BLADE_FLURRY,
        EVENT_SHADOWSTEP,
        EVENT_HEMORRHAGE,
        EVENT_EVISCERATE,
        EVENT_WOUND_POISON,
    };

    public:
        mob_toc_rogue() : CreatureScript("mob_toc_rogue") { }

        struct mob_toc_rogueAI : public boss_faction_championsAI
        {
            mob_toc_rogueAI(Creature* creature) : boss_faction_championsAI(creature, AI_MELEE) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_FAN_OF_KNIVES, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BLIND, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CLOAK, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BLADE_FLURRY, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHADOWSTEP, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HEMORRHAGE, urand(3*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_EVISCERATE, urand(20*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_WOUND_POISON, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                SetEquipmentSlots(false, 47422, 49982, EQUIP_NO_CHANGE);
                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_FAN_OF_KNIVES:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCastAOE(SPELL_FAN_OF_KNIVES);
                            events.ScheduleEvent(EVENT_FAN_OF_KNIVES, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_BLIND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 20.f))
                                DoCast(target, SPELL_BLIND);
                            events.ScheduleEvent(EVENT_BLIND, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                        case EVENT_CLOAK:
                            if (HealthBelowPct(50))
                            {
                                DoCast(SPELL_CLOAK);
                                events.ScheduleEvent(EVENT_CLOAK, 90*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_CLOAK, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_BLADE_FLURRY:
                            if (EnemiesInRange(10.0f) >= 2)
                            {
                                DoCast(SPELL_BLADE_FLURRY);
                                events.ScheduleEvent(EVENT_BLADE_FLURRY, 120*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_BLADE_FLURRY, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_SHADOWSTEP:
                            if (me->IsInRange(me->getVictim(), 10.0f, 40.0f, false))
                            {
                                DoCast(me->getVictim(), SPELL_SHADOWSTEP);
                                events.ScheduleEvent(EVENT_SHADOWSTEP, 30*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_SHADOWSTEP, 5*IN_MILLISECONDS);
                            return;
                        case EVENT_HEMORRHAGE:
                            DoCastVictim(SPELL_HEMORRHAGE);
                            events.ScheduleEvent(EVENT_HEMORRHAGE, urand(3*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                            return;
                        case EVENT_EVISCERATE:
                            DoCastVictim(SPELL_EVISCERATE);
                            events.ScheduleEvent(EVENT_EVISCERATE, urand(30*IN_MILLISECONDS, 40*IN_MILLISECONDS));
                            return;
                        case EVENT_WOUND_POISON:
                            DoCastVictim(SPELL_WOUND_POISON);
                            events.ScheduleEvent(EVENT_WOUND_POISON, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_rogueAI (creature);
        }
};

enum eEnhShamanSpells
{
    SPELL_LAVA_LASH         = 65974,
    SPELL_STORMSTRIKE       = 65970,
    SPELL_WINDFURY          = 65976,
};

class mob_toc_enh_shaman : public CreatureScript
{
    enum
    {
        EVENT_EARTH_SHOCK = 1,
        EVENT_LAVA_LASH,
        EVENT_STORMSTRIKE,
        EVENT_BLOODLUST_HEROISM,
        EVENT_DEPLOY_TOTEM,
        EVENT_WINDFURY,
    };

    public:
        mob_toc_enh_shaman() : CreatureScript("mob_toc_enh_shaman") { }

        struct mob_toc_enh_shamanAI : public boss_faction_championsAI
        {
            mob_toc_enh_shamanAI(Creature* creature) : boss_faction_championsAI(creature, AI_MELEE), Summons(me) {}

            SummonList Summons;
            uint8  m_uiTotemCount;
            float  m_fTotemOldCenterX, m_fTotemOldCenterY;

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_EARTH_SHOCK, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_LAVA_LASH, urand(3*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_STORMSTRIKE, urand(2*IN_MILLISECONDS, 5*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_BLOODLUST_HEROISM, 20*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_DEPLOY_TOTEM, 1*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_WINDFURY, urand(20*IN_MILLISECONDS, 50*IN_MILLISECONDS));

                m_uiTotemCount = 0;
                m_fTotemOldCenterX = me->GetPositionX();
                m_fTotemOldCenterY = me->GetPositionY();
                SetEquipmentSlots(false, 51803, 48013, EQUIP_NO_CHANGE);
                Summons.DespawnAll();
            }

            void JustSummoned(Creature* summoned)
            {
                Summons.Summon(summoned);
            }

            void SummonedCreatureDespawn(Creature* /*pSummoned*/)
            {
                --m_uiTotemCount;
            }

            void DeployTotem()
            {
                m_uiTotemCount = 4;
                m_fTotemOldCenterX = me->GetPositionX();
                m_fTotemOldCenterY = me->GetPositionY();
                /*
                -Windfury (16% melee haste)
                -Grounding (redirects one harmful magic spell to the totem)

                -Healing Stream (unable to find amount of healing in our logs)

                -Tremor (prevents fear effects)
                -Strength of Earth (155 strength and agil for the opposing team)

                -Searing (average ~3500 damage on a random target every ~3.5 seconds)
                */
            }

            void JustDied(Unit* killer)
            {
                boss_faction_championsAI::JustDied(killer);
                Summons.DespawnAll();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_EARTH_SHOCK:
                            if (Unit* target = SelectEnemyCaster(true))
                                DoCast(target, SPELL_EARTH_SHOCK);
                            events.ScheduleEvent(EVENT_EARTH_SHOCK, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_LAVA_LASH:
                            DoCastVictim(SPELL_LAVA_LASH);
                            events.ScheduleEvent(EVENT_LAVA_LASH, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_STORMSTRIKE:
                            DoCastVictim(SPELL_STORMSTRIKE);
                            events.ScheduleEvent(EVENT_STORMSTRIKE, urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                            return;
                        case EVENT_BLOODLUST_HEROISM:
                            if (me->getFaction()) //Am i alliance?
                            {
                                if (!me->HasAura(AURA_EXHAUSTION))
                                    DoCastAOE(SPELL_HEROISM);
                            }
                            else
                            {
                                if (!me->HasAura(AURA_SATED))
                                    DoCastAOE(SPELL_BLOODLUST);
                            }
                            events.ScheduleEvent(EVENT_BLOODLUST_HEROISM, 300*IN_MILLISECONDS);
                            return;
                        case EVENT_DEPLOY_TOTEM:
                            if (m_uiTotemCount < 4 || me->GetDistance2d(m_fTotemOldCenterX, m_fTotemOldCenterY) > 20.0f)
                                DeployTotem();
                            events.ScheduleEvent(EVENT_DEPLOY_TOTEM, 1*IN_MILLISECONDS);
                            return;
                        case EVENT_WINDFURY:
                            DoCastVictim(SPELL_WINDFURY);
                            events.ScheduleEvent(EVENT_WINDFURY, urand(20*IN_MILLISECONDS, 60*IN_MILLISECONDS));
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_enh_shamanAI (creature);
        }
};

enum eRetroPaladinSpells
{
    SPELL_AVENGING_WRATH        = 66011, //3min cd
    SPELL_CRUSADER_STRIKE       = 66003, //6sec cd
    SPELL_DIVINE_STORM          = 66006, //10sec cd
    SPELL_HAMMER_OF_JUSTICE_RET = 66007, //40sec cd
    SPELL_JUDGEMENT_OF_COMMAND  = 66005, //8sec cd
    SPELL_REPENTANCE            = 66008, //60sec cd
    SPELL_SEAL_OF_COMMAND       = 66004, //no cd
};

class mob_toc_retro_paladin : public CreatureScript
{
    enum
    {
        EVENT_AVENGING_WRATH = 1,
        EVENT_CRUSADER_STRIKE,
        EVENT_DIVINE_STORM,
        EVENT_HAMMER_OF_JUSTICE_RET,
        EVENT_JUDGEMENT_OF_COMMAND,
        EVENT_REPENTANCE,
        EVENT_HAND_OF_PROTECTION,
        EVENT_DIVINE_SHIELD,
    };

    public:
        mob_toc_retro_paladin() : CreatureScript("mob_toc_retro_paladin") { }

        struct mob_toc_retro_paladinAI : public boss_faction_championsAI
        {
            mob_toc_retro_paladinAI(Creature* creature) : boss_faction_championsAI(creature, AI_MELEE) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_AVENGING_WRATH, urand(25*IN_MILLISECONDS, 35*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CRUSADER_STRIKE, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DIVINE_STORM, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HAMMER_OF_JUSTICE_RET, urand(10*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_JUDGEMENT_OF_COMMAND, urand(5*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_REPENTANCE, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DIVINE_SHIELD, urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                SetEquipmentSlots(false, 47519, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void EnterCombat(Unit* who)
            {
                boss_faction_championsAI::EnterCombat(who);
                DoCast(SPELL_SEAL_OF_COMMAND);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_AVENGING_WRATH:
                            DoCast(SPELL_AVENGING_WRATH);
                            events.ScheduleEvent(EVENT_AVENGING_WRATH, 180*IN_MILLISECONDS);
                            return;
                        case EVENT_CRUSADER_STRIKE:
                            DoCastVictim(SPELL_CRUSADER_STRIKE);
                            events.ScheduleEvent(EVENT_CRUSADER_STRIKE, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_DIVINE_STORM:
                            if (EnemiesInRange(10.0f) >= 2)
                                DoCast(SPELL_DIVINE_STORM);
                            events.ScheduleEvent(EVENT_DIVINE_STORM, urand(10*IN_MILLISECONDS, 20*IN_MILLISECONDS));
                            return;
                        case EVENT_HAMMER_OF_JUSTICE_RET:
                            DoCastVictim(SPELL_HAMMER_OF_JUSTICE_RET);
                            events.ScheduleEvent(EVENT_HAMMER_OF_JUSTICE_RET, 40*IN_MILLISECONDS);
                            return;
                        case EVENT_JUDGEMENT_OF_COMMAND:
                            DoCastVictim(SPELL_JUDGEMENT_OF_COMMAND);
                            events.ScheduleEvent(EVENT_JUDGEMENT_OF_COMMAND, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
                            return;
                        case EVENT_REPENTANCE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 20.f))
                                DoCast(target, SPELL_REPENTANCE);
                            events.ScheduleEvent(EVENT_REPENTANCE, 60*IN_MILLISECONDS);
                            return;
                        case EVENT_HAND_OF_PROTECTION:
                            if (Unit* target = DoSelectLowestHpFriendly(30.0f))
                            {
                                if (!target->HasAura(SPELL_FORBEARANCE))
                                {
                                    DoCast(target, SPELL_HAND_OF_PROTECTION);
                                    events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 300*IN_MILLISECONDS);
                                }
                                else
                                    events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 10*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_HAND_OF_PROTECTION, 10*IN_MILLISECONDS);
                            return;
                        case EVENT_DIVINE_SHIELD:
                            if (HealthBelowPct(30) && !me->HasAura(SPELL_FORBEARANCE))
                            {
                                DoCast(me, SPELL_DIVINE_SHIELD);
                                events.ScheduleEvent(EVENT_DIVINE_SHIELD, 300*IN_MILLISECONDS);
                            }
                            else
                                events.ScheduleEvent(EVENT_DIVINE_SHIELD, 10*IN_MILLISECONDS);
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_retro_paladinAI (creature);
        }
};

enum eWarlockPetSpells
{
    SPELL_DEVOUR_MAGIC   = 67518,
    SPELL_SPELL_LOCK     = 67519,
};

class mob_toc_pet_warlock : public CreatureScript
{
    enum
    {
        EVENT_DEVOUR_MAGIC = 1,
        EVENT_SPELL_LOCK,
    };

    public:
        mob_toc_pet_warlock() : CreatureScript("mob_toc_pet_warlock") { }

        struct mob_toc_pet_warlockAI : public boss_faction_championsAI
        {
            mob_toc_pet_warlockAI(Creature* creature) : boss_faction_championsAI(creature, AI_PET) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_DEVOUR_MAGIC, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SPELL_LOCK, urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_DEVOUR_MAGIC:
                            DoCastVictim(SPELL_DEVOUR_MAGIC);
                            events.ScheduleEvent(EVENT_DEVOUR_MAGIC, urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                            return;
                        case EVENT_SPELL_LOCK:
                            DoCast(SPELL_SPELL_LOCK);
                            events.ScheduleEvent(EVENT_SPELL_LOCK, urand(24*IN_MILLISECONDS, 30*IN_MILLISECONDS));
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_pet_warlockAI (creature);
        }
};

enum eHunterPetSpells
{
    SPELL_CLAW  = 67793,
};

class mob_toc_pet_hunter : public CreatureScript
{
    enum
    {
        EVENT_CLAW = 1,
    };

    public:
        mob_toc_pet_hunter() : CreatureScript("mob_toc_pet_hunter") { }

        struct mob_toc_pet_hunterAI : public boss_faction_championsAI
        {
            mob_toc_pet_hunterAI(Creature* creature) : boss_faction_championsAI(creature, AI_PET) {}

            void Reset()
            {
                boss_faction_championsAI::Reset();
                events.ScheduleEvent(EVENT_CLAW, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                boss_faction_championsAI::UpdateAI(diff);

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_CLAW:
                            DoCastVictim(SPELL_CLAW);
                            events.ScheduleEvent(EVENT_CLAW, urand(5*IN_MILLISECONDS, 10*IN_MILLISECONDS));
                            return;
                    }
                }
            }
            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_toc_pet_hunterAI (creature);
        }
};

class spell_faction_champion_warl_unstable_affliction : public SpellScriptLoader
{
    public:
        spell_faction_champion_warl_unstable_affliction() : SpellScriptLoader("spell_faction_champion_warl_unstable_affliction") { }

        class spell_faction_champion_warl_unstable_affliction_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_faction_champion_warl_unstable_affliction_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_UNSTABLE_AFFLICTION_DISPEL))
                    return false;
                return true;
            }

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(dispelInfo->GetDispeller(), SPELL_UNSTABLE_AFFLICTION_DISPEL, true, NULL, GetEffect(EFFECT_0));
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_faction_champion_warl_unstable_affliction_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_faction_champion_warl_unstable_affliction_AuraScript();
        }
};

enum DeathGripPull
{
    SPELL_DEATH_GRIP_PULL = 64431,
};

class spell_faction_champion_death_grip : public SpellScriptLoader
{
    public:
        spell_faction_champion_death_grip() : SpellScriptLoader("spell_faction_champion_death_grip") { }

        class spell_faction_champion_death_grip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_faction_champion_death_grip_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DEATH_GRIP_PULL))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Unit* caster = GetCaster())
                        target->CastSpell(caster, SPELL_DEATH_GRIP_PULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_faction_champion_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_faction_champion_death_grip_SpellScript();
        }
};

void AddSC_boss_faction_champions()
{
    new boss_toc_champion_controller();
    new mob_toc_druid();
    new mob_toc_shaman();
    new mob_toc_paladin();
    new mob_toc_priest();
    new mob_toc_shadow_priest();
    new mob_toc_mage();
    new mob_toc_warlock();
    new mob_toc_hunter();
    new mob_toc_boomkin();
    new mob_toc_warrior();
    new mob_toc_dk();
    new mob_toc_rogue();
    new mob_toc_enh_shaman();
    new mob_toc_retro_paladin();
    new mob_toc_pet_warlock();
    new mob_toc_pet_hunter();
    new spell_faction_champion_warl_unstable_affliction();
    new spell_faction_champion_death_grip();
}

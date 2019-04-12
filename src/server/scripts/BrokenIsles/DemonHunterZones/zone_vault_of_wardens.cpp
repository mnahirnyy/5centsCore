/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "QuestPackets.h"
#include "ScenePackets.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "ObjectAccessor.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "MotionMaster.h"
#include "WorldSession.h"
#include "PhasingHandler.h"

enum eQuests
{
    QUEST_BREAKING_OUT          = 38672,
    QUEST_RISE_OF_THE_ILLIDARI  = 38690,
    QUEST_FEL_INFUSION          = 38689,
    QUEST_VAULT_OF_THE_WARDENS  = 39742, // optional bonus objective
    QUEST_STOP_GULDAN_H         = 38723,
    QUEST_STOP_GULDAN_V         = 40253,
    QUEST_GRAND_THEFT_FELBAT    = 39682,
    QUEST_FROZEN_IN_TIME        = 39685,
    QUEST_BEAM_ME_UP            = 39684,
    QUEST_FORGED_IN_FIRE        = 39683,
    QUEST_ALL_THE_WAY_UP        = 39686,
    QUEST_A_NEW_DIRECTION       = 40373,
    QUEST_BETWEEN_US_AND_FREEDOM_HH = 39694,
    QUEST_BETWEEN_US_AND_FREEDOM_HA = 39688,
    QUEST_BETWEEN_US_AND_FREEDOM_VA = 40255,
    QUEST_BETWEEN_US_AND_FREEDOM_VH = 40256,
    QUEST_ILLIDARI_LEAVING_A = 39689,
    QUEST_ILLIDARI_LEAVING_H = 39690,
};

enum eSpells
{
    SPELL_FEL_INFUSION          = 133508,
    SPELL_UNLOCKING_ALTRUIS     = 184012,
    SPELL_UNLOCKING_KAYN        = 177803,
};

enum eKillCredits
{
    KILL_CREDIT_KAYN_PICKED_UP_WEAPONS          = 112276,
    KILL_CREDIT_ALTRUIS_PICKED_UP_WEAPONS       = 112277,
    KILL_CREDIT_REUNION_FINISHED_KAYN           = 99326,
    KILL_CREDIT_REUNION_FINISHED_ALTRUIS        = 112287,
};

// 103658
class npc_kayn_cell : public CreatureScript
{
public:
   npc_kayn_cell() : CreatureScript("npc_kayn_cell") { }

   bool OnGossipHello(Player* player, Creature* creature) override
   {
       player->CastSpell(creature, SPELL_UNLOCKING_KAYN, true);
       creature->DespawnOrUnsummon(25);
       player->KilledMonsterCredit(KILL_CREDIT_REUNION_FINISHED_KAYN, ObjectGuid::Empty);
       player->KilledMonsterCredit(KILL_CREDIT_KAYN_PICKED_UP_WEAPONS, ObjectGuid::Empty);
       return true;
   }
};

// 103655
class npc_altruis_cell : public CreatureScript
{
public:
    npc_altruis_cell() : CreatureScript("npc_altruis_cell") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->CastSpell(creature, SPELL_UNLOCKING_ALTRUIS, true);
        creature->DespawnOrUnsummon(25);
        player->KilledMonsterCredit(KILL_CREDIT_REUNION_FINISHED_ALTRUIS, ObjectGuid::Empty);
        player->KilledMonsterCredit(KILL_CREDIT_ALTRUIS_PICKED_UP_WEAPONS, ObjectGuid::Empty);

        return true;
    }
};

class q_breaking_out : public QuestScript
{
public:
    q_breaking_out() : QuestScript("q_breaking_out") { }

    void OnQuestStatusChange(Player* player, Quest const* /*quest*/, QuestStatus /*oldStatus*/, QuestStatus newStatus) override
    {
        if (newStatus == QUEST_STATUS_NONE)
        {
            PhasingHandler::OnConditionChange(player);
            if (GameObject* go = player->FindNearestGameObject(244925, 200.0f))
                go->ResetDoorOrButton();
        }
        else if (newStatus == QUEST_STATUS_REWARDED)
        {
            if (GameObject* go = player->FindNearestGameObject(244925, 200.0f))
                go->UseDoorOrButton();
        }
        else if (newStatus == QUEST_STATUS_COMPLETE)
        {
            if (Creature* Kayn = player->FindNearestCreature(99631, 30.0f))
                Kayn->AI()->Talk(1);
            if (Creature* Altruis = player->FindNearestCreature(99632, 30.0f))
                Altruis->AI()->Talk(1);
        }
    }
};

class npc_fel_infusion : public CreatureScript
{
public:
    npc_fel_infusion() : CreatureScript("npc_fel_infusion") { }

    struct npc_fel_infusionAI : public ScriptedAI
    {
        npc_fel_infusionAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize() {}

        void Reset() override
        {
            Initialize();
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
            {
                killer->ToPlayer()->SetPower(POWER_ALTERNATE_POWER, killer->GetPower(POWER_ALTERNATE_POWER) + 10);

                for (uint8 i = 0; i < 10; ++i)
                    killer->ToPlayer()->KilledMonsterCredit(89297, ObjectGuid::Empty);
            }
        }

        void EnterCombat(Unit* who) override {}

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);
            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fel_infusionAI(creature);
    }
};

class q_fel_infusion : public QuestScript
{
public:
    q_fel_infusion() : QuestScript("q_fel_infusion") { }

    void OnQuestStatusChange(Player* player, Quest const* /*quest*/, QuestStatus /*oldStatus*/, QuestStatus newStatus) override
    {
        if (newStatus == QUEST_STATUS_NONE)
        {
            player->RemoveAurasDueToSpell(SPELL_FEL_INFUSION);
            PhasingHandler::OnConditionChange(player);
        }
    }
};

// 92986 Altruis
class npc_altruis : public CreatureScript
{
public:
    npc_altruis() : CreatureScript("npc_altruis") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_FEL_INFUSION)
            player->CastSpell(player, SPELL_FEL_INFUSION, true);

        return true;
    }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_FEL_INFUSION)
            player->RemoveAurasDueToSpell(SPELL_FEL_INFUSION);

        return true;
    }
};

// 96665 kayn
class npc_kayn_3 : public CreatureScript
{
public:
    npc_kayn_3() : CreatureScript("npc_kayn_3") { }

    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_RISE_OF_THE_ILLIDARI)
        {
            if (GameObject* go = player->FindNearestGameObject(245467, 200.0f))
                go->UseDoorOrButton();
            if (GameObject* go = player->FindNearestGameObject(244404, 200.0f))
                go->UseDoorOrButton();
            if (GameObject* go = player->FindNearestGameObject(253400, 200.0f))
                go->UseDoorOrButton();
        }
        return true;
    }
};

// guid: 20542913 id: 92984 Kayn Sunfury to fight with Sledge
enum KaynSledgeFightEvents
{
    EVENT_EYE_BEAM = 1,
    EVENT_BLINK = 2,
    EVENT_UPDATE_PHASES = 3,
    SAY_DO_NOT_SPEAK_OLD_TIMES = 10,
    SAY_HE_WAS_FIGHTING = 11,
    SAY_HE_MADE_HARD_CHOICES = 12,
};

enum KaynSledgeFightSpells
{
    SPELL_EYE_BEAM = 117275,
    SPELL_BLINK = 117312,
    SPELL_ANNIHILATE = 199604,
};

enum KaynSledgeFightData
{
    DATA_ALTRUIS_TALK_OLD_TIMES = 21,
    DATA_ALTRUIS_TALK_AFTER_10K_YEARS = 22,
    DATA_ALTRUIS_TALK_FOLLOW_BLINDLY = 23,
    DATA_ALTRUIS_TALK_DID_NOT_MURDER = 24,
    DATA_SLEDGE_DEATH = 4,
};

enum KaynSledgeFightTexts
{
    TEXT_KAYN_ANSWER_1 = 0,
    TEXT_KAYN_ANSWER_2 = 1,
    TEXT_KAYN_ANSWER_3 = 2,
};

enum KaynSledgeFightMisc
{
    NPC_KAYN_SUNFURY_SLEDGE = 92984,
    NPC_ALTRUIS_SUFFERER_CRUSHER = 92985,
    NPC_SLEDGE = 92990,
    DB_PHASE_FIGHT = 543,
    DB_PHASE_AFTER_FIGHT = 993
};

class npc_kayn_sledge_fight : public CreatureScript
{
public:
    npc_kayn_sledge_fight() : CreatureScript("npc_kayn_sledge_fight") { }

    struct npc_kayn_sledge_fightAI : public ScriptedAI
    {
        npc_kayn_sledge_fightAI(Creature* creature) : ScriptedAI(creature) {
            me->SetReactState(REACT_DEFENSIVE);
        }

        bool _talkedKaynFirstLine = false;
        bool _talkedKaynSecondLine = false;
        bool _talkedKaynThirdLine = false;
        
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->IsPlayer())
                if (Creature* creature = me->FindNearestCreature(NPC_SLEDGE, me->GetVisibilityRange(), true))
                    AttackStart(creature);
        }

        void EnterCombat(Unit* who) override
        {
            who->GetAI()->AttackStart(me);
            _events.ScheduleEvent(EVENT_EYE_BEAM, urand(20000, 40000));
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (HealthAbovePct(85))
                damage = urand(1, 2);
            else
                me->SetHealth(me->GetMaxHealth() * 0.85f);
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_ANNIHILATE)
            {
                _events.CancelEvent(EVENT_EYE_BEAM);
                _events.ScheduleEvent(EVENT_BLINK, 1000);
            }
        }

        void SetData(uint32 id, uint32 /*value*/) override
        {
            switch (id)
            {
            case DATA_ALTRUIS_TALK_OLD_TIMES:
                if (!_talkedKaynFirstLine) {
                    Talk(TEXT_KAYN_ANSWER_1);
                    _events.ScheduleEvent(SAY_DO_NOT_SPEAK_OLD_TIMES, 9000);
                    _talkedKaynFirstLine = true;
                }
                break;
            case DATA_ALTRUIS_TALK_AFTER_10K_YEARS:
                if (!_talkedKaynSecondLine) {
                    Talk(TEXT_KAYN_ANSWER_2);
                    _events.ScheduleEvent(SAY_HE_WAS_FIGHTING, 7000);
                    _talkedKaynSecondLine = true;
                }
                break;
            case DATA_ALTRUIS_TALK_FOLLOW_BLINDLY:
                if (!_talkedKaynThirdLine) {
                    Talk(TEXT_KAYN_ANSWER_3);
                    _events.ScheduleEvent(SAY_HE_MADE_HARD_CHOICES, 7000);
                    _talkedKaynThirdLine = true;
                }
                break;
            case DATA_SLEDGE_DEATH:
                EnterEvadeMode(EVADE_REASON_OTHER);
                _events.CancelEvent(EVENT_EYE_BEAM);
                _events.CancelEvent(SAY_DO_NOT_SPEAK_OLD_TIMES);
                _events.CancelEvent(SAY_HE_WAS_FIGHTING);
                _events.CancelEvent(SAY_HE_MADE_HARD_CHOICES);
                break;
            }
        }

        void JustReachedHome() override
        {
            _events.ScheduleEvent(EVENT_UPDATE_PHASES, 5000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {   
                case SAY_DO_NOT_SPEAK_OLD_TIMES:
                    if (Creature* creature = me->FindNearestCreature(NPC_ALTRUIS_SUFFERER_CRUSHER, me->GetVisibilityRange(), true))
                        creature->AI()->SetData(DATA_ALTRUIS_TALK_AFTER_10K_YEARS, DATA_ALTRUIS_TALK_AFTER_10K_YEARS);
                    break;
                case SAY_HE_WAS_FIGHTING:
                    if (Creature* creature = me->FindNearestCreature(NPC_ALTRUIS_SUFFERER_CRUSHER, me->GetVisibilityRange(), true))
                        creature->AI()->SetData(DATA_ALTRUIS_TALK_FOLLOW_BLINDLY, DATA_ALTRUIS_TALK_FOLLOW_BLINDLY);
                    break;
                case SAY_HE_MADE_HARD_CHOICES:
                    if (Creature* creature = me->FindNearestCreature(NPC_ALTRUIS_SUFFERER_CRUSHER, me->GetVisibilityRange(), true))
                        creature->AI()->SetData(DATA_ALTRUIS_TALK_DID_NOT_MURDER, DATA_ALTRUIS_TALK_DID_NOT_MURDER);
                    break;
                case EVENT_EYE_BEAM:
                    DoCastVictim(SPELL_EYE_BEAM);
                    _events.ScheduleEvent(EVENT_EYE_BEAM, urand(20000, 40000));
                    break;
                case EVENT_BLINK:
                    DoCastVictim(SPELL_BLINK);
                    _events.ScheduleEvent(EVENT_EYE_BEAM, urand(15000, 17000));
                    break;
                case EVENT_UPDATE_PHASES:
                    std::list<Player*> players;
                    me->GetPlayerListInGrid(players, me->GetVisibilityRange());
                    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if ((*itr)->ToPlayer()->GetQuestStatus(QUEST_STOP_GULDAN_H) == QUEST_STATUS_COMPLETE ||
                            (*itr)->ToPlayer()->GetQuestStatus(QUEST_STOP_GULDAN_V) == QUEST_STATUS_COMPLETE)
                        {
                            PhasingHandler::AddPhase(*itr, DB_PHASE_AFTER_FIGHT, true);
                            PhasingHandler::RemovePhase(*itr, DB_PHASE_FIGHT, true);
                        }
                    }
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_kayn_sledge_fightAI(creature);
    }
};
// guid: 20542915 npc: 92990 "Sledge"
// (abilities: 199556/brutal-attacks, 199375/demon-link, 199604/annihilate, 199474/leaping-retreat, 199481/shoulder-charge)
enum SledgeEvents
{
    EVENT_BRUTAL_ATTACKS = 1,
    EVENT_ANNIHILATE = 2,
    EVENT_SHOULDER_CHARGE = 3,
    EVENT_LEAPING_RETREAT = 4,
    EVENT_DEMON_LINK = 5,
};

enum SledgeSpells
{
    SPELL_BRUTAL_ATTACKS = 199556,
    SPELL_DEMON_LINK = 199375,
    SPELL_LEAPING_RETREAT = 199474,
    SPELL_SHOULDER_CHARGE = 199481,
    SPELL_DEATH_INVIS = 117555,
    SPELL_SEE_DEATH_INVIS = 117491,
};

enum SledgeMisc
{
    QUEST_KILL_CREDIT = 106241,
};

class npc_sledge : public CreatureScript
{
public:
    npc_sledge() : CreatureScript("npc_sledge") { }

    struct npc_sledgeAI : public ScriptedAI
    {
        npc_sledgeAI(Creature* creature) : ScriptedAI(creature) {
            Initialize();
        }

        void Initialize()
        {
            _playerParticipating = false;
            _secondBrutalAttack = false;
            _brutalAnnounced = false;
        }

        void Reset() override
        {
            _events.Reset();
            Initialize();
            me->setActive(true);
            me->SetReactState(REACT_PASSIVE);
            PhasingHandler::AddPhase(me, 543, true);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_SHOULDER_CHARGE, 8000);
            _events.ScheduleEvent(EVENT_LEAPING_RETREAT, 9000);
            _events.ScheduleEvent(EVENT_DEMON_LINK, 1000);
            _events.ScheduleEvent(EVENT_ANNIHILATE, urand(18000, 20000));
            _events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(18000, 20000));
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (HealthAbovePct(85) && attacker->IsCreature())
                if (attacker->GetEntry() == NPC_KAYN_SUNFURY_SLEDGE)
                    damage = urand(1, 2);

            if (HealthBelowPct(85) && attacker->IsCreature())
                if (attacker->GetEntry() == NPC_KAYN_SUNFURY_SLEDGE)
                    me->SetHealth(me->GetHealth() + damage);

            if (damage >= me->GetHealth())
            {
                std::list<HostileReference*> threatList;
                threatList = me->getThreatManager().getThreatList();
                for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                    if (Player* target = (*itr)->getTarget()->ToPlayer())
                        if (target->GetQuestStatus(QUEST_STOP_GULDAN_H) == QUEST_STATUS_INCOMPLETE ||
                            target->GetQuestStatus(QUEST_STOP_GULDAN_V) == QUEST_STATUS_INCOMPLETE)
                        {
                            target->KilledMonsterCredit(QUEST_KILL_CREDIT);
                        }
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* creature = me->FindNearestCreature(NPC_KAYN_SUNFURY_SLEDGE, me->GetVisibilityRange(), true))
                creature->AI()->SetData(DATA_SLEDGE_DEATH, DATA_SLEDGE_DEATH);

            std::list<Player*> playersVisibility;
            me->GetPlayerListInGrid(playersVisibility, me->GetVisibilityRange());
            for (std::list<Player*>::const_iterator itr = playersVisibility.begin(); itr != playersVisibility.end(); ++itr)
                (*itr)->CastSpell((*itr), SPELL_SEE_DEATH_INVIS, true);

            DoCastSelf(SPELL_DEATH_INVIS, true);
            me->DespawnOrUnsummon(20000, Seconds(1));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (!UpdateVictim())
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BRUTAL_ATTACKS:
                    DoCast(me, SPELL_BRUTAL_ATTACKS, true);
                    // _events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, 24000);
                    break;
                case EVENT_ANNIHILATE:
                    DoCastVictim(SPELL_ANNIHILATE);
                    // DoCast(SelectTarget(SELECT_TARGET_RANDOM, 1), SPELL_ANNIHILATE);
                    _events.ScheduleEvent(SPELL_BRUTAL_ATTACKS, urand(15000, 18000));
                    _events.ScheduleEvent(EVENT_ANNIHILATE, urand(15000, 18000));
                    break;
                case EVENT_SHOULDER_CHARGE:
                    DoCast(SelectTarget(SELECT_TARGET_RANDOM, 1), SPELL_SHOULDER_CHARGE);
                    _events.ScheduleEvent(EVENT_SHOULDER_CHARGE, urand(12000, 16000));
                    _events.ScheduleEvent(EVENT_LEAPING_RETREAT, urand(12000, 16000));
                    break;
                case EVENT_LEAPING_RETREAT:
                    DoCastVictim(SPELL_LEAPING_RETREAT);
                    break;
                case EVENT_DEMON_LINK:
                    DoCast(me, SPELL_DEMON_LINK, true);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        bool _playerParticipating;
        bool _secondBrutalAttack;
        bool _brutalAnnounced;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_sledgeAI(creature);
    }
};

// guid: 20542914 id: 92985 Altruis the Sufferer to fight with Crusher
enum AltruisCrusherFightSpells
{
    SPELL_FACE_KICK = 199645
};

enum AltruisCrusherFightData
{
    DATA_CRUSHER_DEATH = 5,
};

enum AltruisCrusherFightTexts
{
    TEXT_SAY_OLD_TIMES = 0,
    TEXT_SAY_ABOUT_ILLIDAN = 1,
    TEXT_SAY_FOLLOW_BLINDLY = 2,
    TEXT_SAY_DIDNT_MURDER = 3,
};

enum AltruisCrusherFightMisc
{
    NPC_CRUSHER = 97632,
};

class npc_altruis_crusher_fight : public CreatureScript
{
public:
    npc_altruis_crusher_fight() : CreatureScript("npc_altruis_crusher_fight") { }

    enum AltruisCrusherFightEvents
    {
        EVENT_EYE_BEAM = 1,
        EVENT_BLINK = 2,
        EVENT_UPDATE_PHASES = 3,
        EVENT_SET_DATA_FOR_KAYN_ANSWER_1 = 4,
        EVENT_SET_DATA_FOR_KAYN_ANSWER_2 = 5,
        EVENT_SET_DATA_FOR_KAYN_ANSWER_3 = 6,
    };

    struct npc_altruis_crusher_fightAI : public ScriptedAI
    {
        npc_altruis_crusher_fightAI(Creature* creature) : ScriptedAI(creature) {
            me->SetReactState(REACT_DEFENSIVE);
        }

        bool _talkedAltruisFirstLine = false;
        bool _talkedAltruisSecondLine = false;
        bool _talkedAltruisThirdLine = false;
        bool _talkedAltruisFourthLine = false;

        void MoveInLineOfSight(Unit* who) override
        {
            if (who->IsPlayer())
                if (Creature* creature = me->FindNearestCreature(NPC_CRUSHER, me->GetVisibilityRange(), true))
                    AttackStart(creature);
        }

        void EnterCombat(Unit* who) override
        {
            who->GetAI()->AttackStart(me);
            _events.ScheduleEvent(EVENT_EYE_BEAM, urand(20000, 40000));
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (HealthAbovePct(85))
                damage = urand(1, 2);
            else
                me->SetHealth(me->GetMaxHealth() * 0.85f);
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_FACE_KICK)
            {
                _events.CancelEvent(EVENT_EYE_BEAM);
                _events.ScheduleEvent(EVENT_BLINK, 1000);
            }
        }

        void SetData(uint32 id, uint32 /*value*/) override
        {
            switch (id)
            {
            case DATA_ALTRUIS_TALK_OLD_TIMES:
                if (!_talkedAltruisFirstLine) {
                    Talk(TEXT_SAY_OLD_TIMES);
                    _events.ScheduleEvent(EVENT_SET_DATA_FOR_KAYN_ANSWER_1, 3000);
                    _talkedAltruisFirstLine = true;
                }
                break;
            case DATA_ALTRUIS_TALK_AFTER_10K_YEARS:
                if (!_talkedAltruisSecondLine) {
                    Talk(TEXT_SAY_ABOUT_ILLIDAN);
                    _events.ScheduleEvent(EVENT_SET_DATA_FOR_KAYN_ANSWER_2, 7000);
                    _talkedAltruisSecondLine = true;
                }
                break;
            case DATA_ALTRUIS_TALK_FOLLOW_BLINDLY:
                if (!_talkedAltruisThirdLine) {
                    Talk(TEXT_SAY_FOLLOW_BLINDLY);
                    _events.ScheduleEvent(EVENT_SET_DATA_FOR_KAYN_ANSWER_3, 5000);
                    _talkedAltruisThirdLine = true;
                }
                break;
            case DATA_ALTRUIS_TALK_DID_NOT_MURDER:
                if (!_talkedAltruisFourthLine) {
                    Talk(TEXT_SAY_DIDNT_MURDER);
                    _talkedAltruisFourthLine = true;
                }
                break;
            case DATA_CRUSHER_DEATH:
                EnterEvadeMode(EVADE_REASON_OTHER);
                _events.CancelEvent(EVENT_EYE_BEAM);
                _events.CancelEvent(EVENT_SET_DATA_FOR_KAYN_ANSWER_1);
                _events.CancelEvent(EVENT_SET_DATA_FOR_KAYN_ANSWER_2);
                _events.CancelEvent(EVENT_SET_DATA_FOR_KAYN_ANSWER_3);
                break;
            }
        }

        void JustReachedHome() override
        {
            _events.ScheduleEvent(EVENT_UPDATE_PHASES, 5000);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {   
                case EVENT_SET_DATA_FOR_KAYN_ANSWER_1:
                    if (Creature* creature = me->FindNearestCreature(NPC_KAYN_SUNFURY_SLEDGE, me->GetVisibilityRange(), true))
                        creature->AI()->SetData(DATA_ALTRUIS_TALK_OLD_TIMES, DATA_ALTRUIS_TALK_OLD_TIMES);
                    break;
                case EVENT_SET_DATA_FOR_KAYN_ANSWER_2:
                    if (Creature* creature = me->FindNearestCreature(NPC_KAYN_SUNFURY_SLEDGE, me->GetVisibilityRange(), true))
                        creature->AI()->SetData(DATA_ALTRUIS_TALK_AFTER_10K_YEARS, DATA_ALTRUIS_TALK_AFTER_10K_YEARS);
                    break;
                case EVENT_SET_DATA_FOR_KAYN_ANSWER_3:
                    if (Creature* creature = me->FindNearestCreature(NPC_KAYN_SUNFURY_SLEDGE, me->GetVisibilityRange(), true))
                        creature->AI()->SetData(DATA_ALTRUIS_TALK_FOLLOW_BLINDLY, DATA_ALTRUIS_TALK_FOLLOW_BLINDLY);
                    break;
                case EVENT_EYE_BEAM:
                    DoCastVictim(SPELL_EYE_BEAM);
                    _events.ScheduleEvent(EVENT_EYE_BEAM, urand(20000, 40000));
                    break;
                case EVENT_BLINK:
                    DoCastVictim(SPELL_BLINK);
                    _events.ScheduleEvent(EVENT_EYE_BEAM, urand(15000, 17000));
                    break;
                case EVENT_UPDATE_PHASES:
                    std::list<Player*> players;
                    me->GetPlayerListInGrid(players, me->GetVisibilityRange());
                    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        if ((*itr)->ToPlayer()->GetQuestStatus(QUEST_STOP_GULDAN_H) == QUEST_STATUS_COMPLETE ||
                            (*itr)->ToPlayer()->GetQuestStatus(QUEST_STOP_GULDAN_V) == QUEST_STATUS_COMPLETE)
                        {
                            PhasingHandler::AddPhase(*itr, DB_PHASE_AFTER_FIGHT, true);
                            PhasingHandler::RemovePhase(*itr, DB_PHASE_FIGHT, true);
                        }
                    }
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_altruis_crusher_fightAI(creature);
    }
};

enum CrusherMisc
{
    TAKING_POWER_KILL_CREDIT = 106241,
};
// guid: 20542912 npc: 97632 "Crusher"
// (abilities: 199556/brutal-attacks, 199375/demon-link, 199645/face-kick, 199474/leaping-retreat, 199481/shoulder-charge)
class npc_crusher : public CreatureScript
{
public:
    npc_crusher() : CreatureScript("npc_crusher") { }

    enum CrusherEvents
    {
        EVENT_BRUTAL_ATTACKS = 1,
        EVENT_FACE_KICK = 2,
        EVENT_SHOULDER_CHARGE = 3,
        EVENT_LEAPING_RETREAT = 4,
        EVENT_DEMON_LINK = 5,
    };

    struct npc_crusherAI : public ScriptedAI
    {
        npc_crusherAI(Creature* creature) : ScriptedAI(creature) {
            Initialize();
        }

        void Initialize()
        {
            _playerParticipating = false;
            _conversationStarted = false;
            _faceKickAnnounced = false;
        }

        void Reset() override
        {
            _events.Reset();
            Initialize();
            me->setActive(true);
            me->SetReactState(REACT_PASSIVE);
            PhasingHandler::AddPhase(me, 543, true);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_SHOULDER_CHARGE, 8000);
            _events.ScheduleEvent(EVENT_LEAPING_RETREAT, 9000);
            _events.ScheduleEvent(EVENT_DEMON_LINK, 1000);
            _events.ScheduleEvent(EVENT_FACE_KICK, urand(18000, 20000));
            _events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(18000, 20000));
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (HealthAbovePct(85) && attacker->IsCreature())
                if (attacker->GetEntry() == NPC_ALTRUIS_SUFFERER_CRUSHER)
                    damage = urand(1, 2);

            if (HealthBelowPct(85) && attacker->IsCreature())
                if (attacker->GetEntry() == NPC_ALTRUIS_SUFFERER_CRUSHER)
                    me->SetHealth(me->GetHealth() + damage);

            if (HealthBelowPct(75) && !_conversationStarted)
            {
                if (Creature* creature = me->FindNearestCreature(NPC_ALTRUIS_SUFFERER_CRUSHER, me->GetVisibilityRange(), true))
                {
                    creature->AI()->SetData(DATA_ALTRUIS_TALK_OLD_TIMES, DATA_ALTRUIS_TALK_OLD_TIMES);
                    _conversationStarted = true;
                }
            }       
            
            if (damage >= me->GetHealth())
            {
                std::list<HostileReference*> threatList;
                threatList = me->getThreatManager().getThreatList();
                for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                    if (Player* target = (*itr)->getTarget()->ToPlayer())
                        if (target->GetQuestStatus(QUEST_STOP_GULDAN_H) == QUEST_STATUS_INCOMPLETE ||
                            target->GetQuestStatus(QUEST_STOP_GULDAN_V) == QUEST_STATUS_INCOMPLETE)
                                target->KilledMonsterCredit(TAKING_POWER_KILL_CREDIT);
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* creature = me->FindNearestCreature(NPC_ALTRUIS_SUFFERER_CRUSHER, me->GetVisibilityRange(), true))
                creature->AI()->SetData(DATA_CRUSHER_DEATH, DATA_CRUSHER_DEATH);

            std::list<Player*> playersVisibility;
            me->GetPlayerListInGrid(playersVisibility, me->GetVisibilityRange());
            for (std::list<Player*>::const_iterator itr = playersVisibility.begin(); itr != playersVisibility.end(); ++itr)
                (*itr)->CastSpell((*itr), SPELL_SEE_DEATH_INVIS, true);

            DoCastSelf(SPELL_DEATH_INVIS, true);
            me->DespawnOrUnsummon(20000, Seconds(1));
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (!UpdateVictim())
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BRUTAL_ATTACKS:
                    DoCast(me, SPELL_BRUTAL_ATTACKS, true);
                    // _events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, 24000);
                    break;
                case EVENT_FACE_KICK:
                    DoCastVictim(SPELL_FACE_KICK);
                    // DoCast(SelectTarget(SELECT_TARGET_RANDOM, 1), SPELL_FACE_KICK);
                    _events.ScheduleEvent(EVENT_BRUTAL_ATTACKS, urand(16000, 18000));
                    _events.ScheduleEvent(EVENT_FACE_KICK, urand(16000, 18000));
                    break;
                case EVENT_SHOULDER_CHARGE:
                    DoCast(SelectTarget(SELECT_TARGET_RANDOM, 1), SPELL_SHOULDER_CHARGE);
                    _events.ScheduleEvent(EVENT_SHOULDER_CHARGE, urand(12000, 16000));
                    _events.ScheduleEvent(EVENT_LEAPING_RETREAT, urand(12000, 16000));
                    break;
                case EVENT_LEAPING_RETREAT:
                    DoCastVictim(SPELL_LEAPING_RETREAT);
                    break;
                case EVENT_DEMON_LINK:
                    DoCast(me, SPELL_DEMON_LINK, true);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        bool _playerParticipating;
        bool _conversationStarted;
        bool _faceKickAnnounced;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_crusherAI(creature);
    }
};

// 243967
class go_reflective_mirror : public GameObjectScript
{
public:
    go_reflective_mirror() : GameObjectScript("go_reflective_mirror") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        player->CastSpell(player, 191917, true);
        return true;
    }
};

enum eCountermeasures
{
    KILL_CREDIT_WESTERN_COUNTERMEASURE      = 99732,
    KILL_CREDIT_SOUTHERN_COUNTERMEASURE     = 99731,
    KILL_CREDIT_EASTERN_COUNTERMEASURE      = 99709,
};

// 204588
class spell_activate_countermeasure : public SpellScriptLoader
{
public:
    spell_activate_countermeasure() : SpellScriptLoader("spell_activate_countermeasure") { }

    class spell_activate_countermeasure_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_activate_countermeasure_SpellScript);

        void HandleKillCredit()
        {
            if (Creature* creature = GetCaster()->ToPlayer()->FindNearestCreature(99732, 5.0f))
                GetCaster()->ToPlayer()->KilledMonsterCredit(KILL_CREDIT_WESTERN_COUNTERMEASURE, ObjectGuid::Empty);

            if (Creature* creature = GetCaster()->ToPlayer()->FindNearestCreature(99731, 5.0f))
                GetCaster()->ToPlayer()->KilledMonsterCredit(KILL_CREDIT_SOUTHERN_COUNTERMEASURE, ObjectGuid::Empty);

            if (Creature* creature = GetCaster()->ToPlayer()->FindNearestCreature(99709, 5.0f))
                GetCaster()->ToPlayer()->KilledMonsterCredit(KILL_CREDIT_EASTERN_COUNTERMEASURE, ObjectGuid::Empty);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_activate_countermeasure_SpellScript::HandleKillCredit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_activate_countermeasure_SpellScript();
    }
};

class q_frozen_in_time : public QuestScript
{
public:
    q_frozen_in_time() : QuestScript("q_frozen_in_time") { }

    void OnQuestStatusChange(Player* player, Quest const* /*quest*/, QuestStatus /*oldStatus*/, QuestStatus newStatus) override
    {
        if (newStatus == QUEST_STATUS_COMPLETE)
        {
            if (Creature* ashgolm = player->FindNearestCreature(96681, 200.0f))
                ashgolm->CastSpell(nullptr, 200354, true);
        }
    }
};

// 244644 Warden's Ascent
class go_warden_ascent : public GameObjectScript
{
public:
    go_warden_ascent() : GameObjectScript("go_warden_ascent") { }

    struct go_warden_ascentAI : public GameObjectAI
    {
        go_warden_ascentAI(GameObject* pGO) : GameObjectAI(pGO) {}

        uint32 giveKillCredit;

        void Reset()
        {
            giveKillCredit = 1000;
        }

        void UpdateAI(uint32 diff)
        {
            std::list<Player*> playerList;
            go->GetPlayerListInGrid(playerList, 10.0f);
            for (Player* player : playerList)
            {
                if (player->GetQuestStatus(39686) == QUEST_STATUS_INCOMPLETE)
                {
                    if (giveKillCredit <= diff)
                    {
                        if (player->GetPositionZ() >= 253.0f)
                            player->KilledMonsterCredit(96814, ObjectGuid::Empty);

                        giveKillCredit = 1000;
                    }
                    else
                        giveKillCredit -= diff;
                }
            }
        }
    };

    GameObjectAI* GetAI(GameObject* pGO) const
    {
        return new go_warden_ascentAI(pGO);
    }
};

// 243967
class go_pool_of_judgements : public GameObjectScript
{
public:
    go_pool_of_judgements() : GameObjectScript("go_pool_of_judgements") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        player->KilledMonsterCredit(100166, ObjectGuid::Empty);
        player->SendMovieStart(478);
        return true;
    }
};

enum eBastillax
{
    EVENT_FEL_ANNIHILATION = 0,
    EVENT_CRUSHING_SHADOWS = 1,
    EVENT_BLUR_OF_SHADOWS = 2,
    SPELL_FEL_ANNIHILATION = 200007,
    SPELL_CRUSHING_SHADOWS = 200027,
    SPELL_BLUR_OF_SHADOWS = 200002,
};

class npc_bastillax : public CreatureScript
{
public:
    npc_bastillax() : CreatureScript("npc_bastillax") { }

    struct npc_bastillaxAI : public ScriptedAI
    {
        npc_bastillaxAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            events.Reset();
        }

        void JustDied(Unit* killer) override
        {
            if (killer->GetTypeId() == TYPEID_PLAYER)
            {
                killer->ToPlayer()->KilledMonsterCredit(113812, ObjectGuid::Empty);
                killer->ToPlayer()->KilledMonsterCredit(106255, ObjectGuid::Empty);
            }
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_FEL_ANNIHILATION, urand(6000, 8000));
            events.ScheduleEvent(EVENT_CRUSHING_SHADOWS, urand(10000, 12000));
            events.ScheduleEvent(EVENT_BLUR_OF_SHADOWS, 3000);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FEL_ANNIHILATION:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(me, SPELL_FEL_ANNIHILATION, true);

                    events.ScheduleEvent(EVENT_FEL_ANNIHILATION, urand(8000, 10000));
                    break;
                case EVENT_CRUSHING_SHADOWS:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_CRUSHING_SHADOWS, true);

                    events.ScheduleEvent(EVENT_CRUSHING_SHADOWS, 6000);
                    break;
                case EVENT_BLUR_OF_SHADOWS:
                    if (Unit* target = me->GetVictim())
                        me->CastSpell(target, SPELL_BLUR_OF_SHADOWS, true);

                    events.ScheduleEvent(EVENT_BLUR_OF_SHADOWS, 6000);
                    break;
                }
            }

            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bastillaxAI(creature);
    }
};

// 96682
struct npc_vow_immolanth : public ScriptedAI
{
    npc_vow_immolanth(Creature* creature) : ScriptedAI(creature) { }

    enum Spells
    {
        SPELL_BURNING_FEL = 199758,
        SPELL_CHAOS_NOVA = 199828,
    };

    enum Text
    {   
        SAY_60PCT = 0,
        SAY_20PCT = 1,
    };

    void Reset() override { }

    void EnterCombat(Unit*) override
    {
        me->GetScheduler().Schedule(Seconds(15), [this](TaskContext context)
        {
            if (Unit* target = me->GetVictim())
                me->CastSpell(target, SPELL_BURNING_FEL);

            context.Repeat(Seconds(15));
        })
            .Schedule(Seconds(10), [this](TaskContext context)
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                me->CastSpell(target, SPELL_CHAOS_NOVA);

            if (me->GetHealthPct() <= 60)
            {
                Talk(SAY_60PCT);
            }

            if (me->GetHealthPct() <= 20)
            {
                Talk(SAY_20PCT);
            }

            context.Repeat(Seconds(10));
        });
    }

    void JustDied(Unit* /*killer*/) override
    {
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 50.0f);

        for (Player* player : players)
        {
            player->KilledMonsterCredit(106254);
        }
    }
};

// 96681
struct npc_vow_ashgolm : public ScriptedAI
{
    npc_vow_ashgolm(Creature* creature) : ScriptedAI(creature) { }

    enum Spells
    {
        SPELL_FISSURE = 192522,
        SPELL_LAVA = 192519,
        SPELL_LAVA_WREATH = 192631,
    };

    void Reset() override { }

    void EnterCombat(Unit*) override
    {
        me->GetScheduler().Schedule(Seconds(15), [this](TaskContext context)
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                me->CastSpell(target, SPELL_FISSURE);

            context.Repeat(Seconds(35));
        })
            .Schedule(Seconds(45), [this](TaskContext context)
        {
            me->CastSpell(nullptr, SPELL_LAVA);
            me->CastSpell(nullptr, SPELL_LAVA_WREATH);
            
            context.Repeat(Seconds(30));
        });
    }
};

enum eChoices
{
    PLAYER_CHOICE_DH_FOLLOWER_SELECTION = 234,
    PLAYER_CHOICE_DH_FOLLOWER_SELECTION_KAYN = 486,
    PLAYER_CHOICE_DH_FOLLOWER_SELECTION_ALTRUIS = 487,
    SPELL_NEW_DIRECTION_CHOICE_KAYN_OR_ALTRUIS = 196650,
    SPELL_NEW_DIRECTION_CHOSE_ALTRUIS = 196662,
    SPELL_NEW_DIRECTION_CHOSE_KAYN = 196661,
};

class npc_korvas_bloodthorn : public CreatureScript
{
public:
    npc_korvas_bloodthorn() : CreatureScript("npc_korvas_bloodthorn") { }
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->CastSpell(player, SPELL_NEW_DIRECTION_CHOICE_KAYN_OR_ALTRUIS, true); // Display follower choice
        CloseGossipMenuFor(player);
        return true;
    }
};

class PlayerScript_follower_choice : public PlayerScript
{
public:
    PlayerScript_follower_choice() : PlayerScript("PlayerScript_follower_choice") {}

    void OnCompleteQuestChoice(Player* player, uint32 choiceID, uint32 responseID)
    {
        if (choiceID != PLAYER_CHOICE_DH_FOLLOWER_SELECTION)
            return;

        switch (responseID)
        {
        case PLAYER_CHOICE_DH_FOLLOWER_SELECTION_KAYN:
            player->CastSpell(player, SPELL_NEW_DIRECTION_CHOSE_KAYN, true);
            break;
        case PLAYER_CHOICE_DH_FOLLOWER_SELECTION_ALTRUIS:
            player->CastSpell(player, SPELL_NEW_DIRECTION_CHOSE_ALTRUIS, true);
            break;
        default:
            break;
        }
    }
};

// 20543507 (97978)
class npc_khadgar : public CreatureScript
{
public:
    npc_khadgar() : CreatureScript("npc_khadgar") { }
    enum eKhadgar {
        SPELL_TELEPORT_TO_STORMWIND = 192757,
        SPELL_TELEPORT_TO_ORGRIMMAR = 192758,
    };
    bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_ILLIDARI_LEAVING_H)
        {
            player->CastSpell(player, SPELL_TELEPORT_TO_ORGRIMMAR, true);
            // player->TeleportTo(1, 1569.96f, -4397.41f, 16.05f, 0.527317f);
        } else if (quest->GetQuestId() == QUEST_ILLIDARI_LEAVING_A)
        {
            player->CastSpell(player, SPELL_TELEPORT_TO_STORMWIND, true);
            // player->TeleportTo(0, -8554.12f, 477.13f, 104.36f, 3.589650f);
        }

        return true;
    }
};

class scene_guldan_stealing_illidan_corpse : public SceneScript
{
public:
   scene_guldan_stealing_illidan_corpse() : SceneScript("scene_guldan_stealing_illidan_corpse") { }
   enum {
       KILL_CREDIT_ENTER_THE_CHAMBER = 99303,
       DB_PHASE_FIGHT_IN_CHAMBER = 543,
   };
   void OnSceneEnd(Player* player, uint32 /*sceneInstanceID*/, SceneTemplate const* /*sceneTemplate*/) override
   {
       player->KilledMonsterCredit(KILL_CREDIT_ENTER_THE_CHAMBER);
       if (GameObject* go = player->FindNearestGameObject(244923, 50.0f)) {
           go->UseDoorOrButton();
       }   
       player->TeleportTo(1468, 4063.53f, -297.05f, -281.58f, 3.098f, false);
   }
};

class npc_maiev_shadowsong : public CreatureScript
{
public:
    npc_maiev_shadowsong() : CreatureScript("npc_maiev_shadowsong") { }
    enum {
        SCENE_GULDAN_STEAL_ILLIDAN_ID = 1016,
    };
    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_STOP_GULDAN_H || QUEST_STOP_GULDAN_V)
        {
            player->GetSceneMgr().PlayScene(SCENE_GULDAN_STEAL_ILLIDAN_ID);
        }
        return true;
    }
 };

enum eLegionPortal {
    SPELL_PORTAL_EXPLOSION = 196084,
    SPELL_DESTROYING_LEGION_PORTAL = 202064,
    SPELL_KILL_CREDIT_BONUS_OBJECTIVE = 97969,
};
class npc_legion_portal : public CreatureScript
{
public:
    npc_legion_portal() : CreatureScript("npc_legion_portal") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->CastSpell(creature, SPELL_DESTROYING_LEGION_PORTAL, true);
        player->CastSpell(creature, SPELL_PORTAL_EXPLOSION, true);
        creature->DespawnOrUnsummon(25);
        player->KilledMonsterCredit(SPELL_KILL_CREDIT_BONUS_OBJECTIVE, ObjectGuid::Empty);
        return true;
    }
};

class On100DHArrival : public PlayerScript
{
public:
    On100DHArrival() : PlayerScript("On100DHArrival") { }
    enum
    {
        SPELL_DH_IMPRISON = 217832,
        SPELL_DH_DARKNESS = 196718,
        SPELL_DH_BLADE_DANCE = 188499,
        SPELL_DH_SIGIL_OF_FLAME = 204596,
    };

    void OnLogin(Player* player, bool firstLogin) override
    {
        // For recovery purposes
        if (player->getLevel() >= 100 && firstLogin)
            Handle100DHArrival(player);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (oldLevel < 100 && player->getLevel() >= 100)
            Handle100DHArrival(player);
    }

    void Handle100DHArrival(Player* player)
    {
        if (player->getClass() == CLASS_DEMON_HUNTER)
        {
            player->LearnSpell(SPELL_DH_IMPRISON, false);
            if (player->GetSpecializationId() == TALENT_SPEC_DEMON_HUNTER_HAVOC) {
                player->LearnSpell(SPELL_DH_BLADE_DANCE, false);
            }
            else {
                player->LearnSpell(SPELL_DH_SIGIL_OF_FLAME, false);
            }
        }
    }
};

class PlayerScript_bonus_objective : public PlayerScript
{
public:
    PlayerScript_bonus_objective() : PlayerScript("PlayerScript_bonus_objective") {}

    uint32 checkTimer = 1000;

    void OnUpdate(Player* player, uint32 diff) override
    {
        if (checkTimer <= diff)
        {
            if (player->getClass() == CLASS_DEMON_HUNTER && player->GetZoneId() == 7814 && player->GetQuestStatus(QUEST_BREAKING_OUT) == QUEST_STATUS_REWARDED)
            {
                if (player->GetQuestStatus(QUEST_VAULT_OF_THE_WARDENS) == QUEST_STATUS_NONE)
                {
                    if (const Quest* quest = sObjectMgr->GetQuestTemplate(QUEST_VAULT_OF_THE_WARDENS))
                        player->AddQuest(quest, nullptr);
                }
            }

            checkTimer = 1000;
        }
        else checkTimer -= diff;
    }
};

class PlayerScript_switch_phases : public PlayerScript
{
public:
    PlayerScript_switch_phases() : PlayerScript("PlayerScript_switch_phases") {}

    uint32 checkTimer = 1000;

    void OnUpdate(Player* player, uint32 diff) override
    {
        if (checkTimer <= diff)
        {
            if (player->getClass() == CLASS_DEMON_HUNTER &&
                player->GetAreaId() == 7819 && player->GetPositionX() < 4080 &&
                (player->GetQuestStatus(QUEST_STOP_GULDAN_H) == QUEST_STATUS_INCOMPLETE ||
                 player->GetQuestStatus(QUEST_STOP_GULDAN_V) == QUEST_STATUS_INCOMPLETE) &&
                !player->GetPhaseShift().HasPhase(543))
            {
                PhasingHandler::AddPhase(player, 543, true);
                // Crusher respawn
                std::list<Creature*> crushers;
                player->GetCreatureListWithEntryInGrid(crushers, NPC_CRUSHER, 200.0f);
                for (std::list<Creature*>::iterator itr = crushers.begin(); itr != crushers.end(); ++itr)
                    if (!(*itr)->IsAlive())
                        (*itr)->Respawn();
                // Sledge respawn
                std::list<Creature*> sledges;
                player->GetCreatureListWithEntryInGrid(sledges, NPC_SLEDGE, 200.0f);
                for (std::list<Creature*>::iterator itr = sledges.begin(); itr != sledges.end(); ++itr)
                    if (!(*itr)->IsAlive())
                        (*itr)->Respawn();
            }
            checkTimer = 1000;
        }
        else checkTimer -= diff;
    }
};

void AddSC_zone_vault_of_wardens()
{
    new npc_kayn_cell();
    new npc_altruis_cell();
    new npc_fel_infusion();
    new npc_altruis();
    new npc_kayn_3();
    new q_fel_infusion();
    new q_breaking_out();
    new q_frozen_in_time();
    new npc_kayn_sledge_fight();
    new npc_sledge();
    new npc_altruis_crusher_fight();
    new npc_crusher();
    new go_reflective_mirror();
    new spell_activate_countermeasure();
    new go_warden_ascent();
    new npc_korvas_bloodthorn();
    new go_pool_of_judgements();
    new npc_bastillax();
    new npc_khadgar();
    new PlayerScript_follower_choice();
    RegisterCreatureAI(npc_vow_immolanth);
    RegisterCreatureAI(npc_vow_ashgolm);
    new npc_maiev_shadowsong();
    new npc_legion_portal();
    new scene_guldan_stealing_illidan_corpse();

    new On100DHArrival();
    new PlayerScript_bonus_objective();
    new PlayerScript_switch_phases();
}
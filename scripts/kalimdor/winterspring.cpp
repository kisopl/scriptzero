/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Winterspring
SD%Complete: 90
SDComment: Quest support: 4901, 5126 (Loraxs' tale missing proper gossip items text). Vendor Rivern Frostwind. Obtain Cache of Mau'ari
SDCategory: Winterspring
EndScriptData */

/* ContentData
npc_lorax
npc_rivern_frostwind
npc_witch_doctor_mauari
npc_ranshalla
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*######
## npc_lorax
######*/

bool GossipHello_npc_lorax(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestStatus(5126) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Talk to me", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_lorax(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What do you do here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(3759, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I can help you", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(3760, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "What deal?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(3761, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Then what happened?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(3762, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "He is not safe, i'll make sure of that.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->SEND_GOSSIP_MENU(3763, pCreature->GetObjectGuid());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(5126);
            break;
    }
    return true;
}

/*######
## npc_rivern_frostwind
######*/

bool GossipHello_npc_rivern_frostwind(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pCreature->isVendor() && pPlayer->GetReputationRank(589) == REP_EXALTED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_rivern_frostwind(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetObjectGuid());

    return true;
}

/*######
## npc_witch_doctor_mauari
######*/

bool GossipHello_npc_witch_doctor_mauari(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetObjectGuid());

    if (pPlayer->GetQuestRewardStatus(975))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'd like you to make me a new Cache of Mau'ari please.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(3377, pCreature->GetObjectGuid());
    }else
        pPlayer->SEND_GOSSIP_MENU(3375, pCreature->GetObjectGuid());

    return true;
}

bool GossipSelect_npc_witch_doctor_mauari(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction==GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, 16351, false);
    }

    return true;
}

/*####
# npc_ranshalla
####*/

enum
{
    // Escort texts
    SAY_QUEST_START             = -1000816,
    SAY_ENTER_OWL_THICKET       = -1000784,
    SAY_REACH_TORCH_1           = -1000785,
    SAY_REACH_TORCH_2           = -1000786,
    SAY_REACH_TORCH_3           = -1000787,
    SAY_AFTER_TORCH_1           = -1000788,
    SAY_AFTER_TORCH_2           = -1000789,
    SAY_REACH_ALTAR_1           = -1000790,
    SAY_REACH_ALTAR_2           = -1000791,

    // After lighting the altar cinematic
    SAY_RANSHALLA_ALTAR_1       = -1000792,
    SAY_RANSHALLA_ALTAR_2       = -1000793,
    SAY_PRIESTESS_ALTAR_3       = -1000794,
    SAY_PRIESTESS_ALTAR_4       = -1000795,
    SAY_RANSHALLA_ALTAR_5       = -1000796,
    SAY_RANSHALLA_ALTAR_6       = -1000797,
    SAY_PRIESTESS_ALTAR_7       = -1000798,
    SAY_PRIESTESS_ALTAR_8       = -1000799,
    SAY_PRIESTESS_ALTAR_9       = -1000800,
    SAY_PRIESTESS_ALTAR_10      = -1000801,
    SAY_PRIESTESS_ALTAR_11      = -1000802,
    SAY_PRIESTESS_ALTAR_12      = -1000803,
    SAY_PRIESTESS_ALTAR_13      = -1000804,
    SAY_PRIESTESS_ALTAR_14      = -1000805,
    SAY_VOICE_ALTAR_15          = -1000806,
    SAY_PRIESTESS_ALTAR_16      = -1000807,
    SAY_PRIESTESS_ALTAR_17      = -1000808,
    SAY_PRIESTESS_ALTAR_18      = -1000809,
    SAY_PRIESTESS_ALTAR_19      = -1000810,
    SAY_PRIESTESS_ALTAR_20      = -1000811,
    SAY_PRIESTESS_ALTAR_21      = -1000812,
    SAY_QUEST_END_1             = -1000813,
    SAY_QUEST_END_2             = -1000814,

    EMOTE_CHANT_SPELL           = -1000815,

    SPELL_LIGHT_TORCH           = 18953,        // channeled spell by Ranshalla while waiting for the torches / altar

    NPC_RANSHALLA               = 10300,
    NPC_PRIESTESS_ELUNE         = 12116,
    NPC_VOICE_ELUNE             = 12152,
    NPC_GUARDIAN_ELUNE          = 12140,

    NPC_PRIESTESS_DATA_1        = 1,            // dummy member for the first priestess (right)
    NPC_PRIESTESS_DATA_2        = 2,            // dummy member for the second priestess (left)
    DATA_MOVE_PRIESTESS         = 3,            // dummy member to check the priestess movement
    DATA_EVENT_END              = 4,            // dummy member to indicate the event end

    GO_ELUNE_ALTAR              = 177404,
    GO_ELUNE_FIRE               = 177417,
    GO_ELUNE_GEM                = 177414,       // is respawned in script
    GO_ELUNE_LIGHT              = 177415,       // are respawned in script

    QUEST_GUARDIANS_ALTAR       = 4901,
};

static const DialogueEntry aIntroDialogue[] =
{
    {SAY_REACH_ALTAR_1,      NPC_RANSHALLA,        2000},
    {SAY_REACH_ALTAR_2,      NPC_RANSHALLA,        3000},
    {NPC_RANSHALLA,          0,                    0},          // start the altar channeling
    {SAY_PRIESTESS_ALTAR_3,  NPC_PRIESTESS_DATA_2, 1000},
    {SAY_PRIESTESS_ALTAR_4,  NPC_PRIESTESS_DATA_1, 4000},
    {SAY_RANSHALLA_ALTAR_5,  NPC_RANSHALLA,        4000},
    {SAY_RANSHALLA_ALTAR_6,  NPC_RANSHALLA,        4000},       // start the escort here
    {SAY_PRIESTESS_ALTAR_7,  NPC_PRIESTESS_DATA_2, 4000},
    {SAY_PRIESTESS_ALTAR_8,  NPC_PRIESTESS_DATA_2, 5000},       // show the gem
    {GO_ELUNE_GEM,           0,                    5000},
    {SAY_PRIESTESS_ALTAR_9,  NPC_PRIESTESS_DATA_1, 4000},       // move priestess 1 near m_creature
    {NPC_PRIESTESS_DATA_1,   0,                    3000},
    {SAY_PRIESTESS_ALTAR_10, NPC_PRIESTESS_DATA_1, 5000},
    {SAY_PRIESTESS_ALTAR_11, NPC_PRIESTESS_DATA_1, 4000},
    {SAY_PRIESTESS_ALTAR_12, NPC_PRIESTESS_DATA_1, 5000},
    {SAY_PRIESTESS_ALTAR_13, NPC_PRIESTESS_DATA_1, 8000},       // summon voice and guard of elune
    {NPC_VOICE_ELUNE,        0,                    12000},
    {SAY_VOICE_ALTAR_15,     NPC_VOICE_ELUNE,      5000},       // move priestess 2 near m_creature
    {NPC_PRIESTESS_DATA_2,   0,                    3000},
    {SAY_PRIESTESS_ALTAR_16, NPC_PRIESTESS_DATA_2, 4000},
    {SAY_PRIESTESS_ALTAR_17, NPC_PRIESTESS_DATA_2, 6000},
    {SAY_PRIESTESS_ALTAR_18, NPC_PRIESTESS_DATA_1, 5000},
    {SAY_PRIESTESS_ALTAR_19, NPC_PRIESTESS_DATA_1, 3000},       // move the owlbeast
    {NPC_GUARDIAN_ELUNE,     0,                    2000},
    {SAY_PRIESTESS_ALTAR_20, NPC_PRIESTESS_DATA_1, 4000},       // move the first priestess up
    {SAY_PRIESTESS_ALTAR_21, NPC_PRIESTESS_DATA_2, 10000},      // move second priestess up
    {DATA_MOVE_PRIESTESS,    0,                    6000},       // despawn the gem
    {DATA_EVENT_END,         0,                    2000},       // turn towards the player
    {SAY_QUEST_END_2,        NPC_RANSHALLA,        0},
    {0, 0, 0},
};

struct EventLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

static EventLocations aWingThicketLocations[] =
{
    {5515.98f, -4903.43f, 846.30f, 4.58f},      // 0 right priestess summon loc
    {5501.94f, -4920.20f, 848.69f, 6.15f},      // 1 left priestess summon loc
    {5497.35f, -4906.49f, 850.83f, 2.76f},      // 2 guard of elune summon loc
    {5518.38f, -4913.47f, 845.57f},             // 3 right priestess move loc
    {5510.36f, -4921.17f, 846.33f},             // 4 left priestess move loc
    {5511.31f, -4913.82f, 847.17f},             // 5 guard of elune move loc
    {5518.51f, -4917.56f, 845.23f},             // 6 right priestess second move loc
    {5514.40f, -4921.16f, 845.49f}              // 7 left priestess second move loc
};

struct MANGOS_DLL_DECL npc_ranshallaAI : public npc_escortAI, private DialogueHelper
{
    npc_ranshallaAI(Creature* pCreature) : npc_escortAI(pCreature),
        DialogueHelper(aIntroDialogue)
    {
        Reset();
    }

    uint32 m_uiDelayTimer;

    ObjectGuid m_firstPriestessGuid;
    ObjectGuid m_secondPriestessGuid;
    ObjectGuid m_guardEluneGuid;
    ObjectGuid m_voiceEluneGuid;
    ObjectGuid m_altarGuid;

    void Reset()
    {
        m_uiDelayTimer = 0;
    }

    // Called when the player activates the torch / altar
    void DoContinueEscort(bool bIsAltarWaypoint = false)
    {
        if (bIsAltarWaypoint)
            DoScriptText(SAY_RANSHALLA_ALTAR_1, m_creature);
        else
        {
            switch(urand(0, 1))
            {
                case 0: DoScriptText(SAY_AFTER_TORCH_1, m_creature); break;
                case 1: DoScriptText(SAY_AFTER_TORCH_2, m_creature); break;
            }
        }

        m_uiDelayTimer = 2000;
    }

    // Called when Ranshalla starts to channel on a torch / altar
    void DoChannelTorchSpell(bool bIsAltarWaypoint = false)
    {
        // Check if we are using the fire or the altar and remove the no_interact flag
        if (bIsAltarWaypoint)
        {
            if (GameObject* pGo = GetClosestGameObjectWithEntry(m_creature, GO_ELUNE_ALTAR, 10.0f))
            {
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
                m_creature->SetFacingToObject(pGo);
                m_altarGuid = pGo->GetObjectGuid();
            }
        }
        else
        {
            if (GameObject* pGo = GetClosestGameObjectWithEntry(m_creature, GO_ELUNE_FIRE, 10.0f))
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
        }

        // Yell and set escort to pause
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_REACH_TORCH_1, m_creature); break;
            case 1: DoScriptText(SAY_REACH_TORCH_2, m_creature); break;
            case 2: DoScriptText(SAY_REACH_TORCH_3, m_creature); break;
        }

        DoScriptText(EMOTE_CHANT_SPELL, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_LIGHT_TORCH);
        SetEscortPaused(true);
    }

    void DoSummonPriestess()
    {
        // Summon 2 Elune priestess and make each of them move to a different spot
        if (Creature* pPriestess = m_creature->SummonCreature(NPC_PRIESTESS_ELUNE, aWingThicketLocations[0].m_fX, aWingThicketLocations[0].m_fY, aWingThicketLocations[0].m_fZ, aWingThicketLocations[0].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            pPriestess->GetMotionMaster()->MovePoint(0, aWingThicketLocations[3].m_fX, aWingThicketLocations[3].m_fY, aWingThicketLocations[3].m_fZ);
            m_firstPriestessGuid = pPriestess->GetObjectGuid();
        }
        if (Creature* pPriestess = m_creature->SummonCreature(NPC_PRIESTESS_ELUNE, aWingThicketLocations[1].m_fX, aWingThicketLocations[1].m_fY, aWingThicketLocations[1].m_fZ, aWingThicketLocations[1].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            // Left priestess should have a distinct move point because she is the one who starts the dialogue at point reach
            pPriestess->GetMotionMaster()->MovePoint(1, aWingThicketLocations[4].m_fX, aWingThicketLocations[4].m_fY, aWingThicketLocations[4].m_fZ);
            m_secondPriestessGuid = pPriestess->GetObjectGuid();
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || pSummoned->GetEntry() != NPC_PRIESTESS_ELUNE || uiPointId != 1)
            return;

        // Start the dialogue when the priestess reach the altar (they should both reach the point in the same time)
        StartNextDialogueText(SAY_PRIESTESS_ALTAR_3);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 3:
                DoScriptText(SAY_ENTER_OWL_THICKET, m_creature);
                break;
            case 10: // Cavern 1
            case 15: // Cavern 2
            case 20: // Cavern 3
            case 25: // Cavern 4
            case 36: // Cavern 5
                DoChannelTorchSpell();
                break;
            case 39:
                StartNextDialogueText(SAY_REACH_ALTAR_1);
                SetEscortPaused(true);
                break;
            case 41:
                {
                    // Search for all nearest lights and respawn them
                    std::list<GameObject*> m_lEluneLights;
                    GetGameObjectListWithEntryInGrid(m_lEluneLights, m_creature, GO_ELUNE_LIGHT, 20.0f);
                    for (std::list<GameObject*>::const_iterator itr = m_lEluneLights.begin(); itr != m_lEluneLights.end(); ++itr)
                    {
                        if ((*itr)->isSpawned())
                            continue;

                        (*itr)->SetRespawnTime(115);
                        (*itr)->Refresh();
                    }

                    if (GameObject* pAltar = m_creature->GetMap()->GetGameObject(m_altarGuid))
                        m_creature->SetFacingToObject(pAltar);
                    break;
                }
            case 42:
                // Summon the 2 priestess
                SetEscortPaused(true);
                DoSummonPriestess();
                DoScriptText(SAY_RANSHALLA_ALTAR_2, m_creature);
                break;
            case 44:
                // Stop the escort and turn towards the altar
                SetEscortPaused(true);
                if (GameObject* pAltar = m_creature->GetMap()->GetGameObject(m_altarGuid))
                    m_creature->SetFacingToObject(pAltar);
                break;
        }
    }

    void JustDidDialogueStep(int32 iEntry)
    {
        switch(iEntry)
        {
            case NPC_RANSHALLA:
                // Start the altar channeling
                DoChannelTorchSpell(true);
                break;
            case SAY_RANSHALLA_ALTAR_6:
                SetEscortPaused(false);
                break;
            case SAY_PRIESTESS_ALTAR_8:
                // make the gem respawn
                if (GameObject* pGem = GetClosestGameObjectWithEntry(m_creature, GO_ELUNE_GEM, 10.0f))
                {
                    if (pGem->isSpawned())
                        break;

                    pGem->SetRespawnTime(90);
                    pGem->Refresh();
                }
                break;
            case SAY_PRIESTESS_ALTAR_9:
                // move near the escort npc
                if (Creature* pPriestess = m_creature->GetMap()->GetCreature(m_firstPriestessGuid))
                    pPriestess->GetMotionMaster()->MovePoint(0, aWingThicketLocations[6].m_fX, aWingThicketLocations[6].m_fY, aWingThicketLocations[6].m_fZ);
                break;
            case SAY_PRIESTESS_ALTAR_13:
                // summon the Guardian of Elune
                if (Creature* pGuard = m_creature->SummonCreature(NPC_GUARDIAN_ELUNE, aWingThicketLocations[2].m_fX, aWingThicketLocations[2].m_fY, aWingThicketLocations[2].m_fZ, aWingThicketLocations[2].m_fO, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    pGuard->GetMotionMaster()->MovePoint(0, aWingThicketLocations[5].m_fX, aWingThicketLocations[5].m_fY, aWingThicketLocations[5].m_fZ);
                    m_guardEluneGuid = pGuard->GetObjectGuid();
                }
                // summon the Voice of Elune
                if (GameObject* pAltar = m_creature->GetMap()->GetGameObject(m_altarGuid))
                {
                    if (Creature* pVoice = m_creature->SummonCreature(NPC_VOICE_ELUNE, pAltar->GetPositionX(), pAltar->GetPositionY(), pAltar->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000))
                        m_voiceEluneGuid = pVoice->GetObjectGuid();
                }
                break;
            case SAY_VOICE_ALTAR_15:
                // move near the escort npc and continue dialogue
                if (Creature* pPriestess = m_creature->GetMap()->GetCreature(m_secondPriestessGuid))
                {
                    DoScriptText(SAY_PRIESTESS_ALTAR_14, pPriestess);
                    pPriestess->GetMotionMaster()->MovePoint(0, aWingThicketLocations[7].m_fX, aWingThicketLocations[7].m_fY, aWingThicketLocations[7].m_fZ);
                }
                break;
            case SAY_PRIESTESS_ALTAR_19:
                // make the voice of elune leave
                if (Creature* pGuard = m_creature->GetMap()->GetCreature(m_guardEluneGuid))
                {
                    pGuard->GetMotionMaster()->MovePoint(0, aWingThicketLocations[2].m_fX, aWingThicketLocations[2].m_fY, aWingThicketLocations[2].m_fZ);
                    pGuard->ForcedDespawn(4000);
                }
                break;
            case SAY_PRIESTESS_ALTAR_20:
                // make the first priestess leave
                if (Creature* pPriestess = m_creature->GetMap()->GetCreature(m_firstPriestessGuid))
                {
                    pPriestess->GetMotionMaster()->MovePoint(0, aWingThicketLocations[0].m_fX, aWingThicketLocations[0].m_fY, aWingThicketLocations[0].m_fZ);
                    pPriestess->ForcedDespawn(4000);
                }
                break;
            case SAY_PRIESTESS_ALTAR_21:
                // make the second priestess leave
                if (Creature* pPriestess = m_creature->GetMap()->GetCreature(m_secondPriestessGuid))
                {
                    pPriestess->GetMotionMaster()->MovePoint(0, aWingThicketLocations[1].m_fX, aWingThicketLocations[1].m_fY, aWingThicketLocations[1].m_fZ);
                    pPriestess->ForcedDespawn(4000);
                }
                break;
            case DATA_EVENT_END:
                // Turn towards the player
                if (Player* pPlayer = GetPlayerForEscort())
                {
                    m_creature->SetFacingToObject(pPlayer);
                    DoScriptText(SAY_QUEST_END_1, m_creature, pPlayer);
                }
                break;
            case SAY_QUEST_END_2:
                // Turn towards the altar and kneel - quest complete
                if (GameObject* pAltar = m_creature->GetMap()->GetGameObject(m_altarGuid))
                    m_creature->SetFacingToObject(pAltar);
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                if (Player* pPlayer = GetPlayerForEscort())
                    pPlayer->GroupEventHappens(QUEST_GUARDIANS_ALTAR, m_creature);
                break;
        }
    }

    Creature* GetSpeakerByEntry(uint32 uiEntry)
    {
        switch (uiEntry)
        {
            case NPC_RANSHALLA:        return m_creature;
            case NPC_VOICE_ELUNE:      return m_creature->GetMap()->GetCreature(m_voiceEluneGuid);
            case NPC_PRIESTESS_DATA_1: return m_creature->GetMap()->GetCreature(m_firstPriestessGuid);
            case NPC_PRIESTESS_DATA_2: return m_creature->GetMap()->GetCreature(m_secondPriestessGuid);

            default:
                return NULL;
        }

    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        DialogueUpdate(uiDiff);

        if (m_uiDelayTimer)
        {
            if (m_uiDelayTimer <= uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                SetEscortPaused(false);
                m_uiDelayTimer = 0;
            }
            else
                m_uiDelayTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_ranshalla(Creature* pCreature)
{
    return new npc_ranshallaAI(pCreature);
}

bool QuestAccept_npc_ranshalla(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_GUARDIANS_ALTAR)
    {
        DoScriptText(SAY_QUEST_START, pCreature);
        pCreature->setFaction(FACTION_ESCORT_A_NEUTRAL_PASSIVE);

        if (npc_ranshallaAI* pEscortAI = dynamic_cast<npc_ranshallaAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest, true);

        return true;
    }

    return false;
}

bool GOUse_go_elune_fire(Player* pPlayer, GameObject* pGo)
{
    // Check if we are using the torches or the altar
    bool bIsAltar = false;

    if (pGo->GetEntry() == GO_ELUNE_ALTAR)
        bIsAltar = true;

    if (Creature* pRanshalla = GetClosestCreatureWithEntry(pGo, NPC_RANSHALLA, 10.0f))
    {
        if (npc_ranshallaAI* pEscortAI = dynamic_cast<npc_ranshallaAI*>(pRanshalla->AI()))
            pEscortAI->DoContinueEscort(bIsAltar);
    }

    return false;
}

void AddSC_winterspring()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_lorax";
    pNewScript->pGossipHello = &GossipHello_npc_lorax;
    pNewScript->pGossipSelect = &GossipSelect_npc_lorax;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_rivern_frostwind";
    pNewScript->pGossipHello = &GossipHello_npc_rivern_frostwind;
    pNewScript->pGossipSelect = &GossipSelect_npc_rivern_frostwind;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_witch_doctor_mauari";
    pNewScript->pGossipHello = &GossipHello_npc_witch_doctor_mauari;
    pNewScript->pGossipSelect = &GossipSelect_npc_witch_doctor_mauari;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_ranshalla";
    pNewScript->GetAI = &GetAI_npc_ranshalla;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_ranshalla;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_elune_fire";
    pNewScript->pGOUse = &GOUse_go_elune_fire;
    pNewScript->RegisterSelf();
}
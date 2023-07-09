#include "StdAfx.h"
#include "User.h"
#include "defineText.h"
#include "DPSrvr.h"
#include "FunnyCoin.h"
#include "ScriptHelper.h"
#include "DPDatabaseClient.h"
#include "InstanceDungeonBase.h"
#include "FuncTextCmd.h"
#include "honor.h"
#include "worldmng.h"
#include "DPCoreClient.h"
#include "defineObj.h"

static ItemProp * GetDevirtualizedItemPropOf(const CItemElem & pItemBase) {
	ItemProp * pItemProp = pItemBase.GetProp();
	if (!pItemProp) return nullptr;

	if (pItemProp->dwItemKind3 != IK3_LINK) return pItemProp;
	return prj.GetItemProp(pItemProp->dwActiveSkill);
}

bool CUser::DoUseItem(DWORD dwData, DWORD dwFocusId, int nPart) {
	if (IsDie()) return false;

#ifdef __S_SERVER_UNIFY
	if (m_bAllAction == FALSE) return false;
#endif // __S_SERVER_UNIFY

	const WORD dwId = HIWORD(dwData);

	CItemElem * pItemBase = GetItemId(dwId);
	if (!IsUsableItem(pItemBase)) return false;

	ItemProp * const pItemProp = GetDevirtualizedItemPropOf(*pItemBase);
	if (!pItemProp) return false;

	CItemElem & itemBase = *pItemBase;
	const ItemProp & itemProp = *pItemProp;

	const DWORD dwItemKind2 = pItemProp->dwItemKind2;
	const DWORD dwItemKind3 = pItemProp->dwItemKind3;

	// �������ʿ��� ������ ���
	CWorld * pWorld = GetWorld();
	if (pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR) {
		if (m_nGuildCombatState == 0) {
			AddText(prj.GetText(TID_GAME_GUILDCOMBAT_STANDS_NOTUSEITEM));	//"�����ؾ��� : ���߼������� �������� ����Ҽ� �����ϴ�" );
			return false;
		}
	}

	if (GetAdjParam(DST_CHRSTATE) & CHS_SETSTONE) return false;
	
	if (pItemProp->dwParts != NULL_ID) {
		// armor, weapon
		if (m_pActMover->IsActAttack() == FALSE)	// �����߿� ��� ���ٲ�.
		{
			DoUseEquipmentItem(pItemBase, dwId, nPart);
		}
		// ���� �������� Ż�� �����ؾ��ϹǷ� �Ⱓ ���ῡ ���� ��� ���� ����.
	
		return true;
	}
	
	
	// �Ϲ����� ������ ��� 
	if (!IsItemRedyTime(pItemProp, pItemBase->m_dwObjId, TRUE)) return false;

	const auto cooldownType = m_cooltimeMgr.CanUse(*pItemProp);
	if (cooldownType == CCooltimeMgr::CooldownType::OnCooldown) return false;

	// �� ���ȭ ������
	switch (dwItemKind2) {
		case IK2_GUILDHOUSE_FURNITURE:
		case IK2_GUILDHOUSE_NPC:
		case IK2_GUILDHOUSE_PAPERING:
		{
			if (GuildHouseMng->SendWorldToDatabase(this, GUILDHOUSE_PCKTTYPE_LISTUP, GH_Fntr_Info(pItemProp->dwID)))
				g_DPSrvr.PutItemLog(this, "f", "GUILDHOUSE_LISTUP", pItemBase, 1);
			else
				return false;
			break;
		}
		case IK2_GUILDHOUES_COMEBACK:
		{
			if (!GuildHouseMng->EnteranceGuildHouse(this, pItemProp->dwID))
				return false;
			break;
		}
		case IK2_FURNITURE:
		case IK2_PAPERING:
		{
			if (CHousingMng::GetInstance()->ReqSetFurnitureList(this, pItemBase->m_dwItemId)) {
				// �Ͽ�¡ ����Ʈ �߰� �α�
				LogItemInfo aLogItem;
				aLogItem.SendName = GetName();
				aLogItem.RecvName = "HOUSING_USE";
				aLogItem.WorldId = GetWorld()->GetID();
				aLogItem.Gold = GetGold();
				aLogItem.Gold2 = GetGold();
				aLogItem.Action = "f";
				g_DPSrvr.OnLogItem(aLogItem, pItemBase);
			} else
				return false;
			break;
		}
#ifdef __FUNNY_COIN
		case IK2_TOCASH:
		{
			if (!CFunnyCoin::GetInstance()->DoUseFunnyCoin(this, pItemBase))
				return false;
		}
		break;
#endif // __FUNNY_COIN
		case IK2_WARP:
			return DoUseItemWarp(*pItemProp, *pItemBase);
		case IK2_BUFF2:
		{
			if (IsDoUseBuff(pItemProp) != 0)
				return FALSE;
			CTime tm = CTime::GetCurrentTime() + CTimeSpan(0, 0, pItemProp->dwAbilityMin, 0);
			time_t t = (time_t)(tm.GetTime());
			// wID: dwItemId
			// dwLevel
			AddBuff(BUFF_ITEM2, (WORD)(pItemBase->m_dwItemId), t, 0);
			break;
		}
#ifdef __BUFF_TOGIFT
		case IK2_BUFF_TOGIFT:
#endif // __BUFF_TOGIFT
		case IK2_BUFF:
		{
			if (IsDoUseBuff(pItemProp) != 0)
				return FALSE;
			DoApplySkill(this, pItemProp, NULL);		// ��󿡰� ������ȿ���� ������. 
			if (pItemProp->dwItemKind3 == IK3_ANGEL_BUFF) {
#ifdef __EXP_ANGELEXP_LOG
				m_nAngelExpLog = 0;
#endif // __EXP_ANGELEXP_LOG
#ifdef __ANGEL_LOG
				LogItemInfo aLogItem;
				aLogItem.Action = "&";
				aLogItem.SendName = GetName();
				aLogItem.RecvName = "ANGEL_SUMMON";
				aLogItem.WorldId = GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = GetGold();
				//aLogItem.ItemName = pItemProp->szName;
				_stprintf(aLogItem.szItemName, "%d", pItemProp->dwID);
				g_DPSrvr.OnLogItem(aLogItem);
#endif // __ANGEL_LOG
				m_nAngelExp = 0;
				m_nAngelLevel = GetLevel() + 1;
				AddAngelInfo();
			}
		}
		break;
		case IK2_TEXT: //�ؽ�Ʈ ���� ó�� 
			// ����Ʈ�� ������ ����Ʈ ���� 
			if (pItemProp->dwQuestId != QuestIdNone && pItemBase->m_bItemResist == FALSE) {
				::__SetQuest(GetId(), pItemProp->dwQuestId.get());
				UpdateItem(*pItemBase, UI::Element::ActivatedQuest);
			}
			break;
		case IK2_SYSTEM:
		{
#ifdef __JEFF_9_20
#ifdef __WORLDSERVER
			if (pItemProp->dwLimitLevel1 != 0xFFFFFFFF) {
				if (GetLevel() < (int)(pItemProp->dwLimitLevel1)) {
					AddDefinedText(TID_GAME_ITEM_LEVEL, "\"%d\"", pItemProp->dwLimitLevel1);
					return FALSE;
				}
			}
#endif	// __WORLDSERVER
#endif	// __JEFF_9_20
			DoUseSystemAnswer nResult = DoUseItemSystem(pItemProp, pItemBase, nPart);
			{
				if (nResult != DoUseSystemAnswer::Ok) {
					if (nResult == DoUseSystemAnswer::LimitedUse) {
						AddDefinedText(TID_GAME_LIMITED_USE, ""); // ������̾ ����Ҽ� ����
					} else if (nResult == DoUseSystemAnswer::NoOverlap) {
						AddDefinedText(TID_GAME_NOTOVERLAP_USE, ""); // �� �����۰��� �ߺ��Ͽ� ����� �� �����ϴ�
					}
					return FALSE;
				}
			}
		}
		break;

		case IK2_BLINKWING:
			return DoUseItemBlinkWing(pItemProp, pItemBase);

		case IK2_REFRESHER:
		{
			if (pItemProp->dwCircleTime != (DWORD)-1) {
				if (IsSMMode(SM_MAINTAIN_MP) == FALSE && SetSMMode(SM_MAINTAIN_MP, pItemProp->dwCircleTime)) {
					SetPointParam(DST_MP, GetMaxManaPoint());
				} else {
					AddDefinedText(TID_GAME_LIMITED_USE, ""); //   
					return false;
				}
				g_dpDBClient.SendLogSMItemUse("1", this, pItemBase, pItemProp);
			} else {
				if (!DoUseItemFood(*pItemProp)) return false;
			}
			break;
		}
		case IK2_POTION:
			if (pItemProp->dwCircleTime != (DWORD)-1)	// ���������� dwCircleTime�� �����. ���߿� dwSkillTime���� ���տ���.
			{
				if (IsSMMode(SM_MAINTAIN_FP) == FALSE && SetSMMode(SM_MAINTAIN_FP, pItemProp->dwCircleTime)) {
					SetPointParam(DST_FP, GetMaxFatiguePoint());
				} else {
					AddDefinedText(TID_GAME_LIMITED_USE, ""); //   
					return FALSE;
				}
				g_dpDBClient.SendLogSMItemUse("1", this, pItemBase, pItemProp);
			} else {
				if (!DoUseItemFood(*pItemProp)) return false;
			}
			break;
		case IK2_FOOD:
			if (!DoUseItemFood(*pItemProp)) return false;
			break;
		case IK2_AIRFUEL:	// ���࿬���
		{
			ItemProp * pRideProp = prj.GetItemProp(m_dwRideItemIdx);	// ���ڷ� ������Ƽ
			if (pRideProp && m_pActMover->IsFly()) {
				if (pItemProp->dwItemKind3 == IK3_ACCEL)	// ���� ����
				{
					m_tmAccFuel = pItemProp->dwAFuelReMax * 1000;;	// ���ʰ� �����Ҽ� �ֳ�.
				} else {	// ���࿬��
					if (pRideProp->dwItemKind3 == IK3_BOARD && pItemProp->dwItemKind3 == IK3_CFLIGHT)	// Ÿ���ִ°� ���� / ����Ϸ��°� ���ڷ��
					{
						AddDefinedText(TID_GAME_COMBFUEL, "");	// �̰� ���ڷ���̿�~
						return FALSE;
					} else
						if (pRideProp->dwItemKind3 == IK3_STICK && pItemProp->dwItemKind3 == IK3_BFLIGHT)	// Ÿ���ִ°� ���ڷ� / ����Ϸ��°� �����
						{
							AddDefinedText(TID_GAME_BOARDFUEL, "");	// �̰� ������̿�~
							return FALSE;
						}

					m_nFuel += pItemProp->dwFuelRe;		// ������.
					if ((DWORD)(m_nFuel) >= pRideProp->dwFFuelReMax)	// ���ڷ� �����뺸�� ũ�� ��.
						m_nFuel = pRideProp->dwFFuelReMax;
				}
				AddSetFuel(m_nFuel, m_tmAccFuel);		// ��ȭ�� ���� ����. - SetPointParam���� ���ĵ� ��������?
			} else
				return FALSE;
		}
		break;

		case IK2_GMTEXT:
		{
			BOOL bGMTime = TRUE;
			if (pItemProp->dwSkillTime != NULL_ID)		// ���ӽð��� �ִ� ������
			{
				if (pItemProp->dwItemKind3 == IK3_TEXT_DISGUISE)	// ���� �������� �����߿��� ����Ҽ� ����.
				{
					if (IsFly()) {
						AddDefinedText(TID_PK_FLIGHT_NOUSE, ""); // ���� �߿��� ����� �� �����ϴ�.
						bGMTime = FALSE;
					} else if (CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon(GetWorld()->GetID())) {
						AddDefinedText(TID_GAME_INSTANCE_DISGUISE01);
						bGMTime = FALSE;
					}
				}
				if (bGMTime) {
					if (!HasBuffByIk3(pItemProp->dwItemKind3)) {
						DoApplySkill(this, pItemProp, NULL);		// ��󿡰� ������ȿ���� ������. 
					} else {
						AddDefinedText(TID_GAME_LIMITED_USE, ""); // ������̾ ����Ҽ� ����
						bGMTime = FALSE;
					}
				}
			}

			if (pItemProp->dwID == II_SYS_SYS_GM_NODISGUISE) {
				if (HasBuffByIk3(IK3_TEXT_DISGUISE))
					RemoveIk3Buffs(IK3_TEXT_DISGUISE);
				else
					bGMTime = FALSE;
			}

			if (bGMTime) {
				if (pItemProp->dwID != II_SYS_SYS_GM_NODISGUISE) {
					char szGMCommand[64] = { 0, };
					CString szGMText = pItemProp->szTextFileName;
					szGMText.Replace('(', '\"');
					szGMText.Replace(')', '\"');
					int nGMCount = szGMText.Find("/", 1);
					if (nGMCount != -1)
						strncpy(szGMCommand, szGMText, nGMCount);
					else
						strcpy(szGMCommand, szGMText);

					g_textCmdFuncs.ParseCommand(szGMCommand, this, TRUE);
				}
			} else
				return FALSE;
		}
		break;
		case IK2_SKILL:
		{
			BOOL bUseItem = FALSE;
			DWORD dwActiveSkill = pItemProp->dwActiveSkill;
			if (dwActiveSkill != NULL_ID)		// �߰� �ߵ� ��ų�� �ִ�.
			{
				if (pItemProp->dwActiveSkillRate == NULL_ID ||
					xRandom(100) < pItemProp->dwActiveSkillRate)	// �ߵ�Ȯ���� �ɷȴ°�.
				{
					ItemProp * pSkillProp;
					pSkillProp = (ItemProp *)prj.GetSkillProp(dwActiveSkill);
					if (pSkillProp) {
						if (pSkillProp->dwUseChance == WUI_TARGETINGOBJ)	// Ÿ�ٿ��� ����ϴ� ���.
						{
							if (IsPlayer()) {
								DWORD idTarget = m_idSetTarget;
								CMover * pTarget = prj.GetMover(idTarget);
								if (IsValidObj(pTarget)) {
									DoActiveSkill(dwActiveSkill, pItemProp->dwActiveSkillLv, pTarget);	// dwActiveSkill �ߵ�.
								}
							}
						} else
							if (pSkillProp->dwUseChance == WUI_NOW)		// �ڽſ��Ը� ����ϴ� ���.
							{
								DoActiveSkill(dwActiveSkill, pItemProp->dwActiveSkillLv, this);	// �ߵ�
							}
					} else
						Error("DoUseItem, IK2_SKILL, item %s -> %d skill not prop", pItemProp->szName, dwActiveSkill);
				}
			}
		}
		break;
		default:
		{
			if (pItemProp->dwSkillTime != NULL_ID)		// ���ӽð��� �ִ� ������
			{
				DoApplySkill(this, pItemProp, NULL);		// ��󿡰� ������ȿ���� ������. 
			}
		}
	} // switch ik2

	switch (dwItemKind3) {
		case IK3_EGG:
			DoUseSystemPet(pItemBase);
			break;
		case IK3_PET:
			DoUseEatPet(pItemBase);
			break;
	}

	OnAfterUseItem(pItemProp);	// raiders 06.04.20
	pItemBase->UseItem();			// --m_nItemNum;

	UI::Num operation = UI::Num::Sync(*pItemBase);
	if (cooldownType == CCooltimeMgr::CooldownType::Available)	// ��Ÿ�� �������̸� ���ð��� ����Ѵ�.
	{
		m_cooltimeMgr.StartCooldown(*pItemProp);
		operation.startCooldown = true;
	}

	UpdateItem(*pItemBase, operation);	// ������ 0�̸�  ������ ���� , ���� 
	
	return true;
}


CUser::DoUseSystemAnswer CUser::DoUseItemSystem(ItemProp * pItemProp, CItemElem * pItemElem, int nPart) {
	DoUseSystemAnswer nResult = DoUseSystemAnswer::Ok;
	
#ifdef __AZRIA_1023
	if (HasInput()) return DoUseItemInput(pItemProp, pItemElem);
#endif	// __AZRIA_1023

	switch (pItemProp->dwID) {
		case II_CHR_SYS_SCR_RESTATE:
		{
			if (m_nInt == 15 && m_nSta == 15 && m_nStr == 15 && m_nDex == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReState();
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_STR:
		{
			if (m_nStr == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOne(0);
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_STA:
		{
			if (m_nSta == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOne(1);
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_DEX:
		{
			if (m_nDex == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOne(2);
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_INT:
		{
			if (m_nInt == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOne(3);
		}
		break;
		case II_CHR_SYS_SCR_TRANSY:
		{
			if (DoUseItemSexChange(nPart)) {
				if (HasBuffByIk3(IK3_TEXT_DISGUISE)) {
					RemoveIk3Buffs(IK3_TEXT_DISGUISE);
				}
			} else {
				nResult = DoUseSystemAnswer::SilentError;
			}
		}
		break;
		case II_SYS_SYS_SCR_PET_LIFE:
		case II_SYS_SYS_SCR_PET_LIFE02:
		{
			CPet * pPet = GetPet();
			if (pPet == NULL) {
				AddDefinedText(TID_GAME_PET_NOT_FOUND);
				nResult = DoUseSystemAnswer::SilentError;
				break;
			}

			if (pPet->GetLevel() == PL_EGG) {
				AddDefinedText(TID_GAME_PET_CANT_USE_TO_EGG);
				nResult = DoUseSystemAnswer::SilentError;
				break;
			}

			if (pPet->GetLife() >= MAX_PET_LIFE) {
				nResult = DoUseSystemAnswer::SilentError;
				break;
			}

			WORD wLife = pPet->GetLife();
			wLife += (pItemProp->dwID == II_SYS_SYS_SCR_PET_LIFE ? CPetProperty::GetInstance()->GetAddLife() : 1);

			pPet->SetLife(wLife > MAX_PET_LIFE ? MAX_PET_LIFE : wLife);

			CItemElem * pItemElem = GetPetItem();
			AddPetState(pItemElem->m_dwObjId, pPet->GetLife(), pPet->GetEnergy(), pPet->GetExp());
			g_dpDBClient.CalluspPetLog(m_idPlayer, pItemElem->GetSerialNumber(), wLife, PETLOGTYPE_LIFE, pPet);
			break;
		}
		case II_SYS_SYS_SCR_PET_HATCH:
		{
			CPet * pPet = GetPet();
			if (pPet == NULL) {
				AddDefinedText(TID_GAME_PET_NOT_FOUND);
				nResult = DoUseSystemAnswer::SilentError;
				break;
			}
			if (pPet->GetLevel() != PL_EGG) {
				AddDefinedText(TID_GAME_PET_4EGG);
				nResult = DoUseSystemAnswer::SilentError;
				break;
			}
			PetLevelup();
			break;
		}
		case II_GEN_TOO_COL_NORMALBATTERY:
		{
			if (!DoUseItemBattery()) {
				AddDefinedText(TID_GAME_CANT_USE_BATTERY);
				nResult = DoUseSystemAnswer::SilentError;
			}
			break;
		}
		case II_CHR_SYS_SCR_SHOUTFULL15:
		{
			if (!IsShoutFull())
				SetSMMode(SM_SHOUT15, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_SHOUTFULL30:
		{
			if (!IsShoutFull())
				SetSMMode(SM_SHOUT30, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_SHOUTFULL001:
		{
			if (!IsShoutFull())
				SetSMMode(SM_SHOUT001, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_PSKILLFULL1:
		{
			if (!(IsSMMode(SM_PARTYSKILL1) || IsSMMode(SM_PARTYSKILL15) || IsSMMode(SM_PARTYSKILL30))) {
				SetSMMode(SM_PARTYSKILL1, pItemProp->dwCircleTime);
				g_DPCoreClient.SendUserPartySkill(m_idPlayer, PARTY_PARSKILL_MODE, 1000, 0, 1);
			} else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_PSKILLFULL15:
		{
			if (!(IsSMMode(SM_PARTYSKILL1) || IsSMMode(SM_PARTYSKILL15) || IsSMMode(SM_PARTYSKILL30))) {
				SetSMMode(SM_PARTYSKILL15, pItemProp->dwCircleTime);
				g_DPCoreClient.SendUserPartySkill(m_idPlayer, PARTY_PARSKILL_MODE, 1000, 0, 1);
			} else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_PSKILLFULL30:
		{
			if (!(IsSMMode(SM_PARTYSKILL1) || IsSMMode(SM_PARTYSKILL15) || IsSMMode(SM_PARTYSKILL30))) {
				SetSMMode(SM_PARTYSKILL30, pItemProp->dwCircleTime);
				g_DPCoreClient.SendUserPartySkill(m_idPlayer, PARTY_PARSKILL_MODE, 1000, 0, 1);
			} else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_COMMBANK15:
		{
			if (!IsCommBank())
				SetSMMode(SM_BANK15, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_COMMBANK30:
		{
			if (!IsCommBank())
				SetSMMode(SM_BANK30, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_COMMBANK001:
		{
			if (!IsCommBank())
				SetSMMode(SM_BANK001, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_ACTIVITION:
		{
			if (!(IsSMMode(SM_ACTPOINT)))
				SetSMMode(SM_ACTPOINT, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_UPCUTSTONE01:
		{
			if (!(IsSMMode(SM_ATTACK_UP1) || IsSMMode(SM_ATTACK_UP)))
				SetSMMode(SM_ATTACK_UP1, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_CHR_SYS_SCR_UPCUTSTONE:
		{
			if (!(IsSMMode(SM_ATTACK_UP1) || IsSMMode(SM_ATTACK_UP))) {
				SetSMMode(SM_ATTACK_UP, pItemProp->dwCircleTime);
			} else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_SYS_SYS_SCR_BLESSING:
		{
			if (!(IsSMMode(SM_REVIVAL)))
				SetSMMode(SM_REVIVAL, pItemProp->dwCircleTime);
			else
				nResult = DoUseSystemAnswer::LimitedUse;
		}
		break;
		case II_SYS_SYS_SCR_SELPAR:
		{
			CParty * pParty = g_PartyMng.GetParty(GetPartyId());
			if (pParty && pParty->m_nLevel < MAX_PARTYLEVEL) {
				// �ܸ��ش��϶��� ��밡��
					pParty->SetPartyLevel(this, 10, 180, 0);
			} else {
				nResult = DoUseSystemAnswer::SilentError;
				AddDefinedText(TID_GAME_NTROUPEO10, "");
			}
		}
		break;
		case II_SYS_SYS_SCR_HOLY:
			RemoveDebufBuffs();
			break;
		case II_SYS_SYS_SCR_VELOCIJUMP:
		{
			if (!(IsSMMode(SM_VELOCIJUMP))) {
				if (pItemProp->dwDestParam1 != -1)
					SetDestParam(pItemProp->dwDestParam1, pItemProp->nAdjParamVal1, pItemProp->nAdjParamVal1);
				if (pItemProp->dwDestParam2 != -1)
					SetDestParam(pItemProp->dwDestParam2, pItemProp->nAdjParamVal2, pItemProp->nAdjParamVal2);
				SetSMMode(SM_VELOCIJUMP, pItemProp->dwCircleTime);
			} else {
				nResult = DoUseSystemAnswer::LimitedUse;
			}
		}
		break;
		case II_SYS_SYS_SCR_RECCURENCE:
		{
			if (InitSkillExp())
				AddInitSkill();
			else
				nResult = DoUseSystemAnswer::SilentError;
		}
		break;

		case II_SYS_SYS_SCR_AMPES:	// "ES������ �η縶��"		
		{
			// ES, EM�� ���Ͼ� ������.. I�� ���� ����Ҽ� �����ϴ�.
			if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPES) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPEM) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA2) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB2) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC2) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD2) ||
#ifdef __S_ADD_EXP
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS2) ||
#endif // __S_ADD_EXP
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESE)
				)
				nResult = DoUseSystemAnswer::LimitedUse;
			else if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPI))
				nResult = DoUseSystemAnswer::NoOverlap;
			else
				DoApplySkill(this, pItemProp, NULL);
		}
		break;
		case II_SYS_SYS_SCR_AMPESA:
		case II_SYS_SYS_SCR_AMPESB:
		case II_SYS_SYS_SCR_AMPESC:
		case II_SYS_SYS_SCR_AMPESD:
		case II_SYS_SYS_SCR_AMPESE:
#ifdef __S_ADD_EXP
		case II_SYS_SYS_SCR_AMPESS:
#endif // __S_ADD_EXP
		{
			if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPES) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPEM))
				nResult = DoUseSystemAnswer::LimitedUse;
			else if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPI))
				nResult = DoUseSystemAnswer::NoOverlap;

			// ���� �˻�
			int nLevel = GetLevel();

			CString str;
			if (pItemProp->dwID == II_SYS_SYS_SCR_AMPESA) {
				if (20 < nLevel) {
					str.Format("%d - %d", 1, 20);
					AddDefinedText(TID_GAME_EXP_NOTUSELEVEL, "\"%s\"", str); // 
					nResult = DoUseSystemAnswer::SilentError99;
				}
			} else if (pItemProp->dwID == II_SYS_SYS_SCR_AMPESB) {
				if (nLevel < 21 || 40 < nLevel) {
					str.Format("%d - %d", 21, 40);
					AddDefinedText(TID_GAME_EXP_NOTUSELEVEL, "\"%s\"", str); // 
					nResult = DoUseSystemAnswer::SilentError99;
				}
			} else if (pItemProp->dwID == II_SYS_SYS_SCR_AMPESC) {
				if (nLevel < 41 || 60 < nLevel) {
					str.Format("%d - %d", 41, 60);
					AddDefinedText(TID_GAME_EXP_NOTUSELEVEL, "\"%s\"", str); // 
					nResult = DoUseSystemAnswer::SilentError99;
				}
			} else if (pItemProp->dwID == II_SYS_SYS_SCR_AMPESD) {
				if (nLevel < 61 || 80 < nLevel) {
					str.Format("%d - %d", 61, 80);
					AddDefinedText(TID_GAME_EXP_NOTUSELEVEL, "\"%s\"", str); // 
					nResult = DoUseSystemAnswer::SilentError99;
				}
			} else if (pItemProp->dwID == II_SYS_SYS_SCR_AMPESE) {
				if (nLevel < 81 || 120 < nLevel) {
					str.Format("%d - %d", 81, 120);
					AddDefinedText(TID_GAME_EXP_NOTUSELEVEL, "\"%s\"", str); // 
					nResult = DoUseSystemAnswer::SilentError99;
				}
			}

			if (!(nResult == DoUseSystemAnswer::LimitedUse
				|| nResult == DoUseSystemAnswer::NoOverlap
				|| nResult == DoUseSystemAnswer::SilentError99)) {
				BOOL bBuf[3];
				int nBufCount = 0;
				BOOL bSpecial = FALSE;
				int nSpecialCount = 0;

				ZeroMemory(bBuf, sizeof(bBuf));
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA)) {
					bBuf[0] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA1)) {
					bBuf[1] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA2)) {
					bBuf[2] = TRUE;		++nBufCount;
				}

				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB)) {
					bBuf[0] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB1)) {
					bBuf[1] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB2)) {
					bBuf[2] = TRUE;		++nBufCount;
				}

				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC)) {
					bBuf[0] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC1)) {
					bBuf[1] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC2)) {
					bBuf[2] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD)) {
					bBuf[0] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD1)) {
					bBuf[1] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD2)) {
					bBuf[2] = TRUE;		++nBufCount;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESE)) {
					bBuf[0] = TRUE;		++nBufCount;
				}
#ifdef __S_ADD_EXP
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS)) {
					bBuf[0] = TRUE;		++nBufCount;	nSpecialCount++;//bSpecial = TRUE;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS1)) {
					bBuf[1] = TRUE;		++nBufCount;	nSpecialCount++;//bSpecial = TRUE;
				}
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS2)) {
					bBuf[2] = TRUE;		++nBufCount;	nSpecialCount++;//bSpecial = TRUE;
				}
#endif // __S_ADD_EXP

				int nMaxBuf = 3;
				//if( bSpecial == FALSE )
				{
					if (nLevel < 41)
						nMaxBuf = 3;
					else if (nLevel < 61 && nSpecialCount < 2)
						nMaxBuf = 2;
					else if (nLevel <= 120 && nSpecialCount < 1)
						nMaxBuf = 1;
					else
						nMaxBuf = 3;
				}

#ifdef __S_ADD_EXP
				//if( bSpecial && pItemProp->dwID != II_SYS_SYS_SCR_AMPESS )
				if (nSpecialCount && pItemProp->dwID != II_SYS_SYS_SCR_AMPESS) {
					nResult = DoUseSystemAnswer::LimitedUse;
				} else
#endif // __S_ADD_EXP
					if (nBufCount < nMaxBuf) {
						for (int i = 0; i < nMaxBuf; ++i) {
							if (bBuf[i] == FALSE) {
								if (i == 0)
									pItemProp = prj.GetItemProp(pItemProp->dwID);
								else if (i == 1)
									pItemProp = prj.GetItemProp(pItemProp->dwID + 1);
								else
									pItemProp = prj.GetItemProp(pItemProp->dwID + 2);
								break;
							}
						}

						DoApplySkill(this, pItemProp, nullptr);
						float fFactor = 1;
						switch (nBufCount + 1) {
							case 1:
							{
								fFactor *= 1.5f;
							}
							break;
							case 2:
							{
								//if( bSpecial || nLevel < 61 )
								if (nSpecialCount || nLevel < 61)
									fFactor *= 2.0f;
								else
									fFactor *= 1.5f;
							}
							break;
							case 3:
							{
								//if( bSpecial || nLevel < 41 )
								if (nSpecialCount || nLevel < 41)
									fFactor *= 2.5f;
								else if (nLevel < 61)
									fFactor *= 2.0f;
								else
									fFactor *= 1.5f;
							}
							break;
						}
						CString strTemp;
						int nPercent = (int)((fFactor - 1.0f) * 100.0f);
						strTemp.Format("%s %d%% %s", prj.GetText(TID_GAME_EXPITEM_TOOLTIP), nPercent, prj.GetText(TID_GAME_EXPITEM_TOOLTIP1));
						((CUser *)this)->AddText(strTemp); // 
					} else {
						{
							if (nLevel < 21)
								str.Format("%d - %d", 1, 20);
							else if (nLevel < 41)
								str.Format("%d - %d", 21, 40);
							else if (nLevel < 61)
								str.Format("%d - %d", 41, 60);
							else if (nLevel < 81)
								str.Format("%d - %d", 61, 80);
							else if (nLevel <= 120)
								str.Format("%d - %d", 81, 120);

							CString szMaxBuf;
							szMaxBuf.Format("%d", nMaxBuf);
							AddDefinedText(TID_GAME_EXP_NOTUSE, "\"%s\" \"%s\"", str, szMaxBuf); // 
						}
						nResult = DoUseSystemAnswer::SilentError99;
					}
			}
		}
		break;
		case II_SYS_SYS_SCR_AMPEM:	// "EM������ �η縶��"		
		{
			// ES, EM�� ���Ͼ� ������.. I�� ���� ����Ҽ� �����ϴ�.
			if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPES) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPEM) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESA2) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESB2) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESC2) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESD2) ||
#ifdef __S_ADD_EXP
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS1) || HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESS2) ||
#endif // __S_ADD_EXP
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPESE)
				)
				nResult = DoUseSystemAnswer::LimitedUse;
			else if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_AMPI))
				nResult = DoUseSystemAnswer::NoOverlap;
			else
				DoApplySkill(this, pItemProp, nullptr);
		}
		break;
#ifdef __Y_BEAUTY_SHOP_CHARGE
		case II_SYS_SYS_SCR_BEAUTYSHOP: // "Beauty Shop Charge"		
		{
		}
		break;
#endif // __Y_BEAUTY_SHOP_CHARGE
		case II_SYS_SYS_SCR_GLVCHEER:	// ������ �� �尩
		case II_SYS_SYS_SCR_SMELPROT:	// "��ȣ�� �η縶��"		
#ifdef __SM_ITEM_2ND_EX
		case II_SYS_SYS_SCR_SMELPROT2:
#endif	// __SM_ITEM_2ND_EX
		case II_SYS_SYS_SCR_SMELPROT3: // " ��ȣ�� �η縶��(�ֻ��)
		case II_SYS_SYS_SCR_SMELPROT4:	// �Ǽ��縮 ��ȣ�� �η縶��
		case II_SYS_SYS_SCR_SMELTING:	// "������ �η縶��"	
		case II_SYS_SYS_SCR_SUPERLEADERPARTY:	// "������ �ູ"		
		case II_SYS_SYS_SCR_SUPERSMELTING:	// "���� �η縶��(��)"
		case II_SYS_SYS_SCR_FONTEDIT:	// "Font Edit"		
#ifdef __Y_BEAUTY_SHOP_CHARGE
		case II_SYS_SYS_SCR_VENDOR:	// "Vendor Charge"		
#endif //__Y_BEAUTY_SHOP_CHARGE
		case II_SYS_SYS_SCR_SKLINSTRUCT:	// "����� ������"
		case II_SYS_SYS_SCR_SMELTING2:	// �Ӽ� ������ �η縶��

		{
			if (HasBuff(BUFF_ITEM, (WORD)(pItemProp->dwID)))
				nResult = DoUseSystemAnswer::LimitedUse;
			else
				DoApplySkill(this, pItemProp, nullptr);
		}
		break;
		//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
#ifdef	__PROTECT_AWAKE
		case	II_SYS_SYS_SCR_AWAKESAFE:
		{
			if (HasBuff(BUFF_ITEM, (WORD)(pItemProp->dwID))) {
				nResult = DoUseSystemAnswer::LimitedUse;
			} else {
				DoApplySkill(this, pItemProp, nullptr);
			}
		}
		break;
#endif	//__PROTECT_AWAKE
		//	mulcom	END100405	���� ��ȣ�� �η縶��


		case II_SYS_SYS_SCR_PET_TONIC_A:
		case II_SYS_SYS_SCR_PET_TONIC_B:
			nResult = DoUseItemPetTonic(pItemElem);	// �� ������ ���
			break;
		case II_SYS_SYS_SCR_PET_FEED_POCKET:
		{
			nResult = DoUseItemFeedPocket(pItemElem);
			break;
		}
#ifdef __EVE_BALLOON
#endif //__EVE_BALLOON
		case II_SYS_SYS_SCR_GET01:
		case II_SYS_SYS_SCR_GET02:
		{
			if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_GET01) ||
				HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_GET02))
				nResult = DoUseSystemAnswer::LimitedUse;
			else
				DoApplySkill(this, pItemProp, nullptr);
		}
		break;
		case II_SYS_SYS_SCR_RETURN:		// "��ȯ�� �η縶��"		
		{
				if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_RETURN))
					nResult = DoUseSystemAnswer::LimitedUse;
				else {
					CWorld * pWorld = GetWorld();
					if (!pWorld) return DoUseSystemAnswer::Ok;

					D3DXVECTOR3 vPos = GetPos();
					//Ŭ����ũ ��/ �ɹٶ󽺿����� �� �������� ����� �� ������ ���� ����Ϸ� �õ� �� '�� ������ ���� �����̹Ƿ� �������� ����� �� �����ϴ�.'��� �޽����� ����Ѵ�
					if (pWorld->GetID() == WI_WORLD_GUILDWAR || pWorld->GetID() == WI_WORLD_KEBARAS || prj.IsGuildQuestRegion(vPos)) {
						AddDefinedText(TID_GAME_LIMITZONE_USE);
						return DoUseSystemAnswer::Ok;
					}

					if (g_GuildCombat1to1Mng.IsPossibleUser((CUser *)this)) {
						AddDefinedText(TID_GAME_LIMITZONE_USE);
						return DoUseSystemAnswer::Ok;
					}

					SetReturnPos(vPos);

					//1.��ȯ���� ���� 
					DoApplySkill(this, pItemProp, nullptr);
					//2. ���õ� ������ �̵� 
					if (m_lpszVillage) {
						if (const REGIONELEM * pRgnElem = g_WorldMng.GetRevivalPos(GetWorld()->GetID(), m_lpszVillage)) {
							Replace(*pRgnElem, REPLACE_NORMAL, nRevivalLayer);
						}
					} else {
						Error("II_SYS_SYS_SCR_RETURN selected village is NULL\n");
					}
				}

		}
		break;
		case II_SYS_SYS_SCR_CUSTODY2:
		{
			CMover * pMover = (CMover *)CreateObj(OT_MOVER, MI_INFO_PENG);
			lstrcpy(pMover->m_szCharacterKey, "MaFl_InstantBank");
			pMover->InitNPCProperty();
			pMover->InitCharacter(pMover->GetCharacter());
			pMover->SetPos(GetPos());
			pMover->InitMotion(MTI_STAND);
			pMover->UpdateLocalMatrix();
			pMover->m_dwTickCreated = GetTickCount();
			GetWorld()->ADDOBJ(pMover, TRUE, GetLayer());
			break;
		}

		case II_SYS_SYS_SCR_FRIENDSUMMON_A:
		case II_SYS_SYS_SCR_FRIENDSUMMON_B:
		{
			const auto nState = GetSummonState();
			if (nState != SummonState::Ok_0) {
				const DWORD dwMsgId = GetSummonStateTIdForMyself(nState);
				AddDefinedText(TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText(dwMsgId));
			} else if (g_GuildCombat1to1Mng.IsPossibleUser((CUser *)this)) {
				AddDefinedText(TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText(TID_GAME_ATTACK_NOTUSE));
			} else {
				AddSummonFriendUse(pItemElem);
			}
			nResult = DoUseSystemAnswer::SilentError99;
		}
		break;

		case II_SYS_SYS_SCR_PERIN:
		{
			const auto nState = GetSummonState();
			if (nState != SummonState::Ok_0) {
				const DWORD dwMsgId = GetSummonStateTIdForMyself(nState);
				AddDefinedText(TID_GAME_STATE_NOTUSE, "\"%s\"", prj.GetText(dwMsgId));
				nResult = DoUseSystemAnswer::SilentError99;
			} else {
				float	fTmpGold = (float)(((CUser *)this)->GetGold());
				if (fTmpGold >= 2000000000 || fTmpGold < 0) {
					AddDiagText(prj.GetText(TID_GAME_TOOMANYMONEY_USE_PERIN));
					nResult = DoUseSystemAnswer::SilentError99;
				} else {
					AddGold(100000000, true);

					// chipi080711 - �丰��� �α�
					LogItemInfo aLogItem;
					aLogItem.Action = "C";
					aLogItem.SendName = GetName();
					aLogItem.RecvName = "USE_PERIN";
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = (DWORD)(fTmpGold);
					aLogItem.Gold2 = GetGold();
					aLogItem.Gold_1 = GetGold() - static_cast<int>(fTmpGold);
					//aLogItem.ItemName = pItemElem->GetName();
					_stprintf(aLogItem.szItemName, "%d", pItemProp->dwID);
					aLogItem.itemNumber = 1;
					aLogItem.ItemNo = pItemElem->GetSerialNumber();
					g_DPSrvr.OnLogItem(aLogItem);
				}
			}
		}
		break;
#ifdef __ADD_RESTATE_LOW
		case II_CHR_SYS_SCR_RESTATE_STR_LOW:
		{
			if (m_nStr == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOneLow(0);
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_STA_LOW:
		{
			if (m_nSta == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOneLow(1);
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_DEX_LOW:
		{
			if (m_nDex == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOneLow(2);
		}
		break;
		case II_CHR_SYS_SCR_RESTATE_INT_LOW:
		{
			if (m_nInt == 15) {
				AddDefinedText(TID_GAME_DONOTUSE_RESTATE);
				nResult = DoUseSystemAnswer::SilentError;
			} else
				ReStateOneLow(3);
		}
		break;
#endif // __ADD_RESTATE_LOW

		default:
			nResult = DoUseSystemAnswer::SilentError;
			break;
	}

	if (pItemProp->dwItemKind3 == IK3_BALLOON) {
		if (HasBuffByIk3(IK3_BALLOON))
			nResult = DoUseSystemAnswer::LimitedUse;
		else {
			DoApplySkill(this, pItemProp, nullptr);
			nResult = DoUseSystemAnswer::Ok;
		}
	}

	return nResult;
}

CUser::SummonState CUser::GetSummonState()
{
	if (m_vtInfo.GetOther() != NULL)
		return SummonState::Trade_1;
	if (m_bBank)
		return SummonState::Trade_1;
	if (m_bGuildBank)
		return SummonState::Trade_1;

	if (IsDie())
		return SummonState::Die_2;

	if (m_vtInfo.VendorIsVendor() || m_vtInfo.IsVendorOpen())
		return SummonState::Vendor_3;

	if (IsAttackMode())
		return SummonState::Attack_4;

	if (IsFly())
		return SummonState::Fly_5;

	if (m_nDuel)
		return SummonState::Duel_6;

	return SummonState::Ok_0;
}

DWORD CUser::GetSummonStateTIdForMyself(SummonState state) {
	switch (state) {
		default:
		case SummonState::Ok_0    : return NULL;
		case SummonState::Trade_1 : return TID_GAME_TRADE_NOTUSE;
		case SummonState::Die_2   : return TID_GAME_DIE_NOTUSE;
		case SummonState::Vendor_3: return TID_GAME_VENDOR_NOTUSE;
		case SummonState::Attack_4: return TID_GAME_ATTACK_NOTUSE;
		case SummonState::Fly_5   : return TID_GAME_FLY_NOTUSE;
		case SummonState::Duel_6  : return TID_GAME_ATTACK_NOTUSE;
	}
}

DWORD CUser::GetSummonStateTIdForOther(SummonState state) {
	switch (state) {
		default:
		case SummonState::Ok_0    : return NULL;
		case SummonState::Trade_1 : return TID_GAME_TRADE_NOTUSE1;
		case SummonState::Die_2   : return TID_GAME_DIE_NOTUSE1;
		case SummonState::Vendor_3: return TID_GAME_VENDOR_NOTUSE1;
		case SummonState::Attack_4: return TID_GAME_ATTACK_NOTUSE1;
		case SummonState::Fly_5   : return TID_GAME_FLY_NOTUSE1;
		case SummonState::Duel_6  : return TID_GAME_ATTACK_NOTUSE1;
	}
}

// IK2_FOOD / IK2_REFRESHER
bool CUser::DoUseItemFood(ItemProp & pItemProp) {
	if (pItemProp.dwID == II_GEN_FOO_INS_SUPERHOTDOG) {
		SetPointParam(DST_HP, GetMaxHitPoint());
		SetPointParam(DST_MP, GetMaxManaPoint());
		SetPointParam(DST_FP, GetMaxFatiguePoint());
		return true;
	}

	for (int i = 0; i < ItemProp::NB_PROPS; i++) {
		if (pItemProp.dwDestParam[i] == static_cast<DWORD>(-1)) continue;

		int nHP = GetPointParam(pItemProp.dwDestParam[i]);
		int nPoint = pItemProp.nAdjParamVal[i];
		const int nHPMax = GetMaxPoint(pItemProp.dwDestParam[i]);
		const int nMax = pItemProp.dwAbilityMin;

		if (pItemProp.dwCircleTime != -1) { // ��������۸� dwCircleTime���� �� �����....
			const BOOL bRet = DoUseItemFood_SM(pItemProp);
			if (!bRet) return false;
		}
		
		const bool isHpMpFp = pItemProp.dwDestParam[i] == DST_HP
			|| pItemProp.dwDestParam[i] == DST_MP
			|| pItemProp.dwDestParam[i] == DST_FP;

		if (!isHpMpFp) {
			DoApplySkill(this, &pItemProp, nullptr);
			continue;
		}

		if (nPoint != -1) {
			if (nHP >= nMax) {
				if (nHP + (nPoint * 0.3f) >= nHPMax) {
					nHP = nHPMax;
				} else {
					DWORD dwParam = 0;
					switch (pItemProp.dwDestParam[i]) {
						case DST_HP:	dwParam = TID_GAME_LIMITHP;		break;
						case DST_MP:	dwParam = TID_GAME_LIMITMP;		break;
						case DST_FP:	dwParam = TID_GAME_LIMITFP;		break;
					}

					AddDefinedText(dwParam, "");
					
					nPoint = (int)(nPoint * 0.3f);
					nHP = nHP + nPoint;
				}
			} else {
				if (nHP + nPoint >= nHPMax)	// ����Ʈ �������� ���ٵǴ°� ó��.
					nHP = nHPMax;
				else
					nHP = nHP + nPoint;
			}
		}

		SetPointParam(pItemProp.dwDestParam[i], nHP);
	}

	return true;
}

// �ҽ����� �򰥸��ϱ� ��������۸� ���� ����.
bool CUser::DoUseItemFood_SM(const ItemProp & pItemProp) {
	if (IsSMMode(SM_MAX_HP) || IsSMMode(SM_MAX_HP50)) {
		AddDefinedText(TID_GAME_LIMITED_USE, "");	// ������� ��� ������
		return false;
	}

	// MAXHP ������ �������� ����.
	
	DWORD nAdjParamVal;
	int nType;
	if (pItemProp.dwID == II_CHR_FOO_COO_BULLHAMS) {
		nAdjParamVal = pItemProp.nAdjParamVal[1];
		nType = SM_MAX_HP;
	} else if (pItemProp.dwID == II_CHR_FOO_COO_GRILLEDEEL) {
		nAdjParamVal = int(GetMaxOriginHitPoint() / (100 / pItemProp.nAdjParamVal[0]));
		m_nPlusMaxHitPoint = nAdjParamVal;
		nType = SM_MAX_HP50;
	} else {
		return false;
	}

	SetDestParam(pItemProp.dwDestParam[0], nAdjParamVal, nAdjParamVal);
	SetSMMode(nType, pItemProp.dwCircleTime);

	return true;
}


// �� ������ ���
CUser::DoUseSystemAnswer CUser::DoUseItemPetTonic(CItemElem * pItemElem) {
	// �� ������ �� ������ ��� ����� �� ����
	if ((pItemElem->m_dwItemId == II_SYS_SYS_SCR_PET_TONIC_A && HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_PET_TONIC_B))
		|| (pItemElem->m_dwItemId == II_SYS_SYS_SCR_PET_TONIC_B && HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_PET_TONIC_A))
		)
		return DoUseSystemAnswer::LimitedUse;

	// �ð� �����÷ο� ����
	DWORD dwCurr = ::timeGetTime();
	IBuff * pBuff = m_buffs.GetBuff(BUFF_ITEM, (WORD)(pItemElem->m_dwItemId));
	if (pBuff && static_cast<int>(pItemElem->GetProp()->dwSkillTime) > static_cast<int>(pItemElem->GetProp()->dwSkillTime + (pBuff->GetTotal() - (dwCurr - pBuff->GetInst()))))
		return DoUseSystemAnswer::SilentError;

	// ȿ�� ����
	DoApplySkill(this, pItemElem->GetProp(), nullptr);
	return DoUseSystemAnswer::Ok;
}

CUser::DoUseSystemAnswer CUser::DoUseItemFeedPocket(CItemElem * pPocket) {
#ifdef __JEFF_11_1
	if (HasBuff(BUFF_ITEM, II_SYS_SYS_SCR_PET_FEED_POCKET02)) {
		AddDefinedText(TID_GAME_DONOTUSE_PETPOCKET);
		return DoUseSystemAnswer::SilentError4;
	}
#endif	// __JEFF_11_1
	// Ȱ��ȭ �˻�
	if (HasBuff(BUFF_ITEM, (WORD)(pPocket->m_dwItemId))) {
		if (pPocket->m_dwKeepTime != 0) {
			RemoveBuff(BUFF_ITEM, (WORD)(pPocket->m_dwItemId));
			return DoUseSystemAnswer::SilentError4;
		} else {
			return DoUseSystemAnswer::LimitedUse;
		}
	}

	ItemProp * pProp = pPocket->GetProp();
	if (pPocket->m_dwKeepTime == 0)	// ���� �ð��� �����Ǿ� ���� �����Ƿ� ���� ����̴�.
	{
		const int nMax = m_Inventory.GetMax();
		for (int i = 0; i < nMax; i++) {
			CItemElem * pItemElem = m_Inventory.GetAtId(i);
			if (IsUsableItem(pItemElem)
				&& pItemElem->m_dwItemId == pPocket->m_dwItemId
				&& pItemElem->m_dwKeepTime > 0
				&& !pItemElem->IsFlag(CItemElem::expired)
				) {
				// �Ⱓ�� ������� ���� ���� �������� �����Ͽ� ����� �� ����.
				AddDefinedText(TID_GAME_DONOTUSE_PETPOCKET, "");
				return DoUseSystemAnswer::SilentError4;
			}
		}
		ASSERT(pProp->dwAbilityMin < 259200);
		UpdateItem(*pPocket, UI::KeepTime::FromProp);
	}
	DoApplySkill(this, pProp, nullptr);
	
	return DoUseSystemAnswer::Ok;
}

bool CUser::DoUseItemSexChange(int nFace) {
	// 1.
	for (DWORD dwParts = 0; dwParts < MAX_HUMAN_PARTS; dwParts++) {
		if (dwParts == PARTS_HEAD || dwParts == PARTS_HAIR || dwParts == PARTS_LWEAPON
			|| dwParts == PARTS_RWEAPON || dwParts == PARTS_SHIELD || dwParts == PARTS_RIDE
			|| (dwParts >= PARTS_NECKLACE1 && dwParts <= PARTS_BULLET))
			continue;

		if (m_Inventory.GetEquip(dwParts)) {
			AddDefinedText(TID_GAME_CHECK_EQUIP, "");
			return false;
		}
	}

	if (nFace < 0 || nFace >= MAX_DEFAULT_HEAD) {
		AddDefinedText(TID_GAME_CHECK_EQUIP, "");
		return false;
	}

	// 2.
	DWORD dwIndex;
	if (GetSex() == SEX_MALE) {
		dwIndex = MI_FEMALE;
		SetSex(SEX_FEMALE);
	} else {
		dwIndex = MI_MALE;
		SetSex(SEX_MALE);
	}

	SetTypeIndex(OT_MOVER, dwIndex);
	ResetScale();
	SetMotion(MTI_WALK);
	//RedoEquip( FALSE );		// chipi_091125 ����
	RedoEquip(FALSE, FALSE);	// chipi_091125 �߰� - �Ǽ��縮, ���� ���� �������� ���� ���·� Ʈ������ �����ѵ� �ɷ�ġ�� �ٽ�(�ߺ�) �����Ű�� ������ �־���.
	UpdateLocalMatrix();

	// 3.
	g_UserMng.AddSexChange(this);

	// 4.
	m_skin.headMesh = static_cast<std::uint8_t>(nFace);
	g_UserMng.AddChangeFace(*this, (DWORD)nFace);

	g_dpDBClient.SendUpdatePlayerData(this);

	return true;
}

bool CUser::DoUseItemWarp(const ItemProp & pItemProp, CItemElem & pItemElem) {
	if (pItemProp.dwID != II_GEN_WARP_COUPLERING
		&& pItemProp.dwID != II_GEN_WARP_WEDDING_BAND
		&& pItemProp.dwID != II_GEN_WARP_COUPLERING01) {
		return false;
	}

	if (IsFly()) return false;

	const u_long idCouple = (u_long)(pItemElem.GetRandomOptItemId());

	if (idCouple == m_idPlayer) {
		AddDefinedText(TID_GAME_COUPLERING_ERR01, "%s", GetName());
		return false;
	}

	if (idCouple == 0) {
		// carve
		UpdateItem(pItemElem, UI::RandomOptItem(m_idPlayer));
		if (II_GEN_WARP_COUPLERING == pItemProp.dwID) {
			UpdateItem(pItemElem, UI::KeepTime::FromDays(15));
		}
		
		AddDefinedText(TID_GAME_COUPLERING_CARVE, "%s", GetName());
		return true;
	} else {
		// teleport
		const char * lpszPlayer = CPlayerDataCenter::GetInstance()->GetPlayerString(idCouple);
		if (!lpszPlayer) {
			AddDefinedText(TID_DIAG_0061, "%s", lpszPlayer);
			return false;
		}

		CUser * pUser = prj.GetUserByID(idCouple);
		if (!IsValidObj(pUser)) {
			AddDefinedText(TID_DIAG_0061, "%s", lpszPlayer);
			return false;
		}

		if (pUser->IsFly()) {
			AddDefinedText(TID_GAME_COUPLERING_ERR03);
			return false;
		}
		
		static constexpr auto IsTeleportable = [](CUser * user) {
			const CWorld * world = user->GetWorld();
			if (!world) return false;

			const DWORD worldId = world->GetID();
			if (worldId == WI_WORLD_GUILDWAR) return false;
			if (worldId == WI_WORLD_QUIZ) return false;
			if (worldId >= WI_WORLD_GUILDWAR1TO1_0 && worldId <= WI_WORLD_GUILDWAR1TO1_L) return false;


			if (prj.IsGuildQuestRegion(user->GetPos())) return false;

			return true;
		};

		if (!IsTeleportable(this) || !IsTeleportable(pUser)) {
			AddDefinedText(TID_GAME_COUPLERING_ERR02, "%s", lpszPlayer);
			return false;
		}

		if (CRainbowRaceMng::GetInstance()->IsEntry(((CUser *)this)->m_idPlayer)
			|| CRainbowRaceMng::GetInstance()->IsEntry(idCouple)) {
			AddDefinedText(TID_GAME_RAINBOWRACE_NOTELEPORT);
			return false;
		}

		if (GetWorld() != pUser->GetWorld() || GetLayer() != pUser->GetLayer()) {
			AddDefinedText(TID_GAME_COUPLERING_ERR02, "%s", lpszPlayer);
			return false;
		}

		Replace(*pUser, REPLACE_FORCE);
		return true;
	}
}

// �������� ����ϴ� ������ ȣ��ȴ�. 
void CUser::OnAfterUseItem(const ItemProp * pItemProp) {
	if (!pItemProp) return;

	D3DXVECTOR3 sPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const DWORD dwSfxID = pItemProp->dwSfxObj3;		// ������ ���ÿ� ������ ����Ʈ

	if (pItemProp->dwItemKind3 == IK3_MAGICTRICK) // ����Ʈ �����۷�
	{
		float	fTheta = D3DXToRadian(GetAngle());
		switch (pItemProp->dwID) {
			case II_GEN_MAG_TRI_FIRESHOWER:
			case II_GEN_MAG_TRI_HWFIREWORKS:
			case II_CHR_MAG_TRI_ROCKETBOMB:
			case II_CHR_MAG_TRI_HEARTBOMB:
			case II_CHR_MAG_TRI_TWISTERBOMB:
				sPos.x = GetPos().x + sinf(fTheta) * 3.0f;			// �ٶ󺸴� ���� 1���� �տ��� �߻���Ŵ.
				sPos.z = GetPos().z + -cosf(fTheta) * 3.0f;
				sPos.y = GetPos().y + 1.5f;
				sPos.y = GetWorld()->GetUnderHeight(D3DXVECTOR3(sPos.x, sPos.y, sPos.z));
				break;
			case II_GEN_MAG_TRI_NEWYEARBOMB:
			case II_GEN_MAG_TRI_SULNALBOMB:
			case II_GEN_MAG_TRI_GOODBYEBOMB:
				sPos.x = GetPos().x;
				sPos.z = GetPos().z;
				sPos.y = GetPos().y + 3.0f;
				break;
		}
	}

	if (dwSfxID != NULL_ID) {
		g_UserMng.AddCreateSfxObj(this, dwSfxID, sPos.x, sPos.y, sPos.z);	// ������ǥ�� ����.
	}

	SetHonorAdd(pItemProp->dwID, HI_USE_ITEM);
}

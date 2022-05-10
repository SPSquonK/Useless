#include "StdAfx.h"
#include "User.h"
#include "defineText.h"
#include "DPSrvr.h"
#include "FunnyCoin.h"
#include "ScriptHelper.h"
#include "DPDatabaseClient.h"
#include "InstanceDungeonBase.h"
#include "FuncTextCmd.h"

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
			return DoUseItemWarp(pItemProp, pItemBase);
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
			if (pItemProp->dwQuestId && pItemBase->m_bItemResist == FALSE) {
				::__SetQuest(GetId(), pItemProp->dwQuestId);
				pItemBase->m_bItemResist = TRUE;
				UpdateItem((BYTE)(pItemBase->m_dwObjId), UI_IR, pItemBase->m_bItemResist);
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
			int nResult = DoUseItemSystem(pItemProp, pItemBase, nPart);
			{
				if (0 < nResult) {
					if (nResult == 2) {
						AddDefinedText(TID_GAME_LIMITED_USE, ""); // ������̾ ����Ҽ� ����
					} else if (nResult == 3) {
						AddDefinedText(TID_GAME_NOTOVERLAP_USE, ""); // �� �����۰��� �ߺ��Ͽ� ����� �� �����ϴ�
					}
					return FALSE;
				} else {
					if (pItemBase->m_dwItemId == II_SYS_SYS_SCR_RECCURENCE_LINK) {
						g_dpDBClient.SendLogSMItemUse("1", this, pItemBase, pItemProp);
					}

					//////////////////////////////////////////////////////////////////////////
					//	mulcom	BEGIN100125	�̺�Ʈ�� ����ų �� �̺�Ʈ�� ������Ʈ ��� ������ ���� �α� �߰�
					//						( e-mail : [����] ������ �α� �߰� ( 2010-01-25 17:33 ) ���� )
					else if (pItemBase->m_dwItemId == II_SYS_SYS_SCR_RECCURENCE && pItemBase->m_bCharged != TRUE) {
						g_DPSrvr.PutItemLog(this, "w", "USE_RECCURENCE_ITEM", pItemBase, 1);
					} else if (pItemBase->m_dwItemId == II_CHR_SYS_SCR_RESTATE && pItemBase->m_bCharged != TRUE) {
						g_DPSrvr.PutItemLog(this, "w", "USE_RESTATE_ITEM", pItemBase, 1);
					}
					//	mulcom	END100125	�̺�Ʈ�� ����ų �� �̺�Ʈ�� ������Ʈ ��� ������ ���� �α� �߰�
					//////////////////////////////////////////////////////////////////////////
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
				if (DoUseItemFood(pItemProp, pItemBase) == FALSE)
					return false;
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
				if (DoUseItemFood(pItemProp, pItemBase) == FALSE)
					return FALSE;
			}
			break;
		case IK2_FOOD:
			if (DoUseItemFood(pItemProp, pItemBase) == FALSE)
				return FALSE;
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

	if (pItemBase->m_bCharged)		// ���ȭ ������ �α�
		g_dpDBClient.SendLogSMItemUse("1", this, pItemBase, pItemProp);

	CHAR cUIParam = UI_NUM;
	if (cooldownType == CCooltimeMgr::CooldownType::Available)	// ��Ÿ�� �������̸� ���ð��� ����Ѵ�.
	{
		m_cooltimeMgr.StartCooldown(*pItemProp);
		cUIParam = UI_COOLTIME;
	}

	UpdateItem((BYTE)(dwId), cUIParam, pItemBase->m_nItemNum);	// ������ 0�̸�  ������ ���� , ���� 
	
	return true;
}


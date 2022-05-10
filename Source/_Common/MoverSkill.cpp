#include "stdafx.h"
#include "defineItem.h"
#include "defineSkill.h"
#include "defineSound.h"
#include "defineText.h"
#include "ModelObject.h"
#include "CreateObj.h"
#include "defineobj.h"
#include "eveschool.h"
#include "party.h"

#include "definequest.h"

#include "FuncTextCmd.h"

#ifdef __CLIENT
	#include "DPClient.h"
#else	// __CLIENT
	#include "..\_aiinterface\AIPet.h"
	#include "User.h"
	#include "WorldMng.h"
	#include "DPSrvr.h"
	#include "DPCoreClient.h"
	#include "dpdatabaseclient.h"
#ifdef __SYS_TICKET
	#include "ticket.h"
#endif	// __SYS_TICKET
	#include "ItemUpgrade.h"
#endif	// __CLIENT

#include "playerdata.h"

	#include "honor.h"

#ifdef __FUNNY_COIN
#include "FunnyCoin.h"
#endif // __FUNNY_COIN

#ifdef __WORLDSERVER
#include "GuildHouse.h"
#endif

extern void __SetQuest( DWORD dwIdMover, int nQuest );

#ifdef __WORLDSERVER

void	CMover::SetMasterSkillPointUp()
{
	if( m_nLevel != 72 && m_nLevel != 84 && m_nLevel != 96 && m_nLevel != 108 )
		return;
	if( IsHero() )
		return;

	for( int i = 0; i < MAX_SKILL_JOB; i++ ) 
	{				
		LPSKILL lpSkill = &(m_aJobSkill[i]);
		if( lpSkill && lpSkill->dwSkill != NULL_ID )
		{
			ItemProp* pSkillProp    = prj.GetSkillProp( lpSkill->dwSkill );			
			if( pSkillProp == NULL )
				continue;
			if( pSkillProp->dwItemKind1 != JTYPE_MASTER)
				continue;
			lpSkill->dwLevel++;
			g_dpDBClient.SendLogSkillPoint( LOG_SKILLPOINT_USE, 1, this, &(m_aJobSkill[i]) );
		}
	}	
	g_UserMng.AddCreateSfxObj(this, XI_SYS_EXCHAN01, GetPos().x, GetPos().y, GetPos().z);

	CUser *pUser = (CUser *)this;
	pUser->AddDoUseSkillPoint( &m_aJobSkill[0], m_nSkillPoint );

#ifdef __S_NEW_SKILL_2
	g_dpDBClient.SaveSkill( this );
#endif // __S_NEW_SKILL_2
}

// ��ųť�� ��� �ִ� �������� �� ĳ���� �ð��� ���.
int		CMover::GetQueueCastingTime()
{
	if( IsNPC() )	
		return 0;

	CUser *pUser = (CUser *)this;
	int		i;
	SHORTCUT *pSlotQueue = pUser->m_playTaskBar.m_aSlotQueue;
	int		nTotalTime = 0;

	for( i = 0; i < MAX_SLOT_QUEUE; i ++ )
	{
		if( pSlotQueue[i].m_dwShortcut )
		{
			LPSKILL pSkill = GetSkill( 0, pSlotQueue[i].m_dwId );
			
			ItemProp *pSkillProp;
			if( !( pSkillProp = pSkill->GetProp() ) )					// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
			{
				Error( "CMover::GetQueueCastingTime : %s. ��ų(%d)�� ������Ƽ�� ����.", m_szName, pSkill->dwSkill );
				return FALSE;	// property not found
			}
			
			AddSkillProp* pAddSkillProp;
			
		#ifdef __SKILL0517
			if( !( pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, GetSkillLevel( pSkill ) ) ) )					// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
		#else	// __SKILL0517
			if( !( pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, pSkill->dwLevel ) ) )					// JobSkill ����Ʈ���� ���� ��ų�� ������Ƽ�� ����.
		#endif	// __SKILL0517
			{
				Error( "CMover::GetQueueCastingTime : %s. �ֵ彺ų(%d)�� ������Ƽ�� ����.", m_szName, pSkill->dwSkill );
				return FALSE;	// property not found
			}

		#ifdef _DEBUG
			if( (int)pAddSkillProp->dwCastingTime <= 0 )
				Error( "GetQueueCastingTime : %s�� addProp dwCastingTime�� %d", pSkillProp->szName, pAddSkillProp->dwCastingTime );
		#endif
			nTotalTime += (int)pAddSkillProp->dwCastingTime;
		}
	}

	nTotalTime = (int)(nTotalTime * 0.7f);		// 70���� ��������.
	nTotalTime = (int)( (nTotalTime / 1000.0f) * SEC1 );	// ĳ���ô��� �ð����� ��ȯ.
	nTotalTime	= GetCastingTime( nTotalTime );

	return nTotalTime;
}

#endif // worldserver


// ������ ��ų ��� ����� ������
// Mover�� ��밡���� ��ų�迭(m_aJobSkill[3+14])���� nIdx�� �ش��ϴ� ��ų�� ���� �װ��� �����Ѵ�.
// sutType : ��ų���� ����Ű/��ųť�� ���θ� Ŭ��κ��� �޾Ƽ� ó��.  ĳ����Ÿ�� ����� �ٸ���.
//
#ifdef __CLIENT
	BOOL	CMover::DoUseSkill( int nType, int nIdx, OBJID idFocusObj, SKILLUSETYPE sutType, BOOL bControl, const int nCastingTime, DWORD dwSkill, DWORD dwLevel )
#else // __CLIENT
		BOOL	CMover::DoUseSkill( int nType, int nIdx, OBJID idFocusObj, SKILLUSETYPE sutType, BOOL bControl )
#endif // __CLIENT
{
	if( IsNPC() )	
		return FALSE;		// ���Ǿ��� ������ Ÿ�� �ȵȴ�.

#ifdef __WORLDSERVER
	DWORD dwSkill = 0, dwLevel;
#endif	// __WORLDSERVER
	
	if( nIdx >= 0 )
	{
		LPSKILL pSkill = GetSkill( nType, nIdx );
		if( pSkill == NULL )
		{
			Error( "CMover::DoUseSkill : %s�� nIdx�� ��ų�� ������ ���� �ʴ�. %d", m_szName, nIdx );
			return FALSE;
		}
		dwSkill	= pSkill->dwSkill;
	#ifdef __SKILL0517
		dwLevel	= GetSkillLevel( pSkill );
	#else	// __SKILL0517
		dwLevel	= pSkill->dwLevel;
	#endif	// __SKILL0517

		if( dwSkill == DWORD(-1) )
			return FALSE;

		if( dwLevel <= 0 )
			return FALSE;
/*
#ifdef __GUILD_COMBAT_1TO1 // chipi_071227 �ӽ÷� ����
		if( dwSkill == SI_KNT_HERO_DRAWING )
		{
		#ifdef __CLIENT
			if( g_GuildCombat1to1Mng.IsPossibleMover( this ) )
				return FALSE;
		#endif // __CLIENT
		#ifdef __WORLDSERVER
			if( g_GuildCombat1to1Mng.IsPossibleUser( (CUser*)this ) )
			{
				((CUser*)this)->AddDefinedText( TID_GAME_NEVERKILLSTOP );
				return FALSE;
			}
		#endif // __WORLDSERVER
		}
#endif // __GUILD_COMBAT_1TO1
*/
	}

#ifdef __WORLDSERVER
	int nCastingTime = 0;
#endif

	ItemProp *pSkillProp = NULL;
	AddSkillProp *pAddSkillProp = NULL;
	GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, dwLevel, "DoUseSkill_P" );

	if(pSkillProp == NULL)
		return FALSE;

	OBJMSG dwMsg = OBJMSG_NONE;
	int nMotion = (int)pSkillProp->dwUseMotion;
	
	if( pSkillProp->dwSkillType == KT_MAGIC )
	{
		if( sutType == SUT_QUEUESTART )		// ��ųť ���� ��ų
		{
		#ifdef __WORLDSERVER
			nCastingTime = GetQueueCastingTime();	// ť���� ��� ��ų�� ĳ������ ���Ƽ� �Ѵ�.
		#endif
			dwMsg = OBJMSG_MAGICCASTING;
		} 
		else if( sutType == SUT_QUEUEING )	// ��ųť ������ ��ų
		{
		#ifdef __WORLDSERVER
			nCastingTime = 0;						// ĳ���� ���۾��� ����.
		#endif
			nMotion += 2;
			dwMsg = OBJMSG_MAGICSKILL;
		} 
		else
		{									// �Ϲ����� ���� ��ų ���
		#ifdef __WORLDSERVER
		
			nCastingTime	= (int)( ( pAddSkillProp->dwCastingTime / 1000.0f ) * SEC1 );	// ĳ���� ���� ����.
		
			nCastingTime = GetCastingTime( nCastingTime );
		#endif	// __WORLDSERVER
			dwMsg = OBJMSG_MAGICCASTING;
		}
	}
	else if( pSkillProp->dwSkillType == KT_SKILL )
	{
	#ifdef __WORLDSERVER
		nCastingTime = 1;
	#endif
	}

	BOOL bSuccess = DoUseSkill( dwSkill, dwLevel, idFocusObj, sutType, bControl, nCastingTime );
	return  bSuccess;
}

// dwSkill,nLevel�� ������ ��𼭵��� ��밡���� ����	
// ���ͱ��� ��밡��
BOOL CMover::DoUseSkill( DWORD dwSkill, int nLevel, OBJID idFocusObj, SKILLUSETYPE sutType, BOOL bControl, const int nCastingTime )
{
	if( IsDie() )	return FALSE;
	if( m_pActMover->IsFly() )	return FALSE;
	if( m_dwFlag & MVRF_NOATTACK )	
		return FALSE;		// ���ݱ��� ���¿��� ��ų ����.

#ifdef __QUIZ
	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
		return FALSE;
#endif // __QUIZ
	
#if defined(__WORLDSERVER)	
	// �������ʿ��� ������ ���
	CWorld* pWorld	= GetWorld();
	if( pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR )
	{		
		if( m_nGuildCombatState == 0 )
		{
			if( IsPlayer() )
				((CUser*) this)->AddText( prj.GetText( TID_GAME_GUILDCOMBAT_STANDS_NOTUSESKILL ) );	//"�����ؾ��� : ���߼������� ��ų�� ����Ҽ� �����ϴ�" );
			return FALSE;
		}
	}
#endif //defined(__WORLDSERVER)

	if( IsMode( NO_ATTACK_MODE ) ) return FALSE;
	
	ItemProp *pSkillProp = NULL;
	AddSkillProp *pAddSkillProp = NULL;
	GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, nLevel, "DoUseSkill" );

#ifdef __CLIENT
	// �ϴ��� ������ �� ��ũǮ ��� ����
	DWORD dwWorldID(GetWorld()->GetID());
	if(dwWorldID >= WI_WORLD_GUILDWAR1TO1_0 && dwWorldID <= WI_WORLD_GUILDWAR1TO1_L)
	{
		if(pSkillProp->dwID == SI_MAG_MAG_BLINKPOOL)
		{
			g_WndMng.PutString(prj.GetText(TID_GAME_NEVERKILLSTOP), NULL, prj.GetTextColor(TID_GAME_NEVERKILLSTOP));
			return FALSE;
		}
	}
#endif // __CLIENT

	CMover* pTarget = prj.GetMover( idFocusObj );	// Ÿ���� �����͸� ��.
	if( IsInvalidObj( pTarget ) )	
		return FALSE;

	if( pSkillProp->dwID == SI_ASS_HEAL_HEALING )	// ���� ��밡 �׾����� ���
	{
		if( pTarget->IsDie() )
			return FALSE;
	}

#ifdef __WORLDSERVER
#ifdef __PK_PVP_SKILL_REGION
	if( pSkillProp->nEvildoing < 0 )	//PK ������������ ���� ������ ��ų�� PVP ��뿡�Ը� ����ϵ���..
	{
		int nAttackerPK, nDefenderPK;
		nAttackerPK = nDefenderPK = 0;
		if( IsPlayer() && pTarget->IsPlayer() && (this != pTarget) ) //(this!=pTarget) - Ÿ���� ����� ��ų�� Ÿ���� �������̴�.
		{
			DWORD dwRegionAttr	= GetPKPVPRegionAttr();
			nAttackerPK = IsPKPVPInspectionBase( dwRegionAttr, FALSE );
			nDefenderPK = pTarget->IsPKPVPInspectionBase( dwRegionAttr, FALSE );
			if( (nAttackerPK == 1 || nDefenderPK == 1) &&
				!( 
						IsPVPTarget( pTarget )
					|| IsWarTarget( pTarget )
					|| IsGuildCombatTarget( pTarget ) 
#ifdef __JEFF_11_4
					|| IsArenaTarget( pTarget )
#endif	// __JEFF_11_4
				) )
			{
				((CUser*)this)->AddDefinedText( TID_GMAE_PK_NOT_AREA, "" );
				return FALSE;
			}
		}
	}
#endif // __PK_PVP_SKILL_REGION
	if( HasBuff( BUFF_SKILL, SI_BLD_SUP_BERSERK ) )		// ����ũ�� �ɷ��ִ� ���¿��� ��ų��� ����.
		return FALSE;

#ifdef __NPC_BUFF
	if( ( pSkillProp->dwID == SI_ASS_CHEER_QUICKSTEP && pTarget->HasBuff(BUFF_SKILL, SI_GEN_EVE_QUICKSTEP) )
		|| ( pSkillProp->dwID == SI_ASS_CHEER_HASTE && pTarget->HasBuff(BUFF_SKILL, SI_GEN_EVE_HASTE) )
		|| ( pSkillProp->dwID == SI_ASS_CHEER_HEAPUP && pTarget->HasBuff(BUFF_SKILL, SI_GEN_EVE_HEAPUP) )
		|| ( pSkillProp->dwID == SI_ASS_CHEER_ACCURACY && pTarget->HasBuff(BUFF_SKILL, SI_GEN_EVE_ACCURACY) ) )
	{
		if( pTarget->IsPlayer() )
			((CUser*)pTarget)->AddDefinedText( TID_GAME_NPCBUFF_FAILED, "\"%s\"", pSkillProp->szName );
		return FALSE;
	}
#endif // __NPC_BUFF

	if( pSkillProp->dwID == SI_ACR_YOYO_SNITCH || pSkillProp->dwID == SI_JST_YOYO_VATALSTAB )	// ��Ż, ȥ���� �ϰݽ�ų�̸� �����Ϸ����������� �˻��Ѵ�.
	{
		if( !HasBuff( BUFF_SKILL, SI_ACR_SUP_DARKILLUSION ) )		// ��ũ�Ϸ��� ���°� �ƴϸ� ������
		{
			if( IsPlayer() )
				((CUser*)this)->AddDefinedText( TID_GAME_REQ_DARK, "" );		// ��ũ�Ϸ������¿��� �����մϴ�.
			return FALSE;
		}
	}

	if( pSkillProp->dwID == SI_RIG_HERO_RETURN
		&& IsPlayer() && CRainbowRaceMng::GetInstance()->IsEntry( ((CUser*)this)->m_idPlayer ) )
	{
		((CUser*)this)->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
		return FALSE;
	}
#endif //defined(__WORLDSERVER)

	MoverProp *pMoverProp = pTarget->GetProp();
	if( pMoverProp == NULL )
		Error( "DoUseSkill : %s , Ÿ�� %s �� ������Ƽ�� ����", m_szName, pTarget->m_szName );
	
	if( pTarget != this )	// Ÿ�ο��� ���� �����϶��� �Ʒ��˻縦 ��.
	{
	#ifdef __WORLDSERVER
		if( pSkillProp->dwUseChance == WUI_NOW )	// ˬ - ���ҽ� ��ŷ
			return FALSE;
		if( pSkillProp->dwExeTarget == EXT_SELFCHGPARAMET )
			return FALSE;

		if( pTarget->IsNPC() && pTarget->IsPeaceful() )	
			return FALSE;	// NPC���� ģ��?NPC���� ��ų ��� ����
		
		if( !g_eLocal.GetState( EVE_STEAL ) )
		{
			if( pSkillProp->nEvildoing < 0 )	// ���۸����� �����Ҷ�
				if( IsSteal( pTarget ) )	// pTarget�� ��ƿ �Ϸ��ϸ� 
					return FALSE;
		}
#ifdef __JEFF_11_4
		if(  pWorld && pWorld->IsArena() == FALSE )		// �Ʒ����� �ƴϸ�,
#endif	// __JEFF_11_4
		{
			if( pSkillProp->nEvildoing > 0 )	// ���� ������
			{
				HITTYPE hy;
				if( (hy = GetHitType2( pTarget, TRUE, TRUE)) != HITTYPE_FAIL )  	// �������� ��󿡰� �����Ҷ�
				{
					if( IsPlayer() )
						((CUser*)this)->AddDefinedText( TID_GAME_NEVERKILLSTOP, "" );		// ��ų�� ����� �� �����ϴ�.
					return FALSE;		// ��ų ���
				}
			}
		}
		
	#endif
		SetAngle( GetDegree(pTarget->GetPos(), GetPos()) );		// ��ǥ������ ���� ����.
	}
	
	// ����Ĺ��߿��� ��Ȱ��ų ����
	if( GetWorld()->GetID() == WI_WORLD_GUILDWAR )
	{
		if( pSkillProp->dwID == SI_MAG_MAG_BLINKPOOL || pSkillProp->dwID == SI_RIG_HERO_RETURN )
			return FALSE;

		if( g_GuildCombatMng.m_nState != CGuildCombat::WAR_STATE )
		{
			if( pSkillProp->dwID == SI_ASS_HEAL_RESURRECTION )
				return FALSE;
		}
	}

#ifdef __WORLDSERVER

/*
---- 2008.03.25 ����׽�ų ���� ����
	#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
	if( pSkillProp->dwID == SI_KNT_HERO_DRAWING )
	{
		CParty* pParty	= g_PartyMng.GetParty( ((CUser*)this)->m_idparty );
		if( pParty )
			pParty->DoUsePartyReCall( ((CUser*)this)->m_idparty, ((CUser*)this)->m_idPlayer, 0);
	}
	#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans
---- 2008.03.25 ����׽�ų ���� ����
*/
	if( g_eLocal.GetState( EVE_SCHOOL ) )
	{
		if( pSkillProp->dwID == SI_ASS_HEAL_RESURRECTION )
			return FALSE;
	}

	// ��ų�� ����Ҷ� ������ ���⸦ ��� �ִ°� �˻�
	ItemProp* pItemProp = NULL;
	DWORD	dwItemKind3;

	if( pSkillProp->dwLinkKind == IK3_SHIELD )	// �ʿ�ƾ������� �����϶�
	{
		CItemElem *pItemElem = GetEquipItem( PARTS_SHIELD );
		if( pItemElem )
		{
			pItemProp = pItemElem->GetProp();
			dwItemKind3 = pItemProp->dwItemKind3;
		} else
			dwItemKind3 = NULL_ID;
	}
	else	// �ʿ�������� ���а� �ƴҶ��� ��Ƽ�� ��������...
	{
		pItemProp = GetActiveHandItemProp();
		dwItemKind3 = pItemProp->dwItemKind3;
	}
	
	switch( pSkillProp->dwLinkKind )	// ��ų�� �ʿ���ϴ� ����.
	{
	case IK3_ENCHANTWEAPON:
		{
			if( IsPlayer() )
			{
				BOOL bFail = FALSE;

				CItemElem *pItemElemR = GetWeaponItem();
				CItemElem *pItemElemL = GetLWeaponItem();

				if( pItemElemR && pItemElemL )	// ������ ������.
				{	// ��մ� �Ӽ��� ������
					if( (pItemElemR && pItemElemR->m_bItemResist == SAI79::NO_PROP) &&		
						(pItemElemL && pItemElemL->m_bItemResist == SAI79::NO_PROP) )
						bFail = TRUE;		// ������
				} 
				else if( pItemElemR && pItemElemL == NULL )	// �����ո� ��� ������.
				{
					if( pItemElemR && pItemElemR->m_bItemResist == SAI79::NO_PROP )		// �����տ� �Ӽ��� ������
						bFail = TRUE;		// ������
				} 
				else if( pItemElemR == NULL && pItemElemL == NULL )	// ��մ� ������
				{
					bFail = TRUE;		// �̶��� ������ ������
				}

				if( bFail )
				{
					( (CUser*)this )->AddDefinedText( TID_SKILL_NOTELEWEAPON, "" );	// �Ӽ����Ⱑ ���� ������.
					return FALSE;
				}
			}
		}
		break;
	case IK3_MAGICBOTH:
		if( dwItemKind3 != IK3_WAND  && dwItemKind3 != IK3_STAFF )	// �տ��� ���Ⱑ �ϵ嵵 �������� �ƴϸ�
		{
			if( IsPlayer() )
				( (CUser*)this )->AddDefinedText( TID_GAME_WRONGITEM, "" );
			return FALSE;
		}
		break;
	case IK3_YOBO:
		if( dwItemKind3 != IK3_YOYO && dwItemKind3 != IK3_BOW )	
		{
			if( IsPlayer() )
				( (CUser*)this )->AddDefinedText( TID_GAME_WRONGITEM, "" );
			return FALSE;
		}
		break;
	default:
		if( pSkillProp->dwLinkKind != NULL_ID && pSkillProp->dwLinkKind != dwItemKind3 )	// �׿ܴ� IK3�� ���ؼ� Ʋ���� ����.
		{
			if( IsPlayer() )
				( (CUser*)this )->AddDefinedText( TID_GAME_WRONGITEM, "" );
			return FALSE;
		}
		break;
	}

	if(pSkillProp->dwSkillType == BUFF_SKILL)
	{
		CMover* searchMover;
		if(pTarget == this)
			searchMover = this;
		else
			searchMover = pTarget;

#ifdef __BUFF_1107
		IBuff* pBuff	= searchMover->m_buffs.GetBuff( BUFF_SKILL, (WORD)( pSkillProp->dwID ) );
		if( pBuff )
		{
			if( pBuff->GetLevel() > (DWORD)( nLevel ) )
#else	// __BUFF_1107
		SKILLINFLUENCE* hasSkill = searchMover->m_SkillState.Find( BUFF_SKILL, pSkillProp->dwID );
		if( hasSkill )
		{
			if( hasSkill->dwLevel > nLevel )
#endif	// __BUFF_1107
			{
				if( IsPlayer() ) // ������	// this�� CUser�� �ƴ� �� ����.
					( (CUser*)this )->AddDefinedText( TID_GAME_DONOTUSEBUFF, "" );
				return FALSE;
			}
		}
	}

	// ���ڵ���̳� ���ڵ���̳��� ���ǰ˻�.
	if( pSkillProp->dwHanded != NULL_ID )
	{
		if( pItemProp )
		{
			if( IsPlayer() )
			{
				if( pSkillProp->dwHanded == HD_DUAL )		// ���� ��ų�ε�
				{
					if( IsDualWeapon() == FALSE )			// ���� ������ �ִ�.
					{
						((CUser*)this)->AddDefinedText( TID_GAME_WRONGITEM, "" );
						return FALSE;
					}
				} 
				else if( pItemProp->dwHanded != pSkillProp->dwHanded )	// ��ų�� ��� �ִ� �������� Handed ������ �ٸ��� ����
				{
					((CUser*)this)->AddDefinedText( TID_GAME_WRONGITEM, "" );
					return FALSE;
				}
			}
		}
	}

	// �ʿ��Ѿ������� �����Ǿ� �ִٸ�
	if( IsBullet( pSkillProp ) == FALSE )
		return FALSE;

	if( IsPlayer() )
	{
		if( pSkillProp->dwReqDisLV != NULL_ID && GetLevel() < (int)( pSkillProp->dwReqDisLV )  && !IsMaster() && !IsHero() )
			return FALSE;
		
		if( pSkillProp->dwReSkill1 != 0xffffffff )
		{
			LPSKILL pSkillBuf = GetSkill( pSkillProp->dwReSkill1 );
			if( pSkillBuf )
			{
				if( pSkillBuf->dwLevel < pSkillProp->dwReSkillLevel1 )
				{
					( (CUser*)this )->AddText( "SkillLevel1 " );
					return FALSE;
				}
			}
			else
			{
				int nIdx = GetSkillIdx( dwSkill );
				Error( "DoUseSkill : %s NULL GetSkill %d = dwReSkill(%d, %d)", m_szName, nIdx, pSkillProp->dwReSkill1, pSkillProp->dwReSkill2 );
			}
		}

		if( pSkillProp->dwReSkill2 != 0xffffffff )
		{
			LPSKILL pSkillBuf = GetSkill( pSkillProp->dwReSkill2 );
			if( pSkillBuf )
			{
				if( pSkillBuf->dwLevel < pSkillProp->dwReSkillLevel2 )
				{
					( (CUser*)this )->AddText( "SkillLevel2 " );
					return FALSE;
				}				
			}
			else
			{
				int nIdx = GetSkillIdx( dwSkill );
				Error( "DoUseSkill : %s NULL GetSkill %d = dwReSkill(%d, %d)", m_szName, nIdx, pSkillProp->dwReSkill1, pSkillProp->dwReSkill2 );
			}
		}
	}

	if( pSkillProp->dwID == SI_BIL_PST_ASALRAALAIKUM )	// �ѻ���� ��� ����ó��.
	{
		//if( GetFatiguePoint() <= 0  )		// FP�� �ϳ��� ������ ���ȵ�. // chipi_090917 ����
		if( IsPlayer() && GetFatiguePoint() < GetReqFp( pAddSkillProp->nReqFp )  )		// chipi_090917 �ʿ� FP �˻�
		{
			((CUser*)this)->AddDefinedText( TID_GAME_REQFP ); // chipi_00917
			return FALSE;
		}
	}

	// ��Ÿ�� �˻�.
	{
		int nSkillIdx = GetSkillIdx( dwSkill );		// ��ų����Ʈ �ε����� ã��.
		if( nSkillIdx >= 0 )
		{
			if( GetReuseDelay( nSkillIdx ) )		// ������ �ϴ� ��ų ��Ÿ���� ���������� ����ȵ�.
			{
				if( IsPlayer() )
					((CUser*)this)->AddDefinedText( TID_GAME_SKILLWAITTIME, "" );	// ���� ����� �� �����ϴ�.
				return FALSE;
			}
		}
	}
#endif // __WORLDSERVER
		

	SendActMsg( OBJMSG_STOP );
	ClearDest();

	OBJMSG dwMsg = OBJMSG_NONE;
	int nMotion = (int)pSkillProp->dwUseMotion;
	
	//--- ������ ������� �˻�...
	if( pSkillProp->dwSkillType == KT_SKILL )
	{
	#ifdef __WORLDSERVER

			if(GetReqFp( pAddSkillProp->nReqFp ) == 9999999)
			{
				if( GetManaPoint() != GetMaxManaPoint())
				{
					if( TRUE == IsPlayer() )
						( (CUser*)this )->AddDefinedText( TID_GAME_REQMP, "" );
					return FALSE;
				}
				if( GetFatiguePoint() != GetMaxFatiguePoint())
				{
					if( TRUE == IsPlayer() )
						( (CUser*)this )->AddDefinedText( TID_GAME_REQFP, "" );
					return FALSE;
				}
			}
			else
			{
				if( GetFatiguePoint() - GetReqFp( pAddSkillProp->nReqFp ) < 0 )	// FP�� ���ڸ��Ƿ� ���
				{
					if( TRUE == IsPlayer() )
						( (CUser*)this )->AddDefinedText( TID_GAME_REQFP, "" );
					return FALSE;
				}

				// KT_SKILL�߿� MP�� �Ҹ�Ǵ� ���������� FP�� ���� �Ҹ�����ش�.
				if( (int)pAddSkillProp->nReqMp > 0 )
				{
					if( GetManaPoint() - GetReqMp( pAddSkillProp->nReqMp ) < 0 )		// ������ ���ڸ��Ƿ� ���
					{
						if( TRUE == IsPlayer() )
							( (CUser*)this )->AddDefinedText( TID_GAME_REQMP, "" );
						return FALSE;
					}
				}
			}

	#endif
		dwMsg = OBJMSG_MELEESKILL;	// Ŭ�󿡼��� �����ؾ���
	} 
	else if( pSkillProp->dwSkillType == KT_MAGIC )
	{
	#ifdef __WORLDSERVER
		if( GetAdjParam( DST_CHRSTATE ) & CHS_SILENT )		// ħ�����¿��� ������ų �� �� ����.
			return FALSE;

		if(GetReqMp( pAddSkillProp->nReqMp ) == 9999999)
		{
			if( GetManaPoint() != GetMaxManaPoint())
			{
				if( TRUE == IsPlayer() )
					( (CUser*)this )->AddDefinedText( TID_GAME_REQMP, "" );
				return FALSE;
			}
			if( GetFatiguePoint() != GetMaxFatiguePoint())
			{
				if( TRUE == IsPlayer() )
					( (CUser*)this )->AddDefinedText( TID_GAME_REQFP, "" );
				return FALSE;
			}
		}
		else
		{
			if( GetManaPoint() - GetReqMp( pAddSkillProp->nReqMp ) < 0 )		// ������ ���ڸ��Ƿ� ���
			{
				if( TRUE == IsPlayer() )
					( (CUser*)this )->AddDefinedText( TID_GAME_REQMP, "" );
				return FALSE;
			}
		}

	#endif // worldserver
		if( sutType == SUT_QUEUESTART )		// ��ųť ���� ��ų. - ť���� ��� ��ų�� ĳ������ ���Ƽ� �Ѵ�.
		{
			if( IsPlayer() )
				dwMsg = OBJMSG_MAGICCASTING;
		} 
		else if( sutType == SUT_QUEUEING )	// ��ųť ������ ��ų. - ĳ���� ���۾��� ����.
		{
			nMotion += 2;
			dwMsg = OBJMSG_MAGICSKILL;
		} 
		else
		{									// �Ϲ����� ���� ��ų ��� - ĳ���� ���� ����.
			dwMsg = OBJMSG_MAGICCASTING;
		}
	}
	else
		Error( "DoUseSkill : %s dwSkillType = %d", m_szName, pSkillProp->dwSkillType );	// ������Ƽ���� �̻���.

	if( sutType != SUT_QUEUEING && nCastingTime == 0 && pSkillProp->dwID != SI_GEN_ATK_COUNTER )
		nMotion += 2;
	
	//--- ���� ���� -----------------------------------------------------------------
	int nRet = m_pActMover->SendActMsg( dwMsg, nMotion, idFocusObj, nCastingTime, dwSkill );

	if( nRet == 1 )	// ��ų ���� ����.
	{
		// ��ɼ����� �ʿ��� �Ķ���ͼ���.
		SetActParam( OBJACT_USESKILL, dwSkill, idFocusObj );	
		m_nAParam[3]	= nLevel;
		m_nAParam[4]	= bControl;

	#ifdef __WORLDSERVER
		if( pSkillProp->dwSkillType == KT_SKILL )
		{
			// ������ų�� ����ϴ� ��� fp����.
			if( (int)pAddSkillProp->nReqFp > 0 )
				IncFatiguePoint( -GetReqFp( pAddSkillProp->nReqFp ) );			// FP����

			int nReqMp	= GetReqMp( pAddSkillProp->nReqMp );
			if( nReqMp > 0 )
			{
				IncManaPoint( -nReqMp );
			}
		}
		if( pSkillProp->dwSkillType == KT_MAGIC )
		{
			if( GetReqMp( pAddSkillProp->nReqMp ) == 9999999 )
			{
				IncFatiguePoint( -GetFatiguePoint() );			// FP����
				IncManaPoint( -GetManaPoint() );
			}
		}
		
		if( pSkillProp->dwLinkKindBullet != NULL_ID )	// �Ѿ�(?)�� �ʿ�� �ϴ°Ÿ�
		{
			CItemElem *pItemElem = GetEquipItem( PARTS_BULLET );
			if( pItemElem )
				ArrowDown( 1 );
		}

		if( pSkillProp->nEvildoing < 0 )		// ���۸������θ� ���ͳ� �÷��̾ ���Ҽ��ִ�.
		{
			pTarget->SetJJim( this );			// pTarget�� ���ߴٴ°� ǥ��.
			m_nAtkCnt = 1;						// ���ڷ� �����̸� ���� ���� ī���� ����.
		}

		// DST param�� ����Ǵ� ��ų�� ���⼭ ������.
		g_UserMng.AddUseSkill( this, dwSkill, nLevel, idFocusObj, sutType, nCastingTime );
	#endif	// __WORLDSERVER
	} 
	else if( nRet == -2 )	// 0:ť�� �״´� -1:��  -2:���������Ͽ� ��ҽ�Ŵ   -3:��
	{
		return FALSE;
	}
	
	m_dwFlag |= MVRF_SKILL;		// ��ų��� ����� ���۵ƴ�.
#ifdef __CLIENT
	if( IsActiveMover() )		// g_pPlayer�ϰ�� 
	{
		CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
		if( pTaskBar->m_nExecute == 1 )		// ��ų�� ��� ����߻��¿�����
			pTaskBar->m_nExecute = 2;		// ���� ��ų�� ��������� ��ȯ.
	}
#endif

#ifdef __CLIENT
	{
		DWORD dwLinkSfxDouble = NULL_ID;
		switch( dwSkill )
		{	// ����� XI_ �߰��ϸ� CreateSfx�� CSfxPartsLink()���� �߰����Ѿ� ��/
		case SI_BLD_DOUBLEAX_SPRINGATTACK:	dwLinkSfxDouble = XI_SKILL_BLD_DOUBLEAX_SPRINGATTACK01;	break;
		case SI_BLD_DOUBLE_SONICBLADE:		dwLinkSfxDouble = XI_SKILL_BLD_DOUBLE_SONICBLADE01;	break;
		case SI_KNT_TWOSW_CHARGE:			dwLinkSfxDouble = XI_SKILL_KNT_TWOSW_CHARGE01;	break;
		case SI_BLD_DOUBLESW_BLADEDANCE:	dwLinkSfxDouble = XI_SKILL_BLD_DOUBLESW_BLADEDANCE01;      break;

		// ������ ��� ����Ʈ : sfx�������� ���̸� �Ҹ���
		case SI_JST_YOYO_VATALSTAB:	dwLinkSfxDouble = XI_SKILL_JST_YOYO_VATALSTAB01;	break;
		case SI_JST_YOYO_HITOFPENYA:	dwLinkSfxDouble = XI_SKILL_JST_YOYO_HITOFPENYA01;	break;

		case SI_JST_YOYO_CRITICALSWING:	dwLinkSfxDouble = XI_SKILL_JST_SUP_CRITICALSWING01;	break;
		case SI_ACR_SUP_SLOWSTEP:	dwLinkSfxDouble = XI_SKILL_ACR_YOYO_SLOWSTEP01;	break;
		case SI_JST_SUP_POISON:		dwLinkSfxDouble = XI_SKILL_JST_SUP_POISON01;	break;
		case SI_JST_SUP_BLEEDING:	dwLinkSfxDouble = XI_SKILL_JST_SUP_BLEEDING01;	break;
		case SI_JST_YOYO_BACKSTAB:	dwLinkSfxDouble = XI_SKILL_JST_YOYO_BACKSTAB01;	break;
		case SI_JST_SUP_ABSORB:	dwLinkSfxDouble = XI_SKILL_JST_SUP_ABSORB01;	break;
			break;
		}
		if( dwLinkSfxDouble != NULL_ID )
		{
			CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxDouble, GetPos(), GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
			if( pSfx )
			{
				pSfx->SetPartLink( 0 );		// �����տ� ��ũ.
				pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxDouble, GetPos(), GetId() );  // ��ũ���� �ϳ��� ����.
				if( pSfx )
					pSfx->SetPartLink( 1 );		// �տ� ��ũ.
			}
		}
	}

	// �޼� ��ũ ����
	DWORD dwLinkSfxLeft = NULL_ID;
	switch( dwSkill )
	{	// ��ũ�κ� ��ų�̸� Ȱ ���� ����Ʈ �޼տ� ��ũ�Ѵ�
	case SI_RAG_BOW_ICEARROW:	dwLinkSfxLeft = XI_SKILL_RAG_BOW_ICEARROW01;	break;
	case SI_ACR_BOW_JUNKBOW:	dwLinkSfxLeft = XI_SKILL_ACR_BOW_JUNKBOW01;	break;
	case SI_ACR_BOW_AIMEDSHOT:	dwLinkSfxLeft = XI_SKILL_ACR_BOW_AIMEDSHOT01;	break;
	case SI_ACR_BOW_SILENTSHOT:	dwLinkSfxLeft = XI_SKILL_ACR_BOW_SILENTSHOT01;	break;
	case SI_ACR_BOW_ARROWRAIN:	dwLinkSfxLeft = XI_SKILL_ACR_BOW_ARROWRAIN01;      break;
	case SI_ACR_BOW_AUTOSHOT:	dwLinkSfxLeft = XI_SKILL_ACR_BOW_AUTOSHOT01;      break;
		
	case SI_RAG_SUP_FASTATTACK:	  dwLinkSfxLeft = XI_SKILL_RAG_SUP_FASTATTACK01;break;
	case SI_RAG_BOW_FLAMEARROW:	  dwLinkSfxLeft = XI_SKILL_RAG_BOW_FLAMEARROW01;break;
	case SI_RAG_BOW_PIRCINGARROW: dwLinkSfxLeft = XI_SKILL_RAG_BOW_PIRCINGARROW01;break;
	case SI_RAG_BOW_POISONARROW:  dwLinkSfxLeft = XI_SKILL_RAG_BOW_POISONARROW01;break;
	case SI_RAG_BOW_SILENTARROW:  dwLinkSfxLeft = XI_SKILL_RAG_BOW_SILENTARROW01;break;
	case SI_RAG_BOW_TRIPLESHOT:   dwLinkSfxLeft = XI_SKILL_RAG_BOW_TRIPLESHOT01;break;
		
		break;
	}
	
	if( dwLinkSfxLeft != NULL_ID )
	{
		CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxLeft, GetPos(), GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
		if( pSfx )
			pSfx->SetPartLink( 1 );		// �޼տ� ��ũ.
	}

	// ������ ��ũ ����
	DWORD dwLinkSfxRight = NULL_ID;
	switch( dwSkill )
	{
	case SI_MER_ONE_SPLMASH: dwLinkSfxRight = XI_SKILL_MER_ONE_SUPPORT04; break;
	case SI_MER_ONE_GUILOTINE: dwLinkSfxRight = XI_SKILL_MER_ONE_SUPPORT02; break;
	case SI_MER_ONE_REFLEXHIT: dwLinkSfxRight = XI_SKILL_MER_ONE_SUPPORT01; break;
	case SI_MER_ONE_SPECIALHIT: dwLinkSfxRight = XI_SKILL_MER_ONE_SUPPORT03; break;
	case SI_ASS_KNU_POWERFIST:	dwLinkSfxRight = XI_SKILL_ASS_KNU_SUPPORT03;	break;
	case SI_ASS_KNU_TAMPINGHOLE:	dwLinkSfxRight = XI_SKILL_ASS_KNU_SUPPORT02;	break;
	case SI_ASS_KNU_BURSTCRACK:	dwLinkSfxRight = XI_SKILL_ASS_KNU_SUPPORT01;	break;
	case SI_GEN_ATK_COUNTER:	dwLinkSfxRight = XI_SKILL_JST_YOYO_BACKSTAB01;	break;
		break;
	}
	
	if( dwLinkSfxRight != NULL_ID )
	{
		CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, dwLinkSfxRight, GetPos(), GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
		if( pSfx )
			pSfx->SetPartLink( 0 );		// �����տ� ��ũ.
	}

	// ���� Ȥ�� ������ų�� �ߵ����� ����Ʈ�߻�
	// �ڱ�id GetId()
	// Ÿ��id idFocusObj
	if( pSkillProp->dwSfxObj != NULL_ID )
	{
		CreateSfx( g_Neuz.m_pd3dDevice, pSkillProp->dwSfxObj, GetPos(), GetId() );  // ���������̱⶧���� ������ �ڱ⿡�� ��Ÿ����.
	}	
	// ���ݽ�ų�� Ÿ���� ���� �̸� ȭ�鿡 ����� ����
	if( pSkillProp->dwExeTarget != EXT_MELEEATK && 
		pSkillProp->dwExeTarget != EXT_MAGICATK && 
		pSkillProp->dwExeTarget != EXT_AROUNDATK &&  
		pSkillProp->dwExeTarget != EXT_MAGICATKSHOT && 
		pTarget->IsPlayer() )
	{
		CString str;
		if( this == g_pPlayer ) // ���� ����
		{
			str.Format( prj.GetText( TID_GAME_SKILL_SELF ), pSkillProp->szName );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_SKILL_SELF ) );	
		}
		else
		if( pTarget == g_pPlayer && ( this != pTarget || pSkillProp->dwID == SI_ASS_CHEER_CIRCLEHEALING ) ) // ���� ������ �ƴ϶��
		{
			str.Format( prj.GetText( TID_GAME_SKILL_MATE ), GetName(), pSkillProp->szName );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_SKILL_MATE ) );	
		}
	}
#endif	// __CLIENT

#ifdef __CLIENT	
	if( ::GetLanguage() == LANG_JAP )
	{
		if( g_pPlayer )
		{
			if( g_Option.m_bVoice )
			{
				if( IsActiveMover() )
				{
					int nindex = 0;
					
					if( pSkillProp->dwID != -1 )
					{
						if( GetSex() == 0 )
							nindex = pSkillProp->dwID+3000;
						else
							nindex = pSkillProp->dwID+3500;

						g_SoundMng.m_nOldSoundVolume = g_SoundMng.m_nSoundVolume;
						g_SoundMng.m_nSoundVolume = 0;
						PLAYSND( nindex );

						g_SoundMng.m_nSoundVolume = g_SoundMng.m_nOldSoundVolume;
					}
				}
			}
		}
	}
#endif	// __CLIENT

#ifdef __WORLDSERVER
	if( g_eLocal.GetState( EVE_PK ) )
	{
		if( !IsChaotic() && ( pTarget->IsPKPink() || ( pTarget->IsChaotic() && pSkillProp->nEvildoing > 0 ) ) )
		{
			SetPKPink( GetTickCount() + SEC(prj.m_PKSetting.nReadyAttack) );
			g_UserMng.AddPKPink( this, 1 );	
		}
	}
#endif // __WORLDSERVER
	
	return TRUE;
}

//
//	���Ӽ��� ���� ��ų�� �ð��� �ٵ� �������� ȣ���.
//
void	CMover::OnEndSkillState( DWORD dwSkill, DWORD dwLevel )
{
	if( dwSkill < 0 || (int)( dwSkill ) > prj.m_aPropSkill.GetSize() )
	{
		Error( "OnEndSkillState : ��ų�� ������ �Ѿ. %d", dwSkill );
		return;
	}
	ItemProp* pSkillProp = NULL;
	AddSkillProp *pAddSkillProp = NULL;
	GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, dwLevel, "OnEndSkillState" );

	if( dwSkill == SI_ACR_YOYO_COUNTER )	// ī���� ���ý�ų�� ���
	{
		m_pActMover->ResetState( OBJSTA_ATK_ALL );		// ���ݵ��� �ߴܽ�Ŵ.  (�̷� ������ ��ų�� ������ �Ϲ�ȭ ��Ű��)
	}
#if defined( __CLIENT ) 
/*	// 091022 mirchang - �����ؼ�, ��ٸ��� ��ų ���� ���� �� sfx ����
	#if __VER >= 11 // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
	if(	dwSkill == SI_MAG_FIRE_HOTAIR)
	{
		SetRemoveSfxObj( pSkillProp->dwSfxObj2 );
//		CSfx *pSfx = prj.GetSfx((this)->m_dwObjID,
//		if(pSfx)
//			pSfx->m_nSec = 0;	
	}
	if(	dwSkill == SI_PSY_HERO_STONE)
	{
		SetRemoveSfxObj( pSkillProp->dwSfxObj4 );
	}
	#endif // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
*/	// 091022 mirchang - �����ؼ�, ��ٸ��� ��ų ���� ���� �� sfx ����
	switch( dwSkill )	// 091022 mirchang - �����ؼ�, ��ٸ��� ��ų ���� ���� �� sfx ����
	{
		case SI_MAG_FIRE_HOTAIR :
			SetRemoveSfxObj( pSkillProp->dwSfxObj2 );
			break;
		case SI_PSY_HERO_STONE :
		case SI_MER_SHIELD_PANBARRIER:
		case SI_MER_SHIELD_PROTECTION:
			SetRemoveSfxObj( pSkillProp->dwSfxObj4 );
			break;
	}
  #endif	// 

	BOOL	bSend = FALSE;
#ifdef __WORLDSERVER
	bSend = TRUE;			// �����ϰ�� �����鿡�� ������.
	ResetDestParam( pAddSkillProp->dwDestParam1, pAddSkillProp->nAdjParamVal1, bSend );
	ResetDestParam( pAddSkillProp->dwDestParam2, pAddSkillProp->nAdjParamVal2, bSend );
#endif


}

// �Ϲ� ���� ������ �������� ȣ��.
void	CMover::OnEndMeleeAttack( DWORD dwState )
{
#ifdef __WORLDSERVER
	PostAIMsg( AIMSG_END_MELEEATTACK );
#endif // WorldServer
}

DWORD	CMover::GetRemoveSfxObj(DWORD	dwRemoveSfxObjID)
{
	DWORD	dwResult = 0;
	for(int i=0;i<MAX_SKILLBUFF_COUNT;i++)
	{
		if(dwRemoveSfxObj[i] ==  dwRemoveSfxObjID)
		{
			dwResult = dwRemoveSfxObj[i];
			dwRemoveSfxObj[i] = 0;
			return	dwResult;
		}
	}	
	return	dwResult;	
}

void	CMover::SetRemoveSfxObj(DWORD	dwRemoveSfxObjID)
{
	for(int i=0;i<MAX_SKILLBUFF_COUNT;i++)
	{
		if(dwRemoveSfxObj[i] ==  dwRemoveSfxObjID)
			return;
	}
	for( int i=0;i<MAX_SKILLBUFF_COUNT;i++)
	{
		if(dwRemoveSfxObj[i] ==  0)
		{
			dwRemoveSfxObj[i] = dwRemoveSfxObjID;
			return;
		}
	}
}
/*--------------------------------------------------------------------------------------------------------


											Do Use Item


----------------------------------------------------------------------------------------------------------*/

// ������ ID�� ������ ����(�κ��丮�� ������� ���)
BOOL CMover::DoUseItemVirtual( DWORD dwItemId, BOOL bEffectSkip )
{
#ifdef __WORLDSERVER
	//�κ����� ���� ���ô� ������ ������� ���డ���ϰ��Ѵ�.

	ItemProp* pItemProp = prj.GetItemProp( dwItemId );

	if( pItemProp )
	{
		if( IsItemRedyTime( pItemProp, pItemProp->dwID, FALSE ) == FALSE )
			return FALSE;

		D3DXVECTOR3 sPos = D3DXVECTOR3(0.0f,0.0f,0.0f);

		if( dwItemId == II_SYS_SYS_SCR_RETURN )
		{
			// ���ư��� 
			if( HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_RETURN ) )
				RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_RETURN );

			DWORD dwWorldID;
			D3DXVECTOR3 vPos = ((CUser*)this)->GetReturnPos( &dwWorldID );
			((CUser*)this)->REPLACE( g_uIdofMulti, dwWorldID, vPos, REPLACE_NORMAL, nTempLayer );
			return TRUE;
		}


		if( dwItemId == II_CHR_SYS_SCR_ESCAPEBLINKWING )
		{
			if( pItemProp->dwCircleTime != (DWORD)-1 )
			{
				if( !IsSMMode( SM_ESCAPE ) )
				{
					SetSMMode( SM_ESCAPE, pItemProp->dwCircleTime );
				}
				else
				{
					return FALSE;
				}
			}
			return DoUseItemBlinkWing( pItemProp, NULL, FALSE );
		}

		DWORD	dwSfxID = pItemProp->dwSfxObj3;
		if( dwSfxID != NULL_ID && !bEffectSkip )		// ������ ���ÿ� ������ ����Ʈ
		{
			g_UserMng.AddCreateSfxObj( this, dwSfxID, sPos.x, sPos.y, sPos.z );	// ������ǥ�� ����.
		}	
	}
#endif //__WORLDSERVER

	return TRUE;
}

// IK2_BLINKWING�� ó��.
BOOL CMover::DoUseItemBlinkWing( ItemProp *pItemProp, CItemElem* pItemElem, BOOL bUse )
{
#ifdef __WORLDSERVER
	if( m_pActMover->IsSit() ) // 2005.12.08 raiders �ɾ� ������ �ȵǰ� 
		return FALSE;

	if( IsStateMode( STATE_BASEMOTION_MODE ) ) 
		return FALSE;

	if( bUse )
	{
		if( GetWorld()->m_dwWorldID == WI_WORLD_KEBARAS )
			return FALSE;
	}

	if( GetWorld()->m_dwWorldID == WI_WORLD_GUILDWAR )
		return FALSE;

	if( g_GuildCombat1to1Mng.IsPossibleUser( (CUser*)this ) )
	{
		if( IsPlayer() )
			((CUser*)this)->AddDefinedText( TID_GAME_GUILDCOMBAT1TO1_NOBLINKWING );
		return FALSE;
	}

	if( CSecretRoomMng::GetInstance()->IsInTheSecretRoom( this ) )
	{
		if( IsPlayer() )
			((CUser*)this)->AddDefinedText( TID_GAME_SECRETROOM_USEBRINKITEM );
		return FALSE;
	}

	if( CRainbowRaceMng::GetInstance()->IsEntry( ((CUser*)this)->m_idPlayer ) )
	{
		if( IsPlayer() )
			((CUser*)this)->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
		return FALSE;
	}

	if( GetWorld()->GetID() == WI_WORLD_MINIROOM )	// �Ͽ�¡������ ��ũ�� ����.
		return FALSE;

	// ���ѷ����� �ɷ�������.
 	if( pItemProp->dwLimitLevel1 != NULL_ID && (int)( pItemProp->dwLimitLevel1 ) > GetLevel() )
	{
		if( TRUE == IsPlayer() )
			( (CUser*)this )->AddDefinedText( TID_GAME_USINGNOTLEVEL, "\"%s\"", pItemProp->szName );
		return FALSE;
	}

	if( pItemProp->dwItemKind3 == IK3_TOWNBLINKWING )	// IK3_TOWNBLINKWING
	{
		REPLACE_TYPE type = REPLACE_NORMAL;
		if( pItemProp->dwID == II_CHR_SYS_SCR_ESCAPEBLINKWING )
			type = REPLACE_FORCE; 

		// lodestar
		if( bUse )
		{
			if( !IsUsableItem( pItemElem ) )
				return FALSE;

			pItemElem->UseItem();
			UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum );
		}
		
		PRegionElem pRgnElem	= NULL;
		CWorld* pWorld	= GetWorld();
		if( !pWorld )
			return FALSE;

		if( IsChaotic() )
		{
			if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				pRgnElem	= g_WorldMng.GetRevivalPosChao( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), GetPos() );
		}
		else

		{
			if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
				pRgnElem	= g_WorldMng.GetRevivalPos( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPos( pWorld->GetID(), GetPos() );
		}
		if( NULL != pRgnElem )
			REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, type, nRevivalLayer );
		else
			REPLACE( g_uIdofMulti, pWorld->GetID(), GetPos(), type, nDefaultLayer );
	}
	else
	{
		//		x: dwItemAtkOrder1
		//		y: dwItemAtkOrder2
		//		z: dwItemAtkOrder3
		//		angle: dwItemAtkOrder4
		//		dwWeaponType: worldid
		if( pItemProp->dwWeaponType == WI_WORLD_NONE || pItemProp->dwWeaponType == 0xffffffff )
			return FALSE;
		D3DXVECTOR3 vPos( (float)( pItemProp->dwItemAtkOrder1 ), (float)( pItemProp->dwItemAtkOrder2 ), (float)( pItemProp->dwItemAtkOrder3 ) );
		SetAngle( (float)pItemProp->dwItemAtkOrder4 );
		// remove & transfer
		if( bUse )
		{
			if( !IsUsableItem( pItemElem ) )
				return FALSE;

			pItemElem->UseItem();
			UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_NUM, pItemElem->m_nItemNum );
		}
		if( IsChaotic() )
		{
			PRegionElem pRgnElem	= NULL;
			CWorld* pWorld = g_WorldMng.GetWorld( pItemProp->dwWeaponType );
			if( pWorld )
				pRgnElem	= g_WorldMng.GetRevivalPosChao( pItemProp->dwWeaponType, pItemProp->szTextFileName );
			if( NULL == pRgnElem )	// Find near revival pos
				pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), GetPos() );

			if( pRgnElem )
				REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
		}
		else
		{
			REPLACE( g_uIdofMulti, pItemProp->dwWeaponType, vPos, REPLACE_NORMAL, nTempLayer );
		}
	}
#endif // worldserver
	return TRUE;
}

#ifdef __WORLDSERVER
BOOL CMover::IsDoUseBuff( ItemProp* pItemProp )
{
	int nResult = 0;
	if( pItemProp->dwItemKind3 == IK3_POTION_BUFF_STR
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_DEX
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_INT
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_STA
		|| pItemProp->dwItemKind3 == IK3_POTION_BUFF_DEFENSE		
		|| pItemProp->dwItemKind3 == IK3_ANGEL_BUFF		
#ifdef __BUFF_TOGIFT
		|| pItemProp->dwItemKind2 == IK2_BUFF_TOGIFT
#endif // __BUFF_TOGIFT
		)
	{
		if( HasBuffByIk3( pItemProp->dwItemKind3 ) )
			nResult = 1;

#ifdef __PKSERVER_USE_ANGEL
		if( pItemProp->dwItemKind3 == IK3_ANGEL_BUFF )
		{
			if(g_eLocal.GetState( EVE_PK ))	
			{
				if( 0 >= GetPKValue() )
					nResult = 2;
				if( GetQuest( QUEST_PKMINUS ) == NULL )
					nResult = 3;
				if( m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_RED100 ) != NULL 
					|| m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_BLUE100 ) != NULL 
					|| m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_GREEN100 ) != NULL 
					|| m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_WHITE100 ) != NULL 
					)
					nResult = 4;
			}
			else
				nResult = 5;
		}
#else //__PKSERVER_USE_ANGEL
		if( pItemProp->dwItemKind3 == IK3_ANGEL_BUFF )
		{
			if( 0 >= GetPKValue() )
				nResult = 2;
			if( GetQuest( QUEST_PKMINUS ) == NULL )
				nResult = 3;
			if( m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_RED100 ) != NULL 
				|| m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_BLUE100 ) != NULL 
				|| m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_GREEN100 ) != NULL 
				|| m_Inventory.GetAtItemId( II_SYS_SYS_QUE_ANGEL_WHITE100 ) != NULL 
				)
				nResult = 4;
		}
#endif //__PKSERVER_USE_ANGEL
	}
	else if( pItemProp->dwID == II_SYS_SYS_SCR_DEFENSE20 || pItemProp->dwID == II_SYS_SYS_SCR_DEFENSE50 )
	{
		if( HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_DEFENSE20 ) || HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_DEFENSE50 ) )
			nResult = 1;
	}
#ifdef __JEFF_11_1
	else if( pItemProp->dwID == II_SYS_SYS_SCR_PET_FEED_POCKET02 )
	{
		int nMax	= m_Inventory.GetMax();
		CItemElem* pItem;
		for( int i = 0 ; i < nMax; i++ )
		{
			pItem	= m_Inventory.GetAtId( i );
			if( IsUsableItem( pItem )
				&& pItem->m_dwItemId == II_SYS_SYS_SCR_PET_FEED_POCKET
				&& pItem->m_dwKeepTime > 0
				&& !pItem->IsFlag( CItemElem::expired )
				)
			{
				nResult		= 1;
				break;
			}
		}
		if( nResult != 1 )
		{
		#ifdef __BUFF_1107
			IBuff* pBuff	= m_buffs.GetBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) );
			if( pBuff )
			{
				DWORD dwCurr	= ::timeGetTime();
				if( static_cast<int>( pItemProp->dwSkillTime ) > static_cast<int>( pItemProp->dwSkillTime + ( pBuff->GetTotal() - ( dwCurr - pBuff->GetInst() ) ) ) )
					nResult		= 1;
			}
		#else	// __BUFF_1107
			LPSKILLINFLUENCE ptr	= m_SkillState.FindPtr( BUFF_ITEM, pItemProp->dwID );
			if( ptr )
			{
				DWORD dwCurr	= ::timeGetTime();
				if( static_cast<int>( pItemProp->dwSkillTime ) > static_cast<int>( pItemProp->dwSkillTime + ( ptr->tmCount - ( dwCurr - ptr->tmTime ) ) ) )
					nResult		= 1;
			}
		#endif	// __BUFF_1107
		}
	}
#endif	// __JEFF_11_1
	else if( HasBuff( BUFF_ITEM, (WORD)( pItemProp->dwID ) ) )
	{
		nResult = 1;
	}
	else if( pItemProp->dwID == II_GEN_TOO_COL_SILVERBATTERY
		|| pItemProp->dwID == II_GEN_TOO_COL_GOLDBATTERY
		|| pItemProp->dwID == II_GEN_TOO_COL_BATTERY001
		)
	{
		if( HasBuff( BUFF_ITEM2, II_GEN_TOO_COL_SILVERBATTERY )
			|| HasBuff( BUFF_ITEM2, II_GEN_TOO_COL_GOLDBATTERY )
			|| HasBuff( BUFF_ITEM2, II_GEN_TOO_COL_BATTERY001 )
			)
			nResult = 1;
	}
	#define II_SYS_SYS_SCR_PARTYEXPUP01_01 20296
#define II_SYS_SYS_SCR_PARTYSKILLUP01_01 20297
	else if( pItemProp->dwID == II_SYS_SYS_SCR_PARTYEXPUP01
		|| pItemProp->dwID == II_SYS_SYS_SCR_PARTYEXPUP02
		|| pItemProp->dwID == II_SYS_SYS_SCR_PARTYEXPUP01_01
		)
	{
		if( HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYEXPUP01 )
			|| HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYEXPUP02 )
			|| HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYEXPUP01_01 )
			)
			nResult = 1;
	}
	else if( pItemProp->dwID == II_SYS_SYS_SCR_SPETGOOD
		)
	{
		if( HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_SPETGOOD )
			)
			nResult = 1;
	}
	else if( pItemProp->dwID == II_SYS_SYS_SCR_PARTYSKILLUP01
		|| pItemProp->dwID == II_SYS_SYS_SCR_PARTYSKILLUP02
		|| pItemProp->dwID == II_SYS_SYS_SCR_PARTYSKILLUP01_01
		)
	{
		if( HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYSKILLUP01 )
			|| HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYSKILLUP02 )
			|| HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYSKILLUP01_01 )
			)
			nResult = 1;
	}
	
	switch( nResult )
	{
	case 1:
		((CUser*)this)->AddDefinedText( TID_GAME_LIMITED_USE, "" ); // ������̾ ����Ҽ� ����
		break;
	case 2:
		((CUser*)this)->AddDefinedText( TID_GAME_PKVALUE_NOT_ANGEL );
		break;
	case 3:
		((CUser*)this)->AddDefinedText( TID_GAME_NOT_QUEST_ANGEL );
		break;	
	case 4:
		((CUser*)this)->AddDefinedText( TID_GAME_NOT_COMPLETE_ANGEL );
		break;	
	}
	return nResult;
}
#endif // __WORLDSERVER

#ifdef __WORLDSERVER
BOOL CMover::DoUseItemSexChange( int nFace )
{
	// 1.
	for( DWORD dwParts = 0; dwParts < MAX_HUMAN_PARTS; dwParts++ )
	{
		if( dwParts == PARTS_HEAD || dwParts == PARTS_HAIR || dwParts == PARTS_LWEAPON
			|| dwParts == PARTS_RWEAPON || dwParts == PARTS_SHIELD || dwParts == PARTS_RIDE
			|| ( dwParts >= PARTS_NECKLACE1 && dwParts <= PARTS_BULLET ) )
			continue;
		
		CItemElem* pArmor	= m_Inventory.GetEquip( dwParts );
		if( pArmor )
		{
			((CUser*)this)->AddDefinedText( TID_GAME_CHECK_EQUIP, "" );
				return FALSE;
		}
	}

	// 2.
	DWORD dwIndex;
	if( GetSex() == SEX_MALE )
	{
		dwIndex		= MI_FEMALE;
		SetSex( SEX_FEMALE );
	}
	else
	{
		dwIndex	= MI_MALE;
		SetSex( SEX_MALE );
	}

	SetTypeIndex( D3DDEVICE, OT_MOVER, dwIndex );
	ResetScale();
	SetMotion( MTI_WALK );
	//RedoEquip( FALSE );		// chipi_091125 ����
	RedoEquip( FALSE, FALSE );	// chipi_091125 �߰� - �Ǽ��縮, ���� ���� �������� ���� ���·� Ʈ������ �����ѵ� �ɷ�ġ�� �ٽ�(�ߺ�) �����Ű�� ������ �־���.
	UpdateLocalMatrix();

	// 3.
	g_UserMng.AddSexChange( this );

	// 4.
	m_dwHeadMesh	= (DWORD)nFace;
	g_UserMng.AddChangeFace( m_idPlayer, (DWORD)nFace );

	g_dpDBClient.SendUpdatePlayerData( static_cast<CUser*>(this) );

	return TRUE;
}
#endif	// __WORLDSERVER

/*--------------------------------------------------------------------------------------------------------


											Do Wand Attack


----------------------------------------------------------------------------------------------------------*/
//
// �ϵ������ �϶�!
//
int	 CMover::DoAttackMagic( CObj *pTargetObj, int nMagicPower, int idSfxHit )
{
	if( IsInvalidObj(pTargetObj) )		// Ÿ���� �Žñ� �ϸ� ����
		return( -1 );

	ItemProp* pItemProp;
#ifndef __CLIENT
	CItemElem* pItemElem = GetWeaponItem();	// �տ��� ���Ⱑ ������ ����
	if( pItemElem == NULL )
		return( - 1 ) ;
	pItemProp	= pItemElem->GetProp();
#else	// __CLIENT
	if( IsActiveMover() ) {
		CItemElem* pItemElem = GetWeaponItem();
		if( pItemElem == NULL )
			return( - 1 ) ;
		pItemProp	= pItemElem->GetProp();
	}
	else {
		DWORD dwItemId	= m_aEquipInfo[PARTS_RWEAPON].dwId;
		if( dwItemId == NULL_ID )
			return( -1 );
		pItemProp	= prj.GetItemProp( dwItemId );
	}
#endif	// __CLIENT

	if( pItemProp->dwItemKind3 != IK3_WAND )		// ����ִ� ���Ⱑ �ϵ尡 �ƴϸ� ����.
		return( -1 );

#ifdef __CLIENT
	// ���� ������ ��ǥ�������� �����°� ������ �޽��� ����ϰ� ����
	CWorld *pWorld = GetWorld();
	D3DXVECTOR3 vStart = GetPos();				vStart.y += 0.5f;
	D3DXVECTOR3 vEnd   = pTargetObj->GetPos();	vEnd.y += 0.5f;

	if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
	{
		g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
		return -1;
	}
#endif  // CLIENT

	OBJID	idTarget = ((CCtrl*)pTargetObj)->GetId();

	int nError;
	if( ( nError = m_pActMover->SendActMsg( OBJMSG_ATK_MAGIC1, idTarget ) ) == 1 )	// �ϵ���� ���� ����.
	{
		if( m_pActMover->IsFly() == FALSE )		
			SetAngle( GetDegree(pTargetObj->GetPos(), GetPos()) );		// ��ǥ������ ���� ����.
		SetActParam( OBJACT_MAGIC_ATTACK, idTarget, nMagicPower );		// ������ ����� �Ķ���� ���
		
#ifdef __CLIENT	
		if( nMagicPower >= 1 )	// ���� 2�̻��� ���� �׼�����Ʈ �ϳ� �ö�.
		{
			if( g_WndMng.m_pWndTaskBar->m_nActionPoint < 100 )
				g_WndMng.m_pWndTaskBar->m_nActionPoint ++;
		}
#endif
		
#ifdef __WORLDSERVER
		if( IsPlayer() )
		{
			if( nMagicPower >= 1 && ((CUser*)this)->m_playTaskBar.m_nActionPoint < 100 )	// 2�ܰ������̻� ����ϸ� �׼�����Ʈ �ö�.
				((CUser*)this)->m_playTaskBar.m_nActionPoint ++;
		}
		if( nMagicPower == 3 )		// 4�ܰ�¥���� ���� ���� 5����Ʈ ����
			IncManaPoint( -GetReqMp( pItemProp->dwReqMp ) );	
		g_UserMng.AddMagicAttack( this, OBJMSG_ATK_MAGIC1, idTarget, 0, 0, nMagicPower, idSfxHit );
#else	// __WORLDSERVER
#ifdef __CLIENT
		if( IsActiveMover() ) {
			SetActParam( OBJACT_MAGIC_ATTACK, idTarget, nMagicPower );	// ȿ�� �ĺ��� ���
			g_DPlay.SendMagicAttack( OBJMSG_ATK_MAGIC1, ( (CCtrl*)pTargetObj )->GetId(), 0, 0, nMagicPower, m_idSfxHit );
		}
#endif	// __CLIENT
#endif	// __WORLDSERVER
	}
	return( nError );
}

/*--------------------------------------------------------------------------------------------------------


											Do Range Attack


----------------------------------------------------------------------------------------------------------*/
//
// ������������ �϶�!
// dwItemID : ��������� ���̵�
//
int	 CMover::DoAttackRange( CObj *pTargetObj, DWORD dwItemID, int idSfxHit )
{
	ItemProp* pItemProp = NULL;
	int nPower = 0;

	if( IsInvalidObj(pTargetObj) )
		return -1;
	

#ifdef __CLIENT
	if( IsActiveMover() )
	{
		// ���� ������ ��ǥ�������� �����°� ������ �޽��� ����ϰ� ����
		CWorld *pWorld = GetWorld();
		D3DXVECTOR3 vStart = GetPos();				vStart.y += 0.5f;
		D3DXVECTOR3 vEnd   = pTargetObj->GetPos();	vEnd.y += 0.5f;

		if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
			
			return -1;
		}
	}
#endif  // CLIENT
	

	if( IsPlayer() )
	{
		pItemProp = GetActiveHandItemProp();			
		nPower = dwItemID;

	#ifdef __WORLDSERVER
		CItemElem* pItemElem = m_Inventory.GetEquip( PARTS_BULLET );	// ȭ���� �ִ��� üũ
		if( pItemElem == NULL || pItemElem->GetProp()->dwItemKind3 != IK3_ARROW )
			return -1;
	#endif // __WORLDSERVER
	}
	else
	{
		if( dwItemID == NULL_ID || dwItemID == 0 )	
			pItemProp = GetActiveHandItemProp();		// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
		else
			pItemProp = prj.GetItemProp( dwItemID );
	}
	
	if( pItemProp == NULL )
	{
		Error( "DoAttackRange : %s ActiveHandItemProp = NULL", m_szName );
		return -1;
	}

	if( pItemProp->dwWeaponType != WT_RANGE && pItemProp->dwWeaponType != WT_RANGE_BOW )		// ����ִ� ���Ⱑ RANGE�� �ƴϸ� ����
	{
		return -1;
	}
	
	DWORD dwUseMotion = pItemProp->dwUseMotion;	
	if( dwUseMotion == NULL_ID )
	{
		if( IsPlayer() )
		{
			dwUseMotion = MTI_ATK_13 - MTI_STAND_13;//23;
		}
		else
		{
			Error( "DoAttackRange : %s [%d]'s dwUseMotion = NULL_ID", m_szName, pItemProp->dwID );
			dwUseMotion = MTI_ATK1;		// ������Ƽ�� ��Ÿ� ���� ����� �������� �ʾ����� �⺻ ������� �Ѵ�.
		}
	}

	OBJID	idTarget = ((CCtrl*)pTargetObj)->GetId();

	int nError;
	if( ( nError = m_pActMover->SendActMsg( OBJMSG_ATK_RANGE1, idTarget, (int)dwUseMotion ) ) == 1 )	// ��Ÿ�����.
	{
		SetAngle( GetDegree(pTargetObj->GetPos(), GetPos()) );			// ��ǥ������ ���� ����.
		SetActParam( OBJACT_RANGE_ATTACK, idTarget, dwItemID, idSfxHit );	// ȿ�� �ĺ��� ���
		
	#ifdef __WORLDSERVER
		g_UserMng.AddRangeAttack( this, OBJMSG_ATK_RANGE1, idTarget, dwItemID, 0, idSfxHit );
		ArrowDown( 1 );		// ȭ�� 1�� ����
	#else	// __WORLDSERVER
		if( IsActiveMover() )	// Ŭ�󿡼� �÷��̾ ����ѰŶ�� ������ ���� ����.
			g_DPlay.SendRangeAttack( OBJMSG_ATK_RANGE1, ( (CCtrl*)pTargetObj )->GetId(), dwItemID, m_idSfxHit ); 
	#endif	// __WORLDSERVER
	}
	return nError;
}

// ���� Ư������
int		CMover::DoAttackSP( CObj *pTargetObj, DWORD dwItemID )
{
	if( IsInvalidObj(pTargetObj) )
		return( -1 );
	
	ItemProp* pItemProp;
	if( dwItemID == NULL_ID || dwItemID == 0 )	
		pItemProp = GetActiveHandItemProp();	// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else
		pItemProp = prj.GetItemProp( dwItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)
	if( pItemProp == NULL )
	{
		Error( "DoAttackSP : %s �� ItemProp�� NULL %d", m_szName, (int)dwItemID );
		return FALSE;
	}

	DWORD dwUseMotion = pItemProp->dwUseMotion;
	if( dwUseMotion == NULL_ID )
	{
		Error( "DoAttackSP : %s�� ����%d�� dwUseMotion�� �������� �ʾҴ�", m_szName, pItemProp->dwID );
		dwUseMotion = MTI_ATK1;		// ������Ƽ�� ��Ÿ� ���� ����� �������� �ʾ����� �⺻ ������� �Ѵ�.
	}
	
	OBJID	idTarget = ((CCtrl*)pTargetObj)->GetId();
	
	int nError;
	if( ( nError = m_pActMover->SendActMsg( OBJMSG_SP_ATK1, idTarget, (int)dwItemID, (int)dwUseMotion ) ) == 1 )	// ��Ÿ�����.
	{
		SetAngle( GetDegree(pTargetObj->GetPos(), GetPos()) );		// ��ǥ������ ���� ����.
		SetActParam( OBJACT_SP_ATTACK, idTarget, (int)dwItemID );	// ȿ�� �ĺ��� ���
		
#ifdef __WORLDSERVER
		g_UserMng.AddAttackSP( this, OBJMSG_SP_ATK1, idTarget, dwItemID, 0 );
#else	// __WORLDSERVER
	
#if 0 //2009_12_18 ��ȹ���� ���� ��û : �ߵ�Ÿ�ӿ� Ÿ�ٿ� ���� ����Ʈ �ʿ���� 
		DWORD dwSfxObj = pItemProp->dwSfxObj3;		// Ư�����ݿ� �ߵ�����Ʈ�� �ִٸ� 3�����.
		if( dwSfxObj != NULL_ID )
			CreateSfx( D3DDEVICE, dwSfxObj, GetPos(), GetId(), D3DXVECTOR3(0,0,0), idTarget );
#endif 

	//gmpbigsun: �ߵ��� �����ڿ� ���� �ߵ� effect
	DWORD dwSfxObj = pItemProp->dwSfxObj;
	if( NULL_ID != dwSfxObj )
		CreateSfx( D3DDEVICE, dwSfxObj, GetPos() );

		
#endif	// __WORLDSERVER
	}

	return( nError );
}

int		CMover::DoAttackMelee( CMover *pTarget, OBJMSG dwMsg, DWORD dwItemID )
{
	SendActMsg( OBJMSG_STOP );		// ���ڸ� ����.

	if( IsInvalidObj(pTarget) )
		return 0;
	
	ItemProp* pItemProp;
	if( dwItemID == NULL_ID || dwItemID == 0 )	
		pItemProp = GetActiveHandItemProp();	// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else
		pItemProp = prj.GetItemProp( dwItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)
	if( pItemProp == NULL )
	{
		Error( "DoAttackMelee : %s �� ItemProp�� NULL %d", GetName(), (int)dwItemID );
		return FALSE;
	}

	// ��� �����ϰ�.
	int nError	= SendActMsg( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, 0 ) );
	if( nError > 0 )
	{
		SetActParam( OBJACT_MELEE_ATTACK, pTarget->GetId(), (int)dwItemID );	// OnAttackMelee���� ���� �ֵ��� �Ķ���� ���.

#ifdef __CLIENT
#ifdef __HACK_1023
		ItemProp* pHandItemProp		= GetActiveHandItemProp();
		FLOAT fVal	= pHandItemProp? pHandItemProp->fAttackSpeed: 0.0F;
		g_DPlay.SendMeleeAttack( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, (WORD)nError ), fVal );
#else	// __HACK_1023
		g_DPlay.SendMeleeAttack( dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, (WORD)nError ) );
#endif	// __HACK_1023
		if( IsActiveMover() )
		{
			PlayCombatMusic();
		}
#else	// __CLIENT
		g_UserMng.AddMeleeAttack( this, dwMsg, pTarget->GetId(), MAKELONG( 0, 0 ), MAKELONG( 0, (WORD)nError ) );
#endif	// __CLIENT
	}

	return TRUE;	
}	



// �ֺ��� �ִ� �������� �ϳ��� ���´�.
void	CMover::DoPickupItemAround()
{
#ifdef __CLIENT
	if( m_pActMover->IsActAttack() )	return;		// ������ ���ȵ�.
	if( IsDie() )		return;						// �׾����� ���ȵ�.
	
	int nRange	= 4;	// 4m
	float fDistMin = 99999.0f, fDistSq;
	CObj* pObj, *pMinObj = NULL;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;
	FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
	{
		if( pObj->GetType() == OT_ITEM )
		{
			vDist = pObj->GetPos() - vPos;		// this -> �����۱����� ����
			fDistSq = D3DXVec3LengthSq( &vDist );
			if( fDistSq < 4.0f * 4.0f )		// �����۰��� �Ÿ��� x���� �̳��ΰ��� �������.
			{
				if( fDistSq < fDistMin )	// ���� �� ���������� ����
				{
					fDistMin = fDistSq;
					pMinObj = pObj;
				}
			}
		}
	}
	END_LINKMAP

	// ������ ������ �ִ�.
	if( pMinObj )
	{
		CMD_SetUseItem( (CCtrl*)pMinObj );
	}
#endif // __CLIENT
}


// this�� Ÿ������ �ֺ��� �������� �ش�.
// ------------------------------------------
// nDmgType : ������ Ÿ�� ex) AF_MAGICSKILL
// pAttacker : ������(������) - ������ CCtrl*�� �ٲ�� �Ѵ�
// nApplyType : ������. ex) OBJTYPE_PLAYER | OBJTYPE_MONSTER
// nSkill : ����� ��ų����
// fRange : Ÿ���� �߽������� ������ ����(����)
// fTargetRatio : Ÿ�ٿ��� ����Ǵ� ������ ����
// fAroundRatio : �ֺ��� ����Ǵ� ������ ����
// (AF_MAGICSKILL, this, nSkill, 5.0f, 0.0, 1.0f )
void	CMover::SendDamageAround( int nDmgType, CMover *pAttacker, int nApplyType, int nAttackID, float fRange, float fTargetRatio, float fAroundRatio )
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	float fDistSq;
	CObj* pObj;
	CMover *pTarget;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;

	if( fRange <= 4.0f )
		nRange = 4;
	else if( fRange <= 8.0f )
		nRange = 8;
	else if( fRange <= 16.0f )
		nRange = 16;
	else
		nRange = 32;
	
	if( fRange <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		Error( "CMover::SendDamageAround : D:%s A:%s %d %f", GetName(), pAttacker->GetName(), nAttackID, fRange );

	ItemProp* pProp;
	if( nDmgType == AF_MAGICSKILL )
	{
		pProp = prj.GetSkillProp( nAttackID );		// UseSkill���� ����� ��ų�� ������Ƽ ����
		if( pProp == NULL )
		{
			Error( "CMover::SendDamageAround : %s. ��ų(%d)�� ������Ƽ�� ����.", m_szName, nAttackID );
			return;	// property not found
		}
	} else
	{
		pProp = prj.GetItemProp( nAttackID );		// ������ ������Ƽ����
		if( pProp == NULL )
		{
			Error( "CMover::SendDamageAround : %s. ������(%d)�� ������Ƽ�� ����.", m_szName, nAttackID );
			return;	// property not found
		}
	}
	
	BOOL	bDamage = FALSE;
	BOOL	bTarget = FALSE;

	if( nApplyType & OBJTYPE_PLAYER )	// �������� �÷��̾��ΰ� 
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER )
			{
				bDamage = TRUE;

				if( bDamage )
				{
					vDist = pObj->GetPos() - vPos;		// this -> Ÿ�ٱ����� ����
					fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < fRange * fRange )		// Ÿ�ٰ��� �Ÿ��� fRange���� �̳��ΰ��� �������.
					{
						if( pObj == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
						{
						} else
						{
							if( pObj != pAttacker )		// ����Ŀ�� �˻���󿡼� ����.
							{
								pTarget = (CMover *)pObj;
								bTarget = ( pTarget == this );
								if( IsValidObj( (CObj*)pTarget ) && pTarget->IsLive() )
								{
									if( pProp->dwComboStyle == CT_FINISH )
										pTarget->m_pActMover->SendDamageForce( nDmgType, pAttacker->GetId(), (nAttackID << 16), bTarget );
									else
										pTarget->m_pActMover->SendDamage( nDmgType, pAttacker->GetId(), (nAttackID << 16), bTarget );
								}
							}
						}
					}
					
					bDamage = FALSE;	// ���� ������ ���ؼ� �ʱ�ȭ.
				} // bDamage
			}
		}
		END_LINKMAP
	}

	// �������� �����ΰ�.
	if( nApplyType & OBJTYPE_MONSTER )
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER && ((CMover *)pObj)->IsPeaceful() == FALSE )
			{
				vDist = pObj->GetPos() - vPos;		// this -> Ÿ�ٱ����� ����
				fDistSq = D3DXVec3LengthSq( &vDist );
				if( fDistSq < fRange * fRange )		// Ÿ�ٰ��� �Ÿ��� fRange���� �̳��ΰ��� �������.
				{
					if( pObj == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
					{
					} else
					{
						if( pObj != pAttacker )		// �����ڴ� �˻��󿡼� ����.
						{
							pTarget = (CMover *)pObj;
							if( IsValidObj( (CObj*)pTarget ) && pTarget->IsLive() )
							{
								if( pProp->dwComboStyle == CT_FINISH )
									pTarget->m_pActMover->SendDamageForce( nDmgType, pAttacker->GetId(), (nAttackID << 16) );
								else
									pTarget->m_pActMover->SendDamage( nDmgType, pAttacker->GetId(), (nAttackID << 16) );
							}
						}
					}
				}
			}
		}
		END_LINKMAP
	}
		
#endif // WORLDSERVER		
} // SendDamageAround()
/*
BOOL	CMover::DoCollect( CMover *pTarget )
{
	SendActMsg( OBJMSG_STOP );
	BOOL bSuccess = SendActMsg( OBJMSG_COLLECT );
	ClearDest();
		
	return bSuccess;
}
*/


// 
// ��, ����, ���̸� �����Ͽ� ����ü ������ ������ ������ ����.
// nDmgType : AF_???
// nApplyType : OBJTYPE_PLAYER , MONSTER
// nAttackID : ��ų�̳� ������ ID
// fWidth, fHeight, fDepth : ��, ����, ����.
// this == Attacker
void	CMover::SendDamageLine( int nDmgType, int nApplyType, int nAttackID, 
								float fWidth, float fHeight, float fDepth )
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m

	CObj* pObj;
	CMover *pTarget;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;

	// ������ ���� Local AABB.
	D3DXVECTOR3 vMin1 = D3DXVECTOR3( -fWidth / 2.0f, -fHeight / 2.0f, -fDepth );
	D3DXVECTOR3 vMax1 = D3DXVECTOR3(  fWidth / 2.0f,  fHeight / 2.0f, 0 );

	D3DXMATRIX mInv;		// �������� ���忪���
	D3DXMATRIX m1, m2, m3;
	D3DXMatrixRotationY( &m1, D3DXToRadian( -GetAngle() ) );
	D3DXMatrixTranslation( &m2, GetPos().x, GetPos().y, GetPos().z );
	D3DXMatrixMultiply( &m3, &m1, &m2 );	// world = rot * trans
	
	D3DXMatrixInverse( &mInv, NULL, &m3 );		// �������� ����� ����.

	// Ÿ���� ��ǥ�� �������� �������� ����ȯ �Ѱ�.
	D3DXVECTOR3 vDestLocal;

	// �Ϲ������� fDepth�� ���� ��⶧���� �˻� ������ fDepth�� �ߴ�. 
	if( fDepth <= 4.0f )
		nRange = 4;
	else if( fDepth <= 8.0f )
		nRange = 8;
	else if( fDepth <= 16.0f )
		nRange = 16;
	else
		nRange = 32;
	
	if( fDepth <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		Error( "CMover::SendDamageLine : D:%s A:%s %d %f", GetName(), nAttackID, fDepth );

	ItemProp* pProp;
	if( nDmgType == AF_MAGICSKILL )
	{
		pProp = prj.GetSkillProp( nAttackID );		// UseSkill���� ����� ��ų�� ������Ƽ ����
		if( pProp == NULL )
		{
			Error( "CMover::SendDamageLine : %s. ��ų(%d)�� ������Ƽ�� ������.", m_szName, nAttackID );
			return;	// property not found
		}
	} else
	{
		pProp = prj.GetItemProp( nAttackID );		// ������ ������Ƽ����
		if( pProp == NULL )
		{
			Error( "CMover::SendDamageLine : %s. ������(%d)�� ������Ƽ�� ������.", m_szName, nAttackID );
			return;	// property not found
		}
	}
	
	BOOL	bDamage = FALSE;

	if( nApplyType & OBJTYPE_PLAYER )	// �������� �÷��̾��ΰ� 
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER )
			{
				bDamage = TRUE;	

				if( bDamage && pObj != this )		// this�� ���ݴ�󿡼� ����.
				{
					pTarget = (CMover *)pObj;
					if( IsValidObj( pTarget ) && pTarget->IsLive() )
					{
						// Ÿ���� ��ǥ�� ����ȯ.
						const auto targetPos = pTarget->GetPos();
						D3DXVec3TransformCoord( &vDestLocal, &targetPos, &mInv );
						// Ÿ���� AABB
						D3DXVECTOR3 vMin2 = vDestLocal + pTarget->m_pModel->m_vMin;
						D3DXVECTOR3 vMax2 = vDestLocal + pTarget->m_pModel->m_vMax;
						if( ::IsTouchAABB( vMin1, vMax1, vMin2, vMax2 ) )		// AABB�浹�˻�.
						{
						//	if( pProp->dwComboStyle == CT_FINISH )
						//		pTarget->m_pActMover->SendDamageForce( nDmgType, pAttacker->GetId(), (nAttackID << 16) );
						//	else
						//		pTarget->m_pActMover->SendDamage( nDmgType, pAttacker->GetId(), (nAttackID << 16) ); 					
						}
					}
				} // bDamage
			}
		}
		END_LINKMAP
	}

	// �������� �����ΰ�.
	if( nApplyType & OBJTYPE_MONSTER )
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
		{
			if( pObj->GetType() == OT_MOVER && ((CMover *)pObj)->IsPeaceful() == FALSE )
			{
				pTarget = (CMover *)pObj;
				if( IsValidObj( pTarget ) && pTarget->IsLive() )
				{
					// Ÿ���� ��ǥ�� ����ȯ.
					const D3DXVECTOR3 targetPos = pTarget->GetPos();
					D3DXVec3TransformCoord( &vDestLocal, &targetPos, &mInv );
					// Ÿ���� AABB
					const D3DXVECTOR3 vMin2 = vDestLocal + pTarget->m_pModel->m_vMin;
					const D3DXVECTOR3 vMax2 = vDestLocal + pTarget->m_pModel->m_vMax;
					if( ::IsTouchAABB( vMin1, vMax1, vMin2, vMax2 ) )		// AABB�浹�˻�.
					{
						if( pProp->dwComboStyle == CT_FINISH )
							pTarget->m_pActMover->SendDamageForce( nDmgType, GetId(), (nAttackID << 16) );
						else
							pTarget->m_pActMover->SendDamage( nDmgType, GetId(), (nAttackID << 16) ); 
					}
				}
			}
		}
		END_LINKMAP
	}
		
#endif // WORLDSERVER		
} // SendDamageLine()


#ifdef __WORLDSERVER
BOOL CMover::DoUseItemWarp( ItemProp* pItemProp, CItemElem* pItemElem )
{
	switch( pItemProp->dwID )
	{
		case II_GEN_WARP_COUPLERING:
		case II_GEN_WARP_WEDDING_BAND:
		case II_GEN_WARP_COUPLERING01:
			{
				if( IsFly() )
				{
					return FALSE;
				}
//				if( pItemElem->IsFlag( CItemElem::expired ) )
//				{
//					( (CUser*)this )->AddDefinedText( TID_GAME_ITEM_EXPIRED );
//					return FALSE;
//				}
				u_long idCouple	= (u_long)( pItemElem->GetRandomOptItemId() );
				if( idCouple == m_idPlayer )
				{
					( (CUser*)this )->AddDefinedText( TID_GAME_COUPLERING_ERR01, "%s", GetName() );
					return FALSE;
				}

				if( idCouple > 0 )
				{
					// teleport
					const char* lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( idCouple );
					if( lpszPlayer )
					{
						CUser* pUser	= (CUser*)prj.GetUserByID( idCouple );
						if( IsValidObj( (CObj*)pUser ) )
						{
							if( pUser->IsFly() )
							{
								( (CUser*)this )->AddDefinedText( TID_GAME_COUPLERING_ERR03 );
								return FALSE;
							}
							if( pUser->GetWorld() && pUser->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
								return FALSE;
							if( GetWorld() && GetWorld()->GetID() == WI_WORLD_GUILDWAR )
								return FALSE;
							if( GetWorld() && GetWorld()->GetID() >= WI_WORLD_GUILDWAR1TO1_0
								          && GetWorld()->GetID() <= WI_WORLD_GUILDWAR1TO1_L )
								return FALSE;
							if( pUser->GetWorld() && pUser->GetWorld()->GetID() >= WI_WORLD_GUILDWAR1TO1_0
												&& pUser->GetWorld()->GetID() <= WI_WORLD_GUILDWAR1TO1_L )
								return FALSE;
							if( CRainbowRaceMng::GetInstance()->IsEntry( ((CUser*)this)->m_idPlayer )
								|| CRainbowRaceMng::GetInstance()->IsEntry( idCouple ) )
							{
								((CUser*)this)->AddDefinedText( TID_GAME_RAINBOWRACE_NOTELEPORT );
								return FALSE;
							}

							if( GetWorld() == pUser->GetWorld() && !prj.IsGuildQuestRegion( pUser->GetPos() )
								&& GetLayer() == pUser->GetLayer()
#ifdef __QUIZ
								&& GetWorld()->GetID() != WI_WORLD_QUIZ
								&& pUser->GetWorld()->GetID() != WI_WORLD_QUIZ
#endif // __QUIZ
								)
							{
								 g_DPCoreClient.SendTeleportPlayer( m_idPlayer, idCouple );
							}
							else
							{
								( (CUser*)this )->AddDefinedText( TID_GAME_COUPLERING_ERR02, "%s", lpszPlayer );
							}
						}
						else
							( (CUser*)this )->AddDefinedText( TID_DIAG_0061, "%s", lpszPlayer );
					}
				}
				else
				{
					// carve
					UpdateItemEx( (BYTE)( pItemElem->m_dwObjId ), UI_RANDOMOPTITEMID, (__int64)m_idPlayer );
					if( II_GEN_WARP_COUPLERING == pItemProp->dwID )
						UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_KEEPTIME, 21600 );	// 15 days
					( (CUser*)this )->AddDefinedText( TID_GAME_COUPLERING_CARVE, "%s", GetName() );
				}
				return TRUE;
			}
		default:
			break;
	}
	return FALSE;
}
#endif	// __WORLDSERVER

// nIndex skill�� ���� �����̸� �����Ѵ�. 
// nIndex - skill�� index, ( m_tmReUseDelay, m_aJobSkill�� �ε����� ���� �ǹ� )
DWORD CMover::GetReuseDelay( int nIndex )
{
	ASSERT( nIndex >= 0 );	

	DWORD dwCur = ::timeGetTime();
	if( dwCur > m_tmReUseDelay[nIndex] )	// ����ð� ���� ���� �ð��� ũ�� 
		return 0;								// ������ ����.
	else									// �ƴϸ� 
		return m_tmReUseDelay[nIndex] - dwCur;	// ����ð� - ����ð� 
}

#ifdef __WORLDSERVER
void CMover::ActivateSystemPet( CItemElem* pItemElem )
{
#ifdef __QUIZ
	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
		return;
#endif // __QUIZ
	if( IsFly() )
	{
		static_cast<CUser*>( this )->AddDefinedText( TID_GAME_CANNOT_CALL_PET_ON_FLYING );
		return;	
	}
	BOOL bFirst		= FALSE;
	if( pItemElem->m_pPet == NULL )
	{
		pItemElem->m_pPet	= new CPet;
		pItemElem->m_pPet->InitEgg();
		bFirst	= TRUE;
	}
	SetPetId( pItemElem->m_dwObjId );

//#if __VER >= 12 // __PET_0519	// CMover::ProcessPetAvail()�� �̵�
//	SetDestParamRandomOptExtension( pItemElem );	// �ý��� �� Ȱ��ȭ �� ���� ȿ�� ����
//#endif	// __PET_0519
	g_dpDBClient.CalluspPetLog( m_idPlayer, pItemElem->GetSerialNumber(), 0, PETLOGTYPE_CALL, pItemElem->m_pPet );
	// pet, log
#ifdef __PET_1024
	g_UserMng.AddPetCall( this, pItemElem->m_dwObjId, pItemElem->m_pPet->GetIndex(), (BYTE)pItemElem->m_pPet->GetLevel(), pItemElem->m_pPet->GetName() );
#else	// __PET_1024
	g_UserMng.AddPetCall( this, pItemElem->m_dwObjId, pItemElem->m_pPet->GetIndex(), (BYTE)pItemElem->m_pPet->GetLevel() );
#endif	// __PET_1024
	
	if( bFirst )
		static_cast<CUser*>(this )->AddPet( pItemElem->m_pPet, PF_PET );
}

void CMover::InactivateSystemPet( CItemElem* pItemElem )
{
	if( GetPetId() == pItemElem->m_dwObjId )
		PetRelease();
	else
		static_cast<CUser*>(this )->AddDefinedText( TID_GAME_PET_NOWUSE );
}

void CMover::DoUseSystemPet( CItemElem* pItemElem )
{
	if( HasActivatedSystemPet() )
		InactivateSystemPet( pItemElem );
	else
		ActivateSystemPet( pItemElem );
}

void CMover::DoUseEatPet( CItemElem* pItemElem )
{
	if( HasActivatedEatPet() )
		InactivateEatPet();
	else
		ActivateEatPet( pItemElem );
}		

void CMover::ActivateEatPet( CItemElem* pItemElem )
{
	ItemProp* pProp	= pItemElem->GetProp();
	if( !pProp )
		return;

#ifdef __QUIZ
	if( GetWorld() && GetWorld()->GetID() == WI_WORLD_QUIZ )
		return;
#endif // __QUIZ

	if( IsFly() )
	{
		static_cast<CUser*>( this )->AddDefinedText( TID_GAME_CANNOT_CALL_PET_ON_FLYING );
		return;
	}
//		ASSERT( pItemProp->dwLinkKind != NULL_ID )
#ifdef __LAYER_1015
	CMover *pEatPet	= CreateMover( GetWorld(), pProp->dwLinkKind, GetPos(), GetLayer() );
#else	// __LAYER_1015
	CMover *pEatPet	= CreateMover( GetWorld(), pProp->dwLinkKind, GetPos() );
#endif	// __LAYER_1015
	if( pEatPet )
	{
		CAIPet *pAIPet	= static_cast<CAIPet*>( pEatPet->m_pAIInterface );
		if( pAIPet )
		{
			pAIPet->SetOwner( GetId() );
			SetEatPetId( pEatPet->GetId() );
			pAIPet->SetItem( this, pItemElem );		// ���� Ȱ��ȭ �� ���� ȿ�� ����
			if( pItemElem->IsVisPet() )
			{
				if( pItemElem->GetPiercingSize() <= 0 )	// ù ��ȯ�� �����̸� �� ���� 2��(�⺻) �վ���
					UpdateItem( (BYTE)( pItemElem->m_dwObjId ), UI_PETVIS_SIZE, PETVIS_DEFAULT_VIS_SLOT_SZIE );

				SetVisPetItem( pItemElem->m_dwObjId );	// �ش� ������ �����̸� ���� �ɷ�ġ�� �����ϰ� ����Ʈ�� �����Ѵ�.
				SetPetVisDST( pItemElem );
				pEatPet->m_dwMoverSfxId = pItemElem->GetVisPetSfxId();
				static_cast<CUser*>(this)->AddActivateVisPet( pItemElem->m_dwObjId, GetEatPetId() );
			}
		}
	}
}

void CMover::InactivateEatPet( void )
{
	CMover* pEatPet		= prj.GetMover( GetEatPetId() );
	if( IsValidObj( pEatPet ) )
	{
		// ���� ��ȯ ���� �� ���� ȿ�� ����
		CAIPet *pAIPet	= static_cast<CAIPet*>( pEatPet->m_pAIInterface );
		if( pAIPet )
		{
			CItemElem* pItemElem = GetVisPetItem();
			if( pItemElem )
			{
				ResetPetVisDST( pItemElem );
				SetVisPetItem( NULL_ID );
				static_cast<CUser*>(this)->AddActivateVisPet( NULL_ID, NULL_ID );
			}
			pAIPet->ResetItem();
		}

		pEatPet->Delete();
	}
	SetEatPetId( NULL_ID );
}

void CMover::SetPetVisDST( CItemElem* pItemElem )
{
	if( pItemElem && pItemElem->IsVisPet() )
	{
		AddBuff( BUFF_EQUIP, (WORD)( pItemElem->m_dwItemId ), 1, 999999999 );
		
		std::vector<BYTE> vecValidTable = GetValidVisTable( pItemElem );
		for( int i=0; i<pItemElem->GetPiercingSize(); i++ )
		{
			ItemProp* pItemProp = prj.GetItemProp( pItemElem->GetPiercingItem( i ) );
			if( vecValidTable[i] == SUCCSESS_NEEDVIS && pItemProp )
			{
				SetDestParam( 0, pItemProp );
				SetDestParam( 1, pItemProp );
				SetDestParam( 2, pItemProp );
			}
			else
			{
				if( IsPlayer() && pItemProp )
					static_cast<CUser*>( this )->AddDefinedText( TID_GAME_BUFFPET_NOTREQVIS, "\"%s\"", pItemProp->szName );
			}
		}
	}
}

void CMover::ResetPetVisDST( CItemElem* pItemElem )
{
	if( pItemElem && pItemElem->IsVisPet() )
	{
		m_buffs.RemoveBuff( BUFF_EQUIP, (WORD)( pItemElem->m_dwItemId ), FALSE );

		std::vector<BYTE> vecValidTable = GetValidVisTable( pItemElem );
		for( int i=0; i<pItemElem->GetPiercingSize(); i++ )
		{
			ItemProp* pItemProp = prj.GetItemProp( pItemElem->GetPiercingItem( i ) );
			if( vecValidTable[i] == SUCCSESS_NEEDVIS && pItemProp )
			{
				ResetDestParam( 0, pItemProp );
				ResetDestParam( 1, pItemProp );
				ResetDestParam( 2, pItemProp );
			}
		}
	}
}

void CMover::ProcessVisPet()
{
	if( HasActivatedVisPet() )
	{
		CItemElem* pItemElem = GetVisPetItem();
		if( pItemElem )
		{
			for( int i=0; i<pItemElem->GetPiercingSize(); i++ )
			{
				if( pItemElem->GetPiercingItem( i ) != 0 && time_null() >= pItemElem->GetVisKeepTime( i ) )
				{
					ItemProp* pItemProp = prj.GetItemProp( pItemElem->GetPiercingItem( i ) );
					if( pItemProp )
						static_cast<CUser*>( this )->AddDefinedText( TID_GAME_BUFFPET_TIMEOUTVIS, "\"%s\"", pItemProp->szName );
					CItemUpgrade::GetInstance()->RemovePetVisItem( static_cast<CUser*>( this ), i, TRUE );
				}
			}
		}		
	}
}

#endif	// __WORLDSERVER

std::vector<BYTE> CMover::GetValidVisTable( CItemElem* pItemElem )
{
	std::vector<BYTE> vecTemp;
	vecTemp.resize( pItemElem->GetPiercingSize(), UNDEFINED_NEEDVIS );

	for( DWORD i=0; i<vecTemp.size(); i++ )
		if( pItemElem->GetPiercingItem( i ) == 0 )
			vecTemp[i] = FAILED_BOTH_NEEDVIS;
	
	for( DWORD i=0; i<vecTemp.size(); i++ )
		if( vecTemp[i] == UNDEFINED_NEEDVIS )
			SetValidNeedVis( pItemElem, i, vecTemp );

	return vecTemp;
}

BOOL CMover::SetValidNeedVis( CItemElem* pItemElem, int nPos, std::vector<BYTE> & vecValid )
{
	if( vecValid[nPos] != UNDEFINED_NEEDVIS )
		return ( vecValid[nPos] == SUCCSESS_NEEDVIS );
	
	ItemProp* pPropVis = prj.GetItemProp( pItemElem->GetPiercingItem( nPos ) );
	if( !pPropVis )
	{
		vecValid[nPos] = FAILED_BOTH_NEEDVIS;
		return FALSE;
	}

	if( pPropVis->dwReferTarget1 == NULL_ID && pPropVis->dwReferTarget2 == NULL_ID )
	{
		vecValid[nPos] = SUCCSESS_NEEDVIS;
		return TRUE;
	}

	BOOL bResult1 = FALSE, bResult2 = FALSE;
	if( pPropVis->dwReferTarget1 == NULL_ID ) bResult1 = TRUE;
	if( pPropVis->dwReferTarget2 == NULL_ID ) bResult2 = TRUE;

	for( int i=0; i<pItemElem->GetPiercingSize(); i++ )
	{
		if( pPropVis->dwReferTarget1 == pItemElem->GetPiercingItem( i ) )
			bResult1 = SetValidNeedVis( pItemElem, i, vecValid );

		if( pPropVis->dwReferTarget2 == pItemElem->GetPiercingItem( i ) )
			bResult2 = SetValidNeedVis( pItemElem, i, vecValid );

		if( bResult1 && bResult2 )
		{
			vecValid[nPos] = SUCCSESS_NEEDVIS;
			return TRUE;
		}
	}

	if( !bResult1 && bResult2 )
		vecValid[nPos] = FAILED_1ST_NEEDVIS;
	else if( bResult1 && !bResult2 )
		vecValid[nPos] = FAILED_2ND_NEEDVIS;
	else
		vecValid[nPos] = FAILED_BOTH_NEEDVIS;
	return FALSE;
}

BYTE CMover::IsSatisfyNeedVis( CItemElem* pItemElemVisPet, ItemProp* pItemPropVis )
{
	if( pItemPropVis )
	{
		DWORD	dwNeeds[2] = { pItemPropVis->dwReferTarget1, pItemPropVis->dwReferTarget2 };
		if( dwNeeds[0] == NULL_ID && dwNeeds[1] == NULL_ID )
			return SUCCSESS_NEEDVIS;
		
		std::vector<BYTE> vecValidTable = GetValidVisTable( pItemElemVisPet );
		for( int i=0; i<pItemElemVisPet->GetPiercingSize(); i++ )	// �ʿ� �񽺰� �������� ���� ���� �ɷ�ġ�� ������� �ʾ����Ƿ� �ɷ�ġ�� ���� �ȵȴ�.
		{
			DWORD dwVis = pItemElemVisPet->GetPiercingItem( i );
			if( dwNeeds[0] == dwVis && vecValidTable[i] == SUCCSESS_NEEDVIS && time_null() < pItemElemVisPet->GetVisKeepTime( i ) )	dwNeeds[0] = NULL_ID;
			if( dwNeeds[1] == dwVis && vecValidTable[i] == SUCCSESS_NEEDVIS && time_null() < pItemElemVisPet->GetVisKeepTime( i ) )	dwNeeds[1] = NULL_ID;
			if( dwNeeds[0] == NULL_ID && dwNeeds[1] == NULL_ID )
				return SUCCSESS_NEEDVIS;
		}
			
		if( dwNeeds[0] != NULL_ID && dwNeeds[1] == NULL_ID )
			return FAILED_1ST_NEEDVIS;
		else if( dwNeeds[0] == NULL_ID && dwNeeds[1] != NULL_ID )
			return FAILED_2ND_NEEDVIS;
	}

	return FAILED_BOTH_NEEDVIS;
}

#include "stdafx.h"
#include "AIInterface.h"
#include "DefineSkill.h"

#ifndef __CLIENT
#include "User.h"
#endif	// __CLIENT


BEGIN_AISTATE_MAP( CAIInterface, CAIInterface )
END_AISTATE_MAP()

CAIInterface::CAIInterface()
{
	m_pObj = NULL;
	m_pStateFunc = NULL;
	m_uParty	= 0;
}
CAIInterface::CAIInterface( CObj* pObj ) 
{ 
	m_pObj = pObj;
	m_pStateFunc = NULL;
	m_uParty	= 0;
}

CAIInterface::~CAIInterface()
{
}



FUNCAISTATE CAIInterface::GetAIStateFunc( DWORD dwState)
{
	const STATEMAP* lpCurrentStateMap = GetStateMap();
	const STATEMAP_ENTRIES* lpEntries = lpCurrentStateMap->lpStateMapEntries;
	// �Ϻ� ��ü���� ��ġ�ϴ� �Ž����� ã�´�. 
	while( lpCurrentStateMap->lpBaseStateMap->lpStateMapEntries != lpEntries )
	{
		int i = 0;
		while(  lpEntries[ i ].pStateFunc )
		{
			if( lpEntries[ i ].dwAIStateId == dwState )
			{
				return lpEntries[ i ].pStateFunc;
			}
			i++;
		}
		// �߰ߵ��� �ʾҴ�! ���� ��ü�� �����͸� �ٲ۴�.
		lpCurrentStateMap = lpCurrentStateMap->lpBaseStateMap;
		lpEntries = lpCurrentStateMap->lpStateMapEntries;
	}
	return NULL;
}


void CAIInterface::RouteMessage()
{
	AIMSG msg;

	while( !m_MsgQueue.empty() )
	{
		msg	= m_MsgQueue.front();
		m_MsgQueue.pop();
		if( msg.dwMessage == AIMSG_SETSTATE )
		{
			msg.dwMessage	= AIMSG_INIT;
			m_pStateFunc	= GetAIStateFunc( msg.dwParam1 );
		}
		if( m_pStateFunc )
			( this->*( m_pStateFunc ) ) ( msg );
	}
	if( m_pStateFunc )
	{
		msg.dwMessage	= AIMSG_PROCESS;
		msg.dwParam1	= 0;
		msg.dwParam2	= 0;
		( this->*( m_pStateFunc ) ) ( msg );
	}
}

void CAIInterface::SendAIMsg( DWORD dwMessage, DWORD dwParam1, DWORD dwParam2 )
{
	AIMSG msg;
	msg.dwMessage	= dwMessage;
	msg.dwParam1	= dwParam1;
	msg.dwParam2	= dwParam2;
	if( dwMessage == AIMSG_SETSTATE )
	{
		msg.dwMessage	= AIMSG_INIT;
		m_pStateFunc	= GetAIStateFunc( dwParam1 );
	}
	if( m_pStateFunc )
		( this->*( m_pStateFunc ) ) ( msg );
}

void CAIInterface::PostAIMsg( DWORD dwMessage, DWORD dwParam1, DWORD dwParam2 )
{
	switch( dwMessage )
	{
		case AIMSG_ARRIVAL:
			{
				AIMSG msg;
				msg.dwMessage	= dwMessage;
				msg.dwParam1	= dwParam1;
				msg.dwParam2	= dwParam2;
				m_MsgQueue.push( msg );
				break;
			}
		default:	SendAIMsg( dwMessage, dwParam1, dwParam2 );	break;
	}
}


CMover* CAIInterface::ScanTargetNext( CObj* pObjCenter, int nRange, OBJID dwIdTarget, u_long uParty )
{
	// Ÿ���� �������� ��, ���� ������ �����ϴ� �ֺ��� ���� ��Ƽ���� �˻��Ѵ�.
	// ���� Ÿ���� ��Ƽ�� �����ų�, ����� ����
	if( uParty == 0 || nRange <= 0 || GetMover()->GetProp()->dwClass == RANK_GUARD )
		return NULL;

	TRACE( "ScanTargetNext\n" );
	D3DXVECTOR3 vPos	= pObjCenter->GetPos();
	D3DXVECTOR3 vDist;

	BOOL bFlyMob	= GetMover()->IsFlyingNPC();
	float fRadius	= (float)( nRange * nRange );

	for (CObj * pObj : 
		LinkMapRange(pObjCenter->GetWorld(), vPos, nRange, LinkType::Player, GetMover()->GetLayer())
		)	{
		CMover * pTarget	= (CMover *)pObj;
		if( pTarget->IsLive() && pTarget->GetId() != dwIdTarget ) 
		{
			if( bFlyMob == pTarget->IsFly() || bFlyMob == FALSE  )
			{
				vDist	= pTarget->GetPos() - vPos;				
				if( D3DXVec3LengthSq( &vDist ) < fRadius && pTarget->m_idparty == uParty )
				{
					if( pTarget->IsMode( TRANSPARENT_MODE ) == FALSE )
						if( !pTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
							return pTarget;
				}
			}
		}
	}

	return NULL;
}

// pObjCenter�� �߽����� nRangeMeter�ݰ泻�� �����³ѵ��� ������� �ɸ��� �� ���/
// nJobCond : �ش� ������ ������. �����˻� ����.
CMover* CAIInterface::ScanTarget( CObj* pObjCenter, int nRangeMeter, int nJobCond, QuestId dwQuest, DWORD dwItem, int nChao )
{
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;

	bool bCondScan = nJobCond || dwQuest || dwItem || nChao;
	if (!bCondScan) return nullptr;

	const bool bGuard = ( GetMover()->GetProp()->dwClass == RANK_GUARD );
	BOOL bFlyMob = GetMover()->IsFlyingNPC();

	float fRadius = (float)( nRangeMeter * nRangeMeter );	// �Ÿ� 

	for (CUser * pTarget : LinkMapRange(pObjCenter->GetWorld(), vPos, nRangeMeter, LinkType::Player, GetMover()->GetLayer()))
	{
		if (!pTarget->IsLive()) continue;

		// ���� �����̶�� ������ ��ġ�ϴ� �� �����ϴ�.
		// 
		// 
		// 1. ���� ���� üũ    
		if (nJobCond != 0 && nJobCond != JOB_ALL && pTarget->GetJob() != nJobCond)		// JOB_ALL : �������
			continue;

		// 2. ������ ���� üũ  
		if (dwItem != 0 && pTarget->GetItemNum(dwItem) == 0)
			continue;

		// 3. ����Ʈ ���� üũ 
		if (dwQuest && pTarget->GetQuest(dwQuest) == nullptr)
			continue;

		//4. ī�� ���� üũ  
		if( nChao != 0 ) {
			// 100 = ī������, 101 = ��ī��
			if( nChao == 100 && pTarget->IsChaotic()) {
				// ok
			} else if( nChao == 101 && !pTarget->IsChaotic()) {
				// ok
			} else {
				continue;
			}
		}
		
		// ����� ������ ����			
		// ������� �����÷��̾ ����, ������� �����÷��̾�� ���� - 1. true true  2. false false

		if (pTarget->IsMode(TRANSPARENT_MODE)) continue;
		if (pTarget->HasBuffByIk3(IK3_TEXT_DISGUISE)) continue;

		if( bGuard || bFlyMob == pTarget->IsFly() )		
		{
			const D3DXVECTOR3 vDist = pTarget->GetPos() - vPos;
			if( D3DXVec3LengthSq( &vDist ) < fRadius )	// �� ��ü���� �Ÿ��� ���� �̳��̸� 
			{
				return pTarget;
			}
		}
		
	}

	return nullptr;
}

// �ݰ泻���� ���� ���ѳ�(������)�� ���.
CMover* CAIInterface::ScanTargetStrong( CObj* pObjCenter, FLOAT fRangeMeter  )
{
	const D3DXVECTOR3 vPos = pObjCenter->GetPos();
	
	// ���� 
	const FLOAT fRadius = fRangeMeter * fRangeMeter;
	

	CMover * pObjMax = NULL;	// ������ ������.
	for( CMover * pObj : LinkMapRange(pObjCenter->GetWorld(), vPos, (int)( fRangeMeter / MPU ), LinkType::Player, GetMover()->GetLayer()) )
	{
		const D3DXVECTOR3 vDist = pObj->GetPos() - vPos;	// ����ǥ�� ����
		const float fDistSq = D3DXVec3LengthSq( &vDist );		// �ο�����Ʈ���� �Ÿ�Sq
		if( fDistSq < fRadius )	
		{
			if (!pObj->IsMode(TRANSPARENT_MODE)) {
				if (!pObjMax || pObj->GetLevel() > pObjMax->GetLevel()) {
					pObjMax = pObj;
				}

			}
		}
	}

	return pObjMax;
}

// �ݰ泻���� ������ �ϴ� ��� ã�Ƽ� ������.
CMover* CAIInterface::ScanTargetOverHealer( CObj* pObjCenter, FLOAT fRangeMeter  )
{
#ifndef __CLIENT
	const D3DXVECTOR3 vPos = pObjCenter->GetPos();
	
	// ���� 
	const FLOAT fRadius = fRangeMeter * fRangeMeter;
	
	for (CUser * pMover : LinkMapRange(pObjCenter->GetWorld(), vPos, (int)( fRangeMeter / MPU ), LinkType::Player, GetMover()->GetLayer() ))
	{
		const D3DXVECTOR3 vDist = pMover->GetPos() - vPos;	// ����ǥ�� ����
		float fDistSq = D3DXVec3LengthSq( &vDist );		// �ο�����Ʈ���� �Ÿ�Sq
		if( fDistSq < fRadius )	
		{
			if(  ! pMover->IsMode( TRANSPARENT_MODE ) )
			{

					if( pMover->m_nOverHeal > 0 )		// ���������� ã�Ҵ�.
						return pMover;
			}
		}
	}

#endif	// __CLIENT
	return NULL;
}



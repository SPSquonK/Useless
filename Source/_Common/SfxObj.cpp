#include "stdafx.h"
#include "Sfx.h"

#include "dpclient.h"
#include "defineObj.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSfx::CSfx()
{
	m_dwType = OT_SFX;
	m_idSrc = NULL_ID;
	m_idDest = NULL_ID;
	m_vPosDest = D3DXVECTOR3( 0, 0, 0 );
	m_nFrame = 0;
	m_nSec = -1;
	m_idSfxHit	= 0;
	m_nMagicPower = 0;
	m_dwSkill = NULL_ID;
	m_pSfxObj = NULL;
}



CSfx::~CSfx()
{
	if( m_idSfxHit )
	{
		if( m_idSrc == NULL_ID )	// 이런상황이 생겨선 안된다.
		{
			Error( "CSfx::~CSfx : %d %d %d", m_dwIndex, m_idSfxHit, m_idSrc );
		} 
		else
			g_DPlay.SendSfxClear( m_idSfxHit, m_idSrc );	// m_idSfxHit가 있는 거였으면 서버에 요놈 지우라고 보내자 
	}
}

BOOL CSfx::SetIndex( DWORD dwIndex, BOOL bInitProp )
{
	BOOL bResult = FALSE;
	m_idSrc = NULL_ID;
	m_idDest = NULL_ID;
	m_vPosDest = D3DXVECTOR3( 0, 0, 0);

	if( dwIndex >= 0 ) 
	{
		bResult = SetTypeIndex( OT_SFX, dwIndex, bInitProp );
		m_pSfxObj = (CSfxModel*)m_pModel;
	}
	UpdateLocalMatrix();
	return bResult;
}

int	CSfx::SetSfx( int nIndex, 
				   D3DXVECTOR3 vPosSrc, OBJID idSrc, const D3DXVECTOR3 vPosDest, OBJID idDest, int nSec )
{
	m_idSrc = idSrc;
	m_idDest = idDest;
	m_vPosDest = vPosDest;
	m_nSec = nSec;

	vPosSrc.y += 0.01f; // 지면에서 약간 띄워주자. 안그러면 밑에 뭍혀서 지글거림.
	SetPos( vPosSrc );
	
	if( nIndex >= 0 ) 
	{
		SetTypeIndex( OT_SFX, nIndex,TRUE);
		m_pSfxObj = (CSfxModel*)m_pModel;
		return 1;
	}
	UpdateLocalMatrix();

	if( m_pSfxObj )
		return 1;

	return 0;
}

//
// nDmgCnt : 일반적으론 0 : 지속데미지를 사용할경우에 0이 아닌값이 들어온다.
//
void CSfx::DamageToTarget( int nMaxDmgCnt )
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	CCtrl* pObjDest = prj.GetCtrl( m_idDest );
	
	if( IsInvalidObj(pObjSrc) )		return;		// 지금은 걍 리턴하지만 이렇게 실패한경우는 m_idSfxHit을 Clear해주는작업이 필요하다.
	if(	IsInvalidObj(pObjDest) )	return;

	if( CMover * pMover = pObjDest->ToMover() )
	{
		const auto pos = pMover->GetPos();
		PLAYSND( pMover->GetProp()->dwSndDmg2, &pos );	// 마법류 맞을때 타격음.	

		// 쏜놈이 플레이어이거나 / 쏜놈은 플레이어가 아닌데 맞은놈이 플레이어일경우 전송
		if( pObjSrc->IsActiveMover() || (pObjSrc->IsPlayer() == FALSE && pObjDest->IsActiveMover()) )
		{
			pMover->SetDmgCnt( 10 );	// 발사체 맞아도 이제 흔들린다,
			g_DPlay.SendSfxHit( m_idSfxHit, m_dwSkill, pObjSrc->GetId() );
			if( nMaxDmgCnt == 1 )	// 한방짜리 데미지만 id를 클리어 함.
				m_idSfxHit = 0;		// 0으로 해놔야 this가 삭제될때 SendSfxClear를 또 보내지 않는다.
		}
	}
}
void CSfx::Process()
{
	m_nFrame++;
	if( m_nSec == 0 )	// 0은 1회 플레이후 종료.
	{
		if( m_pSfxObj->Process() )		// return true는 애니메이션 끝.
			Delete();
	}
	else
	{
		// 반복 애니메이션
		if( m_pSfxObj->Process() )		
			m_pSfxObj->m_nCurFrame = 0;
		if( m_nSec != -1 )		// 무한반복(-1)이 아니면
		{
			if( m_nFrame > ( m_nSec * 60 ) )	// 시간 체크를 한다.
				Delete();
		}
	}

	if(m_pSfxObj->m_pSfxBase)
	{
		for (auto & pSfxPart : m_pSfxObj->m_pSfxBase->m_aParts) {
			if(pSfxPart->m_nType != SFXPARTTYPE_MESH )
				continue;
			
			if (CModelObject * pMesh = g_SfxMeshMng.Mesh(pSfxPart->m_strTex)) {
				pMesh->FrameMove();
			}
		}
	}

	if( m_idDest != NULL_ID )		// Dest가 지정되어 있을때.
	{
		CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
		if( IsValidObj( pObjDest ) )		// 유효한넘인가?
			m_vPosDest = pObjDest->GetPos();	// 당시 좌표를 계속 받아둠.  Invalid상태가 되면 마지막 좌표로 세팅된다.
		
		SetPos( m_vPosDest );	// 타겟에 오브젝트 발동.

		if( m_dwIndex == XI_SKILL_PSY_HERO_STONE02 )
		{
			if( IsValidObj( pObjDest ) )		// 유효한넘인가?
			{
				DWORD dwTmpID =  pObjDest->GetRemoveSfxObj(XI_SKILL_PSY_HERO_STONE02 );
				if( dwTmpID && m_nFrame > 0)
				{
					Delete();
					return;
				}
			}
		}
		// 091022 mirchang - 프로텍션, 펜바리어 스킬 버프 해제 시 sfx 삭제
		if( m_dwIndex == XI_SKILL_MER_SHIELD_PANBARRIER02 )
		{
			if( IsValidObj( pObjDest ) )		// 유효한넘인가?
			{
				DWORD dwTmpID =  pObjDest->GetRemoveSfxObj( XI_SKILL_MER_SHIELD_PANBARRIER02 );
				if( dwTmpID && m_nFrame > 0)
				{
					Delete();
					return;
				}
			}
		}
		if( m_dwIndex == XI_SKILL_MER_SHIELD_PROTECTION02 )
		{
			if( IsValidObj( pObjDest ) )		// 유효한넘인가?
			{
				DWORD dwTmpID =  pObjDest->GetRemoveSfxObj( XI_SKILL_MER_SHIELD_PROTECTION02 );
				if( dwTmpID && m_nFrame > 0)
				{
					Delete();
					return;
				}
			}
		}
	} else
	{
		// Dest가 지정되어 있지 않을때. Src로...
		if( m_idSrc != NULL_ID )
		{
			CCtrl * pObjSrc = prj.GetCtrl( m_idSrc );
			if( IsValidObj( pObjSrc ) )			// 소스아이디가 지정되어 있으면
				SetPos( pObjSrc->GetPos() );	// 소스측에 이펙 발동.
			else
				Delete();
		}
	}
}

// y축으로만 회전도는 버전.
void CSfx::Render( )
{
	if( !IsVisible() || ( IsCull() && GetType() != 1 ))
		return;
	if( IsUpdateMatrix() )
		UpdateMatrix();
	
	MODELELEM * lpModelElem = prj.m_modelMng.GetModelElem( m_dwType, m_dwIndex );
	// Model이 없으면 출력하지 않음 
	if( lpModelElem == NULL )
		return;

	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vPos.y += 0.2f;		// 0.2씩이나 올릴필요가 있남?
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( NULL );
	
}


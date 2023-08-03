#include "stdafx.h"

#include "rangda.h"
#include "worldmng.h"
#include "lord.h"

CRangda::CRangda( DWORD dwMonster )
:
m_dwMonster( dwMonster ),
m_nInterval( 0 ),
m_nReplace( 0 ),
m_bActiveAttack( FALSE ),
m_nGenerateCountdown( 0 ),
m_nReplaceCountdown( 0 ),
m_bReplaceable( TRUE ),
m_nOldPos( -1 ),
m_objid( NULL_ID )
{
}

void CRangda::AddPos( const RANGDA_POS & vPos )
{	// ��ǥ �߰�
	m_vvPos.push_back( vPos );
}

CRangda::RANGDA_POS CRangda::GetRandomPos( void )
{	// �߰��� ��ǥ �� �ϳ��� ���Ƿ� ��ȯ
	ASSERT( !m_vvPos.empty() );
	
	// �߰��� ��ǥ�� �ϳ���� �Ʒ��� ��Ģ��
	// ���� �� �����Ƿ� �״�� ��ȯ
	if( m_vvPos.size() == 1 )
		return m_vvPos[0];
	
	int nPos;
	do	{	// ��� ��ǥ�� ���� ��ǥ�� �޶�� �Ѵ�
		nPos	= xRandom( 0, m_vvPos.size() );
	}	while( nPos == m_nOldPos );

	m_nOldPos	= nPos;
	return m_vvPos[nPos];
}

void CRangda::SetObj( OBJID objid )
{	// ���� ������ �� ��ü�� ����
	m_objid		= objid;
	m_nGenerateCountdown	= m_nInterval;
	m_nReplaceCountdown		= m_nReplace;
	m_bReplaceable	= TRUE;
}

void CRangda::OnTimer( void )
{
	CMover* pMonster	= GetMonster();
	if( IsValidObj( pMonster ) )	// ��ü�� �����ϸ�
		ProcessReplace( pMonster );		// ������ �����ϸ� ������ ��ǥ�� �̵���Ų��
	else	// ��ü�� �������� ������ ���
		ProcessGenerate();
}

CMover* CRangda::GetMonster( void )
{	// �� ��ü�� �����͸� ��ȯ
	if( m_objid != NULL_ID )
	{
		CMover* pMonster	= prj.GetMover( m_objid );
		if( IsValidObj( pMonster ) )
			return pMonster;
		m_objid		= NULL_ID;
	}
	return NULL;
}

void CRangda::ProcessReplace( CMover* pMonster )
{	// ������ ��ǥ�� �̵�
	if( m_bReplaceable )
	{
		if( IsDamaged( pMonster ) )		// ��ó �Ծ�����
			m_bReplaceable	= FALSE;	// �̵���Ű�� �ʴ´�
		else if( HavetoReplace() )	// �̵��� ������ �����ϸ�
		{
			election::OutputDebugString( "CRangda.ProcessReplace:" );
			pMonster->Delete();		// �����ϰ�
			CreateMonster();	// ���� �����Ѵ�
		}
	}
}

void CRangda::ProcessGenerate( void )
{
	if( HavetoGenerate() )	// ��� ������ �����ϸ�
	{
		election::OutputDebugString( "CRangda.ProcessGenerate:" );
		CreateMonster();	// ���� �����Ѵ�
	}
}

BOOL CRangda::IsDamaged( CMover* pMonster )
{	// ��ó �Ծ���?
	return ( pMonster->GetHitPoint() < pMonster->GetMaxHitPoint() );
}

BOOL CRangda::HavetoReplace( void )
{	// ��ǥ�� �ٲ� �ð��� �Ǿ���?
	if( m_nReplaceCountdown > 0 )
		m_nReplaceCountdown--;
	return ( m_nReplaceCountdown == 0 );
}

BOOL CRangda::HavetoGenerate( void )
{	// ��� �ð��� �Ǿ���?
	if( m_nGenerateCountdown > 0 )
		m_nGenerateCountdown--;
	return ( m_nGenerateCountdown == 0 );
}

void CRangda::CreateMonster( void )
{	// ���� ����
	RANGDA_POS pos	= GetRandomPos();
	CWorld* pWorld	= g_WorldMng.GetWorld( pos.dwWorldId );
	if( !pWorld )
		return;
	CMover* pMonster	= static_cast<CMover*>( ::CreateObj( OT_MOVER, m_dwMonster ) );
	if( pMonster )
	{
		pMonster->m_bActiveAttack	= m_bActiveAttack;
		pMonster->SetPos( pos.vPos );
		pMonster->InitMotion( MTI_STAND );
		pMonster->UpdateLocalMatrix();
		pMonster->AddItToGlobalId();
		pWorld->ADDOBJ( pMonster, FALSE, nDefaultLayer );
		SetObj( pMonster->GetId() );
		election::OutputDebugString( "CRangda.CreateMonster: monster = %d, x = %0.2f, y = %0.2f, z = %0.2f\n",
			pMonster->GetIndex(), pMonster->GetPos().x, pMonster->GetPos().y, pMonster->GetPos().z );
	}
}

////////////////////////////////////////////////////////////////////////////////
// CRangdaController

CRangdaController * CRangdaController::Instance() {
	static CRangdaController sRangdaController;
	return &sRangdaController;
}

void CRangdaController::OnTimer() {
	for (CRangda & pRangda : m_vRangda) {
		pRangda.OnTimer();
	}
}

bool CRangdaController::LoadScript(const char * szFile) {
	CScript s;
	if (!s.Load(szFile)) return false;


	while (true) {
		const DWORD dwMonster = s.GetNumber();		// subject or FINISHED

		if (s.tok == FINISHED) break;

		CRangda & pRangda = m_vRangda.emplace_back(dwMonster);

		s.GetToken();	// {
		while (true) {
			s.GetToken();	// subject or '}'
			if (*s.token == '}') break;

			if (s.Token == _T("nInterval"))
				pRangda.SetInterval(s.GetNumber());
			else if (s.Token == _T("nReplace"))
				pRangda.SetReplace(s.GetNumber());
			else if (s.Token == _T("bActiveAttack"))
				pRangda.SetActiveAttack(static_cast<BOOL>(s.GetNumber()));
			else if (s.Token == _T("vRangda")) {
				s.GetToken();	// {

				while (true) {
					CRangda::RANGDA_POS pos;
					pos.dwWorldId = s.GetNumber();

					if (*s.token == '}') break;

					pos.vPos.x = s.GetFloat();
					pos.vPos.y = s.GetFloat();
					pos.vPos.z = s.GetFloat();

					pRangda.AddPos(pos);
				}
			}
		}
	}

	return true;
}


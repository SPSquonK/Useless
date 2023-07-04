#include "stdafx.h"
#include "Ship.h"

#ifdef __WORLDSERVER
#include "User.h"
#endif

static constexpr float FRIC_AIR = 0.011f;		// ������ �������

CShip::CShip()
{
	m_dwType	= OT_SHIP;
	m_Act.SetShip( this );
	Init();
}

CShip::~CShip()
{
	Destroy();
}

void CShip::Init( void )
{
	m_pMover = NULL;
	m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;
	m_vDeltaUnit.x = m_vDeltaUnit.y = m_vDeltaUnit.z = 0;
	m_vAcc.x = m_vAcc.y = m_vAcc.z = 0;
	m_fAccAng = 0;
	m_nCount = 0;
	m_fDeltaAng = 0;
	m_LinkCtrl.clear();	
	m_LinkCtrl.reserve(8);
}

void CShip::Destroy( void )
{
	Init();
}

void CShip::AddCtrl(OBJID idCtrl) {
	const auto it = std::find(m_LinkCtrl.begin(), m_LinkCtrl.end(), idCtrl);
	if (it != m_LinkCtrl.end()) return;
	m_LinkCtrl.emplace_back(idCtrl);
}

// idCtrl�� ã�Ƽ� ����.
void CShip::RemoveCtrl(OBJID idCtrl) {
	const auto it = std::find(m_LinkCtrl.begin(), m_LinkCtrl.end(), idCtrl);
	if (it != m_LinkCtrl.end()) {
		m_LinkCtrl.erase(it);
	}
}


#ifdef __CLIENT
void CShip::Control( void )
{
	static BOOL	s_bAcced = 0;
	static BOOL	s_bLefted = 0;
	static BOOL	s_bRighted = 0;
	static BOOL s_bBoarded = 0;
	
	const BOOL bAcc = g_bKeyTable[ VK_SPACE ];
	const BOOL bLeft = g_bKeyTable[g_Neuz.Key.chLeft];
	const BOOL bDown = g_bKeyTable[ g_Neuz.Key.chUp ];
	const BOOL bRight = g_bKeyTable[ 'D' ];
	const BOOL bUp = g_bKeyTable[ 'S' ];
	const BOOL bBoard = g_bKeyTable[ 'B' ];
	
	if( bAcc && !s_bAcced )		// ����/���� ���.
	{
		if( m_Act.GetMoveState() == OBJSTA_FMOVE )		// �������̶��
			SendActMsg( OBJMSG_STOP );		// ���߰�
		else
			SendActMsg( OBJMSG_FORWARD );	// �ƴ϶�� ��������.
	}
	s_bAcced = bAcc;

	if( bLeft ) {
		SendActMsg( OBJMSG_LTURN );		// �������� ��
	} else if( bRight ) {
		SendActMsg( OBJMSG_RTURN );		// ���������� ��.
	} else {
		SendActMsg( OBJMSG_STOP_TURN );	// ��/��Ű�� �ȴ������ִٸ� ȸ�� ����.
	}

	if( bUp ) {
		SendActMsg( OBJMSG_LOOKUP );
	} else if( bDown ) {
		SendActMsg( OBJMSG_LOOKDOWN );
	} else {
		SendActMsg( OBJMSG_STOP_LOOK );	// ��/�Ʒ� Ű�� �ȴ������ִٸ� ���/�ϰ� ����.
	}

	if( bBoard && !s_bBoarded )			// �� ������ �����ϱ�.
	{
		g_pShip = NULL;
		SetMover( NULL );
	}

	s_bBoarded = bBoard;
}
#endif // CLIENT

void CShip::Process()
{
	D3DXMatrixInverse( GetInvTM(), NULL, &m_matWorld );		// ��ǥ��ȯ�� �̷������� ��Ʈ������ ������� ���س��� �Ѵ�.
	
	D3DXVECTOR3	vPos = GetPos();
	D3DXVECTOR3 vDeltaAccu = D3DXVECTOR3(0, 0, 0);		// ���� ���ʹ� �׻� �ʱ�ȭ ����� �Ѵ�.

#ifdef __X15
	int		i;
	for( i = 0; i < 4; i ++ )
#endif	
	{
		m_Act.Process();			// ������Ʈ�� ��������� ���¸� ����.
		
		m_vDeltaUnit += m_vAcc;			// ���� �ӵ��� ���ο� ���� ����.
		m_vDeltaUnit *= (1.0f - FRIC_AIR);
		vDeltaAccu += m_vDeltaUnit;		// ���� 15�����ӿ����� ���Ǵ� ������ 4���� ������.
	}
#ifdef __CLIENT
	const FLOAT fLenSq = D3DXVec3Length( &m_vDelta );		// 1/60 sec �ӵ�
	extern int g_nFlySpeed;
	if( m_pMover->IsActiveMover() )
	{
		g_nFlySpeed = (int)( (fLenSq * 60.0f) * 60.0f * 60.0f );
		g_nFlySpeed	= (int)( g_nFlySpeed / 200.0f );
	}
	
#endif	

	m_vDelta = vDeltaAccu;		// �̹��������� this �̵����̵ȴ�.  �׻� m_vDelta�� ���� �̵����� ���� ����.
	vPos += m_vDelta;
	SetPos( vPos );
	m_fDeltaAng = m_fAccAng;
	
	// IA������Ʈ�� �ٸ� ������Ʈ�� �¿�� �ٳ�� �ϹǷ�
	// �ǽð����� ��Ʈ������ ���ŵǾ�� �Ѵ�.
	UpdateLocalMatrix();		
}

//////////////////////////////////////////////////////////////////////////
CShipLoop::CShipLoop()
{
	Init();
}

CShipLoop::~CShipLoop()
{
	Destroy();
}

void CShipLoop::Init( void )
{
}

void CShipLoop::Destroy( void )
{
	Init();
}

//
//
//
void CShipLoop::Process()
{
	D3DXMatrixInverse( GetInvTM(), NULL, &m_matWorld );		// ��ǥ��ȯ�� �̷������� ��Ʈ������ ������� ���س��� �Ѵ�.
	
	D3DXVECTOR3	vPos = GetPos();
	D3DXVECTOR3 vDeltaAccu;
	FLOAT fAng = GetAngle();
	
	vDeltaAccu = D3DXVECTOR3(0, 0, 0);		// ���� ���ʹ� �׻� �ʱ�ȭ ����� �Ѵ�.
	m_fDeltaAng = 0.07f;
#ifdef __X15
	int		i;
	for( i = 0; i < 4; i ++ )
#endif	
	{
		fAng += m_fDeltaAng;		// �������� ��� ȸ��
		m_vAcc = AngleToVectorXZ( fAng, 0.05f );		// fAng�������� �����¹߻�.
		vDeltaAccu += m_vAcc;		// ���� 15�����ӿ����� ���Ǵ� ������ 4���� ������.
	}
	
	m_vDelta = vDeltaAccu;
	vPos += m_vDelta;
	SetPos( vPos );
	SetAngle( fAng );
	
#ifdef __WORLDSERVER
	if( (m_nCount & 127) == 0 )
	{
		g_UserMng.AddSetPosAngle( this, GetPos(), GetAngle() );		// ���� this(Ship)�� ��ġ�� sync��Ŵ.
		// ��ũ�Ǿ� �ִ� ��� ctrl�� ��ġ�� �ٽ� sync��Ŵ.
		
		auto itCtrl = m_LinkCtrl.begin();
		while (itCtrl != m_LinkCtrl.end()) {
			CCtrl *pCtrl = prj.GetCtrl( *itCtrl );
			if (!IsValidObj(pCtrl)) {
				itCtrl = m_LinkCtrl.erase(itCtrl);
			} else if (pCtrl->GetIAObjLink() == this) {
				g_UserMng.AddSetPosAngle(pCtrl, pCtrl->GetPos(), pCtrl->GetAngle());		// this(Ship)�� ��ġ�� sync��Ŵ.
				++itCtrl;
			} else {
				itCtrl = m_LinkCtrl.erase(itCtrl);
			}
		}
	}
#endif
	
	// IA������Ʈ�� �ٸ� ������Ʈ�� �¿�� �ٳ�� �ϹǷ�
	// �ǽð����� ��Ʈ������ ���ŵǾ�� �Ѵ�.
	UpdateLocalMatrix();		

	m_nCount ++;
}

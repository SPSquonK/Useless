#include "stdafx.h"
#include "defineObj.h"
#include "CreateObj.h"

#include "randomoption.h"
#ifdef __WORLDSERVER
#include "ticket.h"
#endif	// __WORLDSERVER

#include "serialnumber.h"
#include "definetext.h"

bool IsUsableItem(const CItemElem * const pItem) {
	return pItem && !pItem->IsEmpty() && pItem->GetExtra() == 0;
}

bool IsUsingItem(const CItemElem * const pItem) {
	return pItem && !pItem->IsEmpty() && pItem->GetExtra() != 0;
}

//////////////////////////////////////////////////////////////////////
// CItemBase
//////////////////////////////////////////////////////////////////////
void CItemElem::SetTexture()
{
#ifdef __CLIENT
	const ItemProp * const pProp =GetProp();
	if (!pProp) {
		Error("CItemBase::SetTexture GetProp() NULL Return %d", m_dwItemId);
		return;
	}

	CString strIcon	= pProp->szIcon;
	if( pProp->dwItemKind3 == IK3_EGG )
	{
		const BYTE nLevel = m_pPet ? m_pPet->GetLevel() : 0;

		switch( nLevel )
		{
			case PL_D:
			case PL_C:
				strIcon.Replace( ".", "_00." );
				break;
			case PL_B:
			case PL_A:
				strIcon.Replace( ".", "_01." );
				break;
			case PL_S:
				strIcon.Replace( ".", "_02." );
				break;
		}
	}
	m_pTexture = CWndBase::m_textureMng.AddTexture( MakePath( DIR_ITEM, strIcon ), 0xffff00ff );
#endif
}

//
// ������ ������ ����ؼ� �����Ѵ�.
// -1�� �����ϸ� ó���ؼ� �ȵȴ�.
//
int	CItemElem::GetCost() const {
	const ItemProp * const pProp = GetProp();
	if (!pProp) return -1;
	if (pProp->dwCost == NULL_ID) return -1;
		
	if( GetAbilityOption() != 0 )
	{		
		//INT((�����۰���+�����۰���*(0.1+(�����۰�ȭ����*�����۰�ȭ����)/18))
		return static_cast<int>( pProp->dwCost + pProp->dwCost * ( 0.1f + ( GetAbilityOption() * GetAbilityOption() ) / 18.0f ) );
	}
	else
	{
		return static_cast<int>(pProp->dwCost);
	}
}

DWORD CItemElem::GetChipCost() const {
	const ItemProp * const pProp = GetProp();
	if (!pProp) return -1;
	if (pProp->dwReferValue1 == NULL_ID) return -1;

	return pProp->dwReferValue1;
}

// ����Ʈ �������ΰ�?
bool CItemElem::IsQuest() const {
	const ItemProp * p = GetProp();
	if (p->dwItemKind3 != IK3_QUEST) return false;

	if (::GetLanguage() == LANG_JAP && p->dwID == II_SYS_SYS_QUE_REDSOCKS) {
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// CItemElem - ������ �ϳ��ϳ��� ��Ҹ� ���Ѵ�.
//////////////////////////////////////////////////////////////////////
/*
	1 ~ 99    
	100 ~ ���� ���� �ΰ�

	�����ۿ� �ΰ� ��ȣ.

*/

CItemElem::CItemElem()
{
	m_szItemText[0] = '\0';
	m_dwItemId = 0;
	m_dwObjId = NULL_ID;
	m_dwObjIndex = NULL_ID;
	m_nExtra = 0;
	m_nCost = 0;
	m_liSerialNumber = 0;

#ifdef __CLIENT
	m_pTexture = nullptr;
#endif

	m_idGuild = 0;
	m_nItemNum = 1;
	m_nAbilityOption = 0;
	m_nRepairNumber	= 0;
	m_nHitPoint		= -1;
	m_nRepair	= 0;
	m_byFlag	= 0;
#ifdef __CLIENT
	m_bRepair	= FALSE;
#endif	// __CLIENT

    m_bItemResist = SAI79::NO_PROP;
	m_nResistAbilityOption = 0;	
	m_nResistSMItemId = 0;

#ifdef __WORLDSERVER
	m_bQuery	= FALSE;
#endif	// __WORLDSERVER

//	memset( &m_piercingInfo, 0, sizeof(m_piercingInfo) );
	m_dwKeepTime	= 0;
	m_iRandomOptItemId	= 0;


	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	m_n64NewRandomOption	= 0;
	//	mulcom	END100405	���� ��ȣ�� �η縶��


/*
#ifdef __XPET2
	m_nMaxLevel = 0;	
	m_nLevel = 0;		
	m_dwHungry = 0;		
	m_dwFeeling = 0;	
#endif // xPet2
*/

	m_pPet	= NULL;
	m_bTranformVisPet = FALSE;

}

CItemElem::~CItemElem()
{
	SAFE_DELETE( m_pPet );
}

void	CItemElem::Empty( void )
{
	m_szItemText[0] = '\0';
	m_dwItemId = 0;
	m_nExtra = 0;

#ifdef __CLIENT
	m_pTexture = nullptr;
#endif

	SAFE_DELETE( m_pPet );
	m_piercing.Clear();
}

// ���� ���� ��´�. 
int	CItemElem::GetGold()
{
	ASSERT( GetProp()->dwItemKind1 == IK1_GOLD );
	return m_nHitPoint;		// int������ ���� ������ �ϱ� ���ؼ� �� ������ ��� 
}

void CItemElem::UseItem()
{
	if( GetProp()->bPermanence != TRUE )
		m_nItemNum--;
}

CItemElem& CItemElem::operator =( CItemElem & ie )
{
	_tcscpy(m_szItemText, ie.m_szItemText);
	m_dwItemId = ie.m_dwItemId;
	m_liSerialNumber = ie.m_liSerialNumber;

#ifdef __CLIENT
	m_pTexture = ie.m_pTexture;
#endif

	SetTexture( ie.GetTexture() );

	m_nItemNum             = ie.m_nItemNum;
	m_nRepairNumber        = ie.m_nRepairNumber;
	m_nHitPoint            = ie.m_nHitPoint;
	m_nRepair				= ie.m_nRepair;
	m_byFlag               = ie.m_byFlag;
	m_nAbilityOption       = ie.m_nAbilityOption;
	m_idGuild              = ie.m_idGuild;
	m_bItemResist          = ie.m_bItemResist;
	m_nResistAbilityOption = ie.m_nResistAbilityOption;
	m_nResistSMItemId      = ie.m_nResistSMItemId;
	m_dwKeepTime           = ie.m_dwKeepTime;
	m_piercing	= ie.m_piercing;
	m_iRandomOptItemId     = ie.GetRandomOptItemId();

	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	m_n64NewRandomOption	= ie.GetNewRandomOption();
	//	mulcom	END100405	���� ��ȣ�� �η縶��

	SAFE_DELETE( m_pPet );
	if( ie.m_pPet )
	{
		m_pPet	= new CPet;
		*m_pPet	= *ie.m_pPet;
	}
	m_bTranformVisPet		= ie.m_bTranformVisPet;

	return *this;
}

SmallDstList CItemElem::GetPiercingAvail() const {
	SmallDstList result;

	for (int i = 0; i < GetPiercingSize(); i++) {
		const PIERCINGAVAIL * ptr	= g_PiercingAvail.GetPiercingAvail( GetPiercingItem( i ) );
		if (ptr) {
			for (const auto & dstToAdd : ptr->params) {
				const auto it = std::find_if(result.begin(), result.end(),
					[&](const SINGLE_DST & dst) { return dst.nDst == dstToAdd.nDst; }
				);

				if (it == result.end()) {
					result.emplace_back(dstToAdd);
				} else {
					it->nAdj += dstToAdd.nAdj;
				}
			}
		}
	}

	return result;
}

// �ֻ����� ���� ������ ������ ����
BOOL CItemElem::IsDiceRefineryAble( const ItemProp* pProp )
{
	if( !pProp )
		return FALSE;

	if( pProp->dwItemKind2 == IK2_ARMOR ||
		pProp->dwItemKind2 == IK2_WEAPON_MAGIC ||
		pProp->dwItemKind2 == IK2_WEAPON_DIRECT ||
		pProp->dwItemKind2 == IK2_ARMORETC )
		return TRUE;
					
	return FALSE;
}

// ī��� ���� ������ ������ ����
BOOL CItemElem::IsEleRefineryAble( const ItemProp* pProp )
{
	if( !pProp )
		return FALSE;

	if( pProp->dwItemKind3 == IK3_SUIT ||
		pProp->dwItemKind2 == IK2_WEAPON_MAGIC ||
		pProp->dwItemKind2 == IK2_WEAPON_DIRECT )
		return TRUE;
	
	return FALSE;
}

BOOL CItemElem::IsBinds( void )
{
	ItemProp* pProperty = GetProp();
	if( m_dwKeepTime && pProperty->dwItemKind2 != IK2_WARP )
		return TRUE;

	if( (pProperty->dwFlag[IP_FLAG::BINDS]))
		return TRUE;

	if( IsFlag( CItemElem::binds ) )
		return TRUE;

	if( g_xRandomOptionProperty.GetRandomOptionSize( GetRandomOptItemId() ) > 0
		&& ( g_xRandomOptionProperty.GetRandomOptionKind( this ) == CRandomOptionProperty::eBlessing || g_xRandomOptionProperty.GetRandomOptionKind( this ) == CRandomOptionProperty::eEatPet ) )
		return TRUE;
	if( GetLevelDown() < 0 )
		return TRUE;

	return FALSE;
}

bool CItemElem::IsUndestructable() const {
	const ItemProp * pProperty = GetProp();
	return pProperty && pProperty->dwFlag[IP_FLAG::UNDESTRUCTABLE];
}

BOOL CItemElem::IsLogable( void )
{
	if( GetProp()->nLog != -1 || GetRandomOptItemId() != 0 )
		return TRUE;
	return FALSE;
}

#ifndef __VM_0820
#ifndef __MEM_TRACE
#ifdef __WORLDSERVER
	CItemPool*	CItem::m_pPool	= new CItemPool( 512 );
#else	// __WORLDSERVER
	CItemPool*	CItem::m_pPool	= new CItemPool( 128 );
#endif	// __WORLDSERVER
#endif	// __MEM_TRACE
#endif	// __VM_0820

//////////////////////////////////////////////////////////////////////
// CItem - �̰��� ���� ���� �� ������ �����ϴ� ������Ʈ �������̴�. 
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
	m_dwType = OT_ITEM;
	m_pItemBase = NULL;
	m_idHolder	= 0;
	m_idOwn = NULL_ID;
	m_dwDropTime = 0;
	m_bDropMob = 0;
	m_dwDropTime = timeGetTime();
#ifdef __CLIENT
	m_fGroundY = 0;
	m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;
#endif //__CLIENT
#ifdef __EVENT_MONSTER
	m_IdEventMonster = NULL_ID;
#endif // __EVENT_MONSTER
}

void CItem::SetOwner( OBJID id )
{
	m_idOwn = id;	// �� �������� ������ pAttacker(����Ŀ)������ ǥ��.
	m_dwDropTime = ::timeGetTime();	// ��� ��������� �ð��� �����.
	m_bDropMob = TRUE;		// ���� �׾ ���� ���� ǥ�ø� �ص�.
}


CItem::~CItem()
{
	SAFE_DELETE( m_pItemBase );
#ifdef __WORLDSERVER
	RefreshSpawn(FALSE);
#endif
}

BOOL CItem::Read( CFileIO* pFile )
{
	CObj::Read( pFile );
	return TRUE;
}

#ifdef __CLIENT
// �������� ������ų�� �̰��� �ҷ���� �߷¿� ���� ��������.
void CItem::SetDelta( float fGroundY, D3DXVECTOR3 &vDelta )
{
	m_fGroundY = fGroundY;
	m_vDelta = vDelta;
}
#endif // __CLIENT

void CItem::Process()
{
	CCtrl::Process();
#ifdef __CLIENT
	AddAngle( 0.5f );
	D3DXVECTOR3 vPos = GetPos();

	//--- ��������� ������ ��ǥ�� ��ó���� �ؼ� �ȵȴ�. 
	if( m_fGroundY )	// �̰� 0�̸� �߷�ó���� �ؼ� �ȵȴ�.
	{
		if( vPos.y > m_fGroundY )		// ���߿� ���ִ���?
		{
			m_vDelta.y -= 0.0075f;		// �̵����Ϳ� �߷� ���� ����.
		} 
		else
		{
			vPos.y = m_fGroundY;		// �ٴڿ� ���� ���¸� ������ǥ�� �����ϰ� ����.
			m_vDelta.x = m_vDelta.y = m_vDelta.z = 0;	// �̵����ʹ� ������. Ƣ��� �Ϸ��� �̷��� �ϸ� �ȵȴ�.
			SetPos( vPos );				// ���� ��ǥ ����.
		}

		// �̵����Ͱ� ������ �������ʿ� ����.
		if( m_vDelta.x == 0 && m_vDelta.y == 0 && m_vDelta.z == 0 )
		{
			// ���� ��ǥ�� ����� �ʿ� ����.
		} 
		else
		{
			m_vDelta.x = m_vDelta.z = 0;		// ���⿡ �� ��������.
			vPos += m_vDelta;			// �̵� ���͸� ����.
			SetPos( vPos );
		}
	}
#endif // __CLIENT

#ifdef __WORLDSERVER
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return;
	if( (int)g_tmCurrent - (int)m_dwDropTime > MIN( 3 ) )
		Delete();
#endif	// __WORLDSERVER
}

void CItem::Render( )
{
#ifndef __WORLDSERVER
	ItemProp *pItemProp = GetProp();
	if( pItemProp && pItemProp->nReflect > 0 )
	{
		// ����!!! : m_pModel�� CModelObject��°��� �����ϰ� �Ѱ��̴�. �ƴ϶�� �̷��� ���� �ȵȴ�.
		((CModelObject*)m_pModel)->SetEffect( 0, XE_REFLECT );	
	}

	CObj::Render( );
	if( xRandom( 50 ) == 1 )
		CreateSfx( XI_GEN_ITEM_SHINE01, GetPos() );
#endif
}

void CItem::RenderName( CD3DFont* pFont, DWORD dwColor )
{
#ifndef __WORLDSERVER
	D3DXVECTOR3 vOut = ProjectWorldCoordToScreenCoord(
		std::nullopt,
		PWCTSC_DoNotResetWorldTransform | PWCTSC_UntouchedViewport
	);

	vOut.x -= pFont->GetTextExtent( m_pItemBase->GetProp()->szName ).cx / 2;
	pFont->DrawText( vOut.x + 1, vOut.y + 1, 0xff000000, m_pItemBase->GetProp()->szName	);
	pFont->DrawText( vOut.x, vOut.y, dwColor, m_pItemBase->GetProp()->szName );
	return;
#endif	// __WORLDSERVER
}

void	CItemElem::SetSerialNumber( void )
{
	m_liSerialNumber	= CSerialNumber::GetInstance()->Get();
}

CString CItemElem::GetName() const
{
	const ItemProp* pProp	= GetProp();
	CString strName		= pProp->szName;
	if( pProp->dwItemKind3 == IK3_EGG && m_pPet /*&& m_pPet->GetLevel() > PL_EGG*/ )
	{
		MoverProp* pMoverProp	= prj.GetMoverProp( m_pPet->GetIndex() );
		if( pMoverProp )
		{
			if( m_pPet->HasName() )
				strName.Format( prj.GetText( TID_GAME_CAGE_STRING ), m_pPet->GetName() );
			else
				strName.Format( prj.GetText( TID_GAME_CAGE_STRING ), pMoverProp->szName );
		}
	}
/*
	else if( pProp->IsUltimate() )
	{
		int	nFind	= strName.Find( "@", 0 );
		if( nFind > -1 )
			strName.Delete( nFind - 1, 2 );
	}
*/
	return strName;
}


enum	{	eNoLevelDown, e5LevelDown, e10LevelDown,	};
	int		GetLevelDown( void );	// 64|63
	void	SetLevelDown( int i );


int	CItemElem::GetLevelDown() const {
	if( m_iRandomOptItemId & 0x8000000000000000 )
		return -10;
	else if( m_iRandomOptItemId & 0x4000000000000000 )
		return -5;
	return 0;
}

void CItemElem::SetLevelDown( int i )
{
	//	0x8000000000000000
	//	0x4000000000000000
	m_iRandomOptItemId	&= ~0xC000000000000000;
	if( i == e5LevelDown )
		m_iRandomOptItemId	|= 0x4000000000000000;
	else if( i == e10LevelDown )
		m_iRandomOptItemId	|= 0x8000000000000000;
}

DWORD CItemElem::GetLimitLevel() const
{
	if( GetProp()->dwLimitLevel1 == 0xFFFFFFFF ) 
		return 0xFFFFFFFF;
	int nLimitLevel	= static_cast<int>( GetProp()->dwLimitLevel1 ) + GetLevelDown();
	if( nLimitLevel < 0 )
		nLimitLevel		= 0;

	return (DWORD)nLimitLevel;
}

bool CItemElem::IsLimitLevel(const CMover * pMover) const {
	// TODO: expect const ref to Mover instead of const ptr

	const ItemProp * prop = GetProp();
	if (!prop) return false;
	
	if (pMover->GetJobType() >= JTYPE_MASTER
		&& pMover->GetJobType() > pMover->GetJobType(prop->dwItemJob)) {
		if (prop->dwLimitLevel1 == NULL_ID || prop->dwLimitLevel1 <= MAX_LEVEL) {
			return false;
		}
	}

	if ((DWORD)(pMover->GetLevel()) >= GetLimitLevel()) {
		return false;
	}

	return true;
}


ItemProps::PiercingType CItemElem::GetPiercingType() const {
	using namespace ItemProps;

	const ItemProp * itemProp = GetProp();
	if (!itemProp) return PiercingType::None;

	if (itemProp->dwItemKind3 == IK3_SUIT) return PiercingType::NumericCard;
	
	if (itemProp->dwItemKind3 == IK3_SHIELD
		|| itemProp->dwItemKind2 == IK2_WEAPON_DIRECT
		|| itemProp->dwItemKind2 == IK2_WEAPON_MAGIC) {
		return PiercingType::LetterCard;
	}

	if (IsVisPet()) return PiercingType::Vis;
	
	return PiercingType::None;
}

// bSize�� �Ǿ�� ����� �ø� �� �ִ��� �˻��� �� TRUE���� setting �Ѵ�.
// bSize�� TRUE�� �� ��� dwTagetItemKind3�� NULL_ID�� �Ѵ�.
BOOL CItemElem::IsPierceAble( DWORD dwTargetItemKind3, BOOL bSize ) const
{
	if( !GetProp() )
		return FALSE;

	int nPiercedSize = GetPiercingSize();
	if( bSize ) // �Ǿ�� ����� �ø��� ���
		nPiercedSize++;
	
	if( GetProp()->dwItemKind3 == IK3_SUIT )
	{
		if( nPiercedSize <= MAX_PIERCING_SUIT )
		{
			if( dwTargetItemKind3 == NULL_ID )
				return TRUE;
			else if( dwTargetItemKind3 == IK3_SOCKETCARD )
					return TRUE;
		}
	}
	
	else if( GetProp()->dwItemKind3 == IK3_SHIELD
		|| GetProp()->dwItemKind2 == IK2_WEAPON_DIRECT
		|| GetProp()->dwItemKind2 == IK2_WEAPON_MAGIC
		)
	{
		if( nPiercedSize <= MAX_PIERCING_WEAPON )
		{
			if( dwTargetItemKind3 == NULL_ID )
				return TRUE;
			else if( dwTargetItemKind3 == IK3_SOCKETCARD2 )
				return TRUE;
		}
	}

	else if( IsVisPet() )
	{
		if( nPiercedSize <= MAX_VIS )
		{
			if( dwTargetItemKind3 == NULL_ID )
				return TRUE;
			else if( dwTargetItemKind3 == IK3_VIS )
				return TRUE;
		}
	}

	return FALSE;
}

#ifdef __WORLDSERVER
BOOL CItemElem::IsActiveTicket( DWORD dwItemId )
{
	if( !IsFlag( expired ) && GetProp()->dwItemKind3 == IK3_TICKET && m_dwKeepTime > 0 )
	{
		const TicketProp*	const pProp1	= g_ticketProperties.GetTicketProp( m_dwItemId );
		const TicketProp*	const pProp2	= g_ticketProperties.GetTicketProp( dwItemId );
		return ( pProp1->dwWorldId == pProp2->dwWorldId );
	}
	return FALSE;
}
#endif	// __WORLDSERVER

[[nodiscard]] bool ItemProp::IsNeedTarget() const noexcept {
	return dwExeTarget == EXT_ITEM;
}

bool CItemElem::IsEgg() const
{
	if( !IsPet() )
		return FALSE;
	return ( !m_pPet || m_pPet->GetLevel() == PL_EGG );
}

void  CItemElem::SetSwapVisItem( int nPos1, int nPos2 )
{
	int nSize = GetPiercingSize();
	if( nPos1 >= nSize || nPos2 >= nSize )
		return;

	DWORD dwTempId = GetPiercingItem( nPos1 );
	time_t tmTemp = GetVisKeepTime( nPos1 );

	SetPiercingItem( nPos1, GetPiercingItem( nPos2 ) );
	SetVisKeepTime( nPos1, GetVisKeepTime( nPos2 ) );
	SetPiercingItem( nPos2, dwTempId );
	SetVisKeepTime( nPos2, tmTemp );
}

DWORD CItemElem::GetVisPetSfxId()
{
	int nLevel = 0;
	for( int i=0; i<GetPiercingSize(); i++ )
	{
		ItemProp* pProp = prj.GetItemProp( GetPiercingItem( i ) );
		if( pProp && pProp->dwAbilityMax > (DWORD)( nLevel ) )
			nLevel = pProp->dwAbilityMax;
	}

	switch( nLevel )
	{
		case 1:	return XI_BUFFPET_GRADE1;
		case 2: return XI_BUFFPET_GRADE2;
		case 3: return XI_BUFFPET_GRADE3;
	}

	return NULL_ID;
}





//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
__int64		CItemElem::GetNewRandomOption()
{
	return m_n64NewRandomOption;
}

__int64*	CItemElem::GetNewRandomOptionPtr()
{
	return &m_n64NewRandomOption;
}

void		CItemElem::ResetNewRandomOption()
{
	m_n64NewRandomOption = 0;

	return;
}

void		CItemElem::SelectNewRandomOption()
{
	m_iRandomOptItemId	= (( m_iRandomOptItemId & 0xC0000000000000FF ) | m_n64NewRandomOption );

	return;
}

bool		CItemElem::SelectRandomOption( BYTE bySelectFlag )
{
	bool	bRetValue	= true;

	if( bySelectFlag == _AWAKE_OLD_VALUE )
	{
		ResetNewRandomOption();
	}
	else if( bySelectFlag == _AWAKE_NEW_VALUE )
	{
		SelectNewRandomOption();
		ResetNewRandomOption();
	}
	else
	{
		WriteLog( "bySelectFlag is invalid value. bySelectFlag : [%d]", (int)( bySelectFlag ) );

		bRetValue	= false;
	}

	return	bRetValue;
}

//	mulcom	END100405	���� ��ȣ�� �η縶��




////////////////////////////////////////////////////////////////////////////////

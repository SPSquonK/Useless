// UltimateWeapon.cpp: implementation of the CUltimateWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UltimateWeapon.h"
#ifdef __WORLDSERVER
#include "ItemUpgrade.h"
#include "User.h"

#include "DPSrvr.h"
#include "dpdatabaseclient.h"
#include "definetext.h"
#include "defineSound.h"
#include "defineObj.h"
#endif // __WORLDSERVER

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUltimateWeapon::CUltimateWeapon()
{
#ifdef __WORLDSERVER
	m_nSetGemProb = 0;
	m_nRemoveGemProb = 0;
	m_nGen2UniProb = 0;
	m_nUni2UltiProb= 0;
#endif // __WORLDSERVER
}

CUltimateWeapon::~CUltimateWeapon()
{

}

BOOL CUltimateWeapon::Load_GemAbility()
{
	CScanner s;
	
	if( !s.Load( "Ultimate_GemAbility.txt" ) )
		return FALSE;
	
	int nMaxGemNum = 0;
	s.GetToken();
	while( s.tok != FINISHED )
	{
		if( s.Token == _T( "ABILITY" ) )
		{
			__GEMABILITYKIND GemAbilityKind;
			s.GetToken();
			GemAbilityKind.nAbility = CScript::GetDefineNum( s.Token );
			
			s.GetToken(); // {
			s.GetToken();
			while( *s.token != '}' )
			{
				__GEMABILITY GemAbility;
				GemAbility.dwGemItemId = CScript::GetDefineNum( s.Token );
				s.GetToken();
				GemAbility.dwAbilityItemId = CScript::GetDefineNum( s.Token );
				for( int i=0; i<5; i++ )
					GemAbility.vecAbility.push_back( s.GetNumber() );
				GemAbilityKind.vecAbilityKind.push_back( GemAbility );
				s.GetToken();
			}
			m_vecGemAbilityKind.push_back( GemAbilityKind );
		}
		else if( s.Token == _T( "LEVEL_GEM" ) )
		{
			__MAKEGEMITEM	MakeGemItem;
			s.GetToken(); // {
			s.GetToken();
			while( *s.token != '}' )
			{
				MakeGemItem.dwItemID = CScript::GetDefineNum( s.Token );
				if( MakeGemItem.dwItemID == (DWORD)-1 )		// cr
				{
					Error( "UltimateWeapon::Load_UltimateWeapon() �������� ���°��� ��������" );
					return FALSE;
				}
				MakeGemItem.nMinLevel = s.GetNumber();
				MakeGemItem.nMaxLevel = s.GetNumber();

				m_vecMakeGemItem.push_back( MakeGemItem );
				s.GetToken();
			}
		}
		s.GetToken();
	}	
	return TRUE;
}

// ������ ���� ������ ���� ���� ����
DWORD CUltimateWeapon::GetGemKind( DWORD dwLimitLevel )
{
	for( DWORD i=0; i<m_vecMakeGemItem.size(); i++ )
	{
		if( (int)( dwLimitLevel ) <= m_vecMakeGemItem[i].nMaxLevel &&
			(int)( dwLimitLevel ) >= m_vecMakeGemItem[i].nMinLevel )
			return m_vecMakeGemItem[i].dwItemID;
	}

	return NULL_ID;
}

#ifdef __WORLDSERVER
BOOL CUltimateWeapon::Load_UltimateWeapon()
{
	CScanner s;
	
	if( !s.Load( "Ultimate_UltimateWeapon.txt" ) )
		return FALSE;
	

	s.GetToken();
	while( s.tok != FINISHED )
	{
		if( s.Token == _T( "SET_GEM" ) )
			m_nSetGemProb = s.GetNumber();
		else if( s.Token == _T( "REMOVE_GEM" ) )
			m_nRemoveGemProb = s.GetNumber();
		else if( s.Token == _T( "GENERAL2UNIQUE" ) )
			m_nGen2UniProb = s.GetNumber();
		else if( s.Token == _T( "UNIQUE2ULTIMATE" ) )
			m_nUni2UltiProb = s.GetNumber();
		else if( s.Token == _T( "MAKE_GEM" ) )
		{
			__MAKEGEMPROB infoMakeGem;
			s.GetToken();	// {
			s.GetToken();
			while( *s.token != '}' )
			{
				int nEnchant = atoi( s.Token );
				infoMakeGem.dwGeneralProb = s.GetNumber();
				infoMakeGem.nGeneralNum = s.GetNumber();
				infoMakeGem.dwUniqueProb = s.GetNumber();
				infoMakeGem.nUniqueNum = s.GetNumber();

				m_mapMakeGemProb.emplace(nEnchant, infoMakeGem);
				s.GetToken();
			}
		}
		else if( s.Token == _T( "ULTIMATE_ENCHANT" ) )
		{
			int nNum = 0;
			int nProb= 0;
			s.GetToken(); // {
			s.GetToken();
			while( *s.token != '}' )
			{
				nNum = atoi( s.Token );
				nProb = s.GetNumber();
				m_mapUltimateProb.emplace( nNum, nProb);
				s.GetToken();
			}
		}
		s.GetToken();
	}	
	return TRUE;
}

// ���� �ռ��� �ο� �Ǵ� �ɷ�(�ɷ�)
DWORD CUltimateWeapon::GetGemAbilityKindRandom( DWORD dwGemItemid )
{
	if( m_vecGemAbilityKind.empty() == TRUE )
		return NULL_ID;

	int nRandom = xRandom( m_vecGemAbilityKind.size() );

	for(DWORD i=0; i<m_vecGemAbilityKind[nRandom].vecAbilityKind.size(); i++ )
	{
		if( m_vecGemAbilityKind[nRandom].vecAbilityKind[i].dwGemItemId == dwGemItemid )
			return m_vecGemAbilityKind[nRandom].vecAbilityKind[i].dwAbilityItemId;
	}
	return NULL_ID;
}

// ���� ���� - ���� ������ �ı�
CUltimateWeapon::MakeGemAnswer CUltimateWeapon::MakeGem(CUser & pUser, OBJID objItemId) {
	if (CItemUpgrade::IsInTrade(pUser)) return Answer::Cancel{};
	
	CItemElem * pItemElem = pUser.m_Inventory.GetAtId(objItemId);
	if (!IsUsableItem(pItemElem)) return Answer::Cancel{};
	if (pUser.m_Inventory.IsEquip(objItemId)) {
		pUser.AddDefinedText(TID_GAME_ULTIMATE_ISEQUIP, "");
		return Answer::Cancel{};
	}

	// ���Ⱑ �ƴ� ��
	if (pItemElem->GetProp()->dwItemKind1 != IK1_WEAPON) {
		return Answer::Cancel{};
	}
	// �Ϲݹ��⳪ ����ũ �������� �ƴ� ��
	if(	pItemElem->GetProp()->dwReferStat1 != WEAPON_GENERAL &&
		pItemElem->GetProp()->dwReferStat1 != WEAPON_UNIQUE )
	{
		return Answer::Cancel{};
	}
	
	// ������ �ش��ϴ� ���� 
	const DWORD dwItemId = GetGemKind( pItemElem->GetProp()->dwLimitLevel1 );
	if( dwItemId == NULL_ID )
		return Answer::Cancel{};

	const int nOpt = pItemElem->GetAbilityOption();
	const auto it = m_mapMakeGemProb.find(nOpt);
	
	if( it == m_mapMakeGemProb.end() )
		return Answer::Cancel{};

	DWORD dwProb = it->second.dwGeneralProb;
	int nItemNum = it->second.nGeneralNum;
	if( pItemElem->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
	{
		dwProb = it->second.dwUniqueProb;
		nItemNum = it->second.nUniqueNum;
	}
	// �κ��丮�� ������ �� - ���
	CItemElem itemElemTemp;
	itemElemTemp.m_dwItemId = dwItemId;
	ItemProp* pItemprop = itemElemTemp.GetProp();
	if( !pItemprop )
		return Answer::Cancel{};

	// ���⸦ �����Ǽ� 1ĭ�� ����Ƿ� 2ĭ�̻� �϶��� �˻�
	if( (DWORD)( nItemNum ) > pItemprop->dwPackMax 
		&& pUser.m_Inventory.IsFull( &itemElemTemp, pItemprop, (short)( nItemNum - pItemprop->dwPackMax ) ) )
		return Answer::Inventory{};
	
	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser.GetName();
	aLogItem.RecvName = "ULTIMATE_MAKEGEM";
	aLogItem.WorldId = pUser.GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser.GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );	
	pUser.RemoveItem( (BYTE)( objItemId ), 1 );
	
	const DWORD nRandom = xRandom( 1000000 );

	if (nRandom >= dwProb) {
		aLogItem.RecvName = "ULTIMATE_MAKEGEM_FAILED";
		g_DPSrvr.OnLogItem(aLogItem);
		return Answer::Fail{};
	}

	CItemElem itemElem;
	itemElem.m_dwItemId = dwItemId;
	itemElem.m_nItemNum	= nItemNum;
	itemElem.SetSerialNumber();
	itemElem.m_nHitPoint	= 0;

	pUser.CreateItem( &itemElem );
	aLogItem.RecvName = "ULTIMATE_MAKEGEM_SUCCESS";
	g_DPSrvr.OnLogItem( aLogItem, &itemElem, nItemNum );
	return MakeGemSuccess{ dwItemId, nItemNum };
}

// ���⿡ ���� �ռ�
int CUltimateWeapon::SetGem( CUser* pUser, OBJID objItemId, OBJID objGemItemId )
{
	CItemElem* pItemElem	= pUser->m_Inventory.GetAtId( objItemId );
	CItemElem* pGemItemElem	= pUser->m_Inventory.GetAtId( objGemItemId );
	if( !IsUsableItem( pItemElem ) || !IsUsableItem( pGemItemElem ) )
		return ULTIMATE_CANCEL;

	// ���͸� ������ �ƴ� ��
	if( pItemElem->GetProp()->dwReferStat1 != WEAPON_ULTIMATE )
		return ULTIMATE_ISNOTULTIMATE;


	// �����Ǿ� ������ �ߴ�
	if( pUser->m_Inventory.IsEquip( objItemId ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}
		
	int nCount = NULL;
	for( ; nCount < pItemElem->GetUltimatePiercingSize(); nCount++ )
		if( pItemElem->GetUltimatePiercingItem( nCount ) == 0 )
			break;
		
	// ����� ������ �ߴ�
	if( nCount == pItemElem->GetUltimatePiercingSize() )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_GEMSPACE, "" );
		return ULTIMATE_CANCEL;
	}

	DWORD dwSetItemId = GetGemAbilityKindRandom( pGemItemElem->m_dwItemId );
	if( dwSetItemId == NULL_ID )
		return ULTIMATE_CANCEL;
	
	// ���� ����
	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "ULTIMATE_PIERCING";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pGemItemElem, 1 );	
	pUser->RemoveItem( (BYTE)( objGemItemId ), 1 );
	
	int nRandom = xRandom( 1000000 );
	if( nRandom < m_nSetGemProb )
	{
		// ���� �ռ�
		pUser->UpdateItem(*pItemElem, UI::Piercing::Item{ UI::Piercing::Kind::Ultimate, nCount, dwSetItemId });
		aLogItem.RecvName = "ULTIMATE_PIERCING_SUCCESS";
		g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );
		return ULTIMATE_SUCCESS;	
	}
	aLogItem.RecvName = "ULTIMATE_PIERCING_FAILED";
	g_DPSrvr.OnLogItem( aLogItem, pItemElem );
	return ULTIMATE_FAILED;
}

// ���⿡ ���������� �ռ��� ���� ����
int CUltimateWeapon::RemoveGem( CUser* pUser, OBJID objItemId, OBJID objItemGem )
{
	CItemElem* pItemElem	= pUser->m_Inventory.GetAtId( objItemId );
	CItemElem* pItemElemGem	= pUser->m_Inventory.GetAtId( objItemGem );
	if( !IsUsableItem( pItemElem ) || !IsUsableItem( pItemElemGem ) )
		return ULTIMATE_CANCEL;
	
	if( pItemElem->GetProp()->dwReferStat1 != WEAPON_ULTIMATE )
		return ULTIMATE_ISNOTULTIMATE;
	
	// �ռ��� ������ ���� �� X
	if( pItemElem->GetUltimatePiercingItem( 0 ) == 0 )
		return ULTIMATE_CANCEL;

	if( pItemElemGem->m_dwItemId != II_GEN_MAT_MOONSTONE
		&& pItemElemGem->m_dwItemId != II_GEN_MAT_MOONSTONE_1 )
		return ULTIMATE_CANCEL;
 
	// ���� �Ǿ� ���� �� X
	if( pUser->m_Inventory.IsEquip( objItemId ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}	

	// ������ ����
	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "ULTIMATE_PIERCING_REMOVE";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem, 1 );
	pUser->RemoveItem( (BYTE)( objItemGem ), 1 );
	
	int nRandom = xRandom( 1000000 );
	if( nRandom < m_nRemoveGemProb )
	{
		// ������ - ���� ����
		for( int i=pItemElem->GetUltimatePiercingSize()-1; i>=0; i-- )
		{
			if( pItemElem->GetUltimatePiercingItem( i ) != 0 )
			{
				pUser->UpdateItem(*pItemElem, UI::Piercing::Item{ UI::Piercing::Kind::Ultimate, i, 0 });
				aLogItem.RecvName = "ULTIMATE_PIERCING_REMOVE_SUC";
				g_DPSrvr.OnLogItem( aLogItem, pItemElem, 1 );
				break;
			}
		}
		return ULTIMATE_SUCCESS;
	}
	aLogItem.RecvName = "ULTIMATE_PIERCING_REMOVE_FAIL";
	g_DPSrvr.OnLogItem( aLogItem, pItemElem );
	return ULTIMATE_FAILED;
}

// ������ ����Į�� ���� - ����Į�� 5��, ������ 5�� �ռ�
CUltimateWeapon::Result CUltimateWeapon::MakeOrichalcum2(CUser & pUser, const std::array<OBJID, MAX_JEWEL> & objItemId) {
	// ~~ Transform into a map the array
	std::map<OBJID, unsigned int> mapObjId;

	for (const OBJID objid : objItemId) {
		++mapObjId[objid];
	}

	// ~~ Check number of orichalcum and moonstones
	static constexpr auto IsOrichalcum = [](const CItemElem & item) {
		return item.m_dwItemId == II_GEN_MAT_ORICHALCUM01 || item.m_dwItemId == II_GEN_MAT_ORICHALCUM01_1;
	};
	static constexpr auto IsMoonstone = [](const CItemElem & item) {
		return item.m_dwItemId == II_GEN_MAT_MOONSTONE || item.m_dwItemId == II_GEN_MAT_MOONSTONE_1;
	};

	unsigned int numberOfOrichalcum = 0;
	unsigned int numberOfMoonstone = 0;

	for (const auto & [objid, quantity] : mapObjId) {
		const CItemElem * const pItemElem = pUser.m_Inventory.GetAtId(objid);
		if (!IsUsableItem(pItemElem)) {
			return Result::Cancel;
		}

		if (quantity > pItemElem->m_nItemNum) {
			return Result::Cancel;
		}

		if (IsOrichalcum(*pItemElem)) {
			numberOfOrichalcum += quantity;
		} else if (IsMoonstone(*pItemElem)) {
			numberOfMoonstone += quantity;
		} else {
			return Result::Cancel;
		}
	}

	if (numberOfOrichalcum != 5 || numberOfMoonstone != 5) {
		return Result::Cancel;
	}
	
	// ~~ Create Orichalcum2
	CItemElem itemElemTemp;
	itemElemTemp.m_dwItemId = II_GEN_MAT_ORICHALCUM02;
	if( !itemElemTemp.GetProp() || pUser.m_Inventory.IsFull( &itemElemTemp, itemElemTemp.GetProp(), 1 ) )
		return Result::Inventory;
	
	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser.GetName();
	aLogItem.RecvName = "ULTIMATE_MAKEITEM";
	aLogItem.WorldId = pUser.GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser.GetGold();
	

	for (const auto & [objid, quantity] : mapObjId) {
		CItemElem * pItemElem = pUser.m_Inventory.GetAtId(objid);
		g_DPSrvr.OnLogItem(aLogItem, pItemElem, quantity);
		pUser.RemoveItem(objid, quantity);
	}

	CItemElem itemElem;
	itemElem.m_dwItemId = II_GEN_MAT_ORICHALCUM02;
	itemElem.m_nItemNum	= 1;
	itemElem.SetSerialNumber();
	itemElem.m_nHitPoint	= 0;
	
	pUser.CreateItem(&itemElem);
	aLogItem.RecvName = "ULTIMATE_MAKEITEM_SUCCESS";
	g_DPSrvr.OnLogItem(aLogItem, &itemElem, 1);
	
	return Result::Success;
}

// ���⺯ȯ(�Ϲ�->����ũ, ����ũ->���͸�)
int CUltimateWeapon::TransWeapon( CUser* pUser, OBJID objItem, OBJID objGem1, OBJID objGem2 )
{
	CItemElem* pItemElemWeapon	= pUser->m_Inventory.GetAtId( objItem );
	CItemElem* pItemElemGem1	= pUser->m_Inventory.GetAtId( objGem1 );
	CItemElem* pItemElemGem2	= pUser->m_Inventory.GetAtId( objGem2 );
	if( !IsUsableItem( pItemElemWeapon )
		|| !IsUsableItem( pItemElemGem1 )
		|| !IsUsableItem( pItemElemGem2 ) )
		return ULTIMATE_CANCEL;
	
	// �����ϰ� ������ ���
	if( pUser->m_Inventory.IsEquip( objItem ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}
		
	// ���Ⱑ �ƴϸ� ���
	if( pItemElemWeapon->GetProp()->dwItemKind1 != IK1_WEAPON )
	{
#ifdef __INTERNALSERVER
		pUser->AddText( "���Ⱑ �ƴ�." );
#endif // __INTERNALSERVER
		return ULTIMATE_CANCEL;
	}

	// Ÿ�� ID �� ���� ��
	if( pItemElemWeapon->GetProp()->dwReferTarget1 == NULL_ID )
	{
		return ULTIMATE_CANCEL;	
	}
	
	// �Ϲ�, ����ũ �������� �ƴ� ��
	if( pItemElemWeapon->GetProp()->dwReferStat1 != WEAPON_GENERAL &&
		  pItemElemWeapon->GetProp()->dwReferStat1 != WEAPON_UNIQUE )
	{
		return ULTIMATE_CANCEL;
	}

	DWORD dwItemId = GetGemKind( pItemElemWeapon->GetProp()->dwLimitLevel1 );
	if( dwItemId != pItemElemGem1->m_dwItemId || pItemElemGem2->m_dwItemId != II_GEN_MAT_ORICHALCUM02 )
		return ULTIMATE_CANCEL;

	CString strTemp = "UNIQUE_"; 
	CString strLog;
	int nProb = m_nGen2UniProb;
	if( pItemElemWeapon->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
	{
		if( pItemElemWeapon->GetAbilityOption() != 10 )
			return ULTIMATE_CANCEL;
		nProb = m_nUni2UltiProb;
		strTemp = "ULTIMATE_";
	}
	// propItem.txt -> dwReferTarget2(����Ÿ��2)�� ���� ��ȯ Ȯ���� �ȴ�.
	// ���� ������ Ultimate_UltimateWeapon.txt�� ���ǵ� ���� ��ȯ Ȯ���� �ȴ�.
	if( pItemElemWeapon->GetProp()->dwReferTarget2 != NULL_ID )
		nProb = pItemElemWeapon->GetProp()->dwReferTarget2;
	
	BOOL bSmelprot3	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT3 )
		&& pItemElemWeapon->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
	{
		bSmelprot3	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT3 );
		
		ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELPROT3 );
		if( pItemProp )
			g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}

	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser->GetName();
	strLog = strTemp + "TRANS";
	aLogItem.RecvName = (LPCTSTR)strLog;
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem1, 1 );
	pUser->RemoveItem( (BYTE)( objGem1 ), 1 );
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem2, 1 );
	pUser->RemoveItem( (BYTE)( objGem2 ), 1 );

	int nRandom = xRandom( 1000000 );
	if( nRandom < nProb )
	{
		CItemElem CreateItem;
		CreateItem	= *pItemElemWeapon;
		CreateItem.m_dwItemId = pItemElemWeapon->GetProp()->dwReferTarget1;
		CreateItem.SetSerialNumber();

		if( pItemElemWeapon->GetProp()->dwReferStat1 == WEAPON_UNIQUE )
			CreateItem.SetAbilityOption( 0 );
		

		g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
		pUser->RemoveItem( (BYTE)( objItem ), 1 );
		if( pUser->CreateItem( &CreateItem ) )
		{
			//aLogItem.RecvName = "ULTIMATE_TRANS_SUCCESS";
			strLog = strTemp + "TRANS_SUCCESS";
			aLogItem.RecvName = (LPCTSTR)strLog;
			g_DPSrvr.OnLogItem( aLogItem, &CreateItem, 1 );
			return ULTIMATE_SUCCESS;
		}
	}
	else //����
	{
		if( !bSmelprot3 )
		{
			//aLogItem.RecvName = "ULTIMATE_TRANS_FAILED";
			strLog = strTemp + "TRANS_FAILED";
			aLogItem.RecvName = (LPCTSTR)strLog;
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
			pUser->RemoveItem( (BYTE)( objItem ), 1 );
		}
		else
		{
			//aLogItem.RecvName = "ULTIMATE_TRANS_PROTECT";
			strLog = strTemp + "TRANS_PROTECT";
			aLogItem.RecvName = (LPCTSTR)strLog;
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon );
		}
	}
	
	return ULTIMATE_FAILED;
}

// ���͸� ���� ����
int CUltimateWeapon::EnchantWeapon( CUser* pUser, OBJID objItem, OBJID objItemGem )
{
	CItemElem* pItemElemWeapon	= pUser->m_Inventory.GetAtId( objItem );
	CItemElem* pItemElemGem	= pUser->m_Inventory.GetAtId( objItemGem );
	if( !IsUsableItem( pItemElemWeapon ) || !IsUsableItem( pItemElemGem ) )
		return ULTIMATE_CANCEL;

	//���͸� ������ �ƴϰų� ������ ����Į���� �ƴϸ� ���
	if( pItemElemWeapon->GetProp()->dwReferStat1 != WEAPON_ULTIMATE ||
		pItemElemGem->m_dwItemId != II_GEN_MAT_ORICHALCUM02 )
		return ULTIMATE_ISNOTULTIMATE;

	// �����ϰ� ������ ���
	if( pUser->m_Inventory.IsEquip( objItem ) )
	{
		pUser->AddDefinedText( TID_GAME_ULTIMATE_ISEQUIP , "" );
		return ULTIMATE_CANCEL;
	}

	int pAbilityOpt = pItemElemWeapon->GetAbilityOption();
	if( (pAbilityOpt+1) > 10 )
	{
		pUser->AddDefinedText( TID_UPGRADE_MAXOVER , "" );
		return ULTIMATE_CANCEL;
	}

	BOOL bSmelprot3	= FALSE;
	if( pUser->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT3 ) )
	{
		bSmelprot3	= TRUE;
		pUser->RemoveBuff( BUFF_ITEM, II_SYS_SYS_SCR_SMELPROT3 );
		
		ItemProp* pItemProp = prj.GetItemProp( II_SYS_SYS_SCR_SMELPROT3 );
		if( pItemProp )
			g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	
	//���� ����
	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "ULTIMATE_ENCHANT";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = aLogItem.Gold2 = pUser->GetGold();
	g_DPSrvr.OnLogItem( aLogItem, pItemElemGem, 1 );	
	pUser->RemoveItem( (BYTE)( objItemGem ), 1 );

	int nRandom = xRandom( 1000000 );
	const auto it = m_mapUltimateProb.find( pAbilityOpt+1 );
	if( it == m_mapUltimateProb.end() )
		return ULTIMATE_CANCEL;
	
	if( nRandom < it->second )
	{
		pUser->UpdateItem(*pItemElemWeapon, UI::AbilityOption::Up);
		if( pAbilityOpt >= 5 )
			pUser->UpdateItem( *pItemElemWeapon, UI::Piercing::Size::Ultimate );
		aLogItem.RecvName = "ULTIMATE_ENCHANT_SUCCESS";
		g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
		return ULTIMATE_SUCCESS;
	}
	else
	{
		if( bSmelprot3 )	// ��ȣ�� �η縶��(�ֻ��) ����
		{
			aLogItem.RecvName = "ULTIMATE_ENCHANT_PROTECT";
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon );
		}
		else	// ���͸� ���� �ı�
		{
			aLogItem.RecvName = "ULTIMATE_ENCHANT_FAILED";
			g_DPSrvr.OnLogItem( aLogItem, pItemElemWeapon, 1 );
			pUser->RemoveItem( (BYTE)( objItem ), 1 );
		}
	}	
	return ULTIMATE_FAILED;
}


BYTE CUltimateWeapon::SmeltSafetyUltimate( CUser* pUser, CItemElem* pItemMain, CItemElem* pItemMaterial, CItemElem* pItemProtScr )
{
	//	���͸ڿ����� �ƴϸ� ����
	if( pItemMain->GetProp()->dwReferStat1 != WEAPON_ULTIMATE )
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

	//	�ֻ�� ��ȣ�� �η縶���� �ƴϸ� ����
	if( pItemProtScr->GetProp()->dwID != II_SYS_SYS_SCR_SMELPROT3)
	{
		//pUser->AddDefinedText( TID_GAME_NOTEQUALITEM );
		return 0;
	}

	//	���ü�ġ�� maxġ�� �Ѿ����� ����
	if( pItemMain->GetAbilityOption() >= 10 )
	{
		//pUser->AddDefinedText( TID_UPGRADE_MAXOVER );
		return 3;
	}


	LogItemInfo aLogItem;
	aLogItem.Action = "-";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "ULTIMATE_ENC_SMELTSAFETY";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	aLogItem.Gold = pUser->GetGold();
	aLogItem.Gold2 = pUser->GetGold();
	
	// ���þ��� ���� - �����̴�, ���д�...
	g_DPSrvr.OnLogItem( aLogItem, pItemMaterial, pItemMaterial->m_nItemNum );
	pUser->RemoveItem( (BYTE)( pItemMaterial->m_dwObjId ), 1 );
	//	�η縶�� �α׳����
	ItemProp* pItemProp = pItemProtScr->GetProp();
	if( pItemProp )
	{
		g_dpDBClient.SendLogSMItemUse( "1", pUser, pItemProtScr, pItemProp );
		g_dpDBClient.SendLogSMItemUse( "2", pUser, NULL, pItemProp );
	}
	pUser->RemoveItem( (BYTE)( pItemProtScr->m_dwObjId ), 1 );

	// ���� Ȯ��
	const auto it = m_mapUltimateProb.find( pItemMain->GetAbilityOption() + 1 );
	if( it == m_mapUltimateProb.end() )
		return 0;
	
	if( (int)( xRandom( 1000000 ) ) > it->second )
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADEFAIL );
		if( ( pUser->IsMode( TRANSPARENT_MODE ) ) == 0 )
			g_UserMng.AddCreateSfxObj(pUser, XI_INT_FAIL);

		aLogItem.RecvName = "ULTIMATE_ENC_FAIL_SMELTSAFETY";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 2;
	}
	else
	{
		// ����
		pUser->AddPlaySound( SND_INF_UPGRADESUCCESS );		
		if((pUser->IsMode( TRANSPARENT_MODE ) ) == 0 )
			g_UserMng.AddCreateSfxObj(pUser, XI_INT_SUCCESS);
		
		pUser->UpdateItem(*pItemMain, UI::AbilityOption::Up);
		if (pItemMain->GetAbilityOption() > 5) {
			pUser->UpdateItem(*pItemMain, UI::Piercing::Size::Ultimate);
		}
		
		aLogItem.RecvName = "ULTIMATE_ENC_SUC_SMELTSAFETY";
		g_DPSrvr.OnLogItem( aLogItem, pItemMain, pItemMain->m_nItemNum );

		return 1;
	}
	return 0;
}
#endif // __WORLDSERVER

// ���� �ռ� �� �ɷ�ġ ����
void CUltimateWeapon::SetDestParamUltimate( CMover* pMover, CItemElem* pItemElem, BOOL bEquip, DWORD dwParts )
{
	
	if( dwParts != PARTS_RWEAPON )
		return;

	for (const auto & pair : GetDestParamUltimate(pItemElem)) {
		if( bEquip )
			pMover->SetDestParam(pair.first, pair.second, NULL_CHGPARAM);
		else 
			pMover->ResetDestParam(pair.first, pair.second, TRUE);
	}
}

// �ռ��� �������� ���� �� �ɷ�
std::map<int, int> CUltimateWeapon::GetDestParamUltimate(const CItemElem * pItemElem) const {
	if( !pItemElem || pItemElem->GetProp()->dwReferStat1 != WEAPON_ULTIMATE )
		return {};
	
	boost::container::small_flat_map<DWORD, int, 10> mapItem;
	for (int i=0; i<pItemElem->GetUltimatePiercingSize(); i++) {
		const DWORD piercingItem = pItemElem->GetUltimatePiercingItem(i);
		if (piercingItem != 0) {
			mapItem[piercingItem] += 1;
		}
	}
	
	std::map<int, int> mapDst;
	for (const auto & [itemId, qtt] : mapItem) {
		const ItemProp * const pItemProp	= prj.GetItemProp(itemId);
		const int nResultDst = GetDST(itemId, qtt, pItemProp->dwReferStat2 );
		mapDst[pItemProp->dwReferStat2] += nResultDst;
	}
	return mapDst;
}

// �ռ��� ������ ������ ������ ���� �ɷ�ġ
int CUltimateWeapon::GetDST( DWORD dwItemId, int nItemNum, int nDSTInfo ) const
{
	int nResultDST = 0;
	__GEMABILITYKIND	GemAbilityKind;

	BOOL bFind = FALSE;
	for( DWORD i=0; i<m_vecGemAbilityKind.size(); i++ )
	{
		if( m_vecGemAbilityKind[i].nAbility == nDSTInfo )
		{
			GemAbilityKind = m_vecGemAbilityKind[i];
			bFind = TRUE;
			break;
		}
	}

	if( !bFind )
		return 0;

	for( DWORD i = 0 ; i < GemAbilityKind.vecAbilityKind.size() ; ++i )
	{
		if( dwItemId == GemAbilityKind.vecAbilityKind[i].dwAbilityItemId )
		{
			nResultDST = GemAbilityKind.vecAbilityKind[i].vecAbility[nItemNum-1];
			break;
		}
	}	
	return nResultDST;
}

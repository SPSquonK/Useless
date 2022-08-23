#include "stdafx.h"
#include "defineSound.h"
#include "defineText.h"
#include "defineObj.h"
#include "AppDefine.h"
#include "WndManager.h"

#include "DPClient.h"

#include "Tax.h"
#include "Vector3Helper.h"


#define PARTSMESH_HAIR( nSex )  ( nSex == SEX_MALE ? _T( "Part_maleHair%02d.o3d" ) : _T( "Part_femaleHair%02d.o3d" ) )
#define PARTSMESH_HEAD( nSex )  ( nSex == SEX_MALE ? _T( "Part_maleHead%02d.o3d" ) : _T( "Part_femaleHead%02d.o3d" ) )
#define PARTSMESH_UPPER( nSex ) ( nSex == SEX_MALE ? _T( "Part_maleUpper.o3d"    ) : _T( "Part_femaleUpper.o3d"    ) )
#define PARTSTEX_UPPER( nSex ) ( nSex == SEX_MALE ? _T( "Part_maleUpper%02d.dds" ) : _T( "Part_femaleUpper%02d.dds" ) )
#define TEX_PART_UPPER( nSex ) ( nSex == SEX_MALE ? _T( "Part_maleUpper01.dds" ) : _T( "Part_femaleUpper01.dds" ) )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndWarning::CWndWarning() 
{ 
	m_pMover = NULL;
	m_pItemElem = NULL;
} 
CWndWarning::~CWndWarning() 
{ 

} 

BOOL CWndWarning::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	InitDialog( dwWndId, pWndParent, WBS_KEY, 0 );
	MoveParentCenter();

	return TRUE;
}

BOOL CWndWarning::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK )
	{
		SAFE_DELETE( ((CWndShop*)m_pParentWnd)->m_pWndConfirmSell );
		((CWndShop*)m_pParentWnd)->m_pWndConfirmSell = new CWndConfirmSell;
		((CWndShop*)m_pParentWnd)->m_pWndConfirmSell->m_pItemElem = m_pItemElem;
		((CWndShop*)m_pParentWnd)->m_pWndConfirmSell->m_pMover = m_pMover;
		((CWndShop*)m_pParentWnd)->m_pWndConfirmSell->Initialize( m_pParentWnd, APP_CONFIRM_SELL );
		Destroy();
	}
	if( nID == WIDC_CANCEL )
	{
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndConfirmSell::CWndConfirmSell() 
{ 
	m_pMover = NULL;
	m_pItemElem = NULL;
	m_pEdit = NULL;
	m_pStatic = NULL;
} 
CWndConfirmSell::~CWndConfirmSell() 
{ 
} 
BOOL CWndConfirmSell::Process( void )
{
	if(m_pItemElem->GetExtra() > 0)
	{
		Destroy();
	}
	return TRUE;
}
void CWndConfirmSell::OnDraw( C2DRender* p2DRender ) 
{ 
	if( m_pItemElem->IsEmpty() )
	{
		m_pStaticGold->SetTitle( CString( "0" ) );
		return;
	}
	LPCTSTR szNumber;
	szNumber = m_pEdit->GetString();
	int nNumber = atoi( szNumber );

	if( m_pItemElem->m_nItemNum == 1 )
	{
		m_pEdit->SetString( "1" );
	}
	else
	{
		if( m_pItemElem->m_nItemNum < nNumber )
		{
			char szNumberbuf[16] = {0, };
			nNumber = m_pItemElem->m_nItemNum;
			_itoa( m_pItemElem->m_nItemNum, szNumberbuf, 10 );
			m_pEdit->SetString( szNumberbuf );
		}
		for( int i = 0 ; i < 8 ; i++ )
		{
			char szNumberbuf[8] = {0, };
			strncpy( szNumberbuf, szNumber, 8 );
			
			// 0 : 공백, 48 : 숫자 0, 57 : 숫자 9
			if( 47 >= szNumberbuf[i] || szNumberbuf[i] >= 58 )
			{
				if( szNumberbuf[i] != 0 )
				{
					nNumber = m_pItemElem->m_nItemNum;
					_itoa( m_pItemElem->m_nItemNum, szNumberbuf, 10 );
					m_pEdit->SetString( szNumberbuf );
					break;												
				}
			}
		}
	}
	DWORD dwCost = m_pItemElem->GetCost();
	DWORD BuyGold = dwCost / 4;
#ifdef __SHOP_COST_RATE
	BuyGold = static_cast< DWORD >( static_cast< float >( BuyGold ) * prj.m_fShopSellRate );
#endif // __SHOP_COST_RATE
	if( CTax::GetInstance()->IsApplyTaxRate( g_pPlayer, m_pItemElem ) )
		BuyGold -= ( static_cast<DWORD>(BuyGold * CTax::GetInstance()->GetSalesTaxRate( g_pPlayer )) );
//	TCHAR szNumberGold[ 64 ];
	CString szNumberGold;


	if( BuyGold < 1 )
		BuyGold = 1;

	BuyGold *= nNumber;
//	_itot( BuyGold, szNumberGold, 10 );
	szNumberGold.Format("%u", BuyGold);
	m_pStaticGold->SetTitle( szNumberGold );
} 
void CWndConfirmSell::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndButton* pOk   = (CWndButton*)GetDlgItem( WIDC_OK );
	CWndBase* pEdit   = (CWndButton*)GetDlgItem( WIDC_EDITSELL );
	CWndStatic* pStatic = (CWndStatic  *)GetDlgItem( WIDC_STATIC2 );
	pStatic->AddWndStyle(WSS_MONEY);
	pOk->SetDefault( TRUE );
	pEdit->SetFocus();
	
	
} 
BOOL CWndConfirmSell::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	InitDialog( APP_CONFIRM_SELL, pWndParent, WBS_KEY, 0 );
	MoveParentCenter();

	CWndStatic* pLabel  = (CWndStatic  *)GetDlgItem( WIDC_STATIC );
//	pLabel->m_dwColor = 0xff000000;
	
	m_pEdit   = (CWndEdit  *)GetDlgItem( WIDC_EDITSELL );
	m_pStatic	= (CWndStatic *)GetDlgItem( WIDC_CONTROL1 );
	m_pStaticGold = (CWndStatic *)GetDlgItem( WIDC_STATIC2 );
//	m_pStatic->m_dwColor = m_pStaticGold->m_dwColor = 0xff000000;
	
	if( m_pItemElem->m_nItemNum == 1 )
	{
		m_pEdit->SetString( "1" );
//		m_pEdit->SetVisible( FALSE );
	}
	else
	{
		TCHAR szNumber[ 64 ];
		_itot( m_pItemElem->m_nItemNum, szNumber, 10 );
		m_pEdit->SetString( szNumber );
	}
	DWORD dwCost = m_pItemElem->GetCost();
	DWORD BuyGold = dwCost / 4;
#ifdef __SHOP_COST_RATE
	BuyGold = static_cast< DWORD >( static_cast< float >( BuyGold ) * prj.m_fShopSellRate );
#endif // __SHOP_COST_RATE
	BuyGold -= ( static_cast<DWORD>(BuyGold * CTax::GetInstance()->GetSalesTaxRate( g_pPlayer )) );

	if( BuyGold < 1 )
		BuyGold = 1;

	BuyGold *= m_pItemElem->m_nItemNum;
//	TCHAR szNumberGold[ 64 ];
	CString szNumberGold;
	szNumberGold.Format("%u", BuyGold);
//	_itot( BuyGold, szNumberGold, 10 );
	m_pStaticGold->SetTitle( szNumberGold );	
	return TRUE;
}
BOOL CWndConfirmSell::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndConfirmSell::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndConfirmSell::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndConfirmSell::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndConfirmSell::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK )
	{

		PLAYSND( SND_INF_TRADE );

		int SellNum = 0;
		if( m_pItemElem->m_nItemNum >= 1 )
		{
			SellNum = atoi( m_pEdit->GetString() );
		}
		if( SellNum != 0 )
		{
			g_DPlay.SendSellItem( (BYTE)( m_pItemElem->m_dwObjId ), SellNum );
		}
	}
	if( nID == WIDC_CANCEL )
	{
	}
	if( nID != WIDC_EDITSELL )
	{
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndConfirmBuy::CWndConfirmBuy() 
{ 
	m_pMover = NULL;
	m_pItemElem = NULL;
	m_dwItemId	= 0;
	m_pEdit = NULL;
	m_pStatic = NULL;
	m_nBuyType = 0;
} 
CWndConfirmBuy::~CWndConfirmBuy() 
{ 
} 

void CWndConfirmBuy::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
} 

BOOL CWndConfirmBuy::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndConfirmBuy::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndConfirmBuy::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 

void CWndConfirmBuy::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

// 살려는 수량이 바뀌면, 가격표시도 변경시킨다.
void CWndConfirmBuy::OnChangeBuyCount( DWORD dwBuy )
{
//	TCHAR szString[ 64 ];
	CString szString;

	if( dwBuy != atoi(m_pEdit->GetString()) )
	{
//		_itot( dwBuy, szString, 10 );	// integer to string
		szString.Format("%u", dwBuy);
		m_pEdit->SetString( szString );
	}

	DWORD dwTotalBuy = 0;
	DWORD dwCost = 0;
	if(m_nBuyType == 0)
	{
		dwCost = m_pItemElem->GetCost();
#ifdef __SHOP_COST_RATE
		dwCost = static_cast< int >( static_cast< float >( dwCost ) * prj.m_fShopBuyRate );
#endif // __SHOP_COST_RATE
		if( CTax::GetInstance()->IsApplyTaxRate( g_pPlayer, m_pItemElem ) )
			dwCost += ( static_cast<DWORD>(dwCost * CTax::GetInstance()->GetPurchaseTaxRate( g_pPlayer )) );
	}
	else if(m_nBuyType == 1)
		dwCost = m_pItemElem->GetChipCost();
	dwTotalBuy = (DWORD)( dwBuy * dwCost * prj.m_fShopCost );
	if( m_pItemElem->m_dwItemId == II_SYS_SYS_SCR_PERIN )
	{
		dwCost = PERIN_VALUE;
		dwTotalBuy = dwBuy * dwCost;
	}

	if(dwTotalBuy > INT_MAX)
	{
		dwBuy--;
		szString.Format("%u", dwBuy);
		m_pEdit->SetString( szString );
		dwTotalBuy = (DWORD)( dwBuy * dwCost * prj.m_fShopCost );
	}
	
	szString.Format("%u", dwTotalBuy);
//	_itot( dwTotalBuy, szString, 10 );	// integer to string
	m_pStaticGold->SetTitle( szString );
}

BOOL CWndConfirmBuy::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	InitDialog( APP_CONFIRM_BUY_, pWndParent, 0, 0 );
	MoveParentCenter();

	if( m_pItemElem )
		m_dwItemId	= m_pItemElem->m_dwItemId;

	CWndStatic* pLabel  = (CWndStatic  *)GetDlgItem( WIDC_STATIC );
//	pLabel->m_dwColor = 0xff000000;
	m_pEdit   = (CWndEdit  *)GetDlgItem( WIDC_CONTROL2 );
	m_pStatic	= (CWndStatic *)GetDlgItem( WIDC_CONTROL1 );
	m_pStaticGold = (CWndStatic *)GetDlgItem( WIDC_STATIC2 );
//	m_pStatic->m_dwColor = m_pStaticGold->m_dwColor = 0xff000000;
	m_pStaticGold->AddWndStyle(WSS_MONEY);
	
	DWORD dwCost;
	if(m_nBuyType == 0)
	{
		dwCost = m_pItemElem->GetCost();
#ifdef __SHOP_COST_RATE
		dwCost = static_cast< int >( static_cast< float >( dwCost ) * prj.m_fShopBuyRate );
#endif // __SHOP_COST_RATE
		if( CTax::GetInstance()->IsApplyTaxRate( g_pPlayer, m_pItemElem ) )
			dwCost += ( static_cast<DWORD>(dwCost * CTax::GetInstance()->GetPurchaseTaxRate( g_pPlayer )) );
	}
	else if(m_nBuyType == 1)
		dwCost = m_pItemElem->GetChipCost();
	if( dwCost == 0 )
	{
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0006) ) ); // "다른 사용자에게 팔렸습니다."
		Destroy();
		return TRUE;
	}

	OnChangeBuyCount( 1 );
	return TRUE;
}

void CWndConfirmBuy::OnDraw( C2DRender* p2DRender ) 
{ 
	DWORD dwCost = m_pItemElem->GetCost();
#ifdef __SHOP_COST_RATE
	dwCost = static_cast< int >( static_cast< float >( dwCost ) * prj.m_fShopBuyRate );
#endif // __SHOP_COST_RATE
	if( CTax::GetInstance()->IsApplyTaxRate( g_pPlayer, m_pItemElem ) )
		dwCost += ( static_cast<DWORD>(dwCost * CTax::GetInstance()->GetPurchaseTaxRate( g_pPlayer )) );
	if( dwCost == 0 )
	{
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0006) ) );  // 다른 사용자에게 팔렸습니다.
		Destroy();
		return;
	}

	return;
} 

const int MAX_BUY_ITEMCOUNT = 99;

BOOL CWndConfirmBuy::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	int nBuyNum = 0;

	switch( nID )
	{
	case WIDC_PLUS:
		nBuyNum = atoi(m_pEdit->GetString());
		++nBuyNum;

		if( m_pItemElem->GetProp()->dwItemKind3 == IK3_BCHARM ||
			m_pItemElem->GetProp()->dwItemKind3 == IK3_RCHARM ||
			m_pItemElem->GetProp()->dwItemKind3 == IK3_ARROW ||
			m_pItemElem->GetProp()->dwID == II_CHP_RED
			)
		{
			if ( nBuyNum > 9999 )
				nBuyNum = 9999;
		}
		else
		{
			if ( nBuyNum > MAX_BUY_ITEMCOUNT )
				nBuyNum = MAX_BUY_ITEMCOUNT;
		}

		OnChangeBuyCount(nBuyNum);
		break;

	case WIDC_MINUS:
		nBuyNum = atoi(m_pEdit->GetString());
		if ( --nBuyNum < 1 )
			nBuyNum = 1;

		OnChangeBuyCount(nBuyNum);
		break;

	case WIDC_MAX:
		{

			if( m_pItemElem->GetProp()->dwItemKind3 == IK3_BCHARM ||
				m_pItemElem->GetProp()->dwItemKind3 == IK3_RCHARM ||
				m_pItemElem->GetProp()->dwItemKind3 == IK3_ARROW ||
				m_pItemElem->GetProp()->dwID == II_CHP_RED
				)
				OnChangeBuyCount( 9999 );
			else if( m_pItemElem->m_dwItemId == II_SYS_SYS_SCR_PERIN )
				OnChangeBuyCount( 21 );
			else
				OnChangeBuyCount( MAX_BUY_ITEMCOUNT );
		}		
		break;

	case WIDC_CONTROL2:
		if( EN_CHANGE == message )
		{
			if( m_pEdit == NULL )
			{
				//ADDERRORMSG( "CWndConfirmBuy::OnChildNotify : m_pEdit == NULL" );
				char szMsg[256];
				sprintf( szMsg, "CWndConfirmBuy::OnChildNotify : more info(%d, %d)", pLResult, GetDlgItem( WIDC_CONTROL2 ) );
				//ADDERRORMSG( szMsg );

				nBuyNum = 1;
			}
			else
				nBuyNum = atoi(m_pEdit->GetString());

			DWORD dwMAXCount = MAX_BUY_ITEMCOUNT;
			if( m_pItemElem->GetProp()->dwItemKind3 == IK3_BCHARM ||
				m_pItemElem->GetProp()->dwItemKind3 == IK3_RCHARM ||
				m_pItemElem->GetProp()->dwItemKind3 == IK3_ARROW ||
				m_pItemElem->GetProp()->dwID == II_CHP_RED
				)
				dwMAXCount = 9999;
			else if( m_pItemElem->m_dwItemId == II_SYS_SYS_SCR_PERIN )
				dwMAXCount = 21;

			nBuyNum = std::clamp( nBuyNum, 0, (int)( dwMAXCount ) );

			OnChangeBuyCount(nBuyNum);
		}
		break;

	case WIDC_CANCEL:
		Destroy();
		break;

	case WIDC_OK:
		OnOK();
		Destroy();
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndConfirmBuy::OnOK()
{
	//아래 메세지 처리 할 것
	DWORD dwCost;
	int nBuy;

	if(m_nBuyType == 1)
	{
		dwCost = m_pItemElem->GetChipCost();
		if( m_pItemElem->m_nItemNum < 1 || dwCost == 0 )
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0006) ) );	// 다른 사용자에게 팔렸습니다.
			return;
		}

		nBuy = atoi( m_pEdit->GetString() );
		if( (int)( (nBuy * dwCost) ) > g_pPlayer->m_Inventory.GetAtItemNum( II_CHP_RED ) )
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_CANNTBUY_REDCHIP) ) );	// 칩이 부족합니다.	
			return;
		}
	}
	else if(m_nBuyType == 0)
	{
		dwCost = m_pItemElem->GetCost();
		if( m_pItemElem->m_dwItemId != II_SYS_SYS_SCR_PERIN )
		{
			if( CTax::GetInstance()->IsApplyTaxRate( g_pPlayer, m_pItemElem ) )
				dwCost += ( static_cast<DWORD>(dwCost * CTax::GetInstance()->GetPurchaseTaxRate( g_pPlayer )) );
		}
		if( m_pItemElem->m_nItemNum < 1 || dwCost == 0 )
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0006) ) );	// 다른 사용자에게 팔렸습니다.
			return;
		}

		nBuy = atoi( m_pEdit->GetString() );
		if( (int)( (nBuy * dwCost) ) > g_pPlayer->GetGold() )
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0009) ) );	// 돈이 부족합니다.	
			return;
		}
	}

	DWORD dwMAXCount = MAX_BUY_ITEMCOUNT;
	if( m_pItemElem->GetProp()->dwItemKind3 == IK3_BCHARM ||
		m_pItemElem->GetProp()->dwItemKind3 == IK3_RCHARM ||
		m_pItemElem->GetProp()->dwItemKind3 == IK3_ARROW ||
		m_pItemElem->GetProp()->dwID == II_CHP_RED
		)
		dwMAXCount = 9999;

	if( nBuy < 1 || nBuy > (int)( dwMAXCount ) )
	{
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0086) ) );	// 상점 거래 중 구입 / 판매 개수가 1 ~ 99 사이가 아닌 수를 입력	
		return;
	}
	if( nBuy > m_pItemElem->m_nItemNum )
	{
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0087) ) );	// 개인 상점 거래 중 구입 하려는 개수가 상점에 판매하는 개수 보다 많거나, 이미 품절	
		return;
	}

	CWndShop* pWndShop = (CWndShop*)GetWndBase( APP_SHOP_ );

	CWndTabCtrl* pTabCtrl	= (CWndTabCtrl*)pWndShop->GetDlgItem( WIDC_INVENTORY );
	CHAR cTab	= (CHAR)pTabCtrl->GetCurSel();
	if(m_nBuyType == 0)
		g_DPlay.SendBuyItem( cTab, (BYTE)( m_pItemElem->m_dwObjId ), nBuy, m_dwItemId );
	else if(m_nBuyType == 1)
		g_DPlay.SendBuyChipItem( cTab, (BYTE)( m_pItemElem->m_dwObjId ), nBuy, m_dwItemId );

	PLAYSND( SND_INF_TRADE );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndItemCtrlVendor::CWndItemCtrlVendor()
{
	m_bVisibleCount = FALSE;
}
CWndItemCtrlVendor::~CWndItemCtrlVendor()
{
}
BOOL CWndItemCtrlVendor::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	if( pShortcut->m_dwShortcut == ShortcutType::Item)
		GetParentWnd()->OnChildNotify( WIN_ITEMDROP, m_nIdWnd, (LRESULT*)pShortcut ); 
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndShop::CWndShop() 
{ 
	m_pMover = NULL;
	m_pWndConfirmSell = NULL;
	m_pWndWarning	= NULL;
	m_bSexSort    = FALSE;
	m_bLevelSort  = FALSE;
} 
CWndShop::~CWndShop() 
{ 
	SAFE_DELETE( m_pWndConfirmSell );
	SAFE_DELETE( m_pWndWarning );
	SAFE_DELETE( g_WndMng.m_pWndTradeGold );
} 
void CWndShop::OnDraw( C2DRender* p2DRender ) 
{ 
	LPCHARACTER lpCharacter = m_pMover->GetCharacter();
	if( lpCharacter == NULL)
		return;

	CString string;

	CWndStatic* pCost = (CWndStatic*) GetDlgItem( WIDC_COST );

	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_INVENTORY );

	WTCITEM item;
	pTabCtrl->GetItem( pTabCtrl->GetCurSel(), &item );
	CWndItemCtrl* pItemCtrl		= (CWndItemCtrl*)item.pWndBase;
	DWORD dwCost	= 0;
	int nIndex	= -1;
	if( pItemCtrl->GetSelectedCount() > 0 )
		nIndex	= pItemCtrl->GetSelectedItem( 0 );
	if( nIndex >= 0 )
	{
		CItemElem * pItemBase	= pItemCtrl->GetItemFromArr( nIndex );
		if( pItemBase && m_pMover )
		{
			LPCHARACTER lpCharacter = m_pMover->GetCharacter();
			if(lpCharacter)
			{
				if(lpCharacter->m_vendor.m_type == CVendor::Type::Penya)
				{
					dwCost += pItemBase->GetCost();
					dwCost = static_cast< int >( static_cast< float >( dwCost ) * prj.m_fShopBuyRate );
					if(CTax::GetInstance()->IsApplyTaxRate( g_pPlayer, pItemBase ))
						dwCost += ( static_cast<DWORD>(dwCost * CTax::GetInstance()->GetPurchaseTaxRate( g_pPlayer )) );
				} else if (lpCharacter->m_vendor.m_type == CVendor::Type::RedChip) {
					dwCost += pItemBase->GetChipCost();
				}
			}
		}
	}

	dwCost = (int)((float)dwCost * prj.m_fShopCost );

	if( nIndex >= 0 )
	{
		CItemElem * pItemBase	= pItemCtrl->GetItemFromArr( nIndex );
		if( pItemBase && pItemBase->m_dwItemId == II_SYS_SYS_SCR_PERIN )
			dwCost = PERIN_VALUE;
	}

	string.Format( _T( "%d" ), dwCost );

	pCost->SetTitle( string );
} 
void CWndShop::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	if(GetWndBase( APP_BAG_EX )) GetWndBase( APP_BAG_EX )->Destroy();
	if( g_WndMng.m_pWndTrade || g_WndMng.m_pWndBank || g_WndMng.m_pWndGuildBank || g_WndMng.GetWndVendorBase() )
	{
		Destroy();
		return;
	}

	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_INVENTORY );
	WTCITEM tabTabItem;
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;

	LPCHARACTER lpCharacter = m_pMover->GetCharacter();

	if( lpCharacter )
	{
		int i = NULL;
		for( ; i < MAX_VENDOR_INVENTORY_TAB; i++ )
		{
			if( lpCharacter->m_vendor.m_venderSlot[ i ].IsEmpty() == FALSE )
			{
				m_wndItemCtrl[ i ].Create( WLVS_ICON, CRect( 0, 0, 250, 250 ), pTabCtrl, i + 10 );
				m_wndItemCtrl[ i ].InitItem( m_pMover->m_ShopInventory[ i ], APP_SHOP_ );
				tabTabItem.pszText = lpCharacter->m_vendor.m_venderSlot[ i ].LockBuffer();
				lpCharacter->m_vendor.m_venderSlot[ i ].UnlockBuffer();
				tabTabItem.pWndBase = &m_wndItemCtrl[ i ];
				pTabCtrl->InsertItem( i, &tabTabItem );
			}
		}
		for( i = pTabCtrl->GetSize(); i < 3; i++ )
		{
			tabTabItem.pszText = "";
			tabTabItem.pWndBase = NULL;
			pTabCtrl->InsertItem( i, &tabTabItem );
		}
	}
	
	CWndInventory* pWndInventory = (CWndInventory*)GetWndBase( APP_INVENTORY );
	CRect rectInventory = pWndInventory->GetWindowRect( TRUE );
	CPoint ptInventory = rectInventory.TopLeft();
	CPoint ptMove;
	
	CRect rect = GetWindowRect( TRUE );
	
	if( ptInventory.x > rect.Width() / 2 )
		ptMove = ptInventory - CPoint( rect.Width(), 0 );
	else
		ptMove = ptInventory + CPoint( rectInventory.Width(), 0 );
	
	Move( ptMove );
} 

BOOL CWndShop::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	return InitDialog( APP_SHOP_, pWndParent, 0, 0 );
} 

BOOL CWndShop::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndShop::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndShop::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 

void CWndShop::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

BOOL CWndShop::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL	bWarning	= FALSE;

	if( message == WIN_ITEMDROP )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT)pLResult;
		CWndBase* pWndFrame = lpShortcut->m_pFromWnd->GetFrameWnd();
		
		BOOL bForbid = TRUE;
		if( lpShortcut->m_dwData != 0 )
		{
			if( nID == 10 || nID == 11 || nID == 12 || nID == 13) // item
			{
				BOOL bResult = TRUE;
				if( ( (CItemElem*)lpShortcut->m_dwData )->IsQuest() )
				{
					bResult = FALSE;
				}

				if( bResult && pWndFrame->GetWndId() == APP_INVENTORY )
				{
					CItemElem *pItemElem = (CItemElem*)lpShortcut->m_dwData;
					if( pItemElem )
					{
						ItemProp *pProp = pItemElem->GetProp();
						if( pProp )
						{
							if( pItemElem->IsCharged() )
							{
								// 090527 김창섭 - 유료지역입장권 사용 후 사용기간이 남아있는 상태에서 NPC를 통해 상점에 팔 경우 경고창이 뜨지않는 현상 수정
								//if( !pItemElem->m_dwKeepTime )
								bWarning	= TRUE;
							}
						} else
						{
							LPCTSTR szErr = Error( "CWndShop::OnChildNotify : pProp==NULL %d", pItemElem->m_dwItemId );
							//ADDERRORMSG( szErr );
						}
					} else
					{
						LPCTSTR szErr = Error( "CWndShop::OnChildNotify : pItemElem==NULL %d", pItemElem->m_dwItemId );
						//ADDERRORMSG( szErr );
					}

					if( FALSE == g_pPlayer->m_Inventory.IsEquip( ( (CItemElem*)lpShortcut->m_dwData)->m_dwObjId ) )
					{
						if( bWarning )
						{
							SAFE_DELETE( m_pWndWarning );
							SAFE_DELETE( m_pWndConfirmSell );
							m_pWndWarning	= new CWndWarning;
							m_pWndWarning->m_pItemElem	= (CItemElem*)lpShortcut->m_dwData;
							m_pWndWarning->m_pMover = m_pMover;
							m_pWndWarning->Initialize( this, APP_WARNING );
						}
						else
						{
							SAFE_DELETE( m_pWndWarning );
							SAFE_DELETE( m_pWndConfirmSell );
							m_pWndConfirmSell = new CWndConfirmSell;
							m_pWndConfirmSell->m_pItemElem = (CItemElem*)lpShortcut->m_dwData;
							m_pWndConfirmSell->m_pMover = m_pMover;
							m_pWndConfirmSell->Initialize( this, APP_CONFIRM_SELL );
						}

						bForbid = FALSE;
					}
					else
					{
						g_WndMng.PutString(TID_GAME_EQUIPTRADE);
					}
				}
			}
		}
		SetForbid( bForbid );
	}

	switch( nID )
	{
	case WIDC_CHECK_LEVEL:
		{
			m_bLevelSort  = !m_bLevelSort;
		}
		break;
	case WIDC_CHECK2:
		{
			m_bSexSort    = !m_bSexSort;
		}
		break;
	}
	
	

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndShop::OnDestroyChildWnd( CWndBase* pWndChild )
{
	if( pWndChild == m_pWndConfirmSell )
		SAFE_DELETE( m_pWndConfirmSell );
}

void CWndShop::OnDestroy( void )
{
	g_pPlayer->m_vtInfo.SetOther( NULL );
	g_DPlay.SendCloseShopWnd();
	CWndInventory* pWndInventory = (CWndInventory*)g_WndMng.GetWndBase( APP_INVENTORY );
	if( pWndInventory )
	{
		SAFE_DELETE( pWndInventory->m_pWndConfirmBuy );
	}
	SAFE_DELETE( m_pWndConfirmSell );
}



//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndBeautyShop::CWndBeautyShop() 
{ 
//	SetPutRegInfo( FALSE );
	m_pWndConfirmSell = NULL;
	m_pModel          = NULL;
	m_dwHairMesh      = 1;
	memset( m_ColorRect, 0, sizeof(CRect)*3 );
	memset( m_fColor, 0, sizeof(FLOAT)*3 );
	m_bLButtonClick   = FALSE;
	m_nHairCost       = 0;
	m_pApplyModel = NULL;
	m_nHairColorCost  = 0;
	
#ifdef __Y_BEAUTY_SHOP_CHARGE
	m_bChange = FALSE;
#endif //__Y_BEAUTY_SHOP_CHARGE

	for(int i=0; i<4; i++)
	{
		m_nHairNum[i] = 0;
	}
	m_pHairModel = NULL;
	m_dwSelectHairMesh = 1;
	m_ChoiceBar = -1;
	m_pWndBeautyShopConfirm = NULL;
	
#ifdef __NEWYEARDAY_EVENT_COUPON
	m_bUseCoupon = FALSE;
	m_pWndUseCouponConfirm = NULL;
#endif //__NEWYEARDAY_EVENT_COUPON
} 

CWndBeautyShop::~CWndBeautyShop() 
{ 
//	m_Texture.DeleteDeviceObjects();
	SAFE_DELETE( m_pModel );
	SAFE_DELETE( m_pWndConfirmSell );
	SAFE_DELETE(m_pApplyModel);
	SAFE_DELETE(m_pHairModel);
	SAFE_DELETE(m_pWndBeautyShopConfirm);
#ifdef __NEWYEARDAY_EVENT_COUPON
	SAFE_DELETE(m_pWndUseCouponConfirm);
#endif //__NEWYEARDAY_EVENT_COUPON
} 

#ifdef __NEWYEARDAY_EVENT_COUPON
void CWndBeautyShop::UseHairCoupon(BOOL isUse)
{
	m_bUseCoupon = isUse;
	if(m_bUseCoupon)
	{
		CString title = GetTitle();
		CString addText;
		addText.Format("  %s", prj.GetText( TID_GAME_NOWUSING_COUPON ));
		title = title + addText;
		SetTitle(title);	
	}
}
#endif //__NEWYEARDAY_EVENT_COUPON

HRESULT CWndBeautyShop::InvalidateDeviceObjects()
{
#ifdef __YDEBUG
	m_Texture.Invalidate();
#endif //__YDEBUG
	
	CWndBase::InvalidateDeviceObjects();
	return S_OK;
}
HRESULT CWndBeautyShop::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	return InvalidateDeviceObjects();
}
HRESULT CWndBeautyShop::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();

	return S_OK;
}

void CWndBeautyShop::OnDraw( C2DRender* p2DRender ) 
{ 
	if( g_pPlayer == NULL || m_pModel == NULL || m_pApplyModel == NULL )
		return;

	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

	pd3dDevice->SetRenderState( D3DRS_AMBIENT,  D3DCOLOR_ARGB( 255, 255,255,255) );
	
	CRect rect = GetClientRect();

	// 뷰포트 세팅 
	D3DVIEWPORT9 viewport;

	// 월드 
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRot;
	D3DXMATRIXA16 matTrans;

	// 카메라 
	D3DXVECTOR3 vecLookAt( 0.0f, 0.0f, 3.0f );
	D3DXVECTOR3 vecPos(  0.0f, 0.7f, -3.5f );
	
	const D3DXMATRIX matView = D3DXR::LookAtLH010(vecPos, vecLookAt);
	
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
#ifdef __YENV
	D3DXVECTOR3 vDir( 0.0f, 0.0f, 1.0f );
	SetLightVec( vDir );
#endif //__YENV
	
	// 왼쪽 원본 모델 랜더링
	{
		LPWNDCTRL lpFace = GetWndCtrl( WIDC_CUSTOM5 );

		viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;//2;
		viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;//5;
		viewport.Width  = lpFace->rect.Width();//p2DRender->m_clipRect.Width();
		viewport.Height = lpFace->rect.Height();// - 10;//p2DRender->m_clipRect.Height();

		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;
		pd3dDevice->SetViewport(&viewport);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;

		D3DXMATRIX matProj;
		D3DXMatrixIdentity( &matProj );
		FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;
/*		
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4.0f, fAspect, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
*/		
		FLOAT fov = D3DX_PI/4.0f;//796.0f;
		FLOAT h = cos(fov/2) / sin(fov/2);
		FLOAT w = h * fAspect;
		D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
		
	    D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matWorld);
		
		D3DXMatrixScaling(&matScale, 4.5f, 4.5f, 4.5f);
		
		if( g_pPlayer->GetSex() == SEX_MALE )
			D3DXMatrixTranslation(&matTrans,0.0f,-5.6f,0.0f);
		else
			D3DXMatrixTranslation(&matTrans,0.0f,-5.2f,0.0f);

		matWorld = matWorld * matScale * matTrans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		// 랜더링 
		pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );//m_bViewLight );
		
		::SetLight( FALSE );
		::SetFog( FALSE );
		SetDiffuse( 1.0f, 1.0f, 1.0f );
		SetAmbient( 1.0f, 1.0f, 1.0f );

		m_pModel->GetObject3D(PARTS_HAIR)->m_fAmbient[0] = g_pPlayer->m_fHairColorR;
		m_pModel->GetObject3D(PARTS_HAIR)->m_fAmbient[1] = g_pPlayer->m_fHairColorG;
		m_pModel->GetObject3D(PARTS_HAIR)->m_fAmbient[2] = g_pPlayer->m_fHairColorB;

		D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 1.0f );
#ifdef __YENV
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvFog, &vConst );
#else //__YENV						
		pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
#endif //__YENV
		::SetTransformView( matView );
		::SetTransformProj( matProj );
	
		if( g_pPlayer )
			g_pPlayer->OverCoatItemRenderCheck(m_pModel);

		m_pModel->Render( p2DRender->m_pd3dDevice, &matWorld );
	}
	// 오른쪽 색입힌 모델 랜더링
	{
		LPWNDCTRL lpFace = GetWndCtrl( WIDC_CUSTOM6 );

		viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;//2;
		viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;//5;
		viewport.Width  = lpFace->rect.Width();//p2DRender->m_clipRect.Width();
		viewport.Height = lpFace->rect.Height();// - 10;//p2DRender->m_clipRect.Height();
		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;
		pd3dDevice->SetViewport(&viewport);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;

		D3DXMATRIX matProj;
		D3DXMatrixIdentity( &matProj );
		FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;
/*
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4.0f, fAspect, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
*/		
		FLOAT fov = D3DX_PI/4.0f;//796.0f;
		FLOAT h = cos(fov/2) / sin(fov/2);
		FLOAT w = h * fAspect;
		D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixRotationY(&matRot,g_tmCurrent/1000.0f);
		D3DXMatrixScaling(&matScale, 4.5f, 4.5f, 4.5f);
		
		if( g_pPlayer->GetSex() == SEX_MALE )
			D3DXMatrixTranslation(&matTrans,0.0f,-5.6f,0.0f);
		else
			D3DXMatrixTranslation(&matTrans,0.0f,-5.2f,0.0f);
		
		matWorld = matWorld * matScale * matRot * matTrans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		
		::SetLight( FALSE );
		::SetFog( FALSE );
		m_pApplyModel->GetObject3D(PARTS_HAIR)->m_fAmbient[0] = m_fColor[0];
		m_pApplyModel->GetObject3D(PARTS_HAIR)->m_fAmbient[1] = m_fColor[1];
		m_pApplyModel->GetObject3D(PARTS_HAIR)->m_fAmbient[2] = m_fColor[2];
		D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 1.0f );
#ifdef __YENV
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvFog, &vConst );
#else //__YENV						
		pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
#endif //__YENV

		if( g_pPlayer )
			g_pPlayer->OverCoatItemRenderCheck(m_pApplyModel);

		::SetTransformView( matView );
		::SetTransformProj( matProj );

		
		m_pApplyModel->Render( p2DRender->m_pd3dDevice, &matWorld );
		
		m_pApplyModel->GetObject3D(PARTS_HAIR)->m_fAmbient[0] = 1.0f;
		m_pApplyModel->GetObject3D(PARTS_HAIR)->m_fAmbient[1] = 1.0f;
		m_pApplyModel->GetObject3D(PARTS_HAIR)->m_fAmbient[2] = 1.0f;
	}
	DrawHairKind(p2DRender, matView);
	viewport.X      = p2DRender->m_ptOrigin.x;// + 5;
	viewport.Y      = p2DRender->m_ptOrigin.y;// + 5;
	viewport.Width  = p2DRender->m_clipRect.Width();
	viewport.Height = p2DRender->m_clipRect.Height();
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;
	pd3dDevice->SetViewport(&viewport);
	
	for( int i=0; i<3; i++ )
	{
		CPoint pt = CPoint( m_ColorScrollBar[i].x - ( m_Texture.m_size.cx / 2 ), m_ColorScrollBar[i].y );

		m_Texture.Render( p2DRender, pt );
	}

	for( int j=0; j<3; j++ )
	{
		if( m_ColorScrollBar[j].x != m_OriginalColorScrollBar[j].x )
			m_Texture.Render( p2DRender, CPoint( m_OriginalColorScrollBar[j].x - ( m_Texture.m_size.cx / 2 ), m_OriginalColorScrollBar[j].y ), 160 );
	}
	
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	// 가격 계산

	BYTE nColorR = (BYTE)( (m_fColor[0] * 255) );
	BYTE nColorG = (BYTE)( (m_fColor[1] * 255) );
	BYTE nColorB = (BYTE)( (m_fColor[2] * 255) );

	BYTE nOrignalR = (BYTE)( g_pPlayer->m_fHairColorR * 255 );
	BYTE nOrignalG = (BYTE)( g_pPlayer->m_fHairColorG * 255 );
	BYTE nOrignalB = (BYTE)( g_pPlayer->m_fHairColorB * 255 );

#ifdef __NEWYEARDAY_EVENT_COUPON
	if( (nColorR != nOrignalR || nColorG != nOrignalG || nColorB != nOrignalB) && !m_bUseCoupon )
#else //__NEWYEARDAY_EVENT_COUPON
	if( nColorR != nOrignalR || nColorG != nOrignalG || nColorB != nOrignalB )
#endif //__NEWYEARDAY_EVENT_COUPON
		m_nHairColorCost = HAIRCOLOR_COST;
	else
		m_nHairColorCost = 0;
	CString string;
	CWndStatic* pCost = (CWndStatic*) GetDlgItem( WIDC_COST );

	if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
	{
		string = "0";
	}
	else
	{
		string.Format( _T( "%d" ), m_nHairCost + m_nHairColorCost );
	}

	pCost->SetTitle( string );

	pCost = (CWndStatic*) GetDlgItem( WIDC_STATIC1 );

	if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
	{
		string = "0";
	}
	else
	{
		string.Format( _T( "%d" ), m_nHairCost );
	}
	
	pCost->SetTitle( string );

	pCost = (CWndStatic*) GetDlgItem( WIDC_STATIC2 );

	if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
	{
		string = "0";
	}
	else
	{
		string.Format( _T( "%d" ), m_nHairColorCost );
	}
	
	pCost->SetTitle( string );
	
} 

void CWndBeautyShop::DrawHairKind(C2DRender* p2DRender, D3DXMATRIX matView)
{
	// 뷰포트 세팅 
	D3DVIEWPORT9 viewport;
	
	// 월드 
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matTrans;
	
	//Hair Kind
	DWORD HairNum = m_dwHairMesh;

	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;
	
	int custom[4] = {WIDC_CUSTOM1, WIDC_CUSTOM2, WIDC_CUSTOM3, WIDC_CUSTOM4};

	if(m_pHairModel == NULL)
		return;

	LPWNDCTRL lpHair = GetWndCtrl( custom[0] );
	viewport.Width  = lpHair->rect.Width() - 2;
	viewport.Height = lpHair->rect.Height() - 2;
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;
	
	D3DXMATRIX matProj;
	D3DXMatrixIdentity( &matProj );
	FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;
	FLOAT fov = D3DX_PI/4.0f;
	FLOAT h = cos(fov/2) / sin(fov/2);
	FLOAT w = h * fAspect;
	D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );

	for(int i=0; i<4; i++)
	{
		( HairNum > MAX_HAIR ) ? HairNum = 1: HairNum;
		m_nHairNum[i] = HairNum;

		lpHair = GetWndCtrl( custom[i] );

		//Model Draw
		CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, HairNum-1, g_pPlayer->m_dwHeadMesh, g_pPlayer->m_aEquipInfo, m_pHairModel, NULL );
		
		viewport.X      = p2DRender->m_ptOrigin.x + lpHair->rect.left;
		viewport.Y      = p2DRender->m_ptOrigin.y + lpHair->rect.top;

		pd3dDevice->SetViewport(&viewport);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;
		
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
		
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 6.0f, 6.0f, 6.0f);
		
		if( g_pPlayer->GetSex() == SEX_MALE )
			D3DXMatrixTranslation(&matTrans,0.05f,-8.0f,-1.0f);
		else
			D3DXMatrixTranslation(&matTrans,0.0f,-7.5f,-1.0f);
		
		matWorld = matWorld * matScale * matTrans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		
		::SetLight( FALSE );
		::SetFog( FALSE );
		SetDiffuse( 1.0f, 1.0f, 1.0f );
		SetAmbient( 1.0f, 1.0f, 1.0f );
		
		m_pHairModel->GetObject3D(PARTS_HAIR)->m_fAmbient[0] = g_pPlayer->m_fHairColorR;
		m_pHairModel->GetObject3D(PARTS_HAIR)->m_fAmbient[1] = g_pPlayer->m_fHairColorG;
		m_pHairModel->GetObject3D(PARTS_HAIR)->m_fAmbient[2] = g_pPlayer->m_fHairColorB;
		
		D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 1.0f );
	#ifdef __YENV
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvFog, &vConst );
	#else //__YENV						
		pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
	#endif //__YENV
		::SetTransformView( matView );
		::SetTransformProj( matProj );
		
		m_pHairModel->Render( p2DRender->m_pd3dDevice, &matWorld );

		//Select Draw
		if(m_dwSelectHairMesh == m_nHairNum[i])
		{
			CRect rect;
			rect = lpHair->rect;
			p2DRender->RenderFillRect( rect, 0x60ffff00 );
		}
		HairNum++;
	}
}

void CWndBeautyShop::UpdateModels()
{
	if(m_pModel != NULL)
		CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pModel, &g_pPlayer->m_Inventory );
	if(m_pApplyModel != NULL)	
		CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, m_dwSelectHairMesh-1, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
}

void CWndBeautyShop::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	CWndInventory* pWndInventory = (CWndInventory*)GetWndBase( APP_INVENTORY );
	CRect rectInventory = pWndInventory->GetWindowRect( TRUE );
	CPoint ptInventory = rectInventory.TopLeft();
	CPoint ptMove;

	CRect rect = GetWindowRect( TRUE );

	if( ptInventory.x > rect.Width() / 2 )
		ptMove = ptInventory - CPoint( rect.Width(), 0 );
	else
		ptMove = ptInventory + CPoint( rectInventory.Width(), 0 );

	Move( ptMove );
	
	LPWNDCTRL lpWndCtrl;
	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM_R );
	rect = lpWndCtrl->rect;
	m_ColorRect[0].left   = rect.left;
	m_ColorRect[0].top    = rect.top + 25;
	m_ColorRect[0].right  = rect.right;
	m_ColorRect[0].bottom = rect.bottom + 25;
	m_ColorRect[0] = rect;

	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM_G );
	rect = lpWndCtrl->rect;
	m_ColorRect[1].left   = rect.left;
	m_ColorRect[1].top    = rect.top + 25;
	m_ColorRect[1].right  = rect.right;
	m_ColorRect[1].bottom = rect.bottom + 25;
	m_ColorRect[1] = rect;
	
	lpWndCtrl = GetWndCtrl( WIDC_CUSTOM_B );
	rect = lpWndCtrl->rect;
	m_ColorRect[2].left   = rect.left;
	m_ColorRect[2].top    = rect.top + 25;
	m_ColorRect[2].right  = rect.right;
	m_ColorRect[2].bottom = rect.bottom + 25;
	m_ColorRect[2] = rect;
	
	ReSetBar( m_fColor[0], m_fColor[1], m_fColor[2] );
	
	m_OriginalColorScrollBar[0] = m_ColorScrollBar[0];
	m_OriginalColorScrollBar[1] = m_ColorScrollBar[1];
	m_OriginalColorScrollBar[2] = m_ColorScrollBar[2];

	m_pRGBEdit[0] = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	m_pRGBEdit[1] = (CWndEdit*)GetDlgItem( WIDC_EDIT2 );
	m_pRGBEdit[2] = (CWndEdit*)GetDlgItem( WIDC_EDIT3 );

	SetRGBToEdit(m_fColor[0], 0);
	SetRGBToEdit(m_fColor[1], 1);
	SetRGBToEdit(m_fColor[2], 2);

	CWndStatic* kindcost = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CWndStatic* colorcost = (CWndStatic*)GetDlgItem( WIDC_STATIC2 );
	CWndStatic* totalcost = (CWndStatic*)GetDlgItem( WIDC_COST );
	kindcost->AddWndStyle(WSS_MONEY);
	colorcost->AddWndStyle(WSS_MONEY);
	totalcost->AddWndStyle(WSS_MONEY);
} 

BOOL CWndBeautyShop::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	if( g_pPlayer == NULL )
		return FALSE;

	m_bLButtonClick = FALSE;
	m_dwHairMesh = g_pPlayer->m_dwHairMesh+1;

	SAFE_DELETE( m_pModel );
	
	int nMover = (g_pPlayer->GetSex() == SEX_MALE ? MI_MALE : MI_FEMALE);
	m_pModel = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pModel,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pModel, &g_pPlayer->m_Inventory );
	
	m_pModel->InitDeviceObjects( g_Neuz.GetDevice() );
	SAFE_DELETE( m_pApplyModel );
	m_pApplyModel = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pApplyModel,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
	m_pApplyModel->InitDeviceObjects( g_Neuz.GetDevice() );

	///
	m_fColor[0] = g_pPlayer->m_fHairColorR;
	m_fColor[1] = g_pPlayer->m_fHairColorG;
	m_fColor[2] = g_pPlayer->m_fHairColorB;

	m_dwSelectHairMesh = m_dwHairMesh;

	SAFE_DELETE(m_pHairModel);
	m_pHairModel = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pHairModel,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pHairModel, &g_pPlayer->m_Inventory );
	m_pHairModel->InitDeviceObjects( g_Neuz.GetDevice() );

	m_nHairCost = 0;
	m_nHairColorCost = 0;	
	m_Texture.LoadTexture( g_Neuz.GetDevice(), MakePath( DIR_THEME, "yellowbuttten.tga" ), 0xffff00ff, TRUE );

	return InitDialog( APP_BEAUTY_SHOP_EX, pWndParent, 0, 0 );
}

void CWndBeautyShop::SetRGBToEdit(float color, int editnum)
{
	char szNumberbuf[8] = {0, };
#ifdef __Y_HAIR_BUG_FIX
	float colorval = (color / 1.0f) * 255;
#else //__Y_HAIR_BUG_FIX
	float colorval = ((color-0.3f)/(1.0f - 0.3f)) * 255 + 0.5f;
#endif //__Y_HAIR_BUG_FIX
	_itot( (int)( colorval ), szNumberbuf, 10 );
	m_pRGBEdit[editnum]->SetString(szNumberbuf);
}

void CWndBeautyShop::SetRGBToBar(int editnum)
{
	float RGBNum;
	int ColorNum;
	CWndEdit* pWndEdit;
	
	if(editnum == WIDC_EDIT1)
	{
		ColorNum = 0;
		pWndEdit = m_pRGBEdit[0];
	}
	else if(editnum == WIDC_EDIT2)
	{
		ColorNum = 1;
		pWndEdit = m_pRGBEdit[1];
	}
	else if(editnum == WIDC_EDIT3)
	{
		ColorNum = 2;
		pWndEdit = m_pRGBEdit[2];
	}

	RGBNum = (float)( atoi(pWndEdit->GetString()) );

	if(RGBNum < 0)
	{
		RGBNum = 0;
		pWndEdit->SetString("0");
	}
#ifdef __Y_HAIR_BUG_FIX
	else if(RGBNum > 255)
	{
		RGBNum = 255;
		pWndEdit->SetString("255");
	}	
#else //__Y_HAIR_BUG_FIX
	else if(RGBNum > 254)
	{
		RGBNum = 254;
		pWndEdit->SetString("254");
	}	
#endif //__Y_HAIR_BUG_FIX

#ifdef __Y_HAIR_BUG_FIX
	m_fColor[ColorNum] = (RGBNum / 255) * 1.0f;
#else //__Y_HAIR_BUG_FIX
	m_fColor[ColorNum] = (RGBNum / 255) * (1.0f - 0.3f) + 0.3f;
#endif //__Y_HAIR_BUG_FIX
	ReSetBar( m_fColor[0], m_fColor[1], m_fColor[2] );
}

void CWndBeautyShop::ReSetBar( FLOAT r, FLOAT g, FLOAT b )
{
#ifdef __Y_HAIR_BUG_FIX
	FLOAT fR = (r/1.0f) * 100.0f;
	FLOAT fG = (g/1.0f) * 100.0f;
	FLOAT fB = (b/1.0f) * 100.0f;
#else //__Y_HAIR_BUG_FIX
	FLOAT fR = ((r-0.3f)/(1.0f - 0.3f)) * 100.0f;
	FLOAT fG = ((g-0.3f)/(1.0f - 0.3f)) * 100.0f;
	FLOAT fB = ((b-0.3f)/(1.0f - 0.3f)) * 100.0f;
#endif //__Y_HAIR_BUG_FIX

	m_ColorScrollBar[0].x = (LONG)( (((m_ColorRect[0].right-m_ColorRect[0].left) * fR) / 100.0f) + m_ColorRect[0].left );
	m_ColorScrollBar[0].y = m_ColorRect[0].top;
	m_ColorScrollBar[1].x = (LONG)( (((m_ColorRect[1].right-m_ColorRect[1].left) * fG) / 100.0f) + m_ColorRect[1].left );
	m_ColorScrollBar[1].y = m_ColorRect[1].top;
	m_ColorScrollBar[2].x = (LONG)( (((m_ColorRect[2].right-m_ColorRect[2].left) * fB) / 100.0f) + m_ColorRect[2].left );
	m_ColorScrollBar[2].y = m_ColorRect[2].top;
}

void CWndBeautyShop::OnMouseWndSurface( CPoint point )
{
	if( g_pPlayer == NULL )
		return;
	CRect rect = CRect( 44, 198, 186, 398 );
	if( !rect.PtInRect( point ) )
		m_bLButtonClick = FALSE;

	if( m_ChoiceBar != -1 && m_bLButtonClick )
	{
		CRect DrawRect = m_ColorRect[m_ChoiceBar];
		point.x = (point.x > DrawRect.right) ? DrawRect.right : point.x;
		
		LONG Width = DrawRect.right - DrawRect.left;
		LONG Pos   = point.x - DrawRect.left;
		
		FLOAT p = ((FLOAT)((FLOAT)Pos / (FLOAT)Width));
			
#ifdef __Y_HAIR_BUG_FIX
		D3DXVECTOR2 vec1= D3DXVECTOR2( 0.0f, 1.0f );
		D3DXVECTOR2 vec2= D3DXVECTOR2( 1.0f, 1.0f );
#else //__Y_HAIR_BUG_FIX
		D3DXVECTOR2 vec1= D3DXVECTOR2( 0.3f, 0.998f );
		D3DXVECTOR2 vec2= D3DXVECTOR2( 0.998f, 0.998f );
#endif //__Y_HAIR_BUG_FIX
		D3DXVECTOR2 vec3;
			
		D3DXVec2Lerp( &vec3, &vec1, &vec2, p );
		
		m_fColor[m_ChoiceBar] = vec3.x;
			
		m_ColorScrollBar[m_ChoiceBar].x = point.x;
			
#ifdef __Y_BEAUTY_SHOP_CHARGE
		if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
			m_bChange = TRUE;
#endif //__Y_BEAUTY_SHOP_CHARGE	
		SetRGBToEdit(m_fColor[m_ChoiceBar], m_ChoiceBar);
	}
}
	
BOOL CWndBeautyShop::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndBeautyShop::OnSize( UINT nType, int cx, int cy )
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndBeautyShop::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	m_ChoiceBar = -1;
	m_bLButtonClick = FALSE;
} 

void CWndBeautyShop::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	int i;
	int custom[4] = {WIDC_CUSTOM1, WIDC_CUSTOM2, WIDC_CUSTOM3, WIDC_CUSTOM4};
	LPWNDCTRL lpWndCtrl;

	for( i=0; i<3; i++ )
	{
		CRect DrawRect = m_ColorRect[i];
		if(DrawRect.PtInRect( point ))
			m_ChoiceBar = i;
	}
	for( i=0; i<4; i++ )
	{
		lpWndCtrl = GetWndCtrl( custom[i] );
		CRect DrawRect = lpWndCtrl->rect;
		if(DrawRect.PtInRect( point ))
		{
			//Hair 선택..
			m_dwSelectHairMesh = m_nHairNum[i];
			CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, m_dwSelectHairMesh-1, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
			//요금 계산..
#ifdef __NEWYEARDAY_EVENT_COUPON
			if( g_pPlayer->m_dwHairMesh != m_dwSelectHairMesh-1 && !m_bUseCoupon)
#else //__NEWYEARDAY_EVENT_COUPON
			if( g_pPlayer->m_dwHairMesh != m_dwSelectHairMesh-1 )
#endif //__NEWYEARDAY_EVENT_COUPON
			{
				m_nHairCost = HAIR_COST;
#ifdef __Y_BEAUTY_SHOP_CHARGE
			if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
					m_bChange = TRUE;
#endif //__Y_BEAUTY_SHOP_CHARGE						
			}
			else
				m_nHairCost = 0;
		}
	}
	m_bLButtonClick = TRUE;
} 

BOOL CWndBeautyShop::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( g_pPlayer == NULL )
		return FALSE;

	if( message == WNM_CLICKED )
	{
		switch(nID)
		{
			case WIDC_BUTTON1: 
				{
					//m_pModel->DeleteDeviceObjects();

					m_dwHairMesh = g_pPlayer->m_dwHairMesh+1;
					CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, m_dwHairMesh-1, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
					
					m_fColor[0] = g_pPlayer->m_fHairColorR;
					m_fColor[1] = g_pPlayer->m_fHairColorG;
					m_fColor[2] = g_pPlayer->m_fHairColorB;

					m_nHairCost = 0;
					
					ReSetBar( m_fColor[0], m_fColor[1], m_fColor[2] );
					m_nHairColorCost = 0;
					m_dwSelectHairMesh = m_dwHairMesh;
					SetRGBToEdit(m_fColor[0], 0);
					SetRGBToEdit(m_fColor[1], 1);
					SetRGBToEdit(m_fColor[2], 2);
#ifdef __Y_BEAUTY_SHOP_CHARGE
					if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
						m_bChange = FALSE;
#endif //__Y_BEAUTY_SHOP_CHARGE
				}
				break;
			case WIDC_HAIRSTYLE_LEFT: // hair
				{
					m_dwHairMesh--;
					( m_dwHairMesh < 1 ) ? m_dwHairMesh = MAX_HAIR: m_dwHairMesh;
				}
				break;
			case WIDC_HAIRSTYLE_RIGHT: // hair
				{
					m_dwHairMesh++;
					( m_dwHairMesh > MAX_HAIR ) ? m_dwHairMesh = 1: m_dwHairMesh;
				}
				break;
			case WIDC_OK:
				{
#ifdef __NEWYEARDAY_EVENT_COUPON
					BOOL noChange = FALSE;
					BYTE nColorR = (BYTE)( (m_fColor[0] * 255) );
					BYTE nColorG = (BYTE)( (m_fColor[1] * 255) );
					BYTE nColorB = (BYTE)( (m_fColor[2] * 255) );
					
					BYTE nOrignalR = (BYTE)( g_pPlayer->m_fHairColorR * 255 );
					BYTE nOrignalG = (BYTE)( g_pPlayer->m_fHairColorG * 255 );
					BYTE nOrignalB = (BYTE)( g_pPlayer->m_fHairColorB * 255 );
					
					if((g_pPlayer->m_dwHairMesh == m_dwSelectHairMesh-1 ) && (nColorR == nOrignalR && nColorG == nOrignalG && nColorB == nOrignalB))
						noChange = TRUE;
#endif //__NEWYEARDAY_EVENT_COUPON
		#ifdef __Y_BEAUTY_SHOP_CHARGE
					if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK )
					{
						if( m_bChange && g_pPlayer )
						{
							if(m_pWndBeautyShopConfirm == NULL)
							{
								m_pWndBeautyShopConfirm = new CWndBeautyShopConfirm;
								m_pWndBeautyShopConfirm->Initialize(this);
							}
						}
					}
					else
		#endif //__Y_BEAUTY_SHOP_CHARGE												
					{
						if( g_pPlayer )
						{
							int nCost = m_nHairCost + m_nHairColorCost;
							
							if( nCost < 0 )
								nCost = 0;
#ifdef __NEWYEARDAY_EVENT_COUPON
							if(m_bUseCoupon && !noChange)
							{
								if(m_pWndUseCouponConfirm == NULL)
								{									
									m_pWndUseCouponConfirm = new CWndUseCouponConfirm;
									m_pWndUseCouponConfirm->SetInfo(APP_BEAUTY_SHOP_EX, 1);
									m_pWndUseCouponConfirm->Initialize(this);							
								}
							}			
#endif //__NEWYEARDAY_EVENT_COUPON
							if(nCost > 0)
							{
								if(m_pWndBeautyShopConfirm == NULL)
								{
									m_pWndBeautyShopConfirm = new CWndBeautyShopConfirm;
									m_pWndBeautyShopConfirm->Initialize(this);
								}
							}
						}
					}
					int nCost = m_nHairCost + m_nHairColorCost;
#ifdef __NEWYEARDAY_EVENT_COUPON
					if(nCost <= 0 && (!m_bUseCoupon || noChange))
#else //__NEWYEARDAY_EVENT_COUPON
					if(nCost <= 0)
#endif //__NEWYEARDAY_EVENT_COUPON
						Destroy();
				}
				break;
			case WIDC_CANCEL:
				{
					Destroy();
				}
				break;
		}
	}
	if(nID == WIDC_EDIT1 || nID == WIDC_EDIT2 || nID == WIDC_EDIT3)
		SetRGBToBar(nID);

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndBeautyShop::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

void CWndBeautyShop::OnDestroy( void )
{
	SAFE_DELETE( m_pModel );
	SAFE_DELETE( m_pWndConfirmSell );
	SAFE_DELETE(m_pApplyModel);
	SAFE_DELETE(m_pHairModel);
	SAFE_DELETE(m_pWndBeautyShopConfirm);
}

#ifdef __NEWYEARDAY_EVENT_COUPON
CWndUseCouponConfirm::CWndUseCouponConfirm() 
{
	m_bUseCoupon = FALSE;
	m_checkClose = TRUE;
	m_TargetWndId = -1;
}
 
CWndUseCouponConfirm::~CWndUseCouponConfirm() 
{ 
} 

void CWndUseCouponConfirm::OnDestroy()
{
	if( m_bUseCoupon == FALSE )
		return;

	if(!m_checkClose && m_MainFlag == 0)
	{
		if(m_TargetWndId != -1)
		{
			if(m_TargetWndId == APP_BEAUTY_SHOP_EX)
			{
				g_WndMng.CreateApplet( APP_INVENTORY );			
				SAFE_DELETE( g_WndMng.m_pWndBeautyShop );
				g_WndMng.m_pWndBeautyShop = new CWndBeautyShop;					
				g_WndMng.m_pWndBeautyShop->Initialize( NULL, APP_BEAUTY_SHOP_EX );
				g_WndMng.m_pWndBeautyShop->UseHairCoupon(m_bUseCoupon);
			}
			else if(m_TargetWndId == APP_BEAUTY_SHOP_SKIN)
			{
				g_WndMng.CreateApplet( APP_INVENTORY );			
				SAFE_DELETE( g_WndMng.m_pWndFaceShop );
				g_WndMng.m_pWndFaceShop = new CWndFaceShop;										
				g_WndMng.m_pWndFaceShop->Initialize( NULL, APP_BEAUTY_SHOP_EX );
				g_WndMng.m_pWndFaceShop->UseFaceCoupon(m_bUseCoupon);
			}	
		}
	}
	if(this->GetParentWnd() == g_WndMng.GetWndBase( APP_WORLD ))
		g_WndMng.m_pWndUseCouponConfirm = NULL;	
	else if(this->GetParentWnd() == g_WndMng.GetWndBase( APP_BEAUTY_SHOP_EX ))
		g_WndMng.m_pWndBeautyShop->m_pWndUseCouponConfirm = NULL;
	else if(this->GetParentWnd() == g_WndMng.GetWndBase( APP_BEAUTY_SHOP_SKIN ))
		g_WndMng.m_pWndFaceShop->m_pWndUseCouponConfirm = NULL;
}

void CWndUseCouponConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndUseCouponConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요
/*	CWndStatic* pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	pStatic->SetVisible(FALSE);
	pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC2 );
	pStatic->SetVisible(FALSE);
	pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC3 );
	pStatic->SetVisible(FALSE);
	pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC4 );
	pStatic->SetVisible(FALSE);
	pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC5 );
	pStatic->SetVisible(FALSE);
	pStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC6 );
	pStatic->SetVisible(FALSE);
*/	
	CWndText* pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	SetTitle(prj.GetText(TID_GAME_USE_CHANGE_COUPON));

	if(m_TargetWndId != -1)
	{
		if(m_MainFlag == 0)
			pText->AddString(prj.GetText( TID_GAME_ASKUSE_COUPON1 ));
		else if(m_MainFlag == 1)
		{
			pText->AddString(prj.GetText( TID_GAME_ASKUSE_COUPON2 ));
			pText->AddString("\n");
			pText->AddString(prj.GetText( TID_GAME_WARNNING_USE_COUPON ), 0xffff0000);
		}
	}
	
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndUseCouponConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( APP_BEAUTY_SHOP_EX_CONFIRM, pWndParent, 0, CPoint( 0, 0 ) );
} 
BOOL CWndUseCouponConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndUseCouponConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndUseCouponConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndUseCouponConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndUseCouponConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_YES )
	{
		if(m_MainFlag == 0)
			m_bUseCoupon = TRUE;
		else if(m_MainFlag == 1)
		{
			if(m_TargetWndId == APP_BEAUTY_SHOP_EX)
			{
				CWndBeautyShop* pWndBeautyShop = (CWndBeautyShop*)this->GetParentWnd();
				g_DPlay.SendSetHair( (BYTE)( pWndBeautyShop->m_dwSelectHairMesh-1 ), pWndBeautyShop->m_fColor[0], pWndBeautyShop->m_fColor[1], pWndBeautyShop->m_fColor[2] );
				pWndBeautyShop->Destroy();
			}
			else if(m_TargetWndId == APP_BEAUTY_SHOP_SKIN)
			{
				CWndFaceShop* pWndFaceShop = (CWndFaceShop*)this->GetParentWnd();
				g_DPlay.SendChangeFace(pWndFaceShop->m_nSelectedFace - 1);
				pWndFaceShop->Destroy();
			}
		}	
		m_checkClose = FALSE;
	}
	else if( nID == WIDC_NO )
		m_checkClose = FALSE;
	else if( nID == WTBID_CLOSE )
		m_checkClose = TRUE;

	Destroy();

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndUseCouponConfirm::SetInfo(DWORD targetWndId, int flag)
{
	m_TargetWndId = targetWndId;
	m_MainFlag = flag;
}
#endif //__NEWYEARDAY_EVENT_COUPON
/*************************
CWndBeautyShopConfirm Class
*************************/
CWndBeautyShopConfirm::CWndBeautyShopConfirm() 
{ 
	m_ParentId = 0;
}
 
CWndBeautyShopConfirm::~CWndBeautyShopConfirm() 
{ 
} 

void CWndBeautyShopConfirm::OnDestroy()
{
	CWndBase* pWndBase = this->GetParentWnd();
	if(m_ParentId == APP_BEAUTY_SHOP_EX)
	{
		CWndBeautyShop* pWndBeautyShop = (CWndBeautyShop*)pWndBase;
		pWndBeautyShop->m_pWndBeautyShopConfirm = NULL;
	}
	else if(m_ParentId == APP_BEAUTY_SHOP_SKIN)
	{
		CWndFaceShop* pWndFaceShop = (CWndFaceShop*)pWndBase;
		pWndFaceShop->m_pWndBeautyShopConfirm = NULL;
	}
}

void CWndBeautyShopConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndBeautyShopConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// 여기에 코딩하세요
/*	char szNumberbuf[16] = {0, };
	int TotalCost;
	CWndStatic* pCostStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC6 );
	pCostStatic->AddWndStyle(WSS_MONEY);
	CWndStatic* pStatic1 = (CWndStatic*)GetDlgItem( WIDC_STATIC2 );
	CWndStatic* pStatic2 = (CWndStatic*)GetDlgItem( WIDC_STATIC5 );
	CWndBase* pWndBase = this->GetParentWnd();
	m_ParentId = pWndBase->GetWndApplet()->dwWndId;
	if(m_ParentId == APP_BEAUTY_SHOP_EX)
	{
		CWndBeautyShop* pWndBeautyShop = (CWndBeautyShop*)pWndBase;
		TotalCost = pWndBeautyShop->m_nHairCost + pWndBeautyShop->m_nHairColorCost;
		pStatic1->SetTitle(prj.GetText(TID_GAME_CHANGE_HAIR));
		pStatic2->SetTitle(prj.GetText(TID_GAME_CHANGE_HAIR_QUESTION));
	}
	else if(m_ParentId == APP_BEAUTY_SHOP_SKIN)
	{
		CWndFaceShop* pWndFaceShop = (CWndFaceShop*)pWndBase;
		TotalCost = pWndFaceShop->m_nCost;
		pStatic1->SetTitle(prj.GetText(TID_GAME_CHANGE_FACE));
		pStatic2->SetTitle(prj.GetText(TID_GAME_CHANGE_FACE_QUESTION));
	}
	_itoa( TotalCost, szNumberbuf, 10 );
	pCostStatic->SetTitle(szNumberbuf);
*/
	int TotalCost;
	CString strText, strNum;

	CWndBase* pWndBase = this->GetParentWnd();
	m_ParentId = pWndBase->GetWndApplet()->dwWndId;
	if(m_ParentId == APP_BEAUTY_SHOP_EX)
	{
		CWndBeautyShop* pWndBeautyShop = (CWndBeautyShop*)pWndBase;
		TotalCost = pWndBeautyShop->m_nHairCost + pWndBeautyShop->m_nHairColorCost;
		strNum.Format("%d", TotalCost);
		strText.Format("%s %s %s %s %s %s", prj.GetText(TID_GAME_SHOP_CHOICE), prj.GetText(TID_GAME_CHANGE_HAIR), prj.GetText(TID_GAME_SHOP_COST),
			GetNumberFormatEx(strNum), prj.GetText(TID_GAME_SHOP_PENYA), prj.GetText(TID_GAME_CHANGE_HAIR_QUESTION) );
	}
	else if(m_ParentId == APP_BEAUTY_SHOP_SKIN)
	{
		CWndFaceShop* pWndFaceShop = (CWndFaceShop*)pWndBase;
		TotalCost = pWndFaceShop->m_nCost;
		strNum.Format("%d", TotalCost);
		strText.Format("%s %s %s %s %s %s", prj.GetText(TID_GAME_SHOP_CHOICE), prj.GetText(TID_GAME_CHANGE_FACE), prj.GetText(TID_GAME_SHOP_COST),
			GetNumberFormatEx(strNum), prj.GetText(TID_GAME_SHOP_PENYA), prj.GetText(TID_GAME_CHANGE_FACE_QUESTION) );
	}

	CWndText* pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	pText->SetString(strText);

	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndBeautyShopConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( APP_BEAUTY_SHOP_EX_CONFIRM, pWndParent, 0, CPoint( 0, 0 ) );
} 
BOOL CWndBeautyShopConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndBeautyShopConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndBeautyShopConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndBeautyShopConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndBeautyShopConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_YES )
	{
		if(m_ParentId == APP_BEAUTY_SHOP_EX)
		{
			CWndBeautyShop* pWndBeautyShop = (CWndBeautyShop*)this->GetParentWnd();
			if( pWndBeautyShop )
			{
				g_DPlay.SendSetHair( (BYTE)( pWndBeautyShop->m_dwSelectHairMesh-1 ), pWndBeautyShop->m_fColor[0], pWndBeautyShop->m_fColor[1], pWndBeautyShop->m_fColor[2] );
			}
			pWndBeautyShop->Destroy();
		}
		else if(m_ParentId == APP_BEAUTY_SHOP_SKIN)
		{
			CWndFaceShop* pWndFaceShop = (CWndFaceShop*)this->GetParentWnd();
			if( pWndFaceShop )
			{
				g_DPlay.SendChangeFace(pWndFaceShop->m_nSelectedFace - 1);
			}
			pWndFaceShop->Destroy();
		}
		Destroy();
	}
	else if( nID == WIDC_NO || nID == WTBID_CLOSE )
		Destroy();
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

/*************************
	CWndFaceShop Class
*************************/
CWndFaceShop::CWndFaceShop()
{ 
	m_pMainModel = NULL;
	m_pApplyModel = NULL;
	m_pFriendshipFace = NULL;
	m_pNewFace = NULL;
	m_pWndBeautyShopConfirm = NULL;
	m_nSelectedFace = 1;
	m_dwFriendshipFace = 1;
	m_dwNewFace = 6;
	m_nCost = 0;
	m_ChoiceBar = -1;
#ifdef __NEWYEARDAY_EVENT_COUPON
	m_bUseCoupon = FALSE;
	m_pWndUseCouponConfirm = NULL;
#endif //__NEWYEARDAY_EVENT_COUPON
} 

CWndFaceShop::~CWndFaceShop() 
{ 
	SAFE_DELETE(m_pMainModel);
	SAFE_DELETE(m_pApplyModel);
	SAFE_DELETE(m_pFriendshipFace);
	SAFE_DELETE(m_pNewFace);
	SAFE_DELETE(m_pWndBeautyShopConfirm);
#ifdef __NEWYEARDAY_EVENT_COUPON
	SAFE_DELETE(m_pWndUseCouponConfirm);
#endif //__NEWYEARDAY_EVENT_COUPON
} 

void CWndFaceShop::OnDestroy()
{
	SAFE_DELETE(m_pMainModel);
	SAFE_DELETE(m_pApplyModel);
	SAFE_DELETE(m_pFriendshipFace);
	SAFE_DELETE(m_pNewFace);
	SAFE_DELETE(m_pWndBeautyShopConfirm);
}

void CWndFaceShop::OnDestroyChildWnd( CWndBase* pWndChild )
{
}

#ifdef __NEWYEARDAY_EVENT_COUPON
void CWndFaceShop::UseFaceCoupon(BOOL isUse)
{
	m_bUseCoupon = isUse;
	if(m_bUseCoupon)
	{
		CString title = GetTitle();
		CString addText;
		addText.Format("  %s", prj.GetText( TID_GAME_NOWUSING_COUPON ));
		title = title + addText;
		SetTitle(title);	
	}		
}
#endif //__NEWYEARDAY_EVENT_COUPON

void CWndFaceShop::OnDraw( C2DRender* p2DRender ) 
{ 
	if( g_pPlayer == NULL || m_pMainModel == NULL || m_pApplyModel == NULL )
		return;

	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;

	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

	pd3dDevice->SetRenderState( D3DRS_AMBIENT,  D3DCOLOR_ARGB( 255, 255,255,255) );
	
	CRect rect = GetClientRect();

	// 뷰포트 세팅 
	D3DVIEWPORT9 viewport;

	// 월드 
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRot;
	D3DXMATRIXA16 matTrans;

	// 카메라 
	D3DXVECTOR3 vecLookAt( 0.0f, 0.0f, 3.0f );
	D3DXVECTOR3 vecPos(  0.0f, 0.7f, -3.5f );
	
	D3DXMATRIX matView = D3DXR::LookAtLH010(vecPos, vecLookAt);
	
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
#ifdef __YENV
	D3DXVECTOR3 vDir( 0.0f, 0.0f, 1.0f );
	SetLightVec( vDir );
#endif //__YENV
	
	// 왼쪽 원본 모델 랜더링
	{
		LPWNDCTRL lpFace = GetWndCtrl( WIDC_CUSTOM5 );

		viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;
		viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;
		viewport.Width  = lpFace->rect.Width();
		viewport.Height = lpFace->rect.Height();

		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;
		pd3dDevice->SetViewport(&viewport);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;

		D3DXMATRIX matProj;
		D3DXMatrixIdentity( &matProj );
		FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;

		FLOAT fov = D3DX_PI/4.0f;//796.0f;
		FLOAT h = cos(fov/2) / sin(fov/2);
		FLOAT w = h * fAspect;
		D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
		
	    D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matWorld);
		
		D3DXMatrixScaling(&matScale, 4.5f, 4.5f, 4.5f);
		
		if( g_pPlayer->GetSex() == SEX_MALE )
			D3DXMatrixTranslation(&matTrans,0.0f,-5.6f,0.0f);
		else
			D3DXMatrixTranslation(&matTrans,0.0f,-5.2f,0.0f);

		matWorld = matWorld * matScale * matTrans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		// 랜더링 
		pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
		pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		
		::SetLight( FALSE );
		::SetFog( FALSE );
		SetDiffuse( 1.0f, 1.0f, 1.0f );
		SetAmbient( 1.0f, 1.0f, 1.0f );

		D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 1.0f );
#ifdef __YENV
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvFog, &vConst );
#else //__YENV						
		pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
#endif //__YENV
		::SetTransformView( matView );
		::SetTransformProj( matProj );
	
		if( g_pPlayer )
			g_pPlayer->OverCoatItemRenderCheck(m_pMainModel);
		
		m_pMainModel->Render( p2DRender->m_pd3dDevice, &matWorld );
	}
	// 오른쪽 얼굴변경 모델 랜더링
	{
		LPWNDCTRL lpFace = GetWndCtrl( WIDC_CUSTOM6 );

		viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;
		viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;
		viewport.Width  = lpFace->rect.Width();
		viewport.Height = lpFace->rect.Height();
		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;
		pd3dDevice->SetViewport(&viewport);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;

		D3DXMATRIX matProj;
		D3DXMatrixIdentity( &matProj );
		FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;

		FLOAT fov = D3DX_PI/4.0f;//796.0f;
		FLOAT h = cos(fov/2) / sin(fov/2);
		FLOAT w = h * fAspect;
		D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixRotationY(&matRot,g_tmCurrent/1000.0f);
		D3DXMatrixScaling(&matScale, 4.5f, 4.5f, 4.5f);
		
		if( g_pPlayer->GetSex() == SEX_MALE )
			D3DXMatrixTranslation(&matTrans,0.0f,-5.6f,0.0f);
		else
			D3DXMatrixTranslation(&matTrans,0.0f,-5.2f,0.0f);
		
		matWorld = matWorld * matScale * matRot * matTrans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		
		::SetLight( FALSE );
		::SetFog( FALSE );

		D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 1.0f );
#ifdef __YENV
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvFog, &vConst );
#else //__YENV						
		pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
#endif //__YENV
		::SetTransformView( matView );
		::SetTransformProj( matProj );

		if( g_pPlayer )
			g_pPlayer->OverCoatItemRenderCheck(m_pApplyModel);

		m_pApplyModel->Render( p2DRender->m_pd3dDevice, &matWorld );
	}
	viewport.X      = p2DRender->m_ptOrigin.x;// + 5;
	viewport.Y      = p2DRender->m_ptOrigin.y;// + 5;
	viewport.Width  = p2DRender->m_clipRect.Width();
	viewport.Height = p2DRender->m_clipRect.Height();
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;
	pd3dDevice->SetViewport(&viewport);

	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	
	DrawFaces(0, p2DRender, matView);
	DrawFaces(1, p2DRender, matView);
} 

void CWndFaceShop::DrawFaces(int ChoiceFlag, C2DRender* p2DRender, D3DXMATRIX matView)
{
	// 뷰포트 세팅 
	D3DVIEWPORT9 viewport;
	
	// 월드 
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matTrans;
	
	//Face Kind
	DWORD FaceNum;
	LPDIRECT3DDEVICE9 pd3dDevice = p2DRender->m_pd3dDevice;
	CModelObject* m_pFaceModel;

	if(ChoiceFlag == 0)
	{
		m_pFaceModel = m_pFriendshipFace;
		FaceNum = m_dwFriendshipFace;
	}
	else if(ChoiceFlag == 1)
	{
		m_pFaceModel = m_pNewFace;
		FaceNum = m_dwNewFace;
	}

	int custom_friend[4] = {WIDC_CUSTOM1, WIDC_CUSTOM2, WIDC_CUSTOM3, WIDC_CUSTOM4};
	int custom_new[4] = {WIDC_CUSTOM7, WIDC_CUSTOM8, WIDC_CUSTOM9, WIDC_CUSTOM10};

	if(m_pFaceModel == NULL)
		return;

	LPWNDCTRL lpFace = GetWndCtrl( custom_friend[0] );
	viewport.Width  = lpFace->rect.Width() - 2;
	viewport.Height = lpFace->rect.Height() - 2;
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;
	
	D3DXMATRIX matProj;
	D3DXMatrixIdentity( &matProj );
	FLOAT fAspect = ((FLOAT)viewport.Width) / (FLOAT)viewport.Height;
	FLOAT fov = D3DX_PI/4.0f;
	FLOAT h = cos(fov/2) / sin(fov/2);
	FLOAT w = h * fAspect;
	D3DXMatrixOrthoLH( &matProj, w, h, CWorld::m_fNearPlane - 0.01f, CWorld::m_fFarPlane );

	for(int i=0; i<4; i++)
	{
		if(ChoiceFlag == 0)
		{
			( FaceNum > MAX_DEFAULT_HEAD ) ? FaceNum = 1: FaceNum;
			lpFace = GetWndCtrl( custom_friend[i] );
			m_nFriendshipFaceNum[i] = FaceNum;
		}
		else if(ChoiceFlag == 1)
		{
			( FaceNum > MAX_HEAD ) ? FaceNum = 6: FaceNum;
			lpFace = GetWndCtrl( custom_new[i] );
			m_nNewFaceNum[i] = FaceNum;
		}

		//Model Draw
		CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, FaceNum-1,g_pPlayer->m_aEquipInfo, m_pFaceModel, NULL );
		
		viewport.X      = p2DRender->m_ptOrigin.x + lpFace->rect.left;
		viewport.Y      = p2DRender->m_ptOrigin.y + lpFace->rect.top;

		pd3dDevice->SetViewport(&viewport);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xffa08080, 1.0f, 0 ) ;
		
		pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
		
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matWorld);
		
		D3DXMatrixScaling(&matScale, 7.5f, 7.5f, 7.5f);
		
		if( g_pPlayer->GetSex() == SEX_MALE )
			D3DXMatrixTranslation(&matTrans,0.05f,-10.0f,-1.0f);
		else
			D3DXMatrixTranslation(&matTrans,0.0f,-9.5f,-1.0f);
		
		matWorld = matWorld * matScale * matTrans;
		pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		
		::SetLight( FALSE );
		::SetFog( FALSE );
		SetDiffuse( 1.0f, 1.0f, 1.0f );
		SetAmbient( 1.0f, 1.0f, 1.0f );
		
		D3DXVECTOR4 vConst( 1.0f, 1.0f, 1.0f, 1.0f );
	#ifdef __YENV
		g_Neuz.m_pEffect->SetVector( g_Neuz.m_hvFog, &vConst );
	#else //__YENV						
		pd3dDevice->SetVertexShaderConstantF( 95, (float*)&vConst, 1 );
	#endif //__YENV
		::SetTransformView( matView );
		::SetTransformProj( matProj );
		
		m_pFaceModel->Render( p2DRender->m_pd3dDevice, &matWorld );

		//Select Draw
		if(ChoiceFlag == 0)
		{
			if(m_nSelectedFace == m_nFriendshipFaceNum[i])
			{
				CRect rect;
				rect = lpFace->rect;
				p2DRender->RenderFillRect( rect, 0x60ffff00 );
			}
		}
		else if(ChoiceFlag == 1)
		{
			if(m_nSelectedFace == m_nNewFaceNum[i])
			{
				CRect rect;
				rect = lpFace->rect;
				p2DRender->RenderFillRect( rect, 0x60ffff00 );
			}
		}
		FaceNum++;
	}
}

void CWndFaceShop::UpdateModels()
{
	if(m_pMainModel != NULL)
		CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pMainModel, &g_pPlayer->m_Inventory );
	if(m_pApplyModel != NULL)	
		CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, m_nSelectedFace-1, g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
}

void CWndFaceShop::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	CWndInventory* pWndInventory = (CWndInventory*)GetWndBase( APP_INVENTORY );
	CRect rectInventory = pWndInventory->GetWindowRect( TRUE );
	CPoint ptInventory = rectInventory.TopLeft();
	CPoint ptMove;
	
	CRect rect = GetWindowRect( TRUE );

	if( ptInventory.x > rect.Width() / 2 )
		ptMove = ptInventory - CPoint( rect.Width(), 0 );
	else
		ptMove = ptInventory + CPoint( rectInventory.Width(), 0 );

	Move( ptMove );
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndFaceShop::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	if( g_pPlayer == NULL )
		return FALSE;

	if(g_pPlayer->m_dwHeadMesh >= 0 && g_pPlayer->m_dwHeadMesh < 6)
		m_dwFriendshipFace = g_pPlayer->m_dwHeadMesh + 1;
	else if(g_pPlayer->m_dwHeadMesh >= 6 && g_pPlayer->m_dwHeadMesh < 15)
		m_dwNewFace = g_pPlayer->m_dwHeadMesh + 1;

	m_nSelectedFace = g_pPlayer->m_dwHeadMesh + 1;	
	
	int nMover = (g_pPlayer->GetSex() == SEX_MALE ? MI_MALE : MI_FEMALE);
	
	SAFE_DELETE( m_pMainModel );
	m_pMainModel = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pMainModel,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pMainModel, &g_pPlayer->m_Inventory );
	m_pMainModel->InitDeviceObjects( g_Neuz.GetDevice() );

	SAFE_DELETE( m_pApplyModel );
	m_pApplyModel = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pApplyModel,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
	m_pApplyModel->InitDeviceObjects( g_Neuz.GetDevice() );	

	SAFE_DELETE(m_pFriendshipFace);
	m_pFriendshipFace = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pFriendshipFace,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pFriendshipFace, &g_pPlayer->m_Inventory );
	m_pFriendshipFace->InitDeviceObjects( g_Neuz.GetDevice() );

	SAFE_DELETE(m_pNewFace);
	m_pNewFace = (CModelObject*)prj.m_modelMng.LoadModel( g_Neuz.m_pd3dDevice, OT_MOVER, nMover, TRUE );
	prj.m_modelMng.LoadMotion( m_pNewFace,  OT_MOVER, nMover, MTI_STAND2 );
	CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, g_pPlayer->m_dwHeadMesh,g_pPlayer->m_aEquipInfo, m_pNewFace, &g_pPlayer->m_Inventory );
	m_pNewFace->InitDeviceObjects( g_Neuz.GetDevice() );
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( APP_BEAUTY_SHOP_SKIN, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndFaceShop::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}
 
void CWndFaceShop::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
}
 
void CWndFaceShop::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
}
 
void CWndFaceShop::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	int i;
	int custom[8] = {WIDC_CUSTOM1, WIDC_CUSTOM2, WIDC_CUSTOM3, WIDC_CUSTOM4,
					WIDC_CUSTOM7, WIDC_CUSTOM8, WIDC_CUSTOM9, WIDC_CUSTOM10};
	LPWNDCTRL lpWndCtrl;
	
	for( i=0; i<8; i++ )
	{
		lpWndCtrl = GetWndCtrl( custom[i] );
		CRect DrawRect = lpWndCtrl->rect;
		if(DrawRect.PtInRect( point ))
		{
			//Face 선택..
			if(i>=0 && i<4)
				m_nSelectedFace = m_nFriendshipFaceNum[i];
			else if(i>=4 && i<8)
				m_nSelectedFace = m_nNewFaceNum[i-4];
			
			CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, m_nSelectedFace-1, g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
			//요금 계산..
#ifdef __NEWYEARDAY_EVENT_COUPON
			if( g_pPlayer->m_dwHeadMesh != m_nSelectedFace-1 && !m_bUseCoupon)
#else //__NEWYEARDAY_EVENT_COUPON
			if( g_pPlayer->m_dwHeadMesh != m_nSelectedFace-1 )
#endif //__NEWYEARDAY_EVENT_COUPON
				m_nCost = CHANGE_FACE_COST;
			else
				m_nCost = 0;
		}
	}
} 

BOOL CWndFaceShop::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( g_pPlayer == NULL )
		return FALSE;

	if( message == WNM_CLICKED )
	{
		switch(nID)
		{
			case WIDC_BTN_RESET: 
				{
					if(g_pPlayer->m_dwHeadMesh >= 0 && g_pPlayer->m_dwHeadMesh < 6)
					{
						m_dwFriendshipFace = g_pPlayer->m_dwHeadMesh + 1;
						m_dwNewFace = 6;
					}
					else if(g_pPlayer->m_dwHeadMesh >= 6 && g_pPlayer->m_dwHeadMesh < 16)
					{
						m_dwNewFace = g_pPlayer->m_dwHeadMesh + 1;
						m_dwFriendshipFace = 1;
					}
					m_nSelectedFace = g_pPlayer->m_dwHeadMesh + 1;
					CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_dwSkinSet, g_pPlayer->m_dwFace, g_pPlayer->m_dwHairMesh, m_nSelectedFace-1,g_pPlayer->m_aEquipInfo, m_pApplyModel, &g_pPlayer->m_Inventory );
					
					m_nCost = 0;
				}
				break;
			case WIDC_FRIENDSHIPFACE_LEFT:
				{
					m_dwFriendshipFace--;
					( m_dwFriendshipFace < 1 ) ? m_dwFriendshipFace = MAX_DEFAULT_HEAD: m_dwFriendshipFace;
				}
				break;
			case WIDC_FRIENDSHIPFACE_RIGHT:
				{
					m_dwFriendshipFace++;
					( m_dwFriendshipFace > MAX_DEFAULT_HEAD ) ? m_dwFriendshipFace = 1: m_dwFriendshipFace;
				}
				break;
			case WIDC_NEWFACE_LEFT:
				{
					m_dwNewFace--;
					( m_dwNewFace < 6 ) ? m_dwNewFace = MAX_HEAD: m_dwNewFace;
				}
				break;
			case WIDC_NEWFACE_RIGHT:
				{
					m_dwNewFace++;
					( m_dwNewFace > MAX_HEAD ) ? m_dwNewFace = 6: m_dwNewFace;
				}
				break;
			case WIDC_BTN_OK:
				{
#ifdef __NEWYEARDAY_EVENT_COUPON
					BOOL noChange = FALSE;
					if(g_pPlayer->m_dwHeadMesh == m_nSelectedFace-1)
						noChange = TRUE;

					if(m_nCost <= 0 && (!m_bUseCoupon || noChange))
#else //__NEWYEARDAY_EVENT_COUPON
					if(m_nCost <= 0)
#endif //__NEWYEARDAY_EVENT_COUPON
						Destroy();
					else if( g_pPlayer )
					{
						if( m_nCost < 0 )
							m_nCost = 0;
#ifdef __NEWYEARDAY_EVENT_COUPON
						if(m_bUseCoupon && !noChange)
						{
							if(m_pWndUseCouponConfirm == NULL)
							{	
								m_pWndUseCouponConfirm = new CWndUseCouponConfirm;
								m_pWndUseCouponConfirm->SetInfo(APP_BEAUTY_SHOP_SKIN, 1);
								m_pWndUseCouponConfirm->Initialize(this);							
							}
						}
#endif //__NEWYEARDAY_EVENT_COUPON
						if(m_nCost > 0)
						{
							if(m_pWndBeautyShopConfirm == NULL)
							{
								m_pWndBeautyShopConfirm = new CWndBeautyShopConfirm;
								m_pWndBeautyShopConfirm->Initialize(this);
							}
						}
					}
				}
				break;
			case WIDC_BTN_CANCEL:
				{
					Destroy();
				}
				break;
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


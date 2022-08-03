#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndRankInfo.h"

/****************************************************
  WndId : APP_RANK_INFOTABLEVEL - �ִ뷹����
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
  CtrlId : WIDC_STATIC4 - ��շ���
****************************************************/

CWndRankInfoTabLevel::CWndRankInfoTabLevel()
	: CWndRankTab(CGuildRank::RANKING::R7, APP_RANK_INFOTABLEVEL) {
}

CString CWndRankInfoTabLevel::ToString(const CGuildRank::GUILD_RANKING & ranking) {
	CString strWin;
	strWin.Format("%d", static_cast<int>(ranking.m_AvgLevel));
	return strWin;
}

/****************************************************
  WndId : APP_RANK_INFOTABPLAYTIME - �����÷����ִ�
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
****************************************************/

CWndRankInfoTabPlayTime::CWndRankInfoTabPlayTime()
	: CWndRankTab(CGuildRank::RANKING::R8, APP_RANK_INFOTABPLAYTIME) {
}

CString CWndRankInfoTabPlayTime::ToString(const CGuildRank::GUILD_RANKING & ranking) {
	return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************
  WndId : APP_RANK_INFO - ����Ÿ ��ŷâ
  CtrlId : WIDC_TABCTRL1 - 
****************************************************/

CWndRankInfo::CWndRankInfo() 
{ 
} 
CWndRankInfo::~CWndRankInfo() 
{ 
} 
void CWndRankInfo::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndRankInfo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 0;
	
	m_WndRankInfoTabLevel.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_INFOTABLEVEL );
	m_WndRankInfoTabPlayTime.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_INFOTABPLAYTIME );
	
	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKLEVEL );
	tabTabItem.pWndBase = &m_WndRankInfoTabLevel;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKPLAYTIME );
	tabTabItem.pWndBase = &m_WndRankInfoTabPlayTime;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );
	
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRankInfo::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_RANK_INFO, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndRankInfo::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndRankInfo::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndRankInfo::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndRankInfo::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndRankInfo::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


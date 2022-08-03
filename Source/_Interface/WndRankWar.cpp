#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndRankWar.h"

/****************************************************
  WndId : APP_RANK_WARTABGIVEUP - �׺���
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
  CtrlId : WIDC_STATIC4 - �׺���
****************************************************/

CWndRankWarTabGiveUp::CWndRankWarTabGiveUp()
	: CWndRankTab(CGuildRank::RANKING::R4, APP_RANK_WARTABGIVEUP) {
}

CString CWndRankWarTabGiveUp::ToString(const CGuildRank::GUILD_RANKING & ranking) {
	CString strWin;
	strWin.Format("%d", ranking.m_nSurrender);
	return strWin;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************
  WndId : APP_RANK_WARTABLOSE - Applet
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
  CtrlId : WIDC_STATIC4 - ��
  CtrlId : WIDC_STATIC5 - /
  CtrlId : WIDC_STATIC6 - ��
  CtrlId : WIDC_STATIC1 - ����
****************************************************/

CWndRankWarTabLose::CWndRankWarTabLose()
	: CWndRankTab(CGuildRank::RANKING::R3, APP_RANK_WARTABLOSE) {
}

CString CWndRankWarTabLose::ToString(const CGuildRank::GUILD_RANKING & ranking) {
	CString strWin;
	strWin.Format("%5d  /  %5d", ranking.m_nWin, ranking.m_nLose);
	return strWin;
}

/****************************************************
  WndId : APP_RANK_WARTABWIN - ������¸�
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
  CtrlId : WIDC_STATIC4 - ��
  CtrlId : WIDC_STATIC5 - /
  CtrlId : WIDC_STATIC6 - ��
****************************************************/

CWndRankWarTabWin::CWndRankWarTabWin()
	: CWndRankTab(CGuildRank::RANKING::R2, APP_RANK_WARTABWIN) {
}

CString CWndRankWarTabWin::ToString(const CGuildRank::GUILD_RANKING & ranking) {
	CString strWin;
	strWin.Format("%5d  /  %5d", ranking.m_nWin, ranking.m_nLose);
	return strWin;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************
  WndId : APP_RANK_WAR - ��������� ��ŷâ
  CtrlId : WIDC_TABCTRL1 - 
****************************************************/

CWndRankWar::CWndRankWar() 
{ 
} 
CWndRankWar::~CWndRankWar() 
{ 
} 
void CWndRankWar::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndRankWar::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 0;

	m_WndRankWarTabWin.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_WARTABWIN );
	m_WndRankWarTabLose.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_WARTABLOSE );
	m_WndRankWarTabGiveUp.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_WARTABGIVEUP );
	
	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKWIN );
	tabTabItem.pWndBase = &m_WndRankWarTabWin;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKLOSE );
	tabTabItem.pWndBase = &m_WndRankWarTabLose;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );
	
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKGIVEUP );
	tabTabItem.pWndBase = &m_WndRankWarTabGiveUp;
	pWndTabCtrl->InsertItem( 2, &tabTabItem );	

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRankWar::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_RANK_WAR, pWndParent, 0, CPoint( 0, 0 ) );
} 
BOOL CWndRankWar::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndRankWar::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndRankWar::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndRankWar::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndRankWar::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndRankCmn.h"

/****************************************************
  WndId : APP_RANK_GUILDTABBEST - �ְ����
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC3 - ����Ī
  CtrlId : WIDC_STATIC4 - ������Ī
  CtrlId : WIDC_STATIC5 - ��
  CtrlId : WIDC_STATIC6 - /
  CtrlId : WIDC_STATIC7 - ��
****************************************************/

CWndRankTabBest::CWndRankTabBest()
	: CWndRankTab(CGuildRank::RANKING::R1, APP_RANK_GUILDTABBEST) {
}

CWndRankTab::ValuesToPrint CWndRankTabBest::GetValuesToPrint(const CGuildRank::GUILD_RANKING & ranking) {
	return { ranking.m_nWin, ranking.m_nLose };
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************
  WndId : APP_RANK_GUILDTABUNITED - �ִ���
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
  CtrlId : WIDC_STATIC4 - ��
  CtrlId : WIDC_STATIC5 - /
  CtrlId : WIDC_STATIC6 - ��
****************************************************/

CWndRankTabUnited::CWndRankTabUnited()
	: CWndRankTab(CGuildRank::RANKING::R5, APP_RANK_GUILDTABUNITED) {
}

CWndRankTab::ValuesToPrint CWndRankTabUnited::GetValuesToPrint(const CGuildRank::GUILD_RANKING & ranking) {
	return { ranking.m_nWin, ranking.m_nLose };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************
  WndId : APP_RANK_GUILDTABPENYA - �ִ����
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����Ī
  CtrlId : WIDC_STATIC3 - ������Ī
****************************************************/

CWndRankTabPenya::CWndRankTabPenya()
	: CWndRankTab(CGuildRank::RANKING::R6, APP_RANK_GUILDTABPENYA) {
}

CWndRankTab::ValuesToPrint CWndRankTabPenya::GetValuesToPrint(const CGuildRank::GUILD_RANKING & ranking) {
	return {};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************
  WndId : APP_RANK_GUILD - ��� ��ŷâ
  CtrlId : WIDC_TABCTRL1 - 
****************************************************/

void CWndRankGuild::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 0;
	
	m_WndRankTabBest.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_GUILDTABBEST );
	m_WndRankTabUnited.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_GUILDTABUNITED );
	m_WndRankTabPenya.Create( WBS_CHILD | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_RANK_GUILDTABPENYA );
	
	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKBEST );
	tabTabItem.pWndBase = &m_WndRankTabBest;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKUNITED );
	tabTabItem.pWndBase = &m_WndRankTabUnited;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );
	
	tabTabItem.pszText = prj.GetText( TID_GAME_TOOLTIP_RANKPENYA );
	tabTabItem.pWndBase = &m_WndRankTabPenya;
	pWndTabCtrl->InsertItem( 2, &tabTabItem );
	
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRankGuild::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_RANK_GUILD, pWndParent, 0, CPoint( 0, 0 ) );
} 

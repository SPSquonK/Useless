#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndRankCmn.h"

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

CWndRankTab::ValuesToPrint CWndRankWarTabGiveUp::GetValuesToPrint(const CGuildRank::GUILD_RANKING & ranking) {
	return { ranking.m_nSurrender };
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

CWndRankTab::ValuesToPrint CWndRankWarTabLose::GetValuesToPrint(const CGuildRank::GUILD_RANKING & ranking) {
	return { ranking.m_nWin, ranking.m_nLose };
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

CWndRankTab::ValuesToPrint CWndRankWarTabWin::GetValuesToPrint(const CGuildRank::GUILD_RANKING & ranking) {
	return { ranking.m_nWin, ranking.m_nLose };
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************
  WndId : APP_RANK_WAR - ��������� ��ŷâ
  CtrlId : WIDC_TABCTRL1 - 
****************************************************/

void CWndRankWar::OnInitialUpdate() {
  CWndNeuz::OnInitialUpdate();

  CWndTabCtrl * pWndTabCtrl = GetDlgItem<CWndTabCtrl>(WIDC_TABCTRL1);
  CRect rect = GetClientRect();
  rect.left = 5;
  rect.top = 0;

  m_tabManager.In(*pWndTabCtrl, rect)
    .Add<CWndRankWarTabWin>(APP_RANK_WARTABWIN, TID_GAME_TOOLTIP_RANKWIN)
    .Add<CWndRankWarTabLose>(APP_RANK_WARTABLOSE, TID_GAME_TOOLTIP_RANKLOSE)
    .Add<CWndRankWarTabGiveUp>(APP_RANK_WARTABGIVEUP, TID_GAME_TOOLTIP_RANKGIVEUP);

  MoveParentCenter();
}

BOOL CWndRankWar::Initialize(CWndBase * pWndParent, DWORD /*dwWndId*/) {
  return CWndNeuz::InitDialog(APP_RANK_WAR, pWndParent, 0, CPoint(0, 0));
}


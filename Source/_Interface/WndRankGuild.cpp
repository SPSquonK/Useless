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

void CWndRankGuild::OnInitialUpdate() {
  CWndNeuz::OnInitialUpdate();

  CWndTabCtrl * pWndTabCtrl = GetDlgItem<CWndTabCtrl>(WIDC_TABCTRL1);
  CRect rect = GetClientRect();
  rect.left = 5;
  rect.top = 0;

  m_tabManager.In(*pWndTabCtrl, rect)
    .Add<CWndRankTabBest>(APP_RANK_GUILDTABBEST, TID_GAME_TOOLTIP_RANKBEST)
    .Add<CWndRankTabUnited>(APP_RANK_GUILDTABUNITED, TID_GAME_TOOLTIP_RANKUNITED)
    .Add<CWndRankTabPenya>(APP_RANK_GUILDTABPENYA, TID_GAME_TOOLTIP_RANKPENYA);

  MoveParentCenter();
}

BOOL CWndRankGuild::Initialize(CWndBase * pWndParent) {
  return CWndNeuz::InitDialog(APP_RANK_GUILD, pWndParent, 0, CPoint(0, 0));
}

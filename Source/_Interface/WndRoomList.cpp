#include "stdafx.h"
#include "resData.h"
#include "WndRoomList.h"
#include "defineText.h"
#include "playerdata.h"
#include "DPClient.h"
#include "WndTListBox.hpp"
#include <boost/range/adaptor/indexed.hpp>

/****************************************************
  WndId : APP_MINIROOM_LIST - �̴Ϸ� ���
  CtrlId : WIDC_BUTTON1 - Button
  CtrlId : WIDC_BUTTON2 - Button
  CtrlId : WIDC_BUTTON3 - Button
  CtrlId : WIDC_LISTBOX1 - Listbox
  CtrlId : WIDC_TEXT1 - ���� ������ ������ �̴Ϸ��� ĳ���� ����Դϴ�.
****************************************************/

void CWndRoomList::Refresh() {
	// ����Ʈ�� ���� ����
	CWndBasicTListBox<Item> * pWndListBox = GetDlgItem<CWndBasicTListBox<Item>>(WIDC_LISTBOX1);

	pWndListBox->ResetContent();

	auto visitable = CHousing::GetInstance()->m_vecVisitable
		| boost::adaptors::indexed(1);

	CString strName;
	for (const auto [nIndex, playerId] : visitable) {
		const PlayerData * pPlayerData = CPlayerDataCenter::GetInstance()->GetPlayerData(playerId);
		if (pPlayerData) {
			strName.Format("%d. %s", nIndex, pPlayerData->szPlayer);
			pWndListBox->Add(Item{ .text = strName, .playerId = playerId });
		}
	}
}

void CWndRoomList::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();

	auto & pWndListBox = ReplaceListBox<Item, WndTListBox::BasicDisplayer<Item>>(WIDC_LISTBOX1);

	// ���⿡ �ڵ��ϼ���
	CHousing::GetInstance()->m_vecVisitable.clear();
	g_DPlay.SendHousingReqVisitableList();

	pWndListBox.ChangeSelectColor(D3DCOLOR_ARGB(255, 255, 0, 0));

	MoveParentCenter();
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRoomList::Initialize(CWndBase * pWndParent, DWORD /*dwWndId*/) {
	return CWndNeuz::InitDialog(APP_MINIROOM_LIST, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndRoomList::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_BUTTON1:// ok ��ư
		{
			const CWndBasicTListBox<Item> * pWndListBox = GetDlgItem<CWndBasicTListBox<Item>>(WIDC_LISTBOX1);

			if (const Item * item = pWndListBox->GetCurSelItem()) {
				if (const u_long dwID = item->playerId) {
					g_DPlay.SendHousingVisitRoom(dwID);
				}

				Destroy();
			}
			break;
		}

		case WIDC_BUTTON2:// refresh ��ư
			g_DPlay.SendHousingReqVisitableList();
			break;

		case WIDC_BUTTON3:// cancel ��ư
			Destroy();
			break;
	};

	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}


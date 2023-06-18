#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndChatFilter.h"

#include "WndManager.h"

#include "DPClient.h"


/****************************************************
  WndId : APP_CHAT_FILTER - ä�� ����
  CtrlId : WIDC_CHECK1 - �Ϲ� ä��
  CtrlId : WIDC_CHECK2 - �ӼӸ�
  CtrlId : WIDC_CHECK3 - ��ġ��
  CtrlId : WIDC_CHECK4 - ��Ƽ
  CtrlId : WIDC_CHECK5 - ���
  CtrlId : WIDC_CHECK6 - ����, ȭ��
  CtrlId : WIDC_CHECK7 - ����, ê 

  CtrlId : WIDC_RADIO1 - ����, ȭ�� 
  CtrlId : WIDC_RADIO2 - ����, ê 
  CtrlId : WIDC_RADIO3 - �˸�, ȭ�� 
  CtrlId : WIDC_RADIO4 - �˸�, ê 

  CtrlId : WIDC_STATIC1 - ä�� ä��
  CtrlId : WIDC_STATIC2 - �Ž��� ����
****************************************************/

int CWndChatFilter::GetChannelForChatsty(int chatsty) {
	switch (chatsty) {
		default:
		case CHATSTY_GENERAL: return 0;
		case CHATSTY_WHISPER: return 1;
		case CHATSTY_SHOUT:   return 2;
		case CHATSTY_PARTY:   return 3;
		case CHATSTY_GUILD:   return 4;
	}
}

std::array<std::pair<UINT, DWORD>, 5> CWndChatFilter::WIDToChatsty = {
	std::pair<UINT, DWORD>{ WIDC_CHECK1, CHATSTY_GENERAL },
	std::pair<UINT, DWORD>{ WIDC_CHECK2, CHATSTY_WHISPER },
	std::pair<UINT, DWORD>{ WIDC_CHECK3, CHATSTY_SHOUT   },
	std::pair<UINT, DWORD>{ WIDC_CHECK4, CHATSTY_PARTY   },
	std::pair<UINT, DWORD>{ WIDC_CHECK5, CHATSTY_GUILD   }
};

void CWndChatFilter::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	SetButtonStatus();

	CRect rect = GetClientRect();
	SetWndRect( CRect(0, 0, rect.right, rect.bottom - 130));

	CWndBase * pWndGroup = GetDlgItem( WIDC_GROUPBOX2 );
	if( pWndGroup )
		pWndGroup->SetVisible(FALSE);	

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 

void CWndChatFilter::SetButtonStatus() {
	const int nChannel = GetChannelForChatsty(CWndChat::m_nChatChannel);
	const DWORD dwChatFilter = g_Option.m_dwChatFilter[ nChannel ];

	for (const auto & [widgetId, chatsty] : WIDToChatsty) {
		CWndButton * pWndCheck = GetDlgItem<CWndButton>(widgetId);
		if (dwChatFilter & chatsty) { pWndCheck->SetCheck(TRUE); }
		if (CWndChat::m_nChatChannel == chatsty) { pWndCheck->EnableWindow(FALSE); }
	}
}

BOOL CWndChatFilter::Initialize(CWndBase * pWndParent, DWORD) {
	return CWndNeuz::InitDialog(APP_CHAT_FILTER, pWndParent, WBS_MODAL, CPoint(0, 0));
}

BOOL CWndChatFilter::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) {
	const auto chatstyIt = std::find_if(
		WIDToChatsty.begin(), WIDToChatsty.end(),
		[nID](const auto & pair) { return pair.first == nID; }
	);

	if (chatstyIt != WIDToChatsty.end()) {
		const int nChannel = GetChannelForChatsty(CWndChat::m_nChatChannel);
		CWndButton * pWndCheck = GetDlgItem<CWndButton>(chatstyIt->first);

		if (pWndCheck->GetCheck()) {
			g_Option.m_dwChatFilter[nChannel] |= chatstyIt->second;
		} else {
			g_Option.m_dwChatFilter[nChannel] &= ~chatstyIt->second;
		}

		SetButtonStatus();
		CWndChat * pWndChat = g_WndMng.GetWndBase<CWndChat>(APP_COMMUNICATION_CHAT);
		if (pWndChat) pWndChat->SetChannel();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

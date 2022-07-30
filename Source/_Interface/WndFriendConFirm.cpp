#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndFriendConFirm.h"

#include "DPClient.h"
#include "MsgHdr.h"
#include "WndManager.h"

/****************************************************
  WndId : APP_FRIEND_CONFIRM - Confirm
****************************************************/

void CWndFriendConFirm::SetMember(u_long uLeader, const char * szLeadName) {
	m_uLeader = uLeader;
	std::strcpy(m_szLeaderName, szLeadName);
}

void CWndFriendConFirm::OnDraw(C2DRender * p2DRender) {
	CString strTemp;
	strTemp.Format(_T(prj.GetText(TID_DIAG_0071)), m_szLeaderName); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�

	CWndText * pWndText = GetDlgItem<CWndText>(WIDC_TEXT1);
	pWndText->SetString(strTemp);
}

void CWndFriendConFirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( ( rectRoot.right - rectWindow.Width() ) / 2, 70 );
	Move( point );
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndFriendConFirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_FRIEND_CONFIRM, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndFriendConFirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )  { 
	if( nID == WIDC_YES )  {
		// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
		g_DPlay.SendPacket<PACKETTYPE_NC_ADDFRIEND, u_long>(m_uLeader);
		Destroy();	// �����ı��� �ٲ�. -xuzhu- 09/16
	} else if (nID == WIDC_NO) {
		// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
		g_DPlay.SendFriendCancel(m_uLeader, g_pPlayer ? g_pPlayer->m_idPlayer : 0);
		Destroy();	// �����ı��� �ٲ�. -xuzhu- 09/16
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

/****************************************************
  WndId : APP_ADDFRIEND - ģ�� �߰�
  CtrlId : WIDC_STATIC1 - �߰��� �̸��� �Է��ϼ���
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_STATIC2 - ��  �� :
  CtrlId : WIDC_OK - Ȯ��
  CtrlId : WIDC_CANCEL - ���
****************************************************/

void CWndAddFriend::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();

	CWndEdit * pWndEdit = (CWndEdit *)GetDlgItem(WIDC_EDIT1);
	pWndEdit->SetFocus();

	MoveParentCenter();
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndAddFriend::Initialize(CWndBase * pWndParent, DWORD) {
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog(APP_ADDFRIEND, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndAddFriend::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	if (nID == WIDC_OK) {
		OnOk();
	} else if (nID == WIDC_CANCEL) {
		// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
		// g_DPlay.SendFriendCancel( m_uLeader, m_uMember );
		Destroy();
	}

	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

void CWndAddFriend::OnOk() {
	CWndEdit * pWndEdit = GetDlgItem<CWndEdit>(WIDC_EDIT1);
	LPCTSTR szAddName = pWndEdit->GetString();

	if (std::strlen(szAddName) >= MAX_NAME) {
		pWndEdit->SetString("");
		g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0057)));
		return;
	}

	if (strcmp(szAddName, g_pPlayer->GetName()) == 0) {
		pWndEdit->SetString("");
		g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0056)));
		return;
	}

	if (g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->GetID() == WI_WORLD_GUILDWAR) {
		g_WndMng.OpenMessageBox(prj.GetText(TID_GAME_GUILDCOMBAT_CANNOT_FRIENDADD));	// "�����ؾ��� : �������忡�� ģ���߰��� �Ҽ� �����ϴ�" );
	} else {
		g_DPlay.SendAddFriendNameReqest(szAddName);
		g_WndMng.PutString(TID_GAME_MSGINVATE, szAddName);
	}
	Destroy();

	// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
}

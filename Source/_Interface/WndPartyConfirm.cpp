#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndParty.h"

#include "DPClient.h"


/****************************************************
  WndId : APP_PARTY_CONFIRM - party confirm
  CtrlId : WIDC_YES - yes
  CtrlId : WIDC_NO - No
  CtrlId : WIDC_STATIC1 - Static
****************************************************/

CWndPartyConfirm::CWndPartyConfirm(u_long uLeader, const TCHAR * szLeaderName)
	: m_uLeader(uLeader) {
	std::strcpy(m_szLeaderName, szLeaderName);
}

void CWndPartyConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	CString strTemp;
	strTemp.Format(_T(prj.GetText(TID_DIAG_0072)),m_szLeaderName); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	pWndText->SetString( strTemp );
	
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( ( rectRoot.right - rectWindow.Width() ) / 2, 70 );
	Move( point );	
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartyConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_PARTY_CONFIRM, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndPartyConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_YES ) 
	{
		// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
		g_DPlay.SendAddPartyMember(m_uLeader);
		Destroy();	// �����ı��� �ٲ� -XUZHU- 09/16
	}
	else if(nID==WIDC_NO || nID==WTBID_CLOSE)
	{
		// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
		g_DPlay.SendPartyMemberCancle(m_uLeader, 0);
		Destroy();	// �����ı��� �ٲ� -XUZHU- 09/16
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


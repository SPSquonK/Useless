
#include "stdafx.h"
#include "resData.h"
#include "WndQuitRoom.h"
#include "MsgHdr.h"
#include "DPClient.h"

/****************************************************
  WndId : APP_QUIT_ROOM - �̴Ϸ�
  CtrlId : WIDC_STATIC1 - �̴Ϸ뿡�� ���� �Ͻðڽ��ϱ�?
  CtrlId : WIDC_BUTTON1 - Button
  CtrlId : WIDC_BUTTON2 - Button
****************************************************/

void CWndQuitRoom::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndQuitRoom::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_QUIT_ROOM, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndQuitRoom::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndQuitRoom::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndQuitRoom::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch(nID)
	{
		case WIDC_BUTTON1:// ok ��ư
			g_DPlay.SendPacket<PACKETTYPE_HOUSING_GOOUT>();
			Destroy();
			break;

		case WIDC_BUTTON2:// cancel ��ư
			Destroy();
			break;
	};
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


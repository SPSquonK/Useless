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
CWndChatFilter::CWndChatFilter() 
{ 
} 
CWndChatFilter::~CWndChatFilter() 
{ 
} 
void CWndChatFilter::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndChatFilter::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	SetButtonStatus();

	CRect rect = GetClientRect();
	SetWndRect( CRect(0, 0, rect.right, rect.bottom - 130));

	CWndGroupBox* pWndGroup = (CWndGroupBox*)GetDlgItem( WIDC_GROUPBOX2 );
	if( pWndGroup )
		pWndGroup->SetVisible(FALSE);	

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 
void CWndChatFilter::SetButtonStatus()
{
	CWndButton* pWndCheck1 = (CWndButton*)GetDlgItem( WIDC_CHECK1 );
	CWndButton* pWndCheck2 = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
	CWndButton* pWndCheck3 = (CWndButton*)GetDlgItem( WIDC_CHECK3 );
	CWndButton* pWndCheck4 = (CWndButton*)GetDlgItem( WIDC_CHECK4 );
	CWndButton* pWndCheck5 = (CWndButton*)GetDlgItem( WIDC_CHECK5 );


	int nChannel = 0;
	switch( CWndChat::m_nChatChannel )
	{
		case CHATSTY_GENERAL: nChannel = 0; break;
		case CHATSTY_WHISPER: nChannel = 1; break;
		case CHATSTY_SHOUT  : nChannel = 2; break;
		case CHATSTY_PARTY  : nChannel = 3; break;
		case CHATSTY_GUILD  : nChannel = 4; break;
	}
	DWORD dwChatFilter = g_Option.m_dwChatFilter[ nChannel ];
	if( dwChatFilter & CHATSTY_GENERAL       ) pWndCheck1->SetCheck( TRUE );
	if( dwChatFilter & CHATSTY_WHISPER       ) pWndCheck2->SetCheck( TRUE );
	if( dwChatFilter & CHATSTY_SHOUT         ) pWndCheck3->SetCheck( TRUE );
	if( dwChatFilter & CHATSTY_PARTY         ) pWndCheck4->SetCheck( TRUE );
	if( dwChatFilter & CHATSTY_GUILD         ) pWndCheck5->SetCheck( TRUE );



	if( CWndChat::m_nChatChannel == CHATSTY_GENERAL ) pWndCheck1->EnableWindow( FALSE );
	if( CWndChat::m_nChatChannel == CHATSTY_WHISPER ) pWndCheck2->EnableWindow( FALSE );
	if( CWndChat::m_nChatChannel == CHATSTY_SHOUT   ) pWndCheck3->EnableWindow( FALSE );
	if( CWndChat::m_nChatChannel == CHATSTY_PARTY   ) pWndCheck4->EnableWindow( FALSE );
	if( CWndChat::m_nChatChannel == CHATSTY_GUILD   ) pWndCheck5->EnableWindow( FALSE );

}
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndChatFilter::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_CHAT_FILTER, pWndParent, WBS_MODAL, CPoint( 0, 0 ) );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndChatFilter::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndChatFilter::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndChatFilter::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndChatFilter::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndChatFilter::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndChatFilter::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndButton* pWndCheck1 = (CWndButton*)GetDlgItem( WIDC_CHECK1 );
	CWndButton* pWndCheck2 = (CWndButton*)GetDlgItem( WIDC_CHECK2 );
	CWndButton* pWndCheck3 = (CWndButton*)GetDlgItem( WIDC_CHECK3 );
	CWndButton* pWndCheck4 = (CWndButton*)GetDlgItem( WIDC_CHECK4 );
	CWndButton* pWndCheck5 = (CWndButton*)GetDlgItem( WIDC_CHECK5 );


	CWndChat* pWndChat = (CWndChat*)g_WndMng.GetWndBase( APP_COMMUNICATION_CHAT );
	int nChannel = 0;
	switch( pWndChat->m_nChatChannel )
	{
		case CHATSTY_GENERAL: nChannel = 0; break;
		case CHATSTY_WHISPER: nChannel = 1; break;
		case CHATSTY_SHOUT  : nChannel = 2; break;
		case CHATSTY_PARTY  : nChannel = 3; break;
		case CHATSTY_GUILD  : nChannel = 4; break;
	}
	switch( nID )
	{
	case WIDC_CHECK1 :
		if( pWndCheck1->GetCheck() )
			g_Option.m_dwChatFilter[nChannel] |= CHATSTY_GENERAL;
		else
			g_Option.m_dwChatFilter[nChannel] &= ~CHATSTY_GENERAL;
		SetButtonStatus();
		break;
	case WIDC_CHECK2 :
		if( pWndCheck2->GetCheck() )
			g_Option.m_dwChatFilter[nChannel] |= CHATSTY_WHISPER;
		else
			g_Option.m_dwChatFilter[nChannel] &= ~CHATSTY_WHISPER;
		SetButtonStatus();
		break;
	case WIDC_CHECK3 :
		if( pWndCheck3->GetCheck() )
			g_Option.m_dwChatFilter[nChannel] |= CHATSTY_SHOUT;
		else
			g_Option.m_dwChatFilter[nChannel] &= ~CHATSTY_SHOUT;
		SetButtonStatus();
		break;
	case WIDC_CHECK4 :
		if( pWndCheck4->GetCheck() )
			g_Option.m_dwChatFilter[nChannel] |= CHATSTY_PARTY;
		else
			g_Option.m_dwChatFilter[nChannel] &= ~CHATSTY_PARTY;
		SetButtonStatus();
		break;
	case WIDC_CHECK5 :
		if( pWndCheck5->GetCheck() )
			g_Option.m_dwChatFilter[nChannel] |= CHATSTY_GUILD;
		else
			g_Option.m_dwChatFilter[nChannel] &= ~CHATSTY_GUILD;
		SetButtonStatus();
		break;
	}
	pWndChat->SetChannel();

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 



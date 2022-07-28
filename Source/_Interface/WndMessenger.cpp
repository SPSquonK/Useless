#include "stdafx.h"
#include "defineSound.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndMessenger.h"
#include "WndFriendCtrl.h"
#include "messenger.h"
#include "WndFriendConFirm.h"
#include "dpcertified.h"
#include "WndManager.h"

#include "DPClient.h"

//////////////////////////////////////////////////////////////////////////
// New Messenger Window
//////////////////////////////////////////////////////////////////////////

CWndMessengerEx::CWndMessengerEx() 
{ 
}

CWndMessengerEx::~CWndMessengerEx() 
{ 
	m_TexMail.DeleteDeviceObjects();
}

void CWndMessengerEx::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	CWndNeuz::SerializeRegInfo( ar, dwVersion );
	CWndTabCtrl* lpTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( ar.IsLoading() )
	{
		int nCurSel;
		ar >> nCurSel;
		lpTabCtrl->SetCurSel( nCurSel );
	}
	else
	{
		ar << lpTabCtrl->GetCurSel();
	}
}

void CWndMessengerEx::OnDraw( C2DRender* p2DRender ) 
{ 
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	// Draw Status Icon
	DWORD dwMyState;
	if( g_WndMng.m_RTMessenger.GetState() == FRS_AUTOABSENT )
		dwMyState	= FRS_ABSENT;
	else if( g_WndMng.m_RTMessenger.GetState() == FRS_ONLINE )
		dwMyState	= 2;
	else if( g_WndMng.m_RTMessenger.GetState() == FRS_OFFLINE )
		dwMyState	= 8;
	else
		dwMyState	= g_WndMng.m_RTMessenger.GetState();
	
	TEXTUREVERTEX2 vertex[ 6 ];
	TEXTUREVERTEX2* pVertices = vertex;
	
	pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( 8, 8 ), dwMyState - 2, &pVertices, 0xffffffff );	
	pWndWorld->m_texPlayerDataIcon.Render( m_pApp->m_pd3dDevice, vertex, ( (int) pVertices - (int) vertex ) / sizeof( TEXTUREVERTEX2 ) );
	
	// Draw Name & Status
	CString strState;
	switch( g_WndMng.m_RTMessenger.GetState() )
	{
		case FRS_ONLINE:
			strState.Format( "(%s)", prj.GetText( TID_FRS_ONLINE_STATUS ) );
			break;
		case FRS_OFFLINE:
			strState.Format( "(%s)", prj.GetText( TID_FRS_OFFLINE_STATUS ) );
			break;
		case FRS_ABSENT:
			strState.Format( "(%s)", prj.GetText( TID_FRS_ABSENT ) );
			break;
		case FRS_HARDPLAY:
			strState.Format( "(%s)", prj.GetText( TID_FRS_HARDPLAY ) );
			break;
		case FRS_EAT:
			strState.Format( "(%s)", prj.GetText( TID_FRS_EAT ) );
			break;
		case FRS_REST:
			strState.Format( "(%s)", prj.GetText( TID_FRS_REST ) );
			break;
		case FRS_MOVE:
			strState.Format( "(%s)", prj.GetText( TID_FRS_MOVE ) );
			break;
	}
	
	LPCTSTR pszName = g_pPlayer->GetName( TRUE );
	p2DRender->TextOut( 47, 10, 1, 1, pszName, 0xff606060 );
	p2DRender->TextOut( 48, 10, 1, 1, pszName, 0xff606060 );
	p2DRender->TextOut( 46, 27, 1, 1, strState, 0xff606060 );

	if( g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX) )
	{
		LPWNDCTRL lpMail = GetWndCtrl( WIDC_CUSTOM1 );
		p2DRender->RenderTexture( CPoint(lpMail->rect.left, lpMail->rect.top), &m_TexMail, m_nFlashCounter );
	}

	if(g_Neuz.m_nTagCount > 0 && g_Neuz.m_bCheckTag == FALSE)
	{
		LPWNDCTRL lpTagButton = GetWndCtrl( WIDC_BUTTON2 );
		p2DRender->RenderTexture( CPoint(lpTagButton->rect.left + 2, lpTagButton->rect.top + 2), &m_TexMail, m_nFlashCounter );
	}

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( pWndTabCtrl == NULL )
		return;
	if( pWndTabCtrl->GetCurSel() == 2 )
	{
		CString strCampusPoint = _T( "" );
		strCampusPoint.Format( prj.GetText( TID_GAME_CAMPUS_POINTS ), g_pPlayer->GetCampusPoint() );
		p2DRender->TextOut( 190, 20, 1, 1, strCampusPoint, 0xff606060 );
	}
	else
	{
		const CString strServerName = sqktd::CStringMaxSize(g_dpCertified.GetServerName(g_Option.m_nSer), 18);
		p2DRender->TextOut( 170, 13, 1, 1, strServerName, 0xff606060 );

		//ä�� ����
		CListedServers::Channel * channel = g_dpCertified.m_servers.GetChannelFromPos(g_Option.m_nSer, g_Option.m_nMSer);
		
		if (channel) {
			const CString strServerName = sqktd::CStringMaxSize(channel->lpName, 18);
			p2DRender->TextOut(170, 27, 1, 1, strServerName, 0xff606060);
		}
	}
} 

BOOL CWndMessengerEx::Process()
{
	if( (g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX)) || (g_Neuz.m_nTagCount > 0 && g_Neuz.m_bCheckTag == FALSE) )
	{
		m_nFlashCounter += (m_nSwitch*8);
		
		if( m_nFlashCounter >= 255 )
		{
			m_nFlashCounter = 255;
			m_nSwitch = -1;
		}
		
		if( m_nFlashCounter <= 50 )
		{
			m_nFlashCounter = 50;
			m_nSwitch = 1;
		}
	}

	if( g_pPlayer && g_pPlayer->IsMode(MODE_MAILBOX) )
	{
		LPWNDCTRL lpMail = GetWndCtrl( WIDC_CUSTOM1 );
		lpMail->m_bDisabled = FALSE;

		CRect  rect    = lpMail->rect;
		CPoint point	= GetMousePoint();
		if( PtInRect( lpMail->rect, point ) )
		{
			ClientToScreen( &point );
			ClientToScreen( &rect );
			g_toolTip.PutToolTip( (DWORD)this, prj.GetText(TID_MAIL_RECEIVE), rect, point );
		}		
	}
	return TRUE;
}

void CWndMessengerEx::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );	
	WTCITEM tabTabItem;

	m_wndFriend.Create( CRect( 0, 0, 250, 250 ), pWndTabCtrl, 11 );
	m_wndFriend.AddWndStyle( WBS_NODRAWFRAME );
	if( g_WndMng.m_RTMessenger.size() )
	{
		g_DPlay.SendGetFriendState();
	}

	m_wndGuild.Create( CRect( 0, 0, 250, 250 ), pWndTabCtrl, 13 );
	m_wndGuild.AddWndStyle( WBS_NODRAWFRAME );

	m_WndCampus.Create( WBS_CHILD | WBS_NODRAWFRAME, GetClientRect(), pWndTabCtrl, APP_MESSENGER_TAB_CAMPUS );
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMMUNITY_FRIEND); //"ģ��"
	tabTabItem.pWndBase = &m_wndFriend;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMPANY); //"�ش�"
	tabTabItem.pWndBase = &m_wndGuild;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );

	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_COMMUNITY_CAMPUS); //"����"
	tabTabItem.pWndBase = &m_WndCampus;
	pWndTabCtrl->InsertItem( 2, &tabTabItem );

	m_wndFriend.ScrollBarPos( 0 );
	m_wndGuild.ScrollBarPos( 0 );

	m_menuState.CreateMenu( this );	
	m_menuState.AppendMenu( 0, FRS_ONLINE   , prj.GetText( TID_FRS_ONLINE_STATUS   ) );
	m_menuState.AppendMenu( 0, FRS_ABSENT   , prj.GetText( TID_FRS_ABSENT   ) );
	m_menuState.AppendMenu( 0, FRS_HARDPLAY , prj.GetText( TID_FRS_HARDPLAY ) );
	m_menuState.AppendMenu( 0, FRS_EAT      , prj.GetText( TID_FRS_EAT      ) );
	m_menuState.AppendMenu( 0, FRS_REST     , prj.GetText( TID_FRS_REST     ) );
	m_menuState.AppendMenu( 0, FRS_MOVE     , prj.GetText( TID_FRS_MOVE     ) );
	m_menuState.AppendMenu( 0, FRS_OFFLINE  , prj.GetText( TID_FRS_OFFLINE_STATUS  ) );
	
	m_TexMail.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "WndMail.dds" ), 0xffff00ff );
	m_nFlashCounter = 0;
	m_nSwitch       = 1;
	
	// ��ư �̹��� ����
	CWndButton* pWndButton = (CWndButton*)GetDlgItem(WIDC_CHANNEL);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK)
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerChannel.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerChannel.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_STATE);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK)
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerState.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerState.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_LEVEL);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK)
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerLevel.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerLevel.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_JOB);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK)
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerJob.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerJob.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_NAME);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_HK)
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerChar.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtMessengerChar.bmp" ), 0xffff00ff );
	}

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), rectRoot.bottom - rectWindow.Height() );
	Move( point );
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndMessengerEx::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	if( g_eLocal.GetState( EVE_SCHOOL ) )
		return FALSE;
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_MESSENGER_EX, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndMessengerEx::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
}

void CWndMessengerEx::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
}

void CWndMessengerEx::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
}

BOOL CWndMessengerEx::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_ADD )
	{
		if( !g_WndMng.m_pWndAddFriend )
		{
			g_WndMng.m_pWndAddFriend = new CWndAddFriend;
			g_WndMng.m_pWndAddFriend->Initialize();
		}
	}
	else if( nID == WIDC_BUTTON1 )
	{
		CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
		CRect rect = pWndButton->GetWindowRect( TRUE );
		ClientToScreen( &rect );
		rect.OffsetRect( CPoint( 0, rect.Height() ) );
		m_menuState.Move( rect.TopLeft() );
		m_menuState.SetVisible( TRUE );
		m_menuState.SetFocus();
	}
	else if( nID == WIDC_BUTTON2 )
	{
		if( !g_WndMng.m_pWndMessengerNote )
		{
			g_WndMng.m_pWndMessengerNote = new CWndMessengerNote;
			g_WndMng.m_pWndMessengerNote->Initialize();
			g_Neuz.m_bCheckTag = TRUE;
		}
	}
	else if( nID == WIDC_TABCTRL1 )
	{
		CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
		CWndBase* pChild = pTabCtrl->GetFocusChild();
		if(pChild == &m_wndFriend)
			m_wndFriend.UpdatePlayerList();
		else if(pChild == &m_wndGuild)
			m_wndGuild.UpdatePlayerList();
		else if( pChild == &m_WndCampus )
			m_WndCampus.UpdatePlayerList();
	}
	else
	{
		if( FRS_ONLINE <= nID && nID < MAX_FRIENDSTAT )
		{
			// �� ���°� �ٲ����~ �ھ�� ������ ��� �˷�����~
			g_DPlay.SendSetState( nID );
		}
	}

	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	int nFocusChild = 0;
	if(pChild == &m_wndFriend)
		nFocusChild = 1;
	else if(pChild == &m_wndGuild)
		nFocusChild = 2;
	else if( pChild == &m_WndCampus )
		nFocusChild = 3;

	switch(nID)
	{
		case WIDC_CHANNEL:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyChannel();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyChannel();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyChannel();
			}
			break;
		case WIDC_STATE:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyStatus();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyStatus();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyStatus();
			}
			break;
		case WIDC_LEVEL:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyLevel();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyLevel();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyLevel();
			}
			break;
		case WIDC_JOB:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyJob();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyJob();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyJob();
			}
			break;
		case WIDC_NAME:
			{
				if(nFocusChild == 1)
					m_wndFriend.SortbyName();
				else if(nFocusChild == 2)
					m_wndGuild.SortbyName();
				else if( nFocusChild == 3 )
					m_WndCampus.SortbyName();
			}
			break;
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}
/*
void CWndMessengerEx::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	AdjustMinRect( &rectWnd, 352, 368 );
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}

void CWndMessengerEx::OnSize(UINT nType, int cx, int cy)
{
	CRect rect = GetClientRect();
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndButton* pAdd = (CWndButton*)GetDlgItem( WIDC_ADD );
	CWndButton* pTag = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_CUSTOM1 );

	rect.top += 78;
	rect.left += 8;
	rect.bottom -= 22;
	rect.right -= 8;
	
	pTabCtrl->SetWndRect( rect );
	pAdd->Move( rect.left, rect.bottom + 3 );
	pTag->Move( rect.left + 22, rect.bottom + 3 );
	wndCtrl->rect.left = rect.left + 49;
	wndCtrl->rect.right = wndCtrl->rect.left + 12;
	wndCtrl->rect.top = rect.bottom + 3;
	wndCtrl->rect.bottom = wndCtrl->rect.top + 12;

	m_wndFriend.ScrollBarPos( 0 );
	m_wndGuild.ScrollBarPos( 0 );

	CWndNeuz::OnSize( nType, cx, cy );
}
*/
void CWndMessengerEx::UpdateFriendList()
{
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	if(pChild == &m_wndFriend)
		m_wndFriend.UpdatePlayerList();
}

void CWndMessengerEx::UpdateGuildMemberList()
{
	CWndTabCtrl* pTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	if(pChild == &m_wndGuild)
		m_wndGuild.UpdatePlayerList();
}

void CWndMessengerEx::UpdateCampusMemberList()
{
	CWndTabCtrl* pTabCtrl = ( CWndTabCtrl* )GetDlgItem( WIDC_TABCTRL1 );
	CWndBase* pChild = pTabCtrl->GetFocusChild();
	if( pChild == &m_WndCampus )
		m_WndCampus.UpdatePlayerList();
}

CWndInstantMsg::CWndInstantMsg() 
{ 
} 
CWndInstantMsg::~CWndInstantMsg() 
{ 
} 
void CWndInstantMsg::OnDraw( C2DRender* p2DRender ) 
{ 
	if( m_timer.IsTimeOut() )
	{
		Destroy();
	}
} 
void CWndInstantMsg::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	m_timer.Set( SEC( 10 ) ); // �����ų�, �Ž����� ���� ���� 10�� �Ŀ� �������.

	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), rectRoot.bottom  - rectWindow.Height() );
	Move( point );
	//MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndInstantMsg::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_INSTANTMSG, pWndParent, WBS_NOFOCUS, CPoint( 0, 0 ) );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndInstantMsg::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndInstantMsg::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndInstantMsg::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndInstantMsg::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	if( m_strPlayer == "Mail" || m_strPlayer == "Event" )
		return;

	CWndMessage* pWndMessage = g_WndMng.OpenMessage( m_strPlayer );
	pWndMessage->AddMessage( m_strPlayer, m_strMessage );
	Destroy();
} 
void CWndInstantMsg::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndInstantMsg::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID != WTBID_CLOSE )
	{
		if( m_strPlayer == "Event" )
		{
			//g_WndMng.ObjectExecutor( SHORTCUT_APPLET, APP_WEBBOX2 );
			Destroy();
		}
		else
		if( m_strPlayer != "Mail" )
		{
			CWndMessage* pWndMessage = g_WndMng.OpenMessage( m_strPlayer );
			pWndMessage->AddMessage( m_strPlayer, m_strMessage );
			Destroy();
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
void CWndInstantMsg::AddMessage( LPCTSTR lpszFrom, LPCTSTR lpszMessage )
{
	PLAYSND( SND_INF_MESSENGERRING );
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	CString strMessage;
	m_strMessage = lpszMessage;
	strMessage.Format( "#cff0000ff%s%s :#nc\n  %s\n", lpszFrom, prj.GetText(TID_GAME_FROM3), lpszMessage );
//	strMessage.Format( "#cff0000ff%s���� �� :#nc\n  %s\n", lpszFrom, lpszMessage );
	pWndText->AddString( strMessage );
	m_timer.Reset();
//	pWndText->m_wndScrollBar.SetMaxScrollPos();
}

void CWndInstantMsg::AddMessageJoin( LPCTSTR lpszJoinName )
{
	PLAYSND( SND_INF_MESSENGERRING );
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	CString strMessage;
	strMessage.Format( "#cff0000ff%s%s\n", lpszJoinName, prj.GetText(TID_GAME_LOGIN) );
//	strMessage.Format( "#cff0000ff%s���� �α����Ͽ����ϴ�\n", lpszJoinName );
	pWndText->AddString( strMessage );
	m_timer.Reset();
}

void CWndInstantMsg::AddPostMessage( LPCTSTR lpszSendName )
{
	PLAYSND( SND_INF_MESSENGERRING );
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	pWndText->AddString( lpszSendName );
	m_timer.Reset();
}
//////////////////////////////////////////////////////////

CWndMessage::CWndMessage() 
{ 
} 
CWndMessage::~CWndMessage() 
{ 
} 
void CWndMessage::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMessage::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT );
	pWndEdit->AddWndStyle( EBS_AUTOVSCROLL );
	pWndEdit->SetNativeMode();

	pWndEdit->SetFocus();

	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
void CWndMessage::InitSize( void )
{
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
}
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndMessage::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_MESSAGE, pWndParent, WBS_THICKFRAME, CPoint( 0, 0 ) );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndMessage::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessage::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessage::SetWndRect( CRect rectWnd, BOOL bOnSize )
{
	AdjustMinRect( &rectWnd, 16 * 10, 16 * 10 );
//	AdjustMaxRect( &rectWnd, 16 * 12, 16 * 12 );
	CWndNeuz::SetWndRect( rectWnd, bOnSize );
}
void CWndMessage::OnSize( UINT nType, int cx, int cy ) \
{ 
	CRect rect = GetClientRect();//GetWndRect();
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT );
	CWndButton* pWndSend = (CWndButton*)GetDlgItem( WIDC_SEND );

	rect.top += 4;
	rect.left += 4;
	rect.right -= 4;
	rect.bottom -= 50;
	pWndText->SetWndRect( rect );

	rect = GetClientRect();//GetWndRect();
	rect.top = rect.bottom - 45;
	rect.left += 4;
	rect.right -= 80;
	rect.bottom -= 4;
	pWndEdit->SetWndRect( rect );
	
	rect = GetClientRect();//GetWndRect();
	rect.top = rect.bottom - 45;
	rect.left = rect.right - 74;
	rect.right -= 4;
	rect.bottom -= 4;
	//pWndSend->SetWndRect( rect );
	pWndSend->Move( rect.TopLeft() );//rect.right + 4, rect.top );

	//pAdd->Move( rect.left + 5, rect.bottom + 2 );
	//pFind->Move( rect.left + 25, rect.bottom + 2 );
	
	//m_wndPlace.Move( CPoint( 0, 0 ) );
	//m_wndZoomIn.Move( CPoint(0, cy - 32 ) );
	//m_wndZoomOut.Move( CPoint(0, cy - 16 ) );
	
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMessage::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessage::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessage::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndEdit* pWndText = (CWndEdit*)GetDlgItem( WIDC_EDIT );
	switch(nID)
	{
		case WIDC_EDIT: // ���� 
			if( message == EN_RETURN)
			{
				if( pWndText->m_string.IsEmpty() == FALSE )
				{
					CString strFormat = pWndText->m_string;					
					pWndText->m_string.GetTextFormat(strFormat);
					CString string;
					string.Format( "/say \"%s\" %s", m_strPlayer, strFormat );
					
					g_DPlay.SendChat( string );
					pWndText->Empty();
				}
				//				pWndText->ResetString();
			}
			break;
		case WIDC_SEND: // ���� 
			{
				CString str = pWndText->m_string;
				if( str.IsEmpty() == FALSE )
				{
					CString strFormat = pWndText->m_string;

					pWndText->m_string.GetTextFormat(strFormat);

					CString string;
					string.Format( "/say \"%s\" %s", m_strPlayer, strFormat );					
					
					g_DPlay.SendChat( string );
					pWndText->Empty();
	//				pWndText->ResetString();
				}
			}
			break;

	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
void CWndMessage::AddMessage( LPCTSTR lpszFrom, LPCTSTR lpszMessage )
{
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	CString strMessage;

	if( !strcmp( lpszFrom, g_pPlayer->GetName() ) )
		strMessage.Format( "#cffff0000%s%s :#nc\n  %s\n", lpszFrom, prj.GetText(TID_GAME_FROM3), lpszMessage );
	//  	strMessage.Format( "#cffff0000%s���� �� :#nc\n  %s\n", lpszFrom, lpszMessage );
	else
		strMessage.Format( "#cff0000ff%s%s :#nc\n  %s\n", lpszFrom, prj.GetText(TID_GAME_FROM3), lpszMessage );
	//  	strMessage.Format( "#cff0000ff%s���� �� :#nc\n  %s\n", lpszFrom, lpszMessage );
	
	pWndText->AddString( strMessage );
	pWndText->m_wndScrollBar.SetMaxScrollPos();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		����â
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************
WndId : APP_MESSAGE_NOTE - ����������â
CtrlId : WIDC_EDIT1 - 
CtrlId : WIDC_BUTTON1 - Button
CtrlId : WIDC_TEXT1 - 
****************************************************/

CWndMessageNote::CWndMessageNote() 
{ 
} 
CWndMessageNote::~CWndMessageNote() 
{ 
} 
void CWndMessageNote::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMessageNote::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
//	m_pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	m_pEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	m_pEdit->AddWndStyle( EBS_WANTRETURN );//| WBS_VSCROLL  );
	m_pEdit->SetNativeMode();
	m_pEdit->SetFocus();
	CString strTitle;
	strTitle.Format( "%s - %s", GetTitle(), m_szName );
	SetTitle( strTitle );
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndMessageNote::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_MESSAGE_NOTE, pWndParent, 0, CPoint( 0, 0 ) );
} 
/*
���� ������ ���� ��� 
BOOL CWndMessageNote::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
CRect rectWindow = m_pWndRoot->GetWindowRect(); 
CRect rect( 50 ,50, 300, 300 ); 
SetTitle( _T( "title" ) ); 
return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessageNote::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessageNote::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMessageNote::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessageNote::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessageNote::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		LPSTR lpSendMessage = (LPSTR)m_pEdit->GetString();
		int adf = strlen( lpSendMessage );
		if( strlen( lpSendMessage ) <= 255 )
		{
			g_DPlay.SendMessageNote( m_dwUserId, (LPSTR)m_pEdit->GetString() );
		}
		else
		{
			// ���� �޼��� 
			g_WndMng.PutString( prj.GetText( TID_GAME_MESSFULLMSG ), NULL, prj.GetTextColor( TID_GAME_MESSFULLMSG ) );
		}
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

/****************************************************
WndId : APP_MESSENGER_NOTE - ����â
CtrlId : WIDC_TEXT1 - 
****************************************************/

CWndMessengerNote::CWndMessengerNote() 
{ 
} 
CWndMessengerNote::~CWndMessengerNote() 
{ 
} 
void CWndMessengerNote::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMessengerNote::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	for( int i = 0 ; i < g_Neuz.m_nTagCount ; i++ )
	{
		CWndText * pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
		pWndText->AddWndStyle( EBS_WANTRETURN );
		CString strMessage;
		strMessage.Format( "%s - %d / %02d / %02d", g_Neuz.m_strTagName[ i ], g_Neuz.m_dwtegDate[ i ] / 10000, ( g_Neuz.m_dwtegDate[ i ] % 10000 ) / 100,  ( g_Neuz.m_dwtegDate[ i ] % 10000 ) % 100 );
		pWndText->AddString( strMessage, 0xff804000 );
		pWndText->AddString( "\n  " );
		pWndText->AddString( g_Neuz.m_strTagMessage[ i ] );
		pWndText->AddString( "\n\n" );
		
//		g_Neuz.m_dwtegDate[ i ]
//		g_Neuz.m_strTagMessage[ i ]
	}
	
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndMessengerNote::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_MESSENGER_NOTE, pWndParent, 0, CPoint( 0, 0 ) );
} 
/*
���� ������ ���� ��� 
BOOL CWndMessengerNote::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
CRect rectWindow = m_pWndRoot->GetWindowRect(); 
CRect rect( 50 ,50, 300, 300 ); 
SetTitle( _T( "title" ) ); 
return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMessengerNote::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMessengerNote::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMessengerNote::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMessengerNote::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMessengerNote::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 



#include "stdafx.h"
#include "AppDefine.h"
#include "WndHelp.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void CWndHelp::OnDraw(C2DRender* p2DRender) {
	const DWORD dwLeft = m_rectClient.Width() * 50 / 100;
	const CRect rect(dwLeft + 5, 5, m_rectClient.Width() - 5, 23);
	p2DRender->TextOut(rect.left + 10, rect.top + 8, m_strKeyword, 0xff000000);
}

void CWndHelp::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();

	CWndTreeCtrl * pWndTreeCtrl = (CWndTreeCtrl *)GetDlgItem(WIDC_TREE1);
	pWndTreeCtrl->LoadTreeScript(MakePath(DIR_CLIENT, _T("treeHelp.inc")));
	
	CWndText * pWndText = (CWndText *)GetDlgItem(WIDC_TEXT1);
	pWndText->AddWndStyle(WBS_VSCROLL);

	MoveParentCenter();
}

BOOL CWndHelp::Initialize(CWndBase * pWndParent, DWORD) {
	return CWndNeuz::InitDialog(g_Neuz.GetSafeHwnd(), APP_HELPER_HELP, 0, CPoint(0, 0), pWndParent);
}

BOOL CWndHelp::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	if (nID == WIDC_TREE1) {
		TREEELEM * lpTreeElem = reinterpret_cast<TREEELEM *>(pLResult);
		if (lpTreeElem) ChangeDisplayedHelp(*lpTreeElem);
	}

	return CWndNeuz::OnChildNotify(message,nID,pLResult);
}

void CWndHelp::ChangeDisplayedHelp(TREEELEM & treeElem) {
	m_strKeyword = treeElem.m_strKeyword;

	const CString string = prj.GetHelp(m_strKeyword);

	CWndText * pWndText = (CWndText *)GetDlgItem(WIDC_TEXT1);
	pWndText->SetString(string);
	pWndText->UpdateScrollBar();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndHelpFAQ::CWndHelpFAQ()
{
}
CWndHelpFAQ::~CWndHelpFAQ()
{
}
void CWndHelpFAQ::OnDraw(C2DRender* p2DRender)
{
	/*
	DWORD dwLeft = m_rectClient.Width() * 50 / 100;
	CRect rect;

	rect.SetRect( dwLeft + 5, 5, m_rectClient.Width() - 5, 23 );

//	p2DRender->RenderFillRect ( rect, D3DCOLOR_ARGB(255,150,150,200), D3DCOLOR_ARGB(255,100,100,150), D3DCOLOR_ARGB(255,150,150,200), D3DCOLOR_ARGB(255,100,100,150));
	//p2DRender->RenderRoundRect( rect, D3DCOLOR_ARGB(255,200,200,250) );

	p2DRender->TextOut( rect.left + 10, rect.top + 10, m_strKeyword, 0xff000000 );
	*/
	/*

	rect.SetRect( dwLeft + 5, 25, m_rectClient.Width() - 5, m_rectClient.Height() - 5 );

	p2DRender->RenderFillRect ( rect, D3DCOLOR_ARGB(255,170,170,230) );
	p2DRender->RenderRoundRect( rect, D3DCOLOR_ARGB(255,200,200,250) );

	CStringArray* pStrArray = prj.GetHelp(m_strKeyword);
	if(pStrArray)
	{
		m_strArray.RemoveAll();
		ClipStrArray(&g_Neuz.m_2DRender,rect,0,pStrArray,&m_strArray);
		CString string;
		for(int i = 0; i < m_strArray.GetSize(); i++)
		{
			string = m_strArray.GetAt(i);
			p2DRender->TextOut(rect.left + 5, rect.top + 5 + i * 16, string);
		}
	}
	*/
}
void CWndHelpFAQ::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	DWORD dwRight = m_rectClient.Width() * 50 / 100;
	//m_wndViewCtrl.Create( WBS_CHILD, CRect( 5, 5, dwRight,m_rectClient.Height() - 5 ), this, 1005 );//,m_pSprPack,-1);//m_pSprPack,16);

	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LIST1 );
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
//	pWndText->AddWndStyle( WBS_VSCROLL );
	//pWndTreeCtrl->LoadTreeScript( MakePath( DIR_CLIENT,  _T( "treeFaq.inc" ) ) );
	CRect rect( 210, 0, 300, 100 );
//	pWndListBox->AddWndStyle( WBS_NODRAWFRAME );
	//pWndListBox->m_nFontColor = 0xff000000;
	//pWndListBox->m_nSelectColor = 0xff0000ff;

	LoadFAQ( MakePath( DIR_CLIENT,  _T( "Faq.inc" ) ) );
	//m_wndText.AddWndStyle( WBS_VSCROLL );
	//m_wndText.Create( 0, rect, this, 10 );
	//rect = m_wndText.GetClientRect();

	//m_wndText.m_string.SetString( _T("����\n �ڶ�������\n �±ر� �տ� ������ ������ ������ ������ ���Ͽ� ���� ������ ���� �漺�� ���� ���� ���� �����մϴ�.\n �ҷ���� ") );
	//m_wndText.m_string.Reset( m_pFont, &rect );

	//pWndTreeCtrl->m_nFontColor = 0xff000000;
	//pWndTreeCtrl->m_nSelectColor = 0xff8080f0;

	//SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "WndHelp.bmp" ), TRUE );
	//FitTextureSize();
	MoveParentCenter();
}
BOOL CWndHelpFAQ::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	CRect rect = m_pWndRoot->MakeCenterRect(400,350);
//etTitle( GETTEXT( TID_APP_HELPER_HELP ) );
	//return CWndNeuz::Create(0|WBS_MOVE|WBS_SOUND|WBS_CAPTION|WBS_THICKFRAME,rect,pWndParent,dwWndId);
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_HELPER_FAQ, 0, CPoint( 0, 0 ), pWndParent );

}

BOOL CWndHelpFAQ::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	/*
	switch(nID)
	{
		case 0: // wnd1 
			g_WndMng.OpenWnd1();
			break;
		case 1: // wnd2
			g_WndMng.OpenCharacter();
			break;
		case 2: // ê 
			g_WndMng.OpenChat();
			break;
		case 3:
			g_clientMsg.PutMessage(&g_Neuz.m_2DRender,0xffffffff,"fuck");
			break;
		case 9:
			g_WndMng.OpenCustomBox("�����Ͻðڽ��ϱ�?",new CWndExitBox);
			break;
	}
	*/
	return CWndNeuz::OnCommand(nID,dwMessage,pWndBase);
}
void CWndHelpFAQ::OnSize(UINT nType, int cx, int cy)
{
	DWORD dwRight = m_rectClient.Width() * 50 / 100;
	//m_wndViewCtrl.SetWndRect( CRect( 5, 5, dwRight,m_rectClient.Height() - 5 ) );

	//DWORD dwLeft = m_rectClient.Width() * 50 / 100;
	CRect rect;
	//rect.SetRect( dwRight + 5, 25, m_rectClient.Width() - 5, m_rectClient.Height() - 5 );
	//m_wndText.SetWndRect( rect );

	CWndNeuz::OnSize( nType, cx, cy );
}
void CWndHelpFAQ::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(IsWndRoot())
		return;

	//if(IsWndStyle(WBS_CAPTION) && m_bPickup)
	{//
	//	m_wndTitleBar.m_wndMinimize.SetVisible(TRUE);
		//m_wndTitleBar.m_wndMaximize.SetVisible(TRUE);
	}
}
void CWndHelpFAQ::OnLButtonDown(UINT nFlags, CPoint point)
{
//	CWndBase::OnLButtonDown(nFlags,point
	if(IsWndRoot())
		return;
//	return;
}
BOOL CWndHelpFAQ::LoadFAQ( LPCTSTR lpszFileName )
{
	CScript s;
	if( s.Load( lpszFileName ) == FALSE )
		return FALSE;
	CString strKeyword;
	s.GetToken();	// keyword
	while( s.tok != FINISHED )
	{
		strKeyword	= s.token;
		s.GetToken();	// {
		s.GetToken();	//
		CString string = "    " + s.Token;
		m_mapFAQ.SetAt( strKeyword, string );
		CWndListBox* pWndListBox	= (CWndListBox*)GetDlgItem( WIDC_LIST1 );
		CString strFAQ;
		strFAQ = "Q.";
		strFAQ	+= strKeyword;
		pWndListBox->AddString( strFAQ );
		s.GetToken();	// }
		s.GetToken();
	}
	return TRUE;
}

BOOL CWndHelpFAQ::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	switch(nID)
	{
		case WIDC_LIST1: // view ctrl
			{
				CString strKey, string;
				CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LIST1 );
				pWndListBox->GetText( pWndListBox->GetCurSel(), strKey );
				{
					strKey = strKey.Right( strKey.GetLength() - 2 );
					m_mapFAQ.Lookup( strKey, string );
					m_strKeyword = strKey;

					CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
					const auto rect = pWndText->GetClientRect();
					pWndText->m_string.Init( m_pFont, &rect );
					pWndText->m_string.SetString("");
					pWndText->m_string.AddString(m_strKeyword, 0xff8080ff, ESSTY_BOLD);
					pWndText->m_string.AddString("\n\n");
					pWndText->m_string.AddString( string );
					pWndText->UpdateScrollBar();
				}
			}
			break;
	}
	return CWndNeuz::OnChildNotify(message,nID,pLResult);
}

CWndHelpTip::CWndHelpTip() 
{ 
} 
CWndHelpTip::~CWndHelpTip() 
{ 
} 
void CWndHelpTip::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndHelpTip::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
	pWndText->AddWndStyle( WBS_NOFRAME );
	pWndText->AddWndStyle( WBS_NODRAWFRAME );
	CScript s;
	if( s.Load( MakePath( DIR_CLIENT, _T("tip.inc" ) ) ) == FALSE )
		return;
	s.GetToken();
	while( s.tok != FINISHED )
	{
		CString string = "    " + s.Token;
		m_aString.Add( string );
		s.GetToken();
	}
	m_nPosString = 0;
	pWndText->SetString( m_aString.GetAt( 0 ), 0xff000000 );
} 
BOOL CWndHelpTip::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	InitDialog( g_Neuz.GetSafeHwnd(), APP_HELPER_TIP );
	MoveParentCenter();
	return TRUE;
} 
BOOL CWndHelpTip::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	switch( nID )
	{
	case WIDC_NEXT:
		{
			m_nPosString++;
			if( m_nPosString == m_aString.GetSize() )
				m_nPosString = 0;
			CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT );
			pWndText->SetString( m_aString.GetAt( m_nPosString ), 0xff000000 );
		}
		break;
	case WIDC_CLOSE:
		Destroy();
	}
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndHelpTip::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndHelpTip::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndHelpTip::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndHelpTip::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_CHECK1:
		g_Option.m_bTip = !g_Option.m_bTip;
		Destroy();
		break;
	//case WIDC_CLOSE:
	//	Destroy();
	//	return TRUE;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
/****************************************************
  WndId : APP_HELP_INSTANT - Instant Help
  CtrlId : WIDC_TEXT1 - 
  CtrlId : WIDC_BUTTON1 - Button
****************************************************/

CWndHelpInstant::CWndHelpInstant() 
{ 
} 
CWndHelpInstant::~CWndHelpInstant() 
{ 
} 
void CWndHelpInstant::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndHelpInstant::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���


	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	pWndText->SetString( prj.GetHelp( m_strHelpKey ) );
	
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndHelpInstant::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_HELP_INSTANT, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndHelpInstant::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndHelpInstant::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndHelpInstant::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndHelpInstant::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndHelpInstant::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndHelpInstant::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_CLOSE )
	{
#ifdef __HELP_BUG_FIX
		Destroy();
#else //__HELP_BUG_FIX
		// ���� ���߻��� ����
		vector<CString>::iterator where = find(g_vecHelpInsKey.begin(), g_vecHelpInsKey.end(), m_strHelpKey );

		if(where != g_vecHelpInsKey.end())
			g_vecHelpInsKey.erase(where);

		Destroy( TRUE );		
#endif //__HELP_BUG_FIX
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


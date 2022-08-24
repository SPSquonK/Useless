#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndGuildTabInfo.h"
#include "WndManager.h"
#include "DPClient.h"
#include "MsgHdr.h"
#include "defineQuest.h"
#include "playerdata.h"

#include "guild.h"
#include "eveschool.h"

/****************************************************
  WndId : APP_GUILD_DISMISS
****************************************************/

void CWndGuildDisMiss::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
}

BOOL CWndGuildDisMiss::Initialize(CWndBase * pWndParent) {
	LPWNDAPPLET lpWndApplet = m_resMng.GetAt(APP_GUILD_DISMISS);
	const CRect rect = CRect(CPoint(0, 0), lpWndApplet->size);
	// TODO: Why did they use Create and not InitDialog?
	return CWndNeuz::Create(WBS_MOVE | WBS_SOUND | WBS_CAPTION | WBS_MODAL, rect, pWndParent, APP_GUILD_DISMISS);
}

BOOL CWndGuildDisMiss::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_YES:
			if (g_GuildCombatMng.m_bRequest)
				g_WndMng.OpenMessageBox(prj.GetText(TID_GAME_GUILDCOMBAT_NOT_DISSOLVE_GUILD));	// "�����ؾ��� : �������� ��û�ѱ��� �����ü�� �Ҽ� �����ϴ�" );
			else
				g_DPlay.SendDestroyGuild();
			Destroy();
			break;
		case WIDC_NO:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}



/****************************************************
  WndId : APP_GUILD_NOTICE
****************************************************/
void CWndGuildNotice::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	// ���⿡ �ڵ��ϼ���
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
	{
		CWndEdit* pNotice = (CWndEdit*) GetDlgItem( WIDC_EDIT1 );
		pNotice->AddWndStyle( EBS_WANTRETURN );
		pNotice->AddWndStyle( EBS_AUTOVSCROLL );
		
		LPWNDCTRL pWndCtrl = GetWndCtrl( WIDC_EDIT1 );
		if(pWndCtrl)
			pNotice->SetWndRect(pWndCtrl->rect);

		pNotice->SetString( pGuild->m_szNotice );		// ���� ���.
	}
	
	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildNotice::Initialize(CWndBase * pWndParent) {
	LPWNDAPPLET lpWndApplet = m_resMng.GetAt(APP_GUILD_NOTICE);
	const CRect rect = CRect(CPoint(0, 0), lpWndApplet->size);
	return CWndNeuz::Create( /*WBS_THICKFRAME |*/ WBS_MOVE | WBS_SOUND | WBS_CAPTION | WBS_MODAL, rect, pWndParent, APP_GUILD_NOTICE);
}

BOOL CWndGuildNotice::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndEdit* pNotice = (CWndEdit*) GetDlgItem( WIDC_EDIT1 );

	switch( nID )
	{
	case WIDC_BUTTON2:
		{
			LPCTSTR szNotice = pNotice->GetString();

			if (strlen(szNotice) < MAX_BYTE_NOTICE) {
				g_DPlay.SendGuildNotice(szNotice);
				Destroy();
			} else {
				g_WndMng.OpenMessageBox(prj.GetText(TID_GUILD_NOTICE_ERROR));
			}
			break;
		}
	case WIDC_BUTTON1:		// ������.
		pNotice->SetString( "" );		// ����Ʈ �ڽ� ������ Ŭ����
		break;
	case WIDC_BUTTON3:
		pNotice->SetFocus();			// ������ �ǵ帮�� �ʰ� ��Ŀ���� ����Ʈ �ڽ��� �̵�.
		break;
	}
		
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


/****************************************************
  WndId : APP_GUILD_SETLOGO
****************************************************/

void CWndGuildSetLogo::OnDraw(C2DRender * p2DRender) {

	if (m_nSelectLogo) {
		CRect rect = m_rect[m_nSelectLogo - 1];

		rect.InflateRect(1, 1, 1, 1);
		p2DRender->RenderRect(rect, D3DCOLOR_XRGB(255, 0, 255));

		rect.InflateRect(1, 1, 1, 1);
		p2DRender->RenderRect(rect, D3DCOLOR_XRGB(255, 0, 255));
	}

	p2DRender->m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	p2DRender->m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	if (CWndWorld * pWndWorld = (CWndWorld *)g_WndMng.GetWndBase(APP_WORLD)) {
		for (int i = 0; i < CUSTOM_LOGO_MAX - 7; i++) {
			p2DRender->RenderTexture(m_rect[i].TopLeft(), &pWndWorld->m_pTextureLogo[i]);
		}
	}

	p2DRender->m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	p2DRender->m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CWndGuildSetLogo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	m_nSelectLogo = 0;
	
	{
		CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

		if( pWndWorld == NULL )
			return;

		// �����ġ ����
		int index = 0;
		for( int y=0; y<4; y++ )
		{
			for( int x=0; x<5; x++ )
			{
				CPoint pt = CPoint( x+24, y+18 );
				
				pt.x += (x*(pWndWorld->m_pTextureLogo[index].m_size.cx+5));
				pt.y += (y*(pWndWorld->m_pTextureLogo[index].m_size.cy+5));
				
				m_rect[index] = CRect(pt, pWndWorld->m_pTextureLogo[index].m_size);
				index++;
			}
		}

		if (CGuild * pGuild = g_pPlayer->GetGuild()) {
			m_nSelectLogo = pGuild->m_dwLogo;
		}
	}

	// Check Game Master
	if (!g_pPlayer->IsAuthHigher(AUTH_GAMEMASTER)) {
		CWndButton * pWndButton = (CWndButton *)GetDlgItem(WIDC_BUTTON1);
		if (pWndButton) {
			pWndButton->SetVisible(FALSE);
			pWndButton->EnableWindow(FALSE);
		}

		CRect rectWindow = GetWindowRect();
		rectWindow.bottom = 208;
		SetWndRect(rectWindow);
	}

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildSetLogo::Initialize(CWndBase * pWndParent) {
	LPWNDAPPLET lpWndApplet = m_resMng.GetAt(APP_GUILD_SETLOGO);
	const CRect rect = CRect(CPoint(0, 0), lpWndApplet->size);
	return CWndNeuz::Create( /*WBS_THICKFRAME |*/ WBS_MOVE | WBS_SOUND | WBS_CAPTION | WBS_MODAL, rect, pWndParent, APP_GUILD_SETLOGO);
}

void CWndGuildSetLogo::OnLButtonUp(UINT nFlags, CPoint point) {
	for (int i = 0; i < CUSTOM_LOGO_MAX - 7; i++) {
		if (PtInRect(&m_rect[i], point)) {
			if (m_nSelectLogo == i + 1) {
				m_nSelectLogo = 0;
			} else {
				m_nSelectLogo = i + 1;
			}
			return;
		}
	}
}

BOOL CWndGuildSetLogo::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch (nID) {
		case WIDC_OK:
			if (CGuild * pGuild = g_pPlayer->GetGuild()) {
				g_DPlay.SendGuildLogo(m_nSelectLogo);
			}

			Destroy();
			break;
		case WIDC_CANCEL:
		{
			Destroy();
			break;
		}
		case WIDC_BUTTON1:
		{
			CGuild * pGuild = g_pPlayer->GetGuild();

			if (pGuild && g_WndMng.m_pWndWorld) {
				if (g_WndMng.m_pWndWorld->GetGMLogoIndex() != -1)
					g_DPlay.SendGuildLogo(g_WndMng.m_pWndWorld->GetGMLogoIndex());
				else
					g_WndMng.OpenMessageBox(prj.GetText(TID_GAME_DONOTHAVE_GMLOGO));
			}

			Destroy();
			break;
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 



/****************************************************
  WndId : APP_GUILD_TABINFO
****************************************************/

void CWndGuildTabInfo::OnDraw( C2DRender* p2DRender ) 
{ 
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	CGuild* pGuild = g_pPlayer->GetGuild();

	if( pWndWorld && pGuild )
	{
		if( pGuild->m_dwLogo != 0 )
		{
			LPWNDCTRL lpWndCtrl = GetWndCtrl( WIDC_CUSTOM1 );

			CPoint point = lpWndCtrl->rect.TopLeft();

			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			
			pWndWorld->m_pTextureLogo[pGuild->m_dwLogo-1].Render( &g_Neuz.m_2DRender, point, 255 );

			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			
		}
	}
} 

void CWndGuildTabInfo::UpdateData()
{
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
	{
		CString strText;
		CWndBase *pWndText;
		
		pWndText = GetDlgItem( WIDC_GUILDNAME );
		pWndText->SetTitle( pGuild->m_szGuild );		// ����
		pWndText = GetDlgItem( WIDC_GUILDLEVEL );
		strText.Format( "%d", pGuild->m_nLevel );
		pWndText->SetTitle( strText );					// ��� ����.
		pWndText = GetDlgItem( WIDC_GUILDMASTER );
		LPCTSTR szMaster	= CPlayerDataCenter::GetInstance()->GetPlayerString( pGuild->m_idMaster );
		
		pWndText->SetTitle( szMaster );

		pWndText = GetDlgItem( WIDC_GUILDNUMBER );
		
		strText.Format( "%d / %d", pGuild->GetSize(), CGuildTable::GetInstance().GetMaxMemeber(pGuild->m_nLevel) );	
		pWndText->SetTitle( strText );  // ��� �ο�
		pWndText = GetDlgItem( WIDC_GUILDEXPMERIT );
		strText.Format( "%u", pGuild->m_dwContributionPxp );
		pWndText->SetTitle( strText );	// ���嵵 pxp
		pWndText = GetDlgItem( WIDC_GUILDPENYAMERIT );
		strText.Format( "%u", pGuild->m_nGoldGuild );
		pWndText->SetTitle( strText );	// �������
		
		CWndText* pNotice = (CWndText*) GetDlgItem( WIDC_TEXT1 );
		if( pNotice )
			pNotice->SetString( pGuild->m_szNotice );
	}
	else
	{
		CString strText;
		CWndBase *pWndText;
		
		pWndText = GetDlgItem( WIDC_GUILDNAME );
		pWndText->SetTitle( "" );		// ����
		pWndText = GetDlgItem( WIDC_GUILDLEVEL );
		pWndText->SetTitle( "" );					// ��� ����.
		pWndText = GetDlgItem( WIDC_GUILDMASTER );
		pWndText->SetTitle( "" );		// ��� ��.
		pWndText = GetDlgItem( WIDC_GUILDNUMBER );
		pWndText->SetTitle( "" );  // ��� �ο�
		pWndText = GetDlgItem( WIDC_GUILDEXPMERIT );
		pWndText->SetTitle( "" );	// ���嵵 pxp
		pWndText = GetDlgItem( WIDC_GUILDPENYAMERIT );
		pWndText->SetTitle( "" );	// �������
		
		CWndText* pNotice = (CWndText*) GetDlgItem( WIDC_TEXT1 );
		if( pNotice )
			pNotice->SetString( "" );
	}
}

void CWndGuildTabInfo::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	UpdateData();
	MoveParentCenter();
}

BOOL CWndGuildTabInfo::Initialize(CWndBase * pWndParent, DWORD /*dwWndId*/) {
	return CWndNeuz::InitDialog(APP_GUILD_TABINFO, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildTabInfo::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CGuild* pGuild = g_pPlayer->GetGuild();

	if( pGuild == NULL )
		return FALSE;
	
	// TODO: this returnfalse is fishy: does it mean that the titlebar is glitched?
	if( !pGuild->IsMaster(g_pPlayer->m_idPlayer) )
		return FALSE;

	switch( nID )
	{
	case WIDC_BUTTON4:		// ��� �̸� ����
		{
			if( strlen(pGuild->m_szGuild) == 0 )
			{
				CWndGuildName *pWndGuildName	= (CWndGuildName*)g_WndMng.GetWndBase( APP_GUILDNAME );
				if( !pWndGuildName )
				{
					pWndGuildName	= new CWndGuildName;
					pWndGuildName->Initialize( &g_WndMng );
				}
				pWndGuildName->SetData();
			}
			else
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDNOTCHGNAME ), MB_OK, this );
			
		}
		break;
	case WIDC_BUTTON1:		// ����
		{
			m_pwndGuildNotice = std::make_unique<CWndGuildNotice>();
			m_pwndGuildNotice->Initialize( this );
		}
		break;
	case WIDC_BUTTON2:		// �ΰ� ����
		{
			if( g_pPlayer->m_idWar != WarIdNone )
			{
				// �����߿� �ΰ� ���ٲ޴�.
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDWARERRORLOGO), MB_OK, this );
				return FALSE;
			}
			if constexpr (!useless_params::CanChangeLogo) {
				if (pGuild->m_dwLogo) {
					g_WndMng.OpenMessageBox(prj.GetText(TID_GAME_GUILDSTILLLOGO), MB_OK, this);
					return FALSE;
				}
			}
			if( pGuild->m_nLevel < 2 )
			{
				g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDNOTLEVEL ), MB_OK, this );
				return FALSE;
			}

			m_pwndGuildSetLogo = std::make_unique<CWndGuildSetLogo>();
			m_pwndGuildSetLogo->Initialize( this );
		}
		break;
	case WIDC_BUTTON3:		// ��� �ǰ���.
		{
			if( g_WndMng.m_pWndGuildBank )
			{
				return FALSE;
			}
			if( g_pPlayer->m_idWar != WarIdNone)
			{
				// �����߿� ��� ���ǰ��ϴ�.
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDWARERRORDISBAND), MB_OK, this );
				return FALSE;
			} else

			if( pGuild->GetStateOfQuest(QUEST_WARMON_LV1) == QS_BEGIN)	// Ŭ����ũ ����Ʈ�� ��� ��ü �ȵ�
			{
				QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( QUEST_WARMON_LV1 );
				if( pQuestProp )
				{
					g_WndMng.OpenMessageBox(prj.GetText( TID_GUILD_QUEST_LEAVEERROR ), MB_OK, this );
				}
				return FALSE;
			} 
			else
			{
				m_pwndGuildDisMiss = std::make_unique<CWndGuildDisMiss>();
				m_pwndGuildDisMiss->Initialize( this );
			}
		}
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
	

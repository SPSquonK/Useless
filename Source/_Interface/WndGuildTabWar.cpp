#include "stdafx.h"
#include "ResData.h"
#include "WndGuildTabWar.h"
#include "guildwar.h"
#include "defineText.h"
#include "MsgHdr.h"

#include "DPClient.h"

/****************************************************
  WndId : APP_GUILD_TABGUILDWAR - �����
  CtrlId : WIDC_STATIC1 - �������Ī :
  CtrlId : WIDC_STATIC2 - ������ΰ� :
  CtrlId : WIDC_STATIC3 - ������¸�Ƚ�� :
  CtrlId : WIDC_STATIC4 - ������й�Ƚ�� :
  CtrlId : WIDC_STATIC5 - ������׺�Ƚ�� :
  CtrlId : WIDC_CUSTOM1 - Custom
  CtrlId : WIDC_GUILDWARGUILDNAME - 
  CtrlId : WIDC_GUILDWARWIN - 
  CtrlId : WIDC_GUILDWARLOSE - 
  CtrlId : WIDC_GUILDWARGIVEUP - 
  CtrlId : WIDC_BUTTON1 - ���������
  CtrlId : WIDC_BUTTON2 - ������û
  CtrlId : WIDC_BUTTON3 - ������׺�
****************************************************/

void CWndGuildTabWar::OnDraw( C2DRender* p2DRender ) 
{ 
	if( !g_pPlayer )
		return;

	CGuildWar* pWar	= g_pPlayer->GetWar();
	if( !pWar )
		return;
	CWndBase* pWndBase	= GetDlgItem( WIDC_GUILDWARLEFTTIME );
	if( pWndBase )
	{
		CTimeSpan tsLeft	= pWar->GetEndTime() - CTime::GetCurrentTime();
		pWndBase->SetTitle( tsLeft.Format( "%H:%M:%S" ) );
	}
	
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	CGuild* pGuild = g_pPlayer->GetGuild();
	DWORD dwEnemyLogo = 0;
	u_long	idEnemyGuild = NULL_ID;

	if( pWar->m_Acpt.idGuild != pGuild->m_idGuild )		// Acpt�� Decl�� �����ϳ��� ������.
		idEnemyGuild = pWar->m_Acpt.idGuild;
	else
	if( pWar->m_Decl.idGuild != pGuild->m_idGuild )
		idEnemyGuild = pWar->m_Decl.idGuild;

	if( idEnemyGuild != NULL_ID )
	{
		CGuild *pEnemyGuild = g_GuildMng.GetGuild( idEnemyGuild );
		if( pEnemyGuild )
			dwEnemyLogo = pEnemyGuild->m_dwLogo;		// ������� �ΰ� �о.
		else
		{
			Error( "CWndGuildTabWar : ����带 ã���� ����." );
			return;
		}
	} else
	{
		Error( "CWndGuildTabWar : ����带 ã���� ����." );
		return;
	}
	

	if( pWndWorld && dwEnemyLogo )
	{
		LPWNDCTRL lpWndCtrl = GetWndCtrl( WIDC_CUSTOM1 );
		
		CPoint point = lpWndCtrl->rect.TopLeft();
		
		p2DRender->m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		p2DRender->m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		
		pWndWorld->m_pTextureLogo[dwEnemyLogo-1].Render( &g_Neuz.m_2DRender, point, 255 );
		
		p2DRender->m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		p2DRender->m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
} 

void CWndGuildTabWar::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	UpdateData();

	// �̱� ��ư �̹��� ����
	CWndButton* pWndButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
	if(pWndButton)
	{
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildDuel.bmp" ), 0xffff00ff );
	}

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 

BOOL CWndGuildTabWar::Initialize(CWndBase * pWndParent, DWORD /*dwWndId*/) {
	return CWndNeuz::InitDialog(APP_GUILD_TABGUILDWAR, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildTabWar::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild == NULL )
		return FALSE;

	switch( nID )
	{
	case WIDC_BUTTON1:		// ����� ����
		if( !pGuild->IsMaster(g_pPlayer->m_idPlayer) )
			return FALSE;

		m_pWndGuildWarDecl = std::make_unique<CWndGuildWarDecl>();
		m_pWndGuildWarDecl->Initialize( this );
		break;
	case WIDC_BUTTON2:		// ����� ����
		if( !pGuild->IsMaster(g_pPlayer->m_idPlayer) )
			return FALSE;

		m_pWndGuildWarPeace = std::make_unique<CWndGuildWarPeace>();
		m_pWndGuildWarPeace->Initialize( this );
		break;
	case WIDC_BUTTON3:		// ����� �׺�
		m_pWndGuildWarGiveUp = std::make_unique<CWndGuildWarGiveUp>();
		m_pWndGuildWarGiveUp->Initialize( this );
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndGuildTabWar::UpdateData( void )
{
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
	{
		CString strText;
		CWndBase *pWndText;

		CGuild *pEnemyGuild = g_GuildMng.GetGuild( pGuild->m_idEnemyGuild );
		
		pWndText = GetDlgItem( WIDC_GUILDWARGUILDNAME );
		if( pEnemyGuild )
			pWndText->SetTitle( pEnemyGuild->m_szGuild );		// ����
		else
			pWndText->SetTitle( "" );

		pWndText	= GetDlgItem( WIDC_GUILDWARLEFTTIME );
		pWndText->SetTitle( "" );

		pWndText = GetDlgItem( WIDC_GUILDWARWIN );
		strText.Format( prj.GetText( TID_GAME_GUILD_WAR_DATA_WIN ), pGuild->m_nWin );
		pWndText->SetTitle( strText );		// x ��
		pWndText = GetDlgItem( WIDC_GUILDWARLOSE );
		strText.Format( prj.GetText( TID_GAME_GUILD_WAR_DATA_LOSE ), pGuild->m_nLose );
		pWndText->SetTitle( strText );		// x ��
		pWndText = GetDlgItem( WIDC_GUILDWARGIVEUP );
		strText.Format( prj.GetText( TID_GAME_GUILD_WAR_DATA_SURRENDER ), pGuild->m_nSurrender );
		pWndText->SetTitle( strText );		// x ȸ
	}
}


/****************************************************
	WndId : APP_GUILD_WARREQUEST - �������ûâ
	CtrlId : WIDC_EDIT1 -
	CtrlId : WIDC_YES - Button
	CtrlId : WIDC_NO - No
****************************************************/

void CWndGuildWarRequest::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();

	CGuild * pEnemyGuild = g_GuildMng.GetGuild(m_idEnemyGuild);
	if (pEnemyGuild) {
		CWndEdit * pWndEdit = GetDlgItem<CWndEdit>(WIDC_EDIT1);

		CString strText;
		strText.Format(prj.GetText(TID_GAME_GUILDWARREQUEST), pEnemyGuild->m_szGuild, m_szMaster);
		pWndEdit->SetString(strText);
		pWndEdit->EnableWindow(FALSE);
	}

	MoveParentCenter();
}

BOOL CWndGuildWarRequest::Initialize(CWndBase * pWndParent, DWORD) {
	return CWndNeuz::InitDialog(APP_GUILD_WARREQUEST, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildWarRequest::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_YES:
			g_DPlay.SendPacket<PACKETTYPE_ACPT_GUILD_WAR, u_long>(m_idEnemyGuild);
			Destroy();
			break;
		case WIDC_NO:
			Destroy();
			break;
	}

	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}


/****************************************************
	WndId : APP_GUILD_WARPEACECONFIRM - ��������â
	CtrlId : WIDC_YES - Yes
	CtrlId : WIDC_NO - No
	CtrlId : WIDC_STATIC1 - ������û�� ���Խ��ϴ�.
	CtrlId : WIDC_STATIC2 - �����Ͻðڽ��ϱ�?
****************************************************/

void CWndGuildWarPeaceConfirm::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
}

BOOL CWndGuildWarPeaceConfirm::Initialize(CWndBase * pWndParent, DWORD) {
	return CWndNeuz::InitDialog(APP_GUILD_WARPEACECONFIRM, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildWarPeaceConfirm::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_YES:
			g_DPlay.SendPacket<PACKETTYPE_ACPT_TRUCE>();
			Destroy();
			break;
		case WIDC_NO:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}


/****************************************************
	WndId : APP_GUILD_WARPEACE - ����â
	CtrlId : WIDC_YES - Yes
	CtrlId : WIDC_NO - No
	CtrlId : WIDC_STATIC1 - ����忡 ������ ��û�ϰڽ��ϱ�?
****************************************************/

void CWndGuildWarPeace::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
}

BOOL CWndGuildWarPeace::Initialize(CWndBase * pWndParent, DWORD /*dwWndId*/) {
	return CWndNeuz::InitDialog(APP_GUILD_WARPEACE, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildWarPeace::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_YES:
			g_DPlay.SendPacket<PACKETTYPE_QUERY_TRUCE>();
			Destroy();
			break;
		case WIDC_NO:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

/****************************************************
	WndId : APP_GUILD_WARGIVEUP - �׺�â
	CtrlId : WIDC_YES - Button
	CtrlId : WIDC_NO - Button
	CtrlId : WIDC_STATIC1 - ��������� �׺��� �ϰڽ��ϱ�?
****************************************************/

void CWndGuildWarGiveUp::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
}

BOOL CWndGuildWarGiveUp::Initialize(CWndBase * pWndParent, DWORD /*dwWndId*/) {
	return CWndNeuz::InitDialog(APP_GUILD_WARGIVEUP, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildWarGiveUp::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_YES:
			g_DPlay.SendPacket<PACKETTYPE_SURRENDER>();
			Destroy();
			break;
		case WIDC_NO:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}



/****************************************************
	WndId : APP_GUILD_WAR - ���������â
	CtrlId : WIDC_OK - Button
	CtrlId : WIDC_CANCEL - Button
	CtrlId : WIDC_STATIC1 - �����
	CtrlId : WIDC_STATIC2 - �������
	CtrlId : WIDC_EDIT1 -
	CtrlId : WIDC_EDIT2 -
****************************************************/


void CWndGuildWarDecl::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();

	CWndEdit * pWndName = (CWndEdit *)GetDlgItem(WIDC_EDIT1);	// ������.
	CWndEdit * pWndPenya = (CWndEdit *)GetDlgItem(WIDC_EDIT2);	// �����ڱ�.

	pWndName->SetTabStop(TRUE);
	pWndPenya->SetTabStop(TRUE);
	pWndName->SetFocus();

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
}

BOOL CWndGuildWarDecl::Initialize(CWndBase * pWndParent, DWORD) {
	return CWndNeuz::InitDialog(APP_GUILD_WAR, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildWarDecl::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_OK:
		{
			CWndEdit * pWndEdit = (CWndEdit *)GetDlgItem(WIDC_EDIT1);
			if (g_pPlayer) {
				CString strGuild = pWndEdit->GetString();
				if (strGuild.GetLength() >= 3 && strGuild.GetLength() < MAX_G_NAME) {
					strGuild.Trim();
					g_DPlay.SendDeclWar(g_pPlayer->m_idPlayer, strGuild.GetString());
					Destroy();
				}
			}
		}
		break;
		case WIDC_CANCEL:
			Destroy();
			break;
	}

	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

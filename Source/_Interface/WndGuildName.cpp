#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndGuildName.h"
#include "WndManager.h"
#include "DPClient.h"


/****************************************************
  WndId : APP_GUILDNAME - ������â
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_STATIC1 - ����Ī�� �Է����ּ���.
  CtrlId : WIDC_OK - Button
  CtrlId : WIDC_CANCEL - Button
****************************************************/

CWndGuildName::CWndGuildName() {
	m_nId = 0xff;
}

void CWndGuildName::OnDraw(C2DRender * p2DRender) {
#ifdef __S_SERVER_UNIFY
	if (g_WndMng.m_bAllAction == FALSE) {
		MoveParentCenter();
	}
#endif // __S_SERVER_UNIFY
}

void CWndGuildName::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CGuild* pGuild = g_pPlayer->GetGuild();
	if (!pGuild) {
		Destroy();
		return;
	}

	CWndEdit * pWndEdit = GetDlgItem<CWndEdit>(WIDC_EDIT1);
	pWndEdit->SetString( pGuild->m_szGuild );		// ����Ʈ �̸��� ����Ʈ �ڽ��� �Է���.


	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildName::Initialize(CWndBase * pWndParent, DWORD) {
	return CWndNeuz::InitDialog(APP_GUILDNAME, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndGuildName::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK )
	{
		CWndEdit *pWndEdit = (CWndEdit *)GetDlgItem( WIDC_EDIT1 );
		LPCTSTR szName = pWndEdit->GetString();

		// �̰����� szName�� ������ ������ �ڵ带 �����ÿ�.
		const auto guildName = CheckGuildName(szName);

		if (guildName.has_value()) {
			if (m_nId == 0xff)
				g_DPlay.SendGuildSetName(guildName->GetString());
			else
				g_DPlay.SendQuerySetGuildName(guildName->GetString(), m_nId);

			Destroy();
		} else {
			const GuildNameError error = guildName.error();

			switch (error) {
				case GuildNameError::TooShort:
				case GuildNameError::TooLong:
					g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0011)));
					break;
				case GuildNameError::DigitLead:
					g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0012)));
					break;
				case GuildNameError::BadEUCKRSymbol:
					g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0014)));
					break;
				case GuildNameError::BadSymbol:
					g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0013)));
					break;
				case GuildNameError::UnallowedName:
					g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0020)));
					break;
			}

			return TRUE;
		}
	}
	else if( nID == WIDC_CANCEL || nID == WTBID_CLOSE )
	{
#ifdef __S_SERVER_UNIFY
		if( g_WndMng.m_bAllAction == FALSE )
			return TRUE;
#endif // __S_SERVER_UNIFY
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


/****************************************************
  WndId : APP_GUILD_NICKNAME - ��庰Ī����
  CtrlId : WIDC_OK - OK
  CtrlId : WIDC_CANCEL - Cancel
  CtrlId : WIDC_STATIC1 - ������ �� ��Ī�� �Է����ּ���.
  CtrlId : WIDC_EDIT1 - 
****************************************************/

CWndGuildNickName::CWndGuildNickName() 
{ 
	m_idPlayer = 0;
} 

void CWndGuildNickName::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildNickName::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_GUILD_NICKNAME, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndGuildNickName::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{
	if( nID == WIDC_OK )
	{
		CWndEdit *pEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
		CString strNickName = pEdit->GetString();
		
		strNickName.TrimLeft();
		strNickName.TrimRight();
		LPCTSTR lpszString = strNickName;
		if( strNickName.IsEmpty() == FALSE )
		{
			const DWORD error = CheckGuildNickName(strNickName);
			if (error != 0) {
				g_WndMng.OpenMessageBox(_T(prj.GetText(error)));
				return TRUE;
			}

			CGuild* pGuild = g_pPlayer->GetGuild();
			if( pGuild )
			{
				CGuildMember* pGuildMember = pGuild->GetMember( g_pPlayer->m_idPlayer );
				if( pGuildMember )
				{
					if( strcmp( pGuildMember->m_szAlias, strNickName ) )
					{
						g_DPlay.SendGuildNickName( m_idPlayer, strNickName );
						Destroy();	
					}
				}
			}
		}
		else
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0011) ) );
		}
	}
	else if( nID == WIDC_CANCEL )
	{
		Destroy();
		//		g_WndMng.OpenMessageBox( _T( "��ȸ�ش� ��Ī�� ���ؾ� �մϴ�." ) );
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}
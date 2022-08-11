#include "stdafx.h"
#include <ranges>
#include "defineText.h"
#include "AppDefine.h"
#include "WndGuild.h"
#include "WndGuildTabMember.h"
#include "WndManager.h"
#include "DPClient.h"
#include "MsgHdr.h"
#include "defineQuest.h"
#include "playerdata.h"

#include "guild.h"

#include "eveschool.h"


/****************************************************
  WndId : APP_GUILD_TABMEMBER 
****************************************************/
#define		MAX_MEMBER_LIST_DRAW	14		// ��ȭ�鿡 ���̴� ����Ʈ ��.
#define		MEMBER_LIST_HEIGHT		18		// ����Ʈ�� �Ѷ��� �� y += MEMBER_LIST_HEIGHT �� ����.

//////////////////////////////////////////////////////////////////////////
// Common Local Func.
//////////////////////////////////////////////////////////////////////////
bool prMemberLevelAsce(MEMBERLIST player1, MEMBERLIST player2)
{
	bool rtn_val = false;

	if(player1.nMemberLv > player2.nMemberLv)
		rtn_val = true;
	else if(player1.nMemberLv == player2.nMemberLv)
	{
		if(player1.nClass < player2.nClass)
			rtn_val = true;
	}
	
	return rtn_val;
}

bool prMemberLevelDesc(MEMBERLIST player1, MEMBERLIST player2)
{
	bool rtn_val = false;

	if(player1.nMemberLv < player2.nMemberLv)
		rtn_val = true;
	else if(player1.nMemberLv == player2.nMemberLv)
	{
		if(player1.nClass > player2.nClass)
			rtn_val = true;
	}
	
	return rtn_val;
}

bool prJobAsce(MEMBERLIST player1, MEMBERLIST player2) {
	const auto nPlayer1JobType = prj.jobs.info[player1.nJob].dwJobType;
	const auto nPlayer2JobType = prj.jobs.info[player2.nJob].dwJobType;

	if (nPlayer1JobType < nPlayer2JobType) return false;
	if (nPlayer1JobType > nPlayer2JobType) return true;
	return player1.nJob > player2.nJob;
}

bool prJobDesc(MEMBERLIST player1, MEMBERLIST player2) {
	const auto nPlayer1JobType = prj.jobs.info[player1.nJob].dwJobType;
	const auto nPlayer2JobType = prj.jobs.info[player2.nJob].dwJobType;

	if (nPlayer1JobType < nPlayer2JobType) return true;
	if (nPlayer1JobType > nPlayer2JobType) return false;
	return player1.nJob < player2.nJob;
}

bool prLevelAsce(MEMBERLIST player1, MEMBERLIST player2) {
	const auto nPlayer1JobType = prj.jobs.info[player1.nJob].dwJobType;
	const auto nPlayer2JobType = prj.jobs.info[player2.nJob].dwJobType;

	if (nPlayer1JobType > nPlayer2JobType) return true;
	if (nPlayer1JobType < nPlayer2JobType) return false;
	if (player1.nLevel > player2.nLevel) return true;
	return false;
}

bool prLevelDesc(MEMBERLIST player1, MEMBERLIST player2) {
	const auto nPlayer1JobType = prj.jobs.info[player1.nJob].dwJobType;
	const auto nPlayer2JobType = prj.jobs.info[player2.nJob].dwJobType;

	if (nPlayer1JobType < nPlayer2JobType) return true;
	if (nPlayer1JobType > nPlayer2JobType) return false;
	if (player1.nLevel < player2.nLevel) return true;
	return false;
}

bool prNameAsce(MEMBERLIST player1, MEMBERLIST player2) {
	CString strplayer1Name = player1.szName;
	CString strplayer2Name = player2.szName;

	return strplayer1Name > strplayer2Name;
}

bool prNameDesc(MEMBERLIST player1, MEMBERLIST player2)
{
	CString strplayer1Name = player1.szName;
	CString strplayer2Name = player2.szName;

	return strplayer1Name < strplayer2Name;
}

bool prAliasAsce(MEMBERLIST player1, MEMBERLIST player2)
{
	bool rtn_val = false;
	CString strplayer1Name, strplayer2Name;

	strplayer1Name.Format("%s", player1.szAlias);
	strplayer2Name.Format("%s", player2.szAlias);

	if(strplayer1Name > strplayer2Name)
		rtn_val = true;
	
	return rtn_val;
}

bool prAliasDesc(MEMBERLIST player1, MEMBERLIST player2)
{
	bool rtn_val = false;
	CString strplayer1Name, strplayer2Name;

	strplayer1Name.Format("%s", player1.szAlias);
	strplayer2Name.Format("%s", player2.szAlias);

	if(strplayer1Name < strplayer2Name)
		rtn_val = true;
	
	return rtn_val;
}

CWndGuildTabMember::CWndGuildTabMember() 
{ 
	m_nMax = 0;
	m_nCurrentList = 0;
	m_nSelect = 0;
	memset( m_aList, 0, sizeof(MEMBERLIST) * MAX_MEMBER );
	m_nMxOld = m_nMyOld = 0;
	m_bSortbyMemberLevel = FALSE;
	m_bSortbyLevel = TRUE;
	m_bSortbyJob = TRUE;
	m_bSortbyName = TRUE;
	m_bSortbyAlias = TRUE;
} 

void CWndGuildTabMember::OnDraw( C2DRender* p2DRender ) 
{ 
	int nPage, nRange;
	nPage = MAX_MEMBER_LIST_DRAW;
	nRange = m_nMax;
	m_wndScrollBar.SetScrollRange( 0, nRange );
	m_wndScrollBar.SetScrollPage( nPage );
	if( m_nMax - m_wndScrollBar.GetScrollPos() + 1 > m_wndScrollBar.GetScrollPage() )
		m_nCurrentList = m_wndScrollBar.GetScrollPos(); 
	if( m_nMax < m_wndScrollBar.GetScrollPos() )
		m_nCurrentList = 0;
	int		i, sx, sy;
	int		nData;
	MEMBERLIST *pMember;
	DWORD	dwColor;
	
	sx = 8;
	sy = 32;	
	
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	// ����, ������ ������ �߰������� vertex �ϳ� ����
	TEXTUREVERTEX2* pVertex = new TEXTUREVERTEX2[ ((m_nMax > MAX_MEMBER_LIST_DRAW) ? MAX_MEMBER_LIST_DRAW: m_nMax) * 6 * 5 ];

	TEXTUREVERTEX2* pVertices = pVertex;

	for( i = m_nCurrentList; i < m_nCurrentList + MAX_MEMBER_LIST_DRAW; i ++ )
	{

		if( i >= m_nMax )	break;
		pMember = &(m_aList[i]);
		nData = pMember->nMemberLv;			
		if( i == m_nSelect )
		{
			dwColor = D3DCOLOR_ARGB( 255, 0, 0, 255 );
		}
		else
		{
			if( pMember->bIsOnLine )
				dwColor = 0xff000000;
			else
				dwColor = 0xFF909090;
		}

		int j;
		int nClass = pMember->nClass;
		++nClass;

		int nposx = sx + 14;
		if(nClass == 2)
			nposx -= 5;
		else if(nClass == 3)
			nposx -= 10;

		switch( pMember->nMemberLv )
		{
		case 0:
			pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( nposx, sy - 4 ), 44, &pVertices, 0xffffffff );
			break;
		case 1:
			{
				for( j = 0 ; j < nClass ; ++j )
				{
					pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( nposx + j * 10, sy - 4 ), 45, &pVertices, 0xffffffff );
				}
			}
			break;
		case 2:
			{
				for( j = 0 ; j < nClass ; ++j )
				{
					pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( nposx + j * 10, sy - 4 ), 46, &pVertices, 0xffffffff );
				}
			}
			break;
		case 3:
			{
				for( j = 0 ; j < nClass ; ++j )
				{
					pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( nposx + j * 10, sy - 4 ), 47, &pVertices, 0xffffffff );
				}
			}
			break;
		case 4:
			{
				for( j = 0 ; j < nClass ; ++j )
				{
					pWndWorld->m_texPlayerDataIcon.MakeVertex( p2DRender, CPoint( nposx + j * 10, sy - 4 ), 48, &pVertices, 0xffffffff );
				}
			}
			break;
		default:
			break;
		}

		DWORD Color;

		if( pMember->bIsOnLine )
			Color = 0xffffffff;
		else
			Color = 0xffff6464;
		// Draw Job Icon
		const auto jobIcons = Project::Jobs::PlayerDataIcon(pMember->nJob, pMember->nLevel);
		if (jobIcons.master != 0) {
			pWndWorld->m_texPlayerDataIcon.MakeVertex(p2DRender, CPoint(sx + 64, sy - 3), jobIcons.master, &pVertices, Color);
		}
		pWndWorld->m_texPlayerDataIcon.MakeVertex(p2DRender, CPoint(sx + 84, sy - 3), jobIcons.job, &pVertices, Color);

		p2DRender->TextOut( sx + 126, sy, pMember->nLevel, dwColor );
		
		const CString strFormat = sqktd::CStringMaxSize(pMember->szName, 10);
		p2DRender->TextOut( sx + 168, sy, strFormat, dwColor );
		p2DRender->TextOut( sx + 264, sy, pMember->szAlias , dwColor );

		sy += MEMBER_LIST_HEIGHT;
	}

	pWndWorld->m_texPlayerDataIcon.Render( m_pApp->m_pd3dDevice, pVertex, ( (int) pVertices - (int) pVertex ) / sizeof( TEXTUREVERTEX2 ) );
	
	//	delete pVertex;
	SAFE_DELETE_ARRAY( pVertex );
} 
void CWndGuildTabMember::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	m_wndMenu.CreateMenu( this );	
	
//	CString strMember;
	CRect rect = GetWindowRect();
	rect.top += 40;
	rect.bottom	-= 10 ;
	rect.left -= 30;
	rect.right -= 5;
	m_wndScrollBar.AddWndStyle( WBS_DOCKING );
	m_wndScrollBar.Create( WBS_VERT, rect, this, 1000 );//,m_pSprPack,-1);

	int nPage, nRange;
	nPage = MAX_MEMBER_LIST_DRAW;
	nRange = m_nMax;//m_pItemContainer->m_dwIndexNum;// - nPage;
	m_wndScrollBar.SetScrollRange( 0, nRange );
	m_wndScrollBar.SetScrollPage( nPage );

	// ��ư �̹��� ����
	CWndButton* pWndButton = (CWndButton*)GetDlgItem(WIDC_MLEVEL);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN )
//		else
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabMLevel.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabMLevel.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_JOB);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN )
//		else
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabJob.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabJob.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_LEVEL);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN )
//		else
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabLevel.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabLevel.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_NAME);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN )
//		else
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabChar.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabChar.bmp" ), 0xffff00ff );
	}

	pWndButton = (CWndButton*)GetDlgItem(WIDC_ALIAS);
	if(pWndButton)
	{
//		if(::GetLanguage() == LANG_KOR || ::GetLanguage() == LANG_FRE || ::GetLanguage() == LANG_TWN )
//		else
		if(::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN)
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabAlias.bmp" ), 0xffff00ff );
		else
			pWndButton->SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "ButtGuildMemberTabAlias.bmp" ), 0xffff00ff );
	}

	UpdateData();

	SortbyMemberLevel();	// Default Sort

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildTabMember::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_GUILD_TABMEMBER_EX, pWndParent, 0, CPoint( 0, 0 ) );
} 
BOOL CWndGuildTabMember::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	if( nID == 1000 && dwMessage == 514 )	// Scroll Bar
	{
		int adf = m_wndScrollBar.GetScrollPos();
		int bbf = m_wndScrollBar.GetScrollRange();
		return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
	}
	int a = 0;

	CMover* pMover	= (CMover*)CMover::GetActiveObj();
	if( !pMover )
		return FALSE;
	CGuild* pGuild	= pMover->GetGuild();
	if( !pGuild )
		return FALSE;
	if( m_nSelect < 0 || m_nSelect >= m_nMax )
		return FALSE;

	if( g_pPlayer->m_idWar != WarIdNone)
	{
		g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDWARERRORMEMBER ) );
		return FALSE;
	}

	if( pWndBase == &m_wndMenu )
	{
		switch( nID )
		{
		case MGI_APPELL_UP:		// ȣĪ ��.
			g_DPlay.SendGuildMemberLv( pMover->m_idPlayer,
				m_aList[m_nSelect].idPlayer, m_aList[m_nSelect].nMemberLv - 1 );
			break;
		case MGI_APPELL_DOWN:
			g_DPlay.SendGuildMemberLv( pMover->m_idPlayer,
				m_aList[m_nSelect].idPlayer, m_aList[m_nSelect].nMemberLv + 1 );
			break;

		case MGI_CLASS_UP:		// ��� ��
			g_DPlay.SendGuildClass( pMover->m_idPlayer,
				m_aList[m_nSelect].idPlayer, 1 );
			break;
		case MGI_CLASS_DOWN:	
			g_DPlay.SendGuildClass( pMover->m_idPlayer,
				m_aList[m_nSelect].idPlayer, 0 );
			break;
		case MGI_CHG_MASTER:
			{
				if( m_aList[m_nSelect].idPlayer != g_pPlayer->m_idPlayer )
				{
					if( m_aList[m_nSelect].bIsOnLine )
						g_DPlay.SendChgMaster( pMover->m_idPlayer, m_aList[m_nSelect].idPlayer );
					else
						g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_NOTLOGIN ), MB_OK, this );
				}
			}
			break;
		case MGI_NICKNAME:		// ��Ī �ο�
			{
				CGuild* pGuild = g_pPlayer->GetGuild();
				if( pGuild && pGuild->IsMaster( g_pPlayer->m_idPlayer ) )
				{
					if( 10 > pGuild->m_nLevel )
					{
						g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_GUILDNOTLEVEL) ) );	
					}
					else
					{
						SAFE_DELETE( g_WndMng.m_pWndGuildNickName );
						g_WndMng.m_pWndGuildNickName = new CWndGuildNickName;
						g_WndMng.m_pWndGuildNickName->m_idPlayer = m_aList[m_nSelect].idPlayer;
						g_WndMng.m_pWndGuildNickName->Initialize( &g_WndMng, APP_GUILD_NICKNAME );
					}
				}
				else
				{
					g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDONLYMASTER ), MB_OK, this );
				}
				
			}
			break;

		case MGI_GUILD_LEAVE:	// Ż��
			{
				if(  pGuild->GetStateOfQuest( QUEST_WARMON_LV1 ) == QS_BEGIN )	// Ŭ����ũ ����Ʈ�� ��� ��ü �ȵ�
				{
					QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( QUEST_WARMON_LV1 );
					if( pQuestProp )
					{
						g_WndMng.OpenMessageBox( prj.GetText( TID_GUILD_QUEST_LEAVEERROR ), MB_OK, this );
					}
					return FALSE;
				}
				else
				{
					if( g_GuildCombatMng.m_bRequest && g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE && g_GuildCombatMng.m_nGCState != CGuildCombat::WAR_CLOSE_STATE )
					//if( g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE && g_GuildCombatMng.m_nGCState != CGuildCombat::WAR_CLOSE_STATE )
						g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDCOMBAT_NOT_LEAVE_GUILD ) );		//"�����ؾ��� : �������� ��û�ѱ��� �����߿��� ��� Ż�� �Ҽ� �����ϴ�" );
					else if( g_GuildCombat1to1Mng.m_nState != CGuildCombat1to1Mng::GC1TO1_CLOSE )
						g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDCOMBAT1TO1_NOTLEAVEGUILD ) );
					else
					{
						g_DPlay.SendRemoveGuildMember( pMover->m_idPlayer, m_aList[m_nSelect].idPlayer );
					}
				}
			}
			break;
		} // switch nID
	}
		
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildTabMember::OnSize( UINT nType, int cx, int cy ) \
{ 
	CRect rect = GetWindowRect();
	rect.top = rect.top + 40;
	rect.bottom	= rect.bottom - 10 ;
	rect.left = rect.right - 40;
	rect.right = rect.right - 25;
	m_wndScrollBar.SetWndRect( rect );
	
	int nPage, nRange;
	nPage = MAX_MEMBER_LIST_DRAW;
	nRange = m_nMax;//m_pItemContainer->m_dwIndexNum;// - nPage;
	m_wndScrollBar.SetScrollRange( 0, nRange );
	m_wndScrollBar.SetScrollPage( nPage );
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildTabMember::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildTabMember::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	int mx = point.x - 8;
	int my = point.y - 32;
	m_nSelect = m_nCurrentList + my / MEMBER_LIST_HEIGHT;
	if( m_nSelect < 0 )
		m_nSelect = 0;
	if( m_nSelect >= m_nMax )
		m_nSelect = m_nMax - 1;
} 

void CWndGuildTabMember::OnRButtonDown( UINT nFlags, CPoint point ) 
{ 
	OnLButtonDown( nFlags, point );
} 

void CWndGuildTabMember::OnRButtonUp( UINT nFlags, CPoint point ) 
{ 
	m_wndMenu.DeleteAllMenu();
	m_wndMenu.AppendMenu( 0, MGI_APPELL_UP,		prj.GetText( TID_GAME_GUILD_APPELLATIONUP ) );
	m_wndMenu.AppendMenu( 0, MGI_APPELL_DOWN,	prj.GetText( TID_GAME_GUILD_APPELLATIONDOWN ) );
	m_wndMenu.AppendMenu( 0, MGI_CLASS_UP,		prj.GetText( TID_GAME_GUILD_CLASSUP ) );
	m_wndMenu.AppendMenu( 0, MGI_CLASS_DOWN,	prj.GetText( TID_GAME_GUILD_CLASSDOWN ) );
	m_wndMenu.AppendMenu( 0, MGI_NICKNAME,		prj.GetText( TID_GAME_GUILD_NICKNAME ) );
	m_wndMenu.AppendMenu( 0, MGI_GUILD_LEAVE,	prj.GetText( TID_GAME_GUILD_LEAVE ) );
	CMover* pMover	= CMover::GetActiveMover();
	if( pMover )
	{
		CGuild* pGuild	= pMover->GetGuild();
		if( pGuild && pGuild->IsMaster( pMover->m_idPlayer ) )
			m_wndMenu.AppendMenu( 0, MGI_CHG_MASTER,	prj.GetText( TID_GAME_CHG_MASTER ) );
	}

	m_wndMenu.Move( CPoint( m_rectCurrentWindow.left, m_rectCurrentWindow.top ) + point );
	m_wndMenu.SetVisible( TRUE );//!m_wndMenuMover.IsVisible() );
	m_wndMenu.SetFocus();
	
} 

BOOL CWndGuildTabMember::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	m_wndMenu.SetVisible( FALSE );

	switch(nID)
	{
		case WIDC_MLEVEL:
			SortbyMemberLevel();
			break;
		case WIDC_JOB:
			SortbyJob();
			break;
		case WIDC_LEVEL:
			SortbyLevel();
			break;
		case WIDC_NAME:
			SortbyName();
			break;
		case WIDC_ALIAS:
			SortbyAlias();
			break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndGuildTabMember::OnMouseMove(UINT nFlags, CPoint point )
{
	if( nFlags & MK_LBUTTON )
	{
		int		nDistY = (m_nMyOld - point.y) / 5;		// ���� ��ǥ���� ����.

		m_nCurrentList += nDistY;
		if( m_nCurrentList < 0 )
			m_nCurrentList = 0;
		if( (m_nCurrentList + MAX_MEMBER_LIST_DRAW - 1) >= m_nMax )
		{
			m_nCurrentList = m_nMax - MAX_MEMBER_LIST_DRAW;
			if( m_nCurrentList < 0 )
				m_nCurrentList = 0;
		}
	
	}

	m_nMxOld = point.x;
	m_nMyOld = point.y;
}
void CWndGuildTabMember::OnMouseWndSurface( CPoint point )
{
	int mx = point.x - 8;
	int my = point.y - 32;
	int nSelect = m_nCurrentList + my / MEMBER_LIST_HEIGHT;

	if( nSelect < 0 )
		nSelect = 0;
	if( nSelect >= m_nMax )
		nSelect = m_nMax - 1;
	
	int nMax = (m_nMax > MAX_MEMBER_LIST_DRAW) ? MAX_MEMBER_LIST_DRAW : m_nMax;

	int offset_y = 30;

	for( int i=0; i<nMax; i++ )
	{
		CRect rect = CRect( 4,offset_y,m_rectLayout.right - m_rectLayout.left, (offset_y+15) );
		offset_y+=MEMBER_LIST_HEIGHT;

		if( PtInRect( &rect, point ) )
		{
			CRect rect2 = CRect( point.x, point.y, point.x+200, point.y+20 );
			CPoint point2 = point;
			ClientToScreen( &point2 );
			ClientToScreen( &rect2 );

			CString str;

			str.Format( "%s\n%s %u\n%s %d\n%s %d", m_aList[ nSelect ].szName,
											   prj.GetText(TID_GAME_TOOLTIP_EXPMERIT ), m_aList[ nSelect ].dwGivePxpCount,
				                               prj.GetText(TID_GAME_TOOLTIP_PENYAMERIT ), m_aList[ nSelect ].nGiveGold,
											   prj.GetText(TID_GAME_TOOLTIP_GIVEUPNUM ), m_aList[ nSelect ].nLose );

			g_toolTip.PutToolTip( m_nIdWnd, str, rect2, point2 );
			break;
		}
	}
}	

BOOL CWndGuildTabMember::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	int		nZoom = 64;
	
//    if( nFlags & MK_SHIFT )
//		nZoom /= 8;
	if( zDelta > 0 )
		m_nCurrentList -= 3;
	else
		m_nCurrentList += 3;

	if( m_nCurrentList < 0 )
		m_nCurrentList = 0;
	if( (m_nCurrentList + MAX_MEMBER_LIST_DRAW - 1) >= m_nMax )
	{
		m_nCurrentList = m_nMax - MAX_MEMBER_LIST_DRAW;
		if( m_nCurrentList < 0 )
			m_nCurrentList = 0;
	}

	m_wndScrollBar.SetScrollPos( m_nCurrentList );
	return TRUE;
}


void CWndGuildTabMember::UpdateData()
{
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
	{
		m_nMax = 0;

		for(const CGuildMember * const pMember : pGuild->m_mapPMember | std::views::values) {

			m_aList[ m_nMax ].idPlayer       = pMember->m_idPlayer;
			m_aList[ m_nMax ].nMemberLv      = pMember->m_nMemberLv;			// ȣĪ
			PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
			m_aList[ m_nMax ].nJob	= pPlayerData->data.nJob;
			m_aList[ m_nMax ].nSex	= pPlayerData->data.nSex;
			m_aList[ m_nMax ].nLevel	= pPlayerData->data.nLevel;	// ����
			m_aList[ m_nMax ].bIsOnLine      = ( pPlayerData->data.uLogin > 0 );

			m_aList[ m_nMax ].nGiveGold      = pMember->m_nGiveGold;					// ��忡 ����� ���
			m_aList[ m_nMax ].dwGivePxpCount = pMember->m_dwGivePxpCount;				// ��忡 ����� PXPȽ��( ��ų ����ġ )
			m_aList[ m_nMax ].nWin           = pMember->m_nWin;							// ������ �̰���?
			m_aList[ m_nMax ].nLose          = pMember->m_nLose;						// ������ �����?
		
			LPCSTR pszPlayer	= pPlayerData->szPlayer;
			lstrcpy( m_aList[m_nMax].szName, pszPlayer );
			strcpy( m_aList[ m_nMax ].szAlias, pMember->m_szAlias );
			m_aList[ m_nMax ].nClass		= pMember->m_nClass;

			m_nMax ++;
		}
	}
	else
	{
		m_nMax = 0;
		m_nCurrentList = 0;
		m_nSelect = 0;
		memset( m_aList, 0, sizeof(m_aList) );
		m_nMxOld = m_nMyOld = 0;
	}		
}


void CWndGuildTabMember::SortbyMemberLevel()
{
	if(m_nMax > 1)
	{
		if(m_bSortbyMemberLevel)
		{
			std::sort( m_aList, m_aList + m_nMax, prMemberLevelAsce );
			m_bSortbyMemberLevel = FALSE;
		}
		else
		{
			std::sort( m_aList, m_aList + m_nMax, prMemberLevelDesc );
			m_bSortbyMemberLevel = TRUE;
		}
	}
}

void CWndGuildTabMember::SortbyJob()
{
	if(m_nMax > 1)
	{
		if(m_bSortbyJob)
		{
			std::sort( m_aList, m_aList + m_nMax, prJobAsce );
			m_bSortbyJob = FALSE;
		}
		else
		{
			std::sort( m_aList, m_aList + m_nMax, prJobDesc );
			m_bSortbyJob = TRUE;
		}
	}
}

void CWndGuildTabMember::SortbyLevel()
{
	if(m_nMax > 1)
	{
		if(m_bSortbyLevel)
		{
			std::sort( m_aList, m_aList + m_nMax, prLevelAsce );
			m_bSortbyLevel = FALSE;
		}
		else
		{
			std::sort( m_aList, m_aList + m_nMax, prLevelDesc );
			m_bSortbyLevel = TRUE;
		}
	}
}

void CWndGuildTabMember::SortbyName()
{
	if(m_nMax > 1)
	{
		if(m_bSortbyName)
		{
			std::sort( m_aList, m_aList + m_nMax, prNameAsce );
			m_bSortbyName = FALSE;
		}
		else
		{
			std::sort( m_aList, m_aList + m_nMax, prNameDesc );
			m_bSortbyName = TRUE;
		}
	}
}

void CWndGuildTabMember::SortbyAlias()
{
	if(m_nMax > 1)
	{
		if(m_bSortbyAlias)
		{
			std::sort( m_aList, m_aList + m_nMax, prAliasAsce );
			m_bSortbyAlias = FALSE;
		}
		else
		{
			std::sort( m_aList, m_aList + m_nMax, prAliasDesc );
			m_bSortbyAlias = TRUE;
		}
	}
}


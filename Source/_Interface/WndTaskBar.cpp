#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "DialogMsg.h"
#include "WndField.h"
#include "WndManager.h"
#include "WndRegVend.h"

#include "DPClient.h"

#define TASKBAR_HEIGHT 48

#define ICON_SIZE    32
#define SKILL_SIZE   28

DWORD   POINT_APPLET_X ;
DWORD   POINT_APPLET_Y ;
DWORD   POINT_ITEM_X   ;
DWORD   POINT_ITEM_Y   ;
DWORD   POINT_QUEUE_X  ;
DWORD   POINT_QUEUE_Y  ;

#define RECT_APPLET CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + m_nMaxSlotApplet * ICON_SIZE , POINT_APPLET_Y + ICON_SIZE  )
#define RECT_ITEM   CRect( POINT_ITEM_X  , POINT_ITEM_Y  , POINT_ITEM_X   + MAX_SLOT_ITEM   * ICON_SIZE , POINT_ITEM_Y   + ICON_SIZE  )
#define RECT_QUEUE  CRect( POINT_QUEUE_X , POINT_QUEUE_Y , POINT_QUEUE_X  + MAX_SLOT_QUEUE  * SKILL_SIZE, POINT_QUEUE_Y  + SKILL_SIZE )

#define POINT_APPLET CPoint( POINT_APPLET_X, POINT_APPLET_Y )
#define POINT_ITEM   CPoint( POINT_ITEM_X  , POINT_ITEM_Y   )
#define POINT_QUEUE  CPoint( POINT_QUEUE_X , POINT_QUEUE_Y  )

void CWndTaskBar::UpdateAllTaskbarTexture() {
	// Ensure has texture and no invalid item shortcut
	const auto EnsureCorrectness = [&](SHORTCUT & shortcut) {
		SetTaskBarTexture(shortcut);

		if (!g_pPlayer) return;

		if (shortcut.m_dwShortcut == ShortcutType::Item) {
			CItemElem * pItemBase = g_pPlayer->GetItemId(shortcut.m_dwId);
			if (pItemBase && pItemBase->GetProp()->dwPackMax > 1) {
				shortcut.m_dwItemId = pItemBase->m_dwItemId;
			}

			if (!pItemBase) {
				shortcut.Empty();
			}
		}
	};

	for (SHORTCUT & shortcut : m_aSlotApplet) {
		EnsureCorrectness(shortcut);
	}

	for (auto & bar : m_aSlotItem) {
		for (SHORTCUT & shortcut : bar) {
			EnsureCorrectness(shortcut);
		}
	}

	for (SHORTCUT & shortcut : m_aSlotQueue) {
		EnsureCorrectness(shortcut);
	}

	// Update m_nCurQueueNum
	for (m_nCurQueueNum = 0; m_nCurQueueNum != m_aSlotQueue.size(); ++m_nCurQueueNum) {
		if (m_aSlotQueue[m_nCurQueueNum].m_dwShortcut != ShortcutType::Skill) {
			break;
		}
	}
}

void CWndTaskBar::SetTaskBarTexture( SHORTCUT & shortcut )
{
	if( shortcut.m_dwShortcut == ShortcutType::Applet )
	{
		AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( shortcut.m_dwId );
		if( pAppletFunc )
		{
			shortcut.m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, pAppletFunc->m_pszIconName ), 0xffff00ff );
		} else
		{
#ifndef __BS_CONSOLE
			Error( "CWndTaskBar::SetTaskBarTexture : %s %d", g_pPlayer->GetName(), shortcut.m_dwId );
#endif
		}
	}
	else if( shortcut.m_dwShortcut == ShortcutType::Item)
	{
		CItemElem * pItemBase = g_pPlayer->GetItemId( shortcut.m_dwId );
		if( pItemBase )
			shortcut.m_pTexture	= pItemBase->GetTexture();
	}
	else if (shortcut.m_dwShortcut == ShortcutType::PartySkill) {
		ItemProp * pProp = prj.GetPartySkill(shortcut.m_dwId);
		shortcut.m_pTexture = m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff);
	}
	else if ( shortcut.m_dwShortcut == ShortcutType::Skill)
	{
		ItemProp* pSkillProp = prj.m_aPropSkill.GetAt(shortcut.m_dwId);
		if( pSkillProp )
			shortcut.m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pSkillProp->szIcon ), 0xffff00ff );
	}
	else if ( shortcut.m_dwShortcut == ShortcutType::Lord)
	{
		CCLord* pLord									= CCLord::Instance();
		CLordSkillComponentExecutable* pComponent		= pLord->GetSkills()->GetSkill(shortcut.m_dwId);	
		if(pComponent) shortcut.m_pTexture							= pComponent->GetTexture();
	}
	else if ( shortcut.m_dwShortcut == ShortcutType::Motion)
	{
		MotionProp* pMotionProp = prj.GetMotionProp( shortcut.m_dwId );
		if(pMotionProp)			//061206 ma	8���� �� ��ǰ����� ���� ���� �߰�	propMotion.txt
		{
			pMotionProp->pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pMotionProp->szIconName ), 0xffff00ff );
			shortcut.m_pTexture = pMotionProp->pTexture;
		}
	}
	else if( shortcut.m_dwShortcut == ShortcutType::Chat)
	{
		shortcut.m_pTexture	= m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, "icon_MacroChat.dds" ), 0xffff00ff );
	}
	else if( shortcut.m_dwShortcut == ShortcutType::Emoticon)
	{
		if( shortcut.m_dwId >= 0 && shortcut.m_dwId < MAX_EMOTICON_NUM  )
		{
			TCHAR buffer[MAX_SHORTCUT_STRING] = { 0 };
			_tcscat( buffer, "/" );
			_tcscat( buffer, g_DialogMsg.m_EmiticonCmd[shortcut.m_dwId].m_szCommand );
			_tcscpy( shortcut.m_szString, buffer );
			shortcut.m_pTexture =	g_DialogMsg.m_texEmoticonUser.GetAt(shortcut.m_dwId);
		}
	}
	else if( shortcut.m_dwShortcut == ShortcutType::SkillFun)
	{
		shortcut.m_pTexture	= m_pTexture;
	}
}

/*
* �׼� ����Ʈ
. �׼� ����Ʈ ���� CWndTaskBar::m_nActionPoint
. ��ų�ٿ� ������ �簢�� �׸���
. �׼�����Ʈ �ø��������
. ���ϰ����� 2��° ������ ���ÿ��� �׼�����Ʈ ���� 1���� - 100�� �Ѱ�
. �׼� ����Ʈ�� ���� ���̵� �׸��� - ��������Ʈ������ �ٸ���.
. ��ų���� �ܰ躰�� �׼�����Ʈ �Ҹ� �ٸ���.

*/

CWndTaskBar::CWndTaskBar()
{
	m_nCurQueueNum = 0;
	m_nPosition = TASKBAR_BOTTOM;
	memset( &m_aSlotSkill, 0, sizeof( m_aSlotSkill ) );

	m_pSelectShortcut = NULL;
	m_bStartTimeBar = FALSE;
	m_nSkillBar = 0;
	m_nUsedSkillQueue = 0;
	m_nExecute = 0;
	m_nSlotIndex = 0;
	m_nActionPoint = 0;
	m_dwHighAlpha = 0;
}

std::array<SHORTCUT, MAX_SLOT_ITEM> & CWndTaskBar::m_paSlotItem() {
	if (m_nSlotIndex < 0 && m_nSlotIndex >= MAX_SLOT_ITEM) {
		m_nSlotIndex = 0;
	}

	return m_aSlotItem[m_nSlotIndex];
}

const std::array<SHORTCUT, MAX_SLOT_ITEM> & CWndTaskBar::m_paSlotItem() const {
	if (m_nSlotIndex < 0 && m_nSlotIndex >= MAX_SLOT_ITEM) {
		return m_aSlotItem[0];
	}

	return m_aSlotItem[m_nSlotIndex];
}

void CWndTaskBar::PutTooTip( LPSHORTCUT pShortcut, CPoint point, CRect* pRect )
{
	ClientToScreen( &point );
	ClientToScreen( pRect );
	if( pShortcut->m_dwShortcut == ShortcutType::Applet)
	{
		AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( pShortcut->m_dwId ); 
		if( pAppletFunc )
		{
			
			CEditString strEdit = pAppletFunc->m_pAppletDesc;
			if( pAppletFunc->m_cHotkey != 0 )
			{
				strEdit.AddString( "\n" );
				CString string;
				string.Format( "[%s %c]", prj.GetText( TID_GAME_TOOLTIP_HOTKEY ), pAppletFunc->m_cHotkey );
				strEdit.AddString( string, 0xff0000ff );
			}

			g_toolTip.PutToolTip( pShortcut->m_dwId, strEdit, *pRect, point, 0 );
		}
	}
	else	
	if( pShortcut->m_dwShortcut == ShortcutType::Chat)
	{
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
	else
	if( pShortcut->m_dwShortcut == ShortcutType::Emoticon)
	{
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
	else	
	if( pShortcut->m_dwShortcut == ShortcutType::Item)
	{
		g_WndMng.PutToolTip_Item( 0, pShortcut->m_dwId, point, pRect );
	}
	else if (pShortcut->m_dwShortcut == ShortcutType::PartySkill) {
		g_WndMng.PutToolTip_Troupe(pShortcut->m_dwId, point, pRect);
	} else if( pShortcut->m_dwShortcut == ShortcutType::Skill) {
		const SKILL * const lpSkill = g_pPlayer->GetSkill(pShortcut->m_dwId);
		g_WndMng.PutToolTip_Skill( lpSkill->dwSkill, lpSkill ? lpSkill->dwLevel : 0, point, pRect );
	}
	else	
	if( pShortcut->m_dwShortcut == ShortcutType::Lord)
	{
		CCLord*							pLord		= CCLord::Instance();
		CLordSkillComponentExecutable*	pComponent	= pLord->GetSkills()->GetSkill(pShortcut->m_dwId);
		CString							string, str;
		CEditString						strEdit;

		if(!pComponent) return;

		string.Format( "#b#cff2fbe6d%s#nb#nc\n%s", pComponent->GetName(), pComponent->GetDesc());
		if(pComponent->GetTick() != 0)
		{
			str.Format( prj.GetText(TID_TOOLTIP_COOLTIME), pComponent->GetTick(), 0);
			string += "\n";
			string += str;
		}
	
		strEdit.SetParsingString( string );
		g_toolTip.PutToolTip(10000, strEdit, *pRect, point, 0);
	}
	else	
	if( pShortcut->m_dwShortcut == ShortcutType::Motion)
	{
		MotionProp* pMotionProp = prj.GetMotionProp( pShortcut->m_dwId );
		if(!pMotionProp)		//061206 ma	8���� �� ��ǰ����� ���� ���� �߰�	propMotion.txt
			return;

		CString string = pMotionProp->szDesc;
		
		if( pMotionProp->dwID == MOT_BASE_CHEER )
		{
			if( g_pPlayer )
			{
				CString str;
				str.Format( prj.GetText(TID_CHEER_MESSAGE1), g_pPlayer->m_nCheerPoint );

				string += "\n\n";
				string += str;

				if( g_pPlayer->m_nCheerPoint < MAX_CHEERPOINT )
				{
					CTimeSpan ct( ((g_pPlayer->m_dwTickCheer - GetTickCount()) / 1000) );		// �����ð��� �ʴ����� ��ȯ�ؼ� �Ѱ���

					if( ct.GetMinutes() <= 0 )
					{
						str.Format( prj.GetText(TID_PK_LIMIT_SECOND), ct.GetSeconds() );
					}
					else
					{
						str.Format( prj.GetText(TID_PK_LIMIT_MINUTE), ct.GetMinutes() );
					}
					
					string += "\n";
					string += (prj.GetText(TID_CHEER_MESSAGE2)+str);
				}
			}
		}

		CEditString strEdit;
		strEdit.SetParsingString( string );
		g_toolTip.PutToolTip( pShortcut->m_dwId , strEdit, *pRect, point, 0 );
	}
	else
	if( pShortcut->m_dwShortcut == ShortcutType::SkillFun)
	{
		strcpy( pShortcut->m_szString, prj.GetText(TID_GAME_SKILLSHORTCUT) );
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
		
}

void CWndTaskBar::OnMouseWndSurface( CPoint point )
{
	CRect rect = CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + ICON_SIZE, POINT_APPLET_Y + ICON_SIZE );
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point) )
		{
			PutTooTip( lpShortcut, point, &rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}

	rect = CRect( POINT_ITEM_X, POINT_ITEM_Y, POINT_ITEM_X + ICON_SIZE, POINT_ITEM_Y + ICON_SIZE );
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem()[i];
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	rect = CRect( POINT_QUEUE_X, POINT_QUEUE_Y, POINT_QUEUE_X + SKILL_SIZE, POINT_QUEUE_Y + SKILL_SIZE );
	for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( SKILL_SIZE, 0 );
	}

	rect = CRect( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y, (POINT_QUEUE_X - ICON_SIZE - 5) + 32, POINT_QUEUE_Y + 32);

	if( rect.PtInRect( point) )
	{
		SHORTCUT Shortcut;
		Shortcut.m_dwShortcut = ShortcutType::SkillFun;
		PutTooTip( &Shortcut, point,&rect );
	}
}

static void DRAW_HOTKEY(C2DRender * p2DRender, CPoint point, char cHotkey) {
	CString string;
	string.Format( "%c", cHotkey );
	p2DRender->TextOut( point.x - 0 + 2, point.y - 0 - 4, string, 0xffffffff );
}

void CWndTaskBar::OnDraw( C2DRender* p2DRender )
{
	if( NULL == g_pPlayer )
		return;

	DWORD dwCur = g_tmCurrent;
	CRect rect = GetWindowRect();
	CRect rectSkillQueue;

	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 
		
	if( m_nPosition == TASKBAR_TOP || m_nPosition == TASKBAR_BOTTOM )
	{
		 rectSkillQueue.SetRect( rect.Width() - 32 * 4 - 6, 0, rect.Width(), rect.Height() );
	}
	else
	if( m_nPosition == TASKBAR_LEFT || m_nPosition == TASKBAR_RIGHT )
	{
		rectSkillQueue.SetRect( 0, rect.Height() - 32 * 4 - 6, rect.Width(), rect.Height() );
	}

	// �巡�� ���̶���Ʈ ���� ǥ��
	if( m_GlobalShortcut.IsEmpty() == FALSE )
	{
		DWORD dwColor;
		if( m_dwHighAlpha < 128 )
			dwColor = D3DCOLOR_ARGB( m_dwHighAlpha, 0, 0, 255 );
		else
			dwColor = D3DCOLOR_ARGB( 128 - ( m_dwHighAlpha - 128 ), 0, 0, 255 );

		LPWNDCTRL pApplet = GetWndCtrl( WIDC_CUSTOM1 );
		LPWNDCTRL pItem   = GetWndCtrl( WIDC_CUSTOM2 );
		LPWNDCTRL pSkill  = GetWndCtrl( WIDC_CUSTOM3 );

		CRect rectApplet = pApplet->rect;
		CRect rectItem = pItem->rect;
		CRect rectSkill = pSkill->rect;

		rectApplet.top = 42;
		rectApplet.bottom = 47;
		rectApplet.left -= 6;
		rectApplet.right += 6;
		rectItem.top = 42;
		rectItem.bottom = 47;
		rectItem.left -= 6;
		rectItem.right += 6;
		rectSkill.top = 42;
		rectSkill.bottom = 47;
		rectSkill.left -= 3;
		rectSkill.right += 4;
		
		switch( m_GlobalShortcut.m_dwShortcut )
		{
			case ShortcutType::Applet:
			case ShortcutType::Item:
			case ShortcutType::Motion:
				p2DRender->RenderFillRect( rectApplet, dwColor );
				p2DRender->RenderFillRect( rectItem, dwColor );
				break;
			case ShortcutType::Skill:
				{
					const DWORD dwSkill = m_GlobalShortcut.m_dwId;
					DWORD dwComboStyleSrc = prj.GetSkillProp( dwSkill )->dwComboStyle;
					if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  )		// �����̳� ���׷��̸� �հ�
						p2DRender->RenderFillRect( rectItem, dwColor );		
					p2DRender->RenderFillRect( rectSkill, dwColor );
				}
				break;
			case ShortcutType::PartySkill:
				p2DRender->RenderFillRect( rectItem, dwColor );
				break;
		}		
	}

	// ���÷� ������ ���
	CPoint point = POINT_APPLET;
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		SHORTCUT & lpShortcut = m_aSlotApplet[i];
		const bool isMissingItem = RenderShortcut(p2DRender, lpShortcut, point, true);

		if (isMissingItem) {
			FindNewStackForShortcut(std::nullopt, i);
		}

		point += CPoint( ICON_SIZE, 0 );
	}
	// ������ ������ ��� 
	point = POINT_ITEM;
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		const SHORTCUT & lpShortcut = m_paSlotItem()[i];
		RenderShortcut(p2DRender, lpShortcut, point, false);

		// hotkey ��� 
		CHAR cHotkey = ( i == 9 ) ? '0' : '1' + i;
		CPoint ptHotkey( point.x + 8, point.y - 9 );

		point += CPoint( ICON_SIZE, 0 );
	}
	
	point = CPoint( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y );
	p2DRender->RenderTexture( point, m_pTexture );

	// ��ų ť 
	point = POINT_QUEUE;
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() && lpShortcut->m_pTexture)
		{
			p2DRender->RenderTexture( point, lpShortcut->m_pTexture );

//				LPSKILL pSkill = g_pPlayer->GetSkill( lpShortcut->m_dwType, lpShortcut->m_dwId );
//				if( g_pPlayer->m_nReUseDelay[ lpShortcut->m_dwId ] > 0 )
//					p2DRender->TextOut( point.x, point.y, g_pPlayer->m_nReUseDelay[ lpShortcut->m_dwId ] );
		}

		point += CPoint( SKILL_SIZE, 0 );
	}

	
	if( !( g_pPlayer->IsSMMode( SM_ACTPOINT ) ) )
	{

		point = POINT_QUEUE;
		point.x += SKILL_SIZE;	// ��ų 2��°ĭ���� ���̵�ȴ�.
		{
			int nAP2Size;	// �׼�����Ʈ�� ���̶���Ʈ������ ��ȯ�Ѱ�.
			if( m_nActionPoint >= LV4MAXAP )	// LV4MAXAP�� �Ѿ�� 5�ܰ� ����.
				nAP2Size = (SKILL_SIZE * 3) + (((m_nActionPoint - LV4MAXAP) * SKILL_SIZE) / (LV5MAXAP - LV4MAXAP));
			else if( m_nActionPoint >= LV3MAXAP )
				nAP2Size = (SKILL_SIZE * 2) + (((m_nActionPoint - LV3MAXAP) * SKILL_SIZE) / (LV4MAXAP - LV3MAXAP));
			else if( m_nActionPoint >= LV2MAXAP )
				nAP2Size = (SKILL_SIZE * 1) + (((m_nActionPoint - LV2MAXAP) * SKILL_SIZE) / (LV3MAXAP - LV2MAXAP));
			else 
				nAP2Size = (m_nActionPoint * SKILL_SIZE) / LV2MAXAP;
			
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			point.x += nAP2Size;
			nAP2Size = (SKILL_SIZE * 4) - nAP2Size;
			p2DRender->RenderFillRect( CRect(point.x, point.y,  point.x + nAP2Size, point.y + SKILL_SIZE + 2), 
										D3DCOLOR_ARGB( 0x7f, 0, 0, 0 ) );
			
			if( m_nCurQueueNum > 0 )
			{
				int show = 1;
				
				if( m_nActionPoint >= LV2MAXAP )
					show = 2;
				if( m_nActionPoint >= LV3MAXAP )
					show = 3;
				if( m_nActionPoint >= LV4MAXAP )
					show = 4;
				if( m_nActionPoint >= LV5MAXAP )
					show = 5;
				
				if( m_nCurQueueNum <= show )
					show = m_nCurQueueNum;
				
				CPoint Point = CPoint( POINT_QUEUE_X, POINT_QUEUE_Y+1 );

				RenderOutLineLamp( POINT_QUEUE_X, POINT_QUEUE_Y+1, show, SKILL_SIZE );
			}
		}
	}

	CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_UP );
	rect = pWndButton->GetWindowRect( TRUE );
	rect.top = rect.bottom - 2;
	p2DRender->TextOut( rect.left, rect.top, m_nSlotIndex + 1, 0xffffffff );
	p2DRender->SetFont( pOldFont );
}

bool CWndTaskBar::RenderShortcut(
	C2DRender * p2DRender, const SHORTCUT & shortcut, CPoint point,
	bool drawLetter
) {
	if (shortcut.IsEmpty()) return true;

	if (shortcut.m_pTexture) {
		p2DRender->RenderTexture(point, shortcut.m_pTexture);
	}

	switch (shortcut.m_dwShortcut) {
		case ShortcutType::Item: {
			CItemElem * const pItemBase = g_pPlayer->GetItemId(shortcut.m_dwId);
			if (!pItemBase) return false;

			const ItemProp & itemProp = *pItemBase->GetProp();

			if (itemProp.dwPackMax > 1) {
				TCHAR szTemp[32];
				_stprintf(szTemp, "%d", g_pPlayer ? g_pPlayer->m_Inventory.GetItemCount(pItemBase->m_dwItemId) : 0);
				CSize size = m_p2DRender->m_pFont->GetTextExtent(szTemp);
				p2DRender->TextOut(point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff);
				p2DRender->TextOut(point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0);
			}

			if (g_pPlayer) {
				if (const auto cooldown = g_pPlayer->m_cooltimeMgr.GetElapsedTime(itemProp)) {
					RenderRadar(p2DRender, point, cooldown->elapsedTime, cooldown->totalWait);
				}
			}
			break;
		}
		case ShortcutType::Applet: {
			AppletFunc * pAppletFunc = g_WndMng.GetAppletFunc(shortcut.m_dwId);
			if (pAppletFunc && pAppletFunc->m_cHotkey) {
				CPoint ptHotkey(point.x + 8, point.y - 9);
				DRAW_HOTKEY(p2DRender, ptHotkey, pAppletFunc->m_cHotkey);
			}
			break;
		}
		case ShortcutType::Skill:
			RenderCollTime(point, shortcut.m_dwId, p2DRender);
			break;
		case ShortcutType::Lord:
			RenderLordCollTime(point, shortcut.m_dwId, p2DRender);
			break;
		case ShortcutType::Motion: {
			if (shortcut.m_dwId == MOT_BASE_ESCAPE) {
				ItemProp * pItem = prj.GetItemProp(g_AddSMMode.dwSMItemID[SM_ESCAPE]);

				if (pItem && g_pPlayer) {
					CTimeSpan ct(g_pPlayer->m_dwSMTime[SM_ESCAPE]);

					if (ct.GetTotalSeconds()) {
						point.y -= 1;
						point.x += 1;
						RenderRadar(m_p2DRender, point, (DWORD)(pItem->dwCircleTime - ct.GetTotalSeconds()), pItem->dwCircleTime);
					}
				}
			}
		}
	}

	return true;
}

void CWndTaskBar::UpdateItem() {
	if (!g_pPlayer) return;

	for (int i = 0; i < m_paSlotItem().size(); ++i) {
		SHORTCUT & shortcut = m_paSlotItem()[i];
		if (shortcut.m_dwShortcut != ShortcutType::Item) continue;
		
		CItemElem * pItemElem = g_pPlayer->GetItemId(shortcut.m_dwId);

		if (pItemElem) {
			if (shortcut.m_pTexture != pItemElem->GetTexture()) {
				shortcut.m_pTexture = pItemElem->GetTexture();
			}
			continue;
		}

		FindNewStackForShortcut(m_nSlotIndex, i);
	}
}

void CWndTaskBar::FindNewStackForShortcut(std::optional<int> where, int i) {
	if (where && where < 0 || where >= MAX_SLOT_ITEM_COUNT) {
		where = 0;
	}

	SHORTCUT & shortcut = where ? m_aSlotItem[*where][i] : m_aSlotApplet[i];

	const auto SendRemove = [&]() {
		shortcut.Empty();
		if (where) {
			g_DPlay.SendRemoveItemTaskBar(*where, i);
		} else {
			g_DPlay.SendRemoveAppletTaskBar(i);
		}
	};

	CItemElem * pItemElem = g_pPlayer->GetItemId(shortcut.m_dwId);
	if (pItemElem) return;

	const ItemProp * const pItemProp = prj.GetItemProp(shortcut.m_dwItemId);
	if (!pItemProp || pItemProp->dwPackMax <= 1) {
		SendRemove();
		return;
	}

	const DWORD dwId = g_pPlayer->m_Inventory.Find(shortcut.m_dwItemId);
	if (dwId == NULL_ID) {
		SendRemove();
		return;
	}

	shortcut.m_dwId = dwId;
	g_DPlay.SendAddInTaskbar(where, i, shortcut);
}

void CWndTaskBar::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
}
void CWndTaskBar::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	LPWNDCTRL pApplet = GetWndCtrl( WIDC_CUSTOM1 );
	LPWNDCTRL pItem   = GetWndCtrl( WIDC_CUSTOM2 );
	LPWNDCTRL pSkill  = GetWndCtrl( WIDC_CUSTOM3 );

	POINT_APPLET_X = pApplet->rect.left;
	POINT_APPLET_Y = pApplet->rect.top;
	POINT_ITEM_X   = pItem->rect.left;
	POINT_ITEM_Y   = pItem->rect.top;
	POINT_QUEUE_X  = pSkill->rect.left;
	POINT_QUEUE_Y  = pSkill->rect.top;

	CRect rect( 5, 7, 754, 40 );

	g_WndMng.m_pWndTaskBar = this;

	{
		rect = g_Neuz.GetDeviceRect();
		m_pWndRoot->m_rectLayout = rect;
		switch( m_nPosition )
		{
		case TASKBAR_TOP:
			rect.bottom = TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.top = rect.bottom;
			break;
		case TASKBAR_BOTTOM:
			rect.top = rect.bottom - TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.bottom = rect.top;
			break;
		case TASKBAR_LEFT:
			rect.right = TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.left = rect.right;
			break;
		case TASKBAR_RIGHT:
			rect.left = rect.right - TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.right = rect.left;
			break;
		}
		SetWndRect( rect );
	}
	m_texPack.LoadScript( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "hotkey.inc" ) );

	m_menuShortcut.CreateMenu( this );	
	m_menuShortcut.AppendMenu( 0, 0 ,_T( "����" ) );

	m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, "icon_ActionSkill.dds" ), 0xffff00ff );
	m_aSlotSkill.m_dwShortcut = ShortcutType::SkillFun;
	m_aSlotSkill.m_pTexture   = m_pTexture;
}

HRESULT CWndTaskBar::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
	
#ifdef __YDEBUG
	m_texPack.InvalidateDeviceObjects();
#endif //__YDEBUG
	return S_OK;
}

HRESULT CWndTaskBar::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	
#ifdef __YDEBUG
	m_texPack.DeleteDeviceObjects();
#endif //__YDEBUG
	return S_OK;
}

HRESULT CWndTaskBar::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	
#ifdef __YDEBUG
	m_texPack.RestoreDeviceObjects(m_pApp->m_pd3dDevice);
#endif //__YDEBUG
	
	return 0;
}

BOOL CWndTaskBar::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	CRect rect = g_Neuz.GetDeviceRect();
	//m_rectLayout = rect;
/*
	switch( m_nPosition )
	{
	case TASKBAR_TOP:
		rect.bottom = TASKBAR_HEIGHT;
		m_pWndRoot->m_rectLayout.top = rect.bottom;
		break;
	case TASKBAR_BOTTOM:
		rect.top = rect.bottom - TASKBAR_HEIGHT;
		m_pWndRoot->m_rectLayout.bottom = rect.top;
		break;
	case TASKBAR_LEFT:
		break;
	case TASKBAR_RIGHT:
		break;
	}
	*/
	//m_pWndRoot->SetWndRect( rectRoot );

//CMainFrame
	//rect.top = rect.bottom;
	//rect.bottom += TASKBAR_HEIGHT;
//	rect.bottom = CLIENT_HEIGHT;
	//m_strMessage = lpszMessage;
	//m_dwType = nType;
	//SetTitle("������ ����");
//	return CWndBase::Create( 0 | /*WBS_MODAL|*/ WBS_MANAGER | WBS_SOUND | WBS_NOFRAME, rect, &g_WndMng, APP_TASKBAR );
	BOOL bResult;
	if( g_Option.m_nResWidth == 800 )
	{
		m_nMaxSlotApplet = 5;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR800, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else
	if( g_Option.m_nResWidth == 1024 )
	{
		m_nMaxSlotApplet = 10;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1024, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else
	if( g_Option.m_nResWidth == 1280 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1280, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else if( g_Option.m_nResWidth == 1360 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1360W, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else if( g_Option.m_nResWidth == 1400 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1400, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else if( g_Option.m_nResWidth == 1440 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1440W, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else if( g_Option.m_nResWidth == 1600 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1600, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}
	else if( g_Option.m_nResWidth == 1680 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( APP_TASKBAR1680W, pWndParent, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ) );
	}	
	
	DelWndStyle( WBS_MOVE );
	return bResult;
	
}

void CWndTaskBar::SetItemSlot( int nSlot )
{
	if (nSlot >= 0 && nSlot < MAX_SLOT_ITEM_COUNT) {
		m_nSlotIndex = nSlot;
	}
}
BOOL CWndTaskBar::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	CWndButton* pWndButton = (CWndButton*) pLResult;
	if( message == WIN_ITEMDROP )
	{
	}
	else
	switch( nID )
	{
		case WIDC_UP:
			m_nSlotIndex--;
			if( m_nSlotIndex < 0 ) m_nSlotIndex = MAX_SLOT_ITEM_COUNT-1;
			break;
		case WIDC_DOWN:
			m_nSlotIndex++;
			if( m_nSlotIndex > MAX_SLOT_ITEM_COUNT-1 ) m_nSlotIndex = 0;
			break;
		case WIDC_BUTTON1: // Menu
		{
			g_WndMng.OpenMenu();
			CRect rect = g_Neuz.GetDeviceRect();
			CRect rectMenu = m_wndMenu.GetScreenRect();
			switch( m_nPosition )
			{
			case TASKBAR_TOP:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( 0, TASKBAR_HEIGHT ) );
				break;
			case TASKBAR_BOTTOM:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( 0, rect.bottom - TASKBAR_HEIGHT - g_WndMng.m_pWndMenu->GetWndRect().Height() ) );
				break;
			case TASKBAR_LEFT:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( TASKBAR_HEIGHT, 0 ) );
				break;
			case TASKBAR_RIGHT:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( rect.right - TASKBAR_HEIGHT - g_WndMng.m_pWndMenu->GetWndRect().Width(), 0 ) );
				break;
			}

		}
		default:
			{
				switch( pWndButton->m_shortcut.m_dwIndex )
				{
					case APP_TRADE:		g_DPlay.SendTrade( (CMover*)g_WorldMng.Get()->GetObjFocus() );	break;
					default:
						g_WndMng.ObjectExecutor( &pWndButton->m_shortcut );	
						break;
				}
			}
	}
	return TRUE;
}
BOOL CWndTaskBar::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return TRUE;
}
void CWndTaskBar::OnSize(UINT nType, int cx, int cy)
{
	CWndNeuz::OnSize( nType, cx, cy );
}
void CWndTaskBar::RemoveSkillQueue( int nIndex, BOOL bSend )
{
	if( m_nExecute )	return;		// ��ųť �����߿� �������� �ʴ´�.
BACK:
	if( !m_aSlotQueue[ nIndex ].IsEmpty() )
	{
		const ItemProp* pItemProp = g_pPlayer->GetSkill( m_aSlotQueue[ nIndex ].m_dwId )->GetProp();
		DWORD dwComboStyleSrc = pItemProp->dwComboStyle;
		int i = NULL;
		for( i = nIndex; i < MAX_SLOT_QUEUE - 1; i++ )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i + 1 ], sizeof( SHORTCUT ) );
			m_aSlotQueue[ i ].m_dwIndex = i;
		}
		m_aSlotQueue[i].Empty();
		m_nCurQueueNum--;
		if( m_nCurQueueNum < 0 ) 
			m_nCurQueueNum = 0;
		if( CheckAddSkill( dwComboStyleSrc, nIndex /*- 1*/ ) == FALSE )
			goto BACK;
	}
	if( bSend )
		g_DPlay.SendSkillTaskBar( );
	// ��ų�� ����� �� ������ ��������.	

}
void CWndTaskBar::OnRButtonUp( UINT nFlags, CPoint point )
{
	CRect rect = RECT_APPLET;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.x /= ICON_SIZE;
		if( FALSE == m_aSlotApplet[ point.x ].IsEmpty() )
		{
#ifdef __MAINSERVER
			if( point.x == 0 && m_aSlotApplet[ point.x ].m_dwId == 400 )
				return;
#endif //__MAINSERVER
			
			m_aSlotApplet[ point.x ].Empty();			
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( point.x ) );		// ������ ���� ������ ����
		}
	}
	rect = RECT_ITEM;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;

		point.x /= ICON_SIZE;
		if( FALSE == m_paSlotItem()[point.x].IsEmpty())
		{
			m_paSlotItem()[point.x].Empty();
			g_DPlay.SendRemoveItemTaskBar( (BYTE)( m_nSlotIndex ), (BYTE)( point.x ) );	// ������ ���� ������ ����			
		}

	}
	rect = RECT_QUEUE;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= SKILL_SIZE;
		RemoveSkillQueue( point.x );
	}
}

void CWndTaskBar::SetShortcut( int nIndex, ShortcutType dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture, CTaskbar::BarName nWhere) {
	SHORTCUT & pShortcut = nWhere == CTaskbar::BarName::Applet ? m_aSlotApplet[nIndex] : m_paSlotItem()[nIndex];

	if (!CanAddShortcut(dwShortcut, pShortcut)) {
		g_WndMng.PutString(TID_GAME_MAX_SHORTCUT_CHAT);
		return;
	}

	pShortcut.m_dwShortcut = dwShortcut;
	pShortcut.m_dwIndex = nIndex;
	pShortcut.m_dwId = dwId;
	pShortcut.m_dwUserId = 0;
	pShortcut.m_dwData = nWhere == CTaskbar::BarName::Applet ? 0 : 1;
	strcpy(pShortcut.m_szString, m_GlobalShortcut.m_szString);

	if (dwShortcut == ShortcutType::Lord)
		pShortcut.m_dwId--;

	if (dwShortcut == ShortcutType::Item) {
		CItemElem * pItemBase = g_pPlayer->GetItemId(dwId);
		if (pItemBase && pItemBase->GetProp()->dwPackMax > 1) {
			pShortcut.m_dwItemId = pItemBase->m_dwItemId;
		}
	}

	if (pTexture) {
		pShortcut.m_pTexture = pTexture;
	} else {
		SetTaskBarTexture(pShortcut);
	}

	std::optional<unsigned int> where;
	if (nWhere == CTaskbar::BarName::Applet) where = std::nullopt;
	else where = m_nSlotIndex;

	g_DPlay.SendAddInTaskbar(where, nIndex, pShortcut);
}
BOOL CWndTaskBar::CheckAddSkill( int nSkillStyleSrc, int nQueueDest  )
{
	/*
	START -> FINISH, FG 
	CIRCLE -> START, CIRCLE
	FINISH -> START, CIRCLE
	*/
	DWORD dwComboStyleSrc = nSkillStyleSrc;
	if( nQueueDest == 0 )		// ùĭ�� ������ �õ� �������
	{
		/*
		if( �̹� ��ųâ�� ��ϵǾ� �ִ� )
		{
			if( �����Ϸ��°� CT_GENERAL�ΰ� )
				if( �̹� ���ԵǾ� �ִ°� CT_GENERAL�ΰ� )
				return TRUE;
			return FALSE;		// CT_GENERAL�̿��� ���� ùĭ�� �����Ϸ��� ��� ����.
		} else
		{
			// �ƹ��͵� ��ϵǾ� ���� �ʴ�.
			// ����, ���׷��� �հ�.
			if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  ) 
				return TRUE;
			return FALSE;	// �׿ܿ� ��� ���հ�.
		}
		 */
		if( m_nCurQueueNum == 0 )		// ����ִ�
		{
			if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  )		// �����̳� ���׷��̸� �հ�
				return TRUE;
			return	FALSE;
		} else
		// ���� �ִ�.
		{
			DWORD dwComboStyleDest;
			DWORD dwSkill = m_aSlotQueue[nQueueDest].m_dwId;
				//m_aSlotQueue[ nQueueDest ].m_dwId;	// �������� ĭ�� ��ų
			if( dwSkill )
				dwComboStyleDest = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
			else 
				dwComboStyleDest = 0;
			if( dwComboStyleSrc == CT_GENERAL && dwComboStyleDest == CT_GENERAL )	// �̹��ִ°͵� ���׷��̰� �����Ϸ��°͵� ���׷��̸� �հ�
				return TRUE;
			return FALSE;	// �׿ܴ� ��� ùĭ�� ������ �� ����.
		}
	}
	else
	if( nQueueDest == 4 )		// ������ĭ�� �������� �������
	{
		if( dwComboStyleSrc == CT_STEP )	// ������ ùĭ�� �ƴϸ� ������ ���ִ´�.
			return FALSE;
		DWORD dwSkill = m_aSlotQueue[ nQueueDest - 1 ].m_dwId;
			//m_aSlotQueue[ nQueueDest - 1 ].m_dwId;	// �������� ĭ�� �տ��ִ� ��ų
		DWORD dwComboStylePrev;
		if( dwSkill )
			dwComboStylePrev = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
		else
			dwComboStylePrev = 0;
		if( dwComboStylePrev == CT_FINISH )		// �� ��ų�� �ǴϽ��� ���̻� ���� �� ����.
			return FALSE;
		// ��Ŭ����� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_CIRCLE && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// �ǴϽ�(�ϳ�)�� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// ���׷��� �������ϰ� ��ĭ�� ���׷��� �ƴϸ� ������.
		if( dwComboStyleSrc == CT_GENERAL && dwComboStylePrev != CT_GENERAL )
			return FALSE;
		
		return TRUE;	// �׿ܴ� ��� �հ�
	}
	else
	// �׿� �߰��� ������ �Ѱ��.
	{
		// ��°ĭ �̻���� �������� ������ ó��.
		if( dwComboStyleSrc == CT_STEP )	// ������ ùĭ�� �ƴϸ� ������ ���ִ´�.
			return FALSE;
		DWORD dwComboStylePrev;
		DWORD dwComboStyleDest;
		DWORD dwSkill = m_aSlotQueue[ nQueueDest - 1 ].m_dwId;
			//m_aSlotQueue[ nQueueDest - 1 ].m_dwId;	// �������� ĭ�� �տ��ִ� ��ų
		if( dwSkill )
			dwComboStylePrev = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
		else
			dwComboStylePrev = 0;
		dwSkill = m_aSlotQueue[ nQueueDest ].m_dwId;
		//dwSkill = m_aSlotQueue[ nQueueDest ].m_dwId;	// �������� ĭ�� ��ų
		if( dwSkill )
			dwComboStyleDest = prj.GetSkillProp( dwSkill )->dwComboStyle;		// ��ĭ�� ��Ÿ��
		else
			dwComboStyleDest = 0;
		if( dwComboStylePrev == CT_FINISH )		// �� ��ų�� �ǴϽ��� ���̻� ���� �� ����.
			return FALSE;
		// ��Ŭ����� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_CIRCLE && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// �ǴϽ�(�ϳ�)�� �������ϰ� ��ĭ�� ���׷��̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// �ǴϽ��� �������ϰ� ��ĭ�� ��Ŭ�̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStyleDest == CT_CIRCLE )
			return FALSE;
		// �ǴϽ��� �������ϰ� ��ĭ�� �ǴϽ��̸� ������.
		if( dwComboStyleSrc == CT_FINISH && dwComboStyleDest == CT_FINISH )
			return FALSE;
		// ���׷��� �������ϰ� 
		if( dwComboStyleSrc == CT_GENERAL )
		{
			if( dwComboStylePrev != CT_GENERAL )	// ��ĭ�� ���׷��� �ƴϸ� ������.
				return FALSE;
			if( dwComboStyleDest != CT_GENERAL && m_nCurQueueNum > nQueueDest )	// ��ĭ�� ���׷��� �ƴϸ� ���ִµ� �� ���̸� ������ �ִ�.
					return FALSE;
		}
		
		return TRUE;	// �� �ܴ� ��� �հ�
	}
	return FALSE;
}
BOOL CWndTaskBar::SetSkillQueue( int nIndex, const DWORD skillId, CTexture* pTexture )
{
	if( m_nExecute )		return FALSE;		// ��ųť �����߿� ��� �ȵ�.
	if( m_nCurQueueNum >= 5 )
		return FALSE;

	// m_nCurQueueNum ���� ���� ���Կ� ����ִ°� �ִٸ� ���� ���´�. Ŭ��������.
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		if( m_aSlotQueue[ i ].IsEmpty() )
		{
			for( i = 0; i < m_nCurQueueNum; i++ )
				m_aSlotQueue[ i ].Empty();
			m_nCurQueueNum = 0;
			break;
		}
	}
	 if( nIndex > m_nCurQueueNum )
		 nIndex = m_nCurQueueNum;

	LPSHORTCUT pShortcut = NULL;
	CRect rect = GetWindowRect();
	SKILL * lpSkill = g_pPlayer->GetSkill(skillId);
	const DWORD dwLevel = lpSkill->dwLevel;
	const ItemProp * pSkillProp = prj.GetSkillProp( skillId );
	const DWORD dwComboStyleSrc = pSkillProp->dwComboStyle;

	if( dwLevel <= 0 )
		return FALSE;

	AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, dwLevel );
	ASSERT( pAddSkillProp );
	if ((int)pAddSkillProp->dwCooldown > 0)	//  ��Ÿ���ִ� ��ų�� �׼ǽ��Կ� ����
	{
		g_WndMng.PutString(TID_GAME_SKILLLNOTUSE, pSkillProp->szName);
		return FALSE;
	}

//	DWORD dwComboStyleDest;
//	ItemProp* pItemProp1, * pItemProp2,* pItemProp3; 
	if( nIndex < m_nCurQueueNum ) // insert		// nIndex�� ������ ĭ�� �ε���
	{
		if( !CheckAddSkill( dwComboStyleSrc, nIndex ) )		// Src�� nIndex�� �־ �Ǵ��� üũ
			return FALSE;
		for( int i = m_nCurQueueNum; i > nIndex; i-- )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i - 1 ], sizeof( SHORTCUT ) );
		}
		pShortcut = &m_aSlotQueue[ nIndex ];
		m_nCurQueueNum++;
	}
	else // add
	{
		if( !CheckAddSkill( dwComboStyleSrc,  m_nCurQueueNum ) )	// ���� ������ ĭ�� �־ �Ǵ��� �׽�Ʈ.
			return FALSE;
		pShortcut = &m_aSlotQueue[ m_nCurQueueNum ];
		nIndex = m_nCurQueueNum;
		m_nCurQueueNum++;
	}
	
	if( pTexture == NULL )
	{
		LPSKILL lpSkill = g_pPlayer->GetSkill( skillId );
		pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon), 0xff000000, FALSE );
	}
	else pShortcut->m_pTexture = pTexture;


	pShortcut->m_dwShortcut = ShortcutType::Skill;
	pShortcut->m_dwIndex    = nIndex;//dwIndex; // ��ų ��Ʈ�ѿ����� ���� 
	pShortcut->m_dwId       = skillId; // ��ų �ε��� 
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = 2;
	_tcscpy( pShortcut->m_szString, m_GlobalShortcut.m_szString );
	g_DPlay.SendSkillTaskBar();
	// ��ų ��� ���⼭ ������ ������ ��
	return TRUE;
}

BOOL CWndTaskBar::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);
	// ������ ������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ����Ʈ�� �׽�ũ�ٰ� �ƴ� ���̶��. �ǵ������� ������ ������� �½�ũ�� �ۿ� ���� ����. ������.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// ������ ���� ������ ����
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
				//g_DPlay.SendSkillTaskBar( );
			}
			lpShortcut->Empty();
			return TRUE;
		}
	//	return TRUE;
	}
	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();


	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT && pWndFrame->GetWndId() != APP_SKILL3 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY && pWndFrame->GetWndId() != APP_LORD_SKILL) )
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == ShortcutType::Skill ) // �شܽ�ų�� �ȵ�
	{
		SKILL * pSkill = g_pPlayer->GetSkill( pShortcut->m_dwId );
		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}
	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1
		&& pShortcut->m_dwShortcut != ShortcutType::SkillFun
		&& pShortcut->m_dwShortcut != ShortcutType::Lord)
	{
		SetForbid( TRUE );
		return FALSE;
	}
	// ���ο��� �̵��ǰų�, �ܺο��� ���η� 
	if( m_nPosition == TASKBAR_TOP || m_nPosition == TASKBAR_BOTTOM )
	{
		BOOL bForbid = TRUE;
		CRect rect = RECT_APPLET;
		if( rect.PtInRect( point ) )
		{
			point.x -= rect.left;
			point.y -= rect.top;
			point.x /= ICON_SIZE;
			// ���ο��� �̵� 
			if( pShortcut->m_pFromWnd == this )
			{
				LPSHORTCUT lpCurShortcut = (LPSHORTCUT) pShortcut->m_dwData;			

				if( &m_aSlotApplet[ point.x ] == lpCurShortcut )
				{
					//SetForbid( TRUE );
					return FALSE;
				}
				// ���������� �׼ǽ����̴�.
				//if( lpCurShortcut->m_dwData == 2 )
				//	RemoveSkillQueue( lpCurShortcut->m_dwIndex );
			}
			if( pShortcut->m_dwShortcut == ShortcutType::Skill)
			{
				const ItemProp * const pProp = prj.GetSkillProp(pShortcut->m_dwId);

				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
				}
			}

#ifdef __MAINSERVER
			if( point.x == 0 && m_aSlotApplet[ point.x ].m_dwId == APP_WEBBOX)
				return FALSE;
#endif //__MAINSERVER
				
			SetShortcut( point.x, pShortcut->m_dwShortcut, 0, pShortcut->m_dwId, pShortcut->m_pTexture, CTaskbar::BarName::Applet );
			bForbid = FALSE;
		}
		rect = RECT_ITEM;
//		rect.right += 32;
		if( rect.PtInRect( point ) )
		{
			point.x -= rect.left;
			point.y -= rect.top;
			point.x /= ICON_SIZE;
			// ���ο��� �̵� 
			if( pShortcut->m_pFromWnd == this )
			{
				LPSHORTCUT lpCurShortcut = (LPSHORTCUT) pShortcut->m_dwData;
				if( &m_aSlotItem[ m_nSlotIndex ][ point.x ] == lpCurShortcut )
					return FALSE;
				// ���������� �׼ǽ����̴�.
				//if( lpCurShortcut->m_dwData == 2 )
				//	RemoveSkillQueue( lpCurShortcut->m_dwIndex );
			}
			if( pShortcut->m_dwShortcut == ShortcutType::Chat)
			{
				CString sChat	= m_GlobalShortcut.m_szString;
				sChat.Replace( "--", "" );
				sChat.Replace( "'", "" );
				sChat.Replace( "/*", "" );
				sChat.Replace( "*/", "" );
				strcpy( m_GlobalShortcut.m_szString, (const char*)sChat );
			}
			else if( pShortcut->m_dwShortcut == ShortcutType::Skill)
			{
				const ItemProp * const pProp = prj.GetSkillProp(pShortcut->m_dwId);

				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// �����Կ� ����Ϸ��� ��ų�� ���ܱ���� �ƴѰ� ��� ����.
				}
			}
			SetShortcut( point.x, pShortcut->m_dwShortcut, 0, pShortcut->m_dwId, pShortcut->m_pTexture, CTaskbar::BarName::Item);
			bForbid = FALSE;
		}
		rect = RECT_QUEUE;
		if( rect.PtInRect( point ) )
		{
			if( pShortcut->m_dwShortcut == ShortcutType::Skill )		// ��ų�ϰ�츸 ���
			{
				point.x -= rect.left;
				point.y -= rect.top;
				point.x /= SKILL_SIZE;
				// ���ο��� �̵� ; ������ �Ʒ� �κп��� ������ ó���ϳ�, ��ų ť������ �̵��� ������ �ٷ� �߰��ؾ��ϱ� ������ ���⼭ ������ �Ѵ�.
				if( pShortcut->m_pFromWnd == this )
				{
					LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
					if( lpShortcut->m_dwData == 2 )
					{
						RemoveSkillQueue( lpShortcut->m_dwIndex, FALSE );
						SetSkillQueue( point.x, pShortcut->m_dwId, pShortcut->m_pTexture );// == FALSE )
						return TRUE;
					}
				}
				if( SetSkillQueue( point.x, pShortcut->m_dwId, pShortcut->m_pTexture ) == FALSE )
					SetForbid( TRUE );
				bForbid = FALSE;
			}
		}
		if( pShortcut->m_pFromWnd != this )
			SetForbid( bForbid );
	}
	// ������ ������ ���, �Ǵ� �ȿ��� �̵����� ��� �������� �κ� ����  
	// �½�ũ�ٿ� �ִ� �������� ���� ���� ���� �κп� �巡������ ��� 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// ������ ���� ������ ����
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return TRUE;
}
LPSHORTCUT CWndTaskBar::Select( CPoint point )
{
	CRect rect = RECT_APPLET;
	LPSHORTCUT pShortcut = NULL;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= ICON_SIZE;
		pShortcut = &m_aSlotApplet[ point.x ];
		if( !pShortcut->IsEmpty() )
		{
			return pShortcut;
		}
	}
	rect = RECT_ITEM;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= ICON_SIZE;
		pShortcut = &m_paSlotItem()[point.x];
		if( !pShortcut->IsEmpty() )
			return pShortcut;
	}
	rect = RECT_QUEUE;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= SKILL_SIZE;
		pShortcut = &m_aSlotQueue[ point.x ];
		if( !pShortcut->IsEmpty() )
			return pShortcut;

	}
	rect = CRect( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y, (POINT_QUEUE_X - ICON_SIZE - 5) + 32, POINT_QUEUE_Y + 32);
	if( rect.PtInRect( point ) )
	{
		m_aSlotSkill.m_dwShortcut = ShortcutType::SkillFun;

		pShortcut   = &m_aSlotSkill;

		if( !pShortcut->IsEmpty() )
			return pShortcut;
	}

	return NULL;
}
void CWndTaskBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bLButtonDown && IsPush() )
	{
	//PSHORTCUT pShortcut = Select( point );
	//	if( pShortcut && pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() )
		if( m_pSelectShortcut && !m_pSelectShortcut->IsEmpty() ) //&& m_pSelectShortcut->m_dwShortcut != SHORTCUT_SKILL )
		{
#ifdef __MAINSERVER
			if( m_pSelectShortcut->m_dwId == APP_WEBBOX )
				return;
#endif //__MAINSERVER
			if(m_pSelectShortcut->m_dwShortcut == ShortcutType::Lord)
			{
				m_pSelectShortcut->m_dwId++;
			}
			memcpy( &m_GlobalShortcut, m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pSelectShortcut;
			m_pSelectShortcut = NULL;
		}
	}
	CWndNeuz::OnMouseMove( nFlags, point );
}

void CWndTaskBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(g_WndMng.m_pWndMenu && g_WndMng.m_pWndMenu->IsVisible())
		g_WndMng.m_pWndMenu->SetVisible(FALSE);

	m_pSelectShortcut = Select( point );
	SetCapture();
}
void CWndTaskBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	LPSHORTCUT pShortcut = Select( point );
	
	if( pShortcut && pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}

}

LPSKILL CWndTaskBar::GetCurrentSkillQueue()
{
	LPSKILL pSkill = NULL;
	LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];

	if( pShortcut && pShortcut->IsEmpty() == FALSE )
		pSkill = g_pPlayer->GetSkill( pShortcut->m_dwId );

	return pSkill;
}
//
// ��ųť ���� ��!
//
BOOL CWndTaskBar::UseSkillQueue( CCtrl* pTargetObj )
{
	if( m_nExecute == 2 )	return FALSE;	// �̹� ��ų�� �������̶�� �ٽ� ������ ����.
	if( g_pPlayer->m_pActMover->IsActAttack() )			return FALSE;		// �̹� ���ݵ����� �̷����� ������ ������ ����.
	if( g_pPlayer->m_pActMover->IsActJump() )			return FALSE;		// �����߿� ��ų��� �ȵ�.
//	if( g_pPlayer->m_pActMover->IsActDamage() )			return FALSE;		// ������ ����߿� ��ų��� �ȵ�.
	if( g_pPlayer->m_pActMover->IsFly() )				return FALSE;
//	if( g_pPlayer->m_dwFlag & MVRF_SKILL )		return FALSE;				// �̹� ��ų ������̸� ��� �ȵ�.
	if( pTargetObj && pTargetObj->GetType() != OT_MOVER )	return FALSE;

	// ������Ʈ�� ������ Ŭ�� ������
	if( pTargetObj )
	{
		if( pTargetObj->GetType() == OT_OBJ )		// ������Ʈ�� OT_OBJ(���)�̸� ���� ���Ѱɷ� ������.
			m_idTarget = NULL_ID;
		else
			m_idTarget = ((CCtrl*)pTargetObj)->GetId();		// OT_OBJ�� �ƴϸ� ���̵� ������
	}
	else
		m_idTarget = NULL_ID;		// Ÿ���� �������� �ʾ����� NULL_ID
	CMover *pTargetMover = (CMover*)pTargetObj;		// prj.GetMover( m_idTarget );
	if( IsInvalidObj( pTargetMover ) )		// �Žñ��� Ÿ���̾����� Ÿ�� ���Ѱɷ� ����.
		m_idTarget = NULL_ID;

	
	m_nUsedSkillQueue = 0;
	
	LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];

	if( pShortcut->IsEmpty() == FALSE )
	{
		LPSKILL pSkill = g_pPlayer->GetSkill( pShortcut->m_dwId );
		if( pSkill == NULL )	return FALSE;
		
		if( g_pPlayer->IsBullet( pSkill->GetProp() ) == FALSE )
			return FALSE;

		m_nExecute = 1;		// 1 ��ų�ٻ�� �������� 
		// ���⿣ m_idTarget�� NULL_ID�� ������ �ִ�.
		if( g_pPlayer->CMD_SetUseSkill( m_idTarget, pShortcut->m_dwId, SUT_QUEUESTART ) == 0 )		// ������ ����� ����. �̵� + ��ų����� ������ ���.
		{
			OnCancelSkill();	// ù��ų���� �����ߴٸ� ��ųť ����� ���.
		} else
		{	// success
			// 1�ܰ� ������ ap�� �Ҹ���� �ʴ´�.
			const ItemProp *pItemProp = g_pPlayer->GetActiveHandItemProp();
			if( pItemProp )
			{   // �տ� ��� �ִ°� �������� ġ�ƽ�� �ƴҶ��� NEXTSKILL_NONE�� ����. ���ϸ� ��ų�� �̾����� ����.
				if( pItemProp->dwItemKind3 != IK3_STAFF && pItemProp->dwItemKind3 != IK3_CHEERSTICK )			
					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );	// 2006/06/12 ��ųť�� ���������� ��ųť��� ��� Ŭ����.-xuzhu-
			}

		}
		return TRUE;
	} else
		g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );	// �׼ǽ�ų�� ��������� ���.

	return FALSE;
}

BOOL CWndTaskBar::Process( void )
{
	// ���� ��� �����ؾ��ϴ°͵��� ���⼭...
	if( m_nExecute )
	{
		if( m_idTarget != NULL_ID )		// m_idTarget�� NULL_ID�ΰ��� �ڱ� �ڽſ��� �����ⶫ�� ĵ�����Ѽ� �ȵȴ�.
		{
			CMover* pObj = prj.GetMover( m_idTarget );
			if( IsInvalidObj(pObj) )						// �����߿� ����� �������ٸ� ��ų����� ����.
				OnCancelSkill();
		}
//		if( pObj && pObj->IsDie() )						// ������ ����� �׾����� ��ų�� ���� ����.
//			OnCancelSkill();

//		if( (g_pPlayer->m_dwFlag & MVRF_SKILL) == 0 )
//			OnCancelSkill();													// ��ų��� ����
	}
	// �巡�� ���̶���Ʈ ���� ���� ó�� 
	if( m_GlobalShortcut.IsEmpty() == FALSE )
	{
		m_dwHighAlpha += 10;
		if( m_dwHighAlpha >= 256 )
			m_dwHighAlpha = 0;
	}
	// �巡���߿��� ��Ű�� ����� �� ���� 
	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ��Ű ó�� 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( g_bKeyTable[ VK_RETURN ] ) 
		{
			g_bKeyTable[ VK_RETURN ] = FALSE;
			CWndBase* pWndBase = GetWndBase( APP_COMMUNICATION_CHAT );
			if( pWndBase == NULL )
			{
				g_WndMng.OpenApplet( APP_COMMUNICATION_CHAT );
				CWndChat* pWndChat = (CWndChat*)GetWndBase( APP_COMMUNICATION_CHAT );
				if( pWndChat != NULL )
				{
					pWndChat->m_wndEdit.SetFocus();
				}
			}
		}
		else
		{
			BOOL bKey = FALSE;

			for (const auto & [dwIdApplet, pApplet] : g_WndMng.m_mapAppletFunc)
			{
				
#ifdef __XKEYEDGE
				bKey = g_bKeyTable[ pApplet->m_cHotkey ];
				if( bKey && !g_bKeyTable2[ pApplet->m_cHotkey ] )
#else
				if( g_bKeyTable[ pApplet->m_cHotkey ] )			//gmpbigsun: �Ͽ�¡ YŰ( ����â ) �˻� 
#endif
				{					
					g_WndMng.OpenApplet( pApplet->m_dwIdApplet );
#ifndef __XKEYEDGE					
					g_bKeyTable[ pApplet->m_cHotkey ] = FALSE;
#endif					
				}			
#ifdef __XKEYEDGE
				g_bKeyTable2[ pApplet->m_cHotkey ] = bKey;
				bKey = FALSE;
#endif

			}
			
			CWndRegVend* pWndRegVend = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_SELL);
			CWndRegVend* pWndVendorBuy = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_BUY);
			
			if( !pWndRegVend || !pWndVendorBuy )
			{
				DWORD *dwHotkey;
#ifdef __Y_INTERFACE_VER3				
				DWORD dwHotkey1[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
				DWORD dwHotkey2[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };

				if( g_Option.m_nInterface == 2 )
					dwHotkey = dwHotkey1;
				else
					dwHotkey = dwHotkey2;								
#else //__Y_INTERFACE_VER3
				DWORD dwHotkey1[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };
				dwHotkey = dwHotkey1;
#endif //__Y_INTERFACE_VER3

				for( int i = 0; i < 10; i++ )
				{
					if( g_bKeyTable[ dwHotkey[i] ] && g_bSlotSwitchAboutEquipItem[ i ] == FALSE )
					{
						LPSHORTCUT lpShortcut = &m_paSlotItem()[i];
						if( lpShortcut->m_dwShortcut == ShortcutType::Item )
						{
							if (const ItemProp * props = g_pPlayer->GetItemIdProp(lpShortcut->m_dwId)) {
								const int nPart = props->dwParts;
								if (nPart != -1) {
									g_bSlotSwitchAboutEquipItem[i] = TRUE;
								}
							}
						}
						g_WndMng.ObjectExecutor( lpShortcut );	
						g_bKeyTable[ dwHotkey[i] ] = 0;
					}
				}			
			}
		}
	}

	UpdateItem();
	
	return CWndBase::Process();
}

// ��ų�� ����� ���(�ߴ�)��.
void CWndTaskBar::OnCancelSkill( void )
{
	if( g_pPlayer->m_dwReqFlag & REQ_USESKILL )
		return;

	m_nExecute = 0;
	m_nUsedSkillQueue = 0;
	g_pPlayer->ClearDestObj();
	g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
}

// ��ų ���� �������� ȣ��.
void CWndTaskBar::OnEndSkill( void )
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �½�ũ ���� �Ŵ� ó�� ����  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void CWndTaskMenu::OnDraw(C2DRender* p2DRender)
{
	return;
	p2DRender->TextOut( 10, 20,  _T( "MASQUERADE" ),  D3DCOLOR_ARGB( 255, 250, 250, 250 ) );

	CRect rect = GetWindowRect();
	// �׵θ� �ڽ� 
	DWORD dwColor1t = D3DCOLOR_ARGB( 055,  91, 104, 205 );
	DWORD dwColor2t = D3DCOLOR_ARGB( 055, 116, 128, 220 );
	DWORD dwColor4t = D3DCOLOR_ARGB( 055, 143, 173, 245 );
	DWORD dwColor1b = D3DCOLOR_ARGB( 055,  41, 104, 155 );
	DWORD dwColor2b = D3DCOLOR_ARGB( 055,  66,  78, 170 );
	DWORD dwColor4b = D3DCOLOR_ARGB( 055, 143, 173, 245 );

	DWORD dwColor3t = D3DCOLOR_ARGB( 155, 250, 250, 250 );
	DWORD dwColor3b = D3DCOLOR_ARGB(  55,  00,  00,  00 );
	
	//p2DRender->RenderRoundRect( rect, dwColor1t, dwColor1t, dwColor1b, dwColor1b );
	rect.DeflateRect(1,1);
	//p2DRender->RenderRoundRect( rect, dwColor2t, dwColor2t, dwColor2b, dwColor2b );
//	rect.DeflateRect(1,1);

	CRect rect1 = rect; rect1.bottom = rect1.top + 40;
	CRect rect2 = rect; rect2.top    = rect2.top + 40;

	p2DRender->RenderFillRect( rect1, dwColor3t, dwColor3t, dwColor3b, dwColor3b );
	p2DRender->RenderFillRect( rect2, dwColor3b, dwColor3b, dwColor4b, dwColor4b );

//	p2DRender->RenderLine( CPoint( 10, 5 * 22 + 54 ), CPoint( 140, 5 * 22 + 54 ), dwColor1t );
	// client ���� ĥ�ϱ� 
	//OnEraseBkgnd(p2DRender);
	//.OffsetRect(-rect.TopLeft());
	//p2DRender->PaintRect( rect,MKHIGHRGB(255>>3,255>>3,255>>3) );
	//p2DRender->TextOut( 5,20,"HP : 100 / 100", dwColor1t );
	//p2DRender->TextOut( 5,35,"MP : 100 / 100", dwColor1t );
}
BOOL CWndTaskMenu::Process()
{
	if(	!CWndBase::m_GlobalShortcut.IsEmpty() )
		return CWndBase::Process();

	if( IsVisible() == FALSE )
		return CWndBase::Process();

	for (auto & pWndButton : m_wndMenuItems) {
		
		if( pWndButton->GetClientRect( TRUE ).PtInRect( m_ptMouse ) )
		{
			// ��� ����� 
			if (pWndButton->m_pWndMenu == NULL || pWndButton->m_pWndMenu->IsVisible() == FALSE) {
				for (auto & subButton : m_wndMenuItems) {
					if (subButton->m_pWndMenu) {
						subButton->m_pWndMenu->SetVisibleSub(FALSE);
					}
				}
			}
			// �� �Ŵ��� ���̰� ��Ŀ�� �ֱ� 
			if( pWndButton->m_pWndMenu )
			{
				if( pWndButton->m_pWndMenu->IsVisible() == FALSE )
				{
					CRect rect = pWndButton->GetScreenRect();
					pWndButton->m_pWndMenu->Move( CPoint( rect.right , rect.top ) );
				}
				// �޴��� ��ǥ ���� 
				CRect rcButton = pWndButton->GetScreenRect();
				pWndButton->m_pWndMenu->Move( CPoint( rcButton.right, rcButton.top ) );
				// �׷��� �� �޴��� ȭ���� ����ٸ� ��ġ�� ���� 
				CRect rcMenu = pWndButton->m_pWndMenu->GetScreenRect();
				CRect rcLayout = m_pWndRoot->GetLayoutRect();
				CPoint pt = rcMenu.TopLeft();
				if( rcMenu.right > rcLayout.right )
				{
					pt.x = rcButton.left - rcMenu.Width();
					//pWndButton->m_pWndMenu->Move( CPoint( rcButton.left - rcMenu.Width(), rcButton.top ) );
				}
				if( rcMenu.bottom > rcLayout.bottom )
				{
					pt.y -= rcMenu.bottom - rcLayout.bottom;
					//pWndButton->m_pWndMenu->Move( CPoint( rcButton.left - rcMenu.Width(), rcButton.top ) );
				}
				pWndButton->m_pWndMenu->Move( pt );
				pWndButton->m_pWndMenu->SetVisible( TRUE );
				pWndButton->m_pWndMenu->SetFocus();
			}
			break;
		}
	}
	return CWndBase::Process();
}

void CWndTaskMenu::OnInitialUpdate()
{
	CWndMenu::OnInitialUpdate();

	if (g_pPlayer->IsAuthHigher(AUTH_GAMEMASTER)) {
		// TODO: it should not be required to pass the TID to AddApplet
		AddApplet(APP_DEBUGINFO, TID_APP_DEBUGINFO);
	}

	AddApplet(APP_STATUS1   , TID_APP_STATUS);
	AddApplet(APP_NAVIGATOR , TID_APP_NAVIGATOR);
	AddApplet(APP_CHARACTER3, TID_APP_CHARACTER);
	AddApplet(APP_INVENTORY , TID_APP_INVENTORY);

#ifndef __TMP_POCKET
	AddApplet(APP_BAG_EX         , TID_APP_BAG_EX);
#endif
	AddApplet(APP_HOUSING        , TID_GAME_HOUSING_BOX);
	AddApplet(APP_VENDOR_REVISION, TID_APP_VENDOR);
	AddApplet(APP_SKILL3         , TID_APP_SKILL);
	AddApplet(APP_QUEST_EX_LIST  , TID_APP_QUEST);
	AddApplet(APP_MOTION         , TID_APP_MOTION);

#ifdef __IMPROVE_MAP_SYSTEM
	AddApplet(APP_MAP_EX         , TID_APP_MAP);
#else // __IMPROVE_MAP_SYSTEM
	AddApplet(APP_MAP            , TID_APP_MAP);
#endif // __IMPROVE_MAP_SYSTEM

	// Note: pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture(pWndButton);
	// is it required for these applets?
	AddFolder(TID_APP_COMMITEM)
		.AddApplet(APP_WEBBOX, TID_APP_ITEMMALL)
		.AddApplet(APP_COMM_ITEM, TID_APP_ITEMTIME);

	AddFolder(TID_APP_COMMUNICATION)
		.AddApplet(APP_COMMUNICATION_CHAT, TID_APP_COMMUNICATION_CHAT)
		.AddApplet(APP_MESSENGER_, TID_APP_MESSENGER)
		.AddApplet(APP_COUPLE_MAIN, TID_GAME_COUPLE);
	

	AddFolder(TID_APP_COMMUNITY)
		.AddApplet(APP_PARTY, TID_APP_PARTY)
		.AddApplet(APP_GUILD, TID_APP_COMPANY)
#ifdef __GUILDVOTE
		.AddApplet(APP_GUILD_VOTE, TID_GAME_TOOLTIP_GUILDVOTE)
#endif
		;

	AddFolder(TID_APP_INFOMATION)
		.AddApplet(APP_INFO_NOTICE, TID_APP_INFO_NOTICE);

	const auto adderForHelp = AddFolder(TID_APP_HELPER)
		.AddApplet(APP_HELPER_HELP, TID_APP_HELPER_HELP)
		.AddApplet(APP_HELPER_TIP, TID_APP_HELPER_TIP)
		.AddApplet(APP_HELPER_FAQ, TID_APP_HELPER_FAQ)
		.AddApplet(APP_INFOPANG, TID_APP_INFOPANG);

#ifdef __NEW_WEB_BOX
#ifdef __INTERNALSERVER
	adderForHelp.AddApplet(APP_WEBBOX2, TID_GAME_HELPER_WEB_BOX_ICON_TITLE);
#else // __INTERNALSERVER
	if ((GetLanguage() == LANG_ENG && GetSubLanguage() == LANG_SUB_USA)) {
		adderForHelp.AddApplet(APP_WEBBOX2, TID_GAME_HELPER_WEB_BOX_ICON_TITLE);
	}
#endif // __INTERNALSERVER
#endif // __NEW_WEB_BOX

	AddApplet(APP_OPTIONEX, TID_APP_OPTION);
	AddApplet(APP_LOGOUT  , TID_APP_LOGOUT);
	AddApplet(APP_QUIT    , TID_APP_QUIT);

	CWndBase::SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, _T( "WndTaskMenu.tga" ) ), TRUE );

	CRect nextRectSurface(10, 50, m_pTexture->m_size.cx - 20, 50 + 20);

	for (auto & pWndButton : m_wndMenuItems) {
		pWndButton->SetWndRect(nextRectSurface);
		nextRectSurface.OffsetRect(CSize(0, 22));
	}

	SetWndRect(CRect(CPoint(0, 0), m_pTexture->m_size));
}

void CWndTaskMenu::AddApplet(DWORD appId, DWORD textId) {
	MakeButton(this, 0, appId, prj.GetText(textId));
}

CWndTaskMenu::FolderAdder CWndTaskMenu::AddFolder(DWORD textId) {
	CWndButton * menuButton = MakeButton(this, 0, 0, prj.GetText(textId));
	menuButton->SetTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ICON, _T("icon_Folder.dds")));

	CWndMenu * menu = new CWndMenu;
	menu->CreateMenu(this);
	m_menus.emplace_back(menu);
	menuButton->SetMenu(menu);
	return FolderAdder(menu);
}

CWndButton* CWndTaskMenu::MakeButton( CWndMenu* pWndMenu, UINT nFlags, UINT nIDNewItem,	LPCTSTR lpszNewItem )
{
	CWndButton* pWndButton = (CWndButton*)pWndMenu->AppendMenu( nFlags, nIDNewItem, lpszNewItem );
	
	const AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( nIDNewItem ); 
	if (!pAppletFunc) return pWndButton;

	pWndButton->m_cHotkey = pAppletFunc->m_cHotkey;
	CString string = pAppletFunc->m_pAppletDesc;
	if (pAppletFunc->m_cHotkey != 0) {
		string.AppendFormat("\n[%s %c]", pAppletFunc->m_pAppletDesc, prj.GetText(TID_GAME_TOOLTIP_HOTKEY), pAppletFunc->m_cHotkey);
	}
	pWndButton->m_strToolTip = string;
	pWndButton->m_shortcut.m_dwShortcut = ShortcutType::Applet; 
	pWndButton->SetTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ICON, g_WndMng.GetAppletFunc(pWndButton->GetWndId())->m_pszIconName));
	
	return pWndButton;
}

BOOL CWndTaskMenu::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	CreateMenu( &g_WndMng );
	SetVisible(TRUE);
	return TRUE;
}

void CWndTaskMenu::OnKillFocus(CWndBase* pNewWnd)
{
	if(g_WndMng.m_pWndTaskBar != pNewWnd)
		CWndMenu::OnKillFocus(pNewWnd);
}

BOOL CWndTaskMenu::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return TRUE;
}
BOOL CWndTaskMenu::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	if( message == WNM_CLICKED )
		g_WndMng.OpenApplet( nID );
	return TRUE;
}

void CWndTaskMenu::OnLButtonUp(UINT nFlags, CPoint point)
{
}
void CWndTaskMenu::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CWndTaskMenu::PaintFrame( C2DRender* p2DRender )
{
	CWndBase::PaintFrame( p2DRender );
}
BOOL CWndTaskMenu::OnEraseBkgnd( C2DRender* p2DRender )
{
	return CWndBase::OnEraseBkgnd( p2DRender );
}


void CWndTaskBar::RenderCollTime(CPoint pt, DWORD dwSkillId, C2DRender* p2DRender )
{
	DWORD dwDelay = g_pPlayer->GetReuseDelay( dwSkillId );
	if( dwDelay > 0 )
	{
		SKILL * skill = g_pPlayer->GetSkill(dwSkillId);
		if (!skill) return;

		const ItemProp* pSkillProp	= skill->GetProp();
		ASSERT( pSkillProp );
		AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, skill->dwLevel );
		ASSERT( pAddSkillProp );
		RenderRadar( p2DRender, pt, 
			            pAddSkillProp->dwCooldown - dwDelay, 
						pAddSkillProp->dwCooldown );	
	}
}

	void CWndTaskBar::RenderLordCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender )
	{
		CCLord* pLord									= CCLord::Instance();
		CLordSkillComponentExecutable* pComponent		= pLord->GetSkills()->GetSkill(dwSkillId);
		if(!pComponent) return;
		DWORD dwDelay = 0;
		if(pComponent->GetTick() > 0)
			RenderRadar( p2DRender, pt, pComponent->GetCooltime() - pComponent->GetTick(), pComponent->GetCooltime() );	
	}

void CWndTaskBar::RenderOutLineLamp(int x, int y, int num, DWORD size)
{
	CPoint Point = CPoint( x, y );
	LONG   thick = 10;

	D3DXCOLOR dwColorDest2  = D3DCOLOR_ARGB( 0, 40, 0, 255 );

	D3DXCOLOR dwColor  = D3DCOLOR_ARGB( 196, 40, 0, 255 );

	D3DXCOLOR dwColor1 = D3DCOLOR_ARGB( 0, 255, 255, 255 );
	D3DXCOLOR dwColor2 = D3DCOLOR_ARGB( 0, 255, 255, 255 );

	static BOOL  bReverse = FALSE;
	static FLOAT fLerp = 0.0f;

	if( bReverse )
		fLerp-=0.05f;
	else
		fLerp+=0.05f;

	if( fLerp > 1.0f )
	{
		bReverse = TRUE;
		fLerp = 1.0f;
	}
	else
	if( fLerp < 0.0f )
	{
		bReverse = FALSE;
		fLerp = 0.0f;
	}
	
	D3DXColorLerp( &dwColor2, &dwColor, &dwColorDest2, fLerp);
	
	CRect Rect = CRect( 0, 0, (size * num), size );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left)-thick, Point.y+Rect.top, (Point.x+Rect.left), Point.y+Rect.bottom ),
		dwColor1, dwColor2, dwColor1, dwColor2 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left), (Point.y+Rect.top)-thick, (Point.x+Rect.right), Point.y+Rect.top ),
		dwColor1, dwColor1, dwColor2, dwColor2 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.right), Point.y+Rect.top, (Point.x+Rect.right)+thick, Point.y+Rect.bottom ),
		dwColor2, dwColor1, dwColor2, dwColor1 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left), (Point.y+Rect.bottom), (Point.x+Rect.right), (Point.y+Rect.bottom)+thick ),
		dwColor2, dwColor2, dwColor1, dwColor1 );
}

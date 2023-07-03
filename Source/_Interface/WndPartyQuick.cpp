#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndManager.h"
#include "Party.h"
#include "WndParty.h"

#include "playerdata.h"

static constexpr std::array<UINT, CWndPartyQuick::MaxPartyMember> StaticID = {
	WIDC_STATIC_MEM1, WIDC_STATIC_MEM2, WIDC_STATIC_MEM3,
	WIDC_STATIC_MEM4, WIDC_STATIC_MEM5, WIDC_STATIC_MEM6,
	WIDC_STATIC_MEM7, WIDC_STATIC_MEM8
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWndPartyQuick
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndPartyQuick::CWndPartyQuick() {
	m_pWndMemberStatic.fill(nullptr);
}

CWndPartyQuick::~CWndPartyQuick() {
	DeleteDeviceObjects();
}

HRESULT CWndPartyQuick::RestoreDeviceObjects() {
	CWndBase::RestoreDeviceObjects();
	if (!m_pVBGauge) {
		return m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBGauge, NULL);
	}
	return S_OK;
}

HRESULT CWndPartyQuick::InvalidateDeviceObjects() {
	CWndBase::InvalidateDeviceObjects();
	SAFE_RELEASE(m_pVBGauge);
	return S_OK;
}

HRESULT CWndPartyQuick::DeleteDeviceObjects() {
	CWndBase::DeleteDeviceObjects();
	SAFE_RELEASE(m_pVBGauge);
	return S_OK;
}

void CWndPartyQuick::OnDraw( C2DRender* p2DRender ) 
{ 
	// ��Ƽ ���� ���
	//Static control
	int nMax = g_Party.m_nSizeofMember;

	SetActiveMember(nMax);

	for(int i=0; i<nMax; i++)
	{
		LPWNDCTRL lpWndCtrl = GetWndCtrl( StaticID[i] );
		CRect rect = lpWndCtrl->rect;
		CMover* pObjMember = prj.GetUserByID( g_Party.m_aMember[i].m_uPlayerId );
		CString strMember;
		
		if(m_pFocusMember == pObjMember)
			p2DRender->RenderFillRect( rect, 0x60ffff00 );

		// ���¿� ���� �� ����
		DWORD dwColor = 0xff000000;
		u_long  nLeadMember = g_Party.m_aMember[i].m_uPlayerId;
		if( IsValidObj(pObjMember) )
		{
			if( pObjMember->GetHitPoint() == 0 ) 
				dwColor = 0xffff0000; // ������
			else if( ((FLOAT)pObjMember->GetHitPoint()) / ((FLOAT)pObjMember->GetMaxHitPoint()) <.1f ) 
				dwColor = 0xffffff00; // HP 10% �����γ�
			if (g_Party.IsLeader(nLeadMember)) //Leader Color Set
			{
				dwColor = 0xff1fb72d; //���� �ؾ���...
			}

			strMember = FormatPlayerName(pObjMember->GetLevel(), pObjMember->GetJob(), pObjMember->GetName());
		}
		else
		{
			dwColor = 0xff878787; // ����Ʈ�� ������ ���³�
			if (g_Party.m_aMember[i].m_remove)  dwColor = 0xff000000; // ������ ���³�
			const char* pszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( g_Party.m_aMember[i].m_uPlayerId );

			PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( g_Party.m_aMember[i].m_uPlayerId );
			int nLevel	= pPlayerData->data.nLevel;
			int nJob	= pPlayerData->data.nJob;

			strMember = FormatPlayerName(nLevel, static_cast<DWORD>(nJob), pszPlayer);
		}
		//Member - Level, Name Draw
		//�� �̸��� ... ����.
		strings::ReduceSize(strMember, 13);
		p2DRender->TextOut( rect.TopLeft().x+5, rect.TopLeft().y+5, strMember, dwColor );

		//Member - Gauge Draw
		rect.TopLeft().y += 18;
		rect.TopLeft().x += 2;
		rect.BottomRight().x -= 2;
		int nWidth	= pObjMember ? pObjMember->GetHitPointPercent( rect.Width() ) : 0;
		CRect rectTemp = rect; 
		rectTemp.right = rectTemp.left + nWidth;
		if( rect.right < rectTemp.right )
			rectTemp.right = rect.right;
		m_Theme.RenderGauge( p2DRender, &rect    , 0xffffffff, m_pVBGauge, &m_texGauEmptyNormal );
		m_Theme.RenderGauge( p2DRender, &rectTemp, 0x64ff0000, m_pVBGauge, &m_texGauFillNormal );
	}
} 

CString CWndPartyQuick::FormatPlayerName(int level, DWORD dwJob, const char * name) {
	CString result;
	
	if (MAX_PROFESSIONAL <= dwJob && dwJob < MAX_MASTER)
		result.Format("%d%s", level, prj.GetText(TID_GAME_TOOLTIP_MARK_MASTER));
	else if (MAX_MASTER <= dwJob)
		result.Format("%d%s", level, prj.GetText(TID_GAME_TOOLTIP_MARK_HERO));
	else
		result.Format("%d", level);

	result.AppendFormat(". %s", name);

	return result;
}

void CWndPartyQuick::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	m_pBtnParty = GetDlgItem<CWndButton>( WIDC_BUTTON1 );

	for (size_t i = 0; i < MaxPartyMember; i++) {
		m_pWndMemberStatic[i] = GetDlgItem<CWndStatic>(StaticID[i]);
	}

	m_texGauEmptyNormal.LoadTexture( MakePath( DIR_THEME, "GauEmptySmall.bmp" ), 0xffff00ff, TRUE );
	m_texGauFillNormal.LoadTexture( MakePath( DIR_THEME, "GauFillSmall.bmp" ), 0xffff00ff, TRUE );
	
	SetActiveMember(g_Party.m_nSizeofMember);
	
	const CRect rectRoot = g_WndMng.GetLayoutRect();
	const CPoint point( 30, (rectRoot.bottom - rectRoot.top) / 4);
	Move(point);
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartyQuick::Initialize( CWndBase* pWndParent )
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	if(g_Party.GetSizeofMember() < 2) //���۰� �Բ� ���� ��� Quickâ�� ���� �ʵ��� �Ѵ�. Party������ ���� ���� �Ұ�.
		return FALSE;
	return CWndNeuz::InitDialog( APP_PARTY_QUICK, pWndParent, 0, CPoint( 0, 0 ) );
} 

void CWndPartyQuick::OnLButtonUp(UINT nFlags, CPoint point) {
	CWndTaskBar * pTaskBar = g_WndMng.m_pWndTaskBar;
	if (g_WndMng.m_pWndWorld->m_bAutoAttack || pTaskBar->m_nExecute != 0)
		return;

	for (int i = 0; i < g_Party.m_nSizeofMember; i++) {
		const WNDCTRL * const lpWndCtrl = GetWndCtrl(StaticID[i]);
		const CRect rect = lpWndCtrl->rect;
		if (!rect.PtInRect(point)) continue;

		g_WndMng.m_pWndWorld->m_pSelectRenderObj = NULL; //���콺�� �ɷ� �׷������� ����� Ÿ���� ��ƾ� �Ȱ�ģ��..
		m_pFocusMember = prj.GetUserByID(g_Party.m_aMember[i].m_uPlayerId);
		if (g_pPlayer != m_pFocusMember) {
			if (IsValidObj(m_pFocusMember)) {
				g_WorldMng()->SetObjFocus(m_pFocusMember);
				g_WndMng.m_pWndWorld->m_pRenderTargetObj = NULL;
			}
		} else {
			g_WorldMng()->SetObjFocus(NULL);
		}

		return;
	}
}

BOOL CWndPartyQuick::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	if (nID == WIDC_BUTTON1) {
		m_pWndParty = (CWndParty *)g_WndMng.GetApplet(APP_PARTY);
		if (!m_pWndParty) {
			m_pWndParty = (CWndParty *)g_WndMng.CreateApplet(APP_PARTY);
		} else {
			m_pWndParty->Destroy();
			m_pWndParty = nullptr;
		}
	}

	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

void CWndPartyQuick::SetActiveMember(size_t MemberNum) {
	if (m_MemberCount == MemberNum) return;

	MemberNum = std::min(MemberNum, MaxPartyMember);

	CRect rect = GetWindowRect(TRUE);
	int height = 60;

	size_t i;
	for (i = 0; i < MemberNum; i++) {
		CWndStatic * lpWndStatic = GetDlgItem<CWndStatic>(StaticID[i]);
		lpWndStatic->EnableWindow(TRUE);
		lpWndStatic->SetVisible(TRUE);
		height += 34;
	}

	for (i = MemberNum; i < MaxPartyMember; i++) {
		CWndStatic * lpWndStatic = GetDlgItem<CWndStatic>(StaticID[i]);
		lpWndStatic->EnableWindow(FALSE);
		lpWndStatic->SetVisible(FALSE);
	}

	rect.bottom = rect.top + height;

	//Button Move
	CRect btnRect;
	btnRect.top = height - 50;
	btnRect.bottom = btnRect.top + 10;
	btnRect.left = 114;
	btnRect.right = btnRect.left + 10;
	m_pBtnParty->SetWndRect(btnRect);

	SetWndRect(rect);
	AdjustWndBase();
	m_MemberCount = MemberNum;
}

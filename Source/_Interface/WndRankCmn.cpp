#include "StdAfx.h"
#include "WndRankCmn.h"
#include "defineText.h"
#include "AppDefine.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CWndRankTab::OnDraw(C2DRender * p2DRender) {
	static constexpr DWORD dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);

	static constexpr int sx = 8;
	int sy = 32;

	CWndWorld * const pWndWorld = (CWndWorld *)g_WndMng.GetWndBase(APP_WORLD);
	const auto & ranked = CGuildRank::Instance[m_rank];

	TEXTUREVERTEX2 pVertex[10 * 6];
	TEXTUREVERTEX2 * pVertices = pVertex;

	const int maxBound = std::min(m_nCurrentList + 10, static_cast<int>(ranked.size()));
	for (int i = m_nCurrentList; i < maxBound; ++i) {

		// ��� ���� �α�
		int nNo = i;
		if (i < 1) nNo = 0;
		else if (i < 3) nNo = 1;
		else if (i < 7) nNo = 2;
		else if (i < 12) nNo = 3;
		else nNo = 4;
		
		pWndWorld->m_texMsgIcon.MakeVertex(p2DRender, CPoint(sx + 0, sy - 3), 49 + nNo, &pVertices, 0xffffffff);

		// ��� ����
		p2DRender->TextOut(sx + 20, sy, i + 1, dwColor);

		// ��� �α�

		const CGuildRank::GUILD_RANKING & guild = ranked[i];

		if (guild.m_dwLogo != 0) {

			CPoint point = CPoint(sx + 42, sy - 5);

			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			pWndWorld->m_pTextureLogo[guild.m_dwLogo - 1].Render(&g_Neuz.m_2DRender, point, 255);
			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// ����
		p2DRender->TextOut(sx + 72, sy, guild.m_szGuild, dwColor);

		// �����͸� (�� ��� ... ����)
		CString strMasterName;
		strMasterName.Format("%s", guild.m_szName);
		strMasterName.TrimRight();
		sqktd::ReduceSize(strMasterName, 12);
		p2DRender->TextOut(sx + 210, sy, strMasterName, dwColor);

		// �� / ��
		
		const ValuesToPrint toPrint = GetValuesToPrint(guild);
		if (toPrint.size() == 1) {
			p2DRender->TextOut(sx + 350, sy, toPrint[0], dwColor);
		} else if (toPrint.size() == 2) {
			CString str;
			str.Format("%5d  /  %5d", toPrint[0], toPrint[1]);
			p2DRender->TextOut(sx + 330, sy, str, dwColor);
		}

		sy += 25;
	}

	pWndWorld->m_texMsgIcon.Render(m_pApp->m_pd3dDevice, pVertex, ((int)pVertices - (int)pVertex) / sizeof(TEXTUREVERTEX2));
}
void CWndRankTab::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	// ���⿡ �ڵ��ϼ���

	m_nCurrentList = 0;
	m_nMxOld = m_nMyOld = 0;

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRankTab::Initialize(CWndBase * pWndParent, DWORD) {
	// TODO: this function is probably never called -> delete it
	return CWndNeuz::InitDialog(m_appId, pWndParent, 0, CPoint(0, 0));
}

void CWndRankTab::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags & MK_LBUTTON) {
		int		nDistY = (m_nMyOld - point.y) / 5;		// ���� ��ǥ���� ����.
		MoveCurrentList(nDistY);
	}

	m_nMxOld = point.x;
	m_nMyOld = point.y;
}

BOOL CWndRankTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	MoveCurrentList(zDelta > 0 ? -3 : +3);
	return TRUE;
}

void CWndRankTab::MoveCurrentList(int delta) {
	m_nCurrentList += delta;

	if (CGuildRank::Instance[m_rank].size() <= 10) {
		m_nCurrentList = 0;
	} else {
		m_nCurrentList = std::clamp(
			m_nCurrentList,
			0,
			static_cast<int>(CGuildRank::Instance[m_rank].size() - 10)
		);
	}
}


///////////////////////////////////////////////////////////////////////////////

void CWndTabCtrlManager::Add(CWndTabCtrl & tabCtrl, CRect rect, CWndBase & tab, DWORD widgetId, DWORD textId) {
	tab.Create(WBS_CHILD | WBS_NODRAWFRAME, rect, &tabCtrl, widgetId);
	tabCtrl.InsertItem(&tab, prj.GetText(textId));
}

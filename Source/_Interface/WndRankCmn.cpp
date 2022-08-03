#include "StdAfx.h"
#include "WndRankCmn.h"
#include "defineText.h"
#include "AppDefine.h"

void CWndRankTab::OnDraw(C2DRender * p2DRender) {
	int		i, sx, sy;
	DWORD	dwColor;

	sx = 8;
	sy = 32;

	CWndWorld * pWndWorld = (CWndWorld *)g_WndMng.GetWndBase(APP_WORLD);
	CGuildRank * pGuildRank = CGuildRank::Instance();

	TEXTUREVERTEX2 * pVertex = new TEXTUREVERTEX2[pGuildRank->m_Total[m_rank] * 6];
	TEXTUREVERTEX2 * pVertices = pVertex;

	for (i = m_nCurrentList; i < m_nCurrentList + 10; ++i) {
		if (i >= pGuildRank->m_Total[m_rank])
			break;

		dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);

		// ��� ���� �α�
		int nNo = i + 1;
		if (nNo < 2)
			nNo = 0;
		else if (nNo < 4)
			nNo = 1;
		else if (nNo < 8)
			nNo = 2;
		else if (nNo < 13)
			nNo = 3;
		else
			nNo = 4;
		pWndWorld->m_texMsgIcon.MakeVertex(p2DRender, CPoint(sx + 0, sy - 3), 49 + nNo, &pVertices, 0xffffffff);

		// ��� ����
		p2DRender->TextOut(sx + 20, sy, i + 1, dwColor);

		// ��� �α�

		if (pGuildRank->m_Ranking[m_rank][i].m_dwLogo != 0) {

			CPoint point = CPoint(sx + 42, sy - 5);

			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			pWndWorld->m_pTextureLogo[pGuildRank->m_Ranking[m_rank][i].m_dwLogo - 1].Render(&g_Neuz.m_2DRender, point, 255);
			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			p2DRender->m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		// ����
		CString guildname = pGuildRank->m_Ranking[m_rank][i].m_szGuild;
		p2DRender->TextOut(sx + 72, sy, guildname, dwColor);

		// �����͸� (�� ��� ... ����)
		CString strMasterName;
		strMasterName.Format("%s", pGuildRank->m_Ranking[m_rank][i].m_szName);
		strMasterName.TrimRight();
		sqktd::ReduceSize(strMasterName, 12);
		p2DRender->TextOut(sx + 210, sy, strMasterName, dwColor);

		// �� / ��
		CString strWin = ToString(pGuildRank->m_Ranking[m_rank][i]);
		p2DRender->TextOut(sx + 330, sy, strWin, dwColor);

		sy += 25;
	}

	pWndWorld->m_texMsgIcon.Render(m_pApp->m_pd3dDevice, pVertex, ((int)pVertices - (int)pVertex) / sizeof(TEXTUREVERTEX2));
	//	delete pVertex;
	SAFE_DELETE_ARRAY(pVertex);
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
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog(m_appId, pWndParent, 0, CPoint(0, 0));
}

void CWndRankTab::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags & MK_LBUTTON) {
		int		nDistY = (m_nMyOld - point.y) / 5;		// ���� ��ǥ���� ����.

		m_nCurrentList += nDistY;
		if (m_nCurrentList < 0)
			m_nCurrentList = 0;
		if ((m_nCurrentList + 10 - 1) >= CGuildRank::Instance()->m_Total[m_rank]) {
			m_nCurrentList = CGuildRank::Instance()->m_Total[m_rank] - 10;
			if (m_nCurrentList < 0)
				m_nCurrentList = 0;
		}

	}

	m_nMxOld = point.x;
	m_nMyOld = point.y;
}

BOOL CWndRankTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	int		nZoom = 64;

	if (zDelta > 0)
		m_nCurrentList -= 3;
	else
		m_nCurrentList += 3;

	if (m_nCurrentList < 0)
		m_nCurrentList = 0;
	if ((m_nCurrentList + 10 - 1) >= CGuildRank::Instance()->m_Total[m_rank]) {
		m_nCurrentList = CGuildRank::Instance()->m_Total[m_rank] - 10;
		if (m_nCurrentList < 0)
			m_nCurrentList = 0;
	}

	return TRUE;
}


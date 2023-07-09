#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndParty.h"
#include "WndManager.h"
#include "Party.h"
#include "dpclient.h"
#include "playerdata.h"
#include "MsgHdr.h"

/****************************************************
  WndId : APP_PARTY - �ش�(Party)
  CtrlId : WIDC_TABCTRL1 - TabCtrl
  CtrlId : WIDC_LEAVE - Ż��
  CtrlId : WIDC_CHANGE - �ش���ȯ
****************************************************/

CWndParty::~CWndParty() {
	SAFE_DELETE(m_WndPartyChangeTroup);
	DeleteDeviceObjects();
}

void CWndParty::SerializeRegInfo(CAr & ar, DWORD & dwVersion) {
	CWndNeuz::SerializeRegInfo(ar, dwVersion);
	CWndTabCtrl * lpTabCtrl = (CWndTabCtrl *)GetDlgItem(WIDC_TABCTRL1);
	if (ar.IsLoading()) {
		if (dwVersion == 0) {
			// noop
		} else if (dwVersion == 1) {
			ar >> *lpTabCtrl;
		}
	} else {
		dwVersion = 1;
		ar << *lpTabCtrl;
	}
}

void CWndParty::OnDraw(C2DRender * p2DRender) {
	static constexpr auto ToBOOL = [](bool v) -> BOOL { return v ? TRUE : FALSE; };

	const bool partyExist = g_Party.GetSizeofMember() >= 2;
	m_pWndLeave->EnableWindow(ToBOOL(partyExist));
	m_pWndTransfer->EnableWindow(ToBOOL(partyExist));
	m_pBtnPartyQuick->EnableWindow(ToBOOL(partyExist));

#ifdef __PARTY_DEBUG_0129		// �ش��� ƨ��� ���� ����� neuz
	if (g_Party.GetSizeofMember() < 2 && g_Party.m_nLevel > 1) {
		m_pWndLeave->EnableWindow(TRUE);
	}
#endif	// __PARTY_DEBUG_0129		// �ش��� ƨ��� ���� ����� neuz

	const bool canChange = g_Party.GetLevel() >= 10 && g_Party.IsLeader(g_pPlayer->m_idPlayer);
	m_pWndChange->EnableWindow(ToBOOL(canChange));

	// ��Ƽ ���� ���
	const char * partyName;
	if (g_Party.m_nKindTroup == 0) {
		partyName = prj.GetText(TID_GAME_PARTY1);
	} else if (0 == strlen(g_Party.m_sParty)) {
		partyName = prj.GetText(TID_GAME_PARTY2);
	} else {
		partyName = g_Party.m_sParty;
	}

	GetDlgItem<CWndStatic>(WIDC_NAME)
		->SetTitle(partyName);

	CString strTemp;
	strTemp.Format("%d", g_Party.m_nLevel);
	GetDlgItem<CWndStatic>(WIDC_LEVEL)
		->SetTitle(strTemp);

	if (g_Party.m_nLevel >= 10) {
		strTemp.Format("%.2f %%", (float)g_Party.m_nExp * 100 / (float)(((50 + g_Party.GetLevel()) * g_Party.GetLevel() / 13) * 10));
	} else {
		strTemp.Format("%.2f %%", (float)g_Party.m_nExp * 100 / (float)prj.m_aExpParty[g_Party.m_nLevel].Exp);
	}
	GetDlgItem<CWndStatic>(WIDC_EXP)
		->SetTitle(strTemp);

	strTemp.Format("%d", g_Party.m_nPoint);
	GetDlgItem<CWndStatic>(WIDC_POINT)
		->SetTitle(strTemp);
	// ������ ����ġ �й��� ���� ����

	GetDlgItem<CWndButton>(WIDC_EXP_SHARE)
		->SetCheck(ToBOOL(g_Party.m_nTroupsShareExp == CParty::ShareExpMode::Level));
	GetDlgItem<CWndButton>(WIDC_RADIO6)
		->SetCheck(ToBOOL(g_Party.m_nTroupsShareExp == CParty::ShareExpMode::Contribution));

	GetDlgItem<CWndButton>(WIDC_ITEM_SHARE)
		->SetCheck(ToBOOL(g_Party.m_nTroupeShareItem == CParty::ShareItemMode::Self));
	GetDlgItem<CWndButton>(WIDC_RADIO2)
		->SetCheck(ToBOOL(g_Party.m_nTroupeShareItem == CParty::ShareItemMode::RoundRobin));
	GetDlgItem<CWndButton>(WIDC_RADIO3)
		->SetCheck(ToBOOL(g_Party.m_nTroupeShareItem == CParty::ShareItemMode::Leader));
	GetDlgItem<CWndButton>(WIDC_RADIO4)
		->SetCheck(ToBOOL(g_Party.m_nTroupeShareItem == CParty::ShareItemMode::Random));
}

void CWndParty::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	m_pWndLeave = GetDlgItem<CWndButton>(WIDC_LEAVE);
	m_pWndChange = GetDlgItem<CWndButton>(WIDC_CHANGE);
	m_pWndTransfer = GetDlgItem<CWndButton>(WIDC_BUTTON1);
	m_pBtnPartyQuick = GetDlgItem<CWndButton>(WIDC_BUTTON2);
	if( g_Party.GetSizeofMember() < 2 )
	{
		m_pWndTransfer->EnableWindow( FALSE );
		m_pWndLeave->EnableWindow( FALSE );
		m_pBtnPartyQuick->EnableWindow( FALSE );
	}
	m_pWndChange->EnableWindow( FALSE );
	
	CWndTabCtrl * pWndTabCtrl = GetDlgItem<CWndTabCtrl>(WIDC_TABCTRL1);
	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 15;

	m_wndPartyInfo.Create(WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_PARTY_INFO);
	pWndTabCtrl->InsertItem(&m_wndPartyInfo, prj.GetText(TID_APP_INFOMATION));
	
	m_wndPartySkill.Create(WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_PARTY_SKILL);
	pWndTabCtrl->InsertItem(&m_wndPartySkill, prj.GetText(TID_APP_SKILL));

	MoveParentCenter();
} 

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndParty::Initialize(CWndBase * pWndParent) {
	return CWndNeuz::InitDialog(APP_PARTY, pWndParent, 0, CPoint(0, 0));
}

BOOL CWndParty::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_LEAVE ) 
	{ // Ż�� ���ȴ�...
		OnLeave();
	}
	// �ش��� �ΰ�.
	else if( nID==WIDC_BUTTON1 )
	{
		OnChangeLeader();
	}
	else if( nID==WIDC_CHANGE )
	{
		if( g_Party.m_nKindTroup == 0 )
		{
			// ��ȸ�ش����� �ٲٱ�~
			SAFE_DELETE( m_WndPartyChangeTroup );
			m_WndPartyChangeTroup = new CWndPartyChangeTroup;
			m_WndPartyChangeTroup->Initialize( this );
		}
	}
	else if(nID == WIDC_BUTTON2)
	{
		if(g_Party.GetSizeofMember() >= 2)
		{
			m_pWndPartyQuick = (CWndPartyQuick*)g_WndMng.GetApplet( APP_PARTY_QUICK );
			if(m_pWndPartyQuick == NULL)
			{
				m_pWndPartyQuick = (CWndPartyQuick*)g_WndMng.CreateApplet( APP_PARTY_QUICK );
				m_pWndPartyQuick->SetActiveMember(g_Party.GetSizeofMember());
			}
			else
			{
				m_pWndPartyQuick->Destroy();
				m_pWndPartyQuick = NULL;
			}
		}
	}
	// ��ȸ�ش��϶��� ������ ����ġ �й��� ����
	else
	if( g_Party.IsLeader(g_pPlayer->m_idPlayer) )
	{
		if( g_Party.m_nKindTroup != 0 )
		{
			// ����ġ �й� ����� ��ȸ�ش� ��
			CParty::ShareExpMode nTroupsShareExp = g_Party.m_nTroupsShareExp;
			
			if(nID==WIDC_EXP_SHARE) nTroupsShareExp   = CParty::ShareExpMode::Level;
			else if(nID==WIDC_RADIO6) nTroupsShareExp = CParty::ShareExpMode::Contribution;

			if (nTroupsShareExp != g_Party.m_nTroupsShareExp) {
				g_DPlay.SendPacket<PACKETTYPE_PARTYCHANGEEXPMODE, CParty::ShareExpMode>(nTroupsShareExp);
			}
		}
		// ������ �й� ����� �ܸ��ش� ��
		CParty::ShareItemMode nTroupeShareItem = g_Party.m_nTroupeShareItem;

		if (nID == WIDC_ITEM_SHARE) nTroupeShareItem = CParty::ShareItemMode::Self;
		else if (nID == WIDC_RADIO2) nTroupeShareItem = CParty::ShareItemMode::RoundRobin;
		else if (nID == WIDC_RADIO3) nTroupeShareItem = CParty::ShareItemMode::Leader;
		else if (nID == WIDC_RADIO4) nTroupeShareItem = CParty::ShareItemMode::Random;

		if (nTroupeShareItem != g_Party.m_nTroupeShareItem) {
			g_DPlay.SendPacket<PACKETTYPE_PARTYCHANGEITEMMODE, CParty::ShareItemMode>(nTroupeShareItem);
		}
	}
	else
	{
		if( nID != WTBID_CLOSE && nID != WTBID_HELP )
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0008) ) );
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

void CWndParty::OnLeave() {
	if (g_pPlayer->m_nDuel == 2) {
		// �شܵ�����̸� Ż�� �ȵ�.
		g_WndMng.PutString(TID_GAME_PPVP_LEAVE);
		return;
	}

	if (g_WndMng.m_pWndPartyLeaveConfirm) return;
	if (g_Party.m_nSizeofMember == 0) return;

	std::optional<u_long> leaver = std::nullopt;

	if (m_wndPartyInfo.m_nSelected != -1 && g_Party.IsLeader(g_pPlayer->m_idPlayer)) {
		// ����ٰ� Ż��
		leaver = g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_uPlayerId; //Ż���ų�� ID
	} else {
		leaver = g_pPlayer->m_idPlayer;
		// �� �ڽ��� �شܿ��� Ż��
	}

	if (leaver) {
		g_WndMng.m_pWndPartyLeaveConfirm = new CWndPartyLeaveConfirm;
		g_WndMng.m_pWndPartyLeaveConfirm->Initialize();
		g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId(leaver.value());
	}

	m_wndPartyInfo.m_nSelected = -1;
}

void CWndParty::OnChangeLeader() {
	if (g_Party.m_nSizeofMember == 0) {
		return g_WndMng.PutString(TID_GAME_PARTY_ISNOTMASTER);
	}

	if (!g_Party.IsLeader(g_pPlayer->m_idPlayer) || m_wndPartyInfo.m_nSelected == -1) {
		return g_WndMng.PutString(TID_GAME_MASTER_AWAY);
	}

	if (g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_remove) {
		return;
	}

	const u_long nLeadMember = g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_uPlayerId;

	if (g_Party.IsLeader(nLeadMember)) {
		return g_WndMng.PutString(TID_GAME_PARTY_ALREADYMASTER);
	}

	if (g_pPlayer->m_nDuel == 2) {	// �ش� ������϶��� �ش����� �ٲܼ� ����
		return g_WndMng.PutString(TID_PK_NOCHANGE_PARTYLEADER);
	}
	
	g_DPlay.SendPartyChangeLeader(g_pPlayer->m_idPlayer, nLeadMember);
}

/****************************************************
  WndId : APP_PARTY_INFO - ���κй�
  CtrlId : WIDC_GROUPBOX1 - �ش�����
  CtrlId : WIDC_GROUPBOX3 - ����ġ�й�
  CtrlId : WIDC_GROUPBOX4 - �����ۺй�
  CtrlId : WIDC_ITEM_SHARE - ���κй�
  CtrlId : WIDC_RADIO2 - �����й�
  CtrlId : WIDC_RADIO3 - �����й�
  CtrlId : WIDC_RADIO4 - �����й�
  CtrlId : WIDC_EXP_SHARE - ���Ϻй�
  CtrlId : WIDC_RADIO6 - �⿩���й�
  CtrlId : WIDC_RADIO7 - ���غй�
  CtrlId : WIDC_STATIC1 - �شܸ�Ī :
  CtrlId : WIDC_STATIC2 - �شܷ��� :
  CtrlId : WIDC_STATIC3 - �شܰ���ġ :
  CtrlId : WIDC_STATIC4 - �ش�����Ʈ :
  CtrlId : WIDC_NAME - ����Ŀ���̵�
  CtrlId : WIDC_LEVEL - 1
  CtrlId : WIDC_EXP - 90
  CtrlId : WIDC_POINT - 0
****************************************************/

CWndPartyInfo::~CWndPartyInfo() {
	DeleteDeviceObjects();
}

void CWndPartyInfo::OnDraw( C2DRender* p2DRender ) 
{
	CString strLevel, strName;
	
	// ���� ���õ� �� ǥ��
	if (m_nSelected != -1) {
		CRect rect(5, 8 + m_nSelected * 15, 410, 22 + m_nSelected * 15);
		p2DRender->RenderFillRect(rect, 0x60ffff00);
	}

	// ��Ƽ�� ���� ���
	if (g_Party.GetSizeofMember() < m_nSelected + 1) {
		m_nSelected = -1;
	}

	const int nLang = ::GetLanguage();
	const bool displayPartyRole = nLang == LANG_FRE || nLang == LANG_GER;

	int y = 10;

	for( int i = 0; i < g_Party.m_nSizeofMember; i++ ) 
	{		
		DWORD dwColor = 0xff000000;
		CMover* pObjMember	= prj.GetUserByID( g_Party.m_aMember[ i ].m_uPlayerId );

		CRect rectTemp,rect;
		const char * strClass = i == 0 ? prj.GetText(TID_GAME_LEADER) : prj.GetText(TID_GAME_MEMBER);

		rectTemp = CRect( 10, y, 20, y + 10 );
		p2DRender->RenderFillRect( rectTemp, dwColor );
		rectTemp = CRect( 11, y + 1, 19, y + 9 );

		// ���¿� ���� �� ����
		DWORD colorStatus = 0xff0000ff; // ����Ʈ�� �̻����
		if( IsValidObj(pObjMember) )
		{
			if( pObjMember->GetHitPoint() == 0 ) 
				colorStatus = 0xffff0000; // ������
			else if( pObjMember->GetMaxHitPoint() > 0 && ( (FLOAT)pObjMember->GetHitPoint() ) / ( (FLOAT)pObjMember->GetMaxHitPoint() ) <.1f ) 
				colorStatus = 0xffffff00; // HP 10% �����γ�
		}
		else
		{
			colorStatus = 0xff878787; // ����Ʈ�� ������ ���³�
			if( g_Party.m_aMember[ i ].m_remove ) 
				colorStatus = 0xff000000; // ������ ���³�
		}
		p2DRender->RenderFillRect( rectTemp, colorStatus );

		u_long  nLeadMember = g_Party.m_aMember[i].m_uPlayerId;
		if(g_Party.IsLeader(nLeadMember)) //Leader Color Set
			dwColor = 0xff1fb72d;

		PlayerInfo info = GetPlayerInfo(nLeadMember, pObjMember);

		strLevel.Format("%d", info.level);
		if (info.levelMode != NULL) {
			strLevel += prj.GetText(info.levelMode);
		}
		p2DRender->TextOut(30, y, 1.0f, 1.0f, strLevel.GetString(), dwColor);

		if (!displayPartyRole) {
			p2DRender->TextOut(80, y, 1.0f, 1.0f, strClass, dwColor);
			p2DRender->TextOut(120, y, 1.0f, 1.0f, info.jobName, dwColor);
		} else {
			p2DRender->TextOut(80, y, 1.0f, 1.0f, info.jobName, dwColor);
		}

		// �̸��� 10����Ʈ°���� ¥����
		strName = info.name;
		strings::ReduceSize(strName, 8);
		p2DRender->TextOut( 180, y, 1.0f, 1.0f, strName.GetString(), dwColor);
		
		// HP ������
		int nWidth	= ( IsValidObj( pObjMember ) ? 110 * pObjMember->GetHitPoint() / pObjMember->GetMaxHitPoint() : 0 );
		if( !displayPartyRole)
			rect.SetRect( 280, y, 280 + 110, y + 12 );
		else
			rect.SetRect( 310, y, 310 + 110, y + 12 );	// ���������� 30 �̵�

		rectTemp = rect; 
		rectTemp.right = rectTemp.left + nWidth;

		m_Theme.RenderGauge( p2DRender, &rect, 0xffffffff, m_pVBGauge, &m_texGauEmptyNormal );
		if( IsValidObj(pObjMember) )
			m_Theme.RenderGauge( p2DRender, &rectTemp, 0x64ff0000, m_pVBGauge, &m_texGauFillNormal );
	
		y+=15; // ������
	}
} 

CWndPartyInfo::PlayerInfo CWndPartyInfo::GetPlayerInfo(u_long playerId, CMover * pObjMember) {
	CWndPartyInfo::PlayerInfo retval;
	if (IsValidObj(pObjMember)) {
		retval.name = pObjMember->GetName();
		retval.level = pObjMember->GetLevel();
		retval.job = pObjMember->GetJob();
		retval.jobName = pObjMember->GetJobString();
	} else {
		const PlayerData * const pPlayerData = CPlayerDataCenter::GetInstance()->GetPlayerData(playerId);
		retval.name = pPlayerData->szPlayer;
		retval.level = static_cast<int>(pPlayerData->data.nLevel);
		retval.job = static_cast<int>(pPlayerData->data.nJob);
		retval.jobName = prj.jobs.info[retval.job].szName;
	}

	if (retval.job >= MAX_MASTER) {
		retval.levelMode = TID_GAME_TOOLTIP_MARK_HERO;
	} else if (retval.job >= MAX_PROFESSIONAL) {
		retval.levelMode = TID_GAME_TOOLTIP_MARK_MASTER;
	} else {
		retval.levelMode = NULL;
	}

	return retval;
}

HRESULT CWndPartyInfo::RestoreDeviceObjects() {
	CWndNeuz::RestoreDeviceObjects();
	if (m_pVBGauge == NULL)
		return m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBGauge, NULL);
	return S_OK;
}

HRESULT CWndPartyInfo::InvalidateDeviceObjects() {
	CWndNeuz::InvalidateDeviceObjects();
	SAFE_RELEASE(m_pVBGauge);
	return S_OK;
}

HRESULT CWndPartyInfo::DeleteDeviceObjects() {
	CWndNeuz::DeleteDeviceObjects();
	InvalidateDeviceObjects();
	return S_OK;
}

void CWndPartyInfo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	RestoreDeviceObjects();
	m_texGauEmptyNormal.LoadTexture( MakePath( DIR_THEME, "GauEmptyNormal.bmp" ), 0xffff00ff, TRUE );
	m_texGauFillNormal.LoadTexture( MakePath( DIR_THEME, "GauEmptyNormal.bmp" ), 0xffff00ff, TRUE );
	
	MoveParentCenter();
} 

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartyInfo::Initialize(CWndBase * pWndParent) {
	return CWndNeuz::InitDialog(APP_PARTY_INFO, pWndParent, 0, CPoint(0, 0));
}

void CWndPartyInfo::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
	if(g_WndMng.m_pWndWorld->m_bAutoAttack || pTaskBar->m_nExecute != 0)
		return;
	
	// ��Ƽâ���� ��� �������� ����
	CRect rect( 10, 10, 365, 10 + g_Party.GetSizeofMember() * 15 );
	if( rect.PtInRect( point ) ) 
		m_nSelected = ( point.y - 10 ) / 15;
	else 
		m_nSelected = -1;
	
	if (m_nSelected == -1) {
		// ������ Ÿ�� �����
		g_WorldMng()->SetObjFocus(nullptr);
		return;
	}

	// ���õȳ� ������
	// �׳��� Ÿ������
	g_WndMng.m_pWndWorld->m_pSelectRenderObj = nullptr; //���콺�� �ɷ� �׷������� ����� Ÿ���� ��ƾ� �Ȱ�ģ��..

	CMover* pObjMember = prj.GetUserByID( g_Party.m_aMember[m_nSelected].m_uPlayerId );
	if (g_pPlayer == pObjMember) {
		g_WorldMng()->SetObjFocus(nullptr);
		return;
	}

	if (IsValidObj(pObjMember)) {
		// ȭ�鿡 ���³��� �н�...
		g_WorldMng()->SetObjFocus(pObjMember);
		g_WndMng.m_pWndWorld->m_pRenderTargetObj = nullptr;
	}
}

/****************************************************
  WndId : APP_PARTY_SKILL - �شܽ�ų
****************************************************/

CWndPartySkill::CWndPartySkill() {
	m_atexSkill.fill(nullptr);
}

void CWndPartySkill::OnDraw( C2DRender* p2DRender )  { 
	const CRect rect = GetClientRect();
	const int nWidth = rect.Width() / 3;
	const int nHeight = rect.Height() / 3;
	
	for (int nCount = 0; nCount < 9; ++nCount) {
		const int i = nCount / 3;
		const int j = nCount % 3;

		const ItemProp * const pItemProp = prj.GetPartySkill(nCount + 1);
		if (!pItemProp) continue;

		m_atexSkill[nCount] = m_textureMng.AddTexture(MakePath(DIR_ICON, pItemProp->szIcon), 0xffff00ff, FALSE);

		const CPoint ptName = CPoint( j * nWidth + 35 , i * nHeight + 6 );
		const CPoint ptIcon = CPoint( j * nWidth + 3  , i * nHeight + 3 );
				
		DWORD textColor;
		DWORD textureOpacity;

		if (g_Party.m_nKindTroup && std::cmp_greater_equal(g_Party.GetLevel(), pItemProp->dwReqDisLV)) {
			if (std::cmp_greater_equal(g_Party.GetPoint(), pItemProp->dwExp)) {
				textColor = 0xff000000;
			} else {
				textColor = 0xff0000ff;
			}

			textureOpacity = 255;
		} else {
			textColor = 0xffff0000;
			textureOpacity = 54;
		}

		p2DRender->TextOut(ptName.x, ptName.y, pItemProp->szName, textColor);
		p2DRender->RenderTexture(ptIcon, m_atexSkill[nCount], textureOpacity);
	}
} 

int CWndPartySkill::GetCurrentlyHoveredSkill(CPoint point) /* const */ {
	const CRect rect = GetClientRect();
	const int nWidth = rect.Width() / 3;
	const int nHeight = rect.Height() / 3;
	point.x /= nWidth;
	point.y /= nHeight;
	return point.y * 3 + point.x;
}


void CWndPartySkill::OnLButtonDown(UINT, const CPoint point) {
	const int nSkill = GetCurrentlyHoveredSkill(point);
	
	const ItemProp * const pItemProp = prj.GetPartySkill( nSkill + 1 );
	if (!pItemProp) {
		m_nSkillSelect = -1;
		return;
	}
	
	m_nSkillSelect = nSkill;
}

void CWndPartySkill::OnMouseWndSurface(CPoint point) {
	const CRect rect = GetClientRect();
	const int nWidth = rect.Width() / 3;
	const int nHeight = rect.Height() / 3;
	int nCount = 0;
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++, nCount++ )
		{
			if (prj.GetPartySkill(nCount + 1))
			{
				CRect rect( j * nWidth + 3, i * nHeight + 3, j * nWidth + 3 + nWidth, i * nHeight + 3 + nHeight);
				if( rect.PtInRect( point ) )
				{
					CPoint point2 = point;
					ClientToScreen( &point2 );
					ClientToScreen( &rect );
					
					g_WndMng.PutToolTip_Troupe( i * 3 + j + 1, point2, &rect );
				}
			}
		}
	}
}

void CWndPartySkill::OnLButtonUp(UINT, CPoint) {
	m_nSkillSelect = -1;
}

void CWndPartySkill::OnMouseMove(UINT, CPoint) {
	if (m_nSkillSelect == -1 || IsPush() == FALSE)
		return;
	
	const ItemProp * const pItemProp =  prj.GetPartySkill( m_nSkillSelect + 1 );
	if (!pItemProp) return;

	m_GlobalShortcut.m_dwShortcut  = ShortcutType::PartySkill;
	m_GlobalShortcut.m_dwIndex = m_nSkillSelect + 1;
	m_GlobalShortcut.m_dwData  = 0;
	m_GlobalShortcut.m_dwId     = m_nSkillSelect + 1; // �÷�Ʈ ����Ʈ�� �� ID�� ����������.
	m_GlobalShortcut.m_pTexture = m_atexSkill[ m_nSkillSelect ];
	_tcscpy( m_GlobalShortcut.m_szString, pItemProp->szName);
}

void CWndPartySkill::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartySkill::Initialize(CWndBase * pWndParent) {
	return CWndNeuz::InitDialog(APP_PARTY_SKILL, pWndParent, 0, CPoint(0, 0));
}

void CWndPartySkill::OnLButtonDblClk(UINT, const CPoint point) {
	const int nSkill = GetCurrentlyHoveredSkill(point);

	if (prj.GetPartySkill(nSkill + 1)) {
		g_WndMng.ObjectExecutor(ShortcutType::PartySkill, nSkill + 1);
	}
}

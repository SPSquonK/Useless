#include "stdafx.h"
#include "resData.h"
#include "WndAwakening.h"
#include "DPClient.h"
#include "WndManager.h"
#include "defineText.h"

#include "randomoption.h"

/****************************************************
  WndId : APP_AWAKENING - ������ ���� â
  CtrlId : WIDC_TEXT1 - 
  CtrlId : WIDC_STATIC1 - 
  CtrlId : WIDC_BUTTON1 - Button
****************************************************/

// gmpbigsun ( 10_04_05 ) : CWndAwakening class�� ���� ������ ����

void CWndAwakening::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	// ���⿡ �ڵ��ϼ���
	CWndButton* pButton = GetDlgItem<CWndButton>(WIDC_BUTTON1);

	if(::GetLanguage() == LANG_FRE)
		pButton->SetTexture( MakePath( DIR_THEME, _T( "ButOk2.bmp" ) ), TRUE);

	pButton->EnableWindow(FALSE);

	CWndText * pText = GetDlgItem<CWndText>(WIDC_TEXT1);
	CWndText::SetupDescription(pText, _T("ItemAwakening.inc"));

	LPWNDCTRL wndCtrl = GetWndCtrl(WIDC_STATIC1);
	m_receiver.Create(0, wndCtrl->rect, this, 900);

	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndAwakening::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_AWAKENING, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndAwakening::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) {
	if( nID == WIDC_BUTTON1 )
	{
		//������ ������ �˸���.
		if (CItemElem * item = m_receiver.GetItem()) {
			CWndButton * pButton = GetDlgItem<CWndButton>(WIDC_BUTTON1);
			pButton->EnableWindow(FALSE);

			// ������ ó�� ��û�ϴ� �Լ� ȣ���ؾ���
			g_DPlay.SendAwakening(item->m_dwObjId);
			Destroy();
		}
	} else if (nID == WIDC_Receiver) {
		GetDlgItem<CWndButton>(WIDC_BUTTON1)->EnableWindow(
			m_receiver.GetItem() ? TRUE : FALSE
		);
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

bool CWndAwakening::CAwakenableItemReceiver::CanReceiveItem(const CItemElem & itemElem, bool verbose) {
	const int nRandomOptionKind = g_xRandomOptionProperty.GetRandomOptionKind(&itemElem);
	if (nRandomOptionKind != CRandomOptionProperty::eAwakening) {
		if (verbose) g_WndMng.OpenMessageBox(prj.GetText(TID_GAME_INVALID_TARGET_ITEM));
		return false;
	}

	if (g_xRandomOptionProperty.GetRandomOptionSize(itemElem.GetRandomOptItemId()) > 0) {
		if (verbose) g_WndMng.OpenMessageBox(prj.GetText(TID_GAME_AWAKE_OR_BLESSEDNESS01));
		return false;
	}

	return true;
}


CWndSelectAwakeCase::CWndSelectAwakeCase(BYTE byObjID, DWORD dwSerialNum, __int64 n64NewOption) {
	m_dwOldTime = g_tmCurrent;

	//server�� ������ �����͸� �����ϰ�, ������ Index�� �̾Ƽ� �������� �׷��� �غ�
	m_byObjID = byObjID;
	m_dwSerialNum = dwSerialNum;
	m_n64NewOption = n64NewOption;

	if (!g_pPlayer) return;
	
	const ItemProp * const pProp = g_pPlayer->GetItemIdProp(m_byObjID);
	if (pProp) {
		m_dwItemIndex = pProp->dwID;
	}
}

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndSelectAwakeCase::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_AWAKE_SELECTCASE, pWndParent, 0, CPoint( 0, 0 ) );
} 

void CWndSelectAwakeCase::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	MoveParentCenter();

	const ItemProp * pProp = prj.GetItemProp( m_dwItemIndex );
	if (pProp) {
		m_pTexture = CWndBase::m_textureMng.AddTexture(MakePath(DIR_ITEM, pProp->szIcon), 0xffff00ff);
	}
	
	m_pTexGuage = CWndBase::m_textureMng.AddTexture( MakePath( DIR_THEME, "Wndguage.tga"   ), 0xffff00ff );
	if (!m_pTexGuage) {
		Error("CWndSelectAwakeCase::OnInitialUpdate m_pTexGuage(Wndguage.tga) is NULL");
	}

	AddWndStyle(WBS_MODAL);

	RestoreDeviceObjects();
} 


BOOL CWndSelectAwakeCase::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		g_DPlay.SendSelectedAwakeningValue( m_byObjID, m_dwSerialNum, _AWAKE_OLD_VALUE );
		g_WndMng.PutString( GETTEXT( TID_GAME_REGARDLESS_USE03 ) );	//���õ�
		Destroy();
	}
	else if( nID == WIDC_BUTTON2 )
	{
		g_DPlay.SendSelectedAwakeningValue( m_byObjID, m_dwSerialNum, _AWAKE_NEW_VALUE );
		g_WndMng.PutString( GETTEXT( TID_GAME_REGARDLESS_USE03 ) );	//���õ�
		Destroy();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndSelectAwakeCase::OnDraw( C2DRender* p2DRender ) 
{ 
	if( !g_pPlayer )
		return;

	CItemElem* pItemElem = g_pPlayer->GetItemId( m_byObjID );
	if( !pItemElem )
	{
		assert( 0 );
		return;
	}

	m_dwDeltaTime += (g_tmCurrent - m_dwOldTime);
	m_dwOldTime = g_tmCurrent;

	if( m_dwDeltaTime > AWAKE_KEEP_TIME )
	{
		//���ѽð��� ������ ����ɼ����� ��û 
		g_DPlay.SendSelectedAwakeningValue( m_byObjID, m_dwSerialNum, _AWAKE_OLD_VALUE );

		g_WndMng.PutString( GETTEXT( TID_GAME_REGARDLESS_USE02 ) );	//���ýð� �ʰ�
		
		m_dwDeltaTime = 0;
		Destroy();
	}

	//old
	OutputOptionString( p2DRender, pItemElem );

	//new
	OutputOptionString( p2DRender, pItemElem, TRUE );


	CRect rect = GetWindowRect();
	rect.left += 14;
	rect.top += 174;

	// �ð� / 100 * 360(�ѱ���) / 600 (�ѽð�)
	rect.right = LONG(( rect.left + (AWAKE_KEEP_TIME - m_dwDeltaTime) / 100 ) * 0.6f) ;		//�������� �� ���� ���̸� �ִ��ʷ�...
	rect.bottom = rect.top + 20;

	m_pTheme->RenderGauge(p2DRender, &rect, 0xffffffff, m_pVertexBufferGauge, m_pTexGuage);

	// draw icon
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
	assert( wndCtrl );
			
	if (m_pTexture) {
		m_pTexture->Render(p2DRender, CPoint(wndCtrl->rect.left, wndCtrl->rect.top));
	}
} 

extern bool IsDst_Rate(int nDstParam);
extern const char *FindDstString( int nDstParam );
void CWndSelectAwakeCase::OutputOptionString( C2DRender* p2DRender, CItemElem* pItemElem, BOOL bNew )
{
	// �ɼ� ���ڿ� �׸���
	if( !pItemElem )
		return;

    int nSize = 0;
	if( !bNew )
		nSize = g_xRandomOptionProperty.GetRandomOptionSize( pItemElem->GetRandomOptItemId() );
	else nSize = g_xRandomOptionProperty.GetViewRandomOptionSize( m_n64NewOption );

	__int64 n64Options = 0;
	n64Options = ( bNew ? m_n64NewOption : pItemElem->GetRandomOptItemId() );

	CString str;

	// option
	for( int i = 0; i < nSize; i++ )
	{
		const auto opt = g_xRandomOptionProperty.GetParam(n64Options, i);
		if (!opt) continue;

		const int nDst = opt->nDst;
		const int nAdj = opt->nAdj;

		if( IsDst_Rate( nDst ) )
		{
			if( nDst == DST_ATTACKSPEED )
				str.Format( "\n%s %c%d%% ", FindDstString( nDst ), ( nAdj > 0? '+': '-' ), ::abs( nAdj / 2 / 10 ) );
			else
				str.Format( "\n%s %c%d%%", FindDstString( nDst ), ( nAdj > 0? '+': '-' ), ::abs( nAdj ) );
		}
		else
		{
			str.Format( "\n%s %c%d", FindDstString( nDst ), ( nAdj > 0? '+': '-' ), ::abs( nAdj ) );
		}

		//�� �������پ� ����
		if( bNew )
			p2DRender->TextOut( 70, 92 + i * 20, str, 0xff000000 );
		else p2DRender->TextOut( 70, 20 + i * 20, str, 0xff000000 );
	}
}

HRESULT CWndSelectAwakeCase::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	if( m_pVertexBufferGauge == NULL )
		m_pApp->m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, 
												 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
												 D3DFVF_TEXTUREVERTEX2, 
												 D3DPOOL_DEFAULT, 
												 &m_pVertexBufferGauge, 
												 NULL);
	assert(m_pVertexBufferGauge != NULL);
	
	return S_OK;
}
HRESULT CWndSelectAwakeCase::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pVertexBufferGauge );
	return S_OK;
}
HRESULT CWndSelectAwakeCase::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	SAFE_RELEASE( m_pVertexBufferGauge );
	return S_OK;
}

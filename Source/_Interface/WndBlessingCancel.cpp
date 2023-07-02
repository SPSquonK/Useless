#include "stdafx.h"
#include "resData.h"
#include "WndBlessingCancel.h"
#include "DPClient.h"
#include "defineText.h"
#include "randomoption.h"


/****************************************************
  WndId : APP_CALCEL_BLESSING - ������ ����
  CtrlId : WIDC_CHANGE - 
  CtrlId : WIDC_DESC - 
  CtrlId : WIDC_START - 
****************************************************/

void CWndBlessingCancel::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CWndButton* pButton = GetDlgItem<CWndButton>(WIDC_START);

	if(::GetLanguage() == LANG_FRE)
		pButton->SetTexture(MakePath( DIR_THEME, _T( "ButOk2.bmp" ) ), TRUE);

	pButton->EnableWindow(FALSE);

	CWndText * m_pText = GetDlgItem<CWndText>( WIDC_DESC );
	CWndText::SetupDescription(m_pText, _T("ItemBlessingCancel.inc"));

	LPWNDCTRL wndCtrl = GetWndCtrl(WIDC_CHANGE);
	m_receiver.Create(0, wndCtrl->rect, this, WIDC_Receiver);

	MoveParentCenter();
} 

// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndBlessingCancel::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	return CWndNeuz::InitDialog( APP_CANCEL_BLESSING, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndBlessingCancel::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) { 
	if (nID == WIDC_START) {
		//������ ������ �˸���.
		if (CItemElem * item = m_receiver.GetItem()) {
			CWndButton * pButton = GetDlgItem<CWndButton>( WIDC_START );
			pButton->EnableWindow(FALSE);

			// ������ ó�� ��û�ϴ� �Լ� ȣ���ؾ���
			g_DPlay.SendBlessednessCancel(item->m_dwObjId);
			Destroy();
			g_WndMng.PutString( prj.GetText(TID_GAME_BLESSEDNESS_CANCEL_INFO), NULL, 0xff0000ff );
		}
	} else if (nID == WIDC_Receiver) {
		GetDlgItem<CWndButton>(WIDC_START)->EnableWindow(
			m_receiver.GetItem() ? TRUE : FALSE
		);
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

bool CWndBlessingCancel::CFashionReceiver::CanReceiveItem(const CItemElem & item, bool verbose) {
	const bool r = g_xRandomOptionProperty.GetRandomOptionKind(&item) == CRandomOptionProperty::eBlessing
		&& g_xRandomOptionProperty.GetRandomOptionSize(item.GetRandomOptItemId());

	if (!r && verbose) {
		g_WndMng.PutString(prj.GetText(TID_GAME_BLESSEDNESS_CANCEL), NULL, 0xffff0000);
	}

	return r;
}

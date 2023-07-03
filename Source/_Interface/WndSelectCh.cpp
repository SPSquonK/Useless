#include "stdafx.h"
#include "resData.h"

#include "defineText.h"
#include "AppDefine.h"

#include "WndSelectCh.h"
#include "DPClient.h"

/****************************************************
  WndId : APP_SELECT_CHANNEL - ä�� ����
  CtrlId : WIDC_LISTBOX1 - Listbox
  CtrlId : WIDC_STATIC1 - GroupBox
****************************************************/

CWndSelectCh::CWndSelectCh(int nItemId, unsigned int nChCount) { 
	m_nItemId  = nItemId;
	m_nChCount = nChCount + 1;	// 0�϶� 1���̹Ƿ� ����������� �����ش�
} 

void CWndSelectCh::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	// ���⿡ �ڵ��ϼ���
	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	pWndListBox->ResetContent();
	CString strTitle;
	for (unsigned int i = 0; i < m_nChCount; ++i) {
		strTitle.Format("%s	%d", prj.GetText(TID_GAME_CHAR_SERVERNAME), i + 1);
		pWndListBox->AddString(strTitle);
	}

	// ������ �߾����� �ű�� �κ�.
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndSelectCh::Initialize( CWndBase* pWndParent ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( APP_SELECT_CHANNEL, pWndParent, 0, CPoint( 0, 0 ) );
} 

BOOL CWndSelectCh::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	int nSelect = 0;

	switch(nID)
	{
		case WIDC_LISTBOX1: // view ctrl
			{
				CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
				char strTemp[8];
				nSelect = pWndListBox->GetCurSel() * -1;
				_itoa(nSelect, strTemp, 10);
				g_DPlay.SendDoUseItemInput(m_nItemId, strTemp);
				Destroy();
			}
			break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

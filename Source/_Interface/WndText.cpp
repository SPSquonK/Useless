#include "stdafx.h"
#include "WndText.h"
#include "DPClient.h"


///////////////////////////////////////////////////////////////////////////////

BOOL CWndTextQuest::Initialize(CWndBase * pWndParent, DWORD dwWndId) {
	return CWndMessageBox::Initialize("����Ʈ�� �����Ͻðڽ��ϱ�?", pWndParent, MB_OKCANCEL);
}

BOOL CWndTextQuest::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case IDOK:
			g_DPlay.SendDoUseItem(MAKELONG(0, m_pItemBase->m_dwObjId), NULL_ID);
			Destroy();
			break;
		case IDCANCEL:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}


///////////////////////////////////////////////////////////////////////////////


void CWndTextFromItem::OnInitialUpdate() {
	CWndNeuz::OnInitialUpdate();
	// ���⿡ �ڵ��ϼ���

	CWndButton * pWndAccept = (CWndButton *)GetDlgItem(WIDC_ACCEPT);
	CWndText * pWndText = (CWndText *)GetDlgItem(WIDC_TEXT1);
	ItemProp * pItemProp = m_pItemBase->GetProp();

	if (pItemProp->dwQuestId == 0 || m_pItemBase->m_bItemResist == TRUE)
		pWndAccept->EnableWindow(FALSE);

	CFileIO file;
	if (file.Open(MakePath(DIR_TEXT, pItemProp->szTextFileName), "rb")) {
		CHAR * pText = new CHAR[file.GetLength() + 1];
		file.Read(pText, file.GetLength());
		pText[file.GetLength()] = 0;
		pWndText->SetString(pText);
		safe_delete(pText);
	}

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point(rectRoot.right - rectWindow.Width(), 110);
	Move(point);
	MoveParentCenter();
}

BOOL CWndTextFromItem::OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) {
	switch (nID) {
		case WIDC_ACCEPT:
		{
			CWndTextQuest * pWndTextQuest = new CWndTextQuest;
			pWndTextQuest->m_pItemBase = m_pItemBase;
			g_WndMng.OpenCustomBox("", pWndTextQuest);
			break;
		}
		case WIDC_CLOSE:
			Destroy();
			break;
	}

	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

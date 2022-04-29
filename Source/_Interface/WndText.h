#pragma once

#include "resdata.h"

class CWndTextQuest : public CWndMessageBox
{ 
public: 
	CItemBase* m_pItemBase;

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwWndId = 0 );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
}; 

class CWndTextFromItem final : public CWndNeuz { 
public: 
	CItemElem * m_pItemBase = nullptr;

	void SetItemBase(CItemElem * pItemBase) { m_pItemBase = pItemBase; }

	BOOL Initialize(CWndBase * pWndParent, CItemElem * pItemBase, DWORD baseApp) {
		m_pItemBase = pItemBase;
		return CWndNeuz::InitDialog(g_Neuz.GetSafeHwnd(), baseApp, 0, CPoint(0, 0), pWndParent);
	}

	void OnInitialUpdate() override;
	BOOL OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) override;
}; 

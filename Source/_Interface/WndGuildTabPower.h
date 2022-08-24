#pragma once 

#include <concepts>

class CWndGuildTabPower : public CWndNeuz {
	// ��������쿡�� �߰��Ǵ� �� ������ ( ��� �Ͽ콺�� ���� ���� ���� )
public:
	BOOL Initialize(CWndBase * pWndParent = NULL, DWORD nType = MB_OK) override;
	void OnInitialUpdate() override;
	BOOL OnChildNotify(UINT message, UINT nID, LRESULT * pLResult) override;

	void UpdateData();
	void SetData(DWORD dwPower[]);
	void EnableButton(BOOL bEnable);

private:
	DWORD m_adwPower[MAX_GM_LEVEL];
	BOOL m_bChanedCheckBox = FALSE;
	bool m_hasBeenChanged = false;

	void ForEachPower(std::invocable<UINT, int, DWORD> auto func);
	void ForEachPower(std::invocable<CWndButton &, int, DWORD> auto func);
};

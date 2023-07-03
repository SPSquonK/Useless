#ifndef __WNDGUILD_TAB_MEMBER__H
#define __WNDGUILD_TAB_MEMBER__H

#include <vector>

struct MEMBERLIST
{
	DWORD	idPlayer;	// �÷��̾� ���̵�.
	int nMemberLv;		// ȣĪ
	int nJob;			// ���� 
	int nSex;			// ��
	int	nLevel;			// ĳ������
	BOOL bIsOnLine;     // �¶���, ��������
	int		nGiveGold;					// ��忡 ����� ���
	DWORD	dwGivePxpCount;				// ��忡 ����� PXPȽ��( ��ų ����ġ )
	short	nWin;							// ������ �̰���?
	short	nLose;						// ������ �����?
	char szName[ MAX_NAME ];	// �̸�.
	char szAlias[ MAX_GM_ALIAS ];	// ��Ī
	int nClass;				// ���
};

class CWndGuildTabMember final : public CWndNeuz
{ 
	int		m_nCurrentList;			// ��µ� �������Ʈ�� ���� �ε���.
	int		m_nSelect;

	int		m_nMxOld, m_nMyOld;			// ���� ��ǥ.
	CWndMenu	m_wndMenu;			// �˾� �޴�.
	CWndScrollBar m_wndScrollBar;

	BOOL m_bSortbyMemberLevel;
	BOOL m_bSortbyLevel;
	BOOL m_bSortbyJob;
	BOOL m_bSortbyName;
	BOOL m_bSortbyAlias;

public: 
	void UpdateData();
	std::vector<MEMBERLIST> m_list;

	CWndGuildTabMember(); 

	CRect	m_rectParent;			// ���â�� �»�� ��ǥ.
	
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual	void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void OnRButtonDown( UINT nFlags, CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point );
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void OnMouseWndSurface( CPoint point );

	void SortbyMemberLevel();
	void SortbyJob();
	void SortbyLevel();
	void SortbyName();
	void SortbyAlias();

	[[nodiscard]] static int GetMemberLevelIcon(int nbClass);
}; 

#endif
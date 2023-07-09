#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

class CWndCommand;
class CWndChatFilter;
//////////////////////////////////////////////////////////////////////////////////////
// ä�� ���� 
//
class CWndEditChat : public CWndEdit
{
public:
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnSetFocus(CWndBase* pOldWnd);
	virtual void OnKillFocus(CWndBase* pNewWnd);
};
class CWndMacroChat : public CWndButton
{
public:
	CTexture* m_pTexMacro;
	CTexture m_texMacroChat;

	virtual void OnDraw( C2DRender* p2DRender );
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual	void OnInitialUpdate();
};

class CWndTextChat : public CWndText {
public:
	BOOL IsPickupSpace(CPoint point) override;
};

class CWndChatLog final : public CWndNeuz
{
	CWndMenu	 m_wndMenuPlace;
	CWndTextChat m_wndText;

public:

	void  PutString( LPCTSTR lpszString, DWORD dwColor = 0xffffffff, DWORD dwPStyle = 0x00000001 ); //CObj* pObj );
	virtual	void OnInitialUpdate();
	BOOL Initialize( CWndBase* pWndParent = nullptr );
	// message
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnDestroy();
	virtual void OnKillFocus(CWndBase* pNewWnd);

	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	virtual BOOL Process ();	
};

class CWndChat final : public CWndNeuz
{
	IMAGE m_wndChatEdit[3];
	//CSize m_sizeWndChatEdit[3];

	CWndMacroChat m_wndMacroChat;
	CTimer m_timerDobe;
	CTexture m_texEdit;
	CTimer m_timerInsMsg;
#ifdef __CSC_GAME_GRADE
	CTimer m_timerAlertGGrade;
#endif //__CSC_GAME_GRADE
	size_t m_nInsMsgCnt;
	int				m_nHistoryIndex;
	std::vector<CString> m_strHistory;
	CTimer m_timerInputTimeOut;

public:
	BOOL m_bChatLock;
	BOOL m_bChatLog;
	BOOL m_bMoveLock;
	static int m_nChatChannel;
	CWndChatFilter* m_pWndChatFilter;
	CString m_strCharName;
	CWndTextChat m_wndText;
	std::vector<CString> m_strArray;
	CWndCommand* m_pWndCommand;

	CWndEditChat m_wndEdit;

	CWndChat();   
	virtual ~CWndChat(); 
	void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
	void  PutString( LPCTSTR lpszString, DWORD dwColor = 0xffffffff, DWORD dwPStyle = 0x00000001 ); //CObj* pObj );
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	BOOL Initialize( CWndBase* pWndParent = nullptr );
	// message
	void SetChannel();
	virtual	void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual void OnDestroy();
	virtual void OnDestroyChildWnd( CWndBase* pWndChild );
	virtual void OnSetFocus(CWndBase* pOldWnd);
	virtual void OnKillFocus(CWndBase* pNewWnd);
	virtual void AdditionalSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat );
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL Process ();	

	[[nodiscard]] static int GetChannelForChatsty(int chatsty);
};

#endif

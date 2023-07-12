#ifndef __WNDNOTICE__H
#define __WNDNOTICE__H

class CWndInfoNotice final : public CWndNeuz
{ 
public: 
	std::vector<CString> m_vecStrBanner;
	DWORD		 m_dwChangeBannerTime;
	LPIMAGE m_atexPannel;

	CWndInfoNotice(); 
	~CWndInfoNotice(); 

	void AfterSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
	virtual BOOL Process ();
	void OnSetCursor() override;
	BOOL Initialize( CWndBase* pWndParent = nullptr ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 
#endif

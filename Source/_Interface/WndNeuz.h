#pragma once


//////////////////////////////////////////////////////////////////////////////////////
// ������ Ÿ��Ʋ �� 
// Ÿ��Ʋ ���, ���� ���� ��ư 
//
#define WTBID_CLOSE     10000
#define WTBID_MAX       10001
#define WTBID_MIN       10002
#define WTBID_HELP      10003
#define WTBID_REPORT    10004
#define WTBID_EXTENSION 10005
#define WTBID_PIN       10006

#define WBS_MINIMIZEBOX 0x00000001L // �ּ�ȭ ��ư 
#define WBS_MAXIMIZEBOX 0x00000002L // �ִ�ȭ ��ư 
#define WBS_HELP        0x00000004L // ���� ��ư 
#define WBS_VIEW        0x00000008L // �� ��ȯ ��ư 
#define WBS_PIN         0x00000010L // �� ��ư 
#define WBS_EXTENSION   0x00000020L // Ȯ�� ��ư 
#define WBS_THICKFRAME  0x00000040L // �������� ���� ����

class CWndTitleBar final : public CWndBase {
	std::array<CWndButton, 7> m_awndButton;
	int m_nButtonMax;

public:
	void OnInitialUpdate() override;
	BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult) override;
	BOOL OnEraseBkgnd(C2DRender* p2DRender) override;

	/// Moves this to the top right of the window
	void Replace();
};

//////////////////////////////////////////////////////////////////////////////////////
// �۾� ���� 
// �޴� ��ư, ��� ������, ���� ������, Ȱ��ȭ ��� ���
//
class CWndNeuz : public CWndBase  
{
	BOOL m_bFullMax;
	// TODO: It is unclear why this boolean exists. If a window does not want to be registered,
	// it could juste keep an empty SerializeRegInfo implementation
	BOOL m_bPutRegInfo;
	
protected:
	CWndTitleBar m_wndTitleBar;
	CRect m_rectBackup;
	CPtrArray m_wndArrayTemp; // List of created controls (to be able to release them only)

public:
	CPoint m_ptMouseCenter;

	// ���� ���� ���� ���� 
	void SetPutRegInfo( BOOL bPutRegInfo ) { m_bPutRegInfo = bPutRegInfo; }
	[[nodiscard]] bool IsPutRegInfo() const noexcept { return m_bPutRegInfo; }

	virtual void SerializeRegInfo( CAr& ar, DWORD& dwVersion );

	// ���� ���� ���� ���� �� 

	BOOL InitDialog( DWORD dwWID, CWndBase * pWndParent = nullptr, DWORD dwStyle = 0, CPoint ptLeftTop = 0 );
	CWndBase* CreateControl( HWND hWnd, LPWNDCTRL lpWndCtrl );
	//CWndBase* GetDlgItem( UINT nID )
protected:
	CWndNeuz();
public:
	~CWndNeuz() override;

	D3DFORMAT m_d3dFormat;
	void AdjustWndBase( D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
	void AdditionalSkinTexture( LPWORD pDest, CSize size, D3DFORMAT d3dFormat = D3DFMT_A4R4G4B4 );
	void PaintTexture( LPVOID pDestData, LPIMAGE pImage, CPoint pt, CSize sizeSurface ) { ::PaintTexture( pDestData, pImage, pt, sizeSurface, m_d3dFormat ); }
	void SetFullMax( BOOL bFullMax ) { m_bFullMax = bFullMax; }
	BOOL IsFullMax() { return m_bFullMax; }
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual	void PaintFrame( C2DRender* p2DRender );
	virtual void OnDraw( C2DRender* p2DRender );
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL,DWORD dwStyle = 0 );
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual BOOL OnChildNotify( UINT message,UINT nID,LRESULT* pLResult );
	virtual void OnSize( UINT nType, int cx, int cy );
	virtual void OnNonClientLButtonDblClk( UINT nFlags, CPoint point );
	virtual void SetWndRect( CRect rectWnd, BOOL bOnSize = TRUE);
	virtual BOOL OnSetCursor( CWndBase* pWndBase, UINT nHitTest, UINT message );

	void SetSizeMax();
	void SetSizeWnd();
};

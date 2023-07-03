// WndOption.h: interface for the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDOPTION_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDOPTION_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndOptionGame.h"	 // ���� �ɼ�

class CWndOption : public CWndNeuz 
{ 
public: 
	CWndOption(); 
	~CWndOption(); 

	BOOL Initialize( CWndBase* pWndParent = nullptr ) override; 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

//////////////////////////////////////////////////////////////////////////////////////
// Sound
//

class CWndOptSound : public CWndNeuz 
{ 
public: 
	CTexture         m_Texture;
	CTexture         m_TexturePt;
	BOOL			 m_bLButtonClick;
	BOOL			 m_bLButtonClick2;
	int				 m_nStep[2];
	CWndOptSound(); 
	~CWndOptSound(); 

	BOOL Initialize( CWndBase* pWndParent = nullptr ) override; 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	void GetRangeSlider( DWORD dwWndId, int& nStep, CPoint point );
	int GetSliderStep( DWORD dwWndId, int &nStep, CPoint point );
	CPoint GetStepPos( int nStep, int nWidth, int nDivision );
	
}; 
class CWndOptVideo : public CWndNeuz 
{ 
public: 
	static constexpr bool MyServerRecksTheUserScreenParameters = false;

	void GetRangeSlider( DWORD dwWndId, int& nStep, CPoint point );
	int GetSliderStep( DWORD dwWndId, int &nStep, CPoint point );
	void GetRangeSlider( DWORD dwWndId, int& nStep, CPoint point, int nDivision);
	int GetSliderStep( DWORD dwWndId, int &nStep, CPoint point, int nDivision);
	CPoint GetStepPos( int nStep, int nWidth, int nDivision );
	CWndOptVideo(); 
	~CWndOptVideo(); 

	int				 m_nStep[5];
	int				 m_nBrightTable[11];

	BOOL			 m_bLButtonClick3;
	BOOL			 m_bLButtonClick4;
	BOOL			 m_bLButtonClick5;

	CTexture         m_Texture;
	CTexture         m_TexturePt;
	BOOL			 m_bLButtonClick;
	BOOL			 m_bLButtonClick2;
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	BOOL Initialize( CWndBase* pWndParent = nullptr ) override; 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	
private:
	void RenderSlider(C2DRender * p2DRender, UINT widgetCtrlId, int value, int max);
}; 



class CWndOptMyInfo : public CWndNeuz 
{ 
public: 
	CWndOptMyInfo(); 
	~CWndOptMyInfo(); 

	BOOL Initialize( CWndBase* pWndParent = nullptr ) override; 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndOptWindow : public CWndNeuz 
{ 
public: 
	CWndOptWindow(); 
	~CWndOptWindow(); 

	BOOL Initialize( CWndBase* pWndParent = nullptr ) override; 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 


class CWndTotalOption : public CWndNeuz
{
public:
	CWndOptVideo		m_OptTabVideoSnd;
	CWndOptionGame		m_OptTabGame;


	BOOL Initialize( CWndBase* pWndParent = nullptr ) override; 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual	void OnInitialUpdate(); 
};


#endif // !defined(AFX_WNDOPTION_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)

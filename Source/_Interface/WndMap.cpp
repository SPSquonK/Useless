#include "stdafx.h"
#include "AppDefine.h"
#include "WndMap.h"
#include "WorldMap.h"

/****************************************************
  WndId : APP_MAP - Map
****************************************************/

CWndMap::CWndMap() 
{ 
} 
CWndMap::~CWndMap() 
{ 
} 
void CWndMap::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndMap::OnInitialUpdate() 
{ 



} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndMap::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	CWorldMap* pWorldMap = CWorldMap::GetInstance();


	// ���������̸� ���� �ƴϸ� �Ҵ�
	if(pWorldMap->IsRender())	pWorldMap->DeleteWorldMap();
	else						pWorldMap->LoadWorldMap();

	return FALSE;
} 
/*
  ���� ������ ���� ��� 
BOOL CWndMap::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = g_WndMng.GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndMap::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndMap::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndMap::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndMap::OnRButtonUp( UINT nFlags, CPoint point ) 
{ 
	Destroy();
} 
void CWndMap::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndMap::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


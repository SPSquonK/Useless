#ifndef __LAYERED_LINK_MAP_H
#define	__LAYERED_LINK_MAP_H

#include <map>
#include "linktype.h"

const	int	nDefaultLayer	= 0;	// ���� ���ɼ��� ���� �⺻ �����̴�.
const	int	nRevivalLayer	= 0;	// ��Ȱ�� �������� ���еǸ� �ش� ����� ��� ������ ����Ѵ�.
const	int	nTempLayer	= 0;	// ��ȹ�� ���� ����� ���ɼ��� �ִ� �ӽ� ���� ���̴�.

class CLinkMap;

class CLayeredLinkMap
{
public:
	CLayeredLinkMap();
	virtual	~CLayeredLinkMap();
public:
	void	Init( int nLandWidth, int nLandHeight, int nView, BOOL bBaseLayer, int nMPU );
	void	Release();
	int	GetLinkWidth( LinkType dwLinkType, int nLinkLevel, int nLayer );
	CObj**	GetObj( LinkType dwLinkType, DWORD dwLinkLevel, int nLayer );
	DWORD	CalcLinkLevel( CObj* pObj, float fObjWidth, int nLayer );
	int	GetMaxLinkLevel( LinkType dwLinkType, int nLayer );
	void	SetMaxLinkLevel( LinkType dwLinkType, int nLevel, int nLayer );
	BOOL	InsertObjLink( CObj* pObj );
	BOOL	RemoveObjLink2( CObj* pObj );
	BOOL	RemoveObjLink( CObj* pObj );
	CObj*	GetObjInLinkMap( const D3DXVECTOR3 & vPos, LinkType dwLinkType, int nLinkLevel, int nLayer );
	BOOL	SetObjInLinkMap( const D3DXVECTOR3 & vPos, LinkType dwLinkType, int nLinkLevel, CObj* pObj, int nLayer );
	void	AddItToView( CCtrl* pCtrl );
	void	ModifyView( CCtrl* pCtrl );

	BOOL	CreateLinkMap( int nLayer );
	CLinkMap*	GetLinkMap( int nLayer );
	void	DeleteLinkMap( int nLayer );
	void	Process( CWorld* pWorld );
	void	Invalidate( int nLayer, BOOL bInvalid );
private:
	std::map<int, CLinkMap *>		m_mapLinkMap;
	int	m_nLandWidth;
	int	m_nLandHeight;
	int	m_nView;

	int m_iMPU;		//gmpbigsun
};

#endif	// __LAYERED_LINK_MAP_H
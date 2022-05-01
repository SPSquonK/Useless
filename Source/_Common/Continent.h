#pragma once

#define min_cont(x,y) (x < y ? x : y)
#define max_cont(x,y) (x > y ? x : y)

#include "ContinentDef.h"


class CContinent  
{
public:
	CContinent();

	static CContinent*	GetInstance( void );
#ifndef __DBSERVER	
	BYTE GetContinent( CMover* pMover );	// ���� ĳ���Ͱ� ��ġ�ϰ� �ִ� ���
	BYTE GetArea( CMover* pMover );			// ���� ĳ���Ͱ� ��ġ�ϰ� �ִ� ����� ����(����, ����...)
	static BOOL	IsValidObj( CMover* pMover );
#endif // !__DBSERVER
	[[nodiscard]] static const char * GetContinentName(BYTE nCont);
#ifdef __WORLDSERVER
	D3DXVECTOR3 GetRevivalPos( BYTE nContinent );
#endif // __WORLDSERVER

#ifdef __CLIENT
	int	GetMapNo( CMover* pMover );
	int	GetMapNo( const D3DXVECTOR3& vPos );		//gmpbigsun : const 
#endif // __CLIENT

	int GetContinentIndex( const char* continentName );

public:
	[[nodiscard]] BYTE GetContinent(const D3DXVECTOR3 & vPos) const {
		return FindContInMap(vPos, m_MapCont);
	}

	[[nodiscard]] BYTE GetTown(const D3DXVECTOR3 & vPos) const {
		return FindContInMap(vPos, m_MapContTown);
	}

private:
	BYTE GetArea( BYTE nCont );

	static BYTE FindContInMap(const D3DXVECTOR3 & vPos, const std::map<BYTE, std::vector<CPoint>> & map);

private:
	void Init();
	static BOOL Point_In_Poly(const std::vector<CPoint> & vecMap, CPoint test_point);
	std::map< BYTE, std::vector<CPoint> > m_MapCont;
	std::map< BYTE, std::vector<CPoint> > m_MapContTown;
};

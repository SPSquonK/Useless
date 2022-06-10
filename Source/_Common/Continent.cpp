// Continent.cpp: implementation of the CContinent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Continent.h"
#include "defineText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContinent::CContinent()
{
	Init();
}

CContinent* CContinent::GetInstance( void )
{
	static CContinent sContinent;
	return &sContinent;	
}

void CContinent::Init()
{
	m_MapCont.clear();
	std::vector<CPoint> vecMap;

	// �����̳� ����� �濡 �����ϴ� ����� ���帮������(���ο���) �����ϸ� �����ε��� �׷������ �����Ѵ�.
	const char* szWorldName = "WdMadrigal";				
	CString strDir = MakePath( DIR_WORLD, szWorldName );
	CString strFullFileName = strDir + CString( "\\" ) + CString(szWorldName) + CString(".wld.cnt");

	CScript script;
	if( script.Load( strFullFileName ) == FALSE )
		return;

	int id = 0;
	BOOL bTown = FALSE;

	do{
		script.GetToken( );

		if( script.Token == _T( "Continent" ) )
		{
			script.GetToken();
			if( script.Token == _T( "BEGIN" ) )
			{
				vecMap.clear( );
			}
			else if( script.Token == _T( "END" ) )
			{
				if( !vecMap.empty() )
					vecMap.push_back( vecMap[ 0 ] );

				std::pair< std::map< BYTE, std::vector<CPoint> >::iterator, BOOL > rst;

				// gmpbigsun(100409): bTown�� TRUE��� �̱����� �����̶�� �ǹ̰�, ����� Client �ʰ��� ������ ���� ��������
				// Server������ ����� ��밡���ϰ�, ����� ���� ���⶧���� Ǯ���.
				rst = ( bTown ? m_MapContTown.insert( make_pair( id, vecMap ) ) : m_MapCont.insert( make_pair( id, vecMap ) ) ) ;
	
				assert( rst.second );

				bTown = FALSE;		//reset town
			}
		}
		else
		if( script.Token == _T( "C_id" ) )
		{
			id = script.GetNumber( );
		}
		else
		if( script.Token == _T( "VERTEX" ) )
		{
			float x = script.GetFloat();
			float y = script.GetFloat();
			float z = script.GetFloat();
			vecMap.push_back( CPoint( (int)( x ), (int)( z ) ) );
		}	
		else
		if( script.Token == _T( "TOWN" ) )
		{
			bTown = script.GetNumber( );
		}
		else 
		if( script.Token == _T( "C_useRealData" ) )
		{
			BOOL bReal = script.GetNumber( );
			if( !bReal )
			{
				//Ŭ������ �����ȭ �����ͷθ� ���δ�. Ŭ��� ���� �ε�� �ش� ����� �ڵ��ε��.
				do{
					script.GetToken( );
				}while( script.Token != "END" );		//�Ѵ�� pass 
			}
		} 

	}while( script.tok != FINISHED );


}

BOOL CContinent::Point_In_Poly(const std::vector<CPoint> & vecMap, CPoint test_point )
{
	int counter = 0;
		
	CPoint p1 = vecMap[0];
	for(int i=1;i<=(int)( vecMap.size() );i++ )
	{
		CPoint p2 = vecMap[i % vecMap.size()];
		
		if (test_point.y > min_cont(p1.y,p2.y))
		{
			if (test_point.y <= max_cont(p1.y,p2.y))
			{
				if (test_point.x <= max_cont(p1.x,p2.x))
				{
					if (p1.y != p2.y)
					{
						double xinters = (test_point.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						
						if (p1.x == p2.x || test_point.x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
	}
	
	if (counter % 2 == 0)
		return FALSE;
	else
		return TRUE;
}

#ifndef __DBSERVER
BYTE CContinent::GetContinent( const CMover* pMover )
{
	if( !CContinent::IsValidObj( pMover ) )
		return CONT_NODATA;

	switch( pMover->GetWorld()->m_dwWorldID )
	{
		case WI_WORLD_MADRIGAL:	return GetContinent( pMover->GetPos() );
		case WI_WORLD_ARENA:	return CONT_DARKON12;	// �Ʒ����� ���� 1, 2 ������ ����
		case WI_WORLD_GUILDWAR : return CONT_FLARIS;	// ���������� �ö󸮽�
	}

	return CONT_NODATA;
}

BYTE CContinent::GetArea( const CMover* pMover )
{
	if( !CContinent::IsValidObj( pMover ) )
		return CONT_NODATA;


	return GetArea( GetContinent(pMover) );	
}

BOOL CContinent::IsValidObj( const CMover* pMover )
{
	return ( ::IsValidObj( pMover ) && pMover->GetWorld() );
}

#endif // !__DBSERVER

BYTE CContinent::FindContInMap(const D3DXVECTOR3 & vPos, const std::map<BYTE, std::vector<CPoint>> & map) {
	CPoint cpoint;
	cpoint.x = (long)(vPos.x);
	cpoint.y = (long)(vPos.z);

	const auto townIt = std::ranges::find_if(map,
		[&](const auto & pair) {
			return Point_In_Poly(pair.second, cpoint);
		}
	);

	if (townIt == map.end()) return CONT_NODATA;
	return townIt->first;
}

BYTE CContinent::GetArea(BYTE nCont) {
	if (nCont == CONT_NODATA)    return CONT_NODATA;
	else if (nCont <= CONT_EAST) return CONT_EAST;
	else if (nCont >= CONT_WEST) return CONT_WEST;

	return CONT_NODATA;
}

const char * CContinent::GetContinentName(const BYTE nCont) {
	switch (nCont) {
		case CONT_FLARIS:       return prj.GetText(TID_GAME_CONT_FLARIS);
		case CONT_SAINTMORNING: return prj.GetText(TID_GAME_CONT_SAINTMORNING);
		case CONT_DARKON12:     return prj.GetText(TID_GAME_CONT_DARKON12);
		case CONT_DARKON3:      return prj.GetText(TID_GAME_CONT_DARKON3);
		case CONT_RICIS:        return prj.GetText(TID_GAME_CONT_RICIS);
		case CONT_EAST:         return prj.GetText(TID_GAME_CONT_EAST);
		case CONT_WEST:         return prj.GetText(TID_GAME_CONT_WEST);
		case CONT_ALL:          return prj.GetText(TID_GAME_CONT_ALL);
		default:                return "";
	}
}

#ifdef __WORLDSERVER
D3DXVECTOR3 CContinent::GetRevivalPos( BYTE nContinent )
{
	D3DXVECTOR3 vPos;
	
	switch( nContinent )
	{
		case CONT_NODATA :			vPos = D3DXVECTOR3( 6968, 0, 3328 );	break;
		case CONT_FLARIS :			vPos = D3DXVECTOR3( 6968, 0, 3328 );	break;
		case CONT_SAINTMORNING :	vPos = D3DXVECTOR3( 8470, 0, 3635 );	break;
		case CONT_DARKON12 :		vPos = D3DXVECTOR3( 3808, 0, 4455 );	break;
		case CONT_DARKON3 :			vPos = D3DXVECTOR3( 3808, 0, 4455 );	break;
		case CONT_RICIS :			vPos = D3DXVECTOR3( 8470, 0, 3635 );	break;
		case CONT_EAST :			vPos = D3DXVECTOR3( 6959, 0, 3217 );	break;
		case CONT_WEST :			vPos = D3DXVECTOR3( 3824, 0, 4456 );	break;
		
		default :					vPos = D3DXVECTOR3( 6968, 0, 3328 );	break;
	}
	
	return vPos;
}
#endif // __WORLDSERVER

#ifdef __CLIENT
int	CContinent::GetMapNo( CMover* pMover )
{
	if( IsValidObj( pMover ) && pMover->GetWorld()->GetID() == WI_WORLD_MADRIGAL )
		return GetMapNo( pMover->GetPos() );
	return 100;
}

int	CContinent::GetMapNo( const D3DXVECTOR3& vPos )
{
	BYTE nCont = GetContinent( vPos );
	switch( nCont )
	{
		case CONT_FLARIS :			return 5;
		case CONT_SAINTMORNING :	return 4;
		case CONT_DARKON12 :		return 1;
		case CONT_DARKON3 :			return 2;
		case CONT_RICIS :			return 3;
		case CONT_HARMONIN :		return 6;
		case CONT_ESTIA :			return 7;
		default:					return 100;
	}
}
#endif // __CLIENT


int CContinent::GetContinentIndex( const char* continentName )
{
	if( strcmp( "Flaris", continentName ) == 0 )
	{
		return CONT_FLARIS;
	}else 
	if( strcmp( "Saint", continentName ) == 0 )
	{
		return CONT_SAINTMORNING;
	}else
	if( strcmp( "Ricis", continentName ) == 0 )
	{
		return CONT_RICIS;
	}else
	if( strcmp( "Darkon12", continentName ) == 0 )
	{
		return CONT_DARKON12;
	}
	else
	if( strcmp( "Darkon3", continentName ) == 0 )
	{
		return CONT_DARKON3;
	}
	else
	if( strcmp( "Harmonin", continentName ) == 0 )
	{
		return CONT_HARMONIN;
	}

	return CONT_NODATA;
}

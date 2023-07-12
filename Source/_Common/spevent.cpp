#include "stdafx.h"
#include "spevent.h"
#include "script.h"
#ifdef __WORLDSERVER
#include "user.h"
#ifdef __EVENT_0117
#include "respawn.h"
#include "worldmng.h"
#endif	// __EVENT_0117
#endif	// __WORLDSERVER
#include <numeric>

CEventItem::CEventItem()
{
	m_dwItemId	= 0;
	m_nMax	= 0;
	m_nNum	= 0;
	m_dwTimeout	= 0;
	m_lSkip	= 0;
	memset( (void*)m_adwInterval, 0, sizeof(m_adwInterval) );
}

CEventItem::CEventItem( DWORD dwItemId, int nMax, int nNum )
{
	m_dwItemId	= dwItemId;
	m_nMax	= nMax;
	m_nNum	= nNum;
	m_dwTimeout	= GetTickCount();
	m_lSkip	= 0;

	static constexpr int s_anHour[24]	=
		{	505, 409, 324, 280, 220, 203, 202, 212,
			227, 261, 302, 349, 571, 701, 764, 803,
			790, 789, 754, 849, 936, 940, 919, 720	};
	
	static constexpr int nTotal = std::reduce(s_anHour, s_anHour + 24);

	for( int i = 0; i < 24; i++ )
		m_adwInterval[i]	= (DWORD)( (float)MIN( 60 ) / ( nMax * (float)s_anHour[i] / (float)nTotal ) );
}

bool CEventItem::IsTimeout(const int nHour) {
	const DWORD dwTickCount = GetTickCount();
	if (dwTickCount < m_dwTimeout) return false;

	m_dwTimeout	= dwTickCount + m_adwInterval[nHour];

	if (m_lSkip <= 0) return true;

	m_lSkip--;
	return false;
}

void CEventItem::Skip( LONG lSkip )
{
	m_lSkip		= lSkip;
}

CAr & operator<<(CAr & ar, const CEventItem & self) {
	ar << self.m_dwItemId;
	ar << self.m_nMax;
	ar << self.m_nNum;
	ar.Write(self.m_adwInterval, sizeof(self.m_adwInterval));

	return ar;
}

CAr & operator>>(CAr & ar, CEventItem & self) {
	ar >> self.m_dwItemId;
	ar >> self.m_nMax;
	ar >> self.m_nNum;
	ar.Read(self.m_adwInterval, sizeof(self.m_adwInterval) );
	
	self.m_dwTimeout		= GetTickCount();
	return ar;
}

CEventGeneric::CEventGeneric()
{
	m_dwFlag	= 0;
}

CEventGeneric::~CEventGeneric()
{
	Clear();	// clear all
}

void CEventGeneric::Clear( BOOL bDestructor )
{
	m_lspEvent.clear();
	m_mapEventItemList.clear();

#ifdef __EVENT_0117
	if( bDestructor )
	{
		for( auto i3	= m_mapSpawn.begin(); i3 != m_mapSpawn.end(); ++i3 )
			safe_delete( i3->second );
		m_mapSpawn.clear();
	}
#endif	// __EVENT_0117
}

BOOL CEventGeneric::LoadScript( LPCSTR lpFilename )
{
	CScript s;
	if( s.Load( lpFilename ) == FALSE )
		return FALSE;
	s.GetToken();	// subject or FINISHED
	
	while( s.tok != FINISHED )
	{
		if( s.Token == _T( "Event" ) )
		{
			EVENT_GENERIC & rEvent = m_lspEvent.emplace_back();
			EVENT_GENERIC * pEvent	= &rEvent;
			pEvent->nId		= s.GetNumber();
			pEvent->nFlag	= 0x00000001 << ( pEvent->nId - 500 );

			s.GetToken();	// {
			s.GetToken();
			while( *s.token != '}' )
			{
				if( s.Token == _T( "Title" ) )
				{
					s.GetToken();	// {
					int nLang	= s.GetNumber();
					while( *s.token != '}' )
					{
						s.GetLastFull();
						CString token	= s.token;
						token.Replace( "\"", "" );
						if( nLang == ::GetLanguage() )
						{
							CString str		= token;
							lstrcpy( pEvent->pszTitle, (LPCSTR)str );
						}
						nLang	= s.GetNumber();
					}
				}

				else if( s.Token == _T( "Time" ) )
				{
#ifdef __DBSERVER
					s.GetToken();	// {
					int nYear	= s.GetNumber();
					int nMonth	= s.GetNumber();
					int nDay	= s.GetNumber();
					int nHour	= s.GetNumber();
					CTime t	= CTime( nYear, nMonth, nDay, nHour, 0, 0 );
					pEvent->tStart	= (time_t)( t.GetTime() );
					nYear	= s.GetNumber();
					nMonth	= s.GetNumber();
					nDay	= s.GetNumber();
					nHour	= s.GetNumber();
					t	= CTime( nYear, nMonth, nDay, nHour, 0, 0 );
					pEvent->tEnd	= (time_t)( t.GetTime() );
					s.GetToken();	// }
#else	// __DBSERVER
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
#endif	// __DBSERVER
				}
				else if( s.Token == _T( "Item" ) )
				{
#ifdef __DBSERVER
					s.GetToken();	// {
					DWORD dwItemId	= s.GetNumber();
					while( *s.token != '}' )
					{
						if( dwItemId == 0 )
						{
							//
						}
						const int nMax	= s.GetNumber();
						const int nNum	= s.GetNumber();

						m_mapEventItemList[pEvent->nId].emplace_back(dwItemId, nMax, nNum);

						dwItemId	= s.GetNumber();
					}
#else	// __DBSERVER
					s.GetToken();
					s.GetToken();
					while( *s.token != '}' )
					{
						s.GetToken();
						s.GetToken();
						s.GetToken();
					}
#endif	// __DBSERVER
				}
				else if( s.Token == _T( "fExpFactor" ) )
				{
#ifdef __DBSERVER
					pEvent->fExpFactor	= s.GetFloat();
#else	// __DBSERVER
					s.GetToken();
#endif	// __DBSERVER
				}
#ifdef __ITEMDROPRATE
				else if( s.Token == _T( "fItemDropRate" ) )
				{
#ifdef __DBSERVER
					pEvent->fItemDropRate = s.GetFloat();
#else	// __DBSERVER
					s.GetToken();
#endif	// __DBSERVER
				}
#endif // __ITEMDROPRATE
				else if( s.Token == _T( "Spawn" ) )
				{
#if defined(__WORLDSERVER ) && defined(__EVENT_0117)
					s.GetToken();	// {
					DWORD dwType	= s.GetNumber();
					DWORD dwIndex	= s.GetNumber();
					int nMax	= s.GetNumber();
					float fRatio	= s.GetFloat();
					DWORD dwInterval	= 86400000 / nMax;	// 86400000	= 1day
					AddSpawn( pEvent->nId, dwType, dwIndex, nMax, fRatio, dwInterval );
					s.GetToken();	// }
#else	// __WORLDSERVER
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
					s.GetToken();
#endif	// __WORLDSERVER
				}
				s.GetToken();
			}
		}
		s.GetToken();
	}
	return TRUE;
}

CEventGeneric*	CEventGeneric::GetInstance( void )
{
	static CEventGeneric	sEventGeneric;
	return &sEventGeneric;
}

#ifdef __WORLDSERVER
std::pair<CEventItem *, int> CEventGeneric::GetItem() {
	const int nHour	= CTime::GetCurrentTime().GetHour();

	for (auto & [eventId, eventItems] : m_mapEventItemList) {
		if (!g_eLocal.GetState(eventId)) continue;

		for (CEventItem & pEventItem : eventItems) {
			if (pEventItem.IsTimeout(nHour)) {
				const int nNum = xRandom(1, pEventItem.m_nNum + 1);
				pEventItem.Skip((LONG)(nNum - 1));
				
				return { &pEventItem, nNum };
			}
		}
	}

	return { nullptr, 0 };
}

FLOAT CEventGeneric::GetExpFactor( void )
{
	FLOAT	fExpFactor	= 1.0f;

	for (const EVENT_GENERIC & pEvent : m_lspEvent) {
		if (g_eLocal.GetState(pEvent.nId)) {
			fExpFactor *= pEvent.fExpFactor;
		}
	}

	return fExpFactor;
}

#ifdef __ITEMDROPRATE
FLOAT CEventGeneric::GetItemDropRateFactor( void )
{
	FLOAT	fFactor	= 1.0f;
	for (const EVENT_GENERIC & pEvent : m_lspEvent) {
		if( g_eLocal.GetState( pEvent.nId ) )
			fFactor	*= pEvent.fItemDropRate;
	}
	return fFactor;
}
#endif // __ITEMDROPRATE
#endif	// __WORLDSERVER
CAr & operator<<(CAr & ar, const CEventGeneric & self) {
	ar << (int)self.m_lspEvent.size();
	for (const EVENT_GENERIC & pEvent : self.m_lspEvent) {
		ar.Write(&pEvent, sizeof(EVENT_GENERIC));
	}

	ar << (int)self.m_mapEventItemList.size();
	for (const auto & [nEvent, pList] : self.m_mapEventItemList) {
		ar << nEvent;

		ar << (int)pList.size();
		for (const CEventItem & pEventItem : pList) {
			ar << pEventItem;
		}
	}

	return ar;
}

CAr & operator>>(CAr & ar, CEventGeneric & self) {
	self.Clear( FALSE );
	int nEventSize;
	ar >> nEventSize;
	for( int i = 0; i < nEventSize; i++ )
	{
		EVENT_GENERIC & pEvent = self.m_lspEvent.emplace_back();
		ar.Read(&pEvent, sizeof(EVENT_GENERIC));
	}

	int nEventItemListSize;
	ar >> nEventItemListSize;
	for( int i = 0; i < nEventItemListSize; i++ )
	{
		int nEvent;
		ar >> nEvent;

		std::vector<CEventItem> & itemList = self.m_mapEventItemList[nEvent];

		int nEventItemSize; ar >> nEventItemSize;
		for( int j = 0; j < nEventItemSize; j++ ) {
			auto & pEventItem = itemList.emplace_back();
			ar >> pEventItem;
		}
	}

	return ar;
}

BOOL CEventGeneric::Run( void )
{
	BOOL f	= FALSE;
	time_t t	= time_null();

	for (const EVENT_GENERIC & pEvent : m_lspEvent) {
		const bool bEvent	= ( t >= pEvent.tStart && t < pEvent.tEnd );
		// ˬ
		char lpOutputString[512]	= { 0, };
		sprintf( lpOutputString, "m_dwFlag=0x%08x, nId=%d, tStart=%d, tEnd=%d, t=%d, nFlag=%d", m_dwFlag, pEvent.nId, pEvent.tStart, pEvent.tEnd, time_null(), pEvent.nFlag );
		OutputDebugString( lpOutputString );

		if( bEvent && !( m_dwFlag & pEvent.nFlag ) )
		{
			m_dwFlag	|= pEvent.nFlag;
			f	= TRUE;
		}
		else if( !bEvent && ( m_dwFlag & pEvent.nFlag ) )
		{
			m_dwFlag	&= ~pEvent.nFlag;
			f	= TRUE;
		}
	}
	return f;
}

const EVENT_GENERIC * CEventGeneric::GetEvent(const int nEvent ) const {
	for (const EVENT_GENERIC & pEvent : m_lspEvent) {
		if (pEvent.nId == nEvent) {
			return &pEvent;
		}
	}

	return nullptr;
}

#ifdef __WORLDSERVER
#ifdef __EVENT_1101
LONG CEventGeneric::GetEventElapsed( void )
{
	const EVENT_GENERIC * pEvent = GetEvent(531);
	if( !pEvent )
		return 0;
	CTime tStart	= CTime( pEvent->tStart );
	CTime tCur	= CTime::GetCurrentTime();
	if( tCur < tStart )
		return 0;
	CTimeSpan ts	= tCur - tStart;
	return	( (LONG)( ts.GetDays() ) );
}

void CEventGeneric::CallTheRoll( void )
{
	const EVENT_GENERIC * pEvent = GetEvent( 531 );	// �̺�Ʈ�� ��ϵǾ� ���� ������
	if( !pEvent )
		return;

	if( g_eLocal.GetState( 531 ) == 0 )	// �̺�Ʈ �Ⱓ�� �ƴϸ�
		return;

	int nElapsed	= (int)GetEventElapsed();
	g_UserMng.CallTheRoll( nElapsed );
}
#endif	// __EVENT_1101
#endif	// __WORLDSERVER

#ifdef __EVENT_0117
void CEventGeneric::AddSpawn( int nEvent, DWORD dwType, DWORD dwIndex, int nMax, float fRatio, DWORD dwInterval )
{
	CSpawn* pSpawn	= new CSpawn( dwType, dwIndex, nMax, fRatio, dwInterval );
	bool bResult	= m_mapSpawn.emplace( nEvent, pSpawn ).second;
}

void CEventGeneric::AddRegionGeneric( int nLevel, DWORD dwWorldId, CRespawnInfo* pi )
{
	REGION_GENERIC	regionGeneric;
	regionGeneric.nLevel	= nLevel;
	regionGeneric.dwWorldId		= dwWorldId;
	regionGeneric.pi	= pi;

	m_aRegionGeneric.push_back( regionGeneric );
}

bool prRegionGeneric( REGION_GENERIC r1, REGION_GENERIC r2 )
{
	return r2.nLevel > r1.nLevel;
}

void CEventGeneric::SortRegionGeneric( void )
{
	std::sort( m_aRegionGeneric.begin(), m_aRegionGeneric.end(),  prRegionGeneric );
}

void CEventGeneric::Spawn( void )
{
	if( m_aRegionGeneric.size() == 0 )
		return;
	for( auto i	= m_mapSpawn.begin(); i != m_mapSpawn.end(); ++i )
	{
		if( g_eLocal.GetState( i->first ) )
		{
			CSpawn* pSpawn	= i->second;
			if( pSpawn->IsTimeout() )
			{
				int nIndex	= pSpawn->GetSpawnIndex( m_aRegionGeneric.size() );
				CWorld* pWorld	= g_WorldMng.GetWorld( m_aRegionGeneric[nIndex].dwWorldId );
				if( pWorld )
				{
					D3DXVECTOR3 v = m_aRegionGeneric[nIndex].pi->GetRandomPosition();
					v.y		=	pWorld->GetLandHeight( v.x, v.z ) + 1.0f;
					CObj* pObj	= CreateObj( pSpawn->m_dwType, pSpawn->m_dwIndex );
					if( pSpawn->m_dwType == OT_ITEM )
					{
						CItemElem* pItemElem	= new CItemElem;
						pItemElem->m_dwItemId	= pSpawn->m_dwIndex;
						pItemElem->m_nItemNum	= 1;
						ItemProp* pItemProp	= pItemElem->GetProp();
						pItemElem->m_nHitPoint	= pItemProp->dwEndurance;
						pItemElem->SetSerialNumber();
						( (CItem*)pObj )->m_dwDropTime	= g_tmCurrent + MIN( 1440 );	// 24�ð� ����
						( (CItem*)pObj )->m_pItemBase	= pItemElem;
						pObj->SetPos( v );
//#ifdef _DEBUG
						char lpOutputString[100]	= { 0, };
						sprintf( lpOutputString, "spawn: type = %d, index = %d, world = %d, x = %f, z = %f\n", OT_ITEM, pItemElem->m_dwItemId, pWorld->GetID(), pObj->GetPos().x, pObj->GetPos().z );
						//g_UserMng.AddWorldMsg( WI_WORLD_MADRIGAL, lpOutputString );
						OutputDebugString( lpOutputString );
//#endif	// _DEBUG
						pWorld->ADDOBJ( pObj, TRUE, nTempLayer );
					}
					else
					{
						// ���Ϳ� ��Ʈ�� ������ �� �� �߰� ����
						SAFE_DELETE( pObj );
					}
				}	// pWorld
			}	// IsTimeout
		}
	}
}

CSpawn::CSpawn()
{
	m_nMax	= 0;
	m_fRatio	= 0.0F;
	m_dwType	= 0;
	m_dwIndex	= 0;
	m_dwInterval	= 0;
	m_dwTimeout		= 0;
}

CSpawn::CSpawn( DWORD dwType, DWORD dwIndex, int nMax, float fRatio, DWORD dwInterval )
{
	m_dwType	= dwType;
	m_dwIndex	= dwIndex;
	m_nMax	= nMax;
	m_fRatio	= fRatio;
	m_dwInterval	= dwInterval;
	m_dwTimeout		= 0;
}

BOOL CSpawn::IsTimeout( void )
{
	if( g_tmCurrent >= m_dwTimeout )
	{
		m_dwTimeout		= g_tmCurrent + m_dwInterval;
		return TRUE;
	}
	return FALSE;
}
#endif	// __EVENT_0117
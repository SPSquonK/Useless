#include "stdafx.h"
#include <algorithm>
#include "ServerDesc.h"
#include "Data.h"
#include "Landscape.h"


CAr & operator<<(CAr & ar, const CServerDesc & self) {
	ar << self.m_uKey;
	ar << static_cast<std::uint32_t>(self.m_lspJurisdiction.size());
	for (const WorldId & jurisdiction : self.m_lspJurisdiction) {
		ar << jurisdiction;
	}
	ar << self.m_szAddr;
	return ar;
}

CAr & operator>>(CAr & ar, CServerDesc & self) {
	u_long uKey; ar >> uKey;
	self.SetKey(uKey);

	self.m_lspJurisdiction.clear();
	std::uint32_t nSize;
	ar >> nSize;
	for (std::uint32_t i = 0; i < nSize; i++) {
		WorldId pJurisdiction; ar >> pJurisdiction;
		self.m_lspJurisdiction.emplace(pJurisdiction);
	}

	ar >> self.m_szAddr;

	return ar;
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CServerDescArray::CServerDescArray()
{
}

CServerDescArray::~CServerDescArray()
{
	Free();
}

void CServerDescArray::Free( void )
{
	CMclAutoLock Lock( m_AddRemoveLock );

	for( auto i = begin(); i != end(); ++i )
	{
		CServerDesc* pServer	= i->second;
		SAFE_DELETE( pServer );
	}
	clear();
}

CServerDesc* CServerDescArray::GetAt( ULONG uKey )
{
	CServerDescArray::iterator i = find( uKey );
	if( i != end() )
		return i->second;
	return NULL;
}
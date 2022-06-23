#include "stdafx.h"
#include "BillingMgr.h"
#include "BillingMgrJP.h"
#include "BillingMgrJP2.h"
#include "BillingMgrTW.h"
#include "BillingMgrTH.h"
#include "query.h"

///////////////////////////////////////////////////////////////////////
// global variable
///////////////////////////////////////////////////////////////////////

static			CBillingMgr* g_pBillingMgr = NULL;	

///////////////////////////////////////////////////////////////////////
// global function
///////////////////////////////////////////////////////////////////////

// strcpy�� ������� ������, NULL������ �����͸� �����Ѵ�.
char* StrCpyExcludeNull( char* dst, const char* src )
{
	char *cp = dst;

	while( *cp++ = *src++ )
		   ; 
	return( cp - 1 );
}

// pCur���� pEnd���� ����(0x20)�� ���δ�.
void AppendSpace( char* pCur, char* pEnd )
{
	if( pEnd - pCur )
		memset( pCur, 0x20, pEnd - pCur );
}

CBillingMgr* GetBillingMgr()
{
	ASSERT( g_pBillingMgr );
	return g_pBillingMgr;
}

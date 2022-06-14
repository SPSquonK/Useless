#include "stdafx.h"
#include "dpaccountclient.h"
#include "dpcertifier.h"
#include "dbmanager.h"
#include "..\Resource\Lang.h"

#include "user.h"

CDPCertifier::CDPCertifier()
{
	BEGIN_MSG;
	ON_MSG( PACKETTYPE_CERTIFY, &CDPCertifier::OnCertify );
	ON_MSG( PACKETTYPE_PING, &CDPCertifier::OnPing );
	ON_MSG( PACKETTYPE_CLOSE_EXISTING_CONNECTION, &CDPCertifier::OnCloseExistingConnection );
	ON_MSG( PACKETTYPE_KEEP_ALIVE, &CDPCertifier::OnKeepAlive );
	ON_MSG( PACKETTYPE_ERROR, &CDPCertifier::OnError );
}

void CDPCertifier::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID dpid )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			{
				LPDPMSG_CREATEPLAYERORGROUP lpCreatePlayer	= (LPDPMSG_CREATEPLAYERORGROUP)lpMsg;
				OnAddConnection( lpCreatePlayer->dpId );
				break;
			}
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
				OnRemoveConnection( lpDestroyPlayer->dpId );
				break;
			}
	}
}

void CDPCertifier::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID dpid )
{
	CAr ar( (LPBYTE)lpMsg, dwMsgSize );
	
	if( dwMsgSize < 4 )
	{
		WriteError( "PACKET//0" );
		return;
	}

	GETTYPE( ar );
	void ( theClass::*pfn )( theParameters )
		=	GetHandler( dw );

//	ASSERT( pfn );
	if( pfn )
		( this->*( pfn ) )( ar, dpid, (LPBYTE)lpMsg + sizeof(DWORD), dwMsgSize - sizeof(DWORD) );
}

void CDPCertifier::OnAddConnection( DPID dpid )
{
	if (!g_CertUserMng.AddUser(dpid)) {
#ifdef __US_LOGIN_0223
		DestroyPlayer(dpid);
#endif
	}
}

// �±��� ��� ���ƿ��� 22:00�ñ����� ���� �ð��� �ʴ����� ������.
long GetPlayLeftTime( BYTE cbAccountFlag )
{
	if( cbAccountFlag & ACCOUNT_FLAG_18 )	// ������ ���� 22:00�ÿ� ������ �ʱ⿡, �ð��� �˸��� �ʰ��� 
		return 0;		

	CTime cur = CTime::GetCurrentTime();
	
	CTimeSpan span; 
	if( cur.GetHour() < 22 )
	{
		CTime target( cur.GetYear(), cur.GetMonth(), cur.GetDay(), 22, 0, 0 );
		span = target - cur;
	}
	else
	{
		CTime target( cur.GetYear(), cur.GetMonth(), cur.GetDay(), 24, 0, 0 );
		span = target - cur;

		CTimeSpan ts( 0, 22, 0, 0 ); 
		span += ts;
	}

	return max( (long)( span.GetTotalSeconds() ), 1 );	// ��� 1�� �̻� ( 0 �� �����ϱ� ���ؼ� )
}

#ifdef __GPAUTH_01
#ifdef __GPAUTH_02
#ifdef __EUROPE_0514
void CDPCertifier::SendServerList( DPID dpId, DWORD dwAuthKey, BYTE cbAccountFlag, long lTimeSpan, const char* szGPotatoNo, const char* szCheck, const char* szBak )
#else	// __EUROPE_0514
void CDPCertifier::SendServerList( DPID dpId, DWORD dwAuthKey, BYTE cbAccountFlag, long lTimeSpan, const char* szGPotatoNo, const char* szCheck )
#endif	// __EUROPE_0514
#else	// __GPAUTH_02
void CDPCertifier::SendServerList( DPID dpId, DWORD dwAuthKey, BYTE cbAccountFlag, long lTimeSpan, const char* szGPotatoNo )
#endif	// __GPAUTH_02
#else	// __GPAUTH_01
void CDPCertifier::SendServerList( DPID dpId, DWORD dwAuthKey, BYTE cbAccountFlag, long lTimeSpan )
#endif	// __GPAUTH_01
{
	BEFORESEND( ar, PACKETTYPE_SRVR_LIST );
	ar << dwAuthKey;
	ar << cbAccountFlag;

#ifdef __BILLING0712	   
	ar << lTimeSpan;
#endif

#ifdef __GPAUTH_01
	BOOL bGPotatoAuth	= ::GetLanguage() == LANG_GER || ::GetLanguage() == LANG_FRE;
	if( bGPotatoAuth )
	{
		ar.WriteString( szGPotatoNo );
#ifdef __GPAUTH_02
		ar.WriteString( szCheck );
#endif	// __GPAUTH_02
	}
#ifdef __EUROPE_0514
	ar.WriteString( szBak );
#endif	// __EUROPE_0514

#endif	// __GPAUTH_01

	if( ::GetLanguage() == LANG_THA )
	{
		long lTimeLeft = GetPlayLeftTime( cbAccountFlag );	// �±��� ��� ���ƿ��� 22:00�ñ����� ���� �ð��� ������.
		ar << lTimeLeft;
	}

	m_servers.read([&ar](const CListedServers & servers) { ar << servers; });

	SEND( ar, this, dpId );
}

void CDPCertifier::SendError( LONG lError, DPID dpId )
{
	BEFORESEND( ar, PACKETTYPE_ERROR );
	ar << lError;
	SEND( ar, this, dpId );
//	DestroyPlayer( dpId ); shutdown�� ȣ������ ���ϴ� �� ���� 
}

#ifdef __GPAUTH
void CDPCertifier::SendErrorString( const char* szError, DPID dpid )
{
	BEFORESEND( ar, PACKETTYPE_ERROR_STRING );
	ar.WriteString( szError );
	SEND( ar, this, dpid );
}
#endif	// __GPAUTH

// Handlers
/*________________________________________________________________________________*/
void CDPCertifier::OnRemoveConnection(const DPID dpid) {
	g_CertUserMng.RemoveUser(dpid);
	g_dpAccountClient.SendRemoveAccount(dpid);
}

void CDPCertifier::OnError( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize )
{
	DestroyPlayer( dpid );
}

void CDPCertifier::OnCertify( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize )
{
	char pszVer[32]	= { 0, };
	ar.ReadString( pszVer, 32 );

	if( strcmp( m_szVer, pszVer ) )
	{
		SendError( ERROR_ILLEGAL_VER, dpid );
		return;
	}
#ifdef __SECURITY_0628
	char pszResVer[100]	= { 0,};
	ar.ReadString( pszResVer, 100 );
	if( lstrlen( m_szResVer ) > 0 && lstrcmp( m_szResVer, pszResVer ) )
	{
		SendError( ERROR_FLYFF_RESOURCE_MODIFIED, dpid );
		return;
	}
#endif	// __SECURITY_0628

	char pszAccount[MAX_ACCOUNT] = {0, }, pszPwd[MAX_PASSWORD] = {0, };
	ar.ReadString( pszAccount, MAX_ACCOUNT );

	//////////////////////////////////////////////////////////////////////////
	//	mulcom	BEGIN100218	�н����� ��ȣȭ
#ifdef __ENCRYPT_PASSWORD
	char		szEnc[ 16 * MAX_PASSWORD ] = {0, };
	char		szDec[ 16 * MAX_PASSWORD ] = {0, };
	
	ar.Read( szEnc, 16 * MAX_PASSWORD );
	
	g_xRijndael->ResetChain();
	g_xRijndael->Decrypt( szEnc, szDec, 16 * MAX_PASSWORD, CRijndael::CBC );
	
	::memcpy( pszPwd, szDec, MAX_PASSWORD );
#else
	ar.ReadString( pszPwd, MAX_PASSWORD );
#endif
	//	mulcom	END100218	�н����� ��ȣȭ
	//////////////////////////////////////////////////////////////////////////


	if( pszAccount[0] == '\0' || StringFind( pszAccount, '\'' ) >= 0 || StringFind( pszPwd, '\'' ) >= 0 )
	{
		DestroyPlayer( dpid );
		return;
	}

#ifdef __EUROPE_0514
	if (!g_CertUserMng.SetAccount(dpid, pszAccount)) {
		return;
	}
#endif	// __EUROPE_0514

	DB_OVERLAPPED_PLUS * pData = g_DbManager.Alloc();
	memset( &pData->AccountInfo, 0, sizeof(ACCOUNT_INFO) );
	strcpy( pData->AccountInfo.szAccount, pszAccount );
	strcpy( pData->AccountInfo.szPassword, pszPwd );
	_tcslwr( pData->AccountInfo.szAccount );
#ifdef __TWN_LOGIN0816
	char pszSessionPwd[MAX_SESSION_PWD]	= { 0,};
	if( GetLanguage() == LANG_TWN )
		ar.ReadString( pszSessionPwd, MAX_SESSION_PWD );
	lstrcpy( pData->AccountInfo.szSessionPwd, pszSessionPwd );
#endif	// __TWN_LOGIN0816
	pData->dwIP         = GetPlayerAddr( dpid );
	pData->dpId			= dpid;
	pData->nQueryMode	= CERTIFY;
	g_DbManager.PostQ( pData );
}

void CDPCertifier::OnPing( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize )
{
	g_dpAccountClient.SendPing( dpid );
}

void CDPCertifier::OnCloseExistingConnection( CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize )
{
	char pszAccount[MAX_ACCOUNT], pszPwd[MAX_PASSWORD];
	ar.ReadString( pszAccount, MAX_ACCOUNT );
	ar.ReadString( pszPwd, MAX_PASSWORD );

	if( pszAccount[0] == '\0' )
		return;

	DB_OVERLAPPED_PLUS * pData = g_DbManager.Alloc();
	strcpy( pData->AccountInfo.szAccount, pszAccount );
	strcpy( pData->AccountInfo.szPassword, pszPwd );
	_tcslwr( pData->AccountInfo.szAccount );
	pData->dwIP			= 0;		// don't use
	pData->dpId			= dpid;
	pData->nQueryMode	= CLOSE_EXISTING_CONNECTION;
	g_DbManager.PostQ( pData );
}

void CDPCertifier::OnKeepAlive(CAr & ar, DPID dpid, LPBYTE lpBuf, u_long uBufSize) {
	g_CertUserMng.KeepAlive(dpid);
}
/*________________________________________________________________________________*/
CDPCertifier	g_dpCertifier;

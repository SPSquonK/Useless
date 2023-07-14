#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "dpcertified.h"
#include "wndbase.h"
#include "wndcloseexistingconnection.h"
#include "Network.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDPCertified	g_dpCertified;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDPCertified::CDPCertified()
{
	m_timer.Set( MIN( 1 ) );
	m_lError = 0;
	m_bRecvSvrList = false;

	ON_MSG( PACKETTYPE_SRVR_LIST, &CDPCertified::OnSrvrList );
	ON_MSG( PACKETTYPE_ERROR, &CDPCertified::OnError );
#ifdef __GPAUTH
	ON_MSG( PACKETTYPE_ERROR_STRING, &CDPCertified::OnErrorString );
#endif	// __GPAUTH
	ON_MSG( PACKETTYPE_KEEP_ALIVE, &CDPCertified::OnKeepAlive );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Should I indicate disconnection?
bool CDPCertified::CheckNofityDisconnected() const noexcept {
	// If the server list is received, the server disconnection is not displayed.
	// and if it is disconnected due to an error, it is not displayed.
	return !m_bRecvSvrList && m_lError == 0;
}

void CDPCertified::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			{
				m_fConn		= true;
				m_bRecvSvrList = false;
			}
			break;
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
				m_lError = lpDestroyPlayer->dwFlags;
			}
			CNetwork::GetInstance().OnEvent( CERT_DISCONNECT );

			m_fConn		= false;

			if( CheckNofityDisconnected() )
			{
				g_WndMng.CloseMessageBox();
				g_WndMng.OpenMessageBox( prj.GetText(TID_DIAG_0023) );

				
				if (CWndLogin * pWndLogin = g_WndMng.GetWndBase<CWndLogin>(APP_LOGIN)) {
					pWndLogin->GetDlgItem(WIDC_OK)->EnableWindow(TRUE);
				}
			}

			m_lError = 0;		// �����ڵ� clear
			break;
	}
}

void CDPCertified::UserMessageHandler(LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID) {
	CAr ar((LPBYTE)lpMsg, dwMsgSize);
	DWORD dw; ar >> dw;
	Handle(ar, dw);
}

void CDPCertified::SendNewAccount( LPCSTR lpszAccount, LPCSTR lpszpw )
{
	BEFORESEND( ar, PACKETTYPE_NEW_ACCOUNT );
	ar.WriteString( lpszAccount );
	ar.WriteString( lpszpw );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 2006.03 MSG_VER�� ������ �κ��� 5�� �������� ����
void CDPCertified::SendCertify()
{
	if( ::GetLanguage() == LANG_ENG && ::GetSubLanguage() == LANG_SUB_USA )
		g_Neuz.m_dwTimeOutDis = GetTickCount() + SEC( 40 );
	else
		g_Neuz.m_dwTimeOutDis = GetTickCount() + SEC( 20 );
		
	BEFORESEND( ar, PACKETTYPE_CERTIFY );
	ar.WriteString( ::GetProtocolVersion() );		// MSG_VER 
#ifdef __SECURITY_0628
	ar.WriteString( CResFile::m_szResVer );
#endif	// __SECURITY_0628
	ar.WriteString( g_Neuz.m_szAccount );

	//////////////////////////////////////////////////////////////////////////
	//	mulcom	BEGIN100218	�н����� ��ȣȭ
#ifdef __ENCRYPT_PASSWORD
	char	szPlain[ 16 * MAX_PASSWORD ] = {0, };
	char	szEnc[ 16 * MAX_PASSWORD ] = {0, };
	
	::memcpy( szPlain, g_Neuz.m_szPassword, sizeof(g_Neuz.m_szPassword) );
	
	
	g_xRijndael->ResetChain();
	g_xRijndael->Encrypt( szPlain, szEnc, 16 * MAX_PASSWORD, CRijndael::CBC );
	
	ar.Write( szEnc, 16 * MAX_PASSWORD );
#else
	ar.WriteString( g_Neuz.m_szPassword );
#endif
	//	mulcom	END100218	�н����� ��ȣȭ


#ifdef __TWN_LOGIN0816
	if (GetLanguage() == LANG_TWN) {
		ar.WriteString(g_Neuz.m_szSessionPwd);
	}
#endif	// __TWN_LOGIN0816
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPCertified::SendCloseExistingConnection( const char* lpszAccount, const char* lpszpw )
{
	BEFORESEND( ar, PACKETTYPE_CLOSE_EXISTING_CONNECTION );
	ar.WriteString( lpszAccount );
	ar.WriteString( lpszpw );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// �����ε����� ������ ������ �̸���  ��´�.
// nServerIndex - ���� ListBox���� ������ ���� ��ȣ ( 0���� ���� )
LPCTSTR CDPCertified::GetServerName(int nServerIndex) const {
	const auto span = m_servers.GetServers();
	if (nServerIndex < 0) return "Unknown";
	if (static_cast<size_t>(nServerIndex) >= span.size()) return "Unknown";
	return span[nServerIndex].lpName;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CDPCertified::OnKeepAlive(CAr &) {
	SendPacket<PACKETTYPE_KEEP_ALIVE>();
}


//	Handlers
void CDPCertified::OnSrvrList( CAr & ar )
{
	m_bRecvSvrList = true;

	ar >> g_Neuz.m_dwAuthKey;
	ar >> g_Neuz.m_cbAccountFlag;

#ifdef __GPAUTH_01
	if( g_Neuz.m_bGPotatoAuth )
	{
		ar.ReadString( g_Neuz.m_szGPotatoNo, 10 );
#ifdef __GPAUTH_02
		ar.ReadString( g_Neuz.m_szCheck, 255 );
#endif	// __GPAUTH_02
	}
#ifdef __EUROPE_0514
	char szBak[MAX_ACCOUNT]	= { 0,};
	ar.ReadString( szBak, MAX_ACCOUNT );
	if( lstrcmp( g_Neuz.m_szAccount, szBak ) )
	{
		Error( "CDPCertified.OnSrvrList" );
		exit( 0 );
	}
#endif	// __EUROPE_0514		
#endif	// __GPAUTH_01

	if( ::GetLanguage() == LANG_THA )
	{
		long lTimeLeft;	
		ar >> lTimeLeft;		// �±��� ��� ���ƿ��� 22:00�ñ����� ���� �ð��� �޴´�.
		g_Neuz.SetLeftTime( lTimeLeft );
	}

	// ���� �׽�Ʈ ���� ���� �����ΰ�?
	CString strAddr	= g_Neuz.m_lpCertifierAddr;
	BOOL bPrivate	= ( ::GetLanguage() == LANG_GER && strAddr.Find( "192.168", 0 ) == 0 );

	ar >> m_servers;

	if (bPrivate) {
		for (CListedServers::Server & server : m_servers.GetServers()) {
			if (bPrivate && server.lpAddr[0] != '\0') {
				lstrcpy(server.lpAddr, g_Neuz.m_lpCertifierAddr);
			}
		}
	}

	CNetwork::GetInstance().OnEvent( CERT_SRVR_LIST );

	if (CWndLogin * pWndBase = g_WndMng.GetWndBase<CWndLogin>(APP_LOGIN)) {
		pWndBase->Connected();
	}
}

#ifdef __GPAUTH
void CDPCertified::OnErrorString( CAr & ar )
{
	char szError[256]	= { 0,};
	ar.ReadString( szError );

	g_WndMng.CloseMessageBox();
	g_WndMng.OpenMessageBox( szError );
		
	CWndLogin* pWndLogin	= (CWndLogin*)g_WndMng.GetWndBase( APP_LOGIN );
	if( pWndLogin )
		pWndLogin->GetDlgItem( WIDC_OK )->EnableWindow( TRUE );

	// ���� ǥ�� �� ������ ����� ��Ŷ�� ������. shutdown�� ���� �ʴ� �� ���� 
	SendPacket<PACKETTYPE_ERROR>();
}
#endif	// __GPAUTH

void CDPCertified::OnError( CAr & ar )
{
	g_Neuz.m_dwTimeOutDis = 0xffffffff;			// Ÿ�� �ƿ� �޼��� �ڽ� ǥ�ø� ���´�.
	int nText = 0;

	ar >> m_lError;

	CNetwork::GetInstance().OnEvent( CERT_ERROR );

	switch( m_lError )
	{
		case ERROR_DUPLICATE_ACCOUNT:			// 103L
			{
				g_WndMng.CloseMessageBox();
				g_WndMng.m_pWndCloseExistingConnection	= new CWndCloseExistingConnection;
				g_WndMng.m_pWndCloseExistingConnection->Initialize();
				return;
			}

		case ERROR_ILLEGAL_VER:
			nText = TID_DIAG_0035;
			break;
#ifdef __SECURITY_0628
		case ERROR_FLYFF_RESOURCE_MODIFIED:
			nText	= TID_GAME_RESOURCE_MODIFIED;
			break;
#endif	// __SECURITY_0628
		case ERROR_ACCOUNT_EXISTS:				// 100L �̹� ���� �̸��� ������ �ֽ��ϴ�.
			nText = TID_DIAG_0032;
			break;
		case ERROR_FLYFF_PASSWORD:				// 120L (��� ��ȣ�� Ʋ���ϴ�.)
			nText = TID_DIAG_0016;
			break;
		case ERROR_FLYFF_ACCOUNT:				// 121L (�߸��� �����Դϴ�.)
			nText = TID_DIAG_0038;
			break;
		case ERROR_OVERFLOW:					// 108L �����ڰ� �ʹ� �����ϴ�.
			nText = TID_DIAG_0041;
			break;
		case ERROR_EXTERNAL_ADDR:				// 109L ���� �������� �ƴմϴ�.
			nText = TID_DIAG_0053;
			break;
		case ERROR_BLOCKGOLD_ACCOUNT:			// 119L ���� �����Դϴ�.
			nText = TID_DIAG_0015;
			break;
		case ERROR_FLYFF_AUTH:					// 122L �Ǹ������� ���������� �����մϴ� www.flyff.com���� �������ֽʽÿ�
			nText = TID_DIAG_0026;
			break;
		case ERROR_FLYFF_PERMIT:				// 123L �������� 12�� �̻� �̿밡 �̹Ƿ� ���������� �Ҽ� �����ϴ�.
			nText = TID_DIAG_0050;
			break;		
		case ERROR_FLYFF_NEED_AGREEMENT:		// 124L 14�� �̸� ������ �е��� �θ�� ���Ǽ��� �����ּž� ���� ������ �����մϴ�. www.flyff.com ���� �����ϼż� Ȯ���� �ּ���.
			nText = TID_DIAG_0001;
			break;
		case ERROR_FLYFF_NO_MEMBERSHIP:			// 125L	 ������ Ż���� �����Դϴ�. www.flyff.com ���� �����ϼż� Ȯ���� �ּ���." ) );
			nText = TID_GAME_ACCOUNTWEBRETIRE;
			break;		
		case ERROR_BILLING_INFO_FAILED:			// ���� ���� ����
			nText = TID_DIAG_NOCHARGING;
			break;
		case ERROR_BILLING_DATABASE_ERROR:		// ���� DB ���� 
			nText = TID_DIAG_DBERROR1;
			break;
		case ERROR_BILLING_TIME_OVER:			// ���� ���ð� ���� 
			nText = TID_DIAG_EXPIRY;
			break;		
		case ERROR_BILLING_OTHER_ERROR:			// ���� ��Ÿ �ٸ� ���� 
			nText = TID_DIAG_DBERROR2;
			break;
		case ERROR_BILLING_DISCONNECTED:
			nText = TID_DIAG_DBERROR2;
			break;
		case ERROR_TOO_LATE_PLAY:				// 131L
			nText = TID_GAME_LIMITCONNECTION;
			break;
		case ERROR_IP_CUT:						// 132L
			nText = TID_GAME_OTHERCOUNTRY; 
			break;
		case ERROR_FLYFF_DB_JOB_ING:			// 133L
			nText = TID_DB_INSPECTION;
			break;
		case ERROR_15SEC_PREVENT:				// 134L
			nText = TID_15SEC_PREVENT;
			break;
		case ERROR_15MIN_PREVENT:				// 135L 
			nText = TID_15MIN_PREVENT;
			break;
		case ERROR_CERT_GENERAL:
			nText = TID_ERROR_CERT_GENERAL;
			break;
		case ERROR_FLYFF_EXPIRED_SESSION_PASSWORD:
			nText	= TID_ERROR_EXPIRED_SESSION_PASSWORD;
			break;
	}

	if( nText > 0 )
	{
		g_WndMng.CloseMessageBox();
		g_WndMng.OpenMessageBox( _T( prj.GetText(nText) ) );
		
		CWndLogin* pWndLogin	= (CWndLogin*)g_WndMng.GetWndBase( APP_LOGIN );
		if( pWndLogin )
			pWndLogin->GetDlgItem( WIDC_OK )->EnableWindow( TRUE );

		// ���� ǥ�� �� ������ ����� ��Ŷ�� ������. shutdown�� ���� �ʴ� �� ���� 
		SendPacket<PACKETTYPE_ERROR>();
	}
}



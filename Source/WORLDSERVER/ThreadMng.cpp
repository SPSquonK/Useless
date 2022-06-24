#include "stdafx.h"
#include "ThreadMng.h"
#include "WorldMng.h"
#include "DPSrvr.h"
#include "User.h"
#include "GlobalTime.h"
#include "party.h"
#include "dpdatabaseclient.h"
#include "guildquest.h"
#include "DisplayedInfo.h"
#include "eveschool.h"
#include "SecretRoom.h"
#include "CreateMonster.h"
#include "rangda.h"
#include "RainbowRace.h"
#include "PCBang.h"
#include "InstanceDungeonParty.h"

#ifdef __QUIZ
#include "Quiz.h"
#endif // __QUIZ

#ifdef __NEW_PROFILE
#include "ProfileInfo.h"
#endif // __NEW_PROFILE

#include "dpcoreclient.h"
#include "dpdatabaseclient.h"
#include "spevent.h"
#include "guild.h"
#include "guildwar.h"
extern	char				g_szDBAddr[16];
extern	char				g_szCoreAddr[16];

extern  DWORD				g_tmCurrent;
extern	char				g_sHeartbeat[32];

extern  BOOL LoadAIScript();		// aimonster2.cpp

float	r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12;

#if defined(_DEBUG) || defined(__TESTSERVER)
	BOOL g_bProfiling = TRUE;
#else
	BOOL g_bProfiling = FALSE;
#endif

void LogPerformance( DWORD dwCurTick );

////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTimeout
////////////////////////////////////////////////////////////////////////////////////////////////////////

CTimeout::CTimeout( DWORD dwInterval, DWORD dwFirstTimeout )
{
	m_dwInterval		= dwInterval;
	m_endTime	= timeGetTime() + dwFirstTimeout;
}

DWORD CTimeout::Over( DWORD dwCurr )
{
	if( dwCurr > m_endTime )
		return ( dwCurr - m_endTime );
	return 0;
}
void CTimeout::Reset( DWORD dwCurr, DWORD dwTimeout )
{
	if( dwTimeout > m_dwInterval )
		m_endTime	= dwCurr;
	else
		m_endTime	= dwCurr + ( m_dwInterval - dwTimeout );
}
BOOL CTimeout::TimeoutReset( DWORD dwCurr )
{
	DWORD dwTimeout	= Over( dwCurr );
	if( dwTimeout > 0 )
	{
		Reset( dwCurr, dwTimeout );
		return TRUE; 
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void LogPerformance( DWORD dwCurTick )
{
	static int nFrame = 0;			
	static DWORD dwPrev = 0;
	static DWORD dwElapsed = 0;

	++nFrame;
	DWORD dwTick = dwCurTick - dwPrev; 
	dwPrev = dwCurTick;
	dwElapsed += dwTick;

	if( dwTick > 100 )
	{
		g_DisplayedInfo.SetLogInfo(LOGTYPE_WARN1, "warning - tick:%d", dwTick);

		if( g_bProfiling )
		{
			float sum = r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + r9 + r10 + r11 + r12 + 1.0f;

			char szBuffer[256];
			sprintf( szBuffer, 
					"%d, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f\n",
					dwTick,
					( r1 * 100.0f / sum ),
					( r2 * 100.0f / sum ),
					( r3 * 100.0f / sum ),
					( r4 * 100.0f / sum ),
					( r5 * 100.0f / sum ),
					( r6 * 100.0f / sum ),
					( r7 * 100.0f / sum ),
					( r8 * 100.0f / sum ),
					( r9 * 100.0f / sum ),
					( r10 * 100.0f / sum ),
					( r11 * 100.0f / sum ),
					( r12 * 100.0f / sum ) );

			g_DisplayedInfo.SetLogInfo(LOGTYPE_WARN2, szBuffer);
			OutputDebugString( szBuffer );
		}
	}

#ifdef __INFINITE_0227
	if( dwTick > 1000 )
	{
		char szBuffer[256];
			sprintf( szBuffer, 
					"OP: %d, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f\n",
					dwTick, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12 );
			Error( szBuffer );
	}
#endif	// __INFINITE_0227

	if( dwElapsed > 1000 )
	{
		g_DisplayedInfo.SetLogInfo( LOGTYPE_CCU, "CCU:%d", g_UserMng.GetCount() );

		if( g_bProfiling )
			g_DisplayedInfo.SetLogInfo( LOGTYPE_PERFOMANCE, "frame:%d tick:%d", nFrame, dwTick );

		dwElapsed = 0; // -= 1000
		nFrame = 0;

		r1 = r2 = r3 = r4 = r5 = r6 = r7 = r8 = r9 = r10 = r11 = r12 = 0;
		g_DisplayedInfo.Redraw();
#ifdef __NEW_PROFILE
		if( CProfileInfo::GetInstance()->IsToggleProfiling() )
			CProfileInfo::GetInstance()->SetProfileInfo();
#endif // __NEW_PROFILE
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRunObject
////////////////////////////////////////////////////////////////////////////////////////////////////////
CRunObject::CRunObject()
{
	m_hRunObject	= (HANDLE)0;
	m_hClose	= (HANDLE)0;
}

CRunObject::~CRunObject()
{
	Close();
}

BOOL CRunObject::Init( void )
{
	if( LoadAIScript() == FALSE )
		return FALSE;

// 
	m_hClose	= CreateEvent( NULL, FALSE, FALSE, NULL );
	DWORD dwThreadId;
	m_hRunObject	= chBEGINTHREADEX( NULL, 0, _Run, (LPVOID)this, 0, &dwThreadId ); 

	if( !g_DPSrvr.StartServer( (u_short)( g_uKey + PN_WORLDSRVR ), TRUE ) )
	{
		OutputDebugString( "Can't start server." );
		return FALSE;
	}
	if( !g_DPCoreClient.Run( g_szCoreAddr, PN_CORESRVR + 0, g_uKey ) )
	{
		OutputDebugString( "Can't connect to core server." );
		return FALSE;
	}
	if( !g_dpDBClient.ConnectToServer( g_szDBAddr, PN_DBSRVR_1, TRUE ) )
	{
		OutputDebugString( "Can't connect to database server." );
		return FALSE;
	}
	if ( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) )
		g_dpDBClient.SendQueryGuildBank();
	if( g_eLocal.GetState( EVE_WORMON ) )
		g_dpDBClient.SendQueryGuildQuest();

	if( g_eLocal.GetState( EVE_RAINBOWRACE ) )
		g_dpDBClient.SendRainbowRaceReqLoad();

#ifdef __INVALID_LOGIN_0320
	g_dpDBClient.CalluspXXXMultiServer( g_uIdofMulti, NULL );
#else	// __INVALID_LOGIN_0320
	g_dpDBClient.CalluspXXXMultiServer( g_uIdofMulti, 0 );
#endif	// __INVALID_LOGIN_0320
	if( !CXMasEvent::GetInstance()->LoadScript( "spevent.txt" ) )
	{
		OutputDebugString( "can't read spevent.txt" );
		return FALSE;
	}
	if( g_eLocal.GetState( EVE_SCHOOL ) )
	{
#ifdef __IDC
		if( !CEveSchool::GetInstance()->LoadPos( "..\\script\\school.txt" ) )	//
#else	// __IDC
		if( !CEveSchool::GetInstance()->LoadPos( "school.txt" ) )
#endif	// __IDC
		{
			OutputDebugString( "school.txt not found" );
			return FALSE;
		}
	}
	if( g_eLocal.GetState( EVE_GUILDCOMBAT ) && !g_GuildCombatMng.LoadScript( "GuildCombat.txt" ) )
	{
		OutputDebugString( "GuildCombat.txt not found" );
		return FALSE;
	}

	return TRUE;
}

void CRunObject::Close( void )
{
	CLOSE_THREAD( m_hRunObject, m_hClose );
}

u_int CRunObject::_Run( LPVOID pParam )
{
	CRunObject* pRunObject	= (CRunObject*)pParam;
	pRunObject->Run();
	return 0;
}

#ifdef __PROFILE_RUN
	#define PROFILE_RUN( a ) \
			 dwCur = timeGetTime(); \
			 a = (float)( dwCur - dwOld ); \
			 dwOld = dwCur;
#else
	#define PROFILE_RUN( a ) ((void)0);
#endif

void CRunObject::Run( void )
{
	HANDLE hHeartbeat;
	if( !( hHeartbeat = CreateEvent( NULL, FALSE, FALSE, g_sHeartbeat ) ) )
		hHeartbeat	= OpenEvent( EVENT_MODIFY_STATE, FALSE, g_sHeartbeat );
	if( NULL == hHeartbeat )
		Error( "MAPI_E_NOT_ENOUGH_RESOURCES" );
	DWORD dwTickHearbeat = timeGetTime() + SEC( 2 );

	CTimeout	timeoutObject( 67, 0 );
	CTimeout	timeoutReadAgent( 67, 20 );
	CTimeout	timeoutReadCore( 67, 40 );
	CTimeout	timeoutReadTrans( 67, 50 );
	CTimeout	timeoutReadAccount( 67, 60 );
	CTimeout	timeoutRespawn( 1000, 200 );
	CTimeout	timeout( 1000, 600 );
#ifdef __EVENT_1101
	CTimeout	timeoutCallTheRoll( MIN( 1 ), 70 );
#endif	// __EVENT_1101

	DWORD dwCur, dwOld;

	while( WaitForSingleObject( m_hClose, 1 ) != WAIT_OBJECT_0 )
	{
		{
			_PROFILE( "CRunObject::Run()" );
			g_tmCurrent		= timeGetTime();
			g_tCurrent	= time( NULL );
			
			dwCur = g_tmCurrent;
			dwOld = dwCur;

			if( timeoutObject.TimeoutReset( g_tmCurrent ) )
			{
				_PROFILE( "CWorldMng::Process()" );
				g_WorldMng.Process();
				if( g_tmCurrent > dwTickHearbeat )
				{
					SetEvent( hHeartbeat );
					dwTickHearbeat = g_tmCurrent + SEC( 2 );
				}
			}
			
			PROFILE_RUN( r1 );

			if( timeoutReadAgent.TimeoutReset( g_tmCurrent ) )
			{
				_PROFILE( "g_DPSrvr.ReceiveMessage()" );
				g_DPSrvr.ReceiveMessage();
			}

			PROFILE_RUN( r2 );

			if( timeoutReadCore.TimeoutReset( g_tmCurrent ) )
			{
				_PROFILE( "g_DPCoreClient.ReceiveMessage()" );
				g_DPCoreClient.ReceiveMessage();
			}

			PROFILE_RUN( r3 );

			if( timeoutReadTrans.TimeoutReset( g_tmCurrent ) )
			{
				_PROFILE( "g_dpDBClient.ReceiveMessage()" );
				g_dpDBClient.ReceiveMessage();
			}

			PROFILE_RUN( r4 );
		
		#ifdef __EVENTLUA_KEEPCONNECT
			if( timeoutReadAccount.TimeoutReset( g_tmCurrent ) )
				prj.m_EventLua.KeepConnectEventProcess();
		#endif // __EVENTLUA_KEEPCONNECT
			PROFILE_RUN( r5 );

			if( timeoutRespawn.TimeoutReset( g_tmCurrent ) )
			{
				_PROFILE( "g_WorldMng.Respawn()" );
				u_long uRespawned = g_WorldMng.Respawn();
//				if( uRespawned > 0 )
				g_DisplayedInfo.SetLogInfo( LOGTYPE_RESPAWN, "Respawn:%d Object:%d", uRespawned, g_WorldMng.GetObjCount() );
			}

			PROFILE_RUN( r6 );
			

			if( timeout.TimeoutReset( g_tmCurrent ) )
			{
				_PROFILE( "1SEC_Process" );
				g_GameTimer.Compute();
				CGuildQuestProcessor::GetInstance()->Process();
				CPartyQuestProcessor::GetInstance()->Process();

				PROFILE_RUN( r7 );

				if( g_eLocal.GetState( EVE_GUILDCOMBAT ) )
					g_GuildCombatMng.Process();
				if( g_eLocal.GetState( EVE_GUILDCOMBAT1TO1 ) )
					g_GuildCombat1to1Mng.Process();

				if( g_eLocal.GetState( EVE_SECRETROOM ) )
				{
					_PROFILE( "CSecretRoomMng::Process()" );
					CSecretRoomMng::GetInstance()->Process();
				}
				if( g_eLocal.GetState( EVE_RAINBOWRACE ) )
				{
					_PROFILE( "CRainbowRaceMng::Process()" );
					CRainbowRaceMng::GetInstance()->Process();
				}
				{
					_PROFILE( "CPCBang::ProcessPCBang()" );
					CPCBang::GetInstance()->ProcessPCBang();
				}
				{
					CInstanceDungeonParty::GetInstance()->Process();
				}

	#ifdef __QUIZ
				{
					if( CQuiz::GetInstance()->IsAutoMode() && CQuiz::GetInstance()->IsRun() )
						CQuiz::GetInstance()->Process();
				}
	#endif // __QUIZ

				PROFILE_RUN( r8 );
			
			
				if( g_eLocal.GetState( ENABLE_GUILD_INVENTORY ) )
					g_GuildMng.Process();

				PROFILE_RUN( r9 );

				if( g_eLocal.GetState( EVE_GUILDWAR ) )
					g_GuildWarMng.Process();

				g_PartyMng.PartyMapInfo();

				PROFILE_RUN( r10 );

				// ���� �̺�Ʈ ���� �ʴ� 1ȸ ƽ
				CRangdaController::Instance()->OnTimer();
	#ifdef __EVENTLUA_SPAWN
				prj.m_EventLua.EventSpawnProcess();
	#endif // __EVENTLUA_SPAWN

				g_DisplayedInfo.UpdateConnectionState({
					.database = g_dpDBClient.IsAlive(),
					.core = g_DPCoreClient.IsAlive()
					});
			}

#ifdef __EVENT_1101
			if( timeoutCallTheRoll.TimeoutReset( g_tmCurrent ) )	// 1�д� 
			{
				CEventGeneric::GetInstance()->CallTheRoll();
				CCreateMonster::GetInstance()->ProcessRemoveMonster();
			}
	#endif	// __EVENT_1101

				PROFILE_RUN( r11 );

	#ifdef __EVENT_0117
			CEventGeneric::GetInstance()->Spawn();
	#endif	// __EVENT_0117

			PROFILE_RUN( r12 );
		}
		
		LogPerformance( timeGetTime() );
	
	}
	CLOSE_HANDLE( m_hClose );
	CLOSE_HANDLE( hHeartbeat );
}

CRunObject*	CRunObject::GetInstance( void )
{
	static CRunObject	sRunObject;
	return &sRunObject;
}

void CRunObject::ToggleProfiling()
{
	g_bProfiling = !g_bProfiling; 
}
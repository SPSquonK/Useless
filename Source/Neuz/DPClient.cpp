#include "stdafx.h"
#include "defineText.h"
#include "defineSound.h"

#include "defineItem.h"
#include "defineSkill.h"

#include "appdefine.h"
#include "dploginclient.h"
#include "dpCertified.h"
#include "dpclient.h"
#include "MsgHdr.h"
#include "dialogmsg.h"
#include "misc.h"
#include "defineObj.h" 
#include "WndCharacter.h"
#include "WndGuild.h"
#include "wndmessenger.h"
#include "WndSkillTree.h"
#include "WndQuest.h" 
#include "WndPost.h"
#include "mover.h"
#include "Ship.h"
#include "Sfx.h"
#include "RunFuncScript.h"

#include "wndvendor.h"
#include "..\_Network\ErrorCode.h"
#include "wndchangeface.h"
#include "definequest.h"
#include "webbox.h"

#ifdef __AZRIA_1023
#include "ticket.h"
#endif	// __AZRIA_1023

#ifdef __EVE_MINIGAME
#include "WndMiniGame.h"
#endif //__EVE_MINIGAME

#include "path.h"

#include "WndSummonFriend.h"

#include "tools.h"

#include "eveschool.h"
#include "CommonCtrl.h"
#include "yUtil.h"
#include "party.h"
#include "WndParty.h"
#include "WndDuelConfirm.h"
#include "Environment.h"

#include "clord.h"

#include "RainbowRace.h"
#include "PCBang.h"
#include "guild.h"
#include "guildwar.h"
#include "Wndguildvote.h"
#include "eveschool.h"
#include "Chatting.h"

#include "WndFriendConfirm.h"
#include "WndCommItem.h"
#include "Network.h"
#include "WndSelectVillage.h"
#include "WndSealChar.h"

#include "post.h"
#include "spevent.h"

#include "playerdata.h"

#include "WndBagEx.h"

#include "SecretRoom.h"

#include "Tax.h"

#include "RainbowRace.h"

#include "Housing.h"
#include "WndHousing.h"

#include "honor.h"
#include "couplehelper.h"

#ifdef __QUIZ
#include "Quiz.h"
#endif // __QUIZ

#include "GuildHouse.h"
#include "WndGuildHouse.h"

#include "CampusHelper.h"

BOOL	s_f	= FALSE;
BOOL	fJoin	= FALSE;

CDPClient::CDPClient()
{
	m_lError = 0; 
	m_dwReturnScroll = 0;
	m_bEventTextColor = TRUE;

	ON_MSG( PACKETTYPE_JOIN_WorldNeuz, &CDPClient::OnJoin );
	ON_MSG( PACKETTYPE_SNAPSHOT, &CDPClient::OnSnapshot );
	ON_MSG( PACKETTYPE_ERROR, &CDPClient::OnError );
	ON_MSG( PACKETTYPE_WHISPER, &CDPClient::OnWhisper );
	ON_MSG( PACKETTYPE_SAY, &CDPClient::OnSay );
	ON_MSG( PACKETTYPE_GMSAY, &CDPClient::OnGMSay );
	ON_MSG( PACKETTYPE_GETPLAYERADDR, &CDPClient::OnGetPlayerAddr );
	ON_MSG( PACKETTYPE_GETPLAYERCOUNT, &CDPClient::OnGetPlayerCount );
	ON_MSG( PACKETTYPE_GETCOREPLAYER, &CDPClient::OnGetCorePlayer );
	ON_MSG( PACKETTYPE_SYSTEM, &CDPClient::OnSystem );
	ON_MSG( PACKETTYPE_CAPTION, &CDPClient::OnCaption );
	
	ON_MSG( PACKETTYPE_DEFINEDTEXT, &CDPClient::OnDefinedText );
	ON_MSG( PACKETTYPE_GET_CLOCK, &CDPClient::OnGetClock );
	ON_MSG( PACKETTYPE_ADDFRIENDJOIN, &CDPClient::OnFriendJoin );
	ON_MSG( PACKETTYPE_ADDFRIENDLOGOUT, &CDPClient::OnFriendLogOut );
	ON_MSG( PACKETTYPE_FRIENDNOINTERCEPT, &CDPClient::OnFriendNoIntercept );
	ON_MSG( PACKETTYPE_FRIENDINTERCEPTSTATE, &CDPClient::OnFriendIntercept );
	ON_MSG( PACKETTYPE_GETFRIENDSTATE, &CDPClient::OnGetFriendState );
	ON_MSG( PACKETTYPE_SETFRIENDSTATE, &CDPClient::OnSetFriendState );
	ON_MSG( PACKETTYPE_GAMERATE, &CDPClient::OnGameRate );
	ON_MSG( PACKETTYPE_ERRORPARTY, &CDPClient::OnErrorParty );
	ON_MSG( PACKETTYPE_GUILD, &CDPClient::OnGuild );
	ON_MSG( PACKETTYPE_ADD_GUILD_MEMBER, &CDPClient::OnAddGuildMember );
	ON_MSG( PACKETTYPE_REMOVE_GUILD_MEMBER, &CDPClient::OnRemoveGuildMember );
	ON_MSG( PACKETTYPE_GUILD_CHAT, &CDPClient::OnGuildChat );
	ON_MSG( PACKETTYPE_GUILD_MEMBER_LEVEL, &CDPClient::OnGuildMemberLv );
	ON_MSG( PACKETTYPE_GUILD_CLASS, &CDPClient::OnGuildClass );
	ON_MSG( PACKETTYPE_GUILD_NICKNAME, &CDPClient::OnGuildNickName );
	ON_MSG( PACKETTYPE_CHG_MASTER, &CDPClient::OnChgMaster );
	ON_MSG( PACKETTYPE_GUILD_GAMELOGIN, &CDPClient::OnGuildMemberLogin );
	ON_MSG( PACKETTYPE_GUILD_GAMEJOIN, &CDPClient::OnGuldMyGameJoin );
	ON_MSG( PACKETTYPE_GUILD_SETNAME, &CDPClient::OnGuildSetName );
	ON_MSG( PACKETTYPE_GUILD_ERROR, &CDPClient::OnGuildError );
	ON_MSG( PACKETTYPE_DECL_GUILD_WAR, &CDPClient::OnDeclWar );
	ON_MSG( PACKETTYPE_ACPT_GUILD_WAR, &CDPClient::OnAcptWar );
	ON_MSG( PACKETTYPE_WAR_DEAD, &CDPClient::OnWarDead );
	ON_MSG( PACKETTYPE_WAR_END, &CDPClient::OnWarEnd );
	ON_MSG( PACKETTYPE_SURRENDER, &CDPClient::OnSurrender );
	ON_MSG( PACKETTYPE_QUERY_TRUCE, &CDPClient::OnQueryTruce );	
	ON_MSG( PACKETTYPE_REQUEST_GUILD_RANKING, &CDPClient::OnGuildRank );
	ON_MSG( PACKETTYPE_ADDFRIENDNAMEREQEST, &CDPClient::OnAddFriendNameReqest );
	ON_MSG( PACKETTYPE_ADDFRIENDNAMENOTFOUND, &CDPClient::OnAddFriendNotConnect );
	ON_MSG( PACKETTYPE_REMOVEFRIENDSTATE, &CDPClient::OnRemoveFriendState );
	ON_MSG( PACKETTYPE_BLOCK, &CDPClient::OnBlock );
	ON_MSG( PACKETTYPE_INSERTTAG_RESULT, &CDPClient::OnTagResult );
	ON_MSG( PACKETTYPE_KEEP_ALIVE, &CDPClient::OnKeepAlive );
#ifdef __TRAFIC_1218
	ON_MSG( PACKETTYPE_TRAFIC_LOG, &CDPClient::OnTraficLog );
#endif	// __TRAFIC_1218

#ifdef __EVE_NEWYEAR
	ON_MSG( PACKETTYPE_NEWYEAR, &CDPClient::OnNewYear );
#endif	// __EVE_NEWYEAR

	ON_MSG( PACKETTYPE_WN_WANTED_LIST, &CDPClient::OnWantedList );
	ON_MSG( PACKETTYPE_WN_WANTED_NAME, &CDPClient::OnWantedName );

	ON_MSG( PACKETTYPE_QUERYSETPLAYERNAME, &CDPClient::OnSetPlayerName );
	ON_MSG( PACKETTYPE_SNOOP, &CDPClient::OnSnoop );

	ON_MSG( PACKETTYPE_UPDATE_PLAYER_DATA, &CDPClient::OnUpdatePlayerData );
	ON_MSG( PACKETTYPE_LOGOUT, &CDPClient::OnLogout );

#ifdef __QUIZ
	ON_MSG( PACKETTYPE_QUIZ_NOTICE, &CDPClient::OnQuizSystemMessage );
#endif // __QUIZ

	memset( (void*)&m_ss, 0, sizeof(SNAPSHOT) );
	
	m_nMaxLoginGuild = 0;
	memset( m_uLoginPlayerIdGuild, 0, sizeof(m_uLoginPlayerIdGuild) );
	memset( m_uLoginGuildMulti, 0, sizeof(m_uLoginGuildMulti) );

	m_pDump	= NULL;
	m_nDumpSize		= 0;

#ifdef __EVE_NEWYEAR
	m_nCountdown	= 0;
#endif	// __EVE_NEWYEAR
}

CDPClient::~CDPClient()
{ 

}

LONG CDPClient::GetNetError()
{
	return m_lError;
}

void CDPClient::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				#ifdef __INFO_SOCKLIB0516
				{
					LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
					m_lError = lpDestroyPlayer->dwFlags;
				}
				#endif
				CNetwork::GetInstance().OnEvent( CACHE_DISCONNECT );

				m_fConn		= FALSE;
				if( s_f )
					g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0017) ) );
  				    //g_WndMng.OpenMessageBox( _T( "비정상적인 클럭이 감지되어 접속 해제 되었습니다. 고객님의 PC를 점검해 주십시오.  문의사항은 홈페이지 E-mail 문의를 이용해 주시기 바랍니다." ) );
//				else
//					g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0023) ) );
				    //g_WndMng.OpenMessageBox( _T( "서버로부터 접속이 해제되었습니다." ) );
				s_f		= FALSE;

				if( g_pPlayer && g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
				{
					CTime time;
					time = CTime::GetCurrentTime();
					FILEOUT( "DisconnectLog.txt", 
						"Day %d, Hour %d, Min %d, Sec %d \n", 
						time.GetDay() ,
						time.GetHour(),
						time.GetMinute(),
						time.GetSecond()
						);
				}
				break;
			}
	}
}

void CDPClient::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
#ifdef __TRAFIC_1218
	m_traficLog.AddTotal( dwMsgSize );
#endif	// __TRAFIC_1218

	m_pDump	= (BYTE*)lpMsg;
	m_nDumpSize	= dwMsgSize;

#ifdef __J
	CTime t	= CTime::GetCurrentTime();
	if( g_timeProcessed.GetSecond() != t.GetSecond() )
	{
		g_lBytesProcessed	= 0;
		g_timeProcessed		= t;
	}
	g_lBytesProcessed	+=	dwMsgSize;
#endif	// __J

	CAr ar( (LPBYTE)lpMsg, dwMsgSize );
	DWORD dw; ar >> dw;
	
	if( Handle(ar, dw) ) {
	}
	else {
/*
		switch( dw ) {
			default:
				TRACE( "Handler not found(%08x)\n", dw );
				break;
		}
*/
	}
	m_pDump	= NULL;
	m_nDumpSize	= 0;
}

BOOL CDPClient::Connect( LPSTR lpszAddr, USHORT uPort )
{
	if( g_dpLoginClient.IsConnected() && CreateDPObject( TRUE /*Single threaded*/ ) == TRUE )
	{
		InitConnection( (LPVOID)lpszAddr, uPort );
#ifdef __CRC
		return ( m_fConn = JoinSession( NULL, CSock::crcWrite ) );
#else	// __CRC
		return ( m_fConn = JoinSession( NULL ) );
#endif	// __CRC
	}
	return ( m_fConn = FALSE );
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Receiver
void CDPClient::OnSnapshot( CAr & ar )
{
	OBJID objidPlayer;
	short cb;

	ar >> objidPlayer >> cb;
	while( cb-- )
	{
		const u_long startedAt = ar.GetOffset();
		const auto [packetSize, objid, hdr] = ar.Extract<u_long, OBJID, WORD>();

#ifdef __TRAFIC_1218
		m_traficLog.Add( (BYTE)( hdr ) );
#endif	// __TRAFIC_1218
//		TRACE( "hdr = %04x, cb = %d\n", hdr, cb );

		switch( hdr )
		{
			case SNAPSHOTTYPE_MOVERMOVED:	OnMoverMoved( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERBEHAVIOR:		OnMoverBehavior( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERMOVED2:	OnMoverMoved2( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERBEHAVIOR2:		OnMoverBehavior2( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERANGLE:		OnMoverAngle( objid, ar );	break;
			case SNAPSHOTTYPE_CREATESFXOBJ:		OnCreateSfxObj( objid, ar );	break;
			case SNAPSHOTTYPE_CREATESFXALLOW:	OnCreateSfxAllow( objid, ar );	break;
			case SNAPSHOTTYPE_DEFINEDTEXT:	OnDefinedText( ar );	break;
			case SNAPSHOTTYPE_CHATTEXT:	OnChatText( ar );	break;
			case SNAPSHOTTYPE_DEFINEDTEXT1: OnDefinedText1( ar );	break;
			case SNAPSHOTTYPE_GAMETIMER:	OnGameTimer( ar );	break;
			case SNAPSHOTTYPE_ADDGAMEJOIN:	OnGameJoin( ar ); break;
			case SNAPSHOTTYPE_TASKBAR:		OnTaskBar( ar ); break;
			case SNAPSHOTTYPE_ERRORPARTY:	OnErrorParty( ar ); break;
			case SNAPSHOTTYPE_PARTYMEMBER:	OnAddPartyMember( ar ); break;
			case SNAPSHOTTYPE_PARTYREQEST: OnPartyRequest( ar ); break;
			case SNAPSHOTTYPE_PARTYREQESTCANCEL: OnPartyRequestCancel( ar ); break;
			case SNAPSHOTTYPE_PARTYEXP: OnPartyExpLevel( ar ); break;
			case SNAPSHOTTYPE_PARTYCHANGETROUP: OnPartyChangeTroup( ar ); break;
			case SNAPSHOTTYPE_PARTYCHANGENAME: OnPartyChangeName( ar ); break;
			case SNAPSHOTTYPE_PARTYSKILL_CALL:	OnPartySkillCall( objid, ar ); break;
			case SNAPSHOTTYPE_PARTYSKILL_BLITZ:	OnPartySkillBlitz( ar ); break;
			case SNAPSHOTTYPE_PARTYSKILL_RETREAT:	OnPartySkillRetreat( objid ); break;
			case SNAPSHOTTYPE_PARTYSKILL_SPHERECIRCLE:	OnPartySkillSphereCircle( objid ); break;
			case SNAPSHOTTYPE_SETPARTYMODE:	OnSetPartyMode( ar ); break;
			case SNAPSHOTTYPE_PARTYCHANGEITEMMODE: OnPartyChangeItemMode( ar ); break;
			case SNAPSHOTTYPE_PARTYCHANGEEXPMODE: OnPartyChangeExpMode( ar ); break;


			case SNAPSHOTTYPE_ENVIRONMENTALL: OnEnvironmentSetting( ar ); break;
			case SNAPSHOTTYPE_ENVIRONMENT: OnEnvironmentEffect( ar ); break;


			case SNAPSHOTTYPE_PARTYCHAT: OnPartyChat( ar ); break;
			case SNAPSHOTTYPE_PVENDOR_OPEN:	OnPVendorOpen( objid, ar );	break;
			case SNAPSHOTTYPE_PVENDOR_CLOSE:		OnPVendorClose( objid, ar );	break;
			case SNAPSHOTTYPE_REGISTER_PVENDOR_ITEM:	OnRegisterPVendorItem( objid, ar );	break;
			case SNAPSHOTTYPE_DOAPPLYUSESKILL:	OnDoApplySkill( objid, ar ); break;
			case SNAPSHOTTYPE_COMMONSKILL:	OnCommonSkill( objid, ar ); break;
			case SNAPSHOTTYPE_UNREGISTER_PVENDOR_ITEM:	OnUnregisterPVendorItem( objid, ar );	break;
			case SNAPSHOTTYPE_PVENDOR_ITEM:				OnPVendorItem( objid, ar );		break;
			case SNAPSHOTTYPE_PVENDOR_ITEM_NUM:			OnPVendorItemNum( objid, ar );	break;
			case SNAPSHOTTYPE_SET_HAIR:					OnSetHair( objid, ar );	break;
			case SNAPSHOTTYPE_ADDPARTYNAME:				OnMyPartyName( ar ); break;
			case SNAPSHOTTYPE_SET_PARTY_MEMBER_PARAM:	OnSetPartyMemberParam( ar );	break;
			case SNAPSHOTTYPE_ADDFRIENDGAMEJOIN: OnFriendGameJoin( ar ); break;
			case SNAPSHOTTYPE_ADDFRIEND: OnAddFriend( ar ); break;
			case SNAPSHOTTYPE_ADDFRIENDREQEST: OnAddFriendReqest( ar ); break;
			case SNAPSHOTTYPE_ADDFRIENDCANCEL: OnAddFriendCancel( ar ); break;
			case SNAPSHOTTYPE_REMOVEFRIEND:	OnRemoveFriend( ar ); break;
			case SNAPSHOTTYPE_ADDFRIENDERROR: OnAddFriendError( ar ); break;
			case SNAPSHOTTYPE_SETSKILLSTATE:	OnSetSkillState( ar ); break;
			case SNAPSHOTTYPE_ADDPARTYCHANGELEADER: OnPartyChangeLeader( ar ); break;
			case SNAPSHOTTYPE_FLYFF_EVENT:	OnFlyffEvent( ar );	break;
			case SNAPSHOTTYPE_SET_LOCAL_EVENT:	OnSetLocalEvent( ar );	break;
			case SNAPSHOTTYPE_GAMERATE:	OnGameRate( ar ); break;
			case SNAPSHOTTYPE_EVENTLUA_DESC: OnEventLuaDesc( ar ); break;
			case SNAPSHOTTYPE_REMOVE_ATTRIBUTE: OnRemoveAttributeResult( ar ); break;
			case SNAPSHOTTYPE_MOTION_ARRIVE: OnMotionArrive( objid, ar ); break;

			case SNAPSHOTTYPE_MONSTERPROP:	OnMonsterProp( ar ); break;
			case SNAPSHOTTYPE_GMCHAT:		OnGMChat( ar ); break;

			case SNAPSHOTTYPE_CHANGEFACE: OnChangeFace( ar ); break;
			case SNAPSHOTTYPE_DEFINEDCAPTION: OnDefinedCaption( ar ); break;
			case SNAPSHOTTYPE_SM_MODE:	OnSMMode( ar ); break;
			case SNAPSHOTTYPE_SM_MODE_ALL:	OnSMModeAll( ar ); break;
			case SNAPSHOTTYPE_RESISTSMMODE: OnResistSMMode( ar ); break;
			case SNAPSHOTTYPE_COMMERCIALELEM: OnCommercialElem( ar ); break;
			case SNAPSHOTTYPE_MOVERFOCUS: OnMoverFocus( ar ); break;
			case SNAPSHOTTYPE_PARTYMAPINFO: OnPartyMapInfo( ar ); break;
			case SNAPSHOTTYPE_DUELCOUNT: OnDuelCount( ar ); break;
			case SNAPSHOTTYPE_DUELREQUEST: OnDuelRequest( ar ); break;
			case SNAPSHOTTYPE_DUELSTART: OnDuelStart( ar ); break;
			case SNAPSHOTTYPE_DUELNO: OnDuelNo( ar ); break;
			case SNAPSHOTTYPE_DUELCANCEL: OnDuelCancel( ar ); break;
			case SNAPSHOTTYPE_DUELPARTYREQUEST: OnDuelPartyRequest( ar ); break;
			case SNAPSHOTTYPE_DUELPARTYSTART:	OnDuelPartyStart( ar ); break;
			case SNAPSHOTTYPE_DUELPARTYNO:		OnDuelPartyNo( ar ); break;
			case SNAPSHOTTYPE_DUELPARTYCANCEL:	OnDuelPartyCancel( ar ); break;
			case SNAPSHOTTYPE_DUELPARTYRESULT:	OnDuelPartyResult( ar ); break;
			case SNAPSHOTTYPE_MOVERCORR:	OnMoverCorr( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERCORR2:	OnMoverCorr2( objid, ar );	break;
			case SNAPSHOTTYPE_QUERYGETPOS:	OnQueryGetPos( ar );	break;
			case SNAPSHOTTYPE_GETPOS:	OnGetPos( objid, ar );	break;
			case SNAPSHOTTYPE_QUERYGETDESTOBJ:	OnQueryGetDestObj( ar );	break;
			case SNAPSHOTTYPE_GETDESTOBJ:	OnGetDestObj( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERDEATH:	OnMoverDeath( objid, ar );	break;
			case SNAPSHOTTYPE_ADD_OBJ:	OnAddObj( objid, ar );	break;
			case SNAPSHOTTYPE_DEL_OBJ:	OnRemoveObj( objid );	break;
			case SNAPSHOTTYPE_CHAT:		OnChat( objid, ar );	break;
			case SNAPSHOTTYPE_EVENTMESSAGE:		OnEventMessage( objid, ar ); break;
//			case SNAPSHOTTYPE_ACTMSG:	OnActMsg( objid, ar );	break;
			case SNAPSHOTTYPE_SHIP_ACTMSG:	OnShipActMsg( objid, ar );	break;
			case SNAPSHOTTYPE_DAMAGE:	OnDamage( objid, ar );	break;
			case SNAPSHOTTYPE_CREATEITEM:	OnCreateItem( objid, ar );	break;
			case SNAPSHOTTYPE_MOVEITEM:		OnMoveItem( ar );	break;
			case SNAPSHOTTYPE_DOEQUIP:	OnDoEquip( objid, ar );	break;
			case SNAPSHOTTYPE_TRADE:	OnTrade( objid, ar );	break;
			case SNAPSHOTTYPE_CONFIRMTRADE: OnConfirmTrade( objid, ar ); break;
			case SNAPSHOTTYPE_CONFIRMTRADECANCEL: OnConfirmTradeCancel( objid, ar ); break;
			case SNAPSHOTTYPE_TRADEPUT:		OnTradePut( objid, ar );	break;
			case SNAPSHOTTYPE_TRADEPULL:	OnTradePull( objid, ar );	break;
			case SNAPSHOTTYPE_TRADEPUTGOLD:		OnTradePutGold( objid, ar );	break;
//raiders.2006.11.28
//			case SNAPSHOTTYPE_TRADECLEARGOLD:	OnTradeClearGold( objid );	break;
			case SNAPSHOTTYPE_TRADECANCEL:	OnTradeCancel( objid, ar );		break;
			case SNAPSHOTTYPE_TRADEOK :		OnTradeOk( objid, ar );		break;
			case SNAPSHOTTYPE_TRADECONSENT:		OnTradeConsent();	break;
			case SNAPSHOTTYPE_TRADELASTCONFIRM:	OnTradelastConfirm();	break;
			case SNAPSHOTTYPE_TRADELASTCONFIRMOK: OnTradelastConfirmOk( objid, ar ); break;

			case SNAPSHOTTYPE_SETGUILDQUEST:	OnSetGuildQuest( ar );	break;
			case SNAPSHOTTYPE_REMOVEGUILDQUEST:	OnRemoveGuildQuest( ar );	break;
			case SNAPSHOTTYPE_SETQUEST:	OnSetQuest( objid, ar );		break;

			case SNAPSHOTTYPE_OPENSHOPWND:	OnOpenShopWnd( objid, ar );		break;
			case SNAPSHOTTYPE_PUTITEMBANK:	OnPutItemBank( objid, ar );		break;
			case SNAPSHOTTYPE_GETITEMBANK:	OnGetItemBank( objid, ar );		break;
			case SNAPSHOTTYPE_PUTGOLDBANK:	OnPutGoldBank( objid, ar );		break;
			case SNAPSHOTTYPE_MOVEBANKITEM:	OnMoveBankItem( objid, ar );	break;
			case SNAPSHOTTYPE_UPDATE_BANKITEM: OnUpdateBankItem( objid, ar );	break;
			case SNAPSHOTTYPE_BANKISFULL:	OnErrorBankIsFull( objid, ar ); break;
			case SNAPSHOTTYPE_BANK:	OnBank( objid, ar ); break;
			case SNAPSHOTTYPE_REMOVESKILLINFULENCE: OnRemoveSkillInfluence( objid, ar ); break;
			case SNAPSHOTTYPE_REMOVEALLSKILLINFULENCE: OnRemoveAllSkillInfluence( objid, ar ); break;				
			case SNAPSHOTTYPE_GUILD_BANK_WND:	OnGuildBankWindow( objid, ar ); break;
			case SNAPSHOTTYPE_PUTITEMGUILDBANK:	OnPutItemGuildBank( objid, ar ); break;
			case SNAPSHOTTYPE_GETITEMGUILDBANK: OnGetItemGuildBank( objid, ar ); break;
			case SNAPSHOTTYPE_QUERY_PLAYER_DATA:	OnQueryPlayerData( ar );	break;
				
			case SNAPSHOTTYPE_GUILD_INVITE:	OnGuildInvite( ar );	break;
			case SNAPSHOTTYPE_SET_GUILD:	OnSetGuild( objid, ar );	break;
			case SNAPSHOTTYPE_CREATE_GUILD:	OnCreateGuild( ar );	break;
			case SNAPSHOTTYPE_DESTROY_GUILD:	OnDestroyGuild( ar );	break;
			case SNAPSHOTTYPE_GUILD:			OnGuild( ar );	break;
			case SNAPSHOTTYPE_ALL_GUILDS:		OnAllGuilds( ar );	break;
			case SNAPSHOTTYPE_GUILD_LOGO:		OnGuildLogo( ar ); break;
			case SNAPSHOTTYPE_GUILD_CONTRIBUTION:		OnGuildContribution( ar ); break;
			case SNAPSHOTTYPE_GUILD_NOTICE:		OnGuildNotice( ar ); break;
			case SNAPSHOTTYPE_GUILD_AUTHORITY:	OnGuildAuthority( ar ); break;
			case SNAPSHOTTYPE_GUILD_PENYA:		OnGuildPenya( ar ); break;
			case SNAPSHOTTYPE_GUILD_REAL_PENYA:	OnGuildRealPenya( ar ); break;
			case SNAPSHOTTYPE_GUILD_ADDVOTE:	OnGuildAddVote( ar ); break;
			case SNAPSHOTTYPE_GUILD_MODIFYVOTE: OnGuildModifyVote( ar ); break;
			case SNAPSHOTTYPE_SET_WAR:	OnSetWar( objid, ar );	break;
			case SNAPSHOTTYPE_WAR:	OnWar( ar );	break;
			case SNAPSHOTTYPE_SETNAVIPOINT: OnSetNaviPoint( objid, ar );	break;
			case SNAPSHOTTYPE_REQUEST_GUILDRANK:	OnRequestGuildRank( ar );	break;
			case SNAPSHOTTYPE_VENDOR:	OnVendor( objid, ar );	break;
			case SNAPSHOTTYPE_UPDATE_VENDOR:	OnUpdateVendor( objid, ar );	break;
			case SNAPSHOTTYPE_UPDATE_ITEM_VARIANT:	OnUpdateItemVariant( objid, ar );	break;
			case SNAPSHOTTYPE_POCKET_ATTRIBUTE:	OnPocketAttribute( ar );	break;
			case SNAPSHOTTYPE_POCKET_ADD_ITEM:	OnPocketAddItem( ar );	break;
			case SNAPSHOTTYPE_POCKET_REMOVE_ITEM:	OnPocketRemoveItem( ar );	break;
			case SNAPSHOTTYPE_SETDESTPARAM:		OnSetDestParam( objid, ar );	break;
			case SNAPSHOTTYPE_RESETDESTPARAM:	OnResetDestParam( objid, ar );	break;
			case SNAPSHOTTYPE_SETPOINTPARAM:	OnSetPointParam( objid, ar );	break;
			case SNAPSHOTTYPE_SETSCALE:		OnSetScale( objid, ar );	break;
#ifndef __S_SERVER_UNIFY
			case SNAPSHOTTYPE_DIAG_TEXT:		OnDiagText( ar );	break;
#endif // __S_SERVER_UNIFY
#ifdef __S_SERVER_UNIFY
			case SNAPSHOTTYPE_ALLACTION:		OnAllAction( ar );	break;
#endif // __S_SERVER_UNIFY
			case SNAPSHOTTYPE_TEXT:		OnText( ar );	break;
			case SNAPSHOTTYPE_SETPOS:	OnSetPos( objid, ar );	break;
			case SNAPSHOTTYPE_SETPOSANGLE:	OnSetPosAngle( objid, ar );	break;
			case SNAPSHOTTYPE_REPLACE:	OnReplace( ar );	break;
			case SNAPSHOTTYPE_SETEXPERIENCE:	OnSetExperience( objid, ar );	break;
			case SNAPSHOTTYPE_SETFXP:	OnSetFxp( objid, ar );	break;
			case SNAPSHOTTYPE_SETLEVEL:	OnSetLevel( objid, ar );	break;
			case SNAPSHOTTYPE_SETFLIGHTLEVEL:	OnSetFlightLevel( objid, ar );	break;
			case SNAPSHOTTYPE_DESTPOS:	OnSetDestPos( objid, ar );	break;
			case SNAPSHOTTYPE_DESTANGLE:	OnSetDestAngle( objid, ar );	break;
			case SNAPSHOTTYPE_SETMOVEPATTERN:	OnSetMovePattern( objid, ar );	break;
			case SNAPSHOTTYPE_MELEE_ATTACK:		OnMeleeAttack( objid, ar );	break;
			case SNAPSHOTTYPE_MELEE_ATTACK2:	OnMeleeAttack2( objid, ar );	break;
			case SNAPSHOTTYPE_MAGIC_ATTACK:		OnMagicAttack( objid, ar );	 break;
			case SNAPSHOTTYPE_RANGE_ATTACK:		OnRangeAttack( objid, ar );	break;
			case SNAPSHOTTYPE_SP_ATTACK:		OnAttackSP( objid, ar );	break;
			case SNAPSHOTTYPE_MOVERSETDESTOBJ: OnMoverSetDestObj( objid, ar );	break;
			case SNAPSHOTTYPE_SHOUT:	OnShout( ar );	break;
			case SNAPSHOTTYPE_PLAYMUSIC:	OnPlayMusic( ar );	break;
			case SNAPSHOTTYPE_PLAYSOUND:	OnPlaySound( ar );	break;
			case SNAPSHOTTYPE_REVIVAL:	OnRevival( objid );	break;
			case SNAPSHOTTYPE_REVIVAL_TO_LODESTAR:	OnRevivalLodestar( objid );	break;	
			case SNAPSHOTTYPE_REVIVAL_TO_LODELIGHT:	OnRevivalLodelight( objid );	break;
			case SNAPSHOTTYPE_USESKILL:	OnUseSkill( objid, ar );	break;
			case SNAPSHOTTYPE_SET_GROWTH_LEARNING_POINT:	OnSetGrowthLearningPoint( objid, ar );	break;
			case SNAPSHOTTYPE_SET_JOB_SKILL:		OnSetChangeJob( objid, ar ); break;
			case SNAPSHOTTYPE_SET_NEAR_JOB_SKILL:   OnSetNearChangeJob( objid, ar ); break;
			case SNAPSHOTTYPE_MODIFYMODE:	OnModifyMode( objid, ar );	break;
			case SNAPSHOTTYPE_STATEMODE: 	OnStateMode( objid, ar );	break;
			case SNAPSHOTTYPE_RETURNSAY:	OnReturnSay( objid, ar ); break;
			case SNAPSHOTTYPE_CLEAR_USESKILL:	OnClearUseSkill( objid );	break;
//			case SNAPSHOTTYPE_CRIME:	OnCrime( objid );	break;
			case SNAPSHOTTYPE_SETFAME:	OnSetFame( objid, ar );	break;
			case SNAPSHOTTYPE_SETFUEL:	OnSetFuel( objid, ar );	break;
			case SNAPSHOTTYPE_RESURRECTION:		OnResurrection( objid );	break;
			case SNAPSHOTTYPE_TRADEPUTERROR:	OnTradePutError( objid, ar );	break;	
			case SNAPSHOTTYPE_CORRREQ:		OnCorrReq( objid, ar );	break;
			case SNAPSHOTTYPE_DISGUISE:		OnDisguise( objid, ar );	break;
			case SNAPSHOTTYPE_NODISGUISE:		OnNoDisguise( objid, ar );	break;	
			case SNAPSHOTTYPE_SETSTATE:		OnSetState( ar ); break;
			case SNAPSHOTTYPE_CMDSETSKILLLEVEL:		OnCmdSetSkillLevel( ar ); break;
//			case SNAPSHOTTYPE_DO_COLLECT:		OnDoCollect( objid, ar ); break;
//			case SNAPSHOTTYPE_STOP_COLLECT:		OnStopCollect( objid, ar );	break;
			case SNAPSHOTTYPE_ACTIVESKILL:		OnCreateSkillEffect( objid, ar );	break;
			case SNAPSHOTTYPE_SETSTUN:			OnSetStun( objid, ar );	break;
			case SNAPSHOTTYPE_SENDACTMSG:		OnSendActMsg( objid, ar );	break;
			case SNAPSHOTTYPE_PUSHPOWER:		OnPushPower( objid, ar );	break;
			case SNAPSHOTTYPE_DO_ESCAPE:		OnEscape( objid, ar );	break;
			case SNAPSHOTTYPE_SETACTIONPOINT:	OnSetActionPoint( objid, ar );	break;
			case SNAPSHOTTYPE_ENDSKILLQUEUE:	OnEndSkillQueue( objid );	break;
			case SNAPSHOTTYPE_SETTARGET:	OnSetTarget( objid, ar );	break;
			case SNAPSHOTTYPE_EXPBOXINFO:	OnSExpBoxInfo( objid, ar );	break;
			case SNAPSHOTTYPE_EXPBOXCOLLTIME:	OnSExpBoxCoolTime( objid, ar );	break;
				
				
			case SNAPSHOTTYPE_MOTION:	OnMotion( objid, ar );	break;
			case SNAPSHOTTYPE_RUNSCRIPTFUNC:	OnRunScriptFunc( objid, ar );	break;
			case SNAPSHOTTYPE_TAG:				OnTag( objid, ar ); break;
			case SNAPSHOTTYPE_SCHOOL_REPORT:	OnSchoolReport( ar );	break;
			case SNAPSHOTTYPE_SEX_CHANGE:	OnSexChange( objid, ar );	break;		
			case SNAPSHOTTYPE_CHATTING: OnChatting( objid, ar ); break;
			case SNAPSHOTTYPE_REMOVEQUEST: OnRemoveQuest( ar ); break;
			case SNAPSHOTTYPE_INITSKILLPOINT: OnInitSkillPoint( ar ); break;
			case SNAPSHOTTYPE_DOUSESKILLPOINT: OnDoUseSkillPoint( ar ); break;

			case SNAPSHOTTYPE_COMMONPlACE: OnCommonPlace( objid, ar ); break;
			case SNAPSHOTTYPE_RESETBUFFSKILL:	OnResetBuffSkill( objid, ar );	break;
			case SNAPSHOTTYPE_RESURRECTION_MESSAGE:	OnResurrectionMessage();	break;
			case SNAPSHOTTYPE_SETDUEL:	OnSetDuel( objid, ar );	break;
			case SNAPSHOTTYPE_PK_RELATION: OnPKRelation( objid, ar ); break;
			case SNAPSHOTTYPE_WANTED_INFO: OnWantedInfo( ar ); break;

			case SNAPSHOTTYPE_WORLDMSG: OnWorldMsg( objid, ar );	break;
			case SNAPSHOTTYPE_SNOOP:	OnSnoop( ar );	break;
			case SNAPSHOTTYPE_SETCHEERPARAM:	OnSetCheerParam( objid, ar );	break;
			case SNAPSHOTTYPE_QUERYEQUIP:	OnQueryEquip( objid, ar );	break;
			case SNAPSHOTTYPE_RETURNSCORLL: OnReturnScrollACK( ar ); break;
			case SNAPSHOTTYPE_GUILDCOMBAT: OnGuildCombat( ar ); break;
			case SNAPSHOTTYPE_QUEST_TEXT_TIME: OnQuestTextTime( ar ); break;
			case SNAPSHOTTYPE_EXPBOXCOLLTIMECANCEL: OnCtrlCoolTimeCancel( ar ); break;
			case SNAPSHOTTYPE_POSTMAIL:		OnPostMail( ar );	break;
			case SNAPSHOTTYPE_REMOVEMAIL:	OnRemoveMail( ar );		break;
			case SNAPSHOTTYPE_QUERYMAILBOX:		OnMailBox( ar );	break;
			case SNAPSHOTTYPE_QUERYMAILBOX_REQ:		OnMailBoxReq( ar );	break;
			case SNAPSHOTTYPE_SUMMON:	OnSummon( ar ); break;
			case SNAPSHOTTYPE_REMOVE_GUILD_BANK_ITEM:	OnRemoveGuildBankItem( ar );	break;
#ifdef __EVENT_1101
			case SNAPSHOTTYPE_CALLTHEROLL:	OnCallTheRoll( ar );	break;
#endif	// __EVENT_1101

			case SNAPSHOTTYPE_ANGEL:	OnAngel( objid, ar );	break;
#ifdef __EVE_MINIGAME
			case SNAPSHOTTYPE_MINIGAME:		OnMiniGame( objid, ar ); break;
#endif // __EVE_MINIGAME

			case SNAPSHOTTYPE_ULTIMATE:		OnUltimateWeapon( objid, ar ); break;

#ifdef __TRADESYS
			case SNAPSHOTTYPE_EXCHANGE:		OnExchange( objid, ar ); break;
#endif // __TRADESYS

			case SNAPSHOTTYPE_PET_CALL:		OnPetCall( objid, ar );	break;
			case SNAPSHOTTYPE_PET_RELEASE:	OnPetRelease( objid, ar );	break;
			case SNAPSHOTTYPE_PET_LEVELUP:	OnPetLevelup( objid, ar );	break;
			case SNAPSHOTTYPE_PET_SET_EXP:	OnPetSetExp( objid, ar );	break;
			case SNAPSHOTTYPE_PET:	OnPet( objid, ar );		break;
			case SNAPSHOTTYPE_PET_STATE:	OnPetState( objid, ar );	break;
			case SNAPSHOTTYPE_PET_FEED:		OnPetFeed( objid, ar );		break;
			case SNAPSHOTTYPE_PET_MILL:		OnPetFoodMill( objid, ar );	break;

			case SNAPSHOTTYPE_SET_SPEED_FACTOR:		OnSetSpeedFactor( objid, ar );	break;
			case SNAPSHOTTYPE_LEGENDSKILLUP_RESULT:	OnLegendSkillUp( ar ); break;

			case SNAPSHOTTYPE_START_COLLECTING:		OnStartCollecting( objid );	break;
			case SNAPSHOTTYPE_STOP_COLLECTING:	OnStopCollecting( objid );	break;
			case SNAPSHOTTYPE_RESTART_COLLECTING:	OnRestartCollecting( objid, ar );	break;

#ifdef __JEFF_11
			case SNAPSHOTTYPE_QUE_PETRESURRECTION_RESULT:	OnQuePetResurrectionResult( ar );	break;
#endif	// __JEFF_11

			case SNAPSHOTTYPE_GC1TO1_TENDEROPENWND: OnGC1to1TenderOpenWnd( ar ); break;
			case SNAPSHOTTYPE_GC1TO1_TENDERVIEW: OnGC1to1TenderView( ar ); break;
			case SNAPSHOTTYPE_GC1TO1_NOWSTATE: OnGC1to1NowState( ar ); break;
			case SNAPSHOTTYPE_GC1TO1_MEMBERLINEUPOPENWND: OnGC1to1MemberLineUpOpenWnd( ar ); break;
			case SNAPSHOTTYPE_GC1TO1_WARRESULT: OnGC1to1WarResult( ar ); break;
	
			case SNAPSHOTTYPE_GUILDLOG_VIEW: OnGuildBankLogList( ar ); break;
			case SNAPSHOTTYPE_SEALCHAR_REQ: OnSealChar( ar ); break;
			case SNAPSHOTTYPE_SEALCHARGET_REQ: OnSealCharGet( ar ); break;
			case SNAPSHOTTYPE_REMOVESFXOBJ:		OnRemoveSfxObj( objid, ar );	break;
			case SNAPSHOTTYPE_HONOR_LIST_ACK: OnHonorListAck( ar ); break;
			case SNAPSHOTTYPE_HONOR_CHANGE_ACK: OnHonorChangeAck( objid,ar ); break;
#ifdef __EVENTLUA_COUPON
			case SNAPSHOTTYPE_EVENT_COUPON: OnEventCoupon( ar ); break;
#endif // __EVENTLUA_COUPON

			case SNAPSHOTTYPE_SECRETROOM_MNG_STATE : OnSecretRoomMngState( ar ); break;
			case SNAPSHOTTYPE_SECRETROOM_INFO : OnSecretRoomInfo( ar ); break;
			case SNAPSHOTTYPE_SECRETROOM_TENDEROPENWND : OnSecretRoomTenderOpenWnd( ar ); break;
			case SNAPSHOTTYPE_SECRETROOM_LINEUPOPENWND : OnSecretRoomLineUpOpenWnd( ar ); break;
			case SNAPSHOTTYPE_SECRETROOM_TENDERVIEW : OnSecretRoomTenderView( ar ); break;

			case SNAPSHOTTYPE_TAX_ALLINFO : OnTaxInfo( ar ); break;
			case SNAPSHOTTYPE_TAX_SETTAXRATE_OPENWND : OnTaxSetTaxRateOpenWnd( ar ); break;
			// 군주 입찰 결과 갱신
			case SNAPSHOTTYPE_ELECTION_ADD_DEPOSIT:	OnElectionAddDeposit( ar );		break;
			// 공약 설정 결과 갱신
			case SNAPSHOTTYPE_ELECTION_SET_PLEDGE:	OnElectionSetPledge( ar );	break;
			// 투표 결과 갱신
			case SNAPSHOTTYPE_ELECTION_INC_VOTE:	OnElectionIncVote( ar );	break;
			// 입후보 시작 상태로 변경
			case SNAPSHOTTYPE_ELECTION_BEGIN_CANDIDACY:		OnElectionBeginCandidacy( ar );		break;
			// 투표 시작 상태로 변경
			case SNAPSHOTTYPE_ELECTION_BEGIN_VOTE:		OnElectionBeginVote( ar );		break;
			// 투표 종료 상태로 변경
			case SNAPSHOTTYPE_ELECTION_END_VOTE:	OnElectionEndVote( ar );	break;
			// 군주 시스템 정보를 수신하여 복원
			case SNAPSHOTTYPE_LORD:	OnLord( ar );	break;
			// 군주 이벤트 시작 처리
			case SNAPSHOTTYPE_L_EVENT_CREATE:	OnLEventCreate( ar );	break;
			// 군주 이벤트 초기화 처리
			case SNAPSHOTTYPE_L_EVENT_INITIALIZE:	OnLEventInitialize( ar );	break;
			// 군주 스킬 재사용 대기 시간 처리
			case SNAPSHOTTYPE_LORD_SKILL_TICK:	OnLordSkillTick( ar );	break;
			// 군주 이벤트 지속 시간 처리
			case SNAPSHOTTYPE_L_EVENT_TICK:		OnLEventTick( ar );	break;
			// 군주 스킬 사용
			case SNAPSHOTTYPE_LORD_SKILL_USE:	OnLordSkillUse( objid, ar );	break;
			case SNAPSHOTTYPE_SET_TUTORIAL_STATE:	OnTutorialState( ar );	break;
			case SNAPSHOTTYPE_RAINBOWRACE_PREVRANKING_OPENWND: OnRainbowRacePrevRankingOpenWnd( ar ); break;
			case SNAPSHOTTYPE_RAINBOWRACE_APPLICATION_OPENWND: OnRainbowRaceApplicationOpenWnd( ar ); break;
			case SNAPSHOTTYPE_RAINBOWRACE_NOWSTATE:	OnRainbowRaceNowState( ar ); break;
			case SNAPSHOTTYPE_RAINBOWRACE_MINIGAMESTATE: OnRainbowRaceMiniGameState( ar ); break;
			case SNAPSHOTTYPE_SET_PET_NAME:		OnSetPetName( objid, ar );	break;
			case SNAPSHOTTYPE_HOUSING_ALLINFO: OnHousingAllInfo( ar ); break;
			case SNAPSHOTTYPE_HOUSING_FURNITURELIST: OnHousingSetFunitureList( ar ); break;
			case SNAPSHOTTYPE_HOUSING_SETUPFURNITURE: OnHousingSetupFurniture( ar ); break;
			case SNAPSHOTTYPE_HOUSING_PAPERINGINFO: OnHousingPaperingInfo( ar ); break;
			case SNAPSHOTTYPE_HOUSING_SETVISITALLOW: OnHousingSetVisitAllow( ar ); break;
			case SNAPSHOTTYPE_HOUSING_VISITABLELIST: OnHousingVisitableList( ar ); break;

			case SNAPSHOTTYPE_QUESTHELPER_NPCPOS: OnNPCPos( ar ); break;
			case SNAPSHOTTYPE_CLEAR_TARGET: OnClearTarget( ar ); break;
			case SNAPSHOTTYPE_COUPLE:	OnCouple( ar );	break;
			case SNAPSHOTTYPE_PROPOSE_RESULT:	OnProposeResult( ar );	break;
			case SNAPSHOTTYPE_COUPLE_RESULT:	OnCoupleResult( ar );	break;
			case SNAPSHOTTYPE_DECOUPLE_RESULT:	OnDecoupleResult( ar );	break;
			case SNAPSHOTTYPE_ADD_COUPLE_EXPERIENCE:	OnAddCoupleExperience( ar );	break;

#ifdef __SPEED_SYNC_0108		// ResetDestParam speed 수정
			case SNAPSHOTTYPE_RESETDESTPARAM_SYNC:	OnResetDestParamSync( objid,ar );	break;
#endif // __SPEED_SYNC_0108		// ResetDestParam speed 수정
			case SNAPSHOTTYPE_PCBANG_INFO:	OnPCBangInfo( ar ); break;
#ifdef __VTN_TIMELIMIT
			case SNAPSHOTTYPE_ACCOUNT_PLAYTIME: OnAccountPlayTime( ar ); break;
#endif // __VTN_TIMELIMIT
			case SNAPSHOTTYPE_SMELT_SAFETY:OnSmeltSafety( ar ); break;
#ifdef __MAP_SECURITY
			case SNAPSHOTTYPE_WORLD_READINFO:	OnWorldReadInfo( ar ); break;
#endif // __MAP_SECURITY
#ifdef __QUIZ
			case SNAPSHOTTYPE_QUIZ_STATE:			OnQuizEventState( ar ); break;
			case SNAPSHOTTYPE_QUIZ_MESSAGE:			OnQuizEventMessage( ar ); break;
			case SNAPSHOTTYPE_QUIZ_QUESTION:		OnQuizQuestion( ar ); break;
#endif // __QUIz
			case SNAPSHOTTYPE_VISPET_ACTIVATE:		OnActivateVisPet( ar ); break;
			case SNAPSHOTTYPE_MOVER_CHANGESFX:		OnChangeMoverSfx( objid, ar ); break;
			case SNAPSHOTTYPE_GUILDHOUSE_PACKET:	OnGuildHousePacket( ar ); break;
			case SNAPSHOTTYPE_GUILDHOUSE_ALLINFO:	OnGuildHouseAllInfo( ar ); break;
			case SNAPSHOTTYPE_GUILDHOUSE_REMOVE:	OnGuildHouseRemove( ar ); break;
			case SNAPSHOTTYPE_GUILDHOUSE_RESTPOINT:	OnRestPoint( ar ); break;
			case SNAPSHOTTYPE_QUEST_CHECKED:		OnCheckedQuest( ar ); break;
			case SNAPSHOTTYPE_CAMPUS_INVITE:		OnInviteCampusMember( ar ); break;
			case SNAPSHOTTYPE_CAMPUS_UPDATE:		OnUpdateCampus( ar ); break;
			case SNAPSHOTTYPE_CAMPUS_REMOVE:		OnRemoveCampus( ar ); break;
			case SNAPSHOTTYPE_CAMPUS_UPDATE_POINT:	OnUpdateCampusPoint( ar ); break;

#ifdef __PROTECT_AWAKE
			case SNAPSHOTTYPE_ITEM_SELECT_AWAKENING_VALUE: OnSafeAwakening( ar ); break;
#endif	//__PROTECT_AWAKE

#ifdef __GUILD_HOUSE_MIDDLE
			case SNAPSHOTTYPE_GUILDHOUSE_TENDER_MAINWND:	OnGuildHouseTenderMainWnd( ar ); break;
			case SNAPSHOTTYPE_GUILDHOUSE_TENDER_INFOWND:	OnGuildHouseTenderInfoWnd( ar ); break;
			case SNAPSHOTTYPE_GUILDHOUSE_TENDER_RESULT:		OnGuildHouseTenderResult( ar ); break;
#endif // __GUILD_HOUSE_MIDDLE
			default:
				{
					ASSERT( 0 );
					break;
				}
		}

		if (ar.GoToOffset(startedAt + packetSize) == CAr::GoToOffsetAnswer::TooFar) {
			static std::set<WORD> glitchedSnapshotsId;
			if (!glitchedSnapshotsId.contains(hdr)) {
				Error("Snapshot 0x%04x went too far", hdr);
				glitchedSnapshotsId.emplace(hdr);
			}
		}
	}
}

void CDPClient::OnWorldMsg( OBJID objid, CAr & ar )
{
	static CHAR	lpString[260];
	ar.ReadString( lpString, 260 );
	g_WndMng.PutString( lpString, NULL, 0xffff99cc, CHATSTY_GAME );

}

void CDPClient::OnJoin( CAr & ar )
{
	CNetwork::GetInstance().OnEvent( CACHE_ACK_JOIN );
	
	// ata2k - (2)시간 해제
	g_Neuz.m_dwTimeOutDis = 0xffffffff;

	CWndBase* pWndBase	= g_WndMng.GetWndBase( APP_SELECT_CHAR );
	if( pWndBase )
		( (CWndSelectChar*)pWndBase )->Destroy();
	g_WndMng.CloseMessageBox();

	fJoin	= TRUE;
	OnSnapshot( ar );
	fJoin	= FALSE;

	CMover* pMover	= g_pPlayer;
	if( pMover )
	{
		time_t	tNULL	= time_null();
		for( int i = 0; i < pMover->m_Inventory.GetMax(); i++ )
		{
			CItemElem* pItemElem	= pMover->m_Inventory.GetAtId( i );
			if( pItemElem && pItemElem->m_dwKeepTime )
			{
				char lpString[260]	= { 0, };
				ItemProp* pItemProp		= pItemElem->GetProp();
				if( pItemElem->IsFlag( CItemElem::expired ) )
				{
					sprintf( lpString, prj.GetText( TID_GAME_ENDUSE ), pItemProp->szName );
					g_WndMng.PutString( lpString, NULL, prj.GetTextColor( TID_GAME_ENDUSE ) );
				}
				else
				{
					time_t t	= pItemElem->m_dwKeepTime - tNULL;
					if( t > 0 )
					{
						CTimeSpan time( t );
						if( time.GetDays() )
						{
							sprintf( lpString, prj.GetText( TID_GAME_DAYUSE ), pItemProp->szName, static_cast<int>(time.GetDays() + 1) );
							g_WndMng.PutString( lpString, NULL, prj.GetTextColor( TID_GAME_DAYUSE ) );
						}
						else
						{
							sprintf( lpString, prj.GetText( TID_GAME_TIMEUSE ), pItemProp->szName, time.GetHours() + 1 );
							g_WndMng.PutString( lpString, NULL, prj.GetTextColor( TID_GAME_TIMEUSE ) );
						}
					}
					else
					{
						sprintf( lpString, prj.GetText( TID_GAME_TIMEUSE ), pItemProp->szName, 0 );
						g_WndMng.PutString( lpString, NULL, prj.GetTextColor( TID_GAME_TIMEUSE ) );
					}
				}
			}
		}
	}

#ifdef __GAME_GRADE_SYSTEM
	g_Neuz.m_dwTimeGameGradeMarkRendering = g_tmCurrent + SEC( CNeuzApp::GAME_GRADE_MARK_RENDERING_INTERVAL_SECOND );
#endif // __GAME_GRADE_SYSTEM
}

void CDPClient::OnKeepAlive( CAr & ar )
{
#ifdef __TRAFIC_1218
	m_traficLog.Clear();
#endif	// __TRAFIC_1218
	SendPacket<PACKETTYPE_KEEP_ALIVE>();
}

void CDPClient::OnUpdatePlayerData( CAr & ar )
{
	u_long idPlayer;
	ar >> idPlayer;
	sPlayerData	data;
	ar.Read( &data, sizeof(sPlayerData) );
	PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( idPlayer );

	TRACE( "OnUpdatePlayerData, o = %s, v = %d, l = %d, ----> v = %d, l = %d\n", 
		pPlayerData->szPlayer, pPlayerData->data.nVer, pPlayerData->data.uLogin,
		data.nVer, data.uLogin );

	if( pPlayerData->data.nVer < data.nVer )
		memcpy( &pPlayerData->data, &data, sizeof(sPlayerData) );

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Any);
	UpdateGuildWnd();
}

void	CDPClient::OnLogout( CAr & ar )
{
	u_long idPlayer;
	ar >> idPlayer;
	PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( idPlayer );
	pPlayerData->data.uLogin	= 0;
	TRACE( "OnLogout : %s\n", pPlayerData->szPlayer );
	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Any);
	UpdateGuildWnd();
}

void CDPClient::OnGetClock( CAr & ar )
{
	BYTE fsETbaseOfClient;
	ar >> fsETbaseOfClient;

//	time_t tClient	= timeGetTime();
	DWORD tClient	= timeGetTime();

	SendPacket<PACKETTYPE_GET_CLOCK, BYTE, DWORD>(fsETbaseOfClient, tClient);
}

void CDPClient::OnError( CAr & ar )
{
	ar >> m_lError;

	CNetwork::GetInstance().OnEvent( CACHE_ERROR );

	switch( m_lError )
	{
	case ERROR_INVALID_CLOCK:	// 비정상적인 클럭이 감지
		{
			g_dpLoginClient.Destroy( TRUE );
			s_f		= TRUE;
			break;
		}
	default:
		break;
	}
}


void CDPClient::OnAddObj( OBJID objid, CAr & ar )
{
//static
	DWORD	dwObjType, dwObjIndex;

	CObj::SetMethod( ( g_pPlayer && g_pPlayer->GetId() != objid ) ? METHOD_EXCLUDE_ITEM : METHOD_NONE );

	dwObjType	= 0;
	ar >> (BYTE &)dwObjType >> dwObjIndex;

	//gmpbigsun(100421) test: dwObjIndex가 우체통이면 패킷 Serialize후 return!!!
#ifdef __BS_NO_CREATION_POST
	if( MI_POSTBOX == dwObjIndex  )
	{
		CObj* pObj	= (CCtrl*)CreateObj( dwObjType, dwObjIndex, dwObjType != OT_MOVER );
		if( pObj == NULL )
		{
			CString string;
			string.Format( "CreateObj Error: Type = %d, Index = %d", dwObjType, dwObjIndex );
			//ADDERRORMSG( string );
		}
		pObj->Serialize( ar );

		SAFE_DELETE( pObj );
		return;
	}
#endif
	
	bool existed = false;

	if (CCtrl * pObj = prj.GetCtrl(objid)) {
		if (CWorld * pWorld = pObj->GetWorld()) {
			pObj->Delete();
			pWorld->DeleteObjects();
		} else {
			delete pObj;
			existed = true;
		}
	}

	CCtrl * pObj = (CCtrl*)CreateObj( dwObjType, dwObjIndex, dwObjType != OT_MOVER );
	if (!pObj) {
		std::string str = std::format("CreateObj Error: Type = {}, Index = {}", dwObjType, dwObjIndex);
		Error("%s", str.c_str());
		throw std::runtime_error(str);
	}

	pObj->Serialize(ar);

	if (dwObjType == OT_MOVER) {
		CMover * pMover = static_cast<CMover *>(pObj);

		TRACE("%s Add\n", pMover->GetName());

		if (pMover->IsPlayer()) {
			pMover->RedoEquip(g_pPlayer != NULL);
			PlayerData pd;
			pd.data.nJob = pMover->GetJob();
			pd.data.nLevel = pMover->GetLevel();
			pd.data.nSex = pMover->GetSex();
			lstrcpy(pd.szPlayer, pMover->GetName());
			CPlayerDataCenter::GetInstance()->AddPlayerData(pMover->m_idPlayer, pd);
		} else {
			pMover->RedoEquip(TRUE);

			if (!fJoin) {
				if (g_pPlayer) {
					D3DXVECTOR3 vTemp = pObj->GetPos() - g_pPlayer->GetPos();
					if (vTemp.x * vTemp.x + vTemp.z * vTemp.z < 32 * 32) {
						CreateSfx(XI_GEN_MONSTER_SPAWN01, pObj->GetPos(), pMover->GetId());
					}
				}
			}
		}

		if (!g_pPlayer) {
			if (pMover->m_idPlayer != g_Neuz.m_idPlayer) {
				Error("error - connected on %lu but expected %lu", pMover->m_idPlayer, g_Neuz.m_idPlayer);
				ExitProcess(-1);
			}

			g_WndMng.SetPlayer(g_pPlayer = pMover);
			//GMPBIGSUN : check world loading
			Error("g_pPlayer is ready", existed ? ", existed = true" : "");			//월드가 생성되고 g_pPlayer가 생성되는지 check
			g_WndMng.OpenField();
			InitFST();

			g_Neuz.m_bTexLoadAlpha = TRUE;

			if (!g_pPlayer->IsFly())
				SendPlayerBehavior();
			else
				SendPlayerBehavior2();
		}

		if( pMover->IsNPC() && pMover->GetProp()->dwClass == RANK_BOSS )
		{
			const D3DXVECTOR3 vDist = pObj->GetPos() - g_pPlayer->GetPos();
			if(D3DXVec3LengthSq(&vDist) < 30 * 30 )
			{
				PlayMusic( BGM_IN_BOSS, 1 );
				LockMusic();
				CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
				g_Caption1.RemoveAll();
				if( ::GetLanguage() != LANG_JAP )
					g_Caption1.AddCaption( prj.GetText(TID_GAME_BIGMONSTER), pWndWorld->m_pFontAPITitle );
				else
					g_Caption1.AddCaption( prj.GetText(TID_GAME_BIGMONSTER), NULL );					
			}
		}

		if (pMover->m_pActMover->IsFly()) {
			pMover->SetRide(pMover->GetRideItemIdx());

			const ItemProp * pItemProp = prj.GetItemProp(pMover->GetRideItemIdx());	// 빗자루 프로퍼티.
			if (pItemProp) {
				if (pMover->m_nFuel == -1)		// 초기값이면
					pMover->m_nFuel = (int)(pItemProp->dwFFuelReMax * 0.2f);	// 빗자루 최대 연료량을 세팅.
				// -1일때만 세팅해야지 연료를 반쯤 쓰다가 빗자루를 바꿔도 새로 세팅 되지 않는다.
			}
		}
		pMover->InitMotion(pMover->m_dwMotion);

	}


	if (!existed) {
		if (CItem * pItem = pObj->ToItem()) {
			const ItemProp * pItemProp = pItem->GetProp();
			assert(pItemProp != NULL);
			const auto pos = pObj->GetPos();
			PLAYSND(pItemProp->dwSubDefine, &pos);
		} else if (CCommonCtrl * pCCtrl = pObj->ToCommonCtrl()) {
			const CtrlProp * pProp = pCCtrl->GetProp();

			if (pProp && pProp->IsGuildHousingObj())		//gmpbigsun : 길드 하우스 오브젝트는 애니메이션 돌림 
			{
				CModel * pModel = pObj->GetModel();
				assert(pModel);

				if (pModel->m_nFrameMax > 0) {
					pCCtrl->m_bAniPlay = TRUE;
					pModel->SetLoop(ANILOOP_LOOP);
				}
			}
		}
	}

	pObj->UpdateLocalMatrix();

#ifdef __YSMOOTH_OBJ
	DWORD dwType = pObj->GetType();
	if( dwType == OT_MOVER )
	{
		pObj->m_bSmooth = TRUE;
	}
#endif //__YSMOOTH_OBJ	
	
	g_WorldMng.Get()->AddObj( pObj, TRUE );
	if( pObj->GetType() == OT_MOVER && ( (CMover*)pObj )->m_vtInfo.IsVendorOpen() )
	{
		g_DialogMsg.AddVendorMessage( pObj, ( (CMover*)pObj )->m_vtInfo.GetTitle(), 0xffffffff );
	}

	if( pObj->GetType() == OT_MOVER )
	{
		CMover *pMover = (CMover *)pObj;
		if( pMover->IsPlayer() )
		{
			if( g_pPlayer->m_idDuelParty && g_pPlayer->m_idDuelParty == pMover->m_idparty )
			{
				pMover->RenderFlag( ENEMY_PVP_PARTY );
				pMover->m_nDuel = 2;
				pMover->m_idDuelParty = g_Party.m_uPartyId;
			}
			if( g_pPlayer->m_nDuel && g_pPlayer->m_nDuel &&	g_pPlayer->m_idDuelOther == pMover->GetId() )
			{
				pMover->RenderFlag( ENEMY_PVP );
				pMover->m_idDuelOther = g_pPlayer->GetId();
			}

			if( pMover->IsStateMode( STATE_BASEMOTION_MODE ) )
			{
				if( pMover->m_dwUseItemId != 0 )
				{
					ItemProp* pItemProp = prj.GetItemProp( pMover->m_dwUseItemId );
					if( pItemProp )
					{
						CreateItemReadySfx( pItemProp->dwSfxObj, pMover->GetPos(), pMover->GetId(), 
							pMover->GetPos(), pMover->GetId(), -1 ); 
					}					
				}
			}

			DWORD dwDisquise = pMover->GetDisguise();
			if( dwDisquise != NULL_ID )
			{
				pMover->Disguise( dwDisquise );
				CWndQuestDetail* pWndQuestDetail = g_WndMng.m_pWndQuestDetail;
				if( pWndQuestDetail )
					pWndQuestDetail->UpdateQuestText();

				CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
				if( pWndQuestQuickInfo )
					pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
			}
#ifdef __QUIZ
			else if( g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->GetID() == WI_WORLD_QUIZ && g_pPlayer && g_pPlayer != pMover )
				pMover->Disguise( MI_AIBATT1 );
#endif // __QUIZ
		}

	}

	if( pObj->GetType() == OT_ITEM )
	{
		CItem *pItem = (CItem *)pObj;
		if( pItem )
		{
			CWorld *pWorld = g_WorldMng.Get();
			D3DXVECTOR3 vPos = pObj->GetPos();
			D3DXVECTOR3 vPosOrig = vPos;			// 오리지날 좌표
			// 康 // 12-3: 드롭된 아이템이 오브젝트 밑에 묻히지 않도록 수정
//			vPos.y += 0.5f;
//			FLOAT fGroundY = pWorld->GetUnderHeight( vPos );		// 아이템이 생성된 곳의 좌표의 높이 구함.
			vPos.y	+= 1.5f;
			FLOAT fGroundY = pWorld->GetItemHeight( vPos );


			if( pItem->m_pModel )
			{
				float fHeight = pItem->m_pModel->GetMaxHeight();	
				if( fHeight < 0 )
				{
					Error( "OnAddObj Height < 0 -%d", pItem->GetIndex() );
					fHeight = -fHeight;
				}
				fGroundY += (fHeight / 2.0f);
				fGroundY += 0.12f;

			}
			
			// 지면의 높이와 생성된 아이템의 높이차가 1미터 이하일때만 떨어지는 효과
			// 공중에서 생성된 아이템은 해당안된다. 비행몹 죽였을때 나오는 아이템 같은.
			if( fabs(fGroundY - vPosOrig.y) < 1.0f )	
			{
				pObj->SetPos( vPos );
				D3DXVECTOR3 vDelta;
				FLOAT fAngXZ = (float)xRandom(360);
				vDelta.x = vDelta.z = 0;
				vDelta.y = 0.15f;
				pItem->SetDelta( fGroundY, vDelta );
				TRACE( "(DROP %5.2f %5.2f %5.2f %5.2f %5.2f) ", pObj->GetPos().y, fGroundY, vDelta.x, vDelta.y, vDelta.z );
			}
		}
	}
}

char szBuf[1024];
char szChat[1024];

void CDPClient::OnChat( OBJID objid, CAr & ar )
{
	ar.ReadString( szBuf, 1024 );
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) ) 
	{
#ifdef __YS_CHATTING_BLOCKING_SYSTEM
		if( pMover && pMover->IsPlayer() == TRUE )
		{
			if( prj.m_setBlockedUserID.contains(pMover->GetName(TRUE)) )
				return;
		}
#endif // __YS_CHATTING_BLOCKING_SYSTEM
		// 말하는애가 길드컴뱃 맵에서 이야기를 했다면...검사해서...무시한다..-_-
		if( IsValidObj( pMover ) && pMover->GetWorld() && pMover->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
		{
			if( pMover->IsMode(GCWAR_RENDER_SKIP_MODE) )
				return;
		}
		if( strlen( szBuf ) && ( szBuf[ 0 ] == '/' || szBuf[ 0 ] == '!' ) )
			g_DialogMsg.AddMessage( pMover, szBuf );
		else
		{
			int nTextId = -1;
			sprintf( szChat, "%s : %s", pMover->GetName( TRUE ), szBuf );
			g_WndMng.PutString( (LPCTSTR)szChat, pMover, 0xffffffff, CHATSTY_GENERAL );
		}
	}
}

void CDPClient::OnEventMessage( OBJID objid, CAr & ar )
{
	int nFlag;
	DWORD dwItemID;
	ar.ReadString( szBuf, 1024 );
	ar >> nFlag;
	ar >> dwItemID;

	CMover* pMover = prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		CString strString;
		strString = szBuf;
		D3DXVECTOR3 vPos = pMover->GetPos();
		vPos.y += pMover->m_pModel->GetMaxHeight();

		switch( nFlag )
		{
		case 0: // 꽝 이벤트 상품권
			{
				CSfx *pSfx = CreateSfx( XI_EVE_EVENT_FAIL, vPos);	// 이벤트 꽝 이펙트.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
			}
			break;
		case 1:
			{
				CSfx *pSfx = NULL;
				switch( dwItemID )
				{
				case II_SYS_SYS_EVE_0504M1NOTEBOOK:
					pSfx = CreateSfx( XI_EVE_EVENT_NOTEBOOK, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M2DCAMARA:
					pSfx = CreateSfx( XI_EVE_EVENT_DCAMARA, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M3AIRSHIP:
					pSfx = CreateSfx( XI_EVE_EVENT_AIRSHIP, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M4USBFANLIGHT:
					pSfx = CreateSfx( XI_EVE_EVENT_USBFANLIGHT, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M5BALLOON:
					pSfx = CreateSfx( XI_EVE_EVENT_BALLOON, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M6GIFTTICKET:
					pSfx = CreateSfx( XI_EVE_EVENT_GIFTTICKET, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M7MOVIETICKET:
					pSfx = CreateSfx( XI_EVE_EVENT_MOVIETICKET, vPos );	// 이벤트 이펙트.
					break;
				case II_SYS_SYS_EVE_0504M8OST:
					pSfx = CreateSfx( XI_EVE_EVENT_OST, vPos );	// 이벤트 이펙트.
					break;
				default:
					break;
				}
				
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3(1.5f, 1.5f, 1.5f) );
				
				if( g_pPlayer == pMover )
					g_WndMng.PutString( strString, NULL, 0xff0000 );
			}
			break;
		case 2:		// 획득 이벤트 아이템
			{
				g_WndMng.PutString( strString, pMover, 0xffffffff );
				if( g_pPlayer == pMover )
				{
					CSfx *pSfx = CreateSfx( XI_EVE_EVENT_WIN, vPos);	// 이벤트 꽝 이펙트.
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
				}
			}
			break;
		case 3:		// 아이뎀 추가시 실패
			{
				if( g_pPlayer == pMover )
					g_WndMng.PutString( prj.GetText( TID_GAME_LACKSPACE ), NULL, prj.GetTextColor( TID_GAME_LACKSPACE ) ); //0xff0000 );
			}
			break;
		}
	}
}

void CDPClient::OnText( CAr & ar )
{
	static TCHAR szText[MAX_PATH]; // 260

#ifdef __S_SERVER_UNIFY
	BYTE nState;
	ar >> nState;
#endif // __S_SERVER_UNIFY
	ar.ReadString( szText, MAX_PATH );

#ifdef __S_SERVER_UNIFY
	switch( nState )
	{
	case TEXT_GENERAL:
		g_WndMng.PutString( szText, NULL, 0xff00ff00 );
		break;
	case TEXT_DIAG:
		g_WndMng.OpenMessageBoxUpper( szText );
		break;
	}
#else // __S_SERVER_UNIFY
	g_WndMng.PutString( szText, NULL, 0xff00ff00 );
#endif // __S_SERVER_UNIFY
}

#ifndef __S_SERVER_UNIFY
void CDPClient::OnDiagText( CAr & ar )
{
	static TCHAR szText[MAX_PATH]; // 260
	
	ar.ReadString( szText, MAX_PATH );
	g_WndMng.OpenMessageBoxUpper( szText );
}
#endif // __S_SERVER_UNIFY

#ifdef __S_SERVER_UNIFY
void CDPClient::OnAllAction( CAr & ar )
{
	BOOL bCharacter;
	ar >> g_WndMng.m_bAllAction;
	ar >> bCharacter;

	if( g_WndMng.m_bAllAction == FALSE )
	{
		if( bCharacter )
		{
			// 캐릭터 이름 변경
			if( !g_WndMng.m_pWndChangeName )
			{
				g_WndMng.m_pWndChangeName		= new CWndChangeName;
				g_WndMng.m_pWndChangeName->Initialize();
			}
		}
		else
		{
			CWndGuildName *pWndGuildName	= (CWndGuildName*)g_WndMng.GetWndBase( APP_GUILDNAME );
			if( !pWndGuildName )
			{
				pWndGuildName	= new CWndGuildName;
				pWndGuildName->Initialize();
			}
			pWndGuildName->SetData( 0x7f );
		}
	}
}
#endif // __S_SERVER_UNIFY

void CDPClient::OnRemoveObj( OBJID objid )
{
	CCtrl* pCtrl	= prj.GetCtrl( objid );
	if( IsValidObj( pCtrl ) == FALSE ) 
		return;

	if( pCtrl->GetWorld() ) 
	{
		if( pCtrl->GetType() == OT_MOVER )
		{
			TRACE( "%s Remove\n", ( (CMover*)pCtrl )->GetName() );
			CMover* pMover	= (CMover*)pCtrl;
			if( pMover->m_pet.GetObj() )
			{
				pMover->m_pet.GetObj()->Delete();
				pMover->m_pet.SetObj( NULL );
			}
		}
		CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
		if( pWndWorld )
		{
			if( pCtrl == pWndWorld->m_pSelectRenderObj )
				pWndWorld->m_pSelectRenderObj = NULL;
		}

		if( pCtrl == g_pPlayer->m_vtInfo.GetOther() )
		{
			if( pCtrl->GetType() == OT_MOVER )
			{
				CWndVendor* pWndVendor = (CWndVendor*)g_WndMng.GetWndVendorBase();
				if( pWndVendor )
				{
					if( pWndVendor == CWndBase::m_GlobalShortcut.m_pFromWnd->GetFrameWnd() )
						CWndBase::m_GlobalShortcut.Empty();
					pWndVendor->Destroy();
				}

				CWndShop* pWndShop = (CWndShop*)g_WndMng.GetWndBase( APP_SHOP_ );
				if( pWndShop )
					pWndShop->Destroy();

				CWndTrade* pWndTrade = (CWndTrade*)g_WndMng.GetWndBase( APP_TRADE );
				if( pWndTrade )
					pWndTrade->Destroy();

				CWndConfirmTrade* pWndConfirmTrade = (CWndConfirmTrade*)g_WndMng.GetApplet( APP_CONFIRM_TRADE );
				if( pWndConfirmTrade )
					pWndConfirmTrade->Destroy();

				CWndTradeConfirm* pWndTradeConfirm = (CWndTradeConfirm*)g_WndMng.GetWndBase( APP_TRADE_CONFIRM );
				if( pWndTradeConfirm )
					pWndTradeConfirm->Destroy();

				SAFE_DELETE( g_WndMng.m_pWndTradeGold );

				g_pPlayer->OnTradeRemoveUser();
				g_pPlayer->m_vtInfo.SetOther( NULL );
			}
		}
		
		if( pCtrl == g_pShip )	// 이 루틴은 못탈수 있다구함 - 현민씨가...
		{
			g_pShip	= NULL;
		}

		pCtrl->Delete();
	}
	else 
	{
#ifdef __BS_SAFE_WORLD_DELETE
		//safety
		if( g_pPlayer )
		{
			CWorld* pWorld = g_pPlayer->GetWorld( );

			// cancel deletion
			const auto it = std::ranges::find(pWorld->m_aDeleteObjs, pCtrl);
			if (it != pWorld->m_aDeleteObjs.end()) {
				pWorld->m_aDeleteObjs.erase(it);
			}
		}
#endif //__BS_SAFE_WORLD_DELETE
			
		SAFE_DELETE( pCtrl );
	}
}

void CDPClient::OnShipActMsg( OBJID objid, CAr & ar )
{
	DWORD dwMsg;
	int nParam1, nParam2;
	OBJID idShip;
	ar >> dwMsg >> nParam1 >> nParam2 >> idShip;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		CShip *pShip = prj.GetShip( idShip );		// pMover가 타고 있는 배.
		if( IsValidObj( pShip ) )
		{
			pShip->SendActMsg( (OBJMSG)dwMsg, nParam1, nParam2 );
		}
	}
//		if( idShip != pShip->GetId() )
//			Error( "OnShipActMsg : 서버로부터 온 배의 아이디(%d)와 클라의 아이디(%d)가 다르다", idShip, pShip->GetId() );
		
}

void CDPClient::OnDamage( OBJID objid, CAr & ar )
{
	OBJID objidAttacker;
	DWORD dwHit;
	ar >> objidAttacker >> dwHit;
	DWORD dwAtkFlags;
	ar >> dwAtkFlags;

	CMover* pMover	= prj.GetMover( objid );	// 데미지 받는 대상.
	
	D3DXVECTOR3 vPos;
	FLOAT		fAngle;

	if( dwAtkFlags & AF_FLYING )
	{
		ar >> vPos;
		ar >> fAngle;

		if( IsValidObj(pMover) )
		{
			pMover->SetPos( vPos );
			pMover->SetAngle( fAngle );
		}
	}

	if( IsValidObj( pMover ) )
	{
		pMover->IncHitPoint( (int)( (-1) * (int)( dwHit ) ) );
//		pMover->SetHitPoint( -dwHit );
		if( pMover->m_nHitPoint == 0 )		// hp가 비동기 난거같다.  죽었을땐 이리로 날아오지 않는다 원래.
			pMover->m_nHitPoint = 1;		// 그래서 일단 1로 해줌.
		if( pMover->m_pActMover->IsSit() )		// 앉아있다가 맞으면 앉기해제 한다.
			pMover->m_pActMover->ResetState( OBJSTA_MOVE_ALL );
		
		// 맞는놈이 플레이어거나 / 때리는놈이 플레이어일때만 데미지정보를 보여줌.
		CObj* pObj = g_pPlayer->GetWorld()->GetObjFocus();

		if( pMover->IsActiveMover() )
		{
			CWndBase* pWndBase	= g_WndMng.GetWndVendorBase();				
			if( pWndBase )
			{
				pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_INVENTORY );
				if( pWndBase )
				{
					pWndBase->Destroy();
					pWndBase->m_GlobalShortcut.Empty();
				}
			}
		}

		CCtrl*	pAttacker = prj.GetCtrl( objidAttacker );
		if( IsValidObj( pAttacker ) == FALSE )
			goto g_PASS;
//			return;
		
		// 옵션에서 모든데미지 표시체크가 되었다면 
		if( g_Option.m_bDamageRender )
		{
			// 반경 30미터안에 있는 넘들만 표시
			if( g_pPlayer->IsRangeObj( pMover->GetPos(), 30 ) )
				goto g_PASS;
		}
		else
		{
			if(g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) && pObj )
			{
				if( pObj == (CObj*)pMover || pMover->IsActiveMover() || ( pAttacker->GetType() == OT_MOVER && ( (CMover *)pAttacker )->IsActiveMover() ) ) 
				{
					goto g_PASS;
				}
			}
		}

		if( pMover->IsActiveMover() || ( pAttacker->GetType() == OT_MOVER && ((CMover *)pAttacker)->IsActiveMover() ) ) 
		{
g_PASS:
			// 시전중이라면 캔슬시킴
			if( g_pPlayer->IsStateMode( STATE_BASEMOTION_MODE ) && ( pMover == g_pPlayer || pAttacker == g_pPlayer ) )
			{
				SendStateModeCancel( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
			}
		

			if( dwAtkFlags & AF_BLOCKING )		// 블록일 때 
			{
				D3DXVECTOR3 vPos = pMover->GetPos();
				vPos.y += 1.5f;
				CSfx *pSfx = CreateSfx( XI_HIT_HITBLOCK01, vPos );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
			}

			if( dwAtkFlags & AF_PARRY )
			{
				D3DXVECTOR3 vPos = pMover->GetPos();
				vPos.y += 1.5f;
				CSfx *pSfx = CreateSfx( XI_HIT_PARRY01, vPos );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
			}
			else
			if( dwAtkFlags & AF_RESIST )
			{
				if( dwHit == 0 )
				{
					D3DXVECTOR3 vPos = pMover->GetPos();
					vPos.y += 1.5f;
					CSfx *pSfx = CreateSfx( XI_HIT_MISS01, vPos );
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
				}
				else
					g_DamageNumMng.AddNumber( pMover->GetPos(), dwHit, ( pMover->IsActiveMover()? 0 : 1 ) );
			}
			else
			if( dwAtkFlags & AF_MISS )		// 
			{
				D3DXVECTOR3 vPos = pMover->GetPos();
				vPos.y += 1.5f;
				CSfx *pSfx = CreateSfx( XI_HIT_MISS01, vPos );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
//				g_DamageNumMng.AddNumber( pMover->GetPos(), 0, 4 );
			}
			else
			{
				if( dwAtkFlags & AF_CRITICAL )		// 크리티컬일때
				{
					D3DXVECTOR3 vPos = pMover->GetPos();
					vPos.y += 1.5f;
					CSfx *pSfx = CreateSfx( XI_HIT_CRITICAL01, vPos );
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );
				}

				g_DamageNumMng.AddNumber( pMover->GetPos(), dwHit, ( pMover->IsActiveMover()? 0 : 1 ) );
			}
		}

		if( IsValidObj( pAttacker ) )
		{
			if( pAttacker->GetType() == OT_MOVER )
			{
				if( dwAtkFlags & AF_CRITICAL )
				{
					const ItemProp *pItemProp = ((CMover *)pAttacker)->GetActiveHandItemProp();
					if (pItemProp && pItemProp->dwSndAttack2 != NULL_ID) {
						const auto attackerPos = pAttacker->GetPos();
						PLAYSND(pItemProp->dwSndAttack2, &attackerPos);
					}
				}
				
				if( dwAtkFlags & AF_FLYING )
				{
					if( (pMover->m_pActMover->GetState() != OBJSTA_STUN) )			//gmpbigsun: -_-의미없는 비교다. 0x01 대기상태가 켜져있는것이 대부분이다.
					{
						pMover->m_pActMover->SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_FLY );
						pMover->SetMotion( MTI_DMGFLY, ANILOOP_CONT );
						if( pAttacker )
						{
							((CMover *)pAttacker)->m_pActMover->SendActMsg( OBJMSG_STOP );		// 어태커는 날려보낸 후엔 일단 서자.
							pMover->m_pActMover->DoDamageFly( pAttacker->GetAngle(), 145.0f, 0.18f );	// 어태커가 보는쪽으로 날려보냄.
						}
					}
				}
			} else
			// 어태커가 유효하지 않다고 나오는 경우 서버로 이사실을 알림.
			{
				if( pMover->IsActiveMover() )
				{
					SendError( FE_INVALIDATTACKER, (int)objidAttacker );
					SendCorrReq( objidAttacker );		// 어태커의 좌표를 보정함.
				}
			}
		}
	} // IsValid( pMover )
}

void CDPClient::OnMoverDeath( OBJID objid, CAr & ar )
{
	DWORD dwMsg;
	OBJID objidAttacker;
	ar >> objidAttacker >> dwMsg;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		int nHitPoint	= pMover->GetHitPoint();
		pMover->IncHitPoint( -nHitPoint );
//		pMover->SetHitPoint( -nHitPoint );
		CMover* pAttacker	= prj.GetMover( objidAttacker );

		if( g_pPlayer == pMover )
		{
			pMover->GetWorld()->SetObjFocus( NULL );
			SAFE_DELETE( g_WndMng.m_pWndDuelConfirm );
			g_WndMng.m_pWndWorld->SetLastTarget( NULL_ID );
		}
		else if( pAttacker && g_pPlayer == pAttacker )
		{
			CObj* pTempObj = NULL;
			if(g_WndMng.m_pWndWorld->m_pNextTargetObj != NULL)
				pTempObj = g_WndMng.m_pWndWorld->m_pNextTargetObj;
			pAttacker->GetWorld()->SetObjFocus( NULL );
			g_WndMng.m_pWndWorld->SetLastTarget( NULL_ID );
			if(pTempObj != NULL)
			{
				g_WndMng.m_pWndWorld->m_pNextTargetObj = pTempObj;
				g_WndMng.m_pWndWorld->SetNextTarget();
			}
		}
		
		if(pMover->IsPlayer() && pMover->m_pAngelFlag) //사망시 엔젤관련 초기화.
		{
			if(pMover->m_pAngel)
				SAFE_DELETE(pMover->m_pAngel);
			pMover->m_pAngel = NULL;
			pMover->m_pAngelFlag = FALSE;
		}
		if((CObj*)pMover == g_WndMng.m_pWndWorld->m_pNextTargetObj)
		{
			if(g_WndMng.m_pWndWorld->m_pNextTargetObj == g_WndMng.m_pWndWorld->m_pRenderTargetObj)
			{
				g_WndMng.m_pWndWorld->m_pNextTargetObj = NULL;
				g_WndMng.m_pWndWorld->m_pRenderTargetObj = g_WorldMng()->GetObjFocus();
			}
			else
				g_WndMng.m_pWndWorld->m_pNextTargetObj = NULL;
		}
//		if( IsValidObj( (CObj*)pAttacker ) )
		// pAttacker가 없을수도있다.
		{
			if( pAttacker )
			{
//				if( pMover->IsActiveMover() )
					if( pAttacker->IsPlayer() )		// pMover를 죽인놈이 유저면
					{
						pMover->m_idMurderer = pAttacker->m_idPlayer;	// pMover의 머더러에 그놈 아이디를 넣음.
					}

				if( pMover->IsActiveMover() || pAttacker->IsActiveMover() )
					g_DamageNumMng.AddNumber( pMover->GetPos(), nHitPoint, ( pMover->IsActiveMover()? 0 : 1 ) );
				pAttacker->m_pActMover->SetObjHit( NULL_ID );
				pAttacker->ClearDestObj();
			}
			pMover->SendActMsg( OBJMSG_DIE, dwMsg, (int)pAttacker );
			if( pMover->IsActiveMover() ) 
			{
				BOOL bBossDie = FALSE;

				if( pMover->GetWorld() && pMover->GetWorld()->GetID() == WI_WORLD_GUILDWAR ||
					pMover->GetWorld()->GetID() == WI_DUNGEON_MUSCLE ||
					pMover->GetWorld()->GetID() == WI_DUNGEON_KRRR ||
					pMover->GetWorld()->GetID() == WI_DUNGEON_BEAR )
				{
					if( pMover->GetWorld()->GetID() == WI_DUNGEON_MUSCLE ||
						pMover->GetWorld()->GetID() == WI_DUNGEON_KRRR ||
						pMover->GetWorld()->GetID() == WI_DUNGEON_BEAR )
						bBossDie = TRUE;
				}
				else if( g_GuildCombat1to1Mng.IsPossibleMover( pMover ) )
				{
					// 아무처리 안함
				}
				else
				{
					PlayMusic( BGM_IN_DEATH );
				}
				
				if( !g_GuildCombat1to1Mng.IsPossibleMover( pMover ) )
				{
#ifdef __JEFF_11_4
					CWorld* pWorld	= pMover->GetWorld();
					if( pWorld && pWorld->IsArena() )
						g_WndMng.PutString(TID_GAME_DEATH_ARENA);
					else
						g_WndMng.PutString(TID_GAME_DEATH);
#else	// __JEFF_11_4
					g_WndMng.PutString(TID_GAME_DEATH);
#endif	// __JEFF_11_4
				}

				if( g_eLocal.GetState( EVE_SCHOOL ) )
				{
					SendHdr( PACKETTYPE_REVIVAL_TO_LODESTAR );
				}
				else if( pMover->m_nGuildCombatState == 1 )
				{
					SendHdr( PACKETTYPE_REVIVAL_TO_LODESTAR );
				}
				else
				{
					if( NULL == g_WndMng.m_pWndRevival )		// 부활창 띄움
					{
						g_WndMng.m_pWndRevival	= new CWndRevival;
						g_WndMng.m_pWndRevival->Initialize();

						if( bBossDie )
						{
							g_WndMng.m_pWndRevival->EnableButton( WIDC_REVIVAL_TO_LODELIGHT, FALSE );
						}
					}
				}
				
				CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

				((CWndWorld*)g_WndMng.m_pWndWorld)->m_bAutoAttack = FALSE;		// g_pPlayer가 죽었으면 오토어택 해제시킴
			}
			CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			if( pWndWorld )
			{
				if( pMover->IsActiveMover() || pWndWorld->m_objidTracking == pMover->GetId() )
					pWndWorld->m_objidTracking	= NULL_ID;
			}
		}
	}
}

/*
void CDPClient::UpdateQuestItemText( CMover* pMover, int nItemIdx, BOOL bRemove )
{
	// 퀘스트 아이템과 일치하는게 들어오면 매시지 출력 
	if( pMover->IsActiveMover() )
	{
		CString strTemp;
		BOOL bResult = FALSE;
		for( int i = 0; i < pMover->m_nQuestSize; i++ )
		{
			LPQUEST lpQuest = (LPQUEST) &pMover->m_aQuest[ i ];
			QuestProp* pQuestProp = (QuestProp*)prj.m_aPropQuest.GetAt( lpQuest->m_wId );
			// 아이템 갯수 
			for( int j = 0; j < 4; j++ )
			{
				if( pQuestProp->m_nEndCondItemIdx[ j ] == nItemIdx  )
				{
					if( bRemove == FALSE )
					{
						ItemProp* pItemProp = prj.GetItemProp( pQuestProp->m_nEndCondItemIdx[ j ] );
						int nNum = g_pPlayer->GetItemNum( pQuestProp->m_nEndCondItemIdx[ j ] );
						strTemp.Format( GETTEXT( TID_QUEST_ITEM ), pItemProp->szName, nNum, pQuestProp->m_nEndCondItemNum[ j ] );
						g_WndMng.PutString( strTemp );
					}
					bResult = TRUE;
				}
			}
		}
		if( bResult )
		{
			CWndQuest* pWndQuest = (CWndQuest*)g_WndMng.GetApplet( APP_QUEST );
			if( pWndQuest ) 
				pWndQuest->UpdateText();
		}
	}
}
*/

void CDPClient::OnCreateItem( OBJID objid, CAr & ar )
{
	CItemElem itemElem;

	BYTE nItemType, pnId[MAX_INVENTORY], nCount;
	short pnNum[MAX_INVENTORY];
	
	ar >> nItemType >> itemElem;

	ar >> nCount;
	ar.Read( (void*)pnId, sizeof(BYTE) * nCount );
	ar.Read( (void*)pnNum, sizeof(short) * nCount );

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		D3DXVECTOR3 vPos = pMover->GetPos();
		
		for( int i = 0; i < nCount; i++ )
		{
			itemElem.m_nItemNum		= pnNum[i];
			pMover->m_Inventory.SetAtId( pnId[i], &itemElem );

			if( pMover->IsActiveMover() )
				PLAYSND( SND_INF_GROUNDPICKUP, &vPos );		
		}

		CWndQuestDetail* pWndQuestDetail = g_WndMng.m_pWndQuestDetail;
		if( pWndQuestDetail )
			pWndQuestDetail->UpdateQuestText();

		CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
		if( pWndQuestQuickInfo )
			pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
	}
}

void CDPClient::OnMoveItem( CAr & ar )
{
	BYTE nItemType, nSrcIndex, nDestIndex;
	ar >> nItemType >> nSrcIndex >> nDestIndex;

	if( g_pPlayer )
	{
		g_pPlayer->m_Inventory.Swap( nSrcIndex, nDestIndex );
	}
}

void CDPClient::OnSetPos( OBJID objid, CAr & ar )
{
	CWorld* pWorld;
	D3DXVECTOR3 vPos;
	ar >> vPos;

	CCtrl* pCtrl	= prj.GetCtrl( objid );
	if( IsValidObj( (CObj*)pCtrl ) && ( pWorld = pCtrl->GetWorld() ) )
	{
		((CWndWorld*)g_WndMng.m_pWndWorld)->m_pSelectRenderObj = NULL;

		if( pCtrl->IsActiveMover() )
		{
			CMover* pMover	= (CMover*)pCtrl;
			if( pMover->m_pet.GetObj() )
			{
				pMover->m_pet.GetObj()->Delete();
				pMover->m_pet.SetObj( NULL );
			}
			pWorld->SetObjFocus( NULL );
			pWorld->RemoveObj( pCtrl );
			pWorld->ReadWorld( vPos, TRUE );
			pCtrl->SetPos( vPos );
			pWorld->AddObj( pCtrl, FALSE );
			((CMover*)pCtrl)->SendActMsg( OBJMSG_STAND );
			pWorld->m_pCamera->m_vPos = vPos;
		}
		else 
		{
			if( pCtrl == pWorld->GetObjFocus() )
			{
				pWorld->SetObjFocus( NULL );
			}
//			pWorld->ReadWorld( vPos, FALSE );		// 타 캐릭터까지 리드 월드를 하더라 이것땜에 블링크윙 쓰면 멈추는 현상생김.
			pCtrl->SetPos( vPos );
			if( pCtrl->GetType() == OT_SHIP )		// 배의 경우 
				pCtrl->UpdateLocalMatrix();			// 매트릭스 갱신.
		}
		if( pCtrl->GetType() == OT_MOVER )
		{
			CMover* pMover	= (CMover*)pCtrl;
			if( pMover->m_pet.GetObj() )
			{
				pMover->m_pet.GetObj()->Delete();
				pMover->m_pet.SetObj( NULL );
			}
			pMover->ClearDest();
			pMover->SendActMsg( OBJMSG_ACC_STOP );
			pMover->SendActMsg( OBJMSG_STOP_TURN );
			pMover->SendActMsg( OBJMSG_STAND );
			pWorld->ProcessAllSfx();
			( (CMover* )pCtrl )->InitInterpolation();
		}
	}
}

void CDPClient::OnSetPosAngle( OBJID objid, CAr & ar )
{
	CWorld* pWorld;
	D3DXVECTOR3 vPos;
	FLOAT fAngle;
	ar >> vPos >> fAngle;
	
	CCtrl* pCtrl	= prj.GetCtrl( objid );
	if( IsValidObj( (CObj*)pCtrl ) && ( pWorld = pCtrl->GetWorld() ) )
	{
		D3DXVECTOR3 vDist = pCtrl->GetPos() - vPos;
		FLOAT fLenSq = D3DXVec3LengthSq( &vDist );
//		if( fLenSq > 1.0f * 1.0f )
		{
			pCtrl->SetPos( vPos );
			pCtrl->SetAngle( fAngle );
//			if( pCtrl->GetType() == OT_SHIP )		// 배의 경우 
			pCtrl->UpdateLocalMatrix();			// 매트릭스 갱신.
		}
	}
}

void CDPClient::OnReplace( CAr & ar )
{
	//GMPBIGSUN: check world loading 
	Error( "DPClient::OnReplace BEGIN" );

	DWORD dwWorldID;
	D3DXVECTOR3 vPos;

	ar >> dwWorldID;
	ar >> vPos;

#ifdef __WINDOW_INTERFACE_BUG
	g_WndMng.CloseBoundWindow(); // 월드가 바뀌면 아이템이 걸려 있거나, 아이템을 조작할 가능성이 있는 모든 창을 강제로 닫는다.
#endif // __WINDOW_INTERFACE_BUG

	if(	CDeployManager::GetInstance()->IsReady())		CDeployManager::GetInstance()->EndDeploy();
	const char* szMapFile[] = { "MapLoading2.tga", "MapLoading3.tga", "MapLoading4.tga" };

	CString strPath = MakePath( DIR_THEME, szMapFile[xRandom( 0, 3 )] );
	// 맵(던전) 로딩시 이미지 출력
	CWorld::m_nZoomLevel = 0;
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	if( dwWorldID == WI_WORLD_GUILDWAR || (dwWorldID >= WI_WORLD_GUILDWAR1TO1_0 && dwWorldID <= WI_WORLD_GUILDWAR1TO1_L) )
	{
		strPath = MakePath( DIR_THEME, "MapLoading_GuildWar.tga" );
	}
	else if( dwWorldID == WI_INSTANCE_OMINOUS )
		strPath = MakePath( DIR_THEME, "MapLoading_Ominous.tga" );
	else if( dwWorldID == WI_INSTANCE_OMINOUS_1 )
		strPath = MakePath( DIR_THEME, "MapLoading_Ominous_1.tga" );
	else if( dwWorldID == WI_INSTANCE_DREADFULCAVE )
		strPath = MakePath( DIR_THEME, "MapLoading_DreadfulCave.tga" );
	else if( GuildHouse->IsMyGuildHouse( dwWorldID ) )
		strPath = MakePath( DIR_THEME, "MapLoading_Guildhouse.tga" );
	else
	{
		if( pWndWorld && g_GuildCombat1to1Mng.m_nState != CGuildCombat1to1Mng::GC1TO1_ENTRANCE)
			pWndWorld->m_dwGuildCombatTime = 0xffffffff;
	}
	if( pWndWorld )
		pWndWorld->m_pSelectRenderObj = NULL;

	g_Neuz.m_TexLoading.LoadTexture( strPath, 0xffff00ff, FALSE );
	
	g_Neuz.m_bTexLoad = TRUE;
	g_Neuz.m_nTexAlpha = 255;
	if(SUCCEEDED(g_Neuz.m_pd3dDevice->BeginScene()))
	{
		// render
		int wideOffsetX = 0;
		
		if(g_Option.m_nResWidth == 1280 && (g_Option.m_nResHeight == 720 || g_Option.m_nResHeight == 768 || g_Option.m_nResHeight == 800)) //Wide Offset
		{
			g_Neuz.m_TexLoading.m_size = CSize(960, FULLSCREEN_HEIGHT);
			wideOffsetX = (g_Option.m_nResWidth - 960) / 2;
			g_Neuz.m_2DRender.RenderFillRect( CRect( 0, 0, g_Option.m_nResWidth, g_Option.m_nResHeight ), 0xff000000 );
		}
		else if(g_Option.m_nResWidth == 1360 && g_Option.m_nResHeight == 768)
		{
			g_Neuz.m_TexLoading.m_size = CSize(1024, FULLSCREEN_HEIGHT);
			wideOffsetX = (g_Option.m_nResWidth - 1024) / 2;
			g_Neuz.m_2DRender.RenderFillRect( CRect( 0, 0, g_Option.m_nResWidth, g_Option.m_nResHeight ), 0xff000000 );
		}
		else if(g_Option.m_nResWidth == 1440 && g_Option.m_nResHeight == 900)
		{
			g_Neuz.m_TexLoading.m_size = CSize(1200, FULLSCREEN_HEIGHT);
			wideOffsetX = (g_Option.m_nResWidth - 1200) / 2;
			g_Neuz.m_2DRender.RenderFillRect( CRect( 0, 0, g_Option.m_nResWidth, g_Option.m_nResHeight ), 0xff000000 );
		}		
		else if(g_Option.m_nResWidth == 1680 && g_Option.m_nResHeight == 1050)
		{
			g_Neuz.m_TexLoading.m_size = CSize(1400, FULLSCREEN_HEIGHT);
			wideOffsetX = (g_Option.m_nResWidth - 1400) / 2;
			g_Neuz.m_2DRender.RenderFillRect( CRect( 0, 0, g_Option.m_nResWidth, g_Option.m_nResHeight ), 0xff000000 );
		}
		else
			g_Neuz.m_TexLoading.m_size = CSize(FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);

		g_Neuz.m_2DRender.RenderTexture( CPoint(0 + wideOffsetX, 0), &g_Neuz.m_TexLoading, g_Neuz.m_nTexAlpha );		
	}
	g_Neuz.m_pd3dDevice->EndScene();
	g_Neuz.m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	if( dwWorldID == WI_WORLD_GUILDWAR )
		Sleep( 2000 );
	
	if( dwWorldID == WI_WORLD_GUILDWAR )
	{
		// 길드대전 맵이면 시간을 초저녁으로 강제 셋팅한다..(분위기상) ^^
		g_GameTimer.SetFixed( TRUE );		
		g_GameTimer.SetTime( 0, 19, 0, 0 );
	}
	else
	{
		g_GameTimer.SetFixed( FALSE );	
	}
	
	g_WndMng.SetPlayer( g_pPlayer = NULL );
	//g_DialogMsg.ClearAllMessage();
	g_DialogMsg.RemoveDeleteObjMsg();
	
//	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	pWndWorld->m_dwIdBgmMusic = 0;
//#if __VER >= 9
//	pWndWorld->m_dwIdBgmMusicOld	= 0;
//#endif	//
	StopMusic();
	g_WorldMng.Open( dwWorldID );
	g_WorldMng.Get()->RestoreDeviceObjects( );
	g_WorldMng.Get()->SetCamera( &g_Neuz.m_camera );
	g_Neuz.m_camera.Transform( g_WorldMng.Get() );
	g_Neuz.m_camera.Reset();
	g_WorldMng.Get()->ReadWorld( vPos );
#ifdef __HOUSING
	if(dwWorldID == WI_WORLD_MINIROOM)
	{
		g_Neuz.m_camera.SetCamMode(CM_MYROOM);
	}
	else
	{
		g_Neuz.m_camera.SetCamMode(CM_NORMAL);
		CDeployManager::GetInstance()->SetMyRoom(FALSE);
	}
#endif
	D3DXVECTOR3& rDestinationArrow = g_WndMng.m_pWndWorld->m_vDestinationArrow;
	rDestinationArrow = D3DXVECTOR3( -1.0F, 0.0F, -1.0F );

	//GMPBIGSUN : check world loading 
	Error( "DPClient::OnReplace END OK" );
}

void CDPClient::OnDoEquip( OBJID objid, CAr & ar )
{
	BYTE nId, fEquip;
	DWORD	idGuild;
	int		nPart = 0;
	
	ar >> nId >> idGuild >> fEquip;
	EQUIP_INFO equipInfo;
	ar.Read( (void*)&equipInfo, sizeof(EQUIP_INFO) );

	ar >> nPart;

	CMover* pPlayer	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPlayer ) )
	{
#ifdef __QUIZ
		if( g_pPlayer != pPlayer && g_pPlayer->GetWorld()->GetID() == WI_WORLD_QUIZ )
			return;
#endif //__QUIZ
		if( pPlayer->IsActiveMover() )
		{
			CItemElem* pItemElem	= pPlayer->m_Inventory.GetAtId( nId );
			ASSERT( pItemElem );
				
				pPlayer->DoEquip( pItemElem, fEquip, nPart );
				if( g_pBipedMesh )
				{
					CMover::UpdateParts( pPlayer->GetSex(), pPlayer->m_skin, pPlayer->m_aEquipInfo, g_pBipedMesh, &pPlayer->m_Inventory );
					CWndBeautyShop* pWndBeautyShop = (CWndBeautyShop*)g_WndMng.GetWndBase(APP_BEAUTY_SHOP_EX);
					if( pWndBeautyShop )
						pWndBeautyShop->UpdateModels();
					CWndFaceShop* pWndFaceShop = (CWndFaceShop*)g_WndMng.GetWndBase(APP_BEAUTY_SHOP_SKIN);
					if( pWndFaceShop )
						pWndFaceShop->UpdateModels();
				}
			if( g_WndMng.m_pWndBeautyShop && g_WndMng.m_pWndBeautyShop->m_pModel )
			{
				CMover::UpdateParts( pPlayer->GetSex(), pPlayer->m_skin, pPlayer->m_aEquipInfo, g_WndMng.m_pWndBeautyShop->m_pModel, &pPlayer->m_Inventory );
			}
			
			CWndInventory* pWndInventory	= (CWndInventory*)g_WndMng.GetWndBase( APP_INVENTORY );
			if(pWndInventory && pWndInventory->GetModel())
			{
				pWndInventory->UpdateParts();
			}
		}
		else
		{
			pPlayer->DoFakeEquip( equipInfo, fEquip, nPart, NULL );
		}
		pPlayer->UpdateParts( !pPlayer->IsActiveMover() );
	}
}

void CDPClient::OnTrade( OBJID objid, CAr & ar )
{
	u_long uidPlayer;
	ar >> uidPlayer;

	CMover* pTrader	= prj.GetMover( objid );
	if (!IsValidObj(pTrader)) return;

	g_pPlayer->m_vtInfo.SetOther( pTrader );
	pTrader->m_vtInfo.SetOther( g_pPlayer );
		
	g_WndMng.CreateApplet( APP_INVENTORY );
	g_WndMng.CreateApplet( APP_TRADE );
}

void CDPClient::OnConfirmTrade( OBJID objid, CAr & ar ) {
	CMover* pTrader	= prj.GetMover( objid );
	if (!pTrader) return;

	// 신청하는 형태의 매시지 박스는 여러개가 날아올 수 있고, 내 의지와는 관계가 없다.
	// 이런 경우 CWndDuelConfirm::OnChildNotify에서 윈도를 닫을 때 Destroy( TRUE )로 해서
	// 메모리에서 완전히 파괴하도록 한다. 안하면 리크로 남던지, ALT+ENTER, ALT+TAB할 때 다운됨.
	//SAFE_DELETE( g_WndMng.m_pWndConfirmTrade );
	if (g_Option.m_bTrade == FALSE) {
		SendBlock(3, g_pPlayer->GetName(), pTrader->GetName());
		return;
	}

	if (Windows::IsOpen(APP_REPAIR)) {
		SendBlock(4, g_pPlayer->GetName(), pTrader->GetName());
		return;
	}
		
	if (Windows::IsOpen(APP_TRADE, APP_CONFIRM_TRADE)) {
		SendBlock(5, g_pPlayer->GetName(), pTrader->GetName());
		return;
	}

	if (Windows::IsOpen(APP_GUILD_BANK, APP_SUMMON_ANGEL)) {
		SendBlock(8, g_pPlayer->GetName(), pTrader->GetName());
		return;
	}
		
	g_WndMng.m_pWndConfirmTrade = new CWndConfirmTrade;
	g_WndMng.m_pWndConfirmTrade->Initialize();			
	g_WndMng.m_pWndConfirmTrade->OnSetName( pTrader->GetName(), objid );
}

void CDPClient::OnConfirmTradeCancel(OBJID objid, CAr & ar) {
	g_WndMng.PutString(TID_GAME_TRADECANCEL);
}


void CDPClient::OnTradePut( OBJID objid, CAr & ar )
{
	// BYTE i, nItemType, nId;
	// short nItemNum;
	// ar >> i >> nItemType >> nId >> nItemNum;

	CMover* pOther = g_pPlayer->m_vtInfo.GetOther();
	if (!pOther) return;
	if (pOther->m_vtInfo.GetOther() != g_pPlayer) return;

	if (objid == NULL_ID) {
		const auto [i, _, nId, nItemNum] = ar.Extract<
			BYTE, BYTE, BYTE, short
		>();

		g_pPlayer->m_vtInfo.TradeSetItem(nId, i, nItemNum);
	} else {
		const auto [i, nItemNum] = ar.Extract<BYTE, short>();
		CItemElem item;
		ar >> item;

		pOther->m_vtInfo.TradeSetItem(item, i, nItemNum);
	}
}

void CDPClient::OnTradePutError(OBJID objid, CAr & ar) {
	g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0054)));
}

void CDPClient::OnTradePull( OBJID objid, CAr & ar )
{
	BYTE i;
	ar >> i;

	CMover* pOther = g_pPlayer->m_vtInfo.GetOther();
	if( pOther && pOther->m_vtInfo.GetOther() == g_pPlayer )
	{
		CMover* pPlayer = ( objid == g_pPlayer->GetId() ? g_pPlayer : g_pPlayer->m_vtInfo.GetOther() );
		pPlayer->m_vtInfo.TradeClearItem( i );
	}
}

void CDPClient::OnTradePutGold( OBJID objid, CAr & ar )
{
	DWORD dwGold;
	ar >> dwGold;

	CMover* pTrader	= g_pPlayer->m_vtInfo.GetOther();
	ASSERT( pTrader );

	CWndTrade* pWndTrade	= (CWndTrade*)g_WndMng.GetApplet( APP_TRADE );
	if( pWndTrade )
	{
		if( g_pPlayer->GetId() == objid )
		{
			g_pPlayer->AddGold( (int)( (-1) * (int)( dwGold ) ), FALSE );		//raiders.2006.11.28
			g_pPlayer->m_vtInfo.TradeSetGold( dwGold );
			pWndTrade->m_nGoldI	= dwGold;
		}
		else
		{
			pTrader->AddGold( (int)( (-1) * (int)( dwGold ) ), FALSE );			//raiders.2006.11.28
			pTrader->m_vtInfo.TradeSetGold( dwGold );
			pWndTrade->m_nGoldYou	= dwGold;
		}
	}
}
//raiders.2006.11.28
/*
void CDPClient::OnTradeClearGold( OBJID objid )
{
	CMover* pTrader	= g_pPlayer->m_vtInfo.GetOther();
	ASSERT( pTrader );

	CWndTrade* pWndTrade = (CWndTrade*)g_WndMng.GetApplet( APP_TRADE );
	if( pWndTrade )
	{
		if( g_pPlayer->GetId() == objid )
		{
			g_pPlayer->m_vtInfo.TradeSetGold( 0 );
			pWndTrade->m_nGoldI	= 0;
		}
		else
		{
			pTrader->m_vtInfo.TradeSetGold( 0 );
			pWndTrade->m_nGoldYou = 0;
		}
	}
}
*/

void CDPClient::SendTradeOk() {
	SendHdr(PACKETTYPE_TRADEOK);
}

void CDPClient::SendChangeFace( DWORD dwFaceNum )
{
	if(g_pPlayer->HasBuffByIk3(IK3_TEXT_DISGUISE))
	{
		g_WndMng.PutString(TID_GAME_ERROR_BEAUTYSHOP_DISGUISE);
	}
	else
	{
		const bool useCoupon = g_WndMng.m_pWndFaceShop && g_WndMng.m_pWndFaceShop->m_bUseCoupon;
		g_DPlay.SendPacket<PACKETTYPE_CHANGEFACE, DWORD, bool>(dwFaceNum, useCoupon);
	}
}
//raiders.2006.11.28
/*
void	CDPClient::SendTradeClearGold( void )
{	
	SendHdr( PACKETTYPE_TRADECLEARGOLD );	
}
*/

void CDPClient::OnTradeCancel( OBJID objid, CAr & ar )
{
	int nMode;
	u_long uidPlayer;
	ar >> uidPlayer;
	ar >> nMode;
	
	CMover* pTrader	= g_pPlayer->m_vtInfo.GetOther();
	if( pTrader )
		pTrader->m_vtInfo.TradeClear();
	g_pPlayer->m_vtInfo.TradeClear();

	CWndTrade* pWndTrade	= (CWndTrade*)g_WndMng.GetApplet( APP_TRADE );
	if( pWndTrade )
		pWndTrade->DoCancel();

	if( nMode )
	{
		if( uidPlayer != g_pPlayer->m_idPlayer )
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0002) ) );
	}
	else
	{
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0003) ) );
		Windows::DestroyIfOpened(APP_TRADE_CONFIRM);
	}

	SAFE_DELETE( g_WndMng.m_pWndTradeGold );
}

void CDPClient::OnTradelastConfirmOk(OBJID objid, CAr &) {
	if (objid == g_pPlayer->GetId()) {
		if (CWndTradeConfirm * pWndTradeConfirm = g_WndMng.GetWndBase<CWndTradeConfirm>(APP_TRADE_CONFIRM)) {
			pWndTradeConfirm->OnTradelastConfirmOk();
		}
	} else if (g_pPlayer->m_vtInfo.GetOther()) {
		g_WndMng.PutString(TID_GAME_FINALCOMFIRM, g_pPlayer->m_vtInfo.GetOther()->GetName());
	}
}

void CDPClient::OnTradeOk( OBJID objid, CAr & ar )
{
	CWndTrade* pWndTrade	= (CWndTrade*)g_WndMng.GetApplet( APP_TRADE );
	if( pWndTrade )
	{
		CWndButton* pWndOkI = (CWndButton*)pWndTrade->GetDlgItem( WIDC_OK_I );
		CWndButton* pWndOkYou= (CWndButton*)pWndTrade->GetDlgItem( WIDC_OK_YOU );
		if( objid == g_pPlayer->GetId() )
		{
			pWndOkI->SetCheck( TRUE );
			pWndOkI->EnableWindow( FALSE );
		}
		else
		{
			pWndOkYou->SetCheck( TRUE );
			pWndOkYou->EnableWindow( FALSE );
			if( g_pPlayer->m_vtInfo.GetOther() )
			{
				g_WndMng.PutString(TID_GAME_TRADEACCPET, g_pPlayer->m_vtInfo.GetOther()->GetName());
			}
		}
	}
}

void CDPClient::OnTradeConsent() {
	if (g_pPlayer->m_vtInfo.TradeConsent()) {
		Windows::DestroyIfOpened(APP_TRADE, APP_TRADE_CONFIRM);
	}
}

void CDPClient::OnTradelastConfirm( void )
{
	g_WndMng.m_pWndTradeConfirm = new CWndTradeConfirm;
	g_WndMng.m_pWndTradeConfirm->Initialize();

	CWndTrade* pWndTrade	= (CWndTrade*)g_WndMng.GetApplet( APP_TRADE );
	if( pWndTrade )
	{
		CWndButton* pWndOkI = (CWndButton*)pWndTrade->GetDlgItem( WIDC_OK_I );
		CWndButton* pWndOkYou= (CWndButton*)pWndTrade->GetDlgItem( WIDC_OK_YOU );
		pWndOkI->SetCheck( TRUE );
		pWndOkI->EnableWindow( FALSE );
		pWndOkYou->SetCheck( TRUE );
		pWndOkYou->EnableWindow( FALSE );
	}
}


void CDPClient::OnOpenShopWnd( OBJID objid, CAr & ar )
{
	CMover* pVendor	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pVendor ) )
	{
		for( int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i++ )
			ar >> *pVendor->m_ShopInventory[i];

		SAFE_DELETE( g_WndMng.m_pWndShop );
		g_WndMng.m_pWndShop		= new CWndShop;
		g_WndMng.m_pWndShop->m_pMover	= pVendor;
		g_pPlayer->m_vtInfo.SetOther( pVendor );

		g_WndMng.CreateApplet( APP_INVENTORY );
		g_WndMng.m_pWndShop->Initialize();
	}
}

void CDPClient::OnPutItemBank(OBJID objid, CAr & ar) {
	BYTE nSlot;
	CItemElem itemElem;
	ar >> nSlot >> itemElem;
	g_pPlayer->m_Bank[nSlot].Add(&itemElem);
}

void CDPClient::OnPutItemGuildBank(OBJID objid, CAr & ar) {
	BYTE mode;
	CItemElem itemElem;
	ar >> mode >> itemElem;

	g_pPlayer->GetGuild()->m_GuildBank.Add(&itemElem);
}

void CDPClient::OnGetItemGuildBank( OBJID objid, CAr & ar )
{
	BYTE mode = 0;
	ar >> mode;

	switch ( mode )
	{
	case	GUILD_PENYA_MINE_UPDATE:
		{
			DWORD	Gold = 0;
			u_long	playerID;
			BYTE	cbCloak;

			ar >> Gold;
			ar >> playerID;
			ar >> cbCloak;

			//g_pPlayer->m_dwGold += Gold;
			g_pPlayer->AddGold( Gold, FALSE );

			if (g_pPlayer->GetGuild()->m_nGoldGuild > Gold)
				g_pPlayer->GetGuild()->m_nGoldGuild -= Gold;
			else
				g_pPlayer->GetGuild()->m_nGoldGuild = 0;

			if( cbCloak != 1 )
			{
				//망토를 사지 않은 경우 공헌도를 하락시킨다.
				g_pPlayer->GetGuild()->DecrementMemberContribution( playerID, Gold, 0 );
			}
			UpdateGuildWnd();

		}
		break;
	case	GUILD_PENYA_ALL_UPDATE:
		{
			DWORD	Gold = 0;
			u_long	playerID;
			BYTE	cbCloak;

			ar >> Gold;
			ar >> playerID;
			ar >> cbCloak;

			if (g_pPlayer->GetGuild()->m_nGoldGuild > Gold)
				g_pPlayer->GetGuild()->m_nGoldGuild -= Gold;
			else
				g_pPlayer->GetGuild()->m_nGoldGuild = 0;

			if( cbCloak != 1 )
			{
				//망토를 사지 않은 경우 공헌도를 하락시킨다.
				g_pPlayer->GetGuild()->DecrementMemberContribution( playerID, Gold, 0 );
			}
			UpdateGuildWnd();
		}
		break;

	case	GUILD_ITEM_MINE_UPDATE:
		{
			CItemElem itemElem;
			ar >> itemElem;
		
			CItemElem* pTempElem = g_pPlayer->GetGuild()->m_GuildBank.GetAtId(itemElem.m_dwObjId);
			if (pTempElem)
			{
				g_pPlayer->m_Inventory.Add( &itemElem );
				
				if (pTempElem->m_nItemNum > itemElem.m_nItemNum )
					pTempElem->m_nItemNum -= itemElem.m_nItemNum;
				else
					g_pPlayer->GetGuild()->m_GuildBank.RemoveAtId(itemElem.m_dwObjId);
			}
		}
		break;
	case	GUILD_ITEM_ALL_UPDATE:
		{
			CItemElem itemElem;
			ar >> itemElem;
			
			CItemElem* pTempElem = g_pPlayer->GetGuild()->m_GuildBank.GetAtId(itemElem.m_dwObjId);
			if (pTempElem)
			{
				if (pTempElem->m_nItemNum > itemElem.m_nItemNum )
					pTempElem->m_nItemNum -= itemElem.m_nItemNum;
				else
					g_pPlayer->GetGuild()->m_GuildBank.RemoveAtId(itemElem.m_dwObjId);
			}
		}
		break;
	}
}

void CDPClient::OnGetItemBank( OBJID objid, CAr & ar )
{
	CItemElem itemElem;
	ar >> itemElem;
	g_pPlayer->m_Inventory.Add( &itemElem );
}

void CDPClient::OnUpdateBankItem( OBJID objid, CAr & ar ) {
	const auto [nSlot, nId, newQuantity] = ar.Extract<BYTE, BYTE, short>();
	
	if (g_pPlayer) g_pPlayer->UpdateItemBank(nSlot, nId, newQuantity);
}
void CDPClient::OnPutGoldBank( OBJID objid, CAr & ar )
{
	BYTE nSlot;
	DWORD dwGold;

	ar >> nSlot;
	ar >> dwGold >> g_pPlayer->m_dwGoldBank[nSlot];
	g_pPlayer->SetGold( dwGold );
}
void CDPClient::OnMoveBankItem( OBJID objid, CAr & ar )
{
}

void CDPClient::OnErrorBankIsFull(OBJID objid, CAr & ar) {
	g_WndMng.PutString(TID_GAME_SLOTFULL);
}

void CDPClient::OnBank( OBJID , CAr & ar )
{
	Subsnapshot::Bank nMode; ar >> nMode;

	SAFE_DELETE(g_WndMng.m_pWndBank);
	SAFE_DELETE(g_WndMng.m_pWndConfirmBank);
	SAFE_DELETE(g_WndMng.m_pWndBankPassword);

	if (nMode == Subsnapshot::Bank::ValidateBankAccess) {
		g_WndMng.CreateApplet(APP_INVENTORY);
		g_WndMng.m_pWndBank = new CWndBank;
		g_WndMng.m_pWndBank->Initialize();
		return;
	}

	OBJID dwId; ar >> dwId;

	switch (nMode) {
		case Subsnapshot::Bank::AskCurrentPassword:
		case Subsnapshot::Bank::OkForNewPassword:
			g_WndMng.m_pWndConfirmBank = new CWndConfirmBank(dwId);
			g_WndMng.m_pWndConfirmBank->Initialize();
			break;
		case Subsnapshot::Bank::InitialRequirePassword:
			g_WndMng.m_pWndBankPassword = new CWndBankPassword(false, dwId);
			g_WndMng.m_pWndBankPassword->Initialize();
			break;
		case Subsnapshot::Bank::InvalidNewPasswordQuery:
			g_WndMng.m_pWndBankPassword = new CWndBankPassword(true, dwId);
			g_WndMng.m_pWndBankPassword->Initialize();

			g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0028)));
			break;
		case Subsnapshot::Bank::InvalidCurrentPassword:
			g_WndMng.m_pWndConfirmBank = new CWndConfirmBank(dwId);
			g_WndMng.m_pWndConfirmBank->Initialize();
			g_WndMng.OpenMessageBox(_T(prj.GetText(TID_DIAG_0028)));
			break;
	}
}


void CDPClient::OnGuildBankWindow( OBJID objid, CAr & ar )
{
	int nMode;
	ar >> nMode;

	switch(nMode) 
	{
	case 0:
		{
			SAFE_DELETE( g_WndMng.m_pWndGuildBank );
			g_WndMng.m_pWndGuildBank = new CWndGuildBank;
			
			int nGuildGold; ar >> nGuildGold;
			g_pPlayer->GetGuild()->m_nGoldGuild = nGuildGold;

			ar >> g_pPlayer->GetGuild()->m_GuildBank;

			g_WndMng.CreateApplet( APP_INVENTORY );
			
			g_WndMng.m_pWndGuildBank->Initialize();
		}
		break;
	default:
		break;
	}
}


void CDPClient::OnVendor(OBJID objid, CAr & ar) {
	CMover * pVendor = prj.GetMover(objid);
	if (!IsValidObj(pVendor)) return;

	for (int i = 0; i < MAX_VENDOR_INVENTORY_TAB; i++) {
		ar >> *pVendor->m_ShopInventory[i];
	}
}

void CDPClient::OnUpdateVendor( OBJID objid, CAr & ar )
{
	CHAR cTab;
	BYTE nId;
	short nNum;

	ar >> cTab >> nId >> nNum;

	CMover* pVendor	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pVendor ) )
	{
		CItemElem* pItemElem
				= pVendor->m_ShopInventory[cTab]->GetAtId( nId );
		if( pItemElem )
		{
			if( nNum == 0 )
			{
				pVendor->m_ShopInventory[cTab]->RemoveAtId( nId );
			}
			else
			{
				if( pItemElem )
					pItemElem->m_nItemNum	= nNum;
			}

			CWndShop* pWndShop	= (CWndShop*)g_WndMng.GetApplet( APP_SHOP_ );
			if( pWndShop )
			{
				if( pWndShop->m_pWndConfirmSell->m_pItemElem == pItemElem )
					SAFE_DELETE( pWndShop->m_pWndConfirmSell );
			}
		}
	}
}

void CDPClient::OnUpdateItemVariant(const OBJID objid, CAr & ar) {
	// ~~ Update memory
	const auto [dwId, operation] = ar.Extract<ItemPos, UI::Variant>();

	if (operation.valueless_by_exception()) {
		Error(__FUNCTION__"(): Invalid variant received");
		return;
	}

	CMover * const pMover = prj.GetMover( objid );
	if (!IsValidObj(pMover)) return;

	if (const UI::Num * uiNum = std::get_if<UI::Num>(&operation); uiNum && uiNum->startCooldown) {
		if (!g_pPlayer)  return;

		if (pMover == g_pPlayer) {
			const CItemElem * const pItemBase = g_pPlayer->GetItemId(dwId);
			if (!pItemBase) return;

			const ItemProp * const pItemProp = pItemBase->GetProp();
			if (!pItemProp) return;

			g_pPlayer->m_cooltimeMgr.StartCooldown(*pItemProp);
		}
	}

	pMover->UpdateItem(dwId, operation);

	// ~~ Update user interface
	if (pMover == g_pPlayer) {
		if (CWndInventory * pWnd = (CWndInventory *)g_WndMng.GetWndBase(APP_INVENTORY)) {
			pWnd->UpdateTooltip();
		}

		if (g_WndMng.m_pWndQuestDetail) {
			g_WndMng.m_pWndQuestDetail->UpdateQuestText();
		}

		if (g_WndMng.m_pWndQuestQuickInfo) {
			g_WndMng.m_pWndQuestQuickInfo->SetUpdateTextSwitch(TRUE);
		}
	}
}

void CDPClient::OnSetDestParam( OBJID objid, CAr & ar )
{
	int nDstParameter, nAdjParameterValue, nChgParameterValue;
	ar >> nDstParameter >> nAdjParameterValue >> nChgParameterValue;

	CMover* pMover	= prj.GetMover( objid );
	if( TRUE == IsValidObj( (CObj*)pMover ) )
	{
		pMover->SetDestParam( nDstParameter, nAdjParameterValue, nChgParameterValue );
	}
}

void CDPClient::OnResetDestParam( OBJID objid, CAr & ar )
{
	int nDstParameter, nAdjParameterValue;
	ar >> nDstParameter >> nAdjParameterValue;

	CMover* pMover	= prj.GetMover( objid );
	if( TRUE == IsValidObj( (CObj*)pMover ) )
	{
		pMover->ResetDestParam( nDstParameter, nAdjParameterValue, 0 );
	}
}

#ifdef __SPEED_SYNC_0108		// ResetDestParam speed 수정
void CDPClient::OnResetDestParamSync( OBJID objid, CAr & ar )
{
	int nDstParameter, nAdjParameterValue, nParameterValue;
	ar >> nDstParameter >> nAdjParameterValue >> nParameterValue;

	CMover* pMover	= prj.GetMover( objid );
	if( TRUE == IsValidObj( (CObj*)pMover ) )
	{
		pMover->ResetDestParamSync( nDstParameter, nAdjParameterValue, nParameterValue, 0 );
	}
}
#endif // __SPEED_SYNC_0108		// ResetDestParam speed 수정

void CDPClient::OnSetPointParam( OBJID objid, CAr & ar )
{
	int nDstParameter, nValue;
	ar >> nDstParameter >> nValue;

	CMover* pMover	= prj.GetMover( objid );
	if( TRUE == IsValidObj( (CObj*)pMover ) )
	{
		pMover->SetPointParam( nDstParameter, nValue );
	}
}

// objid를 부활시킴.
void CDPClient::OnResurrection( OBJID objid )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->m_pActMover->SendActMsg( OBJMSG_RESURRECTION );	// 부활모드로 전환.
		if( g_WndMng.m_pWndRevival && pMover->IsActiveMover() )		// 주인공 캐릭일경우 부활창이 떠있으면 
		{
			g_WndMng.m_pWndRevival->Destroy();	// 없앤다.
		}
	}
}

void CDPClient::OnResurrectionMessage()
{
	if( g_WndMng.m_pWndResurrectionConfirm == NULL )
	{
		g_WndMng.m_pWndResurrectionConfirm = new CWndResurrectionConfirm;
		g_WndMng.m_pWndResurrectionConfirm->Initialize();
	}
}


void CDPClient::OnSetLevel( OBJID objid, CAr & ar )
{
	WORD wLevel;
	ar >> wLevel;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( (int)wLevel > pMover->m_nLevel )	// 레벨업 된상황이면 
		{
			if( pMover->m_pActMover && ( pMover->m_pActMover->IsState( OBJSTA_STAND ) || pMover->m_pActMover->IsState( OBJSTA_STAND2 )) )
				pMover->SetMotion( MTI_LEVELUP, ANILOOP_1PLAY, MOP_FIXED );
			CreateSfx( XI_GEN_LEVEL_UP01, pMover->GetPos(), pMover->GetId() );	// 레벨업 이펙트.
		}

		//현재 클라와 동기화가 안된 상황 - 기본 정보만 일단 셋팅 틱처리하는 부분이 수정되면 자동으로 Hp, Mp는 동기화가 될듯.
		pMover->m_nLevel	= (int)wLevel;
		pMover->m_nHitPoint = pMover->GetMaxHitPoint();
		pMover->m_nManaPoint = pMover->GetMaxManaPoint();
	}
}

void CDPClient::OnSetFlightLevel( OBJID objid, CAr & ar )
{
	WORD wFlightLv;
	ar >> wFlightLv;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( (int)wFlightLv > pMover->GetFlightLv() )	// 레벨업 된상황이면 
			CreateSfx( XI_GEN_LEVEL_UP01, pMover->GetPos(), pMover->GetId() );	// 레벨업 이펙트.
		pMover->SetFlightLv( wFlightLv );
	}
}


void CDPClient::OnSetExperience( OBJID objid, CAr & ar )
{
	EXPINTEGER nExp1, nDeathExp;
	WORD wLevel, wDeathLevel;

	int nSkillLevel;
	int nSkillPoint;
	ar >> nExp1 >> wLevel;
	ar >> nSkillLevel >> nSkillPoint >> nDeathExp >> wDeathLevel;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->SetExperience( nExp1, (int)wLevel );
		if( nDeathExp != static_cast<EXPINTEGER>( -1 ) )
			pMover->m_nDeathExp		= nDeathExp;
		if( wDeathLevel != 0xffff )
			pMover->m_nDeathLevel = wDeathLevel;
		int nSP = pMover->m_nSkillPoint;
		pMover->m_nSkillLevel = nSkillLevel;
		pMover->m_nSkillPoint = nSkillPoint;

		if( nSP < nSkillPoint )
		{
			CreateSfx(XI_GEN_LEVEL_UP01,g_pPlayer->GetPos(),g_pPlayer->GetId());
			if( pMover == g_pPlayer )
			{
				PlayMusic( BGM_IN_LEVELUP );
				if( g_WndMng.m_pWndWorld )
				{
					if( ::GetLanguage() != LANG_JAP )
						g_Caption1.AddCaption(  prj.GetText( TID_GAME_SKILLPOINT_UP ), g_WndMng.m_pWndWorld->m_pFontAPICaption );// CWndBase::m_Theme.m_pFontCaption );
					else
						g_Caption1.AddCaption(  prj.GetText( TID_GAME_SKILLPOINT_UP ), NULL );// CWndBase::m_Theme.m_pFontCaption );
					g_WndMng.m_pWndWorld->GetAdvMgr()->AddAdvButton(APP_SKILL_);
				}
			}
		}

		if( pMover == g_pPlayer )
		{
			if (nSP != nSkillPoint) {
				CWndSkillTreeCommon::ReInitIfOpen();
			}
		}
	}
}

void CDPClient::OnSetFxp( OBJID objid, CAr & ar )
{
	WORD wFxp, wFlightLv;
	ar >> wFxp >> wFlightLv;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->SetFxp( (int)wFxp, (int)wFlightLv );
	}
}

void CDPClient::OnRevival( OBJID objid )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->m_pActMover->ClearState();

		
		if( pMover->IsActiveMover() && g_WndMng.m_pWndResurrectionConfirm )
			g_WndMng.m_pWndResurrectionConfirm->Destroy();
	}
}

void CDPClient::OnRevivalLodestar( OBJID objid )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->m_pActMover->ClearState();

		int nRate;
		int nLevel	= pMover->GetLevel();
		if( nLevel == 1 )	nRate	= 8;
		else if( nLevel == 2 )	nRate	= 6;
		else if( nLevel <= 5 )	nRate	= 5;
		else if( nLevel <= 10 )	nRate	= 4;
		else if( nLevel <= 15 )	nRate	= 3;
		else	nRate	= 3;

		pMover->SetHitPoint( pMover->GetMaxHitPoint() * nRate / 10 );
		int nVal	= pMover->GetMaxManaPoint() * nRate / 10;
		if( pMover->GetManaPoint() < nVal )
			pMover->SetManaPoint( nVal );
		nVal	= pMover->GetMaxFatiguePoint() * nRate / 10;
		if( pMover->GetFatiguePoint() < nVal )
			pMover->SetFatiguePoint( nVal );

		if( pMover->IsActiveMover() && g_WndMng.m_pWndResurrectionConfirm )
			g_WndMng.m_pWndResurrectionConfirm->Destroy();		
	}
}

void CDPClient::OnRevivalLodelight( OBJID objid )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->m_pActMover->ClearState();

		pMover->SetHitPoint( pMover->GetMaxHitPoint() / 5 );

		if( pMover->IsActiveMover() && g_WndMng.m_pWndResurrectionConfirm )
			g_WndMng.m_pWndResurrectionConfirm->Destroy();		
	}
}

void CDPClient::OnSetGrowthLearningPoint(OBJID objid, CAr & ar) {
	long nRemainGP; ar >> nRemainGP;

	CMover * pMover = prj.GetMover(objid);
	if (IsValidObj(pMover)) {
		pMover->m_nRemainGP = nRemainGP;
	}
}

void CDPClient::OnSetChangeJob( OBJID objid, CAr & ar ) {
	CMover* pMover = prj.GetMover( objid );
	if (!IsValidObj(pMover)) return;

	ar >> pMover->m_nJob >> pMover->m_jobSkills;

	CWndSkillTreeCommon::ReInitIfOpen();
		
	if (pMover->IsActiveMover()) {
		if (pMover->m_pActMover && (pMover->m_pActMover->IsState(OBJSTA_STAND) || pMover->m_pActMover->IsState(OBJSTA_STAND2)))
			pMover->SetMotion(MTI_LEVELUP, ANILOOP_1PLAY, MOP_FIXED);
		CreateSfx( XI_GEN_LEVEL_UP01, pMover->GetPos(), pMover->GetId());
		PlayMusic(BGM_IN_LEVELUP);
		g_WndMng.PutString(TID_EVE_CHGJOB, pMover->GetJobString());

		CWndWorld * pWndWorld = g_WndMng.GetWndBase<CWndWorld>(APP_WORLD);

		if (pWndWorld) {
			pWndWorld->m_pWndGuideSystem->ChangeModel(pMover->GetJob());
			pWndWorld->m_pWndGuideSystem->SetAni(pMover->GetJob(), CWndGuideSystem::ANI_IDLE);
		}
	}
}

void CDPClient::OnSetNearChangeJob(OBJID objid, CAr & ar) {
	CMover * pMover = prj.GetMover(objid);
	if (!IsValidObj(pMover)) return;

	ar >> pMover->m_nJob >> pMover->m_jobSkills;

	CreateSfx( XI_GEN_LEVEL_UP01, pMover->GetPos(), pMover->GetId());

	if (pMover->m_pActMover && (pMover->m_pActMover->IsState(OBJSTA_STAND) || pMover->m_pActMover->IsState(OBJSTA_STAND2)))
		pMover->SetMotion(MTI_LEVELUP, ANILOOP_1PLAY, MOP_FIXED);
}

void CDPClient::OnSetDestPos( OBJID objid, CAr & ar )
{
//	TRACE( "OnSetDestPos()\n" );
	D3DXVECTOR3 vPos;
	BYTE fForward;
	ar >> vPos >> fForward;

#ifdef __IAOBJ0622
	CShip* pIAObj	= NULL;
	OBJID objidIAObj;
	ar >> objidIAObj;
	if( objidIAObj != NULL_ID )
	{
		pIAObj	= (CShip*)prj.GetCtrl( objidIAObj );
		if( IsValidObj( (CObj*)pIAObj ) == FALSE )
		{
			Error( "CShip object ID %d not found", objidIAObj );
			return;
		}
	}
#endif	// __IAOBJ0622

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->m_pActMover->DefaultSet();
		pMover->BehaviorActionForceSet();	// 명령이 행동 명령이면 강제 동기하고, 무빙 명령이면 무시한다.
#ifdef __IAOBJ0622
		if( pIAObj )
			pMover->SetDestPos( pIAObj, vPos );
		else
			pMover->SetDestPos( vPos, (fForward != 0), FALSE );
#else	// __IAOBJ0622
		pMover->SetDestPos( vPos, (fForward != 0), FALSE );
#endif	// __IAOBJ0622
	}
}

void CDPClient::OnSetDestAngle( OBJID objid, CAr & ar )
{
	float fDestAngle;
	BYTE fLeft;
	ar >> fDestAngle >> fLeft;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
		pMover->SetDestAngle( fDestAngle, (fLeft != 0) );
}

void CDPClient::OnSetMovePattern( OBJID objid, CAr & ar )
{
	FLOAT fAngle, fAngleX;
	D3DXVECTOR3 vPos;
	int		nPattern;
	
	ar >> nPattern >> vPos >> fAngle >> fAngleX;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->SetMovePattern( nPattern );
		pMover->SetPos( vPos );
		pMover->SetAngle( fAngle );
		pMover->SetAngleX( fAngleX );
#ifdef __XUZHU
		DWORD dwColor = xRand();
		g_WndMng.PutString( "이동패턴 도착", NULL, dwColor );
#endif
	}
}

void CDPClient::OnMeleeAttack( OBJID objid, CAr & ar )
{
	DWORD dwAtkMsg;
	OBJID id;
	int	nParam2, nParam3;
	ar >> dwAtkMsg >> id >> nParam2 >> nParam3;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( pMover->SendActMsg( (OBJMSG)dwAtkMsg, id, nParam2, nParam3 ) == 0 )
			pMover->m_pActMover->m_qMeleeAtkMsg.AddTail(ACTMSG( (OBJMSG)dwAtkMsg, id, nParam2, nParam3 ));
	}
}

void CDPClient::OnMeleeAttack2( OBJID objid, CAr & ar )
{
	DWORD dwAtkMsg;
	OBJID id;
	int	nParam2, nParam3;
	ar >> dwAtkMsg >> id >> nParam2 >> nParam3;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( pMover->SendActMsg( (OBJMSG)dwAtkMsg, id, nParam2, nParam3 ) == 0 )
		{
			pMover->m_pActMover->m_qMeleeAtkMsg.AddTail(ACTMSG( (OBJMSG)dwAtkMsg, id, nParam2, nParam3 ));
		}
	}
}


void CDPClient::OnMagicAttack( OBJID objid, CAr & ar )
{
	DWORD dwAtkMsg;
	OBJID id;
	int	nParam2, nParam3, nMagicPower, idSfxHit;
	ar >> dwAtkMsg >> id >> nParam2 >> nParam3 >> nMagicPower >> idSfxHit;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		CCtrl* pTarget		= prj.GetCtrl( id );
		if( IsValidObj( pTarget ) ) 
		{
			if( pMover->DoAttackMagic( pTarget, nMagicPower, idSfxHit ) == 0 )
				pMover->m_pActMover->m_qMagicAtkMsg.AddTail(MAGICATKMSG( (OBJMSG)dwAtkMsg, id, nParam2, nParam3, nMagicPower, idSfxHit ));
		}
	}
}

void CDPClient::OnRangeAttack( OBJID objid, CAr & ar )
{
	DWORD dwAtkMsg;
	OBJID id;
	int nParam2, nPower;
	int	idSfxHit;
	DWORD dwItemID;

	ar >> dwAtkMsg >> id >> nParam2 >> nPower >> idSfxHit;
	dwItemID = nParam2;	// NULL_ID나 0이 올수 있음.
	CMover* pMover	= prj.GetMover( objid );
	CMover* pTarget = prj.GetMover( id );

	if( IsValidObj( pMover ) )
	{
		if( IsValidObj( pTarget ) ) 
		{
			if( pMover->DoAttackRange( pTarget, dwItemID, idSfxHit ) == 0 )
				pMover->m_pActMover->m_qMagicAtkMsg.AddTail(MAGICATKMSG((OBJMSG)dwAtkMsg, id, 1, dwItemID, 0, idSfxHit ));
		}
	}
}

void CDPClient::OnAttackSP( OBJID objid, CAr & ar )
{
	DWORD dwAtkMsg;
	OBJID id;
	int nParam2, nParam3;
	DWORD dwItemID;
	
	ar >> dwAtkMsg >> id >> nParam2 >> nParam3;
	dwItemID = nParam2;	// NULL_ID나 0이 올수 있음.
	CMover* pMover	= prj.GetMover( objid );
	CMover* pTarget = prj.GetMover( id );
	if( IsValidObj( pMover ) ) 
	{
		pMover->DoAttackSP( pTarget, dwItemID );
	}
}

void CDPClient::OnUseSkill( OBJID objid, CAr & ar )
{
	OBJID id;
	int nUseType = 0;
	int nCastingTime = 0;
	DWORD dwSkill, dwLevel;
	ar >> dwSkill >> dwLevel;
	ar >> id >> nUseType >> nCastingTime;
	TRACE( "OnUseSkill: %d\n", dwLevel );
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->m_dwReqFlag &= (~REQ_USESKILL);	// 응답 받았음.
		BOOL bSuccess = pMover->DoUseSkillPre(-1, id, (SKILLUSETYPE)nUseType, FALSE, nCastingTime, dwSkill, dwLevel );
		if( bSuccess )
		{
			TRACE( "OnUseSkill성공 %d, ", dwSkill );
			
			if( g_Option.m_bAutoAttack && pMover->IsActiveMover() )		// 여기야여기 - 자동공격 ON
			{
				const ItemProp *pItemProp = g_pPlayer->GetActiveHandItemProp();
				if( pItemProp )
				{   
					// 손에 들고 있는게 스태프나 치어스틱이 아닐때만 자동공격.
					if( pItemProp->dwItemKind3 != IK3_STAFF && pItemProp->dwItemKind3 != IK3_CHEERSTICK && pItemProp->dwItemKind3 != IK3_WAND )
					{
						ItemProp* pSkillProp;
						pSkillProp = prj.GetSkillProp( dwSkill );
						//자기 자신에게 시전하는 스킬일 경우 타겟이 있으면 자동공격 시작되는 문제 수정.
						if( g_WndMng.m_pWndWorld && pSkillProp && pSkillProp->dwExeTarget != EXT_SELFCHGPARAMET && 
							dwSkill != SI_KNT_HERO_DRAWING ) //영웅 나이트의 리콜 스킬은 공격 스킬아님.
							//pSkillProp->dwUseChance == WUI_NOW && dwSkill != SI_KNT_HERO_DRAWING ) //영웅 나이트의 리콜 스킬은 공격 스킬아님.
						{
							g_WndMng.m_pWndWorld->m_bAutoAttack = TRUE;		// 스킬공격 시작되면 자동공격도 시작.
						}
					}
				}
			}
		} 
		else
		{
			TRACE( "OnUseSkill실패 %d, ", dwSkill );
		}
	}
}

void CDPClient::OnClearUseSkill( OBJID objid )
{
	CMover *pMover = prj.GetMover( objid );
	if( IsInvalidObj(pMover) )	return;

	pMover->m_dwReqFlag &= (~REQ_USESKILL);	// 응답 받았음.
	if( pMover->IsActiveMover() )		// g_pPlayer일경우 
	{
		CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
		pTaskBar->m_nExecute = 0;		// 실제 스킬바 사용중으로 전환.
		g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
	}
}


void CDPClient::OnSetFame( OBJID objid, CAr & ar )
{
	int nFame;
	ar >> nFame;
	
	CMover* pPlayer	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPlayer ) ) 
	{
		pPlayer->m_nFame = nFame;
	}
}

void CDPClient::OnSetFuel( OBJID objid, CAr & ar )
{
	int nFuel;
	int	tmAccFuel;
	ar >> nFuel >> tmAccFuel;
	
	CMover* pPlayer	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPlayer ) ) 
	{
		pPlayer->m_nFuel = nFuel;
		if( nFuel == 0 )	// 연료가 없으면 제자리에 멈춤
			pPlayer->SendActMsg( OBJMSG_ACC_STOP );	
		pPlayer->m_tmAccFuel = tmAccFuel;
	}
}



void CDPClient::OnCreateSfxObj( OBJID objid, CAr & ar )
{
	DWORD dwSfxObj;
	float	x, y, z;
	BOOL	bFlag;				// 0이면 고정 1이면 따라다니기
	ar >> dwSfxObj >> x >> y >> z;
	ar >> bFlag;

	D3DXVECTOR3 vWorld( x, y, z );		// 이 좌표가 지정되어 있다면 여기다 sfx를 생성.
	D3DXVECTOR3 vPos;

	CCtrl* pObj	= prj.GetCtrl( objid );
	if( IsValidObj( (CObj*)pObj ) ) 
	{
		OBJID idObj = pObj->GetId();
		if( x || y || z )		// 절대좌표가 있으면 그곳을 생성 위치로 한다.
		{
			idObj = NULL_ID;
			vPos = vWorld;
		}
		else
			vPos = pObj->GetPos();	// 없으면 오브젝트 좌표로 한다.
		CreateSfx( dwSfxObj, vPos, idObj, vPos, idObj );
	}
	else
	{
		if( x || y || z )		// 절대좌표가 있으면 그곳을 생성 위치로 한다.
			CreateSfx( dwSfxObj, vWorld, NULL_ID, vWorld, NULL_ID );
	}
}

void CDPClient::OnRemoveSfxObj( OBJID objid, CAr & ar )
{
	DWORD dwSfxObj;
	float	x, y, z;
	BOOL	bFlag;				// 0이면 고정 1이면 따라다니기
	ar >> dwSfxObj >> x >> y >> z;
	ar >> bFlag;

//	D3DXVECTOR3 vWorld( x, y, z );		// 
//	D3DXVECTOR3 vPos;

	CMover* pObj	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pObj ) ) 
	{
		pObj->SetRemoveSfxObj( dwSfxObj );
	}
}
void CDPClient::OnCreateSfxAllow( OBJID objid, CAr & ar )
{
	TRACE( "ON RANGE ATTACK\n" );
	DWORD dwSfxObjAllow;
	DWORD dwSfxObjHit;
	D3DXVECTOR3 v3DestPos;
	int   idTarget;
	ar >> dwSfxObjAllow >> dwSfxObjHit >> v3DestPos.x >> v3DestPos.y >> v3DestPos.z >> idTarget;
	
	CCtrl* pObj	= prj.GetCtrl( objid );
	if( IsValidObj( (CObj*)pObj ) ) 
	{
		CMover* pHit = prj.GetMover( idTarget );	// 타겟의 포인터를 얻어냄.
		
		if( IsInvalidObj(pHit) )	
			return;						

		CMover* pMover = (CMover*)pObj;

		D3DXVECTOR3 vPosDest = pHit->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // 발사 지점을 임의로 올려준다. 땜빵 
		D3DXVECTOR3 vPosSrc  = pMover->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // 발사 지점을 임의로 올려준다. 땜빵 

		CSfx* pSfx = NULL;

		pSfx = pMover->CreateSfxArrow( dwSfxObjAllow, dwSfxObjHit, vPosDest, idTarget );
	}
}


void CDPClient::OnDefinedText( CAr & ar )
{
	TCHAR lpString[1024], argv[32][256], *pargv[32];
	int argc	= 0;

	DWORD dwText;
	ar >> dwText;
	ar.ReadString( lpString, 1024 );

	if( dwText == TID_GAME_REAPMONEY )
	{
		//예외처리: 돈 문자열에 ','가 들어가서 CScanner로 작업하기 원할치 않다.
		CString rString;
		AfxExtractSubString( rString, lpString, 0, ' ' );
		strcpy( argv[0], rString );
		AfxExtractSubString( rString, lpString, 1, ' ' );
		strcpy( argv[1], rString );
		argc = 2;
	}
	else
	{
		CScanner s( lpString );
		s.GetToken();
		while( s.tok != FINISHED )
		{
			s.Token.Replace( " @", "" );
			_tcscpy( argv[argc], s.Token );
			argc++;
			s.GetToken();
		}
	}

	for( int i = 0; i < argc; i++ )
	{
		pargv[ i ] = argv[ i ];
	}

	CString string;
	string.FormatV( prj.GetText( dwText ), (va_list) pargv );



	g_WndMng.PutString( (LPCTSTR)string, NULL, prj.GetTextColor( dwText ) );
}

void CDPClient::OnChatText( CAr & ar )
{
	TCHAR lpString[1024], *pargv[32];
	memset( pargv, 0, sizeof(pargv) );
	int argc	= 0;
	
	DWORD dwText;
	ar >> dwText;
	ar.ReadString( lpString, 1024 );	
}

void CDPClient::OnDefinedText1( CAr & ar )
{
	DWORD dwText;
	ar >> dwText;

	CString strText	= GETTEXT( dwText );
	
	DWORD dwColor	= prj.GetTextColor( dwText );
	g_WndMng.PutString( (LPCTSTR)strText, NULL, dwColor );
}

void CDPClient::OnGameTimer( CAr & ar )
{
	double dCurrentTime;
	ar >> dCurrentTime;

	g_GameTimer.SetCurrentTime( dCurrentTime );

	for( int i = 0 ; i < MAX_SKILL ; i++ )
	{
		g_WndMng.m_dwSkillTime[i] = 0;
	}
	
}

void CDPClient::OnGameJoin( CAr & ar )
{
	ar >> g_WndMng.m_dwSavePlayerTime;
//	dwSavePlayerTime /= 60;
	CString strMessage;
//	strMessage.Format( prj.GetText( TID_GAME_TOOLTIP_PLAYTIME ), dwSavePlayerTime / 60, dwSavePlayerTime % 60 );
//	g_WndMng.PutString( strMessage, NULL, 0xff0000 );

	if (g_pPlayer->m_quests) {
		for (const QUEST & quest : g_pPlayer->m_quests->current) {
			if (quest.m_nState == QS_END) continue;

			const QuestProp * pQestProp = quest.GetProp();
			if (!pQestProp) continue;

			strMessage.Format(prj.GetText(TID_GAME_TOOLTIP_ROLEQUEST), pQestProp->m_szTitle);
			g_WndMng.PutString(strMessage, NULL, 0xffff0000);
		}
	}
	
	strMessage.Format( prj.GetText( TID_GAME_TOOLTIP_PRELEVEL ) , g_pPlayer->GetLevel() );
	g_WndMng.PutString( strMessage, NULL, 0xffff0000 );	
}

void CDPClient::OnTaskBar(CAr & ar) {
	ar >> (*g_WndMng.m_pWndTaskBar);
	g_WndMng.m_pWndTaskBar->UpdateAllTaskbarTexture();
}

void CDPClient::OnErrorParty( CAr & ar )
{
	DWORD dw;
	ar >> dw;
	switch( dw ) {
	case ERROR_NOLEADER:
		g_WndMng.PutString(TID_GAME_PARTYNOINVATE);
		break;
	case ERROR_FULLPARTY:
		{
			LPCTSTR szMessageBuf;
			if (1 <= g_Party.GetLevel() && g_Party.GetLevel() <= 5) {
				szMessageBuf = prj.GetText(TID_GAME_FULLPARTY1);
			} else if (6 <= g_Party.GetLevel() && g_Party.GetLevel() <= 9) {
				szMessageBuf = prj.GetText(TID_GAME_FULLPARTY2);
			} else {
				szMessageBuf = prj.GetText(TID_GAME_FULLPARTY3);
			}

			g_WndMng.PutString( szMessageBuf, NULL, 0xff99cc00 );
		}
		break;
	case ERROR_NOPARTY:
		g_WndMng.PutString(TID_GAME_NOPARTY);
		break;
	case ERROR_DIFFRENTPARTY:
		g_WndMng.PutString(TID_GAME_DIFFRENTPARTY);
		break;
	case ERROR_DIFFERNTUSERNAME:
		{
			SAFE_DELETE( g_WndMng.m_pWndPartyChangeName );
			g_WndMng.m_pWndPartyChangeName = new CWndPartyChangeName;
			g_WndMng.m_pWndPartyChangeName->Initialize();
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0033) ) );
		}
		break;
	case ERROR_NOTPARTYPOINT:
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0046) ) );
		break;
	case ERROR_NOTPARTYSKILL:
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0019) ) );
		break;
	case ERROR_NOTTARGET:
		{
			DWORD dwSkill;
			ar >> dwSkill;
			if( dwSkill == ST_BLITZ )
			{
				ItemProp * pItemProp  = prj.GetPartySkill( ST_BLITZ );
				g_WndMng.m_dwSkillTime[ST_BLITZ] = timeGetTime();	// + pItemProp->dwCircleTime;
			}
			else if( dwSkill == ST_SPHERECIRCLE )
			{
				ItemProp * pItemProp  = prj.GetPartySkill( ST_SPHERECIRCLE );
				g_WndMng.m_dwSkillTime[ST_SPHERECIRCLE] = timeGetTime();	// + pItemProp->dwCircleTime;
				 
			}
			g_WndMng.PutString(TID_GAME_NOTTARGET);
		}
		break;
	case ERROR_SAMLEADER:
		g_WndMng.PutString(TID_GAME_PARTY_ALREADYMASTER);
		break;
	case ERROR_NOTMAMBER:
		g_WndMng.PutString(TID_GAME_PARTYNOTEXMEMBER);
		break;
	default:
		break;
	}
}

void CDPClient::OnAddPartyMember( CAr & ar )
{
	u_long uLeaderId = g_Party.m_aMember[0].m_uPlayerId;
	u_long uPlayerId;
	int nSizeofMember;
	ar >> uPlayerId;
	char pszLeader[MAX_PLAYER]	= { 0,};
	char pszMember[MAX_PLAYER]	= { 0,};
	ar.ReadString( pszLeader, MAX_PLAYER );
	ar.ReadString( pszMember, MAX_PLAYER );
	ar >> nSizeofMember;
	int nOldSize	= g_Party.GetSizeofMember();


	if( nSizeofMember != 0 )
	{
		ar >> g_Party;

		CString sMessage;
		if( nOldSize == 0 && nSizeofMember == 2 )	// new
		{
			if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) )	// leader
			{
				g_WndMng.PutString( prj.GetText(TID_GAME_PARTYCREATE), NULL, prj.GetTextColor(TID_GAME_PARTYCREATE) );
				sMessage.Format( prj.GetText( TID_GAME_PARTYJOIN1 ), pszMember );
			}
			else	// member
			{
				sMessage.Format( prj.GetText( TID_GAME_PARTYJOIN2 ), pszLeader );
			}
			PlayMusic( BGM_IN_FITUP );
		}
		else if( nOldSize < nSizeofMember )		// add
		{
			sMessage.Format( prj.GetText(TID_GAME_PARTYJOIN1), pszMember );
		}
		else if( nOldSize > nSizeofMember )		// remove
		{
			sMessage.Format( prj.GetText(TID_GAME_PARTYREMOVE), pszMember );
		}
		g_WndMng.PutString( sMessage, NULL, 0xff99cc00 );

		if( g_Party.m_idDuelParty )
		{
			g_pPlayer->m_idDuelParty	= g_Party.m_idDuelParty;	
			g_pPlayer->m_nDuel	= 2;
		}
	}
	else
	{
		g_Party.InitParty();
		g_pPlayer->m_idparty	= 0;
		if( g_pPlayer->m_idPlayer == uPlayerId )
			g_WndMng.PutString( prj.GetText(TID_GAME_PARTYEXIT), NULL, prj.GetTextColor(TID_GAME_PARTYEXIT) );
		else
			g_WndMng.PutString( prj.GetText(TID_GAME_PARTYDESTROY), NULL, prj.GetTextColor(TID_GAME_PARTYDESTROY) );

		CWndWorld *pWndWorld = (CWndWorld *)g_WndMng.m_pWndWorld;
		pWndWorld->m_buffs.Clear();
	}

	if( !g_Party.IsLeader( uLeaderId ) )
		memset( g_WndMng.m_dwSkillTime, 0, sizeof(DWORD) * MAX_SKILL );
}

void CDPClient::OnPartyRequest( CAr & ar ) {
	u_long uLeader;
	char szLeaderName[MAX_PLAYER];
	
	ar >> uLeader;
	ar.ReadString(szLeaderName);
	
	if (g_Option.m_bParty == FALSE) {
		SendPartyMemberCancle(uLeader, 3);
		return;
	}

#ifdef __S_SERVER_UNIFY
	if( g_WndMng.m_bAllAction == FALSE )
		return;
#endif // __S_SERVER_UNIFY

	CWndBase* pWndBaseBuf = g_WndMng.GetWndBase( APP_PARTY_CONFIRM );
	
	if( pWndBaseBuf )
	{
		SendBlock( 7, g_pPlayer->GetName(), szLeaderName );
		return;
	}

	g_WndMng.m_pWndPartyConfirm = new CWndPartyConfirm(uLeader, szLeaderName);
	g_WndMng.m_pWndPartyConfirm->Initialize();
}

void CDPClient::OnPartyRequestCancel( CAr & ar )
{
	const auto [uMember, nMode] = ar.Extract<u_long, int>();

	CString sMessage;
	switch( nMode ) {
	case 0:
		{
			CMover *pMember = prj.GetUserByID( uMember );
			if( IsValidObj( (CObj*)pMember ) )
			{
				//sMessage = pMember->GetName();
				//sMessage += " 님이 극단 참여를 거부하였습니다.";
				sMessage.Format( prj.GetText(TID_GAME_PARTYFAILINVATE), pMember->GetName() );
			}
			break;
		}
	case 1:
		{
			CMover *pMember = prj.GetUserByID( uMember );
			if( IsValidObj( (CObj*)pMember ) )
			{
				if( g_Party.FindMember( uMember ) == -1 )
				{
					sMessage.Format( prj.GetText(TID_GAME_PARTYOTHER), pMember->GetName() );
				}
				else
				{
					sMessage.Format( prj.GetText(TID_GAME_PARTYEXISTCHR), pMember->GetName() );
				}
			}
			break;
		}
	case 2:
		{
			sMessage = prj.GetText(TID_GAME_PARTYFAILENTER);//"극단 참여에 실패하였습니다.";
			break;
		}
	case 3:
		{
			CMover *pMember = prj.GetUserByID( uMember );
			if( IsValidObj( (CObj*)pMember ) )
			{
				sMessage.Format( prj.GetText(TID_GAME_PARTYDENY), pMember->GetName() );
			}
			break;
		}
	case 4:
		{
			sMessage = prj.GetText(TID_GAME_PARTYINVATEOTHERSVR);//"다른서버에 있어서 참여 시킬수가 없습니다";
			break;
		}
	default:
		break;
	}

	g_WndMng.PutString( sMessage, NULL, 0xff99cc00 );
}

void CDPClient::OnDuelCount( CAr & ar )
{
	int nCount;
	ar >> nCount;	
	g_Neuz.m_nPVPCount = nCount;
	g_Neuz.m_dwPVPTime = GetTickCount();
}

void CDPClient::OnDuelRequest( CAr & ar )
{
	u_long uidSrc, uidDst;
	ar >> uidSrc >> uidDst;
	
	// 신청하는 형태의 매시지 박스는 여러개가 날아올 수 있고, 내 의지와는 관계가 없다.
	// 이런 경우 CWndDuelConfirm::OnChildNotify에서 윈도를 닫을 때 Destroy( TRUE )로 해서
	// 메모리에서 완전히 파괴하도록 한다. 안하면 리크로 남던지, ALT+ENTER, ALT+TAB할 때 다운됨.
	CMover *pOffer = prj.GetUserByID( uidSrc );	// 신청자의 포인터 받아옴
	if( IsValidObj(pOffer) && pOffer->IsDie() == FALSE )
	{
		SAFE_DELETE( g_WndMng.m_pWndDuelConfirm );
		g_WndMng.m_pWndDuelConfirm = new CWndDuelConfirm;
		strcpy( g_WndMng.m_pWndDuelConfirm->m_szSrc, pOffer->GetName() );		// 듀얼 신청자의 이름을 넣음.
		g_WndMng.m_pWndDuelConfirm->m_idSrc = pOffer->GetId();				// 듀얼 신청자의 아이디를 넣음.
		g_WndMng.m_pWndDuelConfirm->Initialize();
	}
//	pDuelConfirm->SetMember( uidSrc, uidDst );
}

// 듀얼 시작!
void CDPClient::OnDuelStart( CAr & ar )
{
	int nFlag = 0;
	u_long uidTarget;
	ar >> uidTarget;
	
	ar >> nFlag;
	
	SAFE_DELETE( g_WndMng.m_pWndPartyConfirm );


	CString str;
	CMover *pTarget = prj.GetUserByID( uidTarget );		// 듀얼 대상자의 포인터
	if( IsValidObj(pTarget) )
	{
		switch( nFlag )
		{
		case 0:
			{
				g_pPlayer->m_nDuel = 1;		// 듀얼시작을 표시
				g_pPlayer->m_idDuelOther = pTarget->GetId();	// 듀얼 상대의 아이디 기록해둠.
				pTarget->m_nDuel = 1;						// 듀얼상대방에도 정보 기록함.
				pTarget->m_idDuelOther = g_pPlayer->GetId();
				g_pPlayer->m_nDuelState = 104;
				pTarget->RenderFlag( ENEMY_PVP );
			}
			break;
		case 1:
			{
				g_pPlayer->m_nDuelState = 1;
				str.Format( prj.GetText( TID_GAME_DUELSTART ), pTarget->GetName() );
				g_WndMng.PutString( str, NULL, prj.GetTextColor( TID_GAME_DUELSTART ) );			
			}
			break;
		case 2:
			{
				g_pPlayer->ClearDuel();
				pTarget->ClearDuel();
			}
			break;
		case 3:		//win
			{
				// 신청하는 형태의 매시지 박스는 여러개가 날아올 수 있고, 내 의지와는 관계가 없다.
				// 이런 경우 CWndDuelConfirm::OnChildNotify에서 윈도를 닫을 때 Destroy( TRUE )로 해서
				// 메모리에서 완전히 파괴하도록 한다. 안하면 리크로 남던지, ALT+ENTER, ALT+TAB할 때 다운됨.
				if( IsValidObj(pTarget) && pTarget->IsDie() == FALSE )
				{
					SAFE_DELETE( g_WndMng.m_pWndDuelConfirm );
					g_WndMng.m_pWndDuelResult = new CWndDuelResult;
					strcpy( g_WndMng.m_pWndDuelResult->m_szSrc, pTarget->GetName() );		// 듀얼 신청자의 이름을 넣음.
					g_WndMng.m_pWndDuelResult->m_idSrc = pTarget->GetId();				// 듀얼 신청자의 아이디를 넣음.
					g_WndMng.m_pWndDuelResult->m_bDuelWin = TRUE;	// 창은 같은걸 공유한다.
					g_WndMng.m_pWndDuelResult->Initialize();
				}
			}
			break;
		case 4:		//lose
			{
				// 신청하는 형태의 매시지 박스는 여러개가 날아올 수 있고, 내 의지와는 관계가 없다.
				// 이런 경우 CWndDuelConfirm::OnChildNotify에서 윈도를 닫을 때 Destroy( TRUE )로 해서
				// 메모리에서 완전히 파괴하도록 한다. 안하면 리크로 남던지, ALT+ENTER, ALT+TAB할 때 다운됨.
				if( IsValidObj(pTarget) && pTarget->IsDie() == FALSE )
				{
					SAFE_DELETE( g_WndMng.m_pWndDuelConfirm );
					g_WndMng.m_pWndDuelResult = new CWndDuelResult;
					strcpy( g_WndMng.m_pWndDuelResult->m_szSrc, pTarget->GetName() );		// 듀얼 신청자의 이름을 넣음.
					g_WndMng.m_pWndDuelResult->m_idSrc = pTarget->GetId();				// 듀얼 신청자의 아이디를 넣음.
					g_WndMng.m_pWndDuelResult->m_bDuelWin = FALSE;	// 창은 같은걸 공유한다.
					g_WndMng.m_pWndDuelResult->Initialize();
				}
			}
			break;
		}
	}
}

// idTarget이 듀얼을 거절했음
void CDPClient::OnDuelNo( CAr & ar )
{
	OBJID	idTarget;
	ar >> idTarget;
	
	CMover *pTarget = prj.GetMover( idTarget );		// 듀얼 대상자의 포인터
	if( IsValidObj( pTarget ) )
	{
		CString str;
		//str = pTarget->GetName();
		//str += "님이 듀얼신청을 거절했습니다";
		//g_WndMng.PutString( str, NULL, 0xff99cc00 );

		str.Format( prj.GetText( TID_GAME_DUELNO ), pTarget->GetName() );
		g_WndMng.PutString( str, NULL, prj.GetTextColor( TID_GAME_DUELNO ) );
	}
}

// 듀얼중 취소시킴.
void CDPClient::OnDuelCancel( CAr & ar )
{
	OBJID	idTarget;
	ar >> idTarget;
	
	CMover *pMover1 = g_pPlayer;		// 결투자 1
	CMover *pTarget = prj.GetMover( idTarget );		// 결투자 2
	if( IsValidObj(pMover1) )
	{
		pMover1->ClearDuel();
		TRACE( "m_nDuel		= 0		// OnDuelCancel\n" );
	}
	if( IsValidObj(pTarget) )
		pTarget->ClearDuel();

	g_WndMng.PutString( prj.GetText(TID_GAME_DUELCANCEL), NULL, prj.GetTextColor(TID_GAME_DUELCANCEL) );
}

// 파티 듀열------------------------------------------------------------
void CDPClient::OnDuelPartyRequest( CAr & ar )
{
	u_long uidSrc, uidDst;
	ar >> uidSrc >> uidDst;
	
	// 신청하는 형태의 매시지 박스는 여러개가 날아올 수 있고, 내 의지와는 관계가 없다.
	// 이런 경우 CWndDuelConfirm::OnChildNotify에서 윈도를 닫을 때 Destroy( TRUE )로 해서
	// 메모리에서 완전히 파괴하도록 한다. 안하면 리크로 남던지, ALT+ENTER, ALT+TAB할 때 다운됨.
	CMover *pOffer = prj.GetUserByID( uidSrc );	// 신청자의 포인터 받아옴
	if( IsValidObj(pOffer) && pOffer->IsDie() == FALSE )
	{
		SAFE_DELETE( g_WndMng.m_pWndDuelConfirm );
		g_WndMng.m_pWndDuelConfirm = new CWndDuelConfirm;
		strcpy( g_WndMng.m_pWndDuelConfirm->m_szSrc, pOffer->GetName() );		// 듀얼 신청자의 이름을 넣음.
		g_WndMng.m_pWndDuelConfirm->m_idSrc = pOffer->GetId();				// 듀얼 신청자의 아이디를 넣음.
		g_WndMng.m_pWndDuelConfirm->m_bDuelParty = TRUE;	// 창은 같은걸 공유한다.
		g_WndMng.m_pWndDuelConfirm->Initialize();
	}
	
	
	//	pDuelConfirm->SetMember( uidSrc, uidDst );
}

// 듀얼 시작!
void CDPClient::OnDuelPartyStart( CAr & ar )
{
//	u_long idDstParty;
//	ar >> idDstParty;
	int		nMax, i;
	OBJID	pDstMembers[ MAX_PTMEMBER_SIZE ];
	u_long	idDstParty;
	TCHAR	szPartyName[ 64 ];
	memset( szPartyName, 0, sizeof(szPartyName) );
	ar >> nMax;
	ar >> idDstParty;
	ar.ReadString( szPartyName, 64 );		// 상대방 파티 이름.
	for( i = 0; i < nMax; i ++ )
		ar >> pDstMembers[i];

	int nFlag = 0;
	ar >> nFlag;

	// 결투상대 파티를 받으면 그 멤버들의 CMover를 조사해서 m_idparty에 값을 세팅해주자.
	CMover *pMember;
	for( i = 0; i < nMax; i ++ )
	{
		pMember = prj.GetMover( pDstMembers[i] );
		if( IsValidObj( pMember ) )
		{
			pMember->m_idparty = idDstParty;
			pMember->m_nDuel = 2;
			pMember->m_idDuelParty = g_Party.m_uPartyId;		// 내 파티id를 상대 파티원들에게 심어주고 듀얼시작 알림.

			if( nFlag == 1 )
			{
				pMember->m_nDuelState = 1;
			}
			else if( nFlag == 0 )
			{
				// 결투상대라는 sfx를 머리위에 띄워줌. - 같은 코드가 OnAddObj()에도 있다.
				pMember->RenderFlag( ENEMY_PVP_PARTY );
				pMember->m_nDuelState = 104;
			}
		}
	}
	g_pPlayer->m_idparty = g_Party.m_uPartyId;
	g_pPlayer->m_nDuel = 2;						// g_pPlayer도 듀얼 시작됨을 세팅
	g_pPlayer->m_idDuelParty = idDstParty;
	
	g_Party.m_idDuelParty = idDstParty;			// g_pPlayer의 파티에도 세팅.

	if( nFlag == 1 )
	{
		g_pPlayer->m_nDuelState = 1;
		CString str;
		str.Format( prj.GetText( TID_GAME_PDSTART ), szPartyName );
		g_WndMng.PutString( str, NULL, prj.GetTextColor( TID_GAME_PDSTART ) );	
	}
	else if( nFlag == 0 )
	{
		g_pPlayer->m_nDuelState = 104;
	}
}

// idTarget이 듀얼을 거절했음
void CDPClient::OnDuelPartyNo( CAr & ar )
{
	OBJID	idTarget;
	ar >> idTarget;
	
	CMover *pTarget = prj.GetMover( idTarget );		// 듀얼 대상자의 포인터
	if( IsValidObj( pTarget ) )
	{
		//CString str;
		//str = pTarget->GetName();
		//str += "님이 듀얼신청을 거절했습니다";
		//g_WndMng.PutString( str, NULL, 0xff99cc00 );
		
		CString str;
		str.Format( prj.GetText(TID_GAME_DUELNO), pTarget->GetName() );
		g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_GAME_DUELNO) );
	}
}

// 듀얼중 취소시킴.
void CDPClient::OnDuelPartyCancel( CAr & ar )
{
	int		i, nSize;
	u_long	uPlayerID;
	CMover	*pOtherMember;
	ar >> nSize;		// 상대파티원 수
	for( i = 0; i < nSize; i ++ )
	{
		ar >> uPlayerID;
		pOtherMember = prj.GetUserByID( uPlayerID );		// 상대파티원 유저아이디로 CMover를 꺼냄
		if( IsValidObj( pOtherMember ) )
		{
			pOtherMember->ClearDuelParty();			// 여기 클라이언트의 상대파티원 CMover에도 값을 클리어시킴.
		}
	}
	g_pPlayer->ClearDuelParty();
	g_Party.m_idDuelParty = 0;

	g_WndMng.PutString( prj.GetText(TID_GAME_PDCANCEL), NULL, prj.GetTextColor(TID_GAME_PDCANCEL) );
}

void CDPClient::OnDuelPartyResult( CAr & ar )
{
	BOOL	bWin;
	ar >> bWin;

	// 클라에도 파티원들과 상대파티원들에게 듀얼끝났다고 세팅해야한다.
	int		i, nSize;
	u_long	uPlayerID;
	CMover	*pOtherMember;
	ar >> nSize;		// 상대파티원 수
	for( i = 0; i < nSize; i ++ )
	{
		ar >> uPlayerID;
		pOtherMember = prj.GetUserByID( uPlayerID );		// 상대파티원 유저아이디로 CMover를 꺼냄
		if( IsValidObj( pOtherMember ) )
		{
			pOtherMember->ClearDuelParty();			// 여기 클라이언트의 상대파티원 CMover에도 값을 클리어시킴.
		}
	}

	g_pPlayer->ClearDuelParty();
	g_Party.m_idDuelParty = 0;

	CString str;
	if( bWin )
	{
		//str = "극단듀얼에서 승리했습니다.";
		g_WndMng.PutString( prj.GetText(TID_GAME_PDVICTORY), NULL, prj.GetTextColor(TID_GAME_PDVICTORY) );
	} 
	else
	{
		//str = "극단듀얼에서 패배했습니다.";
		g_WndMng.PutString( prj.GetText(TID_GAME_PDLOSE), NULL, prj.GetTextColor(TID_GAME_PDLOSE) );
	}
}


void CDPClient::OnPartyExpLevel( CAr & ar )
{
	int nLevel;
	ar >> g_Party.m_nExp >> nLevel >> g_Party.m_nPoint;
	if( nLevel > g_Party.m_nLevel )
	{
		if( nLevel == MAX_PARTYLEVEL )
			g_WndMng.PutString( prj.GetText(TID_GAME_TROUPEUP), NULL, prj.GetTextColor(TID_GAME_TROUPEUP) );
		else
			g_WndMng.PutString( prj.GetText(TID_GAME_PARTYLUP), NULL, prj.GetTextColor(TID_GAME_PARTYLUP) ); // "극단의 레벨이 올랐습니다."
	}
	g_Party.m_nLevel = nLevel;	
}

void CDPClient::OnPartyChangeLeader(CAr & ar) {
	u_long uidChangeLeader; ar >> uidChangeLeader;
	g_Party.ChangeLeader(uidChangeLeader);

	g_WndMng.m_pWndWorld->m_buffs.Clear();
	for (int i = 0; i < MAX_SKILL; i++) {
		g_WndMng.m_dwSkillTime[i] = 0;
	}
}

void CDPClient::OnSetPartyMode( CAr & ar )
{
	/*
		#define ST_CALL                            1
		#define ST_BLITZ                           2
		#define ST_RETREAT                         3
		#define ST_SCOUT                           4
		#define ST_SPHERECIRCLE                    5
		#define ST_LINKATTACK                      6
		#define ST_FORTUNECIRCLE                   7
		#define ST_STRETCHING                      8
		#define ST_GIFTBOX                         9
	 */
	int nMode;
	BOOL bOnOff;
	DWORD	dwSkillTime;
	ar >> nMode >> dwSkillTime >> bOnOff;
	if( bOnOff == TRUE )
	{
		ar >> g_Party.m_nPoint;
	}
	g_Party.m_nModeTime[nMode] = bOnOff;
	CString csBufString;

	ItemProp* pItemProp = NULL;

	switch( nMode ) {
	case PARTY_LINKATTACK_MODE:
		{
			pItemProp = prj.GetPartySkill( ST_LINKATTACK );
			
			if( bOnOff )
			{			
				if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
				{
					D3DXVECTOR3 v = g_pPlayer->GetPos();
					
					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, NULL_ID, D3DXVECTOR3(0,0,0), g_pPlayer->GetId() );	// 머리위에 sfx생성.

					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(0.8f, 0.8f, 0.8f) );
				}
				else
				{
					u_long idPlayer = g_Party.GetPlayerId( 0 );
					CMover* pMover = prj.GetUserByID( idPlayer );
					
					if( !IsValidObj(pMover) )
						break;
					
					D3DXVECTOR3 v = pMover->GetPos();
					
					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, pMover->GetId() );	// 머리위에 sfx생성.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(0.8f, 0.8f, 0.8f) );
					
				}
			}

			csBufString = pItemProp->szName;
		}
		break;
	case PARTY_FORTUNECIRCLE_MODE:
		{
			pItemProp = prj.GetPartySkill( ST_FORTUNECIRCLE );

			if( bOnOff )
			{

				if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
				{
					D3DXVECTOR3 v = g_pPlayer->GetPos();
					
					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, NULL_ID, D3DXVECTOR3(0,0,0), g_pPlayer->GetId() );	// 머리위에 sfx생성.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(0.7f, 0.7f, 0.7f) );
				}
				else
				{
					u_long idPlayer = g_Party.GetPlayerId( 0 );
					CMover* pMover = prj.GetUserByID( idPlayer );
					
					if( !IsValidObj(pMover) )
						break;
					
					D3DXVECTOR3 v = pMover->GetPos();
					
					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, pMover->GetId() );	// 머리위에 sfx생성.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(0.7f, 0.7f, 0.7f) );
				}
			}

			csBufString = pItemProp->szName;
		}
		break;
	case PARTY_STRETCHING_MODE:
		{
			pItemProp = prj.GetPartySkill( ST_STRETCHING );
			
			// 단원들이 앉아있는 상태이면 SFX생성
			for( int i = 0 ; i < g_Party.GetSizeofMember() ; i++ )
			{
				u_long idPlayer = g_Party.GetPlayerId( i );
				CMover* pPlayerMover = prj.GetUserByID( idPlayer );
				
				if( !IsValidObj(pPlayerMover) )
					continue;

				if( pPlayerMover->m_pActMover->IsSit() )
				{
					D3DXVECTOR3 v = pPlayerMover->GetPos();
					
					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, pPlayerMover->GetId() );	// 머리위에 sfx생성.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
				}
			}
			
			// 앉아있는 상태가 아니면 앉기 메세지 날림
			// CActionMover::ProcessState1 에서 생성시킴

			csBufString = pItemProp->szName;

			if( bOnOff )
			{
				// 이메세지가 오면 앉게 하면 됨
				if( g_pPlayer->m_pActMover->IsStateFlag( OBJSTAF_SIT ) ) 
				{	// 앉아있는 상태일때
				}
				else {														// 앉은상태가 아닐땐
					if( FALSE == g_pPlayer->m_pActMover->IsActJump() &&
						FALSE == g_pPlayer->m_pActMover->IsActAttack() && 
						FALSE == g_pPlayer->m_pActMover->IsActDamage() &&
						FALSE == g_pPlayer->m_pActMover->IsAction() &&
						!( g_pPlayer->m_dwReqFlag & REQ_USESKILL ) )
					{
						g_pPlayer->ClearDest();
						SendMotion( OBJMSG_SITDOWN );
					}
				}
			}
		}
		break;
	case PARTY_GIFTBOX_MODE:
		{
			pItemProp = prj.GetPartySkill( ST_GIFTBOX );

			if( bOnOff )
			{
				if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
				{
					D3DXVECTOR3 v = g_pPlayer->GetPos();

					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, g_pPlayer->GetId() );	// 머리위에 sfx생성.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
				}
				else
				{
					u_long idPlayer = g_Party.GetPlayerId( 0 );
					CMover* pMover = prj.GetUserByID( idPlayer );
					
					if( !IsValidObj(pMover) )
						break;
					
					D3DXVECTOR3 v = pMover->GetPos();
					
					CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, pMover->GetId() );	// 머리위에 sfx생성.
					
					if( pSfx )
						pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
				}
			}

			csBufString = pItemProp->szName;
		}
		break;
	default:
		break;
	}

	CString str;

	if( pItemProp )
	{
		if( bOnOff )
		{
			CWndWorld *pWndWorld = (CWndWorld *)g_WndMng.m_pWndWorld;
			pWndWorld->m_buffs.AddBuff( BUFF_SKILL, (WORD)( pItemProp->dwID ), 1, dwSkillTime );
			str.Format( prj.GetText( TID_GAME_TROUPESKILLUSE ), pItemProp->szName );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_TROUPESKILLUSE ) );
		}
		else
		{
			str.Format( prj.GetText( TID_GAME_TROUPESKILLSTOP ), pItemProp->szName );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_TROUPESKILLSTOP ) );
		}
	}
}

void CDPClient::OnPartyChangeItemMode(CAr & ar) {
	ar >> g_Party.m_nTroupeShareItem;
}

void CDPClient::OnPartyChangeExpMode(CAr & ar) {
	ar >> g_Party.m_nTroupsShareExp;
}

void CDPClient::OnPartyChangeName( CAr & ar )
{
	BOOL	bNoName = FALSE;
	TCHAR	sParty[33];		
	ar.ReadString( sParty, 33 );
	
	if( 0 != strlen( sParty ) )
	{
		if( 0 != strcmp( sParty, prj.GetText(TID_GAME_PARTY_NAME) ) )
		{
			strcpy( g_Party.m_sParty, sParty );
			CString str;
			str.Format( prj.GetText(TID_GAME_PARTY), g_Party.m_sParty );
			g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_GAME_PARTY));
			if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
			{
				strcpy( g_pPlayer->m_szPartyName , g_Party.m_sParty );
			}
		}
		else
		{
			bNoName = TRUE;
		}
	}
	else
	{
		bNoName = TRUE;
	}


	if( bNoName && g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
	{
		SAFE_DELETE( g_WndMng.m_pWndPartyChangeName );
		g_WndMng.m_pWndPartyChangeName = new CWndPartyChangeName;
		g_WndMng.m_pWndPartyChangeName->Initialize( );		
	}
	
	

	//char szMessage[256] = {0,};
	//sprintf( szMessage, "극단이름이 %s로 변경되었습니다.", g_Party.m_sParty );
	//g_WndMng.PutString( szMessage, NULL, 0xff99cc00 );
}

void CDPClient::OnPartyChangeTroup( CAr & ar )
{
	BOOL	bNoName = FALSE;
	TCHAR	sParty[33];		
	ar.ReadString( sParty, 33 );

	g_Party.m_nKindTroup = 1;
	g_WndMng.PutString( prj.GetText(TID_GAME_PARTYCHANGE), NULL, prj.GetTextColor(TID_GAME_PARTYCHANGE)); //순회극단으로 변경되었습니다.
	PlayMusic( BGM_IN_FITUP );
	
	if( 0 != strlen( sParty ) )
	{
		strcpy( g_Party.m_sParty, sParty );
	}

	for( int j = 0 ; j < MAX_SKILL ; j++ )
	{
		g_WndMng.m_dwSkillTime[j] = 0;
	}
}

void CDPClient::OnSetPartyMemberParam(CAr & ar) {
	const auto [idMember, nVal] = ar.Extract<u_long, bool>();

	const int i = g_Party.FindMember( idMember );
	if (i < 0) return;

	g_Party.m_aMember[i].m_remove	= nVal;

	if (nVal != TRUE || i != 0) return;

	bool fRemoveParty	= true;

	for( int j = 1; j < g_Party.m_nSizeofMember; j++ ) {
		if (!g_Party.m_aMember[j].m_remove) {
			fRemoveParty	= false;
			g_Party.SwapPartyMember( 0, j );
			g_WndMng.m_pWndWorld->m_buffs.Clear();
			for( int i = 0 ; i < MAX_SKILL ; i++ )
				g_WndMng.m_dwSkillTime[i] = 0;
			break;
		}
	}

	if (fRemoveParty) {
		g_Party.InitParty();
	}
}

void CDPClient::OnPartySkillCall( OBJID objid, CAr & ar )
{
	D3DXVECTOR3	vLeader;
	ar >> vLeader;
	
	if( IsInvalidObj(g_pPlayer) )	return;
	
	D3DXVECTOR3 v = g_pPlayer->GetPos();
	v.y += g_pPlayer->m_pModel->GetMaxHeight();;

	ItemProp* pItemProp = prj.GetPartySkill( ST_CALL );
	
	if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
	{
		/*
			#define ST_CALL                            1
			#define ST_BLITZ                           2
			#define ST_RETREAT                         3
			#define ST_SCOUT                           4
			#define ST_SPHERECIRCLE                    5
			#define ST_LINKATTACK                      6
			#define ST_FORTUNECIRCLE                   7
			#define ST_STRETCHING                      8
			#define ST_GIFTBOX                         9
		 */
		CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj2, v, objid );	// 머리위에 sfx생성.
	
		if( pSfx )
			pSfx->SetScale( D3DXVECTOR3(1.1f, 1.1f, 1.1f) );
	}
	else
	{
		// 단장 머리 표시하기
		u_long idPlayer      = g_Party.GetPlayerId( 0 );
		CMover* pLeaderMover = prj.GetUserByID( idPlayer );
		
		if( IsValidObj(pLeaderMover) )
		{
			D3DXVECTOR3 v = pLeaderMover->GetPos();
			
			CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj2, v, pLeaderMover->GetId() );	// 머리위에 sfx생성.
			
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3(1.1f, 1.1f, 1.1f) );
		}
		
		// 단원 표시하기
		CSfx *pSfx2 = CreateSfx( pItemProp->dwSfxObj, v, objid );	// 머리위에 sfx생성.
		if( pSfx2 )
		{
			pSfx2->SetScale( D3DXVECTOR3(1.1f, 1.1f, 1.1f) );

			float xDist = vLeader.x - v.x;
			float zDist = vLeader.z - v.z;
			float fr = (float)atan2( xDist, zDist );
			float fa = D3DXToDegree(fr);
			//fa += 90.0f;

			pSfx2->SetAngle( fa );
		}
	}
}

void CDPClient::OnPartySkillBlitz( CAr & ar )
{
	OBJID idTarget;
	ar >> idTarget;
	
	CMover *pTarget = prj.GetMover( idTarget );
	if( IsInvalidObj(pTarget) )	return;
	
	D3DXVECTOR3 v = pTarget->GetPos();
	v.y += pTarget->m_pModel->GetMaxHeight();;
	
	OBJID tagetID= pTarget->GetId();

	ItemProp* pItemProp = prj.GetPartySkill( ST_BLITZ );
	
	CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, NULL_ID, D3DXVECTOR3(0,0,0), tagetID );	// 머리위에 sfx생성.

	if( pSfx )
		pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
}


void CDPClient::OnPartySkillRetreat( OBJID objid )
{
	if( IsInvalidObj(g_pPlayer) )	return;

	ItemProp* pItemProp = prj.GetPartySkill( ST_RETREAT );
	
	for( int i = 0 ; i < g_Party.GetSizeofMember() ; i++ )
	{
		u_long idPlayer = g_Party.GetPlayerId( i );
		CMover* pPlayerMover = prj.GetUserByID( idPlayer );

		if( !IsValidObj(pPlayerMover) )
			continue;

		D3DXVECTOR3 v = pPlayerMover->GetPos();
		
		CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, pPlayerMover->GetId() );	// 머리위에 sfx생성.

		if( pSfx )
			pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
	}
}

// 1회타격에 한해서 크리티컬 발동확률을 높인다.
void CDPClient::OnPartySkillSphereCircle( OBJID objid )
{
	if( IsInvalidObj(g_pPlayer) )	return;

//	pMover->m_dwFlag |= MVRF_CRITICAL;

	CMover *pTarget = prj.GetMover( objid );
	
	if( IsInvalidObj(pTarget) )	return;

	D3DXVECTOR3 v = pTarget->GetPos();
	
	ItemProp* pItemProp = prj.GetPartySkill( ST_SPHERECIRCLE );
	
	CMover* pLeader	= g_Party.GetLeader();
	if( IsValidObj( (CObj*)pLeader ) )
	{
		CSfx *pSfx = CreateSfx( pItemProp->dwSfxObj, v, pLeader->GetId(), D3DXVECTOR3(0,0,0) );	// 머리위에 sfx생성.
		
		if( pSfx )
			pSfx->SetScale( D3DXVECTOR3(1.0f, 1.0f, 1.0f) );
	}
}

void CDPClient::OnEnvironmentSetting( CAr & ar ) {
	int nSeason; ar >> nSeason;
	CEnvironment::GetInstance()->SetSeason( nSeason );

	if (CEnvironment::GetInstance()->GetSeason() == SEASON_SPRING || CEnvironment::GetInstance()->GetSeason() == SEASON_FALL) {
		static constexpr auto trees1 = std::to_array({ "MaCoPrTr01", "MaCoPrTr03", "MaCoPrTr10", "MaCoPrTr20", "MaCoPrTr14", "MaCoPrTr11" });
		static constexpr auto trees2 = std::to_array({ "MaCoPrTr04", "MaCoPrTr05", "MaCoPrTr12", "MaCoPrTr15", "MaCoPrTr13", "MaCoPrTr02" });
		static constexpr auto IsOneOf = [](const MODELELEM & modelElem, const auto & trees) -> bool {
			return std::ranges::any_of(trees, [&](const char * treeName) {
				return _tcsicmp(modelElem.m_szName, treeName) == 0;
				});
		};

		CFixedArray<MODELELEM> & apModelElem = prj.m_modelMng.GetModelElems(OT_OBJ);

		for (MODELELEM & pModelElem : apModelElem) {

			if (IsOneOf(pModelElem, trees1)) {
				pModelElem.m_fScale = 1.0f;

				if (CEnvironment::GetInstance()->GetSeason() == SEASON_SPRING) {
					_tcscpy(pModelElem.m_szName, "MaCoPrTr16");
				} else if (CEnvironment::GetInstance()->GetSeason() == SEASON_FALL) {
					_tcscpy(pModelElem.m_szName, "MapleTree01");
				}
			} else if (IsOneOf(pModelElem, trees2)) {
				pModelElem.m_fScale = 1.0f;

				if (CEnvironment::GetInstance()->GetSeason() == SEASON_SPRING) {
					_tcscpy(pModelElem.m_szName, "MaCoPrTr17");
				} else if (CEnvironment::GetInstance()->GetSeason() == SEASON_FALL) {
					_tcscpy(pModelElem.m_szName, "MapleTree02");
				}
			}
		}
	}
}

void CDPClient::OnEnvironmentEffect( CAr & ar )
{
	ar >> *CEnvironment::GetInstance();

	if( CEnvironment::GetInstance()->GetEnvironmentEffect() == TRUE )
	{
		char szEffectTitle[256] = {0,};
		::memset( szEffectTitle, 0, sizeof( szEffectTitle ) );
		ar.ReadString( szEffectTitle, 255 );
		if( _tcslen( szEffectTitle ) > 0 )
		{
			char lpszTitle[512] = {0,};
			if( CEnvironment::GetInstance()->GetEnvironmentEffect() == TRUE )
			{
				_stprintf( lpszTitle, GETTEXT( TID_GAME_START_EVENT ), szEffectTitle );
			}
			else
			{
				_stprintf( lpszTitle, GETTEXT( TID_GAME_END_EVENT ), szEffectTitle );
			}

			if( m_bEventTextColor )
				g_WndMng.PutString( lpszTitle, NULL, 0xffffff99 );
			else
				g_WndMng.PutString( lpszTitle, NULL, 0xffccffcc );
			m_bEventTextColor = !m_bEventTextColor;
		}
	}

	if( g_pPlayer && g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->m_bIsIndoor && g_pPlayer->GetWorld()->GetID() != WI_DUNGEON_KRRR )
		return;

	if( g_pPlayer->GetWorld()->GetID() == WI_DUNGEON_KRRR )
		g_pPlayer->GetWorld()->m_bViewWeather = TRUE;

	int nWeather = WEATHER_NONE;
	switch( CEnvironment::GetInstance()->GetSeason() )
	{
	case SEASON_NONE:
		{
			nWeather = WEATHER_NONE;
		}
		break;

	case SEASON_SUMMER:
		{
			nWeather = WEATHER_RAIN;
		}
		break;

	case SEASON_SPRING:
	case SEASON_FALL:
	case SEASON_WINTER:
		{
			nWeather = WEATHER_SNOW;
		}
		break;

	default:
		break;
	}

	CWorld::m_skyBox.SetWeather( nWeather , CEnvironment::GetInstance()->GetEnvironmentEffect() );
}


void CDPClient::OnPartyChat( CAr & ar )
{
static	\
	CHAR	lpName[MAX_PLAYER], lpString[260];
	OBJID objid;
	ar >> objid;
	ar.ReadString( lpName, MAX_PLAYER );
	ar.ReadString( lpString, 260 );

#ifdef __YS_CHATTING_BLOCKING_SYSTEM
	CMover* pMover = prj.GetMover( objid );
	if( pMover && pMover->IsPlayer() == TRUE )
	{
		if (prj.m_setBlockedUserID.contains(lpName))
			return;
	}
#endif // __YS_CHATTING_BLOCKING_SYSTEM

	CString str;
	str.Format( "[%s] %s", lpName, lpString );
	g_WndMng.PutString( str, NULL, 0xff99cc00, CHATSTY_PARTY );

#ifndef __YS_CHATTING_BLOCKING_SYSTEM
	CMover* pMover	= prj.GetMover( objid );
#endif // __YS_CHATTING_BLOCKING_SYSTEM
	if( IsValidObj( pMover ) ) 
	{
		if( strlen( lpString ) )
			g_DialogMsg.AddMessage( pMover, lpString, 0xfffffff, CHAT_PARTY );
	}
}

void CDPClient::OnMyPartyName( CAr & ar )
{
	ar.ReadString( g_pPlayer->m_szPartyName, 32 );
}

void CDPClient::OnSMMode( CAr & ar )
{
	int nType;
	DWORD dwTime;
	ar >> nType;
	ar >> dwTime;
	
	if( nType < 0 || SM_MAX <= nType )
	{
		LPCTSTR szErr = Error( "CDPClient::OnSMMode %d", nType );
		//ADDERRORMSG( szErr );
		return;		// 일단 안팅기게 리턴
	}

	if( g_pPlayer == NULL )
	{
		LPCTSTR szErr = Error( "CDPClient::OnSMMode Player NULL" );
		//ADDERRORMSG( szErr );
		return;
	}

	g_pPlayer->m_dwSMTime[nType] = dwTime;

	if( nType == SM_BANK15 || nType == SM_BANK30 || nType == SM_BANK001 )
	{
		CWndBase* pWndBase	= g_WndMng.GetWndBase( APP_COMMON_BANK );
		if( pWndBase )
		{
			( (CWndBank*)pWndBase )->ReSetBank();
		}
	}
	
	if( nType == SM_RESIST_ATTACK_LEFT || nType == SM_RESIST_ATTACK_RIGHT )
	{
		BYTE nfind = 0;
		if( nType == SM_RESIST_ATTACK_LEFT )
			nfind = g_pPlayer->m_nAttackResistLeft;
		else
			nfind = g_pPlayer->m_nAttackResistRight;
		
		switch( nfind )
		{
		case SAI79::FIRE:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_FIREASTONE;
			break;
		case SAI79::WATER:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_WATEILSTONE;
			break;
		case SAI79::WIND:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_WINDYOSTONE;
			break;
		case SAI79::ELECTRICITY:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_LIGHTINESTONE;
			break;
		case SAI79::EARTH:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_EARTHYSTONE;
			break;
		}
	}
	else if( nType == SM_RESIST_DEFENSE )
	{
		switch( g_pPlayer->m_nDefenseResist )
		{
		case SAI79::FIRE:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEFIREASTONE;
			break;
		case SAI79::WATER:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEWATEILSTONE;
			break;
		case SAI79::WIND:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEWINDYOSTONE;
			break;
		case SAI79::ELECTRICITY:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DELIGHTINESTONE;
			break;
		case SAI79::EARTH:
			g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEEARTHYSTONE;
			break;
		}
	}
}

void CDPClient::OnSMModeAll( CAr & ar )
{
	short nCount;
	ar >> nCount;

	DWORD dwTime;
	int	nType;
	for( int i = 0 ; i < nCount; ++i )
	{
		ar >> nType;
		ar >> dwTime;

		if( nType < 0 || SM_MAX <= nType )
		{
			LPCTSTR szErr = Error( "CDPClient::OnSMModeAll %d", nType );
			//ADDERRORMSG( szErr );
			int *p = NULL;
			*p = 1;
		}
		
		if( g_pPlayer == NULL )
		{
			LPCTSTR szErr = Error( "CDPClient::OnSMModeAll Player NULL" );
			//ADDERRORMSG( szErr );
			int *p = NULL;
			*p = 1;
		}

		g_pPlayer->m_dwSMTime[nType] = dwTime;

		if( nType == SM_RESIST_ATTACK_LEFT || nType == SM_RESIST_ATTACK_RIGHT )
		{
			BYTE nfind;
			if( nType == SM_RESIST_ATTACK_LEFT )
				nfind = g_pPlayer->m_nAttackResistLeft;
			else
				nfind = g_pPlayer->m_nAttackResistRight;

			switch( nfind )
			{
			case SAI79::FIRE:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_FIREASTONE;
				break;
			case SAI79::WATER:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_WATEILSTONE;
				break;
			case SAI79::WIND:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_WINDYOSTONE;
				break;
			case SAI79::ELECTRICITY:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_LIGHTINESTONE;
				break;
			case SAI79::EARTH:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_EARTHYSTONE;
				break;
			}
		}
		else if( nType == SM_RESIST_DEFENSE )
		{
			switch( g_pPlayer->m_nDefenseResist )
			{
			case SAI79::FIRE:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEFIREASTONE;
				break;
			case SAI79::WATER:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEWATEILSTONE;
				break;
			case SAI79::WIND:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEWINDYOSTONE;
				break;
			case SAI79::ELECTRICITY:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DELIGHTINESTONE;
				break;
			case SAI79::EARTH:
				g_AddSMMode.dwSMItemID[nType] = II_CHR_SYS_SCR_DEEARTHYSTONE;
				break;
			}
		}
	}

	if( g_pPlayer && ( 0 < g_pPlayer->m_dwSMTime[SM_BANK15] || 0 < g_pPlayer->m_dwSMTime[SM_BANK30] || 0 < g_pPlayer->m_dwSMTime[SM_BANK001] ) )
	{
		CWndBase* pWndBase	= g_WndMng.GetWndBase( APP_COMMON_BANK );
		if( pWndBase )
			( (CWndBank*)pWndBase )->ReSetBank();
	}
}

void CDPClient::OnResistSMMode( CAr & ar )
{
	BYTE nAttackResistLeft, nAttackResistRight, nDefenseResist;
	ar >> nAttackResistLeft;
	ar >> nAttackResistRight;
	ar >> nDefenseResist;

	if( g_pPlayer == NULL )
	{
		LPCTSTR szErr = Error( "CDPClient::OnSMMode Player NULL" );
		//ADDERRORMSG( szErr );
		return;
	}
	g_pPlayer->m_nAttackResistLeft = nAttackResistLeft;
	g_pPlayer->m_nAttackResistRight = nAttackResistRight;
	g_pPlayer->m_nDefenseResist = nDefenseResist;

	switch( g_pPlayer->m_nAttackResistLeft )
	{
	case SAI79::FIRE:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_LEFT] = II_CHR_SYS_SCR_FIREASTONE;
		break;
	case SAI79::WATER:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_LEFT] = II_CHR_SYS_SCR_WATEILSTONE;
		break;
	case SAI79::WIND:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_LEFT] = II_CHR_SYS_SCR_WINDYOSTONE;
		break;
	case SAI79::ELECTRICITY:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_LEFT] = II_CHR_SYS_SCR_LIGHTINESTONE;
		break;
	case SAI79::EARTH:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_LEFT] = II_CHR_SYS_SCR_EARTHYSTONE;
		break;
	}

	switch( g_pPlayer->m_nAttackResistRight )
	{
	case SAI79::FIRE:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_RIGHT] = II_CHR_SYS_SCR_FIREASTONE;
		break;
	case SAI79::WATER:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_RIGHT] = II_CHR_SYS_SCR_WATEILSTONE;
		break;
	case SAI79::WIND:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_RIGHT] = II_CHR_SYS_SCR_WINDYOSTONE;
		break;
	case SAI79::ELECTRICITY:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_RIGHT] = II_CHR_SYS_SCR_LIGHTINESTONE;
		break;
	case SAI79::EARTH:
		g_AddSMMode.dwSMItemID[SM_RESIST_ATTACK_RIGHT] = II_CHR_SYS_SCR_EARTHYSTONE;
		break;
	}
	
	switch( g_pPlayer->m_nDefenseResist )
	{
	case SAI79::FIRE:
		g_AddSMMode.dwSMItemID[SM_RESIST_DEFENSE] = II_CHR_SYS_SCR_DEFIREASTONE;
		break;
	case SAI79::WATER:
		g_AddSMMode.dwSMItemID[SM_RESIST_DEFENSE] = II_CHR_SYS_SCR_DEWATEILSTONE;
		break;
	case SAI79::WIND:
		g_AddSMMode.dwSMItemID[SM_RESIST_DEFENSE] = II_CHR_SYS_SCR_DEWINDYOSTONE;
		break;
	case SAI79::ELECTRICITY:
		g_AddSMMode.dwSMItemID[SM_RESIST_DEFENSE] = II_CHR_SYS_SCR_DELIGHTINESTONE;
		break;
	case SAI79::EARTH:
		g_AddSMMode.dwSMItemID[SM_RESIST_DEFENSE] = II_CHR_SYS_SCR_DEEARTHYSTONE;
		break;
	}
}

void CDPClient::OnCommercialElem( CAr & ar )
{
	DWORD dwItemId;
	int nSmItemId;
	ar >> dwItemId;
	ar >> nSmItemId;

	CItemElem* pItemElem	= g_pPlayer->m_Inventory.GetAtId( dwItemId );
	if( NULL != pItemElem )
	{
		if( nSmItemId == 9999 )
		{
			pItemElem->m_bItemResist = SAI79::NO_PROP;
			pItemElem->m_nResistAbilityOption = 0;
			
			g_WndMng.PutString( prj.GetText( TID_GAME_REMOVEELEMENT ), NULL, prj.GetTextColor( TID_GAME_REMOVEELEMENT ) );
		}
		else
		{
			pItemElem->m_nResistSMItemId = nSmItemId;
		}
	}	
}

void CDPClient::OnMoverFocus( CAr & ar )
{
	u_long uidPlayer;
	DWORD dwGold;
	EXPINTEGER	nEXP;
	ar >> uidPlayer;
	ar >> dwGold;
	ar >> nEXP;
	CMover* pMover = prj.GetUserByID( uidPlayer );
	if( pMover )
	{
		//pMover->m_dwGold = dwGold;
		pMover->SetGold( dwGold );
		pMover->m_nExp1 = nEXP;
	}
}

void CDPClient::OnPartyMapInfo( CAr & ar )
{
	int nPartyIndex;
	D3DXVECTOR3 vPos;

	ar >> nPartyIndex;
	ar >> vPos;

	CMover* pMover = prj.GetUserByID( g_Party.m_aMember[nPartyIndex].m_uPlayerId );
	if( !IsValidObj( pMover ) )
		g_Party.SetPos( nPartyIndex, vPos );
}

void CDPClient::OnFlyffEvent( CAr & ar )
{
	ar >> g_eLocal;

	for( int i = 0; i < MAX_EVENT; i++ )
	{
		BYTE nState	= g_eLocal.GetState( i );
		if (nState == 0) continue;
		
			if (const EVENT_GENERIC * pEvent = CEventGeneric::GetInstance()->GetEvent(i)) {
				if( strlen( pEvent->pszTitle ) == 0 )
					continue;
				char lpString[200]	= { 0, };
				sprintf( lpString, GETTEXT( TID_GAME_ON_EVENT ), pEvent->pszTitle );
				if( m_bEventTextColor )
					g_WndMng.PutString( lpString, NULL, 0xffffff99 );
				else
					g_WndMng.PutString( lpString, NULL, 0xffccffcc );
				m_bEventTextColor = !m_bEventTextColor;
				continue;
			}

			switch( i )
			{
				case EVE_0401A:
					{
						g_WndMng.PutString( "'드롭률 100% 독도를 지켜라' 이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'드롭률 100% 독도를 지켜라' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
				case EVE_SCHOOL_BATTLE:
					{
						g_WndMng.PutString( "'학교 대항전' 이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'학교 대항전' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
#ifdef __EVENT0913
				case EVE_EVENT0913:
					{
						g_WndMng.PutString( "'아이템 드롭 확률 두배 UP~!'이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'아이템 드롭 확률 두배 UP~!' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
#endif	// __EVENT0913
#ifdef __EVENT1206
				case EVE_EVENT1206:
					{
						g_WndMng.PutString( "'보상' 이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'보상' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
#endif	// __EVENT0913

				case EVE_EVENT1219:
					{
						g_WndMng.PutString( "'해피 크리스마스~ 특명!! 양말을 찾아라~!!' 이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'해피 크리스마스~ 특명!! 양말을 찾아라~!!' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
				case EVE_EVENT0127:
					{
						g_WndMng.PutString( "'즐거운 설날. 떡국도 먹고 복 주머니도 타가세요' 이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'즐거운 설날. 떡국도 먹고 복 주머니도 타가세요' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
				case EVE_EVENT0214:
					{
						g_WndMng.PutString( "'훗! 우리는 무적의 솔로부대다!' 이벤트가 진행중입니다.", NULL, 0xffffff99 );
						g_WndMng.PutString( "'훗! 우리는 무적의 솔로부대다!' 이벤트가 진행중입니다.", NULL, 0xffccffcc );
						break;
					}
			}
		
	}
}

void CDPClient::OnSetLocalEvent( CAr & ar )
{
	short id;
	BYTE nState;
	ar >> id >> nState;
	g_eLocal.SetState( id, nState );
	
	if (const EVENT_GENERIC * pEvent = CEventGeneric::GetInstance()->GetEvent(id)) {
		if( std::strlen( pEvent->pszTitle ) == 0 )
			return;

		const DWORD textId = nState == 0 ? TID_GAME_END_EVENT : TID_GAME_START_EVENT;
		const DWORD textColor = m_bEventTextColor ? 0xffffff99 : 0xffccffcc;

		char lpString[200] = "";
		std::sprintf(lpString, prj.GetText(textId), pEvent->pszTitle);
		g_WndMng.PutString(lpString, NULL, textColor);
		
		m_bEventTextColor = !m_bEventTextColor;
	}

	switch( id )
	{
		case EVE_0401A:
			{
				if( nState == 0 )
				{
					g_WndMng.PutString( "'드롭률 100% 독도를 지켜라' 이벤트가 종료되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'드롭률 100% 독도를 지켜라' 이벤트가 종료되었습니다.", NULL, 0xffccffcc );
				}
				else
				{
					g_WndMng.PutString( "'드롭률 100% 독도를 지켜라' 이벤트가 시작되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'드롭률 100% 독도를 지켜라' 이벤트가 시작되었습니다.", NULL, 0xffccffcc );
				}
				break;
			}
#ifdef __EVENT0913
		case EVE_EVENT0913:
			{
				if( nState == 0 )
				{
					g_WndMng.PutString( "'아이템 드롭 확률 두배 UP~!' 이벤트가 종료되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'아이템 드롭 확률 두배 UP~!' 이벤트가 종료되었습니다.", NULL, 0xffccffcc );
				}
				else
				{
					g_WndMng.PutString( "'아이템 드롭 확률 두배 UP~!' 이벤트가 시작되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'아이템 드롭 확률 두배 UP~!' 이벤트가 시작되었습니다.", NULL, 0xffccffcc );
				}
				break;
			}
#endif	// __EVENT0913
#ifdef __EVENT1206
		case EVE_EVENT1206:
			{
				if( nState == 0 )
				{
					g_WndMng.PutString( "'보상' 이벤트가 종료되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'보상' 이벤트가 종료되었습니다.", NULL, 0xffccffcc );
				}
				else
				{
					g_WndMng.PutString( "'보상' 이벤트가 시작되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'보상' 이벤트가 시작되었습니다.", NULL, 0xffccffcc );
				}
				break;
			}
#endif	// __EVENT1206
		case EVE_EVENT1219:
			{
				if( nState == 0 )
				{
					g_WndMng.PutString( "'해피 크리스마스~ 특명!! 양말을 찾아라~!!' 이벤트가 종료되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'해피 크리스마스~ 특명!! 양말을 찾아라~!!' 이벤트가 종료되었습니다.", NULL, 0xffccffcc );
				}
				else
				{
					g_WndMng.PutString( "'해피 크리스마스~ 특명!! 양말을 찾아라~!!' 이벤트가 시작되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'해피 크리스마스~ 특명!! 양말을 찾아라~!!' 이벤트가 시작되었습니다.", NULL, 0xffccffcc );
				}
				break;
			}
		case EVE_EVENT0127:
			{
				if( nState == 0 )
				{
					g_WndMng.PutString( "'즐거운 설날. 떡국도 먹고 복 주머니도 타가세요' 이벤트가 종료되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'즐거운 설날. 떡국도 먹고 복 주머니도 타가세요' 이벤트가 종료되었습니다.", NULL, 0xffccffcc );
				}
				else
				{
					g_WndMng.PutString( "'즐거운 설날. 떡국도 먹고 복 주머니도 타가세요' 이벤트가 시작되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'즐거운 설날. 떡국도 먹고 복 주머니도 타가세요' 이벤트가 시작되었습니다.", NULL, 0xffccffcc );
				}
				break;
			}
		case EVE_EVENT0214:
			{
				if( nState == 0 )
				{
					g_WndMng.PutString( "'훗! 우리는 무적의 솔로부대다!' 이벤트가 종료되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'훗! 우리는 무적의 솔로부대다!' 이벤트가 종료되었습니다.", NULL, 0xffccffcc );
				}
				else
				{
					g_WndMng.PutString( "'훗! 우리는 무적의 솔로부대다!' 이벤트가 시작되었습니다.", NULL, 0xffffff99 );
					g_WndMng.PutString( "'훗! 우리는 무적의 솔로부대다!' 이벤트가 시작되었습니다.", NULL, 0xffccffcc );
				}
				break;
			}

		case EVE_SCHOOL_BATTLE:
			{
				if( nState == SBS_READY )
				{
					g_Caption1.RemoveAll();
					if( ::GetLanguage() == LANG_THA )
					{
						CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
						if( pWndWorld )
							g_Caption1.AddCaption( prj.GetText( TID_SBEVE_SBREADY ), pWndWorld->m_pFontAPITitle );
					}
					else
					{
						g_WndMng.PutString( prj.GetText( TID_SBEVE_SBREADY ), NULL, 0xffffff99 );
						g_WndMng.PutString( prj.GetText( TID_SBEVE_SBREADY ), NULL, prj.GetTextColor( TID_SBEVE_SBREADY ) );
					}
				}
				else if( nState == SBS_START )
				{
					g_Caption1.RemoveAll();
					if( ::GetLanguage() == LANG_THA )
					{
						CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
						if( pWndWorld )
							g_Caption1.AddCaption( prj.GetText( TID_SBEVE_SBMOVE ), pWndWorld->m_pFontAPITitle );
					}
					else
					{
						g_WndMng.PutString( prj.GetText( TID_SBEVE_SBMOVE ), NULL, 0xffffff99 );
						g_WndMng.PutString( prj.GetText( TID_SBEVE_SBMOVE ), NULL, prj.GetTextColor( TID_SBEVE_SBMOVE ) );
					}
				}
				else if( nState == SBS_START2 )
				{
					g_Caption1.RemoveAll();
					if( ::GetLanguage() == LANG_THA )
					{
						CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
						if( pWndWorld )
							g_Caption1.AddCaption( prj.GetText( TID_SBEVE_SBBATTLE ), pWndWorld->m_pFontAPITitle );
					}
					else
					{
						g_WndMng.PutString( prj.GetText( TID_SBEVE_SBBATTLE ), NULL, 0xffffff99 );
						g_WndMng.PutString( prj.GetText( TID_SBEVE_SBBATTLE ), NULL, prj.GetTextColor( TID_SBEVE_SBBATTLE ) );
					}
				}
				else if( nState == SBS_END )
				{
					g_Caption1.RemoveAll();
					g_WndMng.PutString( prj.GetText( TID_SBEVE_SBEND ), NULL, 0xffffff99 );
					g_WndMng.PutString( prj.GetText( TID_SBEVE_SBEND ), NULL, prj.GetTextColor( TID_SBEVE_SBEND ) );
				}
				break;
			}
	}
}

void CDPClient::OnEventLuaDesc( CAr & ar )
{
	int nState;
	char lpString[200];
	char strDesc[100];

	ar >> nState;
	ar.ReadString(strDesc);

	switch( nState )
	{
		case 0 :
			sprintf( lpString, GETTEXT( TID_GAME_END_EVENT ), strDesc );
			break;
		case 1 :
			sprintf( lpString, GETTEXT( TID_GAME_START_EVENT ), strDesc );
			break;
		case 2 :
			sprintf( lpString, GETTEXT( TID_GAME_ON_EVENT ), strDesc );
			break;
	}
	if( m_bEventTextColor )
		g_WndMng.PutString( lpString, NULL, 0xffffff99 );
	else
		g_WndMng.PutString( lpString, NULL, 0xffccffcc );
	m_bEventTextColor = !m_bEventTextColor;
}

void CDPClient::OnRemoveAttributeResult( CAr & ar )
{
	BOOL bSuccess;
	ar >> bSuccess;

	CWndRemoveAttribute* pWndRemoveAttribute = (CWndRemoveAttribute*)g_WndMng.GetWndBase( APP_REMOVE_ATTRIBUTE );
	if(pWndRemoveAttribute != NULL)
		pWndRemoveAttribute->ReceiveResult(bSuccess);	
}
void CDPClient::OnGameRate( CAr & ar )
{
	const auto [fRate, nFlag] = ar.Extract<FLOAT, BYTE>();

	char chMessage[128] = {0,};
	BOOL bMessage = FALSE;
	switch( nFlag )
	{
	case GAME_RATE_ITEMDROP:
		{
			prj.m_fItemDropRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set ItemDropRate (0.1 ~ 20.0) : %f", prj.m_fItemDropRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_GOLDDROP:
		{
			prj.m_fGoldDropRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set GoldDropRate (0.1 ~ 5.0) : %f", prj.m_fGoldDropRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_MONSTEREXP:
		{
			prj.m_fMonsterExpRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set MonsterExpRate (0.1 ~ 5.0) : %f", prj.m_fMonsterExpRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_MONSTERHIT:
		{
			prj.m_fMonsterHitRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set MonsterHitRate (0.1 ~ 5.0) : %f", prj.m_fMonsterHitRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_X3:
		{
			prj.m_fItemDropRate = fRate;
			prj.m_fGoldDropRate = fRate;
			prj.m_fMonsterExpRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, prj.GetText(TID_GAME_APPLYGAMEENI), (int)fRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_REBIRTH:
		{
			prj.m_fMonsterRebirthRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set Monster Rebirth Rate (0.1 ~ 5.0) : %f", prj.m_fMonsterRebirthRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_HITPOINT:
		{
			prj.m_fMonsterHitpointRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set Monster Hitpoint Rate (0.1 ~ 5.0) : %f", prj.m_fMonsterHitpointRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_AGGRESSIVE:
		{
			prj.m_fMonsterAggressiveRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set Monster Aggressive Rate (0.1 ~ 5.0) : %f", prj.m_fMonsterAggressiveRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_RESPAWN:
		{
			prj.m_fMonsterRespawnRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set Monster Aggressive Rate (0.1 ~ 5.0) : %f", prj.m_fMonsterRespawnRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_SKILL_VAGSP:
		{
			prj.m_dwVagSP = (DWORD)fRate;
		}
		break;
	case GAME_SKILL_EXPERTSP:
		{
			prj.m_dwExpertSP = (DWORD)fRate;
		}
		break;
	case GAME_SKILL_PROSP:
		{
			prj.m_dwProSP = (DWORD)fRate;
		}
		break;
#ifdef __SHOP_COST_RATE
	case GAME_RATE_SHOP_BUY:
		{
			prj.m_fShopBuyRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set Shop Buy Rate : %f", prj.m_fShopBuyRate );
				bMessage = TRUE;
			}
		}
		break;
	case GAME_RATE_SHOP_SELL:
		{
			prj.m_fShopSellRate = fRate;
			if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
			{
				sprintf( chMessage, "Set Shop Sell Rate : %f", prj.m_fShopSellRate );
				bMessage = TRUE;
			}
		}
		break;
#endif // __SHOP_COST_RATE
	default:
		break;
	}

	if( bMessage )
	{
		g_WndMng.PutString( chMessage, NULL, 0xffff0000, CHATSTY_GENERAL );		
	}
}

void CDPClient::OnMonsterProp( CAr & ar )
{
	char	szMonsterName[32];
	int		nHitPoint;
	int		nAttackPower;
	int		nDefence;
	int		nExp;
	int		nItemDrop;
	int		nPenya;
	
	int		nMonsterPropSize;
	
	prj.m_nAddMonsterPropSize = 0;
	prj.m_nRemoveMonsterPropSize = 0;
	
	ar >> nMonsterPropSize;
	for( int i = 0 ; i < nMonsterPropSize ; ++i )
	{
		ar.ReadString( szMonsterName, 32 );
		ar >> nHitPoint;
		ar >> nAttackPower;
		ar >> nDefence;
		ar >> nExp;
		ar >> nItemDrop;
		ar >> nPenya;
		
		MoverProp* pMoverProp	= NULL;
		pMoverProp	= prj.GetMoverProp( szMonsterName );
		if( pMoverProp )
		{
			pMoverProp->m_fHitPoint_Rate = (float)nHitPoint / 100.0f;
			pMoverProp->m_fAttackPower_Rate = (float)nAttackPower / 100.0f;
			pMoverProp->m_fDefence_Rate = (float)nDefence / 100.0f;
			pMoverProp->m_fExp_Rate = (float)nExp / 100.0f;
			pMoverProp->m_fItemDrop_Rate = (float)nItemDrop / 100.0f;
			pMoverProp->m_fPenya_Rate = (float)nPenya / 100.0f;
			pMoverProp->m_bRate = TRUE;
			
			MONSTER_PROP MonsterProp;
			strcpy( MonsterProp.szMonsterName, szMonsterName );
			MonsterProp.nHitPoint = nHitPoint;
			MonsterProp.nAttackPower = nAttackPower;
			MonsterProp.nDefence = nDefence;
			MonsterProp.nExp = nExp;
			MonsterProp.nItemDrop = nItemDrop;
			MonsterProp.nPenya = nPenya;
			prj.AddMonsterProp( MonsterProp );
		}
	} 
	
	int	nRemovePropSize;
	ar >> nRemovePropSize;
	for( int i = 0 ; i < nRemovePropSize ; ++i )
	{
		ar.ReadString( szMonsterName, 32 );
		MoverProp* pMoverProp	= NULL;
		pMoverProp	= prj.GetMoverProp( szMonsterName );
		if( pMoverProp )
		{
			pMoverProp->m_fHitPoint_Rate = 1.0f;
			pMoverProp->m_fAttackPower_Rate = 1.0f;
			pMoverProp->m_fDefence_Rate = 1.0f;
			pMoverProp->m_fExp_Rate = 1.0f;
			pMoverProp->m_fItemDrop_Rate = 1.0f;
			pMoverProp->m_fPenya_Rate = 1.0f;
			pMoverProp->m_bRate = FALSE;
			prj.RemoveMonsterProp( szMonsterName );
		}
	}
}

void CDPClient::OnGMChat( CAr & ar )
{
	int nSize;
	char chMessage[256];
	CString str;
	ar >> nSize;
	for( int i = 0 ; i < nSize ; ++i )
	{
		ar.ReadString( chMessage, 256 );
		str.Format( prj.GetText(TID_ADMIN_ANNOUNCE), chMessage );
		g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_ADMIN_ANNOUNCE), CHATSTY_SYSTEM );
	}
}

void CDPClient::OnChangeFace( CAr & ar )
{
	u_long uidPlayer;
	DWORD dwFace;
	
	ar >> uidPlayer >> dwFace;

	CMover* pMover = prj.GetUserByID( uidPlayer );
	
	if( IsValidObj( pMover ) )
	{
		pMover->m_skin.headMesh = dwFace;
		if( g_pPlayer == pMover )
		{
			CWndInventory* pWndInventory	= (CWndInventory*)g_WndMng.GetWndBase( APP_INVENTORY );
			if(pWndInventory && pWndInventory->GetModel())
			{
				pWndInventory->UpdateParts();
			}
			
			CMover::UpdateParts( pMover->GetSex(), pMover->m_skin, pMover->m_aEquipInfo, (CModelObject*)pMover->m_pModel, &pMover->m_Inventory );		
			CMover::UpdateParts( pMover->GetSex(), pMover->m_skin, pMover->m_aEquipInfo, g_pBipedMesh, &pMover->m_Inventory );
		}
		else
		{
			CMover::UpdateParts( pMover->GetSex(), pMover->m_skin, pMover->m_aEquipInfo, (CModelObject*)pMover->m_pModel, NULL );
		}

	}		
}

void CDPClient::OnQuestTextTime( CAr & ar )
{
	BOOL  bFlag;
	GroupQuest::ProcessState   nState;
	DWORD dwTime;

	ar >> bFlag;
	ar >> nState;
	ar >> dwTime;
	
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	if( pWndWorld )
	{
		pWndWorld->m_QuestTime.bFlag       = bFlag;
		pWndWorld->m_QuestTime.nState      = nState;
		pWndWorld->m_QuestTime.dwQuestTime = GetTickCount() + dwTime;
	}
}

void CDPClient::OnGuildCombat( CAr & ar )
{
	BYTE nState;
	ar >> nState;
	switch( nState )
	{
	case GC_IN_WINDOW:
		OnGCInWindow( ar );
		break;
/*	case GC_IN_COMPLETE:
		OnGCIn( ar );
		break;
*/	case GC_SELECTPLAYER:
		OnGCSelectPlayer( ar );
		break;
	case GC_REQUEST_STATUS:
		OnGCRequestStatus( ar );
		break;
	case GC_USERSTATE:
		OnGCUserState( ar );
		break;
	case GC_GUILDSTATUS:
		if (CWndWorld * pWndWorld = g_WndMng.GetWndBase<CWndWorld>(APP_WORLD)) {
			pWndWorld->m_infoGC.OnGuildStatus(ar);
		}
		break;
	case GC_GUILDPRECEDENCE:
		if (CWndWorld * pWndWorld = g_WndMng.GetWndBase<CWndWorld>(APP_WORLD)) {
			pWndWorld->m_GCprecedence.OnGuildPrecedence(ar);
		}
		break;
	case GC_PLAYERPRECEDENCE:
		if (CWndWorld * pWndWorld = g_WndMng.GetWndBase<CWndWorld>(APP_WORLD)) {
			pWndWorld->m_GCprecedence.OnPlayerPrecedence(ar);
		}
		break;
	case GC_SELECTWARPOS:
		OnGCJoinWarWindow( ar );
		break;
	case GC_GCSTATE:
		OnGuildCombatState( ar );
		break;
	case GC_NEXTTIMESTATE:
		OnGuildCombatNextTimeState( ar );
		break;
	case GC_ENTERTIME:
		OnGuildCombatEnterTime( ar );
		break;
	case GC_WINGUILD:
		OnGCWinGuild( ar );
		break;
	case GC_BESTPLAYER:
		OnGCBestPlayer( ar );
		break;
	case GC_GETPENYAGUILD:
		OnGCGetPenyaGuild( ar );
		break;
	case GC_GETPENYAPLAYER:
		OnGCGetPenyaPlayer( ar );
		break;
	case GC_DIAGMESSAGE:
		OnGCDiagMessage( ar );
		break;
	case GC_TELE:
		OnGCTele( ar );
		break;
	case GC_WARPLAYERLIST:
		if (CWndWorld * pWndWorld = g_WndMng.GetWndBase<CWndWorld>(APP_WORLD)) {
			pWndWorld->m_infoGC.OnPlayerList(ar);
		}
		break;
	case GC_ISREQUEST:
		OnIsRequest( ar );
		break;
	case GC_LOG:
		OnGCLog( ar );
		break;
	case GC_LOG_REALTIME:
		OnGCLogRealTime( ar );
		break;
	case GC_PLAYERPOINT:
		OnGCPlayerPoint( ar );
		break;
	}
}
void CDPClient::OnGCInWindow( CAr& ar )
{
	__int64 nPrizePenya;		// 총 상금액
	DWORD dwRequestPenya;	// 자신 길드 상금액
	DWORD dwMinPenya;		// 최소 페냐
	ar >> nPrizePenya >> dwRequestPenya >> dwMinPenya ;
	
	if( g_GuildCombatMng.m_nState != CGuildCombat::CLOSE_STATE )
	{
		g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDCOMBAT_REQUESTEND ) );
		return;
	}
	
	SAFE_DELETE(g_WndMng.m_pWndGuildCombatOffer);
	g_WndMng.m_pWndGuildCombatOffer = new CWndGuildCombat1to1Offer(
		CWndGuildCombat1to1Offer::CombatType::GCGuild
	);

	g_WndMng.m_pWndGuildCombatOffer->Initialize();				

	g_WndMng.m_pWndGuildCombatOffer->SetMinGold( dwMinPenya );
		
	if( dwRequestPenya == 0 )
	{
		g_WndMng.m_pWndGuildCombatOffer->SetReqGold(dwRequestPenya);
		g_WndMng.m_pWndGuildCombatOffer->SetBackupGold( dwMinPenya );
		g_WndMng.m_pWndGuildCombatOffer->SetGold( dwMinPenya );
	}
	else
	{
		g_WndMng.m_pWndGuildCombatOffer->SetReqGold(dwRequestPenya);
		g_WndMng.m_pWndGuildCombatOffer->SetBackupGold( dwRequestPenya );
		g_WndMng.m_pWndGuildCombatOffer->SetGold( dwRequestPenya );			
	}


	g_WndMng.m_pWndGuildCombatOffer->EnableAccept( TRUE );
	
}

void CDPClient::OnGCRequestStatus( CAr& ar )
{
	__int64 nPrizePenya;
	DWORD dwRequstPenya;
	int nSize;
	char strGuildName[128];
	std::vector<CString> vecRequestGuild;
	time_t tNext;		// 다음에 길드대전이 시작할 시간.
	ar >> tNext;
	ar >> nPrizePenya;						// 총 상금액
	ar >> nSize;
	
	SAFE_DELETE(g_WndMng.m_pWndGuildWarState);
	g_WndMng.m_pWndGuildWarState = new CWndGuildCombatState(0);

	if( g_WndMng.m_pWndGuildWarState )
	{
		g_WndMng.m_pWndGuildWarState->Initialize();
		g_WndMng.m_pWndGuildWarState->SetTime( tNext );
		
		int nRate = -1;
		CGuild* pGuild = g_pPlayer->GetGuild();

		for( int i = 0 ; i < nSize ; ++i )
		{
			ar.ReadString( strGuildName, 128 );		// 길드명
			vecRequestGuild.push_back( strGuildName );

			if( pGuild && nRate == -1 )
			{
				if( stricmp( pGuild->m_szGuild, strGuildName ) == 0 )
				{
					nRate = i+1;
				}
			}
			
			g_WndMng.m_pWndGuildWarState->InsertGuild( strGuildName, "1", i+1 );
		}

		g_WndMng.m_pWndGuildWarState->SetRate( nRate );	
	}

	ar >> dwRequstPenya;					// My 길드가 신청한 금액

	if( g_WndMng.m_pWndGuildWarState )
	{
		g_WndMng.m_pWndGuildWarState->SetGold( dwRequstPenya );	
		g_WndMng.m_pWndGuildWarState->SetTime( tNext );
	}	
}

void CDPClient::OnGCSelectPlayer( CAr& ar )
{
	BOOL bRequestWar, bWindow;
	u_long uidDefender, uidPlayer;
	int nSize;
	int nMaxJoinMember; // 대전에 참가할수 있는 최대 개수
	int nMaxWarMember;	// 대전에 선수로 들어감
	std::vector<u_long> vecSelectPlayer;
	ar >> nMaxJoinMember;
	ar >> nMaxWarMember;

	ar >> bRequestWar;		// 출전할수 있는 길드인가?
	if( bRequestWar )
	{
		ar >> bWindow;			// 윈도우 창띄운 메세지 인지?
	
		if( bWindow )
		{
			ar >> uidDefender;		// 디펜더
			ar >> nSize;
			SAFE_DELETE(g_WndMng.m_pWndGuildCombatSelection);
			g_WndMng.m_pWndGuildCombatSelection = new CWndGuildCombatSelection;
			if( g_WndMng.m_pWndGuildCombatSelection )
			{
				g_WndMng.m_pWndGuildCombatSelection->Initialize();	
				g_WndMng.m_pWndGuildCombatSelection->SetMemberSize(nMaxJoinMember, nMaxWarMember );
			}
			
			for( int i = 0 ; i < nSize ; ++i )
			{
				ar >> uidPlayer;
				vecSelectPlayer.push_back( uidPlayer );
			}
			
			if( g_WndMng.m_pWndGuildCombatSelection )
			{
				g_WndMng.m_pWndGuildCombatSelection->ReceiveLineup(vecSelectPlayer, uidDefender);
			}	
		}
		else
		{
			g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDCOMBAT_PLAYER_MAKEUP ) ); // "수정해야함 : 선수 구성이 완료 되었습니다." );
		}
	}
	else
	{
		// 출전할수 없음.. 그러므로 선택할수 없음.
	}
}
void CDPClient::OnGCUserState( CAr & ar )
{
	int		nGuildCombatState;
	OBJID	idPlayer;
	ar >> idPlayer;
	ar >> nGuildCombatState;
	CMover *pMover = prj.GetMover( idPlayer );
	if( IsValidObj( pMover ) )
	{
		pMover->m_nGuildCombatState = nGuildCombatState;
	}
}
// 자신의 길드 상황
void WndWorld::GuildCombatInfo::OnGuildStatus(CAr & ar) {
	GuildStatus.clear();

	int nJoinCount; ar >> nJoinCount;
	int nSize;  ar >> nSize;

	for (int i = 0; i < nSize; ++i) {
		GUILDRATE rate;
		ar >> rate.m_uidPlayer;
		ar >> rate.nLife;
		rate.bJoinReady = nJoinCount == i && nJoinCount != 0;
		GuildStatus.emplace_back(rate);
	}
}

// 길드 순위
void WndWorld::GuildCombatPrecedence::OnGuildPrecedence(CAr & ar) {
	guilds.clear();
	idToGuildName.clear();

	int nSize; ar >> nSize;

	u_long guildId;
	int nGuildPoint;
	char strGuildName[128];
	for (int i = 0; i < nSize; ++i) {
		ar >> guildId;
		ar >> strGuildName;
		ar >> nGuildPoint;

		guilds.emplace(nGuildPoint, guildId);
		idToGuildName.emplace(guildId, strGuildName);
	}
}

// 개인 순위
void WndWorld::GuildCombatPrecedence::OnPlayerPrecedence(CAr & ar) {
	players.clear();

	int nGuildSize; ar >> nGuildSize;
	for (int i = 0; i < nGuildSize; ++i) {
		int nPlayerSize; ar >> nPlayerSize;
		for (int j = 0; j < nPlayerSize; ++j) {
			const auto [uidPlayer, nPoint] = ar.Extract<u_long, int>();

			players.emplace(nPoint, uidPlayer);
		}
	}
}

void CDPClient::OnGCJoinWarWindow( CAr & ar )
{
	// 대전 참가 윈도우 창을 띄우기
	int nMap;
	int nJoinWarTime;
	ar >> nMap;					// 기존값 , 디폴트 값 99
	ar >> nJoinWarTime;		// 대전에 참가할 시간

	SAFE_DELETE(g_WndMng.m_pWndGuildCombatJoinSelection);
	g_WndMng.m_pWndGuildCombatJoinSelection = new CWndGuildCombatJoinSelection;

	if( g_WndMng.m_pWndGuildCombatJoinSelection  )
	{
		g_WndMng.m_pWndGuildCombatJoinSelection->Initialize();
		g_WndMng.m_pWndGuildCombatJoinSelection->SetMapNum( nMap );
		g_WndMng.m_pWndGuildCombatJoinSelection->SetTimeOut( nJoinWarTime * 1000 );
	}
}
void CDPClient::OnGCGetPenyaGuild( CAr & ar )
{
	int		nGetResult;			
	__int64	nGetPenya;
	ar >> nGetResult;
	ar >> nGetPenya;
	switch( nGetResult )
	{
	case 0:		// 성공( 보상금 )
		{
			CString str;
			str.Format( prj.GetText(TID_GAME_GUILDCOMBAT_JACKPOT), nGetPenya );
			g_WndMng.OpenMessageBox( str );
		}
		break;
	case 1:		// 성공( 신청금 )
		{
			CString str;
			str.Format( prj.GetText(TID_GAME_GUILDCOMBAT_JACKPOT_CANCEL), nGetPenya );
			g_WndMng.OpenMessageBox( str );
		}
		break;
	case 2:		// 얻을게 없음
		{
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT_WINNER_NOT_GUILD) ); //금주 길드대전에서 우승한 길드가 아닙니다.	
		}
		break;
	case 3:		// 얻을것은 있지만 오버플로 가능 하므로 따로 처리
		{
			__int64 nPenya = nGetPenya + (__int64)g_pPlayer->GetGold();

			if( nGetPenya > INT_MAX )
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT_JACKPOT_LIMIT2) );
			}
			else
			if( nPenya > INT_MAX )
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT_JACKPOT_LIMIT1) );
			}

		}
		break;
	}
}
void CDPClient::OnGCGetPenyaPlayer( CAr & ar )
{
	int		nGetResult;			
	__int64 nGetPenya;
	ar >> nGetResult;
	ar >> nGetPenya;
	switch( nGetResult )
	{
	case 0:		// 성공( 보상금 )
		{
			CString str;
			str.Format( prj.GetText(TID_GAME_GUILDCOMBAT_MVP_PENYA), nGetPenya ); //MVP상금올 %d페냐를 지급받았습니다. 다시한번 축하드립니다!
			g_WndMng.OpenMessageBox( str );
		}
		break;
	case 1:		// 얻을게 없음
		{
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT_NOT_MVP) ); //당신은 금주 길드대전의 MVP가 아닙니다.
		}
		break;
	case 2:		// 얻을것은 있지만 오버플로 가능 하므로 따로 처리
		{
			__int64 nPenya = nGetPenya + (__int64)g_pPlayer->GetGold();
			
			if( nGetPenya > INT_MAX )
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT_JACKPOT_LIMIT2) );
			}
			else
			if( nPenya > INT_MAX )
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT_JACKPOT_LIMIT1) );
			}				
		}
		break;
	}
}
void CDPClient::OnGCDiagMessage( CAr & ar )
{
	char strMessage[512] = {0,};
	ar.ReadString( strMessage, 512 );
	
	SAFE_DELETE( g_WndMng.m_pWndGuildCombatInfoMessageBox2);
	g_WndMng.m_pWndGuildCombatInfoMessageBox2 = new CGuildCombatInfoMessageBox2;
	g_WndMng.m_pWndGuildCombatInfoMessageBox2->Initialize();
	g_WndMng.m_pWndGuildCombatInfoMessageBox2->SetString( strMessage );
}
void CDPClient::OnGCTele( CAr & ar )
{
	char strMessage[512] = {0,};
	ar.ReadString( strMessage, 512 );

	SAFE_DELETE( g_WndMng.m_pWndGuildCombatInfoMessageBox);
	g_WndMng.m_pWndGuildCombatInfoMessageBox = new CGuildCombatInfoMessageBox(0);
	g_WndMng.m_pWndGuildCombatInfoMessageBox->Initialize();
	g_WndMng.m_pWndGuildCombatInfoMessageBox->SetString( strMessage );
}

void CDPClient::OnIsRequest( CAr & ar )
{
	BOOL bRequest;
	ar >> bRequest;
	g_GuildCombatMng.m_bRequest = bRequest;
}

void CDPClient::OnGCLog( CAr & ar )
{
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	if (!pWndWorld) return;

	SAFE_DELETE( g_WndMng.n_pWndGuildCombatResult );
	g_WndMng.n_pWndGuildCombatResult = new CWndGuildCombatResult;
	g_WndMng.n_pWndGuildCombatResult->Initialize();

	static constexpr auto MakeName = [](const char * name) -> StaticString<MAX_NAME> {
		StaticString<MAX_NAME> retval;
		ComputeShortenName(retval.GetBuffer(), name, 10);
		return retval;
	};

	// 길드 순위
	CString strTemp;

	int nRate = 0;
	int nOldPoint = 0xffffffff;

	for (const auto & [nPoint, guildId] : pWndWorld->m_GCprecedence.guilds | std::views::reverse) {
		if (nOldPoint != nPoint)
			nRate++;

		const auto name = MakeName(pWndWorld->m_GCprecedence.GetGuildName(guildId));

		if (nOldPoint != nPoint) {
			if (nRate == 1)
				strTemp.Format("%2d   %s\t(%d) WINNER", nRate, name.GetRawStr(), nPoint);
			else
				strTemp.Format("%2d   %s\t(%d)", nRate, name.GetRawStr(), nPoint);
		} else {
			strTemp.Format("%s   %s\t(%d)", "  ", name.GetRawStr(), nPoint);
		}

		g_WndMng.n_pWndGuildCombatResult->InsertGuildRate(strTemp);

		nOldPoint = nPoint;
	}

	// 개인순위
	nRate = 0;
	nOldPoint = 0xffffffff;

	for (const auto & [nPoint, uiPlayer] : pWndWorld->m_GCprecedence.players | std::views::reverse) {
		if (nOldPoint != nPoint)
			nRate++;

		const auto name = MakeName(CPlayerDataCenter::GetInstance()->GetPlayerString(uiPlayer));

		if (nOldPoint != nPoint) {
			strTemp.Format("%2d   %s\t(%d)", nRate, name.GetRawStr(), nPoint);
		} else {
			strTemp.Format("%s   %s\t(%d)", "  ", name.GetRawStr(), nPoint);
		}

		if (uiPlayer == g_GuildCombatMng.m_uBestPlayer) {
			strTemp += " MVP";
		}

		g_WndMng.n_pWndGuildCombatResult->InsertPersonRate(strTemp);

		nOldPoint = nPoint;
	}

	// 로그 표시
	CGuild * pPlayerGuild = g_pPlayer->GetGuild();
	CString textualLog;
	const char * const szTempAttack = prj.GetText(TID_GAME_ATTACK);
	
	g_GuildCombatMng.m_vecGCGetPoint.clear();
	u_long uSize; ar >> uSize;
	for (u_long k = 0; k < uSize; ++k) {
		CGuildCombat::__GCGETPOINT GCGetPoint;
		ar >> GCGetPoint;
		g_GuildCombatMng.m_vecGCGetPoint.push_back(GCGetPoint);

		// 길드 정보
		const CGuild * const pGuildAtk = g_GuildMng.GetGuild(GCGetPoint.uidGuildAttack);
		const CGuild * const pGuildDef = g_GuildMng.GetGuild(GCGetPoint.uidGuildDefence);

		const char * const strGuild1 = GCGetPoint.bMaster ? prj.GetText(TID_GAME_GC_LOG_MASTER) : "";
		const char * const strAtk = CPlayerDataCenter::GetInstance()->GetPlayerString(GCGetPoint.uidPlayerAttack);
		const char * const strDef = CPlayerDataCenter::GetInstance()->GetPlayerString(GCGetPoint.uidPlayerDefence);
		const char * const strGuild2 = GCGetPoint.bDefender ? prj.GetText(TID_GAME_GC_LOG_DEFENDER) : "";

		CString szTempGuild; szTempGuild.Format(prj.GetText(TID_GAME_GC_LOG_GUILD), pGuildAtk->m_szGuild);
		CString szTempPoint; szTempPoint.Format(prj.GetText(TID_GAME_GC_LOG_POINT), GCGetPoint.nPoint);
		CString szTempGuildDef; szTempGuildDef.Format(prj.GetText(TID_GAME_GC_LOG_GUILD), pGuildDef->m_szGuild);

		const bool weAttacked = GCGetPoint.uidPlayerAttack == g_pPlayer->m_idPlayer
			|| (pPlayerGuild && pPlayerGuild->GetGuildId() == GCGetPoint.uidGuildAttack);

		const bool weDefended = !weAttacked && (pPlayerGuild && pPlayerGuild->GetGuildId() == GCGetPoint.uidGuildDefence);

		if (weAttacked) textualLog += "#cff009c00";
		textualLog.AppendFormat("%s %s %s%s", szTempGuild.GetString(), strGuild1, strAtk, szTempPoint.GetString());
		if (weAttacked) textualLog += "#nc";

		textualLog.AppendFormat(" %s -> ", szTempAttack); // TODO: get the real arrow symbol

		if (weDefended) textualLog += "#cff9c0000";
		textualLog.AppendFormat("%s %s %s", szTempGuildDef.GetString(), strGuild2, strDef);
		if (weDefended) textualLog += "#nc";
		textualLog += '\n';


		strTemp = prj.GetText(TID_GAME_GC_LOG1);
		if (GCGetPoint.bMaster)             strTemp.AppendFormat(", %s", prj.GetText(TID_GAME_GC_LOG2));
		if (GCGetPoint.bDefender)           strTemp.AppendFormat(", %s", prj.GetText(TID_GAME_GC_LOG3));
		if (GCGetPoint.bLastLife)           strTemp.AppendFormat(", %s", prj.GetText(TID_GAME_GC_LOG5));

		textualLog.AppendFormat("< %s >\n\r\n", strTemp.GetString());
	}

	g_WndMng.n_pWndGuildCombatResult->InsertLog(textualLog);
}

void CDPClient::OnGCLogRealTime( CAr & ar )
{
	char szAttacker[MAX_NAME], szDefender[MAX_NAME];
	CGuildCombat::__GCGETPOINT GCGetPoint;
	ar >> GCGetPoint;
	ar.ReadString( szAttacker, MAX_NAME );
	ar.ReadString( szDefender, MAX_NAME );
	
	LPCTSTR strGuild1; LPCTSTR strGuild2;

	CGuild* pGuildAtk = g_GuildMng.GetGuild( GCGetPoint.uidGuildAttack );
	CGuild* pGuildDef = g_GuildMng.GetGuild( GCGetPoint.uidGuildDefence );

	
	//[abc] 길드마스터 홍길동 님이 [울랄라] 엉터리 님을 쓰러뜨려 4점 획득!!
	if( ::GetLanguage() == LANG_ENG || ::GetLanguage() == LANG_VTN )
	{
		sprintf( szChat, "[%s][%s] has killed [%s][%s] and has gained %d points.",
			pGuildAtk->m_szGuild, szAttacker, pGuildDef->m_szGuild, szDefender, GCGetPoint.nPoint );
	}
	else
	{
		const bool bAtkMaster = pGuildAtk->IsMaster(GCGetPoint.uidPlayerAttack);
		strGuild1 = bAtkMaster ? prj.GetText(TID_GAME_GC_LOG_MASTER) : "";

		const bool bDefMaster = pGuildDef->IsMaster(GCGetPoint.uidPlayerDefence);
		strGuild2 =
			GCGetPoint.bDefender ? prj.GetText(TID_GAME_GC_LOG_DEFENDER) :
			bDefMaster           ? prj.GetText(TID_GAME_GC_LOG_MASTER)   :
			                       "";

		CString strAtk; strAtk.Format(prj.GetText(TID_GAME_GC_FROM3), szAttacker);
		CString strDef; strDef.Format(prj.GetText(TID_GAME_GC_FROM4), szDefender);

		CString szTempGuild;
		szTempGuild.Format(prj.GetText(TID_GAME_GC_LOG_GUILD), pGuildAtk->m_szGuild);
		CString szTempPoint;
		szTempPoint.Format(prj.GetText(TID_GAME_GC_ATTACK_POINT), GCGetPoint.nPoint);
		CString szTempGuildDef;
		szTempGuildDef.Format(prj.GetText(TID_GAME_GC_LOG_GUILD), pGuildDef->m_szGuild);

		sprintf( szChat, "%s %s %s %s %s %s %s",
			szTempGuild.GetString(), strGuild1, strAtk.GetString(),
			szTempGuildDef.GetString(), strGuild2, strDef.GetString(),
			szTempPoint.GetString()
		);
	}

	g_WndMng.PutString( (LPCTSTR)szChat, NULL, 0xffffffff, CHATSTY_GENERAL );
}

void CDPClient::OnGCPlayerPoint( CAr & ar )
{
	g_GuildCombatMng.m_vecGCPlayerPoint.clear();
	u_long uSize;
	ar >> uSize;

	SAFE_DELETE( g_WndMng.m_pWndGuildCombatRanking );
	g_WndMng.m_pWndGuildCombatRanking = new CWndGuildCombatRank_Person;	
	g_WndMng.m_pWndGuildCombatRanking->Initialize();

	std::vector<PDVer> vecPlayer;

	for( int i = 0 ; i < (int)( uSize ) ; ++i )
	{
		CGuildCombat::__GCPLAYERPOINT GCPlayerPoint;
		ar >> GCPlayerPoint;
		g_GuildCombatMng.m_vecGCPlayerPoint.push_back( GCPlayerPoint );

		vecPlayer.emplace_back(CPlayerDataCenter::GetInstance()->ToPDVer(GCPlayerPoint.uidPlayer));

		g_WndMng.m_pWndGuildCombatRanking->InsertRank( GCPlayerPoint.nJob, GCPlayerPoint.uidPlayer, GCPlayerPoint.nPoint );
	}

		CWndGuildCombatRank_Person* pWndRank	= (CWndGuildCombatRank_Person*)g_WndMng.GetWndBase( APP_GUILDCOMBAT_RANK_P );
		if( pWndRank )
			pWndRank->DivisionList();

		SendQueryPlayerData( vecPlayer );

	
}
void CDPClient::OnGuildCombatState( CAr & ar )
{
	int		nState;
	OBJID	nWarState;
	ar >> nState >> nWarState;
	g_GuildCombatMng.m_nState = nState;
	g_GuildCombatMng.m_nGCState = nWarState;

	if( g_GuildCombatMng.m_nGCState == CGuildCombat::NOTENTER_COUNT_STATE )
	{
		CGuild* pGuild	= g_pPlayer->GetGuild();
		if( pGuild && g_GuildCombatMng.IsRequest() )	
		{
			if( pGuild->IsMaster( g_pPlayer->m_idPlayer ) )
			{
				g_WndMng.PutString( prj.GetText(TID_GAME_GUILDCOMBAT_NOW_PLAYER_MAKEUP) ); //지금 부터 명단작성을 할 수 있습니다.
			}
		}
	}
	

	CWorld* pWorld = g_pPlayer->GetWorld();
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	if( pWndWorld && pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR )
	{
		// 정비시작
		if( g_GuildCombatMng.m_nState == CGuildCombat::WAR_STATE &&
			g_GuildCombatMng.m_nGCState == CGuildCombat::MAINTENANCE_STATE )
		{
			CD3DFont* pOldFont = g_Neuz.m_2DRender.GetFont();
			g_Neuz.m_2DRender.SetFont( CWndBase::m_Theme.m_pFontText );
			g_WndMng.PutString( prj.GetText(TID_GAME_GUILDCOMBAT_MAINTENANCE2), NULL, 0xffffffff );
			g_Neuz.m_2DRender.SetFont( pOldFont );			
		}
			
		// 전투시작
		if( g_GuildCombatMng.m_nState == CGuildCombat::WAR_STATE &&
			g_GuildCombatMng.m_nGCState == CGuildCombat::WAR_WAR_STATE )
		{
			PlayMusic( BGM_BA_CRISIS, 0 );
			g_Caption1.AddCaption( prj.GetText(TID_GAME_GUILDCOMBAT_WAR), pWndWorld->m_pFontAPITitle );			
		}

		// 전투종료
		if( g_GuildCombatMng.m_nState == CGuildCombat::COMPLET_CLOSE_STATE &&
			g_GuildCombatMng.m_nGCState == CGuildCombat::WAR_CLOSE_STATE )
		{
			g_Caption1.AddCaption( prj.GetText(TID_GAME_GUILDCOMBAT_WAR_END), pWndWorld->m_pFontAPITitle );			
			//g_GuildCombatMng.m_bRequest = FALSE;
		}

		// 길드대전 종료 대기
		if( g_GuildCombatMng.m_nState == CGuildCombat::COMPLET_CLOSE_STATE &&
			g_GuildCombatMng.m_nGCState == CGuildCombat::WAR_CLOSE_WAIT_STATE )
		{
			g_GuildCombatMng.m_bRequest = FALSE;
		}
		
		// 돌아가는시점
		if( g_GuildCombatMng.m_nState == CGuildCombat::CLOSE_STATE &&
			g_GuildCombatMng.m_nGCState == CGuildCombat::WAR_TELEPORT_STATE )
		{
			pWndWorld->m_dwGuildCombatTime = 0xffffffff;

			// 길드원들 왼쪽 리스트 상태...
			pWndWorld->m_infoGC.ClearPlayerList();
		}
	}
}

void CDPClient::OnGuildCombatNextTimeState( CAr & ar )
{
	DWORD dwTime, dwGuildCombatState;
	ar >> dwTime >> dwGuildCombatState;
	
	CWorld* pWorld = g_pPlayer->GetWorld();
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	if( pWorld == NULL || pWndWorld == NULL )
	{
		Error( "CDPClient::OnGuildCombatNextTimeState()에 World가 없다" );
		return;
	}
	
	{
		BOOL bTimeSet = FALSE;
		switch( dwGuildCombatState )
		{
			case CGuildCombat::WAR_WAR_STATE:
				{
					if( pWorld->GetID() == WI_WORLD_GUILDWAR )
					{
						bTimeSet = TRUE;
						sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_MAINTENANCE) );			
					}
				}
				break;
			case CGuildCombat::WAR_CLOSE_STATE:
				{
					if( pWorld->GetID() == WI_WORLD_GUILDWAR )
					{
						bTimeSet = TRUE;
						sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_WAR_END) );			
					}
				}
				break;
			case CGuildCombat::WAR_TELEPORT_STATE:
				{
					if( pWorld->GetID() == WI_WORLD_GUILDWAR )
					{
						bTimeSet = TRUE;
						sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_EXIT_MSG) );			
					}
				}
				break;
			case CGuildCombat::ENTER_STATE:
				{
					if( g_pPlayer == NULL )
						return;

					CGuild* pGuild = g_pPlayer->GetGuild();
					CGuildMember* pGuildMemberl;
					
					if( pGuild && g_GuildCombatMng.IsRequest() )
					{
						pGuildMemberl = pGuild->GetMember( g_pPlayer->m_idPlayer );
						
						if( pGuildMemberl && (pGuildMemberl->m_nMemberLv == GUD_MASTER || pGuildMemberl->m_nMemberLv == GUD_KINGPIN) )
						{
							bTimeSet = TRUE;
							sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_PLAYER_MAKEUP_FINISH) ); //길드대전 명단작성 및 준비시간
						}
					}
				}
				break;
		}

		if( bTimeSet )
		{
			pWndWorld->m_dwGuildCombatTime = GetTickCount() + dwTime;
		}
	}
}

void CDPClient::OnGuildCombatEnterTime( CAr & ar )
{
	DWORD dwEnterTime;
	ar >> dwEnterTime;
	
	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	if( pWndWorld && g_GuildCombatMng.IsRequest())
	{
		pWndWorld->m_dwGuildCombatTime = GetTickCount() + dwEnterTime;
		
		sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_JOIN_LIMIT_TIME) );
		
		g_WndMng.PutString( prj.GetText(TID_GAME_GUILDCOMBAT_JOIN_NOW), NULL, 0xffffffff );	
	}
	
}
void CDPClient::OnGCWinGuild( CAr & ar )
{
	ar >> g_GuildCombatMng.m_nGuildCombatIndex >> g_GuildCombatMng.m_uWinGuildId >> g_GuildCombatMng.m_nWinGuildCount;
}
void CDPClient::OnGCBestPlayer( CAr & ar )
{
	ar >> g_GuildCombatMng.m_uBestPlayer;
}

void CDPClient::OnFriendGameJoin( CAr & ar )
{
	g_WndMng.m_RTMessenger.Serialize(ar);
	g_WndMng.m_bConnect = TRUE;

	if (!g_WndMng.m_RTMessenger.empty()) {
		SendGetFriendState();
	}

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}

void CDPClient::OnAddFriend( CAr & ar ) {
	u_long uidPlayer;
	char lpName[MAX_PLAYER]	= { 0,};

	ar >> uidPlayer;
	ar.ReadString(lpName, MAX_PLAYER);

	g_WndMng.m_RTMessenger.SetFriend(uidPlayer);
	SendGetFriendState();

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);

	g_WndMng.PutString(TID_GAME_MSGINVATECOM, lpName);
}

void CDPClient::OnRemoveFriend( CAr & ar )
{
	u_long uidPlayer;
	ar >> uidPlayer;
	g_WndMng.m_RTMessenger.RemoveFriend( uidPlayer );

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}


void CDPClient::OnAddFriendReqest( CAr & ar )
{
	char szLeaderName[MAX_NAME] = {0,};
	u_long uLeaderid;
	LONG nLeaderJob;
	BYTE nLeaderSex;
	ar >> uLeaderid;
	ar >> nLeaderSex;
	ar >> nLeaderJob;
	ar.ReadString( szLeaderName, MAX_NAME );
	
	/*
	CWndFriendConFirm* pFriendConfirm=new CWndFriendConFirm;
	pFriendConfirm->Initialize();
	pFriendConfirm->SetMember( uLeaderid, g_pPlayer->m_idPlayer, nLeaderSex );
	*/
	// 신청하는 형태의 매시지 박스는 여러개가 날아올 수 있고, 내 의지와는 관계가 없다.
	// 이런 경우 CWndDuelConfirm::OnChildNotify에서 윈도를 닫을 때 Destroy( TRUE )로 해서
	// 메모리에서 완전히 파괴하도록 한다. 안하면 리크로 남던지, ALT+ENTER, ALT+TAB할 때 다운됨.
	//SAFE_DELETE( g_WndMng.m_pWndFriendConfirm );

	if( g_Option.m_bMessenger == FALSE )
	{
		SendBlock( 2, g_pPlayer->GetName(), szLeaderName );
		return;
	}

	CWndBase* pWndBaseBuf = g_WndMng.GetWndBase( APP_FRIEND_CONFIRM );
	
	if( pWndBaseBuf )
	{
		SendBlock( 6, g_pPlayer->GetName(), szLeaderName );
		return;
	}

	g_WndMng.m_pWndFriendConfirm = new CWndFriendConFirm;
	g_WndMng.m_pWndFriendConfirm->Initialize();
	g_WndMng.m_pWndFriendConfirm->SetMember( uLeaderid, szLeaderName );
}

void CDPClient::OnAddFriendCancel( CAr & ar )
{
	//g_WndMng.PutString( "메신저 추가를 거절하였습니다.", NULL, 0xffffff00 );
	g_WndMng.PutString( prj.GetText(TID_GAME_MSGDENYINVATE), NULL, prj.GetTextColor(TID_GAME_MSGDENYINVATE));
}

void CDPClient::OnAddFriendError( CAr & ar )
{
	char szName[64] = {0,};
	BYTE nError;
	ar >> nError;
	ar.ReadString( szName, 64 );
	CString szMessage;
	if( nError == 1 )
		szMessage.Format( prj.GetText(TID_DIAG_0059), szName );// += "님은 이미 메신저에 추가 되어 있습니다";
	else if( nError == 2 )
		szMessage.Format( prj.GetText(TID_DIAG_0060), szName );// += " 이름을 가지고 있는 사용자가 없습니다";

	g_WndMng.OpenMessageBox( szMessage );
}

void CDPClient::OnAddFriendNotConnect( CAr & ar )
{
	char szName[64] = {0,};
	ar.ReadString( szName, 64 );
	CString szMessage;
	szMessage.Format( prj.GetText(TID_DIAG_0061), szName );
//	szMessage += "님은 접속되어 있지 않습니다";
	g_WndMng.OpenMessageBox( szMessage );
}

void CDPClient::OnRemoveFriendState( CAr & ar )
{
	u_long uRemoveid;
	ar >> uRemoveid;

	g_WndMng.m_RTMessenger.SetFriend( uRemoveid );

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}

void CDPClient::OnFriendJoin(CAr & ar) {
	const auto [idFriend, dwState] = ar.Extract<u_long, FriendStatus>();

	Friend * const pFriend = g_WndMng.m_RTMessenger.GetFriend(idFriend);
	if (!pFriend) return;

	pFriend->dwState = dwState;

	if (g_Option.m_bMessengerJoin && pFriend->dwState != FriendStatus::OFFLINE) {
		const char * friendName = CPlayerDataCenter::GetInstance()->GetPlayerString(idFriend);

		if (CWndInstantMsg * pWndInstantMsg = g_WndMng.OpenInstantMsg(friendName)) {
			pWndInstantMsg->AddMessageJoin(friendName);
		}
	}

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}

void CDPClient::OnFriendLogOut( CAr & ar )
{
	u_long idFriend;
	ar >> idFriend;
	Friend* pFriend		= g_WndMng.m_RTMessenger.GetFriend( idFriend );
	if( pFriend )
	{
		pFriend->dwState	= FriendStatus::OFFLINE;

		CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
	}
}

void CDPClient::OnFriendNoIntercept( CAr & ar )
{
	u_long idFriend;
	ar >> idFriend;
	g_WndMng.m_RTMessenger.SetBlock(idFriend, false);

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}

void CDPClient::OnFriendIntercept( CAr & ar )
{
	u_long idPlayer, idFriend;
	ar >> idPlayer >> idFriend;
	if( g_pPlayer )
	{
		if( g_pPlayer->m_idPlayer == idPlayer )
		{
			g_WndMng.m_RTMessenger.SetBlock( idFriend, true );
		}
		else
		{
			Friend* pFriend		= g_WndMng.m_RTMessenger.GetFriend( idPlayer );
			if( pFriend )
				pFriend->dwState	= FriendStatus::OFFLINE;
		}
		
		CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
	}
}

void CDPClient::OnGetFriendState(CAr & ar) {
	std::uint32_t size; ar >> size;

	if (size == 0) return;

	for (std::uint32_t i = 0; i != size; ++i) {
		const auto [uidPlayer, dwState] = ar.Extract<u_long, FriendStatus>();

		Friend * pFriend = g_WndMng.m_RTMessenger.GetFriend(uidPlayer);
		if (pFriend) {
			pFriend->dwState = dwState;
		}
	}

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}

void CDPClient::OnSetFriendState(CAr & ar) {
	const auto [uidPlayer, dwState] = ar.Extract<u_long, FriendStatus>();

	if (!g_pPlayer) return;

	if (g_pPlayer->m_idPlayer == uidPlayer) {
		g_WndMng.m_RTMessenger.SetState(dwState);
	} else {
		if (Friend * pFriend = g_WndMng.m_RTMessenger.GetFriend(uidPlayer)) {
			pFriend->dwState = dwState;
		}

		CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
	}
}

void CDPClient::OnBlock(CAr & ar) {
	const auto [nGu, szName] = ar.Extract<BYTE, char[MAX_NAME]>();

	CString szMessage;
	if (nGu == 1) {
		if (CWndMessage * pWndMessage = g_WndMng.GetMessage(szName)) {
			pWndMessage->Destroy();
		}
	}

	switch (nGu) {
		case 1: g_WndMng.PutString(TID_GAME_MSGCHATDENY, szName);     break;
		case 2: g_WndMng.PutString(TID_GAME_MSGDENY, szName);         break;
		case 3: g_WndMng.PutString(TID_GAME_TRADEDENY, szName);       break;
		case 4: g_WndMng.PutString(TID_GAME_REPAIR_NOTTRADE, szName); break;
		case 5: g_WndMng.PutString(TID_GAME_YETTRADE);                break;
		case 6: g_WndMng.PutString(TID_GAME_YETFRIEND);               break;
		case 7: g_WndMng.PutString(TID_GAME_YETPARTY);                break;
		case 8: g_WndMng.PutString(TID_GAME_TRADELIMITPC);            break;
	}
}

void CDPClient::OnTagResult(CAr & ar) {
	bool cbResult; ar >> cbResult;
	// 결과: 0 - 실패(20개 초과의 경우), 1 - 성공 
	if (!cbResult) {
		g_WndMng.PutString(TID_GAME_MESSFULLNOTE);
	} else if (cbResult) {
		g_WndMng.PutString(TID_GAME_MESSSENTNOET);
	}
}

void CDPClient::OnSetGuildQuest(CAr & ar) {
	const auto [nQuestId, nState] = ar.Extract<int, int>();

	if (!g_pPlayer) return;

	CGuild * pGuild = g_pPlayer->GetGuild();
	if (!pGuild) return;
	
	TRACE("SNAPSHOTTYPE_SETGUILDQUEST\n");
	pGuild->SetQuest(nQuestId, nState);
}

void CDPClient::OnRemoveGuildQuest(CAr & ar) {
	int nQuestId; ar >> nQuestId;

	if (!g_pPlayer) return;

	CGuild * pGuild = g_pPlayer->GetGuild();
	if (!pGuild) return;

	TRACE("SNAPSHOTTYPE_REMOVEGUILDQUEST\n");
	pGuild->RemoveQuest(nQuestId);
}


void CDPClient::OnSetQuest( OBJID objid, CAr & ar ) {
	QUEST quest; ar >> quest;
	
	CString strTemp;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) && pMover->IsActiveMover() )
	{
		CWndQuestDetail* pWndQuestDetail = g_WndMng.m_pWndQuestDetail;
		CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

		LPQUEST lpCurQuest = pMover->GetQuest( quest.m_wId );
		const QuestProp * pQuestProp = quest.GetProp();
		// 기존 내용 수정 
		if( lpCurQuest && quest.m_nState != QS_END )
		{
			// 시간 출력 
			DWORD dwCurTime = lpCurQuest->m_wTime & 0x7fff;
			DWORD dwTime = quest.m_wTime & 0x7fff;
			if( dwCurTime != dwTime )
			{
				strTemp.Format( GETTEXT( TID_QUEST_LIMIT_TIME ), dwTime );
				g_WndMng.PutString( strTemp );
				if( dwTime  )
				{
					if( ::GetLanguage() != LANG_JAP )
						g_CapTime.SetTime( dwTime, pWndWorld->m_pFontAPITime );
					else
						g_CapTime.SetTime( dwTime, NULL );
				}
				else
				{
					g_WndMng.PutString( GETTEXT( TID_QUEST_LIMIT_TIMEOUT ) );
					g_WndMng.PutString( GETTEXT( TID_QUEST_FAILURE ) ); 
					SendRemoveQuest(quest.m_wId);
				}
			}
			// 정찰 지역 
			if( lpCurQuest->m_bPatrol != quest.m_bPatrol )
			{
				strTemp.Format( GETTEXT( TID_QUEST_PATROL  ), pQuestProp->m_szPatrolZoneName );
				g_WndMng.PutString( strTemp );
			}
			// 사냥 갯수 
			for( int i = 0; i < 2; i++ )
			{
				if( lpCurQuest->m_nKillNPCNum[ i ] != quest.m_nKillNPCNum[ i ] )
				{
					MoverProp* pMoverProp = prj.GetMoverProp( pQuestProp->m_nEndCondKillNPCIdx[ i ] );
					strTemp.Format( GETTEXT( TID_QUEST_KILL_NPC ), pMoverProp->szName, quest.m_nKillNPCNum[ i ], pQuestProp->m_nEndCondKillNPCNum[ i ] );
					g_WndMng.PutString( strTemp );
				}		
			}
			// 대화하기 
			if( lpCurQuest->m_bDialog != quest.m_bDialog )
			{
				LPCHARACTER lpCharacter = prj.GetCharacter( pQuestProp->m_szEndCondDlgCharKey );
				strTemp.Format( GETTEXT( TID_QUEST_DIALOG ), lpCharacter->m_strName );
				g_WndMng.PutString( strTemp );
			}
			pMover->SetQuest( &quest );
			if( pWndQuestDetail )
				pWndQuestDetail->UpdateQuestText();

			CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
			if( pWndQuestQuickInfo )
				pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
		}
		// 신규 추가 
		else
		{
			pMover->SetQuest( &quest );
			if( pWndQuestDetail )
				pWndQuestDetail->UpdateQuestText( quest.m_wId.get() ); 

			CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
			if( pWndQuestQuickInfo )
				pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
		}

		//if( nQuestIdx == 1 ) 
		//{
		//QuestProp* pQuestProp = prj.m_aPropQuest.GetAt( quest.m_wId );
		if( pQuestProp )
		{
			QuestState* pQuestState = pQuestProp->m_questState[ quest.m_nState ];
			if( quest.m_nState == 1 )
				g_WndMng.PutDefinedString( TID_EVE_STARTJOB1, ""  );
				//g_WndMng.PutString( prj.GetText( TID_EVE_STARTJOB1 ), NULL, prj.GetTextColor( TID_EVE_STARTJOB1 ) );
			else 
			if( quest.m_nState == QS_END ) // VHOME
			{
				if( quest.m_wId != QuestId(QUEST_CREGUILD))
					g_WndMng.PutDefinedString( TID_EVE_ENDJOB1, ""  );
			}
				//g_WndMng.PutString( prj.GetText( TID_EVE_ENDJOB1 ), NULL, prj.GetTextColor( TID_EVE_ENDJOB1 ) );
		}
		//}
	}
}

void CDPClient::OnMoverSetDestObj( OBJID objid, CAr & ar )
{
	CMover* pMover	= prj.GetMover( objid );
	float fRange;
	ar >> objid >> fRange;

	CCtrl* pCtrl	= prj.GetCtrl( objid );
	if( IsValidObj( (CObj*)pMover ) && IsValidObj( (CCtrl*)pCtrl ) )
		pMover->SetDestObj( objid, fRange, FALSE );
}

const	int	nRevision	= 35;

void CDPClient::OnMoverMoved( OBJID objid, CAr & ar )
{
	TRACE( "OnMoverMoved()\n" );
	D3DXVECTOR3 v, vd;
	float f;
	DWORD dwState, dwStateFlag, dwMotion	= 0;
	int nMotionEx	= 0, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) ) {
		if( pMover->m_pActMover->IsFly() )		return;
		int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 16.6667f );
		// 2
		// 3
			TRACE( "d = %d\n", delay );
			delay	-= nRevision;	// an accidental error
#ifdef __SYNC_1217
			BOOL bBMove	= ( pMover->m_pActMover->GetMoveState() == OBJSTA_BMOVE ) 
											|| ( ( dwState & OBJSTA_MOVE_ALL ) == OBJSTA_BMOVE );
#endif	// __SYNC_1217
			if( delay >= MAX_CORR_SIZE_45 ) {
				pMover->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
			}
			else {
				if( pMover->m_pActMover->IsStateFlag( OBJSTAF_FLY ) || dwStateFlag & OBJSTAF_FLY ) {
					pMover->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
				}
				else {
					pMover->m_pActMover->DefaultSet();

					int nRemnant	= (int)( MAX_CORR_SIZE_45 - delay );
#ifdef __SYNC_1217
					if( bBMove )
						nRemnant	= 1;
#endif	// __SYNC_1217
					pMover->m_uRemnantCorrFrm	= nRemnant;
					D3DXVECTOR3 d	= v - pMover->GetPos();
					float fLength	= D3DXVec3Length( &d );
#ifdef __SYNC_1217
					float fSpeed	= pMover->GetSpeed( pMover->m_pActMover->m_fSpeed );
#else	// __SYNC_1217
					float fSpeed	= ( fLength / (float)nRemnant ) + 0.005f;
#endif	// __SYNC_1217
					pMover->m_fCrrSpd	= pMover->m_pActMover->IsStateFlag( OBJSTAF_WALK )? fSpeed*4.0f: fSpeed;

					pMover->m_CorrAction.v	= v;
					pMover->m_CorrAction.vd		= vd;
					pMover->m_CorrAction.f	= f;
					pMover->m_CorrAction.dwState	= dwState;
					pMover->m_CorrAction.dwStateFlag	= dwStateFlag;
					pMover->m_CorrAction.dwMotion	= dwMotion;
					pMover->m_CorrAction.nMotionEx	= nMotionEx;
					pMover->m_CorrAction.nLoop	= nLoop;
					pMover->m_CorrAction.dwMotionOption	= dwMotionOption;
					pMover->m_CorrAction.fBehavior	= FALSE;
					pMover->m_CorrAction.fValid	= TRUE;

					DWORD dwTurnState	= dwState & OBJSTA_TURN_ALL;
					if( pMover->m_pActMover->GetMoveState() == OBJSTA_STAND && ( dwState & OBJSTA_MOVE_ALL ) == OBJSTA_STAND && dwTurnState != 0 ) {
						pMover->SetDestAngle( f, ( dwTurnState == OBJSTA_LTURN )? 1: 0 );
					}
					else {
						pMover->ClearDestAngle();
						pMover->SendActMsg( OBJMSG_STOP_TURN );
						pMover->SetDestPos( v, ( dwState & OBJSTA_MOVE_ALL ) != OBJSTA_BMOVE );
					}
				}
			}
	}
}

void CDPClient::OnMoverBehavior( OBJID objid, CAr & ar )
{
	TRACE( "OnMoverBehavior()\n" );
	D3DXVECTOR3 v, vd;
	float f;
	DWORD dwState, dwStateFlag, dwMotion	= 0;
	int nMotionEx	= 0, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) ) {
		if( pMover->m_pActMover->IsFly() )		return;
		int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 16.6667f );
		TRACE( "d = %d\n", delay );
		delay	-= nRevision;	// an accidental error

		if( !pMover->IsEmptyDestPos() && pMover->m_CorrAction.fValid == FALSE ) {
//			D3DXVECTOR3 vTmp	= pMover->GetDestPos();
			memset( &pMover->m_CorrAction, 0, sizeof(CORR_ACTION) );
			pMover->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
//			pMover->SetDestPos( vTmp );
		}
		else
		{
//			if( delay >= MAX_CORR_SIZE_45 ) {
				pMover->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
				/*
			}
			else {
				pMover->m_pActMover->DefaultSet();

				int nRemnant	= (int)( MAX_CORR_SIZE_45 - delay );
				pMover->m_uRemnantCorrFrm	= nRemnant;
				D3DXVECTOR3 d	= v - pMover->GetPos();
				float fLength	= D3DXVec3Length( &d );
				float fSpeed	= ( fLength / (float)nRemnant ) + 0.005f;

//				pMover->SetDestParam( DST_SPEED, 0, (int)( pMover->m_pActMover->IsStateFlag( OBJSTAF_WALK )? fSpeed*4.0f: fSpeed ) * 10000, FALSE );
				pMover->m_fCrrSpd	= pMover->m_pActMover->IsStateFlag( OBJSTAF_WALK )? fSpeed*4.0f: fSpeed;

				pMover->m_CorrAction.v	= v;
				pMover->m_CorrAction.vd		= vd;
				pMover->m_CorrAction.f	= f;
				pMover->m_CorrAction.dwState	= dwState;
				pMover->m_CorrAction.dwStateFlag	= dwStateFlag;
				pMover->m_CorrAction.dwMotion	= dwMotion;
				pMover->m_CorrAction.nMotionEx	= nMotionEx;
				pMover->m_CorrAction.nLoop	= nLoop;
				pMover->m_CorrAction.dwMotionOption	= dwMotionOption;
				pMover->m_CorrAction.fBehavior	= TRUE;
				pMover->m_CorrAction.fValid	= TRUE;

				pMover->ClearDestAngle();
				pMover->SetDestPos( v );
			}
			*/
		}
	}
}

void CDPClient::OnMoverMoved2( OBJID objid, CAr & ar )
{
	TRACE( "OnMoverMoved2()\n" );
	D3DXVECTOR3 v, vd;
	float f, fAngleX, fAccPower, fTurnAngle;
	DWORD dwState, dwStateFlag, dwMotion	= 0;
	int nMotionEx	= 0, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;
	BYTE nFrame;

	ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;
	ar >> nFrame;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) ) {
		if( pMover->m_pActMover->IsFly() == FALSE )		return;
		// 2
		// 3
		int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 16.6667f );
		TRACE( "d = %d\n", delay );
//		delay	-= ( nRevision * 2);	// an accidental error

		if( delay >= nFrame ) {
			pMover->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
		}
		else
		{
			if( dwState & OBJSTA_STAND )
			{
				D3DXVECTOR3 v1	= v - pMover->GetPos();
				if( D3DXVec3LengthSq( &v1 ) > 0.01 )
				{
					D3DXVECTOR3 vDestPos	= v;
					float fmov	= GetDegree( vDestPos, pMover->GetPos() );
					float fdiff	= fabs( fmov - f );
					if( fdiff > 90.0f && fdiff < 270.0f )
					{
						pMover->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
						return;
					}
				}
			}

			pMover->m_pActMover->DefaultSet();

			pMover->m_CorrAction.v	= v;
			pMover->m_CorrAction.vd		= vd;
			pMover->m_CorrAction.f	= f;
			pMover->m_CorrAction.dwState	= dwState;
			pMover->m_CorrAction.dwStateFlag	= dwStateFlag;
			pMover->m_CorrAction.dwMotion	= dwMotion;
			pMover->m_CorrAction.nMotionEx	= nMotionEx;
			pMover->m_CorrAction.nLoop	= nLoop;
			pMover->m_CorrAction.dwMotionOption	= dwMotionOption;
			pMover->m_CorrAction.fAngleX	= fAngleX;
			pMover->m_CorrAction.fAccPower	= fAccPower;
			pMover->m_CorrAction.fTurnAngle	= fTurnAngle;
			pMover->m_CorrAction.fBehavior	= FALSE;
			pMover->m_CorrAction.fValid	= TRUE;

			DWORD dwTurnState	= dwState & OBJSTA_TURN_ALL;
			DWORD dwLookState	= dwState & OBJSTA_LOOK_ALL;

			bool fLook	= false;
			 if( ( dwState & OBJSTA_MOVE_ALL ) == OBJSTA_STAND && pMover->m_pActMover->GetLookState() == 0 && dwLookState != 0 )
			 {
				 fLook	= true;
				pMover->SendActMsg( dwLookState == OBJSTA_LOOKUP? OBJMSG_LOOKUP: OBJMSG_LOOKDOWN );
			 }
			 else if( ( dwState & OBJSTA_MOVE_ALL ) == OBJSTA_STAND && pMover->m_pActMover->GetLookState() != 0 && dwLookState == 0 )
			 {
				 fLook	= true;
				pMover->SendActMsg( OBJMSG_STOP_LOOK );
			 }

			if( ( dwState & OBJSTA_MOVE_ALL ) == OBJSTA_STAND && pMover->m_pActMover->GetTurnState() == 0 && dwTurnState != 0 )
			{
				pMover->SetDestAngle( f, ( dwTurnState == OBJSTA_LTURN )? 1: 0 );
			}
			else if( ( dwState & OBJSTA_MOVE_ALL ) == OBJSTA_STAND && pMover->m_pActMover->GetTurnState() != 0 && dwTurnState == 0 )
			{
				pMover->ClearDestAngle();
				pMover->SendActMsg( OBJMSG_STOP_TURN );
				pMover->SetAngle( f );
			}
			else {
				if( fLook == true )
					return;

				pMover->ClearDestAngle();
//				int nRemnant	= (int)( ( nFrame ) - delay );
				int nRemnant	= (int)nFrame;
				pMover->m_uRemnantCorrFrm	= nRemnant;
				pMover->SetDestPos( v );
			}
//			pMover->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
		}
	}
}

void CDPClient::OnMoverBehavior2( OBJID objid, CAr & ar )
{
	D3DXVECTOR3 v, vd;
	float f, fAngleX, fAccPower, fTurnAngle;
	DWORD dwState, dwStateFlag, dwMotion;
	int nMotionEx, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	CMover* pMover	= prj.GetMover( objid );


	int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 16.6667f );

	TRACE( "d = %d\n", delay );
	delay	-= ( nRevision * 2 );	// an accidental error

	if( IsValidObj( (CObj*)pMover ) ) {
		if( pMover->m_pActMover->IsFly() == FALSE )		return;
		if( dwStateFlag & OBJSTAF_FLY ) {
			if( delay >= MAX_CORR_SIZE_150 ) {
				pMover->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
			}
			else {
				pMover->m_pActMover->DefaultSet();

				pMover->m_CorrAction.v	= v;
				pMover->m_CorrAction.vd		= vd;
				pMover->m_CorrAction.f	= f;
				pMover->m_CorrAction.dwState	= dwState;
				pMover->m_CorrAction.dwStateFlag	= dwStateFlag;
				pMover->m_CorrAction.dwMotion	= dwMotion;
				pMover->m_CorrAction.nMotionEx	= nMotionEx;
				pMover->m_CorrAction.nLoop	= nLoop;
				pMover->m_CorrAction.dwMotionOption	= dwMotionOption;
				pMover->m_CorrAction.fAngleX	= fAngleX;
				pMover->m_CorrAction.fAccPower	= fAccPower;
				pMover->m_CorrAction.fTurnAngle	= fTurnAngle;
				pMover->m_CorrAction.fBehavior	= TRUE;
				pMover->m_CorrAction.fValid	= TRUE;
				pMover->ClearDestAngle();

				int nRemnant	= (int)( MAX_CORR_SIZE_150 - delay );
				pMover->m_uRemnantCorrFrm	= nRemnant;
				pMover->SetDestPos( v );
			}
		}
		else 
		{
			if( pMover->IsActiveMover() )
				g_Neuz.m_camera.Lock();
			pMover->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
		}
	}
}

void CDPClient::OnMoverAngle( OBJID objid, CAr & ar )
{
	D3DXVECTOR3 v, vd;
	float f, fAngleX, fAccPower, fTurnAngle;
	__int64 nTickCount;

	ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
	ar >> nTickCount;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( pMover->m_pActMover->IsFly() == FALSE )		return;
		int delay	= (int)( (float)g_TickCount.GetOffset( nTickCount ) / 16.6667f );
		TRACE( "d = %d\n", delay );

//		if( delay >= MAX_CORR_SIZE_150 )
//		{
//			pMover->HalfForceSet( v, vd, f, fAngleX, fAccPower, fTurnAngle );
//		}
//		else
		{
			pMover->m_pActMover->DefaultSet();

			pMover->m_CorrAction.v	= v;
			pMover->m_CorrAction.vd		= vd;
			pMover->m_CorrAction.f	= f;
			pMover->m_CorrAction.fAngleX	= fAngleX;
			pMover->m_CorrAction.fAccPower	= fAccPower;
			pMover->m_CorrAction.fTurnAngle	= fTurnAngle;
			pMover->m_CorrAction.fBehavior	= FALSE;
			pMover->m_CorrAction.fHalf	= !pMover->m_CorrAction.fValid;
			pMover->m_CorrAction.fValid	= TRUE;

			pMover->ClearDestAngle();
			pMover->m_uRemnantCorrFrm	= MAX_CORR_SIZE_150;// - delay;
			pMover->SetDestPos( v );
		}
	}
}


void CDPClient::OnMoverCorr( OBJID objid, CAr & ar )
{
//	TRACE( "OnMoverCorr()\n" );
	D3DXVECTOR3 v, vd;
	float f;
	DWORD dwState, dwStateFlag, dwMotion;
	int nMotionEx, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) ) {
		if( pMover->m_pActMover->IsFly() )		return;
		//		pMover->ClearDest();
		pMover->ClearDestPos();
		pMover->ClearDestAngle();
		memset( &pMover->m_CorrAction, 0, sizeof(CORR_ACTION) );
		OBJID objidDest	= pMover->GetDestId();
		pMover->ActionForceSet( v, vd, f, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
		pMover->SetDestObj( objidDest, 0.0f, FALSE );
	}
}

void CDPClient::OnMoverCorr2( OBJID objid, CAr & ar )
{
//	TRACE( "OnMoverCorr2()\n" );
	D3DXVECTOR3 v, vd;
	float f, fAngleX, fAccPower, fTurnAngle;
	DWORD dwState, dwStateFlag, dwMotion;
	int nMotionEx, nLoop;
	DWORD dwMotionOption;
	__int64 nTickCount;

	ar >> v >> vd >> f >> fAngleX >> fAccPower >> fTurnAngle;
	ar >> dwState >> dwStateFlag;
	ar >> dwMotion >> nMotionEx;
	ar >> nLoop >> dwMotionOption;
	ar >> nTickCount;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) ) {
		if( pMover->m_pActMover->IsFly() == FALSE )		return;
		//		pMover->ClearDest();
		pMover->ClearDestPos();
		pMover->ClearDestAngle();
		memset( &pMover->m_CorrAction, 0, sizeof(CORR_ACTION) );
		pMover->ActionForceSet2( v, vd, f, fAngleX, fAccPower, fTurnAngle, dwState, dwStateFlag, dwMotion, nMotionEx, nLoop, dwMotionOption );
	}
}

void CDPClient::OnQueryGetDestObj(CAr & ar) {
	if (g_pPlayer && !g_pPlayer->IsEmptyDestObj()) {
		SendPacket<PACKETTYPE_GETDESTOBJ, OBJID>(g_pPlayer->GetDestId());
	}
}

void CDPClient::OnGetDestObj( OBJID objid, CAr & ar )
{
	OBJID objidDest;
	FLOAT fRange = 0;
	ar >> objidDest;
	ar >> fRange;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
		pMover->SetDestObj( objidDest, fRange, FALSE );
}

void CDPClient::OnQueryGetPos( CAr & ar )
{
	OBJID objid;
	ar >> objid;

	if( NULL != g_pPlayer )
		SendGetPos( g_pPlayer->GetPos(), g_pPlayer->GetAngle(), objid );
}

void CDPClient::OnGetPos( OBJID objid, CAr & ar )
{
	D3DXVECTOR3 vPos;
	float fAngle;
	
	ar >> vPos >> fAngle;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->SetPos( vPos );
		pMover->SetAngle( fAngle );

		pMover->m_fWaitQueryGetPos	= FALSE;
		
		if( FALSE == pMover->IsEmptyDestPos() )
			pMover->SetDestPos( pMover->GetDestPos(), pMover->m_bForward, FALSE );
	}
}

// 현상범의 리스트를 요청하면 그 응답으로 
void CDPClient::OnWantedName( CAr & ar )		
{
	char szName[64];
	ar.ReadString( szName, 64 );

	if( szName[0] != '\0' )
	{
		SAFE_DELETE( g_WndMng.m_pReWanted );
		g_WndMng.m_pReWanted = new CWndReWanted;
		g_WndMng.m_pReWanted->SetWantedName( szName );
		g_WndMng.m_pReWanted->Initialize();
	}
	else
	{
//		g_WndMng.OpenMessageBox( "PK를 당한적이 없거나, PK를 한 플레이어가 삭제되었습니다." );
		g_WndMng.OpenMessageBox( prj.GetText( TID_PK_REWARD_NO2 ) );
	}
}

// 현상범의 리스트를 요청하면 그 응답으로 
void CDPClient::OnWantedList(CAr & ar) {
	const auto [lTime, nCount] = ar.Extract<int, long>();

	SAFE_DELETE(g_WndMng.m_pWanted);
	g_WndMng.m_pWanted = new CWndWanted(static_cast<time_t>(lTime));

	for (int i = 0; i < nCount; i++) {
		WANTED_ENTRY entry;
		ar.ReadString(entry.szPlayer);
		ar >> entry.nGold;
		ar >> entry.nEnd;
		ar.ReadString(entry.szMsg);

		g_WndMng.m_pWanted->InsertWanted(entry);
	}

	g_WndMng.m_pWanted->Initialize();
}

// Sender
void CDPClient::SendJoin( BYTE nSlot, CMover* pMover, CRTMessenger* pRTMessenger, u_long uIdofMulti )
{
#ifdef __FLYFF_INITPAGE_EXT
	CWndBase::m_Theme.DestoryTitleWorld();
#endif //__FLYFF_INITPAGE_EXT
	
#ifndef __MAP_SECURITY
	g_WorldMng.Open( dwWorldID );
	g_WorldMng()->RestoreDeviceObjects( );
	g_WorldMng()->SetCamera( &g_Neuz.m_camera );
	g_Neuz.m_camera.Transform( g_WorldMng.Get() );
	g_Neuz.m_camera.Reset();
	g_WorldMng()->ReadWorld( pMover->GetPos() );
#endif // __MAP_SECURITY

	g_Neuz.m_idPlayer	=  pMover->m_idPlayer;
	
	BEFORESENDSOLE( ar, PACKETTYPE_JOIN_NeuzCache, DPID_UNKNOWN );
	ar << pMover->m_idPlayer;
	ar << g_Neuz.m_dwAuthKey;
	ar << pMover->m_idparty;
	ar << pMover->m_idGuild;
	ar << pMover->m_idWar;
	ar << uIdofMulti;
	ar << nSlot;
	ar.WriteString( pMover->GetName() );
#ifdef __GPAUTH_01
	ar.WriteString( g_Neuz.m_bGPotatoAuth?g_Neuz.m_szGPotatoNo: g_Neuz.m_szAccount );
#else	// __GPAUTH_01
	ar.WriteString( g_Neuz.m_szAccount );
#endif	// __GPAUTH_01
	ar.WriteString( g_Neuz.m_szPassword );
	pRTMessenger->Serialize( ar );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendChat( LPCSTR lpszChat )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHAT, DPID_UNKNOWN );
	ar.WriteString( lpszChat );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 대전창 띄움
void CDPClient::SendGuildCombatWindow( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_IN_GUILDCOMBAT, DPID_UNKNOWN );
	ar << GC_IN_WINDOW;
	SEND( ar, this, DPID_SERVERPLAYER );
}
// 대전 신청
void CDPClient::SendGuildCombatApp( DWORD dwPenya )
{
	BEFORESENDSOLE( ar, PACKETTYPE_IN_GUILDCOMBAT, DPID_UNKNOWN );
	ar << GC_IN_APP;
	ar << dwPenya;
	SEND( ar, this, DPID_SERVERPLAYER );
}
// 대전 취소
void CDPClient::SendGuildCombatCancel() {
	SendPacket<PACKETTYPE_OUT_GUILDCOMBAT>();
}
// 대전 신청 현황
void CDPClient::SendGCRequestStatusWindow() {
	SendPacket<PACKETTYPE_REQUEST_STATUS>();
}
// 대전 선택 캐릭창 띄움
void CDPClient::SendGCSelectPlayerWindow( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SELECTPLAYER_GUILDCOMBAT, DPID_UNKNOWN );
	ar << (BOOL)TRUE;		// bWindow
	SEND( ar, this, DPID_SERVERPLAYER );
}
// 대전 선택 캐릭 신청
void CDPClient::SendGCSelectPlayer(std::vector<u_long> vecSelectPlayer, u_long uidDefender )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SELECTPLAYER_GUILDCOMBAT, DPID_UNKNOWN );
	ar << (BOOL)FALSE;		// bWindow
	ar << uidDefender;
	int nSize = vecSelectPlayer.size();
	ar << nSize;
	for( int i = 0 ; i < nSize ; ++i )
	{
		ar << vecSelectPlayer[i];
	}
	SEND( ar, this, DPID_SERVERPLAYER );
}
// 대전 맵 선택
void CDPClient::SendGCSelectMap(int nMap) {
	SendPacket<PACKETTYPE_SELECTMAP_GUILDCOMBAT, int>(nMap);
}
// 길드대전 입장
void CDPClient::SendGCJoin() {
	SendPacket<PACKETTYPE_JOIN_GUILDCOMBAT>();
}
// 길드 신청금액 및 보상받기
void CDPClient::SendGCGetPenyaGuild() {
	SendPacket<PACKETTYPE_GETPENYAGUILD_GUILDCOMBAT>();
}
// 베스트 플레이어 보상 받기
void CDPClient::SendGCGetPenyaPlayer() {
	SendPacket<PACKETTYPE_GETPENYAPLAYER_GUILDCOMBAT>();
}
void CDPClient::SendGCTele() {
	SendPacket<PACKETTYPE_TELE_GUILDCOMBAT>();
}
void CDPClient::SendGCPlayerPoint() {
	SendPacket<PACKETTYPE_PLAYERPOINT_GUILDCOMBAT>();
}

void CDPClient::SendActMsg(CMover *, DWORD dwMsg, int nParam1, int nParam2) {
	SendSnapshot(TRUE);

	SendPacket<PACKETTYPE_ACTMSG, DWORD, int, int>(dwMsg, nParam1, nParam2);
}

void CDPClient::SendError(int nCode, int nData) {
	SendPacket<PACKETTYPE_ERRORCODE, int, int>(nCode, nData);
}

void CDPClient::SendShipActMsg( CShip *pShip, DWORD dwMsg, int nParam1, int nParam2 )
{
	OBJID	idShip = pShip->GetId();

	SendPacket<PACKETTYPE_SHIP_ACTMSG, DWORD, int, int, OBJID>(
		dwMsg, nParam1, nParam2, idShip
	);
}

void CDPClient::SendLocalPosFromIA(const D3DXVECTOR3 & vLocal, OBJID idIA) {
	SendPacket<PACKETTYPE_LOCALPOSFROMIA, D3DXVECTOR3, OBJID>(vLocal, idIA);
}

void CDPClient::SendRemoveItem(CItemElem * pItemElem, int nNum) {
	SendPacket<PACKETTYPE_REMOVEINVENITEM, DWORD, int>(pItemElem->m_dwObjId, nNum);
}

void CDPClient::SendDoEquip( CItemElem* pItemElem, int nPart, BOOL bResult )
{
	// nPart 가 -1 이면 입히는 것임
	if( bResult && 
		( nPart == PARTS_LWEAPON || nPart == PARTS_RWEAPON || nPart == PARTS_UPPER_BODY ) )
	{
		CItemElem* pItemElemParts = g_pPlayer->m_Inventory.GetEquip( nPart );
		if( pItemElemParts && pItemElemParts->m_nResistSMItemId != 0 )
		{
			// 물어봐야 함
			SAFE_DELETE( g_WndMng.m_pRemoveElem );
			g_WndMng.m_pRemoveElem = new CWndRemoveElem;
			g_WndMng.m_pRemoveElem->Initialize();
			g_WndMng.m_pRemoveElem->OnSetItem( 2, 0, 0, nPart, pItemElem );
			return;
		}
		
	}

	if( g_pPlayer->IsUseItemReadyTime( pItemElem->GetProp(), pItemElem->m_dwObjId ) == FALSE )
		return;

	if( pItemElem->GetProp()->dwParts == PARTS_RIDE && g_pPlayer->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
	{
		g_WndMng.PutString( prj.GetText( TID_QUEST_DISQUISE_NOTFLY ), NULL, prj.GetTextColor( TID_QUEST_DISQUISE_NOTFLY ) );
		return;
	}

	// 헤어샵 / 메이크업 사용 중에는 머리에 쓰는 아이템을 사용할 수 없다.
	if(pItemElem->GetProp()->dwParts == PARTS_HAT || pItemElem->GetProp()->dwParts == PARTS_CAP)
	{
		if( g_WndMng.GetWndBase( APP_BEAUTY_SHOP_EX ) || g_WndMng.GetWndBase( APP_BEAUTY_SHOP_SKIN ))
		{
			g_WndMng.PutString( prj.GetText(TID_GAME_DONOTUSE_SHOP), NULL, prj.GetTextColor(TID_GAME_DONOTUSE_SHOP) );
			return;
		}
	}

	if( pItemElem->GetProp()->dwParts == PARTS_RWEAPON )
	{
		CItemElem* pItemElemWepon = g_pPlayer->m_Inventory.GetEquip( PARTS_RWEAPON	);
		if( pItemElemWepon && pItemElemWepon->GetProp()->dwItemKind3 == IK3_BOW ) // 장착되어있는것이 활이면
		{
			if( pItemElem->GetProp()->dwItemKind3 != IK3_BOW )	// 장착되어있는것이 보우면 안벗겨두 됨
			{
				CItemElem* pItemElemBullet = g_pPlayer->m_Inventory.GetEquip( PARTS_BULLET );	
				if( pItemElemBullet && pItemElemBullet->GetProp()->dwItemKind3 == IK3_ARROW )
				{
					BEFORESENDSOLE( ar, PACKETTYPE_DOEQUIP, DPID_UNKNOWN );
					ar << pItemElemBullet->m_dwObjId;
					ar << PARTS_BULLET;
					SEND( ar, this, DPID_SERVERPLAYER );
				}			
			}
		}
	}
	
	BEFORESENDSOLE( ar, PACKETTYPE_DOEQUIP, DPID_UNKNOWN );
	ar << pItemElem->m_dwObjId;
	ar << nPart;
#ifdef __HACK_1023
	ar << pItemElem->GetProp()->fFlightSpeed;
#endif	// __HACK_1023
	SEND( ar, this, DPID_SERVERPLAYER );
}
/*
void CDPClient::SendDoEquip( BYTE nId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_DOEQUIP, DPID_UNKNOWN );
	ar << nId;
	SEND( ar, this, DPID_SERVERPLAYER );
}
*/

void CDPClient::SendDropItem(DWORD dwItemId, short nITemNum, const D3DXVECTOR3 & vPos) {
	if (g_WndMng.m_pWndDialog) {
		g_WndMng.PutString(TID_GAME_DIALOGNODROPITEM);
		return;
	}

	SendPacket<PACKETTYPE_DROPITEM, DWORD, short, D3DXVECTOR3>(
		dwItemId, nITemNum, vPos
	);

	CWndWorld * pWndWorld = (CWndWorld *)g_WndMng.GetWndBase(APP_WORLD);
	if (pWndWorld)
		pWndWorld->m_dwDropTime = GetTickCount();
}

void CDPClient::SendTrade( CMover* pTrader )
{
	if( IsValidObj( pTrader ) && pTrader->GetType() == OT_MOVER && ( (CMover*)pTrader )->IsPlayer() )
	{
		SendPacket<PACKETTYPE_TRADE, OBJID>(pTrader->GetId());
	}
}

void CDPClient::SendConfirmTrade( CMover* pTrader )
{
	if( IsValidObj( pTrader ) && pTrader->GetType() == OT_MOVER && ( (CMover*)pTrader )->IsPlayer() )
	{
		SendPacket<PACKETTYPE_CONFIRMTRADE, OBJID>(pTrader->GetId());
	}
}

void CDPClient::SendTradePut(BYTE i, BYTE nItemType, BYTE nId, short ItemNum) {
	SendPacket<PACKETTYPE_TRADEPUT, BYTE, BYTE, BYTE, short>(i, nItemType, nId, ItemNum);
}

void CDPClient::SendTradePull(int i) {
	SendPacket<PACKETTYPE_TRADEPULL, BYTE>(static_cast<BYTE>(i));
}

void CDPClient::SendTradePutGold(DWORD dwGold) {
	SendPacket<PACKETTYPE_TRADEPUTGOLD, DWORD>(dwGold);
}

void CDPClient::SendTradeCancel(int nMode) {
	SendPacket<PACKETTYPE_TRADECANCEL, int>(nMode);
}

void CDPClient::SendMessageNote(u_long uidTo, LPCTSTR strMessage) {
	BEFORESENDSOLE(ar, PACKETTYPE_SENDTAG, DPID_UNKNOWN);
	ar << uidTo;
	ar.WriteString(strMessage);
	SEND(ar, this, DPID_SERVERPLAYER);
}

void CDPClient::SendPartySkillUse(int nSkill) {
	SendPacket<PACKETTYPE_PARTYSKILLUSE, int>(nSkill);
}

void CDPClient::SendPartyMemberCancle(u_long uLeader, int nMode) {
	SendPacket<PACKETTYPE_MEMBERREQUESTCANCLE, u_long, int>(uLeader, nMode);
}

void CDPClient::SendAddPartyMember(const u_long uLeader) {
	SendPacket<PACKETTYPE_ADDPARTYMEMBER_NeuzCore, u_long>(uLeader);
}

void CDPClient::SendChangeTroup( BOOL bSendName, const char * szPartyName )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHANGETROUP, DPID_UNKNOWN );
	ar << g_pPlayer->m_idPlayer;
	ar << bSendName;
	if( bSendName )
	{
		ar.WriteString( szPartyName );
	}
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendChangePartyName( const char * szPartyName )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHANPARTYNAME, DPID_UNKNOWN );
	ar << g_pPlayer->m_idPlayer;
	ar.WriteString( szPartyName );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendPartyChangeLeader(u_long uChangerLeaderid) {
	SendPacket<PACKETTYPE_PARTYCHANGELEADER, u_long>(uChangerLeaderid);
}

// pSrc : 듀얼 신청자
// pDst : 듀얼 타겟
void CDPClient::SendDuelRequest(CMover * pSrc, CMover * pDst) {
	SendPacket<PACKETTYPE_DUELREQUEST, u_long, u_long>(pSrc->m_idPlayer, pDst->m_idPlayer);
}

// 듀얼시작을 알림
void CDPClient::SendDuelYes(CMover * pSrc, CMover * pDst) {
	SendPacket<PACKETTYPE_DUELYES, u_long, u_long>(pSrc->m_idPlayer, pDst->m_idPlayer);
}

// 신청자에게 듀얼신청을 거부한다는걸 알림.
void CDPClient::SendDuelNo(CMover * pSrc) {
	SendPacket<PACKETTYPE_DUELNO, u_long>(pSrc->m_idPlayer);
}

/// 파티 듀얼.
// pSrc : 듀얼 신청자
// pDst : 듀얼 타겟
void CDPClient::SendDuelPartyRequest(CMover * pSrc, CMover * pDst) {
	SendPacket<PACKETTYPE_DUELPARTYREQUEST, u_long, u_long>(pSrc->m_idPlayer, pDst->m_idPlayer);
}

// 듀얼시작을 알림
void CDPClient::SendDuelPartyYes(CMover * pSrc, CMover * pDst) {
	SendPacket<PACKETTYPE_DUELPARTYYES, u_long, u_long>(pSrc->m_idPlayer, pDst->m_idPlayer);
}

// 신청자에게 듀얼신청을 거부한다는걸 알림.
void CDPClient::SendDuelPartyNo(CMover * pSrc) {
	SendPacket<PACKETTYPE_DUELPARTYNO, u_long>(pSrc->m_idPlayer);
}

void CDPClient::SendMoverFocus(u_long uidPlayer) {
	SendPacket<PACKETTYPE_MOVERFOCOUS, u_long>(uidPlayer);
}

void CDPClient::SendScriptDialogReq( OBJID objid, LPCTSTR lpKey, int nGlobal1, QuestId nGlobal2, int nGlobal3, int nGlobal4 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPTDLG, DPID_UNKNOWN );
	ar << objid;
	ar.WriteString( ( lpKey ? lpKey : _T( "" ) ) );
	ar << nGlobal1 << nGlobal2 << nGlobal3 << nGlobal4;
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendOpenShopWnd(OBJID objid) {
	SendPacket<PACKETTYPE_OPENSHOPWND, OBJID>(objid);
}

void CDPClient::SendCloseShopWnd() {
	ASSERT(g_pPlayer);
	SendPacket<PACKETTYPE_CLOSESHOPWND>();
}

void CDPClient::SendOpenBankWnd(const DWORD dwId) {
	SendPacket<PACKETTYPE_OPENBANKWND, OBJID>(dwId);
}

void CDPClient::SendOpenGuildBankWnd() {
	SendPacket<PACKETTYPE_GUILD_BANK_WND>();
}

void CDPClient::SendCloseGuildBankWnd() {
	SendPacket<PACKETTYPE_GUILD_BANK_WND_CLOSE>();
}

void CDPClient::SendCloseBankWnd() {
	SendPacket<PACKETTYPE_CLOSEBANKWND>();
}

void CDPClient::SendDoUseSkillPoint(const MoverSkills & skills) {
	SendPacket<PACKETTYPE_DOUSESKILLPOINT, MoverSkills>(skills);
}

void CDPClient::SendStateModeCancel( DWORD dwStateMode, BYTE nFlag )
{
	BEFORESENDSOLE( ar, PACKETTYPE_STATEMODE, DPID_UNKNOWN );
	ar << dwStateMode;
	ar << nFlag;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendPutItemBank( BYTE nSlot, BYTE nId, short ItemNum )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PUTITEMBACK, DPID_UNKNOWN );
	ar << nSlot << nId << ItemNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendPutItemBankToBank( BYTE nPutSlot, BYTE nSlot, BYTE nId, short ItemNum )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PUTBACKTOBANK, DPID_UNKNOWN );
	BYTE nFlag = 1;
	ar << nFlag<< nPutSlot << nSlot << nId << ItemNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendPutGoldBankToBank( BYTE nPutSlot, BYTE nSlot, DWORD dwGold )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PUTBACKTOBANK, DPID_UNKNOWN );
	BYTE nFlag = 0;
	ar << nFlag << nPutSlot << nSlot << dwGold;
	SEND( ar, this, DPID_SERVERPLAYER );	
}
void CDPClient::SendPutItemGuildBank( BYTE nId, DWORD ItemNum, BYTE p_Mode )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PUTITEMGUILDBANK, DPID_UNKNOWN );
	ar << nId << ItemNum << p_Mode;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGetItemGuildBank( BYTE nId, DWORD ItemNum, BYTE p_Mode )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETITEMGUILDBANK, DPID_UNKNOWN );
	ar << nId << ItemNum << p_Mode;
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::SendPutGoldBank( BYTE nSlot, DWORD dwGold )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PUTGOLDBACK, DPID_UNKNOWN );
	ar << nSlot << dwGold;
	SEND( ar, this, DPID_SERVERPLAYER );	
}

void CDPClient::SendGetItemBank( BYTE nSlot, BYTE nId, short ItemNum )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETITEMBACK, DPID_UNKNOWN );
	ar << nSlot << nId << ItemNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGetGoldBank( BYTE nSlot, DWORD dwGold )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETGOLDBACK, DPID_UNKNOWN );
	ar << nSlot << dwGold;
	SEND( ar, this, DPID_SERVERPLAYER );	
}

void CDPClient::SendBuyItem( CHAR cTab, BYTE nId, short nNum, DWORD dwItemId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_BUYITEM, DPID_UNKNOWN );
	ar << cTab << nId << nNum << dwItemId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendBuyChipItem( CHAR cTab, BYTE nId, short nNum, DWORD dwItemId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_BUYCHIPITEM, DPID_UNKNOWN );
	ar << cTab << nId << nNum << dwItemId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSellItem( BYTE nId, short nNum )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SELLITEM, DPID_UNKNOWN );
	ar << nId << nNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}

#ifdef __HACK_1023
void CDPClient::SendMeleeAttack( OBJMSG dwAtkMsg, OBJID objid, int nParam2, int nParam3, FLOAT fVal )
#else	// __HACK_1023
void CDPClient::SendMeleeAttack( OBJMSG dwAtkMsg, OBJID objid, int nParam2, int nParam3 )
#endif	// __HACK_1023
{
	BEFORESENDSOLE( ar, PACKETTYPE_MELEE_ATTACK, DPID_UNKNOWN );
#ifdef __HACK_1023
	ar << dwAtkMsg << objid << nParam2 << nParam3 << fVal;
#else	// __HACK_1023
	ar << dwAtkMsg << objid << nParam2 << nParam3/* f */;
#endif	// __HACK_1023
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendMeleeAttack2( OBJMSG dwAtkMsg, OBJID objid, int nParam2, int nParam3 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_MELEE_ATTACK2, DPID_UNKNOWN );
	ar << dwAtkMsg << objid << nParam2 << nParam3/* f */;
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::SendMagicAttack( OBJMSG dwAtkMsg, OBJID objid, int nParam2, int nParam3, int nMagicPower, int idSfxHit )
{
	BEFORESENDSOLE( ar, PACKETTYPE_MAGIC_ATTACK, DPID_UNKNOWN );
	ar << dwAtkMsg << objid << nParam2 << nParam3 << nMagicPower << idSfxHit;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRangeAttack( OBJMSG dwAtkMsg, OBJID objid, DWORD dwItemID, int idSfxHit )
{
	BEFORESENDSOLE( ar, PACKETTYPE_RANGE_ATTACK, DPID_UNKNOWN );
	ar << dwAtkMsg << objid << dwItemID << idSfxHit;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendUseSkill(DWORD skillId, OBJID objid, int nUseSkill, BOOL bControl )
{
	BEFORESENDSOLE( ar, PACKETTYPE_USESKILL, DPID_UNKNOWN );
	ar << skillId << objid << nUseSkill;
	ar << bControl;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendTeleSkill( OBJID objid, D3DXVECTOR3 vPos )
{
	BEFORESENDSOLE( ar, PACKETTYPE_TELESKILL, DPID_UNKNOWN );
	ar << vPos;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSfxID( OBJID idTarget, int idSfxHit, DWORD dwType, DWORD dwSkill, int nMaxDmgCnt )
{
	if( nMaxDmgCnt <= 0 )
		nMaxDmgCnt = 1;
	BEFORESENDSOLE( ar, PACKETTYPE_SFX_ID, DPID_UNKNOWN );
	ar << idTarget << idSfxHit << dwType << dwSkill << nMaxDmgCnt;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// idTaget에게 g_pPlayer가 타겟을 잡았다는것을 알림.
void CDPClient::SendSetTarget( OBJID idTarget, BYTE bClear )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SETTARGET, DPID_UNKNOWN );
	ar << idTarget << bClear;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendExp( EXPINTEGER nExp )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SEND_TO_SERVER_EXP, DPID_UNKNOWN );
	ar << nExp;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendCorrReq( CObj *pObj )
{
	CMover *pMover = NULL;
	if( pObj )
	{
		if( pObj->GetType() == OT_MOVER )
			pMover = (CMover *)pObj;	// 선택한 오브젝트를 요청한다.
		else
			return;
	} else
	{
		pMover = g_pPlayer;				// 선택되어 있지 않다면 플레이어를 요청한다.
	}

	BEFORESENDSOLE( ar, PACKETTYPE_CORR_REQ, DPID_UNKNOWN );
	ar << pMover->GetId();
	SEND( ar, this, DPID_SERVERPLAYER );
	
//	SendHdr( PACKETTYPE_CORR_REQ );
}

void CDPClient::SendCorrReq( OBJID idObj )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CORR_REQ, DPID_UNKNOWN );
	ar << idObj;
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::OnMotion( OBJID objid, CAr & ar )
{
	DWORD dwMsg;
	ar >> dwMsg;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->m_dwReqFlag		&= ~REQ_MOTION;
		if( (OBJMSG)dwMsg != OBJMSG_NONE )
		{
			if( dwMsg != OBJMSG_STANDUP )
			{
				pMover->ClearDest();
				pMover->ClearDestAngle();
				memset( &pMover->m_CorrAction, 0, sizeof(CORR_ACTION) );
			}
			if( pMover->IsActiveMover() )
				if( g_WndMng.m_pWndWorld )
					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
			pMover->SendActMsg( (OBJMSG)dwMsg, 0, 0, 1 );
		}
	}
}

void CDPClient::OnSExpBoxInfo( OBJID objid, CAr & ar )
{
	OBJID ctrlid;
	DWORD dwElemSet;
	DWORD dwTime;
	u_long uidPlyaer;
	ar >> ctrlid >> dwElemSet >> dwTime >> uidPlyaer;
	
	CCtrl* pCtrl	= prj.GetCtrl( ctrlid );
	if( IsValidObj( pCtrl ) )
	{
		((CCommonCtrl*)pCtrl)->m_CtrlElem.m_dwSet     = dwElemSet;
		((CCommonCtrl*)pCtrl)->m_dwDelete  = timeGetTime()+dwTime;
		((CCommonCtrl*)pCtrl)->m_idExpPlayer  = uidPlyaer;
		CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
		if( pWndWorld )
			pWndWorld->m_bCtrlInfo = TRUE;
	}
}

void CDPClient::OnSExpBoxCoolTime( OBJID objid, CAr & ar )
{
	OBJID ctrlid;
	DWORD dwTime;
	ar >> ctrlid >> dwTime;
	
	CCtrl* pCtrl	= prj.GetCtrl( ctrlid );
	CMover *pMover = prj.GetMover( objid );
	if( IsValidObj( pCtrl ) && IsValidObj( pMover ) )
	{
		pMover->m_dwCtrlReadyTime = timeGetTime() + dwTime;
		pMover->m_dwCtrlReadyId	  = ctrlid;
		
		((CCommonCtrl*)pCtrl)->m_dwCtrlReadyTime = timeGetTime() + dwTime;
	}
}

void CDPClient::OnCorrReq( OBJID objid, CAr & ar )
{
//	OBJID idObj;
	D3DXVECTOR3 vPos;
	OBJID idAttacker;
	ar >> vPos >> idAttacker;
	
	CMover *pMover = prj.GetMover( objid );
	if( IsValidObj( pMover ) ) 
	{
		pMover->SetPos( vPos );
		pMover->m_idAttacker = idAttacker;
	}
}

void CDPClient::SendScriptRemoveAllItem( DWORD dwItemId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_REMOVE_ALL_ITEM, DPID_UNKNOWN );
	ar << dwItemId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendScriptEquipItem( DWORD dwItemId, int nOption )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_EQUIP_ITEM, DPID_UNKNOWN );
	ar << dwItemId << nOption;
	SEND( ar, this, DPID_SERVERPLAYER );
}

//{{AFX
void CDPClient::SendScriptCreateItem( BYTE nItemType, DWORD dwItemId, short nNum, int nOption )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_CREATE_ITEM, DPID_UNKNOWN );
	ar << nItemType << dwItemId << nNum << nOption;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendScriptAddGold( int nGold )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_ADD_GOLD, DPID_UNKNOWN );
	ar << nGold;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendScriptRemoveQuest( int nQuestId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_REMOVE_QUEST, DPID_UNKNOWN );
	ar << nQuestId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSetQuest( int nQuestIdx, int nState )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SETQUEST, DPID_UNKNOWN );
	ar << nQuestIdx << nState;
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendScriptAddExp( int nExp )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_ADD_EXP, DPID_UNKNOWN );
	ar << nExp;
	SEND( ar, this, DPID_SERVERPLAYER );
}

//}}AFX

void CDPClient::SendPlayerMoved( void )
{
	CMover* pPlayer	= g_pPlayer;
	if( pPlayer ) {
		D3DXVECTOR3 v, vd;
		float f;
		pPlayer->m_pActMover->PresupposePos( &v, &vd, &f, MAX_CORR_SIZE_45 );

		DWORD dwMotion	= pPlayer->m_dwMotion;
//		if( pPlayer->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) ) {
//			ItemProp* pItemProp	= pPlayer->GetActiveHandItemProp();
//			switch( pItemProp->dwItemKind3 )
//			{
//				case IK3_SWD:	dwMotion	-= 100;	break;
//				case IK3_WAND:	dwMotion	-= 100;	break;
//				default:	dwMotion	-= 100;	break;
//			}
//		}

		BEFORESENDSOLE( ar, PACKETTYPE_PLAYERMOVED, DPID_UNKNOWN );

		// 60
		ar << v << vd << f;		// 28
		ar << pPlayer->m_pActMover->GetState() << pPlayer->m_pActMover->GetStateFlag();		// 8
		ar << dwMotion << pPlayer->m_pActMover->m_nMotionEx;	// 8
		ar << pPlayer->m_pModel->m_nLoop << pPlayer->m_dwMotionOption; //(BOOL)0	// 8
		ar << g_TickCount.GetTickCount();	// 8

		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendPlayerBehavior( void )
{
	CMover* pPlayer	= g_pPlayer;
	if( pPlayer ) {
		D3DXVECTOR3 vd, v	= pPlayer->GetPos();
		vd	= pPlayer->m_pActMover->m_vDelta;
		float f	= pPlayer->GetAngle();

		DWORD dwMotion	= pPlayer->m_dwMotion;
//		if( pPlayer->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) ) {
//			ItemProp* pItemProp	= pPlayer->GetActiveHandItemProp();
//			switch( pItemProp->dwItemKind3 )
//			{
//				case IK3_SWD:	dwMotion	-= 100;	break;
//				case IK3_WAND:	dwMotion	-= 100;	break;
//				default:	dwMotion	-= 100;	break;
//			}
//		}

		BEFORESENDSOLE( ar, PACKETTYPE_PLAYERBEHAVIOR, DPID_UNKNOWN );

		ar << v << vd << f;
		ar << pPlayer->m_pActMover->GetState() << pPlayer->m_pActMover->GetStateFlag();
		ar << dwMotion << pPlayer->m_pActMover->m_nMotionEx;
		ar << pPlayer->m_pModel->m_nLoop << pPlayer->m_dwMotionOption;	//(BOOL)0
		ar << g_TickCount.GetTickCount();

		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendPlayerMoved2( BYTE nFrame )
{
	PostPlayerAngle( false );	

	CMover* pPlayer	= g_pPlayer;
	if( pPlayer ) 
	{
		D3DXVECTOR3 v, vd;
		float f, fAngleX, fAccPower;
		pPlayer->m_pActMover->PresupposePos2( &v, &vd, &f, &fAngleX, &fAccPower, nFrame );

		DWORD dwMotion	= pPlayer->m_dwMotion;

		BEFORESENDSOLE( ar, PACKETTYPE_PLAYERMOVED2, DPID_UNKNOWN );

		// 73
		ar << v << vd << f << fAngleX << fAccPower << pPlayer->m_pActMover->m_fTurnAngle;	// 40
		ar << pPlayer->m_pActMover->GetState() << pPlayer->m_pActMover->GetStateFlag();	// 8
		ar << dwMotion << pPlayer->m_pActMover->m_nMotionEx;	// 8
		ar << pPlayer->m_pModel->m_nLoop << pPlayer->m_dwMotionOption;		//(BOOL)0	// 8
		ar << g_TickCount.GetTickCount();	// 8
		ar << nFrame;	// 1

		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendPlayerBehavior2( void )
{
	PostPlayerAngle( false );	//

	CMover* pPlayer	= g_pPlayer;
	if( pPlayer ) 
	{
		D3DXVECTOR3 vd, v	= pPlayer->GetPos();
		vd	= pPlayer->m_pActMover->m_vDelta;
		float f		= pPlayer->GetAngle();
		float fAngleX	= pPlayer->GetAngleX();
		float fAccPower	= pPlayer->m_pActMover->m_fAccPower;
		float fTurnAngle	= pPlayer->m_pActMover->m_fTurnAngle;

		DWORD dwMotion	= pPlayer->m_dwMotion;

		BEFORESENDSOLE( ar, PACKETTYPE_PLAYERBEHAVIOR2, DPID_UNKNOWN );

		ar << v << vd << f << fAngleX << fAccPower << fTurnAngle;
		ar << pPlayer->m_pActMover->GetState() << pPlayer->m_pActMover->GetStateFlag();
		ar << dwMotion << pPlayer->m_pActMover->m_nMotionEx;
		ar << pPlayer->m_pModel->m_nLoop << pPlayer->m_dwMotionOption;	
		ar << g_TickCount.GetTickCount();

		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendPlayerAngle(void) {
	if (!g_pPlayer) return;

	const float f = g_pPlayer->GetAngle();
	const float fAngleX = g_pPlayer->GetAngleX();
	SendPacket<PACKETTYPE_PLAYERANGLE, float, float>(f, fAngleX);
}


int		g_nItemDelay = 0;		// 땜빵.

void CDPClient::SendRemoveItemLevelDown( DWORD dwId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_REMOVE_ITEM_LEVEL_DOWN, DPID_UNKNOWN );
	ar << dwId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendDoUseItemTarget( DWORD dwMaterial, DWORD dwTarget )
{
	BEFORESENDSOLE( ar, PACKETTYPE_DO_USE_ITEM_TARGET, DPID_UNKNOWN );
	ar << dwMaterial << dwTarget;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendAwakening( int nItem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_AWAKENING, DPID_UNKNOWN );
	ar << nItem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendBlessednessCancel( int nItem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_BLESSEDNESS_CANCEL, DPID_UNKNOWN );
	ar << nItem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendDoUseItem( DWORD dwItemId, OBJID objid, int nPart, BOOL bResult )
{
	CMover* pPlayer	= g_pPlayer;
	if( !pPlayer ) 
		return;

	if( pPlayer->IsDie() )	
		return;

	WORD dwType	= LOWORD( dwItemId );
	WORD dwId	= HIWORD( dwItemId );
	CItemElem * pItemBase	= pPlayer->GetItemId( dwId );
	if (!pItemBase) return;

	const ItemProp * const pItemProp = pItemBase->GetProp();
	if (!pItemProp) return;

#ifdef __S_SERVER_UNIFY
	if( g_WndMng.m_bAllAction == FALSE )
		return;
#endif // __S_SERVER_UNIFY

	if(pItemProp->dwParts == PARTS_HAT  || pItemProp->dwParts == PARTS_CAP)
	{	
		if(g_WndMng.GetWndBase( APP_BEAUTY_SHOP_EX ) || g_WndMng.GetWndBase( APP_BEAUTY_SHOP_SKIN ))
		{
			//헤어샵 및 메이크업 이용 중 머리에 쓰는 아이템은 사용 불가.
			g_WndMng.PutString(TID_GAME_DONOTUSE_SHOP);
			return;
		}
	}

	//얼터멋 웨폰 변환창에서는 최상급 두루마리 사용가능 그 외 아이템 사용 불가
	//유니크 웨폰 변환창에서는 모든 아이템 사용 불가
	CWndChangeWeapon* pWndChangeWeapon = (CWndChangeWeapon*)g_WndMng.GetWndBase( APP_SMELT_CHANGEWEAPON );
	if(pWndChangeWeapon != NULL)
	{
		if(pWndChangeWeapon->m_nWeaponType != WEAPON_UNIQUE || pItemProp->dwID != II_SYS_SYS_SCR_SMELPROT3)
		{
			g_WndMng.PutString( prj.GetText(TID_SBEVE_NOTUSEITEM), NULL, prj.GetTextColor(TID_SBEVE_NOTUSEITEM) );
			return;
		}
	}
		
	BOOL bCreateMonster = FALSE;
	if( g_Neuz.m_pCreateMonItem )
	{
		if( pItemProp->dwItemKind3 != IK3_CREATE_MONSTER )
		{
			g_Neuz.m_pCreateMonItem = NULL;
			bCreateMonster = TRUE;
		}
	}
	
	if (pPlayer->m_cooltimeMgr.CanUse(*pItemProp) == CCooltimeMgr::CooldownType::OnCooldown) {
		g_WndMng.PutString(TID_GAME_ATTENTIONCOOLTIME);
		return;
	}

	if( g_pPlayer->IsUseItemReadyTime( pItemProp, pItemBase->m_dwObjId ) == FALSE )
		return;

	if( g_pPlayer->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
	{
		if(pItemProp->dwParts == PARTS_RIDE)
		{
			g_WndMng.PutString(TID_QUEST_DISQUISE_NOTFLY);
			return;
		}
		else if(pItemProp->dwID == II_CHR_MAG_TRI_ANGELWING || pItemProp->dwID == II_SYS_SYS_EVE_WINGS) //변신중에는 날개 아이템 사용 불가.
		{
			g_WndMng.PutString(TID_GAME_DISQUISE_DNT_WING);
			return;
		}
	}
	
	if( g_pPlayer && ( g_pPlayer->HasBuff( BUFF_ITEM, II_CHR_MAG_TRI_ANGELWING ) || g_pPlayer->HasBuff( BUFF_ITEM, II_SYS_SYS_EVE_WINGS ) ) )
	{
		if(pItemProp->dwItemKind3 == IK3_TEXT_DISGUISE)
		{
			g_WndMng.PutString(TID_GAME_DISQUISE_DNT_WING);
			return;
		}

		BOOL check = FALSE;
		if(g_pPlayer->HasBuff( BUFF_ITEM, II_CHR_MAG_TRI_ANGELWING ))
		{
			if(pItemProp->dwID == II_SYS_SYS_EVE_WINGS)
				check = TRUE;
		}
		else if(g_pPlayer->HasBuff( BUFF_ITEM, II_SYS_SYS_EVE_WINGS ))
		{
			if(pItemProp->dwID == II_CHR_MAG_TRI_ANGELWING)
				check = TRUE;
		}

		if(check) //두종의 날개 중복사용 금지.
		{
			g_WndMng.PutString(TID_GAME_NOTUSESKILL_ITEMUSE);
			return;
		}
	}

	if( bResult )
	{
		CItemElem *pItemElemOld = NULL;
		if( nPart == PARTS_LWEAPON || nPart == PARTS_RWEAPON || nPart == PARTS_UPPER_BODY )
		{
			pItemElemOld = g_pPlayer->m_Inventory.GetEquip( nPart );
		}
		
		if( pItemElemOld != NULL )
		{
			if( pItemElemOld->m_nResistSMItemId != 0 )
			{
				// 물어봐야 함
				SAFE_DELETE( g_WndMng.m_pRemoveElem );
				g_WndMng.m_pRemoveElem = new CWndRemoveElem;
				g_WndMng.m_pRemoveElem->Initialize();
				g_WndMng.m_pRemoveElem->OnSetItem( 1, dwItemId, objid, nPart, NULL );
				return;
			}
		}
	}
	
	if( pItemProp->dwItemKind2 == IK2_BLINKWING	&& g_pPlayer->GetWorld()->m_dwWorldID == WI_WORLD_KEBARAS )
	{
		char szMessege[128] = {0,};
		sprintf( szMessege, prj.GetText(TID_PK_BLINK_NO), g_pPlayer->GetWorld()->m_szWorldName );
		g_WndMng.PutString( szMessege, NULL, prj.GetTextColor(TID_PK_BLINK_NO) );
		return;
	}
	
	if( pItemProp->dwItemKind2 == IK2_BLINKWING	&& g_pPlayer->GetWorld()->m_dwWorldID == WI_WORLD_GUILDWAR )
	{
		g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_GAME_GUILDCOMBAT_NO_OUTWAR) );
		return;
	}

	if( pItemProp->dwItemKind2 == IK2_BLINKWING	&& g_pPlayer->GetWorld()->m_dwWorldID == WI_WORLD_MINIROOM )
	{
		char szMessege[128] = {0,};
		sprintf( szMessege, prj.GetText(TID_PK_BLINK_NO), g_pPlayer->GetWorld()->m_szWorldName );
		g_WndMng.PutString( szMessege, NULL, prj.GetTextColor(TID_PK_BLINK_NO) );
		return;
	}

	if( bResult && pItemProp->dwID == II_SYS_SYS_SCR_RETURN )		// 귀환의 두루마리 (bResult - 더블클릭의 경우)
	{
		if( g_pPlayer->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_RETURN ) )
		{
			g_WndMng.PutString(TID_GAME_LIMITED_USE);
			return;
		}

		if( g_WndMng.m_pWndMessageBox == NULL )
		{
			CReturnScrollMsgBox * pMsgBox = new CReturnScrollMsgBox;
			m_dwReturnScroll = dwItemId;
			g_WndMng.OpenCustomBox( pMsgBox );
		}
		return;
	}

	if( bResult && ( pItemProp->dwID == II_SYS_SYS_SCR_SMELPROT 		// 보호의 두루마리(하급)
#ifdef __SM_ITEM_2ND_EX		
		|| pItemProp->dwID == II_SYS_SYS_SCR_SMELPROT2
#endif	// __SM_ITEM_2ND_EX
		|| pItemProp->dwID == II_SYS_SYS_SCR_SMELPROT3
		|| pItemProp->dwID == II_SYS_SYS_SCR_SMELPROT4
		))
	{
		SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
		g_WndMng.m_pWndCommItemDlg	= new CWndCommItemDlg;
		g_WndMng.m_pWndCommItemDlg->Initialize();
		g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_REGARDLESS_USE, dwId, objid );
		return;
	}

	if( bResult && pItemProp->dwID == II_SYS_SYS_SCR_SMELTING )	// 제련의 두루마리
	{
		SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
		g_WndMng.m_pWndCommItemDlg	= new CWndCommItemDlg;
		g_WndMng.m_pWndCommItemDlg->Initialize();
		g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_LV7OVER_NOTUSE, dwId, objid );
		return;
	}

	if( bResult && pItemProp->dwID == II_SYS_SYS_SCR_SMELTING2 )	// 속성 제련의 두루마리
	{
		SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
		g_WndMng.m_pWndCommItemDlg	= new CWndCommItemDlg;
		g_WndMng.m_pWndCommItemDlg->Initialize();
		g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_LV10OVER_NOTUSE, dwId, objid );
		return;
	}

	if( pItemProp->dwID == II_SYS_SYS_SCR_PIEPROT )		// 보호의 두루마리(상급)
	{
		g_WndMng.PutString(TID_GAME_ONLYPIER_USE);
		return;
	}

	if( pItemProp->dwID == II_SYS_SYS_SCR_HAIRCHANGE )	// 헤어 체인지 교환권
	{
		if(g_WndMng.m_pWndUseCouponConfirm == NULL)
		{
			g_WndMng.m_pWndUseCouponConfirm = new CWndUseCouponConfirm;
			g_WndMng.m_pWndUseCouponConfirm->SetInfo(APP_BEAUTY_SHOP_EX, 0);
			g_WndMng.m_pWndUseCouponConfirm->Initialize();
		}
		return;
	}
	else if( pItemProp->dwID == II_SYS_SYS_SCR_FACEOFFFREE )	// 메이크업 무료 이용권
	{
		if(g_WndMng.m_pWndUseCouponConfirm == NULL)
		{
			g_WndMng.m_pWndUseCouponConfirm = new CWndUseCouponConfirm;
			g_WndMng.m_pWndUseCouponConfirm->SetInfo(APP_BEAUTY_SHOP_SKIN, 0);
			g_WndMng.m_pWndUseCouponConfirm->Initialize();
		}
		return;
	}

	if( bResult && pItemProp->dwID == II_SYS_SYS_SCR_SUPERSMELTING )		// 제련의 두루마리
	{
		SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
		g_WndMng.m_pWndCommItemDlg	= new CWndCommItemDlg;
		g_WndMng.m_pWndCommItemDlg->Initialize();
		g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_LV7OVER_NOTUSE, dwId, objid );
		return;
	}

	if( pItemProp->dwID == II_SYS_SYS_SCR_CHANAM )
	{
		if( !g_WndMng.m_pWndChangeName )
		{
			g_WndMng.m_pWndChangeName		= new CWndChangeName;
			g_WndMng.m_pWndChangeName->Initialize();
		}
		g_WndMng.m_pWndChangeName->SetData( (WORD)( pItemBase->m_dwObjId ), 0 );
		return;
	}
	
	if( pItemProp->dwID == II_SYS_SYS_SCR_CUSTODY )
	{
		SendOpenBankWnd( pItemBase->m_dwObjId );
		return;
	}
	
	if( pItemProp->dwID == II_SYS_SYS_SCR_GCHANAM )
	{
		CWndGuildName *pWndGuildName	= (CWndGuildName*)g_WndMng.GetWndBase( APP_GUILDNAME );
		if( !pWndGuildName )
		{
			pWndGuildName	= new CWndGuildName;
			pWndGuildName->Initialize();
		}
		pWndGuildName->SetData( (BYTE)( pItemBase->m_dwObjId ) );
		return;
	}

	if( pItemProp->dwItemKind1 == IK1_RIDE ) // 탈 것을 사용하려면 메뉴항목을 없애고 사용
	{
		CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
		if( pWndWorld )
			pWndWorld->m_wndMenuMover.SetVisible(FALSE);
	}
#ifdef __FUNNY_COIN
	if(bResult && pItemProp->dwItemKind2 == IK2_TOCASH)
	{
		SAFE_DELETE( g_WndMng.m_pFunnyCoinConfirm );
		g_WndMng.m_pFunnyCoinConfirm = new CWndFunnyCoinConfirm;
		g_WndMng.m_pFunnyCoinConfirm->SetInfo( dwId, pItemBase );
		g_WndMng.m_pFunnyCoinConfirm->Initialize();
		
		return;
	}
#endif //__FUNNY_COIN
	if( pItemProp->dwID == II_CHR_SYS_SCR_TRANSY && nPart == -1 )
	{		
		// 이 아이템은 방어구가 장착되어있으면 안된다~~~~
		for( DWORD dwParts = 0; dwParts < MAX_HUMAN_PARTS; dwParts++ )
		{
			if( dwParts == PARTS_HEAD || dwParts == PARTS_HAIR || dwParts == PARTS_LWEAPON
				|| dwParts == PARTS_RWEAPON || dwParts == PARTS_SHIELD || dwParts == PARTS_RIDE
				|| ( dwParts >= PARTS_NECKLACE1 && dwParts <= PARTS_BULLET ) )
				continue;
			
			CItemElem* pArmor	= g_pPlayer->m_Inventory.GetEquip( dwParts );
			if( pArmor )
			{
				g_WndMng.PutString(TID_GAME_CHECK_EQUIP);
				return;
			}			
		}
		
		CWndChangeSex* pWndChangeSex	= (CWndChangeSex*)g_WndMng.GetWndBase( APP_CHANGESEX );
		if( NULL == pWndChangeSex )
		{
			pWndChangeSex	= new CWndChangeSex;
			pWndChangeSex->Initialize();
		}
		pWndChangeSex->SetData( dwItemId, objid );
		CWndStatic* pWndStatic = (CWndStatic*)pWndChangeSex->GetDlgItem( WIDC_STATIC1 );
		if( pWndStatic )
			pWndStatic->SetVisible( FALSE );
		return;
	}

#ifdef __Y_BEAUTY_SHOP_CHARGE
	if( ::GetLanguage() == LANG_TWN )
	{		
		if( pItemProp->dwID == II_SYS_SYS_SCR_BEAUTYSHOP && nPart == -1 )
		{		
			if( g_WndMng.m_pWndBeautyShop )
				return;

			g_WndMng.m_pWndBeautyShop = new CWndBeautyShop;
			g_WndMng.m_pWndBeautyShop->Initialize();

			return;
		}
	}
#endif //__Y_BEAUTY_SHOP_CHARGE
	
	if( pItemProp->dwExeTarget == EXT_PET )
	{
		CPet* pPet	= g_pPlayer->GetPet();
		if( !pPet )
		{
			g_WndMng.PutString(TID_GAME_PET_NOT_FOUND);
			return;
		}
	}
	if( bResult )
	{
		switch( pItemProp->dwID )
		{
			case II_SYS_SYS_SCR_PET_TAMER_MIRACLE:
				SAFE_DELETE( g_WndMng.m_pWndPetMiracle );
				g_WndMng.m_pWndPetMiracle = new CWndPetMiracle;
				g_WndMng.m_pWndPetMiracle->Initialize();
				g_WndMng.m_pWndPetMiracle->SetItem( dwId );
				return;
			case II_SYS_SYS_SCR_PET_TAMER_MISTAKE:
				SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
				g_WndMng.m_pWndCommItemDlg = new CWndCommItemDlg;
				g_WndMng.m_pWndCommItemDlg->Initialize();
				g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_PET_MISTAKE_DESC, dwId, II_SYS_SYS_SCR_PET_TAMER_MISTAKE );
				return;
			case II_SYS_SYS_SCR_PET_HATCH:
				SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
				g_WndMng.m_pWndCommItemDlg = new CWndCommItemDlg;
				g_WndMng.m_pWndCommItemDlg->Initialize();
				g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_PET_HATCH_DESC, dwId, objid );
				return;	
			case II_SYS_SYS_FEED_01:
				{	// 별도의 개수 입력 없이 최대 사용 가능한 양을 사용하도록 수정
					SendUsePetFeed( dwId );
					return;
				}
			case II_SYS_SYS_FEED_MAKER:
				if(g_WndMng.m_pWndPetFoodMill != NULL)
					g_WndMng.PutString( prj.GetText( TID_GAME_WND_OVERLAPED ), NULL, prj.GetTextColor( TID_GAME_WND_OVERLAPED ) );
				else
				{
					SAFE_DELETE( g_WndMng.m_pWndPetFoodMill );
					g_WndMng.m_pWndPetFoodMill = new CWndPetFoodMill;
					g_WndMng.m_pWndPetFoodMill->Initialize();
					g_WndMng.m_pWndPetFoodMill->SetItem( dwId );
				}
				return;
			case II_SYS_SYS_SCR_PET_LIFE:
				SAFE_DELETE( g_WndMng.m_pWndPetLifeConfirm );
				g_WndMng.m_pWndPetLifeConfirm = new CWndPetLifeConfirm;
				g_WndMng.m_pWndPetLifeConfirm->SetItem(dwId);
				g_WndMng.m_pWndPetLifeConfirm->Initialize();				
				return;
			case II_GEN_TOO_COL_NORMALBATTERY:
				SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
				g_WndMng.m_pWndCommItemDlg = new CWndCommItemDlg;
				g_WndMng.m_pWndCommItemDlg->Initialize();
				g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_BETTERY_DESC, dwId, II_GEN_TOO_COL_NORMALBATTERY );
				return;	
			case II_GEN_TOO_COL_BATTERY001:
			case II_GEN_TOO_COL_SILVERBATTERY:
			case II_GEN_TOO_COL_GOLDBATTERY:
				SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
				g_WndMng.m_pWndCommItemDlg = new CWndCommItemDlg;
				g_WndMng.m_pWndCommItemDlg->Initialize();
				g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_CACHE_BETTERY_DESC, dwId, pItemProp->dwID );
				return;	
			default:
				break;
		}
	}

#ifdef __AZRIA_1023
	CItemElem * pItem = pItemBase;
	if( !pItem->IsFlag( CItemElem::expired ) )
	{
		const TicketProp * const pTicketProp = g_ticketProperties.GetTicketProp(pItemProp->dwID);
		if( pTicketProp && g_pPlayer )
		{
			if( g_pPlayer->GetWorld()->GetID() == pTicketProp->dwWorldId )
			{
				SAFE_DELETE( g_WndMng.m_pWndCommItemDlg );
				g_WndMng.m_pWndCommItemDlg	= new CWndCommItemDlg;
				g_WndMng.m_pWndCommItemDlg->Initialize();
				g_WndMng.m_pWndCommItemDlg->SetItem( TID_GAME_TICKET_DESC, dwId,  pItemProp->dwID );
				return;
			}
			else
			{
				// 확장 레이어 개수
				const unsigned int nExpand	= g_ticketProperties.GetExpandedLayer( pTicketProp->dwWorldId );
				/*
				1. 개요
					입장권 아이템에 정의된 레이어 선택 창을 출력한다.(CDPClient::SendDoUseItem에서 __AZRIA_1023이 있는 곳에서)
					사용자가 레이어를 선택하면, CDPClient::SendDoUseItemInput( DWORD dwData, char* szInput )을
					호출한다. dwData값에는 dwItemId을 넘기고, szInput은 선택한 레이어의 키를 문자열로 변환하여
					넘긴다(입력값이 있는 아이템 사용의 일반화를 위함)
				2. 레이어 키
					만약 위의 nExpand가 0이라면 확장된 레이어가 없다는 의미이고,
					선택 가능한 레이어의 키는 기본레이어 키 0이 되며, szInput에 "0" 을 넘긴다.
					만약 nExpand가 1이라면(이번 13차 아즈리아 예) 확장된 레이어가
					하나라는 의미이고, 진입할 수 있는 레이어는 기본레이어(레이어 키 0)
					와 확장된 레이어(레이어키  -1)이다.
					확장 레이어의 키는 -1 부터 1씩 감소한다. 즉 nExpand가 3이라면
					들어갈 수 있는 레이어의 키는 0, -1, -2, -3
					시스템이 미리 정의하는 레이어의 키는 음수로 확장되고, 사용자에 의해서
					가변적으로 만들어지는 레이어(하우징, 인스턴스 던전 등)은 양수로 확장된다.
				*/
				SAFE_DELETE( g_WndMng.m_pWndSelectCh );
				g_WndMng.m_pWndSelectCh = new CWndSelectCh( dwItemId, nExpand);
				g_WndMng.m_pWndSelectCh->Initialize();
			}
		}
	}
#endif	// __AZRIA_1023

	if( pItemProp->dwID == II_CHR_SYS_SCR_ITEMTRANSY_A || pItemProp->dwID == II_CHR_SYS_SCR_ITEMTRANSY_B )
	{
		CWndItemTransy* pWndItemTransy = (CWndItemTransy*)g_WndMng.GetWndBase( APP_ITEM_TRANSY );
		if( !pWndItemTransy )
		{
			pWndItemTransy	= new CWndItemTransy;
			pWndItemTransy->Initialize();
			pWndItemTransy->Init( pItemBase );
		}
		return;
	}
	
	if( pItemProp->dwItemKind3 == IK3_CREATE_MONSTER )
	{
		if( bCreateMonster == FALSE && g_Neuz.m_pCreateMonItem == NULL )
		{
			g_Neuz.m_pCreateMonItem = pItemBase;
			CreateSfx( XI_CHR_CURSOR1, g_pPlayer->GetPos(), g_pPlayer->GetId(), g_pPlayer->GetPos(), g_pPlayer->GetId(), -1 );
		}
		return;
	}

	if( pItemProp->dwID == II_SYS_SYS_SCR_PARTYSUMMON )
	{
		if( !g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_TROUPECREATE ), NULL, prj.GetTextColor(TID_GAME_TROUPECREATE) );
			return;
		}

		CWndSummonParty *pWndSummonParty = (CWndSummonParty*)g_WndMng.GetWndBase( APP_SUMMON_PARTY );
		if( !pWndSummonParty )
		{
			pWndSummonParty = new CWndSummonParty;
			pWndSummonParty->Initialize();
		}
		pWndSummonParty->SetData( (WORD)( pItemBase->m_dwObjId ), 0 );
		return;
	}

	if( pItemBase->GetProp()->dwParts == PARTS_RWEAPON )
	{
		CItemElem* pItemElemWepon = g_pPlayer->m_Inventory.GetEquip( PARTS_RWEAPON	);
		if( pItemElemWepon && pItemElemWepon->GetProp()->dwItemKind3 == IK3_BOW ) // 장착되어있는것이 활이면
		{
			if( pItemBase->GetProp()->dwItemKind3 != IK3_BOW )	// 장착되어있는것이 보우면 안벗겨두 됨
			{
				CItemElem* pItemElemBullet = g_pPlayer->m_Inventory.GetEquip( PARTS_BULLET );	
				if( pItemElemBullet && pItemElemBullet->GetProp()->dwItemKind3 == IK3_ARROW )
				{
					BEFORESENDSOLE( ar, PACKETTYPE_DOUSEITEM, DPID_UNKNOWN );
					ar << MAKELONG( ITYPE_ITEM, pItemElemBullet->m_dwObjId );
					ar << pItemElemBullet->m_dwObjId;
					ar << PARTS_BULLET;
					SEND( ar, this, DPID_SERVERPLAYER );
				}			
			}
		}
	}

	if( pItemProp->dwID == II_SYS_SYS_SCR_SEAL )
	{
		if( !g_WndMng.m_pWndSealChar )
		{
			g_WndMng.m_pWndSealChar		= new CWndSealChar;
			g_WndMng.m_pWndSealChar->Initialize();
		}
		return;
	}
	if( pItemProp->dwID == II_SYS_SYS_SCR_SEALCHARACTER )
	{
		CWndSealCharSet::OpenOrResetWindow(dwItemId);
		return;
	}

	if( pItemProp->dwID == II_SYS_SYS_SCR_PET_NAMING )
	{
		// 펫 작명 입력 창을 활성화 시킨다.
		// 위 창에서 이름을 입력한 후 확인 버튼을 누르면
		// CDPClient::SendDoUseItemInput( DWORD dwData, char* szInput )을
		// 호출한다. dwData값에는 dwItemId을 넘기고, szInput은 이름 문자열을 넘긴다.
		SAFE_DELETE( g_WndMng.m_pWndChangePetName );
		g_WndMng.m_pWndChangePetName = new CWndChangePetName;
		g_WndMng.m_pWndChangePetName->Initialize();
		g_WndMng.m_pWndChangePetName->SetItemId(dwItemId);
		return;
	}

	BEFORESENDSOLE( ar, PACKETTYPE_DOUSEITEM, DPID_UNKNOWN );
	ar << dwItemId << objid;
	ar << nPart;
#ifdef __HACK_1023
	ar << pItemBase->GetProp()->fFlightSpeed;
#endif	// __HACK_1023
	SEND( ar, this, DPID_SERVERPLAYER );
	g_nItemDelay = 60;

	// 쿨타임 아이템이면 사용시각은 서버에서 ACK받고 기록한다.
}

void CDPClient::SendPlayerCorr( void )
{
	CMover* pPlayer	= g_pPlayer;
	if( pPlayer ) {
		D3DXVECTOR3 vd, v	= pPlayer->GetPos();
		vd	= pPlayer->m_pActMover->m_vDelta;
		float f	= pPlayer->GetAngle();

		DWORD dwMotion	= pPlayer->m_dwMotion;
//		if( pPlayer->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) ) {
//			ItemProp* pItemProp	= pPlayer->GetActiveHandItemProp();
//			switch( pItemProp->dwItemKind3 )
//			{
//				case IK3_SWD:	dwMotion	-= 100;	break;
//				case IK3_WAND:	dwMotion	-= 100;	break;
//				default:	dwMotion	-= 100;	break;
//			}
//		}

		BEFORESENDSOLE( ar, PACKETTYPE_PLAYERCORR, DPID_UNKNOWN );

		ar << v << vd << f;
		ar << pPlayer->m_pActMover->GetState() << pPlayer->m_pActMover->GetStateFlag();
		ar << dwMotion << pPlayer->m_pActMover->m_nMotionEx;
		ar << pPlayer->m_pModel->m_nLoop << pPlayer->m_dwMotionOption;	//(BOOL)0
		ar << g_TickCount.GetTickCount();

		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendPlayerCorr2( void )
{
	CMover* pPlayer	= g_pPlayer;
	if( pPlayer ) 
	{
		D3DXVECTOR3 vd, v	= pPlayer->GetPos();
		vd	= pPlayer->m_pActMover->m_vDelta;
		float f		= pPlayer->GetAngle();
		float fAngleX	= pPlayer->m_pActMover->m_fTurnAngle;
		float fAccPower	= pPlayer->m_pActMover->m_fAccPower;
		float fTurnAngle	= pPlayer->m_pActMover->m_fTurnAngle;

		DWORD dwMotion	= pPlayer->m_dwMotion;

		BEFORESENDSOLE( ar, PACKETTYPE_PLAYERCORR2, DPID_UNKNOWN );

		ar << v << vd << f << fAngleX << fAccPower << fTurnAngle;
		ar << pPlayer->m_pActMover->GetState() << pPlayer->m_pActMover->GetStateFlag();
		ar << dwMotion << pPlayer->m_pActMover->m_nMotionEx;
		ar << pPlayer->m_pModel->m_nLoop << pPlayer->m_dwMotionOption;	//(BOOL)0
		ar << g_TickCount.GetTickCount();

		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendPlayerDestObj(OBJID objid, float fRange) {
	if (IsValidObj(g_pPlayer)) {
		SendPacket<PACKETTYPE_PLAYERSETDESTOBJ, OBJID, float>(objid, fRange);
	}
}

void CDPClient::SendQueryGetDestObj( CMover* pMover )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYGETDESTOBJ, DPID_UNKNOWN );
	ar << pMover->GetId();
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendQueryGetPos( CMover* pMover )
{
	if( FALSE == pMover->m_fWaitQueryGetPos )
	{
		pMover->m_fWaitQueryGetPos	= TRUE;
		BEFORESENDSOLE( ar, PACKETTYPE_QUERYGETPOS, DPID_UNKNOWN );
		ar << pMover->GetId();
		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendGetPos( const D3DXVECTOR3 & vPos, float fAngle, OBJID objid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETPOS, DPID_UNKNOWN );
	ar << vPos << fAngle << objid;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendCtrlCoolTimeCancel( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CTRL_COOLTIME_CANCEL, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSkillTaskBar( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SKILLTASKBAR, DPID_UNKNOWN );
	ar << g_WndMng.m_pWndTaskBar->m_aSlotQueue;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendAddInTaskbar(
	const std::optional<unsigned int> where,
	const unsigned int nIndex,
	const SHORTCUT & shortcut
) {
	using Operation = CTaskbar::Operation;
	using BarName = CTaskbar::BarName;

	if (where.has_value()) {
		SendPacket<PACKETTYPE_MODIFYTASKBAR,
			Operation, BarName, unsigned int, unsigned int, SHORTCUT
		>(Operation::Add, BarName::Item, where.value(), nIndex, shortcut);
	} else {
		SendPacket<PACKETTYPE_MODIFYTASKBAR,
			Operation, BarName, unsigned int, SHORTCUT
		>(Operation::Add, BarName::Applet, nIndex, shortcut);
	}
}

void CDPClient::SendRemoveAppletTaskBar(BYTE nIndex) {
	using Operation = CTaskbar::Operation;
	using BarName = CTaskbar::BarName;

	SendPacket<PACKETTYPE_MODIFYTASKBAR,
		Operation, BarName, unsigned int
	>(
		Operation::Remove, BarName::Applet, nIndex
		);
}

void CDPClient::SendRemoveItemTaskBar(BYTE nSlotIndex, BYTE nIndex) {
	using Operation = CTaskbar::Operation;
	using BarName = CTaskbar::BarName;

	SendPacket<PACKETTYPE_MODIFYTASKBAR,
		Operation, BarName, unsigned int, unsigned int
	>(
		Operation::Remove, BarName::Item, nSlotIndex, nIndex
		);
}

void CDPClient::SendAddFriendReqest( u_long uidPlayer )
{
	if( g_pPlayer == NULL )
		return;

	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDREQEST, DPID_UNKNOWN );
	ar << g_pPlayer->m_idPlayer << uidPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendAddFriendNameReqest( const char* szName )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDNAMEREQEST, DPID_UNKNOWN );
	ar << g_pPlayer->m_idPlayer;
	ar.WriteString(szName);
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendFriendCancel( u_long uidLeader, u_long uidMember )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADDFRIENDCANCEL, DPID_UNKNOWN );
	ar << uidLeader << uidMember;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void	CDPClient::SendAvailPocket( int nPocket, int nItem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_AVAIL_POCKET, DPID_UNKNOWN );
	ar << nPocket << nItem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void	CDPClient::SendMoveItem_Pocket( int nPocket1, int nItem, short nNum, int nPocket2 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_MOVE_ITEM_POCKET, DPID_UNKNOWN );
	ar << nPocket1 << nItem << nNum << nPocket2;
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::SendGetFriendState() {
	SendPacket<PACKETTYPE_GETFRIENDSTATE>();
}

void CDPClient::SendRemoveFriend( u_long uidPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_REMOVEFRIEND, DPID_UNKNOWN );
	ar << g_pPlayer->m_idPlayer;
	ar << uidPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );	
}

void CDPClient::SendBlock( BYTE Gu, const char *szName, const char *szFrom )
{
	BEFORESENDSOLE( ar, PACKETTYPE_BLOCK, DPID_UNKNOWN );
	ar << Gu;
	ar.WriteString( szName );
	ar.WriteString( szFrom );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::PutPlayerDestPos(const PLAYERDESTPOS & playerDestPos, BYTE f) {
	m_ss.playerdestpos = playerDestPos;
	if (f)	m_ss.uFrameMove = 1;
}

void CDPClient::ClearPlayerDestPos() {
	m_ss.playerdestpos = std::nullopt;
}

void CDPClient::SendSnapshot( BOOL fUnconditional )
{
	m_ss.uFrameMove++;
	if( fUnconditional || m_ss.uFrameMove % 30 == 0 )	// 5 / 1
	{
	if( m_ss.playerdestpos )
	{
		if( g_pPlayer->m_pActMover->IsSit() )
			SendMotion( OBJMSG_STANDUP );
	}
		BEFORESENDSOLE( ar, PACKETTYPE_SNAPSHOT, DPID_UNKNOWN );

		auto pC = ar.PushBack<BYTE>(0);

		if( m_ss.playerdestpos )
		{
			ar << SNAPSHOTTYPE_DESTPOS;
			ar << m_ss.playerdestpos->vPos;	// 12
			ar << m_ss.playerdestpos->fForward;	// 1

#ifdef __IAOBJ0622
			ar << m_ss.playerdestpos->objidIAObj;
#endif	// __IAOBJ0622

			m_ss.playerdestpos = std::nullopt;
			(*pC)++;
		}

		if (*pC > 0) {
			SEND(ar, this, DPID_SERVERPLAYER);
		}

		m_ss.uFrameMove		= 0;
	}
}

void CDPClient::SendSfxHit(int idSfxHit, DWORD dwSkill, OBJID idAttacker) {
	SendPacket<PACKETTYPE_SFX_HIT>(idSfxHit, dwSkill, idAttacker);
}

void CDPClient::SendSfxClear( int idSfxHit, OBJID idMover )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SFX_CLEAR, DPID_UNKNOWN );
	ar << idSfxHit;
	ar << idMover;
	
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRemoveQuest(const QuestId dwQuest) {
	SendPacket<PACKETTYPE_REMOVEQUEST, QuestId>(dwQuest);
}

void CDPClient::SendHdr( DWORD dwHdr )
{
	BEFORESENDSOLE( ar, dwHdr, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendScriptReplace( DWORD dwWorld, D3DXVECTOR3 vPos )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_REPLACE, DPID_UNKNOWN );
	ar << dwWorld << vPos;
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendScriptReplaceKey( DWORD dwWorld, LPCSTR lpszKey )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SCRIPT_REPLACE_KEY, DPID_UNKNOWN );
	ar << dwWorld;
	ar.WriteString( lpszKey );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendCreateAngel(const std::vector<ItemPos> & items) {
	BEFORESENDSOLE( ar, PACKETTYPE_CREATEANGEL, DPID_UNKNOWN );
	ar << items.size();
	for (const ItemPos objid : items) {
		ar << objid;
	}
	SEND( ar, this, DPID_SERVERPLAYER );
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void CDPClient::OnAngel(OBJID objid, CAr & ar) {
	CMover * pMover = prj.GetMover(objid);
	if (!pMover) return;

	bool bComplete; ar >> bComplete;

	ar >> pMover->m_nAngelExp >> pMover->m_nAngelLevel;

	if (bComplete) {
		g_WndMng.PutString(TID_GAME_COMPLETE_ANGEL);
	}
}

#ifdef __EVE_MINIGAME
void CDPClient::OnMiniGame( OBJID objid, CAr & ar )
{
	BYTE nState;
	ar >> nState;
	switch( nState )
	{
	case MINIGAME_KAWIBAWIBO_RESUTLT:
		OnKawibawibo_Result( ar );
		break;
	case MINIGAME_REASSEMBLE_RESULT:
		OnReassemble_Result( ar );
		break;
	case MINIGAME_REASSEMBLE_OPENWND:
		OnReassemble_OpenWnd( ar );
		break;
	case MINIGAME_ALPHABET_OPENWND:
		OnAlphabet_OpenWnd( ar );
		break;
	case MINIGAME_ALPHABET_RESULT:
		OnAlphabet_Result( ar );
		break;
	case MINIGAME_FIVESYSTEM_OPENWND:
		OnFiveSystem_OpenWnd( ar );
		break;
	case MINIGAME_FIVESYSTEM_RESULT:
		OnFiveSystem_Result( ar );
	}
}

void CDPClient::OnKawibawibo_Result( CAr & ar )
{
	int nResult;

	int		nWinCount		= 0;
	int		nMaxWinCount	= 0;
	DWORD	dwItemId		= (DWORD)( -1 );
	DWORD	dwNextItemId	= (DWORD)( -1 );
	int		nItemCount		= -1;
	int		nNextItemCount	= -1;

	ar >> nResult;
	ar >> nWinCount;

	CWndKawiBawiBoGame* pWndGame = (CWndKawiBawiBoGame*)g_WndMng.GetWndBase( APP_MINIGAME_KAWIBAWIBO );
	
	if( nResult == CMiniGame::KAWIBAWIBO_WIN )
	{
		ar >> dwItemId >> dwNextItemId;
		ar >> nItemCount >> nNextItemCount;
		ar >> nMaxWinCount;
		
		pWndGame->m_nWinningMaxCount = nMaxWinCount;
		ItemProp* ITem = prj.GetItemProp(dwItemId);
		ItemProp* ITemNext = prj.GetItemProp(dwNextItemId);
	}

	else if( nResult == CMiniGame::KAWIBAWIBO_GETITEM )
	{
		ar >> dwItemId;
		ar >> nItemCount;	
		ItemProp* ITem = prj.GetItemProp(dwItemId);

		CString message;

		if(pWndGame)
		{
			message.Format(prj.GetText( TID_GAME_KAWIBAWIBO_WIN ), pWndGame->m_nWinningCount, ITem->szName, nItemCount);
			pWndGame->m_nWinningCount = 0;
			pWndGame->RefreshInfo();
		}
		else
			message.Format(prj.GetText( TID_GAME_KAWIBAWIBO_WIN ), nWinCount, ITem->szName, nItemCount);
		
		g_WndMng.OpenMessageBox( message );				
	}

	if(pWndGame != NULL)
	{
		pWndGame->ReceiveResult(nResult, nItemCount, nNextItemCount, dwItemId, dwNextItemId);
	}
}

void CDPClient::OnReassemble_OpenWnd( CAr & ar )
{
	int nCount;
	ar >> nCount;

	std::vector<DWORD> vecItemId;
	DWORD dwItemId;
	for( int i=0; i<nCount; ++i )
	{
		ar >> dwItemId;
		vecItemId.push_back( dwItemId );
	}

	// vecItemId - 퍼즐 아이템의 ID값	
	CWndPuzzleGame* pWndGame = (CWndPuzzleGame*)g_WndMng.GetWndBase( APP_MINIGAME_PUZZLE );
	if(pWndGame != NULL)
		pWndGame->SetPuzzle(vecItemId);
}


void CDPClient::OnReassemble_Result( CAr & ar )
{
	DWORD dwItemId;
	int nItemCount;
	CWndPuzzleGame* pWndGame = (CWndPuzzleGame*)g_WndMng.GetWndBase( APP_MINIGAME_PUZZLE );

	ar >> dwItemId;
	ar >> nItemCount;

	CString strMessage;

	ItemProp* ITem = prj.GetItemProp(dwItemId);
	strMessage.Format( "성공 %s %d", ITem->szName, nItemCount );
	g_WndMng.PutString( strMessage, NULL, prj.GetTextColor(TID_ADMIN_ANNOUNCE), CHATSTY_GAME_CHAT );

	if(pWndGame != NULL)
		pWndGame->ReceiveResult();
}

void CDPClient::OnAlphabet_OpenWnd( CAr & ar )
{
	int nQuestionID;
	int nPublic;
	char chPublicChar;
	char szQuestion[255] = {0,};

	ar >> nQuestionID;
	ar >> nPublic;
	ar >> chPublicChar;
	ar.ReadString( szQuestion, 254 );

	// 창띄움 알파벳 게임
	CWndFindWordGame* pWndGame = (CWndFindWordGame*)g_WndMng.GetWndBase( APP_MINIGAME_WORD );
	if(pWndGame != NULL)
	{
		pWndGame->m_nQuestionID = nQuestionID;
		pWndGame->SetFirstWord(nPublic, chPublicChar);
		pWndGame->SetQuestion( szQuestion );
	}
}

void CDPClient::OnAlphabet_Result( CAr & ar )
{
	int nResult = CMiniGame::ALPHABET_FAILED;
	DWORD dwItemId = NULL_ID;
	int nItemCount = 0;

	ar >> nResult;
	
	if(nResult != CMiniGame::ALPHABET_FAILED)
	{
		// 성공시
		ar >> dwItemId;
		ar >> nItemCount;
	}

	CWndFindWordGame* pWndGame = (CWndFindWordGame*)g_WndMng.GetWndBase( APP_MINIGAME_WORD );
	if(pWndGame != NULL)
		pWndGame->ReceiveResult(nResult, dwItemId, nItemCount);
}

void CDPClient::OnFiveSystem_OpenWnd( CAr & ar )
{
	int nMinBetPenya;
	int nMaxBetPenya;
	int nMulitple;

	ar >> nMinBetPenya;
	ar >> nMaxBetPenya;
	ar >> nMulitple;
	
	CWndDiceGame* pWndGame = (CWndDiceGame*)g_WndMng.GetWndBase( APP_MINIGAME_DICE );
	if(pWndGame != NULL)
		pWndGame->SetMinMaxPenya(nMinBetPenya, nMaxBetPenya, nMulitple);
}

void CDPClient::OnFiveSystem_Result( CAr & ar )
{
	int nResult;
	int nGetPenya;
	int nBetNum;

	ar >> nResult;
	
	CWndDiceGame* pWndGame = (CWndDiceGame*)g_WndMng.GetWndBase( APP_MINIGAME_DICE );

	if( nResult == CMiniGame::FIVESYSTEM_NOTENOUGH )
	{
		// 입찰금 부족
	}
	else if( nResult == CMiniGame::FIVESYSTEM_OVERMAX )
	{
		// 당첨시 최대 금액 초과
	}
	else if( nResult == CMiniGame::FIVESYSTEM_FAILED )
	{
		// 실패 - 입찰 금액이 (-)이거나 당첨금 지급 실패
	}
	else if( nResult == CMiniGame::FIVESYSTEM_BETOK )
	{
		ar >> nGetPenya;
		ar >> nBetNum;

		if(pWndGame != NULL)
			pWndGame->SetTenderPenya(nBetNum, nGetPenya);		
	}
	else
	{
		ar >> nGetPenya; // 지급액
		// 결과
		if(pWndGame != NULL)
			pWndGame->ReceiveResult(nResult, nGetPenya);
	}
}
#endif // __EVE_MINIGAME

void CDPClient::OnUltimateWeapon( OBJID objid, CAr & ar )
{
	BYTE nState;
	ar >> nState;
	switch( nState )
	{
		case ULTIMATE_MAKEITEM: {
			CUltimateWeapon::Result nResult; ar >> nResult;

			if (CWndMixJewel * pWndMixJewel = g_WndMng.GetWndBase<CWndMixJewel>(APP_SMELT_MIXJEWEL)) {
				pWndMixJewel->ReceiveResult(nResult);
			}
			break;
		}
	case ULTIMATE_TRANSWEAPON:
		OnUltimateTransWeapon( objid, ar );
		break;
	case ULTIMATE_MAKEGEM: OnUltimateMakeGem(ar); break;
	case ULTIMATE_SETGEM:
		OnUltimateSetGem( objid, ar );
		break;
	case ULTIMATE_REMOVEGEM:
		OnUltimateRemoveGem( objid, ar );
		break;
	case ULTIMATE_ENCHANTWEAPON:
		OnUltimateEnchantWeapon( objid, ar );
		break;		
	}
}

void CDPClient::OnUltimateTransWeapon( OBJID objid, CAr & ar )
{
	int nResult;
	ar >> nResult;
	
	CWndChangeWeapon* pWndChangeWeapon = (CWndChangeWeapon*)g_WndMng.GetWndBase( APP_SMELT_CHANGEWEAPON );
	if(pWndChangeWeapon != NULL)
		pWndChangeWeapon->ReceiveResult(nResult);	
}

void CDPClient::OnUltimateMakeGem(CAr & ar) {
	CUltimateWeapon::MakeGemAnswer nResult; ar >> nResult;

	if (CWndExtraction * pWndExtraction = g_WndMng.GetWndBase<CWndExtraction>(APP_SMELT_EXTRACTION)) {
		pWndExtraction->ReceiveResult(nResult);
	}
}

void CDPClient::OnUltimateSetGem( OBJID objid, CAr & ar )
{
	int nResult;
	ar >> nResult;

	CWndSmeltJewel* pWndSmeltJewel = (CWndSmeltJewel*)g_WndMng.GetWndBase( APP_SMELT_JEWEL );
	if(pWndSmeltJewel != NULL)
		pWndSmeltJewel->ReceiveResult(nResult);	
}

void CDPClient::OnUltimateRemoveGem( OBJID objid, CAr & ar )
{
	int nResult;
	ar >> nResult;
}

void CDPClient::OnUltimateEnchantWeapon( OBJID objid, CAr & ar )
{
	int nResult;
	ar >> nResult;
}

#ifdef __TRADESYS
void CDPClient::OnExchange( OBJID objid, CAr & ar )
{
	BYTE nState;
	ar >> nState;
	switch( nState )
	{
	case EXCHANGE_RESULT: 
		OnExchangeResult( ar );
		break;
	}
}

void CDPClient::OnExchangeResult( CAr & ar )
{
	int nResult;
	ar >> nResult;

	CWndDialogEvent* pWndDialogEvent = (CWndDialogEvent*)g_WndMng.GetWndBase( APP_DIALOG_EVENT );
	if(pWndDialogEvent != NULL)
		pWndDialogEvent->ReceiveResult(nResult);	
}
#endif // __TRADESYS

void CDPClient::OnLegendSkillUp( CAr & ar )
{
	int nResult;
	ar >> nResult;

	CWndHeroSkillUp* pWndHeroSkillUp = (CWndHeroSkillUp*)g_WndMng.GetWndBase( APP_HERO_SKILLUP );
	if(pWndHeroSkillUp != NULL)
		pWndHeroSkillUp->ReceiveResult(nResult);
}

void CDPClient::OnShout( CAr & ar )
{
static	\
CHAR	lpszPlayer[MAX_PLAYER], lpString[260];

	OBJID objid;
	ar >> objid;
	ar.ReadString( lpszPlayer, MAX_PLAYER );
	ar.ReadString( lpString, 260 );
	DWORD dwColor	= 0xffff99cc;
	ar >> dwColor;

	CMover* pMover	= prj.GetMover( objid );

#ifdef __YS_CHATTING_BLOCKING_SYSTEM
	if( pMover && pMover->IsPlayer() == TRUE )
	{
		const auto BlockedUserIterator = prj.m_setBlockedUserID.find( lpszPlayer );
		if( BlockedUserIterator != prj.m_setBlockedUserID.end() )
			return;
	}
#endif // __YS_CHATTING_BLOCKING_SYSTEM

	// 말하는애가 길드컴뱃 맵에서 이야기를 했다면...검사해서...무시한다..-_-
	if( IsValidObj( pMover ) && pMover->GetWorld() && pMover->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
	{
		if( pMover->IsMode(GCWAR_RENDER_SKIP_MODE) )
			return;
	}

	if( g_Option.m_bShout )
	{
		CString str;
		str.Format( "[%s] %s", lpszPlayer, lpString );
		g_WndMng.PutString( str, NULL, dwColor, CHATSTY_SHOUT );
		if( g_pPlayer && g_pPlayer->m_dwAuthorization >= AUTH_GAMEMASTER )
			FILEOUT( "CHAT.LOG", (LPCSTR)str );

		if( IsValidObj( pMover ) ) 
		{
			if( strlen( lpString ) )
				g_DialogMsg.AddMessage( pMover, lpString, 0xfffffff, CHAT_SHOUT );
		}
	}
}

void CDPClient::OnPlayMusic( CAr & ar )
{
	u_long idmusic;
	ar >> idmusic;
	TRACE( "OnPlayMusic, %d\n", idmusic );
	PlayMusic( idmusic, 1 );	
}

void CDPClient::OnPlaySound( CAr & ar )
{
	char szSound[MAX_PATH]	= { 0, };
	u_long idsound;

	BYTE nType;
	ar >> nType;
	if( nType == 0 )
	{
		ar >> idsound;
		PLAYSND( idsound );
	}
	else if( nType == 1 )
	{
		ar.ReadString( szSound, MAX_PATH );
		if( g_pPlayer )
		{
			if( g_Option.m_bVoice )
			{
				PLAYSND( szSound, NULL, TRUE );
			}
		}
	}
}

void CDPClient::OnWhisper( CAr & ar )
{
	if( g_pPlayer == NULL )
		return;
static	\
CHAR	sPlayerFrom[MAX_PLAYER], sPlayerTo[MAX_PLAYER], lpString[260];
	int nSearch;
	u_long idFrom, idTo;

	ar.ReadString( sPlayerFrom, MAX_PLAYER );
	ar.ReadString( sPlayerTo, MAX_PLAYER );		//
	ar.ReadString( lpString, 260 );
	ar >> idFrom >> idTo;
	ar >> nSearch;

	Friend* pFriend		= NULL;
	u_long idPlayer		= 0;
	BOOL	bCheck	= FALSE;
	if( strcmp( sPlayerFrom, g_pPlayer->GetName() ) == 0 )
	{
		idPlayer	= idTo;
		pFriend		= g_WndMng.m_RTMessenger.GetFriend( idTo );
		bCheck	= TRUE;
	}
	else
	{
		idPlayer	= idFrom;
		pFriend		= g_WndMng.m_RTMessenger.GetFriend( idFrom );
	}

#ifdef __YS_CHATTING_BLOCKING_SYSTEM
	const auto BlockedUserIterator = prj.m_setBlockedUserID.find( sPlayerFrom );
	if( ( pFriend && pFriend->bBlock ) || ( BlockedUserIterator != prj.m_setBlockedUserID.end() ) )
#else // __YS_CHATTING_BLOCKING_SYSTEM
	if( pFriend && pFriend->bBlock )
#endif // __YS_CHATTING_BLOCKING_SYSTEM
	{
		if( bCheck )
		{
			CString szMessage;
			szMessage.Format( prj.GetText( TID_GAME_MSGBLOCKCHR ), sPlayerTo );		//szMessage += "님은 차단되어 있어 메세지를 보낼수 없습니다";
			g_WndMng.PutString( szMessage, NULL, prj.GetTextColor( TID_GAME_NOTLOGIN ) );
			CWndMessage* pWndMessage	=	g_WndMng.GetMessage( sPlayerTo );
			if( pWndMessage )
			{
				CWndEdit* pWndText	= (CWndEdit*)pWndMessage->GetDlgItem( WIDC_EDIT );
				CWndButton* pWndSend	= (CWndButton*)pWndMessage->GetDlgItem( WIDC_SEND );
				if( pWndText && pWndSend )
				{
					pWndSend->EnableWindow( FALSE );
					pWndText->SetVisible( FALSE );
				}
			}
		}
		return;
	}
	if( nSearch )
	{
		//wsprintf( lpString, "접속중이 아닙니다." );.
		//g_WndMng.PutString( lpString, NULL, 0xffffff00 );
		g_WndMng.PutString( prj.GetText( TID_GAME_NOTLOGIN ), NULL, prj.GetTextColor( TID_GAME_NOTLOGIN ) );
		return;
	}
	
	TRACE( "OnWhisper, %s - %s\n", sPlayerFrom, lpString );

	BOOL bWhisperOK = FALSE;
	BOOL bMySend = FALSE;
	// 속삭임
	CString str;
	if( strcmp( g_pPlayer->GetName(), sPlayerFrom ) == 0 )
	{
		str.Format( "-> [%s] %s", sPlayerTo, lpString );
		bMySend = TRUE;
	}
	else
	{
		str.Format( "<- [%s] %s", sPlayerFrom, lpString );
		if( g_pPlayer->IsAuthHigher( AUTH_LOGCHATTING ) )
		{
			BEFORESENDSOLE( ar1, PACKETTYPE_LOG_GAMEMASTER_CHAT, DPID_UNKNOWN );
			ar1.WriteString( sPlayerFrom );
			ar1.WriteString( lpString );
			SEND( ar1, this, DPID_SERVERPLAYER );
		}	
		bWhisperOK = TRUE;
	}

	if( bMySend == FALSE && g_Option.m_bSay == FALSE )
	{
		SendBlock( 1, g_pPlayer->GetName(), sPlayerFrom );
		return;
	}

	if( bWhisperOK && lstrlen( sPlayerFrom ) < MAX_NAME )
		sprintf( g_Neuz.m_szWhisperName, "%s", sPlayerFrom );

	g_WndMng.PutString( str, NULL, 0xffffff00, CHATSTY_WHISPER );
}

void CDPClient::OnSay( CAr & ar )
{
	if( g_pPlayer == NULL )
		return;
static	\
CHAR	sPlayerFrom[MAX_PLAYER], sPlayerTo[MAX_PLAYER], lpString[260];
	int nSearch;
	u_long idFrom, idTo;

	ar.ReadString( sPlayerFrom, MAX_PLAYER );
	ar.ReadString( sPlayerTo, MAX_PLAYER );		//
	ar.ReadString( lpString, 260 );
	ar >> idFrom >> idTo;
	ar >> nSearch;

	u_long idPlayer	= 0;
	DWORD dwState	= 0xFFFFFFFF;
	BOOL bCheck	= FALSE;
	if( strcmp( sPlayerFrom, g_pPlayer->GetName() ) == 0 )
	{
		idPlayer	= idTo;
		bCheck	= TRUE;
	}
	else
		idPlayer	= idFrom;
	Friend* pFriend		= g_WndMng.m_RTMessenger.GetFriend( idPlayer );
	
	if( pFriend )
	{
		if( pFriend->bBlock )
		{
			if( bCheck )
			{
				CString szMessage;
				szMessage.Format( prj.GetText(TID_GAME_MSGBLOCKCHR), sPlayerTo );		//szMessage += "님은 차단되어 있어 메세지를 보낼수 없습니다";

				g_WndMng.PutString( szMessage, NULL, prj.GetTextColor( TID_GAME_NOTLOGIN ) );
				CWndMessage* pWndMessage	=	g_WndMng.GetMessage( sPlayerTo );
				if( pWndMessage )
				{
					CWndEdit* pWndText	= (CWndEdit*)pWndMessage->GetDlgItem( WIDC_EDIT );
					CWndButton* pWndSend	= (CWndButton*)pWndMessage->GetDlgItem( WIDC_SEND );
					if( pWndText && pWndSend )
					{
						pWndSend->EnableWindow( FALSE );
						pWndText->SetVisible( FALSE );
					}
				}
			}
			return;
		}
	}

	if( nSearch )
	{
		//wsprintf( lpString, "접속중이 아닙니다." );.
		//g_WndMng.PutString( lpString, NULL, 0xffffff00 );

		g_WndMng.PutString( prj.GetText( TID_GAME_NOTLOGIN ), NULL, prj.GetTextColor( TID_GAME_NOTLOGIN ) );
		
		return;
	}
	
	TRACE( "OnSay, %s - %s\n", sPlayerFrom, lpString );

	CString strFrom = sPlayerFrom;
	CString strTo = sPlayerTo;
	// 내가 보낸 메시지일 경우 
	if( strcmp( sPlayerFrom, g_pPlayer->GetName() ) == 0 )
	{
		strFrom = sPlayerTo;
		strTo = sPlayerFrom;
		CWndMessage* pWndMessage =	g_WndMng.GetMessage( strFrom );
		if( pWndMessage ) 
			pWndMessage->AddMessage( sPlayerFrom, lpString );
		else
		{
			pWndMessage = g_WndMng.OpenMessage( strFrom );
			if( pWndMessage == NULL )
			{
				char szText[512];
				sprintf( szText, "OnSay : pWndMessage가 NULL %s %s %s", strFrom.GetString(), strTo.GetString(), lpString);
				//ADDERRORMSG( szText );
			}
				
			pWndMessage->AddMessage( strTo, lpString );
		}
	}
	else
	{	 
		if( 1 ) //g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) == FALSE )
		{
			if( g_Option.m_bSay == FALSE )
			{
				SendBlock( 1, g_pPlayer->GetName(), sPlayerFrom );
				return;
			}

			CWndMessage* pWndMessage =	g_WndMng.GetMessage( strFrom );
			if( pWndMessage ) 
				pWndMessage->AddMessage( sPlayerFrom, lpString );
			else
			{
				CWndInstantMsg* pWndInstantMsg = g_WndMng.OpenInstantMsg( strFrom );
				if( pWndInstantMsg ) 
					pWndInstantMsg->AddMessage( sPlayerFrom, lpString );

				CString str;
				str.Format( "[%s] %s", sPlayerFrom, lpString );
				g_WndMng.PutString( str, NULL, 0xffffff00 );		
			}
		}
		else
		{
			// 귓속말 
			CString str;
			str.Format( "[%s] %s", sPlayerFrom, lpString );
			g_WndMng.PutString( str, NULL, 0xffffff00 );		
		}
	}
}

void CDPClient::OnGMSay( CAr & ar )
{
static	\
CHAR	sPlayerFrom[MAX_PLAYER], sPlayerTo[MAX_PLAYER], lpString[260];

	ar.ReadString( sPlayerFrom, MAX_PLAYER );
	ar.ReadString( sPlayerTo, MAX_PLAYER );		//
	ar.ReadString( lpString, 260 );

	TRACE( "OnSay, %s - %s\n", sPlayerFrom, lpString );
	// 귓속말 
	CString str;
	str.Format( "[%s] %s", sPlayerFrom, lpString );
	g_WndMng.PutString( str, NULL, 0xffffff00 );
}

void CDPClient::OnGetPlayerAddr( CAr & ar )
{
static	\
CHAR	lpszPlayer[MAX_PLAYER], lpAddr[16];

	ar.ReadString( lpszPlayer, MAX_PLAYER );
	ar.ReadString( lpAddr, 16 );
	TRACE( "OnGetPlayerAddr, %s/%s\n", lpszPlayer, lpAddr );
	CString str;
	str.Format( "[IP] %s : %s", lpszPlayer, lpAddr );
	g_WndMng.PutString( str, NULL, 0xff0000ff );
	// ip addr
}

void CDPClient::OnGetPlayerCount( CAr & ar )
{
	u_short	uCount;
	ar >> uCount;
	TRACE( "OnGetPlayerCount, %d\n", uCount );

	CString str;
	//str.Format( "[월드동접] %d", uCount);

	str.Format( prj.GetText(TID_ADMIN_WORLDCOUNT), uCount );
	g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_ADMIN_WORLDCOUNT), CHATSTY_GENERAL );

	// 월드 동접자 
}

void CDPClient::OnGetCorePlayer( CAr & ar )
{
static	\
CHAR	lpszPlayer[64];
	u_long uCount;

	ar >> uCount;
	// 전체 
	TRACE( "OnGetCorePlayer, %d\n", uCount );
	TRACE( "--------------------------------------------------------------------------------\n" );
	for( u_short i = 0; i < uCount; i++ )
	{
		ar.ReadString( lpszPlayer, 64 );
		TRACE( "%s\n", lpszPlayer );
		CString str;
		str.Format( "%s", lpszPlayer);

		g_WndMng.PutString( str, NULL, 0xffffff80, CHATSTY_GENERAL );
	}
	TRACE( "--------------------------------------------------------------------------------\n" );
}

// 운영자 알림 메세지 
void CDPClient::OnSystem( CAr & ar )
{
	CHAR	lpString[512];

	ar.ReadString( lpString, 512 );

	CString str;
	str.Format( prj.GetText(TID_ADMIN_ANNOUNCE), lpString );		// [알림] %s
	g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_ADMIN_ANNOUNCE), CHATSTY_SYSTEM );
	if( ::GetLanguage() != LANG_GER && ::GetLanguage() != LANG_FRE )
	{
		CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
		if( pWndWorld )
			g_Caption1.AddCaption( str, pWndWorld->m_pFontAPITitle, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 153 ) );
	}
}

void CDPClient::OnCaption( CAr & ar )
{
	static	\
		CHAR	lpString[260];

	BOOL bSmall;
	DWORD dwWorldID;
	ar >> bSmall;
	ar >> dwWorldID;
	ar.ReadString( lpString, 260 );
	TRACE( "OnCaption, %s\n", lpString );

	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	if( pWndWorld )
	{
		BOOL bCaption = FALSE;
		if( dwWorldID == 0 )
			bCaption = TRUE;
		else if( g_pPlayer && g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->GetID() == dwWorldID )
			bCaption = TRUE;

		if( bCaption )
		{
			if( bSmall )
			{
				if( ::GetLanguage() != LANG_JAP )
					g_Caption1.AddCaption( lpString, pWndWorld->m_pFontAPICaption );// CWndBase::m_Theme.m_pFontCaption );
				else
					g_Caption1.AddCaption( lpString, NULL );// CWndBase::m_Theme.m_pFontCaption );
			}
			else
			{
				if( ::GetLanguage() != LANG_JAP )
					g_Caption1.AddCaption( lpString, pWndWorld->m_pFontAPITitle );// CWndBase::m_Theme.m_pFontCaption );
				else
					g_Caption1.AddCaption( lpString, NULL );// CWndBase::m_Theme.m_pFontCaption );
			}
		}
	}
}

void CDPClient::OnCtrlCoolTimeCancel(CAr & ar) {
	if (g_pPlayer) {
		g_pPlayer->m_dwCtrlReadyId = NULL_ID;
		g_pPlayer->m_dwCtrlReadyTime = 0xffffffff;
	}
}

void CDPClient::OnDefinedCaption( CAr & ar )
{
	TCHAR lpString[1024], argv[32][256], *pargv[32];
	int argc	= 0;
	BOOL bSmall;
	
	DWORD dwText;
	ar >> bSmall;
	ar >> dwText;
	ar.ReadString( lpString, 1024 );
	
	if( dwText == TID_GAME_REAPMONEY )
	{
		//예외처리: 돈 문자열에 ','가 들어가서 CScanner로 작업하기 원할치 않다.
		CString rString;
		AfxExtractSubString( rString, lpString, 0, ' ' );
		strcpy( argv[0], rString );
		AfxExtractSubString( rString, lpString, 1, ' ' );
		strcpy( argv[1], rString );
		argc = 2;
	}
	else
	{
		CScanner s( lpString );
		s.GetToken();
		while( s.tok != FINISHED )
		{
			_tcscpy( argv[argc], s.Token );
			argc++;
			s.GetToken();
		}
	}
	
	for( int i = 0; i < argc; i++ )
	{
		pargv[ i ] = argv[ i ];
	}
	
	CString string;
	string.FormatV( prj.GetText( dwText ), (va_list) pargv );

	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	if( pWndWorld )
	{
		if( bSmall )
		{
			if( ::GetLanguage() != LANG_JAP )
				g_Caption1.AddCaption( string, pWndWorld->m_pFontAPICaption );// CWndBase::m_Theme.m_pFontCaption );
			else
				g_Caption1.AddCaption( string, NULL );// CWndBase::m_Theme.m_pFontCaption );
		}
		else
		{
			if( ::GetLanguage() != LANG_JAP )
				g_Caption1.AddCaption( string, pWndWorld->m_pFontAPITitle );// CWndBase::m_Theme.m_pFontCaption );
			else
				g_Caption1.AddCaption( string, NULL );// CWndBase::m_Theme.m_pFontCaption );
		}
	}	
}

void CDPClient::OnModifyMode( OBJID objid, CAr & ar )
{
	DWORD dwMode;
	ar >> dwMode;

	CMover* pPlayer	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPlayer ) ) {
		pPlayer->m_dwMode	= dwMode;
		if( pPlayer->IsActiveMover() && g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) ) // 여기서 나면 출력
		{
			CString str;
			if( pPlayer->m_dwMode & MATCHLESS_MODE )
			{
				str += prj.GetText(TID_ADMIN_IMMORTAL);//"무적 ";
			}
			if( pPlayer->m_dwMode & MATCHLESS2_MODE )
			{
				str += prj.GetText(TID_ADMIN_IMMORTALHALF);//"반무적 ";
			}
			if( pPlayer->IsMode( TRANSPARENT_MODE ) )
			{
				str += prj.GetText(TID_ADMIN_INVISIBLE);//"투명 ";
			}
			if( pPlayer->IsMode( OBSERVE_MODE ) )
			{
				str += "관전";
			}
			if( pPlayer->IsMode( ITEM_MODE ) )
			{
				str += "아이템";
			}
			if( pPlayer->IsMode( NO_ATTACK_MODE ) )
			{
				str += "공격";
			}
			if( pPlayer->IsMode( COMMUNITY_MODE ) )
			{
				str += "커뮤니티";
			}
			if( pPlayer->m_dwMode & ONEKILL_MODE )
			{
				str += prj.GetText(TID_ADMIN_MIGHTY);//"초필 ";
			}

			if( pPlayer->IsMode( SAYTALK_MODE ) )
			{
				str += "귓속말할수 없는";
			}
			if( pPlayer->IsMode( TALK_MODE ) )
			{
				str += "채팅할수 없는";
			}
			if( pPlayer->IsMode( SHOUTTALK_MODE ) )
			{
				str += "외치기할수 없는";
			}
			if( pPlayer->m_dwMode & DONMOVE_MODE )
			{
				str += prj.GetText(TID_ADMIN_NOMOVING);//"움직일수 없는 ";
			}
			if( pPlayer->IsMode( FREEPK_MODE ) )
			{
				str += "PK ";
			}
			if( pPlayer->IsMode( PVPCONFIRM_MODE ) )
			{
				str += "PVP ";				
			}
			if( pPlayer->m_dwMode != 0 )
			{
				str += prj.GetText(TID_ADMIN_STATE);//"상태 ";
			}
			else
			{
				str += prj.GetText(TID_ADMIN_NORMAL);//"평민 상태";
			}

			g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_ADMIN_NORMAL) );
		}

		if( pPlayer->IsActiveMover() )
		{
			if( pPlayer->IsMode( MODE_MAILBOX ) )
			{
				CString str;
				str += prj.GetText(TID_MAIL_RECEIVE);
				
				CWndInstantMsg* pWndInstantMsg = g_WndMng.OpenInstantMsg( "Mail" );
				if( pWndInstantMsg )
				{
					pWndInstantMsg->AddPostMessage(prj.GetText(TID_MAIL_RECEIVE));
					pWndInstantMsg->m_timer.Set( MIN(1) );		//chipi_080808 - 우편 메세지 창 유지 1분으로 변경 
				}
				
				g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_ADMIN_NORMAL) );

				//MAIL LOG
				Error( "CDPClient::OnModifyMode  Mode:%d", dwMode ); 
			}
		}
	}
}

void CDPClient::OnStateMode( OBJID objid, CAr & ar )
{
	DWORD dwStateMode, dwOldStateMode;
	BYTE nFlag;
	OBJID dwItemId;
	ar >> dwStateMode;
	ar >> nFlag;
	if( nFlag == STATEMODE_BASEMOTION_ON )
	{
		ar >> dwItemId;
	}
	
	CMover* pPlayer	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPlayer ) )
	{
		if( nFlag == STATEMODE_BASEMOTION_ON )
		{
			ItemProp* pItemProp = prj.GetItemProp( dwItemId );
			if( pItemProp )
			{
				CreateItemReadySfx( pItemProp->dwSfxObj, pPlayer->GetPos(), pPlayer->GetId(), 
					pPlayer->GetPos(), pPlayer->GetId(), -1 ); 
			}
		}

		dwOldStateMode = pPlayer->m_dwStateMode;
		pPlayer->m_dwStateMode	= dwStateMode;
		if( pPlayer->IsActiveMover() ) // 여기서 나면 출력
		{
			CString strMessage;	// 메세지 처리
			switch( nFlag )
			{
			case STATEMODE_BASEMOTION_ON:
				{
					g_pPlayer->m_nReadyTime = 0;
					g_pPlayer->m_dwUseItemId = dwItemId;
					g_pPlayer->m_nReadyTime = timeGetTime();
//					g_WndMng.PutString( strMessage, NULL, prj.GetTextColor( TID_GAME_PARTYNOINVATE ) );	
					if( g_pPlayer->m_pActMover->IsState( OBJSTA_STAND ) == FALSE )
					{
						SendStateModeCancel( STATE_BASEMOTION_MODE, STATEMODE_BASEMOTION_CANCEL );
					}
				}
				break;
			case STATEMODE_BASEMOTION_OFF:
				{
					g_pPlayer->m_nReadyTime = 0;
//					g_WndMng.PutString( strMessage, NULL, prj.GetTextColor( TID_GAME_PARTYNOINVATE ) );
				}
				break;
			case STATEMODE_BASEMOTION_CANCEL:
				{
					g_pPlayer->m_nReadyTime = 0;
//					g_WndMng.PutString( strMessage, NULL, prj.GetTextColor( TID_GAME_PARTYNOINVATE ) );
				}
				break;
			}
		}
	}
}

void CDPClient::OnReturnSay( OBJID objid, CAr & ar )
{
	int ReturnFlag;
	CHAR	lpString[260];
	
	ar >> ReturnFlag;
	ar.ReadString( lpString, 260 );
	CMover* pPlayer = prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPlayer ) )
	{
		CString Str;
		if( ReturnFlag == 1 )				// 보낸데이터
		{
			Str.Format( "[%s] %s", pPlayer->GetName(), lpString );
		}
		else if( ReturnFlag == 2 )			// 자기자신
		{
			//Str += "자기자신에게는 보낼수가 없습니다";
			Str += prj.GetText(TID_GAME_MSGNOTTRANSELF);
		}
		else								// 찾을수가 없음
		{
			//Str.Format( "[%s]님은 존재하지 않는 ID입니다", lpString );
			Str.Format( prj.GetText(TID_GAME_MSGNOTEXIST), lpString );
		}
		//g_WndMng.PutString( Str, NULL, 0xffffff00 );
		g_WndMng.PutString( Str, NULL, prj.GetTextColor(TID_GAME_MSGNOTEXIST) );
	}
}

void CDPClient::SendGuildInvite( OBJID objid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_INVITE, DPID_UNKNOWN );
	ar << objid;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendIgnoreGuildInvite( u_long idPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_IGNORE_GUILD_INVITE, DPID_UNKNOWN );
	ar << idPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendCreateGuild( const char* szGuild )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CREATE_GUILD, DPID_UNKNOWN );
	ar.WriteString( szGuild );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendDestroyGuild() {
	SendPacket<PACKETTYPE_DESTROY_GUILD>();
}

void CDPClient::SendRemoveGuildMember( u_long idMaster, u_long idPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_REMOVE_GUILD_MEMBER, DPID_UNKNOWN );
	ar << idMaster << idPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnCreateGuild( CAr & ar )
{
	u_long idPlayer, idGuild;
	char szGuild[MAX_G_NAME];
	char lpszPlayer[MAX_PLAYER];
	ar >> idPlayer >> idGuild;
	ar.ReadString( lpszPlayer, MAX_PLAYER );
	ar.ReadString( szGuild, MAX_G_NAME );

	CPlayerDataCenter::GetInstance()->AddPlayerData( idPlayer, lpszPlayer );

	CGuild* pGuild	= new CGuild;
	pGuild->m_idGuild	= idGuild;
	lstrcpy( pGuild->m_szGuild, szGuild );
	pGuild->m_idMaster		= idPlayer;
	pGuild->m_aPower[GUD_MASTER].SetAll();
	idGuild		= g_GuildMng.AddGuild( pGuild );
	if( idGuild > 0 )
	{
		CGuildMember* pMember	= new CGuildMember;
		pMember->m_idPlayer	= idPlayer;
		pMember->m_nMemberLv	= GUD_MASTER;
		if( pGuild->AddMember( pMember ) )
		{
			CMover* pMover	= prj.GetUserByID( idPlayer );
			if( pMover )
			{
				pMover->m_idGuild	= idGuild;
				if( pMover->IsActiveMover() )
				{
					// CWndX
				}
			}
			CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
			if( pGuild )
			{
				CString str;
				str.Format( prj.GetText( TID_GAME_COMCREATE ), lpszPlayer, pGuild->m_szGuild );
				g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_COMCREATE ) );

				if(g_pPlayer->m_idPlayer == idPlayer)
				{
					CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
				}
			}
		}
		else
		{
			SAFE_DELETE( pMember );
			g_GuildMng.RemoveGuild( idGuild );
		}
	}
	else
	{
		SAFE_DELETE( pGuild );
	}
}

void CDPClient::OnDestroyGuild( CAr & ar )
{
	char lpszPlayer[MAX_PLAYER];
	u_long idGuild;
	ar.ReadString( lpszPlayer, MAX_PLAYER );
	ar >> idGuild;

	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( pGuild )
	{
		CString str;
		str.Format( prj.GetText( TID_GAME_COMDISMESS ), lpszPlayer, pGuild->m_szGuild );
		if( idGuild == g_pPlayer->m_idGuild )
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_COMDISMESS ) );
		g_GuildMng.RemoveGuild( idGuild );

		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabApp.UpdateData();
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabMember.UpdateData();
			pWndGuild->m_WndGuildTabWar.UpdateData();
		}

		CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
	}
}

void CDPClient::OnGuild( CAr & ar )
{
	u_long idGuild;
	ar >> idGuild;
	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( !pGuild )
	{
		pGuild	= new CGuild;
		pGuild->Serialize( ar, FALSE );
		g_GuildMng.AddGuild( pGuild );
	}
	else
	{
		pGuild->Serialize( ar, FALSE );
	}
	
	CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
	if( pWndGuild )
	{
		pWndGuild->m_WndGuildTabApp.UpdateData();
		pWndGuild->m_WndGuildTabInfo.UpdateData();
		pWndGuild->m_WndGuildTabMember.UpdateData();
		pWndGuild->m_WndGuildTabWar.UpdateData();
	}

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);

	if( pGuild )
	{
		std::vector<PDVer>	aPDVer;
		CGuildMember* pMember;
		for( auto i = pGuild->m_mapPMember.begin(); i != pGuild->m_mapPMember.end(); ++i )
		{
			pMember		= i->second;
			aPDVer.emplace_back(CPlayerDataCenter::GetInstance()->ToPDVer(pMember->m_idPlayer));
		}
		SendQueryPlayerData( aPDVer );
	}
}


void CDPClient::OnSetGuild( OBJID objid, CAr & ar )
{
	u_long idGuild;
	ar >> idGuild;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) ) 
	{
		u_long idGuildOld		= pMover->m_idGuild;
		pMover->m_idGuild	= idGuild;
		if( g_pPlayer && g_pPlayer->GetId() == objid )
		{
			if( idGuild == 0 )
			{
				CGuild* pGuild	= g_GuildMng.GetGuild( idGuildOld );
				if( pGuild )
				{
					CString str;
					str.Format( prj.GetText( TID_GAME_COMLEAVE ), pGuild->m_szGuild );
					g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_COMLEAVE ) );
				}
				GuildHouse->Clear();
			}
			else
			{
				CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
				if( pGuild )
				{
					PlayMusic( BGM_IN_COMPANY, 1 );
					CString str;
					str.Format( prj.GetText( TID_GAME_COMACCEPT ), pGuild->m_szGuild );
					g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_COMACCEPT ) );
					
					CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
					if( pWndGuild )
					{
						pWndGuild->m_WndGuildTabApp.UpdateData();
						pWndGuild->m_WndGuildTabInfo.UpdateData();
						pWndGuild->m_WndGuildTabMember.UpdateData();
						pWndGuild->m_WndGuildTabWar.UpdateData();
					}

					CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Any);
				}
			}
//			CWndX
		}
	}
}

void CDPClient::OnAddGuildMember( CAr & ar )
{
//	u_long idPlayer, idGuild;
	u_long idGuild;
	GUILD_MEMBER_INFO info;
	char lpszPlayer[MAX_PLAYER];
//	ar >> idPlayer >> idGuild;
	ar.Read( &info, sizeof(GUILD_MEMBER_INFO) );
	ar >> idGuild;
	ar.ReadString( lpszPlayer, MAX_PLAYER );


	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( pGuild )
	{
		CGuildMember* pMember	= new CGuildMember;
		pMember->m_idPlayer	= info.idPlayer;
		pMember->m_nMemberLv	= GUD_ROOKIE;
		if( pGuild->AddMember( pMember ) )
		{
			CString str;
			str.Format( prj.GetText( TID_GAME_COMACCEPTWELCOME ), lpszPlayer );
			g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_COMACCEPTWELCOME ) );
//			CWndX
		}
		
		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabMember.UpdateData();
		}
		if(g_pPlayer->m_idGuild == idGuild)
		{
			CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
		}
	}
}

void CDPClient::OnRemoveGuildMember( CAr & ar )
{
	u_long idPlayer, idGuild;
	ar >> idPlayer >> idGuild;
	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( pGuild )
	{
		if( pGuild->RemoveMember( idPlayer ) )
		{
			LPCSTR lpszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( idPlayer );
			if( lpszPlayer )
			{
				CString str;
				str.Format( prj.GetText( TID_GAME_COMLEAVEBYE ), lpszPlayer );
				g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_COMLEAVEBYE ) );
			}
//			CWndX
			
			CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
			if( pWndGuild )
			{
				pWndGuild->m_WndGuildTabApp.UpdateData();
				pWndGuild->m_WndGuildTabInfo.UpdateData();
				pWndGuild->m_WndGuildTabMember.UpdateData();
				pWndGuild->m_WndGuildTabWar.UpdateData();
			}
			
			CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
		}
	}
}

void CDPClient::OnGuildChat( CAr & ar )
{
	char sChat[1024];
	char sPlayer[MAX_PLAYER]	= { 0, };
	OBJID objid;
	ar >> objid;
	
	ar.ReadString( sPlayer, MAX_PLAYER );
	ar.ReadString( sChat, 1024 );

#ifdef __YS_CHATTING_BLOCKING_SYSTEM
	CMover* pMover = prj.GetMover( objid );
	if( pMover && pMover->IsPlayer() == TRUE )
	{
		auto BlockedUserIterator = prj.m_setBlockedUserID.find( pMover->GetName( TRUE ) );
		if( BlockedUserIterator != prj.m_setBlockedUserID.end() )
			return;
	}
#endif // __YS_CHATTING_BLOCKING_SYSTEM

	CString str;
	str.Format( "[%s] %s", sPlayer, sChat );
	g_WndMng.PutString( str, NULL, prj.GetTextColor( TID_GUILD_CHAT ), CHATSTY_GUILD );
	
#ifndef __YS_CHATTING_BLOCKING_SYSTEM
	CMover* pMover	= prj.GetMover( objid );
#endif // __YS_CHATTING_BLOCKING_SYSTEM
	if( IsValidObj( pMover ) ) 
	{
		if( strlen( sChat ) )
			g_DialogMsg.AddMessage( pMover, sChat, 0xfffffff, CHAT_GUILD );
	}
}

void CDPClient::OnGuildMemberLv( CAr & ar )
{
	u_long idPlayer;
	int nMemberLv;
	ar >> idPlayer >> nMemberLv;

	CGuild* pGuild	= g_pPlayer->GetGuild();
	if( pGuild && pGuild->IsMember( idPlayer ) )
	{
		TRACE( "OnGuildMemberLv\n" );
		CGuildMember* pMember	= pGuild->GetMember( idPlayer );
		
		CString str;
		if( pMember->m_nMemberLv < nMemberLv )
		{
			if( pMember->GetMover() )
				str.Format( prj.GetText(TID_GAME_GUILDMEM_APPUP),  pMember->GetMover()->GetName() );
		}
		else
		{
			if( pMember->GetMover() )
				str.Format( prj.GetText(TID_GAME_GUILDMEM_APPDOWN),  pMember->GetMover()->GetName() );
		}

		pMember->m_nMemberLv	= nMemberLv;

		pMember->m_nClass = 0;

		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabMember.UpdateData();
		}

		CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
	}
}

void CDPClient::OnGuildClass( CAr & ar )
{
	u_long idPlayer;
	int nClass;
	ar >> idPlayer >> nClass;
	CGuild* pGuild	= g_pPlayer->GetGuild();
	if( pGuild && pGuild->IsMember( idPlayer ) )
	{
		TRACE( "OnGuildClass\n" );
		CGuildMember* pMember	= pGuild->GetMember( idPlayer );
		
		if( pMember )
			pMember->m_nClass = nClass;
		
		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabMember.UpdateData();
		}
	}
}

void CDPClient::OnChgMaster( CAr & ar )
{
	u_long idPlayer, idPlayer2;
	ar >> idPlayer >> idPlayer2;
	CGuild* pGuild	= g_pPlayer->GetGuild();
	if( pGuild && pGuild->IsMember( idPlayer ) && pGuild->IsMember( idPlayer2 ) )
	{
		TRACE( "OnChgMaster\n" );
		pGuild->m_idMaster	= idPlayer2;

		CGuildMember* pMember	= pGuild->GetMember( idPlayer );
		if( pMember )
		{
			pMember->m_nMemberLv	= GUD_ROOKIE;
			pMember->m_nClass = 0;
		}

		pMember	= pGuild->GetMember( idPlayer2 );
		if( pMember )
		{
			pMember->m_nMemberLv	= GUD_MASTER;
			pMember->m_nClass = 0;
		}

		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabMember.UpdateData();
		}

		if( pMember->GetMover() )
		{
			CString szMsg;
			szMsg.Format( prj.GetText(TID_GAME_CHANGE_GUILDMASTER), pMember->GetMover()->GetName() );
			g_WndMng.PutString( szMsg, NULL, prj.GetTextColor( TID_GAME_CHANGE_GUILDMASTER ) );
		}
		// output
	}
}

void CDPClient::OnGuildNickName( CAr & ar )
{
	u_long idPlayer;
	char strNickName[MAX_G_NAME] = {0, };
	ar >> idPlayer;
	ar.ReadString( strNickName, MAX_G_NAME );
	CGuild* pGuild	= g_pPlayer->GetGuild();
	if( pGuild && pGuild->IsMember( idPlayer ) )
	{
		TRACE( "OnGuildClass\n" );
		CGuildMember* pMember	= pGuild->GetMember( idPlayer );
		
		strcpy( pMember->m_szAlias, strNickName );
		
		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabMember.UpdateData();
		}
	}	
}


void CDPClient::OnGuildMemberLogin( CAr & ar )
{
	BYTE nLogin;
	u_long uPlayerId, uMultiNo;
	ar >> nLogin >> uPlayerId >> uMultiNo;

	if( g_pPlayer )
	{
		CGuild* pGuild = g_pPlayer->GetGuild();
		if( pGuild && pGuild->IsMember( uPlayerId ) )
		{
			TRACE( "OnGuildMemberLogin\n" );
			CGuildMember* pMember	= pGuild->GetMember( uPlayerId );
//			PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( uPlayerId );
//			pPlayerData->data.uLogin	= uMultiNo;

			CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
			if( pWndGuild )
				pWndGuild->m_WndGuildTabMember.UpdateData();
			
			CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
		}
	}
}

void CDPClient::OnGuldMyGameJoin( CAr & ar )
{
	int		nMaxLoginGuild;
	u_long  uLoginPlayerIdGuild[ 200 ];
	u_long  uLoginGuildMulti[ 200 ];

	nMaxLoginGuild = 0;
	memset( uLoginPlayerIdGuild, 0, sizeof(uLoginPlayerIdGuild) );
	memset( uLoginGuildMulti, 0, sizeof(uLoginGuildMulti) );

	ar >> nMaxLoginGuild;
	ar.Read( uLoginPlayerIdGuild, sizeof( u_long ) * nMaxLoginGuild );
	ar.Read( uLoginGuildMulti, sizeof( u_long ) * nMaxLoginGuild );
	BOOL bGetInfo = FALSE;
	if( g_pPlayer )
	{
		CGuild* pGuild = g_pPlayer->GetGuild();
		if( pGuild )
		{
			bGetInfo = TRUE;
			CGuildMember* pGuildMember;
			for( int i = 0 ; i < nMaxLoginGuild ; ++i )
			{
				pGuildMember = pGuild->GetMember( uLoginPlayerIdGuild[ i ] );
				if( pGuildMember )
				{
//					PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( pGuildMember->m_idPlayer );
//					pPlayerData->data.uLogin	= uLoginGuildMulti[i];
				}
			}

			CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
			if( pWndGuild )
			{
				pWndGuild->m_WndGuildTabInfo.UpdateData();
				pWndGuild->m_WndGuildTabMember.UpdateData();
			}
			
			CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Guild);
		}
	}

	if( bGetInfo == FALSE )
	{
		m_nMaxLoginGuild = nMaxLoginGuild;
		memcpy( m_uLoginPlayerIdGuild, uLoginPlayerIdGuild, sizeof(uLoginPlayerIdGuild) );
		memcpy( m_uLoginGuildMulti, uLoginGuildMulti, sizeof(uLoginGuildMulti) );
	}
}

void CDPClient::OnGuildError( CAr & ar )
{
	int nError;
	ar >> nError;

	switch( nError )
	{
	case 1:
		{
			g_WndMng.PutString( prj.GetText(TID_GAME_COMOVERLAPNAME), NULL, prj.GetTextColor( TID_GAME_COMOVERLAPNAME ) );
			break;
		}
	case 2:
		{
			g_WndMng.PutString( prj.GetText(TID_GAME_GUILDONLYNUMBER), NULL, prj.GetTextColor( TID_GAME_GUILDONLYNUMBER ) );
			break;			
		}
	default:
		break;
	}
}

void CDPClient::OnGuildInvite( CAr & ar ) {
	const auto [idGuild, idMaster] = ar.Extract<u_long, u_long>();

#ifdef __S_SERVER_UNIFY
	if (!g_WndMng.m_bAllAction) return;
#endif // __S_SERVER_UNIFY

	CGuild * pGuild = g_GuildMng.GetGuild(idGuild);
	if (!pGuild) return;

	CWndGuildConfirm::OpenWindow(idMaster, *pGuild);
}

void CDPClient::OnAllGuilds( CAr & ar )
{
	g_GuildMng.Serialize( ar, TRUE );
}

// 길드 멤버 호칭
void CDPClient::SendGuildMemberLv( u_long idMaster, u_long idPlayer, int nMemberLv )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_MEMBER_LEVEL, DPID_UNKNOWN );
	ar << idMaster << idPlayer << nMemberLv;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGuildClass( u_long idMaster, u_long idPlayer, BYTE nFlag )
{
	if( idMaster != idPlayer )
	{
		BEFORESENDSOLE( ar, PACKETTYPE_GUILD_CLASS, DPID_UNKNOWN );
		ar << nFlag;		// UP : 1, DOWN : 0
		ar << idMaster << idPlayer;
		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::SendGuildNickName( u_long idPlayer, LPCTSTR strNickName )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_NICKNAME, DPID_UNKNOWN );
	ar << g_pPlayer->m_idPlayer << idPlayer;
	ar.WriteString( strNickName );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 길드로고를 변경한다.
void CDPClient::SendGuildLogo( DWORD dwLogo )
{
	BEFORESENDSOLE( ar, PACKETTYPE_NW_GUILDLOGO, DPID_UNKNOWN );
	ar << dwLogo;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 길드에 PXP나 페냐를 공헌한다.
// PXP를 공헌하는 경우 - cbPxpCount = 1, nGold = 0
// 페냐를 공헌하는 경우 - cbPxpCount = 0, nGold = 공헌량 
// 아이템을 공헌하는 경우 - cbItemFlag = 1
//void CDPClient::SendGuildContribution( BYTE cbPxpCount, int nGold, BYTE cbItemID, BYTE cbItemNum )
void CDPClient::SendGuildContribution( BYTE cbPxpCount, int nGold, BYTE cbItemFlag )
{
	ASSERT( cbPxpCount == 0 || cbPxpCount == 1 );
	ASSERT( nGold >= 0 );

	BEFORESENDSOLE( ar, PACKETTYPE_NW_GUILDCONTRIBUTION, DPID_UNKNOWN );
	ar << cbPxpCount << nGold;
	ar << cbItemFlag;

	SEND( ar, this, DPID_SERVERPLAYER );
}

// 길드 공지사항을 변경한다.
void CDPClient::SendGuildNotice( const char* szNotice )
{
	ASSERT( szNotice );

	BEFORESENDSOLE( ar, PACKETTYPE_NW_GUILDNOTICE, DPID_UNKNOWN );
	ar.WriteString( szNotice );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 길드 권한설정 변경
void CDPClient::SendGuildAuthority(const GuildPowerss & powers) {
	SendPacket<PACKETTYPE_GUILD_AUTHORITY, GuildPowerss>(powers);
}

void CDPClient::SendGuilPenya( u_long, DWORD dwType, DWORD dwSendPenya )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILD_PENYA, DPID_UNKNOWN );
	ar << dwType << dwSendPenya;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGuildSetName(LPCTSTR szName) {
	BEFORESENDSOLE(ar, PACKETTYPE_GUILD_SETNAME, DPID_UNKNOWN);
	ar.WriteString(szName);
	SEND(ar, this, DPID_SERVERPLAYER);
}

void CDPClient::SendGuildRank( DWORD nVer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_REQUEST_GUILD_RANKING, DPID_UNKNOWN );
	ar << nVer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendChgMaster( DWORD idPlayer, u_long idPlayer2 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHG_MASTER, DPID_UNKNOWN );
	ar << idPlayer << idPlayer2;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 길드로고가 변경된 경우 호출된다.
void CDPClient::OnGuildLogo( CAr & ar )
{
	DWORD dwLogo;
	u_long idGuild;
	ar >> idGuild >> dwLogo;

	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( pGuild == NULL )
		return;

	pGuild->m_dwLogo = dwLogo;
}

void CDPClient::UpdateGuildWnd()
{
	CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
	if( pWndGuild )
	{
		pWndGuild->m_WndGuildTabInfo.UpdateData();
		pWndGuild->m_WndGuildTabMember.UpdateData();
	}	 
}

// 공헌에 의해서 공헌도와 레벨이 변경된 경우 호출된다.
void CDPClient::OnGuildContribution( CAr & ar )
{
	CONTRIBUTION_CHANGED_INFO info;
	ar >> info;

	CGuild* pGuild	= g_GuildMng.GetGuild( info.idGuild );
	if( pGuild == NULL )
		return;

	if( info.nGuildLevel > pGuild->m_nLevel )
	{
		g_WndMng.PutString( prj.GetText(TID_GAME_GUILDLEVELUP), NULL, prj.GetTextColor(TID_GAME_GUILDLEVELUP) );
		//g_WndMng.PutString( "길드의 레벨이 올랐습니다.", NULL, 0xff99cc00 );
	}
	
	pGuild->SetContribution( info );
	UpdateGuildWnd();
}

// 투표추가 응답 
void CDPClient::OnGuildAddVote( CAr & ar )
{
	VOTE_INSERTED_INFO info;
	ar >> info;

	CGuild* pGuild	= g_GuildMng.GetGuild( info.idGuild );
	if( pGuild == NULL )
		return;
	
	if( info.idVote )
	{
		// 투표추가 성공 
		pGuild->AddVote( info );
		
		CWndGuildVote* pWndGuildVote = (CWndGuildVote*)g_WndMng.GetWndBase( APP_GUILD_VOTE );
		if( pWndGuildVote )
		{
			CWndComboBox* pCombo = (CWndComboBox*)pWndGuildVote->GetDlgItem(WIDC_COMBOBOX1);
			pCombo->ResetContent();

			for (const CGuildVote * guildVote : pGuild->m_votes) {
				auto & voteListItem = pCombo->AddString(guildVote->GetTitle());
				voteListItem.m_dwData = guildVote->GetID();
			}

			const int lastId = static_cast<int>(pGuild->m_votes.size()) - 1;

			pCombo->SetCurSel(lastId);
			pWndGuildVote->SelChange( pGuild, lastId);
		}	 
	}
	else
	{
		// 투표추가 에러. ( 마스터만 이 응답 패킷을 받는다. )
	}
}

// 투표삭제, 종료, 투표하기 응답 
void CDPClient::OnGuildModifyVote( CAr & ar )
{
	u_long idVote;
	BYTE cbOperation;
	BYTE cbExtra;

	ar >> idVote;
	ar >> cbOperation;
	ar >> cbExtra;

	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild == NULL )
		return;

	pGuild->ModifyVote( idVote, cbOperation, cbExtra );
	
	CGuildVote* pGuildVote = pGuild->FindVote( idVote );

	CWndGuildVote* pWndGuildVote = (CWndGuildVote*)g_WndMng.GetWndBase(APP_GUILD_VOTE);

	if( pWndGuildVote )
	{
		pWndGuildVote->SetCount( pGuildVote->GetVoteSelect(0).cbCount, pGuildVote->GetVoteSelect(1).cbCount, 
			pGuildVote->GetVoteSelect(2).cbCount, pGuildVote->GetVoteSelect(3).cbCount );
	}
}

// 공지사항을 변경하면 호출된다.
void CDPClient::OnGuildNotice( CAr & ar )
{
	u_long idGuild;
	char szNotice[MAX_BYTE_NOTICE];
	ar >> idGuild;
	ar.ReadString( szNotice, MAX_BYTE_NOTICE );

	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( pGuild == NULL )
		return;

	pGuild->SetNotice( szNotice );

	// 길드의 공지사항이 업데이트 되었습니다. 길드창에서 확인해주세요.
	g_WndMng.PutString( prj.GetText( TID_GAME_GUILDNOTICEUPDATE ), NULL, prj.GetTextColor( TID_GAME_GUILDNOTICEUPDATE ) );

	CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
	if( pWndGuild )
		pWndGuild->m_WndGuildTabInfo.UpdateData();
}

void CDPClient::OnGuildAuthority(CAr & ar) {
	const auto [dwAuthority] = ar.Extract<GuildPowerss>();
	CGuild * pGuild = g_pPlayer->GetGuild();
	if (!pGuild) return;

	pGuild->m_aPower = dwAuthority;

	if (CWndGuild * pWndGuild = g_WndMng.GetWndBase<CWndGuild>(APP_GUILD)) {
		pWndGuild->m_WndGuildTabApp.UpdateData();
	}

	g_WndMng.PutString(TID_GAME_GUILDCHGWARRANT);
}

void CDPClient::OnGuildPenya( CAr & ar )
{
	DWORD dwType, dwPenya; ar >> dwType >> dwPenya;

	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
	{
		pGuild->m_aPenya[dwType] = dwPenya;
		
		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
		{
			pWndGuild->m_WndGuildTabInfo.UpdateData();
			pWndGuild->m_WndGuildTabApp.SetPenya();
		}

		g_WndMng.PutString(TID_GAME_GUILDCHGPAY);
	}
}

void CDPClient::OnGuildRealPenya(CAr & ar) {
	const auto [nGoldGuild, nType] = ar.Extract<int, int>();

	CGuild * pGuild = g_pPlayer->GetGuild();
	if (!pGuild) return;

	pGuild->m_nGoldGuild = nGoldGuild;

	if (CWndGuild * pWndGuild = g_WndMng.GetWndBase<CWndGuild>(APP_GUILD)) {
		pWndGuild->m_WndGuildTabInfo.UpdateData();
	}

	if (pGuild->m_aPenya[nType] > 0) {
		g_WndMng.PutString(TID_GAME_GUILD_PAYTAKE, pGuild->m_aPenya[nType]);
	}
}

void CDPClient::OnGuildSetName( CAr & ar )
{
	u_long idGuild;
	char szName[ MAX_G_NAME ];
	ar >> idGuild;
	ar.ReadString( szName, MAX_G_NAME );

	CGuild* pGuild = g_GuildMng.GetGuild( idGuild );
	if( pGuild )
	{
		CString str;

		str.Format( prj.GetText(TID_GAME_GUILDNAMECHG), pGuild->m_szGuild, szName );

		g_GuildMng.SetName( pGuild, szName );

		g_WndMng.PutString( str );
		
		CWndGuild* pWndGuild = (CWndGuild*)g_WndMng.GetWndBase( APP_GUILD );
		if( pWndGuild )
			pWndGuild->m_WndGuildTabInfo.UpdateData();
	}		
}

void CDPClient::OnGuildRank(CAr & ar) {
	ar >> CGuildRank::Instance;
}

void CDPClient::SendQueryPlayerData(u_long idPlayer) {
	SendPacket<PACKETTYPE_QUERY_PLAYER_DATA, u_long>(idPlayer);
}

void CDPClient::SendQueryPlayerData( const std::vector<PDVer>& vecPlayer )
{
	if (vecPlayer.empty()) return;
	BEFORESENDSOLE( ar, PACKETTYPE_QUERY_PLAYER_DATA2, DPID_UNKNOWN );
	ar << static_cast<std::uint32_t>(vecPlayer.size());
	for (const PDVer & pdVer : vecPlayer) {
		ar << pdVer;
	}
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::OnSetState( CAr & ar )
{
	LONG nRemainLP;		// 사용하지 않음 
	ar >> g_pPlayer->m_nStr >> g_pPlayer->m_nSta >> g_pPlayer->m_nDex >> g_pPlayer->m_nInt;
	ar >> nRemainLP >> g_pPlayer->m_nRemainGP;
	// 서버와 동기화
	g_pPlayer->SetHitPoint( g_pPlayer->GetMaxHitPoint() );
	g_pPlayer->SetManaPoint( g_pPlayer->GetMaxManaPoint() );
	g_pPlayer->SetFatiguePoint( g_pPlayer->GetMaxFatiguePoint() );
}

void CDPClient::OnCmdSetSkillLevel( CAr & ar )
{
	DWORD dwSkill;
	DWORD dwSkillLevel;
	ar >> dwSkill >> dwSkillLevel;

	SKILL * pSkill = g_pPlayer->GetSkill(dwSkill);
	if (!pSkill) return;

	pSkill->dwLevel = dwSkillLevel;
	ItemProp* pSkillProp = prj.GetSkillProp( pSkill->dwSkill );
	if (pSkillProp) {
		g_WndMng.PutString(TID_GAME_SKILLLEVELUP, pSkillProp->szName, pSkill->dwLevel);
	}
		
	g_pPlayer->PutLvUpSkillName_2(dwSkill);

	CWndSkillTreeCommon::ReInitIfOpen();

	CWndQuestDetail* pWndQuestDetail = g_WndMng.m_pWndQuestDetail;
	if( pWndQuestDetail )
		pWndQuestDetail->UpdateQuestText();

	CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
	if( pWndQuestQuickInfo )
		pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
	
}

void CDPClient::OnQueryPlayerData( CAr & ar )
{
	u_long idPlayer;
	ar >> idPlayer;
	PlayerData pd;
	ar.ReadString( pd.szPlayer, MAX_PLAYER );
	ar.Read( &pd.data, sizeof(sPlayerData) );
	PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( idPlayer, FALSE );

	TRACE( "OnQueryPlayerData, o = %s, v = %d, l = %d, ----> v = %d, l = %d\n", 
		pPlayerData->szPlayer, pPlayerData->data.nVer, pPlayerData->data.uLogin,
		pd.data.nVer, pd.data.uLogin );

	if( pPlayerData->data.nVer < pd.data.nVer )
	{
		CPlayerDataCenter::GetInstance()->AddPlayerData( idPlayer, pd );
	}

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Any);
	UpdateGuildWnd();

}

void CDPClient::OnDisguise( OBJID objid, CAr & ar )
{
	DWORD dwMoverIdx;
	ar >> dwMoverIdx;

	CMover* pMover =  prj.GetMover( objid );
	if( IsInvalidObj(pMover) )		
		return;

	if( pMover )
	{
		pMover->Disguise( dwMoverIdx );
		CWndQuestDetail* pWndQuestDetail = g_WndMng.m_pWndQuestDetail;
		if( pWndQuestDetail )
			pWndQuestDetail->UpdateQuestText();

		CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
		if( pWndQuestQuickInfo )
			pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
	}
}
void CDPClient::OnNoDisguise( OBJID objid, CAr & ar )
{
	CMover* pMover =  prj.GetMover( objid );
	if( IsInvalidObj(pMover) )		
		return;

	if( pMover )
	{
		pMover->NoDisguise( );
		CWndQuestDetail* pWndQuestDetail = g_WndMng.m_pWndQuestDetail;
		if( pWndQuestDetail )
			pWndQuestDetail->UpdateQuestText();

		CWndQuestQuickInfo* pWndQuestQuickInfo = g_WndMng.m_pWndQuestQuickInfo;
		if( pWndQuestQuickInfo )
			pWndQuestQuickInfo->SetUpdateTextSwitch( TRUE );
	}
}

void CDPClient::SendPVendorOpen( const char* szVendor )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PVENDOR_OPEN, DPID_UNKNOWN );
	ar.WriteString( szVendor );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendUnregisterPVendorItem( BYTE iIndex )
{
	BEFORESENDSOLE( ar, PACKETTYPE_UNREGISTER_PVENDOR_ITEM, DPID_UNKNOWN );
	ar << iIndex;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendPVendorClose( OBJID objidVendor )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PVENDOR_CLOSE, DPID_UNKNOWN );
	ar << objidVendor;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRegisterPVendorItem( BYTE iIndex, BYTE nType, BYTE nId, short nNum, int nCost )
{
	BEFORESENDSOLE( ar, PACKETTYPE_REGISTER_PVENDOR_ITEM, DPID_UNKNOWN );
	ar << iIndex << nType << nId << nNum << nCost;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendQueryPVendorItem( OBJID objidVendor )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERY_PVENDOR_ITEM, DPID_UNKNOWN );
	ar << objidVendor;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendBuyPVendorItem( OBJID objidVendor, BYTE nItem, DWORD dwItemId, short nNum )
{
	BEFORESENDSOLE( ar, PACKETTYPE_BUY_PVENDOR_ITEM, DPID_UNKNOWN );
	ar << objidVendor << nItem << dwItemId << nNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnPVendorOpen( OBJID objid, CAr & ar )
{
	char szPVendor[MAX_VENDORNAME] = {0, };
	ar.ReadString( szPVendor, MAX_VENDORNAME );

	CMover* pMover = prj.GetMover( objid );
	if( IsValidObj( pMover ) == FALSE )
		return;

	pMover->m_vtInfo.SetTitle( szPVendor );
	g_DialogMsg.AddVendorMessage( pMover, pMover->m_vtInfo.GetTitle(), 0xffffffff );

	if (!pMover->IsActiveMover()) return;

	CWndVendor* pWnd = (CWndVendor*)g_WndMng.GetWndVendorBase();
	if( pWnd == NULL )
		return;

	CWndEdit* pWndEdit	= (CWndEdit*)pWnd->GetDlgItem( WIDC_EDIT1 );
	pWndEdit->EnableWindow( FALSE );

	SendEnterChattingRoom( pMover->m_idPlayer );
		
	if( pWnd->m_pwndVenderMessage == NULL )
	{
		g_Chatting.m_bState = TRUE;

		pWnd->m_pwndVenderMessage = new CWndVendorMessage;
			if(pWnd->m_pVendor->IsActiveMover())
				pWnd->m_pwndVenderMessage->m_nIsOwner = TRUE;
#ifdef __FIX_WND_1109
		pWnd->m_pwndVenderMessage->Initialize( pWnd );
#else	// __FIX_WND_1109
		pWnd->m_pwndVenderMessage->Initialize( );
#endif	// __FIX_WND_1109
		pWnd->SetFocus();   // 개인상점창이 채팅창 보다 앞에 나오게 한다.(기획상)
	}

	CString strTitle = prj.GetText( TID_GAME_VENDOR_TITLE );
	strTitle += prj.GetText( TID_GAME_VENDOR_TITLE0 );
	pWnd->SetTitle( strTitle );
}

void CDPClient::OnPVendorClose( OBJID objid, CAr & ar )
{
	BYTE byClearTitle;
	ar >> byClearTitle;

	CMover* pMover = prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		if( pMover == g_pPlayer->m_vtInfo.GetOther() )
		{
			g_pPlayer->m_vtInfo.SetOther( NULL );
			CWndVendor* pWndVendor	= (CWndVendor*)g_WndMng.GetWndVendorBase();
			if( pWndVendor )
			{
				if( pWndVendor == CWndBase::m_GlobalShortcut.m_pFromWnd->GetFrameWnd() )
					CWndBase::m_GlobalShortcut.Empty();
				pWndVendor->Destroy();
			}
		}

		BOOL bReSetVendor = FALSE;
		CString strVendorTitle;

		if( pMover == g_pPlayer )
		{
			if( pMover->m_vtInfo.IsVendorOpen() )
			{
				bReSetVendor = TRUE;
				strVendorTitle = pMover->m_vtInfo.GetTitle();
			}
		}

		pMover->m_vtInfo.VendorClose( byClearTitle );
		if( byClearTitle )
			g_DialogMsg.RemoveVendorMessage( pMover );

		if( bReSetVendor )
		{
			CWndVendor* pWndVendor	= (CWndVendor*)g_WndMng.GetWndBase( APP_VENDOR_REVISION );
			if( !pWndVendor )
			{
				g_WndMng.CreateApplet( APP_VENDOR_REVISION );
				// 수정된 버젼에서는 개인상점 초기화시 일괄적으로 처리해준다
				pWndVendor = (CWndVendor*)g_WndMng.GetWndBase(APP_VENDOR_REVISION);
				if(pWndVendor)
				{
					CWndEdit* pWndEdit	= (CWndEdit*)pWndVendor->GetDlgItem( WIDC_EDIT1 );
					if( pWndEdit )
					{
						// 줄바꿈 스페이스 표기로 바꿔주기
						for(int j=0; j<strVendorTitle.GetLength(); j++)
						{
							if(strVendorTitle.GetAt(j) == '\n')
							{
								strVendorTitle.SetAt(j, ' ');
							}
						}
						pWndEdit->SetString( strVendorTitle );
					}
				}
				
			}
		}
	}
}

void CDPClient::OnUnregisterPVendorItem( OBJID objid, CAr & ar )
{
	BYTE iIndex;
	ar >> iIndex;

	CMover* pMover	= prj.GetMover( objid );
	if (IsValidObj(pMover) && pMover->IsActiveMover()) {
		pMover->m_vtInfo.VendorClearItem(iIndex);
	}

	g_Neuz.m_savedInven[iIndex].Clear();
}

void CDPClient::OnRegisterPVendorItem( OBJID objid, CAr & ar )
{
	BYTE iIndex, nType, nId;
	short nNum;
	int nCost;

	ar >> iIndex >> nType >> nId >> nNum >> nCost;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) && pMover->IsActiveMover() )
	{
		pMover->m_vtInfo.VendorSetItem( nId, iIndex, nNum, nCost );
	}
}

void CDPClient::OnPVendorItemNum( OBJID objid, CAr & ar )
{
	BYTE nItem;
	short nVend;
	ar >> nItem >> nVend;
	char sBuyer[MAX_PLAYER]	= { 0,};
	ar.ReadString( sBuyer, MAX_PLAYER );
	CMover* pPVendor	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pPVendor ) )
	{
		if( pPVendor->IsActiveMover() )
		{
			CItemElem* pItemElem	= pPVendor->m_vtInfo.GetItem( nItem );
			if( pItemElem )
			{
				CString strItem		= pItemElem->GetName();
				int nNum	= pItemElem->GetExtra() - nVend;
				int nTotal	= nNum * pItemElem->m_nCost;
				// sBuyer가 strItem을 nNum개 샀다.
				// nTotal을 벌었다.
				CWndVendor* pWndVendor = (CWndVendor*)g_WndMng.GetWndVendorBase();

				if( pWndVendor->m_pwndVenderMessage )
					pWndVendor->m_pwndVenderMessage->WriteBuyInfo(sBuyer, strItem, nNum, nTotal);
			}			
		}
		pPVendor->m_vtInfo.VendorItemNum( nItem, nVend );
	}

	g_Neuz.m_savedInven[nItem].Clear();
}

void CDPClient::OnPVendorItem( OBJID objid, CAr & ar )
{
	std::array<sqktd::maybe_owned_ptr<CItemElem>, MAX_VENDITEM> apItemVd = { nullptr };

	BYTE nVendorItem, iIndex;
	int nExtra;

	ar >> nVendorItem;
	for( int i = 0; i < nVendorItem; i++ )
	{
		CItemElem* pItemElem = new CItemElem;
		
		ar >> iIndex;
		ar >> *pItemElem;
		ar >> nExtra;
		ar >> pItemElem->m_nCost;
		
		pItemElem->SetExtra( nExtra );
		pItemElem->SetTexture();		
		apItemVd[iIndex] = pItemElem;
	}
	ar >> g_Chatting.m_bState;

	CMover* pPVendor	= prj.GetMover( objid );
	if( IsValidObj( pPVendor ) && pPVendor->IsPlayer() )
	{
		g_pPlayer->m_vtInfo.SetOther( pPVendor );
		pPVendor->m_vtInfo.VendorCopyItems(std::move(apItemVd));

		CWndVendor* pWndVendor	= (CWndVendor*)g_WndMng.CreateApplet( APP_VENDOR_REVISION );
		if( pWndVendor )
		{
			SendEnterChattingRoom( pPVendor->m_idPlayer );

			if( g_Chatting.m_bState )
			{
				pWndVendor->m_pwndVenderMessage = new CWndVendorMessage;
				pWndVendor->m_pwndVenderMessage->Initialize( pWndVendor );
			}

			pWndVendor->SetVendor( pPVendor );
		}			
	}
}

void CDPClient::OnAddFriendNameReqest( CAr & ar )
{
	u_long uLeader, uMember;
	LONG nLeaderJob;
	BYTE nLeaderSex;
	char szLeaderName[64] = {0,};
	ar >> uLeader >> uMember;
	ar >> nLeaderJob >> nLeaderSex;
	ar.ReadString( szLeaderName, 64 );

	if( g_pPlayer == NULL )
		return;

#ifdef __S_SERVER_UNIFY
	if( g_WndMng.m_bAllAction == FALSE )
		return;
#endif // __S_SERVER_UNIFY
	
	if( g_Option.m_bMessenger == FALSE )
	{
		SendBlock( 2, g_pPlayer->GetName(), szLeaderName );
		return;
	}

	if( g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
		return;

	g_WndMng.m_pWndFriendConfirm = new CWndFriendConFirm;
	g_WndMng.m_pWndFriendConfirm->Initialize();
	g_WndMng.m_pWndFriendConfirm->SetMember( uLeader, szLeaderName );
}

void CDPClient::SendSetHair( BYTE nHair, float r, float g, float b )//, int nCost )
{
	if(g_pPlayer->HasBuffByIk3(IK3_TEXT_DISGUISE))
	{
		g_WndMng.PutString( prj.GetText( TID_GAME_ERROR_BEAUTYSHOP_DISGUISE ), NULL, prj.GetTextColor( TID_GAME_ERROR_BEAUTYSHOP_DISGUISE  ) );
	}
	else
	{
		BYTE nR, nG, nB;
		nR	= (BYTE)( r * 255.0f );
		nG	= (BYTE)( g * 255.0f );
		nB	= (BYTE)( b * 255.0f );

		BEFORESENDSOLE( ar, PACKETTYPE_SET_HAIR, DPID_UNKNOWN );
		BOOL bUseCoupon = FALSE;
		if(g_WndMng.m_pWndBeautyShop != NULL)
		{
			bUseCoupon = g_WndMng.m_pWndBeautyShop->m_bUseCoupon;
		}
		ar << nHair << nR << nG << nB << bUseCoupon;
		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

void CDPClient::OnSetHair( OBJID objid, CAr & ar )
{
	BYTE nHair, nR, nG, nB;

	ar >> nHair >> nR >> nG >> nB;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) && pMover->IsPlayer() )
	{
		pMover->SetHair( nHair );
		float r, g, b;

		r	= (float)nR / 255.0f;
		g	= (float)nG / 255.0f;
		b	= (float)nB / 255.0f;

		pMover->SetHairColor( r, g, b );
		
		if( g_pPlayer )
		{
			if( g_pPlayer == pMover )
			{
				if( g_pBipedMesh )
					CMover::UpdateParts( g_pPlayer->GetSex(), g_pPlayer->m_skin, g_pPlayer->m_aEquipInfo, g_pBipedMesh, &g_pPlayer->m_Inventory );

				CWndInventory* pWndInventory	= (CWndInventory*)g_WndMng.GetWndBase( APP_INVENTORY );
				if(pWndInventory && pWndInventory->GetModel())
				{
					pWndInventory->UpdateParts();
				}
			}
		}

	}
}

void CDPClient::OnSetSkillState( CAr & ar )
{
	WORD wType, wID;
	DWORD dwLevel, dwTime;
	OBJID	idTarget;
	ar >> idTarget >> wType >> wID;
	if( wType == BUFF_ITEM2 )
	{
		time_t t;
		ar >> t;
		dwLevel		= time_null() + t;
	}
	else
		ar >> dwLevel;
	ar >> dwTime;

	CMover *pTarget = prj.GetMover( idTarget );
	if( IsValidObj( pTarget ) )
		pTarget->AddBuff( wType, wID, dwLevel, dwTime );
}

void CDPClient::SendCreateGuildCloak( void )
{
//	BEFORESENDSOLE( ar, PACKETTYPE_CREATE_GUILDCLOAK, DPID_UNKNOWN );
//	ar << pMover->GetId();
//	SEND( ar, this, DPID_SERVERPLAYER );
	
	SendHdr( PACKETTYPE_CREATE_GUILDCLOAK );
}


void CDPClient::SendMotion( DWORD dwMotion )
{
	if( g_pPlayer->m_dwReqFlag & REQ_MOTION )
		return;

	g_pPlayer->m_dwReqFlag	|= REQ_MOTION;

	BEFORESENDSOLE( ar, PACKETTYPE_MOTION, DPID_UNKNOWN );
	ar << dwMotion;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRepairItem( DWORD* pdwIdRepair )
{
	if (!g_pPlayer) return;

		BEFORESENDSOLE( ar, PACKETTYPE_REPAIRITEM, DPID_UNKNOWN );
		u_long uOffset	= ar.GetOffset();
		BYTE c	= 0;
		ar << c;

		for( int i = 0; i < MAX_REPAIRINGITEM; i++ )
		{
//			if( pMover->m_adwIdRepair[i] != NULL_ID )
			if( pdwIdRepair[i] != NULL_ID )
			{
				CItemElem* pItemElem = g_pPlayer->m_Inventory.GetAtId( pdwIdRepair[i] );
				if( pItemElem )
				{
					ItemProp* pItemProp	= pItemElem->GetProp();
					if( pItemProp && pItemElem->m_nHitPoint < (int)( pItemProp->dwEndurance ) && pItemElem->m_nRepair != pItemProp->nMaxRepair )
					{
						ar << (BYTE)pdwIdRepair[i];
						c++;
					}
				}
//				ar << (BYTE)pMover->m_adwIdRepair[i];
			}
		}

		if( c > 0 )
		{
			BYTE* lpBuf		= ar.GetBuffer( &nBufSize );
			*( lpBuf + uOffset )	= c;
			Send( (LPVOID)lpBuf, nBufSize, DPID_SERVERPLAYER );
		}
		else
		{
			//수리할것이 없습니다.
			g_WndMng.PutString( prj.GetText( TID_GAME_REPAIR_NO ), NULL, prj.GetTextColor( TID_GAME_REPAIR_NO ) );
		}

}

void CDPClient::OnTag( OBJID objid, CAr & ar )
{
	g_Neuz.m_nTagCount = 0;
	short m_nCount;
	ar >> m_nCount;
	g_Neuz.m_nTagCount = m_nCount;
	g_Neuz.m_bCheckTag = FALSE;
	
	for( int i = 0 ; i < g_Neuz.m_nTagCount ; i++ )
	{
		ar.ReadString( g_Neuz.m_strTagName[ i ], 65 );
		ar >> g_Neuz.m_dwtegDate[ i ];
		ar.ReadString( g_Neuz.m_strTagMessage[ i ], 256 );
	}

	if( 0 < g_Neuz.m_nTagCount )
	{
		CString str;
		str.Format( prj.GetText( TID_GAME_MESSNEWNOTE ), g_Neuz.m_nTagCount );
		g_WndMng.PutString( (LPCTSTR)str, NULL, prj.GetTextColor( TID_GAME_MESSNEWNOTE ) );
	}
}

void CDPClient::OnRunScriptFunc( OBJID, CAr & ar )
{
	CWndDialog* pWndDialog	= g_WndMng.GetWndBase<CWndDialog>( APP_DIALOG_EX );

	RunScriptFunc::Variant rsf;
	ar.Read(&rsf, sizeof(rsf));

	using namespace RunScriptFunc;

	struct Visitor {
		CWndDialog * pWndDialog;

		void operator()(const AddKey & rsf) const {
			if (pWndDialog) {
				pWndDialog->AddKeyButton(rsf);
			}
		}

		void operator()(const RemoveKey & rsf) const {
			if (pWndDialog) {
				pWndDialog->RemoveKeyButton(rsf.lpszVal1);
			}
		}

		void operator()(const RemoveAllKey &) const {
			if (pWndDialog) {
				pWndDialog->RemoveAllKeyButton();
			}
		}

		void operator()(const AddAnswer & rsf) const {
			if (pWndDialog)
				pWndDialog->AddAnswerButton(rsf);
		}

		void operator()(const Say & rsf) const {
			if (pWndDialog) {
				pWndDialog->m_bSay = TRUE;
				pWndDialog->Say(rsf.lpszVal1, rsf.dwVal2);
			}
		}

		void operator()(const Quest & rsf) const {
			if (pWndDialog) {
				pWndDialog->AddQuestInList(rsf.lpszVal1, rsf.lpszVal2, rsf.dwVal2, rsf.isNew);
			}
		}

		void operator()(const Exit &) const {
			if (pWndDialog)
				pWndDialog->Destroy();
		}

		void operator()(const InitStat & rsf) const {
			if (IsValidObj(g_pPlayer)) {
				g_pPlayer->SetStr(15);
				g_pPlayer->SetInt(15);
				g_pPlayer->SetDex(15);
				g_pPlayer->SetSta(15);
				g_pPlayer->m_nRemainGP = (long)rsf.dwVal1;
			}
		}

		void operator()(const InitStr & rsf) const {
			if (IsValidObj(g_pPlayer)) {
				g_pPlayer->SetStr(15);
				g_pPlayer->m_nRemainGP = (long)rsf.dwVal1;
			}
		}

		void operator()(const InitSta & rsf) const {
			if (IsValidObj(g_pPlayer)) {
				g_pPlayer->SetSta(15);
				g_pPlayer->m_nRemainGP = (long)rsf.dwVal1;
			}
		}

		void operator()(const InitDex & rsf) const {
			if (IsValidObj(g_pPlayer)) {
				g_pPlayer->SetDex(15);
				g_pPlayer->m_nRemainGP = (long)rsf.dwVal1;
			}
		}

		void operator()(const InitInt & rsf) const {
			if (IsValidObj(g_pPlayer)) {
				g_pPlayer->SetInt(15);
				g_pPlayer->m_nRemainGP = (long)rsf.dwVal1;
			}
		}

		void operator()(const QuestSetPlayerName & rsf) const {
			if (!g_WndMng.m_pWndChangeName) {
				g_WndMng.m_pWndChangeName = new CWndChangeName;
				g_WndMng.m_pWndChangeName->Initialize();
			}
			g_WndMng.m_pWndChangeName->SetData(0xffff, 1);
		}
	};

	std::visit(Visitor{ pWndDialog }, rsf);
}

void CDPClient::PostPlayerAngle(bool f) {
	m_pa.fValid = f;
}

void CDPClient::FlushPlayerAngle( void )
{
	if( m_pa.fValid && ( ++m_pa.nCounter % 30 ) == 0 )
	{
		m_pa.fValid		= false;
		CMover* pPlayer	= g_pPlayer;
		if( pPlayer )
		{
			BOOL nIsFly = pPlayer->m_pActMover->IsFly();

#ifndef __Y_INTERFACE_VER3
			if( FALSE == pPlayer->m_pActMover->IsFly() )
				return;
#endif //__Y_INTERFACE_VER3

			D3DXVECTOR3 v, vd;
			float f, fAngleX, fAccPower;
			pPlayer->m_pActMover->PresupposePos2( &v, &vd, &f, &fAngleX, &fAccPower, MAX_CORR_SIZE_150 );

			TRACE( "%f\t%f\n", pPlayer->GetAngleX(), fAngleX );

			BEFORESENDSOLE( ar, PACKETTYPE_PLAYERANGLE, DPID_UNKNOWN );
			ar << v << vd << f << fAngleX << fAccPower << pPlayer->m_pActMover->m_fTurnAngle;
			ar << g_TickCount.GetTickCount();	//
			SEND( ar, this, DPID_SERVERPLAYER );
		}
	}
}


// 길드투표를 입력한다.
// szTitle: 제목 
// szQuestion: 질문내용 
// szSelections: 4지선다의 내용 
void CDPClient::SendAddVote( const char* szTitle, const char* szQuestion, const char* szSelections[] )
{
	ASSERT( szTitle );
	ASSERT( szQuestion );
	ASSERT( szSelections );

	BEFORESENDSOLE( ar, PACKETTYPE_NC_ADDVOTE, DPID_UNKNOWN );
	ar << g_pPlayer->m_idGuild;
	ar << g_pPlayer->m_idPlayer;

	ar.WriteString( szTitle );
	ar.WriteString( szQuestion );
	for( int i=0; i<4; ++i )
	{
		ar.WriteString( szSelections[i] );
	}
	SEND( ar, this, DPID_SERVERPLAYER );
}

// idVote - 투표를 취소한다.
void CDPClient::SendRemoveVote( u_long idVote )
{
	ASSERT( idVote );
	
	BEFORESENDSOLE( ar, PACKETTYPE_NC_REMOVEVOTE, DPID_UNKNOWN );
	ar << g_pPlayer->m_idGuild;
	ar << g_pPlayer->m_idPlayer;
	ar << idVote;
	SEND( ar, this, DPID_SERVERPLAYER );
}


// idVote - 투표를 종료시킨다.
void CDPClient::SendCloseVote( u_long idVote )
{
	ASSERT( idVote );

	BEFORESENDSOLE( ar, PACKETTYPE_NC_CLOSEVOTE, DPID_UNKNOWN );
	ar << g_pPlayer->m_idGuild;
	ar << g_pPlayer->m_idPlayer;
	ar << idVote;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// idVote - 투표를한다.
// cbSelection - 투표 선택 
void CDPClient::SendCastVote( u_long idVote, BYTE cbSelection )
{
	ASSERT( idVote );
	ASSERT( cbSelection < 4 );

	BEFORESENDSOLE( ar, PACKETTYPE_NC_CASTVOTE, DPID_UNKNOWN );
	ar << g_pPlayer->m_idGuild;
	ar << g_pPlayer->m_idPlayer;
	ar << idVote;
	ar << cbSelection;
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::OnSchoolReport( CAr & ar )
{
	SCHOOL_ENTRY school[MAX_SCHOOL];
	ar.Read( school, sizeof(SCHOOL_ENTRY) * MAX_SCHOOL );

	short nElapse;
	ar >> nElapse;

	switch( g_eLocal.GetState( EVE_SCHOOL_BATTLE ) )
	{
		case SBS_END:
			{
				for( int i = 0; i < MAX_SCHOOL; i++ )
				{
					if( lstrlen( school[i].lpName ) > 0 && school[i].nSurvivor > 0 )
					{
						TRACE( "%d\t%s\t%d\n", i, school[i].lpName, school[i].nSurvivor );
						char sReport[260]	= { 0, };
						sprintf( sReport, prj.GetText( TID_SBEVE_SBRESULT ), i + 1, school[i].lpName, school[i].nSurvivor );
						g_WndMng.PutString( sReport, NULL, 0xffffff99 );
						g_WndMng.PutString( sReport, NULL, prj.GetTextColor( TID_SBEVE_SBRESULT ) );
					}
				}
				break;
			}
		default:
			{
				for( int i = 0; i < MAX_SCHOOL; i++ )
				{
					if( school[i].nSurvivor )
					{
						/*
						경기 시작 후 시간	학교 이름	생존자	사망자
						경기 시간 %d 분 경과 현재 상황	%s	%d 명	%s 명
						Text ID	TID_SBEVE_SBREALTIME
						*/
						TRACE( "%d\t%s\t%d\t%d\n", nElapse, school[i].lpName, school[i].nSurvivor, school[i].nDead );
						char sReport[260]	= { 0, };
						sprintf( sReport, prj.GetText( TID_SBEVE_SBREALTIME ), nElapse, school[i].lpName, school[i].nSurvivor, school[i].nDead );
						g_WndMng.PutString( sReport, NULL, 0xffffff99 );
						g_WndMng.PutString( sReport, NULL, prj.GetTextColor( TID_SBEVE_SBREALTIME ) );
					}
				}
				break;
			}
	}
}


void CDPClient::OnRemoveQuest( CAr & ar ) {
	int nRemoveType; ar >> nRemoveType;

	if (nRemoveType == -1 || nRemoveType == 0) {
		QuestId dwQuestCancelID; ar >> dwQuestCancelID;

		if (nRemoveType == -1) {
			const QuestProp * pQuestPorp = dwQuestCancelID.GetProp();
			g_WndMng.PutString(TID_GAME_REMOVEQUEST, pQuestPorp->m_szTitle);
		}

		g_pPlayer->RemoveQuest( dwQuestCancelID );
		g_QuestTreeInfoManager.DeleteTreeInformation(dwQuestCancelID.get());
	} else if (nRemoveType == 1) {
		g_pPlayer->RemoveAllQuest();
		g_QuestTreeInfoManager.DeleteAllTreeInformation();
	} else if (nRemoveType == 2) {
		g_pPlayer->RemoveCompleteQuest();
	}

	if (CWndQuest * pWndQuest = (CWndQuest *)g_WndMng.GetApplet(APP_QUEST_EX_LIST)) {
		pWndQuest->Update();
	}
}


void CDPClient::OnInitSkillPoint( CAr & ar )
{
	int nSkillPoint;
	ar >> nSkillPoint;
	if( g_pPlayer->m_nSkillPoint < nSkillPoint )
	{
		CreateSfx(XI_GEN_LEVEL_UP01,g_pPlayer->GetPos(),g_pPlayer->GetId());
		PlayMusic( BGM_IN_LEVELUP );
	}
	g_pPlayer->m_nSkillPoint = nSkillPoint;

	for (SKILL & skill : g_pPlayer->m_jobSkills) {
		const ItemProp * pSkillProp = skill.GetProp();
		if (!pSkillProp) continue;
		
		if (pSkillProp->dwItemKind1 == JTYPE_MASTER
			|| pSkillProp->dwItemKind1 == JTYPE_HERO) {
			continue;
		}

		skill.dwLevel = 0;
	}
	
	CWndSkillTreeCommon::ReInitIfOpen();
	
	g_WndMng.PutString(TID_GAME_RECCURENCE);
}

void CDPClient::OnDoUseSkillPoint(CAr & ar) {
	ar >> g_pPlayer->m_jobSkills;
	ar >> g_pPlayer->m_nSkillPoint;

	CWndSkillTreeCommon::ReInitIfOpen();
}

void CDPClient::SendRandomScroll( OBJID objid, OBJID objid2 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_RANDOMSCROLL, DPID_UNKNOWN );
	ar << objid;
	ar << objid2;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendEnchant( OBJID objid, OBJID objMaterialId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ENCHANT, DPID_UNKNOWN );
	ar << objid;
	ar << objMaterialId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRemoveAttribute(const OBJID objid) {
	SendPacket<PACKETTYPE_REMVOE_ATTRIBUTE, OBJID>(objid);
}

// 피어싱(뚫음)
void CDPClient::SendPiercingSize( OBJID objid1, OBJID objid2, OBJID objid3 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PIERCING_SIZE, DPID_UNKNOWN );
	ar << objid1;
	ar << objid2;
	ar << objid3;
	SEND( ar, this, DPID_SERVERPLAYER );
}

#ifdef __SYS_ITEMTRANSY
void CDPClient::SendItemTransy( OBJID objid0, OBJID objid1, DWORD dwChangeId, BOOL bCash )
#else // __SYS_ITEMTRANSY
void CDPClient::SendItemTransy( OBJID objid0, OBJID objid1 )
#endif // __SYS_ITEMTRANSY
{
	BEFORESENDSOLE( ar, PACHETTYPE_ITEMTRANSY, DPID_UNKNOWN );
	ar << objid0;
	ar << objid1;
#ifdef __SYS_ITEMTRANSY
	ar << dwChangeId;
	ar << bCash;
#endif // __SYS_ITEMTRANSY
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendCommercialElem( DWORD dwItemId, DWORD dwItemId1 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_COMMERCIALELEM, DPID_UNKNOWN );
	ar << dwItemId;
	ar << dwItemId1;
	SEND( ar, this, DPID_SERVERPLAYER );
}

/*
// pObj를 채집할꺼라고 서버로 보냄.
void	CDPClient::SendDoCollect( CObj *pObj )
{
	if( IsInvalidObj(pObj) )	return;
	if( pObj->GetType() != OT_MOVER )	return;

	CMover *pTarget = (CMover *)pObj;
	if( pTarget->IsPlayer() || pTarget->IsLive() )		// 플레이어를 대상으로 했거나 살아있는놈이면 실패.
		return;
	
	BEFORESENDSOLE( ar, PACKETTYPE_DO_COLLECT, DPID_UNKNOWN );
	ar << pTarget->GetId();
	SEND( ar, this, DPID_SERVERPLAYER );
	
}

// objid가 채집을 시작하라고 클라에서 받음.
void	CDPClient::OnDoCollect( OBJID objid, CAr & ar )
{
	OBJID idTarget;
	ar >> idTarget;
	
	CMover *pPlayer = prj.GetMover( objid );
	if( IsValidObj( pPlayer ) ) 
	{
		CMover *pTarget = prj.GetMover( idTarget );
		if( IsValidObj( pTarget ) )
			pPlayer->DoCollect( pTarget );
	}
}

// objid의 채집이 끝남.
void	CDPClient::OnStopCollect( OBJID objid, CAr & ar )
{
	CMover *pPlayer = prj.GetMover( objid );
	if( IsValidObj( pPlayer ) ) 
	{
		pPlayer->m_pActMover->ResetState( OBJSTA_ACTION_ALL );
#ifdef __XUZHU
		g_WndMng.PutString( "채집 중지" );
#endif
	}
}
*/

// 액티브 스킬 발동.
void	CDPClient::OnCreateSkillEffect( OBJID objid, CAr & ar )
{
	OBJID idTarget;
	DWORD dwSkill, dwLevel;
	ar >> idTarget >> dwSkill >> dwLevel;
	
	CMover *pAttacker = prj.GetMover( objid );		// 액티브 스킬 발동자.
	if( IsValidObj( pAttacker ) ) 
	{
		Error( "OnCreateSkillEffect : 누가 이걸 불렀으....%d %d", dwSkill, dwLevel );
	}
}

// objid가 스턴에 걸림.
void	CDPClient::OnSetStun( OBJID objid, CAr & ar )
{
	DWORD tmMaxTime;
	ar >> tmMaxTime;
	
	CMover *pMover = prj.GetMover( objid );		// 
	if( IsValidObj( pMover ) ) 
	{
		if( tmMaxTime > 0 )
			pMover->SetStun( TRUE, tmMaxTime );
		else
			pMover->SetStun( FALSE );
			
	}
}

// objid의 SendActMsg를 수행
void	CDPClient::OnSendActMsg( OBJID objid, CAr & ar )
{
	int nParam1, nParam2, nParam3;
	OBJMSG dwMsg;
	BOOL	bFly;
	ar >> nParam1;	dwMsg = (OBJMSG)nParam1;
	ar >> bFly;
	ar >> nParam1 >> nParam2 >> nParam3;
	
	CMover *pMover = prj.GetMover( objid );		// 
	if( IsValidObj( pMover ) ) 
	{
		if( bFly == FALSE )
			pMover->m_pActMover->ProcessActMsg1( pMover, dwMsg, nParam1, nParam2, nParam3, 1, 0 );
		else
			pMover->m_pActMover->ProcessActMsg2( pMover, dwMsg, nParam1, nParam2, nParam3, 1, 0 );
	}
}


void	CDPClient::OnRequestGuildRank(CAr & ar) {
	ar >> CGuildRank::Instance;
}


void CDPClient::OnSexChange( OBJID objid, CAr & ar )
{
	u_short uIndex;
	ar >> uIndex;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->SetSex( (uIndex==MI_MALE) ? SEX_MALE : SEX_FEMALE );
		if( pMover->m_pModel && pMover->m_pModel->IsAniable() )
			SAFE_DELETE( pMover->m_pModel );
		pMover->SetTypeIndex( OT_MOVER, (DWORD)uIndex );
		pMover->ResetScale();
		pMover->UpdateLocalMatrix();
		pMover->m_dwMotion = -1;
		pMover->SetMotion( MTI_STAND );
		pMover->UpdateParts( !pMover->IsActiveMover() );
		if( pMover->IsActiveMover() && g_pBipedMesh )
		{
			CMover::UpdateParts( pMover->GetSex(), pMover->m_skin, pMover->m_aEquipInfo, g_pBipedMesh, &pMover->m_Inventory );
		}

		if( pMover->IsActiveMover() )
		{
			CWndInventory* pWndInventory	= (CWndInventory*)g_WndMng.GetWndBase( APP_INVENTORY );
			if(pWndInventory && pWndInventory->GetModel())
			{
				pWndInventory->UpDateModel();
			}
		}		
	}
}

void CDPClient::OnRemoveSkillInfluence( OBJID objid, CAr & ar )
{
	WORD	wType, wID;
	ar >> wType >> wID;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		pMover->RemoveBuff( wType, wID );
	}
}

void CDPClient::OnRemoveAllSkillInfluence( OBJID objid, CAr & ar )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
		pMover->RemoveCommonBuffs();
}


void CDPClient::OnSetScale( OBJID objid, CAr & ar )
{
	float fScalePerscale;
	ar >> fScalePerscale;
	
	CMover* pMover	= prj.GetMover( objid );
	if( TRUE == IsValidObj( pMover ) )
	{
		FLOAT fScale = pMover->m_pModel->m_pModelElem->m_fScale;		// mdlDyna에 있던 scale값이 100%
		fScale *= fScalePerscale;										// 줄어듬.
		pMover->m_fDestScale = fScale;
		pMover->m_fDestScaleSlerp = 1.0f;
	}
}

void CDPClient::SendEnterChattingRoom( u_long uidChatting )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ENTERCHTTING, DPID_UNKNOWN );
	ar << uidChatting;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendChatting( char * pszChat )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHATTING, DPID_UNKNOWN );
	ar.WriteString( pszChat );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendOpenChattingRoom()
{
	BEFORESENDSOLE( ar, PACKETTYPE_OPENCHATTINGROOM, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendCloseChattingRoom()
{
	BEFORESENDSOLE( ar, PACKETTYPE_CLOSECHATTINGROOM, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnChatting( OBJID objid, CAr & ar )
{
	BYTE nType;
	ar >> nType;

	u_long uidPlayer;
	TCHAR szName[MAX_NAME] = {0,};

	switch( nType )
	{
	case CHATTING_NEWCHATTING:
		{
		  g_Chatting.m_members.clear();
			int nSize;
			ar >> nSize;
			for( int i = 0 ; i < nSize ; ++i )
			{
				ar >> uidPlayer;
				ar.ReadString( szName, MAX_NAME );
				g_Chatting.AddChattingMember(CChatting::ChatMember(uidPlayer, szName));
			}
		}		
		break;
	case CHATTING_ENTERMEMBER:
		{
			ar >> uidPlayer;
			ar.ReadString( szName, MAX_NAME );
			const bool ok = g_Chatting.AddChattingMember(CChatting::ChatMember(uidPlayer, szName));

			if (ok) {
				CWndVendor * pWndVendor = (CWndVendor *)g_WndMng.GetWndVendorBase();

				if (pWndVendor && pWndVendor->m_pwndVenderMessage) {
					pWndVendor->m_pwndVenderMessage->AddChattingMemver(szName);
				}
			}
		}
		break;
	case CHATTING_DELETECHATTING:
		{
		g_Chatting.m_members.clear();
		}
		break;
	case CHATTING_REMOVEMEMBER:
		{
			ar >> uidPlayer;

			CWndVendor* pWndVendor = (CWndVendor*)g_WndMng.GetWndVendorBase();
			
			if (pWndVendor && pWndVendor->m_pwndVenderMessage) {
				const auto member = g_Chatting.GetMember(uidPlayer);
				if (member) {
					pWndVendor->m_pwndVenderMessage->RemoveChattingMemver(member->m_szName);
				}
			}
			
			g_Chatting.RemoveChattingMember( uidPlayer );
		}
		break;
	case CHATTING_CHATTING:
		{
			static	TCHAR	sChat[1024];

			ar >> uidPlayer;
			ar.ReadString( sChat, 1024 );
			
			CWndVendor* pWndVendor = (CWndVendor*)g_WndMng.GetWndVendorBase();

			if (pWndVendor && pWndVendor->m_pwndVenderMessage) {
				const auto member = g_Chatting.GetMember(uidPlayer);
				if (member) {
					pWndVendor->m_pwndVenderMessage->AddMessage(member->m_szName, sChat);
				}
			}
			// 채팅 
		}
		break;
	case CHATTING_CHATTINGROOMSTATE:
		{
			BOOL bState;
			ar >> bState;
			g_Chatting.m_bState = bState;
			if( bState == FALSE )
			{
				CWndVendor* pVendor = (CWndVendor*)g_WndMng.GetWndVendorBase();
				if( pVendor )
				{
					if( pVendor->m_pwndVenderMessage != NULL )					
						SAFE_DELETE( pVendor->m_pwndVenderMessage );
				}
			}
		}
		break;
	}
}

void CDPClient::SendCommandPlace( BYTE nType )
{
	BEFORESENDSOLE( ar, PACKETTYPE_COMMONPLACE, DPID_UNKNOWN );
	ar << nType;
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::OnCommonPlace( OBJID objid, CAr & ar )
{
	BYTE nType;

	ar >> nType;

	CCommonCtrl* pObj	= (CCommonCtrl*)prj.GetCtrl( objid );

	if( pObj != NULL )
	{
		switch( nType )
		{
		case COMMONPLACE_ACTION:
			pObj->SetActionPlay();
//			g_WorldMng()->SetObjFocus( NULL ); //Focus된 Ctrl을 해제.
			break;
		case COMMONPLACE_ALPHA:
			pObj->m_bAlpha = TRUE;
			break;
		case COMMONPLACE_QUAKE:
			{
				CWorld* pWorld = pObj->GetWorld();
				
				if( pWorld )
				{
					if( pWorld->m_pCamera )
					{
						pWorld->m_pCamera->SetQuake( 1400, 1.0f );
					}
				}
			}
			break;			
//		case COMMONPLACE_ERROR:
//			g_WorldMng()->SetObjFocus( NULL ); //Focus된 Ctrl을 해제.
//			break;
		}
	}
}

void CDPClient::OnClearTarget(CAr & ar)
{
	g_WorldMng()->SetObjFocus( NULL );
}

void CDPClient::OnDoApplySkill( OBJID objid, CAr & ar )
{
	OBJID idTarget;
	DWORD dwSkill, dwLevel;
	ar >> idTarget >> dwSkill >> dwLevel;

	CCtrl* pCtrl	= prj.GetCtrl( objid );
	if( IsValidObj( (CObj*)pCtrl ) )
	{
		ItemProp *pSkillProp;
		AddSkillProp *pAddSkillProp;

		g_pPlayer->GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, dwLevel, "OnDoApplySkill" );

		CCtrl* pCtrlTarget	= prj.GetCtrl( idTarget );

		if( IsValidObj( (CObj*)pCtrlTarget ) )
		{
			if( pCtrlTarget->GetType() == OT_MOVER )
			{
				pCtrl->DoApplySkill( pCtrlTarget, pSkillProp, pAddSkillProp );
			}
		}
	}
}

void CDPClient::OnCommonSkill( OBJID objid, CAr & ar )
{
	DWORD dwSkill, dwLevel;
	ar >> dwSkill >> dwLevel;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		ItemProp* pSkill = prj.GetSkillProp(dwSkill);
		
		if(pSkill == NULL )
			return;
		
		AddSkillProp* pAddSkill = prj.GetAddSkillProp(pSkill->dwSubDefine, dwLevel);
		
		if( pAddSkill == NULL )
			return;

		g_Flash.SetTime( pAddSkill->dwCircleTime, pAddSkill->dwSkillTime, TRUE );
	}
}

// Mover를 밀게 하다.
void	CDPClient::OnPushPower( OBJID objid, CAr & ar )
{
	D3DXVECTOR3 vPos;
	FLOAT fAngle, fPushAngle, fPower;
	ar >> vPos;	
	ar >> fAngle;
	ar >> fPushAngle >> fPower;
	
	CMover *pMover = prj.GetMover( objid );		// 
	if( IsValidObj( pMover ) ) 
	{
		//pMover->SetPos( vPos );			// 동기화땜에 서버랑 좌표를 일단 맞춤.
		pMover->SetAngle( fAngle );
		pMover->m_pActMover->m_vDeltaE = AngleToVectorXZ( fPushAngle, fPower );	// 밀리는 힘 설정.
	}
}

// 버프스킬 리셋
void	CDPClient::OnResetBuffSkill( OBJID objid, CAr & ar )
{
	CMover *pPlayer = prj.GetMover( objid );
	if( IsValidObj( pPlayer ) ) 
		pPlayer->ClearBuffInst();
}

//	이 시간은 네비게이터에 남아 있는 시간으로 디파인이나 파일로 뺄수도 있을듯하지만 우선 하드 코딩이닷
void CDPClient::OnSetNaviPoint( OBJID objid, CAr & ar )
{
	NaviPoint nv;
	TCHAR szName[64] = { 0, };

	ar >> nv.Pos;
	ar.ReadString( szName, 64 );

	nv.Name = szName;
	nv.objid = objid;
	nv.Time = 200;

	const auto existingPoint = std::find_if(
		g_pPlayer->m_vOtherPoint.begin(), g_pPlayer->m_vOtherPoint.end(),
		[objid](const NaviPoint & point) { return point.objid == objid; }
	);

	if (existingPoint != g_pPlayer->m_vOtherPoint.end()) {
		*existingPoint = nv;
		return;
	}

	if (g_pPlayer->m_vOtherPoint.size() >= 10) {
		return;
	}

	g_pPlayer->m_vOtherPoint.emplace_back(nv);
}

void CDPClient::SendSetNaviPoint(const D3DXVECTOR3 & Pos, OBJID objidTarget) {
	SendPacket<PACKETTYPE_SETNAVIPOINT, D3DXVECTOR3, OBJID>(Pos, objidTarget);
}


// CreateSfx
// idPlayer가 NULL_ID가 아니면 Mover에게
// bFlag :	FALSE : 그자리에 나오게
//			TRUE  : 따라다니게 하기
void CDPClient::SendCreateSfxObj( DWORD dwSfxId, u_long idPlayer, BOOL bFlag )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CREATESFXOBJ, DPID_UNKNOWN );
	ar << dwSfxId;
	ar << idPlayer;
	ar << bFlag;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 현상금 걸기 패킷 
// dwGold: 걸 현상금 
void CDPClient::SendWantedGold( int nGold, LPCTSTR szMsg )
{
	BEFORESENDSOLE( ar, PACKETTYPE_NW_WANTED_GOLD, DPID_UNKNOWN );
	ar << nGold;
	ar.WriteString( szMsg );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// 현상금 리스트 요청 패킷
void CDPClient::SendWantedList()
{
	SendHdr( PACKETTYPE_NW_WANTED_LIST );
}

// 현상범이름 요청 
void CDPClient::SendWantedName()
{
	SendHdr( PACKETTYPE_NW_WANTED_NAME );
}

// 현상범 자세한정보 요청 패킷 
void CDPClient::SendWantedInfo(LPCTSTR szPlayer) {
	BEFORESENDSOLE(ar, PACKETTYPE_NW_WANTED_INFO, DPID_UNKNOWN);
	ar.WriteString(szPlayer);
	SEND(ar, this, DPID_SERVERPLAYER);
}

// 접속 종료시도를 알린다. 
// ( 이 패킷을 보내면, 서버에서 10초후에 접속을 끊는다. )
void CDPClient::SendReqLeave()
{
	SendHdr( PACKETTYPE_REQ_LEAVE );
}

void CDPClient::SendResurrectionOK()
{
	BEFORESENDSOLE( ar, PACKETTYPE_RESURRECTION_OK, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendResurrectionCancel()
{
	BEFORESENDSOLE( ar, PACKETTYPE_RESURRECTION_CANCEL, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendChangePKPVPMode( DWORD dwMode, int nFlag )
{
	BEFORESENDSOLE( ar, PACKETTYPE_MODE, DPID_UNKNOWN );
	ar << dwMode;
	ar << nFlag;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSetDuel( OBJID objid, CAr & ar )
{
	TRACE( "SNAPSHOTTYPE_SETDUEL\n" );
	int nDuel, nDuelState;
	OBJID idDuelOther;
	OBJID dwObjid;
	u_long idDuelParty;
	
	ar >> dwObjid >> nDuel >> nDuelState >> idDuelOther >> idDuelParty;

	CMover *pPlayer = prj.GetMover( dwObjid );
	if( IsValidObj( pPlayer ) )
	{
		if( pPlayer->IsActiveMover() )
		{
			if( pPlayer->m_nDuel == 1 && nDuel == 0 )	// clear
			{
				CMover* pOther	= prj.GetMover( pPlayer->m_idDuelOther );
				if( IsValidObj( (CObj*)pOther ) )
					pOther->ClearDuel();
			}
		}

		if( nDuel == 2 && nDuelState == 300 )
		{
			CString str;
			str.Format( prj.GetText( TID_GAME_PVPLEAVE ), pPlayer->GetName() );
			g_WndMng.PutString( str , NULL, prj.GetTextColor( TID_GAME_PVPLEAVE  ) );
		}

		pPlayer->m_nDuel	= nDuel;
		pPlayer->m_nDuelState	= nDuelState;
		pPlayer->m_idDuelOther	= idDuelOther;
		pPlayer->m_idDuelParty	= idDuelParty;
	}
}

void CDPClient::OnPKRelation(OBJID objid, CAr & ar) {
	CMover * pMover = prj.GetMover(objid);
	if (!IsValidObj(pMover)) return;

	Subsnapshot::PK byType;
	ar >> byType;
	switch (byType) {
		case Subsnapshot::PK::PINK:
		{
			BYTE byPink;
			ar >> byPink;
			pMover->SetPKPink(byPink);
			break;
		}
		case Subsnapshot::PK::PROPENSITY:
		{
			DWORD dwPKPropensity;
			ar >> dwPKPropensity;
			pMover->SetPKPropensity(dwPKPropensity);
			break;
		}
		case Subsnapshot::PK::PKVALUE:
		{
			int nPKValue;
			ar >> nPKValue;
			pMover->SetPKValue(nPKValue);
			break;
		}
	}
}

void CDPClient::OnWantedInfo( CAr & ar )
{
	D3DXVECTOR3 vPos;			// 현상범의 위치 
	BYTE		byOnline;		// if online = 1
	DWORD		dwWorldID;		// 월드번호 
	char		szWorld[32];	// 월드설명 

	ar >> vPos;
	ar >> byOnline;
	ar >> dwWorldID;		
	ar.ReadString( szWorld, 32 );

	if( byOnline )
	{
		char szBuffer[512] = { 0, };

		CWorld* pWorld = g_pPlayer->GetWorld();

		if( pWorld && dwWorldID == pWorld->GetID() )
		{
			g_WndMng.m_pWndWorld->SetRenderArrowWanted( TRUE, vPos );
			sprintf( szBuffer, prj.GetText(TID_PK_POINT_SAME), vPos.x, vPos.y, vPos.z );
		}
		else
		{
			sprintf( szBuffer, prj.GetText(TID_PK_POINT_DIFFER), szWorld, vPos.x, vPos.y, vPos.z );
		}
		g_WndMng.OpenMessageBoxUpper( szBuffer );		
	}
	else
	{
		g_WndMng.OpenMessageBoxUpper( prj.GetText( TID_GAME_NOTLOGIN ) );
	}
}

void CDPClient::SendSummonFriend( DWORD dwData, const char* lpszPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SUMMON_FRIEND, DPID_UNKNOWN );
	ar << dwData;
	ar.WriteString( lpszPlayer );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSummonFriendConfirm( OBJID objid, DWORD dwData )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SUMMON_FRIEND_CONFIRM, DPID_UNKNOWN );
	ar << objid << dwData;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSummonFriendCancel( OBJID objid, DWORD dwData )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SUMMON_FRIEND_CANCEL, DPID_UNKNOWN );
	ar << objid << dwData;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSummonParty( DWORD dwData )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SUMMON_PARTY, DPID_UNKNOWN );
	ar << dwData;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSummonPartyConfirm( OBJID objid, DWORD dwData )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SUMMON_PARTY_CONFIRM, DPID_UNKNOWN );
	ar << objid << dwData;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendQuerySetPlayerName( DWORD dwData, const char* lpszPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYSETPLAYERNAME, DPID_UNKNOWN );
	ar << dwData;
	ar.WriteString( lpszPlayer );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSetPlayerName( CAr & ar )
{
	u_long idPlayer;
	char lpszPlayer[MAX_PLAYER]	= { 0, };
	ar >> idPlayer;
	ar.ReadString( lpszPlayer, MAX_PLAYER );

	char lpOutputString[128]	= { 0, };
	sprintf( lpOutputString, "NEUZ.EXE\t// PAKCETTYPE_QUERYSETPLAYERNAME\t// %d\t// %s", idPlayer, lpszPlayer );
	OutputDebugString( lpOutputString );

	const char* pszOld	= CPlayerDataCenter::GetInstance()->GetPlayerString( idPlayer );
	if( lstrlen( pszOld ) > 0  )
		g_WndMng.UpdateMessage( pszOld, lpszPlayer );
	// prj
	CMover* pPlayer	= prj.GetUserByID( idPlayer );
	if( IsValidObj( (CObj*)pPlayer ) )
		pPlayer->SetName( lpszPlayer );
	CPlayerDataCenter*	pPlayerDataCenter	= CPlayerDataCenter::GetInstance();
	PlayerData* pPlayerData		= pPlayerDataCenter->GetPlayerData( idPlayer );
	if( pPlayerData )
	{
		PlayerData pd;
		memcpy( &pd, pPlayerData, sizeof(PlayerData) );
		pPlayerDataCenter->DeletePlayerData( idPlayer );
		lstrcpy( pd.szPlayer, lpszPlayer );
		pPlayerDataCenter->AddPlayerData( idPlayer, pd );
	}
	// Guild
	UpdateGuildWnd();
	// Messenger
	// Chat
	if (g_Chatting.RemoveChattingMember(idPlayer))
		g_Chatting.AddChattingMember(CChatting::ChatMember(idPlayer, lpszPlayer));

	CMover* pMover	= prj.GetUserByID( idPlayer );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( pMover->IsActiveMover() )
			g_WndMng.PutString( prj.GetText( TID_GAME_CHANGENAME ), NULL, prj.GetTextColor( TID_GAME_CHANGENAME  ) );
		CreateSfx( XI_SYS_EXCHAN01, pMover->GetPos(), pMover->GetId(), pMover->GetPos(), pMover->GetId() );
	}
}

void	CDPClient::SendDoEscape( void )
{	
	if( g_pPlayer )
	{
		CWorld* pWorld = g_pPlayer->GetWorld();

		if( pWorld && pWorld->GetID() == WI_WORLD_GUILDWAR )
		{
			g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_GAME_GUILDCOMBAT_NO_OUTWAR) );
			return;
		}

		SendHdr( PACKETTYPE_DO_ESCAPE );	
	}
}

// 서버에서 탈출성공판정후 클라에서 타임스탬프 갱신.
void	CDPClient::OnEscape( OBJID objid, CAr & ar )
{
}


void CDPClient::OnSnoop( CAr & ar )
{
	static char lpszString[2048]	= { 0, };
	ar.ReadString( lpszString, 2048 );
	g_WndMng.PutString( lpszString, NULL, 0xffffffff, CHATSTY_GENERAL );
}

void CDPClient::SendCheering( OBJID objid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CHEERING, DPID_UNKNOWN );
	ar << objid;
	SEND( ar, this, DPID_SERVERPLAYER );	
}

void CDPClient::OnSetCheerParam( OBJID objid, CAr & ar )
{
	int nCheerPoint;
	DWORD dwRest;
	ar >> nCheerPoint >> dwRest;
	BOOL bAdd;
	ar >> bAdd;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) )
	{
		if( bAdd )	// chipi_080411
			g_WndMng.PutString( prj.GetText(TID_CHEER_MESSAGE5) );

		pMover->SetCheerParam( nCheerPoint, GetTickCount(), dwRest );
		//
	}
}

void CDPClient::SendQueryEquip( OBJID objid )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYEQUIP, DPID_UNKNOWN );
	ar << objid;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendQueryEquipSetting( BOOL bAllow )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYEQUIPSETTING, DPID_UNKNOWN );
	ar << bAllow;
	SEND( ar, this, DPID_SERVERPLAYER );
}
/*
	ata2k
*/
void CDPClient::OnQueryEquip(OBJID objid, CAr & ar) {
	std::unique_ptr<std::array<CItemElem, MAX_HUMAN_PARTS>> aEquipInfoAdd
		= std::make_unique<std::array<CItemElem, MAX_HUMAN_PARTS>>();

	for (CItemElem & item : *aEquipInfoAdd) {
		item.Empty();
	}

	int cbEquip; ar >> cbEquip;

	int nParts;
	for (int i = 0; i < cbEquip; i++) {
		ar >> nParts;
		ar >> (*aEquipInfoAdd)[nParts];
	}

	CMover * pMover = prj.GetMover(objid);
	if (!IsValidObj(pMover)) return;

	SAFE_DELETE(g_WndMng.m_pWndQueryEquip);
	g_WndMng.m_pWndQueryEquip = new CWndQueryEquip(*pMover, std::move(aEquipInfoAdd));
	g_WndMng.m_pWndQueryEquip->Initialize();
}

void CDPClient::SendReturnScroll( int nSelect )
{
	BEFORESENDSOLE( ar, PACKETTYPE_RETURNSCROLL, DPID_UNKNOWN );
	ar << nSelect;
	SEND( ar, this, DPID_SERVERPLAYER );
}

//귀환의 두루마리 사용ACK
void CDPClient::OnReturnScrollACK( CAr & ar )
{
	SendDoUseItem( m_dwReturnScroll, NULL_ID, -1, FALSE );	// FALSE - 창을 띄우지 않는다.
}

void CDPClient::SendQuerySetGuildName( LPCSTR pszGuild, BYTE nId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYSETGUILDNAME, DPID_UNKNOWN );
	ar << nId;
	ar.WriteString( pszGuild );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnEndSkillQueue( OBJID objid )
{
	CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
	pTaskBar->OnCancelSkill();
}

// 서버로부터 클라의 액션포인트 갱신.
void	CDPClient::OnSetActionPoint( OBJID objid, CAr & ar )
{
	int nAP;
	ar >> nAP;		// 서버로부터 최대 쿨타임시간을 받음.
	if( IsInvalidObj(g_pPlayer) )	return;
	
	g_WndMng.m_pWndTaskBar->m_nActionPoint = nAP;
}

// 서버로부터 클라의 ObjFocus 변경.
void	CDPClient::OnSetTarget( OBJID, CAr & ar )
{
	OBJID idTarget;
	ar >> idTarget;		// 서버로부터 최대 쿨타임시간을 받음.
	if( IsInvalidObj(g_pPlayer) )	return;
	
	CMover *pMover = prj.GetMover( idTarget );
	if( IsValidObj(pMover) )
		g_WorldMng()->SetObjFocus( pMover );
}

void CDPClient::SendEndSkillQueue( void )	
{	
	SendHdr( PACKETTYPE_ENDSKILLQUEUE );	
}

void CDPClient::SendCreateMonster( DWORD dwItemId, D3DXVECTOR3 vPos )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CREATEMONSTER, DPID_UNKNOWN );
	ar << dwItemId;
	ar << vPos;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendQueryPostMail( BYTE nItem, short nItemNum, char* lpszReceiver, int nGold, char* lpszTitle, char* lpszText )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYPOSTMAIL, DPID_UNKNOWN );
	ar << nItem << nItemNum;
	ar.WriteString( lpszReceiver );
	ar << nGold;
	ar.WriteString( lpszTitle );
	ar.WriteString( lpszText );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendQueryRemoveMail(u_long nMail) {
	SendPacket<PACKETTYPE_QUERYREMOVEMAIL, u_long>(nMail);
}

void CDPClient::SendQueryGetMailItem(u_long nMail) {
	SendPacket<PACKETTYPE_QUERYGETMAILITEM, u_long>(nMail);
}

void CDPClient::SendQueryGetMailGold(u_long nMail) {
	SendPacket<PACKETTYPE_QUERYGETMAILGOLD, u_long>(nMail);
}

void CDPClient::SendQueryReadMail(u_long nMail) {
	SendPacket<PACKETTYPE_READMAIL, u_long>(nMail);
}

void CDPClient::SendQueryMailBox() {
	SendPacket<PACKETTYPE_QUERYMAILBOX>();
}

void CDPClient::OnPostMail(CAr & ar) {
	CMail * pMail = new CMail;
	ar >> *pMail;
	CMailBox::GetInstance()->AddMail(pMail);
}

void CDPClient::OnRemoveMail(CAr & ar) {
	const auto [nMail, nType] = ar.Extract<u_long, int>();

	CMailBox * pMailBox = CMailBox::GetInstance();
	switch (nType) {
		case CMail::mail: pMailBox->RemoveMail(nMail);     break;
		case CMail::item: pMailBox->RemoveMailItem(nMail); break;
		case CMail::gold: pMailBox->RemoveMailGold(nMail); break;
		case CMail::read: pMailBox->ReadMail(nMail);       break;
	}
}

void CDPClient::OnMailBox( CAr & ar ) {
	ar >> *CMailBox::GetInstance();

	if (CWndPost * pWndPost = g_WndMng.GetWndBase<CWndPost>(APP_POST)) {
		pWndPost->EnableWindow( TRUE );
		pWndPost->m_PostTabReceive.UpdateScroll();
		pWndPost->CloseMailRequestingBox();

		g_WndMng.m_bWaitRequestMail = FALSE;
	}
}

void CDPClient::OnMailBoxReq( CAr & ar )
{
	bool bCheckTransMailBox = FALSE;
	ar >> bCheckTransMailBox;

	if( bCheckTransMailBox )
	{
		//받은거고 지금부터 5초세고 그래도 메일을 못받으면 메일 요청함.
		g_WndMng.m_bWaitRequestMail = TRUE;
	}
	else 
	{
		
		// 요청 window는 닫아준다 메일이 없으므로,,,
		CWndPost* pWndPost	= g_WndMng.GetWndBase<CWndPost>( APP_POST );
		if( pWndPost )
		{
			pWndPost->CloseMailRequestingBox();
			pWndPost->EnableWindow( TRUE );			//메일이 없으면 활성화 
		}

		CMailBox::GetInstance()->Clear();

		// 메일이 없기 때문에 받은 목록을 초기화
		if( pWndPost )
		{
			pWndPost->m_PostTabReceive.UpdateScroll();
		}

		// 사용자의 마지막 메일 정보 저장해 놓은 파일 삭제
		if( g_pPlayer == NULL )
		{
			return;
		}

		CString strFileName = _T("");
		strFileName.Format( "%s_MailData.Temp", g_pPlayer->GetName() );
		if( DeleteFile( strFileName ) == FALSE)
		{
			Error( "OnMailBoxReq : Fail Delete %s ", strFileName );
		}

	}

}

void CDPClient::OnSummon( CAr & ar )
{
	BYTE nState;
	ar >> nState;
	switch( nState )
	{
	case SOMMON_FRIEND:
		OnSummonFriend( ar );
		break;
	case SOMMON_FRIEND_CONFIRM:
		OnSummonFriendConfirm( ar );
		break;
	case SOMMON_PARTY_CONFIRM:
		OnSummonPartyConfirm( ar );
		break;
	}
}
void CDPClient::OnSummonFriend( CAr & ar )
{
	DWORD dwData;
	OBJID objid;
	ar >> dwData;
	ar >> objid;

	DWORD dwId = HIWORD( dwData );
	const CItemElem * const pItemBase	= g_pPlayer->GetItemId( dwId );
	if (!pItemBase) return;

	CWndSummonFriend *pWndSummonFriend	= (CWndSummonFriend*)g_WndMng.GetWndBase( APP_SUMMON_FRIEND );
	if( !pWndSummonFriend )
	{
		pWndSummonFriend	= new CWndSummonFriend;
		pWndSummonFriend->Initialize();
	}
	pWndSummonFriend->SetData( (WORD)( pItemBase->m_dwObjId ), 0 );
}
void CDPClient::OnSummonFriendConfirm( CAr & ar )
{
	OBJID objid;
	DWORD dwData;
	char szName[MAX_NAME] = {0,};
	char szWorldName[128] = {0,};
	ar >> objid >> dwData;
	ar.ReadString( szName, MAX_NAME );
	ar.ReadString( szWorldName, 128 );
	
	if( g_pPlayer )
	{
		CWndSummonFriendMsg *pWndSummonFriendConfirm	= (CWndSummonFriendMsg*)g_WndMng.GetWndBase( APP_MESSAGEBOX_SUMMONFRINED );
		if( !pWndSummonFriendConfirm )
		{
			pWndSummonFriendConfirm	= new CWndSummonFriendMsg;
			pWndSummonFriendConfirm->Initialize();
		}
		pWndSummonFriendConfirm->SetData( objid, dwData, szName, szWorldName );		
	}
}
void CDPClient::OnSummonPartyConfirm( CAr & ar )
{
	OBJID objid;
	DWORD dwData;
	char szWorldName[256] = {0,};
	ar >> objid >> dwData;
	ar.ReadString( szWorldName, 256 );
	
	if( g_pPlayer )
	{
		CWndSummonPartyMsg* pWndSummonPartyConfirm = (CWndSummonPartyMsg*)g_WndMng.GetWndBase( APP_MESSAGEBOX_SUMMONPARTY );
		if( !pWndSummonPartyConfirm )
		{
			pWndSummonPartyConfirm = new CWndSummonPartyMsg;
			pWndSummonPartyConfirm->Initialize();
		}
		pWndSummonPartyConfirm->SetData( objid, dwData, szWorldName );
	}
}

void CDPClient::OnRemoveGuildBankItem( CAr & ar )
{
	u_long idGuild;
	DWORD dwId;
	DWORD dwItemNum;
	ar >> idGuild >> dwId >> dwItemNum;
	CGuild* pGuild	= g_GuildMng.GetGuild( idGuild );
	if( pGuild )
	{
		if( dwId == NULL_ID )
			pGuild->m_nGoldGuild	-= dwItemNum;
		else
			pGuild->RemoveItem( dwId, (short)( dwItemNum ) );
	}
}

void CDPClient::OnMotionArrive( OBJID objid, CAr & ar )
{
	DWORD objmsg;
	
	ar >> objmsg;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( (CObj*)pMover ) ) 
	{
		pMover->m_dwMotionArrive = (OBJMSG)objmsg;
	}
}

#ifdef __EVENT_1101
void CDPClient::OnCallTheRoll( CAr & ar )
{
#ifdef __EVENT_1101_2
	EXPINTEGER nEventFlag;
	ar >> nEventFlag;
	CWebBox::GetInstance()->m_nEventFlag	= nEventFlag;
#else // __EVENT_1101_2
	DWORD	dwEventFlag;
	ar >> dwEventFlag;
	CWebBox::GetInstance()->m_dwEventFlag	= dwEventFlag;
#endif // __EVENT_1101_2
	//
	CWndInstantMsg* pWndInstantMsg = g_WndMng.OpenInstantMsg( "Event" );
	if( pWndInstantMsg )
	{
		CTime ctTime = CTime::GetCurrentTime();
		char lpString[260]	= { 0, };
		sprintf( lpString, prj.GetText( TID_GAME_EVENT_INSTANCE_TITLE ), ctTime.GetYear(), ctTime.GetMonth(), ctTime.GetDay() );

		pWndInstantMsg->AddPostMessage( lpString );						
	#ifdef __INTERNALSERVER		
		pWndInstantMsg->m_timer.Set( MIN(5) );
	#else //__INTERNALSERVER
		pWndInstantMsg->m_timer.Set( MIN(30) );
	#endif //__INTERNALSERVER
	}
}
#endif	// __EVENT_1101

#ifdef __TRAFIC_1218
void CDPClient::OnTraficLog( CAr & ar )
{
	BEFORESENDSOLE( ar2, PACKETTYPE_TRAFIC_LOG, DPID_UNKNOWN );
	m_traficLog.Serialize( ar2 );
	SEND( ar2, this, DPID_SERVERPLAYER );
}
#endif	// __TRAFIC_1218

#ifdef __EVE_MINIGAME
void CDPClient::SendKawibawiboStart()
{
	BEFORESENDSOLE( ar, PACKETTYPE_KAWIBAWIBO_START, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendKawibawiboGetItem()
{
	BEFORESENDSOLE( ar, PACKETTYPE_KAWIBAWIBO_GETITEM, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendReassembleStart(OBJID* pdwItemId, int count)
{
	BEFORESENDSOLE( ar, PACKETTYPE_REASSEMBLE_START, DPID_UNKNOWN );

	for(int i=0; i<count; i++)
		ar << pdwItemId[i];

	SEND( ar, this, DPID_SERVERPLAYER );
}	

void CDPClient::SendReassembleOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_REASSEMBLE_OPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendAlphabetOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_ALPHABET_OPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}	

void CDPClient::SendAlphabetStart(OBJID* pdwItemId, int count, int nQuestionID)
{
	BEFORESENDSOLE( ar, PACKETTYPE_ALPHABET_START, DPID_UNKNOWN );

	ar << nQuestionID;
	for(int i=0; i<count; i++)
		ar << pdwItemId[i];
	
	SEND( ar, this, DPID_SERVERPLAYER );
}	

void CDPClient::SendFiveSystemOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_FIVESYSTEM_OPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendFiveSystemBet( int nBetNum, int nBetPenya )
{
	BEFORESENDSOLE( ar, PACKETTYPE_FIVESYSTEM_BET, DPID_UNKNOWN );
	
	ar << nBetNum;
	ar << nBetPenya;
	
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendFiveSystemStart()
{
	BEFORESENDSOLE( ar, PACKETTYPE_FIVESYSTEM_START, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendFiveSystemDestroyWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_FIVESYSTEM_DESTROYWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}
#endif // __EVE_MINIGAME

void CDPClient::SendUltimateMakeGem( OBJID objItemId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ULTIMATE_MAKEGEM, DPID_UNKNOWN );
	ar << objItemId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendUltimateSetGem( OBJID objItemWeapon, OBJID objItemGem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ULTIMATE_SETGEM, DPID_UNKNOWN );
	ar << objItemWeapon;
	ar << objItemGem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendUltimateRemoveGem( OBJID objItemWeapon, OBJID objItemGem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ULTIMATE_REMOVEGEM, DPID_UNKNOWN );
	ar << objItemWeapon;
	ar << objItemGem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendUltimateEnchantWeapon( OBJID objItemWeapon, OBJID objItemGem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ULTIMATE_ENCHANTWEAPON, DPID_UNKNOWN );
	ar << objItemWeapon;
	ar << objItemGem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

#ifdef __TRADESYS
void CDPClient::SendExchange( int nMMIId, int nListNum )
{
	BEFORESENDSOLE( ar, PACKETTYPE_EXCHANGE, DPID_UNKNOWN );
	ar << nMMIId;
	ar << nListNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}
#endif // __TRADESYS

void CDPClient::OnPetCall( OBJID objid, CAr & ar )
{
	DWORD dwPetId, dwIndex;
	BYTE nPetLevel;
	ar >> dwPetId >> dwIndex >> nPetLevel;
	char szPetName[MAX_PET_NAME]	= { 0,};
	ar.ReadString( szPetName, MAX_PET_NAME );
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		if( pMover->IsActiveMover() )
			pMover->SetPetId( dwPetId );
		else
			pMover->SetPetId( MAKELONG( (WORD)dwIndex, (WORD)nPetLevel ) );
		pMover->m_pet.SetName( szPetName );
	}
}

void CDPClient::OnPetRelease( OBJID objid, CAr & ar )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->PetRelease();

		if( pMover->IsActiveMover() )
		{
			CWndPetStatus* pWndPetStatus = (CWndPetStatus*)g_WndMng.GetWndBase( APP_PET_STATUS );
			if(pWndPetStatus != NULL)
				pWndPetStatus->Destroy();

		//	pWndPetStatus = (CWndPetStatus*)g_WndMng.GetWndBase( APP_BUFFPET_STATUS );
		//	if(pWndPetStatus != NULL)
		//		pWndPetStatus->Destroy();

			CWndPetMiracle* pWndPetMiracle = (CWndPetMiracle*)g_WndMng.GetWndBase( APP_PET_MIRACLE );
			if(pWndPetMiracle != NULL)
				pWndPetMiracle->Destroy();

			if(g_WndMng.m_pWndFoodConfirm != NULL)
				g_WndMng.m_pWndFoodConfirm->Destroy();
		}
		pMover->m_pet.SetName( "" );
	}
}

void CDPClient::SendPetRelease( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PET_RELEASE, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

// dwFeedId : m_dwObjId
void CDPClient::SendUsePetFeed( DWORD dwFeedId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_USE_PET_FEED, DPID_UNKNOWN );
	ar << dwFeedId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendMakePetFeed( DWORD dwMaterialId, short nNum, DWORD dwToolId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_MAKE_PET_FEED, DPID_UNKNOWN );
	ar << dwMaterialId << dwToolId << nNum;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// dwId : pItemElem->m_dwObjId
void CDPClient::SendPetTamerMistake( DWORD dwId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PET_TAMER_MISTAKE, DPID_UNKNOWN );
	ar << dwId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

// dwId : pItemElem->m_dwObjId
void CDPClient::SendPetTamerMiracle( DWORD dwId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PET_TAMER_MIRACLE, DPID_UNKNOWN );
	ar << dwId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendFeedPocketInactive( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_FEED_POCKET_INACTIVE, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnPetLevelup( OBJID objid, CAr & ar )
{
	// 他
	DWORD dwPetId;
	ar >> dwPetId;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->SetPetId( dwPetId );
		pMover->PetLevelup();
	}
}

void CDPClient::OnPetSetExp( OBJID objid, CAr & ar )
{
	// 自
	DWORD dwExp;
	ar >> dwExp;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) && pMover->IsActiveMover() )
	{
		CPet* pPet	= pMover->GetPet();
		if( pPet )
		{
			pPet->SetExp( dwExp );
			if( pPet->GetLevel() == PL_EGG )
			{
				if(pPet->GetExp() == MAX_PET_EGG_EXP)
					g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_PETEGG_HATCH) ) );
			}
			else
			{
				if(pPet->GetExp() == MAX_PET_EXP )
					g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_PETEGG_HATCH) ) );
			}
		}
	}
}

void CDPClient::OnPet( OBJID objid, CAr & ar )
{
	CPet pet;
	BYTE nPetLevelup;
	ar >> pet >> nPetLevelup;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) && pMover->IsActiveMover() )
	{
		CItemElem* pItemElem	= pMover->GetPetItem();
		if( pItemElem )
		{
			SAFE_DELETE( pItemElem->m_pPet );
			pItemElem->m_pPet	= new CPet;
			*( pItemElem->m_pPet )	= pet;
			pMover->m_pet.SetLevelup( nPetLevelup );
			if( nPetLevelup != 0 && pMover->m_pet.GetObj() )
			{
				pItemElem->m_dwItemId	= pItemElem->m_pPet->GetItemId();
				pItemElem->SetTexture();
				pMover->m_pet.GetObj()->Delete();
				pMover->m_pet.SetObj( NULL );
			}
		}
	}
}

void CDPClient::OnPetState( OBJID objid, CAr & ar )
{
	// 自
	DWORD dwPetId, dwExp;
	WORD wLife, wEnergy;
	ar >> dwPetId >> wLife >> wEnergy >> dwExp;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) && pMover->IsActiveMover() )
	{
		CItemElem* pItemElem	= pMover->GetItemId( dwPetId );
		if( pItemElem && pItemElem->IsPet() && pItemElem->m_pPet )
		{
			WORD wOldLife	= pItemElem->m_pPet->GetLife();

			pItemElem->m_pPet->SetLife( wLife );
			pItemElem->m_pPet->SetEnergy( wEnergy );
			pItemElem->m_pPet->SetExp( dwExp );

			if( wOldLife > wLife )
			{
				// 신찬 : 펫 사망 효과
			}
			else if( wOldLife < wLife )
			{
				// 회복 : wLife - wOldLife
				CString strTemp;
				strTemp.Format( prj.GetText(TID_GAME_PET_LIFE), wLife - wOldLife );
				g_WndMng.OpenMessageBox( strTemp );
			}
		}
	}
}

void CDPClient::OnPetFeed( OBJID objid, CAr & ar )
{
	WORD wEnergy;
	ar >> wEnergy;
	
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		CPet* pPet	= pMover->GetPet();
		if( pPet )
		{
			pPet->SetEnergy( wEnergy );
			// 효과 출력
		}
	}
}

void CDPClient::OnPetFoodMill( OBJID objid, CAr & ar )
{
	int nResult, nCount;
	ar >> nResult >> nCount;

	CWndPetFoodMill* pWndMill = (CWndPetFoodMill*)g_WndMng.GetWndBase( APP_PET_FOODMILL );
	if(pWndMill != NULL)
	{
		pWndMill->ReceiveResult(nResult, nCount);
		if( pWndMill->m_dwObjId != NULL_ID )
			pWndMill->Destroy();
	}
}

void CDPClient::OnSetSpeedFactor( OBJID objid, CAr & ar )
{
	FLOAT fSpeedFactor;
	ar >> fSpeedFactor;

	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
		pMover->SetSpeedFactor( fSpeedFactor );
}

void CDPClient::SendModifyStatus(int nStrCount, int nStaCount, int nDexCount, int nIntCount)
{
	BEFORESENDSOLE( ar, PACKETTYPE_MODIFY_STATUS, DPID_UNKNOWN );
	
	ar << nStrCount << nStaCount << nDexCount << nIntCount;
	
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendQueryStartCollecting( void )
{
	SendHdr( PACKETTYPE_QUERY_START_COLLECTING );
}

void CDPClient::SendQueryStopCollecting( void )
{
	SendHdr( PACKETTYPE_QUERY_STOP_COLLECTING );
}

void CDPClient::OnStartCollecting( OBJID objid )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->StartCollecting();
		if( pMover->IsActiveMover() )
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_START_COLLECTING ), NULL, prj.GetTextColor( TID_GAME_START_COLLECTING ) );
			// 채집창 버튼을 'start'으로
			g_WndMng.m_pWndCollecting->SetButtonCaption(true);
		}
	}
}

void CDPClient::OnRestartCollecting( OBJID objid, CAr & ar )
{
	DWORD dwItemId;
	ar >> dwItemId;
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) && pMover->IsActiveMover() )
	{
		ItemProp* pItemProp	= prj.GetItemProp( dwItemId );
		if( pItemProp )
		{
			char lpText[200]	= { 0,};
			sprintf( lpText, prj.GetText( TID_GAME_REAPITEM ), pItemProp->szName );
			g_WndMng.PutString( lpText, NULL, prj.GetTextColor( TID_GAME_REAPITEM ) );
			// 남은 채집 시간 초기화
			pMover->m_nCltTime = 0;
		}
	}
}

void CDPClient::OnStopCollecting( OBJID objid )
{
	CMover* pMover	= prj.GetMover( objid );
	if( IsValidObj( pMover ) )
	{
		pMover->StopCollecting();
		if( pMover->IsActiveMover() )
		{
			// 채집창이 있다면 버튼을 'stop'으로
			if(g_WndMng.m_pWndCollecting)
				g_WndMng.m_pWndCollecting->SetButtonCaption(false);
		}
	}
}

void CDPClient::SendGC1to1TenderOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TENDEROPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}
void CDPClient::SendGC1to1Tender( int nPenya )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TENDER, DPID_UNKNOWN );
	ar << nPenya;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1TenderCancel()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TENDERCANCEL, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1TenderFailed()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TENDERFAILED, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1TenderView()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TENDERVIEW, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1MemberLienUpOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_MEMBERLINEUPOPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1MemberLienUp(std::vector<u_long>& vecMemberId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_MEMBERLINEUP, DPID_UNKNOWN );
	ar << vecMemberId.size();
	for( int i=0; i<(int)( vecMemberId.size() ); i++ )
		ar << vecMemberId[i];
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1TeleportToNPC()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TELEPORTTONPC, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGC1to1TeleportToStage()
{
	BEFORESENDSOLE( ar, PACKETTYPE_GC1TO1_TELEPORTTOSTAGE, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnGC1to1TenderOpenWnd( CAr & ar )
{
	int nPenya;
	ar >> nPenya;
	ar >> g_GuildCombat1to1Mng.m_nJoinPenya;
	
	SAFE_DELETE(g_WndMng.m_pWndGuildCombat1to1Offer);
	g_WndMng.m_pWndGuildCombat1to1Offer = new CWndGuildCombat1to1Offer(
		CWndGuildCombat1to1Offer::CombatType::GC1to1
	);

	g_WndMng.m_pWndGuildCombat1to1Offer->Initialize();
	if( nPenya == 0 ) //한번도 입찰한 금액이 없을 경우
	{
		g_WndMng.m_pWndGuildCombat1to1Offer->SetReqGold( nPenya );
		g_WndMng.m_pWndGuildCombat1to1Offer->SetBackupGold( g_GuildCombat1to1Mng.m_nJoinPenya );
		g_WndMng.m_pWndGuildCombat1to1Offer->SetGold( g_GuildCombat1to1Mng.m_nJoinPenya );
	}
	else
	{
		g_WndMng.m_pWndGuildCombat1to1Offer->SetReqGold( nPenya );
		g_WndMng.m_pWndGuildCombat1to1Offer->SetBackupGold( nPenya );
		g_WndMng.m_pWndGuildCombat1to1Offer->SetGold( nPenya );			
	}
}

void CDPClient::OnGC1to1TenderView( CAr & ar )
{
	time_t t;
	int nPenya, nRanking, nSize;
	u_long ulTemp;

	ar >> nPenya;
	ar >> nRanking;
	ar >> t;
	ar >> nSize;

	SAFE_DELETE(g_WndMng.m_pWndGuildWarState);
	g_WndMng.m_pWndGuildWarState = new CWndGuildCombatState(1);

	if(g_WndMng.m_pWndGuildWarState)
	{
		g_WndMng.m_pWndGuildWarState->Initialize();
		g_WndMng.m_pWndGuildWarState->SetTime(t);
		g_WndMng.m_pWndGuildWarState->SetRate(nRanking);
		g_WndMng.m_pWndGuildWarState->SetGold(nPenya);
		
		for( int i=0; i<nSize; i++ )
		{
			ar >> ulTemp;
			g_WndMng.m_pWndGuildWarState->InsertGuild(g_GuildMng.GetGuild(ulTemp)->m_szGuild, "1", i+1);
		}
	}
}

void CDPClient::OnGC1to1NowState( CAr & ar )
{
	int nState;
	int nTime;

	ar >> nState >> nTime;

	g_GuildCombat1to1Mng.m_nState = nState;

	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	if( pWndWorld == NULL )
	{
		Error( "CDPClient::OnGuildCombatNextTimeState()에 World가 없다" );
		return;
	}
	
	BOOL bTimeSet = FALSE;
	switch( nState )
	{
		case CGuildCombat1to1Mng::GC1TO1_OPEN:
			{
				CGuild* pGuild = g_pPlayer->GetGuild();
				if(pGuild)
				{
					CGuildMember* pGuildMemberl = pGuild->GetMember( g_pPlayer->m_idPlayer );

					if( pGuildMemberl )
					{
						if( pGuildMemberl->m_nMemberLv == GUD_MASTER )
						{
							bTimeSet = TRUE;
							sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_READY_TIME) );
							pWndWorld->m_bGuildCombat1to1Wait = FALSE;
						}
					}
				}
			}
			break;
		case CGuildCombat1to1Mng::GC1TO1_ENTRANCE:
			{
				// 초기화
				g_GuildCombat1to1Mng.m_nMyGuildCount = 0;
				g_GuildCombat1to1Mng.m_nVsGuildCount = 0;
				g_GuildCombat1to1Mng.m_nGuildCombat1to1Guild = 0;
				g_GuildCombat1to1Mng.ClearGuildCombat1to1Players();

				bTimeSet = TRUE;
				sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_ENTRANCE_TIME) );	

				SAFE_DELETE( g_WndMng.m_pWndGuildCombatInfoMessageBox );
				g_WndMng.m_pWndGuildCombatInfoMessageBox = new CGuildCombatInfoMessageBox(1);
	
				g_WndMng.m_pWndGuildCombatInfoMessageBox->Initialize();
				LPCTSTR strMsg = prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_TELEPORT_MSG);
				g_WndMng.m_pWndGuildCombatInfoMessageBox->SetString(strMsg);

				int nSize;
				u_long nPlayerId;
				u_long nGuildId;
			
				ar >> nSize;

				for( int i=0; i<nSize; i++ )
				{
					ar >> nPlayerId;
					g_GuildCombat1to1Mng.AddGuildCombat1to1Player(nPlayerId);
				}
				
				ar >> nGuildId;
				g_GuildCombat1to1Mng.SetGuildCombat1to1Guild(nGuildId);
				pWndWorld->m_bGuildCombat1to1Wait = FALSE;
			}
			break;
		case CGuildCombat1to1Mng::GC1TO1WAR_WAIT:
			{
				int nCount=0;
				CString strmsg;

				bTimeSet = TRUE;
				ar >> nCount;
				nCount++;

				strmsg.Format(prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_WAIT_TIME), nCount);
				sprintf( pWndWorld->m_szGuildCombatStr, strmsg );	// 전투대기
				pWndWorld->m_bGuildCombat1to1Wait = TRUE;
			}
			break;
		case CGuildCombat1to1Mng::GC1TO1WAR_WAR:
			{
				bTimeSet = TRUE;
				sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_WAR_TIME) );	// 전투시작 - 전투종료 시간 표시
				pWndWorld->m_bGuildCombat1to1Wait = FALSE;
			}
			break;
		case CGuildCombat1to1Mng::GC1TO1WAR_CLOSEWAIT:
			{
				bTimeSet = TRUE;
				sprintf( pWndWorld->m_szGuildCombatStr, prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_WAR_CLOSEWAIT) );	// 1:1 길드대전 종료 안내
				pWndWorld->m_bGuildCombat1to1Wait = FALSE;
			}
			break;
		case CGuildCombat1to1Mng::GC1TO1_CLOSE:
		case CGuildCombat1to1Mng::GC1TO1WAR_CLOSE:
		case CGuildCombat1to1Mng::GC1TO1_WAR:
			{
				sprintf( pWndWorld->m_szGuildCombatStr, "" );
				pWndWorld->m_dwGuildCombatTime = 0xffffffff;
				pWndWorld->m_bGuildCombat1to1Wait = FALSE;
			}
			break;
	}

	if( bTimeSet )
		pWndWorld->m_dwGuildCombatTime = GetTickCount() + nTime;
}


void CDPClient::OnGC1to1WarResult( CAr & ar )
{
	int m_nState, nWinCount0, nWinCount1;
	u_long uIdPlayer;

	ar >> m_nState >> uIdPlayer >> nWinCount0 >> nWinCount1;

	g_GuildCombat1to1Mng.SetGuildCombat1to1State(m_nState, uIdPlayer, nWinCount0, nWinCount1);
}

void CDPClient::OnGC1to1MemberLineUpOpenWnd( CAr & ar )
{
	ar >> g_GuildCombat1to1Mng.m_nMinJoinPlayerLevel;
	ar >> g_GuildCombat1to1Mng.m_nMinJoinPlayer;
	ar >> g_GuildCombat1to1Mng.m_nMaxJoinPlayer;

	SAFE_DELETE(g_WndMng.m_pWndGuildCombat1to1Selection);
	g_WndMng.m_pWndGuildCombat1to1Selection = new CWndGuildCombat1to1Selection;
	g_WndMng.m_pWndGuildCombat1to1Selection->Initialize();	

	std::vector<u_long> selection;
	ar >> selection;


	g_WndMng.m_pWndGuildCombat1to1Selection->SetSelection(selection);
}


void CDPClient::SendReqGuildBankLogList( BYTE byListType )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILDLOG_VIEW, DPID_UNKNOWN );
	ar << byListType;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnGuildBankLogList( CAr & ar )
{
	BYTE byListType;
	short nLogCount = 0;
	char szPlayer[42];
	long nDate = 0;
	long nItem = 0;
	long nAbilityOption = 0;
	long nItem_count = 0;
	long nPenya = 0;
	std::vector < CString >* pvString = NULL;

	CWndGuildBankLog* pWndGuildBankLog = (CWndGuildBankLog*)g_WndMng.GetWndBase(APP_GUILD_BANK_LOG);
	CGuild* pGuild = g_pPlayer->GetGuild();

	if(pWndGuildBankLog == NULL || pGuild == NULL)
		return;

	ar >> byListType;
	ar >> nLogCount;
	
	switch( byListType )
	{
		case GI_LOG_VIEW_ITEM_ADD:
			{
				pvString = &pWndGuildBankLog->m_wndAddItemLog.m_vLogList;
			}
			break;
		case GI_LOG_VIEW_ITEM_REMOVE:
			{
				pvString = &pWndGuildBankLog->m_wndRemoveItemLog.m_vLogList;
			}
			break;
		case GI_LOG_VIEW_MONEY_ADD:
			{
				pvString = &pWndGuildBankLog->m_wndReceivePenyaLog.m_vLogList;
			}
			break;
		case GI_LOG_VIEW_MONEY_REMOVE:
			{
				pvString = &pWndGuildBankLog->m_wndInvestPenyaLog.m_vLogList;
			}
			break;
	}
	
	char szDate[256] = { 0 };			
	for(short i=0; i<nLogCount; i++)
	{
		CString strLog;
		ar.ReadString(szPlayer, 42);
		ar >> nDate;
		if(byListType == GI_LOG_VIEW_ITEM_ADD || byListType == GI_LOG_VIEW_ITEM_REMOVE)
		{
			ar >> nItem;
			ar >> nAbilityOption;
			ar >> nItem_count;
			CTime cTime( (time_t)nDate );		
			sprintf( szDate, "%.2d%.2d%.2d %.2d:%.2d   ", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute() );

			const ItemProp * pItemProp = prj.GetItemProp(nItem);
			
			CString strCharName = strings::CStringMaxSize(szPlayer, 9);

			CString strItemName;
			if(pItemProp->dwReferStat1 == WEAPON_ULTIMATE)
				strItemName.Format("(U)%s", pItemProp->szName);
			else
				strItemName.Format("%s", pItemProp->szName);

			BOOL bGuildCombatCloak = FALSE;

			if( nItem == II_ARM_S_CLO_CLO_DRAGON1 || nItem == II_ARM_S_CLO_CLO_DRAGON2 ||
				nItem == II_ARM_S_CLO_CLO_DRAGON3 || nItem == II_ARM_S_CLO_CLO_DRAGON4 )
				bGuildCombatCloak  = TRUE;

			if( pItemProp->dwItemKind3 == IK3_CLOAK  )	// 
			{
				CGuild *pGuild = g_GuildMng.GetGuild( g_pPlayer->m_idGuild );
				if( pGuild && bGuildCombatCloak == FALSE )
					strItemName.Format( prj.GetText( TID_GUILD_CLOAK ), pGuild->m_szGuild );
				else
					strItemName.Format( "%s", pItemProp->szName );
			}

			strings::ReduceSize(strItemName, 25);

			if(nAbilityOption > 0)
			{
				CString strAblility;
				strAblility.Format(" +%d", nAbilityOption);
				strItemName += strAblility;
			}

			if(pItemProp)
			{
				if(byListType == GI_LOG_VIEW_ITEM_ADD)
					strLog.Format(prj.GetText(TID_GAME_GUILDBANK_LOG_ITEM_ADD), szDate, strCharName, strItemName, nItem_count);
				else if(byListType == GI_LOG_VIEW_ITEM_REMOVE)
					strLog.Format(prj.GetText(TID_GAME_GUILDBANK_LOG_ITEM_REMOVE), szDate, strCharName, strItemName, nItem_count);
			}
	
			if(pvString)
				pvString->push_back(strLog);
		}
		else if(byListType == GI_LOG_VIEW_MONEY_ADD || byListType == GI_LOG_VIEW_MONEY_REMOVE)
		{
			ar >> nPenya;

			CTime cTime( (time_t)nDate );		
			sprintf( szDate, "%.2d%.2d%.2d %.2d:%.2d   ", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute() );

			if(byListType == GI_LOG_VIEW_MONEY_ADD)
				strLog.Format(prj.GetText(TID_GAME_GUILDBANK_LOG_PENYA_ADD), szDate, szPlayer, nPenya);
			else if(byListType == GI_LOG_VIEW_MONEY_REMOVE)
				strLog.Format(prj.GetText(TID_GAME_GUILDBANK_LOG_PENYA_REMOVE), szDate, szPlayer, nPenya);

			if(pvString)
				pvString->push_back(strLog);
		}
	}

	pWndGuildBankLog->UpdateScroll();
}

void CDPClient::OnSealChar(CAr & ar) {
	std::vector<CWndSealCharSelect::Target> targets;
	targets.reserve(3);

	short nCount = 0; ar >> nCount;

	for (short i = 0; i < nCount; i++) {
		CWndSealCharSelect::Target target;
		ar >> target.slot >> target.id >> target.name;
		targets.emplace_back(target);
	}

	if (!g_pPlayer) return;

	const auto thisPlayer = std::ranges::find_if(targets,
		[](const CWndSealCharSelect::Target & target) {
			return target.id == g_pPlayer->m_idPlayer;
		});

	targets.erase(thisPlayer);
	
	CWndSealCharSelect::OpenOrResetWindow(std::move(targets));
}

void CDPClient::OnSealCharGet( CAr & ar )
{
	if( g_pPlayer )
	{
		g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_DIAG_0023), MB_OK, TRUE );	//서버로부터 접속이 해제되었습니다.		  
	}
}

void CDPClient::SendReqHonorList() {
	SendPacket<PACKETTYPE_HONOR_LIST_REQ>();
}

void CDPClient::SendReqHonorTitleChange( int nChange ) {
	SendPacket<PACKETTYPE_HONOR_CHANGE_REQ, int>(nChange);
}

void CDPClient::OnHonorListAck( CAr & ar )
{
	for(int i = 0 ; i<MAX_HONOR_TITLE ; i++) {
		int honorTitle;
		ar >> honorTitle;

		if (g_pPlayer) g_pPlayer->m_aHonorTitle[i] = honorTitle;

		if (CTitleManager::Instance()->UpdateEarned(i, honorTitle)) {
			g_WndMng.PutString(TID_GAME_GET_TITLE, CTitleManager::Instance()->GetTitle(i));
		}
	}

	if (CWndCharacter * pWndBase = g_WndMng.GetWndBase<CWndCharacter>(APP_CHARACTER3)) {
		pWndBase->m_wndHonor.RefreshList();
	}
}

void CDPClient::OnHonorChangeAck(OBJID objid, CAr & ar) {
	int nChange; ar >> nChange;

	CMover * pMover = prj.GetMover(objid);
	if (IsValidObj(pMover)) {
		pMover->m_nHonor = nChange;
		pMover->SetTitle(CTitleManager::Instance()->GetTitle(pMover->m_nHonor));
	}
}


void CDPClient::OnPocketAttribute( CAr & ar )
{
	int nAttribute, nPocket, nData;
	ar >> nAttribute >> nPocket >> nData;

	if(g_pPlayer)
	{
		g_pPlayer->m_Pocket.SetAttribute( nAttribute, nPocket, nData );
		
		if (CWndBagEx * pWnd = g_WndMng.GetWndBase<CWndBagEx>(APP_BAG_EX)) {
			pWnd->InitItem();
		}
	}
}

void CDPClient::OnPocketAddItem( CAr & ar )
{
	int nPocket;
	CItemElem item;
	ar >> nPocket >> item;

	if(g_pPlayer)
		g_pPlayer->m_Pocket.Add( nPocket, &item );
}

void CDPClient::OnPocketRemoveItem( CAr & ar )
{
	int nPocket, nItem;
	short nNum;
	ar >> nPocket >> nItem >> nNum;

	if(g_pPlayer)
		g_pPlayer->m_Pocket.RemoveAtId( nPocket, nItem, nNum );
}

#ifdef __NPC_BUFF
void CDPClient::SendNPCBuff( const char* szKey )
{
	BEFORESENDSOLE( ar, PACKETTYPE_NPC_BUFF, DPID_UNKNOWN );
	ar.WriteString( szKey );
	SEND( ar, this, DPID_SERVERPLAYER );	
}
#endif // __NPC_BUFF

#ifdef __EVENTLUA_COUPON
void CDPClient::OnEventCoupon( CAr & ar )
{
	int nCoupon;
	ar >> nCoupon;

	CWndInstantMsg* pWndInstantMsg = g_WndMng.OpenInstantMsg( "Mail" );
	if( pWndInstantMsg )
	{
		char lpString[260]	= { 0, };
		//sprintf( lpString, prj.GetText( TID_GAME_EVENT_COUPON ), nCoupon );
		sprintf( lpString, prj.GetText( TID_GAME_EVENT_COUPON ) );

		pWndInstantMsg->AddPostMessage( lpString );						
		pWndInstantMsg->m_timer.Set( MIN(5) );
	}
}
#endif // __EVENTLUA_COUPON

#ifdef __JEFF_11
void	CDPClient::SendQuePetResurrection( int nItem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUE_PETRESURRECTION, DPID_UNKNOWN );
	ar << nItem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void	CDPClient::OnQuePetResurrectionResult( CAr & ar )
{
	BOOL bResult;
	ar >> bResult;
	if(bResult)
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_PETTRADE_OK) ) );
	else
		g_WndMng.OpenMessageBox( _T( prj.GetText(TID_GAME_PETTRADE_FAIL) ) );
}
#endif	// __JEFF_11

void	CDPClient::SendSecretRoomTender( int nPenya )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_TENDER, DPID_UNKNOWN );
	ar << nPenya;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void	CDPClient::SendSecretRoomTenderCancelReturn()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_TENDERCANCELRETURN, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSecretRoomLineUpMember(std::vector<DWORD> vecLineUpMember )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_LINEUPMEMBER, DPID_UNKNOWN );
	ar << vecLineUpMember.size();
	for( int i=0; i<(int)( vecLineUpMember.size() ); i++ )
		ar << vecLineUpMember[i];
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSecretRoomMngState( CAr & ar )
{
	DWORD dwTime;
	ar >> CSecretRoomMng::GetInstance()->m_nState;
	ar >> dwTime;

	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

	if( pWndWorld == NULL )
	{
		Error( "CDPClient::OnGuildCombatNextTimeState()에 World가 없다" );
		return;
	}

	BOOL bTimeSet = FALSE;

	CString strTemp;
	switch( CSecretRoomMng::GetInstance()->m_nState )
	{
		case SRMNG_LINEUP :
			{
				CGuild* pGuild = g_pPlayer->GetGuild();
				if(pGuild)
				{
					CGuildMember* pGuildMemberl = pGuild->GetMember( g_pPlayer->m_idPlayer );

					if( pGuildMemberl )
					{
						if( pGuildMemberl->m_nMemberLv == GUD_MASTER )
						{
							bTimeSet = TRUE;
							sprintf( pWndWorld->m_szSecretRoomStr, prj.GetText(TID_GAME_SECRETROOM_READY_TIME) );
						}
					}
				}
				strTemp = prj.GetText(TID_GAME_SECRETROOM_MSG_SELECTION); //"지금 부터 ""비밀의 방 참가자 구성""을 할 수 있습니다.";		
			}
			break;
		case SRMNG_ENTRANCE :
			{
				strTemp = prj.GetText(TID_GAME_SECRETROOM_MSG_ENTRANCE); //"지금 부터 비밀의 방에 입장할 수 있습니다.";

				SAFE_DELETE( g_WndMng.m_pWndSecretRoomMsg );
				g_WndMng.m_pWndSecretRoomMsg = new CWndSecretRoomInfoMsgBox();

				if( g_WndMng.m_pWndSecretRoomMsg )
				{
					g_WndMng.m_pWndSecretRoomMsg->Initialize();
					CString strMsg;
					strMsg.Format("%s", prj.GetText(TID_GAME_SECRETROOM_TELEPORT_MSG));
					g_WndMng.m_pWndSecretRoomMsg->SetString(strMsg);
				}

				bTimeSet = FALSE;
			}
			break;
		case SRMNG_WARWAIT :
			{
				bTimeSet = TRUE;
				sprintf( pWndWorld->m_szSecretRoomStr, prj.GetText(TID_GAME_SECRETROOM_START_TIME) );

				strTemp = prj.GetText(TID_GAME_SECRETROOM_MSG_WARWAIT); //"잠시 후 비밀의 방이 시작될 예정입니다. 전투를 준비하세요.";
			}
			break;
		case SRCONT_WAR :
			{
				bTimeSet = TRUE;
				sprintf( pWndWorld->m_szSecretRoomStr, prj.GetText(TID_GAME_SECRETROOM_WAR_TIME) );
				strTemp = prj.GetText(TID_GAME_SECRETROOM_MSG_START); //"비밀의 방 전투가 시작되었습니다!";				
			}
			break;
		case SRCONT_CLOSEWAIT :
			{
				bTimeSet = TRUE;
				sprintf( pWndWorld->m_szSecretRoomStr, prj.GetText(TID_GAME_SECRETROOM_CLOSE_TIME) );
				strTemp = prj.GetText(TID_GAME_SECRETROOM_MSG_END); //"비밀의 방 전투가 종료되었습니다!";
			}
			break;
		default:
			{
				sprintf( pWndWorld->m_szSecretRoomStr, "" );
				CSecretRoomMng::GetInstance()->m_dwRemainTime = 0;
			}
			break;
	}

	if(bTimeSet)
		CSecretRoomMng::GetInstance()->m_dwRemainTime = dwTime + GetTickCount();

	if( ::GetLanguage() != LANG_JAP )
		g_Caption1.AddCaption( strTemp, pWndWorld->m_pFontAPICaption );// CWndBase::m_Theme.m_pFontCaption );
	else
		g_Caption1.AddCaption( strTemp, NULL );// CWndBase::m_Theme.m_pFontCaption );
}

void CDPClient::OnSecretRoomInfo( CAr & ar )
{
	BYTE nType, nContinent;
	int nSize;
	ar >> nContinent;
	ar >> nType;
	
	const auto it = CSecretRoomMng::GetInstance()->m_mapSecretRoomContinent.find(nContinent);
	CSecretRoomContinent* pSRCont = it->second;

	switch( nType )
	{
		// 같은 대륙에 참여한 길드 목록을 받아온다.	
		case SECRETROOM_GUILDLIST :
			{
				if( pSRCont )
				{
					pSRCont->m_vecSecretRoomTender.clear();
					CSecretRoomMng::GetInstance()->m_pSRCont = pSRCont;	// 내가 속한 대륙을 멤버로 설정한다.
				}
	
				ar >> nSize;
				for( int i=0; i<nSize; i++ )
				{
					__SECRETROOM_TENDER srTender;
					ar >> srTender.dwGuildId;
					ar >> srTender.nWarState;
					ar >> srTender.nKillCount;
					if( pSRCont )
						pSRCont->m_vecSecretRoomTender.push_back( srTender );

					if( g_pPlayer->GetGuild() && srTender.dwGuildId == g_pPlayer->GetGuild()->m_idGuild )
					{
						int nLineupSize;
						DWORD dwLineupId;
						ar >> nLineupSize;

						for(int j=0; j<nLineupSize; j++)
						{
							ar >> dwLineupId;
							pSRCont->m_vecSecretRoomTender[i].vecLineUpMember.push_back( dwLineupId );
						}
					}
				}
				ar >> nSize;
				for( int i=0; i<nSize; i++ )
				{
					int nMonsterType, nNum;
					ar >> nMonsterType;
					ar >> nNum;
					CSecretRoomMng::GetInstance()->m_mapMonsterNum.emplace( nMonsterType, nNum );
				}
			}
			break;
		case SECRETROOM_KILLCOUNT :
			{
				int nIndex, nKillConunt;
				ar >> nIndex;
				ar >> nKillConunt;
				if( pSRCont )
				{
					ASSERT( nIndex < (int)( pSRCont->m_vecSecretRoomTender.size() ) );
					pSRCont->m_vecSecretRoomTender[nIndex].nKillCount = nKillConunt;
				}
			}
			break;

		case SECRETROOM_WARSTATE :
			{
				int nIndex, nWarState;
				ar >> nIndex;
				ar >> nWarState;
				if( pSRCont )
				{
					ASSERT( nIndex < (int)( pSRCont->m_vecSecretRoomTender.size() ) );
					pSRCont->m_vecSecretRoomTender[nIndex].nWarState = nWarState;
					pSRCont->m_vecSecretRoomTender[nIndex].nKillCount = 0;
				}

				CString strTemp;
				switch( nWarState )
				{
					case MONSTER_WIN :
						strTemp.Empty();
						break;
					case MONSTER_FAILED :
						ASSERT( nIndex < (int)( pSRCont->m_vecSecretRoomTender.size() ) );
						strTemp.Empty();
						CGuild* pGuild = g_GuildMng.GetGuild( pSRCont->m_vecSecretRoomTender[nIndex].dwGuildId );
						if( pGuild )
							strTemp.Format( prj.GetText(TID_GAME_SECRETROOM_GUILD_FAILED), pGuild->m_szGuild );
						//strTemp.Format( "%s 길드가 탈락했습니다!", g_GuildMng.GetGuild( pSRCont->m_vecSecretRoomTender[nIndex].dwGuildId )->m_szGuild );
						break;
				}		
				
				CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );

				if( pWndWorld == NULL )
				{
					Error( "CDPClient::OnGuildCombatNextTimeState()에 World가 없다" );
					return;
				}
				
				if( !strTemp.IsEmpty() )
				{
					if( ::GetLanguage() != LANG_JAP )
						g_Caption1.AddCaption( strTemp, pWndWorld->m_pFontAPICaption );// CWndBase::m_Theme.m_pFontCaption );
					else
						g_Caption1.AddCaption( strTemp, NULL );// CWndBase::m_Theme.m_pFontCaption );
				}
			}
			break;
	}
}

void CDPClient::SendSecretRoomTenderOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_TENDEROPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSecretRoomTenderOpenWnd( CAr & ar )
{
	int nTenderPenya;

	ar >> CSecretRoomMng::GetInstance()->m_nMinPenya;
	ar >> nTenderPenya;

	SAFE_DELETE(g_WndMng.m_pWndSecretRoomOffer);
	g_WndMng.m_pWndSecretRoomOffer = new CWndSecretRoomOffer();

	if(g_WndMng.m_pWndSecretRoomOffer)
		g_WndMng.m_pWndSecretRoomOffer->Initialize();

	if( nTenderPenya == 0 ) //한번도 입찰한 금액이 없을 경우
	{
		g_WndMng.m_pWndSecretRoomOffer->SetReqGold( nTenderPenya );
		g_WndMng.m_pWndSecretRoomOffer->SetBackupGold( CSecretRoomMng::GetInstance()->m_nMinPenya );
		g_WndMng.m_pWndSecretRoomOffer->SetGold( CSecretRoomMng::GetInstance()->m_nMinPenya );
	}
	else
	{
		g_WndMng.m_pWndSecretRoomOffer->SetReqGold( nTenderPenya );
		g_WndMng.m_pWndSecretRoomOffer->SetBackupGold( nTenderPenya );
		g_WndMng.m_pWndSecretRoomOffer->SetGold( nTenderPenya );			
	}
}

void CDPClient::SendSecretRoomLineUpOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_LINEUPOPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSecretRoomLineUpOpenWnd( CAr & ar )
{
	ar >> CSecretRoomMng::GetInstance()->m_nMinGuildMemberNum;
	ar >> CSecretRoomMng::GetInstance()->m_nMaxGuildMemberNum;

	SAFE_DELETE(g_WndMng.m_pWndSecretRoomSelection);
	g_WndMng.m_pWndSecretRoomSelection = new CWndSecretRoomSelection();
	g_WndMng.m_pWndSecretRoomSelection->Initialize();

	int nSize; ar >> nSize;
	std::vector<u_long> lineup;
	for (int i = 0; i < nSize; i++) {
		ar >> lineup.emplace_back();
	}

	g_WndMng.m_pWndSecretRoomSelection->ResetLineup(lineup);
}

void CDPClient::SendSecretRoomEntrance()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_ENTRANCE, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSecretRoomTeleportToNPC()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_TELEPORTTONPC, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSecretRoomTenderView()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_TENDERVIEW, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSecretRoomTenderView( CAr & ar )
{
	int nTenderPenya;
	int nRanking;
	time_t t;
	int nSize;

	ar >> nTenderPenya;
	ar >> nRanking;
	ar >> t;
	ar >> nSize;

	SAFE_DELETE(g_WndMng.m_pWndSecretRoomOfferState);
	g_WndMng.m_pWndSecretRoomOfferState = new CWndSecretRoomOfferState();

	if(g_WndMng.m_pWndSecretRoomOfferState)
	{
		g_WndMng.m_pWndSecretRoomOfferState->Initialize();
		g_WndMng.m_pWndSecretRoomOfferState->SetTime(t);
		g_WndMng.m_pWndSecretRoomOfferState->SetRate(nRanking);
		g_WndMng.m_pWndSecretRoomOfferState->SetGold(nTenderPenya);

		DWORD dwGuildId;
		int nRank	= 0;
		for( int i=0; i<nSize; i++ )
		{
			ar >> dwGuildId;
			// 삭제된 길드면 출력에서 제외시킨다.	// 康	// 2008-09-18
			if( !g_GuildMng.GetGuild(dwGuildId) )
				continue;
			g_WndMng.m_pWndSecretRoomOfferState->InsertGuild(g_GuildMng.GetGuild(dwGuildId)->m_szGuild, "1", ++nRank );
		}
	}
}

void CDPClient::SendTeleportToSecretRoomDungeon()
{
	BEFORESENDSOLE( ar, PACKETTYPE_SECRETROOM_TELEPORTTODUNGEON, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}


void CDPClient::OnTaxInfo(CAr & ar) {
	ar >> *CTax::GetInstance();

	SAFE_DELETE(g_WndMng.m_pWndSecretRoomChangeTaxRate);
}

void CDPClient::OnTaxSetTaxRateOpenWnd( CAr & ar )
{
	BYTE nCont;
	int nMinTaxRate, nMaxTaxRate;

	ar >> nCont;
	ar >> nMinTaxRate >> nMaxTaxRate;

	SAFE_DELETE(g_WndMng.m_pWndSecretRoomChangeTaxRate);
	g_WndMng.m_pWndSecretRoomChangeTaxRate = new CWndSecretRoomChangeTaxRate;

	if(g_WndMng.m_pWndSecretRoomChangeTaxRate)
	{
		g_WndMng.m_pWndSecretRoomChangeTaxRate->SetDefaultTax(nMinTaxRate, nMaxTaxRate, nCont);
		g_WndMng.m_pWndSecretRoomChangeTaxRate->Initialize();
	}
}

void CDPClient::SendTaxRate( BYTE nCont, int nSalesTaxRate, int nPurchaseTaxRate )
{
	BEFORESENDSOLE( ar, PACKETTYPE_TAX_SET_TAXRATE, DPID_UNKNOWN );
	ar << nCont << nSalesTaxRate << nPurchaseTaxRate;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendTeleportToHeavenTower( int nFloor )
{
	BEFORESENDSOLE( ar, PACKETTYPE_HEAVENTOWER_TELEPORT, DPID_UNKNOWN );
	ar << nFloor;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnElectionAddDeposit( CAr & ar )
{	// 군주 입찰 결과 갱신
	u_long idPlayer;
	__int64 iDeposit;
	time_t tCreate;
	ar >> idPlayer >> iDeposit >> tCreate;

	IElection* pElection	= CCLord::Instance()->GetElection();
	pElection->AddDeposit( idPlayer, iDeposit, tCreate );
}

void CDPClient::OnElectionSetPledge( CAr & ar )
{	// 공약 설정 결과 갱신
	u_long idPlayer;
	char szPledge[CCandidate::nMaxPledgeLen]	= {0, };
	ar >> idPlayer;
	ar.ReadString( szPledge, CCandidate::nMaxPledgeLen );

	IElection* pElection	= CCLord::Instance()->GetElection();
	pElection->SetPledge( idPlayer, szPledge );
}

void CDPClient::OnElectionIncVote( CAr & ar )
{	// 투표 결과 갱신
	u_long idPlayer, idElector;
	ar >> idPlayer >> idElector;

	IElection* pElection	= CCLord::Instance()->GetElection();
	pElection->IncVote(  idPlayer, idElector );
}

void CDPClient::OnElectionBeginCandidacy( CAr & ar )
{	// 입후보 시작 상태로 변경
	IElection* pElection	= CCLord::Instance()->GetElection();
	pElection->BeginCandidacy();
}

void CDPClient::OnElectionBeginVote( CAr & ar )
{	// 투표 시작 상태로 변경
	int nRequirement;
	ar >> nRequirement;

	IElection* pElection	= CCLord::Instance()->GetElection();
	pElection->BeginVote( nRequirement );
}

void CDPClient::OnElectionEndVote( CAr & ar )
{	// 투표 종료 상태로 변경
	u_long idPlayer;
	ar >> idPlayer;

	IElection* pElection	= CCLord::Instance()->GetElection();
	pElection->EndVote( idPlayer );
}

void CDPClient::OnLord( CAr & ar )
{	// 군주 시스템 정보를 수신하여 복원
	ar >> *CCLord::Instance();
}

void CDPClient::OnLEventCreate( CAr & ar )
{	// 군주 이벤트 시작 처리
	CLEComponent pComponent;
	ar >> pComponent;

	ILordEvent* pEvent	= CCLord::Instance()->GetEvent();
	pEvent->AddComponent( pComponent );
}

void CDPClient::OnLEventInitialize( CAr & ar )
{	// 군주 이벤트 초기화 처리
	ILordEvent* pEvent	= CCLord::Instance()->GetEvent();
	pEvent->Initialize();
}

void CDPClient::OnLordSkillTick( CAr & ar )
{	// 군주 스킬 재사용 대기 시간 처리
	CLordSkill* pSkills		= CCLord::Instance()->GetSkills();
	pSkills->ReadTick( ar );
}

void CDPClient::OnLEventTick( CAr & ar )
{	// 군주 이벤트 지속 시간 처리
	ILordEvent* pEvent		= CCLord::Instance()->GetEvent();
	pEvent->ReadTick(ar);
	pEvent->EraseExpiredComponents();
}

void CDPClient::OnLordSkillUse( OBJID objid, CAr & ar )
{
	u_long idTarget;
	int nSkill;
	ar >> idTarget >> nSkill;	// 자기 자신에게 쓰는 군주 스킬인 경우
												// idTarget은 군주 자신의 식별자이다.
													// 이런 경우, pSkill->GetDstSfx()는 통상 NULL_ID이다.
	CLordSkillComponentExecutable* pSkill	= CCLord::Instance()->GetSkills()->GetSkill( nSkill );
	if( !pSkill )
		return;

	pSkill->Execute( CCLord::Instance()->Get(), idTarget, NULL );

	CMover* pLord	= prj.GetMover( objid );
	CMover* pTarget	= prj.GetUserByID( idTarget );
	
	if( IsValidObj( pLord ) && pSkill->GetSrcSfx() != NULL_ID )
	{
		// pLord에 pSkill->GetSrcSfx() 효과 주기
		if(pTarget)
		{
			g_pPlayer->SetAngle( GetDegree(pTarget->GetPos(), pLord->GetPos()) );
		}
		g_pPlayer->SetMotion( MTI_ATK1, ANILOOP_1PLAY, MOP_SWDFORCE | MOP_NO_TRANS | MOP_FIXED );
		CreateSfx( (int)pSkill->GetSrcSfx(), pLord->GetPos(), pLord->GetId());
	}
	if( IsValidObj( pTarget ) && pSkill->GetDstSfx() != NULL_ID )
	{
		// pTarget에 pSkill->GetDstSfx() 효과 주기
		CreateSfx( (int)pSkill->GetDstSfx(), pTarget->GetPos(), pTarget->GetId());
	}
}

void CDPClient::SendElectionAddDeposit( __int64 iDeposit )
{	// 군주 입찰 처리 요청
	BEFORESENDSOLE( ar, PACKETTYPE_ELECTION_ADD_DEPOSIT, DPID_UNKNOWN );
	ar << iDeposit;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendElectionSetPledge( const char* szPledge )
{	// 공약 설정 처리 요청
	BEFORESENDSOLE( ar, PACKETTYPE_ELECTION_SET_PLEDGE, DPID_UNKNOWN );
	ar.WriteString( szPledge );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendElectionIncVote( u_long idPlayer )
{	// 투표 처리 요청
	BEFORESENDSOLE( ar, PACKETTYPE_ELECTION_INC_VOTE, DPID_UNKNOWN );
	ar << idPlayer;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendLEventCreate( int iEEvent, int iIEvent )
{	// 군주 이벤트 시작 요청
	BEFORESENDSOLE( ar, PACKETTYPE_L_EVENT_CREATE, DPID_UNKNOWN );
	ar << iEEvent << iIEvent;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendLordSkillUse( int nSkill, const char* szTarget )
{	// 군주 스킬 사용 요청
	BEFORESENDSOLE( ar, PACKETTYPE_LORD_SKILL_USE, DPID_UNKNOWN );
	ar << nSkill;
	ar.WriteString( szTarget );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendTransformItem(CTransformStuff & stuff) {
	// 알변환 프로토콜
	// stuff는 변환에 필요한 아이템 목록
	SendPacket<PACKETTYPE_TRANSFORM_ITEM, CTransformStuff>(stuff);
}

void CDPClient::SendPickupPetAwakeningCancel( DWORD dwItem )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PICKUP_PET_AWAKENING_CANCEL, DPID_UNKNOWN );
	ar << dwItem;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendTutorialState( int nState )
{
	char szOut[64]	= { 0,};
	MakeTutorialStateString( szOut, nState, g_pPlayer->GetName() );
	BEFORESENDSOLE( ar, PACKETTYPE_TUTORIAL_STATE, DPID_UNKNOWN );
	ar << nState;
	ar.WriteString( szOut );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnTutorialState( CAr & ar )
{
	int nTutorialState;
	ar >> nTutorialState;
	if( IsValidObj( g_pPlayer ) )
		g_pPlayer->SetTutorialState( nTutorialState );
}

void CDPClient::SendOptionEnableRenderMask( BOOL bEnable )
{
	BEFORESENDSOLE( ar, PACKETTYPE_OPTION_ENABLE_RENDER_MASK, DPID_UNKNOWN );
	ar << bEnable;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRainbowRacePrevRankingOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_RAINBOWRACE_PREVRANKING_OPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRainbowRaceApplicationOpenWnd()
{
	BEFORESENDSOLE( ar, PACKETTYPE_RAINBOWRACE_APPLICATION_OPENWND, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRainbowRaceApplication()
{
	BEFORESENDSOLE( ar, PACKETTYPE_RAINBOWRACE_APPLICATION, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRainbowRaceReqFinish()
{
	BEFORESENDSOLE( ar, PACKETTYPE_RAINBOWRACE_REQ_FINISH, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnRainbowRacePrevRankingOpenWnd( CAr & ar )
{
	u_long adwPlayerId[5]	= { 0,};

	int nSize;
	ar >> nSize;
	
	for( int i=0; i<nSize; i++)
		ar >> adwPlayerId[i];

	if(g_WndMng.m_pWndRainbowRaceRanking)
		SAFE_DELETE(g_WndMng.m_pWndRainbowRaceRanking);

	g_WndMng.m_pWndRainbowRaceRanking = new CWndRainbowRaceRanking;

	if(g_WndMng.m_pWndRainbowRaceRanking)
	{
		g_WndMng.m_pWndRainbowRaceRanking->SetRankingPlayer(adwPlayerId);
		g_WndMng.m_pWndRainbowRaceRanking->Initialize();
	}
}

void CDPClient::OnRainbowRaceApplicationOpenWnd( CAr & ar )
{
	int nNum;
	ar >> nNum;

	if(g_WndMng.m_pWndRainbowRaceOffer)
		SAFE_DELETE(g_WndMng.m_pWndRainbowRaceOffer);

	g_WndMng.m_pWndRainbowRaceOffer = new CWndRainbowRaceOffer;

	if(g_WndMng.m_pWndRainbowRaceOffer)
	{
		g_WndMng.m_pWndRainbowRaceOffer->SetOfferCount(nNum);
		g_WndMng.m_pWndRainbowRaceOffer->Initialize();
	}
}

void CDPClient::OnRainbowRaceNowState(CAr & ar) {
	ar >> *CRainbowRace::GetInstance();
}

void CDPClient::OnRainbowRaceMiniGameState( CAr & ar )
{
	__MINIGAME_PACKET pMiniGamePacket;
	ar >> pMiniGamePacket;

	if( !(CRainbowRace::GetInstance()->m_wNowGame & pMiniGamePacket.wNowGame) )
	{
		return;
	}

	switch( CRainbowRace::GetInstance()->GetGameKey() )
	{
		case RMG_GAWIBAWIBO:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameKawiBawiBo);
					g_WndMng.m_pWndRRMiniGameKawiBawiBo = new CWndRRMiniGameKawiBawiBo;
					g_WndMng.m_pWndRRMiniGameKawiBawiBo->Initialize();
				}
				else 
				{
					if(pMiniGamePacket.nState == MP_FINISH)
					{
						if(g_WndMng.m_pWndRainbowRaceMiniGame)
							g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;
					}

					CWndRRMiniGameKawiBawiBo* pWndRRMiniGameKawiBawiBo = (CWndRRMiniGameKawiBawiBo*)g_WndMng.GetWndBase( APP_RR_MINIGAME_KAWIBAWIBO );
					if(pWndRRMiniGameKawiBawiBo != NULL)
						pWndRRMiniGameKawiBawiBo->ReceiveResult(pMiniGamePacket.nState);
				}
			}
			break;
		case RMG_DICEPLAY:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameDice);
					g_WndMng.m_pWndRRMiniGameDice = new CWndRRMiniGameDice;
					g_WndMng.m_pWndRRMiniGameDice->SetTargetNumber(pMiniGamePacket.nParam1);
					g_WndMng.m_pWndRRMiniGameDice->Initialize();
				}
				else
				{
					if(pMiniGamePacket.nState == MP_FINISH)
					{
						if(g_WndMng.m_pWndRainbowRaceMiniGame)
							g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;
					}

					CWndRRMiniGameDice* m_pWndRRMiniGameDice = (CWndRRMiniGameDice*)g_WndMng.GetWndBase( APP_RR_MINIGAME_DICE );
					if(m_pWndRRMiniGameDice != NULL)
						m_pWndRRMiniGameDice->ReceiveResult(pMiniGamePacket.nState, pMiniGamePacket.nParam1, pMiniGamePacket.nParam2);
				}
			}
			break;
		case RMG_ARITHMATIC:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameArithmetic);
					g_WndMng.m_pWndRRMiniGameArithmetic = new CWndRRMiniGameArithmetic;
					const std::string strQuestion = pMiniGamePacket.vecszData.empty() ? "" : pMiniGamePacket.vecszData.back();
						
					g_WndMng.m_pWndRRMiniGameArithmetic->Initialize();
					g_WndMng.m_pWndRRMiniGameArithmetic->SetNextQuestion(strQuestion.c_str(), pMiniGamePacket.nParam1);
				}
				else if(pMiniGamePacket.nState == MP_TRUE || pMiniGamePacket.nState == MP_FAIL)
				{
					if(g_WndMng.m_pWndRRMiniGameArithmetic)
					{
						const std::string strQuestion = pMiniGamePacket.vecszData.empty() ? "" : pMiniGamePacket.vecszData.back();

						g_WndMng.m_pWndRRMiniGameArithmetic->SetNextQuestion(strQuestion.c_str(), pMiniGamePacket.nParam1);
					}
				}
				else if(pMiniGamePacket.nState == MP_FALSE)
				{
					g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_REMOVE_ARITHMETIC_FAIL ) );
					if(g_WndMng.m_pWndRRMiniGameArithmetic)
					{
						const std::string strQuestion = pMiniGamePacket.vecszData.empty() ? "" : pMiniGamePacket.vecszData.back();
						
						g_WndMng.m_pWndRRMiniGameArithmetic->SetNextQuestion(strQuestion.c_str(), pMiniGamePacket.nParam1);
					}
				}
				else if(pMiniGamePacket.nState == MP_FINISH)
				{
					if(g_WndMng.m_pWndRainbowRaceMiniGame)
						g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;

					SAFE_DELETE(g_WndMng.m_pWndRainbowRaceMiniGameEnd);

					g_WndMng.m_pWndRainbowRaceMiniGameEnd = new CWndRainbowRaceMiniGameEnd;
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->SetGameID(RMG_ARITHMATIC);
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->Initialize();

					if(g_WndMng.m_pWndRRMiniGameArithmetic)
						SAFE_DELETE(g_WndMng.m_pWndRRMiniGameArithmetic);			
				}
			}
			break;
		case RMG_STOPWATCH:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameStopWatch);

					g_WndMng.m_pWndRRMiniGameStopWatch = new CWndRRMiniGameStopWatch;
					g_WndMng.m_pWndRRMiniGameStopWatch->Initialize();
					g_WndMng.m_pWndRRMiniGameStopWatch->SetTargetTime(pMiniGamePacket.nParam1);
				}
				else if(pMiniGamePacket.nState == MP_FAIL)
				{
					g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_REMOVE_ARITHMETIC_FAIL ) );
					if(g_WndMng.m_pWndRRMiniGameStopWatch)
						g_WndMng.m_pWndRRMiniGameStopWatch->SetTargetTime(pMiniGamePacket.nParam1);
				}
				else if(pMiniGamePacket.nState == MP_FINISH)
				{
					if(g_WndMng.m_pWndRainbowRaceMiniGame)
						g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;
					
					SAFE_DELETE(g_WndMng.m_pWndRainbowRaceMiniGameEnd);

					g_WndMng.m_pWndRainbowRaceMiniGameEnd = new CWndRainbowRaceMiniGameEnd;

					g_WndMng.m_pWndRainbowRaceMiniGameEnd->SetGameID(RMG_STOPWATCH);
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->Initialize();

					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameStopWatch);
				}
			}
			break;
		case RMG_TYPING:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameTyping);

					g_WndMng.m_pWndRRMiniGameTyping = new CWndRRMiniGameTyping;

						g_WndMng.m_pWndRRMiniGameTyping->Initialize();

						for( int i=0; i<(int)(pMiniGamePacket.vecszData.size() ); i++ )
						{
							g_WndMng.m_pWndRRMiniGameTyping->SetQuestion(i, pMiniGamePacket.vecszData[i].c_str());
						}						
				}
				else if(pMiniGamePacket.nState == MP_FAIL)
				{
					g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_REMOVE_ARITHMETIC_FAIL ) );

					if(g_WndMng.m_pWndRRMiniGameTyping)
						g_WndMng.m_pWndRRMiniGameTyping->ResetQuestion();
				}
				else if(pMiniGamePacket.nState == MP_FINISH)
				{
					if(g_WndMng.m_pWndRainbowRaceMiniGame)
						g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;

					SAFE_DELETE(g_WndMng.m_pWndRainbowRaceMiniGameEnd);
					g_WndMng.m_pWndRainbowRaceMiniGameEnd = new CWndRainbowRaceMiniGameEnd;
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->SetGameID(RMG_TYPING);
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->Initialize();

					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameTyping);
				}
			}
			break;
		case RMG_PAIRGAME:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{				
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameCard);

					g_WndMng.m_pWndRRMiniGameCard = new CWndRRMiniGameCard;
					g_WndMng.m_pWndRRMiniGameCard->Initialize();

					g_WndMng.m_pWndRRMiniGameCard->SetQuestion(pMiniGamePacket.vecszData[0].c_str());
				}
				else if(pMiniGamePacket.nState == MP_TRUE || pMiniGamePacket.nState == MP_FAIL || pMiniGamePacket.nState == MP_FALSE)
				{
					if(g_WndMng.m_pWndRRMiniGameCard)
						g_WndMng.m_pWndRRMiniGameCard->ReceiveResult(pMiniGamePacket.nState);
				}
				else if(pMiniGamePacket.nState == MP_FINISH)
				{
					if(g_WndMng.m_pWndRainbowRaceMiniGame)
						g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;

					SAFE_DELETE(g_WndMng.m_pWndRainbowRaceMiniGameEnd);

					g_WndMng.m_pWndRainbowRaceMiniGameEnd = new CWndRainbowRaceMiniGameEnd;
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->SetGameID(RMG_PAIRGAME);
					g_WndMng.m_pWndRainbowRaceMiniGameEnd->Initialize();

					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameCard);
				}
			}
			break;
		case RMG_LADDER:
			{
				if(pMiniGamePacket.nState == MP_OPENWND)
				{
					SAFE_DELETE(g_WndMng.m_pWndRRMiniGameLadder);

					g_WndMng.m_pWndRRMiniGameLadder = new CWndRRMiniGameLadder;
					g_WndMng.m_pWndRRMiniGameLadder->Initialize();
				}
				else if(pMiniGamePacket.nState == MP_FAIL)
				{
					if(g_WndMng.m_pWndRRMiniGameLadder)
						g_WndMng.m_pWndRRMiniGameLadder->ReceiveResult(pMiniGamePacket.nState);
				}
				else if(pMiniGamePacket.nState == MP_FINISH)
				{
					if(g_WndMng.m_pWndRainbowRaceMiniGame)
						g_WndMng.m_pWndRainbowRaceMiniGame->m_bGameInfoRefresh = FALSE;
					
					if(g_WndMng.m_pWndRRMiniGameLadder)
						g_WndMng.m_pWndRRMiniGameLadder->ReceiveResult(pMiniGamePacket.nState);
				}
			}
			break;
	}

}

#ifdef __AZRIA_1023
void CDPClient::SendDoUseItemInput( DWORD dwData, const char* szInput )
{
	BEFORESENDSOLE( ar, PACKETTYPE_DO_USE_ITEM_INPUT, DPID_UNKNOWN );
	ar << dwData;
	ar.WriteString( szInput );
	SEND( ar, this, DPID_SERVERPLAYER );
}
#endif	// __AZRIA_1023

void CDPClient::SendClearPetName()
{
	SendHdr( PACKETTYPE_CLEAR_PET_NAME );
}

void CDPClient::OnSetPetName( OBJID objid, CAr & ar )
{
	char szPetName[MAX_PET_NAME]	= { 0,};
	ar.ReadString( szPetName, MAX_PET_NAME );

	CMover* pPlayer	= prj.GetMover( objid );
	if( IsValidObj( pPlayer ) )
	{
		pPlayer->m_pet.SetName( szPetName );
		if( IsValidObj( pPlayer->m_pet.GetObj() ) )
		{
			if( strlen( szPetName ) == 0 )	// 펫 작명 취소
				pPlayer->m_pet.GetObj()->SetName( pPlayer->m_pet.GetObj()->GetProp()->szName );
			else	// 펫 작명
				pPlayer->m_pet.GetObj()->SetName( szPetName );
		}
		if( pPlayer->IsActiveMover() )
		{
			CPet* pPet	= pPlayer->GetPet();
			pPet->SetName( szPetName );
			g_WndMng.PutString( prj.GetText( TID_GAME_NAME_PET_S00 ), NULL, prj.GetTextColor( TID_GAME_NAME_PET_S00 ) );
		}
	}
}

void CDPClient::OnHousingAllInfo(CAr & ar) {
	ar >> *CHousing::GetInstance();
}

void CDPClient::OnHousingSetFunitureList( CAr & ar )
{
	auto [housingInfo, bAdd] = ar.Extract<HOUSINGINFO, BOOL>();

	CHousing::GetInstance()->SetFurnitureList( housingInfo, bAdd );

	CWndHousing* pWndHousing = (CWndHousing*)g_WndMng.GetApplet( APP_HOUSING );
	if(pWndHousing)	pWndHousing->RefreshItemList();

	ItemProp* pItemProp = prj.GetItemProp( housingInfo.dwItemId );
	if( pItemProp )
	{
		DWORD dwTextId;
		CString strTemp;
		if( bAdd )
			dwTextId = TID_GAME_HOUSING_ADDITION;
		else
			dwTextId = TID_GAME_HOUSING_TIMEOUT;

		strTemp.Format( prj.GetText( dwTextId ), pItemProp->szName );
		g_WndMng.PutString( strTemp, NULL, prj.GetTextColor( dwTextId ) );
	}

}

void CDPClient::OnHousingSetupFurniture( CAr & ar )
{
	HOUSINGINFO housingInfo;
	ar >> housingInfo;

	CHousing::GetInstance()->SetupFurniture( housingInfo );

	CWndHousing* pWndHousing = (CWndHousing*)g_WndMng.GetApplet( APP_HOUSING );
	if(pWndHousing)	pWndHousing->RefreshItemList();

	ItemProp* pItemProp = prj.GetItemProp( housingInfo.dwItemId );
	if( pItemProp )
	{
		DWORD dwTextId;
		CString strTemp;
		if( housingInfo.bSetup )
			dwTextId = TID_GAME_HOUSING_INSTALL;
		else
			dwTextId = TID_GAME_HOUSING_DEMOUNT;

		strTemp.Format( prj.GetText( dwTextId ), pItemProp->szName );
		g_WndMng.PutString( strTemp, NULL, prj.GetTextColor( dwTextId ) );
	}
}

void CDPClient::OnHousingPaperingInfo( CAr & ar )
{
	DWORD dwItemId;
	BOOL bSetup;
	ar >> dwItemId;
	ar >> bSetup;

	
	if( dwItemId == NULL_ID && !bSetup )	// 벽지, 장판 초기화 조건...
	{
		CDeployManager::GetInstance()->ChangeWallTex();
		CDeployManager::GetInstance()->ChangeTileTex();
		return;
	}

	ItemProp* pItemProp = prj.GetItemProp( dwItemId );
	if( pItemProp && pItemProp->dwItemKind2 == IK2_PAPERING )
	{
		switch( pItemProp->dwItemKind3 )
		{
			case IK3_WALLPAPER: 
				if(bSetup)
					CDeployManager::GetInstance()->ChangeWallTex(pItemProp->szTextFileName);
				else
					CDeployManager::GetInstance()->ChangeWallTex();
				break;
			case IK3_CARPET: 
				if(bSetup)
					CDeployManager::GetInstance()->ChangeTileTex(pItemProp->szTextFileName);
				else
					CDeployManager::GetInstance()->ChangeTileTex();
				break;
		}
	}
}

void CDPClient::OnHousingSetVisitAllow( CAr & ar )
{
	DWORD dwTargetId;
	BOOL bAllow;

	ar >> dwTargetId >> bAllow;

	CHousing::GetInstance()->SetVisitAllow( dwTargetId, bAllow );

	DWORD dwTextId;
	if( bAllow )
		dwTextId = TID_GAME_HOUSING_INVIATION;
	else
		dwTextId = TID_GAME_HOUSING_INVIATION_CANCEL;

	g_WndMng.PutString(dwTextId, CPlayerDataCenter::GetInstance()->GetPlayerString(dwTargetId));

	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Friend);
}

void CDPClient::OnHousingVisitableList( CAr & ar )
{
	int nSize;
	DWORD dwPlayerId;

	ar >> nSize;
	CHousing::GetInstance()->m_vecVisitable.clear();
	for( int i=0; i<nSize; i++ )
	{
		ar >> dwPlayerId;
		CHousing::GetInstance()->m_vecVisitable.push_back(dwPlayerId);
	}
	CWndRoomList* pWndRoomList = g_WndMng.m_pWndRoomList;
	if(pWndRoomList) pWndRoomList->Refresh();
}

void CDPClient::SendHousingReqSetupFurniture(HOUSINGINFO housingInfo) {
	SendPacket<PACKETTYPE_HOUSING_SETUPFURNITURE, HOUSINGINFO>(housingInfo);
}

void CDPClient::SendHousingReqSetVisitAllow( DWORD dwPlayerId, BOOL bAllow )
{
	BEFORESENDSOLE( ar, PACKETTYPE_HOUSING_SETVISITALLOW, DPID_UNKNOWN );
	ar << dwPlayerId << bAllow;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendHousingVisitRoom( DWORD dwPlayerId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_HOUSING_VISITROOM, DPID_UNKNOWN );
	if(g_pPlayer) 
	{
		if(g_pPlayer->m_idPlayer == dwPlayerId)
			CDeployManager::GetInstance()->SetMyRoom(TRUE);
		else
			CDeployManager::GetInstance()->SetMyRoom(FALSE);
	}
	ar << dwPlayerId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendHousingReqVisitableList()
{
	BEFORESENDSOLE( ar, PACKETTYPE_HOUSING_REQVISITABLELIST, DPID_UNKNOWN );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendReqNPCPos( const char* szCharKey )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUESTHELPER_REQNPCPOS, DPID_UNKNOWN );
	ar.WriteString( szCharKey );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnNPCPos( CAr & ar )
{
	D3DXVECTOR3 vPos;
	ar >> vPos;

	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	if(pWndWorld)
	{
		pWndWorld->m_bSetQuestNPCDest = TRUE;
		pWndWorld->m_vQuestNPCDest = vPos;
	}
}

void CDPClient::SendPropose( const char* pszTarget )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PROPOSE, DPID_UNKNOWN );
	ar.WriteString( pszTarget );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRefuse()   { SendPacket<PACKETTYPE_REFUSE>(); }
void CDPClient::SendCouple()   { SendPacket<PACKETTYPE_COUPLE>();   }
void CDPClient::SendDecouple() { SendPacket<PACKETTYPE_DECOUPLE>(); }
void CDPClient::OnCouple             (CAr & ar) { CCoupleHelper::Instance.OnCouple(ar);              }
void CDPClient::OnProposeResult      (CAr & ar) { CCoupleHelper::Instance.OnProposeResult(ar);       }
void CDPClient::OnCoupleResult       (CAr & ar) { CCoupleHelper::Instance.OnCoupleResult(ar);        }
void CDPClient::OnDecoupleResult     (CAr & ar) { CCoupleHelper::Instance.OnDecoupleResult();        }
void CDPClient::OnAddCoupleExperience(CAr & ar) { CCoupleHelper::Instance.OnAddCoupleExperience(ar); }

void CDPClient::OnPCBangInfo( CAr & ar )
{
	ar >> *CPCBangInfo::GetInstance();
}
#ifdef __VTN_TIMELIMIT
void CDPClient::OnAccountPlayTime( CAr & ar )
{
	int nPlayTime;
	ar >> nPlayTime;
	if( nPlayTime > -1 )
	{
		g_pPlayer->m_nAccountPlayTime = nPlayTime * SEC( 1 );
		g_pPlayer->m_nAccountPlayTime = g_tmCurrent - nPlayTime;
	}
}
#endif // __VTN_TIMELIMIT

void CDPClient::SendSmeltSafety(OBJID objid, OBJID objMaterialId, OBJID objProtScrId, OBJID objSmeltScrId  )
{
	BEFORESENDSOLE( ar, PACKETTYPE_SMELT_SAFETY, DPID_UNKNOWN );
	ar << objid << objMaterialId << objProtScrId << objSmeltScrId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSmeltSafety( CAr & ar )
{
	BYTE nResult = 0;
	ar >> nResult;

	CWndSmeltSafety* pWndSmeltSafety = g_WndMng.m_pWndSmeltSafety;
	if(pWndSmeltSafety == NULL)
		return;

	switch( nResult )
	{
	case 0:			//	안전 제련: 잘못된 재료를 등록했을 때
		break;
	case 1:			//	안전 제련: 제련 성공했을 때
		{
			int nCurrentSmeltNumber(pWndSmeltSafety->GetCurrentSmeltNumber());
			pWndSmeltSafety->SetResultStatic(true, nCurrentSmeltNumber);
			pWndSmeltSafety->SetCurrentSmeltNumber(++nCurrentSmeltNumber);
			pWndSmeltSafety->RefreshInformation();
			pWndSmeltSafety->DisableScroll2();
			pWndSmeltSafety->SetResultSwitch(true);
			break;
		}
	case 2:			//	안전 제련: 제련 실패했을 때
		{
			int nCurrentSmeltNumber(pWndSmeltSafety->GetCurrentSmeltNumber());
			pWndSmeltSafety->SetResultStatic(false, nCurrentSmeltNumber);
			pWndSmeltSafety->SetCurrentSmeltNumber(++nCurrentSmeltNumber);
			pWndSmeltSafety->RefreshValidSmeltCounter();
			pWndSmeltSafety->SetResultSwitch(true);
			break;
		}
	case 3:			//	안전 제련: 최대치까지 제련되어 더 이상 제련할 수 없을 때
		break;
	}
}

#ifdef __MAP_SECURITY
void CDPClient::OnWorldReadInfo( CAr & ar )
{
	DWORD dwWorldId;
	D3DXVECTOR3 vPos;
	ar >> dwWorldId >> vPos;

	g_WorldMng.Open( dwWorldId );
	g_WorldMng.Get()->RestoreDeviceObjects( );
	g_WorldMng.Get()->SetCamera( &g_Neuz.m_camera );
	g_Neuz.m_camera.Transform( g_WorldMng.Get() );
	g_Neuz.m_camera.Reset();
	g_WorldMng.Get()->ReadWorld( vPos );
}

void CDPClient::SendMapKey( const char* szFileName, const char* szMapKey )
{
	BEFORESENDSOLE( ar, PACKETTYPE_MAP_KEY, DPID_UNKNOWN );
	ar.WriteString( szFileName );
	ar.WriteString( szMapKey );
	SEND( ar, this, DPID_SERVERPLAYER );
}
#endif // __MAP_SECURITY

#ifdef __QUIZ
void CDPClient::OnQuizSystemMessage( CAr & ar )
{
	BOOL bAll;
	int nDefinedTextId, nChannel, nTime;
	ar >> nDefinedTextId >> bAll >> nChannel >> nTime;
	
	CString strMessage;
	//채널 정보
	if( nChannel )
	{
		CListedServers::Channel * channel = g_dpCertified.m_servers.GetChannel(g_Neuz.m_dwSys, nChannel);
		const char * strChannelName = channel ? channel->lpName : "";

		if( nTime )
			strMessage.Format( prj.GetText( nDefinedTextId ), strChannelName, nTime );
		else
			strMessage.Format( prj.GetText( nDefinedTextId ), strChannelName );
	}
	else
		strMessage.Format( prj.GetText( nDefinedTextId ) );

	CString str;
	str.Format( prj.GetText( TID_ADMIN_ANNOUNCE ), strMessage );		// [알림] %s
	g_WndMng.PutString( str, NULL, prj.GetTextColor( TID_ADMIN_ANNOUNCE ), CHATSTY_SYSTEM );
	if( ::GetLanguage() != LANG_GER && ::GetLanguage() != LANG_FRE )
	{
		if( bAll )
		{
			CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			if( pWndWorld )
				g_Caption1.AddCaption( str, pWndWorld->m_pFontAPITitle, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 153 ) );
		}
		else
		{
			if( g_pPlayer && g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->GetID() != WI_WORLD_QUIZ )
			{
				CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
				if( pWndWorld )
					g_Caption1.AddCaption( str, pWndWorld->m_pFontAPITitle, FALSE, D3DCOLOR_ARGB( 255, 255, 255, 153 ) );
			}
		}
	}
}

void CDPClient::OnQuizEventState( CAr & ar )
{
	int nType, nState;
	ar >> nType >> nState;
	CQuiz::GetInstance()->SetType( nType );
	CQuiz::GetInstance()->SetState( nState );
	CQuiz::GetInstance()->InitQuizData( nType );
	CQuiz::GetInstance()->m_nQCount = 0;
}

void CDPClient::OnQuizQuestion( CAr & ar )
{
	int nType;
	char szQuestion[1024] = {0, };
	ar >> nType;
	ar.ReadString( szQuestion, 1024 );
	CQuiz::GetInstance()->m_strQuestion = szQuestion;

	// UI출력
	if( CQuiz::GetInstance()->GetType() == TYPE_OX )
	{
		if( g_WndMng.m_pWndQuizEventQuestionOX )
			SAFE_DELETE( g_WndMng.m_pWndQuizEventQuestionOX );
		g_WndMng.m_pWndQuizEventQuestionOX = new CWndQuizEventQuestionOX;
		if( g_WndMng.m_pWndQuizEventQuestionOX )
		{
			g_WndMng.m_pWndQuizEventQuestionOX->Initialize();
			g_WndMng.m_pWndQuizEventQuestionOX->UpdateQuestion( szQuestion );
		}
	}
	else
	{
		if( g_WndMng.m_pWndQuizEventQuestion4C )
			SAFE_DELETE( g_WndMng.m_pWndQuizEventQuestion4C );
		g_WndMng.m_pWndQuizEventQuestion4C = new CWndQuizEventQuestion4C;
		if( g_WndMng.m_pWndQuizEventQuestion4C )
		{
			g_WndMng.m_pWndQuizEventQuestion4C->Initialize();
			g_WndMng.m_pWndQuizEventQuestion4C->UpdateQuestion( szQuestion );
		}
	}

	ar >> CQuiz::GetInstance()->m_nQCount;
}

void CDPClient::OnQuizEventMessage( CAr & ar )
{
	CHAR	lpString[256];
	ar.ReadString( lpString, 256 );
	int nState;
	ar >> nState;

	CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
	
	if( pWndWorld )
	{
		if( ::GetLanguage() != LANG_JAP )
			g_Caption1.AddCaption( lpString, pWndWorld->m_pFontAPICaption );// CWndBase::m_Theme.m_pFontCaption );
		else
			g_Caption1.AddCaption( lpString, NULL );// CWndBase::m_Theme.m_pFontCaption );
	}
	if( nState )
	{
		CQuiz::GetInstance()->SetState( nState );
		if( nState == CQuiz::QE_CORRECT_ANSWER )
		{
			CQuiz::GetInstance()->m_strQuestion = "";
			CQuiz::GetInstance()->m_nQCount = 0;
			if( g_WndMng.m_pWndQuizEventQuestionOX )
				SAFE_DELETE( g_WndMng.m_pWndQuizEventQuestionOX );
			if( g_WndMng.m_pWndQuizEventQuestion4C )
				SAFE_DELETE( g_WndMng.m_pWndQuizEventQuestion4C );
			
/*			DWORD dwId = NULL;
			if( CQuiz::GetInstance()->GetType() == TYPE_OX )
				dwId = 1116;
			else
				dwId = 1115;
			
			CQuiz::GetInstance()->m_pVObj = (CCtrl*)CreateObj( OT_OBJ, dwId );
			if( !CQuiz::GetInstance()->m_pVObj )
				return;
			
			CQuiz::GetInstance()->m_pVObj->SetPos( CQuiz::GetInstance()->m_sQuizData.vQuizZonePos );
			CQuiz::GetInstance()->m_pVObj->UpdateLocalMatrix();
			g_WorldMng.Get()->AddObj( CQuiz::GetInstance()->m_pVObj, TRUE );
			
			if( CQuiz::GetInstance()->GetType() == TYPE_4C )
			{
				CQuiz::GetInstance()->m_pHObj = (CCtrl*)CreateObj( OT_OBJ, dwId );
				if( !CQuiz::GetInstance()->m_pHObj )
					return;
				
				CQuiz::GetInstance()->m_pHObj->SetPos( CQuiz::GetInstance()->m_sQuizData.vQuizZonePos );
				CQuiz::GetInstance()->m_pHObj->SetAngle( 90 );
				CQuiz::GetInstance()->m_pHObj->UpdateLocalMatrix();
				g_WorldMng.Get()->AddObj( CQuiz::GetInstance()->m_pHObj, TRUE );
			}*/
			
		}
/*		else if( nState == CQuiz::QE_DROP_OUT )
		{
			g_WorldMng.Get()->RemoveObj( CQuiz::GetInstance()->m_pVObj );
			CQuiz::GetInstance()->m_pVObj = NULL;
			if( CQuiz::GetInstance()->GetType() == TYPE_4C )
			{
				g_WorldMng.Get()->RemoveObj( CQuiz::GetInstance()->m_pHObj );
				CQuiz::GetInstance()->m_pHObj = NULL;
			}
		}*/
	}
}

void CDPClient::SendQuizEventEntrance()
{
	SendHdr( PACKETTYPE_QUIZ_ENTRANCE );
}

void CDPClient::SendQuizEventTeleport( int nZone )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUIZ_TELEPORT, DPID_UNKNOWN );
	ar << nZone;
	SEND( ar, this, DPID_SERVERPLAYER );
}
#endif // __QUIZ

void CDPClient::SendRemoveVis( int nPos )
{
	BEFORESENDSOLE( ar, PACKETTYPE_VISPET_REMOVEVIS, DPID_UNKNOWN );
	ar << nPos;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendSwapVis( int nPos1, int nPos2 )
{
	BEFORESENDSOLE( ar, PACKETTYPE_VISPET_SWAPVIS, DPID_UNKNOWN );
	ar << nPos1 << nPos2;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnActivateVisPet( CAr & ar )
{
	OBJID objIdVisPetItem;
	OBJID objIdVisPetId;
	ar >> objIdVisPetItem >> objIdVisPetId;
	
	g_pPlayer->SetEatPetId( objIdVisPetId );
	g_pPlayer->SetVisPetItem( objIdVisPetItem );

	//Is Activation?
	BOOL bActivate = g_pPlayer->HasActivatedVisPet( );
	if( bActivate )
	{
		//비스펫 활성화댐
		CWndBase* pBase = g_WndMng.GetWndBase( APP_BUFFPET_STATUS );
		if( FALSE == g_WndMng.GetWndBase( APP_BUFFPET_STATUS ) )
		{
			SAFE_DELETE( g_WndMng.m_pWndBuffPetStatus );
			g_WndMng.m_pWndBuffPetStatus = new CWndBuffPetStatus;
			g_WndMng.m_pWndBuffPetStatus->Initialize();
		}

	}else
	{
		//비스펫 비활성화 
		SAFE_DELETE( g_WndMng.m_pWndBuffPetStatus );
	}

}

void CDPClient::OnChangeMoverSfx( OBJID objId, CAr & ar )
{
	DWORD dwSfxId;
	ar >> dwSfxId;
	CMover* pMover	= prj.GetMover( objId );
	if( IsValidObj( pMover ) )
	{
		pMover->m_dwMoverSfxId = dwSfxId;
		pMover->SetSfxBuffPet( dwSfxId );
	}
}

void CDPClient::OnGuildHousePacket( CAr & ar )
{
	int nPacketType, nIndex;
	GH_Fntr_Info gfi;

	ar >> nPacketType >> nIndex >> gfi;

	GuildHouse->OnGuildHousePacket( nPacketType, gfi, nIndex );

	CWndGuildHousing* pWnd = (CWndGuildHousing*)g_WndMng.GetApplet( APP_GH_FURNITURE_STORAGE );
	if( pWnd )
		pWnd->RefreshItemList( );			// 윈도창이 떠있으면 Refresh
	else
		GuildHouse->ApplyEFTexture( );		// 윈도창이 안떠있으면 환경 텍스쳐만 
}

void CDPClient::OnGuildHouseAllInfo( CAr & ar )
{
	BOOL bSetFurnitureChannel, bHaveGuildHouse;
	ar >> bSetFurnitureChannel >> bHaveGuildHouse;
	
	GuildHouse->SetFurnitureChannel( bSetFurnitureChannel );
	if( bHaveGuildHouse )
		ar >> *GuildHouse;

	GuildHouse->ApplyEFTexture( );
}

void CDPClient::OnGuildHouseRemove( CAr & ar )
{
	GuildHouse->Clear();
}

void CDPClient::OnRestPoint( CAr & ar )
{
	int nRestPoint;
	ar >> nRestPoint;

	int val = (int)( nRestPoint * 100 / MAX_REST_POINT );
	assert( val > -1 && val < 101 );
	
	GuildHouse->m_nExtraExp = val;

	if( g_pPlayer && g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
	{
		char szRestPoint[512] = {0,};
		sprintf( szRestPoint, "Rest Point : %d", nRestPoint );
		g_WndMng.PutString( szRestPoint, NULL, 0xffffff99 );
	}
}

void CDPClient::SendTeleporterReq( const char* szCharKey, int nIndex )
{
	BEFORESENDSOLE( ar, PACKETTYPE_TELEPORTER, DPID_UNKNOWN );
	ar.WriteString( szCharKey );
	ar << nIndex;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnCheckedQuest(CAr & ar) {
	if (g_pPlayer && g_pPlayer->m_quests) {
		ar >> g_pPlayer->m_quests->checked;
	}
}

void CDPClient::SendCheckedQuestId( QuestId nQuestId, BOOL bCheck )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUEST_CHECK, DPID_UNKNOWN );
	ar << nQuestId << bCheck;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnInviteCampusMember( CAr & ar )
{
	u_long idRequest;
	char szRequest[MAX_PLAYER]	= {0, };
	ar >> idRequest;
	ar.ReadString( szRequest, MAX_PLAYER );
	// 수락 동의 창 띄움
	PlayerData* pPlayerData = CPlayerDataCenter::GetInstance()->GetPlayerData( idRequest );
	if( pPlayerData == NULL )
		return;
	if( g_WndMng.m_pWndCampusInvitationConfirm )
		SAFE_DELETE( g_WndMng.m_pWndCampusInvitationConfirm );
	g_WndMng.m_pWndCampusInvitationConfirm = new CWndCampusInvitationConfirm( idRequest, szRequest );
	g_WndMng.m_pWndCampusInvitationConfirm->Initialize();
}

void CDPClient::OnUpdateCampus(CAr & ar) {
	CCampusHelper::GetInstance()->OnUpdateCampus(ar);
	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Campus);
}

void CDPClient::OnRemoveCampus(CAr & ar) {
	CCampusHelper::GetInstance()->OnRemoveCampus(ar);
	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Campus);
}

void CDPClient::OnUpdateCampusPoint(CAr & ar) {
	CCampusHelper::GetInstance()->OnUpdateCampusPoint(ar);
	CWndMessengerEx::TryUpdateList(CWndMessengerEx::UpdateListType::Campus);
}

void CDPClient::SendInviteCampusMember( u_long idTarget )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CAMPUS_INVITE, DPID_UNKNOWN );
	ar << idTarget;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendAcceptCampusMember( u_long idRequest )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CAMPUS_ACCEPT, DPID_UNKNOWN );
	ar << idRequest;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRefuseCampusMember( u_long idRequest )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CAMPUS_REFUSE, DPID_UNKNOWN );
	ar << idRequest;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendRemoveCampusMember( u_long idTarget )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CAMPUS_REMOVE_MEMBER, DPID_UNKNOWN );
	ar << idTarget;
	SEND( ar, this, DPID_SERVERPLAYER );
}

#ifdef __PROTECT_AWAKE
void CDPClient::SendSelectedAwakeningValue( DWORD dwObjID, DWORD dwSerialNum, BYTE bySelectFlag )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ITEM_SELECT_AWAKENING_VALUE, DPID_UNKNOWN );
	ar << dwObjID << dwSerialNum << bySelectFlag;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnSafeAwakening( CAr& ar )
{
	//각성 취소보호중 각성을 시도했다. 옵션 선택창을 띄운다.
	const auto [byItemObjID, dwSerialNumber, n64NewRandomOption] = ar.Extract<BYTE, DWORD, __int64>();
	
	if( !g_pPlayer ) return;
	
	g_WndMng.m_pWndSelectAwakeCase = new CWndSelectAwakeCase(byItemObjID, dwSerialNumber, n64NewRandomOption);
	g_WndMng.m_pWndSelectAwakeCase->Initialize();
}

#endif //__PROTECT_AWAKE

#ifdef __GUILD_HOUSE_MIDDLE
void CDPClient::OnGuildHouseTenderMainWnd( CAr & ar )
{
	//setup window
	//CWndGuildHouseBid 윈도우가 초기화할때 요청패킷을 보내게 되는데, 만약 생성이 되어있지 않았다면 창이 뜨자마자 바로 없애버린경우?
	if( !g_WndMng.m_pWndGHBid )
	{
		g_WndMng.m_pWndGHBid = new CWndGuildHouseBid;		
		g_WndMng.m_pWndGHBid->Initialize( );
	}

	int nSize;
	ar >> nSize;
	for( int i = 0; i < nSize; ++i )
	{
		OBJID objGHId;
		char szTitle[128] = {0, };
		ar >> objGHId;
		ar.ReadString( szTitle, 128 );

		g_WndMng.m_pWndGHBid->UpdateData_HouseList( objGHId, szTitle );
	}

	//Refresh window
	g_WndMng.m_pWndGHBid->RefreshWnd_HouseList( );
}

void CDPClient::OnGuildHouseTenderInfoWnd( CAr & ar )
{
	// 요청한 항목(하우징리스트)에 대한 세부정보
	if( !g_WndMng.m_pWndGHBid )
	{
		//이경우는 패킷무시하고 처음부터 다다시, 이경우 역시 리스트하나를 선택하고 창을 곧바로 닫아버린경우?
		g_WndMng.m_pWndGHBid = new CWndGuildHouseBid;
		g_WndMng.m_pWndGHBid->Initialize( );
		return;
	}

	vector< DWORD > guildIDs;

	OBJID objGHId;
	int nTenderMinPenya = 0, nSize = 0;
	__int64 nTenderPenya = 0;		//길드항목중 내길드가 있다면 이값이 내길드의 입찰금
	ar >> objGHId >> nTenderMinPenya >> nTenderPenya >> nSize;
	for( int i = 0; i < nSize; ++i )
	{
		DWORD dwGuildId;
		ar >> dwGuildId;

		guildIDs.push_back( dwGuildId );

	}

	g_WndMng.m_pWndGHBid->UpdateData_HouseInfo( objGHId, nTenderMinPenya,nTenderPenya, guildIDs );

	//Refresh window
	g_WndMng.m_pWndGHBid->RefreshWnd_HouseInfo( );
}

void CDPClient::OnGuildHouseTenderResult( CAr & ar )
{
	OBJID objGHId;
	BOOL bResult;
	ar >> objGHId >> bResult;

	//입찰 버튼 활성화 
	g_WndMng.m_pWndGHBid->SetEnableWindow_Apply( TRUE );

	//머니 입력창 초기화
	g_WndMng.m_pWndGHBid->ResetInputMoneyWindows( );

	//현재 선택된 하우스에대한 세부정보 요청 
	g_WndMng.m_pWndGHBid->RequestCurrHouseInfo( );
}

void CDPClient::SendGuildHouseTenderMainWnd( DWORD dwGHType, OBJID objNpcId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILDHOUSE_TENDER_MAINWND, DPID_UNKNOWN );
	ar << dwGHType << objNpcId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGuildHouseTenderInfoWnd( OBJID objGHId )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILDHOUSE_TENDER_INFOWND, DPID_UNKNOWN );
	ar << objGHId;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::SendGuildHouseTenderJoin( OBJID objGHId, int nTenderPerin, int nTenderPenya )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GUILDHOUSE_TENDER_JOIN, DPID_UNKNOWN );
	ar << objGHId << nTenderPerin << nTenderPenya;
	SEND( ar, this, DPID_SERVERPLAYER );
}
#endif // __GUILD_HOUSE_MIDDLE

CDPClient	g_DPlay;
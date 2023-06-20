#pragma once

#include "mempooler.h"
#include <map>
#include <string>
#include "ar.h"
#include "script.h"
#if defined(__DBSERVER) || defined(__CORESERVER)
#include "..\DatabaseServer\project.h"
#include "..\_Network\Objects\obj.h"
#endif

#include "guildquest.h"
#include <boost/container/flat_map.hpp>
#include "sqktd/enumset.hpp"

////////////////////////////////////////////////////////////////////////////////
// ������ ��� 
////////////////////////////////////////////////////////////////////////////////

const int MAX_GM_ALIAS			= 48;
const int MAX_GM_LEVEL			= 5;
const int MAX_GM_SIZE			= 128;
const int MAX_SQL_G_NAME		= 16;
const int MAX_G_NAME			= 48;
const int MAX_BYTE_NOTICE		= 128;		// ���������� ���� �ִ���� 
const int MAX_GUILD_LEVEL		= 50;		// ��� �ִ뷹�� 	
const int MAX_BYTE_VOTETITLE	= 26;		// ��ǥ ���� ���� �ִ� ���� 
const int MAX_BYTE_VOTEQUESTION	= 171;		// ��ǥ ���� ���� �ִ� ���� 
const int MAX_BYTE_VOTESELECT   = 21;		// ��ǥ ���û��� ���� �ִ� ���� 
const int MAX_VOTE_ENTRY		= 20;		// ��尡 ��ǥ�� �ִ� � ������ �ִ°�

#define GM_MASTER 1
#define GM_KINGPIN 5
#define GM_CAPTAIN 10
#define GM_SUPPORTER 20
#define GM_ROOKIE 80

enum class GuildPower {
	MemberLevel,
	Level,
	Invitation,
	Penya,
	Item,
	GuildHouseFurniture,
	GuildHouseUpKeep,
	_MAX
};

using GuildPowers = sqktd::EnumSet<GuildPower>;

typedef struct _SGuildMsgHeader
{
	enum
	{
		GUILD_BANK		= 0x0001,
		PENYA			= 0x0001,
		ITEM			= 0x0002,
	};

	union
	{
		DWORD		HeadA;			//
		struct 
		{
			WORD	HeadASub;		//	��� ���̵�
			WORD	HeadAMain;		//	������Ʈ�� �� Ÿ�� ����
		};
	};

	union
	{
		DWORD		HeadB;			//	������Ʈ Ÿ��
		struct 
		{
			WORD	HeadBSub;		//	�뵵�� �°� �ɰ� ����.
			WORD	HeadBMain;
		};
	};

}GUILD_MSG_HEADER, *LPGUILD_MSG_HEADER;

////////////////////////////////////////////////////////////////////////////////
//��ǥ ���� 
////////////////////////////////////////////////////////////////////////////////

struct VOTE_INSERTED_INFO
{
	u_long  idGuild;									// ���ID
	u_long	idVote;										// ��ǥID
	char    szTitle[MAX_BYTE_VOTETITLE];				// ���� 
	char	szQuestion[MAX_BYTE_VOTEQUESTION];			// ��ǥ�������� 
	char	szSelections[4][MAX_BYTE_VOTESELECT];		// ���ù��� 
};

extern CAr&  operator<<(CAr& ar, const VOTE_INSERTED_INFO& info);
extern CAr&  operator>>(CAr& ar, VOTE_INSERTED_INFO& info);


// ��� ��ǥ�� ����  
struct GUILD_VOTE_SELECT
{
	char szString[MAX_BYTE_VOTESELECT];		// ���ù��� 
	BYTE cbCount;							// ����count
};

// ��� ��ǥ 
class CGuildVote final
{
public:
	CGuildVote();
	friend CAr & operator<<(CAr & ar, const CGuildVote & self);
	friend CAr & operator>>(CAr & ar, CGuildVote & self);
	bool	IsCompleted() const { return m_bCompleted; }
	void	SetComplete() { m_bCompleted = true; }
	void    Init( const VOTE_INSERTED_INFO& info, bool bCompleted, BYTE* cbCounts );
	void	Cast( BYTE cbSelect );

	u_long  GetID() const { return m_idVote; }
	LPCTSTR	GetTitle() const { return m_szTitle; }
	LPCTSTR	GetQuestion() const { return m_szQuestion; }
	GUILD_VOTE_SELECT& GetVoteSelect(int i) { return m_selects[i]; }

private:
	u_long	m_idVote;								// ID
	bool	m_bCompleted;							// �Ϸ���� : true, ������: false
	char    m_szTitle[MAX_BYTE_VOTETITLE];			// ���� 
	char	m_szQuestion[MAX_BYTE_VOTEQUESTION];	// ��ǥ�������� 
	GUILD_VOTE_SELECT	m_selects[4];				// ���� 
};

////////////////////////////////////////////////////////////////////////////////
//CONTRIBUTION_CHANGED_INFO ���� 
////////////////////////////////////////////////////////////////////////////////
struct CONTRIBUTION_CHANGED_INFO
{
	u_long idGuild;					// ���������� ����� ����� ���id
	u_long idPlayer;				// ������ �÷��̾� 
	DWORD dwPxpCount, dwPenya;		// �÷��̾ ������ pxpȽ��, ��� 
	DWORD dwGuildPxpCount, dwGuildPenya;	// ���忡 ���� ����� ��� pxpȽ��, ��� 
	WORD nGuildLevel;						// ��巹�� 
};

extern CAr&  operator<<(CAr& ar, CONTRIBUTION_CHANGED_INFO& info);
extern CAr&  operator>>(CAr& ar, CONTRIBUTION_CHANGED_INFO& info);

////////////////////////////////////////////////////////////////////////////////
//CGuildTable ���� 
////////////////////////////////////////////////////////////////////////////////

struct GUILD_TABLE_ENTRY {
	static constexpr bool Archivable = true;

	DWORD	dwPxpCount;		// �ʿ���嵵 
	DWORD	dwPenya;		// �ʿ���� 
	WORD	nMaxMember;		// �ִ��ο� 
};

class CGuildTable
{
public:
	BOOL	Load(LPCTSTR szFileName);
	DWORD	GetPxpCount(WORD nLevel) const;
	DWORD	GetPenya(WORD nLevel) const;
	WORD	GetMaxMemeber(WORD nLevel) const;
	friend CAr & operator<<(CAr & ar, const CGuildTable & self);
	friend CAr & operator>>(CAr & ar, CGuildTable & self);
	BOOL	ReadBlock( CScript & script );
	int		GetMaxLevel() const { return m_nCount; } 

	static CGuildTable& GetInstance();
	
private:
	CGuildTable() = default;
	GUILD_TABLE_ENTRY	m_table[MAX_GUILD_LEVEL];
	int					m_nCount = 0;					// m_table�� ���� ����Ÿ�� 
};

////////////////////////////////////////////////////////////////////////////////
//CGuildMember ���� 
////////////////////////////////////////////////////////////////////////////////

class CGuildMember final
{
public:
	u_long	m_idPlayer;						// Player ID
	int		m_nPay;							// ����
	int		m_nGiveGold;					// ��忡 ����� ���
	DWORD	m_dwGivePxpCount;				// ��忡 ����� PXPȽ��( ��ų ����ġ )
	short	m_nWin;							// ������ �̰���?
	short	m_nLose;						// ������ �����?
	char	m_szAlias[MAX_GM_ALIAS];	
	BYTE	m_nMemberLv;					// ���� ����
	u_long  m_idSelectedVote;				// ������ ��ǥID
	int		m_nSurrender;	// �׺� ȸ��
	int		m_nClass;						// ��� A, B, C

public:
//	Constructions
	CGuildMember();
//	Operations
	friend CAr & operator<<(CAr & ar, const CGuildMember & self);
	friend CAr & operator>>(CAr & ar, CGuildMember & self);

	CGuildMember &	operator = ( CGuildMember & source );
	
#ifdef __CLIENT
	CMover *GetMover( void ) { return prj.GetUserByID( m_idPlayer ); }
#endif // client
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CGuildMember>*	sm_pPool;
	void*	operator new( size_t nSize )	{	return CGuildMember::sm_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CGuildMember::sm_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CGuildMember::sm_pPool->Free( (CGuildMember*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CGuildMember::sm_pPool->Free( (CGuildMember*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

////////////////////////////////////////////////////////////////////////////////
//CGuild ���� 
////////////////////////////////////////////////////////////////////////////////

// �����Ǵܰ�� 
enum CONTRIBUTION_RESULT						
{
	CONTRIBUTION_OK,							// ���尡�� 
	CONTRIBUTION_FAIL_MAXLEVEL,					// ����Ұ� - �ְ� ���� 
	CONTRIBUTION_FAIL_GUILD_OVERFLOW_PXP,		// ����Ұ� - ��� PXP overflow
	CONTRIBUTION_FAIL_GUILD_OVERFLOW_PENYA,		// ����Ұ� - ��� Penya overflow
	CONTRIBUTION_FAIL_INVALID_CONDITION,		// ����Ұ� - 
	CONTRIBUTION_FAIL_OVERFLOW_PXP,				// ����Ұ� - ������ PXP overflow
	CONTRIBUTION_FAIL_OVERFLOW_PENYA			// ����Ұ� - ������ Penya overflow
};

#define	GF_WARTIME		(DWORD)0x00000001

typedef	struct	_GUILD_MEMBER_INFO
{
	u_long	idPlayer = 0;
	BYTE	nMultiNo = 100;
}
GUILD_MEMBER_INFO, *PGUILD_MEMBER_INFO;

class	CGuildWar;

using GuildPowerss = std::array<GuildPowers, MAX_GM_LEVEL>;

class CGuild
{
public:
	u_long	m_idGuild;							// Guild ID
	char	m_szGuild[MAX_G_NAME];				// Guild Name
	std::map<u_long, CGuildMember*>	m_mapPMember;	// ȸ����
	u_long	m_idMaster;							// ����� ID
	int		m_nLevel;							// Guild Level
	std::array<GuildPowers, MAX_GM_LEVEL> m_aPower;			// ���� ������
	std::array<DWORD, MAX_GM_LEVEL> m_aPenya; // ��� ������
	BOOL	m_bActive;							// Ȱ����? ������? �˻��ϴ°ǰ�?
	DWORD   m_dwLogo;							// �ΰ� �̹��� ��ȣ
	DWORD   m_dwContributionPxp;				// ����� PXP
	char	m_szNotice[MAX_BYTE_NOTICE];		// ��������
	DWORD	m_dwFlag;							// ��� ���
	BOOL	m_bSendPay;							// ��� ������ ����� Ȯ��
	
#if !defined( __CORESERVER)
	CItemContainer m_GuildBank;	// ��� â��
#endif
	DWORD						m_nGoldGuild;	// ��� â�� ��� / ����� ���

	std::list <CGuildVote*>			m_votes;		// ��ǥ����Ÿ�� 
	u_long	m_idEnemyGuild;						// �������� ������
	int		m_nWinPoint;
	int		m_nWin;
	int		m_nLose;
	int		m_nSurrender;
	int		m_nDead;

#ifdef __CORESERVER
	BOOL	m_bLog;
#endif	// __CORESERVER

public:
//	Constructions
	CGuild();
	virtual ~CGuild();
	void	Clear( void );
//	Operations
//	If an error occurs, return nonzero.
	u_long	GetGuildId( void )		{ return m_idGuild; };
	BOOL	AddMember( CGuildMember* pMember );
	BOOL	RemoveMember( u_long idPlayer );
	BOOL	IsMember( u_long idPlayer )		{	return( GetMember( idPlayer ) != NULL );	}
	BOOL	IsMaster( u_long idPlayer )	{	return( m_idMaster == idPlayer );	}
	void	Serialize( CAr & ar, BOOL bDesc );
	bool SetLogo(DWORD dwLogo);
	void	SetNotice( const char* szNotice );

	void	SetContribution( CONTRIBUTION_CHANGED_INFO & info );
	BOOL	AddContribution( DWORD dwPxp, DWORD dwPenya, u_long idPlayer );
	CONTRIBUTION_RESULT CanContribute( DWORD dwPxp, DWORD dwPenya, u_long idPlayer );
	void	DecrementMemberContribution( u_long idPlayer, DWORD dwPenya, DWORD dwPxpCount );

	void	AddVote( const VOTE_INSERTED_INFO& info, bool bCompleted = false, BYTE* cbCounts = NULL );
	CGuildVote* FindVote( u_long idVote );
	bool	ModifyVote( u_long idVote, BYTE cbOperation, BYTE cbExtra );
		
	//	��� â���� ��ĸ� �����ü� �ִ��� Ȯ��
	[[nodiscard]] bool IsGetPenya(u_long idPlayer) { return IsAuthority(idPlayer, GuildPower::Penya); }
	//	��� â���� �������� �����ü� �ִ��� Ȯ��
	[[nodiscard]] bool IsGetItem(u_long idPlayer) { return IsAuthority(idPlayer, GuildPower::Item); }
	
	// ���� �˻�
	[[nodiscard]] bool IsCmdCap(int nMemberLv, const GuildPower dwPower) const {
		return m_aPower[nMemberLv][dwPower];
	}

	[[nodiscard]] bool IsAuthority(const DWORD dwPlayerId, const GuildPower guildPower) /* const */ {
		const CGuildMember * gm = GetMember(dwPlayerId);
		return gm ? IsCmdCap(gm->m_nMemberLv, guildPower) : false;
	}

	//	Attributes
	int		GetSize( void )	{	return m_mapPMember.size();	}
	CGuildMember*	GetMember( u_long idPlayer );
	WarId	m_idWar = WarIdNone;
	CGuildWar* GetWar( void );
	int		GetMaxMemberSize( void );

	[[nodiscard]] int GetMemberLvSize(int nMemberLv) const;
	int		GetMaxMemberLvSize( int nMemberLv );

	CGuild &	operator = ( CGuild & source );

	boost::container::flat_map<int /* QuestId */, int /* nState */> m_quests;
	std::optional<int> GetStateOfQuest(int nQuestId) const;
	BOOL	RemoveQuest(int nQuestId);
	void	SetQuest(int nQuestId, int nState);

#ifdef __WORLDSERVER
	void	ReplaceLodestar( const CRect & rect );
	void	Replace( DWORD dwWorldId, D3DXVECTOR3 & vPos, BOOL bMasterAround = FALSE );
#endif	// __WORLDSERVER

#ifndef __CORESERVER
	CItemElem*	GetItem( DWORD dwItemId, SERIALNUMBER iSerialNumber );
	short	RemoveItem( DWORD dwId, short nItemNum );
#endif	// __CORESERVER

	static	int	sm_anMaxMemberSize[MAX_GUILD_LEVEL];
	static	int	sm_anMaxMemberLvSize[MAX_GM_LEVEL];
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CGuild>*	sm_pPool;
	void*	operator new( size_t nSize )	{	return CGuild::sm_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CGuild::sm_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CGuild::sm_pPool->Free( (CGuild*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CGuild::sm_pPool->Free( (CGuild*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820

	[[nodiscard]] static bool IsValidName(const char * szGuild);

#ifdef __WORLDSERVER
public:
	template<WORD SnapshotId, typename... Ts>
	void SendSnapshotNoTarget(const Ts & ... ts) const;

	template<WORD SnapshotId, typename... Ts>
	void SendSnapshotWithTarget(DWORD targetId, const Ts & ... ts) const;
#endif
};

////////////////////////////////////////////////////////////////////////////////
//CGuildMng ���� 
////////////////////////////////////////////////////////////////////////////////

class CPlayer;

class CGuildMng
{
public:
	u_long	m_id;								// load
	std::map<u_long, CGuild*>	m_mapPGuild;		// Guild ID�� �̿��Ͽ� ��� ����
	std::map<std::string, CGuild*>	m_mapPGuild2;		// Guild Name�� �̿��Ͽ� ��� ����
#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	CRIT_SEC	m_AddRemoveLock;
#endif

public:
//	Constructions
	CGuildMng();
	virtual ~CGuildMng();
	void	Clear( void );
	u_long	AddGuild( CGuild* pGuild );
	BOOL	RemoveGuild( u_long idGuild );
	CGuild*		GetGuild( u_long idGuild );
	CGuild*		GetGuild( const char* szGuild );
	void	Serialize( CAr & ar, BOOL bDesc );
	BOOL	SetName( CGuild* pGuild, const char* szName );
public:
#ifdef __WORLDSERVER
	BOOL	m_bSendPay;
	void	Process( void );
#endif // __WORLDSERVER
#ifdef __CORESERVER	
	void	AddConnection( CPlayer* pPlayer );
	void	RemoveConnection( CPlayer* pPlayer );
#endif	// __CORESERVER
//	Attributes
	int		GetSize( void )	{	return m_mapPGuild.size();	}
};

extern CGuildMng g_GuildMng;

enum {	GUILD_PENYA_MINE_UPDATE, GUILD_ITEM_MINE_UPDATE, GUILD_PENYA_ALL_UPDATE, GUILD_ITEM_ALL_UPDATE };
enum { GUILD_PUT_ITEM, GUILD_GET_ITEM, GUILD_PUT_PENYA, GUILD_GET_PENYA, GUILD_CLOAK, GUILD_QUERY_REMOVE_GUILD_BANK };


#define MAX_RANK_LIST	20

#ifdef __DBSERVER
extern	APP_INFO	g_appInfo;
#endif

////////////////////////////////////////////////////////////////////////////////
//	CGuildRank 
////////////////////////////////////////////////////////////////////////////////
class CGuildRank
{
public:
	static CGuildRank & Instance;

	/*
	 *	R1 : �ְ����
	 *	R2 : �ִٽ�
	 *	R3 : �ִ���
	 *	R4 : �ִ��׺���
	 *	R5 : �ְ��ӷ�
	 *	R6 : �ְ��ڱ�
	 *	R7 : ��հ�
	 *	R8 : �ִ��÷���
	 */
	enum RANKING
	{
		R1, R2, R3, R4, R5, R6, R7, R8, RANK_END
	};

	// ��ũ ���� ����ü
	struct GUILD_RANKING {
		static constexpr bool Archivable = true;

		int			m_dwLogo;
		char		m_szGuild[17];
		char		m_szName[33];
		int			m_nWin;
		int			m_nLose;
		int			m_nSurrender;
		float		m_AvgLevel;
		int			m_nWinPoint;
	};
	
	std::array<boost::container::static_vector<GUILD_RANKING, MAX_RANK_LIST>, RANK_END> m_Ranking;

	DWORD			m_Version = 0;

#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	mutable CRIT_SEC		m_Lock;
#endif
	CTime			m_UpdateTime = m_UpdateTime = CTime::GetCurrentTime();;

	boost::container::static_vector<GUILD_RANKING, MAX_RANK_LIST> & operator[](RANKING i) {
		return m_Ranking[i];
	}

public:
#ifdef __DBSERVER
	BOOL RankingDBUpdate(CQuery * pQuery, LPCTSTR p_strQuery);
	BOOL GetRanking(CQuery * pQuery, LPCTSTR p_strQuery);
#endif
	
	friend CAr & operator<<(CAr & ar, const CGuildRank & self);
	friend CAr & operator>>(CAr & ar, CGuildRank & self);
};

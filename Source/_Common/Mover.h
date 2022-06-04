#pragma once

#include "pocket.h"

#ifdef __BUFF_1107
#include "buff.h"
#endif	// __BUFF_1107

#include <array>
#include <optional>
#include <type_traits>
#include <boost/container/static_vector.hpp>
#include "sqktd_maybe_owned_ptr.h"

#include "Ctrl.h"
#include "..\_AIInterface\ActionMover.h"
#include "SkillInfluence.h"
#include "CooltimeMgr.h"
#include "mempooler.h"

#ifdef __WORLDSERVER
	#include "NpcProperty.h"
	#include "SfxHitArray.h"
	#include "..\_AIInterface\AIInterface.h"
#else
	#include "CooltimeMgr.h"
#endif

typedef	MemPooler<CMover>	CMoverPool;

#define	MAX_NAME				64

#define	COLOR_PLAYER			0xffffffff
#define COLOR_GAMEMASTER        0xff2D7CAE
#define COLOR_ADMINISTRATOR     0xffAE2D2D
#define	COLOR_NPC				0xffa0a0ff  
#define	COLOR_MONSTER			0xffffffa0
#define	COLOR_PARTY				0xff70f070
#define COLOR_GUILDCOMBAT		0xff00ff00

/// SetMotion() option
#define	MOP_NONE				0x00000000
#define	MOP_SWDFORCE			0x00000001		/// Sword trailing(?)
#define	MOP_FIXED				0x00000002		/// Load the dwMotion value as it is (not +100..)
#define	MOP_NO_TRANS			0x00000004		/// No motion transition.
#define	MOP_HITSLOW				0x00000008		/// Slow after RBI.

#define	REQ_USESKILL			0x00000001		/// Response request flag - skill use response
#define	REQ_MOTION				0x00000002		/// Response Request Flag - Motion Response

#define	OBJTYPE_PLAYER			0x00000001		
#define	OBJTYPE_MONSTER			0x00000002		
#define OBJTYPE_CTRL			0x00000004		
#define	OBJTYPE_FLYING			0x00010000		/// Used by m_dwTypeFlag.

#define	CRITICAL_PROBABILITY	3
#define	CRITICAL_BERSERK_HP		30
#define	CRITICAL_BERSERK_PROB	22
#define	CRITICAL_AFTER_DEATH	22

// UPDATEITEM
#define	UI_NUM					0
#define	UI_HP					1
#define	UI_RN					2
#define	UI_AO					3
#define	UI_RAO					4
#define	UI_IR					5
#define UI_COOLTIME				8
#define	UI_FLAG					9
#define	UI_RANDOMOPTITEMID		10
#define	UI_KEEPTIME				11
#define UI_REPAIR_NUM			12

#include "UpdateItem.h"

#ifdef __WORLDSERVER
namespace sqktd {
	template<typename F>
	concept UIGenerator =
		(
		std::invocable<F, const CItemElem &>
		&& requires (const F & f, const CItemElem & e) { std::is_constructible_v<decltype(f(e)), UI::Variant>; }
	) || (
		std::invocable<F>
		&& requires (const F & f) { std::is_constructible_v<decltype(f()), UI::Variant>; }
		)
	;
}
#endif

namespace sqktd {
	template<class> inline constexpr bool always_false_v = false;
}

static constexpr int PETVIS_DEFAULT_VIS_SLOT_SZIE	=	2;

#ifdef __ADD_RESTATE_LOW
#define RESTATE_LOW				50
#endif // __ADD_RESTATE_LOW


#define MAX_SKILL_JOB	( MAX_JOB_SKILL + MAX_EXPERT_SKILL + MAX_PRO_SKILL + MAX_MASTER_SKILL + MAX_HERO_SKILL )

#ifdef __INTERNALSERVER
#define	TICK_CHEERPOINT	MIN( 1 )
#else	// __INTERNALSERVER
#define	TICK_CHEERPOINT	MIN( 60 )
#endif	// __INTERNALSERVER

#define	MAX_CHEERPOINT	3
#define	NULL_CHGPARAM	0x7FFFFFFF

const DWORD NEXT_TICK_DUEL          = SEC(1);
const DWORD NEXT_TICK_ENDDUEL       = MIN(3);
const DWORD NEXT_TICK_RECOVERY      = SEC(2);
const DWORD NEXT_TICK_RECOVERYSTAND = SEC(3);



#define	LEGEND_CLASS_NORMAL			0
#define	LEGEND_CLASS_MASTER			1
#define	LEGEND_CLASS_HERO			2



/// Type used in Replace function
enum REPLACE_TYPE
{
	REPLACE_NORMAL,				/// Block when the vehicle is full and the inventory is full
	REPLACE_FORCE,				/// Move unconditionally
};

/// skill use type
enum SKILLUSETYPE
{
	SUT_NORMAL,			/// The usual way to put it in a shortcut and use it.
	SUT_QUEUESTART,		/// This is a skill that starts with a skill cue.
	SUT_QUEUEING,		/// It is a skill during continuous firing of skill cue. (Fire directly without casting action)
};

////////////////////////////////////////////////////////////////////////////////
// PVP ���� 
////////////////////////////////////////////////////////////////////////////////

/// Player vs Player mode
enum PVP_MODE
{
	PVP_MODE_NONE,
	PVP_MODE_GUILDWAR,
	PVP_MODE_PK,
	PVP_MODE_DUEL
};


/// Transaction Confirmation Type
enum TRADE_CONFIRM_TYPE
{
	TRADE_CONFIRM_ERROR,		/// If there is no room in the inventory, or if GOLD is OVERFLOW
	TRADE_CONFIRM_OK,			/// Opponent also ok (final transaction)
};

/// Transaction status
enum TRADE_STATE
{
	TRADE_STEP_ITEM,			/// Item proposal stage
	TRADE_STEP_OK,			    /// Step by pressing OK - The ok button was pressed.
	TRADE_STEP_CONFIRM,			/// CONFIRM stage - final confirmation stage
};

////////////////////////////////////////////////////////////////////////////////
// struct
////////////////////////////////////////////////////////////////////////////////

/// Equipment information structure
typedef	struct	_EQUIP_INFO
{
	DWORD	dwId;
	int		nOption;
	BYTE	byFlag;
}	EQUIP_INFO,	*PEQUIP_INFO;

/// Mounting Additional Information Structure
typedef	struct	_EQUIP_INFO_ADD
{
	__int64		iRandomOptItemId;
	CPiercing	piercing;

	BYTE			bItemResist;
	int				nResistAbilityOption;
#ifdef __CLIENT
	CTexture*		pTexture;
#endif	// __CLIENT
	_EQUIP_INFO_ADD()
	{
		iRandomOptItemId	= 0;
		piercing.Clear();
		bItemResist	= FALSE;
		nResistAbilityOption	= 0;
#ifdef __CLIENT
		pTexture	= NULL;
#endif	// __CLIENT
	}
	
}	EQUIP_INFO_ADD, *PEQUIP_INFO_ADD;

/// Player appearance struct
typedef struct tagPLAYER
{
	EQUIP_INFO	m_aEquipInfo[MAX_HUMAN_PARTS];
	char		m_byCostume;
	char		m_bySkinSet;
	char		m_byFace;
	char		m_byHairMesh;
	char		m_byHairColor;
	char		m_byHeadMesh;
	char		m_bySex;
	char		m_byJob;
	u_short		m_uSlot;
} PLAYER,* LPPLAYER;

/// Quest structure
typedef struct tagQuest
{
	static constexpr bool Archivable = true;

	BYTE		m_nState;
	WORD		m_wTime;
	QuestId		m_wId;
	WORD		m_nKillNPCNum[ 2 ];
	BYTE		m_bPatrol   : 1;
	BYTE		m_bDialog   : 1;
	BYTE		m_bReserve3 : 1;
	BYTE		m_bReserve4 : 1;
	BYTE		m_bReserve5 : 1;
	BYTE		m_bReserve6 : 1;
	BYTE		m_bReserve7 : 1;
	BYTE		m_bReserve8 : 1;

	QuestProp* GetProp() const { return prj.m_aPropQuest.GetAt( m_wId.get() ); }

} QUEST,* LPQUEST;

/// Structs used in synchronization
typedef	struct tagCORR_ACTION
{
	float		f;
	float		fSpeed;
	DWORD		dwState;
	DWORD		dwStateFlag;
	DWORD		dwMotion;
	int			nMotionEx;
	int			nLoop;
	DWORD		dwMotionOption;
	D3DXVECTOR3	v;
	D3DXVECTOR3	vd;
	float		fAngleX;
	float		fAccPower;
	float		fTurnAngle;
	BOOL		fValid;
	BOOL		fBehavior;
	BOOL		fHalf;
} CORR_ACTION, *PCORR_ACTION;

using MoverEnnemies = std::map<OBJID, unsigned int>;

/// Type of attack
enum HITTYPE
{
	HITTYPE_FAIL	= 0,
	HITTYPE_GENERIC,			/// Player vs Monster
	HITTYPE_PVP,				/// Player vs Player
	HITTYPE_WAR,				/// ����� 
	HITTYPE_SCHOOL,				/// �б� ���� �̺�Ʈ Ÿ�� 
	HITTYPE_PK,					/// Player Killing
	HITTYPE_GUILDCOMBAT,		/// ������ 
#ifdef __JEFF_11_4
	HITTYPE_ARENA,		// �Ʒ���
#endif	// __JEFF_11_4
};

/// Resurrection information
struct RESURRECTION_DATA {
	u_long               dwPlayerID;
	const ItemProp *     pSkillProp;
	const AddSkillProp * pAddSkillProp;
};

/// Vendor and Trade class
class CVTInfo
{
private:
	DWORD					m_dwTradeGold;					/// Money in trade
	std::array<sqktd::maybe_owned_ptr<CItemElem>, MAX_VENDITEM> m_items_VT = {}; /// vendor and trader share pointer array
	OBJID					m_objId;						/// Counterparty id
	CMover*					m_pOwner;						/// Class owner
	TRADE_STATE				m_state;						/// transaction status
	std::string					m_strTitle;						/// Personal store title

public:
	OBJID					GetOtherID() { return m_objId; }
	CMover*					GetOther();
	void					SetOther( CMover* pMover );
	void					Init( CMover* pMover );
	[[nodiscard]] CItemElem * GetItem(const BYTE i) { return m_items_VT[i]; }
	void SetItem(const BYTE i, CItemElem * pItemBase) { m_items_VT[i] = pItemBase; }
	LPCTSTR					GetTitle();
	void					SetTitle( LPCTSTR szTitle );
	BOOL					IsVendorOpen();

	void					TradeClear();
	void					TradeSetItem( BYTE nId, BYTE i, short nItemNum );
	void					TradeSetItem( CItemElem & item, BYTE i, short nItemNum);
	BOOL					TradeClearItem( BYTE i );
	void					TradeSetGold( DWORD dwGold );
	int						TradeGetGold();
	BOOL					TradeConsent();
	DWORD					TradeSetItem2( BYTE nId, BYTE i, short & nItemNum );	// Return: dwText 
	TRADE_CONFIRM_TYPE		TradeLastConfirm( CAr & ar );
	TRADE_STATE				TradeGetState();
	void					TradeSetState( TRADE_STATE state );

	void					VendorClose( BOOL bClearTitle = TRUE );	// Close Private Store
	void					VendorItemNum( BYTE i, short nNum );
	void					VendorSetItem( BYTE nId, BYTE i, short nNum, int nCost );
	BOOL					VendorClearItem( BYTE i );

	/// Post-transaction result structure
	struct VENDOR_SELL_RESULT {
		bool isOk;
		CItemElem	item;			/// Sold item
		int			nRemain = 0;		/// Number of sold
		int			nErrorCode = 0;	/// Error code

		explicit(false) VENDOR_SELL_RESULT(int errorCode) : isOk(false), nErrorCode(errorCode) {}
		VENDOR_SELL_RESULT(CItemElem & itemElem, int nCost, int remain);
	};
	VENDOR_SELL_RESULT VendorSellItem( CMover* pBuyer, BYTE nItem, DWORD dwItemId, short nNum );

	[[nodiscard]] BOOL VendorIsVendor() const noexcept;
	[[nodiscard]] BOOL IsTrading(const CItemElem * pItemElem) const noexcept;

	void VendorCopyItems(const std::array<CItemElem *, MAX_VENDITEM> & values) {
		for (size_t i = 0; i != MAX_VENDITEM; ++i) {
			m_items_VT[i] = values[i];
		}
	}
};

/// �������� SFX��ŷ ������ ���� �ڷ��� 
struct SFXHIT_INFO
{
	OBJID	idTarget;
	int		nMagicPower;
	DWORD	dwSkill;
	int		nDmgCnt;
	float	fDmgAngle;
	float	fDmgPower;
	DWORD	dwAtkFlags;
	BOOL	bControl;
	DWORD	dwTickCount;
};

struct	SFXHIT_COUNT
{
	DWORD	dwSkill;
	DWORD	dwTickCount;
};

class	CGuild;
class	CGuildWar;
class	CNpcProperty;
struct	ATTACK_INFO;
struct	REGIONELEM;
class	CParty;
class	CActionMover;

#ifdef __LAYER_1015
#define	REPLACE( uMulti, dwWorld, vPos, type, nLayer )	Replace( (uMulti), (dwWorld), (vPos), (type), (nLayer) )
#else	// __LAYER_1015
#define	REPLACE( uMulti, dwWorld, vPos, type, nLayer )	Replace( (uMulti), (dwWorld), (vPos), (type) )
#endif	// __LAYER_1015

#ifdef __CLIENT
class CClientPet
{
public:
	CClientPet();
	virtual	~CClientPet();
public:
	void	SetObj( CMover* pObj )	{	m_pObj	= pObj;	}
	CMover*	GetObj()	{	return m_pObj;		}
	void	SetLevelup( BYTE nLevelup )		{	m_nLevelup	= nLevelup;		}
	BYTE	GetLevelup()	{	return m_nLevelup;	}
#ifdef __PET_1024
	void	SetName( const char* szName );
	char*	GetName()	{	return m_szName;	}
	BOOL	HasName()	{	return strlen( m_szName ) > 0;	}
#endif	// __PET_1024
private:
	CMover*	m_pObj;
	BYTE	m_nLevelup;		// PLU_LEVEL_UP �̸�, �� ���� �� �� ������ ȿ�� �����ϰ� 0���� �ʱ�ȭ
	
#ifdef __PET_1024
	char	m_szName[MAX_PET_NAME];
#endif	// __PET_1024
};
#endif	// __CLIENT

#include "MoverCommon.h"

/// �÷��̾�� NPC
class CMover : public CCtrl
{	
public:	
	CMover();
	virtual ~CMover();

	friend			CActionMover;
	enum			{ PLAYER, NPC };			/// CMover�� Ÿ�� 

public:
	BOOL			m_bPlayer;					/// Player�ΰ�, �ƴ϶�� NPC
	u_long			m_idPlayer;					/// player�� db��ȣ, NPC�� 0xffffffff
	CActionMover*	m_pActMover;				/// ������ �����̴� ��ü. ���⼭ ������ �������� ��Ʈ���Ѵ�.
	DWORD			m_dwTypeFlag;				/// OBJTYPE_FLYING �� ��� flag
	DWORD			m_dwMode;					/// ����, ����, ���� ��� flag
	DWORD			m_dwFlag;					/// MVRF_ ���� flag �ø���
	DWORD			m_dwStateMode;				/// PK, PVP, ���� ���� ... flag
	FLOAT			m_fCrrSpd;					/// �̵� �ӵ�  
	float			m_fAniSpeed;				/// �ִϸ��̼� �ӵ� ���. 1.0�� �⺻. 2.0�̸� �ι������. 2.0�� �ƽ���.
	float			m_fArrivalRange;			/// ��ǥ�� �󸶸�ŭ �����ؾ��ϴ°�. ���ʹ���. ����Ʈ 0
	OBJID			m_idDest;					/// ��ǥ ��ü ���̵�
	D3DXVECTOR3		m_vDestPos;					/// ��ǥ ���� 
	float			m_fDestAngle;				/// ��ǥ ���� 		
	CShip*          m_pIADestPos;				/// ���⿡ �����Ͱ� �ִٸ� m_vDestPos�� ��ǥ�� m_pIADestPos�������� ���� �����ǥ�� �ȴ�.
	BOOL			m_fWaitQueryGetPos;			/// ����ȭ ���� 
	int				m_nCorr;					/// ����ȭ ���� - �����ִ� ���� ������ ��(��ü)
	u_long			m_uRemnantCorrFrm;			/// ����ȭ ���� - �����ִ� ���� ������ ��(��Ʈ)
	CORR_ACTION		m_CorrAction;				/// ����ȭ ���� ����ü 
	bool			m_bForward;					/// ������?
	bool			m_bPositiveX;				/// GetPos().x - m_vDestPos.x > 0.0f
	bool			m_bPositiveZ;				/// GetPos().z - m_vDestPos.z > 0.0f
	bool			m_bLeft;					/// leftȸ����? Ŭ��??
	DWORD			m_dwRegionAttr;				/// ���� ��ǥ�� region �Ӽ� 
	DWORD			m_dwOldRegionAttr;			/// ���� ��ǥ�� region �Ӽ� 
	DWORD			m_dwMotion;					/// ��� ��ȣ 
	DWORD			m_dwMotionOption;			/// ��� �߰� �ɼ�
	OBJMSG			m_dwMotionArrive;			/// �̵� �Ϸ� �� ���

	DWORD			m_dwTickRecovery;			/// ���� ������ hp, mp ���� timer
	DWORD			m_dwTickRecoveryStand;		/// stand ������ hp, mp ���� timer

#ifdef __VTN_TIMELIMIT
	int				m_nAccountPlayTime;		//ms���� ���� �÷��� �ð�
#endif // __VTN_TIMELIMIT

	BOOL	HasBuff( WORD wType, WORD wId );
	void	RemoveBuff( WORD wType, WORD wId );
	BOOL	HasBuffByIk3( DWORD dwIk3 );
	BOOL	HasPet();
#ifdef __WORLDSERVER
	void	RemovePet();
#endif	// __WORLDSERVER
	DWORD	GetDisguise();
	BOOL	AddBuff( WORD wType, WORD wId, DWORD dwLevel, DWORD tmTime, OBJID oiAttacker = NULL_ID );
	void	RemoveCommonBuffs();
	void	RemoveChrStateBuffs( DWORD dwChrState );
	void	RemoveIk3Buffs( DWORD dwIk3 );
	void	RemoveAllBuff();
	void	RemoveAttackerBuffs( OBJID oiAttacker );
	void	RemoveDstParamBuffs( DWORD dwDstParam );
	void	RemoveDebufBuffs();
	void	ProcessBuff();
#ifdef __CLIENT
	void	ClearBuffInst();
	int		GetCurrentMaxSkillPoint();
#endif	// __CLIENT

#ifdef __BUFF_1107
	CBuffMgr	m_buffs;
#else	// __BUFF_1107
	CSkillInfluence m_SkillState;				/// ���Ӽ��� ���� ��ų�� ���� ó��(��:�����ؼ� ??�ʵ��� ���� �� ����)
#endif	// __BUFF_1107
	DWORD			m_dwSMTime[SM_MAX];			/// ���� ������ �ð� ���� ������ ����
	SKILL			m_aJobSkill[ MAX_SKILL_JOB ];		/// ��ų �迭 
	DWORD			m_tmReUseDelay[ MAX_SKILL_JOB ];	/// ��ų ����ð�
	
	LONG			m_nStr, m_nSta, m_nDex, m_nInt;		/// ���� 
	LONG			m_nLevel;					/// ���� 
	LONG			m_nDeathLevel;				/// �׾��� �� ����
	EXPINTEGER		m_nExp1, m_nDeathExp;		/// m_nExp1 - ����ġ, DeathExp - �׾��� �� ����ġ

private:
	int		m_nTutorialState;
public:
	LONG	GetFlightLv( void )	{	return ( GetLevel() >= 20? 1: 0 );		}
	void	SetFlightLv( LONG nFlightLv )	{}
	int		GetTutorialState( void )	{	return m_nTutorialState;	}
	void	SetTutorialState( int nTutorialState )	{	m_nTutorialState	= nTutorialState;	}

	LONG			m_nFxp;		/// ���෹��, 
	LONG			m_nHitPoint;				/// HP
	LONG			m_nManaPoint;				/// MP
	LONG			m_nFatiguePoint;			/// FP( ������ )
	LONG			m_nRemainGP;				/// ���� ����Ʈ 
	LONG			m_nDefenseMin, m_nDefenseMax;	/// ���� min, ���� max
	int				m_nAdjHitRate, m_nAdjParry;		/// ����ġ 
	BYTE			m_bySex;						/// ���� 
	DWORD			m_dwSkinSet, m_dwFace, m_dwHairMesh, m_dwHairColor, m_dwHeadMesh;	/// �ܾ� 
	FLOAT			m_fHairColorR, m_fHairColorG, m_fHairColorB;						/// �Ӹ��� 
	LONG			m_nJob;						/// ���� 
	DWORD			m_dwAuthorization;			/// ���� ���� (�Ϲݿ��� �ְ� �����ڱ��� )

	TCHAR			m_szCharacterKey[32];		/// NPC key���ڿ� 
	DWORD			m_dwBelligerence;			/// ȣ���� ���� 
	BOOL			m_bActiveAttack;			/// �������ΰ�?
	DWORD			m_dwVirtItem;				/// npc�� ���(����)�� ������. �Ǵ� �÷��̾ ���� �������� ����� �⺻ ������. 
	DWORD			m_dwVirtType;				/// m_dwVirtItem�� ���������� ��ų���� ���� VT_ITEM, VT_SKILL
private:
	OBJID	m_oiEatPet;
public:
	int				m_nFuel;					/// ����ü ���� ����.
	int				m_tmAccFuel;				/// ���ӿ���(�ð�����)
	BOOL			m_bItemFind;				/// ĳ���� �Ŀ� �������� �ִ��� �˻��� ���ΰ�? 
	int				m_nReadyTime;				/// ����ü Ÿ�� ĳ���� �ð�
	OBJID			m_dwUseItemId;				/// ����ü Ÿ�� ������ ���̵� 

	DWORD			m_dwCtrlReadyTime;			/// box open ĳ���� Ÿ��  
	OBJID			m_dwCtrlReadyId;			/// ĳ���� ��� box ���̵�  

	OBJID			m_idAttacker;				/// this�� ���� �����ѳ�
	OBJID			m_idTargeter;				/// this�� Ÿ����� �����ϰ� �ִ³�.
	OBJID			m_idTracking;				/// ������ �ڵ������ϰ� �ִ� Ÿ��.
	OBJID			m_idLastHitMover;			/// this�� ���������� �ƴ� �������̵�
	float			m_fExpRatio;				/// exp ���޷�. this�� �׿����� this�� exp�� ���ۼ�Ʈ�� �ٰ��ΰ�. ����Ʈ�� 1.0�̴�. �̰��� ��Ƽ���� ������� �ʴ´�.
	int				m_nAtkCnt;					/// �����ϸ� ī���Ͱ� 0�̵Ǹ鼭 ��� �����Ѵ�.
	int				m_nReflexDmg;				/// ���÷��� ������ ���� (������)
	LONG			m_nPlusMaxHitPoint;			/// �߰� MAX HP
	DWORD			m_tmPoisonUnit;				/// �� ���� �ð�
	DWORD			m_tmBleedingUnit;			/// ���ʸ��� �ѹ����ΰ�?
	OBJID			m_idPoisonAttacker;			/// �� ������
	OBJID			m_idBleedingAttacker;		/// ���� ������
	short			m_wStunCnt;					/// ���ϻ��� �ð� 
	short			m_wPoisonCnt;				/// ������ �ð� 
	short			m_wPoisonDamage;			/// ������ ���� ������
	short			m_wDarkCnt;					/// ���� ���ӽð�
	short			m_wDarkVal;					/// ���浿�� ���� ���߷� ����ġ
	short			m_wBleedingCnt;				/// ���� ���� �ð� 
	short			m_wBleedingDamage;			/// ������ ���� ������
	short			m_wDarkCover;				/// �Ⱥ��̰� ����
	short			m_nHealCnt;		//	0�� ��츸 �ڵ� ġ��
	FLOAT	m_fSpeedFactor;
	void	SetSpeedFactor( FLOAT fSpeedFactor );
	FLOAT	GetSpeedFactor( void )	{	return m_fSpeedFactor;	}
#ifdef __WORLDSERVER
	BOOL	IsReturnToBegin( void );
#endif	// __WORLDSERVER

	BOOL	IsRank( DWORD dwClass )	
		{
			MoverProp* pProp	= GetProp();
			return( pProp && pProp->dwClass == dwClass );
		}

	BYTE			m_nAttackResistLeft;		/// �ڵ� ���������� ���ӳ� ������� ���� ( �޼� �Ӽ� ���ݷ� �߰� ) 
	BYTE			m_nAttackResistRight;		/// �ڵ� ���������� ���ӳ� ������� ���� ( ������ �Ӽ� ���ݷ� �߰� ) 
	BYTE			m_nDefenseResist;			/// �ڵ� ���������� ���ӳ� ������� ���� ( �Ӽ� ���� ) 
	
	u_long			m_idparty;					/// ��Ƽ ��ȣ (Ŭ�󿡼� Ư���� ��찡 �ƴϸ� ����ؼ� �ȵ�  g_Party.m_uPartyId�� ��� )
	u_long			m_idGuild;					/// ��� ��ȣ 
	BOOL			m_bGuildBank;				/// ��� ��ũ�� �̿������� Ȯ��
	DWORD			m_idGuildCloak;				/// ������ ����ȣ�� ������ ����
	WarId			m_idWar;					/// ����� ���� ��ȣ 

	DWORD			m_tmActionPoint;			/// �׼�����Ʈ �ڵ���� Ÿ�̸�(3��) 
	DWORD			m_dwTickCheer;				/// ���� Ÿ�̸� 
	int				m_nCheerPoint;				/// Ÿ�ο��� ���� ���� ��  

	DWORD			m_dwTickDuel;				/// ��� ���� Ÿ�̸� 
	DWORD			m_dwTickEndDuel;			/// ��� ���� Ÿ�̸� 
	int				m_nDuelState;				/// ������ 
	int				m_nDuel;					/// ����� ���� ( 0:���� 1:���ε���� 2:��Ƽ����� )
	OBJID			m_idDuelOther;				/// ��� ����� ���̵�
	u_long			m_idDuelParty;				/// ��Ƽ������̸� ���� ��Ƽ�� ���̵� 
	
	int				m_nFame;					/// ��ġ 
	u_long			m_idMurderer;				/// �� ���������� �׿����� �÷��̾� ���̵�
	DWORD			m_dwPKTime;					/// ��ũ ���� �ð�
	int				m_nPKValue;					/// PK ��ġ
	DWORD			m_dwPKPropensity;			/// PK ����
	DWORD			m_dwPKExp;					/// PK ���� ���� ����ġ		
	EXPINTEGER		m_nAngelExp;				/// ���� ����ġ
	LONG			m_nAngelLevel;				/// ���� Level

	BYTE			m_nLegend;
	DWORD			dwRemoveSfxObj[MAX_SKILLBUFF_COUNT];			// ������ ȿ�� ������Ʈ 
	DWORD			GetRemoveSfxObj(DWORD	dwRemoveSfxObjID);
	void			SetRemoveSfxObj(DWORD	dwRemoveSfxObjID);

	int				m_nHonor;					// ���μ��� 
	DWORD			m_dwHonorCheckTime;			//���� �ð�üũ
	int				m_aHonorTitle[MAX_HONOR_TITLE];			// ���μ�ġ
	int				GetHonorTitle(int nIdx)	{	return m_aHonorTitle[nIdx];	}
	void			SetHonorCount(int nIdx , int nCount );
	void			CheckHonorStat();
	void			CheckHonorTime();
	void			SetHonorAdd(int nSmallIdx,int nLargeGroup,int nDefault = 0);
	CString			m_strTitle;
	LPCTSTR			GetTitle();
	void			SetTitle(LPCTSTR	pTitle);


#ifdef __CLIENT
	CSfx*			m_pSfxWing;
	DWORD			m_dwWingTime;

	CSfx*			m_pSfxBuffPet;
	void			SetSfxBuffPet( const DWORD idEffect );
#endif //__CLIENT

#ifdef __JEFF_9_20
	DWORD		m_dwMute;
#endif	// __JEFF_9_20

	std::optional<RESURRECTION_DATA> m_Resurrection_Data;		/// �������� ��ų ���� 
	CItemContainer*	m_ShopInventory[ MAX_VENDOR_INVENTORY_TAB ];	/// ����NPC�� ��ǰ ���� 
	CItemContainer	m_Inventory;				/// �κ��丮
	CPocketController	m_Pocket;
	EQUIP_INFO		m_aEquipInfo[MAX_HUMAN_PARTS];				/// �������� 	
	CVTInfo			m_vtInfo;					/// �ŷ��� ���λ��� ��ȣ 

	BYTE			m_nSlot;					/// �α��� ȭ���� 3�� ĳ���� ���� ��ȣ 
	BOOL			m_bBank;					/// ���� �̿� ��?
	u_long			m_idPlayerBank[3];			/// 3�� ĳ���� idPlayer
	DWORD			m_dwGoldBank[3];			/// 3�� ĳ���� ��嵷 
	CItemContainer	m_Bank[3];		/// 3�� ĳ���� ���� ������ 

	BYTE			m_nQuestKeeping;			/// Ŭ����ũ ��� ����Ʈ ��ȣ  
	BYTE			m_nPartyQuestKeeping;		/// �Ŵ� ���� ����Ʈ ��ȣ 

	std::unique_ptr<MoverSub::Quests> m_quests;

	int				m_nDead;					/// ���� �� 5�ʰ��� ����
	int				m_nGuildCombatState;		/// ��� ���� ���� 
	D3DXVECTOR3		m_vMarkingPos;				/// �ε����Ʈ���� ��ŷ�� ��ǥ 
	OBJID			m_idMarkingWorld;			/// �ε����Ʈ���� ��ŷ�� ���� ���̵� 

#ifdef __CLIENT
	static BOOL		m_bQuestEmoticonAdd;		/// ����Ʈ ������ Ȯ�� ���μ��� 
	static FLOAT	m_fQuestEmoticonScale;
	int				m_nQuestEmoticonIndex;
	bool			m_bShowQuestEmoticon;

	NaviPoint		m_nvPoint;
	V_NaviPoint		m_vOtherPoint;
	int				m_idSfxHit;
	BYTE			m_nWaterCircleCount;
	DWORD			m_dwReqFlag;				/// �����û �÷���.
	char			m_szPartyName[32];
	float			m_fDestScale;				/// �ɱ׶�鶧 �ε巴�� �ϱ�����..
	float			m_fDestScaleSlerp;			/// 1.0f ~ 0.0���� ��ȭ��.
	CCooltimeMgr	m_cooltimeMgr;	
	LPDIRECT3DTEXTURE9		m_pCloakTexture;	/// �����ؽ���.

	static LPDIRECT3DTEXTURE9		m_pTextureEye[2][MAX_HEAD];
	static LPDIRECT3DTEXTURE9		m_pTextureEyeFlash[2][MAX_HEAD];

#endif // __CLIENT

#ifdef __WORLDSERVER
	CNpcProperty*	m_pNpcProperty;				/// NPC ���� ����( ���, ����Ʈ, �����̸� ) ����� ���� NPC ��ȭó����
	CTimer          m_timerQuestLimitTime;		/// ����Ʈ ���� �ð� Ÿ�̸� 
	DWORD	        m_dwPKTargetLimit;			/// PK�������ѽð�
	DWORD			m_dwTickCreated;			/// ���� �ð� 
	BOOL			m_bLastPK;					/// ���������� �׾����� �÷��̾��(�������) �����ǰ� �׿��� ���� �����ǰ�.
	BOOL			m_bLastDuelParty;			/// ��Ƽ���� ���?
	CTime			m_tGuildMember;				/// ��� Ż�� �ð� 
	BOOL			m_bGuildCombat;				/// ��� ���� ��?
//	OBJID			m_idCollecter;				/// this�� ä��������.
//	int				m_nResource;				/// �ڿ� ������. - ���������� ���.
//	int				m_nCollectOwnCnt;			/// > 0 �λ�Ȳ�� this�� m_idCollecter���̴�.
	CSfxHitArray	m_sfxHitArray;				/// sfx hit ���� �迭 
	std::map< OBJID, std::queue< SFXHIT_INFO> >	m_mapSFXInfo;		/// sfx ��ŷ ������ ....	
//	map< OBJID, int>					m_mapSFXCount;		/// sfx�� �߻�Ǹ� ++ 
	std::map< OBJID, std::queue< SFXHIT_COUNT > >	m_mapSFXCount;
#endif	// __WORLDSERVER

#ifdef __WORLDSERVER
protected:
	void	DoUseEatPet( CItemElem* PitemElem );
	void	DoUseSystemPet( CItemElem* pItemElem );
public:
	void	ActivateEatPet( CItemElem* pItemElem );
	void	ActivateSystemPet( CItemElem* pItemElem );
	void	InactivateEatPet( void );
	void	InactivateSystemPet( CItemElem* pItemElem );

	void	SetPetVisDST( CItemElem* pItemElem );
	void	ResetPetVisDST( CItemElem* pItemElem );
	void	ProcessVisPet();
#endif	// __WORLDSERVER
	BOOL	IsUsingEatPet( CItemElem* pItemElem );	// ������� �����ΰ�?

	BOOL	HasActivatedEatPet( void )		{	return m_oiEatPet != NULL_ID;	}
	BOOL	HasActivatedSystemPet( void )	{	return m_dwPetId != NULL_ID;	}
	OBJID	GetEatPetId( void )		{	return m_oiEatPet;		}
	void	SetEatPetId( OBJID oiEatPet )		{	m_oiEatPet	= oiEatPet;	}
	DWORD	GetPetId( void )	{	return m_dwPetId;	}
	void	SetPetId( DWORD dwPetId )		{	m_dwPetId	= dwPetId;	}
protected:
	DWORD	m_dwPetId;	// ��ȯ���� ���� �κ��丮 ��ġ(�), ��ȯ���� �� �ε���(��)
private:
	BOOL	SetValidNeedVis( CItemElem* pItemElem, int nPos, std::vector<BYTE> & vecValid );
public:
	std::vector<BYTE>	GetValidVisTable( CItemElem* pItemElem );
	BYTE		IsSatisfyNeedVis( CItemElem* pItemElemVisPet, ItemProp* pItemPropVis );			
	CItemElem*	GetVisPetItem( void )	{ return m_Inventory.GetAtId( m_objIdVisPet ); }
	void		SetVisPetItem( OBJID objId )	{ m_objIdVisPet = objId; }
	BOOL		HasActivatedVisPet()	{ return m_objIdVisPet != NULL_ID; }
	DWORD		m_dwMoverSfxId;	// ������ �ٴ� ����Ʈ
private:
	OBJID		m_objIdVisPet;	// ��ȯ���� ������ �κ��丮 ��ġ

public:
	CPet*	GetPet( void );
	CItemElem*	GetPetItem( void );
	void	PetLevelup( void );
	void	PetRelease( void );

#ifdef __CLIENT
	CClientPet	m_pet;
	CModelObject*	GetPetModel( void )		{	return (CModelObject*)( m_pet.GetObj()? m_pet.GetObj()->m_pModel: NULL );	}
	void	CreatePetSfx( void );
	BOOL	IsOptionRenderMask()	{	return !IsMode( MODE_OPTION_DONT_RENDER_MASK );	}
#else	// __CLIENT	// __WORLDSERVER
	void	ProcessPetAvail( void );
	void	ProcessPetEnergy( void );
	void	ProcessPetExp( void );
#endif	// __CLIENT

#ifdef __SYS_TICKET
	CItemElem*	GetTicket( void );
#endif	// __SYS_TICKET

	CItemElem*	GetItemId2( int nPocket, int nItem, BOOL bExpiration = TRUE );
	BOOL	CreateItem2( CItemElem* pItem, int nPocket );
	void	RemoveItem2( int nItem, short nNum, int nPocket );

	int						m_nMaxCltTime;
	int						m_nCltTime;
	BOOL	IsCollecting( void )	{	return	m_pActMover->GetActionState() == OBJSTA_COLLECT;	}
	virtual	void	ProcessCollecting( void );
	virtual	void	StartCollecting( void );
	virtual	void	StopCollecting( void );
	CItemElem*	GetCollector( void );

	int				m_nSkillLevel;				/// ���ݱ��� �ø� ��ų����
	int				m_nSkillPoint;				/// ��ų ���� ����Ʈ ��ġ 

	CTimer			m_SkillTimer;
	BOOL			m_SkillTimerStop;
	
	BOOL			m_pAngelFlag;				/// Angel Buff Exist?
	CModelObject*	m_pAngel;					/// Angel Model Object.
	D3DXVECTOR3		m_AngelPos;					/// Angel Position.
	D3DXMATRIX		m_AngelWorldM;				/// Angel Matrix.
	DWORD			m_dwAngelKind;				/// Angel Kind.
	
#ifdef __EVE_BALLOON
	BOOL			m_pBalloonFlag;				/// Balloon Buff Exist?
	CModelObject*	m_pBalloon;					/// Balloon Model Object.
	D3DXVECTOR3		m_BalloonPos;				/// Balloon Position.
	D3DXMATRIX		m_BalloonWorldM;			/// Balloon Matrix.
	DWORD			m_dwBalloonKind;			/// Balloon Kind.
	float			m_fBalloonYPos;				/// Balloon Y Position
	BOOL			m_bBalloon;
#endif //__EVE_BALLOON

private:
	MoverEnnemies m_idEnemies;				/// ������ ���ݹ��� ���� 

	OBJACT			m_oaCmd;					/// ������Ʈ ���  
	int				m_nCParam[3];				/// ���� ��� �Ķ����
	OBJACT			m_oaAct;					/// ������Ʈ �ൿ����
	int				m_nAParam[5];				/// ���� �ൿ �Ķ����

	LONG			m_adjParamAry[ MAX_ADJPARAMARY ];		/// ���� �Ķ����( ��: hp + ����hp )
	LONG			m_chgParamAry[ MAX_ADJPARAMARY ];		/// ��ü �Ķ����( ��: ��ü hp )

	BOOL			m_bRegenItem;				/// NPC ������ �� ������ ������ �ξ��°� flag  
	int				m_nMovePattern;				/// NPC ���� �̵� Ÿ��
	int				m_nMoveEvent;				/// NPC ���� �̵��� ���̴� ���º�ȭ 99�� ���³�
	int				m_nMoveEventCnt;			/// NPC �����̵��� ���̴� ���º�ȭī����

#ifdef	__CLIENT
	DWORD			m_dwLadolfFlag;
	CModelObject*   m_pLadolf;					/// ������
	int				m_nDmgCnt;					/// ������ ī��Ʈ
	enum WingStateFlag
	{
		FLOATING, FLYING, TURNING
	};

	WingStateFlag	m_eWingStateFlag;		/// �ִϸ��̼� �÷��� ����ġ
	float			m_fOldLengthSq;			/// ���� �ִϸ��̼� �ӵ� ��ȭ���� �������� �����ϱ� ���� ����
	CTimer			m_tmEye[2];
#endif // __CLIENT

private:
	u_long	m_idCampus;			// ���� ���̵�
	int		m_nCampusPoint;		// ���� ����Ʈ
public:
	u_long	GetCampusId()		{	return m_idCampus;	}
	void	SetCampusId( u_long idCampus )		{	m_idCampus = idCampus;	}
	int		GetCampusPoint()	{	return m_nCampusPoint;	}
	void	SetCampusPoint( int nMPPoint )		{	m_nCampusPoint = nMPPoint;	}

protected:
	int				m_nCount;					/// ������ �������� ���� ������ ī����. ������ �ܿ� 0���� �ʱ�ȭ ���� ����.
	DWORD			m_dwGold;					/// ��� 
	DWORD			m_dwRideItemIdx;			/// ����ü�� ������ �ε���
	CModelObject*	m_pRide;					/// ����ü ��ü ������ 
	TCHAR			m_szName[MAX_NAME];			/// �̸� 
	
public:
	static CMover*  GetActiveMover() { return (CMover*)m_pObjActive; }	// ���ΰ� ��ü ��� 	
	static	int		GetHairCost( CMover* pMover, BYTE nR, BYTE nG, BYTE nB, BYTE nHair );
	static void		UpdateParts( int nSex, int nSkinSet, int nFace, int nHairMesh, int nHeadMesh, PEQUIP_INFO pEquipInfo, CModelObject* pModel, CItemContainer* pInventory, BOOL bIfParts = TRUE, CMover* pMover = NULL );
	static BOOL		DoEquip( int nSex, int nSkinSet, CItemElem* pItemElem, int nPart, const EQUIP_INFO & rEquipInfo, CItemContainer* pInventory, PEQUIP_INFO pEquipeInfo, CModelObject* pModel, BOOL bEquip, CMover *pMover );
	static	float	GetItemEnduranceInfluence( int nEndurance );	
	static	int		GetItemEnduranceWeight( int nEndurance );	

	virtual	BOOL	SetIndex( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwIndex, BOOL bInitProp = FALSE, BOOL bDestParam = TRUE );
	virtual BOOL	Read( CFileIO* pFile );
	virtual void	Process();
	virtual	void	Serialize( CAr & ar ); // �ø�������� ; ��Ʈ�� ���¿��� ������ Ŭ���̾�Ʈ, Ŭ���̾�Ʈ ������ �ְ���� ��Ŷ ���� 
	virtual	CModel* LoadModel( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwType, DWORD dwIndex );
	virtual void	InitProp( BOOL bInitAI = TRUE );		// ��ü�� ������Ƽ �������� �ʱ�ȭ 	
//	virtual int		OnActCollecting();				// User�� ���Ǵ� ���̹Ƿ� CUser���� ã����.
	virtual int		SendDamage( DWORD dwAtkFlag, OBJID idAttacker, int nParam = 0, BOOL bTarget = TRUE ) { return m_pActMover->SendDamage( dwAtkFlag, idAttacker, nParam, bTarget );  }
	virtual int		SendDamageForce( DWORD dwAtkFlag, OBJID idAttacker, int nParam = 0, BOOL bTarget = TRUE ) { return m_pActMover->SendDamageForce( dwAtkFlag, idAttacker, nParam, bTarget ); }	// ������

	void			Init();										// ��ü �ʱ�ȭ 
	void			InitLevel( int nJob, LONG nLevel, BOOL bGamma = TRUE );			// ��ü�� nLevel�� �ٲ���
	void			ProcessAniSpeed();
	void			AllocShopInventory( LPCHARACTER pCharacter );
	BOOL			IsVendorNPC();
	void			CheckTickCheer();
	void			SetCheerParam( int nCheerPoint, DWORD dwTickCount, DWORD dwRest );
	void			ClearEquipInfo();
	int				GetMaxPoint(int nDest);
	DWORD			GetReuseDelay( int nIndex );
	BOOL			InitSkillExp();
	void			InitCharacter( LPCHARACTER lpCharacter );	// �̸� �ʱ�ȭ LoadDialogȣ�� 
	LPCHARACTER		GetCharacter();								// 
	void			InitNPCProperty();							// NPC���� ��ũ��Ʈ �ʱ�ȭ 	
	BOOL			LoadDialog();								// dialog ��ũ��Ʈ �ε� 
	void			ProcessRecovery();							// HP, MP, FPȸ���� ó���Ѵ�.
	BOOL			IsActiveMover() { return m_pObjActive == this; }	// ���� ���ΰ� ��ü�ΰ�?
	int				IsSteal( CMover *pTarget );		// pTarget�� ��ƿ�Ϸ� �ϴ°�.
	int				IsSteal( OBJID idTaget );		// id�� �˻��ϴ� ����.
	u_long			GetPartyId() { return m_idparty; }
	BOOL			IsMode( DWORD dwMode ); 
	void			SetMode( DWORD dwMode )		{ m_dwMode |= dwMode; }	// �������� �V��
	void			SetNotMode( DWORD dwMode )	{ m_dwMode &= (~dwMode); } // �������� ���V
	BOOL			SetDarkCover( BOOL bApply, DWORD tmMaxTime = 0 );
	BOOL			SetStun( BOOL bApply, DWORD tmMaxTime = 0 );
	BOOL			SetPoison( BOOL bApply, OBJID idAttacker = NULL_ID, DWORD tmMaxTime = 0, DWORD tmUnit = 0, short wDamage = 0 );	// this�� ���� �ɸ��� �Ѵ�.
	BOOL			SetDark( BOOL bApply, DWORD tmMaxTime = 0, int nAdjHitRate = 0 );	// this�� ������¿� ������ �Ѵ� .
	BOOL			SetBleeding( BOOL bApply, OBJID idAttacker = NULL_ID, DWORD tmMaxTime = 0, DWORD tmUnit = 0, short wDamage = 0 );	// this�� ������Ų��.
	void			RemoveDebuff( DWORD dwState );		// ����� ����
	void			RemoveBuffOne( DWORD dwSkill = 0 );		// ���� �ϳ� ����
	void			RemoveBuffAll();				// ���� ��� ����.
	BOOL			IsStateMode( DWORD dwMode ); 
	void			SetStateMode( DWORD dwMode, BYTE nFlag );		// �������� �V��
	void			SetStateNotMode( DWORD dwMode, BYTE nFlag );	// �������� ���V
	BOOL			IsUseItemReadyTime( const ItemProp* pItemProp, OBJID dwObjItemId );
	BOOL			IsNPC()				{ return !m_bPlayer; }
	BOOL			IsPlayer()			{ return m_bPlayer; }
	BOOL			IsEquipableNPC()	{	return( GetCharacter() && GetCharacter()->m_nEquipNum > 0 );	}
	BOOL			IsFlyingNPC()		{ return (m_dwTypeFlag & OBJTYPE_FLYING) ? TRUE : FALSE; }		// ������ ���ΰ�.
	BOOL			IsFly();
	MoverProp*		GetProp() { return prj.GetMoverProp( m_dwIndex ); }	// ��ü�� ������Ƽ ��� 	
	ItemProp*		GetActiveHandItemProp( int nParts = PARTS_RWEAPON );							// �տ� ����� �������� ������Ƽ�� ��� 
	ItemProp*		GetTransyItem( ItemProp* pItemProp, BOOL bCheck = FALSE, LPCTSTR lpszFileName = NULL );
	OBJID			GetDestId()			{ return m_idDest; }
	CCtrl*			GetDestObj()		{ return prj.GetCtrl( m_idDest ); }
	void			SetDestObj( CCtrl* pObj, float fRange = 0.0f )	{	SetDestObj( pObj->GetId(), fRange );	}
	D3DXVECTOR3		GetDestPos()		{ return m_vDestPos; }
	void			SetDestPos( CONST D3DXVECTOR3& vDestPos, bool bForward = true, BOOL fTransfer = TRUE );
	void			SetDestPos( CShip *pIAObj, const D3DXVECTOR3 &vDestPos );
	
	void			ClearDestObj()     	{ m_idDest = NULL_ID; m_fArrivalRange = 0.0f; }
	void			ClearDestPos()		{ m_vDestPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); }		
	void			ClearDest()			{ ClearDestObj(); ClearDestPos(); }
	void			SetStop()			{ SendActMsg( OBJMSG_STAND ); ClearDestObj(); } // ���� 

	BOOL			IsEmptyDestObj()	{ return m_idDest == NULL_ID; }
	BOOL			IsEmptyDestPos()	{ return m_vDestPos.x == 0.0f && m_vDestPos.z == 0.0f; }
	BOOL			IsEmptyDest()		{ return IsEmptyDestPos() && IsEmptyDestObj(); }
	void			SetDestAngle( float fDestAngle, bool bLeft )	{	m_fDestAngle	= fDestAngle;	m_bLeft	= bLeft;	}
	void			ClearDestAngle()	{ m_fDestAngle	= -1.0f; }
	BOOL			IsRegionAttr( DWORD dwAttribite ) { return ( m_dwRegionAttr & dwAttribite ) == dwAttribite ? TRUE : FALSE; }
	REGIONELEM*		UpdateRegionAttr();
	DWORD			GetRideItemIdx()	{ return m_dwRideItemIdx; }
	void			SetRide( CModel *pModel, int nRideItemIdx = 0 ) { m_dwRideItemIdx = nRideItemIdx; m_pRide = (CModelObject*)pModel; 	};
	void			ClearDuel();
	void			ClearDuelParty();		
	int				SendActMsg( OBJMSG dwMsg, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0, int nParam4 = 0 ); 		
	void			SendAIMsg( DWORD dwMsg, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	void			PostAIMsg( DWORD dwMsg, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	BOOL			SetMotion(DWORD dwMotion, int nLoop = ANILOOP_LOOP, DWORD dwOption = 0 );	// MOP_SWDFORCE, MOP_FIXED, MOP_NO_TRANS, MOP_HITSLOW
	BOOL			InitMotion(DWORD dwMotion);
	void			SetDamagedMotion( CMover* pAttacker, DWORD dwAtkFlags );
	CItemElem*		GetWeaponItem( int nParts = PARTS_RWEAPON );					// ������ ���� ��� 
	CItemElem*		GetLWeaponItem();					// �޼տ� ������ ���� ���.
	CItemElem*		GetEquipItem( int nParts );			// ������ ������ ��� 
	ItemProp*		GetEquipItemProp( CItemContainer * pInventory, PEQUIP_INFO pEquipInfo, int nParts );
	BOOL			IsDualWeapon();
	void			RedoEquip( BOOL fFakeParts, BOOL bDestParam = TRUE );	
	void			UpdateParts( BOOL bFakeParts  = FALSE ); // normal or fake
	int				InvalidEquipOff( BOOL fFakeparts );		// �Žñ��� ��� ����.
	BOOL			DoEquip( CItemElem* pItemElem, BOOL bEquip, int nPart = -1 ); // for normal
	BOOL			IsEquipAble( CItemElem* pItem,BOOL bIgnoreLevel = FALSE );		// ���������Ѱ�?
	BOOL			IsUnEquipAble( ItemProp *pItemProp );					// ���°� �����Ѱ�?
	void			SetEquipDstParam();
	void			SumEquipAdjValue( int* pnAdjHitRate, int* pnAdjParray );
	int				SumEquipDefenseAbility( LONG* pnMin, LONG* pnMax );
	void			SetDestParamEquip( ItemProp* pItemProp, CItemElem* pItemElem, BOOL bIgnoreSetItem = FALSE );
	void			ResetDestParamEquip( ItemProp* pItemProp, CItemElem* pItemElem );
	[[nodiscard]] QUEST *			FindQuest(QuestId nQuestId );
	[[nodiscard]] QUEST *			GetQuest(QuestId nQuestId) { return FindQuest(nQuestId); }
	BOOL			RemoveQuest(QuestId nQuestId );
	[[nodiscard]] BOOL			IsCompleteQuest(QuestId nQuestId ) const;
	[[nodiscard]] std::optional<BYTE> GetQuestState(QuestId nQuestId) const;
	BOOL			SetQuest( QuestId nQuestId, int nState, LPQUEST lpReturnQuest );
	BOOL			SetQuest( LPQUEST lpQuest );
	void			RemoveAllQuest();
	void			RemoveCompleteQuest();
	BOOL            IsDisguise();
	BOOL			NoDisguise( LPDIRECT3DDEVICE9 pd3dDevice = NULL );
	BOOL			Disguise( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwMoverIndex );
	DWORD			IsAuthorization( DWORD dwAuthorization ) { return dwAuthorization == m_dwAuthorization; }
	DWORD			IsAuthHigher( DWORD dwAuthorization ) { return dwAuthorization <= m_dwAuthorization; }
	void			UpdateParam();		
	[[nodiscard]] int GetParam(int nDestParameter, int nParam) const;
	void			SetDestParam( int nDstParameter, int nAdjParameterValue, int nChgParameterValue, BOOL fSend = TRUE );
	void			SetDestParam( int nItemIdx2, BOOL fSend = TRUE );
	void			SetDestParam( int nIdx, ItemProp *pProp, BOOL bSend = TRUE );
	void			ResetDestParam( int nDstParameter, int nAdjParameterValue, BOOL fSend = TRUE );
#ifdef __SPEED_SYNC_0108		// ResetDestParam speed ����
	void			ResetDestParamSync( int nDstParameter,int nAdjParameterValue, int nParameterValue, BOOL fSend = TRUE );
#endif // __SPEED_SYNC_0108		// ResetDestParam speed ����
	void			ResetDestParam( int nIdx, ItemProp *pProp, BOOL bSend = TRUE );
	[[nodiscard]] int GetAdjParam(int nDestParameter) const;
	[[nodiscard]] int	GetChgParam(int nDestParameter) const;
	int				GetPointParam( int nDstParameter );
	void			SetPointParam( int nDstParameter, int nValue, BOOL bTrans = FALSE ); // bTrans�� TRUE�̸� ��������
	void			OnApplySM();
	LPSKILL			GetSkill( int nType, int nIdx );
	LPSKILL			GetSkill( DWORD dwSkill );
	int				GetSkillIdx( DWORD dwSkill );
	void			OnEndSkillState( DWORD dwSkill, DWORD dwLevel );
	BOOL			CheckSkill( DWORD dwSkill );
	void			SetHair( int nHair );
	void			SetHairColor( DWORD dwHairColor );
	void			SetHairColor( FLOAT r, FLOAT g, FLOAT b );
	void			SetHead( int nHead );
	void			SetSkinSet( int nSkinSet );
	void			SetName( const char* lpszName )		{	lstrcpy( m_szName, lpszName );	}
	LPCTSTR			GetName( BOOL bNickname = FALSE );	// ��ü�� �̸� ���
	BYTE			GetSex() { return m_bySex; }
	void			SetSex( BYTE bySex ) { m_bySex = bySex; }
	BOOL			IsPeaceful() { return m_dwBelligerence == BELLI_PEACEFUL; }
	BOOL			IsBaseJob();
	BOOL			IsExpert();
	BOOL			IsPro();
	BOOL			IsInteriorityJob( int nJob );
	BOOL			SetExpert( int nExpert );
	BOOL			AddChangeJob( int nJob );
	int				GetJob();
	int				GetExpPercent();
	int				SetLevel( int nSetLevel );
	int				AddGPPoint( int nAddGPPoint );
	void			SetJobLevel( int nLevel, int nJob );
	BOOL			IsJobType( DWORD dwJobType ); 
	[[nodiscard]] int GetLevel() const { return m_nLevel; }
	int				GetFxp() { return m_nFxp; }
	int				GetTxp() { return m_nFxp; }
	EXPINTEGER		GetExp1()	{	return m_nExp1;	}
	EXPINTEGER		GetMaxExp1()	{	return prj.m_aExpCharacter[m_nLevel+1].nExp1;	}
	EXPINTEGER		GetMaxExp2()	{	return prj.m_aExpCharacter[m_nLevel].nExp2;	}
	int				GetRemainGP(); // growth
	void			IncHitPoint( int nVal) ;
	void			IncManaPoint(int nVal) ;
	void			IncFatiguePoint(int nVal) ;
	void			SetHitPoint( int nVal) ;
	void			SetManaPoint( int nVal ) ;
	void			SetFatiguePoint( int nVal ) ;
	float			GetExpFactor();
	float			GetItemDropRateFactor( CMover* pAttacker );
	float			GetPieceItemDropRateFactor( CMover* pAttacker );
	BOOL			AddExperience( EXPINTEGER nExp, BOOL bFirstCall = TRUE, BOOL bMultiply = TRUE, BOOL bMonster = FALSE );	// bMultiply : ���ȭ ������ ����? // bMonster : ���͸� �׿��� �� ����ġ �ΰ�?
	BOOL			DecExperience( EXPINTEGER nExp, BOOL bExp2Clear, BOOL bLvDown );	// ����ġ�� ��´�.
	BOOL			DecExperiencePercent( float fPercent, BOOL bExp2Clear, BOOL bLvDown  );	// ����ġ�� �ۼ�Ʈ�� ��´�.
	BOOL			AddFxp( int nFxp );
	BOOL			SetFxp( int nFxp, int nFlightLv );
	BOOL			IsPVPTarget( CMover* pMover );
	BOOL			IsWarTarget( CMover* pMover );
	BOOL			IsSchoolTarget( CMover* pMover );
	HITTYPE			GetHitType( CMover* pMover, BOOL bTarget, int nReflect );
#ifdef __JEFF_11_4
	BOOL	IsArenaTarget( CMover* pMover );
#endif	// __JEFF_11_4
	HITTYPE			GetHitType2( CMover* pMover, BOOL bTarget, BOOL bGood );
	void			ReSetDuelTime( CMover* pAttacker , CMover* pDefender);
	BOOL			IncStrLevel();
	BOOL			IncStaLevel();
	BOOL			IncDexLevel();
	BOOL			IncIntLevel();
	int				GetHR();
	void			SetStr( int nStr ) { m_nStr = nStr; }
	void			SetSta( int nSta ) { m_nSta = nSta; }
	void			SetDex( int nDex ) { m_nDex = nDex; }
	void			SetInt( int nInt ) { m_nInt = nInt; }	
	[[nodiscard]] int GetStr() const;
	[[nodiscard]] int GetDex() const;
	[[nodiscard]] int GetInt() const;
	[[nodiscard]] int GetSta() const ;
	FLOAT			GetSpeed(FLOAT fSrcSpeed);    
	int				GetGold();
	int		GetPerinNum( void );
	__int64		GetTotalGold( void );
#ifdef __WORLDSERVER
	int		RemoveTotalGold( __int64 iGold );	// ���ŵ� �丰 ����
	int		RemovePerin( int nPerin );
#endif	// __WORLDSERVER
	void			SetGold( int nGold );
	BOOL			AddGold( int nGold, BOOL bSend = TRUE );
	int				GetHitPointPercent( int nPercent = 100 );
	int				GetManaPointPercent( int nPercent = 100 );
	int				GetFatiguePointPercent( int nPercent = 100 );
	int				GetHitPoint();
	int				GetManaPoint();
	int				GetFatiguePoint();
	[[nodiscard]] int GetMaxHitPoint();
	int				GetMaxManaPoint();
	int				GetMaxFatiguePoint();
	int				GetMaxOriginHitPoint( BOOL bOrinal = TRUE );
	int				GetMaxOriginManaPoint( BOOL bOrinal = TRUE );
	int				GetMaxOriginFatiguePoint( BOOL bOrinal = TRUE );
	int				GetHPRecovery();
	int				GetMPRecovery();
	int				GetFPRecovery();
	int				GetNaturalArmor();
 	float			GetAttackSpeed();
	float			GetCastingAniSpeed();
	int				GetCastingTime( int nCastingTime );
	float			GetDamageMultiplier( ATTACK_INFO* pInfo );
	int				PostCalcMagicSkill( int nATK, ATTACK_INFO* pInfo );
	int				PostCalcGeneric( int nATK, ATTACK_INFO* pInfo );
	float			GetMagicSkillFactor( CMover* pDefender, SAI79::ePropType skillType );
	float			GetATKMultiplier( CMover* pDefender, DWORD dwAtkFlags );
	float			GetDEFMultiplier( ATTACK_INFO* pInfo );
	float			GetBlockFactor( CMover* pAttacker, ATTACK_INFO* pInfo );
	int				GetWeaponATK( DWORD dwWeaponType );
	int				GetPlusWeaponATK( DWORD dwWeaponType );
	int				GetWeaponPlusDamage( int nDamage, BOOL bRandom = TRUE );
	void			GetDamagePropertyFactor( CMover* pDefender, int* pnATKFactor, int* pnDEFFactor, int nParts );
	int				GetPropATKPlus( int nParts );
	int				GetPropDEFPlus();
	void			GetDamageRange( int& nMin, int& nMax );
	BOOL			IsBlocking( CMover* pAttacker );
	BOOL			CanFlyByAttack();
	BOOL			IsCriticalAttack( CMover* pDefense, DWORD dwAtkFlags );
	int				GetCriticalProb();
	int				GetReqMp( int nReqMp );
	int				GetReqFp( int nReqFp );
	BOOL			GetAttackResult( CMover* pHitObj, DWORD dwOption );
	int				GetAdjHitRate();
	int				GetHitPower( ATTACK_INFO* pInfo );
	int				GetRangeHitPower( ATTACK_INFO* pInfo );
	int				GetParrying();
	int				GetDefenseByItem( BOOL bRandom = TRUE );
	int				GetShowDefense( BOOL bRandom );
	int				GetResistMagic();
	int				GetResistSpell( int nDestParam );
	int				GetMeleeSkillPower( ATTACK_INFO* pInfo );	// �������� ��ų������
	int				GetMagicHitPower( int nMagicPower );			// �ϵ���� ������
	int				GetItemAbility( int nItem );
	int				GetItemAbilityMin( int nItem );
	int				GetItemAbilityMax( int nItem );
	void			GetHitMinMax( int* pnMin, int* pnMax, ATTACK_INFO *pInfo = NULL );
	BOOL			IsAfterDeath();
	BOOL			IsDie() { return m_pActMover->IsDie() || m_nHitPoint == 0; }
	BOOL			IsLive() { return m_pActMover->IsDie() == FALSE || m_nHitPoint > 0; }		// && ��  ||�� �ٲ���.  !=�� >�� �ٲ�
	int				GetCount() { return m_nCount; }
	void			SetPKPink( DWORD dwTime ) { if( dwTime == 0 || m_dwPKTime < dwTime ) m_dwPKTime = dwTime; }
	DWORD			GetPKPink( void ) { return m_dwPKTime; }
	BOOL			IsPKPink( void )	{ return m_dwPKTime > 0; }			/// PK ��ũ �������
	BOOL			IsChaotic( void ) { return m_dwPKPropensity > 0; }	/// ī������
	void			SetPKValue( int nValue );
	int				GetPKValue( void ) { return m_nPKValue; }
	void			SetPKPropensity( DWORD dwValue );
	DWORD			GetPKPropensity( void ) { return m_dwPKPropensity; }
	DWORD			NextPKPropensity( int nPKValue );
	float			GetResist(SAI79::ePropType p_PropType);
	int				GetSetItemParts(DWORD dwParts);
	int				GetSetItemClient();
	int				GetSetItem( CItemElem* pItemElem = NULL );
	BOOL			IsSetItemPart( DWORD dwParts );
	void			SetSetItemAvail( int nAbilityOption );
	void			ResetSetItemAvail( int nAbilityOption );
	void			DestParamPiercingAvail( CItemElem* pItemElem, BOOL bSET = TRUE );
#ifdef __WORLDSERVER
	BOOL			Pierce( CItemElem *pSuit, DWORD dwItemId );
#endif
	void			SetDestParamSetItem( CItemElem* pItemElem );
	void			ResetDestParamSetItem( CItemElem* pItemElem );
	[[nodiscard]] int GetEquipedSetItemNumber(const CSetItem & pSetItem);
	[[nodiscard]] bool IsEquipedPartItem(const CSetItem::PartItem & partItem);

	void			SetDestParamRandomOpt( CItemElem* pItemElem );
	void			ResetDestParamRandomOpt( CItemElem* pItemElem );
	
	void	SetDestParamRandomOptOrigin(const CItemElem * pItemElem);
	void	SetDestParamRandomOptExtension( CItemElem* pItemElem );
	void	ResetDestParamRandomOptOrigin(const CItemElem * pItemElem);
	void	ResetDestParamRandomOptExtension( CItemElem* pItemElem );

	void			GetItemATKPower( int *pnMin, int *pnMax, ItemProp* pItemProp, CItemElem *pWeapon ); 
	float			GetItemMultiplier( CItemElem* pItemElem );
	BOOL			SubLootDropNotMob( CItem *pItem );
	BOOL			SubLootDropMobSingle( CItem *pItem );
	BOOL			SubLootDropMobParty( CItem *pItem, CParty *pParty );
	BOOL			SubLootDropMob( CItem *pItem );
	BOOL			DoLoot( CItem *pItem );		// �ٴڿ� ������ pItem�� �ݴ´�.
	void			PickupGoldCore( int nGold );
	void			PickupGold( int nGold, BOOL bDropMob );
	BOOL			IsDropable( CItemElem* pItemElem, BOOL bPK );
 	BOOL			IsDropableState( BOOL bPK );
	CItem*			DropGold( DWORD dwGold, const D3DXVECTOR3& vPos, BOOL bPK = FALSE );
	void			UpdateItem( BYTE nId, CHAR cParam, DWORD dwValue, DWORD dwTime = 0 );

public:
#ifdef __CLIENT
	void UpdateItem(ItemPos dwId, const UI::Variant & operation);
#endif
#ifdef __WORLDSERVER
	void UpdateItem(CItemElem & itemElem, const UI::Variant & operation);

	template <sqktd::UIGenerator Generator>
	void UpdateItem(CItemElem & itemElem, Generator generator) {
		if constexpr (std::is_invocable_v<Generator, const CItemElem &>) {
			UpdateItem(itemElem, generator(itemElem));
		} else if constexpr (std::is_invocable_v<Generator>) {
			UpdateItem(itemElem, generator());
		} else {
			static_assert(sqktd::always_false_v<Generator>, "Could not find a valid invocation");
		}
	}
#endif
private:
	void UpdateItem(CItemElem * itemElem, ItemPos dwId, const UI::Variant & operation);
public:
	CItem*			_DropItemNPC( DWORD dwItemType, DWORD dwID, short nDropNum, const D3DXVECTOR3 &vPos );
	CItem*			DropItem( DWORD dwID, short nDropNum, const D3DXVECTOR3 &vPos, BOOL bPlayer = FALSE );
	int				DoDropItemRandom( BOOL bExcludeEquip, CMover* pAttacker, BOOL bOnlyEquip = FALSE );
	int				GetItemNum( DWORD dwItemId );
#ifdef __CLIENT
	int				GetItemNumForClient( DWORD dwItemId ); // Client������ ����ϴ� ������ ���� ���ϱ�(Null check �̿��� Usable check����)
#endif //__CLIENT
	int				RemoveAllItem( DWORD dwItemId );
	BOOL			AddItem( CItemElem * pItemBase );
	CItemElem *		GetItemId( DWORD dwId );
	[[nodiscard]] const ItemProp * GetItemIdProp(DWORD dwId);
	void			RemoveItemId( DWORD dwId  );
	void			SetKeeptimeInven( DWORD dwItemId, DWORD dwTime );
	void			SetKeeptimeBank( DWORD dwItemId, DWORD dwTime );
	void			SetKeeptimeItem( CItemElem* pItemElem, DWORD dwTime );
	void			OnTradeRemoveUser();
	BOOL			AddItemBank( int nSlot, CItemElem* pItemElem );
	void			UpdateItemBank( int nSlot, BYTE nId, CHAR cParam, DWORD dwValue );
	[[nodiscard]] CItemElem * GetItemBankId( int nSlot, DWORD dwId );
	void			RemoveItemBankId( int nSlot, DWORD dwId );
	void			GenerateVendorItem( ItemProp** apItemProp, int* pcbSize, int nMax, const CVendor::CategoryItem & pVendor );
	BOOL			DropItemByDied( CMover* pAttacker );		// �׾ ����Ʈ���� ���.
	BOOL			DoUseSkill( DWORD dwSkill, int nLevel, OBJID idFocusObj, SKILLUSETYPE sutType = SUT_NORMAL, BOOL bControl = FALSE, const int nCastingTime = 0 );		// dwSkill/nLevel�� ������ ����� �� �ִ� ����.
	void			SetPKTargetLimit( int nSec );
	void			ClearCmd(); 
	OBJACT			GetCmd() { return m_oaCmd; }
	OBJACT			GetAct() { return m_oaAct; }
	int				GetCmdParam( int nIdx ) { return m_nCParam[nIdx]; }	
	void			ClearActParam(); 
	int				GetEnemyCount();
	OBJID			GetMaxEnemyHitID();
	void AddEnemy(OBJID objid, int nHit);
	void			RemoveEnemy( OBJID objid  );
	void			RemoveAllEnemies();
	BOOL			IsAttackAble( CObj *pObj );
	FLOAT			GetAttackRange( DWORD dwAttackRange );
	BOOL			IsAttackAbleNPC( CMover* pNPC );
	BOOL			IsPKInspection( CMover* pMover );
	int				IsPKPVPInspectionBase( DWORD dwRegionAttr, BOOL bPVP = TRUE );
	int				IsPVPInspectionBase( DWORD dwRegionAttr,DWORD dwWorldID, BOOL bPVP = TRUE );//8�� ������� ������� PVP�����ϰ���   Neuz, World
	BOOL			SubPKPVPInspectionBase( CMover* pMover, CMover* pMover1, DWORD dwPKAttr, int nFlag );
	void			PrintString( CMover* pMover, DWORD dwId );
	int				CMD_SetUseSkill( OBJID idTarget, int nSkillIdx, SKILLUSETYPE sutType = SUT_NORMAL );
	void			CMD_SetMeleeAttack( OBJID idTarget, FLOAT fRange = 0.0f );
	void			CMD_SetMagicAttack( OBJID idTarget, int nMagicPower );
	void			CMD_SetRangeAttack( OBJID idTarget, int nPower );
	void			CMD_SetMoveToPos( const D3DXVECTOR3 &vPos );
	void			CMD_SetMoveToObj( OBJID idObj );
	void			CMD_SetUseItem( CCtrl *pCtrl );
//	void			CMD_SetCollect( CObj *pObj );
	void			UnequipRide();		
	void			EquipItem( CItemElem *pItemElem, BOOL bEquip, int nPart );
	BOOL			DoUseEquipmentItem( CItemElem* pItemElem, DWORD dwId, int nPart );
	BOOL			DoUseItemVirtual( DWORD dwItemId, BOOL bEffectSkip );
	BOOL			DoUseItemBlinkWing( ItemProp *pItemProp, CItemElem* pItemElem, BOOL bUes = TRUE );
	void			DoPickupItemAround();
	int				DoAttackMagic( CObj *pTargetObj, int nMagicPower,int idSfxHit = 0 );
	int				DoAttackRange( CObj *pTargetObj, DWORD dwItemID, int idSfxHit );
	int				DoAttackSP( CObj *pTargetObj, DWORD dwItemID );
	int				DoAttackMelee( CMover *pTarget, OBJMSG dwMsg = OBJMSG_ATK1, DWORD dwItemID = 0 );
	int				DoAttackMelee( OBJID idTarget, OBJMSG dwMsg = OBJMSG_ATK1, DWORD dwItemID = 0 );
	int				DoDie( CCtrl *pAttackCtrl, DWORD dwMsg = 0 );	// ������.	
	void			ChangeFame( CMover *pLose );
	int				DuelResult( CMover *pLose );
	PVP_MODE		GetPVPCase( CMover *pAttacker );
	void			SubPVP( CMover *pAttacker, int nReflect );	// ����� ����� �ο򿡼� �׾�����.
	int				SubDuel( CMover *pAttacker );	// ����߿� �׾����� 
	int				SubPK( CMover *pAttacker, int nReflect );		// PK������ �׾�����.
	int				SubWar( CMover *pAttacker );	// ���ﶧ �׾���
	int				SubExperience( CMover *pDead );
	void			AddKillRecovery();
	[[nodiscard]] bool IsValidArea(const CMover * pMover, float fLength) const;
	void			SetJJim( CMover *pJJimer );		// this�� pJJimer�� ���ߴٴ°� ǥ��.
	void			SubReferTime(DWORD & pTime1, DWORD & pTime2, const ItemProp & pSkillProp, const AddSkillProp * pAddSkillProp) const;
	[[nodiscard]] DWORD GetReferTime(DWORD referTarget, DWORD referStat, DWORD referValue, DWORD skillLevel) const;
	BOOL			ApplyParam( CCtrl *pSrc, const ItemProp *pSkillProp, const AddSkillProp *pAddSkillProp, BOOL bSend = TRUE, int nDamage = 0 );	// this���� �Ķ���͸� ����. pSrc�� ������.
	BOOL			ApplyMagicSkill( CMover *pMover, DWORD dwSkill, DWORD dwLevel, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp );	// pMover���� ��ųȿ���� ������.
	BOOL			GetSkillProp( ItemProp **ppSkillProp, AddSkillProp **ppAddSkillProp, int nSkill, DWORD dwLevel, LPCTSTR szErr );
	BOOL			CreateSkillEffect( OBJID idTarget, int nSkill, DWORD dwLevel );		// ���� ��ų �ߵ�.
	void			RemoveInvisible();		// ����ȭ ���°ɷ������� ������.
	void			SendDamageAround( int nDmgType, CMover *pAttacker, int nApplyType, int nAttackID, float fRange, float fTargetRatio, float fAroundRatio );	// this�� �߽����� �ֺ��� �������� ����.
	void			SendDamageLine( int nDmgType, int nApplyType, int nAttackID, float fWidth, float fHeight, float fDepth );
	void			ChangeExpRatio( CMover* pAttacker, CMover* pDefender );
	void			OnAttacked( CMover* pAttacker, int nDamage, BOOL bTarget, int nReflect );
	BOOL			OnDamage( int nItemID, DWORD dwState, CMover *pHitObj, const D3DXVECTOR3 *pvDamagePos = NULL, DWORD dwAtkFlag = AF_GENERIC );
	void			SetCoolTime( AddSkillProp* pAddSkillProp, LPCTSTR szCall );
	BOOL			OnMeleeSkill( int nType, int nCount = 1 );		// OBJMSG_ATK_MELEESKILL������ Ÿ���� �Ǿ����� �߻��ϴ� �̺�Ʈ.
	BOOL			OnMagicSkill( int nType, int nCount = 1 );		// OBJMSG_ATK_MAGICSKILL������ Ÿ���� �Ǿ����� �߻��ϴ� �̺�Ʈ.
	BOOL			OnAttackRange();		// ��Ÿ����Ⱑ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ
	BOOL			OnAttackMelee( DWORD dwState, CMover *pHitObj );		// �ٰŸ����ݽ� Ÿ���� �Ǿ����� �߻��ϴ� �̺�Ʈ.
	BOOL			OnAttackMeleeContinue( DWORD dwState );		// ù��° Ÿ�����ĺ��� �ִϳ��������� ���.
	void			OnAttackMelee_ClockWorks( DWORD dwState, CMover *pHitObj );
	void			OnAttackMelee_BigMuscle( DWORD dwState, CMover *pHitObj );	
	void			OnAttackMelee_Krrr( DWORD dwState, CMover *pHitObj );	
	void			OnAttackMelee_Bear( DWORD dwState, CMover *pHitObj );	
	void			OnAttackMelee_Meteonyker( DWORD dwState, CMover *pHitObj );
	BOOL			OnAttackMagic();		// �ϵ������ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ.
	BOOL			OnAttackSP();			// Ư������.
	void			OnActDrop();			// ĳ���Ͱ� ���������� �������� �����ϴ� ������ �߻�.
	void			OnActCollision();		//
	void			OnActEndMeleeSkill();	// ������ų ������ ������ ������ �߻�.
	void			OnActEndMagicSkill();	// ������ų ������ ������ ������ �߻�.
	void			OnEndMeleeAttack( DWORD dwState );		// �Ϲݵ��� ���� ������ �߻�(����).
	void			OnActEndJump4();		// ���� �÷��װ� ������� ����.
	void			OnActLanding();		// ���� �ϴ� ����.
	void			OnActFMove();
	void			OnActIALanding( CObj *pIAObj, const D3DXVECTOR3 &vPos );		// Interact������Ʈ�� ����������.
	void			OnAttackStart( CMover *pTarget, OBJMSG dwMsg );					// ������ �� ����������(�Ϲ�/��ų/�ϵ� ��)
	BOOL			IsBullet( ItemProp* pItemProp );
	void			ProcessMovePattern();
	int				GetMovePattern() { return m_nMovePattern; }
	int				GetMoveEvent() { return m_nMoveEvent; }
	void			SetMovePattern( int nPattern );		// �̵����� ����.
//	BOOL			DoCollect( CMover *pTarget );
	void			BehaviorActionForceSet();		// ����� �ൿ ����̸� ���� �����ϰ�, ���� ����̸� �����Ѵ�.
	void			ActionForceSet( D3DXVECTOR3 &vPos, D3DXVECTOR3 &vDelta, FLOAT fAngle, 
									DWORD dwState, DWORD dwStateFlag, int nMotion, int nMotionEx, 
									int nLoop, DWORD dwMotionOption );
	void			ActionForceSet2( D3DXVECTOR3 &vPos, D3DXVECTOR3 &vDelta, FLOAT fAngle,
									 FLOAT fAngleX, FLOAT fAccPower, FLOAT fTurnAngle,
									 DWORD dwState, DWORD dwStateFlag, int nMotion, int nMotionEx, 
									 int nLoop, DWORD dwMotionOption );
	void			HalfForceSet( D3DXVECTOR3 & vPos, D3DXVECTOR3 & vd,	float fAngle, float fAnlgeX, 
		                          float fAccPower, float fTurnAngle );
	CGuild*			GetGuild();
	CGuildWar*		GetWar();
	BOOL			IsSMMode( int nType ) { return ( m_dwSMTime[nType] > 0 ) ? TRUE : FALSE; }
	void			ReState();
#ifdef __S_ADD_RESTATE
	void			ReStateOne( int nKind );
#endif // __S_ADD_RESTATE
#ifdef __ADD_RESTATE_LOW
	void			ReStateOneLow( int nKind );
#endif // __ADD_RESTATE_LOW
	float			GetJobPropFactor( JOB_PROP_TYPE type );
	int				CalcDefense( ATTACK_INFO* pInfo, BOOL bRandom = TRUE );
	int				CalcDefenseCore( CMover* pAttacker, DWORD dwAtkFlags, BOOL bRandom = TRUE );	
	int				CalcDefensePlayer( CMover* pAttacker, DWORD dwAtkFlags );
	int				CalcDefenseNPC( CMover* pAttacker, DWORD dwAtkFlags );
	void			PutLvUpSkillName_1( DWORD dwLevel );
	void			PutLvUpSkillName_2( DWORD dwSkill );
	BOOL			IsRegionMove( DWORD dwOlgRegionAttr, DWORD dwRegionAttr );
	DWORD			GetPKPVPRegionAttr();
	BOOL			IsAttackMode();
	void			AddSkillPoint( int nPoint);
	
	void			DoPVPEnd( CCtrl *pAttackCtrl, bool bWinner , DWORD dwMsg = 0 );	// ��󳡳��� ó��
	void			EndPVP(int	nPVPHP);	// ��󳡳��� ó��
	BOOL			m_bPVPEnd;			//����� �����°�

	BYTE			GetLegendChar()	;
	void			SetLegendChar(	BYTE	nLegend	)	{	m_nLegend	=	nLegend;	}
	BOOL			IsMaster();
	BOOL			IsHero();
	void			SetMasterSkillPointUp();

	DWORD			GetJobType( int nJob = NULL_ID )	{	if( nJob == NULL_ID ) nJob = m_nJob;	return prj.m_aJob[ nJob ].dwJobType;	}	// ���� Ÿ��


	void			AngelMoveProcess();
#ifdef __EVE_BALLOON
	void			BalloonMoveProcess();
#endif //__EVE_BALLOON

	void			OnAttackSFX( OBJID	idTarget, int nMagicPower, DWORD dwSkill, int nDmgCnt, float	fDmgAngle, float fDmgPower,  DWORD dwAtkFlags, BOOL bControl );
	BOOL			IsGuildCombatTarget( CMover* pMover );
#ifdef __WORLDSERVER
	void	ProcessSFXDamage( void );
	void	ProcessSFXExpire( void );
#endif	// __WORLDSERVER

	void	ProcessPet( void );

#ifdef __WORLDSERVER
	void			SetDestObj( OBJID idObj, float fRange = 0.0f, BOOL bSend = FALSE );
#else
	void			SetDestObj( OBJID idObj, float fRange = 0.0f, BOOL bSend = TRUE );
#endif

#if defined(__WORLDSERVER)
	BOOL			IsPVPInspection( CMover* pMover, int nFlag = 0 );
	void			AddExperienceKillMember( CMover *pDead, EXPFLOAT fExpValue, MoverProp* pMoverProp, float fFxpValue );
//	BOOL			IsResourceMonster() { return m_nResource != -1; }		// �ڿ����ͳ�? -1�̸� �ڿ����Ͱ� �ƴϴ�. 0 ~ �ڿ����Ͷ�� ��.
	void			ArrowDown( int nCount );
	int				GetQueueCastingTime();
	BOOL			DoUseSkill( int nType, int nIdx, OBJID idFocusObj, SKILLUSETYPE sutType, BOOL bControl );
	float			SubDieDecExp( BOOL bTransfer = TRUE, DWORD dwDestParam = 0, BOOL bResurrection = FALSE  );	// �׾����� ��ġ ��� �κ�.

	void			SubAroundExp( CMover *pAttacker, float fRange );		// this�� �߽����� fRange�����ȿ� �ִ� �������� ����ġ�� ����Ѵ�.
	void			GetDieDecExp( int nLevel, FLOAT& fRate, FLOAT& fDecExp, BOOL& bPxpClear, BOOL& bLvDown );
	void			GetDieDecExpRate( FLOAT& fDecExp, DWORD dwDestParam, BOOL bResurrection );
	BOOL			CreateItem( CItemElem * pItemBase, BYTE* pnId = NULL, short* pnNum = NULL, BYTE nCount = 0 );
	void			RemoveItem( BYTE nId, short nNum );
	int				RemoveItemA( DWORD dwItemId, short nNum );
	void			RemoveVendorItem( CHAR chTab, BYTE nId, short nNum );
	void			RemoveItemIK3( DWORD dwItemKind3 );
	BOOL			ReplaceInspection( REGIONELEM* pPortkey );
#ifdef __LAYER_1015
	BOOL			Replace( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3 & vPos, REPLACE_TYPE type, int nLayer );
#else	// __LAYER_1015
	BOOL			Replace( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3 & vPos, REPLACE_TYPE type );
#endif	// __LAYER_1015
	bool Replace(const CMover & pTarget, REPLACE_TYPE replaceType = REPLACE_TYPE::REPLACE_NORMAL);
	BOOL			IsLoot( CItem *pItem, BOOL bPet = FALSE );
	void			ProcInstantBanker();
	void			ProcInstantGC();
	BOOL			IsItemRedyTime( ItemProp* pItemProp, OBJID dwObjid, BOOL bItemFind );
	int				ApplyDPC( int ATK, ATTACK_INFO* pInfo );
	int				CalcLinkAttackDamage( int nDamage ); // ��Ƽ��ų:��ũ������ �߰� ���̹̰�� 	
	int				CalcPropDamage( CMover* pDefender, DWORD dwAtkFlags );
	int				CalcGenericDamage( CMover* pDefender, DWORD& dwAtkFlags );
	int				GetMagicSkillPower( ATTACK_INFO* pInfo );
	void			SubSMMode();
	void			ClearAllSMMode();
	BOOL			SetSMMode( int nType, DWORD dwTime );
	void			Abrade( CMover* pAttacker, int nParts = PARTS_RWEAPON );
	DWORD			GetRandomPartsAbraded();
	void			SetMarkingPos();
	void			RemoveSFX( OBJID idTarget, int id, BOOL bForce, DWORD dwSkill );
	int				GetSFXCount( OBJID idTarget );
	void			IncSFXCount( OBJID idTarget, DWORD dwSkill );
	void			AddSFXInfo( OBJID idTarget, SFXHIT_INFO& info );
	void			ClearSFX( OBJID idTarget );
	int				GetSummonState();
	BOOL			IsDoUseBuff( ItemProp* pItemProp );
	
#endif // __WORLDSERVER

#ifdef __CLIENT
	virtual void	Render( LPDIRECT3DDEVICE9 pd3dDevice );

	void			InitInterpolation();
	void			Interpolate();
	BOOL			IsStateDbuff();
	BOOL			EndMotion();
	BOOL			IsPKAttackAble( CMover* pMover );
	BOOL			IsPVPAttackAble( CMover* pMover );
	CSfx*			CreateSfxArrow( DWORD dwSfxObjArrow, DWORD dwSfxObjHit, D3DXVECTOR3 vPosDest, int idTarget );
	void			SetDmgCnt( int nDmgCnt ) { m_nDmgCnt = nDmgCnt; }
	void			CreateAbilityOption_SetItemSFX( int nAbilityOption );
	BOOL			GetEquipFlag( int nParts, BYTE* pbyFlag );
	void			OverCoatItemRenderCheck(CModelObject* pModel);
	void			PlayCombatMusic();
	BOOL			IsLoot( CItem *pItem ) { return TRUE; }
	LPCTSTR			GetFameName();						// �� �̸� ���
	LPCTSTR			GetJobString();						// ���� �̸� ��� 
	void			DialogOut( LPCTSTR lpszText );		// ��ǳ���� ���� ��� ���
	BOOL			DoFakeEquip( const EQUIP_INFO & rEquipInfo, BOOL bEquip, int nPart, CModelObject* pModel = NULL ); // for Fake
	void			RenderGauge( LPDIRECT3DDEVICE9 pd3dDevice, int nValue );
	void			RenderTurboGauge( LPDIRECT3DDEVICE9 pd3dDevice, DWORD nColor, int nValue, int nMaxValue );
	void			SetRenderPartsEffect( int nParts );
	void			RenderPartsEffect( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor = 0xffffffff );
	void			RenderHP(LPDIRECT3DDEVICE9 pd3dDevice);
	void			RenderCltGauge(LPDIRECT3DDEVICE9 pd3dDevice);
	void			RenderChrState(LPDIRECT3DDEVICE9 pd3dDevice);
	void			RenderFlag( int nType );
	void			RenderCasting( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderCtrlCasting( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderSkillCasting( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderPVPCount( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderQuestEmoticon( LPDIRECT3DDEVICE9 pd3dDevice );
	void			RenderGuildNameLogo( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor );
	BOOL			DoUseSkill( int nType, int nIdx, OBJID idFocusObj, SKILLUSETYPE sutType, BOOL bControl, const int nCastingTime, DWORD dwSkill = 0, DWORD dwLevel = 0 );
	void			RenderAngelStatus(LPDIRECT3DDEVICE9 pd3dDevice);
#endif	// __CLIENT

	int				GetSkillLevel( SKILL* pSkill );
	BOOL			SetExperience( EXPINTEGER nExp1, int nLevel );

	BOOL	IsUsing( CItemElem* pItemElem );

	BOOL	IsShoutFull( void );
	BOOL	IsCommBank( void );

private:
	void			ProcessMove();
	void			ProcessMoveArrival( CCtrl *pObj );
	void			ProcessRegenItem();
	void			ProcessRegion();
	void			ProcessQuest();
	void			ProcessIAObjLink( D3DXVECTOR3& vPos);
	void			ProcessActionPoint();
#ifdef __CLIENT
	void			ProcessScaleSlerp();
	void			ProcessWaterCircle( const D3DXVECTOR3& vPosTemp, const D3DXVECTOR3& vPos );
	void			ProcessWalkSound();
	void			ProcessDustSFX();
	void			ProcessETC();
#else
	void			ProcessScript();
	void			ProcessAbnormalState();
	void			ProcessTarget();
#endif // __CLIENT

	BOOL			DropItem( CMover* pAttacker );		// �׾ ����Ʈ���� ���.
	void			EnforcedGhostCorr();
	void			ApproachGhostAngle();
	void			OnArriveAtPos();
	void			OnArrive( DWORD dwParam1, DWORD dwParam2 );
	void			SetActParam( OBJACT act, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0 );
	int				GetActParam( int nIdx ) { return m_nAParam[nIdx]; }
	void			SetCmd( OBJACT cmd, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0 );
	void			SetCmdParam( int nIdx, int nValue ) { m_nCParam[nIdx] = nValue; }
	BOOL			__SetQuest( LPQUEST lpQuest, LPQUEST lpNewQuest );

public:
#ifdef __CLIENT
	void			CreateAngelParticle(D3DXVECTOR3 vPos);

	void			CreatePetParticle(D3DXVECTOR3 vPos);
	void			ProcessEyeFlash();
	void			WingMotionSetting( const CModelObject* pModel );
	float			GetRideFrameSpeed( void );

#ifdef __BS_EFFECT_LUA
	const char*		GetNameO3D( );
	BOOL			SetDataMTE( const char* fname1, const char* fname2 );
#endif

#endif //__CLIENT
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static			CMoverPool*		m_pPool;
	void*			operator new( size_t nSize )									{ return CMover::m_pPool->Alloc();	}
	void*			operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{ return CMover::m_pPool->Alloc();	}
	void			operator delete( void* lpMem )									{ CMover::m_pPool->Free( (CMover*)lpMem );	}
	void			operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{ CMover::m_pPool->Free( (CMover*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820

public:
	template<MultipleDsts DstList>
	void SetDSTs(const DstList & dsts) {
		for (const auto & dst : dsts) {
			SetDestParam(dst.nDst, dst.nAdj, NULL_CHGPARAM);
		}
	}

	template<MultipleDsts DstList>
	void ResetDSTs(const DstList & dsts) {
		for (const auto & dst : dsts) {
			ResetDestParam(dst.nDst, dst.nAdj);
		}
	}
};


// �������� �˾ƺ�
inline	BOOL CMover::IsMode( DWORD dwMode ) 
{ 
#ifdef __HACK_0516
	switch( dwMode )
	{
		case MATCHLESS_MODE:		// ���� ����
		case ONEKILL_MODE:		// ���� ����
		case MATCHLESS2_MODE:	// ���� ����2
			if( m_dwAuthorization == AUTH_GENERAL )
				return FALSE;
			break;
	}
#endif	// __HACK_0516
	if( (dwMode & TRANSPARENT_MODE) )	// ����ȭ�� �˻��Ϸ� ��������
		if( GetAdjParam(DST_CHRSTATE) & CHS_INVISIBILITY )	// ��������ȭ�� �ɷȴ°�?
			return TRUE;
	return ( ( m_dwMode & dwMode ) == dwMode ) ? TRUE : FALSE; 
}	

inline	BOOL CMover::IsStateMode( DWORD dwMode ) 
{ 
	return ( ( m_dwStateMode & dwMode ) == dwMode ) ? TRUE : FALSE; 
}	

inline BOOL CMover::IsFly() 
{ 
#ifdef _DEBUG
	if( IsNPC() )
	{
		Error( _T( "CMover::IsFly : NPC�� IsFly()�� ����ϸ� �ȵȴ�. IsFlyingNPC()�� ����ϼ�! %s" ), m_szName );
		return FALSE;
	}
#endif
	return m_pActMover->IsFly();
}

inline int	CMover::DoAttackMelee( OBJID idTarget, OBJMSG dwMsg, DWORD dwItemID )
{
	CMover *pTarget = (CMover *)prj.GetMover( idTarget );
	if( IsInvalidObj(pTarget) )
		return 0;

	return DoAttackMelee( pTarget, dwMsg, dwItemID );
}

#if !defined(__WORLDSERVER)
inline int CMover::IsSteal( OBJID idTaget )		// id�� �˻��ϴ� ����.
{
	CMover *pTarget = prj.GetMover( idTaget );
	if( IsInvalidObj(pTarget) )		
		return 0;
	else
		return 	IsSteal( pTarget );
}
#endif	// __WORLDSERVER

// �׼� �Ž����� ���� ; ��ü�� �������� �Ž����� ������ 
inline int CMover::SendActMsg( OBJMSG dwMsg, int nParam1, int nParam2, int nParam3, int nParam4 ) 	
{	
	if( m_pActMover )	
		return m_pActMover->SendActMsg( dwMsg, nParam1, nParam2, nParam3, nParam4 );	
	else
		return 0;
}

//raiders.2006.11.28	 trade���� ��꿡 �����ϴ� ���� ����
inline int CMover::GetGold()
{
	int nGold = m_dwGold;
//	nGold -= m_vtInfo.TradeGetGold();
	ASSERT( nGold >= 0 );
	return nGold;
}

inline void CMover::SetGold( int nGold )
{
	ASSERT( nGold >= 0 );
	m_dwGold = (DWORD)nGold;
}

inline void CMover::ClearCmd() 
{ 
//	TRACE( "ClearCmd %d->0 ", m_oaCmd );
	m_oaCmd = OBJACT_NONE; 
	memset( m_nCParam, 0xcd, sizeof(m_nCParam) ); 
}

inline void CMover::ClearActParam() 
{ 
	m_oaAct = OBJACT_NONE; 
	memset( m_nAParam, 0xcd, sizeof(m_nAParam) );
}


extern int GetWeaponPlusDamage( int nDamage, BOOL bRandom, ItemProp* pItemProp , int nOption );
extern int __IsEndQuestCondition( CMover* pMover, int nQuestId );
extern int __IsBeginQuestCondition( CMover* pMover, int nQuestId );
extern int __IsNextLevelQuest( CMover* pMover, int nQuestId );
extern	BOOL AttackBySFX( CMover* pAttacker, SFXHIT_INFO & info );

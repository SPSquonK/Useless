#pragma once

#include <array>
#include <vector>
#include <string>
#include <exception>
#include <span>
#include <boost/container/small_vector.hpp>
#include "FlyFFTypes.h"
#include "defineJob.h"
#include <variant>
#include <bitset>

#define	MAX_OBJARRAY			8
#define	MAX_INVENTORY			42			// 60 //42
#define MAX_GUILDBANK			42
#define	MAX_BANK				42
#define	MAX_TRADE				25
#define	MAX_VENDITEM			30
#define	MAX_REPAIRINGITEM		25
#define	MAX_VENDORNAME			48
#define	MAX_EXPCHARACTER		200
#define	MAX_FXPCHARACTER		200
#define	MAX_CHARACTER_LIST		3 
#define	MAX_VENDOR_INVENTORY	100
#define	MAX_VENDOR_INVENTORY_TAB	4
#define	MAX_QUEST				100
#define	MAX_COMPLETE_QUEST		300 
#define MAX_CHECKED_QUEST		5
#define MAX_PARTYLEVEL			10

static constexpr size_t MAX_PROPMOVER = 2000; // MoverProp�迭�� �ִ밹�� 

#define	MAX_HONOR_TITLE		150	// ����Ÿ��Ʋ ����
#define MAX_VENDOR_REVISION		20

#include "defineitemkind.h"

struct tagColorText {
	DWORD dwColor;
	CString lpszData;
};

struct CtrlProp {
	DWORD	dwID          = 0;		// ������	
	TCHAR	szName[64]    = _T("");	// �ѱ۸�Ī 
	DWORD	dwAI          = 0;		// AIInterface
	DWORD   dwCtrlKind1 = 0;
	DWORD   dwCtrlKind2 = 0;
	DWORD   dwCtrlKind3 = 0;
	DWORD   dwSfxCtrl   = 0;

	//��� �Ͽ�¡ ���� ������Ʈ �ΰ�?
	[[nodiscard]] bool IsGuildHousingObj() const noexcept { return CK1_GUILD_HOUSE == dwCtrlKind1; }
};

struct AddSkillProp
{
	static constexpr int NB_PROPS = 2;

	DWORD	dwID;				// ������	
	DWORD	dwName;				// ��Ī
	DWORD	dwSkillLvl;			// ��ų����
	DWORD	dwAbilityMin;		// �ּҴɷ�
	DWORD	dwAbilityMax;		// �ִ�ɷ�
	DWORD	dwAttackSpeed;		// ���ݼӵ�
	DWORD	dwDmgShift;			// Ÿ�ݽ� �ݵ�
	DWORD	nProbability;		// �ߵ� Ȯ��
	DWORD	dwTaunt;			// Ÿ��Ʈ
	DWORD	dwDestParam[2];		// ������1
	DWORD	nAdjParamVal[2];	// ���밪1;
	DWORD	dwChgParamVal[2];	//���뺯ȭ��1
	int		nDestData1[3];		// ���뵥��Ÿ3��, DestParam[0]�� �ش�.
	DWORD	dwActiveSkill;		// �ߵ� ����
	DWORD	dwActiveSkillRate;	// �ߵ� ���� Ȯ��.
	DWORD	dwActiveSkillRatePVP;	// ���� �ߵ� ���� Ȯ��.
	int		nReqMp;				//�ʿ�MP
	int		nReqFp;				//�ʿ�FP
	DWORD	dwCooldown;		// ��ٿ�
	DWORD	dwCastingTime;		//��� �غ�ð�
	DWORD	dwSkillRange;		// ��� �����Ÿ�	
	DWORD	dwCircleTime;		//�����ð�
	DWORD   dwPainTime;         // ���� ���ؽð�
	DWORD	dwSkillTime;		//���ӽð�
	int		nSkillCount;		// �߻�ü �߻� ����. ������ݷ� = �Ѱ��ݷ� / nSkillCount;
	DWORD   dwSkillExp;         //��ų ����ġ
	DWORD	dwExp;				//�������ġ
	DWORD	dwComboSkillTime;	//�޺���ųŸ��
	DWORD	dwAbilityMinPVP;	// �ּҴɷ�(����)
	DWORD	dwAbilityMaxPVP;	// �ִ�ɷ�(����)
	DWORD	nProbabilityPVP;	// �ߵ� Ȯ��(����)

	AddSkillProp()
	{
		dwID = dwName = 0;
		nProbability = 0;
		dwSkillLvl = dwAbilityMin = dwAbilityMax = dwAttackSpeed = dwDmgShift = dwTaunt = 0;
		dwDestParam[0] = dwDestParam[1] = nAdjParamVal[0] = nAdjParamVal[1] = dwChgParamVal[0] = dwChgParamVal[1] = 0;
		nReqMp = nReqFp = 0;
		dwActiveSkillRate	= 0;
		dwCastingTime	= 0;
		dwAbilityMinPVP		= 0;
		dwAbilityMaxPVP		= 0;
		dwCooldown	= 0;
		nProbabilityPVP	= 0;
		dwActiveSkillRatePVP	= 0;
		dwCircleTime = dwSkillTime = dwExp = dwComboSkillTime = 0;
	}
};

enum class _FILEWITHTEXT
{	
	FILE_FILTER	= 0, 
	FILE_INVALID = 1, 
	FILE_NOTICE = 2,	
	FILE_GUILDCOMBAT_TEXT_1 = 3, 
	FILE_GUILDCOMBAT_TEXT_2 = 4, 
	FILE_GUILDCOMBAT_TEXT_3 = 5, 
	FILE_GUILDCOMBAT_TEXT_4 = 6, 
	FILE_GUILDCOMBAT_TEXT_5 = 7,
	FILE_GUILDCOMBAT_TEXT_6 = 8,
	FILE_ALLOWED_LETTER	= 9,
	FILE_GUILDCOMBAT_1TO1_TEXT_1 = 10,
	FILE_GUILDCOMBAT_1TO1_TEXT_2 = 11,
	FILE_GUILDCOMBAT_1TO1_TEXT_3 = 12,
	FILE_GUILDCOMBAT_1TO1_TEXT_4 = 13,
	FILE_GUILDCOMBAT_1TO1_TEXT_5 = 14,
	FILE_ALLOWED_LETTER2	= 15
};

using enum _FILEWITHTEXT;

// Item Property Type
struct IP_FLAG {
	static constexpr size_t BINDS = 0;				// 1 - �ͼ� ������ 
	static constexpr size_t UNDESTRUCTABLE = 1;	//	2 - ���� �Ұ� ������
	static constexpr size_t EQUIP_BIND = 2;
	static constexpr size_t MAX = 3;
};

struct ItemProp
{
	static constexpr size_t NB_PROPS = 3;

	DWORD	dwID = 0;		// ������	
	TCHAR	szName[64] = _T("");	// �ѱ۸�Ī 

	DWORD	dwMotion;			// ���� 
	DWORD	dwNum;				// �⺻��������	
	DWORD	dwPackMax;			// �ִ��ħ����	
	DWORD	dwItemKind1;		// 1������������	
	DWORD	dwItemKind2;		// 2������������	
	DWORD	dwItemKind3;		// 3������������	
	DWORD	dwItemJob;			// �����������Ҽ� 
	BOOL	bPermanence;		// �Һ�Ұ�	
	DWORD	dwUseable;			// ������	
	DWORD	dwItemSex;			// ��뼺��	
	DWORD	dwCost;				// ����	 
	DWORD	dwEndurance;		// ������	
	int		nLog;				// �α�
	int		nMaxRepair;			// ����Ƚ��
	DWORD	dwHanded;			// �������	
	std::bitset<IP_FLAG::MAX> dwFlag;				// �ٸ��� �÷���	
	DWORD	dwParts;			// ������ġ	
	DWORD	dwPartsub;			// ������ġ	
	DWORD	bPartsFile;			// �������� ��뿩�� 
	DWORD	dwExclusive;		// ���ŵ���ġ	
	DWORD	dwBasePartsIgnore;
	DWORD	dwItemLV;			// �����۷���	
	DWORD	dwItemRare;			// ��ͼ�	
	DWORD   dwShopAble;
	int		nShellQuantity;		// �ִ���������	- IK�� ä�������϶��� �ִ� ä������ �ȴ�.(���� ����)
	DWORD	dwActiveSkillLv;	// �ߵ� ���� ����
	DWORD   dwFuelRe;			// �Ѵ翬��������
	DWORD	dwAFuelReMax;		// ���ӿ���������
	DWORD	dwSpellType;		// �����Ӽ�	
	DWORD	dwLinkKindBullet;	// �ʿ� �Ҹ���� ������	
	DWORD	dwLinkKind;			// �ʿ� ����������
	DWORD	dwAbilityMin;		// �ּҴɷ�ġ - ���ݷ�, ����, ���� ��Ÿ��� 
	DWORD	dwAbilityMax;		// �ִ�ɷ�ġ - ���ݷ�, ����, ���� ��Ÿ��� 
	BOOL	bCharged;		
	SAI79::ePropType	eItemType;
	short	wItemEatk;			// �Ӽ� ������( �� �Ӽ� Ÿ������ ���ݷ��� �����Ѵ�. )
	DWORD   dwParry;			// ȸ���� 
	DWORD   dwblockRating;		// �� ��ġ 
	int		nAddSkillMin;		// �ּ� �߰� ��ų
	int		nAddSkillMax;		// �ִ� �߰� ��ų.
	DWORD	dwAtkStyle;			// ���� ��Ÿ�� 
	DWORD	dwWeaponType;		// �������� 
	DWORD	dwItemAtkOrder1;	// ������1�����ݼ���
	DWORD	dwItemAtkOrder2;	// ������2�����ݼ���
	DWORD	dwItemAtkOrder3;	// ������3�����ݼ���
	DWORD	dwItemAtkOrder4;	// ������4�����ݼ���
    DWORD	tmContinuousPain;	// ���� ���� 
	DWORD	dwRecoil;			// �ݵ�	
	DWORD	dwLoadingTime;		// �����ð�	- IK�� ä�������϶��� ä���ӵ�(�ɷ�)�� �ȴ�.
	LONG	nAdjHitRate;		// �߰����ݼ�����	
	FLOAT	fAttackSpeed;		// ���ݼӵ�	
	DWORD	dwDmgShift;			// Ÿ�ݽ� �ݵ�	
	DWORD	dwAttackRange;		// ���ݹ���	
	int		nProbability;		// ����Ȯ��
	DWORD	dwDestParam[NB_PROPS];		// ������1	
	LONG	nAdjParamVal[NB_PROPS];	// ���밪1	
	DWORD	dwChgParamVal[NB_PROPS];	// ���뺯ȭ��1	
	int		nDestData1[NB_PROPS];		// ���뵥��Ÿ�� 3��, destParam1���� �ش��.
	DWORD	dwActiveSkill;		// �ߵ� ����
	DWORD	dwActiveSkillRate;	// �ߵ� ���� Ȯ��.
	DWORD	dwReqMp;			// �ʿ�MP	
	DWORD	dwReqFp;			// �ʿ�FP	
	DWORD	dwReqDisLV;
	DWORD   dwReSkill1;
	DWORD   dwReSkillLevel1;
	DWORD   dwReSkill2;
	DWORD   dwReSkillLevel2;
	DWORD	dwSkillReadyType;
	DWORD	dwSkillReady;		// ��� �غ�ð�	
	DWORD	_dwSkillRange;		// ��� �����Ÿ�	
	DWORD	dwSfxElemental;
	DWORD	dwSfxObj;			// ������ ȿ�� ������Ʈ 
	DWORD	dwSfxObj2;			// ������ ȿ�� ������Ʈ 
	DWORD	dwSfxObj3;			// �ߵ�ȿ��, Ÿ��
	DWORD	dwSfxObj4;			// ���ӵǴ� ȿ�� ����Ʈ.
	DWORD	dwSfxObj5;			// �ߵ�ȿ��, ������ 
	DWORD	dwUseMotion;		// ��� ���
	DWORD	dwCircleTime;		// �����ð� 
	DWORD	dwSkillTime;		// ���ӽð�	
	DWORD	dwExeTarget;
	DWORD	dwUseChance;
	DWORD	dwSpellRegion;		// ���� ���� 
	DWORD   dwReferStat1;
	DWORD   dwReferStat2;
	DWORD   dwReferTarget1;
	DWORD   dwReferTarget2;
	DWORD   dwReferValue1;
	DWORD   dwReferValue2;
	DWORD	dwSkillType;		// ����Ӽ�	
	int		nItemResistElecricity;
	int		nItemResistFire;
	int		nItemResistWind;
	int		nItemResistWater;
	int		nItemResistEarth;
	LONG	nEvildoing;			// ����	
	DWORD	dwExpertLV;	
	DWORD	dwExpertMax;		// �ִ���÷��� 
	DWORD	dwSubDefine;
	DWORD	dwExp;				// �������ġ	
	DWORD	dwComboStyle;
	FLOAT	fFlightSpeed;		// ����������(�ӵ�)
	FLOAT	fFlightLRAngle;		// �¿� �� ����.
	FLOAT	fFlightTBAngle;		// ���� �� ����.
	DWORD	dwFlightLimit;		// �������ѷ���
	DWORD	dwFFuelReMax;		// ���࿬��������
	DWORD	dwLimitLevel1;		// ���ѷ���1
	int		nReflect;			// ���÷��� �ɼ�.
	DWORD	dwSndAttack1;		// ȿ���� : ���� 1
	DWORD	dwSndAttack2;		// ȿ���� : ���� 2
	QuestId	dwQuestId;
	TCHAR	szTextFileName[64];	// item�� GM command�� �ִ� �Ϳ� ���

#ifdef __CLIENT
    TCHAR	szIcon[64];			// dds���� �̸� 
	TCHAR	szCommand[256];		// ���� 
#endif
	int		nVer;

#ifdef __VERIFY_0201
	TCHAR	szIcon[64];
	TCHAR	szCommand[256];		// ���� 
#endif	// __VERIFY_0201

	[[nodiscard]] DWORD GetCoolTime() const noexcept { return dwSkillReady; }
	[[nodiscard]] bool IsUltimate() const noexcept {
		return (dwItemKind2 == IK2_WEAPON_DIRECT || dwItemKind2 == IK2_WEAPON_MAGIC)
			&& dwReferStat1 == WEAPON_ULTIMATE;
	}
	BOOL	IsVisPet()	{ return (dwItemKind3 == IK3_PET) && (dwReferStat1 == PET_VIS); }
	[[nodiscard]] bool IsVis() const noexcept { return dwItemKind3 == IK3_VIS; }
#ifdef __CLIENT
	BOOL	IsVisKey()	{ return (II_SYS_SYS_VIS_KEY01 == dwID ); }
	BOOL	IsPickupToBuff( ) { return ( II_SYS_SYS_SCR_PET_MAGIC == dwID ); }
#endif //__CLIENT

#ifdef __CLIENT
	[[nodiscard]] CTexture * GetTexture() const;
#endif
};

class CItemElem;

namespace ItemProps {
	class PiercingType {
		enum class V { None, LetterCard, NumericCard, Vis };
	public:
		using enum V;

		constexpr PiercingType(const V value) : m_value(value) {}
		constexpr operator V() const { return m_value; }

		constexpr bool IsOnEquipement() const {
			return m_value == V::LetterCard || m_value == V::NumericCard;
		}

		[[nodiscard]] unsigned int GetNumberOfPiercings() const;

	private:
		V m_value;
	};

	[[nodiscard]] bool IsOrichalcum(const CItemElem & item);
	[[nodiscard]] bool IsOrichalcum(const ItemProp & item);

	[[nodiscard]] bool IsMoonstone(const CItemElem & item);
	[[nodiscard]] bool IsMoonstone(const ItemProp & item);

#ifdef __CLIENT
	[[nodiscard]] CTexture * GetItemIconTexture(DWORD itemId);
#endif

}

// ������ ���� ������Ƽ ( propJob.inc���� �о���� )
struct JobProp {
	static const JobProp NullObject;
	float AttackSpeed = 1.0f;			//���� 
	float FactorMaxHP = 1.0f;			//�ִ� HP ��꿡 ���Ǵ� factor
	float FactorMaxMP = 1.0f;			//�ִ� MP ��꿡 ���Ǵ� factor
	float FactorMaxFP = 1.0f;			//�ִ� FP ��꿡 ���Ǵ� factor
	float FactorDef = 1.0f;				//���� ���� ��꿡 ���Ǵ� factor
	float FactorHPRecovery = 1.0f;		//HPȸ�� factor
	float FactorMPRecovery = 1.0f;		//MPȸ�� factor
	float FactorFPRecovery = 1.0f;		//FPȸ�� factor
	float MeleeSWD = 1.0f;				//WT_MELEE_SWD�� ATK factor
	float MeleeAXE = 1.0f;				//WT_MELEE_AXE�� ATK factor
	float MeleeSTAFF = 1.0f; 			//WT_MELEE_STAFF�� ATK factor
	float MeleeSTICK = 1.0f;			//WT_MELEE_STICK�� ATK factor
	float MeleeKNUCKLE = 1.0f;			//WT_MELEE_KNUCKLE�� ATK factor
	float MagicWAND = 1.0f;				//WT_MAGIC_WAND�� ATK factor 
	float Blocking = 1.0f;				//���ŷ factor
	float MeleeYOYO = 1.0f;				//����� ATK factor 
	float Critical = 1.0f;				//ũ��Ƽ�� ó��
};

struct JobSkills : std::array<std::vector<const ItemProp *>, MAX_JOB> {
	void Load(CFixedArray<ItemProp> & aPropSkill);
};

/*----------------------------------------------------------------------------------------------------*/
struct DROPKIND {
	DWORD	dwIK3;
	short	nMinUniq;
	short	nMaxUniq;
};

struct QUESTITEM {
	QuestId	dwQuest;
	DWORD   dwState;
	DWORD	dwIndex;
	DWORD	dwProbability;
	DWORD	dwNumber; 
};

class CDropItemGenerator final {
public:
	struct Item {
		DWORD	dwIndex;       // Id of item
		DWORD	dwProbability; // Probability over 3e+9
		DWORD	dwLevel;       // Upgrade of dropped item
		DWORD	dwNumber;      // Quantity

		[[nodiscard]] bool IsDropped(BOOL bUniqueMode, float fProbability = 0.0f) const;
	};

	struct Money {
		DWORD	dwNumber;      // Min quantity
		DWORD	dwNumber2;     // Max quantity, exclusive
	};

private:
	std::vector<Item>	m_items;
	boost::container::small_vector<Money, 1> m_money;

public:
	DWORD				m_dwMax = 0;

public:
	void Add(const Item  & rDropItem ) { m_items.emplace_back(rDropItem); }
	void Add(const Money & rDropMoney) { m_money.emplace_back(rDropMoney); }

	[[nodiscard]] std::span<const Item > GetItems() const { return m_items; }
	[[nodiscard]] std::span<const Money> GetMoney() const { return m_money; }
};

using CDropKindGenerator  = std::vector<DROPKIND>;
using CQuestItemGenerator = std::vector<QUESTITEM>;

struct MonsterTransform {
	float fHPRate = 0.0f;
	DWORD dwMonsterId = NULL_ID;
};
/*----------------------------------------------------------------------------------------------------*/

struct MoverProp : CtrlProp
{
	DWORD dwStr = 0;	// ��,                  
	DWORD dwSta = 0;	// ü��,
	DWORD dwDex = 0;	// ��ø,
	DWORD dwInt = 0;	// ����,
	DWORD dwHR  = 0;
	DWORD dwER  = 0;
	DWORD	dwBelligerence = 0;		// ȣ����,
	DWORD	dwLevel   = 0;	// ����,
	DWORD	dwSize    = 0;		// ũ��,
	DWORD dwClass   = 0;
	BOOL	bIfParts  = FALSE;	// ������?
	int		nChaotic  = 0;	// ���۳� ���̳ʽ�/ ������ �÷���
	DWORD	dwUseable = 0;	// ��� ĳ����,
	DWORD	dwAtkMin  = 0;	// �ּ�Ÿ��ġ,
	DWORD	dwAtkMax  = 0;	// �ִ�Ÿ��ġ,
	DWORD	dwAtk1    = 0;
	DWORD	dwAtk2    = 0;
	DWORD	dwAtk3    = 0;
	DWORD	dwAtk4    = 0;		// dwHorizontalRate�� �̰ɷ� �ٲ�.
	FLOAT	fFrame    = 1.0f;	// �̵� �� ������ ����ġ

	DWORD	dwAttackSpeed   = 0;	// ���ݼӵ�,
	DWORD	dwReAttackDelay = 0;
	DWORD	dwAddHp         = 0;		// ,
	DWORD	dwAddMp         = 0;		// ,
	DWORD	dwNaturalArmor  = 0;	// �ڿ����� 

	SAI79::ePropType eElementType = SAI79::NO_PROP;
	short wElementAtk             = 0;		// �Ӽ� ������( �� �Ӽ� Ÿ������ ���ݷ��� �����Ѵ�. )

	DWORD	dwHideLevel = 0;	// ���� �Ⱥ��̴³��̳�..
	FLOAT	fSpeed = 0.0f;	// �̵��ӵ�,
	DWORD	dwFlying    = 0;	// ��������,
	DWORD	dwResisMgic = 0;	//�������� 

	int		nResistElecricity = 0;
	int		nResistFire       = 0;
	int		nResistWind       = 0;
	int		nResistWater      = 0;
	int		nResistEarth      = 0;
	
	DWORD	dwSourceMaterial  = 0;	// �������
	DWORD	dwMaterialAmount  = 0;	// ����
	DWORD	dwHoldingTime     = 0;	// ��ü�����ð�
	DWORD	dwCorrectionValue = 0;	// �����ۻ���������
	EXPINTEGER	nExpValue = 0;
	int		nFxpValue = 0;		// �������ġ.
	DWORD	bKillable = 0;	// ��������,

	DWORD   dwSndDmg1  = 0;
	DWORD   dwSndDmg2  = 0;
	DWORD   dwSndIdle1 = 0;

	short m_nEvasionHP     = 0;
	short m_nEvasionSec    = 0;
	short m_nRunawayHP     = 0; // HP�� 10 ���ϸ� ���� 
	short m_nCallHelperMax = 0; // �� ���� 
	short m_nCallHP        = 0; // �����û�ϱ� ���� HP
	short m_nCallHelperIdx  [ 5 ]; // �����û�ϱ� Id
	short m_nCallHelperNum  [ 5 ]; // �����û�ϱ� Id
	short m_bCallHelperParty[ 5 ]; // �����û�ϱ� Id

	short m_dwAttackMoveDelay = 0;
	short m_dwRunawayDelay    = 0;

#if !defined(__CORESERVER) 
	int		m_nScanJob = 0;		// Ÿ���� �˻��Ҷ� Ư�� �������� �˻��ϴ°�. 0 �̸� ALL
	// �������ǵ��� ��Ʈ���� �������� �����Ǿ� ������ ����.
	int		m_nHPCond         = 0;		// Ÿ���� hp% �����϶� ������ ��.
	int		m_nLvCond         = 0;		// Ÿ�ٰ� �������Ͽ� ������ ������ ��.
	int		m_nRecvCondMe     = 0;	// ȸ�� ����.  ��hp�� ��%���Ϸ� ����������?
	int		m_nRecvCondHow    = 0;	// ȸ���Ҷ� ȸ���� �� %����
	int		m_nRecvCondMP     = 0;	// ȸ���Ҷ� MP�Ҹ� %����
	BYTE	m_bMeleeAttack    = 0;	// ���� ���� AI�� �ִ°�?
	BYTE	m_bRecvCondWho    = 0;	// ������ ġ���Ҳ���. 0:�����ȵ� 1:�ٸ��� 2:�� 3:���.
	BYTE	m_bRecvCond       = 0;	// ġ���ϳ�? 0:ġ����� 1:�����߿��� ġ���� 2:����/������ ��� ġ��
	BYTE	m_bHelpWho        = 0;		// �����û�� - 0:�θ������� 1:�ƹ���  2:����������.
	BYTE	m_bRangeAttack[ MAX_JOB ];		// �� ������ ���Ÿ� ���� �Ÿ�.
	int		m_nSummProb       = 0;	// ��ȯ Ȯ�� : 0�̸� ��ȯ�ɷ� ����.
	int		m_nSummNum        = 0;		// �ѹ��� ����� ��ȯ�ϳ�.
	int		m_nSummID         = 0;		// � ����?
	int		m_nHelpRangeMul   = 0;	// �����û �Ÿ�. �þ��� ���
	DWORD	m_tmUnitHelp      = 0;			// ���� Ÿ�̸�.
	int		m_nBerserkHP      = 0;		// ����Ŀ�� �Ǳ� ���� HP%
	float	m_fBerserkDmgMul  = 0.0f;	// ����Ŀ�� �Ǿ����� ������ ���.
	int		m_nLoot           = 0;			// ���ø��ΰ�.
	int		m_nLootProb       = 0;		// ���� Ȯ��
	QuestId   m_dwScanQuestId = QuestIdNone;
	DWORD   m_dwScanItemIdx = 0; 
	int		m_nScanChao       = 0;		// ī��, ��ī�� �˻�
#endif // !__CORESERVER
	
#ifndef __CORESERVER
	float	m_fHitPoint_Rate    = 0.0f;		// ���� �ִ� HP�� // dwAddHp * m_nHitPoint_Rate
	float	m_fAttackPower_Rate = 0.0f;	// ���� �ִ� ���ݷ� // dwAtkMin * m_nAttackPower_Rate
	float	m_fDefence_Rate     = 0.0f;		// ���� �ִ� ���� // dwAddHp * m_nDefence_Rate
	float	m_fExp_Rate         = 0.0f;			// ���� �ִ� ����ġ�� // dwAddHp * m_nExp_Rate
	float	m_fItemDrop_Rate    = 0.0f;		// ���� �ִ� ������ ��ӷ� // dwAddHp * m_nItemDrop_Rate
	float	m_fPenya_Rate       = 0.0f;			// ���� �ִ� ��ķ� // dwAddHp * m_nPenya_Rate
	BOOL	m_bRate = FALSE;
#endif

	
	short   m_nAttackFirstRange = 0;
	CDropItemGenerator	m_DropItemGenerator;
	CQuestItemGenerator		m_QuestItemGenerator;
	CDropKindGenerator	m_DropKindGenerator;
	MonsterTransform	m_MonsterTransform;
	
	MoverProp()
	{
		memset( m_nCallHelperIdx, 0, sizeof(m_nCallHelperIdx) );
		memset( m_nCallHelperNum, 0, sizeof(m_nCallHelperNum) );
		memset( m_bCallHelperParty, 0, sizeof(m_bCallHelperParty) );

#if !defined(__CORESERVER) 
		memset( m_bRangeAttack, 0, sizeof(m_bRangeAttack) );
#endif // !__CORESERVER
	}
};

#ifdef __WORLDSERVER
enum class SpawnType : bool { Region, Script };

struct CtrlSpawnInfo {
	int       spawnId;
	SpawnType type;
};
#endif

#define TASKBAR_TOP    0
#define TASKBAR_BOTTOM 1
#define TASKBAR_LEFT   2
#define TASKBAR_RIGHT  3

// short cut
#define MAX_SLOT_APPLET			18
#define MAX_SLOT_ITEM			9
#define MAX_SLOT_QUEUE			5
#define	MAX_SLOT_ITEM_COUNT		8

enum class ShortcutType {
	None = 0,
	Etc = 1,
	Applet = 2,
	Motion = 3,
	Script = 4,
	Item = 5,
	Skill = 6,
	Object = 7,
	Chat = 8,
	SkillFun = 9,
	Emoticon = 10,
	Lord = 11,
	PartySkill = 12
};

#if defined ( __CLIENT )
class CWndBase;
#endif	// __CLIENT

class CAr;

struct SHORTCUT {
#if defined ( __CLIENT )
	CWndBase * m_pFromWnd = nullptr;
	CTexture * m_pTexture = nullptr;
	DWORD	m_dwItemId = 0;
#endif	// __CLIENT
	ShortcutType m_dwShortcut = ShortcutType::None;
	DWORD     m_dwId = 0;
	DWORD     m_dwIndex = 0;
	DWORD     m_dwData = NULL;
	TCHAR     m_szString[MAX_SHORTCUT_STRING] = ""; // SHORTCUT_CHAT�� ��� ����.
	BOOL IsEmpty() const { return m_dwShortcut == ShortcutType::None; }
	void Empty() { m_dwShortcut = ShortcutType::None; }

	friend CAr & operator<<(CAr & ar, const SHORTCUT & self);
	friend CAr & operator>>(CAr & ar, SHORTCUT & self);

	// ==========================================================================

	struct Source {
		struct Inventory { DWORD itemPos; };
		struct Penya {};
		struct Bag { int bagId; DWORD itemPos; };
		struct Bank { int slot; DWORD itemPos; };
		struct BankPenya { int slot; };
		struct Guild { DWORD itemPos; };
		struct GuildMoney {};
	};

	struct Sources {
		using ItemOrMoney = std::variant<
			Source::Inventory, Source::Penya,
			Source::Bank, Source::BankPenya,
			Source::Guild, Source::GuildMoney,
			Source::Bag
		>;
	};
};
using LPSHORTCUT = SHORTCUT *;

struct EXPPARTY {
	DWORD	Exp;
	DWORD	Point;
};

struct EXPCHARACTER {
	EXPINTEGER	nExp1;
	DWORD dwLPPoint;
	EXPINTEGER	nLimitExp;
};

#include "NameValidation.h"

class ProjectLoadError : public std::exception {
	const char * m_errorMessage;
	char m_fullMessage[512] = "Error when loading a file: ";
public:
	ProjectLoadError(const char * errorMessage, const char * fileName)
		: m_errorMessage(errorMessage) {
		if (errorMessage != nullptr && std::strlen(errorMessage) < 400) {
			std::strcat(m_fullMessage, errorMessage);
		}
		if (fileName != nullptr && std::strlen(fileName) < 70) {
			std::strcat(m_fullMessage, " - Filename= ");
			std::strcat(m_fullMessage, fileName);
		}
	}

	[[nodiscard]] const char * what() const { return m_fullMessage; }
};

#include "ProjectJob.h"

enum class LoadPropItemStyle {
	V15, V22SkillsSkip
};


#pragma once

#include <array>
#include <vector>
#include <string>
#include <exception>
#include <span>
#include <boost/container/small_vector.hpp>
#include "FlyFFTypes.h"
#include "defineJob.h"

#define	MAX_OBJARRAY			8
#define	MAX_QUICKSLOT			21
#define	MAX_EQUIPMENT			18
#define	MAX_INVENTORY			42			// 60 //42
#define MAX_GUILDBANK			42
#define	MAX_BANK				42
#define	MAX_REPAIR				16
#define	MAX_TRADE				25
#define	MAX_VENDITEM			30
#define	MAX_REPAIRINGITEM		25
#define	MAX_VENDORNAME			48
#define	MAX_STUFF				( MAX_INVENTORY + MAX_QUICKSLOT )
#define	MAX_EXPLPPOINT			41
#define	MAX_EXPSKILL			21
#define	MAX_EXPCHARACTER		200
#define	MAX_FXPCHARACTER		200
#define	MAX_CHARACTER_LIST		3 
#define	MAX_VENDOR_INVENTORY	100
#define	MAX_VENDOR_INVENTORY_TAB	4
#define	MAX_QUEST				100
#define	MAX_COMPLETE_QUEST		300 
#define MAX_CHECKED_QUEST		5
#define MAX_PARTYLEVEL			10
#define MAX_RENEWEVENT			8000
#define MAX_REALITEM			8			// �̺�Ʈ ��ǰ ������
#define MAX_EVENTREALITEM		4096
#define MAX_EXPJOBLEVEL			90
#define	ITEM_KIND_MAX			100

const int	MAX_PROPMOVER =		2000;		// MoverProp�迭�� �ִ밹�� 

#define	MAX_HONOR_TITLE		150	// ����Ÿ��Ʋ ����

#define MAX_VENDOR_REVISION		20

#define dwDestParam1	dwDestParam[0]
#define dwDestParam2	dwDestParam[1]
#define nAdjParamVal1	nAdjParamVal[0]
#define nAdjParamVal2	nAdjParamVal[1]
#define dwChgParamVal1	dwChgParamVal[0]
#define dwChgParamVal2	dwChgParamVal[1]

#ifdef __PROP_0827
#define dwDestParam3	dwDestParam[2]
#define nAdjParamVal3	nAdjParamVal[2]
#define dwChgParamVal3	dwChgParamVal[2]
#endif	// __PROP_0827

#include "defineitemkind.h"

struct tagColorText
{
	DWORD dwColor;
	TCHAR * lpszData;
}; 

struct ObjProp
{
	DWORD	dwID;		// ������	
	TCHAR	szName[64];	// �ѱ۸�Ī 
	DWORD	dwType; 
	DWORD	dwAI;		// AIInterface
	DWORD	dwHP;
	ObjProp()
	{
		*szName	= '\0';
		dwID	= dwType	= dwAI	= dwHP	= 0;
	}
};

struct CtrlProp : ObjProp
{
	DWORD   dwCtrlKind1;
	DWORD   dwCtrlKind2;
	DWORD   dwCtrlKind3;
	DWORD   dwSfxCtrl;
	DWORD   dwSndDamage;
	CtrlProp() : ObjProp()
	{
		dwCtrlKind1	= dwCtrlKind2	= dwCtrlKind3	= dwSfxCtrl	= dwSndDamage	= 0;
	}

	BOOL IsGuildHousingObj( )	{ return CK1_GUILD_HOUSE == dwCtrlKind1; } //��� �Ͽ�¡ ���� ������Ʈ �ΰ�?
	BOOL IsHousingObj( )		{ return CK1_HOUSING == dwCtrlKind1; }		//���� �Ͽ�¡ ���� ������Ʈ 
};

struct AddSkillProp
{
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

struct PartySkillProp
{
	DWORD	dwID;				// ������	
	DWORD	dwName;				// ��Ī
	DWORD	dwSkillLvl;			// ��ų����
	DWORD	dwAbilityMin;		// �ּҴɷ�
	DWORD	dwAtkAbilityMax;	// �ִ�ɷ�
	DWORD	dwAttackSpeed;		// ���ݼӵ�
	DWORD	dwDmgShift;			// Ÿ�ݽ� �ݵ�
	DWORD	dwGroggy;			// �׷�Ű
	DWORD	dwTaunt;			// Ÿ��Ʈ
	DWORD	dwDestParam[2];		// ������1
	DWORD	nAdjParamVal[2];	// ���밪1;
	DWORD	dwChgParamVal[2];	//���뺯ȭ��1
	DWORD	dwReqMp;			//�ʿ�MP
	DWORD	dwReqFp;			//�ʿ�FP
	DWORD	dwSkillReady;		//��� �غ�ð�
	DWORD	dwCircleTime;		//�����ð�
	DWORD	dwSkillTime;		//���ӽð�
	DWORD	dwExp;				//�������ġ
	DWORD	dwComboSkillTime;	//�޺���ųŸ��
	PartySkillProp()
	{
		dwID = dwName = 0;
		dwSkillLvl = dwAbilityMin = dwAtkAbilityMax = dwAttackSpeed = dwDmgShift = dwGroggy = dwTaunt = 0;
		dwDestParam[0] = dwDestParam[1] = nAdjParamVal[0] = nAdjParamVal[1] = dwChgParamVal[0] = dwChgParamVal[1] = 0;
		dwReqMp = dwReqFp = dwSkillReady = 0;
		dwCircleTime = dwSkillTime = dwExp = dwComboSkillTime = 0;
	}
};

enum 
{	
	FILE_FILTER	= 0, 
	FILE_INVALID = 1, 
	FILE_NOTICE = 2,	
	FILE_GUILDCOMBAT_TEXT_1 = 3, 
	FILE_GUILDCOMBAT_TEXT_2 = 4, 
	FILE_GUILDCOMBAT_TEXT_3 = 5, 
	FILE_GUILDCOMBAT_TEXT_4 = 6, 
	FILE_GUILDCOMBAT_TEXT_5 = 7,
	FILE_GUILDCOMBAT_TEXT_6 = 8
#ifdef __RULE_0615
	,FILE_ALLOWED_LETTER	= 9
#endif	// __RULE_0615
	,
	FILE_GUILDCOMBAT_1TO1_TEXT_1 = 10,
	FILE_GUILDCOMBAT_1TO1_TEXT_2 = 11,
	FILE_GUILDCOMBAT_1TO1_TEXT_3 = 12,
	FILE_GUILDCOMBAT_1TO1_TEXT_4 = 13,
	FILE_GUILDCOMBAT_1TO1_TEXT_5 = 14
#ifdef __VENDOR_1106
	,FILE_ALLOWED_LETTER2	= 15
#endif	// __VENDOR_1106
};

// Item Property Type
enum IP_TYPE 
{
	IP_FLAG_NONE	= 0x00,				// �÷��� ���� 
	IP_FLAG_BINDS	= 0x01,				// 1 - �ͼ� ������ 
	IP_FLAG_UNDESTRUCTABLE	= 0x02	//	2 - ���� �Ұ� ������
	,IP_FLAG_EQUIP_BIND	= 0x04
//	0x04
};

struct ItemProp : CtrlProp
{
#ifdef __PROP_0827
	static constexpr size_t NB_PROPS = 3;
#else
	static constexpr size_t NB_PROPS = 2;
#endif

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
	int		nAbrasion;			// ������
	int		nMaxRepair;			// ����Ƚ��
	DWORD	dwHanded;			// �������	
	DWORD	dwFlag;				// �ٸ��� �÷���	
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
	int		nItemResistDark;
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


// ������ ���� factor ENUM
enum JOB_PROP_TYPE
{
	JOB_PROP_SWD,
	JOB_PROP_AXE,	
	JOB_PROP_STAFF,
	JOB_PROP_STICK,
	JOB_PROP_KNUCKLE,
	JOB_PROP_WAND,
	JOB_PROP_BLOCKING,
	JOB_PROP_YOYO,
	JOB_PROP_CRITICAL,
};

// ������ ���� ������Ƽ ( propJob.inc���� �о���� )
struct JobProp
{
	float	fAttackSpeed;			//���� 
	float	fFactorMaxHP;			//�ִ� HP ��꿡 ���Ǵ� factor
	float	fFactorMaxMP;			//�ִ� MP ��꿡 ���Ǵ� factor
	float	fFactorMaxFP;			//�ִ� FP ��꿡 ���Ǵ� factor
	float   fFactorDef;				//���� ���� ��꿡 ���Ǵ� factor
	float	fFactorHPRecovery;		//HPȸ�� factor
	float	fFactorMPRecovery;		//MPȸ�� factor
	float	fFactorFPRecovery;		//FPȸ�� factor
	float	fMeleeSWD;				//WT_MELEE_SWD�� ATK factor
	float	fMeleeAXE;				//WT_MELEE_AXE�� ATK factor
	float   fMeleeSTAFF; 			//WT_MELEE_STAFF�� ATK factor
	float   fMeleeSTICK;			//WT_MELEE_STICK�� ATK factor
	float   fMeleeKNUCKLE;			//WT_MELEE_KNUCKLE�� ATK factor
	float   fMagicWAND;				//WT_MAGIC_WAND�� ATK factor 
	float   fBlocking;				//���ŷ factor
	float	fMeleeYOYO;				//����� ATK factor 
	float   fCritical;				//ũ��Ƽ�� ó��
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
	DWORD	dwStr;	// ��,                  
	DWORD	dwSta;	// ü��,
	DWORD	dwDex;	// ��ø,
	DWORD	dwInt;	// ����,
	DWORD	dwHR;
	DWORD   dwER;
	DWORD	dwRace;		// ����,
	DWORD	dwBelligerence;		// ȣ����,
	DWORD	dwGender;	// ����,
	DWORD	dwLevel;	// ����,
	DWORD	dwFlightLevel;	// ���෹��
	DWORD	dwSize;		// ũ��,
	DWORD   dwClass;
	BOOL	bIfParts;	// ������?
	int		nChaotic;	// ���۳� ���̳ʽ�/ ������ �÷���
	DWORD	dwUseable;	// ��� ĳ����,
	DWORD	dwActionRadius;		// �����ൿ����,
	DWORD	dwAtkMin;	// �ּ�Ÿ��ġ,
	DWORD	dwAtkMax;	// �ִ�Ÿ��ġ,
	DWORD	dwAtk1;
	DWORD	dwAtk2;
	DWORD	dwAtk3;
	DWORD	dwAtk4;		// dwHorizontalRate�� �̰ɷ� �ٲ�.
	FLOAT	fFrame;	// �̵� �� ������ ����ġ
	DWORD	dwOrthograde;
	DWORD	dwThrustRate;	// ������,

	DWORD	dwChestRate;
	DWORD	dwHeadRate;  
	DWORD	dwArmRate;
	DWORD	dwLegRate;

	DWORD	dwAttackSpeed;	// ���ݼӵ�,
	DWORD	dwReAttackDelay;
	DWORD	dwAddHp;		// ,
	DWORD	dwAddMp;		// ,
	DWORD	dwNaturalArmor;	// �ڿ����� 
	int		nAbrasion;	// ����
	int		nHardness;	// �浵
	DWORD	dwAdjAtkDelay;	// �߰����ݼӵ�����,

	SAI79::ePropType	eElementType;
	short				wElementAtk;		// �Ӽ� ������( �� �Ӽ� Ÿ������ ���ݷ��� �����Ѵ�. )

	DWORD	dwHideLevel;	// ���� �Ⱥ��̴³��̳�..
	FLOAT	fSpeed;	// �̵��ӵ�,
	DWORD	dwShelter;	// ��������,
	DWORD	dwFlying;	// ��������,
	DWORD	dwJumpIng;	// ���̶ٱ� 
	DWORD	dwAirJump;	// �ָ��ٱ�
	DWORD	bTaming;	// �������� 
	DWORD	dwResisMgic;	//�������� 

	int		nResistElecricity;
	int		nResistDark;
	int		nResistFire;
	int		nResistWind;
	int		nResistWater;
	int		nResistEarth;
	
	DWORD	dwCash;		// �����ݾ�
	DWORD	dwSourceMaterial;	// �������
	DWORD	dwMaterialAmount;	// ����
	DWORD	dwCohesion;	// ���������
	DWORD	dwHoldingTime;	// ��ü�����ð�
	DWORD	dwCorrectionValue;	// �����ۻ���������
	EXPINTEGER	nExpValue;
	int		nFxpValue;		// �������ġ.
	DWORD	nBodyState;		// ������,
	DWORD	dwAddAbility;	// �߰��ɷ�,
	DWORD	bKillable;	// ��������,

	DWORD	dwVirtItem[3];
	DWORD	bVirtType[3]; 

	DWORD   dwSndAtk1  ;
	DWORD   dwSndAtk2  ;
	DWORD   dwSndDie1  ;
	DWORD   dwSndDie2  ;
	DWORD   dwSndDmg1  ;
	DWORD   dwSndDmg2  ;
	DWORD   dwSndDmg3  ;
	DWORD   dwSndIdle1 ;
	DWORD   dwSndIdle2 ;

	short   m_nEvasionHP;
	short	m_nEvasionSec;
	short   m_nRunawayHP          ; // HP�� 10 ���ϸ� ���� 
	short   m_nCallHelperMax       ; // �� ���� 
	short   m_nCallHP              ; // �����û�ϱ� ���� HP
	short   m_nCallHelperIdx  [ 5 ]; // �����û�ϱ� Id
	short   m_nCallHelperNum  [ 5 ]; // �����û�ϱ� Id
	short   m_bCallHelperParty[ 5 ]; // �����û�ϱ� Id

	short   m_dwAttackMoveDelay;
	short   m_dwRunawayDelay;
	short   m_bPartyAttack;
	short   m_nHelperNum;

#if !defined(__CORESERVER) 
	int		m_nScanJob;		// Ÿ���� �˻��Ҷ� Ư�� �������� �˻��ϴ°�. 0 �̸� ALL
	// �������ǵ��� ��Ʈ���� �������� �����Ǿ� ������ ����.
	int		m_nHPCond;		// Ÿ���� hp% �����϶� ������ ��.
	int		m_nLvCond;		// Ÿ�ٰ� �������Ͽ� ������ ������ ��.
	int		m_nRecvCondMe;	// ȸ�� ����.  ��hp�� ��%���Ϸ� ����������?
	int		m_nRecvCondHow;	// ȸ���Ҷ� ȸ���� �� %����
	int		m_nRecvCondMP;	// ȸ���Ҷ� MP�Ҹ� %����
	BYTE	m_bMeleeAttack;	// ���� ���� AI�� �ִ°�?
	BYTE	m_bRecvCondWho;	// ������ ġ���Ҳ���. 0:�����ȵ� 1:�ٸ��� 2:�� 3:���.
	BYTE	m_bRecvCond;	// ġ���ϳ�? 0:ġ����� 1:�����߿��� ġ���� 2:����/������ ��� ġ��
	BYTE	m_bHelpWho;		// �����û�� - 0:�θ������� 1:�ƹ���  2:����������.
	BYTE	m_bRangeAttack[ MAX_JOB ];		// �� ������ ���Ÿ� ���� �Ÿ�.
	int		m_nSummProb;	// ��ȯ Ȯ�� : 0�̸� ��ȯ�ɷ� ����.
	int		m_nSummNum;		// �ѹ��� ����� ��ȯ�ϳ�.
	int		m_nSummID;		// � ����?
	int		m_nHelpRangeMul;	// �����û �Ÿ�. �þ��� ���
	DWORD	m_tmUnitHelp;			// ���� Ÿ�̸�.
	int		m_nBerserkHP;		// ����Ŀ�� �Ǳ� ���� HP%
	float	m_fBerserkDmgMul;	// ����Ŀ�� �Ǿ����� ������ ���.
	int		m_nLoot;			// ���ø��ΰ�.
	int		m_nLootProb;		// ���� Ȯ��
	DWORD   m_dwScanQuestId; 
	DWORD   m_dwScanItemIdx; 
	int		m_nScanChao;		// ī��, ��ī�� �˻�
#endif // !__CORESERVER
	
#ifndef __CORESERVER
	float	m_fHitPoint_Rate;		// ���� �ִ� HP�� // dwAddHp * m_nHitPoint_Rate
	float	m_fAttackPower_Rate;	// ���� �ִ� ���ݷ� // dwAtkMin * m_nAttackPower_Rate
	float	m_fDefence_Rate;		// ���� �ִ� ���� // dwAddHp * m_nDefence_Rate
	float	m_fExp_Rate;			// ���� �ִ� ����ġ�� // dwAddHp * m_nExp_Rate
	float	m_fItemDrop_Rate;		// ���� �ִ� ������ ��ӷ� // dwAddHp * m_nItemDrop_Rate
	float	m_fPenya_Rate;			// ���� �ִ� ��ķ� // dwAddHp * m_nPenya_Rate
	BOOL	m_bRate;
#endif

	
	short	m_nAttackItemNear;
	short	m_nAttackItemFar;
	short	m_nAttackItem1;
	short	m_nAttackItem2;
	short	m_nAttackItem3;
	short	m_nAttackItem4;
	short	m_nAttackItemSec;
	short	m_nMagicReflection;
	short	m_nImmortality;
	BOOL 	m_bBlow;
	short	m_nChangeTargetRand;

	short   m_nAttackFirstRange;
	CDropItemGenerator	m_DropItemGenerator;
	CQuestItemGenerator		m_QuestItemGenerator;
	CDropKindGenerator	m_DropKindGenerator;
	MonsterTransform	m_MonsterTransform;
	
	MoverProp()
	{
		dwStr	= dwSta	= dwDex	= dwInt	= dwHR	= dwER	= dwRace	= dwBelligerence	=dwGender
		= dwLevel	= dwFlightLevel	= dwSize	= dwClass	= bIfParts	= nChaotic
		= dwUseable
		= dwActionRadius	= dwAtkMin	= dwAtkMax	= dwAtk1	= dwAtk2	= dwAtk3	= dwAtk4	= 0;
			fFrame	= 1.0F;
			dwOrthograde	= 0;
		dwThrustRate		= 0;
		dwChestRate			= 0;
		dwHeadRate			= 0;
		dwArmRate			= 0;
		dwLegRate			= 0;
		dwAttackSpeed		= 0;
		dwReAttackDelay		= 0;
		dwAddHp				= 0;
		dwAddMp				= 0;
		dwNaturalArmor		= 0;
		nAbrasion			= 0;
		nHardness			= 0;
		dwAdjAtkDelay		= 0;
		dwHideLevel			= 0;
		eElementType		= SAI79::NO_PROP;
		wElementAtk			= 0;
		fSpeed				= 0.0F;
		dwShelter			= 0;
		dwFlying			= 0;
		dwJumpIng			= 0;
		dwAirJump			= 0;
		bTaming				= 0;
		dwResisMgic			= 0;
		nResistElecricity	= 0;
		nResistDark			= 0;
		nResistFire			= 0;
		nResistWind			= 0;
		nResistWater		= 0;
		nResistEarth		= 0;
		dwCash				= 0;
		dwSourceMaterial	= 0;
		dwMaterialAmount	= 0;
		dwCohesion			= 0;
		dwHoldingTime		= 0;
		dwCorrectionValue	= 0;
		nExpValue			= 0;
		nFxpValue			= 0;
		nBodyState			= 0;
		dwAddAbility		= 0;
		bKillable			= 0;
		
		memset( dwVirtItem, 0, sizeof(dwVirtItem) );
		memset( bVirtType, 0, sizeof(bVirtType) );

		dwSndAtk1			= 0;
		dwSndAtk2			= 0;
		dwSndDie1			= 0;
		dwSndDie2			= 0;
		dwSndDmg1			= 0;
		dwSndDmg2			= 0;
		dwSndDmg3			= 0;
		dwSndIdle1			= 0;
		dwSndIdle2			= 0;
		m_nEvasionHP		= 0;
		m_nEvasionSec		= 0;
		m_nRunawayHP		= 0;
		m_nCallHelperMax	= 0;
		m_nCallHP			= 0;
		
		memset( m_nCallHelperIdx, 0, sizeof(m_nCallHelperIdx) );
		memset( m_nCallHelperNum, 0, sizeof(m_nCallHelperNum) );
		memset( m_bCallHelperParty, 0, sizeof(m_bCallHelperParty) );
		
		m_dwAttackMoveDelay	= 0;
		m_dwRunawayDelay	= 0;
		m_bPartyAttack		= 0;
		m_nHelperNum		= 0;

#if !defined(__CORESERVER) 
		m_nScanJob			= 0;
		m_nHPCond			= 0;
		m_nLvCond			= 0;
		m_nRecvCondMe		= 0;
		m_nRecvCondHow		= 0;
		m_nRecvCondMP		= 0;
		m_bMeleeAttack		= 0;
		m_bRecvCondWho		= 0;
		m_bRecvCond			= 0;
		m_bHelpWho			= 0;
		memset( m_bRangeAttack, 0, sizeof(m_bRangeAttack) );
		m_nSummProb			= 0;
		m_nSummNum			= 0;
		m_nSummID			= 0;
		m_nHelpRangeMul		= 0;
		m_tmUnitHelp		= 0;
		m_nBerserkHP		= 0;
		m_fBerserkDmgMul	= 0;
		m_nLoot				= 0;
		m_nLootProb			= 0;
		m_dwScanQuestId		= 0;
		m_dwScanItemIdx		= 0;
		m_nScanChao			= 0;
#endif // !__CORESERVER

#ifndef __CORESERVER
		m_fHitPoint_Rate	=
		m_fAttackPower_Rate	= 
		m_fDefence_Rate	=
		m_fExp_Rate	= 
		m_fItemDrop_Rate	= 
		m_fPenya_Rate	= 0.0F;
		m_bRate	=
#endif
		m_nAttackItemNear	=
		m_nAttackItemFar	=
		m_nAttackItem1	=
		m_nAttackItem2	=
		m_nAttackItem3	=
		m_nAttackItem4	=
		m_nAttackItemSec	=
		m_nMagicReflection	=
		m_nImmortality	=
		m_bBlow	=
		m_nChangeTargetRand	=
		m_nAttackFirstRange	= 0;
	}
};

#ifdef __WORLDSERVER
enum class SpawnType : bool { Region, Script };
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
	DWORD     m_dwUserId = 0;
	DWORD     m_dwData = NULL;
	TCHAR     m_szString[MAX_SHORTCUT_STRING] = ""; // SHORTCUT_CHAT�� ��� ����.
	BOOL IsEmpty() const { return m_dwShortcut == ShortcutType::None; }
	void Empty() { m_dwShortcut = ShortcutType::None; }

	friend CAr & operator<<(CAr & ar, const SHORTCUT & self);
	friend CAr & operator>>(CAr & ar, SHORTCUT & self);
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


#ifdef __RULE_0615

class CNameValider {
	// Note: CNameValider logic is incompatible with wide char encoding.
	
	// We prefer std::array<bool, 256> instead of std::bitset<256>
	// https://stackoverflow.com/a/58476584
	// Some bit trickery might be possible though

private:
	std::vector<std::string> m_invalidNames;
	std::array<bool, 256> m_allowedLetters{ false, };
	std::array<bool, 256> m_allowedLettersForVendor{ false, };
	static_assert(sizeof(char) == 1, "char should be on only one byte so its value goes from 0 included to 256 excluded");

public:
	bool Load();

	static void Formalize(LPSTR szName);
	[[nodiscard]] bool IsNotAllowedName(LPCSTR name) const;
	[[nodiscard]] bool IsNotAllowedVendorName(LPCSTR name) const;
	[[nodiscard]] bool IsAllowedLetter(LPCSTR name) const { return AllLettersAreIn(name, m_allowedLetters); }

private:
	[[nodiscard]] bool IsInvalidName(LPCSTR name) const;
	[[nodiscard]] static bool AllLettersAreIn(LPCSTR name, const std::array<bool, 256> & allowed);
};
#endif	// __RULE_0615

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


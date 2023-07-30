#pragma once

#ifndef __CLIENT
static_assert(false, "Sfx.h may only be included in client")
#endif

#include "light.h"
class CSfxGenMoveMark;
extern CSfxGenMoveMark* g_pMoveMark;

#include "mempooler.h"

class CSfxModel;

/// ȿ��(��ƼŬ, �߻�ü)�� ó���ϴ� base class
class CSfx  : public CCtrl
{
protected:
	CSfxModel m_SfxObj2; // ���� ȿ�� : ����ϴ��� ������  
	CSfxModel m_SfxObj3; // ���� ȿ�� : ����ϴ��� ������ 

	DWORD	m_dwSkill;		// ��ų�߻�ü�� ��ų ���̵�..
	
public:
	int m_nSec;
	int m_nFrame;

	CSfxModel* m_pSfxObj; // ����Ʈ �� ������
	OBJID       m_idSrc;    // ��� ID
	OBJID       m_idDest;   // ������ ID
	D3DXVECTOR3 m_vPosDest; // ���� ��ġ

	CLight  m_light;
	int		m_idSfxHit;
	int		m_nMagicPower;	// ���� �ܰ�

	CSfx();
	CSfx( int nIndex, const OBJID idSrc, const D3DXVECTOR3& vPosSrc, const OBJID idDest, const D3DXVECTOR3& vPosDest );
	virtual ~CSfx();

	void	SetSkill( DWORD dwSkill )
	{
		m_dwSkill = dwSkill;
	}
	DWORD	GetSkill()
	{
		return m_dwSkill;
	}
	
	void Process(); // �� �����Ӹ��� ��ġ�̵�, �ִϸ��̼� �� ó��
	void DamageToTarget( int nMaxDmgCnt = 1 ); // ��ǥ���� �¾����� �������� �ش�
	BOOL SetIndex( DWORD dwIndex, BOOL bInitProp = FALSE );

	virtual	int SetSfx( int nIndex, D3DXVECTOR3 vPosSrc, OBJID idSrc, D3DXVECTOR3 vPosDest, OBJID idDest, int nSec = 0 ); // ����� SFX ����
	virtual void ShootSfx( float fAngXZ, float fAngY, float fSpeed ) {}
	virtual void SetPartLink( int nPart ) {}
	virtual void Render(); // ����

	/* Remove the LPDIRECT3DDEVICE9 parameter */
	virtual void Render(LPDIRECT3DDEVICE9) final = delete;
};


inline CSfx * CObj::ToSfx() {
	if (GetType() != OT_SFX) return nullptr;
	return static_cast<CSfx *>(this);
}

inline const CSfx * CObj::ToSfx() const {
	if (GetType() != OT_SFX) return nullptr;
	return static_cast<const CSfx *>(this);
}

// ��� ���ư��� �°� ������ �Ϲ����� ����Ʈ
class CSfxShoot : public CSfx
{
public:
	BOOL	m_bHit; // �����ߴ����� ����
	BOOL	m_bDir;
	DWORD	m_dwSndHit;		// �¾����� �Ҹ�.
	DWORD	m_dwSfxHit;		// ���� ����Ʈ
	FLOAT	m_fHitScale;	// ���߾� �����ϸ�.
	
	CSfxShoot();
	
	void	SetHitSfx( LPCTSTR szFileName, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetHitSfx( DWORD dwIndex, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetDir( BOOL bDir );		// ���⼺�ִ� sfx �ΰ�.
	void	SetSndHit( DWORD dwSndHit ) { m_dwSndHit = dwSndHit; }
	
	void Process() override;
	void Render() override;
};


// CSfx�� �Ȱ��� idSrc�� ȸ��������� �Ȱ�����.
class CSfxRotate : public CSfx
{
public:
	void Process() override;
	void Render() override;
};

class CSfxDuelParty : public CSfx
{
public:
	int	m_nType = 0; // 0:���  1:����.

	void DeleteSfx(CMover * pEnemy);

	void Process() override;
	void Render() override;
};

class CSfxSetItem : public CSfx
{
public:
	~CSfxSetItem();
	void Process() override;
	void Render() override;
};


class CSfxCollect : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// Ŭ����ũ ���� ������ ����.
class CSfxClockWorksCharge : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// Ŭ����ũ ���� ���ȿ��� �߻��Ҷ� ������ ����Ʈ.
class CSfxClockWorksCannon : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// �ӽ���Ʈ ���� ������ ����.
class CSfxMushmootCharge : public CSfx {
public:
	int	m_nEventPos;
	void Process() override;
	void Render() override;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �Ϲ� ����Ʈ : ��� ������
class CSfxGenNormalDmg01 : public CSfx
{
public:
	void Process() override;
	void Render() override;
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CSfxGenNormalDmg01>* m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenNormalDmg01::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenNormalDmg01::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenNormalDmg01::m_pPool->Free( (CSfxGenNormalDmg01*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenNormalDmg01::m_pPool->Free( (CSfxGenNormalDmg01*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

// �شܽ�ų ����Ʈ : ��Ʈ��Ī01
class CSfxTroStretching01 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �شܽ�ų ����Ʈ : ��Ʈ��Ī02
class CSfxTroStretching02 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �شܽ�ų ����Ʈ : ���߰���
class CSfxTroBlitz : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// �Ϲ� ����Ʈ : ���� �Ⱦ�
class CSfxGenSuperDmg01 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ���� �Ⱦ�
class CSfxGenSkillDmg01 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ���� �Ⱦ�
class CSfxGenMonsterDmg01 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ��Ȱ
class CSfxGenRestoration01 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ġ��
class CSfxGenCure : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ��ȭ
class CSfxGenIncrease01 : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ������
class CSfxGenLevelUp : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : �α���
class CSfxGenLogin : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ����
class CSfxGenWarp : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : ���
class CSfxGenPcDie : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : �� ������
class CSfxGenMonsterSpawn : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : �̵���ġ
class CSfxGenMoveMark: public CSfx
{
public:
	void Process() override;
	void Render() override;

public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxGenMoveMark>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenMoveMark::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenMoveMark::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenMoveMark::m_pPool->Free( (CSfxGenMoveMark*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenMoveMark::m_pPool->Free( (CSfxGenMoveMark*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};
// �Ϲ� ����Ʈ : ���� ����
class CSfxGenWaterCircle: public CSfx
{
public:
	void Process() override;
	void Render() override;

public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxGenWaterCircle>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenWaterCircle::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenWaterCircle::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenWaterCircle::m_pPool->Free( (CSfxGenWaterCircle*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenWaterCircle::m_pPool->Free( (CSfxGenWaterCircle*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

// �Ϲ� ����Ʈ : ���� ����
class CSfxGenRainCircle: public CSfx
{
public:
	void Process() override;
	void Render() override;

public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxGenRainCircle>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenRainCircle::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenRainCircle::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenRainCircle::m_pPool->Free( (CSfxGenRainCircle*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenRainCircle::m_pPool->Free( (CSfxGenRainCircle*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

// �Ϲ� ����Ʈ : �� Ƣ���
class CSfxGenWaterCrown : public CSfx {
public:
	void Process() override;
	void Render() override;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CSfxMagicMiAtk1 : public CSfxShoot
{
public:
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.
	
	CSfxMagicMiAtk1();
	void Process() override;
	void Render() override;
};


// �ϵ�� ������ ���� �ܰ躰�� 4����
// �Ϲ� ����Ʈ : �ϵ�1
class CSfxItemWandAtk1 : public CSfxShoot
{
public:
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.

	CSfxItemWandAtk1();
	void Process() override;
	void Render() override;
};

class CSfxItemRangeAtk1 : public CSfxShoot
{
public:
	CTailEffectBelt*	m_pTail = nullptr;		// ������ ����Ʈ	
	FLOAT			m_fOriLen;
	D3DXVECTOR3		m_v3SrcPos;
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.

	virtual ~CSfxItemRangeAtk1();
	void Process() override;
	void Render() override;
};

// �Ʒ��� �������� ȭ�� ����Ʈ
class CSfxItemRangeAtk1_Allow : public CSfxShoot
{
public:
	FLOAT	m_fSpeed;
	CSfxItemRangeAtk1_Allow();
	void Process() override;
	void Render() override;
};

// CSfxItemRangeAtk1_Allow���� ���� - ���� ���Ǿ����� �κ�
class CSfxItemRangeAtk1_AllowRain : public CSfxShoot
{
public:
	int		m_nCount;
	CSfxItemRangeAtk1_AllowRain();
	void Process() override;
	void Render() override;
};

// CSfxItemRangeAtk1_Allow���� ���� - ���� ���Ǿ����� �κ�
class CSfxItemRangeAtk1_Stone : public CSfxShoot
{
public:
	FLOAT	m_fSpeed;
	CSfxItemRangeAtk1_Stone();
	void Process() override;
	void Render() override;
};


class CSfxItemRangeAtk1_StoneRain : public CSfxShoot
{
public:
	int		m_nCount;
	CSfxItemRangeAtk1_StoneRain();
	void Process() override;
	void Render() override;
};

class CSfxItemRangeAtk_JunkBow : public CSfxShoot
{
public:
	CTailEffectBelt*	m_pTail[3];		// ������ ����Ʈ	
	FLOAT			m_fOriLen;
	D3DXVECTOR3		m_v3SrcPos;

	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.
	CSfxItemRangeAtk_JunkBow();
	virtual ~CSfxItemRangeAtk_JunkBow();
	void Process() override;
	void Render() override;
};

D3DXVECTOR3	SplineSlerp(D3DXVECTOR3 * v1, D3DXVECTOR3 * v2, D3DXVECTOR3 * v3, D3DXVECTOR3 * v4, float fSlerp);

class CSfxItemYoyoAtk : public CSfxShoot
{
public:
	int				m_nMaxSpline;
	CTailEffectModel*	m_pTail;		// ������ ����Ʈ	
	D3DXVECTOR3		m_aSpline[30];
	int				m_nStep;
	int				m_nType;
	int				m_nCount;
	
	FLOAT			m_fMaxLength;
	D3DXVECTOR3		m_v3SrcPos;
	D3DXVECTOR3		m_vDelta;
	int				m_nDelayCount;
	
	

	
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.

	void  MakePath(int nType);
	
	CSfxItemYoyoAtk();
	virtual ~CSfxItemYoyoAtk();
	void Process() override;
	void Render() override;
};


// ���������� �߻��ϴ� ��Ÿ� źȯ��.
class CSfxAtkStraight : public CSfxShoot
{
	D3DXVECTOR3 m_vDelta;			// �̵� ������.
	DWORD		m_dwExplosion;		// �°��� ������ ���� ���̵�.
public:
	
	CSfxAtkStraight();
	void ShootSfx( float fAngXZ, float fAngH, float fSpeed, DWORD dwExplosion );		// �߻�ü�� �̵��������� �������. 
	
	void Process() override;
	void Render() override;
};

class CSfxItemWandAtkAir : public CSfxShoot
{
	D3DXVECTOR3		m_vDelta;		// ����.
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtkAir();
	int SetSfx( int nIndex, D3DXVECTOR3 vPosSrc, OBJID idSrc, D3DXVECTOR3 vPosDest, OBJID idDest, int nSec = 0 ) override; // ����� SFX ����
	void ShootSfx( float fAngXZ, float fAngY, float fSpeed ) override;

	void Process() override;
	void Render() override;
};


// �Ϲ� ����Ʈ : �ϵ�2
class CSfxItemWandAtk2 : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtk2();

	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : �ϵ�3
class CSfxItemWandAtk3 : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtk3();
	void Process() override;
	void Render() override;
};
// �Ϲ� ����Ʈ : �ϵ�4
class CSfxItemWandAtk4 : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtk4();
	void Process() override;
	void Render() override;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����� ��ų : ����Ŀ��
class CSfxSkillVagOverCutter : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// ����� ��ų : Ŭ����Ʈ
class CSfxSkillVagCleanHit : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// ����� ��ų : �귣��
class CSfxSkillVagBrandish : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// �Ӽųʸ� ��ų : Ų��
class CSfxSkillMerKeenWheel : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// �Ӽųʸ� ��ų : ���øŽ�
class CSfxSkillMerSplmash : public CSfx
{
public:
	CSfxSkillMerSplmash();

	void Process() override;
	void Render() override;
};
// �Ӽųʸ� ��ų : ����ε� ���̵�
class CSfxSkillMerBlindSide : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// ������ �ֹ��ܱ�
class CSfxSkillMagFireCasting : public CSfx {
public:
	void Process() override;
	void Render() override;
};
class CSfxSkillMagWindCasting : public CSfx {
public:
	void Process() override;
	void Render() override;
};

// ��� ��Ŭ�߰�..

class CSfxSkillAssBurstcrack : public CSfx {
public:
	void Process() override;
	void Render() override;
};

class CSfxSkillAssTampinghole : public CSfx {
public:
	void Process() override;
	void Render() override;
};

class CSfxNpcDirSteam : public CSfx
{
public:
	BOOL m_fInit = TRUE;

	void Process() override;
	void Render() override;
};

	

// ������ ��ų : ��Ʈ������
class CSfxSkillMagStrongWind : public CSfxShoot
{
	float m_fAngle;	
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagStrongWind();

	void Process() override;
	void Render() override;
};
// ������ ��ų : �ҵ�����
class CSfxSkillMagSwordWind : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagSwordWind();
	void Process() override;
	void Render() override;
};
// ������ ��ų : ���̾�θ޶�
class CSfxSkillMagFireBoomerang : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagFireBoomerang();
	void Process() override;
	void Render() override;
};
// ������ ��ų : ���̾��
class CSfxSkillMagFireBomb : public CSfx {
public:
	void Process() override;
	void Render() override;
};
// ������ ��ų : �ֿ���
class CSfxSkillMagHotAir : public CSfxShoot
{
	int m_nDmgCnt;		// ���ӵ������� ������ ī��Ʈ. ������ ���϶����� �ϳ��� �ö󰣴�.
public:
	
	CSfxSkillMagHotAir();
	void Process() override;
	void Render() override;
};



// ������ ��ų : ���̽��̻���
class CSfxSkillMagIceMissile : public CSfxShoot
{
public:
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagIceMissile();
	void Process() override;
	void Render() override;
};

// ������ ��ų : ����Ʈ�� ��
class CSfxSkillMagLightningBall : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagLightningBall();
	void Process() override;
	void Render() override;
};


// ������ ��ų : ������ũ ����
class CSfxSkillMagSpikeStone : public CSfxShoot
{
public:
	
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagSpikeStone();
	void Process() override;
	void Render() override;
};


#define MAX_SHOOTWAVE_TAIL		8
// �ٴڿ� �پ ���ư��� ����Ʈ.
class CSfxShootWave : public CSfxShoot
{
public:
	BOOL	m_bHit; // �����ߴ����� ����
	BOOL	m_bDir;
	DWORD	m_dwSndHit;		// �¾����� �Ҹ�.
	FLOAT	m_fHitScale;	// ���߾� �����ϸ�.
	D3DXVECTOR3 m_vTail[MAX_SHOOTWAVE_TAIL];	// �������� ��ǥ�� ���ʷ� ����.
	int		m_nTailFrame[ MAX_SHOOTWAVE_TAIL ];	// ������ ��ȣ.

	CSfxShootWave();
	
	void	SetHitSfx( LPCTSTR szFileName, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetHitSfx( DWORD dwIndex, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetDir( BOOL bDir );		// ���⼺�ִ� sfx �ΰ�.
	void	SetSndHit( DWORD dwSndHit ) { m_dwSndHit = dwSndHit; }
	
	void Process() override;
	void Render() override;
};


// ������ǥ ���� ���� sfx
class CSfxFixed : public CSfx
{
	float m_fAngle;
public:
	void Process() override;
	void Render() override;
};

class CSfxPartsLink : public CSfx	// ��������Ǵ� ����Ʈ		
{
public:
	BOOL	m_bEndFrame = FALSE;
	int		m_nPartsLink = 0;		// ��ũ�� ��ġ. 0:������ 1:�޼�
	
	void Process() override;
	void Render() override;
	void SetPartLink(int nPart) override { m_nPartsLink = nPart; }
};

// ������ ��ũ�Ǵ� sfx(���̵��� �տ� ���)
class CSfxPartsLinkBlade : public CSfxPartsLink		// ���̵� ��Ʈ ��ũ ���� : �и� ����� ������ �Ҹ�
{
public:
	void Process() override;
};

class CSfxPartsLinkJst : public CSfxPartsLink		// ������ ���� ��ũ ���� : ����Ʈ�� EndFrame�̸� �Ҹ�
{
public:
	void Process() override;
};



// ������ ��ũ�Ǵ� sfx(��ũ�ι��� �տ� ���)
class CSfxAllowPartsLink : public CSfx
{
public:
	int		m_nPartsLink = 0;		// ��ũ�� ��ġ. 0:������ 1:�޼�
	
	void Process() override;
	void Render() override;
	void SetPartLink(int nPart) override { m_nPartsLink = nPart; }
};

class CSfxPartsLinkShoulder : public CSfxPartsLink		// ���̵� ��Ʈ ��ũ ���� : �и� ����� ������ �Ҹ�
{
public:
	int				m_nOldAbilityOption;
	D3DXVECTOR3		m_v3Offset;

	CSfxPartsLinkShoulder();
	~CSfxPartsLinkShoulder();

	void Process() override;
	void Render() override;
};

// ������ ���� ����Ʈ
class CSfxReady : public CSfx
{
public:
	void Process() override;
};

class CSfxCursor : public CSfx
{
public:
	void Process() override;
};

class CSfxLinkMover : public CSfx			
{
public:
	//gmpbigsun : ������ �پ� �ٴϴ� �Ϲ����� ��Ȳ�����
	void Process() override;
	void Render() override;
};



class CSfxHitParts : public CSfxPartsLink {
public:
	enum class HIT_PARTS {
		HIT_BODY,
		HIT_OVERHEAD,
	};
	//gmpbigsun : For the general situation of being attached to the mover
	CSfxHitParts(const HIT_PARTS eParts = HIT_PARTS::HIT_BODY);
	void Process() override;
	void Render() override;

protected:
	HIT_PARTS m_eHitParts;
	BOOL m_bBuff;
};

class CSfxAttackParts : public CSfxPartsLink {
public:
	enum class HIT_PARTS {
		HIT_BODY,
		HIT_OVERHEAD,
		HIT_RHAND,
	};
	//gmpbigsun : For the general situation of being attached to the mover
	CSfxAttackParts(const HIT_PARTS eParts = HIT_PARTS::HIT_BODY);
	void Process() override;
	void Render() override;

	HIT_PARTS m_eHitParts;
};

// It's the same as CSfx, and the rotation direction of idSrc is the same.
class CSfxRotate_New : public CSfx {
public:
	void Process() override;
	void Render() override;
};

#pragma once

#include <d3dx9.h>
#include "file.h"
#include <functional>

#define		MAX_VS_BONE		28		// The maximum number of skeleton registers available in VS.

struct BONE {
	BONE *			m_pParent;    // Parent node

	int					m_nParentIdx; // Parent index
	char				m_szName[32]; // Bode name
	D3DXMATRIX	m_mTM;        // original TM
	D3DXMATRIX	m_mInverseTM; // pre-converted inverseTM
	D3DXMATRIX	m_mLocalTM;   // pretransformed local TM
};

#define MAX_MDL_EVENT		8	// #event xx

//
// ����Ʈ�� ����
class CBones final
{
public:
	int		m_nID;				// ���� ���� ID(���ϳ��� �ִ�)
	char	m_szName[64];		// ���� ���ϸ� - ���߿� �̰� ������.
	int		m_nMaxBone;			// ���� ����
	std::unique_ptr<BONE[]> m_pBones;			// ���� ����Ʈ
	int		m_bSendVS;			// �������� MAX_VS_BONE���� ������...

	D3DXMATRIX	m_mLocalRH;		// ������ ������ RHand�κ����� ���TM
	D3DXMATRIX	m_mLocalLH;		// ������ ������ LHand�κ����� ���TM
	D3DXMATRIX	m_mLocalShield;	// ������ LForeArm���κ����� ���TM
	D3DXMATRIX	m_mLocalKnuckle;	// ��Ŭ.
	int		m_nRHandIdx;		// ������ �ε���
	int		m_nLHandIdx;		// �޼� �ε���
	int		m_nRArmIdx;			// ������ �ȶ�
	int		m_nLArmIdx;			// �޼� �ȶ� �ε���.
	D3DXVECTOR3	m_vEvent[MAX_MDL_EVENT];	
	int		m_nEventParentIdx[MAX_MDL_EVENT];	// �̺�Ʈ��ǥ�� �θ��ε���.
	
public:
	CBones();

	BONE * GetBone(int nIdx) { return &m_pBones[nIdx]; }

	int		LoadBone( LPCTSTR szFileName );		// ������ �� ������ ����
};



// read only bone database
class CBonesMng final {
private:
	std::map<std::string, std::unique_ptr<CBones>, std::less<>> m_mapBones;

public:
	// Bone file is read and loaded into memory.
	CBones * LoadBone(LPCTSTR szFileName);
};

extern CBonesMng		g_BonesMng;


//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define		MA_HIT			(0x00000001)		// Ÿ��
#define		MA_SOUND		(0x00000002)		// �Ҹ�
#define		MA_QUAKE		(0x00000004)		// ����.
struct TM_ANIMATION
{
//	D3DXMATRIX			m_mAniTM;
	D3DXQUATERNION		m_qRot;
	D3DXVECTOR3			m_vPos;
};

// ����� ���� ������ ���谡 �ִ�.  ���� ũ�Ⱑ ����, ���� ������ ���� ����� �ƴϸ� ������ �� �����ִ�.
// ĳ���� �����ϳ��� ���� Ŭ����
struct	BONE_FRAME
{
	TM_ANIMATION	*m_pFrame;
	D3DXMATRIX		m_mLocalTM;
};

// �� �������� �Ӽ���
struct	MOTION_ATTR
{
	DWORD	m_dwAttr;
//	char	m_szFileName[32];
	int		m_nSndID;		// ���� ���ҽ� ���̵�
	float	m_fFrame;		// �Ҽ��� ������ ��Ȯ�� ������ ��ȣ 
};

class CMotion
{
private:
	TM_ANIMATION	*m_pMotion;			// ���������� ������ �ִϸ��̼��� ����ִ�.  �ִϰ� ���� ����� LocalTM�� �ִ�.
	MOTION_ATTR		*m_pAttr;			// ������ũ�⸸ŭ �Ҵ�.  �� �����ӼӼ�.
	int		m_nMaxFrame;				// ��� �����Ӽ�
public:
	int		m_nID;						// ����� ���� ID(���ϳ��� �ִ�)
	char	m_szName[64];				// ��� ���ϸ� - ���߿� �̰� ������.
	float	m_fPerSlerp;				//
	int		m_nMaxBone;					// �̰��� �������� Ȯ�ο� �����ϻ��̴�.  ���� ���� ������ �ʿ��ϴٸ� CBone���� �о�� �Ѵ�.
	D3DXVECTOR3	m_vPivot;				// 0,0,0(��)���ؿ��� ���(��Ʈ)�� ��ǥ.  0��Ű�� ��ǥ�� ����.
	BONE_FRAME	*m_pBoneFrame;		// ���밹�� ��ŭ �Ҵ�. �ε��� ������ ����ϱ� ������ �����͸� ���ġ�Ѵ�.  �������� ���°��� NULL
	BONE		*m_pBoneInfo;		// ���뱸������
	int		m_nMaxEvent;
	D3DXVECTOR3	m_vEvent[4];
	D3DXVECTOR3		*m_pPath;			// path
	
	CMotion();
	~CMotion();

	void Init( void );
	void Destroy( void );

	MOTION_ATTR *GetMotionAttr( int nNumFrm ) { return m_pAttr + nNumFrm; }	
	DWORD	GetAttr( int nNumFrm ) 
	{ 
		return m_pAttr[ nNumFrm ].m_dwAttr; 
	}
//#if 0
	DWORD	IsAttrHit( float fOldFrm, float fNumFrm ) 
	{ 
		MOTION_ATTR		*pAttr = &m_pAttr[ (int)fNumFrm ];
		if( pAttr->m_dwAttr & MA_HIT )
			if( fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm )	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr->m_dwAttr;
			return 0;
	}
	MOTION_ATTR *IsAttrSound( float fOldFrm, float fNumFrm )
	{ 
		MOTION_ATTR		*pAttr = &m_pAttr[ (int)fNumFrm ];
		if( pAttr->m_dwAttr & MA_SOUND )
			if( fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm )	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr;
			return NULL;
	}
	DWORD	IsAttrQuake( float fOldFrm, float fNumFrm ) 
	{ 
		MOTION_ATTR		*pAttr = &m_pAttr[ (int)fNumFrm ];
		if( pAttr->m_dwAttr & MA_QUAKE )
			if( fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm )	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr->m_dwAttr;
			return 0;
	}
	void	SetAttr( float fNumFrm, DWORD dwAttr ) 
	{
		m_pAttr[ (int)fNumFrm ].m_dwAttr |= dwAttr;
		m_pAttr[ (int)fNumFrm ].m_fFrame = fNumFrm;
	}
	void	ResetAttr( int nNumFrm, DWORD dwAttr ) { m_pAttr[ nNumFrm ].m_dwAttr &= (~dwAttr); }
	TM_ANIMATION*	GetFrame( int nNumFrm ) { return &m_pMotion[nNumFrm]; }
	int		GetMaxFrame( void ) { return m_nMaxFrame; }

	int		LoadMotion( LPCTSTR strFileName );			// ���Ͽ��� ��������� ����
	void	ReadTM( CResFile *file, int nNumBone, int nNumFrame );

	void	AnimateBone( D3DXMATRIX *pmUpdateBone, CMotion *pMotionOld, float fFrameCurrent, int nNextFrame, int nFrameOld, BOOL bMotionTrans, float fBlendWeight );
	
};


// read only motion database
class CMotionMng final {
private:
	std::map<std::string, std::unique_ptr<CMotion>, std::less<>> m_mapMotions;

public:
	// Load the motion into memory, or return the already existing motion if
	// already loaded.
	CMotion * LoadMotion(LPCTSTR strFileName);
};

extern CMotionMng		g_MotionMng;

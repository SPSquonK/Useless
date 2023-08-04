#pragma once

#include <d3dx9.h>
#include "file.h"
#include <functional>
#include "sqktd/dynamic_array.hpp"

struct BONE {
	BONE *			m_pParent;    // Parent node

	int					m_nParentIdx; // Parent index
	char				m_szName[32]; // Bode name
	D3DXMATRIX	m_mTM;        // original TM
	D3DXMATRIX	m_mInverseTM; // pre-converted inverseTM
	D3DXMATRIX	m_mLocalTM;   // pretransformed local TM
};



//
// ����Ʈ�� ����
class CBones final
{
public:
	static constexpr size_t MAX_VS_BONE   = 28; // The maximum number of skeleton registers available in VS.
	static constexpr int    MAX_MDL_EVENT = 8;  // #event xx

	int		m_nID;                            // Unique ID of the bone (located in the file)
	char	m_szName[64];                     // The name of the model's file - this will be removed later.
	sqktd::dynamic_array<BONE> m_pBones;
	int		m_bSendVS;                        // Whether the number of bones is less than MAX_VS_BONE...

	D3DXMATRIX	m_mLocalRH;                 // Relative transformation matrice from RHand of weapon
	D3DXMATRIX	m_mLocalLH;                 // Relative transformation matrice from LHand of weapon
	D3DXMATRIX	m_mLocalShield;             // Relative transformation matrice from left forarm of shields
	D3DXMATRIX	m_mLocalKnuckle;            // Relative transformation matrice from RHand of knuckles
	int		m_nRHandIdx;                      // Index of the right hand
	int		m_nLHandIdx;                      // Index of the left hand
	int		m_nRArmIdx;                       // Index of the right forarm
	int		m_nLArmIdx;                       // Index of the left forarm
	D3DXVECTOR3	m_vEvent[MAX_MDL_EVENT];	
	int		m_nEventParentIdx[MAX_MDL_EVENT];	// Parent indices of event coordinates.
	
public:
	CBones();

	[[nodiscard]] const BONE * GetBone(int nIdx) const { return &m_pBones[nIdx]; }

	int		LoadBone( LPCTSTR szFileName );		// ������ �� ������ ����
};



// read only bone database
class CBonesMng final {
private:
	std::map<std::string, std::unique_ptr<CBones>, std::less<>> m_mapBones;

public:
	// Bone file is read and loaded into memory.
	const CBones * LoadBone(LPCTSTR szFileName);
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

class CMotion final
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
	BONE_FRAME	*m_pBoneFrame;		// ���밹�� ��ŭ �Ҵ�. �ε��� ������ ����ϱ� ������ �����͸� ���ġ�Ѵ�.  �������� ���°��� NULL
	BONE		*m_pBoneInfo;		// ���뱸������
	int		m_nMaxEvent;
	D3DXVECTOR3	m_vEvent[4];
	
	CMotion();
	~CMotion();

	//void Init( void );

	
	DWORD	GetAttr( int nNumFrm ) 
	{ 
		return m_pAttr[ nNumFrm ].m_dwAttr; 
	}

private:

	template<DWORD MA_FLAG>
	const MOTION_ATTR * GetMotionAttr(float fOldFrm, float fNumFrm) const {
		const MOTION_ATTR * pAttr = &m_pAttr[static_cast<int>(fNumFrm)];
		if (pAttr->m_dwAttr & MA_FLAG)
			if (fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm)	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr;
		return 0;
	}


public:

	[[nodiscard]] bool IsAttrHit(float fOldFrm, float fNumFrm) const {
		return GetMotionAttr<MA_HIT>(fOldFrm, fNumFrm);
	}

	[[nodiscard]] const MOTION_ATTR * IsAttrSound(float fOldFrm, float fNumFrm) const {
		return GetMotionAttr<MA_SOUND>(fOldFrm, fNumFrm);
	}

	[[nodiscard]] DWORD IsAttrQuake(float fOldFrm, float fNumFrm) const {
		const MOTION_ATTR * pAttr = GetMotionAttr<MA_QUAKE>(fOldFrm, fNumFrm);
		return pAttr ? pAttr->m_dwAttr : 0;
	}

	void	SetAttr( float fNumFrm, DWORD dwAttr ) 
	{
		m_pAttr[ (int)fNumFrm ].m_dwAttr |= dwAttr;
		m_pAttr[ (int)fNumFrm ].m_fFrame = fNumFrm;
	}
	void	ResetAttr( int nNumFrm, DWORD dwAttr ) { m_pAttr[ nNumFrm ].m_dwAttr &= (~dwAttr); }
	
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

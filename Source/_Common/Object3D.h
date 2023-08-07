#ifndef	__OBJECT3D_H__
#define	__OBJECT3D_H__

#include <d3dx9.h>
#include "Material.h"
#include "Bone.h"
#include <span>

#define D3DFVF_SKINVERTEX_BUMP (D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_NORMAL | D3DFVF_TEX1)
#define D3DFVF_SKINVERTEX (D3DFVF_XYZB3 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1)
#define D3DFVF_NORMALVERTEX_BUMP (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_NORMAL | D3DFVF_TEX1)
#define D3DFVF_NORMALVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

// Mesh Type
enum GMTYPE
{
	GMT_ERROR = -1,
	GMT_NORMAL,
	GMT_SKIN,
	GMT_BONE
};

// Vertex Buffer Type
enum VBTYPE
{
	VBT_ERROR = -1,
	VBT_NORMAL,
	VBT_BUMP,
};

struct SKINVERTEX
{
    D3DXVECTOR3 position;	// The 3D position for the vertex
	FLOAT		w1, w2;		// vertex weight
	DWORD		matIdx;
    D3DXVECTOR3 normal;		// The surface normal for the vertex
    FLOAT       tu, tv;		// The texture coordinates
};

#ifdef __YENV
struct SKINVERTEX_BUMP
{
    D3DXVECTOR3 position;	// The 3D position for the vertex
	FLOAT		w1, w2;		// vertex weight
	DWORD		matIdx;
    D3DXVECTOR3 normal;		// The surface normal for the vertex
	D3DXVECTOR3 tangent;	
	FLOAT       tu, tv;		// The texture coordinates
};
#endif //__YENV

struct NORMALVERTEX
{
    D3DXVECTOR3 position;	// The 3D position for the vertex
    D3DXVECTOR3 normal;		// The surface normal for the vertex
    FLOAT       tu, tv;		// The texture coordinates
};

#ifdef __YENV
struct NORMALVERTEX_BUMP
{
    D3DXVECTOR3 position;	// The 3D position for the vertex
    D3DXVECTOR3 normal;		// The surface normal for the vertex
	D3DXVECTOR3 tangent;	
	FLOAT       tu, tv;		// The texture coordinates
};
#endif //__YENV

struct	MATERIAL_BLOCK
{
	int		m_nStartVertex;			// ���ؽ� ���ۿ����� ���� �ε���
	int		m_nPrimitiveCount;		// ������Ƽ��(�ﰢ��)����
	int		m_nTextureID;			// �ؽ��� ��ȣ
	DWORD	m_dwEffect;
	int		m_nAmount;		// ����
	int		m_nMaxUseBone;
	int		m_UseBone[CBones::MAX_VS_BONE];
};

//
// ���ؽ��� �������� ���� �޽������� ����Ÿ ����
// Geometry Object
//
struct GMOBJECT		// base object
{
	GMTYPE	m_Type;					// GMOBJECT Type
	int			m_nVertexSize;		// sizeof(FVF)
	DWORD		m_dwFVF;			// FVF ����
	int			m_nMaxUseBone;		
	int			m_UseBone[ CBones::MAX_VS_BONE ];
	int			m_nID;
	int			m_bOpacity;
	int			m_bBump;
	int			m_bLight;
	int			m_nMaxVertexList;			// read
	int			m_nMaxFaceList;				// read
	int			m_nMaxMtrlBlk;				// read
	int			m_nMaxMaterial;				// read
	GMTYPE		m_ParentType;				// �θ��� Ÿ��
	int			m_nParentIdx;				// read		// ��Ų�϶� ���� �ε����� �ǰ� �Ϲ��϶� GMOBJECT�� �ε���
	GMOBJECT*	m_pParent;					// �θ�, ������带 �θ�� ���� ��ũ�޽ô� �̰ɾȾ���.
	D3DXMATRIX		m_mLocalTM;			// read
	D3DXMATRIX		m_mInverseTM;			// read �̸� ��ȯ�� �ι���TM 

	D3DXVECTOR3		*m_pVertexList;			// read �� ������ ���� Local��ǥ�� ��

	int				*m_pPhysiqueVertex;		// read ����ũ, �� ���ؽ� ����xũ���� �����ϴ� �� ID
	MATERIAL_BLOCK	*m_pMtrlBlk;			// read
	LPDIRECT3DTEXTURE9	*m_pMtrlBlkTexture;		// ���͸��� ���� �ؽ���������

#ifdef __YENV
	VBTYPE		m_VBType;
	LPDIRECT3DTEXTURE9	*m_pNormalTexture;		// �븻��(������)
	LPDIRECT3DTEXTURE9	*m_pNoSpecTexture;		// �뽺��ŧ����(������)
#endif //__YENV
	
	TM_ANIMATION	*m_pFrame;				// �ִϸ��̼��� ������ �����Ӽ���ŭ �ִ�

	D3DXVECTOR3		m_vBBMin, m_vBBMax;			// read bound box min, max

	struct TextureName { char strBitMapFileName[64]; };
	int				m_bMaterial;				// MES���Ͽ� ���͸����� �־�����?.  ������ ���ؼ� ��� �޾Ƶ�
	TextureName		m_MaterialAry[16];			// MES�� ����Ǿ� �ִ� Material����.  Save���� �ٽ� ���͸����� ���� �ؾ��� �ʿ䰡 ������ ���

	// void*�� VB�� ���� �׻� ĳ������ �Ұ�.
//	NORMALVERTEX	*m_ptempVB;
	void			*m_pVB;			// Indexed�� ���ؽ� ���� - ���ؽ� ����Ʈ�� �ٸ����� ��ǥ�� ������ TU,TV�� �ٸ� ���ؽ����� ���� �������̴�. matIdx�� ������ ���� �ʴ�.

	WORD			*m_pIB;			// �ε��� ����, m_pIIndexBuffer�� ���纻.
	WORD			*m_pIIB;		// m_pVB�� ��� m_pVertexList�� �����Ǵ����� ���� �ε���, m_nMaxIVB, ��Ű�׿� ����.  ���ؽ� ���ۿ� ���ؽ�����Ʈ�� ũ�Ⱑ �ٸ��⶧���� �̷��� �ʿ��ϴ�.
	int				m_nMaxVB;
	int				m_nMaxIB;

	// ��Ų�� ������Ʈ�϶��� VB�� NULL�̴�. �ܺ�����VB�� ���� ��� �Ѵ�.
	LPDIRECT3DVERTEXBUFFER9		m_pd3d_VB;		// Indexed�� VB
	LPDIRECT3DINDEXBUFFER9		m_pd3d_IB;		// Indexed�� IB

};

//
// 3D ������Ʈ
// �ΰ�, ����, ����, ȸ�������� ��.....
// Object3D�� GMOBJECT��ν� �����ȴ�.  Ŭ���������� ���ǻ� Object�� �θ���.
// o3d���� �̴�.
// 3D ������Ʈ ����Ÿ�� �ּҴ���.
// 
//
/*
�������� ����
* ��Ų,��Ų,��Ų,�Ϲ�,�Ϲ�,�Ϲ�
. ��Ų���� �Ѽ�Ʈ�� ������ ������ ����.
. �Ϲݵ��� ���� ����������Ʈ�� ��Ʈ������ ����.

��Ų
. ������ ���� ����.

�Ϲ�,�Ϲ�,�Ϲ�,�Ϲ�,�Ϲ�
. ���� ��Ʈ������ ����.

*/

#define		MAX_GROUP		3

#define		LOD_HIGH	0
#define		LOD_MEDIUM	1
#define		LOD_LOW		2

struct LOD_GROUP
{
	int			m_nMaxObject;
	GMOBJECT*	m_pObject;		// sizeof(GMOBJECT) * m_nMaxObject
	// �������� �ִϸ��̼��� ���ŵ� ��Ʈ�������� ��� �ӽù��� m_nMaxObject��ŭ ����. 
	// ���� ��Ų�� �ִ� ������Ʈ��� �̰��� �������� �ʴ´�.
	D3DXMATRIX	*_mUpdate;
};

class CObject3D
{
private:
	LOD_GROUP		*m_pGroup;		// ���� ���õ� LOD�׷�
	LOD_GROUP		m_Group[ MAX_GROUP ];		// LOD �׷�
	D3DXMATRIX	*m_pmExternBone;	// �ܺο��� �����ϴ� �� ������.
	D3DXMATRIX	*m_pmExternBoneInv;	// �ܺο��� �����ϴ� �� Inv ������.
	
	int			m_nMaxBone;			// �̿�����Ʈ�� ����ϴ� ������ ����. �ܺο��� ���븦 �����Ҷ��� �� ������ �´��� Ȯ������.
	D3DXMATRIX	*m_pBaseBone;		// o3d���� ����ִ� ����Ʈ ���� ��Ʈ. �̹̰�� ���� matWorld����Ʈ�� ����ִ�. ���븦 ���� �ʴ´ٸ� �̰��� NULL�̴�.
	D3DXMATRIX	*m_pBaseBoneInv;	// BaseBone�� Inverse. �̰��� destroy���� �۴´�.
	CMotion		*m_pMotion;			// o3d�� ���ִϸ��̼��� ���ԵǾ� �ִٸ� �Ҵ��.
	
	int		m_nMaxEvent;

	int		m_nTextureEx;			// ��� �ؽ��� ��Ʈ�� ���� �ؾ��ϴ°�.
	MOTION_ATTR		*m_pAttr;			// ������ũ�⸸ŭ �Ҵ�.  �� �����ӼӼ�.
	
public:
	D3DXVECTOR3	m_vEvent[CBones::MAX_MDL_EVENT];
	int		m_bSendVS;
	int		m_bLOD;						// LOD����Ÿ�� �ִ°�?
	int		m_nID;						// ������Ʈ�� ���� ���̵�
	int		m_nHavePhysique;			// ����ũ ������Ʈ�� ������ �ִ°�.
	char	m_szFileName[64];			// ������Ʈ ���� ��
	GMOBJECT	m_CollObject;	// �浹�� �޽�
	
	// �˱������� �ε����δ� �ȵǰ� ���� ���ؽ� ��ġ�� ����ؾ� �Ұ��̴�.
	D3DXVECTOR3		m_vBBMin, m_vBBMax;			// read bound box min, max
	D3DXVECTOR3		m_vForce1, m_vForce2;		// �˱� ������,����
	D3DXVECTOR3		m_vForce3, m_vForce4;			// �߰����� ������,����

	int		m_nMaxFace;					// ������Ʈ���� ������ ���� ����
	int		m_nNoTexture;				// �ؽ��ĸ� ������� ����.
	int		m_nNoEffect;				// ���¼����� �ǵ帮�� ����.
	
	// ���� ��ü Ű������ �ִϸ��̼��� ������� ���ȴ�.
	float		m_fPerSlerp;
	int			m_nMaxFrame;

	int		m_nUseCnt;		// ��� �����Ǿ� ���Ǿ��°�.	

	FLOAT	m_fAmbient[3];

#ifdef __BS_EFFECT_LUA
	DWORD m_dwEffect_EX;	// Render effect Ȯ��

	struct MTE_DATA	// Multi Texturing Effect
	{
		MTE_DATA() { _pTex[ 0 ] = NULL; _pTex[ 1 ] = NULL; _kUV[0] = D3DXVECTOR3( 0.f, 0.f, 0.f ); _kUV[1] = D3DXVECTOR3( 0.f, 0.f, 0.f ); }
		LPDIRECT3DTEXTURE9 _pTex[2];
		D3DXVECTOR3 _kUV[2];
	};
	MTE_DATA* m_pMteData;
#endif //__BS_EFFECT_LUA
	
private:
	void	Init( void );
	void	Destroy( void );

	int		LoadGMObject( CResFile *file, GMOBJECT *pObject );
	int		LoadTMAni	( CResFile *file, GMOBJECT *pObject );
	int		SaveGMObject( FILE *fp, GMOBJECT *pObject );
	int		SaveTMAni	( FILE *fp, GMOBJECT *pObject );

public:

#ifdef __YENV
	LPDIRECT3DVERTEXDECLARATION9 m_pNormalDecl;
	HRESULT						ExtractBuffers( int nType, LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB, GMOBJECT *pObject );
	LPDIRECT3DTEXTURE9			CreateNormalMap( int nType, LPDIRECT3DTEXTURE9* pTexture, LPCTSTR strFileName, LPCTSTR szPath = NULL );
#endif //__YENV
	
	CObject3D();
	~CObject3D();

	const int	GetMaxEvent( )		{ return m_nMaxEvent; }
	GMOBJECT*	GetGMOBJECT( int nIdx = 0 ) { return &m_pGroup->m_pObject[ nIdx ]; }
	int		GetMaxObject( void ) { return m_pGroup->m_nMaxObject; }
	int		GetMaxFrame( void ) { return m_nMaxFrame; }
	int		GetMaxBone( void )	{ return m_nMaxBone; }
	D3DXMATRIX *GetBaseBone( void ) { return m_pBaseBone; }
	void	SetExternBone( D3DXMATRIX *pmBone, D3DXMATRIX *pmBoneInv ) { if(m_pBaseBone == NULL) { m_pmExternBone = pmBone; m_pmExternBoneInv = pmBoneInv; } }	// ��Ų���� m_pBaseBone�� �̹� �Ҵ��ϰ� �ִ�.
//	int		Find( int nID );
	void	SetTexture( LPCTSTR szTexture );		// �������� �ؽ��ĸ� ������.
	void	SetTexture( LPDIRECT3DTEXTURE9 pTexture );
	void	SetTextureEx( GMOBJECT *pObj, int nNumEx );
	void	SetTextureEx( int nNumEx )
	{
		for( int i = 0; i < m_pGroup->m_nMaxObject; i ++ )
			SetTextureEx( &(m_pGroup->m_pObject[i]), nNumEx );		
	}
	void	LoadTextureEx( int nNumEx, GMOBJECT *pObj, std::span<LPDIRECT3DTEXTURE9, 16> pmMaterial );
	void	ChangeTexture( LPCTSTR szSrc, LPCTSTR szDest );
	void	SetBone( D3DXMATRIX *pmBone ) { m_pmExternBone = pmBone; }
	int		GetMaxVertex( void );
	int		GetMaxFace( void );
	int		GetMaxMtrlBlk( void );
	BOOL	IsHaveCollObj( void ) { return (m_CollObject.m_Type != GMT_ERROR)? TRUE : FALSE; }
	BOOL	IsUseExternBone( void ) { if( m_nHavePhysique && m_pBaseBone == NULL ) return TRUE; else return FALSE; }	// ��Ų������Ʈ�̰� ��ü ���� ������ ���� �ʴ�. �̰��� ���庻�� ����ϴ� ��Ų�̴�
	void	SetLOD( int nLevel ) 
	{ 
		if( nLevel < 0 || nLevel >= MAX_GROUP )
		{
			Error( "SetLOD : lv=%d, %s", nLevel, m_szFileName );
			nLevel = 0;
		}
		if( m_bLOD == 0 ) nLevel = 0;	// LOD����Ÿ�� ���ٸ� �׻� High�����θ�..
		m_pGroup = &m_Group[nLevel]; 
	}	// 0:high,  1:medium,   2:low

private:
	template<DWORD MA_FLAG>
	const MOTION_ATTR * GetMotionAttr(float fNumFrm) const {
		if (!m_pAttr) return nullptr;
		const MOTION_ATTR * pAttr = &m_pAttr[static_cast<int>(fNumFrm)];
		if (pAttr->m_dwAttr & MA_FLAG)
			if (pAttr->m_fFrame == fNumFrm)		// �Ӽ��� 8.0�� �ִµ� fNumFrm==8.5 �� �������� ��Ȯ�ϰ� ���ϱ� ���� �ʿ�.
				return pAttr;
		return nullptr;
	}

public:
	[[nodiscard]] bool IsAttrHit(float fNumFrm) const {
		return GetMotionAttr<MA_HIT>(fNumFrm);
	}

	[[nodiscard]] std::optional<int> IsAttrSound(float fNumFrm) const {
		const MOTION_ATTR * pAttr = GetMotionAttr<MA_SOUND>(fNumFrm);
		return pAttr && pAttr->m_nSndID > 0 ? std::optional(pAttr->m_nSndID) : std::nullopt;
	}

	[[nodiscard]] bool IsAttrQuake(float fNumFrm) const {
		return GetMotionAttr<MA_QUAKE>(fNumFrm);
	}
	
	HRESULT	CreateDeviceBuffer( GMOBJECT *pObject, LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB, D3DPOOL pool = D3DPOOL_MANAGED );
	HRESULT SendVertexBuffer( GMOBJECT *pObj, LPDIRECT3DVERTEXBUFFER9 pd3d_VB );
	HRESULT SendIndexBuffer( GMOBJECT *pObj );
	int		LoadObject( LPCTSTR szFileName );
static 	BOOL IsAnimateFile( LPCTSTR szFileName );		// szFileName�� �ִϸ��̼��� �ֳ�?
	int		SaveObject( LPCTSTR szFileName );
	void	ClearNormal( void );

	// �浹/���� ��ƾ��.
	int		SlideVectorXZ( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vEnd , const D3DXMATRIX &mWorld, BOOL bCollObj/* = TRUE*/, int nSlideCnt );
	int		SlideVectorXZ2( D3DXVECTOR3 *pOut, D3DXVECTOR3 *pIntersect, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vEnd , const D3DXMATRIX &mWorld, BOOL bCollObj = TRUE);
	int		SlideVectorUnder( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vEnd , const D3DXMATRIX &mWorld, D3DXVECTOR3 *pIntersect );
	void	FindTouchTriLine( D3DXVECTOR3 **pTriOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vEnd , const D3DXMATRIX &mWorld, FLOAT *pDist, BOOL bCollObj = TRUE );
	D3DXVECTOR3 *IntersectRayTri( const D3DXVECTOR3 &vRayOrig, const D3DXVECTOR3 &vRayDir, const D3DXMATRIX &mWorld, D3DXVECTOR3* pvIntersect, FLOAT* pfDist, BOOL bColl = FALSE );
	BOOL	SimpleTriIntersect(D3DXMATRIX mWorld, GMOBJECT* TargetObj, D3DXMATRIX mTargetWorld);
	void	ComputeInterval(float fVV0,float fVV1,float fVV2,float fD0,float fD1,float fD2,float fD0D1,float fD0D2,float &fA,float &fB,float &fC,float &fX0,float &fX1);
	void	Animate( FLOAT fFrameCurrent, int nNextFrame );
#ifndef __WORLDSERVER
	void    SetState( MATERIAL_BLOCK* pBlock, int nEffect, DWORD dwBlendFactor );
	void    ResetState( MATERIAL_BLOCK* pBlock, int nEffect, DWORD dwBlendFactor );
	void	SetShader( const D3DXMATRIX *mWorld );
	void	Render( LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB, FLOAT fFrameCurrent, int nNextFrame, const D3DXMATRIX *mWorld, int nEffect = 0, DWORD dwBlendFactor = 0xff000000 );
	void	RenderSkin( LPDIRECT3DVERTEXBUFFER9 pd3d_VB, GMOBJECT *pObj, const D3DXMATRIX *mWorld, int nEffect, DWORD dwBlendFactor = 0xff000000 );
	void	RenderNormal( GMOBJECT *pObj, const D3DXMATRIX *mWorld, int nEffect, int nBlendFactor = 255 );
	
	void	RenderBB( const D3DXMATRIX *mWorld );
#ifdef __SHADOW
	void	RenderNormalShadow( GMOBJECT *pObj, const D3DXMATRIX *mWorld, int nBlendFactor );
	void	RenderShadow( LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB, FLOAT fFrameCurrent, int nNextFrame, const D3DXMATRIX *mWorld, int nBlendFactor = 255 );
//	void	RenderShadow( FLOAT fFrameCurrent, int nNextFrame, const D3DXMATRIX *mWorld, int nBlendFactor );
#endif
#endif
	
	HRESULT InitDeviceObjects( ) { return S_OK; }
	HRESULT RestoreDeviceObjects( LPDIRECT3DVERTEXBUFFER9 *ppd3d_VB, D3DPOOL pool = D3DPOOL_MANAGED );
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

/* A collection of CObject3D. It enables the user to load the objects.
The CObject3DMng remains the owner of the CObject3D: users must call the
DeleteObject3D when they stop using a loaded CObject3D.

The same CObject3D instance may be returned multiple times.
*/
class	CObject3DMng final {
private:
	std::map<std::string, std::unique_ptr<CObject3D>, std::less<>> m_mapObject3D;

public:
	~CObject3DMng();

	[[nodiscard]] CObject3D * LoadObject3D(LPCTSTR szFileName);
	void DeleteObject3D(CObject3D * pObject3D);

	void Destroy();
};

extern CObject3DMng		g_Object3DMng;
extern D3DXMATRIX	g_mReflect;

HRESULT		CreateShadowVS( LPCTSTR szFileName );
HRESULT		CreateSkinningVS( LPCTSTR szFileName );
void		DeleteVertexShader( );


void	SetTransformViewProj( const D3DXMATRIX &mViewProj );

D3DXVECTOR3 *GetLastPickTri( void );		// ���������� ��ŷ�� �ﰢ�����ؽ��� ����������.



#endif // OBJECT3D

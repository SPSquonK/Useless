#ifndef __SFXBASE_H
#define __SFXBASE_H

#include <map>
#include <memory>
#include <functional>
#include <string_view>
#include <string>
#include <boost/container/stable_vector.hpp>
#include <boost/container/small_vector.hpp>
class CModel;

#define DEGREETORADIAN(x) ((2.0f*D3DX_PI*(x))/(360.0f))

class CSfxTexture;
class CSfxMeshMng;
class CSfxMng; 
class CSfxObjMng;
extern CSfxMng g_SfxMng; // SFX ���� ������
extern CSfxTexture g_SfxTex; // SFX�� ���Ǵ� �ؽ��ĸ� �����ϴ� ������
extern CSfxMeshMng g_SfxMeshMng; // SFX�� ���Ǵ� �޽��� �����ϴ� ������

enum SFXPARTTYPE {
	SFXPARTTYPE_BILL=1,
	SFXPARTTYPE_PARTICLE=2,
	SFXPARTTYPE_MESH=3,
	SFXPARTTYPE_CUSTOMMESH=4,
};
enum SFXPARTBILLTYPE {
	SFXPARTBILLTYPE_BILL=1,
	SFXPARTBILLTYPE_BOTTOM=2,
	SFXPARTBILLTYPE_POLE=3,
	SFXPARTBILLTYPE_NORMAL=4,
};
enum SFXPARTALPHATYPE {
	SFXPARTALPHATYPE_BLEND=1,
	SFXPARTALPHATYPE_GLOW=2,
};

//
// �ϳ��� SFX�� SFX������ ������. 
// 
//
//
//
//

#define D3DFVF_D3DSFXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
#ifndef DIR_SFX
	#define DIR_SFX     _T( "SFX\\"  )
#endif 

struct D3DSFXVERTEX // SFX�� ���Ǵ� ���ؽ� ����
{
	D3DXVECTOR3 p;
	FLOAT       tu1, tv1;
};

struct SfxKeyFrame
{
	WORD nFrame;       // Ű �������� ��ġ (������)
	D3DXVECTOR3 vPos;  // ��ġ
	D3DXVECTOR3 vPosRotate; // ���� ������ �߽����� �� �࿡ ���� ��ġȸ��
	D3DXVECTOR3 vScale; // ũ��
	D3DXVECTOR3 vRotate; // ȸ��
	int nAlpha;

	[[nodiscard]] static SfxKeyFrame FromFile(CResFile & file);
}; // Ű������ ����ü

struct Particle
{
	WORD nFrame;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSpeed;
	D3DXVECTOR3 vScale;
//#ifdef __ATEST
	D3DXVECTOR3 vRotation;
	D3DXVECTOR3 vScaleStart;
	D3DXVECTOR3 vScaleEnd;
	D3DXVECTOR3 vScaleSpeed;
	BOOL        bSwScal;
//#endif	
}; // ��ƼŬ ����ü

class CSfxPart
{
public:
//#ifdef __ATEST
	CString m_strName;  // ���̾� �̸�...
//#endif
	CString m_strTex; // �ؽ��� ���ϸ�
	SFXPARTTYPE m_nType; // �� ��Ʈ�� ���ϴ� ��Ʈ�ΰ�
	SFXPARTBILLTYPE m_nBillType; // �� ��Ʈ�� �������ΰ� �ٴ��ΰ�
	SFXPARTALPHATYPE m_nAlphaType; // ������ �뵵
	WORD m_nTexFrame; // �ؽ��� �ִϸ��̼� ������ �� (���)
	WORD m_nTexLoop;  // �ؽ��� �ִϸ��̼� �ѹ��� ���µ��� �ɸ��� ������ �� (�ð�)
	BOOL m_bUseing;

	// TODO: can we replace this with a std::vector<SfxKeyFrame>?
	boost::container::stable_vector<SfxKeyFrame> m_aKeyFrames; // Ű������ �迭

	CSfxPart() = default;
	virtual ~CSfxPart() = default;
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) ); // ����
#endif
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) ); // ����

	void AddAndAdjustKeyFrame(SfxKeyFrame frame); // Ű������ �߰� -  Ư�� �������� Ű�������� ������ ����

	SfxKeyFrame* GetPrevKey(WORD nFrame); // �־��� �����Ӻ��� ���� Ű�������� ����´�
	SfxKeyFrame* GetNextKey(WORD nFrame); // �־��� �����Ӻ��� ���� Ű�������� ����´�
	virtual void Load(CResFile& file); // �ε�
	virtual void OldLoad(CResFile& file); // �������� �ε�
	virtual void Load2(CResFile& file); 
	virtual void Load3(CResFile& file) {}; // �ֽŹ���
	
	struct ComputedKeyResult { SfxKeyFrame key; float lerp; };
	[[nodiscard]] std::optional<ComputedKeyResult> GetComputedKey(WORD nFrame, bool usePreviousScale = false) const; // Ư���������� ���������̼ǵ� Ű���� ����´�

};

class CSfxPartBill : public CSfxPart
{
public:
	CSfxPartBill();
	~CSfxPartBill() {}
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#endif
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void OldLoad(CResFile& file);
};

class CSfxPartParticle: public CSfxPart
{
public:
	WORD m_nParticleCreate;     // ������Ű�� ����(������)
	WORD m_nParticleCreateNum;     // ������Ű�� ����(������)

	// ��ƼŬ �� ���� ����������Ŭ
	WORD m_nParticleFrameAppear; // ��Ÿ���µ� �ɸ��� �ð�(������)
	WORD m_nParticleFrameKeep; // �����ð�(������)
	WORD m_nParticleFrameDisappear; // ������µ� �ɸ��� �ð�(������)

	/// ���� �ʱ�ġ�� �������� �ּҰ� ������ �������̴�.
	FLOAT m_fParticleStartPosVar; // ������ġ XZ���� (�ݰ�) �������� ��ġ�� �߽����� XZ���󿡼� �� ����ŭ ������ ������ ��ġ���� ����
	FLOAT m_fParticleStartPosVarY; // ������ġ Y ����. �������� Y��ǥ���� �� �� ������ ������ ��ġ���� ����
	FLOAT m_fParticleYLow; // �����ӵ� �ʱ�ġ ������
	FLOAT m_fParticleYHigh; // �����ӵ� �ʱ�ġ �ִ밪
	FLOAT m_fParticleXZLow; // ����ӵ� �ʱ�ġ ������
	FLOAT m_fParticleXZHigh; // ����ӵ� �ʱ�ġ �ִ밪

	D3DXVECTOR3 m_vParticleAccel;  // ���ӵ� ����... �����Ӹ��� �ӵ��� �������� ��

	D3DXVECTOR3 m_vScale; // �ʱ� ũ��
	D3DXVECTOR3 m_vScaleSpeed; // �����Ӹ��� ũ�⿡ �������� ��
	D3DXVECTOR3 m_vRotation; 
	D3DXVECTOR3 m_vRotationLow; 
	D3DXVECTOR3 m_vRotationHigh; 

//#ifdef __ATEST
	BOOL		m_bRepeatScal;
	FLOAT		m_fScalSpeedXLow;
	FLOAT		m_fScalSpeedXHigh;
	FLOAT		m_fScalSpeedYLow;
	FLOAT		m_fScalSpeedYHigh;
	FLOAT		m_fScalSpeedZLow;
	FLOAT		m_fScalSpeedZHigh;
	D3DXVECTOR3 m_vScaleSpeed2;
	D3DXVECTOR3 m_vScaleEnd;
//#endif
	
	
	BOOL        m_bRepeat;
	
	
	CSfxPartParticle();
	~CSfxPartParticle();

#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  ); // ��ƼŬ�� ��� �� �����Լ��� �����Ϳ����� ���ȴ�. �ٸ� ��Ʈ�� ����, ������ ��� ���.
#endif
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void Load3(CResFile& file);
	virtual void OldLoad(CResFile& file);
};
class CSfxPartMesh: public CSfxPart
{
public:
	CSfxPartMesh();
	~CSfxPartMesh();
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#endif
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void Load3(CResFile& file) {};
	virtual void OldLoad(CResFile& file);
};
class CSfxPartCustomMesh: public CSfxPart
{
public:
	int m_nPoints;
	CSfxPartCustomMesh();
	~CSfxPartCustomMesh();
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#endif
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void Load3(CResFile& file) {};
	virtual void OldLoad(CResFile& file);
};

// SFX�� ����. �ε��ϸ� SFX���� �����ص״ٰ� ���ӻ� ������Ʈ�� ���� ����.
class CSfxBase final {
public:
	std::vector<std::unique_ptr<CSfxPart>> m_aParts; // ��Ʈ�� �迭

	CSfxPart * AddPart(SFXPARTTYPE nType); // ��Ʈ �߰�
	BOOL Load(const std::string & filename); // �ε�
};

// CSfxBase ������
class CSfxMng
{
public:
	static LPDIRECT3DDEVICE9 m_pd3dDevice; // d3d ����̽� ������
	static LPDIRECT3DVERTEXBUFFER9 m_pSfxVB; // �� ����� ���ؽ� ����

	FLOAT m_fScale = 0.5f; // sfx�� ũ��

private:
	std::map<std::string, std::unique_ptr<CSfxBase>, std::less<>> m_sfxBases;

public:

	~CSfxMng();

	CSfxBase* GetSfxBase(std::string_view strSfxName); // SfxBase�� �̸����� ������ SfxBase�� �����͸� ����´�.

	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

// ���ӻ� ������Ʈ���� SFX�� ����� ���� CModel ������Ʈ
class CSfxModel : public CModel
{
public:

	WORD m_nSfxBase; //
	CSfxBase* m_pSfxBase;

	D3DXMATRIX  m_matScale;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vRotate;
	D3DXVECTOR3 m_vScale;
	WORD m_nCurFrame;
	
	using Particles = std::optional<std::vector<Particle>>;
	std::vector<Particles> m_apParticles;
	static std::vector<Particles> DuplicateStructure(CSfxBase & pSfxBase);

	CSfxModel();
	~CSfxModel();
	void DeleteAll(void);

	void SetSfx(CSfxBase* pSfxBase);
	void SetSfx(DWORD dwIndex);	
	void SetSfx(LPCTSTR strSfxName);
#ifndef __WORLDSERVER	
	virtual BOOL Render( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	BOOL RenderZ( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	void RenderParticles( D3DXVECTOR3 vPos,WORD nFrame,FLOAT fAngle,CSfxPartParticle* pPartParticle, Particles & pParticles, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
#endif
	virtual BOOL Render2( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	void RenderParticles2( D3DXVECTOR3 vPos,WORD nFrame,D3DXVECTOR3 fAngle,CSfxPartParticle* pPartParticle, Particles & pParticles, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	//void Render(void);
	BOOL Process(void);

#ifdef __CLIENT
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxModel>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxModel::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxModel::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxModel::m_pPool->Free( (CSfxModel*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxModel::m_pPool->Free( (CSfxModel*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
#endif	// __CLIENT
};

namespace useless_util {
	template<typename T>
	struct releaser {
		void operator()(T * t) const {
			if (t) {
				t->Release();
				delete t;
			}
		}
	};
}

template<typename T>
using unique_ptr_release = ::std::unique_ptr<T, useless_util::releaser<T>>;

// SFX������ ���Ǵ� �ؽ��� ������
class CSfxTexture final {
	std::map<CString, unique_ptr_release<IDirect3DTexture9>> m_aTextures; // ��Ʈ������ �ؽ��� �����͸� �����ϴ� �ؽ����̺�
public:
	//  ������ �̸��� �ؽ��� �����͸� �����ش�
	LPDIRECT3DTEXTURE9 Tex(const CString & str);

	// ��� �ؽ��� ����
	void DeleteAll() { m_aTextures.clear(); }
};

class CSfxMeshMng final {
public:
	LPDIRECT3DDEVICE9 m_pd3dDevice; // d3d����̽� ������
	std::map<CString, std::unique_ptr<CModelObject>> m_aMeshs; // ��Ʈ������ �޽� �����͸� �����ϴ� �ؽ����̺�

	CModelObject * Mesh(const CString & str); //  ������ �̸��� �޽� �����͸� �����ش�
	void DeleteAll() { m_aMeshs.clear(); } // ���� ����

	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

#ifdef __BS_EFFECT_LUA

void open_lua_sfx();
void close_lua_sfx();

void run_lua_sfx( int nState, OBJID caller, const char* szMoverName );

//for lua glue
struct lua_State;

// ��ü update 
struct SfxModelSet final {
	struct ConstructorParams {
		OBJID idMaster;
		const char * szSfxName;
		const char * szBoneName;
		BOOL bLoop;
		int nState;
	};

	SfxModelSet(const ConstructorParams & params)
		: SfxModelSet(params.idMaster, params.szSfxName, params.szBoneName, params.bLoop, params.nState) {
	}

	SfxModelSet(OBJID idMaster, const char * szSfxName, const char * szBoneName, BOOL bLoop, int _nState);
	
	bool Update();
	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	
	OBJID _idMaster;
	char _szBone[ 64 ];
	char _szFileName[ 64 ];
	int _nMaxFrame;
	BOOL _bLoop;
	std::unique_ptr<CSfxModel> _pModel; /* never empty */

	int _nState;		//���� : ������϶� �ߵ��Ǵ� ȿ������
};

//gmpbigsun(100128 ) : Lua���� ȣ���� sfx( ��� �Ӽ��� data�� �����ϴ� ) ������
// bone �� ��ũ�Ǿ� loop�Ǵ� ���常 �����Ѵ�. 
class CSfxModelMng final {
public:
	bool AddData(SfxModelSet::ConstructorParams sfxParams);
	
	BOOL SubData( OBJID objID );						//�ش� ������Ʈ�� ��� lua���� sfx���� 
	BOOL SubData( OBJID objID, const int nState );		//�ش� ������Ʈ�� ��� ���¿� �ش��ϴ� sfx����
	BOOL SubData( OBJID objID, const char* szBone );	//�ش� ������Ʈ�� �ش� ���� ��ũ�� ��� sfx����
	
	void Update( );
	void Render( LPDIRECT3DDEVICE9 pd3dDevice );

	static CSfxModelMng* GetThis();
	static void Free();

public:
	std::map< DWORD, int > _cWaitingObj;

private:
	static CSfxModelMng* _pThis;

	std::map<OBJID, boost::container::small_vector<SfxModelSet, 10>> _cDatas;
};


#endif //__BS_EFFECT_LUA

#endif

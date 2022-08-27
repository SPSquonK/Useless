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
extern CSfxMng g_SfxMng; // SFX 원형 관리자
extern CSfxObjMng g_SfxObjMng; // 실제 생성된 SFX 오브젝트 관리자 (게임에서는 CModelMng를 사용하므로 필요없음)
extern CSfxTexture g_SfxTex; // SFX에 사용되는 텍스쳐를 관리하는 관리자
extern CSfxMeshMng g_SfxMeshMng; // SFX에 사용되는 메쉬를 관리하는 관리자

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
// 하나의 SFX는 SFX원형을 가진다. 
// 
//
//
//
//

#define D3DFVF_D3DSFXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
#ifndef DIR_SFX
	#define DIR_SFX     _T( "SFX\\"  )
#endif 

struct D3DSFXVERTEX // SFX에 사용되는 버텍스 포맷
{
	D3DXVECTOR3 p;
	FLOAT       tu1, tv1;
};

struct SfxKeyFrame
{
	WORD nFrame;       // 키 프레임의 위치 (프레임)
	D3DXVECTOR3 vPos;  // 위치
	D3DXVECTOR3 vPosRotate; // 로컬 원점을 중심으로 각 축에 대한 위치회전
	D3DXVECTOR3 vScale; // 크기
	D3DXVECTOR3 vRotate; // 회전
	int nAlpha;

	[[nodiscard]] static SfxKeyFrame FromFile(CResFile & file);
}; // 키프레임 구조체

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
}; // 파티클 구조체

class CSfxPart
{
public:
//#ifdef __ATEST
	CString m_strName;  // 레이어 이름...
//#endif
	CString m_strTex; // 텍스쳐 파일명
	SFXPARTTYPE m_nType; // 이 파트는 뭐하는 파트인가
	SFXPARTBILLTYPE m_nBillType; // 이 파트는 빌보드인가 바닥인가
	SFXPARTALPHATYPE m_nAlphaType; // 알파의 용도
	WORD m_nTexFrame; // 텍스쳐 애니메이션 프레임 수 (장수)
	WORD m_nTexLoop;  // 텍스쳐 애니메이션 한바퀴 도는동안 걸리는 프레임 수 (시간)
	BOOL m_bUseing;

	// TODO: can we replace this with a std::vector<SfxKeyFrame>?
	boost::container::stable_vector<SfxKeyFrame> m_aKeyFrames; // 키프레임 배열

	CSfxPart() = default;
	virtual ~CSfxPart() = default;
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) ); // 렌더
#endif
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) ); // 렌더

	void DeleteAllKeyFrame();
	void AddAndAdjustKeyFrame(SfxKeyFrame frame); // 키프레임 추가 -  특정 프레임의 키프레임의 내용을 갱신

	SfxKeyFrame* GetPrevKey(WORD nFrame); // 주어진 프레임부터 이전 키프레임을 갖고온다
	SfxKeyFrame* GetNextKey(WORD nFrame); // 주어진 프레임부터 다음 키프레임을 갖고온다
	virtual void Load(CResFile& file); // 로드
	virtual void OldLoad(CResFile& file); // 이전버젼 로드
	virtual void Load2(CResFile& file); 
	virtual void Load3(CResFile& file) {}; // 최신버젼
	
	struct ComputedKeyResult { SfxKeyFrame key; float lerp; };
	[[nodiscard]] std::optional<ComputedKeyResult> GetComputedKey(WORD nFrame, bool usePreviousScale = false) const; // 특정프레임의 인터폴레이션된 키값을 갖고온다

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
	WORD m_nParticleCreate;     // 생성시키는 간격(프레임)
	WORD m_nParticleCreateNum;     // 생성시키는 간격(프레임)

	// 파티클 한 개의 라이프사이클
	WORD m_nParticleFrameAppear; // 나타나는데 걸리는 시간(프레임)
	WORD m_nParticleFrameKeep; // 유지시간(프레임)
	WORD m_nParticleFrameDisappear; // 사라지는데 걸리는 시간(프레임)

	/// 다음 초기치는 최저값과 최소값 사이의 랜덤값이다.
	FLOAT m_fParticleStartPosVar; // 시작위치 XZ범위 (반경) 생성기의 위치를 중심으로 XZ평면상에서 이 값만큼 떨어진 임의의 위치에서 생성
	FLOAT m_fParticleStartPosVarY; // 시작위치 Y 범위. 생성기의 Y좌표에서 이 값 사이의 임의의 위치에서 생성
	FLOAT m_fParticleYLow; // 수직속도 초기치 최저값
	FLOAT m_fParticleYHigh; // 수직속도 초기치 최대값
	FLOAT m_fParticleXZLow; // 수평속도 초기치 최저값
	FLOAT m_fParticleXZHigh; // 수평속도 초기치 최대값

	D3DXVECTOR3 m_vParticleAccel;  // 가속도 벡터... 프레임마다 속도에 더해지는 값

	D3DXVECTOR3 m_vScale; // 초기 크기
	D3DXVECTOR3 m_vScaleSpeed; // 프레임마다 크기에 더해지는 값
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
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  ); // 파티클의 경우 이 렌더함수는 에디터에서만 사용된다. 다른 파트는 게임, 에디터 모두 사용.
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

// SFX의 원형. 로딩하면 SFX별로 저장해뒀다가 게임상 오브젝트가 꺼내 쓴다.
class CSfxBase final
{
public:
	CPtrArray m_apParts; // 파트의 배열
	CSfxPart* Part(BYTE nIndex) { // 지정한 파트의 포인터를 꺼내온다. 범위를 벗어나면 NULL을 리턴
		if(m_apParts.GetSize()>nIndex) return (CSfxPart*)(m_apParts[nIndex]);
		return NULL;
	}

	CSfxBase();
	~CSfxBase();

	CSfxPart* AddPart(SFXPARTTYPE nType); // 파트 추가
	void DeletePart(BYTE nIndex); // 파트 삭제
	void AdjustPart(BYTE nIndex); // 파트 수정

	BOOL Load(const std::string & filename); // 로드
};

// CSfxBase 관리자
class CSfxMng
{
public:
	static LPDIRECT3DDEVICE9 m_pd3dDevice; // d3d 디바이스 포인터
	static LPDIRECT3DVERTEXBUFFER9 m_pSfxVB; // 에 사용할 버텍스 버퍼

	FLOAT m_fScale = 0.5f; // sfx의 크기

private:
	std::map<std::string, std::unique_ptr<CSfxBase>, std::less<>> m_sfxBases;

public:

	~CSfxMng();

	CSfxBase* GetSfxBase(std::string_view strSfxName); // SfxBase의 이름으로 지정한 SfxBase의 포인터를 갖고온다.

	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

// 게임상 오브젝트에서 SFX의 출력을 위한 CModel 오브젝트
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

	CPtrArray m_apParticles;

	CSfxModel();
	~CSfxModel();
	void DeleteAll(void);
	void RemovePart( int nIndex );
	void SetSfx(CSfxBase* pSfxBase);
	void SetSfx(DWORD dwIndex);	
	void SetSfx(LPCTSTR strSfxName);
#ifndef __WORLDSERVER	
	virtual BOOL Render( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	BOOL RenderZ( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	void RenderParticles( D3DXVECTOR3 vPos,WORD nFrame,FLOAT fAngle,CSfxPartParticle* pPartParticle,CPtrArray* pParticles, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
#endif
	virtual BOOL Render2( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	void RenderParticles2( D3DXVECTOR3 vPos,WORD nFrame,D3DXVECTOR3 fAngle,CSfxPartParticle* pPartParticle,CPtrArray* pParticles, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
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

// 게임상 실제 SFX오브젝트들의 관리자. (만들어는 놨는데 다른걸로 관리하므로 실제로는 안쓴다)
class CSfxObjMng
{
public:
	CPtrArray m_apSfxObj; // 게임상 SFX오브젝트들의 배열

	CSfxObjMng();
	~CSfxObjMng();
	void AddObj(CSfxBase *pSfxBase,D3DXVECTOR3 vPos=D3DXVECTOR3(0,0,0),D3DXVECTOR3 vRotate=D3DXVECTOR3(0,0,0)); // 
	void Process(void);
#ifndef __WORLDSERVER
	void Render(void);
#endif
	void RemoveAll(void); // 구성요소를 전부 파괴한다
};

// SFX에서만 사용되는 텍스쳐 관리자
class CSfxTexture
{
public:
	CMapStringToPtr m_apTexture; // 스트링으로 텍스쳐 포인터를 참조하는 해쉬테이블

	CSfxTexture();
	~CSfxTexture();

	LPDIRECT3DTEXTURE9 AddTex(CString str); // 텍스쳐 추가
	void DeleteTex(CString str); // 텍스쳐 삭제
	LPDIRECT3DTEXTURE9 Tex(CString str); //  지정한 이름의 텍스쳐 포인터를 돌려준다
	void DeleteAll(void); // 모든 텍스쳐 삭제
};

class CSfxMeshMng
{
public:
	LPDIRECT3DDEVICE9 m_pd3dDevice; // d3d디바이스 포인터
	CMapStringToPtr m_apMesh; // 스트링으로 메쉬 포인터를 참조하는 해쉬테이블

	CSfxMeshMng();
	~CSfxMeshMng();

	CModelObject* AddMesh(CString str);// 메쉬 추가
	void DeleteMesh(CString str); // 메쉬 삭제
	CModelObject* Mesh(CString str); //  지정한 이름의 메쉬 포인터를 돌려준다
	void DeleteAll(void); // 전부 삭제

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

// 자체 update 
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

	int _nState;		//상태 : 어떤상태일때 발동되는 효과인지
};

//gmpbigsun(100128 ) : Lua에서 호출한 sfx( 모든 속성이 data로 존재하는 ) 관리자
// bone 에 링크되어 loop되는 이펙만 지원한다. 
class CSfxModelMng final {
public:
	bool AddData(SfxModelSet::ConstructorParams sfxParams);
	
	BOOL SubData( OBJID objID );						//해당 오브젝트의 모든 lua관련 sfx삭제 
	BOOL SubData( OBJID objID, const int nState );		//해당 오브젝트의 모든 상태에 해당하는 sfx삭제
	BOOL SubData( OBJID objID, const char* szBone );	//해당 오브젝트의 해당 본에 링크된 모든 sfx삭제
	
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

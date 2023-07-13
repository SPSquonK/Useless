#ifndef		__MATERIAL_H__
#define		__MATERIAL_H__

#include <d3d9.h>

struct MATERIAL
{
	D3DMATERIAL9			m_Material;
	LPDIRECT3DTEXTURE9		m_pTexture;		// �ؽ��ĸ޸� ������
	char			strBitMapFileName[64];	// �ؽ��� ���ϸ�
	int						m_nUseCnt;		// ���� �ؽ��İ� ��� ���ƴ��� ...
	BOOL					m_bActive;
};

#define		MAX_MATERIAL		2048

class CTextureManager
{
protected:
	int		m_nMaxTexture;
	
public:
	MATERIAL	m_pMaterial[ MAX_MATERIAL ];		// ���ӿ��� ����ϴ� ��� ��Ʈ������ ����.
	CTextureManager();
	~CTextureManager();

	HRESULT DeleteDeviceObjects();

	int		DeleteMaterial( LPDIRECT3DTEXTURE9 pTexture );	// pTexture�� ����ϴ� ���͸����� ã�� �����Ѵ�.
	
	D3DMATERIAL9			*GetMaterial( int nIdx ) { return &m_pMaterial[ nIdx ].m_Material; }
	LPDIRECT3DTEXTURE9		GetTexture( int nIdx ) { return m_pMaterial[ nIdx ].m_pTexture; }

	MATERIAL	*AddMaterial( LPCTSTR strFileName, LPCTSTR szPath = NULL );		// ���͸��� �ϳ��� �߰��ϰ� �� �����͸� �ǵ�����.
};

extern CTextureManager		g_TextureMng;


#endif


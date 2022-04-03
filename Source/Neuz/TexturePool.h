
// gmpbigsun
// date : 2009_09_09
// texture pooling

#pragma  once 


typedef std::map< string, IDirect3DTexture9* >		Texture_Container;
typedef Texture_Container::iterator					Texture_Iter;

class TexturePool 
{
	// ��� IDirect3DTexture9*�� �����ϴ� pool�� �������� ��������� ������ �����͸� ������ �ȴ� -_-;
public:
	TexturePool( );
	~TexturePool( );

	static TexturePool* Get( );
	void Init( IDirect3DDevice9* pDevice )		{ _pMyDevice = pDevice; }

	void Free( );
	IDirect3DTexture9* GetTexture( const string& dir, const string& filename );		//if not find, create the texture

protected:
	IDirect3DDevice9* _pMyDevice;
	Texture_Container _cTextures;

};







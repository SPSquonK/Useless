
// gmpbigsun
// date : 2009_09_09
// texture pooling

#pragma  once 


typedef std::map< std::string, IDirect3DTexture9* >		Texture_Container;
typedef Texture_Container::iterator					Texture_Iter;

class TexturePool 
{
	// ��� IDirect3DTexture9*�� �����ϴ� pool�� �������� ��������� ������ �����͸� ������ �ȴ� -_-;
public:
	TexturePool( );
	~TexturePool( );

	static TexturePool* Get( );

	void Free( );
	IDirect3DTexture9* GetTexture( const std::string& dir, const std::string& filename );		//if not find, create the texture

protected:
	Texture_Container _cTextures;

};








#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_



#include <d3d11.h>
#include <d3dx11tex.h>
#include <string>


class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	std::string GetTexturePath();

private:
	ID3D11ShaderResourceView* m_texture;
	std::string path;
	std::string name;
};

#endif
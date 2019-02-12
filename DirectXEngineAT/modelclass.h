
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_



#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;



#include "textureclass.h"
#include <vector>

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};



public:

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ModelType* GetModel();

	TextureClass* texture;
	ID3D11ShaderResourceView* GetTexture();
	TextureClass* GetTextureObj();

	std::string getName() { return name; }
	void setName(std::string name) { this->name = name; }


	void SetPosition(float x, float y, float z);
	void GetPosition(float & x, float & y, float & z);

	void SetRotation(float x, float y, float z);
	void GetRotationY(float & y);

	void SetVertexBuffers(ID3D11Buffer*);

private:
	std::string name;
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();


private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationY;
};

#endif
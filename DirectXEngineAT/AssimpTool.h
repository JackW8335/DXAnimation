#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include <vector>
//#include <D3DX10math.h>
//#include <D3D11.h>
#include "modelclass.h"
class AssimpTool
{
public:
	AssimpTool();
	~AssimpTool(void);

	bool loadModel(std::string file);
	bool processData();
	std::vector<float> *getVertexData();
	std::vector<uint16_t> *getIndexData();
	bool Initialize(ID3D11Device*, char*, WCHAR*);

	void Shutdown();
	void RenderBuffers(ID3D11DeviceContext * deviceContext);
	void Render(ID3D11DeviceContext* deviceContext);

	ID3D11ShaderResourceView * GetTexture();

	bool LoadTexture(ID3D11Device * device, WCHAR * filename);

	int GetIndexCount();

	void ReleaseTexture();

private:
	bool InitializeBuffers(ID3D11Device * device);
	void InitNodes();
	//bool assimpGetMeshData(const aiMesh *mesh);
	TextureClass* m_Texture;

	void ShutdownBuffers();
private:
	Assimp::Importer            importer;
	const aiScene               *modelScene;
	const aiNode                *modelNode;
	const aiMesh                *modelMesh;
	const aiFace                *modelFace;

	std::vector<const aiNode*>  nodeBuff;
	unsigned int                numNodeBuff;

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};


	int m_vertexCount, m_indexCount;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};
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
#include "modelclass.h"
#include "Mesh.h"
#include <map>
#include <algorithm>
#include "Bone.h"

struct VertexBoneData
{
	UINT IDs[AI_SBBC_DEFAULT_MAX_BONES];
	float Weights[AI_SBBC_DEFAULT_MAX_BONES];
};

class AssimpTool
{
public:
	AssimpTool();
	~AssimpTool(void);

	bool Load(HWND hwnd, ID3D11Device* dev, ID3D11DeviceContext* devcon, std::string filename);
	bool Initialize(ID3D11Device*, char*, WCHAR*);

	void Shutdown();
	void RenderBuffers(ID3D11DeviceContext * deviceContext);
	void Render(ID3D11DeviceContext* deviceContext);
	void Draw(ID3D11DeviceContext * devcon);

	ID3D11ShaderResourceView * GetTexture();

	bool LoadTexture(ID3D11Device * device, WCHAR * filename);

	int GetIndexCount();

	void ReleaseTexture();

	void SetRotation(float x, float y, float z);

	void GetRotation(float & x, float & y, float & z);

private:
	bool InitializeBuffers(ID3D11Device * device);
	void Close();
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

	aiMatrix4x4 m_GlobalInverseTransform;

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	void processNode(aiNode* node, const aiScene* scene);
	void processBones(aiMesh * mesh);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	int m_vertexCount, m_indexCount;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;
	std::vector<Mesh> meshes;
	std::vector<BONE> bones;
	string directory;
	HWND hwnd;

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
};
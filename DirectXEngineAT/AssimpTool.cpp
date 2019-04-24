#include "AssimpTool.h"

AssimpTool::AssimpTool()
{
	m_vertexCount = 0;
	m_indexCount = 0;
}

AssimpTool::~AssimpTool(void)
{
}

bool AssimpTool::Load(HWND hwnd, ID3D11Device * dev, ID3D11DeviceContext * devcon, std::string filename)
{

	modelScene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded);

	if (modelScene == NULL)
		return false;

	this->directory = filename.substr(0, filename.find_last_of('/'));

	this->dev = dev;
	this->hwnd = hwnd;

	m_GlobalInverseTransform = modelScene->mRootNode->mTransformation;
	m_GlobalInverseTransform.Inverse();

	processNode(modelScene->mRootNode, modelScene);

	return true;
}



//bool AssimpTool::InitializeBuffers(ID3D11Device* device)
//{
//	VertexType* vertices;
//	unsigned long* indices;
//	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
//	D3D11_SUBRESOURCE_DATA vertexData, indexData;
//	HRESULT result;
//	aiMesh* mesh;
//
//	InitNodes();
//
//	if (!modelScene)
//	{
//		return false;
//	}
//	if (modelScene->mNumMeshes > 0)
//	{
//		int faceCount = 0;
//		for (unsigned int m = 0; m < modelScene->mNumMeshes; m++)
//		{
//			m_vertexCount += modelScene->mMeshes[m]->mNumVertices;
//			faceCount += modelScene->mMeshes[m]->mNumFaces;
//		}
//	
//
//		m_indexCount = m_vertexCount;
//
//		// Create the vertex array.
//		vertices = new VertexType[m_vertexCount];
//		if (!vertices)
//		{
//			return false;
//		}
//
//		 //Create the index array.
//		indices = new unsigned long[faceCount * 3];
//		if (!indices)
//		{
//			return false;
//		}
//
//		int current_indice = 0;
//
//		for (unsigned int a = 0; a < nodeBuff.size(); a++)
//		{
//			/*modelNode = nodeBuff.at(a);
//
//			if (modelNode->mNumMeshes > 0)
//				for (unsigned int b = 0; b < modelNode->mNumMeshes; b++)
//				{
//					mesh = modelScene->mMeshes[modelNode->mMeshes[b]];
//
//					for (int v = 0; v < mesh->mNumVertices; v++)
//					{
//						vertices[v].position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
//						vertices[v].texture = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
//						vertices[v].normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
//					}
//
//					for (int f = 0; f < mesh->mNumFaces; f++)
//					{
//						aiFace* face = &mesh->mFaces[f];
//
//						indices[current_indice] = face->mIndices[0];
//						indices[current_indice + 1] = face->mIndices[1];
//						indices[current_indice + 2] = face->mIndices[2];
//
//						current_indice += 3;
//					}
//				}*/
//		}
//	}
//	// Set up the description of the static vertex buffer.
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	vertexData.pSysMem = vertices;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now create the vertex buffer.
//	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Set up the description of the static index buffer.
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//	indexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the index data.
//	indexData.pSysMem = indices;
//	indexData.SysMemPitch = 0;
//	indexData.SysMemSlicePitch = 0;
//
//	// Create the index buffer.
//	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Release the arrays now that the vertex and index buffers have been created and loaded.
//	delete[] vertices;
//	vertices = 0;
//
//	delete[] indices;
//	indices = 0;
//
//	return true;
//}

void AssimpTool::Close()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Close();
	}

	dev->Release();
}

void AssimpTool::InitNodes()
{
	bool repeat = true;

	nodeBuff.push_back(modelScene->mRootNode);
	// I raise all nodes tree to the root level 
	while (repeat)
	{
		for (unsigned int a = 0; a < nodeBuff.size(); a++)
		{
			modelNode = nodeBuff.at(a);
			if (modelNode->mNumChildren > 0)
				for (unsigned int c = 0; c < modelNode->mNumChildren; c++)
				{
					nodeBuff.push_back(modelNode->mChildren[c]);

				}

			else repeat = false;
		}
	}
}

void AssimpTool::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void AssimpTool::Draw(ID3D11DeviceContext * devcon)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(devcon);
	}
}

void AssimpTool::processNode(aiNode * node, const aiScene * scene)
{


	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->processMesh(mesh, scene));
		m_indexCount += mesh->mNumVertices;

		processBones(mesh);

	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

void AssimpTool::processBones(aiMesh * mesh)
{

	std::vector<int> boneArray;

	boneArray.reserve(20);
	boneArray.assign(20, 0);
	std::vector<int> boneIndex1;
	std::vector<int> boneIndex2;

	
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		aiBone* bone = mesh->mBones[i];

		aiString name = bone->mName;
		for (int j = 0; j < bone->mNumWeights; j++)
		{
			aiVertexWeight weight = bone->mWeights[j];

			int vertexIndex = weight.mWeight;
			int fIndex = vertexIndex;
			
			if (boneIndex1.size() < vertexIndex)
			{
				boneArray[fIndex] = j;
				boneArray[fIndex + 2] = weight.mWeight;
			}
			else if (boneIndex1[vertexIndex] == 0)
			{
				boneArray[fIndex + 1] = j;
				boneArray[fIndex + 3] = weight.mWeight;
			}
			else if (boneIndex2.size() < vertexIndex)
			{
				boneArray[fIndex + 4] = j;
				boneArray[fIndex + 6] = weight.mWeight;
			}
			else if (boneIndex2[vertexIndex] == 0)
			{
				boneArray[fIndex + 5] = j;
				boneArray[fIndex + 7] = weight.mWeight;
			}
			BONE bone;
			bone.name = name.data;
			bone.weight = weight.mWeight;
			bone.boneID = fIndex;
			bones.push_back(bone);

			aiMatrix4x4 inverseRootTransform = modelScene->mRootNode->mTransformation;
			
			
		}

		Bone* bones = new Bone[boneArray.size];

		for (int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone *bone = mesh->mBones[b];

		}
	}

	
}

Mesh AssimpTool::processMesh(aiMesh * mesh, const aiScene * scene)
{
	//// Data to fill
	vector<VERTEX> vertices;
	vector<UINT> indices;


	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX vertex;

		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	return Mesh(dev, vertices, indices);
}


bool AssimpTool::Initialize(ID3D11Device *device, char *fileName, WCHAR* textureFile)
{

	Load(hwnd, device, devcon, fileName);

	//InitializeBuffers(dev);

	LoadTexture(dev, textureFile);

	return true;
}

void AssimpTool::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	Close();
	//ShutdownBuffers();
	return;
}

void AssimpTool::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void AssimpTool::Render(ID3D11DeviceContext* deviceContext)
{
	//RenderBuffers(deviceContext);
	Draw(deviceContext);
}

ID3D11ShaderResourceView* AssimpTool::GetTexture()
{
	return m_Texture->GetTexture();
}

bool AssimpTool::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

int AssimpTool::GetIndexCount()
{
	return m_indexCount;
}

void AssimpTool::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void AssimpTool::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void AssimpTool::GetRotation(float &x, float& y, float&z)
{
	x = m_rotationX;
	y = m_rotationY;
	y = m_rotationZ;
	return;
}


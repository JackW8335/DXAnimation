#include "AssimpTool.h"

AssimpTool::AssimpTool()
{
}

AssimpTool::~AssimpTool(void)
{
}

bool AssimpTool::loadModel(std::string file)
{
	modelScene = importer.ReadFile(file, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure | 0

	);

	if (!modelScene)
	{
		MessageBoxA(NULL, importer.GetErrorString(), "Error: ", MB_ICONERROR);
		return false;
	}
	else
		//processData();

	return true;
}


bool AssimpTool::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	aiMesh* mesh;

	//InitNodes();
	//// Get node information from the root level (all nodes)
	//for (unsigned int a = 0; a < nodeBuff.size(); a++)
	//{
	//	modelNode = nodeBuff.at(a);

	//	if (modelNode->mNumMeshes > 0)
	//		for (unsigned int b = 0; b < modelNode->mNumMeshes; b++) {
	//			aiMesh* mesh = modelScene->mMeshes[b];
	//		}
	//}
	if (!modelScene)
	{
		return false;
	}
	if (modelScene->mNumMeshes > 0)
	{

		for (unsigned int m = 0; m < modelScene->mNumMeshes; m++)
		{
			mesh = modelScene->mMeshes[m];

			m_vertexCount = mesh->mNumVertices;
			m_indexCount = mesh->mFaces->mNumIndices;
			m_indexCount = m_vertexCount;
			// Create the vertex array.
			vertices = new VertexType[m_vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Create the index array.
			indices = new unsigned long[m_vertexCount];
			if (!indices)
			{
				return false;
			}

			// Load the vertex array and index array with data.
			for (int i = 0; i < m_vertexCount; i++)
			{
				vertices[i].position = D3DXVECTOR3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertices[i].texture = D3DXVECTOR2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				vertices[i].normal = D3DXVECTOR3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);			
			}
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
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
//bool AssimpTool::assimpGetMeshData(const aiMesh *mesh)
//{
//	aiFace *face;
//
//	for (unsigned int vertices = 0; vertices < mesh->mNumVertices; vertices++)
//	{
//		vertexBuff.push_back(mesh->mVertices[vertices].x);
//		vertexBuff.push_back(mesh->mVertices[vertices].y);
//		vertexBuff.push_back(mesh->mVertices[vertices].z);
//
//
//		vertexBuff.push_back(mesh->mNormals[vertices].x);
//		vertexBuff.push_back(mesh->mNormals[vertices].y);
//		vertexBuff.push_back(mesh->mNormals[vertices].z);
//
//
//
//
//		if (mesh->HasTextureCoords(0)) {
//			vertexBuff.push_back(mesh->mTextureCoords[0][vertices].x);
//			vertexBuff.push_back(mesh->mTextureCoords[0][vertices].y);
//		}
//		else
//		{
//			vertexBuff.push_back(0);
//			vertexBuff.push_back(0);
//		}
//		/*
//		vertexBuff.push_back(mesh->mTangents[v].x);
//		vertexBuff.push_back(mesh->mTangents[v].y);
//		vertexBuff.push_back(mesh->mTangents[v].z);*/
//
//	}
//
//	for (unsigned int faces = 0; faces < mesh->mNumFaces; faces++)
//	{
//		face = &mesh->mFaces[faces];
//		indexBuff.push_back(face->mIndices[0]);
//		indexBuff.push_back(face->mIndices[1]);
//		indexBuff.push_back(face->mIndices[2]);
//	}
//
//	return true;
//}
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
bool AssimpTool::processData()
{
	bool repeat = true;

	nodeBuff.push_back(modelScene->mRootNode);


	/* if (modelScene->mNumMeshes > 0)
	{
	for (unsigned int m=0;m<modelScene->mNumMeshes;m++)
	this->assimpGetMeshData(modelScene->mMeshes[m]);
	}*/

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

	// Get node information from the root level (all nodes)
	for (unsigned int a = 0; a < nodeBuff.size(); a++)
	{
		modelNode = nodeBuff.at(a);

		if (modelNode->mNumMeshes > 0)
			for (unsigned int b = 0; b < modelNode->mNumMeshes; b++) {
				//assimpGetMeshData(modelScene->mMeshes[b]);
			}
	}
	return true;
}

//std::vector<float> *AssimpTool::getVertexData()
//{
//	return &vertexBuff;
//}
//
//std::vector<uint16_t> *AssimpTool::getIndexData()
//{
//	return &indexBuff;
//}

bool AssimpTool::Initialize(ID3D11Device *device, char *fileName, WCHAR* textureFile)
{
	loadModel(fileName);

	InitializeBuffers(device);

	LoadTexture(device, textureFile);

	return true;
}

void AssimpTool::Shutdown()
{
		// Release the model texture.
		ReleaseTexture();

		// Shutdown the vertex and index buffers.
		ShutdownBuffers();
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
	RenderBuffers(deviceContext);
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




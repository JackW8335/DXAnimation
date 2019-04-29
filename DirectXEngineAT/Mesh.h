#pragma once

#include <vector>
using namespace std;

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

struct VERTEX {
	FLOAT X, Y, Z;
	XMFLOAT2 texcoord;
};

struct BONE {
	FLOAT X, Y, Z;
	XMFLOAT2 texcoord;
	string name;
	UINT boneID;
	FLOAT weight;
};

class Mesh {
public:
	vector<VERTEX> vertices;
	vector<UINT> indices;
	
	ID3D11Device *dev;

	Mesh(ID3D11Device *dev, vector<VERTEX> vertices, vector<UINT> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		this->dev = dev;

		this->setupMesh(dev);
	}

	void Draw(ID3D11DeviceContext *devcon)
	{
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;

		devcon->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		devcon->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//devcon->PSSetShaderResources(0, 1, &textures[0].texture);
		devcon->DrawIndexed(indices.size(), 0, 0);
		//devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// Set the vertex buffer to active in the input assembler so it can be rendered.
		//devcon->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

		//// Set the index buffer to active in the input assembler so it can be rendered.
		//devcon->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		//devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Close()
	{
		VertexBuffer->Release();
		IndexBuffer->Release();
	}
private:
	/*  Render data  */
	ID3D11Buffer *VertexBuffer, *IndexBuffer;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	bool setupMesh(ID3D11Device *dev)
	{

		HRESULT hr;

		D3D11_BUFFER_DESC vertexBufferDesc;

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VERTEX) * vertices.size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &vertices[0];

		hr = dev->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer);
		if (FAILED(hr))
			return false;

		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * indices.size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = &indices[0];

		hr = dev->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer);
		if (FAILED(hr))
			return false;
			//HRESULT hr;

			//D3D11_BUFFER_DESC vbd;

			//vbd.Usage = D3D11_USAGE_IMMUTABLE;
			//vbd.ByteWidth = sizeof(VERTEX) * vertices.size();
			//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			//vbd.CPUAccessFlags = 0;
			//vbd.MiscFlags = 0;

			//D3D11_SUBRESOURCE_DATA initData;
			//initData.pSysMem = &vertices[0];

			//hr = dev->CreateBuffer(&vbd, &initData, &VertexBuffer);
			//if (FAILED(hr))
			//	return false;

			//D3D11_BUFFER_DESC ibd;
			//ibd.Usage = D3D11_USAGE_IMMUTABLE;
			//ibd.ByteWidth = sizeof(UINT) * indices.size();
			//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			//ibd.CPUAccessFlags = 0;
			//ibd.MiscFlags = 0;

			//initData.pSysMem = &indices[0];

			//hr = dev->CreateBuffer(&ibd, &initData, &IndexBuffer);
			//if (FAILED(hr))
			//	return false;
		return true;
	}
};


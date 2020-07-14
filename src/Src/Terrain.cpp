#include "Terrain.h"

Terrain::Terrain() {
	vertexBuffer = indexBuffer = nullptr;
	vertexCount = indexCount = 0;
}

bool Terrain::Initialize(ID3D11Device* device) {
	return InitializeBuffers(device);
}

void Terrain::Shutdown() {
	ShutdownBuffers();
}

bool Terrain::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);

	return true;
}

int Terrain::GetIndexCount() {
	return indexCount;
}

double map(double input, double input_start, double input_end, double output_start, double output_end) {
	return (input - input_start) * (output_end - output_start) / (input_end - input_start) + output_start;
}

bool Terrain::InitializeBuffers(ID3D11Device* device) {
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, j, terrainWidth, terrainHeight, index;
	XMFLOAT4 color;
	float posX, posZ;

	terrainHeight = 256;
	terrainWidth = 256;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 8;
	indexCount = vertexCount;

	vertices = new Vertex[vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[indexCount];
	if (!indices)
		return false;

	index = 0;
	for (j = 0; j < (terrainHeight - 1); ++j) {
		for (i = 0; i < (terrainWidth - 1); ++i) {
			posX = i;
			posZ = j + 1;
			
			float mappedRed = (float)map(posX + (double)posZ, 0, 510, 0.0, 0.9);
			float mappedBlue = (float)map(posZ, 0, 255, 0.0, 0.8);
			float mappedGreen = (float)map(255 - (double)posX, 255, 0, 1, 0.1);
			color = XMFLOAT4(mappedRed, mappedBlue, mappedGreen, 1.0f);
			
			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i + 1;
			posZ = j + 1;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i + 1;
			posZ = j + 1;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i + 1;
			posZ = j;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i + 1;
			posZ = j;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i;
			posZ = j;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i;
			posZ = j;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;

			posX = i;
			posZ = j + 1;

			vertices[index].position = XMFLOAT3(posX, 0.0f, posZ);
			vertices[index].color = color;
			indices[index] = index;
			++index;
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
		return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	delete[] indices;
	vertices = nullptr;
	indices = nullptr;

	return true;
}

void Terrain::ShutdownBuffers() {
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void Terrain::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride, offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
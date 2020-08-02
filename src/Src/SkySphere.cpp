#include "SkySphere.h"

SkySphere::SkySphere() {
	model = nullptr;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

bool SkySphere::Initialize(ID3D11Device* device) {
	bool result;

	result = LoadSkySphereModel("C:/Users/Fatal/Documents/Visual Studio 2019/Projects/DX11_Terrain/src/Assets/skysphere.txt");
	if (!result)
		return false;

	result = InitializeBuffers(device);
	if (!result)
		return false;

	apexColor = XMFLOAT4(0.0f, 0.05f, 0.6f, 1.0f);
	centerColor = XMFLOAT4(0.0f, 0.5f, 0.8f, 1.0f);

	return true;
}

void SkySphere::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);
}

bool SkySphere::LoadSkySphereModel(const char* filename) {
	ifstream fin;
	char input;
	int i;

	fin.open(filename);
	if (fin.fail())
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin >> vertexCount;

	indexCount = vertexCount;
	model = new Model[vertexCount];
	if (!model)
		return false;

	fin.get(input);
	while (input != ':')
		fin.get(input);

	fin.get(input);
	fin.get(input);

	for (i = 0; i < vertexCount; ++i) {
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	fin.close();

	return true;
}

bool SkySphere::InitializeBuffers(ID3D11Device* device) {
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	vertices = new Vertex[vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[indexCount];
	if (!indices)
		return false;

	for (i = 0; i < vertexCount; ++i) {
		vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
		indices[i] = i;
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
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void SkySphere::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SkySphere::ReleaseBuffers() {
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void SkySphere::ReleaseSkySphereModel() {
	if (model) {
		delete[] model;
		model = nullptr;
	}
}

int SkySphere::GetIndexCount() {
	return indexCount;
}

XMFLOAT4 SkySphere::GetApexColor() {
	return apexColor;
}

XMFLOAT4 SkySphere::GetCenterColor() {
	return centerColor;
}

void SkySphere::Shutdown() {
	ReleaseBuffers();
	ReleaseSkySphereModel();
}
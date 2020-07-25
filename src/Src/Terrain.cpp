#include "Terrain.h"

Terrain::Terrain() {
	vertexBuffer = indexBuffer = nullptr;
	vertexCount = indexCount = 0;
	terrainFilename = nullptr;
	heightMap = nullptr;
	terrainModel = nullptr;
}

bool Terrain::Initialize(ID3D11Device* device, const char* setupFilename) {
	bool result;

	result = LoadSetupFile(setupFilename);
	if (!result)
		return false;

	result = LoadBitmapHeightMap();
	if (!result)
		return false;

	SetTerrainCoordinates();
	
	result = BuildTerrainModel();
	if (!result)
		return false;

	ShutdownHeightMap();

	result = InitializeBuffers(device);
	if (!result)
		return false;

	ShutdownTerrainModel();

	return true;
}

void Terrain::Shutdown() {
	ShutdownBuffers();

	ShutdownTerrainModel();

	ShutdownHeightMap();
}

bool Terrain::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);

	return true;
}

int Terrain::GetIndexCount() {
	return indexCount;
}

bool Terrain::LoadSetupFile(const char* fileName) {
	int stringLength;
	ifstream fin;
	char input[256];
	char* splitLine;
	char* nextToken = { 0 };
	const char* delim = ":\n";

	stringLength = 256;
	terrainFilename = new char[stringLength];
	if (!terrainFilename)
		return false;

	fin.open(fileName);
	if (fin.fail())
		return false;

	fin.getline(input, stringLength);
	splitLine = strtok_s(input, delim, &nextToken);
	strcpy_s(terrainFilename, 256, &nextToken[1]);

	fin.getline(input, stringLength);

	splitLine = strtok_s(input, delim, &nextToken);
	terrainHeight = atoi(&nextToken[1]);

	fin.getline(input, stringLength);
	splitLine = strtok_s(input, delim, &nextToken);
	terrainWidth = atoi(&nextToken[1]);

	fin.getline(input, stringLength);
	splitLine = strtok_s(input, delim, &nextToken);
	heightScale = atoi(&nextToken[1]);

	fin.close();

	return true;
}

bool Terrain::LoadBitmapHeightMap() {
	int error, imageSize, i, j, k, index;
	FILE* fp;
	unsigned long long count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapImage;
	unsigned char height;

	heightMap = new HeightMap[terrainWidth * terrainHeight];
	if (!heightMap)
		return false;

	error = fopen_s(&fp, terrainFilename, "rb");
	if (error != 0) {
		printf("%s\n", terrainFilename);
		printf("%d\n", error);
		return false;
	}
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	if (count != 1)
		return false;

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if (count != 1)
		return false;

	if (bitmapInfoHeader.biHeight != terrainHeight || bitmapInfoHeader.biWidth != terrainWidth)
		return false;

	imageSize = terrainHeight * ((terrainWidth * 3) + 1);

	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
		return false;

	fseek(fp, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, fp);


	error = fclose(fp);
	if (error != 0)
		return false;

	k = 0;
	for (j = 0; j < terrainHeight; ++j) {
		for (i = 0; i < terrainWidth; ++i) {
			index = (terrainWidth * (terrainHeight - 1 - j)) + i;
			height = bitmapImage[k];

			heightMap[index].y = (float)height;

			k += 3;
		}

		++k;
	}

	delete[] bitmapImage;
	bitmapImage = nullptr;

	delete[] terrainFilename;
	terrainFilename = nullptr;

	return true;
}

void Terrain::ShutdownHeightMap() {
	if (heightMap) {
		delete[] heightMap;
		heightMap = nullptr;
	}
}

void Terrain::SetTerrainCoordinates() {
	int i, j, index;

	for (j = 0; j < terrainHeight; ++j) {
		for (i = 0; i < terrainWidth; ++i) {
			index = (terrainWidth * j) + i;

			heightMap[index].x = (float)i;
			heightMap[index].z = -(float)j;
			heightMap[index].z += (float)(terrainHeight - 1);

			heightMap[index].y /= heightScale;
		}
	}
}

bool Terrain::BuildTerrainModel() {
	int i, j, index, index1, index2, index3, index4;

	vertexCount = (terrainHeight - 1) * (terrainWidth - 1) * 6;
	terrainModel = new Model[vertexCount];
	if (!terrainModel)
		return false;

	index = 0;
	for (j = 0; j < terrainHeight - 1; ++j) {
		for (i = 0; i < terrainWidth - 1; ++i) {
			index1 = (terrainWidth * j) + i;
			index2 = (terrainWidth * j) + (i + 1);
			index3 = (terrainWidth * (j + 1)) + i;
			index4 = (terrainWidth * (j + 1)) + (i + 1);

			// Triangle 1 - Upper left.
			terrainModel[index].x = heightMap[index1].x;
			terrainModel[index].y = heightMap[index1].y;
			terrainModel[index].z = heightMap[index1].z;
			index++;

			// Triangle 1 - Upper right.
			terrainModel[index].x = heightMap[index2].x;
			terrainModel[index].y = heightMap[index2].y;
			terrainModel[index].z = heightMap[index2].z;
			index++;

			// Triangle 1 - Bottom left.
			terrainModel[index].x = heightMap[index3].x;
			terrainModel[index].y = heightMap[index3].y;
			terrainModel[index].z = heightMap[index3].z;
			index++;

			// Triangle 2 - Bottom left.
			terrainModel[index].x = heightMap[index3].x;
			terrainModel[index].y = heightMap[index3].y;
			terrainModel[index].z = heightMap[index3].z;
			index++;

			// Triangle 2 - Upper right.
			terrainModel[index].x = heightMap[index2].x;
			terrainModel[index].y = heightMap[index2].y;
			terrainModel[index].z = heightMap[index2].z;
			index++;

			// Triangle 2 - Bottom right.
			terrainModel[index].x = heightMap[index4].x;
			terrainModel[index].y = heightMap[index4].y;
			terrainModel[index].z = heightMap[index4].z;

			index++;
		}
	}

	return true;
}

void Terrain::ShutdownTerrainModel() {
	if (terrainModel) {
		delete[] terrainModel;
		terrainModel = nullptr;
	}
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
	int i, terrainWidth, terrainHeight, index;
	XMFLOAT4 color;
	float posX, posZ;

	terrainHeight = 256;
	terrainWidth = 256;

	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	indexCount = vertexCount;

	vertices = new Vertex[vertexCount];
	if (!vertices)
		return false;

	indices = new unsigned long[indexCount];
	if (!indices)
		return false;

	for (i = 0; i < vertexCount; ++i) {
		float r, g, b;
		r = map(terrainModel[i].y, 0, 80, 0, 1);
		g = map(terrainModel[i].y, 0, 80, 0, 1);
		b = map(terrainModel[i].y, 0, 20, 0.5, 0.1);
		vertices[i].position = XMFLOAT3(terrainModel[i].x, terrainModel[i].y, terrainModel[i].z);
		vertices[i].color = XMFLOAT4(r, 0, b, 1);
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
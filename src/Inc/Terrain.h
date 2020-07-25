#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <stdio.h>
#include <wingdi.h>
#include <string.h>
#include <string>

using namespace DirectX;
using namespace std;

class Terrain {
public:
	Terrain();

	bool Initialize(ID3D11Device*, const char*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadSetupFile(const char*);
	bool LoadBitmapHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	int terrainHeight, terrainWidth;
	float heightScale;
	char* terrainFilename;

	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	struct HeightMap {
		float x, y, z;
	};

	HeightMap* heightMap;

	struct Model {
		float x, y, z;
	};

	Model* terrainModel;
};
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class SkySphere {
public:
	SkySphere();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	XMFLOAT4 GetApexColor();
	XMFLOAT4 GetCenterColor();

private:
	int vertexCount, indexCount;
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	XMFLOAT4 apexColor, centerColor;

	bool LoadSkySphereModel(const char*);
	void ReleaseSkySphereModel();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	struct Model {
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	Model* model;

	struct Vertex {
		XMFLOAT3 position;
	};
};
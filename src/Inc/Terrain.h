#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Terrain {
public:
	Terrain();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;

	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
};
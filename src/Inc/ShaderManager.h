#pragma once

#include "d3d.h"
#include "ColorShader.h"
#include "SkySphereShader.h"

class ShaderManager {
public:
	ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderColorShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);
	bool RenderSkySphereShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT4);

private:
	ColorShader* colorShader;
	SkySphereShader* skySphereShader;
};
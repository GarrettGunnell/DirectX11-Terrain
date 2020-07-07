#pragma once

#include "d3d.h"
#include "ColorShader.h"

class ShaderManager {
public:
	ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderColorShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	ColorShader* colorShader;
};
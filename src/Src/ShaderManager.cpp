#include "ShaderManager.h"

ShaderManager::ShaderManager() {
	colorShader = nullptr;
	skySphereShader = nullptr;
}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd) {
	bool result;

	colorShader = new ColorShader();
	if (!colorShader)
		return false;

	result = colorShader->Initialize(device, hwnd);
	if (!result)
		return false;

	skySphereShader = new SkySphereShader();
	if (!skySphereShader)
		return false;

	result = skySphereShader->Initialize(device, hwnd);
	if (!result)
		return false;

	return true;
}

void ShaderManager::Shutdown() {
	if (colorShader) {
		colorShader->Shutdown();
		delete colorShader;
		colorShader = nullptr;
	}

	if (skySphereShader) {
		skySphereShader->Shutdown();
		delete skySphereShader;
		skySphereShader = nullptr;
	}
}

bool ShaderManager::RenderColorShader(ID3D11DeviceContext* device, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) {
	return colorShader->Render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix);
}

bool ShaderManager::RenderSkySphereShader(ID3D11DeviceContext* device, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor) {
	return skySphereShader->Render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
}
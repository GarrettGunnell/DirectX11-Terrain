#pragma once

#include "Input.h"
#include "d3d.h"
#include "ShaderManager.h""
#include "Timer.h"
#include "Fps.h"
#include "Zone.h"

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Application {
public:
	Application();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	Input* input;
	D3D* direct3D;
	ShaderManager* shaderManager;
	Timer* timer;
	Fps* fps;
	Zone* zone;
};
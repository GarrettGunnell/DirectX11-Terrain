#pragma once

#include "d3d.h"
#include "Input.h"
#include "ShaderManager.h"
#include "Timer.h"
#include "UserInterface.h"
#include "Camera.h"
#include "Position.h"
#include "Terrain.h"

class Zone {
public:
	Zone();

	bool Initialize(D3D*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3D*, Input*, ShaderManager*, float, int);

private:
	void HandleMovementInput(Input*, float);
	bool Render(D3D*, ShaderManager*);

	UserInterface* userInterface;
	Camera* camera;
	Position* position;
	Terrain* terrain;
	bool displayUI;
};
#include "Zone.h"

Zone::Zone() {
	userInterface = nullptr;
	camera = nullptr;
	position = nullptr;
	terrain = nullptr;
	displayUI = false;
}

bool Zone::Initialize(D3D* direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth) {
	bool result;

	userInterface = new UserInterface();
	if (!userInterface)
		return false;

	result = userInterface->Initialize(direct3D, screenHeight, screenWidth);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the user interface", L"Error", MB_OK);
		return false;
	}

	camera = new Camera();
	if (!camera)
		return false;

	camera->SetPosition(0.0f, 0.0f, -10.0f);
	camera->Render();
	camera->RenderBaseViewMatrix();

	position = new Position();
	if (!position)
		return false;

	position->SetPosition(128.0f, 5.0f, -10.0f);
	position->SetRotation(0.0f, 0.0f, 0.0f);

	terrain = new Terrain();
	if (!terrain)
		return false;

	result = terrain->Initialize(direct3D->GetDevice());
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the terrain", L"Error", MB_OK);
		return false;
	}

	displayUI = true;

	return true;
}

void Zone::Shutdown() {
	if (terrain) {
		terrain->Shutdown();
		delete terrain;
		terrain = nullptr;
	}

	if (position) {
		delete position;
		position = nullptr;
	}

	if (camera) {
		delete camera;
		camera = nullptr;
	}

	if (userInterface) {
		userInterface->Shutdown();
		delete userInterface;
		userInterface = nullptr;
	}
}

bool Zone::Frame(D3D* direct3D, Input* input, ShaderManager* shaderManager, float frameTime, int fps) {
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	HandleMovementInput(input, frameTime);

	position->GetPosition(posX, posY, posZ);
	position->GetRotation(rotX, rotY, rotZ);

	result = userInterface->Frame(direct3D->GetDeviceContext(), fps, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
		return false;

	result = Render(direct3D, shaderManager);
	if (!result)
		return false;

	return true;
}

void Zone::HandleMovementInput(Input* input, float frameTime) {
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	position->SetFrameTime(frameTime);

	keyDown = input->IsLeftPressed();
	position->TurnLeft(keyDown);

	keyDown = input->IsRightPressed();
	position->TurnRight(keyDown);

	keyDown = input->IsUpPressed();
	position->MoveForward(keyDown);

	keyDown = input->IsDownPressed();
	position->MoveBackward(keyDown);

	keyDown = input->IsAPressed();
	position->MoveUpward(keyDown);

	keyDown = input->IsZPressed();
	position->MoveDownward(keyDown);

	keyDown = input->IsPgUpPressed();
	position->LookUpward(keyDown);

	keyDown = input->IsPgDownPressed();
	position->LookDownward(keyDown);

	position->GetPosition(posX, posY, posZ);
	position->GetRotation(rotX, rotY, rotZ);

	camera->SetPosition(posX, posY, posZ);
	camera->SetRotation(rotX, rotY, rotZ);

	if (input->IsF1Toggled()) {
		displayUI = !displayUI;
	}
}

bool Zone::Render(D3D* direct3D, ShaderManager* shaderManager) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;

	camera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	camera->GetBaseViewMatrix(baseViewMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	terrain->Render(direct3D->GetDeviceContext());
	result = shaderManager->RenderColorShader(direct3D->GetDeviceContext(), terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	if (displayUI) {
		result = userInterface->Render(direct3D, shaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if (!result)
			return false;
	}

	direct3D->EndScene();

	return true;
}
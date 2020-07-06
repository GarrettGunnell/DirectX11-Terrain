#include "Zone.h"

Zone::Zone() {
	camera = nullptr;
	position = nullptr;
	terrain = nullptr;
}

bool Zone::Initialize(D3D* direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth) {
	bool result;

	camera = new Camera();
	if (!camera)
		return false;

	camera->SetPosition(0.0f, 0.0f, -10.0f);
	camera->Render();

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
}

bool Zone::Frame(D3D* direct3D, Input* input, ShaderManager* shaderManager, float frameTime) {
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	HandleMovementInput(input, frameTime);

	position->GetPosition(posX, posY, posZ);
	position->GetRotation(rotX, rotY, rotZ);

	result = Render(direct3D, shaderManager);
	if (!result)
		return false;

	return true;
}

void Zone::HandleMovementInput(Input* input, float frameTime) {
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	position->SetFrameTime(frameTime);

	keyDown = input->IsKeyDown(0x41); //A
	position->TurnLeft(keyDown);

	keyDown = input->IsKeyDown(0x44); //D
	position->TurnRight(keyDown);

	keyDown = input->IsKeyDown(0x57); //W
	position->MoveForward(keyDown);

	keyDown = input->IsKeyDown(0x53); //S
	position->MoveBackward(keyDown);

	keyDown = input->IsKeyDown(VK_SPACE);
	position->MoveUpward(keyDown);

	keyDown = input->IsKeyDown(VK_CONTROL);
	position->MoveDownward(keyDown);

	keyDown = input->IsKeyDown(0x52); //R
	position->LookUpward(keyDown);

	keyDown = input->IsKeyDown(0x46); //F
	position->LookDownward(keyDown);

	position->GetPosition(posX, posY, posZ);
	position->GetRotation(rotX, rotY, rotZ);

	camera->SetPosition(posX, posY, posZ);
	camera->SetRotation(rotX, rotY, rotZ);
}

bool Zone::Render(D3D* direct3D, ShaderManager* shaderManager) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	camera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	terrain->Render(direct3D->GetDeviceContext());
	result = shaderManager->RenderColorShader(direct3D->GetDeviceContext(), terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	direct3D->EndScene();

	return true;
}
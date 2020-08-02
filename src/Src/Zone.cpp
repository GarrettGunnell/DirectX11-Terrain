#include "Zone.h"

Zone::Zone() {
	camera = nullptr;
	position = nullptr;
	terrain = nullptr;
	skySphere = nullptr;
	wireFrame = true;
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

	position->SetPosition(128.0f, 10.0f, -10.0f);
	position->SetRotation(0.0f, 0.0f, 0.0f);

	skySphere = new SkySphere();
	if (!skySphere)
		return false;

	result = skySphere->Initialize(direct3D->GetDevice());
	if (!result) {
		MessageBox(hwnd, L"Could not initialize sky sphere", L"Error", MB_OK);
		return false;
	}

	terrain = new Terrain();
	if (!terrain)
		return false;

	result = terrain->Initialize(direct3D->GetDevice(), "C:/Users/Fatal/Documents/Visual Studio 2019/Projects/DX11_Terrain/src/Assets/setup.txt");
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the terrain", L"Error", MB_OK);
		return false;
	}

	wireFrame = true;

	return true;
}

void Zone::Shutdown() {
	if (terrain) {
		terrain->Shutdown();
		delete terrain;
		terrain = nullptr;
	}

	if (skySphere) {
		skySphere->Shutdown();
		delete skySphere;
		skySphere = nullptr;
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

	keyDown = input->IsKeyPressed(DIK_Q);
	position->StrafeLeft(keyDown);

	keyDown = input->IsKeyPressed(DIK_E);
	position->StrafeRight(keyDown);

	keyDown = input->IsKeyPressed(DIK_A);
	position->TurnLeft(keyDown);

	keyDown = input->IsKeyPressed(DIK_D);
	position->TurnRight(keyDown);

	keyDown = input->IsKeyPressed(DIK_W);
	position->MoveForward(keyDown);

	keyDown = input->IsKeyPressed(DIK_S);
	position->MoveBackward(keyDown);

	keyDown = input->IsKeyPressed(DIK_SPACE);
	position->MoveUpward(keyDown);

	keyDown = input->IsKeyPressed(DIK_LCONTROL);
	position->MoveDownward(keyDown);

	keyDown = input->IsKeyPressed(DIK_R);
	position->LookUpward(keyDown);

	keyDown = input->IsKeyPressed(DIK_F);
	position->LookDownward(keyDown);

	keyDown = input->IsKeyPressed(DIK_F1);
	if (keyDown) {
		wireFrame = !wireFrame;
	}

	position->GetPosition(posX, posY, posZ);
	position->GetRotation(rotX, rotY, rotZ);

	camera->SetPosition(posX, posY, posZ);
	camera->SetRotation(rotX, rotY, rotZ);
}

bool Zone::Render(D3D* direct3D, ShaderManager* shaderManager) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	XMFLOAT3 cameraPosition;

	camera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	cameraPosition = camera->GetPosition();

	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	/* Sky Sphere */
	direct3D->TurnOffCulling();
	direct3D->TurnZBufferOff();

	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	
	skySphere->Render(direct3D->GetDeviceContext());
	result = shaderManager->RenderSkySphereShader(direct3D->GetDeviceContext(), skySphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		skySphere->GetApexColor(), skySphere->GetCenterColor());
	if (!result)
		return false;

	/* Terrain */

	direct3D->GetWorldMatrix(worldMatrix);

	direct3D->TurnZBufferOn();
	direct3D->TurnOnCulling();

	if (wireFrame) {
		direct3D->EnableWireframe();
	}

	terrain->Render(direct3D->GetDeviceContext());
	result = shaderManager->RenderColorShader(direct3D->GetDeviceContext(), terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	if (wireFrame) {
		direct3D->DisableWireframe();
	}

	direct3D->EndScene();

	return true;
}
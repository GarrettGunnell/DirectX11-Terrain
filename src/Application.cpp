#include "Application.h"

Application::Application() {
	input = nullptr;
	direct3D = nullptr;
	timer = nullptr;
	shaderManager = nullptr;
	zone = nullptr;
}

bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight) {
	bool result;

	input = new Input();
	if (!input)
		return false;

	direct3D = new D3D();
	if (!direct3D)
		return false;

	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct 3D", L"Error", MB_OK);
		return false;
	}

	shaderManager = new ShaderManager();
	if (!shaderManager)
		return false;

	result = shaderManager->Initialize(direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the shader manager", L"Error", MB_OK);
		return false;
	}

	timer = new Timer();
	if (!timer)
		return false;

	result = timer->Initialize();
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the timer", L"Error", MB_OK);
		return false;
	}

	zone = new Zone();
	if (!zone)
		return false;

	result = zone->Initialize(direct3D, hwnd, screenWidth, screenHeight, SCREEN_DEPTH);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the zone object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Application::Shutdown() {
	if (zone) {
		zone->Shutdown();
		delete zone;
		zone = nullptr;
	}

	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (shaderManager) {
		shaderManager->Shutdown();
		delete shaderManager;
		shaderManager = nullptr;
	}

	if (direct3D) {
		direct3D->Shutdown();
		delete direct3D;
		direct3D = nullptr;
	}

	if (input) {
		delete input;
		input = nullptr;
	}
}

bool Application::Frame() {
	bool result;

	timer->Frame();

	if (input->IsKeyDown(VK_ESCAPE))
		return false;

	result = zone->Frame(direct3D, input, shaderManager, timer->GetTime());
	if (!result)
		return false;

	return result;
}
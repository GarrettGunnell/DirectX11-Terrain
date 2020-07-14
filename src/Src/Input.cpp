#include "Input.h"

Input::Input() {
	directInput = nullptr;
	keyboard = nullptr;
}


bool Input::Initialize(HINSTANCE hinstance, HWND hwnd) {
	HRESULT result;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(result))
		return false;

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(result))
		return false;

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	result = keyboard->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

void Input::Shutdown() {
	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = nullptr;
	}

	if (directInput) {
		directInput->Release();
		directInput = nullptr;
	}
}

bool Input::Frame() {
	bool result;

	result = ReadKeyboard();
	if (!result)
		return false;

	ProcessInput();

	return true;
}

bool Input::ReadKeyboard() {
	HRESULT result;

	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result)) {
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			keyboard->Acquire();
		else
			return false;
	}

	return true;
}

void Input::ProcessInput() {

}

bool Input::IsKeyPressed(unsigned char key) {
	if (keyboardState[key] & 0x80)
		return true;

	return false;
}
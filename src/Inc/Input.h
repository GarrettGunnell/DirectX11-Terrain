#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input {
public:
	Input();

	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();
	
	bool IsKeyPressed(unsigned char);

private:
	bool ReadKeyboard();
	void ProcessInput();

	IDirectInput8* directInput;
	IDirectInputDevice8* keyboard;

	unsigned char keyboardState[256];
};

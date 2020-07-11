#include "System.h"
#include <io.h>
#include <fcntl.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	System* system;
	bool result;

	system = new System;
	if(!system)
		return 0;

	result = system->Initialize();
	if (!result)
		return 0;

	system->Run();
	system->Shutdown();

	delete system;
	system = nullptr;
	return 0;
}

/* 
Normally this app would use the Windows linker since this is a graphical application but to easily enable printing to a console
I am using this dummy main function with the normal visual studio console linker so that I don't have to do complicated console window allocation
*/
int main() {
	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}
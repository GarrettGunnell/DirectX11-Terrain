#include "Input.h"


void Input::Initialize() {
	int i;
	
	for(i = 0; i < 256; i++) {
		keys[i] = false;
	}
}


void Input::KeyDown(unsigned int input) {
	keys[input] = true;
}


void Input::KeyUp(unsigned int input) {
	keys[input] = false;
}


bool Input::IsKeyDown(unsigned int key) {
	return keys[key];
}
#include "Gamepad.h"

#pragma comment(lib, "Xinput.lib")

Gamepad::Gamepad() {}

Gamepad::Gamepad(int index)
{
	controllerN = index - 1;		// Set gamepad index, -1 is so we start from 0 (designers dont get counting from 0)
}

XINPUT_STATE Gamepad::GetState()
{
	XINPUT_STATE tempState;

	XInputGetState(controllerN, &tempState);

	return tempState;
}

int Gamepad::GetIndex()
{
	return 0;
}

bool Gamepad::Connected()
{
	return false;
}


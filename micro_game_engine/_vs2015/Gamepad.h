#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <windows.h>
#include <Xinput.h>

class Gamepad
{
public:
	Gamepad();
	Gamepad(int index);

	XINPUT_STATE GetState();		// Gets the state
	int GetIndex();					// Gets the index
	bool Connected();				// Returns true if the contoller is connected

private:

	XINPUT_STATE controllerState;	// Current contoller state
	int controllerN;				// Contoller number
};

#endif
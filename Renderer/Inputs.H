#pragma once

#include <Windows.h>


static bool Z_KEY_STATE = false;
static bool Q_KEY_STATE = false;
static bool S_KEY_STATE = false;
static bool D_KEY_STATE = false;
static bool A_KEY_STATE = false;
static bool E_KEY_STATE = false;
static bool ESC_KEY_STATE = false;
static bool SPACE_KEY_STATE = false;


void updateInputs() {

	Z_KEY_STATE=GetKeyState(0x5A) & 0x8000;
	Q_KEY_STATE=GetKeyState(0x51) & 0x8000;
	D_KEY_STATE=GetKeyState(0x44) & 0x8000;
	S_KEY_STATE=GetKeyState(0x53) & 0x8000;
	E_KEY_STATE=GetKeyState(0x45) & 0x8000;
	A_KEY_STATE=GetKeyState(0x41) & 0x8000;
	SPACE_KEY_STATE=GetKeyState(0x20) & 0x8000;
	ESC_KEY_STATE = GetKeyState(VK_ESCAPE) & 0x8000;

}

void interpreteInputs() {

	if (Z_KEY_STATE) {}
	if (Q_KEY_STATE) {}
	if (S_KEY_STATE) {}
	if (D_KEY_STATE) {}

}
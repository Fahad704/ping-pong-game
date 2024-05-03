#pragma once
enum Buttons
{
	BUTTON_UP,
	BUTTON_DOWN, 
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_W,
	BUTTON_S,
	BUTTON_ENTER,
	BUTTON_ESC,
	BUTTON_Q,
	BUTTON_COUNT,
};
struct Button_state {
	bool is_down;
	bool changed;
};
struct Input {
	Button_state buttons[BUTTON_COUNT];
};
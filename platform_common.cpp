struct Button_state{
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_W,
	BUTTON_S,
	BUTTON_A,
	BUTTON_D,
	BUTTON_ESC,
	BUTTON_ENTER,
	BUTTON_COUNT,//should be last item
};

struct Input{
	Button_state buttons[BUTTON_COUNT];
};
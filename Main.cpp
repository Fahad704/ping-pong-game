#pragma once
#include "Utility.cpp"
#include <Windows.h>
#include "resource.h"

global_variable bool running = true;
struct Render_State {
	int height;
	int width;
	void* memory;
	BITMAPINFO bitmapinfo;
};
global_variable Render_State render_state;



#include "platform_common.cpp" 
#include "Renderer.cpp"
#include "game.cpp"

LRESULT window_callback(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
		if(render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
	}break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;

		int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);

		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		render_state.bitmapinfo.bmiHeader.biSize          = sizeof(render_state.bitmapinfo.bmiHeader);
		render_state.bitmapinfo.bmiHeader.biWidth         = render_state.width;
		render_state.bitmapinfo.bmiHeader.biHeight        = render_state.height;
		render_state.bitmapinfo.bmiHeader.biPlanes        = 1;
		render_state.bitmapinfo.bmiHeader.biBitCount      = 32;
		render_state.bitmapinfo.bmiHeader.biCompression   = BI_RGB;

	}break;
	default: {
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	}
	return result;
}
int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd) {

	//Create Window Class
	WNDCLASS window_class = {};
	window_class.lpfnWndProc = window_callback;
	window_class.lpszClassName = L"Window Class";
	window_class.style = CS_HREDRAW | CS_VREDRAW;

	//Register Window Class 
	RegisterClass(&window_class);

	//Create Window 
	HWND window = CreateWindow(window_class.lpszClassName , L"Ping Pong!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(window, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	Input input = {};

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (running) {
		
		//Message loop
		MSG message;
		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}


		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)){
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				u32 vk_code = (u32)message.wParam;
				bool is_down = ((message.lParam & (1 << 31)) == 0);
#define process_messages(b,vk)\
 case vk:{\
input.buttons[b].changed = (is_down !=input.buttons[b].is_down);\
input.buttons[b].is_down = is_down;\
}break;
				switch (vk_code) {
					process_messages(BUTTON_UP, VK_UP);
					process_messages(BUTTON_DOWN,VK_DOWN);
					process_messages(BUTTON_LEFT, VK_LEFT);
					process_messages(BUTTON_RIGHT, VK_RIGHT);
					process_messages(BUTTON_W, 'W');
					process_messages(BUTTON_S,'S');
					process_messages(BUTTON_ENTER, VK_RETURN);
					process_messages(BUTTON_ESC, VK_ESCAPE);
					process_messages(BUTTON_Q, 'Q');
				}

			}
			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			}
		}
		
		//Game Loop
		simulate_game(&input,delta_time);

		//Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart)/performance_frequency;
		frame_begin_time = frame_end_time;
	}
	

}
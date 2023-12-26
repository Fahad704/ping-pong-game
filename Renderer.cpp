internal void clear_screen(u32 color) {
	unsigned* pixel = (u32*)render_state.memory;
	for (int x = 0; x < render_state.width; x++) {
		for (int y = 0; y < render_state.height; y++) {
			*pixel++ = color;
		}
	}
}

internal void draw_rect_in_pixel(int x0,int y0,int x1,int y1,u32 color) {
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height); 
	for (int y = y0; y < y1; y++) {
	u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	x *= render_state.height* render_scale;
	y *= render_state.height*render_scale;
	half_size_x *= render_state.height*render_scale;
	half_size_y *= render_state.height*render_scale;

	x += render_state.width/2.f;
	y += render_state.height/2.f;
	

	//change to pixels
	int x0 = (int)(x - half_size_x);
	int x1 = (int)(x + half_size_x);
	int y0 = (int)(y - half_size_y);
	int y1 = (int)(y + half_size_y);
	draw_rect_in_pixel(x0,y0,x1,y1, color);
}
const char* letters[][7] = {
		" 00",
		"0  0",
		"0  0",
		"0000",
		"0  0",
		"0  0",
		"0  0",

		"000",
		"0  0",
		"0  0",
		"000",
		"0  0",
		"0  0",
		"000",

		" 000",
		"0",
		"0",
		"0",
		"0",
		"0",
		" 000",

		"000",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"000",

		"0000",
		"0",
		"0",
		"0000",
		"0",
		"0",
		"0000",

		"0000",
		"0",
		"0",
		"000",
		"0",
		"0",
		"0",

		" 000",
		"0",
		"0",
		"0 00",
		"0  0",
		"0  0",
		" 00",

		"0  0",
		"0  0",
		"0  0",
		"0000",
		"0  0",
		"0  0",
		"0  0",

		" 000",
		"  0",
		"  0",
		"  0",
		"  0",
		"  0",
		" 000",

		"0000",
		"  0",
		"  0",
		"  0",
		"  0",
		"  0",
		"000",

		"0  0",
		"0 0",
		"00",
		"0",
		"00",
		"0 0",
		"0  0",

		"0",
		"0",
		"0",
		"0",
		"0",
		"0",
		"0000",

		"0   0",
		"00 00",
		"0 0 0",
		"0   0",
		"0   0",
		"0   0",
		"0   0",

		"0  0",
		"00 0",
		"0 00",
		"0  0",
		"0  0",
		"0  0",
		"0  0",

		"0000",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"0000",

		" 00",
		"0  0",
		"0  0",
		"000",
		"0",
		"0",
		"0",

		"0000",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"0000",
		"    0",

		" 00",
		"0  0",
		"0  0",
		"000",
		"00",
		"0 0",
		"0  0",

		" 000",
		"0   ",
		"0   ",
		"000",
		"   0",
		"   0",
		"000",

		" 000",
		"  0",
		"  0",
		"  0",
		"  0",
		"  0",
		"  0",

		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		"0  0",
		" 00",


		"0  0",
		"0  0",
		"0  0",
		"0  0",
		" 00",
		" 00",
		" 00",

		"0 0 0",
		"0 0 0",
		"0 0 0",
		"0 0 0",
		"0 0 0",
		"0 0 0",
		" 000 ",
	
		"0  0",
		"0  0",
		" 00",
		" 00",
		"0  0",
		"0  0",
		"0  0",

		"0   0",
		"0   0",
		" 0 0",
		"  0",
		"  0",
		"  0",
		"  0",

		"0000",
		"   0",
		"  0",
		" 0",
		"0",
		"0",
		"0000",
};
internal void draw_text(const char* text,float x, float y, float size, u32 color) {
	float half_size = size * 0.5f;
	float og_y = y;
	

	while (*text) {
		const char** letter_a = letters[*text - 'A'];
		float og_x = x;
		for (int i = 0; i < 7; i++) {
			if (*text == ' ') {
				continue;
			}
			const char* row = letter_a[i];

			while (*row) {
				if (*row == '0') {
					draw_rect(x, y, half_size, half_size, color);
				}
				x += size;
				row++;
				if (row == nullptr) {
					break;
				}
			}
			y -= size;
			x = og_x;
		}
		text++;
		if (text == nullptr) {
			break;
		}
		x += size * 6.f;
		y = og_y;
	}
	
}

internal void draw_number(int number, float x, float y, float size, u32 color) {
	
	float half_size = size * 0.5f;
	bool drew_num = false;
	
	while ( number || !drew_num ) {
		drew_num = true;
		int digit = number % 10;
		number /= 10;
		switch (digit) {
		case 0: {
			draw_rect(x - size, y, half_size, size * 2.5f, color);
			draw_rect(x + size, y, half_size, size * 2.5f, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			x -= size * 4.f;
		}break;
		case 1: {
			draw_rect(x, y, half_size, size * 2.8f, color);
			x -= size * 4.f;

		}break;
		case 2: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			x -= size * 4.f;
		}break;
		case 3: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y, half_size, size * 2.5f, color);
			x -= size * 4.f;
		}break;
		case 4: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y, half_size, size * 2.5f, color);
			draw_rect(x - size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			x -= size * 4.f;
		}break;
		case 5: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			x -= size * 4.f;
		}break;
		case 6: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			x -= size * 4.f;
		}break;
		case 7: {
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y, half_size, size * 2.5f, color);
			x -= size * 4.f;
		}break;
		case 8: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x + size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			x -= size * 4.f;

		}break;
		case 9: {
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x, y + size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.5f, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y - half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x - size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			draw_rect(x + size, y + half_size * 2.5f, half_size, half_size * 2.5f, color);
			x -= size * 4.f;
		}break;
		}
	}
}

internal void draw_outline_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	draw_rect(x - half_size_x, y, 0.01f, half_size_y, color);
	draw_rect(x + half_size_x, y, 0.01f, half_size_y, color);
	draw_rect(x, y + half_size_y,half_size_x, 0.01f, color);
	draw_rect(x, y - half_size_y, half_size_x, 0.01f, color);
}
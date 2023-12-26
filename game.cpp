#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;//player Position and Velocity
float arena_half_size_x = 90,arena_half_size_y = 45;
float player_half_size_x=1, player_half_size_y=8;
float ball_p_x, ball_p_y, ball_dp_x=100, ball_dp_y,ball_half_size=1;

int player_1_score, player_2_score;

enum Gamemode{
	GM_MENU,
	GM_GAMEPLAY,
};
enum Controls {
	CT_SINGLE_PLAYER,
	CT_MULTIPLAYER,
	CT_AI_VS_AI,
};
Controls controls;
Gamemode current_gamemode;
int hot_button;
internal void simulate_player(float* p, float* dp, float *ddp, float dt) {
	*ddp -= *dp * 10.f;//Friction
	*p = *p + *dp * dt + ((*ddp * dt * dt) * 0.5f);//Speed
	*dp = *dp + *ddp * dt;//Velocity
	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}
internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float  p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}
internal void automate_player(float* player_p,float *player_ddp) {
	*player_ddp = (ball_p_y - *player_p) * 75;
	if (*player_ddp > 1300) *player_ddp = 1300;
	if (*player_ddp < -1300) *player_ddp = -1300;

}
internal void simulate_game(Input* input,float dt) {
	clear_screen(0xff1100);
	draw_rect(0,0, arena_half_size_x, arena_half_size_y,0x000000);//Arena Square

	if (current_gamemode == GM_GAMEPLAY) {
		float player_1_ddp = 0.f; //Acceleration
		float player_2_ddp = 0.f;
		if (is_down(BUTTON_UP)) player_2_ddp += 1000;
		if (is_down(BUTTON_DOWN)) player_2_ddp -= 1000;
		if (is_down(BUTTON_RIGHT)) player_2_ddp += 1000;
		if (is_down(BUTTON_LEFT)) player_2_ddp -= 1000;
		if ((pressed(BUTTON_ESC))) {
			current_gamemode = GM_MENU;
			player_1_p = 0.f;
			player_2_p = 0.f;
			ball_p_x = 0.f, ball_p_y = 0.f;
			ball_dp_y = 0.f;
			player_1_dp = 0.f;
			player_2_dp = 0.f;
			player_1_ddp = 0.f;
			player_2_ddp = 0.f;
			player_1_score = 0;
			player_2_score = 0;
		}

		if (is_down(BUTTON_W)) player_1_ddp += 1000;
		if (is_down(BUTTON_S)) player_1_ddp -= 1000;
		if (is_down(BUTTON_D)) player_1_ddp += 1000;
		if (is_down(BUTTON_A)) player_1_ddp -= 1000;


		if (controls == CT_SINGLE_PLAYER) {
			automate_player(&player_1_p,&player_1_ddp);
		}
		else if (controls == CT_AI_VS_AI) {
			automate_player(&player_1_p, &player_1_ddp);
			automate_player(&player_2_p,&player_2_ddp);
		}

		simulate_player(&player_1_p, &player_1_dp, &player_1_ddp, dt);
		simulate_player(&player_2_p, &player_2_dp, &player_2_ddp, dt);

		//Simulate Ball
		{
			ball_p_x += ball_dp_x * dt;//Ball Speed
			ball_p_y += ball_dp_y * dt;

			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 85, player_2_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = 85 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = player_2_dp * 0.75f;
				ball_dp_y += (ball_p_y - player_2_p+1);
			}
			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -85, player_1_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = -85 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = player_1_dp * 0.75f;
				ball_dp_y += (ball_p_y - player_1_p+1);
			}

			//Ball Collsion with arena borders
			if (ball_p_y + ball_half_size > arena_half_size_y) {
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_y < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}

			if (ball_p_x + ball_half_size > arena_half_size_x) {
				ball_p_x = 0;
				ball_p_y = 0;
				ball_dp_y = 0;
				ball_dp_x *= -1;
				player_1_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_p_x = 0;
				ball_p_y = 0;
				ball_dp_y = 0;
				ball_dp_x *= -1;
				player_2_score++;
			}
		}

		//Render
		draw_number(player_1_score, -10, 40, 1, 0xaaaaaa);//Player 1 score
		draw_number(player_2_score, 10, 40, 1, 0xaaaaaa);//Player 2 score0
		draw_rect(ball_p_x, ball_p_y, 1, 1, 0x5cff6b);//Ball
		draw_rect(85, player_2_p, 1, 8, 0xffdddd);//Player 2
		draw_rect(-85, player_1_p, 1, 8, 0xffdddd);//AI or Player 1
	}
	else if (current_gamemode == GM_MENU) {
		
		if (pressed(BUTTON_RIGHT)||pressed(BUTTON_DOWN)) {
			hot_button++;
			if (hot_button > 2) hot_button = 0;
		}
		else if (pressed(BUTTON_LEFT)||pressed(BUTTON_UP)) {
			hot_button--;
			if (hot_button < 0) {
				hot_button = 2;
			}
		}
		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
			if (hot_button == 1) {
				controls = CT_MULTIPLAYER;
			}
			else if (hot_button == 2) {
				controls = CT_AI_VS_AI;
			}
			else {
				controls = CT_SINGLE_PLAYER;
			}
		}

		if (hot_button == 0) {
			draw_text("PING PONG BY FAHAD", -48, 20, 1, 0xff0000);
			draw_text("SINGLE PLAYER",-20, 0, 0.5f, 0x8f7953);
			draw_text("MULTIPLAYER", -17, -10, 0.5f, 0xff0000);
			draw_text("AI VS AI", -13, -20, 0.5f, 0xff0000);
		}
		else if (hot_button == 1) {
			draw_text("PING PONG BY FAHAD", -48, 20, 1, 0xff0000);
			draw_text("SINGLE PLAYER", -20, 0, 0.5f, 0xff0000);
			draw_text("MULTIPLAYER", -17, -10, 0.5f, 0x8f7953);
			draw_text("AI VS AI", -13, -20, 0.5f, 0xff0000);
		}
		else if (hot_button == 2) {
			draw_text("PING PONG BY FAHAD", -48, 20, 1, 0xff0000);
			draw_text("SINGLE PLAYER", -20, 0, 0.5f, 0xff0000);
			draw_text("MULTIPLAYER", -17, -10, 0.5f, 0xff0000);
			draw_text("AI VS AI", -13, -20, 0.5f, 0x8f7953);
		}
		
		
	}
} 
#pragma once
#include "Utility.cpp"
#include "platform_common.cpp"
#include "Renderer.cpp"
#include "Main.cpp"
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
class Player {
public:
	float pos_y;
	float velocity;
	float ddp;
	int score;
	Player() {
		pos_y = 0.f;
		ddp = 0.f;
		velocity = 0.f;
		score = 0.f;
	}
};

enum Gamemode {
	GM_MENU,
	GM_SINGLE_PLAYER,
	GM_MULTIPLAYER,
};
int hot_key = 0;
Gamemode current_gamemode = Gamemode(GM_MENU);
//Arena
float arena_half_size_x = 87;
float arena_half_size_y = 45;
//Players
Player player_1;
Player player_2;
float player_half_size_x=1.2, player_half_size_y=8;
float player_1_pos_x = -80,player_2_pos_x = 80;
//Ball
float ball_half_size = 1;
float ball_pos_x = 0.f;
float ball_pos_y = 0.f;
float ball_velocity_x = 100.f;
float ball_velocity_y = 0.f;
int winstate = 0;
void simulate_player(Player &player,float dt) {
	//Physics
	player.ddp -= player.velocity * 10.f;//friction
	player.velocity = player.ddp * dt;//velocity
	player.pos_y = player.pos_y + player.velocity * dt + ((player.ddp * dt * dt) * 0.5f);//Some Calculation 

	//Collision (Player to Arena)
	if (player.pos_y > arena_half_size_y - player_half_size_y) {
		player.pos_y = arena_half_size_y - player_half_size_y;
	}
	if (player.pos_y < -arena_half_size_y + player_half_size_y) {
		player.pos_y = -arena_half_size_y + player_half_size_y;
	}
}
internal void simulate_game(Input* input, float dt) {
	clear_screen(0xcc0000);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x0b030b);//arena

	if (current_gamemode != GM_MENU) {
		if (is_down(BUTTON_UP)) player_2.ddp += 1000;
		if (is_down(BUTTON_DOWN)) player_2.ddp -= 1000;
		if (pressed(BUTTON_ESC)) {
			player_1.score = 0;
			player_2.score = 0;
			player_1.pos_y = 0.f;
			player_2.pos_y = 0.f;
			player_1.ddp = 0;
			player_2.ddp = 0;
			ball_pos_x = 0;
			ball_pos_y = 0;
			current_gamemode = Gamemode(GM_MENU);
		}
		if (current_gamemode == GM_MULTIPLAYER) {
			if (is_down(BUTTON_W)) player_1.ddp += 1000;
			if (is_down(BUTTON_S)) player_1.ddp -= 1000;
		}
		else {
			if (ball_pos_y > player_1.pos_y + 4.f) player_1.ddp += (ball_pos_y - player_1.pos_y)*100.f;
			if (ball_pos_y < player_1.pos_y - 4.) player_1.ddp -= (player_1.pos_y - ball_pos_y)*100.f;
			if (player_1.ddp > 6000) {
				player_1.ddp = 6000;
			}
			else if (player_1.ddp < -6000) {
				player_1.ddp = -6000;
			}
		}

		simulate_player(player_1, dt);
		simulate_player(player_2, dt);

		//Simulate ball
		{

			//ball_velocity_x = ball_ddp_x * dt;
			//ball_pos_x = ball_pos_x + ball_velocity_x * dt + ((ball_ddp_x * dt * dt) * 0.5f);


			//ball_velocity_y = ball_ddp_y * dt;
			//ball_pos_y = ball_pos_y + ball_velocity_y * dt + ((ball_ddp_y * dt * dt) * 0.5f);
			ball_pos_x += ball_velocity_x * dt;
			ball_pos_y += ball_velocity_y * dt;

			//Ball collision with arena
			if (ball_pos_x > arena_half_size_x - ball_half_size) {
				ball_pos_x = 0;
				ball_pos_y = 0;
				ball_velocity_x *= -1;
				ball_velocity_y = 0;
				player_1.score++;
			}
			if (ball_pos_x < -arena_half_size_x + ball_half_size) {
				ball_pos_x = 0;
				ball_pos_y = 0;
				ball_velocity_x *= -1;
				ball_velocity_y = 0;
				player_2.score++;
			}
			if (ball_pos_y > arena_half_size_y - ball_half_size) {
				ball_pos_y = arena_half_size_y - ball_half_size;
				ball_velocity_y *= -1;
			}
			if (ball_pos_y < -arena_half_size_y + ball_half_size) {
				ball_pos_y = -arena_half_size_y + ball_half_size;
				ball_velocity_y *= -1;
			}

			//aabb vs aabb
			if ((ball_pos_x + ball_half_size > player_2_pos_x - player_half_size_x) &&
				(ball_pos_x - ball_half_size < player_2_pos_x + player_half_size_x) &&
				(ball_pos_y + ball_half_size > player_2.pos_y - player_half_size_y) &&
				(ball_pos_y - ball_half_size < player_2.pos_y + player_half_size_y)) {
				ball_velocity_x *= -1;
				ball_pos_x = player_2_pos_x - player_half_size_x - ball_half_size;
				ball_velocity_y += player_2.ddp * 0.001f;
				ball_velocity_y += (ball_pos_y - player_2.pos_y);
			}
			if ((ball_pos_x + ball_half_size > player_1_pos_x - player_half_size_x) &&
				(ball_pos_x - ball_half_size < player_1_pos_x + player_half_size_x) &&
				(ball_pos_y + ball_half_size > player_1.pos_y - player_half_size_y) &&
				(ball_pos_y - ball_half_size < player_1.pos_y + player_half_size_y)) {
				ball_velocity_x *= -1;
				ball_pos_x = player_1_pos_x + player_half_size_x + ball_half_size;
				ball_velocity_y += player_1.ddp * 0.001f;
				ball_velocity_y += (ball_pos_y - player_1.pos_y);
			}
		}
		if (player_1.score == 20) {
			winstate = 1;
			player_1.score = 0;
			player_2.score = 0;
			player_1.pos_y = 0.f;
			player_2.pos_y = 0.f;
			player_1.ddp = 0;
			player_2.ddp = 0;
			ball_velocity_y = 0;
			ball_pos_x = 0;
			ball_pos_y = 0;
			current_gamemode = Gamemode(GM_MENU);
		}
		else if (player_2.score == 20) {
			winstate = 2;
			player_1.score = 0;
			player_2.score = 0;
			player_1.pos_y = 0.f;
			player_2.pos_y = 0.f;
			player_1.ddp = 0;
			player_2.ddp = 0;
			ball_velocity_y = 0;
			ball_pos_x = 0;
			ball_pos_y = 0;
			current_gamemode = Gamemode(GM_MENU);
		}
	}
	//Render
	if (current_gamemode == GM_MENU) {
		//menu
		if (pressed(BUTTON_UP) || pressed(BUTTON_DOWN)) hot_key = !hot_key;
		if (pressed(BUTTON_Q)) running = false;
		if (!hot_key) {
			if (pressed(BUTTON_ENTER)) current_gamemode = Gamemode(GM_SINGLE_PLAYER);
			draw_text("PING PONG BY FAHAD", -60, 30, 1.2, 0xff0000);
			draw_text("SINGLE PLAYER", -35, 0, 0.8, 0xffff00);
			draw_text("MULTIPLAYER", -30, -10, 0.8, 0xff0000);
			draw_text("Q TO QUIT", -80, 40, 0.5, 0xff9955);
			if (winstate == 1) {
				draw_text("PLAYER ONE WON", -30, 40, 0.7, 0xffffff);
			}
			else if (winstate == 2) {
				draw_text("PLAYER TWO WON", -30, 40, 0.7, 0xffffff);
			}
		}
		else {
			if (pressed(BUTTON_ENTER)) current_gamemode = Gamemode(GM_MULTIPLAYER);
			draw_text("PING PONG BY FAHAD", -60, 30, 1.2, 0xff0000);
			draw_text("SINGLE PLAYER", -35, 0, 0.8, 0xff0000);
			draw_text("MULTIPLAYER", -30, -10, 0.8, 0xffff00);
			draw_text("Q TO QUIT", -80, 40, 0.5, 0xff9955);
			if (winstate == 1) {
				draw_text("PLAYER ONE WON", -30, 40, 0.7, 0xffffff);
			}
			else if (winstate == 2) {
				draw_text("PLAYER TWO WON", -30, 40, 0.7, 0xffffff);
			}
		}
	}
	else {
		//gameplay
		draw_rect(player_1_pos_x, player_1.pos_y, player_half_size_x, player_half_size_y, 0xaaaaaa);//player_1
		draw_rect(player_2_pos_x, player_2.pos_y, player_half_size_x, player_half_size_y, 0xaaaaaa);//player_2
		draw_rect(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 0x00aa00);//Ball
		draw_number(player_1.score, -20, 40, 1, 0x6f4d5a);//player_1 score
		draw_number(player_2.score, 20, 40, 1, 0x6f4d5a);//player_2 score
	}
}
#include "raylib.h"

Texture2D game_sprite;
Rectangle rect_source = { 0.0f, 0.0f, 25.0f, 25.0f };

void load_asset() {
	game_sprite = LoadTexture("D:/source/repos/TestGame/snake_game.png");
}

void unload_asset() {
	UnloadTexture(game_sprite);
}

typedef struct {
	int x;
	int y;
} grid_node;

int grid_size = 20;
bool game_over;
int hi_score = 0;
int score;

grid_node snake_head;
grid_node snake_body[398];
int snake_length;
int direction_x;
int direction_y;

grid_node food;

void init_snake_head() {
	snake_head.x = 8;
	snake_head.y = 9;
}

void move_snake_head() {
	snake_head.x += direction_x;
	snake_head.y += direction_y;
}

void init_snake_body() {
	for (int i = 0; i <= 397; i++) {
		snake_body[i].x = 0;
		snake_body[i].y = 0;
	}
	snake_body[0].x = 7;
	snake_body[0].y = 9;
	snake_body[1].x = 6;
	snake_body[1].y = 9;
	snake_body[2].x = 5;
	snake_body[2].y = 9;
	snake_length = 3;
}

void move_snake_body() {
	for (int i = snake_length - 1; i > 0; i--) {
		snake_body[i].x = snake_body[i-1].x;
		snake_body[i].y = snake_body[i-1].y;
	}
	snake_body[0].x = snake_head.x;
	snake_body[0].y = snake_head.y;
}

void init_food() {
	food.x = 15;
	food.y = 9;
}

bool is_space_free() {
	if (food.x == snake_head.x and food.y == snake_head.y) {
		return false;
	}
	for (int i = 0; i < snake_length; i++) {
		if (food.x == snake_body[i].x and food.y == snake_body[i].y) {
			return false;
		}
	}
	return true;
}

void move_food() {
	while (true) {
		food.x = GetRandomValue(0, 19);
		food.y = GetRandomValue(0, 19);
		if (is_space_free) {
			break;
		}
	}
}

void init_game() {
	score = 0;
	game_over = false;
	init_snake_head();
	init_snake_body();
	init_food();
	direction_x = 1;
	direction_y = 0;
}

void check_collision() {
	// Collision with food
	if (snake_head.x == food.x and snake_head.y == food.y) {
		move_food();
		snake_body[snake_length].x = snake_body[snake_length - 1].x;
		snake_body[snake_length].y = snake_body[snake_length - 1].y;
		snake_length++;
		score += 10;
	}

	// Collision with wall
	if (snake_head.x < 0 or snake_head.x > 19 or snake_head.y < 0 or snake_head.y > 19) {
		game_over = true;
	}

	// Collision with snake body
	for (int i = 2; i < snake_length; i++) {
		if (snake_head.x == snake_body[i].x and snake_head.y == snake_body[i].y) {
			game_over = true;
			return;
		}
	}
}

void render_snake() {
	// Draw body
	rect_source.x = 50.0f;
	rect_source.y = 0.0f;
	for (int i = 0; i < snake_length; i++) {
		DrawTextureRec(game_sprite, rect_source, { snake_body[i].x * 25.0f, snake_body[i].y * 25.0f }, WHITE);
	}

	// Draw head
	if (!game_over) {
		if (direction_x == 1) {
			rect_source.x = 25.0f;
			rect_source.y = 0.0f;
			DrawTextureRec(game_sprite, rect_source, { snake_head.x * 25.0f, snake_head.y * 25.0f }, WHITE);
		}
		else if (direction_x == -1) {
			rect_source.x = 0.0f;
			rect_source.y = 25.0f;
			DrawTextureRec(game_sprite, rect_source, { snake_head.x * 25.0f, snake_head.y * 25.0f }, WHITE);
		}
		else if (direction_y == 1) {
			rect_source.x = 25.0f;
			rect_source.y = 25.0f;
			DrawTextureRec(game_sprite, rect_source, { snake_head.x * 25.0f, snake_head.y * 25.0f }, WHITE);
		}
		else if (direction_y == -1) {
			rect_source.x = 0.0f;
			rect_source.y = 0.0f;
			DrawTextureRec(game_sprite, rect_source, { snake_head.x * 25.0f, snake_head.y * 25.0f }, WHITE);
		}
	}

	// Draw tail
	Rectangle rect_source_2 = { 0, 0, 25, 25 };
	if (snake_body[snake_length - 1].x < snake_body[snake_length - 2].x) {
		rect_source.x = 25.0f;
		rect_source.y = 50.0f;
		DrawTextureRec(game_sprite, rect_source, { snake_body[snake_length - 1].x * 25.0f, snake_body[snake_length - 1].y * 25.0f }, WHITE);
	}
	else if (snake_body[snake_length - 1].x > snake_body[snake_length - 2].x) {
		rect_source.x = 0.0f;
		rect_source.y = 75.0f;
		DrawTextureRec(game_sprite, rect_source, { snake_body[snake_length - 1].x * 25.0f, snake_body[snake_length - 1].y * 25.0f }, WHITE);
	}
	else if (snake_body[snake_length - 1].y < snake_body[snake_length - 2].y) {
		rect_source.x = 25.0f;
		rect_source.y = 75.0f;
		DrawTextureRec(game_sprite, rect_source, { snake_body[snake_length - 1].x * 25.0f, snake_body[snake_length - 1].y * 25.0f }, WHITE);
	}
	else if (snake_body[snake_length - 1].y > snake_body[snake_length - 2].y) {
		rect_source.x = 0.0f;
		rect_source.y = 50.0f;
		DrawTextureRec(game_sprite, rect_source, { snake_body[snake_length - 1].x * 25.0f, snake_body[snake_length - 1].y * 25.0f }, WHITE);
	}
}

void render_food() {
	rect_source.x = 50.0f;
	rect_source.y = 25.0f;
	DrawTextureRec(game_sprite, rect_source, { food.x * 25.0f, food.y * 25.0f }, WHITE);
}

void read_input() {
	if (IsKeyDown(KEY_R) and game_over) {
		init_game();
	}
	if (!game_over) {
		if (IsKeyDown(KEY_UP) and direction_y != 1) {
			direction_y = -1;
			direction_x = 0;
		}
		else if (IsKeyDown(KEY_DOWN) and direction_y != -1) {
			direction_y = 1;
			direction_x = 0;
		}
		else if (IsKeyDown(KEY_LEFT) and direction_x != 1) {
			direction_y = 0;
			direction_x = -1;
		}
		else if (IsKeyDown(KEY_RIGHT) and direction_x != -1) {
			direction_y = 0;
			direction_x = 1;
		}
	}
}

void update_hi_score() {
	if (score > hi_score) {
		hi_score = score;
	}
}

int main() {
	InitWindow(800, 600, "Snake");
	SetTargetFPS(10);
	load_asset();
	init_game();
	while (!WindowShouldClose()) {
		// Game logic
		read_input();
		update_hi_score();
		if (!game_over) {
			move_snake_body();
			move_snake_head();
			check_collision();
		}

		// Game render
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangle(0, 0, 20 * 25, 20 * 25, ORANGE);
		render_snake();
		render_food();
		if (game_over) {
			DrawText("GAME OVER!\nPress R to restart.", 500, 60, 30, BLUE);
		}
		DrawText(TextFormat("Hi-Score: %i", hi_score), 500, 0, 30, BLUE);
		DrawText(TextFormat("Score: %i", score), 500, 30, 30, BLUE);
		EndDrawing();
	}
	unload_asset();
	CloseWindow();
	return 0;
}
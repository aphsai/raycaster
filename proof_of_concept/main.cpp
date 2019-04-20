#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

#define PI 3.14159265

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 500;
const int SCREEN_SEGMENT = 500;
const int BLOCK_SIZE = 20;
const int PLAYER_SIZE = 7;
const int PLAYER_SPEED = 3;
const int DIRECTION_INDICATOR_LENGTH = 40;
const int WALL_WIDTH = 100;
const double ROTATION_SPEED = 0.0872;  //Five degrees ~~
const double FIELD_OF_VIEW = PI / 6.0;

int map[10][10];

double player_x = 0;
double player_y = 0;
double player_direction = 0;

bool turnLeft = false;
bool turnRight = false;
bool moveForward = false;
bool moveBackward = false;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect screen;

void initializeMap() {
	ifstream fin ("map.txt");
	if (fin.is_open()) {
		for (int y = 0; y < 10; y++) 
			for (int x = 0; x < 10; x++) 
				fin >> map[y][x];
	}
	fin.close();
}
void clearScreen() {
	SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
	SDL_RenderClear(renderer);
}

void drawBoundaries() {
	SDL_Rect red = { 0, 0, SCREEN_SEGMENT, SCREEN_HEIGHT };
	SDL_Rect blue = { SCREEN_SEGMENT, 0, SCREEN_SEGMENT, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &red);
	SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
	SDL_RenderDrawRect(renderer, &blue);
}
void calculateXIntersection() {
}
void drawRed() {
	SDL_Rect player = { SCREEN_SEGMENT / 2 + (int)player_x, SCREEN_SEGMENT / 2 + (int)player_y, PLAYER_SIZE, PLAYER_SIZE }; // Player model

	SDL_SetRenderDrawColor(renderer, 255, 218, 185, 255);
	SDL_RenderFillRect(renderer, &player);
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Center line

	int direction_origin_x = player.x + player.w / 2;
	int direction_origin_y = player.y + player.h / 2;	

	SDL_RenderDrawLine(renderer, direction_origin_x, direction_origin_y, direction_origin_x + DIRECTION_INDICATOR_LENGTH * cos(player_direction) , direction_origin_y + DIRECTION_INDICATOR_LENGTH * sin(player_direction));

	SDL_Rect wall = { SCREEN_SEGMENT/2 - WALL_WIDTH / 2, 200, WALL_WIDTH, 2 };
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderFillRect(renderer, &wall);
} 

void drawBlue() {
	SDL_Rect player = { SCREEN_SEGMENT + SCREEN_SEGMENT / 2, SCREEN_SEGMENT / 2, PLAYER_SIZE, PLAYER_SIZE };
	SDL_SetRenderDrawColor(renderer, 255, 218, 185, 255);
	SDL_RenderFillRect(renderer, &player);

	float wall_direction = -player_direction + PI;

	float wall_x1 = -WALL_WIDTH / 2 - player_x;
	float wall_y1 = -50 - player_y;
	float wall_x2 = WALL_WIDTH / 2 - player_x; 
	float wall_y2 = -50 - player_y;
	
	float wall_rel_y1 = wall_x1 * sin(wall_direction) + wall_y1 * cos(wall_direction);
	float wall_rel_y2 = wall_x2 * sin(wall_direction) + wall_y2 * cos(wall_direction);
	float wall_rel_x1 = wall_x1 * cos(wall_direction) - wall_y1 * sin(wall_direction);
	float wall_rel_x2 = wall_x2 * cos(wall_direction) - wall_y2 * sin(wall_direction);

	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderDrawLine(renderer, SCREEN_SEGMENT + SCREEN_SEGMENT / 2 - wall_rel_x1, SCREEN_SEGMENT / 2 - wall_rel_y1, SCREEN_SEGMENT + SCREEN_SEGMENT / 2 - wall_rel_x2, SCREEN_SEGMENT / 2 - wall_rel_y2);

	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Center line

	int direction_origin_x = player.x + player.w / 2;
	int direction_origin_y = player.y + player.h / 2;	

	SDL_RenderDrawLine(renderer, direction_origin_x, direction_origin_y, direction_origin_x + DIRECTION_INDICATOR_LENGTH, direction_origin_y);
}


void draw() {
	clearScreen();
	drawBoundaries();
	drawRed();
	drawBlue();
	SDL_RenderPresent(renderer);
}


void update() {
	if (turnLeft) player_direction -= ROTATION_SPEED;
	if (turnRight) player_direction +=  ROTATION_SPEED;
	if (moveForward) player_x += PLAYER_SPEED * cos(player_direction), player_y += PLAYER_SPEED * sin(player_direction);
	if (moveBackward) player_x -= PLAYER_SPEED * cos(player_direction), player_y -= PLAYER_SPEED * sin(player_direction);
}

void handleInput(SDL_Event &e) {
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
			case SDLK_h:
				turnLeft = true;
				break;
			case SDLK_l:
				turnRight = true;
				break;
			case SDLK_j:
				moveBackward = true;
				break;
			case SDLK_k:
				moveForward = true;
				break;
		}
	} else if (e.type == SDL_KEYUP) {
		switch(e.key.keysym.sym) {
			case SDLK_h:
				turnLeft = false;
				break;
			case SDLK_l:
				turnRight = false;
				break;
			case SDLK_j:
				moveBackward = false;
				break;
			case SDLK_k:
				moveForward = false;
				break;
		}
	}
	if (fabs(player_direction) > 2 * PI) {
		if (player_direction > 0) player_direction -= 2 * PI;
		else player_direction += 2 * PI;
	}
}

int main() {
	window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	initializeMap();

	bool run = true;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	unsigned int frameStart;
	int frameTime;

	SDL_Event e;

	while(run) {
		frameStart = SDL_GetTicks();
		SDL_PollEvent(&e);
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) run = false;

		draw();
		update();
		handleInput(e);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

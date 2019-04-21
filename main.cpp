#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include "vector2d.hpp"

#define PI 3.14159265

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect screen;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int map_width;
int map_height;

const double MOVESPEED = 0.1; 
const double ROTATIONSPEED = 0.0872 * 2 / 5.0; // ~2.5 Degree

Vector2D position(22, 12);
Vector2D direction(-1, 0);
Vector2D cameraPlane(0, 0.66);

bool move_forwards = false, move_backwards = false, rotate_left = false, rotate_right = false;

std::vector<std::vector<int>> map;
//Default map
//	std::vector<std::vector<int>> map =
//	{
//	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
//	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,2,2,2,0,0,0,1},
//	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
//	};

void loadMap(const char* s) {
	std::fstream fin (s);
	fin >> map_height >> map_width;
	map = std::vector<std::vector<int>>(map_height, std::vector<int>(map_width, 0));
	for (int x = 0;  x < map_height; x++) {
		for (int y = 0; y < map_width; y++) {
			fin >>	map[x][y];
		}
	}
	fin.close();
}
int getMapTile(int x, int y) {
	return map[x][y];
}

void draw() {
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_RenderClear(renderer);
	//Draw floor
	SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
	SDL_Rect floor = { 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(renderer, &floor);
	//Draw boxes
	for (int x = 0; x < SCREEN_WIDTH; x++) {

		double cameraX = 2 * double(x) / double(SCREEN_WIDTH) - 1;

		Vector2D rayPosition = { position.x, position.y };
		Vector2D rayDirection = direction + cameraPlane * cameraX;
		Vector2D mapPosition { int(rayPosition.x), int(rayPosition.y) };
		
		//Calculates distance to next y tile and next x tile
		double deltaDistanceY = sqrt(1 + (rayDirection.x * rayDirection.x) / (rayDirection.y * rayDirection.y));
		double deltaDistanceX = sqrt(1 + (rayDirection.y * rayDirection.y) / (rayDirection.x * rayDirection.x));

		double nextSideDistanceX;
		double nextSideDistanceY;

		Vector2D stepDirection;

		if (rayDirection.x < 0) {
			stepDirection.x = -1;
			nextSideDistanceX = (rayPosition.x - mapPosition.x) * deltaDistanceX;
		} else {
			stepDirection.x = 1;
			nextSideDistanceX = (mapPosition.x - rayPosition.x + 1) * deltaDistanceX;
		}
		if (rayDirection.y < 0) {
			stepDirection.y = -1;
			nextSideDistanceY = (rayPosition.y - mapPosition.y) * deltaDistanceY;
		} else {
			stepDirection.y = 1;
			nextSideDistanceY = (mapPosition.y - rayPosition.y + 1) * deltaDistanceY;
		}

		int side = 0;
		while(true) {
			if (nextSideDistanceX < nextSideDistanceY) {
				nextSideDistanceX = nextSideDistanceX + deltaDistanceX;
				mapPosition.x = mapPosition.x + stepDirection.x;
				side = 0;
			} else {
				nextSideDistanceY = nextSideDistanceY + deltaDistanceY;
				mapPosition.y = mapPosition.y + stepDirection.y;
				side = 1;
			}
			//Check if ray hit wall
			if (getMapTile(mapPosition.x, mapPosition.y) > 0) break;
		}
		double perpendicularWallDistance;
		if (side == 0) perpendicularWallDistance = (mapPosition.x - rayPosition.x + (1 - stepDirection.x) / 2 ) / rayDirection.x;
		else perpendicularWallDistance = (mapPosition.y - rayPosition.y + (1 - stepDirection.y) / 2 ) / rayDirection.y;

		perpendicularWallDistance = abs(perpendicularWallDistance);

		int lineHeight = (int) (SCREEN_HEIGHT / perpendicularWallDistance);
		double linePercent = double(lineHeight) / double(SCREEN_HEIGHT);
		switch (getMapTile(mapPosition.x, mapPosition.y)) {
			case 1: 
				SDL_SetRenderDrawColor(renderer, 255 / (side + 1), 0, 0, 255);
				break;
			case 2:
				SDL_SetRenderDrawColor(renderer, 0, 255 / (side + 1), 0, 255);
				break;
			case 3:
				SDL_SetRenderDrawColor(renderer, 0, 0, 255 / (side + 1), 255);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 255 / (side + 1), 0, 255 / (side + 1), 255);
				break;
		}

		SDL_RenderDrawLine(renderer, x, SCREEN_HEIGHT / 2 - lineHeight / 2, x, SCREEN_HEIGHT / 2 + lineHeight / 2);
	}
	SDL_RenderPresent(renderer);
}

void update() {
	if (move_forwards) {
		if (getMapTile(position.x + direction.x * MOVESPEED, position.y) == 0) position.x = position.x + direction.x * MOVESPEED;
		if (getMapTile(position.x, position.y + direction.y * MOVESPEED) == 0) position.y = position.y + direction.y * MOVESPEED;
	}
	if (move_backwards) {
		if (getMapTile(position.x - direction.x * MOVESPEED, position.y) == 0) position.x = position.x - direction.x * MOVESPEED;
		if (getMapTile(position.x, position.y - direction.y * MOVESPEED) == 0) position.y = position.y - direction.y * MOVESPEED;
	}
	if (rotate_right) {
		double old_direction_x = direction.x;
		direction.x = direction.x * cos(-ROTATIONSPEED) - direction.y * sin(-ROTATIONSPEED);
		direction.y = old_direction_x * sin(-ROTATIONSPEED) + direction.y * cos(-ROTATIONSPEED);
		double old_plane_x = cameraPlane.x;
		cameraPlane.x = cameraPlane.x * cos(-ROTATIONSPEED) - cameraPlane.y * sin(-ROTATIONSPEED);
		cameraPlane.y = old_plane_x * sin(-ROTATIONSPEED) + cameraPlane.y * cos(-ROTATIONSPEED);
	}
	if (rotate_left) {
		double old_direction_x = direction.x;
		direction.x = direction.x * cos(ROTATIONSPEED) - direction.y * sin(ROTATIONSPEED);
		direction.y = old_direction_x * sin(ROTATIONSPEED) + direction.y * cos(ROTATIONSPEED);
		double old_plane_x = cameraPlane.x;
		cameraPlane.x = cameraPlane.x * cos(ROTATIONSPEED) - cameraPlane.y * sin(ROTATIONSPEED);
		cameraPlane.y = old_plane_x * sin(ROTATIONSPEED) + cameraPlane.y * cos(ROTATIONSPEED);
	}
}

void handleInput(SDL_Event &e) { 
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
			//Left Rotation
			case SDLK_h: 
				rotate_left = true;
				break;
			//Move forwards
			case SDLK_j:
				move_forwards = true;
				break;
			//Move backwards
			case SDLK_k:
				move_backwards = true;
				break;
			//Right rotation
			case SDLK_l: 
				rotate_right = true;
				break;
		}
	} else if (e.type == SDL_KEYUP) {
		switch(e.key.keysym.sym) {
			//Left Rotation
			case SDLK_h: 
				rotate_left = false;
				break;
			//Move forwards
			case SDLK_j:
				move_forwards = false;
				break;
			//Move backwards
			case SDLK_k:
				move_backwards = false;
				break;
			//Right rotation
			case SDLK_l: 
				rotate_right = false;
				break;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		loadMap(argv[1]);
	}
	window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

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
		handleInput(e);
		update();
		draw();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

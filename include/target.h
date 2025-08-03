#pragma once
#include <SDL.h>

class Target
{
	public:
		SDL_Rect rect;

		Target(int width = 64, int height = 64);
		void randomize(int screenWidth, int screenHeight);
		void renderWindow(SDL_Renderer* renderer);
};
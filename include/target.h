#pragma once
#include <SDL.h>

class Target
{
	public:
		SDL_Rect rect;
		SDL_Texture* texture = nullptr;

		Target(int width = 64, int height = 64);
		~Target();
		void randomize(int screenWidth, int screenHeight);
		void renderWindow(SDL_Renderer* renderer);
		bool loadTexture(SDL_Renderer* renderer, const char* file);
};
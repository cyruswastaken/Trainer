#include "target.h"
#include <iostream>

Target::Target(int width, int height)
{
	rect.w = width;
	rect.h = height;
	rect.x = 0;
	rect.y = 0;
}


void Target::randomize(int screenWidth, int screenHeight) {
	rect.x = rand() % (screenWidth - rect.w);
	rect.y = rand() % (screenHeight - rect.h);
}

void Target::renderWindow(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
}

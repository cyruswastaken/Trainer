#include<SDL.h>
#include<iostream>
#include<ctime>
#include "target.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Trainer",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!window)
	{
		cerr << "Window creation failed: " << SDL_GetError() << endl;
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
	{
		cerr << "Renderer creation failed: " << SDL_GetError() << endl;
		return -1;
	}

	srand((unsigned int)time(nullptr));

	Target target;
	target.randomize(SCREEN_WIDTH, SCREEN_HEIGHT);

	bool isRunning = true;
	SDL_Event e;
	int score = 0;

	while (isRunning)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				isRunning = false;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
			{
				int mx = e.button.x;
				int my = e.button.y;
				SDL_Point mousePoint = { mx, my };
				if (SDL_PointInRect(&mousePoint, &target.rect)) {
					score++;
					std::cout << "Score: " << score << std::endl;
					target.randomize(SCREEN_WIDTH, SCREEN_HEIGHT);
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); 
		SDL_RenderClear(renderer);

		target.renderWindow(renderer);

		SDL_RenderPresent(renderer);
		SDL_Delay(16); //~60fps
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
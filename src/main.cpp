#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BALL_SIZE = 32;

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "SDL_image Init Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Init Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Aim Trainer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Surface* surface = IMG_Load("assets/images/ball.png");
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!ballTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return 1;
    }

    Mix_Chunk* hitSound = Mix_LoadWAV("assets/audio/hit.wav");
    if (!hitSound) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    SDL_Rect targetRect = {
        rand() % (WINDOW_WIDTH - BALL_SIZE),
        rand() % (WINDOW_HEIGHT - BALL_SIZE),
        BALL_SIZE, BALL_SIZE
    };

    int score = 0;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (mouseX >= targetRect.x && mouseX <= targetRect.x + targetRect.w &&
                    mouseY >= targetRect.y && mouseY <= targetRect.y + targetRect.h) {


                    int winW, winH;
                    SDL_GetWindowSize(window, &winW, &winH);
                    targetRect.x = rand() % (winW - BALL_SIZE);
                    targetRect.y = rand() % (winH - BALL_SIZE);


                    score++;
                    Mix_PlayChannel(-1, hitSound, 0);
                    std::cout << "Score: " << score << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, ballTexture, nullptr, &targetRect);
        SDL_RenderPresent(renderer);
    }

    Mix_FreeChunk(hitSound);
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

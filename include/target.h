#pragma once
#include <SDL.h>

class Target {
public:
    Target(int width = 64, int height = 64);
    ~Target();
    bool loadTexture(SDL_Renderer* renderer, const char* file);
    void randomize(int screenWidth, int screenHeight);
    void render(SDL_Renderer* renderer) const;

private:
    SDL_Rect rect;
    SDL_Texture* texture;
};
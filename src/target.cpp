#include "target.h"
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>

Target::Target(int width, int height) : rect{0, 0, width, height}, texture(nullptr) {}

Target::~Target() {
    if (texture) SDL_DestroyTexture(texture);
}

bool Target::loadTexture(SDL_Renderer* renderer, const char* file) {
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        std::cerr << "IMG_Load failed: " << IMG_GetError() << std::endl;
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Target::randomize(int screenWidth, int screenHeight) {
    rect.x = rand() % (screenWidth - rect.w);
    rect.y = rand() % (screenHeight - rect.h);
}

void Target::render(SDL_Renderer* renderer) const {
    if (texture) SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
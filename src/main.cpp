#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

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


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

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
    bool show_menu = true;
    bool show_settings = false;
    bool show_debug_window = false; 

    SDL_Event event;

    while (running) {
    
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_KEYDOWN) {
              
                if (event.key.keysym.sym == SDLK_TAB) {
                    show_debug_window = !show_debug_window;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                // Only process game logic if the menu is not visible and ImGui is not consuming the mouse
                if (!io.WantCaptureMouse && !show_menu) {
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
        }

       
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // -------------------------------------------------------------
        // ImGui UI Logic - Menu and Debug
        // -------------------------------------------------------------
        if (show_menu) {
            ImGui::Begin("Aim Trainer Menu");
            ImGui::SetWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
            ImGui::Text("Welcome to the Aim Trainer!");
            ImGui::Separator();
            if (ImGui::Button("Start Game", ImVec2(280, 0))) {
                show_menu = false;
                score = 0;
            }
            if (ImGui::Button("Settings", ImVec2(280, 0))) {
                show_settings = true;
            }
            if (ImGui::Button("Quit", ImVec2(280, 0))) {
                running = false;
            }
            ImGui::End();
        }

        if (show_settings) {
            ImGui::Begin("Settings", &show_settings);
            ImGui::Text("Adjust game parameters here.");
            ImGui::Separator();
            static int target_size_slider = BALL_SIZE;
            ImGui::SliderInt("Target Size", &target_size_slider, 16, 128, "%d pixels");
            if (ImGui::Button("Apply")) {
                targetRect.w = target_size_slider;
                targetRect.h = target_size_slider;
            }
            ImGui::End();
        }

        // The debug window is now conditional on both not being in the menu AND the 'Tab' key being pressed
        if (!show_menu && show_debug_window) {
            ImGui::Begin("Debug Window", &show_debug_window); // The &show_debug_window allows you to close it with the 'X' button
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("Current Score: %d", score);
            ImGui::Text("Target Position: (%d, %d)", targetRect.x, targetRect.y);
            if (ImGui::Button("Return to Menu")) {
                show_menu = true;
            }
            ImGui::End();
        }

        
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

      
        if (!show_menu) {
            SDL_RenderCopy(renderer, ballTexture, nullptr, &targetRect);
        }

       
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    Mix_FreeChunk(hitSound);
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
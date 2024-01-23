#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_sdl2.h"
#include "../external/imgui/backends/imgui_impl_sdlrenderer2.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("SDL2 + ImGui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1000, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize ImGui with SDL and OpenGL2
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls	

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    io.Fonts->AddFontDefault();

	// load textures for testing purpose
	SDL_Surface* surface = IMG_Load("/home/sbstndbs/sdl-imgui/img/fish.png");
	if(!surface){
		fprintf(stderr, "Error loading image FISH\n");
		return -1;
	}	
	//crreate texture 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture){
		fprintf(stderr, "Error from surface\n");
	}


    // Main loop
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                quit = true;
        }

        // Start a new ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // Your ImGui code here
        ImGui::Begin("Salut les ZINZINS");
        ImGui::Text("fenêtre d'un ZINZIN");
        ImGui::End();

        // Render ImGui
        ImGui::Render();

        // Render SDL
	//
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 200, 20, 20, 255);
        SDL_SetRenderDrawColor(renderer, 50 , 50, 50, 50);
        SDL_RenderDrawLine(renderer, 100, 100, 500, 500);
	
	// render surface
		SDL_RenderCopy(renderer, texture, NULL, NULL);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);

        // Wait a short time to avoid excessive CPU usage
        SDL_Delay(16);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


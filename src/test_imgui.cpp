

#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <math.h>

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_sdl2.h"
#include "../external/imgui/backends/imgui_impl_sdlrenderer2.h"


struct poisson {
	float x ; 
	float y ;
        float vx ; 
	float vy ;
	float angle ; 
};


float speed(struct poisson fish){
	float s = sqrt(fish.vx*fish.vx + fish.vy*fish.vy);
	return s;
}

float distance(struct poisson fish1, struct poisson fish2){
        float d = sqrt((fish1.x - fish2.x)*(fish1.x - fish2.x) + (fish1.y - fish2.y)*(fish1.y - fish2.y));
        return d;
}


void fish_evolution(struct  poisson *fish, int size, float dt){
	struct poisson *fish1 ;
	struct poisson *fish2 ;	

	for (int i = 0 ; i < size ; i++){
		float moveX = 0.0f;
		float moveY = 0.0f;
		float avgDX = 0.0f; 
		float avgDY = 0.0f; 
		int voisin = 0 ; 
		float d ; 
		for (int j = 0 ; j < size; j++){
			if (i != j){
				fish1 = &fish[i];
				fish2 = &fish[j];
				d = distance(*fish1, *fish2);
				//printf("distance : %f\n", d);
				//avoid
				if (d < 20.0){
				//	printf("avoid between %d and %d\n", i, j);
					moveX += fish1->x - fish2->x ; 
					moveY += fish1->y - fish2->y ;
				}
				// match velocity
				if (d < 80.0 && d >= 20.0){
                                  //     printf("match between %d and %d\n", i, j);

					avgDX += fish2->vx;
					avgDY += fish2->vy;
					voisin +=1;
				}
			}
		}
		//avoid
		fish1->vx += moveX*0.009;
		fish1->vy += moveY*0.009;
	
		// match velocity 
		if (voisin){
			fish1->vx += ((avgDX/voisin) - fish1->vx)*0.03;
	                fish1->vy += ((avgDY/voisin) - fish1->vy)*0.03;
		}
	}

	// limit speed
	float speed_limit = 30.0 ; 
	for (int i = 0 ; i < size ; i++){
		float s = speed(fish[i]);
		if (s > speed_limit){
			fish[i].vx = fish[i].vx * speed_limit / s ; 
                      fish[i].vy = fish[i].vy * speed_limit / s ;

		}
	}
	float speed_under_limit = 1.0 ;
        for (int i = 0 ; i < size ; i++){
                float s = speed(fish[i]);
                if (s < speed_under_limit){
                        fish[i].vx = fish[i].vx * speed_under_limit / s ;
                      fish[i].vy = fish[i].vy * speed_under_limit / s ;

                }
        }



	// bounds 
	//
	for (int i = 0 ; i < size ; i++){
		if (fish[i].x < 0.0 || fish[i].x > 1200.0){
			fish[i].vx = - fish[i].vx ; 
		}
	        if (fish[i].y < 0.0 || fish[i].y > 800.0){
	                fish[i].vy = - fish[i].vy ;
	        }
	}

	// center
	


        for (int i = 0 ; i < size ; i++){
                fish[i].x += fish[i].vx*dt ;
                fish[i].y += fish[i].vy*dt ;	
		fish[i].angle = atan2(fish[i].vy, fish[i].vx)*(180.0/M_PI);
        }

}





int main(int argc, char* argv[]) {

	// initialise poisson structure 
	int n = 500 ; 
	int px = 1200 ;
	int py = 800 ;
	int vxmax = 10.0; 
	int vymax = 10.0;
	poisson* p = (poisson*) malloc(sizeof(poisson) * n);
	for (int i = 0 ; i < n ; i++){
		// create random numbers 
		//
		p[i].x = (float)rand()/(float)(RAND_MAX/px)  ;
		p[i].y = (float)rand()/(float)(RAND_MAX/py); 
                p[i].vx = (float)(rand()/(float)(RAND_MAX/(float)vxmax)) - vxmax/2.0;
                p[i].vy = (float)(rand()/(float)(RAND_MAX/(float)vymax)) - vymax/2.0;
		
		p[i].angle = (float)(0.5*i);
	}

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("SDL2 + ImGui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, px, py, SDL_WINDOW_SHOWN);
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

       static  float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};


    io.Fonts->AddFontDefault();

	// load textures for testing purpose
	SDL_Surface* surface = IMG_Load("../img/fleche.png");
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



	int size_fish = 10.0 ; 
	float dt = 0.01 ;

	float dist_repulsion = 2 ; 
	float dist_alignement = 10 ; 
	float dist_attraction = 30 ; 

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
	ImGui::SliderInt("Size", &size_fish, 1, 100);
	ImGui::SliderFloat("dt", &dt, 0.01, 2.0);
        ImGui::SliderFloat("distance répulsion", &dist_repulsion, 1, 10);
        ImGui::SliderFloat("distance alignement", &dist_alignement, 10, 15);
        ImGui::SliderFloat("distance attraction", &dist_attraction, 15, 50);
	ImGui::ColorEdit3("Background Color", color);
        ImGui::End();

        // Render ImGui
        ImGui::Render();

        // Render SDL
	//
	SDL_SetRenderDrawColor(renderer, (int)(color[0]*255), (int)(color[1]*255), (int)(color[2]*255), (int)(color[3]*255));
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 50 , 50, 50, 50);
//        SDL_RenderDrawLine(renderer, 100, 100, 500, 500);
	
	// render surface
	//
	//
    // Obtenir la largeur et la hauteur originales de l'image
    int originalWidth, originalHeight;
    SDL_QueryTexture(texture, NULL, NULL, &originalWidth, &originalHeight);

    // Redimensionner l'image à la moitié de sa taille d'origine (par exemple)
    int newWidth = (int)(size_fish);
    int newHeight = (int)(size_fish);

    // Définir la destination pour le redimensionnement
    SDL_Rect Rect = {0,0, originalWidth, originalHeight};
    SDL_Rect destinationRect;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_Point center ; 
        for (int i = 0 ; i < n ; i++){
		destinationRect = {(int)(p[i].x - newWidth/2), (int)(p[i].y - newHeight/2), newWidth,newHeight};
		float angle = p[i].angle;

		SDL_RenderCopyEx(renderer, texture, &Rect, &destinationRect, angle, NULL, flip);
	}
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);

	// update position
	fish_evolution(p, n, dt);
        // Wait a short time to avoid excessive CPU usage
	//
	//
        SDL_Delay(1);
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


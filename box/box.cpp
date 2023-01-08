#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

void clear(SDL_Renderer* render){
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderClear(render);
}

void draw_box(SDL_Renderer* render, SDL_Rect box){
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &box);
}

void move_box(SDL_Event& event, SDL_Rect& box){
	int speed = 15;

	switch(event.type){
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_w:
					box.y-=speed;
					break;
				case SDLK_s:
					box.y+=speed;
					break;
				case SDLK_a:
					box.x-=speed;
					break;
				case SDLK_d:
					box.x+=speed;
					break;
			}
	}
}

int main(){
	bool quit = false;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << SDL_GetError();
	}

 	SDL_Window* window = SDL_CreateWindow("box", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;
	
	SDL_Rect box = {0, 0, 50, 50};

	while(!quit){
		clear(render);
		
		draw_box(render, box);
		
		SDL_RenderPresent(render);
		
		while(SDL_PollEvent(&event)){
			move_box(event, box);
			
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;
}

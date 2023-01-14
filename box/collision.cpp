#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

void clear(SDL_Renderer* render){
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderClear(render);
}

void draw_box(SDL_Renderer* render, SDL_Rect box, int r, int g, int b, bool fill=true){
	SDL_SetRenderDrawColor(render, r, g, b, 255);
	
	if(fill)
		SDL_RenderFillRect(render, &box);
	else
		SDL_RenderDrawRect(render, &box);
}

void move_box(SDL_Event& event, SDL_Rect& box){
	int speed = 10;

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

bool collide(SDL_Rect& r1, SDL_Rect& r2){
	if(r1.x + r1.w/2 <= r2.x - r2.w/2){
		return false;
	}
	
	if(r1.x - r1.w/2 >= r2.x + r2.w/2){
		return false;
	}
	
	if(r1.y + r1.h/2 <= r2.y - r2.h/2){
		return false;
	}

	if(r1.y - r1.h/2 >= r2.y + r2.h/2){
		return false;
	}
	
	return true;
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
	SDL_Rect collider = {500, 100, 50, 50};

	bool hit = false;

	while(!quit){
		clear(render);

		if(!hit){
			draw_box(render, box, 0, 0, 0);
			draw_box(render, collider, 0, 0, 255, 0);
		}else{
			draw_box(render, collider, 0, 0, 0);
		}
		
		SDL_RenderPresent(render);
		
		while(SDL_PollEvent(&event)){
			if(!collide(box, collider)){
				move_box(event, box);
			}else{
				hit = true;
			}
		
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;
}

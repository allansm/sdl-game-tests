#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

void clear(SDL_Renderer* render){
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
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

struct Ball{
	int speed_x = -1;
	int speed_y = 0;

	SDL_Rect rect = {800/2 - 8, 600/2 - 8, 8, 8};

	void move(){
		rect.x+=speed_x;
		rect.y+=speed_y;
	}
	
	void draw(SDL_Renderer* render){
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderFillRect(render, &rect);
	}
};


int main(){
	bool quit = false;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << SDL_GetError();
	}

 	SDL_Window* window = SDL_CreateWindow("box", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;

	Ball ball;
	
	SDL_Rect left = {0, 0, 1, 600};
	SDL_Rect right = {799, 0, 1, 600};
	
	int sx = 1;

	while(!quit){
		clear(render);

		ball.draw(render);
		
		SDL_RenderPresent(render);

		if(collide(left, ball.rect)){
			sx+=2;
			if(sx > 10) sx = 1;
	
			ball.speed_x = sx;
		}

		if(collide(right, ball.rect)){
			sx+=2;
			
			if(sx > 10) sx = 1;

			ball.speed_x = sx*-1;
		}

		ball.move();
		
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;
}

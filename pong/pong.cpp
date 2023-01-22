#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

void clear(SDL_Renderer* render){
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
}

bool collide(SDL_Rect& r1, SDL_Rect& r2){
	if(r1.x < r2.x + r2.w){
		if(r1.x + r1.w > r2.x){
			if(r1.y < r2.y + r2.h){
				if(r1.y + r1.h > r2.y){
					return true;
				}
			}
		}
	}
	
	return false;
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
	
	bool hit(SDL_Rect& rect){
		return collide(this->rect, rect);
	}
};

struct Block{
	SDL_Rect rect;
	
	Block(){
	
	}

	Block(int x, int y, int w, int h){
		rect = {x, y, w, h};
	}
	
	void move(int x ,int y){
		rect.x += x;
		rect.y += y;
	}
	
	void draw(SDL_Renderer* render){
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderFillRect(render, &rect);
	}
	
	bool hit(SDL_Rect& rect){
		return collide(this->rect, rect);
	}
};

struct Player{
	Block top;
	Block mid;
	Block bot;
	
	int speed = 31;
	int atk_speed = 3;

	int up;
	int down;

	Player(int x, int y, int up, int down){
		top = {x, y, 8, 8*3};
		mid = {x, y+8*3, 8, 8*3};
		bot = {x, y+8*3*2, 8, 8*3};
		
		this->up = up;
		this->down = down;
	}
	
	void draw(SDL_Renderer* render){
		top.draw(render);
		mid.draw(render);
		bot.draw(render);
	}

	void hit(Ball& ball){
		if(top.hit(ball.rect)){
			ball.speed_x *= -atk_speed;
			ball.speed_y = 1;
		}

		else if(mid.hit(ball.rect)){
			ball.speed_x *= -atk_speed;
			ball.speed_y = 0;
		}
		
		else if(bot.hit(ball.rect)){
			ball.speed_x *= -atk_speed;
			ball.speed_y = -1;
		}
		
		if(ball.speed_x > atk_speed){
			ball.speed_x = atk_speed;
		}else if(ball.speed_x < -atk_speed){
			ball.speed_x = -atk_speed;
		}
	}

	void move(int x, int y){
		top.move(x, y);
		mid.move(x, y);
		bot.move(x, y);
	}
	
	void move(SDL_Event& event){
		switch(event.type){
			case SDL_KEYDOWN:
				int key = event.key.keysym.sym;
				
				if(key == up){
					move(0, speed*-1);
					break;
				}

				if(key == down){
					move(0, speed);
					break;
				}
		}
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

	Player p1(25, ((600-(8*3*2))/2)-9, 'w', 's');
	Player p2(799-25-8, ((600-(8*3*2))/2)-9, 'i', 'k');
	
	SDL_Rect left = {-5, 0, 5, 600};
	SDL_Rect right = {799+4, 0, 5, 600};
	SDL_Rect top = {0, -5, 800, 5};
	SDL_Rect bottom = {0, 599+4, 800, 5};

	while(!quit){
		clear(render);

		ball.draw(render);

		p1.draw(render);
		p2.draw(render);
		
		SDL_RenderPresent(render);
	
		p1.hit(ball);
		p2.hit(ball);

		if(ball.hit(left)){
			ball.speed_x = 1;
		}

		if(ball.hit(right)){
			ball.speed_x = -1;
		}

		if(ball.hit(top)){
			ball.speed_y = 1;
		}
		
		if(ball.hit(bottom)){
			ball.speed_y = -1;
		}

		ball.move();
		
		while(SDL_PollEvent(&event)){
			p1.move(event);
			p2.move(event);	
	
			if(event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;
}

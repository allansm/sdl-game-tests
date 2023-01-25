#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>

#include <sys/time.h>

void clear(SDL_Renderer* render){
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
}

void lines(SDL_Renderer* render){
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderDrawLine(render, (800/2), 0, (800/2), 600);
	SDL_RenderDrawLine(render, 0, 0, 800, 0);
	SDL_RenderDrawLine(render, 0, 599, 800, 599);
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

struct Elapse{
	int start;
	
	int ms(){
		struct timeval t;
		
		gettimeofday(&t, NULL);

		return t.tv_sec * 1000 + t.tv_usec / 1000;
	}
	
	Elapse(){
		start = ms();
	}
	
	int elapsed(){
		return ms() - start;
	}

	bool elapsed(int time){
		return elapsed() >= time;
	}
};

struct Ball{
	int speed_x = -1;
	int speed_y = 0;

	bool can_move = true;

	Elapse timer;

	SDL_Rect rect = {800/2 - 4, 600/2 - 4, 8, 8};

	void move(){
		if(can_move){
			rect.x+=speed_x;
			rect.y+=speed_y;
		}else{
			if(timer.elapsed(600)){
				can_move = true;
			}
		}
	}
	
	void draw(SDL_Renderer* render){
		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderFillRect(render, &rect);
	}
	
	bool hit(SDL_Rect& rect){
		return collide(this->rect, rect);
	}
	
	void reset(){
		speed_x *= -1;
		speed_y = 0;

		rect = {800/2 - 4, 600/2 - 4, 8, 8};

		can_move = false;

		timer = Elapse();
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
	
	int speed = 30;
	int atk_speed = 4;

	int up;
	int down;
	
	Elapse timer;

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
		struct P{
			Ball& ball;
			Elapse& timer;
		};
		
		P p = {ball, timer};
		
		auto hit_power = [&p](int min, int max){
			Ball& ball = p.ball;
			Elapse timer = p.timer;

			if(ball.speed_x >= 1 || ball.speed_x <= -1){
				if(timer.elapsed() % 2 == 0){
					ball.speed_x = ball.speed_x > 0 ? ball.speed_x+max : ball.speed_x-max;
				}else{
					auto tmp = ball.speed_x > 0 ? ball.speed_x-min : ball.speed_x+min;

					if(tmp != 0){
						ball.speed_x = tmp;
					}
				}
			}
		};

		if(top.hit(ball.rect)){
			ball.speed_x *= -1;
			ball.speed_y = 1;

			hit_power(1, 2);
		}

		else if(mid.hit(ball.rect)){
			ball.speed_x *= -1;
			ball.speed_y = 0;

			hit_power(1, 2);
		}
		
		else if(bot.hit(ball.rect)){
			ball.speed_x *= -1;
			ball.speed_y = -1;
			
			hit_power(1, 2);
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

		lines(render);
		
		SDL_RenderPresent(render);
	
		p1.hit(ball);
		p2.hit(ball);

		if(ball.hit(left)){
			ball.speed_x = 1;
			ball.reset();
		}

		if(ball.hit(right)){
			ball.speed_x = -1;
			ball.reset();
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

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "primit.h"

int main(int argc, char *argv[]){
	/* What we dealing with here is */
	// Initialization
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "[FAILED] SDL_Init: " << SDL_GetError() << std::endl;
		return 1;
	}
	// Creating window
	SDL_Window *win = SDL_CreateWindow("Graphix", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if(win == nullptr){
		std::cout << "[FAILED] SDL_CreateWindow: " << SDL_GetError() << std::endl;
		return 1;
	}
	// Creating Renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr){
		std::cout << "[FAILED] SDL_CreateRenderer: " << SDL_GetError() << std::endl;
		return 1;
	}
	//
	// Beats, bass and analogue shit (c)
	//
	srand(time(NULL));
	SDL_Event e;
	bool quit = false;
	Figure F(ren);
	/*
	Line L0(0, 0, 100, 100, ren), L1(100, 0, 0, 100, ren);
	Circle C0(50, 50, 35, 0, ren);
	Rect R0(10, 10, 90, 90, ren);
	F.add((Base*)&L0);
	F.add((Base*)&L1);
	F.add((Base*)&C0);
	F.add((Base*)&R0);
	*/
	/*
	Line L0(ren, 50, 50, 50, 200), L1(ren, 150, 50, 150, 110), L2(ren, 150, 70, 180, 50), L3(ren, 380, 0, 380, 110);
	Circle C0(ren, 80, 80, 30, 0), C1(ren, 250, 80, 30, 0), C2(ren, 350, 80, 30, 0);
	F.add((Base*)&L0);
	F.add((Base*)&L1);
	F.add((Base*)&L2);
	F.add((Base*)&C0);
	F.add((Base*)&C1);
	F.add((Base*)&C2);
	F.add((Base*)&L3);
	F.saveToFile("prod.fig.txt");
	*/
	F.loadFromFile("prod.fig.txt");
	F.move(150, 150);
	F.show();
	while(!quit){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				quit = true;
			}
			if(e.type == SDL_KEYDOWN){
				quit = true;
			}
			if(e.type == SDL_MOUSEBUTTONDOWN){
				quit = true;
			}
		}
        SDL_RenderClear(ren);
        /*
        L.show();
        K.show();
        */
        int ddx = rand()%10-5;
        int ddy = rand()%10-5;
        F.move(ddx, ddy);
        SDL_RenderPresent(ren);
        SDL_Delay(100);
	}
	// Cleaning up
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
	/*  a total lack of respect of law */
}


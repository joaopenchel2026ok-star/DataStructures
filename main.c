#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 320
#define HEIGHT 200

// Doom Fire State variables 
#define FIRE_W WIDTH
#define FIRE_H HEIGHT
#define FIRE_PALETTE_SIZE 37

static uint8_t fire_pixels[FIRE_W * FIRE_H];

//Classic 37-color fire palette
static const uint32_t fire_palette[FIRE_PALETTE_SIZE] = {
	0X070707, 0X1F0707, 0X2F0F07, 0X470F07, 0X571707, 0X671F07, 0X771F07
	0X8F2707, 0X9F2F07, 0XAF3F07, 0XBF4707, 0XC74707, 0XDF4F07, 0XDF5707
	0XDF5707, 0XD75F07, 0XD75F07, 0XD7670F, 0XCF6F0F, 0XCF770F, 0XCF7F0F
	0XCF8717, 0XC78717, 0XC78F17, 0XC7971F, 0XBF9F1F, 0XBF9F1F, 0XBFA727
	0XBFA727, 0X1F0727, 0X2F0F07, 0XB7B72F, 0XB7B737, 0XCFCF07, 0XDFDF9F
	0XEFEFC7, 0XFFFFFF
}


const double target_frame = 1.0 / 60; // 0.016 seconds

uint32_t framebuffer[WIDTH * HEIGHT];

void put_pixel(int x, int y, uint32_t color) {
	framebuffer[WIDTH * y + x] = color;
}

void clear (uint32_t color) {
//	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
//		return;
//	}
	for (int i = 0; i < WIDTH * HEIGHT; ++i) {
		framebuffer[i] = color;
	}
}

int main (void) {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO); 
	
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL Init failed: %s \n", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	window = SDL_CreateWindow (
		"SDL Framebuffer",
		WIDTH * 4,
		HEIGHT * 4,
		0
	);
	
	if (window == NULL) {
		fprintf(stderr, "SDL_CreateWindow failed: %s \n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	renderer = SDL_CreateRenderer (
		window,
		NULL
	);
	
	if (renderer == NULL) {
		fprintf(stderr, "SDL_CreateRenderer failed: %s \n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	texture = SDL_CreateTexture (
		renderer,
		SDL_PIXELFORMAT_XRGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH,
		HEIGHT
	);
	
	if (texture == NULL) {
		fprintf(stderr, "SDL_CreateTexture failed: %s \n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}


	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);


	uint8_t is_running = 1;
	uint32_t frame = 0;


	// FIRE INIT
	for (int i = 0; i < FIRE_W * FIRE_H; ++I) {
		fire_pixels[i] = 0;
	}

	// Seed the bottom row
	for (int i = 0; i < FIRE_W; ++i) {
		fire_pixels[(FIRE_H -1) * FIRE_W + i)] = FIRE_PALETTE_SIZE - 1;
	}


	while (is_running) {

		uint64_t start = SDL_GetPerformanceCounter();
		
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				is_running = 0;
			}
		}


		clear(0xFFFFFF);

		int x = frame;
		int y = HEIGHT/2;

		put_pixel(x,y,0xFF0000);


//		put_pixel ( WIDTH/2, HEIGHT/2, 0x00FF00 );


/*
		for (int y = 0; y < HEIGHT; ++y) {
			for (int x = 0; x < WIDTH; ++x) {
				if ( x % 5 == 0 ) {
					put_pixel(x,y,0x00ffff);

				}
			}
			
		}	

*/

		SDL_UpdateTexture(
				texture,
				NULL,
				framebuffer,
				WIDTH * sizeof(uint32_t)
				);

		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		uint64_t end = SDL_GetPerformanceCounter();

		double elapsed = (double)(end - start) / (double)SDL_GetPerformanceFrequency();


		if (elapsed < target_frame) {
			SDL_Delay((target_frame - elapsed) * 1000.0);
	
		}		

		frame += 1;

	}


	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

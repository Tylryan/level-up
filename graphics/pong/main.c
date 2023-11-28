
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "logger.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define PADDLE_HEIGHT (SCREEN_HEIGHT / 6)
#define PADDLE_WIDTH 16
#define BALL_DIMS 24
#define BALL_CENTER_X ((SCREEN_WIDTH / 2) - BALL_DIMS)
#define BALL_CENTER_Y ((SCREEN_HEIGHT / 2) - BALL_DIMS)


enum OBJECT_TYPE
{
	OBJ_BALL,
	OBJ_PADDLE
};

typedef struct {
	bool isRunning;
	SDL_Window * window;
	SDL_Renderer * renderer;
} Game;

typedef struct {
	SDL_Texture * texture;
	SDL_Renderer * renderer;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	
} GameObject;

typedef struct {
	GameObject object;
	int xDirection;
	int yDirection;
	int speed;
} Ball;

GameObject * GOInit(char * textureSheet, SDL_Renderer * renderer, int x, int y, int w, int h);

void GODestroy(GameObject * object);
void GOUpdate(GameObject * object);
void GORender(GameObject * object);
void GOMovePaddle(GameObject * object, int y);
void Ball_Move(Ball * self, int x, int y);

Game game = {NULL};

GameObject net;
GameObject paddles[2];
//GameObject ball;
Ball ball;

void init();
void handleEvents();
void render();
void update();
void clean();
void close();

SDL_Texture * loadTexture(char * file_name);

int
main(void)
{
	init();
	/* Do not write any code above this */

	net = *GOInit("./net-32-by-32.png",
		     game.renderer, 0, 0,
		     SCREEN_WIDTH, SCREEN_HEIGHT);

	paddles[0] = *GOInit("./paddle-32-by-32.png",
			     game.renderer,
			     0, 0,
			     PADDLE_WIDTH, PADDLE_HEIGHT);

	paddles[1] = *GOInit("./paddle-32-by-32.png",
			     game.renderer,
			     SCREEN_WIDTH - PADDLE_WIDTH, 0,
			     PADDLE_WIDTH, PADDLE_HEIGHT);
	ball.object = *GOInit("./ball-32-by-32.png",
			game.renderer,
		        BALL_CENTER_X, BALL_CENTER_Y,
			BALL_DIMS, BALL_DIMS);

	ball.xDirection = 1;
	ball.yDirection = 1;
	ball.speed      = 1;

	const int FPS        = 60;
	const int frameDelay = 1000 / FPS;

	long frameStart;
	int frameTime;

	while (game.isRunning)
	{
		frameStart = SDL_GetTicks();

		handleEvents();
		update();
		render();

		frameTime = SDL_GetTicks() - frameStart;


		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

	}

	clean();
}

void
init()
{
	if (SDL_Init(SDL_INIT_EVENTS) != 0)
	{
		game.isRunning = false;

		LOG(ERR, "SDL FAILED TO INITIALIZE: %s\n", SDL_GetError());
		return;
	}

	game.window = SDL_CreateWindow("TITLE",
				       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				       SCREEN_WIDTH, SCREEN_HEIGHT,
				       SDL_WINDOW_SHOWN);

	if (game.window == NULL)
	{
		game.isRunning = false;

		LOG(ERR, "SDL Failed to create window: %s\n", SDL_GetError());
		return;
	}

	game.renderer = SDL_CreateRenderer(game.window, -1, 0);

	if (game.renderer == NULL)
	{
		game.isRunning = false;

		LOG(ERR, "SDL Failed to create renderer: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRenderDrawColor(game.renderer, 0,0,0,0);

	game.isRunning = true;
}

void
handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	    case  SDL_QUIT:
	    {
		    LOG(INFO, "USER QUIT GAME");
		    game.isRunning = false;
		    break;
	    }
	    case SDL_KEYDOWN:
	    {
		    LOG(INFO, "USER PRESSED KEY DOWN");
		    switch (event.key.keysym.sym)
		    {
			case SDLK_j:
			{
				LOG(INFO, "USER PRESSED J");
				GOMovePaddle(&paddles[1], 20);
				LOG(INFO, "DEST RECT: %d\n", paddles[1].destRect.y);
				LOG(INFO, "DEST OBJECT HEIGHT: %d\n", paddles[1].srcRect.h);
				break;
			}
			case SDLK_f:
			{
				LOG(INFO, "USER PRESSED F");
				GOMovePaddle(&paddles[0], 20);
				break;
			}
			case SDLK_k:
			{
				LOG(INFO, "USER PRESSED K");
				GOMovePaddle(&paddles[1], -20);
				break;
			}
			case SDLK_d:
			{
				LOG(INFO, "USER PRESSED d");
				GOMovePaddle(&paddles[0], -20);
				break;
			}
			default: break;
		    }

	    }
	    default: break;
	}
}

void
update()
{
	GOUpdate(&net);
	Ball_Move(&ball, 1,1);

}

void
render()
{
	SDL_RenderClear(game.renderer);

	GORender(&net);
	GORender(&paddles[0]);
	GORender(&paddles[1]);
	GORender(&ball.object);

	SDL_RenderPresent(game.renderer);
}

void
clean()
{
	GODestroy(&net);
	GODestroy(&paddles[0]);
	GODestroy(&paddles[1]);
	GODestroy(&ball.object);

	SDL_DestroyWindow(game.window);
	SDL_DestroyRenderer(game.renderer);
	SDL_Quit();
	LOG(INFO, "GAME CLEANED");
}

SDL_Texture *
loadTexture(char * file_path)
{
	SDL_Surface * temp_surface = IMG_Load(file_path);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(game.renderer, temp_surface);

	SDL_FreeSurface(temp_surface);

	return texture;
}


GameObject *
GOInit(char * tex_path, SDL_Renderer * renderer, int x, int y, int w, int h)
{
	GameObject * object = malloc(sizeof(GameObject));
	object->renderer = renderer;
	object->texture  = loadTexture(tex_path);
	object->destRect.x = x;
	object->destRect.y = y;
	object->destRect.w = w;
	object->destRect.h = h;

	object->srcRect.h = h;
	object->srcRect.w = w;

	return object;
}

void
GODestroy(GameObject * object)
{
	SDL_DestroyRenderer(object->renderer);
	SDL_DestroyTexture(object->texture);
}

void
GOUpdate(GameObject * object)
{
	object->srcRect.x = 0;
	object->srcRect.y = 0;
	object->srcRect.h = 32;
	object->srcRect.w = 32;
}

void
GORender(GameObject * object)
{
	SDL_RenderCopy(object->renderer, object->texture, NULL, &object->destRect);
	
}

void
GOMovePaddle(GameObject * self, int y)
{
	int object_height = self->srcRect.h;
	int object_width  = self->srcRect.w;

	self->destRect.y += y;

	if (self->destRect.y + object_height > SCREEN_HEIGHT)
	{
		self->destRect.y = SCREEN_HEIGHT - object_height;
	}

	if (self->destRect.y < 0)
	{
		self->destRect.y = 0;
	}
}

/* TODO(tyler) left collision is not correct, but right is */
bool
GO_CheckCollision(GameObject * self, GameObject * other)
{
	int sXpos   = self->destRect.x;
	int sYpos   = self->destRect.y;
	int sWidth  = self->srcRect.w;
	int sHeight = self->srcRect.h;

	int oXpos   = other->destRect.x;
	int oYpos   = other->destRect.y;
	int oWidth  = other->srcRect.w;
	int oHeight = other->srcRect.h;

	bool xRightLap = sXpos + sWidth >= oXpos;
	bool xLeftLap  = sXpos <= oXpos + oWidth;
	bool yTopLap   = sYpos >= oYpos;
	bool yBotLap   = sYpos <= oYpos;

	bool rightCollision = (xRightLap && yTopLap) || (xRightLap && yBotLap);
	bool leftCollision  = (xLeftLap && yTopLap)  || (xLeftLap && yBotLap);
	return rightCollision;
}

void
Ball_Move(Ball * self, int x, int y)
{
	int ball_width = self->object.destRect.w;
	int ball_height = self->object.srcRect.h;

	self->object.destRect.x +=(x * self->xDirection);
	self->object.destRect.y += (y * self->yDirection);

	bool roob = self->object.destRect.x + ball_width > SCREEN_WIDTH;
	bool loob = self->object.destRect.x <= 0;
	/* Out of bounds */
	if (roob || loob)
	{
		self->object.destRect.x = BALL_CENTER_X;
		self->object.destRect.y = BALL_CENTER_Y;
	}

	/* Got to the bottom of screen */
	bool hit_bottom = self->object.destRect.y + ball_height > SCREEN_HEIGHT;
	bool hit_top = self->object.destRect.y < 0;

	if (hit_bottom || hit_top)
	{
		//self->xDirection*=-1;
		self->yDirection*=-1;
		
	}

	/* TODO(tyler) left collision detection in any manner doesn't work */
	bool leftCollision  = GO_CheckCollision(&ball.object, &paddles[0]);
	bool rightCollision = GO_CheckCollision(&ball.object, &paddles[1]);
	if (rightCollision)
	{
		self->xDirection*=-1;
	}
}

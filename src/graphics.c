#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "error.h"
#include "graphics.h"
#include "input.h"

// Blit a button on the main screen surface
int blit_button(struct s_context *cxt, const char *path, SDL_Rect pos, const char * text, const char *fpath)
{
	int t = strlen(text);

	SDL_Colour c;

	SDL_Rect txtpos;

	SDL_Surface *tmp;
	SDL_Surface *b = IMG_Load(path);

	TTF_Font *f = TTF_OpenFont(fpath, 36); // 16x16 pixels by character


	if(b == NULL)
	{
		log_error("Could not create button surface", "blit_button", IMG_GetError(), 0);
		return -1;
	}

	// Set text to fully opaque green-ish white (0xB0FEB0)
	c.r = 0xB0;
	c.g = 0xFE;
	c.b = 0xB0;
	c.a = 0xFE;

	tmp = TTF_RenderText_Blended(f, text, c);

	if(tmp == NULL)
	{
		log_error("Could not put text on button surface", "blit_button", TTF_GetError(), 0);
		return -1;
	}

	// The text must be less than 13 characters long
	// centralizes the string 'text' in the button
	txtpos.x = 105 - ((t * 16) / 2);
	txtpos.y = 30;

	SDL_BlitSurface(tmp, NULL, b, &txtpos);
	SDL_BlitSurface(b, NULL, cxt->screen, &pos);

	// Clean up the mess
	SDL_FreeSurface(tmp);
	SDL_FreeSurface(b);
	TTF_CloseFont(f);

	return 0;
}

int close_app(struct s_context *ctx)
{
	SDL_FreeSurface(ctx->screen);
	SDL_DestroyWindow(ctx->win);
	SDL_DestroyRenderer(ctx->ren);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}

int create_context(struct s_context *cxt, const char *title, const int sizex, const int sizey)
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		log_error("Could not initialize SDL", "create_context", SDL_GetError(), 1);
		close_app(cxt);
	}

	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		log_error("Could not initialize SDL_image", "create_context", IMG_GetError(), 1);
		close_app(cxt);
	}

	if(TTF_Init())
	{
		log_error("Could not initialize SDL_ttf", "create_context", TTF_GetError(), 1);
		close_app(cxt);
	}


	cxt->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizex, sizey, SDL_WINDOW_SHOWN);
	cxt->ren = SDL_CreateRenderer(cxt->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(cxt->win == NULL || cxt->ren == NULL)
	{
		log_error("Can't create window and/or renderer", "create_context", SDL_GetError(), 1);
		close_app(cxt);
	}

	// Set background as pale green (0x90EE90)
	cxt->screen = SDL_GetWindowSurface(cxt->win);
	SDL_FillRect(cxt->screen, NULL, SDL_MapRGB(cxt->screen->format, 0x90, 0xEE, 0x90));

	return 0;
}

int show(struct s_context cxt)
{
	SDL_Texture *tmp = SDL_CreateTextureFromSurface(cxt.ren, cxt.screen);

	SDL_RenderCopy(cxt.ren, tmp, NULL, NULL);

	SDL_RenderPresent(cxt.ren);

	SDL_DestroyTexture(tmp);

	return 0;
}

int blit_player(struct s_context *cxt, const int player, const int posx, const int posy)
{
	SDL_Rect position;
	SDL_Surface *mark;

	position.x = posx;
	position.y = posy;

	/* FIXME = Performance can be improved by loading the image once
	   and using it until the game is over, thus avoiding acessing
	   the disk every time a player makes a move */

	if(player == 1) // X
		mark = IMG_Load("res/x_100x100.png");

	else if(player == -1) // O
		mark = IMG_Load("res/o_100x100.png");

	else
		return -1; // the player parameter is invalid

	if(mark == NULL)
	{
		log_error("Coudn't load image file as surface", "blit_player", IMG_GetError(), 0);
		return -2; // Coudn't load resource
	}

	SDL_BlitSurface(mark, NULL, cxt->screen, &position);

	return 0;
}

int blit_text(struct s_context * cxt, const int size, const char *text, const char *fpath, SDL_Rect pos)
{
	SDL_Surface *tmp;

	SDL_Colour c;

	TTF_Font *f = TTF_OpenFont(fpath, size);

	// fully opaque pale green
	c.r = 0xB0;
	c.g = 0x00;
	c.b = 0xB0;
	c.a = 0xFF;

	// in case there's nothing to blit here
	if(strlen(text) == 0)
		return 0;

	tmp = TTF_RenderText_Blended(f, text, c);

	if(tmp == NULL)
	{
		log_error("Could not put text on temporary surface", "blit_text", TTF_GetError(), 0);
		return -3; // Coudn't render
	}

	SDL_BlitSurface(tmp, NULL, cxt->screen, &pos);

	SDL_FreeSurface(tmp);
	TTF_CloseFont(f);

	return 0;
}

int clear_screen(struct s_context * cxt)
{
	// Clean main surface
	cxt->screen = SDL_GetWindowSurface(cxt->win);
	SDL_FillRect(cxt->screen, NULL, SDL_MapRGB(cxt->screen->format, 0x90, 0xEE, 0x90));

	return 0;
}

int put_cursor(struct s_context * cxt, const int player)
{
	int posx, posy;

	SDL_Rect player_pos;

	// if player is o (-1), load o cursor, else loads x cursor
	SDL_Surface *player_surf = (player == -1) ? IMG_Load("res/cursor_o_16x16.png") : IMG_Load("res/cursor_x_16x16.png");

	if(player_surf == NULL)
	{
		log_error("Coudn't load image file as surface", "blit_cursor", IMG_GetError(), 0);
		return -1;
	}

	SDL_GetMouseState(&posx, &posy);

	if(posy < 110) // tests if cursor is out of the game space and is hidden
	{
		SDL_ShowCursor(1); // show cursor
		return 1;
	} else {
		SDL_ShowCursor(0);
	}

	player_pos.h = 16;
	player_pos.w = 16;
	player_pos.x = posx;
	player_pos.y = posy;

	SDL_BlitSurface(player_surf, NULL, cxt->screen, &player_pos);

	// clean up
	SDL_FreeSurface(player_surf);

	return 0;
}

int blit_element(struct s_context * cxt, const char * path, SDL_Rect pos)
{
	SDL_Surface *element = IMG_Load(path);

	if(element == NULL)
	{
		log_error("Coudn't load image file as surface", "blit_element", IMG_GetError(), 0);
		return -2; // Coudn't load resource
	}

	SDL_BlitSurface(element, NULL, cxt->screen, &pos);

	SDL_FreeSurface(element);

	return 0;
}

int show_error_window(const char * msg)
{
	SDL_Event errev;

	struct s_context errormsgcxt;

	SDL_Rect ok_b;
	SDL_Rect txt;

	txt.x = 130 - ((strlen(msg) * 10) / 2);
	txt.y = 20;

	ok_b.h = 30;
	ok_b.w = 50;
	ok_b.x = 100;
	ok_b.y = 60;

	errormsgcxt.win = SDL_CreateWindow("-ERROR-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 250, 100, SDL_WINDOW_SHOWN);
	errormsgcxt.ren = SDL_CreateRenderer(errormsgcxt.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	clear_screen(&errormsgcxt);

	blit_text(&errormsgcxt, 24, msg, "res/f.ttf", txt);

	blit_element(&errormsgcxt, "res/OK_50x30.png", ok_b);

	show(errormsgcxt);

	while(1)
	{
		SDL_PollEvent(&errev);

		if(clicked(errev, ok_b))
		{
			SDL_FreeSurface(errormsgcxt.screen);
			SDL_DestroyWindow(errormsgcxt.win);
			SDL_DestroyRenderer(errormsgcxt.ren);

			return 0;
		}
	}

	return 0;
}

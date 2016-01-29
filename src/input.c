
#include <string.h>
#include <SDL2/SDL.h>

#include "graphics.h"
#include "input.h"

int clicked(SDL_Event ev, SDL_Rect pos)
{
	int mx, my;

	SDL_GetMouseState(&mx, &my);

	if(ev.type == SDL_MOUSEBUTTONDOWN)
	{
		// test if the mouse is inside the buttons boundaries
		if((mx > pos.x && (mx < (pos.x + pos.w))) && (my > pos.y && (my < (pos.y + pos.h))))
		{
			wait_unpress(&ev); // wait until user releases the button
			return CLICKED;
		}
	}

	return NOT_CLICKED;
}

int get_mark_position(SDL_Event ev, int *posx, int *posy, int *gmx, int *gmy)
{
	if(ev.type == SDL_MOUSEBUTTONDOWN)
	{
		SDL_GetMouseState(posx, posy);

		// the if is to make sure the click was inside the boundaries of the game
		if((*posx > 140 && *posy > 110) && (*posx < 500 && *posy < 470))
		{
			// This calculates the position in the game matrix of the player's click
			*gmx = (*posx - 140) / 120;
			*gmy = (*posy - 110) / 120;
		} else {
			*gmx = -1;
			*gmy = -1;
		}
	}

	return 0;
}

int wait_unpress(SDL_Event *ev)
{
	// wait until the user has released the button
	SDL_Delay(1);

	// Sets the event as a mouse button unclicked
	ev->type = SDL_MOUSEBUTTONUP;

	return 0;
}

int get_text(struct s_context *cxt, const char * msg, char * input_text)
{
	int got_text = 0;

	char tmp[32] = {""};

	SDL_Rect txt;
	SDL_Rect input_box;

	SDL_Event ev;

	txt.x = 120;
	txt.y = 180;

	input_box.h = 40;
	input_box.w = 300;
	input_box.x = 170;
	input_box.y = 310;

	clear_screen(cxt);

	blit_text(cxt, 30, msg, "res/f.ttf", txt);

	while(!got_text)
	{
		SDL_FillRect(cxt->screen, &input_box, SDL_MapRGB(cxt->screen->format, 255, 255, 255));

		while(SDL_PollEvent(&ev))
		{
			const Uint8 *keybrd =  SDL_GetKeyboardState(NULL);

			if(keybrd[SDL_SCANCODE_BACKSPACE])
				tmp[strlen(tmp) - 1] = '\0';

			else if(keybrd[SDL_SCANCODE_RETURN])
				got_text = 1;

			if(ev.type == SDL_TEXTINPUT)
				strcat(tmp, ev.text.text);

			else if(ev.type == SDL_QUIT)
				return SDL_QUIT;
		}

		blit_text(cxt, 30, tmp, "res/f.ttf", input_box);

		show(*cxt);
	}

	sscanf(tmp, "%s", input_text);

	return 0;
}

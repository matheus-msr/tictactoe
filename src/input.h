#ifndef INPUT_H
#define INPUT_H

#include "graphics.h"

#define CLICKED 1
#define NOT_CLICKED 0

int clicked(SDL_Event ev, SDL_Rect pos);

int get_mark_position(SDL_Event ev, int *posx, int *posy, int *gmx, int *gmy);

int wait_unpress(SDL_Event * ev);

int get_text(struct s_context *cxt, char * msg, char * in_text);

#endif

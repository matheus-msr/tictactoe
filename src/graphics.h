#ifndef GRAPHICS_H
#define GRAPHICS_H

struct s_context
{
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Surface *screen;
};

int blit_button(struct s_context *cxt, char *path, SDL_Rect pos, char *text, char *fpath);

int close_app(struct s_context *ctx);

int create_context(struct s_context *cxt, char *title, int sizex, int sizey);

int show(struct s_context cxt);

int blit_player(struct s_context *cxt, int player, int posx, int posy);

int blit_text(struct s_context *cxt, int size, char *text, char *fpath, SDL_Rect pos);

int clear_screen(struct s_context *cxt);

int put_cursor(struct s_context *cxt, int player);

int blit_element(struct s_context *cxt, char * path, SDL_Rect pos);

int show_error_window(char * msg);

#endif

#ifndef GRAPHICS_H
#define GRAPHICS_H

struct s_context
{
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Surface *screen;
};

int blit_button(struct s_context * cxt, const char *path, SDL_Rect pos, const char * text, const char *fpath);

int close_app(struct s_context * ctx);

int create_context(struct s_context * cxt, const char *title, const int sizex, const int sizey);

int show(struct s_context cxt);

int blit_player(struct s_context * cxt, const int player, const int posx, const int posy);

int blit_text(struct s_context * cxt, const int size, const char * text, const char * fpath, SDL_Rect pos);

int clear_screen(struct s_context *cxt);

int put_cursor(struct s_context *cxt, const int player);

int blit_element(struct s_context *cxt, const char * path, SDL_Rect pos);

int show_error_window(const char * msg);

#endif

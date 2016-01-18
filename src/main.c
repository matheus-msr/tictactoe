#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "error.h"
#include "mechanics.h"
#include "input.h"
#include "graphics.h"
#include "network.h"

#define FRAMERATE 1000/60 // Limits to 60 frames per second max.
#define WINH 480 // window's height
#define WINW 640 // window's width
#define ON_LINE 1
#define OFF_LINE 2

int main(void)
{
	int quit = 0;
	int mainmenu = 1;
	int single = 0;
	int multi = 0;
	int online_opt = 0;
	int offline_opt = 0;
	int player = 0;
	int ai_player = 0;
	int online_player = 0;
	int mp_type = 0;
	int posx = 0; // position of where the mark is to be blitted in the grid
	int posy = 0;
	int gmx = -1; // position of where the game matrix is to be marked by the player
	int gmy = -1; // To avoid marking a space when first entering the game set both to -1
	int winner = 0;

	int game[3][3] = {{0,0,0},\
			  {0,0,0},\
			  {0,0,0}};

	char addr[16];
	char move[6];

	struct s_context context;

	struct s_socket conn;

	SDL_Surface *bkp;

	SDL_Event ev;

	// graphic elements and their dimensions and positions
	SDL_Rect sp;
	SDL_Rect mp;
	SDL_Rect exit;
	SDL_Rect back;
	SDL_Rect online;
	SDL_Rect offline;
	SDL_Rect host;
	SDL_Rect conn_to_host;
	SDL_Rect grid;
	SDL_Rect x;
	SDL_Rect o;
	SDL_Rect txt;

	grid.h = 360;
	grid.w = 360;
	grid.x = 140;
	grid.y = 115;

	mp.h = 100;
	mp.w = 200;
	mp.x = 430;
	mp.y = 10;

	sp.h = 100;
	sp.w = 200;
	sp.x = 10;
	sp.y = 10;

	exit.h = 100;
	exit.w = 200;
	exit.x = 220;
	exit.y = 10;

	back.h = 100;
	back.w = 200;
	back.x = 10;
	back.y = 10;

	online.h = 100;
	online.w = 200;
	online.x = 110;
	online.y = 200;

	offline.h = 100;
	offline.w = 200;
	offline.x = 330;
	offline.y = 200;

	host.h = 100;
	host.w = 200;
	host.x = 110;
	host.y = 200;

	conn_to_host.h = 100;
	conn_to_host.w = 200;
	conn_to_host.x = 330;
	conn_to_host.y = 200;

	x.h = 100;
	x.w = 100;

	o.h = 100;
	o.w = 100;

	create_context(&context, "Tic-Tac-Toe!", WINW, WINH);

	// Main game loop
	while(!quit)
	{
		SDL_PollEvent(&ev);

		// Limit frame rate
		SDL_Delay(FRAMERATE);

		// Local and online multiplayer and singleplayer mode
		if(multi || single)
		{
			if(player == ai_player && single) {
				ai_move(game, &gmx, &gmy);

			} else if(mp_type == ON_LINE && player == online_player) {
				receive_move(move, conn);
				sscanf(move, "%d %d", &gmx, &gmy);

			} else {
				// get were the game space was clicked
				get_mark_position(ev, &posx, &posy, &gmx, &gmy);
				if(mp_type == ON_LINE)
				{
					sprintf(move, "%d %d", gmx, gmy);
					send_move(move, conn);
				}
			}

			// copy original screen to temp surface so the cursor won't leave a trail across the screen
			bkp = SDL_ConvertSurface(context.screen, context.screen->format, 0);

			put_cursor(&context, player);

			// Update screen
			show(context);

			SDL_FreeSurface(context.screen);
			context.screen = SDL_ConvertSurface(bkp, bkp->format, 0);

			// delete temp surface (bkp becomes a pointer of type SDL_Surface that points to NULL)
			SDL_FreeSurface(bkp);

			// To avoid players from overwriting marked slots and that gmx and gmy aren't crazy or set with pre-game values
			if(!game[gmx][gmy] && (gmx < 3 || gmx >= 0) && (gmy < 3 || gmy >= 0))
			{
				game[gmx][gmy] = player;

				// these numbers were calculated to blit the mark in the top left corner of the clicked spot
				blit_player(&context, player, (gmx * 120) + 145, (gmy * 120) + 125);

				// Change current player to the opponent
				player = (player == X_DEF) ? O_DEF : X_DEF;

				winner = search_winner(game);

				if(winner)
				{
					// Update screen and wait 0.5 seconds so the player can see the last move
					show(context);
					SDL_Delay(500);

					SDL_ShowCursor(1);

					SDL_Rect pos;

					// Clean main surface
					clear_screen(&context);

					pos.x = 200;
					pos.y = 340;

					if(winner == 1)
						blit_text(&context, 48, "The X won!", "res/f.ttf", pos);

					else if(winner == -1)
						blit_text(&context, 48, "The O won!", "res/f.ttf", pos);

					else if(winner == 2)
						blit_text(&context, 48, "Draw!", "res/f.ttf", pos);

					blit_button(&context, "res/base_100x200.png", back, "BACK", "res/f.ttf");

					multi = 0;
					single = 0;

					// Update screen after blitting the winner
					show(context);
				}
			}
		}

		// Main menu
		if(mainmenu)
		{
			clear_screen(&context);

			blit_button(&context, "res/base_100x200.png", sp, "SINGLE PLAYER", "res/f.ttf");
			blit_button(&context, "res/base_100x200.png", exit, "EXIT", "res/f.ttf");
			blit_button(&context, "res/base_100x200.png", mp, "MULTI PLAYER", "res/f.ttf");

			// Update screen
			show(context);

			if(clicked(ev, sp)) {
				clear_screen(&context);

				ai_player = 0;
				player = 0;

				txt.x = 165;
				txt.y = 120;
				x.x = 190;
				x.y = 180;
				o.x = 320;
				o.y = 180;
				blit_text(&context, 44, "Choose your mark:", "res/f.ttf", txt);
				blit_element(&context, "res/x_100x100.png", x);
				blit_element(&context, "res/o_100x100.png", o);

				show(context);

				do {
					SDL_PollEvent(&ev);

					if(clicked(ev, x)){
						ai_player = O_DEF;
						player = X_DEF;
					} else if(clicked(ev, o)) {
						ai_player = X_DEF;
						player = O_DEF;
					} else if(ev.type == SDL_QUIT) {
						// go straight to exit without going through any more code
						goto exit_app;
					}

				} while(ai_player == 0);

				clear_screen(&context);

				blit_button(&context, "res/base_100x200.png", back, "BACK", "res/f.ttf");
				blit_button(&context, "res/base_100x200.png", exit, "EXIT", "res/f.ttf");
				blit_element(&context, "res/grid_360x360.png", grid);

				reset_game(game);
				mainmenu = 0;
				multi = 0;
				single = 1;

				gmx = -1;
				gmy = -1;

				wait_unpress(&ev);

			} else if(clicked(ev, mp)) {
				mainmenu = 0;
				single = 0;
				multi = 1;

				clear_screen(&context);

				txt.x = 200;
				txt.y = 130;

				blit_text(&context, 30, "ONLINE OR OFFLINE?", "res/f.ttf", txt);

				blit_button(&context, "res/base_100x200.png", online, "ON-LINE", "res/f.ttf");
				blit_button(&context, "res/base_100x200.png", offline, "OFF-LINE", "res/f.ttf");

				show(context);

				// defines if it's online or offline multiplayer
				do {
					SDL_PollEvent(&ev);

					if(clicked(ev, online)) {
						// mp_type becomes ON_LINE after a connection is sucessfully established
						online_opt = 1;

					} else if(clicked(ev, offline)) {
						mp_type = OFF_LINE;
						offline_opt = 1;

					} else if(ev.type == SDL_QUIT) {
						// go straight to exit without going through any more code
						goto exit_app;
					}

				} while(offline_opt == 0 && online_opt == 0);

				txt.x = 80;
				txt.y = 130;

				clear_screen(&context);

				blit_text(&context, 30, "HOST A MATCH OR CONNECT TO OTHER HOST?", "res/f.ttf", txt);

				blit_button(&context, "res/base_100x200.png", host, "BE HOST", "res/f.ttf");
				blit_button(&context, "res/base_100x200.png", conn_to_host, "CONNECT", "res/f.ttf");

				show(context);

				// defines which player will host the match
				do {
					wait_unpress(&ev);
					SDL_PollEvent(&ev);

					if(clicked(ev, host)) {
						clear_screen(&context);
						blit_text(&context, 30, "PLEASE WAIT, CONNECTION BEING ESTABLISHED...", "res/f.ttf", txt);

						show(context);

						if(host_connection(&conn) < 0)
						{
							show_error_window("Error hosting match!");
							goto exit_app;
						}

						mp_type = ON_LINE;

						online_player = X_DEF;

					} else if(clicked(ev, conn_to_host)) {
						if(get_text(&context, "TYPE THE IP ADDRESS OF THE HOST:", addr) == SDL_QUIT)
							goto exit_app;

						clear_screen(&context);
						blit_text(&context, 30, "PLEASE WAIT, CONNECTION BEING ESTABLISHED...", "res/f.ttf", txt);

						if(connect_to_host(addr, &conn) < 0)
						{
							show_error_window("Error connecting to host!");
							goto exit_app;
						}

						if(ev.type == SDL_QUIT)
							goto exit_app;

						mp_type = ON_LINE;

						online_player = O_DEF;

					} else if(ev.type == SDL_QUIT) {
						// go straight to exit without going through any more code
						goto exit_app;
					}


					show(context);

				} while(mp_type != ON_LINE && online_opt == 1);

				clear_screen(&context);

				blit_button(&context, "res/base_100x200.png", back, "BACK", "res/f.ttf");
				blit_button(&context, "res/base_100x200.png", exit, "EXIT", "res/f.ttf");

				blit_element(&context, "res/grid_360x360.png", grid);

				show(context);

				reset_game(game);

				gmx = -1;
				gmy = -1;

				player = X_DEF;

				wait_unpress(&ev);
			}
		}

		// Handle back button
		if(clicked(ev, back))
		{
			clear_screen(&context);

			// clear variables related to game flow and set main_menu
			single = 0;
			multi = 0;
			offline_opt = 0;
			online_opt = 0;
			online_player = 0;
			mp_type = 0;
			mainmenu = 1;

			wait_unpress(&ev);
		}

		// Handle exit button
		if(clicked(ev, exit) || ev.type == SDL_QUIT)
			quit = 1;
	}

exit_app:
	if(mp_type == ON_LINE)
		close_connection(conn);

	close_app(&context);

	return 0;
}

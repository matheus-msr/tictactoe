#ifndef MECHANICS_H
#define MECHANICS_H

#define X_DEF 1
#define O_DEF -1
#define OLDLADY 2

int search_winner(int game[3][3]);

int reset_game(int game[3][3]);

int ai_move(int game[3][3], int * gmx, int * gmy);

#endif

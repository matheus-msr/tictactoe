#include <stdlib.h>
#include <time.h>

#include "mechanics.h"

int search_winner(int game[3][3])
{
	int a, b; // GP counters to use in the 'for's
	int moves = 0; // counts how many moves have been made in the game
	int vic[8] = {0,0,0,0,0,0,0,0}; // holds the sum of every sequence that can win the game

	for(a = 0; a < 3; a++)
	{
		vic[6] += game[a][a]; // adds diagonal
		vic[7] += game[a][2-a]; // adds other diagonal

		for(b = 0; b < 3; b++)
		{
			vic[a] += game[a][b]; // adds lines
			vic[a+3] += game[b][a]; // adds columns

			// Adds how many moves have been made by the players
			if(game[a][b] != 0)
				moves++;
		}
	}

	// search for winner sequences filled by the same player
	for(a = 0; a < 8; a++)
	{
		if(vic[a] == (3 * X_DEF))
			return X_DEF;

		else if(vic[a] == (3 * O_DEF))
			return O_DEF;
	}

	if(moves == 9)
		return OLDLADY;


	// when nothing important has happened
	return 0;
}

int reset_game(int game[3][3])
{
	int x, y;

	for(x = 0; x < 3; x++)
		for(y = 0; y < 3; y++)
			game[x][y] = 0;

	return 0;
}

int ai_move(int game[3][3], int * gmx, int * gmy)
{
	srand(time(NULL)); // Random seed

	do {
		*gmx = rand() % 3;
		*gmy = rand() % 3;

	} while(game[*gmx][*gmy] != 0);

	return 0;
}

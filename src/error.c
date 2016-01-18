#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "error.h"

void log_error(char *type, char *function, const char *sdlerror, int fatal)
{
	if(fatal == 0)
		fprintf(stderr, "[WARNING] %s: %s (SDL: %s)\n", function, type, sdlerror);
	else if(fatal == 1)
		fprintf(stderr, "[ERROR] %s: %s (SDL: %s)\n- CLOSING APPLICATION -\n", function, type, sdlerror);
}

void log_net_error(char *type, int fatal)
{
	if(fatal == 0)
		fprintf(stderr, "[WARNING] networking error: %s\n", type);
	else if(fatal == 1)
		fprintf(stderr, "[ERROR] networking error: %s\n- CLOSING CONNECTION -\n", type);
}

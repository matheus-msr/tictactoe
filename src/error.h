#ifndef ERROR_H
#define ERROR_H

#define GRFERR -1
#define NETERR -2
#define SNDERR -3

void log_error(char *type, char *function, const char *sdlerror, int fatal);

void log_net_error(char *type, int fatal);

#endif

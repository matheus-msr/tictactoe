#ifndef ERROR_H
#define ERROR_H

#define GRFERR -1
#define NETERR -2
#define SNDERR -3

void log_error(const char *type, const char *function, const char *sdlerror, const int fatal);

void log_net_error(const char *type, const int fatal);

#endif

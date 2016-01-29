#ifndef NETWORK_LINUX_H
#define NETWORK_LINUX_H

struct s_socket {
	// file descriptor for linux implementation
	int fd;
};

int host_connection(struct s_socket * connection);

int connect_to_host(const char * address, struct s_socket * connection);

int send_move(const char * move, const struct s_socket connection);

int receive_move(char * move, const struct s_socket connection);

int close_connection(const struct s_socket connection);

#endif

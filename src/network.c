#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "error.h"
#include "network.h"

#define PORT 46000

int host_connection(struct s_socket * connection)
{
	int init_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	static socklen_t cli_size;

	struct sockaddr_in host;
	static struct sockaddr_in client;

	host.sin_family = AF_INET;
	host.sin_port = htons(PORT);
	host.sin_addr.s_addr = INADDR_ANY;

	// binds socket to a port and wait for connections
	if(bind(init_sock_fd, (struct sockaddr *) &host, sizeof(host)) < 0 || listen(init_sock_fd, 5) < 0)
	{
		log_net_error("Coudn't bind to port and wait for connections, try checking your firewall permissions", 1);
		close(init_sock_fd);
		return -1;
	}

	// accept connection
	connection->fd = accept(init_sock_fd, (struct sockaddr *) &client, &cli_size);

	if(connection->fd < 0)
	{
		log_net_error("coudn't accept connection", 1);
		close(init_sock_fd);
		return -1;
	}

	return 0;
}

int connect_to_host(const char * address, struct s_socket * connection)
{
	struct sockaddr_in host;

	host.sin_family = AF_INET;
	host.sin_port = htons(PORT);
	host.sin_addr.s_addr = inet_addr(address);

	connection->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(connection->fd, (struct sockaddr*) &host, sizeof(host)) < 0)
	{
		log_net_error("Coudn't connect to host, check host's address and/or firewall permissions", 1);
		close_connection(*connection);
		return -1;
	}

	return 0;
}

int send_move(const char * move, const struct s_socket connection)
{
	if(write(connection.fd, move, strlen(move)) < 0)
		log_net_error("Coudn't send move, check your connection", 0);

	return 0;
}

int receive_move(char * move, const struct s_socket connection)
{
//	move = {""};

	if(read(connection.fd, move, 32) < 0)
	{
		log_net_error("Coudn't receive move, check your connection", 0);
		return -1;
	}

	return 0;
}

int close_connection(const struct s_socket connection)
{
	close(connection.fd);

	return 0;
}

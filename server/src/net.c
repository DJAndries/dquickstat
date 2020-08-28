#include "net.h"
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 4020

static int server_fd = -1;
static int curr_cli_fd = -1;

int init_net() {
	struct sockaddr_in maddr;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		dlog(LOG_ERROR, "Failed to create server socket");
		return 1;
	}

	memset(&maddr, 0, sizeof(struct sockaddr_in));
	maddr.sin_family = AF_INET;
	maddr.sin_port = htons(4020);
	maddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_fd, (struct sockaddr*)&maddr, sizeof(struct sockaddr)) == -1) {
		dlog(LOG_ERROR, "Failed to bind on port %u", PORT);
		close(server_fd);
		return 2;
	}

	if (listen(server_fd, 1) == -1) {
		dlog(LOG_ERROR, "Failed to listen on port %u", PORT);
		close(server_fd);
		return 3;
	}

	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	dlog(LOG_DEBUG, "Listening on port %u", PORT);

	return 0;
}

void close_net() {
	if (curr_cli_fd != -1) close(curr_cli_fd);
	if (server_fd != -1) close(server_fd);
	server_fd = curr_cli_fd = -1;
}

int net_get_data(comp_data* in_data) {
	struct sockaddr_in caddr;
	size_t sin_size = sizeof(struct sockaddr_in);
	size_t bytes_read;
	comp_data temp;

	if (curr_cli_fd == -1) {
		if ((curr_cli_fd = accept(server_fd, (struct sockaddr*)&caddr,
				&sin_size)) == -1) {
			return 1;
		}
		fcntl(curr_cli_fd, F_SETFL, O_NONBLOCK);
		dlog(LOG_DEBUG, "Accepted connection");
	}

	while ((bytes_read = recv(curr_cli_fd, &temp, sizeof(comp_data), MSG_PEEK)) == sizeof(comp_data)) {
		recv(curr_cli_fd, in_data, sizeof(comp_data), 0);
	}

	if (send(curr_cli_fd, "k", 1, MSG_NOSIGNAL) != 1) {
		dlog(LOG_DEBUG, "Closed connection");
		close(curr_cli_fd);
		curr_cli_fd = -1;
		return 2;
	}

	return 0;
}

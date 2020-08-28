#include "net.h"
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 4020

static int client_fd = -1;

int connect_net(const char* ip) {
	struct sockaddr_in maddr;

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		dlog(LOG_ERROR, "Failed to create client socket");
		return 1;
	}

	memset(&maddr, 0, sizeof(struct sockaddr_in));
	maddr.sin_family = AF_INET;
	maddr.sin_port = htons(4020);
	maddr.sin_addr.s_addr = inet_addr(ip);

	if (connect(client_fd, (struct sockaddr*)&maddr, sizeof(struct sockaddr)) == -1) {
		dlog(LOG_ERROR, "Failed to connect to %s:%u", ip, PORT);
		close(client_fd);
		client_fd = -1;
		return 2;
	}

	dlog(LOG_DEBUG, "Connected to %s:%u", ip, PORT);

	return 0;
}

void close_net() {
	if (client_fd != -1) close(client_fd);
	client_fd = -1;
}

int send_data(comp_data* out_data) {
	char ka_temp[128];
	if (send(client_fd, out_data, sizeof(comp_data), MSG_NOSIGNAL) != sizeof(comp_data)) {
		dlog(LOG_ERROR, "Failed to send data");
		close(client_fd);
		client_fd = -1;
		return 2;
	}

	/* recv our keep alives */
	recv(client_fd, ka_temp, 128, MSG_DONTWAIT);

	return 0;
}

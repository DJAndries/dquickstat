#include <stdio.h>
#include <unistd.h>
#include "net.h"
#include "comp_data.h"
#include "info.h"

#include <string.h>

int main(int argc, const char** argv) {
	comp_data cdata;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <server ip address>\n", argv[0]);
		return 1;
	}

	if (connect_net(argv[1])) {
		return 2;
	}
	init_info();


	while (1) {
		memset(&cdata, 0, sizeof(comp_data));
		if (query_cpu(&cdata) || query_mem(&cdata)) {
			return 4;
		}
		/* Ignore errors for reading thermals */
		query_thermal(&cdata);

		if (send_data(&cdata)) {
			return 3;
		}

		usleep(1100 * 1000);
	}

	close_net();
	return 0;
}

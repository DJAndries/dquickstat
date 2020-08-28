#include <stdio.h>
#include <unistd.h>
#include "fb.h"
#include "net.h"
#include "comp_data.h"

int main(int argc, const char** argv) {
	const char def_device[] = "/dev/fb1";
	comp_data cdata;

	if (init_fb(argc > 1 ? argv[1] : def_device)) {
		return 1;
	}
	if (init_net()) {
		return 2;
	}

	while (1) {
		if (net_get_data(&cdata) == 0) {
			draw_frame(&cdata);
		}

		usleep(500 * 1000);
	}

	free_fb();
	close_net();
	return 0;
}

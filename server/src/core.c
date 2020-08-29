#include <stdio.h>
#include <unistd.h>
#include "fb.h"
#include "net.h"
#include "comp_data.h"
#include "backlight.h"

int main(int argc, const char** argv) {
	const char def_device[] = "/dev/fb1";
	comp_data cdata;
	int data_result;

	if (init_fb(argc > 1 ? argv[1] : def_device)) {
		return 1;
	}
	if (init_net()) {
		return 2;
	}

	switch_backlight("1");

	while (1) {
		data_result = net_get_data(&cdata);
		switch(data_result) {
			case NET_CONN_CLOSE:
				switch_backlight("1");
				break;
			case NET_MSG:
				draw_frame(&cdata);
			case NET_NEW_CONN:
				switch_backlight("0");
		}

		usleep(950 * 1000);
	}

	free_fb();
	close_net();
	return 0;
}

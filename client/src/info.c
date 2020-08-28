#include "info.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>

unsigned long long last_cpu_info[8][2];

void init_info() {
	memset(last_cpu_info, 0, sizeof(unsigned long long) * 8 * 2);
}

int query_cpu(comp_data* data) {
	size_t i;
	char temp[1024];
	unsigned long long a, b, c, used, idle;
	double interval_total, interval_used;

	FILE* stat_file = fopen("/proc/stat", "r");

	if (stat_file == 0) {
		dlog(LOG_ERROR, "Failed to query /proc/stat file");
		return 1;
	}

	/* skip first line */
	fgets(temp, 1024, stat_file);

	for (i = 0; i < 8; i++) {
		if (fscanf(stat_file, "%s %llu %llu %llu %llu",
					temp, &a, &b, &c, &idle) != 5) {
			dlog(LOG_ERROR, "Failed to read line in /proc/stat file");
			fclose(stat_file);
			return 2;
		}

		if (memcmp(temp, "cpu", 3) != 0) {
			break;
		}

		used = a + b + c;

		if (last_cpu_info[i][0] > 0) {
			interval_used = used - last_cpu_info[i][0];
			interval_total = (used + idle) - (last_cpu_info[i][0] +
					last_cpu_info[i][1]);

			if (interval_total > 0) {
				data->numrows[i] = (unsigned)(interval_used /
						interval_total * 100.0);
			}
		}

		last_cpu_info[i][0] = used;
		last_cpu_info[i][1] = idle;
	}

	fclose(stat_file);
	return 0;
}

int query_mem(comp_data* data) {
	struct sysinfo info;

	if (sysinfo(&info)) {
		dlog(LOG_ERROR, "Failed to query sysinfo");
		return 1;
	}
	
	data->numrows[ID_TOTALMEM] = info.totalram;
	data->numrows[ID_USEDMEM] = info.totalram - info.freeram - info.bufferram;
	data->numrows[ID_TOTALSWAP] = info.totalswap;
	data->numrows[ID_USEDSWAP] = info.totalswap - info.freeswap;
	return 0;
}

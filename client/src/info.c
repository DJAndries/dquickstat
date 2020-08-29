#include "info.h"
#include "log.h"
#include <stdio.h>
#include <string.h>

unsigned long long last_cpu_info[8][2];

void init_info() {
	memset(last_cpu_info, 0, sizeof(unsigned long long) * 8 * 2);
}

int query_cpu(comp_data* data) {
	size_t i;
	char temp[1024];
	char item[64];
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
		fgets(temp, 1024, stat_file);
		if (sscanf(temp, "%s %llu %llu %llu %llu",
					item, &a, &b, &c, &idle) != 5) {
			dlog(LOG_ERROR, "Failed to read line in /proc/stat file");
			fclose(stat_file);
			return 2;
		}

		if (memcmp(item, "cpu", 3) != 0) {
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
	char cmd[64];
	unsigned long long value;
	unsigned long long totalram, availram, totalswap, freeswap;
	FILE* info_file = fopen("/proc/meminfo", "r");

	if (info_file == 0) {
		dlog(LOG_ERROR, "Failed to query sysinfo");
		return 1;
	}

	totalram = availram = totalswap = freeswap = 0;

	while (fscanf(info_file, "%s %llu kB\n", cmd, &value) == 2) {
		if (totalram && availram && totalswap && freeswap)
			break;

		if (!totalram && strcmp(cmd, "MemTotal:") == 0) {
			totalram = value * 1024;
		}
		if (!availram && strcmp(cmd, "MemAvailable:") == 0) {
			availram = value * 1024;
		}
		if (!totalswap && strcmp(cmd, "SwapTotal:") == 0) {
			totalswap = value * 1024;
		}
		if (!freeswap && strcmp(cmd, "SwapFree:") == 0) {
			freeswap = value * 1024;
		}
	}
	
	data->numrows[ID_TOTALMEM] = totalram;
	data->numrows[ID_USEDMEM] = totalram - availram;
	data->numrows[ID_TOTALSWAP] = totalswap;
	data->numrows[ID_USEDSWAP] = totalswap - freeswap;

	fclose(info_file);
	return 0;
}

int query_thermal(comp_data* data) {
	FILE* fd;
	unsigned long temp;

	fd = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if (fd == 0) return 1;

	if (fscanf(fd, "%lu", &temp) == 1) {
		data->numrows[ID_CPUTEMP] = temp;
	}
	fclose(fd);

	/* Hardcoded assumption for gpu pci mapping */
	fd = fopen("/sys/devices/pci0000:00/0000:00:01.0/0000:01:00.0/hwmon/hwmon1/temp1_input", "r");
	if (fd == 0) return 2;

	if (fscanf(fd, "%lu", &temp) == 1) {
		data->numrows[ID_GPUTEMP] = temp;
	}
	fclose(fd);

	return 0;
}

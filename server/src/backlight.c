#include "backlight.h"
#include <dirent.h>
#include <stdio.h>

int switch_backlight(const char* off_mode) {
	DIR* dirp;
	struct dirent* dp;
	FILE* bl_file;
	char bl_filename[512];
	bl_filename[0] = 0;
	
	if ((dirp = opendir("/sys/class/backlight")) == 0) {
		return 1;
	}

	while ((dp = readdir(dirp)) != 0) {
		sprintf(bl_filename, "/sys/class/backlight/%s/bl_power", dp->d_name);
	}

	closedir(dirp);

	if (bl_filename[0] == 0) {
		return 2;
	}

	if ((bl_file = fopen(bl_filename, "w")) == 0) {
		return 3;
	}

	fwrite(off_mode, sizeof(char), 1, bl_file);
	fclose(bl_file);
	return 0;
}

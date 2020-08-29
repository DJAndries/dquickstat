#include "fbmagic/fbmagic.h"
#include <stdio.h>
#include <stdlib.h>
#include "comp_data.h"

#define MB_BYTES 1048576

static fbmagic_ctx* fb_ctx;
static fbmagic_image* bg;
static fbmagic_font* font;

int init_fb(const char* device) {
	char* path_prefix = getenv("RESOURCE_PATH_PREFIX");
	char res_prefix[256];
	char filename[300];
	if (path_prefix == 0) {
		sprintf(res_prefix, "res/");
	} else {
		sprintf(res_prefix, "%sres/", path_prefix);
	}

	if ((fb_ctx = fbmagic_init(device)) == 0) {
		return 1;
	}

	sprintf(filename, "%s%s", res_prefix, "bg.bmp");
	if ((bg = fbmagic_load_bmp(fb_ctx, filename)) == 0) {
		fbmagic_exit(fb_ctx);
		return 2;
	}

	sprintf(filename, "%s%s", res_prefix, "gohufont-11.bdf");
	if ((font = fbmagic_load_bdf(filename)) == 0) {
		fbmagic_exit(fb_ctx);
		fbmagic_free_image(bg);
		return 3;
	}

	return 0;
}

void free_fb() {
	fbmagic_free_image(bg);
	free(font);
	fbmagic_exit(fb_ctx);
}

static size_t draw_cpu(comp_data* data, size_t x, size_t y, uint32_t main_color, uint32_t disabled_color) {
	size_t i;
	size_t val;
	char label[256];

	for (i = 0; i < 8; i++) {
		val = (size_t)(((float)data->numrows[i] / 100) * 80);
		x = (i % 2 == 0) ? 20 : 165;
		sprintf(label, "CPU %u:", i);
		fbmagic_draw_text(fb_ctx, font, x, y, label,
				val == 0 ? disabled_color : main_color, 1.5f);
		fbmagic_stroke(fb_ctx, x + 55, y + 3, 80, 7, 1,
				val == 0 ? disabled_color : main_color);
		fbmagic_fill(fb_ctx, x + 55, y + 3, val, 7, main_color);

		if (i % 2 == 1) y += 20;
	}

	return y;
}

static size_t draw_memory(comp_data* data, size_t x, size_t y, uint32_t main_color) {
	size_t total_mem_mb = data->numrows[ID_TOTALMEM] / MB_BYTES;
	size_t used_mem_mb = data->numrows[ID_USEDMEM] / MB_BYTES;
	size_t total_swap_mb = data->numrows[ID_TOTALSWAP] / MB_BYTES;
	size_t used_swap_mb = data->numrows[ID_USEDSWAP] / MB_BYTES;
	char memstr[256];
	char swapstr[256];
	size_t val;

	sprintf(memstr, "Memory: %u MB / %u MB", used_mem_mb, total_mem_mb);
	sprintf(swapstr, "Swap: %u MB / %u MB", used_swap_mb, total_swap_mb);

	val = total_mem_mb == 0 ? 0 : (size_t)(((float)used_mem_mb / total_mem_mb) * 280);
	fbmagic_draw_text(fb_ctx, font, x, y, memstr,
				main_color, 1.5f);
	fbmagic_stroke(fb_ctx, x, y + 20, 280, 7, 1, main_color);
	fbmagic_fill(fb_ctx, x, y + 20, val, 7,
			main_color);
	y += 32;

	val = total_swap_mb == 0 ? 0 : (size_t)(((float)used_swap_mb / total_swap_mb) * 280);
	fbmagic_draw_text(fb_ctx, font, x, y, swapstr,
				main_color, 1.5f);
	fbmagic_stroke(fb_ctx, x, y + 20, 280, 7, 1, main_color);
	fbmagic_fill(fb_ctx, x, y + 20, val, 7,
			main_color);
	y += 32;

	return y;
}

size_t draw_thermal(comp_data* data, size_t x, size_t y, uint32_t main_color) {
	double cpu_c, gpu_c;
	char cpu_label[64];
	char gpu_label[64];
	
	cpu_c = data->numrows[ID_CPUTEMP] / 1000.0;
	gpu_c = data->numrows[ID_GPUTEMP] / 1000.0;

	sprintf(cpu_label, "CPU Temp: %.1lfC", cpu_c);
	sprintf(gpu_label, "GPU Temp: %.1lfC", gpu_c);

	fbmagic_draw_text(fb_ctx, font, x, y, cpu_label,
			main_color, 1.5f);
	x += 150;

	fbmagic_draw_text(fb_ctx, font, x, y, gpu_label,
			main_color, 1.5f);
	
	return y + 32;
}

void draw_frame(comp_data* data) {
	size_t x, y;
	uint32_t main_color = fbmagic_color_value(fb_ctx, 0x0, 0x02, 0x59);
	uint32_t disabled_color = fbmagic_color_value(fb_ctx, 0x90, 0x90, 0x90);
	fbmagic_draw_image_quick(fb_ctx, 0, 0, bg);

	x = 20;
	y = 20;
	
	y = draw_cpu(data, x, y, main_color, disabled_color);
	y = draw_memory(data, x, y, main_color);
	y = draw_thermal(data, x, y + 8, main_color);

	fbmagic_flush(fb_ctx);
}

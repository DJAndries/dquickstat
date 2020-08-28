#ifndef COMP_DATA_H
#define COMP_DATA_H

#define ID_CPU0 0
#define ID_CPU1 1
#define ID_CPU2 2
#define ID_CPU3 3
#define ID_CPU4 4
#define ID_CPU5 5
#define ID_CPU6 6
#define ID_CPU7 7

#define ID_TOTALMEM 8
#define ID_USEDMEM 9
#define ID_TOTALSWAP 10
#define ID_USEDSWAP 11

#define TOTAL_NUMS 12
#include <stdint.h>

struct comp_data {
	uint64_t numrows[TOTAL_NUMS];
};
typedef struct comp_data comp_data;

#endif

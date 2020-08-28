#ifndef NET_H
#define NET_H

#include "comp_data.h"

int init_net();
void close_net();
int net_get_data(comp_data* in_data);

#endif

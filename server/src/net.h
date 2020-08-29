#ifndef NET_H
#define NET_H

#include "comp_data.h"

#define NET_MSG 0
#define NET_NEW_CONN 1
#define NET_CONN_CLOSE 2

int init_net();
void close_net();
int net_get_data(comp_data* in_data);

#endif

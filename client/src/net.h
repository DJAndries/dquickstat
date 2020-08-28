#ifndef NET_H
#define NET_H

#include "comp_data.h"

int connect_net(const char* ip);
void close_net();
int send_data(comp_data* out_data);

#endif

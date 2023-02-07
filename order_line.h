#include "order.h"
#include "status.h"

struct order_line {
    struct order orders[8];
    struct status *status;
};

void add(){};
void remove(){};
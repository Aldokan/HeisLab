#include "order.h"
#include "status.h"

struct order_line {
    struct order *orders[8]; //linked list er nok mer hensiktsmessig for å få en dynamisk array
    struct status *status;
};

void add_order(struct order_line o);
void remove_order(struct order_line o);

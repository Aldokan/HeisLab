#include "status.h"
#include "order_line.h"

struct control {
    struct order_line *order_line;
    struct status *status;
};

void move(struct control c);
void stop(struct control c);


#include "order.h"
#include "status.h"

struct order_line {
    struct node {
        struct order *data;
        struct node *next;
    };
};

void add_order(struct order_line *o, struct node** head_ref, struct order *new_data, struct status *s);
void remove_order(struct order_line o, struct node** head_ref, int key);

#include "order.h"
#include "status.h"

struct order_line {
    struct order* data;
    struct order_line* next;
};

void add_order(struct order_line** head_ref, struct order *new_data, struct status *s);
void remove_order(struct order_line** head_ref, int key);
void clear_line(struct order_line** head_ref);

void sort_line(struct order_line** head_ref, struct status *s);
void pretty_print_line(struct order_line** head_ref);
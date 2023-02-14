#include "order_line.h"

void add_order(struct order_line** head_ref, struct order *new_data, struct status *s) {
    struct order_line* new_order_line = (struct order_line*) malloc(sizeof(struct order_line));
    struct order_line *last = *head_ref;
    new_order_line->data = (struct order*) malloc(sizeof(struct order));
    new_order_line->data->src  = new_data->src;
    new_order_line->data->to_floor  = new_data->to_floor;
    new_order_line->next = NULL;
    if (*head_ref == NULL)
    {
       *head_ref = new_order_line;
       return;
    }  
    while (last->next != NULL)
        last = last->next;

    last->next = new_order_line;   
};

void remove_order(struct order_line** head_ref, int key) {
    struct order_line* temp = *head_ref, *prev;
    if (temp != NULL && temp->data->to_floor == key) {
        *head_ref = temp->next; 
        free(temp);
        return;
    }
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    free(temp);
}


#include "order_line.h"

void add_order(struct order_line** head_ref, struct order *new_data, struct status *s) {
    struct order_line* new_order_line = (struct order_line*) malloc(sizeof(struct order_line));
    struct order_line *last = *head_ref;
    new_order_line->data = (struct order*) malloc(sizeof(struct order));
    new_order_line->data->src  = new_data->src;
    new_order_line->data->to_floor  = new_data->to_floor;
    new_order_line->data->dir = new_data->dir;
    new_order_line->next = NULL;
    if ((*head_ref)->data == NULL)
    {
       *head_ref = new_order_line;
       return;
    }  
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_order_line;   
};

void remove_order(struct order_line** head_ref, int key) {
    struct order_line* temp = *head_ref, *prev;
    if (temp != NULL && temp->data->to_floor == key) {
        if (temp->next==NULL) {
            temp->data=NULL;
            return;
        }
        *head_ref = temp->next; 
        free(temp);
        return;
    }
    while (temp != NULL && temp->data->to_floor != key) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    free(temp);
}

void clear_line(struct order_line** head_ref) {
    struct order_line* current = *head_ref;
    struct order_line* next;

    while (current != NULL) {
        if (current->next == NULL) {
            current->data = NULL;
            return;
        }
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

void sort_line(struct order_line** head_ref, struct status *s) {
    struct order_line *current = *head_ref;
    struct order_line *closest = (struct order_line*) malloc(sizeof(struct order_line));
    struct order* swap;
    //traverse linked list
    while (current != NULL) {
        closest = current;
        while (closest->next != NULL) {
            if (abs((closest->data->to_floor) - (s->current_floor)) > abs((closest->next->data->to_floor) - (s->current_floor))) {
                swap = closest->data;
                closest->data = closest->next->data;
                closest->next->data = swap;
            }
            closest = closest->next;
        }
        current = current->next;
    }
    enum direction dir;
    if (closest->data->dir == irrelevant) {
        dir = ((closest->data->to_floor) - (s->current_floor)) < 0? down: up;
    } else {
        dir = closest->data->dir;
    }
    while (current != NULL) {
        closest = current;
        while (closest->next != NULL) {
            if (closest->data->dir != dir) {
                swap = closest->data;
                closest->data = closest->next->data;
                closest->next->data = swap;
            }
            closest = closest->next;
        }
        current = current->next;
    } 
     
}

void pretty_print_line(struct order_line** head_ref) {
    struct order_line* current = (struct order_line*) malloc(sizeof(struct order_line));
    int i = 1;
    printf("--------------------------------------\n");
    for (current = *head_ref; current != NULL; current=(current->next)) {
        printf("%d: To floor: %d    Source of order: %s\n", i, current->data->to_floor, (current->data->src == outside_elevator? "outside": "inside"));
        i++;
    }
    printf("--------------------------------------\n");
}

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

void remove_order(struct order_line** head_ref, int floor) {
    struct order_line* temp;
    while (*head_ref != NULL && (*head_ref)->data->to_floor == floor) {
        temp = *head_ref;
        *head_ref = (*head_ref)->next;
        free(temp);
    }

    struct order_line* current = *head_ref;
    while (current != NULL) {
        if (current->next != NULL && current->next->data->to_floor == floor) {
            temp = current->next;
            current->next = temp->next;
            free(temp);
        } else {
            current = current->next;
        }
    }

    if (*head_ref == NULL) {
        *head_ref = (struct order_line*) malloc(sizeof(struct order_line));
        (*head_ref)->next = NULL;
        (*head_ref)->data = NULL;
    }
}

void clear_line(struct order_line** head_ref) {
    struct order_line* temp;

    while (*head_ref != NULL) {
        temp = *head_ref;
        *head_ref = (*head_ref)->next;
        free(temp);
    }
    *head_ref = NULL;
    *head_ref = (struct order_line*) malloc(sizeof(struct order_line));
    (*head_ref)->next = NULL;
    (*head_ref)->data = NULL;
}

void sort_line(struct order_line** head_ref, struct status *s) {
    struct order_line *current = *head_ref;
    struct order_line *closest = (struct order_line*) malloc(sizeof(struct order_line));
    struct order* swap;

    while (current != NULL) {
        closest = current;
        while (closest->next != NULL) {
            if (abs((closest->data->to_floor) - (s->current_floor)) > abs((closest->next->data->to_floor) - (s->current_floor))
            && (closest->next->data->dir == 0 || closest->next->data->dir == s->last_direction)) {
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
        printf("%d: To floor: %d    Source of order: %s    Direction: %s\n", i, current->data->to_floor, (current->data->src == outside_elevator? "outside": "inside"),
        current->data->dir == DIRN_UP? "upwards": current->data->dir == DIRN_DOWN? "downwards": "irrelevant");
        i++;
    }
    printf("--------------------------------------\n");
}

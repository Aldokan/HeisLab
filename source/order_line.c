#include "order_line.h"

void add_order(struct order_line** head_ref, struct order *new_data, struct status *s) {
    struct order_line* new_order_line = (struct order_line*) malloc(sizeof(struct order_line));
    struct order_line *last = *head_ref;
    new_order_line->data = (struct order*) malloc(sizeof(struct order));
    new_order_line->data->src  = new_data->src;
    new_order_line->data->to_floor  = new_data->to_floor;
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
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

void sort_line(struct order_line** head_ref, struct status *s) {
    struct order_line *current = (struct order_line*) malloc(sizeof(struct order_line));
    struct order_line *closest = (struct order_line*) malloc(sizeof(struct order_line));
    struct order_line *prev = (struct order_line*) malloc(sizeof(struct order_line));
    struct order_line *closest_prev = (struct order_line*) malloc(sizeof(struct order_line));

    //traverse linked list
    for (current = *head_ref; current !=NULL; current=(current->next))
    {
        closest=current;
        closest_prev=prev;

        //Find the node with the closest order
        for (struct order_line *queue = (current->next); queue!=NULL; queue=(queue->next))
        {
            if (abs((queue->data->to_floor) - (s->current_floor)) < abs((closest->data->to_floor) - (s->current_floor)))
            {
                closest=queue;
                closest_prev=prev;
            }
            
        }
        //Move the node with the closest distance to the front
        if (closest!=current)
        {
            closest_prev->next=closest->next;
            closest->next = *head_ref;
            *head_ref = closest;
        }

        prev=current;
        
    }
    
}

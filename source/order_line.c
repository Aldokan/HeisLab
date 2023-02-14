#include "order_line.h"

void add_order(struct order_line *o, struct node** head_ref, struct order *new_data, struct status *s) {
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->data  = new_data;
    new_node->next = NULL;
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;

    last->next = new_node;   
};

void remove_order(struct order_line o, struct node** head_ref, int key) {
    struct node* temp = *head_ref, *prev;
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


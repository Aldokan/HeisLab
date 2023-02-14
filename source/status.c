#include "status.h"

void pretty_print(struct status* s) {
    printf("------------------------\n");
    printf("| Movement: %s \n", s->bool_movement? "true": "false");
    printf("| Current floor: %d \n", s->current_floor);
    printf("| Floor indicators: ");

    for (int i = 0; i < sizeof(s->floor_light) / sizeof(int); i++)
        printf("%d ", s->floor_light[i]);
        
    printf(" \n");

    printf("| Button lights: ");
    for (int i = 0; i < sizeof(s->button_light) / sizeof(int); i++)
        printf("%d ", s->button_light[i]);
    printf(" \n");

    printf("| Emergency stop: %s \n", s->bool_stop? "on": "off");
    printf("| Obstruction present: %s \n", s->bool_obstruction? "true": "false");
    printf("| Door status: %s \n", s->door_status==OPEN? "open": "closed");
    printf("------------------------\n");
}
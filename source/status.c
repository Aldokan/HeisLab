#include "status.h"

void pretty_print_status(struct status* s) {
    printf("------------------------\n");
    printf("| Movement: %s \n", s->bool_movement? "true": "false");
    printf("| Last direction: %s \n", s->last_direction < 0? "Down": "Up");
    printf("| Current floor: %d \n", s->current_floor);
    printf("| Floor indicators: ");

    for (int i = 0; i < sizeof(s->floor_light) / sizeof(int); i++)
        printf("%d ", s->floor_light[i]);
        
    printf(" \n");

    for (int i = 0; i < sizeof(s->button_status) / sizeof(s->button_status[0]); i++) {
        printf("| ");
        printf("%s", i == BUTTON_CAB? "Inside Buttons:    ": i == BUTTON_HALL_UP? "Upwards Buttons:   ": "Downwards Buttons: ");
        for (int j = 0; j < sizeof(s->button_status[0]) / sizeof(s->button_status[0][0]); j++) {
            printf("%d ", s->button_status[i][j]);
        }
        printf("\n");
    }
        

    printf("| Emergency stop: %s \n", s->bool_stop? "on": "off");
    printf("| Obstruction present: %s \n", s->bool_obstruction? "true": "false");
    printf("| Door status: %s \n", s->door_status==OPEN? "open": "closed");
    printf("| Is idle: %s \n", s->bool_start? "yes": "no");
    printf("------------------------\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "control.h"

#define true 1
#define false 0

void start(struct status *s) {
    while (elevio_floorSensor() == -1) {
        elevio_motorDirection(DIRN_DOWN);
        s->bool_movement = true;
    }
    elevio_motorDirection(DIRN_STOP);
    s->bool_movement = false;
    s->current_floor = elevio_floorSensor();

    for (int i = 0; i < sizeof(s->floor_light) / sizeof(int); i++)
        s->floor_light[i] = 0;
    s->floor_light[s->current_floor] = 1;

    for (int i = 0; i < sizeof(s->button_light) / sizeof(int); i++)
        s->button_light[i] = 0;

    s->bool_stop = false;
    s->bool_obstruction = false;
    s->door_status = CLOSED;
    s->bool_start = 0; 
}

int main(){
    elevio_init();
    struct status* s;
    struct order_line* queue = (struct order_line*) malloc(sizeof(struct order_line));
    queue->next = NULL;
    struct order* ord = (struct order*) malloc(sizeof(int) + sizeof(enum source));
    queue->data = ord;
    s->bool_start = 1;
    start(s);
    pretty_print(s);
    while (s->bool_start != 1) {
        // Loop through all buttons and add order based on where it's from.
        for (int floor_nr = 0; floor_nr < N_FLOORS; floor_nr++) {
            for (enum source btn_type = BUTTON_HALL_UP; btn_type <= BUTTON_CAB; btn_type++) {
                if (elevio_callButton(floor_nr, btn_type) == 1) {
                    ord->to_floor = floor_nr;
                    ord->src = (btn_type == BUTTON_CAB? inside_elevator: outside_elevator);
                    add_order(&queue, ord, s);
                }
            }
        }
    }
    free(ord);
    free(queue);
    

    // printf("=== Example Program ===\n");
    // printf("Press the stop button on the elevator panel to exit\n");

    //elevio_motorDirection(DIRN_UP);

    // while(1){
    //     int floor = elevio_floorSensor();
    //     printf("floor: %d \n",floor);

    //     if(floor == 0){
    //         elevio_motorDirection(DIRN_UP);
    //     }

    //     if(floor == N_FLOORS-1){
    //         elevio_motorDirection(DIRN_DOWN);
    //     }


    //     for(int f = 0; f < N_FLOORS; f++){
    //         for(int b = 0; b < N_BUTTONS; b++){
    //             int btnPressed = elevio_callButton(f, b);
    //             elevio_buttonLamp(f, b, btnPressed);
    //         }
    //     }

    //     if(elevio_obstruction()){
    //         elevio_stopLamp(1);
    //     } else {
    //         elevio_stopLamp(0);
    //     }
        
    //     if(elevio_stopButton()){
    //         elevio_motorDirection(DIRN_STOP);
    //         break;
    //     }
        
    //     nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    // }

    return 0;
}

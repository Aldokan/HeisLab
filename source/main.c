#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "control.h"

#define true 1
#define false 0

void start(struct status *s) {
    // O1 and O3 - get to defined state and do not consider unrealistic conditions.
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
    struct status* s = (struct status*) malloc(sizeof(struct status));
    struct order_line* queue = (struct order_line*) malloc(sizeof(struct order_line));
    queue->next = NULL;
    struct order* ord = (struct order*) malloc(sizeof(int) + sizeof(enum source));
    queue->data = ord;
    s->bool_start = 1;
    start(s);
    pretty_print(s);
    while (s->bool_start != 1) { // O2 - main loop won't run until elevator has come to defined state.
        printf("%d\n", elevio_stopButton());
        // L3 and L4 - floor lights
        if (s->current_floor != -1) {
            elevio_floorIndicator(s->current_floor);
        }

        // Loop through all buttons and add order based on which of them are pressed.
        for (int floor_nr = 0; floor_nr < N_FLOORS; floor_nr++) {
            for (enum source btn_type = BUTTON_HALL_UP; btn_type <= BUTTON_CAB; btn_type++) {
                if (elevio_callButton(floor_nr, btn_type)) {
                    ord->to_floor = floor_nr;
                    ord->src = (btn_type == BUTTON_CAB? inside_elevator: outside_elevator);
                    add_order(&queue, ord, s);

                    s->button_light[floor_nr] = 1;
                }
            }
        }

        if (elevio_stopButton()) {
            elevio_motorDirection(DIRN_STOP); // S4 - elevator will stop immediately upon pressing the stop button
            elevio_stopLamp(1); // L6.1 - the stop button light will light up immediately upon pressing the stop button
            clear_line(&queue); // S5 - all non-processed orders will be removed upon pressing the stop button

            if (elevio_floorSensor() != -1) {
                elevio_doorOpenLamp(1); // D3.1 - the door will open if the elevator is not between floors when stop button is pressed
            }
            s->bool_stop = true;

            while(elevio_stopButton()) { } // S6 - all attempts to order (or do anything else) are ignored while the stop button is held
            elevio_stopLamp(0); // L6.2 - the stop button light will turn off immediately upon releasing the stop button
            nanosleep(&(struct timespec){0, 3*1000*1000*1000}, NULL);
            s->bool_stop = false;
            elevio_doorOpenLamp(0); // D3.2 - the door will close three seconds after the release of the stop button
        }

    }
    free(ord);
    free(s);
    clear_line(&queue);
    return 0;
}

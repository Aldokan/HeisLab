#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
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

    for (int i = 0; i < sizeof(s->button_status) / sizeof(s->button_status[0]); i++)
        for (int j = 0; j < sizeof(s->button_status[0]) / sizeof(int); j++)
            s->button_status[i][j] = 0;

    s->bool_stop = false;
    s->bool_obstruction = false;
    s->bool_start = 0; 
    elevio_doorOpenLamp(0); 
    s->door_status = CLOSED;
}

void take_order(struct order_line** queue, struct status* s, struct order* ord) {
    for (int floor_nr = 0; floor_nr < N_FLOORS; floor_nr++) {
            for (enum source btn_type = BUTTON_HALL_UP; btn_type <= BUTTON_CAB; btn_type++) {
                if (elevio_callButton(floor_nr, btn_type) && s->button_status[btn_type][floor_nr] == 0) {
                    ord->to_floor = floor_nr;
                    ord->src = (btn_type == BUTTON_CAB? inside_elevator: outside_elevator);
                    if (ord->src == outside_elevator) {
                        ord->dir = (btn_type == BUTTON_HALL_UP? DIRN_UP: DIRN_DOWN);
                    } else {
                        int diff = floor_nr - s->current_floor;
                        if (diff == 0 && elevio_floorSensor() == -1) {
                            ord->dir = -(s->last_direction);
                        } else {
                            ord->dir = (diff < 0)? DIRN_DOWN: DIRN_UP;
                        }
                        
                    }
                    add_order(queue, ord, s);
                    s->button_status[btn_type][floor_nr] = 1;
                }
            }
        }
}

int main(){
    elevio_init();
    struct status* s = (struct status*) malloc(sizeof(struct status));
    struct order_line* queue = (struct order_line*) malloc(sizeof(struct order_line));
    queue->next = NULL;
    struct order* ord = (struct order*) malloc(sizeof(struct order));
    struct control* c = (struct control*) malloc(sizeof(struct control));
    c->order_line = &queue;
    c->arrival_timer = 0; c->obstruction_timer = 0; c->stop_timer = 0;
    queue->data = NULL;
    c->status = s;
    c->status->bool_start = true;
    start(c->status);
    while (c->status->bool_start != 1) { // O2 - main loop won't run until elevator has come to defined state.
        system("clear");
        pretty_print_status(c->status);
        pretty_print_timers(c);

        if (queue->data != NULL) {
            pretty_print_line(&queue);
        } else {
            printf("No orders!\n");
        }

        // L1 and L2 - button lights
        button_lights(c);

        // L3, L4 and L5 - floor lights
        if (c->status->current_floor != -1) {
            elevio_floorIndicator(c->status->current_floor);
        }

        // Loop through all buttons and add order based on which of them are pressed.
        take_order(&queue, c->status, ord);

        if (queue->data != NULL) { 
            sort_line(&queue, c->status);
            if (queue->data->to_floor > c->status->current_floor && c->status->door_status == CLOSED) {
                move(c,DIRN_UP);
                c->status->last_direction = DIRN_UP;
            }
            if (queue->data->to_floor < c->status->current_floor && c->status->door_status == CLOSED) {
                move(c,DIRN_DOWN);
                c->status->last_direction = DIRN_DOWN;
            }
            if (c->status->current_floor == queue->data->to_floor) {
                // R3 - no further calibration is needed after elevator has come to a defined state  
                if (elevio_floorSensor() != -1) {
                    stop_in_floor(c);
                } else if (c->status->dir_before_stop < 0) {
                    move(c, DIRN_UP);
                } else {
                    move(c, DIRN_DOWN);
                }
                
            }
        }

        if (elevio_stopButton()) {
            c->arrival_timer = 0;
            c->stop_timer = 0;
            emergency_stop(&queue, c);
        }

        if (c->status->door_status == OPEN && elevio_obstruction()) {
            c->status->bool_obstruction = true;
            c->obstruction_timer = 0;
        } else if (!elevio_obstruction() && c->status->bool_obstruction) {
            if (c->obstruction_timer == 0) {
                start_timer(c, t_obstruction);
            }
        }

        if (timer_is_done(c, t_obstruction)) {
            c->status->bool_obstruction = false;
            c->obstruction_timer = 0;
            elevio_doorOpenLamp(0);
            c->status->door_status = CLOSED;
        }

        if (timer_is_done(c, t_stop) && !c->status->bool_obstruction) {
            c->stop_timer = 0;
            c->status->bool_stop = false;
            elevio_doorOpenLamp(0); // D3.2 - the door will close three seconds after the release of the stop button
            c->status->door_status = CLOSED;
        }

    }
    free(ord);
    free(s);
    free(c);
    clear_line(&queue);
    return 0;
}

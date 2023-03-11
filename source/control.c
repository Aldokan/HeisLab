#include "control.h"
#include <time.h>

#define true 1
#define false 0

void move(struct control* c, MotorDirection d){
    elevio_motorDirection(d);
    c->status->bool_movement = 1;
    if (elevio_floorSensor() != -1) {
        c->status->current_floor = elevio_floorSensor();
        elevio_floorIndicator(c->status->current_floor);
    }
}

void stop_in_floor(struct control* c){
    elevio_motorDirection(DIRN_STOP);
    if (c->arrival_timer == 0) {
        start_timer(c, t_arrival);
    }
    c->status->bool_movement = 0;
    c->status->current_floor = elevio_floorSensor();
    elevio_doorOpenLamp(1);
    c->status->door_status = OPEN;

    if (timer_is_done(c, t_arrival)) {
        // D1 - door closes three seconds after opening in floor
        c->arrival_timer = 0;
        if (!c->status->bool_obstruction) {
            // D4 - door won't close as long as obstruction is present
            elevio_doorOpenLamp(0);
            c->status->door_status = CLOSED;
        }
        // H3 - all orders in current floor are removed.
        remove_order(c->order_line, c->status->current_floor);
        for (int i = 0; i < sizeof(c->status->button_status) / sizeof(c->status->button_status[0]); i++) {
            c->status->button_status[i][c->status->current_floor] = 0;
        }
    }
}

void button_lights(struct control* c) {
    for (int i = 0; i < sizeof(c->status->button_status) / sizeof(c->status->button_status[0]); i++) {
        for (int j = 0; j < sizeof(c->status->button_status[0]) / sizeof(c->status->button_status[0][0]); j++) {
            elevio_buttonLamp(j, i, c->status->button_status[i][j]);
        }
    }
}

void emergency_stop(struct order_line** queue, struct control* c) {
    elevio_motorDirection(DIRN_STOP); // S4 - elevator will stop immediately upon pressing the stop button
    c->status->dir_before_stop = c->status->last_direction;
    c->status->bool_movement = false;
    elevio_stopLamp(1); // L6.1 - the stop button light will light up immediately upon pressing the stop button
    clear_line(queue); // S5 - all non-processed orders will be removed upon pressing the stop button
    for (int i = 0; i < sizeof(c->status->button_status) / sizeof(c->status->button_status[0]); i++) {
        for (int j = 0; j < sizeof(c->status->button_status[0]) / sizeof(c->status->button_status[0][0]); j++) {
            c->status->button_status[i][j] = 0;
        }
    }
    button_lights(c);

    if (elevio_floorSensor() != -1) {
        elevio_doorOpenLamp(1); // D3.1 - the door will open if the elevator is not between floors when stop button is pressed
        c->status->door_status = OPEN;
    }
    c->status->bool_stop = true;

    while(elevio_stopButton()) {} // S6 - all attempts to order (or do anything else) are ignored while the stop button is held
    elevio_stopLamp(0); // L6.2 - the stop button light will turn off immediately upon releasing the stop button
    if (c->stop_timer == 0) {
        start_timer(c, t_stop);
    }
}

void start_timer(struct control* c, enum type timer_type) {
    switch (timer_type)
    {
    case t_arrival:
        c->arrival_timer = time(NULL);
        break;
    
    case t_stop:
        c->stop_timer = time(NULL);
        break;

    case t_obstruction:
        c->obstruction_timer = time(NULL);
        break;
    } 
}

int timer_is_done(struct control* c, enum type timer_type) {
    time_t timer;
    switch (timer_type) {
        case t_arrival: {
            timer = c->arrival_timer;
            break;
        }
            
        case t_stop: {
            timer = c->stop_timer;
            break;
        }

        case t_obstruction: {
            timer = c->obstruction_timer;
            break;
        }
    }

    return (timer != 0) && (time(NULL) - timer >= 3);
}

void pretty_print_timers(struct control* c) {
    printf("------------------------------------------------\n");
    printf("Arrival timer: %lds\n", c->arrival_timer == 0? 0: time(NULL) - c->arrival_timer);
    printf("Stop timer: %lds\n", c->stop_timer == 0? 0: time(NULL) - c->stop_timer);
    printf("Obstruction timer: %lds\n", c->obstruction_timer == 0? 0: time(NULL) - c->obstruction_timer);
    printf("------------------------------------------------\n");
}
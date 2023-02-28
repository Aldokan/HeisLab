#include "control.h"
#include <time.h>

void move(struct control* c, MotorDirection d){
    elevio_motorDirection(d);
    c->status->bool_movement=1;
    if (elevio_floorSensor()!=-1) {
        c->status->current_floor=elevio_floorSensor();
        elevio_floorIndicator(elevio_floorSensor());
    }
}

void stop(struct control* c){
    elevio_motorDirection(DIRN_STOP);
    c->status->bool_movement = 0;
    c->status->current_floor = elevio_floorSensor();
    elevio_doorOpenLamp(1);
    c->status->door_status = OPEN;
    nanosleep(&(struct timespec){0, (long)3*1000*1000*1000}, NULL);
    elevio_doorOpenLamp(0);
    c->status->door_status = CLOSED;
    remove_order(c->order_line, c->status->current_floor);
    for (int i = 0; i < sizeof(c->status->button_status) / sizeof(c->status->button_status[0]); i++) 
        c->status->button_status[i][c->status->current_floor] = 0;
}
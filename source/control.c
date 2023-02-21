#include "control.h"

void move(struct control c, MotorDirection d){
    elevio_motorDirection(d);
    c.status->bool_movement=1;
    if (elevio_floorSensor()!=-1) {
        c.status->current_floor=elevio_floorSensor();
        elevio_floorIndicator(elevio_floorSensor());
    }
}

void stop(struct control c){
    elevio_motorDirection(DIRN_STOP);
    c.status->bool_movement=0;
    c.status->current_floor=elevio_floorSensor();
}
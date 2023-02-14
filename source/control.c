#include "control.h"
#include "elevio.h"

void move(struct control c){
    
};

void stop(struct control c){
    if (c.status->bool_stop==1)
    {
       elevio_motorDirection(DIRN_STOP); 
    };
};

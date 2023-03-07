#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "driver/elevio.h"

struct status
{
    int bool_movement;
    int current_floor;
    int floor_light[4];
    int button_light[4];
    int button_status[3][4];
    int bool_stop;
    int bool_obstruction;
    int bool_start;
    enum door{OPEN, CLOSED} door_status;
};

void pretty_print_status(struct status* status);



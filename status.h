#include <stdio.h>
#include <stdlib.h>

struct status
{
    int bool_movement;
    int current_floor;
    int floor_light;
    int button_light[4];
    int bool_stop;
    int bool_obstruction;
    enum door{open, closed};
};



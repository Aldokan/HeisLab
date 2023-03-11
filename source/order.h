#include "driver/elevio.h"
struct order{
    int to_floor;
    enum source{outside_elevator, inside_elevator} src;
    MotorDirection dir;
};

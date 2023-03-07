#include "order_line.h"

enum type{t_arrival, t_stop, t_obstruction};

struct control {
    struct order_line** order_line;
    struct status* status;
    time_t arrival_timer;
    time_t stop_timer;
    time_t obstruction_timer;
};

void move(struct control* c, MotorDirection d);
void stop_in_floor(struct control* c);
void emergency_stop(struct order_line* queue, struct status* s, struct control* c);
void start_timer(struct control* c, enum type timer_type);
int timer_is_done(struct control* c, enum type timer_type);


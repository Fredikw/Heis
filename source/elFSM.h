#include "elUtils.h"

void FSM_stop(struct Elevator *e);

void FSM_new_order(struct Elevator *e);

void FSM_time_out(struct Elevator *e);

void FSM_arrived_new_floor(struct Elevator *e);

void FSM_run();
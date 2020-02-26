#include "hardware.h"

typedef struct Elevator f;

typedef enum {
    IDEL,
    MOVING,
    DOOR_OPEN
} ElevatorState;

typedef enum {
    FLOOR1,
    FLOOR2,
    FLOOR3,
    FLOOR4
} FloreType;

struct Elevator 
{
    FloreType floor;
    HardwareMovement direction;
    int elevator_queue[4][3];
    ElevatorState state;
};

void elFSM_init_elevator(struct Elevator *e);
void elFSM_clear_order_queue(struct Elevator *e);

HardwareMovement elFSM_set_direction_for_idel(struct Elevator *e);
void elFSM_add_new_order(struct Elevator *e);
void clear_order(struct Elevator *e);
int elFSM_check_if_arrived_new_floor(struct Elevator *e);
int should_i_stop(struct Elevator *e);
int should_i_continue(struct Elevator *e);

void elFSM_stop(struct Elevator *e);
void elFSM_new_order(struct Elevator *e);
void elFSM_time_out(struct Elevator *e);
void elFSM_arrived_new_floor(struct Elevator *e);


void elFSM_run();
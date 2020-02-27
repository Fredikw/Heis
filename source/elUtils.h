#include "hardware.h"

typedef enum {
    IDEL,
    MOVING,
    DOOR_OPEN
} ElevatorState;

typedef enum {
    FLOOR1,
    FLOOR2,
    FLOOR3,
    FLOOR4,
    UNDEFINED
} FloreType;

struct Elevator 
{
    FloreType floor;
    HardwareMovement direction;
    int elevator_queue[4][3];
    ElevatorState state;
};

void elUtils_init_elevator(struct Elevator *e);

void elUtils_clear_order_queue(struct Elevator *e);

HardwareMovement elUtils_set_direction_for_idel(struct Elevator *e);

void elUtils_add_new_order(struct Elevator *e);

void elUtils_clear_order(struct Elevator *e);

int elFSM_check_if_arrived_new_floor(struct Elevator *e);

int should_i_stop(struct Elevator *e);

int should_i_continue(struct Elevator *e);

int should_i_turn(struct Elevator *e);
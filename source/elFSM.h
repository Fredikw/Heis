#pragma once
#include "hardware.h"

typedef enum {
    IDEL,
    MOVING,
    DOOR_OPEN,
} ElevatorState;

struct Elevator 
{
    int floor;
    HardwareMovement movement;
    int elevator_queue[4][3];
    ElevatorState state;

}; 

void elFSM_stop(struct Elevator *e);
void elFSM_add_new_order(struct Elevator *e);
void elFSM_arrived_floor(struct Elevator *e);

void elFSM_run();
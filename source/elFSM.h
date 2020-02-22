#pragma once
#include "hardware.h"

struct State 
{
    int floor;
    HardwareMovement movement;
    int elevator_queue[4][3];
}; 



void elFSM_stop(struct State *e);
void elFSM_new_order();
void elFSM_arrived_floor();
void elFSM_stop();

void elFSM_run();

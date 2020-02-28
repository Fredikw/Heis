/**
 * @file
 * @brief Final state machine for elevator.
 */
#ifndef FSM_H
#define FSM_H

#include "elUtils.h"

/**
 * @brief Movement type used in @c hardware_command_movement.
 */
void FSM_stop(struct Elevator *e);

void FSM_new_order(struct Elevator *e);

void FSM_time_out(struct Elevator *e);

void FSM_arrived_new_floor(struct Elevator *e);

void FSM_run();

#endif
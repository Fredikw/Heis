/**
 * @file
 * @brief Final state machine for elevator.
 */
#ifndef FSM_H
#define FSM_H

#include "elUtils.h"

/**
 * @brief Executes if @c hardware_read_stop_signal, is set high.
 * Clears order queue and all oder lights, and sets elevator state to idel.
 */
void FSM_stop(struct Elevator *e);

/**
 * @brief Adds new order to elevator order matrix.
 * If the state of the elevator is idel. The elevator determines the proper direction
 * based on whether the elevator is at a floor or between two floors.
 */
void FSM_new_order(struct Elevator *e);

/**
 * @brief Executes at experation of timer.
 * Determines if the cabine should continue or change direction by @c should_i_continue, and @c should_i_turn 
 * respectively in that order. 
 * If not, the elevator goes to idel. For every case the cabin state parameters are set accordingly.
 */
void FSM_time_out(struct Elevator *e);

/**
 * @brief Executes at arrival of new floor. Determines if the cabine should stop by @c should_i_stop
 * and sets the cabin state parameters accordingly. Either the cabine stops and dispatches an order,
 * or it continues in the same direction.
 */
void FSM_arrived_new_floor(struct Elevator *e);

/**
 * @brief Final state machine of the system. Checks for the triggers and executes the above functions accordingly.
 */
void FSM_run();

#endif
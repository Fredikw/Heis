/**
 * @file
 * @brief Utilities for the elevator system.
 */
#ifndef ELUTILS_H
#define ELUTILS_H

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
} FloreType;

/**
 * @brief Struct representing the elevator cabine.
 */
struct Elevator 
{
    FloreType floor;
    HardwareMovement direction;
    int elevator_queue[4][3];
    ElevatorState state;
};

/**
 * @brief An extension of @c hardware_init
 * Commands the elevator to move to first floor and go to idel.
 */
void elUtils_init_elevator(struct Elevator *e);

/**
 * @brief Sets all elements of elevator queue matrix to null.
 */
void elUtils_clear_order_queue(struct Elevator *e);

/**
 * @brief Polls the elevator queue matrix for orders.
 * @return Direction for movement according to first order.
 */
HardwareMovement elUtils_set_direction_for_idel(struct Elevator *e);

/**
 * @brief Adds new order to elevator queue matrix.
 */
void elUtils_add_new_order(struct Elevator *e);

/**
 * @brief Clears all orders at the current floor from elevator queue matrix.
 */
void elUtils_clear_order(struct Elevator *e);

/**
 * @brief An extension of @c hardware_read_floor_sensor
 * @return 1 on arrival at new floor, 0 otherwise.
 */
int elUtils_read_new_floor(struct Elevator *e);

/**
 * @brief Polls the elevator queue matrix for orders at current floor.
 * @return 1 if there is an order from inside the cabin for current floor,
 * or outside in the same direction as the movement of the cabine.
 */
int should_i_stop(struct Elevator *e);

/**
 * @brief Polls the elevator queue matrix for orders in the same 
 * direction as the movement of the cabine.
 * @return 1 on success, 0 otherwise.
 */
int should_i_continue(struct Elevator *e);

/**
 * @brief Polls the elevator queue matrix for orders in the opposite 
 * direction of the movement of the cabine.
 * @return 1 on success, 0 otherwise.
 */
int should_i_turn(struct Elevator *e);

#endif
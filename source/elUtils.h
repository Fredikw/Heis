/**
 * @file
 * @brief Driver for the elevator hardware.
 *
 * Neatly wraps up Martin Korsgaard's spaghetti
 * from 2006 ;)
 *
 * Kolbjørn Austreng
 */
#pragma once
#include "hardware.h"
#include "elFSM.h"

/**
 * @brief Initializes the elevator control hardware.
 * Commands the elevator to move to floor 1.
 * Commands the hardware to turn on the floor indicator for floor 1.
 */
void elUtils_init_elevator();

/**
 * @brief clears all order lights
 */
void elUtils_clear_all_order_lights();

/**
 * @brief clears order queue
 */
void elUtils_clear_order_queue(struct Elevator *e);

/**
 * @brief adds new order to elevator_queue.
 */
void elUtils_add_new_order(struct Elevator *e);

/**
 * @brief Polls the hardware for orders
 * @return 1 on oder.
 */
int elUtils_check_for_orders();

/**
 * @brief Checks if elevator reached a floor.
 * @return 1 if true.
 */
int elUtils_check_if_arrived_floor();
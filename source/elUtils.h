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

/**
 * @brief Initializes the elevator control hardware.
 * Commands the elevator to move to floor 1.
 * Commands the hardware to turn on the floor indicator for floor 1.
 */
void elUtils_init_elevator();


/**
 * @brief Pools queue for orders
 * @return 1 for order. Zero for none.
 */
int elUtils_check_for_orders();

/**
 * @brief clear all order lights
 */
void elUtils_clear_all_order_lights();


/**
 * @brief Checks if elevator reached a floor.
 * @return 1 if true.
 */
int elUtils_check_if_arrived_floor();
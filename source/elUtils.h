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
void elUtils_init_hardware();

/**
 * @brief clears all order lights
 */
void elUtils_clear_all_order_lights();

/**
 * @brief Polls the hardware for orders
 * @return 1 on oder.
 */
int elUtils_read_order_button();

/**
 * @brief Checks if elevator reached a floor.
 * @return 1 if true.
 */
int elUtils_check_if_arrived_floor();

/**
 * @return an integer corresponding to current floor.
 */
int elUtils_check_current_floor();
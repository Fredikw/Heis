/**
 * @file
 * @brief Utilities for time management.
 */
#ifndef TIMER_H
#define TIMER_H

extern int timer_enable;

/**
 * @brief Sets global variablel end_time in the source file to
 * current time pluss 3 seconds.
 */
void timer_start();

/**
 * @brief Check timer
 * @return 1 if the timer has expired, 0 otherwise.
 */
int timer_out();

#endif
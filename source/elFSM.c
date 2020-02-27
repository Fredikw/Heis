#include "hardware.h"
#include "elFSM.h"
#include "timer.h"

void FSM_stop(struct Elevator *e){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);
    hardware_command_clear_all_order_lights();
    elUtils_clear_order_queue(e);

    switch (hardware_read_all_floor_sensors())
    {
    case 0:

        while(hardware_read_stop_signal());
        hardware_command_stop_light(0);

        e->state = IDEL;
        break;
    
    default:
        hardware_command_door_open(1);
        while(hardware_read_stop_signal());
        hardware_command_stop_light(0);
        e->state = DOOR_OPEN;
        timer_start();
        break;
    }
}

void FSM_new_order(struct Elevator *e){
    elUtils_add_new_order(e);

    switch (e->state)
    {
    case IDEL:

        if(elUtils_set_direction_for_idel(e) == HARDWARE_MOVEMENT_STOP && hardware_read_all_floor_sensors()){
            e->direction = elUtils_set_direction_for_idel(e);
            e->state = DOOR_OPEN;
            elUtils_clear_order(e);
            hardware_command_door_open(1);
            timer_start();
            break;
        }
        else if(elUtils_set_direction_for_idel(e) == HARDWARE_MOVEMENT_STOP && !hardware_read_all_floor_sensors())
        {
            e->floor = UNDEFINED;
            if(e->direction == HARDWARE_MOVEMENT_UP){
                e->direction = HARDWARE_MOVEMENT_DOWN;
                e->state = MOVING;
                hardware_command_movement(e->direction);
                break;
            }
            else
            {
                e->direction = HARDWARE_MOVEMENT_UP;
                e->state = MOVING;
                hardware_command_movement(e->direction);
                break;
            }
            
        }
        else
        {
            e->direction = elUtils_set_direction_for_idel(e);
            e->state = MOVING;
            hardware_command_movement(e->direction);
            break;
        }
    default:
        break;
    }
}

void FSM_arrived_new_floor(struct Elevator *e){
    e->floor = hardware_read_current_floor();
    hardware_command_floor_indicator_on(e->floor);

    switch (should_i_stop(e))
    {
    case 1:
        elUtils_clear_order(e);
        e->state = DOOR_OPEN;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        hardware_command_door_open(1);
        timer_start();
        break;
    
    default:
        break;
    }
}

void FSM_time_out(struct Elevator *e){
    timer_enable = 0;
    hardware_command_door_open(0);

    if (should_i_continue(e)){
        e->state = MOVING;
        hardware_command_movement(e->direction);
    }

    else if(should_i_turn(e)){
        if(e->direction == HARDWARE_MOVEMENT_UP){
            e->direction = HARDWARE_ORDER_DOWN;
        }
        else
        {
            e->direction = HARDWARE_MOVEMENT_UP;
        }
        hardware_command_movement(e->direction);
    }
    else
    {
        e->direction = HARDWARE_MOVEMENT_STOP;
        e->state = IDEL;
    }
}

void FSM_run(){
    struct Elevator el;
    elUtils_init_elevator(&el);
    do
    {
        if (hardware_read_stop_signal())
        {
            FSM_stop(&el);
        }
        if (hardware_read_all_orders())
        {
            FSM_new_order(&el);
        }
        if (elFSM_check_if_arrived_new_floor(&el))
        {
            FSM_arrived_new_floor(&el);
        }
        if (timer_out() && timer_enable)
        {
            FSM_time_out(&el);
        }
        if (hardware_read_obstruction_signal() && el.state == DOOR_OPEN)
        {
            timer_start();
        }
    } while (1);
}
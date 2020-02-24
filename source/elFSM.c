#include "hardware.h"
#include "elFSM.h"
#include "elUtils.h"
#include "timer.h"

void elFSM_init_elevator(struct Elevator *e){
    elUtils_init_hardware();
    elFSM_clear_order_queue(e);
    hardware_command_floor_indicator_on(0);
    
    while(!hardware_read_floor_sensor(0)){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    e->state = IDEL;
    e->floor = FLOOR1;
    e->movement = HARDWARE_MOVEMENT_STOP;
}

void elFSM_clear_order_queue(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
            e->elevator_queue[i][j] = 0;
        }
    }
}

HardwareMovement elFSM_set_direction_for_idel(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
            if(e->elevator_queue[i][j]){
                if((e->floor-1) < i){
                    return HARDWARE_MOVEMENT_UP;
                }
                return HARDWARE_MOVEMENT_DOWN;
            }
        }
    }
    return HARDWARE_MOVEMENT_STOP;
}

void elFSM_add_new_order(struct Elevator *e){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
            if(hardware_read_order(f, i)){
                e->elevator_queue[f][i] = 1;
                hardware_command_order_light(f, order_types[i], 1);
            }
        }
    }
}

int elFSM_check_if_arrived_new_floor(struct Elevator *e){
    if(e->floor-1 != elUtils_check_if_arrived_floor()){
        return 1;
    }
    return 0;
}

int should_i_stop(struct Elevator *e){
    if(e->movement == HARDWARE_MOVEMENT_UP){
        if(e->elevator_queue[(e->floor)-1][0] || e->elevator_queue[(e->floor)-1][1]){
            return 1;
        }
    }
    if(e->movement == HARDWARE_MOVEMENT_DOWN){
        if(e->elevator_queue[(e->floor)-1][0] || e->elevator_queue[(e->floor)-1][2]){
            return 1;
        }
    }
    if((e->floor==FLOOR1) || (e->floor==FLOOR1)){
        return 1;
    }
    return 0;
}

int should_i_continue(struct Elevator *e){
    if(e->movement == HARDWARE_MOVEMENT_UP){
        for(int i = e->floor; i < FLOOR4; i++){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return 1;
                }
            }
        }
    }
    if(e->movement == HARDWARE_MOVEMENT_DOWN){
        for(int i = (e->floor)-2; i >= NONE; i--){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return 1;
                }
            }
        }
    }
    return 0;
}


//--------------------------------------------------------------------------------//



void elFSM_stop(struct Elevator *e){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);
    elUtils_clear_all_order_lights();
    elFSM_clear_order_queue(e);

    switch (e->floor)
    {
    case NONE:

        while(hardware_read_stop_signal());
        hardware_command_stop_light(0);

        e->state = IDEL;
        break;
    
    default:
        hardware_command_door_open(1);
        while(hardware_read_stop_signal());
        hardware_command_stop_light(0);
        e->state = IDEL;
        e->state = DOOR_OPEN;
        timer_start();
        break;
    }
}

void elFSM_new_order(struct Elevator *e){
    elFSM_add_new_order(e);
    
    switch (e->state)
    {
    case IDEL:
        e->movement = elFSM_set_direction_for_idel(e);
        e->state = MOVING;
        hardware_command_movement(e->movement);
        break;
    default:
        break;
    }
}

void elFSM_arrived_new_floor(struct Elevator *e){
    e->floor = elUtils_check_current_floor();
    hardware_command_floor_indicator_on(e->floor);

    switch (should_i_stop(e))
    {
    case 1:
        e->state = DOOR_OPEN;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        hardware_command_door_open(1);
        timer_start();                                          //Blir denne satt riktig
        break;
    
    default:
        break;
    }
}


void elFSM_time_out(struct Elevator *e){
    hardware_command_door_open(0);

}

void elFSM_run(){
    struct Elevator el;
    elFSM_init_elevator(&el);

    do
    {
        if (hardware_read_stop_signal())
        {
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        if (elUtils_read_order_button())
        {
            elFSM_new_order(&el);
        }
        if (elFSM_check_if_arrived_new_floor(&el))
        {
            elFSM_arrived_new_floor(&el);
        }
        if (timer_out())
        {
            elFSM_run();
        }
        if (hardware_read_obstruction_signal())                             //Kan jeg løse dette på en simplere måte
        {
            timer_start();
        }
    } while (1);
}
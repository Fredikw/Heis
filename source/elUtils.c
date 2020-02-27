#include "hardware.h"
#include "elUtils.h"
#include <stdio.h>
#include <stdlib.h>

void elUtils_init_elevator(struct Elevator *e){

    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }


    elUtils_clear_order_queue(e);
    hardware_command_floor_indicator_on(0);
    
    while(!hardware_read_floor_sensor(0)){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    e->state = IDEL;
    e->floor = FLOOR1;
    e->direction = HARDWARE_MOVEMENT_STOP;
}

void elUtils_clear_order_queue(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
            e->elevator_queue[i][j] = 0;
        }
    }
}

HardwareMovement elUtils_set_direction_for_idel(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
            if(e->elevator_queue[i][j]){
                if((e->floor) < i){
                    return HARDWARE_MOVEMENT_UP;
                }
                if((e->floor) > i){
                    return HARDWARE_MOVEMENT_DOWN;
                }
            }
        }
    }
    return HARDWARE_MOVEMENT_STOP;
}

void elUtils_add_new_order(struct Elevator *e){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
            if(hardware_read_order(f, i)){
                if(!(hardware_read_all_floor_sensors() && f == e->floor)){
                    e->elevator_queue[f][i] = 1;
                    hardware_command_order_light(f, order_types[i], 1);
                }
            }
        }
    }
}

void elUtils_clear_order(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
        e->elevator_queue[e->floor][i] = 0;
        hardware_command_order_light(e->floor, i, 0);
    }
}

int elFSM_check_if_arrived_new_floor(struct Elevator *e){
    if((hardware_read_all_floor_sensors() != 0) && e->floor != hardware_read_current_floor()){
        return 1;
    }
    return 0;
}

int should_i_stop(struct Elevator *e){
    if(e->direction == HARDWARE_MOVEMENT_UP){
        if(e->elevator_queue[e->floor][0] || e->elevator_queue[e->floor][1] || !should_i_continue(e)){
            return 1;
        }
    }
    if(e->direction == HARDWARE_MOVEMENT_DOWN){
        if(e->elevator_queue[e->floor][1] || e->elevator_queue[e->floor][2] || !should_i_continue(e)){
            return 1;
        }
    }
    if((e->floor==FLOOR1) || (e->floor==FLOOR4)){
        e->state = DOOR_OPEN;
        return 1;
    }
    return 0;
}

int should_i_continue(struct Elevator *e){
    if(e->direction == HARDWARE_MOVEMENT_UP){
        for(int i = (e->floor)+1; i < HARDWARE_NUMBER_OF_FLOORS; i++){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return 1;
                }
            }
        }
    }
    if(e->direction == HARDWARE_MOVEMENT_DOWN){
        for(int i = (e->floor)-1; i >= FLOOR1; i--){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return 1;
                }
            }
        }
    }
    return 0;
}

int should_i_turn(struct Elevator *e){
    if(e->direction == HARDWARE_MOVEMENT_UP){
        for(int i = (e->floor)-1; i >= FLOOR1; i--){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return 1;
                }
            }
        }
    }
    if(e->direction == HARDWARE_MOVEMENT_DOWN){
        for(int i = (e->floor)+1; i < HARDWARE_NUMBER_OF_FLOORS; i++){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return 1;
                }
            }
        }
    }
    return 0;
}
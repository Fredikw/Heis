#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "elUtils.h"

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void elUtils_init_elevator(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);
}

void elUtils_clear_all_order_lights(){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}

void elUtils_clear_order_queue(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
            e->elevator_queue[i][j] = 0;
        }
    }
}

void elUtils_add_new_order(struct Elevator *e){
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            if(hardware_read_order(f, i)){
                e->elevator_queue[f][i] = 1;
            }
        }
    }
}

int elUtils_check_for_orders(){
    HardwareOrder order_type[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };
    
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_type[i];
            if(hardware_read_order(f, i)){
                return 1;
            }
        }
    }
    return 0;
}

int elUtils_check_if_arrived_floor(){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        if(hardware_read_floor_sensor(i)){
            return 1;
        }
    }
    return 0;
}
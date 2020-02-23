#include "hardware.h"
#include "elFSM.h"
#include "elUtils.h"

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
}

void elFSM_clear_order_queue(struct Elevator *e){
    for(int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
        for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
            e->elevator_queue[i][j] = 0;
        }
    }
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
    return e->floor-1 != elUtils_check_if_arrived_floor();
}

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
        // Door open sequence
        break;
    }
}
HardwareMovement elFSM_stop_or_go(struct Elevator *e){
    if(e->movement == HARDWARE_MOVEMENT_UP){
        if(e->elevator_queue[(e->floor)-1][0] || e->elevator_queue[(e->floor)-1][1]){
            return HARDWARE_MOVEMENT_STOP;
        }

        for(int i = e->floor; i < FLOOR4; i++){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return HARDWARE_MOVEMENT_UP;
                }
            }
        }
    }

    if(e->movement == HARDWARE_MOVEMENT_DOWN){
        if(e->elevator_queue[(e->floor)-1][0] || e->elevator_queue[(e->floor)-1][2]){
            return HARDWARE_MOVEMENT_STOP;
        }

        for(int i = (e->floor)-2; i >= FLOOR1-1; i--){
            for(int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
                if(e->elevator_queue[i][j]){
                    return HARDWARE_MOVEMENT_DOWN;
                }
            }
        }
    }
    return HARDWARE_MOVEMENT_STOP;
}

void elFSM_arrived_floor(struct Elevator *e){
    e->floor = elUtils_check_current_floor();
    hardware_command_floor_indicator_on((e->floor)-1);

    switch (elFSM_stop_or_go(e))
    {
    case HARDWARE_MOVEMENT_STOP:
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        // Door open sequence
        hardware_command_movement(e->movement);
        break;
    
    default:
        break;
    }
}

HardwareMovement elFSM_set_direction(struct Elevator *e){
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

void elFSM_new_order(struct Elevator *e){
    elFSM_add_new_order(e);
    switch (e->state)
    {
    case IDEL:
        e->movement = elFSM_set_direction(e);
        hardware_command_movement(e->movement);
        e->state = MOVING;
        break;
    default:
        break;
    }
}

// void elFSM_run(){
//     struct Elevator el;
//     elFSM_init_elevator(&el);

//     do
//     {
//         if (hardware_read_stop_signal())
//         {
//             elFSM_stop(&el);
//             break;
//         }
//         if (elUtils_read_order_button())
//         {
//             break;
//         }
//         if (elFSM_check_if_arrived_new_floor(&el))
//         {
//             break;
//         }
//     } while (1);
// }
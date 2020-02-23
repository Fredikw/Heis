#include "hardware.h"
#include "elFSM.h"
#include "elUtils.h"

void elFSM_stop(struct Elevator *e){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);
    elUtils_clear_all_order_lights();
    elUtils_clear_order_queue(&e);

    switch (e->state)
    {
    case MOVING:

        while(hardware_read_stop_signal());
        hardware_command_stop_light(0);

        e->state = IDEL;
        break;
    
    default:
        
        break;
    }
}

// void elFSM_run(){
//     do
//     {
//         if (hardware_read_stop_signal())
//         {
//             /* code */
//         }
//         if (elUtils_check_for_orders())
//         {
//             /* code */
//         }
//         if (elUtils_check_if_arrived_floor())
//         {
//             /* code */
//         }
//         if (timer_timeout())
//         {
//             /* code */
//         }
        
//     } while (1);
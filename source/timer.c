#include "timer.h"
#include <time.h>
#define DUR 1

static time_t end_time;
int timer_enable;

void timer_start(){
    end_time = time(NULL) + DUR;
    timer_enable = 1;
}

int timer_out(){
    if(end_time < time(NULL)){
        return 1;
    }
    return 0;
}
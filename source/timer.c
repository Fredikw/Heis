#include "timer.h"
#include <time.h>
#define DUR 3

static time_t end_time;

void timer_start(){
    end_time = time(NULL) + DUR;
}

int timer_out(){
    if(end_time < time(NULL)){
        return 1;
    }
    return 0;
}
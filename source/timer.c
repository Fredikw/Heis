#include "timer.h"
#include <time.h>
#define DUR 3

static time_t end_time;
static int enable = 0;

void start_timer(){
    end_time = time(NULL) + DUR;
    enable = 1;
}

int check_timer(){
    if(enable || (end_time < time(NULL))){
        enable = 0;
        return 1;
    }
    return 0;
}
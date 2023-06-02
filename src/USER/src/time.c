#include "headfile.h"
#include "time.h"

void time_begin(){
    SysTick->CTLR = 0;
    SysTick->CNT = 0;
    SysTick->CTLR = 1;
}

void time_end(){
    SysTick->CTLR = 0;
}

int get_time(){
    return SysTick->CNT / (SystemCoreClock/9000);
}
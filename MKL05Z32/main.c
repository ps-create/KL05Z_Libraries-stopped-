
#include "uart.h"
#include "EvTim.h"
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  char str[50];
  SysTick_Config(SystemCoreClock);
  uart_init(115200);
  evTim_data_t timeEvent = {0};
  evTim_data_t *timeEventptr = NULL;
  while(1)
  {
    switch(EvTim_IsReady(timeEventptr))
    {
      case EVTIM_STOP:
      case EVTIM_TIMES_UP: 
      {
        EvTim_ActivateUs(&timeEvent,1000);
        sprintf(str, "%d", SysTick->VAL);
        uart_sendStr(str);
      }
      case EVTIM_ERROR:
        timeEventptr = &timeEvent;
        break;
      case EVTIM_IN_PROGRESS:
        break;
    }
  }
}
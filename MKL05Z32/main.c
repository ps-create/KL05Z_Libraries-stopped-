/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 09.06.2022r.
----------------------------------------------------------------------------*/
#include "uart.h"
#include "EvTim.h"
#include "tpm.h"
#include "i2c.h"
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  char str[50];
	tpm_init(TPM0,TPM_DIVIDE128,TPM_CLOCK_MCGFLLCLK,MAX_TIMER_TICK,TPM_TOIE_OFF,TPM_EDGE_PWM);
  uart_init(115200);
	i2c_init(Mult_2,0x19);
	tpm_timer_start(TPM0);
  evTim_data_t timeEvent = {0};
  evTim_data_t *timeEventptr = NULL;
  sprintf(str, "%d\n", TPM0->CNT);
  uart_sendStr(str);
  while(1)
  {
    switch(EvTim_IsReady(timeEventptr))
    {
      case EVTIM_STOP:
      case EVTIM_TIMES_UP: 
      {
        EvTim_ActivateUs(TPM0,&timeEvent,MAX_TIMER_TICK*2);
        sprintf(str, "%d\n ", TPM0->CNT);
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
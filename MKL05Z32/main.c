
#include "uart.h"
#include "EvTim.h"
#include "tpm.h"
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  char str[50];
	tpm_init(TPM0,tpm_divide64,0xFFFF,TPM_TOIE_OFF,TPM_EDGE_PWM);
  uart_init(115200);
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
        EvTim_ActivateUs(TPM0,&timeEvent,5000000);
        sprintf(str, "%d\n", TPM0->CNT);
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
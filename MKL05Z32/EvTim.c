/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 21.05.2022r.
----------------------------------------------------------------------------*/
#include "EvTim.h"

/*TO FIX (USE EXTERNAL CLOCK)*/
void EvTim_ActivateUs(evTim_data_t *timeEvent_p, uint32_t delayUs)
{
  timeEvent_p->timeStamp = SysTick->VAL + delayUs;
  timeEvent_p->activate = i2c_succes;
}

evTim_State_t EvTim_IsReady(evTim_data_t *timeEvent_p)
{
  evTim_State_t return_state = EVTIM_ERROR;
  if(NULL == timeEvent_p)
  {
    return_state = EVTIM_ERROR;
  }
  else if(timeEvent_p->activate == i2c_failure)
  {
    return_state = EVTIM_STOP;
  }
  else if((int32_t)SysTick->VAL - (int32_t)timeEvent_p->timeStamp < 0)
  {
    return_state = EVTIM_IN_PROGRESS;
  }
  else
  {
    timeEvent_p->activate = i2c_failure;
    return_state = EVTIM_TIMES_UP;
  }
  return return_state;
}
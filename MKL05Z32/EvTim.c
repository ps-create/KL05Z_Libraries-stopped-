/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 21.05.2022r.
----------------------------------------------------------------------------*/
#include "EvTim.h"

void EvTim_ActivateUs(TPM_Type* tpmType,evTim_data_t *timeEvent_p, uint32_t delayUs)
{
  timeEvent_p->timeStamp = tpmType->CNT + delayUs;
  timeEvent_p->activate = evTim_succes;
	timeEvent_p->tpmType = tpmType;
}

evTim_State_t EvTim_IsReady(evTim_data_t *timeEvent_p)
{
  evTim_State_t return_state = EVTIM_ERROR;
  if(NULL == timeEvent_p)
  {
    return_state = EVTIM_ERROR;
  }
  else if(timeEvent_p->activate == evTim_failure)
  {
    return_state = EVTIM_STOP;
  }
  else if( (timeEvent_p->timeStamp - timeEvent_p->tpmType->CNT) & (1 << 16))
  {
    return_state = EVTIM_IN_PROGRESS;
  }
  else
  {
    timeEvent_p->activate = evTim_failure;
    return_state = EVTIM_TIMES_UP;
  }
  return return_state;
}
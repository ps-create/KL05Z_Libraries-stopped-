/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 21.05.2022r.
----------------------------------------------------------------------------*/

#ifndef EVTIM_H
#define EVTIM_H

#include "MKL05Z4.h"

#define NULL 0

typedef enum
{
	i2c_succes = !0,
	i2c_failure = 0
} evTim_status;

typedef struct
{
  uint32_t timeStamp;
  evTim_status activate;
} evTim_data_t;

typedef enum
{
  EVTIM_STOP = 0,
  EVTIM_IN_PROGRESS,
  EVTIM_TIMES_UP,
  EVTIM_ERROR 
} evTim_State_t;

void EvTim_ActivateUs(evTim_data_t *timeEvent_p, uint32_t delayUs);

evTim_State_t EvTim_IsReady(evTim_data_t *timeEvent_p);


#endif /* EVTIM_H */
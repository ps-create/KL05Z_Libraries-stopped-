/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 09.06.2022r.
----------------------------------------------------------------------------*/
#ifndef TPM_H
#define TPM_H

#include "MKL05Z4.h"

#define MAX_TIMER_TICK		(uint16_t)(65535)
#define US_CONVERSION			(uint32_t)(1000000)
#define MCGFLLCLK   			0x01

#define TPM_PWM_H 		TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK
#define TPM_PWM_L			TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK

typedef enum
{
	tpm_divide1 = 0,
	tpm_divide2 = 1,
	tpm_divide4,
  tpm_divide8,
  tpm_divide16,
  tpm_divide32,
  tpm_divide64,
  tpm_divide128
}tpm_prescaler_t;

typedef enum
{
	tpm_pwm_channel_0 = 0,
	tpm_pwm_channel_1,
	tpm_pwm_channel_2,
	tpm_pwm_channel_3,
	tpm_pwm_channel_4,
	tpm_pwm_channel_5,
	tpm_pwm_channel_NC
}tpm_pwm_channel_t;

typedef enum
{
	tpm_ok,
	tpm_not_ok,
	tpm_error
} tpm_status_t;

typedef enum
{
	TPM_EDGE_PWM = 0,
	TPM_CENTER_PWM,
	TPM_OFF_PWM
} tpm_counting_mod_t;

typedef enum
{
	TPM_TOIE_OFF = 0,
	TPM_TOIE_ON
}tpm_overflow_interrupt_t;

tpm_status_t tpm_init(TPM_Type* tpmType, tpm_prescaler_t prescaler, uint16_t modulo, tpm_overflow_interrupt_t tpmOverflowInterruptStatus , tpm_counting_mod_t countingMod);
void tpm_timer_start(TPM_Type* tpmType);
void tpm_timer_stop (TPM_Type* tpmType);

#endif /* TPM_H */
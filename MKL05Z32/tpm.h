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

#define RESET 0

typedef enum
{
	LPTPM_DISABLED = 0,
	LPTPM_INCREMENTS,
	LPTPM_INCREMENTS_WITH_RISING_EDGE,
	LPTPM_RESERVED
} tpm_cmod_t;


typedef enum
{
	TPM_CLOCK_DISABLED = 0,
	TPM_CLOCK_MCGFLLCLK,
	TPM_CLOCK_OSCERCLK,
	TPM_CLOCK_MCGIRCLK
} tpm_tpmsrc_t;

typedef enum  
{
	TPM_DIVIDE1 = 0,
	TPM_DIVIDE2 = 1,
	TPM_DIVIDE4,
  TPM_DIVIDE8,
  TPM_DIVIDE16,
  TPM_DIVIDE32,
  TPM_DIVIDE64,
  TPM_DIVIDE128
}tpm_prescaler_t;

typedef enum
{
	TPM_PWM_CHANNEL_0 = 0,
	TPM_PWM_CHANNEL_1,
	TPM_PWM_CHANNEL_2,
	TPM_PWM_CHANNEL_3,
	TPM_PWM_CHANNEL_4,
	TPM_PWM_CHANNEL_5,
	TPM_PWM_CHANNEL_NC
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

tpm_status_t tpm_init(TPM_Type* tpmType, tpm_prescaler_t prescaler,tpm_tpmsrc_t  clk, uint16_t module,
									tpm_overflow_interrupt_t tpmOverflowInterruptStatus , tpm_counting_mod_t countingMod);
void tpm_timer_start(TPM_Type* tpmType);
void tpm_timer_stop (TPM_Type* tpmType);
void tpm_timer_reset(TPM_Type* tpmType);
void tpm_cmod_change(TPM_Type* tpmType,tpm_cmod_t cmodChose);

#endif /* TPM_H */
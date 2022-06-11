/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 09.06.2022r.
----------------------------------------------------------------------------*/
#include "tpm.h"

tpm_status_t tpm_init(TPM_Type* tpmType, tpm_prescaler_t prescaler, uint16_t modulo, tpm_overflow_interrupt_t tpmOverflowInterruptStatus , tpm_counting_mod_t countingMod)
{
	tpm_status_t status = tpm_not_ok;
	
	if(tpmType == TPM0)
	{
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
		NVIC_EnableIRQ(TPM0_IRQn);
	}
	else if(tpmType == TPM1)
	{
		SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
		NVIC_EnableIRQ(TPM1_IRQn);
	}
	else
	{
		status = tpm_error;
	}
	
	if(countingMod == TPM_CENTER_PWM)
	{
		tpmType->SC |= TPM_SC_CPWMS_MASK;
	}
	else if(countingMod == TPM_EDGE_PWM)
	{
		tpmType->SC &= ~TPM_SC_CPWMS_MASK;
	}
	else if(countingMod == TPM_OFF_PWM)
	{
		/* no action */
	}
	else
	{
		status = tpm_error;
	}
	
	if(tpm_error != status)
	{
		status = tpm_ok;
		SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);	
		tpmType->CNT 	= TPM_CNT_COUNT(0);
		tpmType->MOD = modulo;
		if(TPM_TOIE_ON == tpmOverflowInterruptStatus)
		{
			tpmType->SC |= TPM_SC_TOIE_MASK | TPM_SC_PS(prescaler);	;
		}
		else
		{
			tpmType->SC |= TPM_SC_PS(prescaler);	
		}
	}
	else
	{
		/* no action */
	}
	return status;
}

void tpm_timer_start(TPM_Type* tpmType)
{
	tpmType->SC |= TPM_SC_CMOD(1);
}

void tpm_timer_stop(TPM_Type* tpmType)
{
	tpmType->SC |= TPM_SC_CMOD(0);
}



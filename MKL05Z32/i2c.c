/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 15.05.2022r.
----------------------------------------------------------------------------*/
/*
TODO

(C1 REGISTER)
I2C Interrupt Enable IICIE 
Master Mode Select MST (0 SLAVE 1 MASTER)
Transmit Mode Select TX (O RECIVE 1 TRANSMIT)
Transmit Acknowledge Enable TXAK 
Repeat START RESTA
Wakeup Enable WUEN

(AFTER LBILARY DO DMA CONFIG)
DMA Enable DMAEN
*/

/*
I2C baud rate = bus speed (Hz)/(mul ¡Á SCL divider)
SDA hold time = bus period (s) ¡Á mul ¡Á SDA hold value
SCL start hold time = bus period (s) ¡Á mul ¡Á SCL start hold value
SCL stop hold time = bus period (s) ¡Á mul ¡Á SCL stop hold value
*/

#include "i2c.h"

#define SCL   3
#define SDA   4

static i2c_status error;
static uint16_t timeout;

void i2c_init(i2c_mult_t mult, uint8_t icr)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;		/* clock for PORTB */
	
	PORTB->PCR[SCL] |= PORT_PCR_MUX(2); 	/* I2C0 SCL	(PTB3) */
	PORTB->PCR[SDA] |= PORT_PCR_MUX(2); 	/* I2C0 SDA	(PTB4) */	
	
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK ;		/* clock for I2C0  */
	
	I2C0->C1 &= ~(I2C_C1_IICEN_MASK);			/* disable module during modyfications*/
	I2C0->F = I2C_F_ICR(icr) | I2C_F_MULT(mult);
	I2C0->C1 = 0; 								// Clear
	I2C0->C1 |= I2C_C1_IICEN_MASK;/*Enable IIC*/
}

i2c_status i2c_start(void)
{
	error = i2c_succes;
	I2C0->C1 |= I2C_C1_TX_MASK;
  I2C0->C1 |= I2C_C1_MST_MASK;
	while((!(I2C0->S & I2C_S_BUSY_MASK)) && (timeout<10000))
    timeout++;
	if (timeout >= 10000)
    error = i2c_failure;
  return error; // Wait until BUSY=1
}

i2c_status i2c_stop(void)
{
	error = i2c_succes;
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	while((!(I2C0->S & I2C_S_BUSY_MASK)) && (timeout<10000))
    timeout++;
	if (timeout >= 10000)
    error = i2c_failure;
  return error; // Wait until BUSY=1
}

void i2c_wait(void)
{
	uint32_t i = 100;
	while(((I2C0->S & I2C_S_IICIF_MASK) == 0) && i)
	{
		i--;
	}
	I2C0->S |= I2C_S_IICIF_MASK;
}








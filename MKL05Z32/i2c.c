/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 15.05.2022r.
----------------------------------------------------------------------------*/
#include "i2c.h"

#define SCL   3
#define SDA   4

static i2c_status i2c_waitUintilBusy(void)
{
	i2c_status error = i2c_succes;
	uint16_t timeout;
	while((!(I2C0->S & I2C_S_BUSY_MASK)) && (timeout<10000))
    timeout++;
	if (timeout >= 10000)
    error = i2c_failure;
  return error; // Wait until BUSY=1
}

static i2c_status i2c_master_start(void)
{
	i2c_status error = i2c_succes;
  I2C0->C1 |= I2C_C1_MST_MASK;
	error = i2c_waitUintilBusy();
  return error; 
}

static i2c_status i2c_master_stop(void)
{
	i2c_status error = i2c_succes;
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	error = i2c_waitUintilBusy();
  return error; 
}

static i2c_status i2c_master_rstart(void) {
	i2c_status error = i2c_succes;
	uint8_t tempF = I2C0->F;								
	I2C0->F   = CLEAR;												
  I2C0->C1 |= I2C_C1_RSTA_MASK;
	I2C0->F   = tempF;			
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_transmit(void)
{
	i2c_status error = i2c_succes;
	I2C0->C1 |= I2C_C1_TX_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_recive(void)
{
	i2c_status error = i2c_succes;
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_enable(void) 
{
	i2c_status error = i2c_succes;
  I2C0->C1 |= I2C_C1_IICEN_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_disable(void) 
{
	i2c_status error = i2c_succes;
  I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_send(uint8_t data) 
{
	i2c_status error = i2c_succes;
	I2C0->D = data;
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_recv(uint8_t* data)
{
	i2c_status error = i2c_succes;
	*data = I2C0->D;
	error = i2c_waitUintilBusy();	
	return error;
}

static i2c_status i2c_nack(void) 
{
	i2c_status error = i2c_succes;
  I2C0->C1 |= I2C_C1_TXAK_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}
/**
 * @brief I2C transmit acknowledge bit.
 */
static i2c_status  i2c_ack(void) 
{
	i2c_status error = i2c_succes;
  I2C0->C1 &= ~I2C_C1_TXAK_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}
static i2c_status  i2c_clr_IICIF(void) 
{
	i2c_status error = i2c_succes;
  I2C0->S |= I2C_S_IICIF_MASK;
	error = i2c_waitUintilBusy();	
	return error;
}

static void i2c_wait(void)
{
	uint32_t i = 100;
	while(((I2C0->S & I2C_S_IICIF_MASK) == 0) && i)
	{
		i--;
	}
	I2C0->S |= I2C_S_IICIF_MASK;
}

i2c_status i2c_init(i2c_mult_t mult, uint8_t icr)
{
	i2c_status error = i2c_succes;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;		/* clock for PORTB */
	
	PORTB->PCR[SCL] |= PORT_PCR_MUX(2); 	/* I2C0 SCL	(PTB3) */
	PORTB->PCR[SDA] |= PORT_PCR_MUX(2); 	/* I2C0 SDA	(PTB4) */	
	
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK ;		/* clock for I2C0  */
	
	I2C0->C1 &= ~(I2C_C1_IICEN_MASK);			/* disable module during modyfications*/
	I2C0->F = I2C_F_ICR(icr) | I2C_F_MULT(mult);
	I2C0->C1 = 0; 								// Clear
	I2C0->C1 |= I2C_C1_IICEN_MASK;/*Enable IIC*/
	error = i2c_waitUintilBusy();	
	return error;
}

i2c_status i2c_Write(uint8_t address, uint8_t data) 
{
	
	i2c_status error = i2c_succes;
	
	error &= i2c_enable();
	error &= i2c_transmit();															
	error &= i2c_master_rstart();													
	error &= i2c_send((uint8_t)(address << 1));  		
	i2c_wait();															
	error &= i2c_send(data);													
	i2c_wait();
	error &= i2c_master_stop();												
	error &= i2c_disable();
	
	return error;
}

i2c_status i2c_Read(uint8_t address, uint8_t* data) 
{
	uint8_t dummy = 0;
	i2c_status error = i2c_succes;
	error &= i2c_enable();
	error &= i2c_transmit();															
	error &= i2c_master_rstart();													
	error &= i2c_send((uint8_t)(address << 1)|0x01); 		
	i2c_wait();	
	error &= i2c_recive();	
	error &= i2c_nack();		
	error &= i2c_recv(&dummy);		
	i2c_wait();
	error &= i2c_master_stop();			
	error &= i2c_recv(data);	
	error &= i2c_disable();
	
	return error;
}








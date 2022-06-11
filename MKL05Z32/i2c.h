/*-------------------------------------------------------------------------------------
					autor: Pawel Smietana
					version: 09.06.2022r.
----------------------------------------------------------------------------*/
#ifndef I2C_H
#define I2C_H

#include "MKL05Z4.h"

#define CLEAR 0

typedef enum
{
	Mult_1 = 0,
	Mult_2,
	Mult_4,
}i2c_mult_t;

typedef enum
{
	i2c_succes = !0,
	i2c_failure = 0
} i2c_status;

i2c_status i2c_init(i2c_mult_t mult, uint8_t icr);
i2c_status i2c_Write(uint8_t address, uint8_t data);
i2c_status i2c_Read(uint8_t address, uint8_t* data);


#endif /* I2C_H */
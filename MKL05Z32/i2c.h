#ifndef I2C_H
#define I2C_H

#include "MKL05Z4.h"

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

void i2c_init(i2c_mult_t mult, uint8_t icr);
i2c_status i2c_start(void);


#endif /* I2C_H */
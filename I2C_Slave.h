/*
 * I2C_Slave.h
 *
 *  Created on: 15/03/2019
 *      Author: Juan Avelar
 */

#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

#include <stdint.h>
#define SimpleLink_Slave_Address 0x44
void        I2C_Slave_init(uint8_t address);
void        I2C_Slave_Store_Data(void);
uint32_t    I2C_Slave_Get_Data(void);
void        I2C_Slave_close(void);

#endif /* I2C_SLAVE_H_ */

/*
 * I2C_Slave.c
 *
 *  Created on: 15/03/2019
 *      Author: Juan Avelar
 */
#define SimpleLink_Slave_Dir 0x40020000
#define SimpleLink_Master_Dir 0x40020000

#define INT_I2CA0               24          // I2C controller

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//#include <functional>
#include <ti/devices/cc32xx/driverlib/i2c.h>
#include <ti/devices/cc32xx/inc/hw_types.h>//utilizado en pin.h
#include <ti/devices/cc32xx/driverlib/pin.h>
#include <ti/devices/cc32xx/driverlib/interrupt.h>

#include "I2C_Slave.h"
#include <ti/drivers/power/PowerCC32XX.h>
#include <ti/drivers/i2c/I2CCC32XX.h>

static uint32_t data_from_master;
tBoolean* data_ready;
void        I2C_Slave_Store_Data(void);

void        I2C_Slave_init(uint8_t address){
    //para poder usare sta funcion se debe inicializar el power management
    Power_setDependency(PowerCC32XX_PERIPH_I2CA0);
    I2CMasterDisable(SimpleLink_Master_Dir);
    I2CSlaveDisable(SimpleLink_Slave_Dir);
    IntPendClear(INT_I2CA0);
    I2CSlaveIntClear(SimpleLink_Slave_Dir);//interrupt.h
    //GPIO10 y 11 pueden ser usados como SDA y SCL y se configuran en GPIO_PAD_CONFIG
    //scl
    PinTypeI2C(I2CCC32XX_PIN_01_I2C_SCL & 0xFF, I2CCC32XX_PIN_01_I2C_SCL >> 8);//pin.h
    //sda
    PinTypeI2C(I2CCC32XX_PIN_02_I2C_SDA & 0xFF, I2CCC32XX_PIN_02_I2C_SDA >> 8);//pin.h
    //clock_enable();Se empieza con Ti no rtos
    I2CSlaveInit        (SimpleLink_Slave_Dir,address);
    //I2CSlaveACKOverride (SimpleLink_Slave_Dir,true);
    data_from_master = 0;
    //NVIC_enable();Se habilita cuando le pasas la funcion
    //void (*FcnPtr)() = &I2C_Slave_Store_Data;
    //std::function<void (const esclavo_i2c&)> FcnPtr = func;
    I2CIntRegister      (SimpleLink_Slave_Dir,&I2C_Slave_Store_Data);
    I2CSlaveIntEnable   (SimpleLink_Slave_Dir);

}
/*Interrupt handler*/
void        I2C_Slave_Store_Data(void){
    data_from_master = I2CSlaveDataGet(SimpleLink_Slave_Dir);
    //data_from_master = data_from_master << 8;
    //data_from_master |= I2CSlaveDataGet(SimpleLink_Slave_Dir);
    I2CSlaveIntClear(SimpleLink_Slave_Dir);
    *data_ready = true;
}
uint32_t    I2C_Slave_Get_Data(void){
    return data_from_master;
    //*data_ready = false;
}
void        I2C_Slave_close(void){
    I2CSlaveDisable(SimpleLink_Slave_Dir);
    IntPendClear(INT_I2CA0);
}


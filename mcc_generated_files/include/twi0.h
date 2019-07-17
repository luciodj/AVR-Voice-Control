/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../utils/compiler.h"
#include "i2c_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TWI0_BAUD(F_SCL, T_RISE)    \
    ((((((float)10000000 / (float)F_SCL)) - 10 - ((float)10000000 * T_RISE / 1000000))) / 2)

/**
 * \brief Initialize I2C interface
 * If module is configured to disabled state, the clock to the I2C is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the init was successful
 * \retval 1 the init was not successful
 */
uint8_t I2C_0_init(void);

/**
 * \brief Open the I2C for communication
 *
 * \param[in] address The slave address to use in the transfer
 *
 * \return Initialization status.
 * \retval I2C_NOERR The I2C open was successful
 * \retval I2C_BUSY  The I2C open failed because the interface is busy
 * \retval I2C_FAIL  The I2C open failed with an error
 */
i2c_error_t I2C_0_open(i2c_address_t address);

/**
 * \brief Close the I2C interface
 *
 * \return Status of close operation.
 * \retval I2C_NOERR The I2C open was successful
 * \retval I2C_BUSY  The I2C open failed because the interface is busy
 * \retval I2C_FAIL  The I2C open failed with an error
 */
i2c_error_t I2C_0_close(void);

/**
 * \brief Start an operation on an opened I2C interface
 *
 * \param[in] read Set to true for read, false for write
 *
 * \return Status of operation
 * \retval I2C_NOERR The I2C open was successful
 * \retval I2C_BUSY  The I2C open failed because the interface is busy
 * \retval I2C_FAIL  The I2C open failed with an error
 */
i2c_error_t I2C_0_master_operation(bool read);

/**
 * \brief Identical to I2C_0_master_operation(false);
 */
i2c_error_t I2C_0_master_write(void); // to be depreciated

/**
 * \brief Identical to I2C_0_master_operation(true);
 */
i2c_error_t I2C_0_master_read(void); // to be depreciated

/**
 * \brief Set timeout to be used for I2C operations. Uses the Timeout driver.
 *
 * \param[in] to Timeout in ticks
 *
 * \return Nothing
 */
void I2C_0_set_timeout(uint8_t to);

/**
 * \brief Sets up the data buffer to use, and number of bytes to transfer
 *
 * \param[in] buffer Pointer to data buffer to use for read or write data
 * \param[in] bufferSize Number of bytes to read or write from slave
 *
 * \return Nothing
 */
void I2C_0_set_buffer(void *buffer, size_t bufferSize);

// Event Callback functions.

/**
 * \brief Set callback to be called when all specifed data has been transferred.
 *
 * \param[in] cb Pointer to callback function
 * \param[in] p  Pointer to the callback function's parameters
 *
 * \return Nothing
 */
void I2C_0_set_data_complete_callback(i2c_callback cb, void *p);

/**
 * \brief Set callback to be called when there has been a bus collision and arbitration was lost.
 *
 * \param[in] cb Pointer to callback function
 * \param[in] p  Pointer to the callback function's parameters
 *
 * \return Nothing
 */
void I2C_0_set_write_collision_callback(i2c_callback cb, void *p);

/**
 * \brief Set callback to be called when the transmitted address was NACK'ed.
 *
 * \param[in] cb Pointer to callback function
 * \param[in] p  Pointer to the callback function's parameters
 *
 * \return Nothing
 */
void I2C_0_set_address_nack_callback(i2c_callback cb, void *p);

/**
 * \brief Set callback to be called when the transmitted data was NACK'ed.
 *
 * \param[in] cb Pointer to callback function
 * \param[in] p  Pointer to the callback function's parameters
 *
 * \return Nothing
 */
void I2C_0_set_data_nack_callback(i2c_callback cb, void *p);

/**
 * \brief Set callback to be called when there was a bus timeout.
 *
 * \param[in] cb Pointer to callback function
 * \param[in] p  Pointer to the callback function's parameters
 *
 * \return Nothing
 */
void I2C_0_set_timeout_callback(i2c_callback cb, void *p);

#ifdef __cplusplus
}
#endif

#endif /* I2C_MASTER_H_INCLUDED */
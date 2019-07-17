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

#include "../include/twi0.h"
#include "../include/i2c_types.h"
#include <stdbool.h>
#include <stdlib.h>

/***************************************************************************/
// I2C STATES
typedef enum {
    I2C_IDLE = 0,
    I2C_SEND_ADR_READ,
    I2C_SEND_ADR_WRITE,
    I2C_TX,
    I2C_RX,
    I2C_TX_EMPTY,
    I2C_SEND_RESTART_READ,
    I2C_SEND_RESTART_WRITE,
    I2C_SEND_RESTART,
    I2C_SEND_STOP,
    I2C_RX_DO_ACK,
    I2C_TX_DO_ACK,
    I2C_RX_DO_NACK_STOP,
    I2C_RX_DO_NACK_RESTART,
    I2C_RESET,
    I2C_ADDRESS_NACK,
    I2C_BUS_COLLISION,
    I2C_BUS_ERROR
} i2c_fsm_states_t;

// I2C Event Callback List
typedef enum {
    i2c_dataComplete = 0,
    i2c_writeCollision,
    i2c_addressNACK,
    i2c_dataNACK,
    i2c_timeOut,
    i2c_NULL
} i2c_callback_index;

// I2C Status Structure
typedef struct {
    unsigned         busy : 1;
    unsigned         inUse : 1;
    unsigned         bufferFree : 1;
    unsigned         addressNACKCheck : 1;
    i2c_address_t    address;       /// The I2C Address
    uint8_t *        data_ptr;      /// pointer to a data buffer
    size_t           data_length;   /// Bytes in the data buffer
    uint16_t         timeout;       /// I2C Timeout Counter between I2C Events.
    uint16_t         timeout_value; /// Reload value for the timeouts
    i2c_fsm_states_t state;         /// Driver State
    i2c_error_t      error;
    /*if timeoutDriverEnabled
    timerStruct_t timeout;
    */
    i2c_callback callbackTable[6];
    void *       callbackPayload[6]; ///  each callback can have a payload
} i2c_status_t;

typedef i2c_fsm_states_t(stateHandlerFunction)(void);

i2c_status_t I2C_0_status = {0};

static void I2C_0_set_callback(i2c_callback_index idx, i2c_callback cb, void *p);
static i2c_operations_t I2C_0_return_stop(void *p);
static i2c_operations_t I2C_0_return_reset(void *p);
static i2c_fsm_states_t I2C_0_do_I2C_SEND_ADR_READ(void);
static i2c_fsm_states_t I2C_0_do_I2C_SEND_ADR_WRITE(void);
static void             I2C_0_master_isr(void);

void I2C_0_set_data_complete_callback(i2c_callback cb, void *p)
{
    I2C_0_set_callback(i2c_dataComplete, cb, p);
}

void I2C_0_set_write_collision_callback(i2c_callback cb, void *p)
{
    I2C_0_set_callback(i2c_writeCollision, cb, p);
}

void I2C_0_set_address_nack_callback(i2c_callback cb, void *p)
{
    I2C_0_set_callback(i2c_addressNACK, cb, p);
}

void I2C_0_set_data_nack_callback(i2c_callback cb, void *p)
{
    I2C_0_set_callback(i2c_dataNACK, cb, p);
}

void I2C_0_set_timeout_callback(i2c_callback cb, void *p)
{
    I2C_0_set_callback(i2c_timeOut, cb, p);
}

uint8_t I2C_0_init()
{

    //     TWI0.CTRLA = 0 << TWI_FMPEN_bp /* FM Plus Enable: disabled */
    //                 | TWI_SDAHOLD_OFF_gc /* SDA Hold Time: SDA hold time off */
    //                 | TWI_SDASETUP_4CYC_gc; /* SDA Setup Time: SDA setup time is 4 clock cycles */

    //     TWI0.DBGCTRL = 0 << TWI_DBGRUN_bp; /* Debug Run: disabled */

    TWI0.MBAUD = (uint8_t)TWI0_BAUD(100000, 0); /* Master Baud Rate Control */

        TWI0.MCTRLA = 1 << TWI_ENABLE_bp /* Enable TWI Master: enabled */
                    | 0 << TWI_QCEN_bp /* Quick Command Enable: disabled */
                    | 1 << TWI_RIEN_bp /* Read Interrupt Enable: enabled */
                    | 0 << TWI_SMEN_bp /* Smart Mode Enable: disabled */
                    | TWI_TIMEOUT_DISABLED_gc /* Inactive Bus Timeout: Bus Timeout Disabled */
                    | 1 << TWI_WIEN_bp; /* Write Interrupt Enable: enabled */

    //     TWI0.MSTATUS = 0 << TWI_ARBLOST_bp /* Arbitration Lost: disabled */
    //                 | 0 << TWI_BUSERR_bp /* Bus Error: disabled */
    //                 | TWI_BUSSTATE_UNKNOWN_gc /* Bus State: Unknown Bus State */
    //                 | 0 << TWI_CLKHOLD_bp /* Clock Hold: disabled */
    //                 | 0 << TWI_RIF_bp /* Read Interrupt Flag: disabled */
    //                 | 0 << TWI_WIF_bp; /* Write Interrupt Flag: disabled */

    //     TWI0.MADDR = 0; /* Master Address */

    //     TWI0.MCTRLB = TWI_ACKACT_ACK_gc /* Acknowledge Action: Send ACK */
    //                 | 0 << TWI_FLUSH_bp /* Flush: disabled */
    //                 | TWI_MCMD_NOACT_gc; /* Command: No Action */

    //     TWI0.MDATA = 0; /* Master Data */

    return 0;
}

// when you call open, you supply a device address.
// if you get the bus, the function returns true
i2c_error_t I2C_0_open(i2c_address_t address)
{
    i2c_error_t ret = I2C_BUSY;

    if (!I2C_0_status.inUse) {
        I2C_0_status.address          = address;
        I2C_0_status.busy             = 0;
        I2C_0_status.inUse            = 1;
        I2C_0_status.addressNACKCheck = 0;
        I2C_0_status.state            = I2C_RESET;
        I2C_0_status.timeout_value    = 500; // MCC should determine a reasonable starting value here.
        I2C_0_status.bufferFree       = 1;
        
        // set all the call backs to a default of sending stop
        I2C_0_status.callbackTable[i2c_dataComplete]     = I2C_0_return_stop;
        I2C_0_status.callbackPayload[i2c_dataComplete]   = NULL;
        I2C_0_status.callbackTable[i2c_writeCollision]   = I2C_0_return_stop;
        I2C_0_status.callbackPayload[i2c_writeCollision] = NULL;
        I2C_0_status.callbackTable[i2c_addressNACK]      = I2C_0_return_stop;
        I2C_0_status.callbackPayload[i2c_addressNACK]    = NULL;
        I2C_0_status.callbackTable[i2c_dataNACK]         = I2C_0_return_stop;
        I2C_0_status.callbackPayload[i2c_dataNACK]       = NULL;
        I2C_0_status.callbackTable[i2c_timeOut]          = I2C_0_return_reset;
        I2C_0_status.callbackPayload[i2c_timeOut]        = NULL;

        TWI0.MCTRLB |= TWI_FLUSH_bm;
        TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;
        // Reset module
        TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);

        // uncomment the IRQ enable for an interrupt driven driver.
        TWI0.MCTRLA |= (TWI_RIEN_bm | TWI_WIEN_bm);

        ret = I2C_NOERR;
    }
    return ret;
}

void I2C_0_set_address(i2c_address_t address)
{
    I2C_0_status.address = address;
}

// close the bus if it is not busy
i2c_error_t I2C_0_close(void)
{
    i2c_error_t ret = I2C_BUSY;
    // Bus is in error state, reset I2C hardware and report error
    if (TWI0.MSTATUS & TWI_BUSERR_bm) {
        I2C_0_status.busy  = false;
        I2C_0_status.error = I2C_FAIL;
    }
    if (!I2C_0_status.busy) {
        I2C_0_status.inUse = 0;
        // close it down
        I2C_0_status.address = 0xff; // 8-bit address is invalid so this is FREE
        TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);
        TWI0.MCTRLA &= ~(TWI_RIEN_bm | TWI_WIEN_bm);
        ret = I2C_0_status.error;
    }
    return ret;
}

void I2C_0_set_timeout(uint8_t to)
{
    TWI0.MCTRLA &= ~(TWI_RIEN_bm | TWI_WIEN_bm);
    I2C_0_status.timeout_value = to;
    TWI0.MCTRLA |= (TWI_RIEN_bm | TWI_WIEN_bm);
}

void I2C_0_set_buffer(void *buffer, size_t bufferSize)
{
    if (I2C_0_status.bufferFree) {
        I2C_0_status.data_ptr    = buffer;
        I2C_0_status.data_length = bufferSize;
        I2C_0_status.bufferFree  = false;
    }
}
i2c_error_t I2C_0_master_operation(bool read)
{
    i2c_error_t ret = I2C_BUSY;
    if (!I2C_0_status.busy) {
        I2C_0_status.busy = true;
        ret                       = I2C_NOERR;

        if (read) {
            I2C_0_status.state = I2C_SEND_ADR_READ;
        } else {
            I2C_0_status.state = I2C_SEND_ADR_WRITE;
        }
        I2C_0_master_isr();
    }
    return ret;
}

i2c_error_t I2C_0_master_read(void)
{
    return I2C_0_master_operation(true);
}

i2c_error_t I2C_0_master_write(void)
{
    return I2C_0_master_operation(false);
}

/************************************************************************/
/* Helper Functions                                                     */
/************************************************************************/

static i2c_fsm_states_t I2C_0_do_I2C_RESET(void)
{
    TWI0.MCTRLB |= TWI_FLUSH_bm;
    TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;
    I2C_0_status.busy  = false; // Bus Free
    I2C_0_status.error = I2C_NOERR;
    return I2C_RESET; // park the FSM on reset
}

static i2c_fsm_states_t I2C_0_do_I2C_IDLE(void)
{
    I2C_0_status.busy  = false; // Bus Free
    I2C_0_status.error = I2C_NOERR;
    return I2C_IDLE; // park the FSM on IDLE
}

static i2c_fsm_states_t I2C_0_do_I2C_SEND_RESTART_READ(void)
{
    return I2C_0_do_I2C_SEND_ADR_READ();
}

static i2c_fsm_states_t I2C_0_do_I2C_SEND_RESTART_WRITE(void)
{
    return I2C_0_do_I2C_SEND_ADR_WRITE();
}

static i2c_fsm_states_t I2C_0_do_I2C_SEND_RESTART(void)
{
    return I2C_0_do_I2C_SEND_ADR_READ();
}

static i2c_fsm_states_t I2C_0_do_I2C_SEND_STOP(void)
{
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
    return I2C_0_do_I2C_IDLE();
}

// TODO: probably need 2 addressNACK's one from read and one from write.
//       the do NACK before RESTART or STOP is a special case that a new state simplifies.
static i2c_fsm_states_t I2C_0_do_I2C_DO_ADDRESS_NACK(void)
{
    I2C_0_status.addressNACKCheck = 0;
    I2C_0_status.error            = I2C_FAIL;
    switch (I2C_0_status.callbackTable[i2c_addressNACK](I2C_0_status.callbackPayload[i2c_addressNACK])) {
    case i2c_restart_read:
        return I2C_0_do_I2C_SEND_RESTART_READ();
    case i2c_restart_write:
        return I2C_0_do_I2C_SEND_RESTART_WRITE();
    default:
        return I2C_0_do_I2C_SEND_STOP();
    }
}

static i2c_fsm_states_t I2C_0_do_I2C_SEND_ADR_READ(void)
{

    I2C_0_status.addressNACKCheck = 1;
    TWI0.MADDR                            = I2C_0_status.address << 1 | 1;
    return I2C_RX;
}

static i2c_fsm_states_t I2C_0_do_I2C_SEND_ADR_WRITE(void)
{

    I2C_0_status.addressNACKCheck = 1;
    TWI0.MADDR                            = I2C_0_status.address << 1;
    return I2C_TX;
}

static i2c_fsm_states_t I2C_0_do_I2C_RX_DO_ACK(void)
{
    TWI0.MCTRLB &= ~(1 << TWI_ACKACT_bp);
    return I2C_RX;
}

static i2c_fsm_states_t I2C_0_do_I2C_TX_DO_ACK(void)
{
    TWI0.MCTRLB &= ~(1 << TWI_ACKACT_bp);
    return I2C_TX;
}

static i2c_fsm_states_t I2C_0_do_I2C_DO_NACK_STOP(void)
{
    TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
    TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
    return I2C_0_do_I2C_IDLE();
}

static i2c_fsm_states_t I2C_0_do_I2C_DO_NACK_RESTART(void)
{
    TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
    return I2C_SEND_RESTART;
}

static i2c_fsm_states_t I2C_0_do_I2C_TX(void)
{
    if ((TWI0.MSTATUS & TWI_RXACK_bm)) // Slave replied with NACK
    {
        switch (I2C_0_status.callbackTable[i2c_dataNACK](I2C_0_status.callbackPayload[i2c_dataNACK])) {
        case i2c_restart_read:
            return I2C_0_do_I2C_SEND_RESTART_READ();
        case i2c_restart_write:
            return I2C_0_do_I2C_SEND_RESTART_WRITE();
        default:
        case i2c_continue:
        case i2c_stop:
            return I2C_0_do_I2C_SEND_STOP();
        }
    } else {
        I2C_0_status.addressNACKCheck = 0;
        TWI0.MDATA                            = *I2C_0_status.data_ptr++;
        return (--I2C_0_status.data_length) ? I2C_TX : I2C_TX_EMPTY;
    }
}

static i2c_fsm_states_t I2C_0_do_I2C_RX(void)
{
    I2C_0_status.addressNACKCheck = 0;

    if (I2C_0_status.data_length == 1)
        TWI0.MCTRLB |= TWI_ACKACT_NACK_gc; // Next byte will be last to be received, setup NACK
    else
        TWI0.MCTRLB &= ~(1 << TWI_ACKACT_bp); // More bytes to receive, setup ACK

    if (--I2C_0_status.data_length) {
        *I2C_0_status.data_ptr = TWI0.MDATA;
        I2C_0_status.data_ptr++;
        TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc;
        return I2C_RX;
    } else {
        *I2C_0_status.data_ptr = TWI0.MDATA;
        I2C_0_status.data_ptr++;
        I2C_0_status.bufferFree = true;
        switch (I2C_0_status.callbackTable[i2c_dataComplete](I2C_0_status.callbackPayload[i2c_dataComplete])) {
        case i2c_restart_write:
        case i2c_restart_read:
            return I2C_0_do_I2C_DO_NACK_RESTART();
        default:
        case i2c_continue:
        case i2c_stop:
            return I2C_0_do_I2C_DO_NACK_STOP();
        }
    }
}

static i2c_fsm_states_t I2C_0_do_I2C_TX_EMPTY(void)
{
    if ((TWI0.MSTATUS & TWI_RXACK_bm)) // Slave replied with NACK
    {
        switch (I2C_0_status.callbackTable[i2c_dataNACK](I2C_0_status.callbackPayload[i2c_dataNACK])) {
        case i2c_restart_read:
            return I2C_0_do_I2C_SEND_RESTART_READ();
        case i2c_restart_write:
            return I2C_0_do_I2C_SEND_RESTART_WRITE();
        default:
        case i2c_continue:
        case i2c_stop:
            return I2C_0_do_I2C_SEND_STOP();
        }
    } else {
        I2C_0_status.bufferFree = true;
        switch (I2C_0_status.callbackTable[i2c_dataComplete](I2C_0_status.callbackPayload[i2c_dataComplete])) {
        case i2c_restart_read:
            return I2C_0_do_I2C_SEND_RESTART_READ();
        case i2c_restart_write:
            return I2C_0_do_I2C_SEND_RESTART_WRITE();
        case i2c_continue:
            return I2C_0_do_I2C_TX();
        default:
        case i2c_stop:
            return I2C_0_do_I2C_SEND_STOP();
        }
    }
}

static i2c_fsm_states_t I2C_0_do_I2C_BUS_COLLISION(void)
{
    // Clear bus collision status flag
    TWI0.MSTATUS |= TWI_ARBLOST_bm;

    I2C_0_status.error = I2C_FAIL;
    switch (I2C_0_status.callbackTable[i2c_writeCollision](I2C_0_status.callbackPayload[i2c_writeCollision])) {
    case i2c_restart_read:
        return I2C_0_do_I2C_SEND_RESTART_READ();
    case i2c_restart_write:
        return I2C_0_do_I2C_SEND_RESTART_WRITE();
    default:
        return I2C_0_do_I2C_RESET();
    }
}

static i2c_fsm_states_t I2C_0_do_I2C_BUS_ERROR(void)
{
    TWI0.MCTRLB |= TWI_FLUSH_bm;
    TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;
    I2C_0_status.busy  = false;
    I2C_0_status.error = I2C_FAIL;
    return I2C_RESET; // park the FSM on reset
}

stateHandlerFunction *I2C_0_fsmStateTable[] = {
    I2C_0_do_I2C_IDLE,               // I2C_IDLE
    I2C_0_do_I2C_SEND_ADR_READ,      // I2C_SEND_ADR_READ
    I2C_0_do_I2C_SEND_ADR_WRITE,     // I2C_SEND_ADR_WRITE
    I2C_0_do_I2C_TX,                 // I2C_TX
    I2C_0_do_I2C_RX,                 // I2C_RX
    I2C_0_do_I2C_TX_EMPTY,           // I2C_TX_EMPTY
    I2C_0_do_I2C_SEND_RESTART_READ,  // I2C_SEND_RESTART_READ
    I2C_0_do_I2C_SEND_RESTART_WRITE, // I2C_SEND_RESTART_WRITE
    I2C_0_do_I2C_SEND_RESTART,       // I2C_SEND_RESTART
    I2C_0_do_I2C_SEND_STOP,          // I2C_SEND_STOP
    I2C_0_do_I2C_RX_DO_ACK,          // I2C_RX_DO_ACK
    I2C_0_do_I2C_TX_DO_ACK,          // I2C_TX_DO_ACK
    I2C_0_do_I2C_DO_NACK_STOP,       // I2C_RX_DO_NACK_STOP
    I2C_0_do_I2C_DO_NACK_RESTART,    // I2C_RX_DO_NACK_RESTART
    I2C_0_do_I2C_RESET,              // I2C_RESET
    I2C_0_do_I2C_DO_ADDRESS_NACK,    // I2C_ADDRESS_NACK
    I2C_0_do_I2C_BUS_COLLISION,      // I2C_BUS_COLLISION
    I2C_0_do_I2C_BUS_ERROR           // I2C_BUS_ERROR
};

ISR(TWI0_TWIM_vect)
{
    I2C_0_master_isr();
}

void I2C_0_master_isr(void)
{
    TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);

    // NOTE: We are ignoring the Write Collision flag.

    // Address phase received NACK from slave, override next state
    if (I2C_0_status.addressNACKCheck && (TWI0.MSTATUS & TWI_RXACK_bm)) {
        I2C_0_status.state = I2C_ADDRESS_NACK; // State Override
    }

    // Bus arbitration lost to another master, override next state
    if (TWI0.MSTATUS & TWI_ARBLOST_bm) {
        I2C_0_status.state = I2C_BUS_COLLISION; // State Override
    }

    // Bus error, override next state
    if (TWI0.MSTATUS & TWI_BUSERR_bm) {
        I2C_0_status.state = I2C_BUS_ERROR; // State Override
    }

    I2C_0_status.state = I2C_0_fsmStateTable[I2C_0_status.state]();
}

/************************************************************************/
/* Helper Functions                                                     */
/************************************************************************/
static i2c_operations_t I2C_0_return_stop(void *p)
{
    return i2c_stop;
}

static i2c_operations_t I2C_0_return_reset(void *p)
{
    return i2c_reset_link;
}

static void I2C_0_set_callback(i2c_callback_index idx, i2c_callback cb, void *p)
{
    if (cb) {
        I2C_0_status.callbackTable[idx]   = cb;
        I2C_0_status.callbackPayload[idx] = p;
    } else {
        I2C_0_status.callbackTable[idx]   = I2C_0_return_stop;
        I2C_0_status.callbackPayload[idx] = NULL;
    }
}
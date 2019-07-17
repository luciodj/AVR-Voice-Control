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

#include "../include/spi0.h"
#include "../include/pin_manager.h"


typedef struct SPI_0_descriptor_s {
    spi_transfer_status_t status;
} SPI_0_descriptor_t;


static SPI_0_descriptor_t SPI_0_desc;

uint8_t SPI_0_init()
{

        SPI0.CTRLA = 1 << SPI_CLK2X_bp /* Enable Double Speed: enabled */
                    | 0 << SPI_DORD_bp /* Data Order Setting: disabled */
                    | 1 << SPI_ENABLE_bp /* Enable Module: enabled */
                    | 1 << SPI_MASTER_bp /* Master Operation Enable: enabled */
                    | SPI_PRESC_DIV4_gc; /* Prescaler: System Clock / 4 */

    //     SPI0.CTRLB = 0 << SPI_BUFEN_bp /* Buffer Mode Enable: disabled */
    //                 | 0 << SPI_BUFWR_bp /* Buffer Write Mode: disabled */
    //                 | SPI_MODE_0_gc /* SPI Mode: SPI Mode 0 */
    //                 | 0 << SPI_SSD_bp; /* Slave Select Disable: disabled */

    //     SPI0.INTCTRL = 0 << SPI_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
    //                 | 0 << SPI_IE_bp /* Interrupt Enable: disabled */
    //                 | 0 << SPI_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
    //                 | 0 << SPI_SSIE_bp /* Slave Select Trigger Interrupt Enable: disabled */
    //                 | 0 << SPI_TXCIE_bp; /* Transfer Complete Interrupt Enable: disabled */
    
    SPI_0_desc.status = SPI_FREE;

    //     SPI0.INTFLAGS = 0 << SPI_BUFOVF_bp /* Buffer Overflow: disabled */
    //                 | 0 << SPI_DREIF_bp /* Data Register Empty Interrupt Flag: disabled */
    //                 | 0 << SPI_RXCIF_bp /* Receive Complete Interrupt Flag: disabled */
    //                 | 0 << SPI_SSIF_bp /* Slave Select Trigger Interrupt Flag: disabled */
    //                 | 0 << SPI_TXCIF_bp /* Transfer Complete Interrupt Flag: disabled */
    //                 | 0 << SPI_IF_bp /* Interrupt Flag: disabled */
    //                 | 0 << SPI_WRCOL_bp; /* Write Collision: disabled */

    return 0;
}

void SPI_0_enable()
{
    SPI0.CTRLA |= SPI_ENABLE_bm;
}

void SPI_0_disable()
{
    SPI0.CTRLA &= ~SPI_ENABLE_bm;
}


uint8_t SPI_0_exchange_byte(uint8_t data)
{
    // Blocking wait for SPI free makes the function work
    // seamlessly also with IRQ drivers.
    while (SPI_0_desc.status == SPI_BUSY)
        ;
    SPI0.DATA = data;
    while (!(SPI0.INTFLAGS & SPI_RXCIF_bm))
        ;
    return SPI0.DATA;
}

void SPI_0_exchange_block(void *block, uint8_t size)
{
    uint8_t *b = (uint8_t *)block;
    while (size--) {
        SPI0.DATA = *b;
        while (!(SPI0.INTFLAGS & SPI_RXCIF_bm))
            ;
        *b = SPI0.DATA;
        b++;
    }
}

void SPI_0_write_block(void *block, uint8_t size)
{
    uint8_t *b = (uint8_t *)block;
    while (size--) {
        SPI0.DATA = *b;
        while (!(SPI0.INTFLAGS & SPI_RXCIF_bm))
            ;
        b++;
    }
}

void SPI_0_read_block(void *block, uint8_t size)
{
    uint8_t *b = (uint8_t *)block;
    while (size--) {
        SPI0.DATA = 0;
        while (!(SPI0.INTFLAGS & SPI_RXCIF_bm))
            ;
        *b = SPI0.DATA;
        b++;
    }
}
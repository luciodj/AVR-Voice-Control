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

#include "../include/usart2.h"

#if defined(__GNUC__)

int USART_2_printCHAR(char character, FILE *stream)
{
    USART_2_write(character);
    return 0;
}

FILE USART_2_stream = FDEV_SETUP_STREAM(USART_2_printCHAR, NULL, _FDEV_SETUP_WRITE);

#elif defined(__ICCAVR__)

int putchar(int outChar)
{
    USART_2_write(outChar);
    return outChar;
}
#endif

/* Static Variables holding the ringbuffer used in IRQ mode */
static uint8_t          USART_2_rxbuf[USART_2_RX_BUFFER_SIZE];
static volatile uint8_t USART_2_rx_head;
static volatile uint8_t USART_2_rx_tail;
static volatile uint8_t USART_2_rx_elements;
static uint8_t          USART_2_txbuf[USART_2_TX_BUFFER_SIZE];
static volatile uint8_t USART_2_tx_head;
static volatile uint8_t USART_2_tx_tail;
static volatile uint8_t USART_2_tx_elements;

void USART_2_default_rx_isr_cb(void);
void (*USART_2_rx_isr_cb)(void) = &USART_2_default_rx_isr_cb;
void USART_2_default_udre_isr_cb(void);
void (*USART_2_udre_isr_cb)(void) = &USART_2_default_udre_isr_cb;

void USART_2_default_rx_isr_cb(void)
{
    uint8_t data;
    uint8_t tmphead;

    /* Read the received data */
    data = USART2.RXDATAL;
    /* Calculate buffer index */
    tmphead = (USART_2_rx_head + 1) & USART_2_RX_BUFFER_MASK;
        
    if (tmphead == USART_2_rx_tail) {
            /* ERROR! Receive buffer overflow */
    }else {
    /*Store new index*/
    USART_2_rx_head = tmphead;
    
    /* Store received data in buffer */
    USART_2_rxbuf[tmphead] = data;
    USART_2_rx_elements++;
    }
}

void USART_2_default_udre_isr_cb(void)
{
    uint8_t tmptail;

    /* Check if all data is transmitted */
    if (USART_2_tx_elements != 0) {
        /* Calculate buffer index */
        tmptail = (USART_2_tx_tail + 1) & USART_2_TX_BUFFER_MASK;
        /* Store new index */
        USART_2_tx_tail = tmptail;
        /* Start transmission */
        USART2.TXDATAL = USART_2_txbuf[tmptail];
        
        USART_2_tx_elements--;
    }

    if (USART_2_tx_elements == 0) {
            /* Disable UDRE interrupt */
            USART2.CTRLA &= ~(1 << USART_DREIE_bp);
    }
}

void USART_2_set_ISR_cb(usart_callback cb, usart_2_cb_t type)
{
    switch (type) {
        case USART_2_RX_CB:
                USART_2_rx_isr_cb = cb;
                break;
        case USART_2_UDRE_CB:
                USART_2_udre_isr_cb = cb;
                break;
        default:
                // do nothing
                break;
    }
}

/* Interrupt service routine for RX complete */
ISR(USART2_RXC_vect)
{
    if (USART_2_rx_isr_cb != NULL)
    {
        (*USART_2_rx_isr_cb)();
    }
}

/* Interrupt service routine for Data Register Empty */
ISR(USART2_DRE_vect)
{
    if (USART_2_udre_isr_cb != NULL)
    {
        (*USART_2_udre_isr_cb)();
    }
}

ISR(USART2_TXC_vect)
{
	
}

bool USART_2_is_tx_ready()
{
    return (USART_2_tx_elements != USART_2_TX_BUFFER_SIZE);
}

bool USART_2_is_rx_ready()
{
    return (USART_2_rx_elements != 0);
}

bool USART_2_is_tx_busy()
{
    return (!(USART2.STATUS & USART_TXCIF_bm));
}

uint8_t USART_2_read(void)
{
    uint8_t tmptail;

    /* Wait for incoming data */
    while (USART_2_rx_elements == 0)
            ;
    /* Calculate buffer index */
    tmptail = (USART_2_rx_tail + 1) & USART_2_RX_BUFFER_MASK;
    /* Store new index */
    USART_2_rx_tail = tmptail;
    ENTER_CRITICAL(R);
    USART_2_rx_elements--;
    EXIT_CRITICAL(R);

    /* Return data */
    return USART_2_rxbuf[tmptail];
}

void USART_2_write(const uint8_t data)
{
    uint8_t tmphead;

    /* Calculate buffer index */
    tmphead = (USART_2_tx_head + 1) & USART_2_TX_BUFFER_MASK;
    /* Wait for free space in buffer */
    while (USART_2_tx_elements == USART_2_TX_BUFFER_SIZE)
            ;
    /* Store data in buffer */
    USART_2_txbuf[tmphead] = data;
    /* Store new index */
    USART_2_tx_head = tmphead;
    ENTER_CRITICAL(W);
    USART_2_tx_elements++;
    EXIT_CRITICAL(W);
    /* Enable UDRE interrupt */
    USART2.CTRLA |= (1 << USART_DREIE_bp);
}

int8_t USART_2_init()
{
    USART2.BAUD = (uint16_t)USART2_BAUD_RATE(9600); /* set baud rate register */

    // 	USART2.CTRLA = 0 << USART_ABEIE_bp /* Auto-baud Error Interrupt Enable: disabled */
    // 				| 0 << USART_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
    // 				| 0 << USART_LBME_bp /* Loop-back Mode Enable: disabled */
    // 				| USART_RS485_OFF_gc /* RS485 Mode internal transmitter: RS485 Mode disabled */
    // 				| 0 << USART_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
    // 				| 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */
    // 				| 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

    	USART2.CTRLB = 0 << USART_MPCM_bp /* Multi-processor Communication Mode: disabled */
    				| 0 << USART_ODME_bp /* Open Drain Mode Enable: disabled */
    				| 1 << USART_RXEN_bp /* Reciever enable: enabled */
    				| USART_RXMODE_NORMAL_gc /* Receiver Mode: Normal mode */
    				| 0 << USART_SFDEN_bp /* Start Frame Detection Enable: disabled */
    				| 1 << USART_TXEN_bp; /* Transmitter Enable: enabled */

    	USART2.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Communication Mode: Asynchronous Mode */
    				| 0 << USART_UCPHA_bp /* SPI Master Mode, Clock Phase: disabled */
    				| 0 << USART_UDORD_bp /* SPI Master Mode, Data Order: disabled */
    				| USART_CHSIZE_8BIT_gc /* Character Size: Character size: 8 bit */
    				| USART_PMODE_DISABLED_gc /* Parity Mode: No Parity */
    				| USART_SBMODE_1BIT_gc; /* Stop Bit Mode: 1 stop bit */

    // 	USART2.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

    // 	USART2.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

    // 	USART2.RXPLCTRL = 0 << USART_RXPL_gp; /* Receiver Pulse Lenght: 0 */

    // 	USART2.TXPLCTRL = 0 << USART_TXPL_gp; /* Transmit pulse length: 0 */

    uint8_t x;

    /* Initialize ringbuffers */
    x = 0;

    USART_2_rx_tail     = x;
    USART_2_rx_head     = x;
    USART_2_rx_elements = x;
    USART_2_tx_tail     = x;
    USART_2_tx_head     = x;
    USART_2_tx_elements = x;

#if defined(__GNUC__)
    stdout = &USART_2_stream;
#endif

    return 0;
}

void USART_2_enable()
{
    USART2.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

void USART_2_enable_rx()
{
    USART2.CTRLB |= USART_RXEN_bm;
}

void USART_2_enable_tx()
{
    USART2.CTRLB |= USART_TXEN_bm;
}

void USART_2_disable()
{
    USART2.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

uint8_t USART_2_get_data()
{
    return USART2.RXDATAL;
}
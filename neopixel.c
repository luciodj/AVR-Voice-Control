/*
\file   neopixel.c

\brief  A NeoPixel RGB LED driver sharing the SPI port (@10MHz)

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
#include <string.h>
#include <stdlib.h>
#include "mcc_generated_files/include/spi0.h"
#include "mcc_generated_files/include/port.h"
#include "mcc_generated_files/utils/interrupt_avr8.h"
#include "mcc_generated_files/config/clock_config.h"
#include "util/delay.h"
#include "mcc_generated_files/include/port.h"
#include "neopixel.h"

#define V       64
#define RED     V, 0, 0
#define BLUE    0, 0, V
#define GREEN   0, V, 0
#define YELLOW  V, V, 0

#define SMOOTH  5  //ms
#define ROWS    4
#define LEDxROW 4
#define STEPS   100

// NOTE: since the SPI bus is shared between the WINC and the NeoPixel driver
//       the WINC communication must be slowed down in order to avoid commands
//       being intercepted by the RGB LED panel. A 50us delay has been inserted
//       in nm_bus_wrapper_mega.c in function spi_rw()

// display matrix
uint8_t array[ROWS * LEDxROW * 3];
int16_t threshold[ROWS];

#ifdef __WS2811__
    #define WS_1    0x0F // 1.2us
    #define WS_0    0x3F // 0.6us
#else
    #define WS_1    0xF0 // 0.8us
    #define WS_0    0xFC // 0.4us
#endif

void SendByte(uint8_t b){
    uint8_t i;
    for(i=0; i<8; i++){
        if (b & 0x80) SPI_0_exchange_byte(WS_1);
        else SPI_0_exchange_byte(WS_0);
        b <<= 1;
    }
}

void SendPixel(uint8_t r, uint8_t g, uint8_t b){
    SendByte(g);
    SendByte(r);
    SendByte(b);
}

void NEO_fill(uint8_t r, uint8_t g, uint8_t b){
    uint8_t i;
    uint8_t *pi = array;
    for(i=0; i<ROWS*LEDxROW; i++){
        *pi++ = g;
        *pi++ = r;
        *pi++ = b;
    }
}

void NEO_display(void){
    uint8_t i;
    uint8_t *pi = array;

    cpu_irq_disable();
    PORTA_pin_set_inverted(4, 1);
    PORTC_set_pin_level(0, 1);    // enable NeoPixel output
    _delay_ms(1);
    for(i=0; i<ROWS*LEDxROW; i++){
        SendByte(*pi++);
        SendByte(*pi++);
        SendByte(*pi++);
    }
    _delay_ms(1);
    PORTA_pin_set_inverted(4, 0);
    PORTC_set_pin_level(0, 0);    // disable Neopixel output
    cpu_irq_enable();
}

void NEO_migrate(uint8_t r, uint8_t g, uint8_t b){
    float fcr = array[1], fcg = array[0], fcb = array[2];
    float fr = r-fcr, fg = g-fcg, fb = b-fcb;

    uint16_t x;
    uint8_t row, p;
    // compute thresholds
    for(x=0; x<ROWS; x++){
        threshold[x] = -STEPS / ROWS * x;
    }
    // migration loop
    for(x=0; x<STEPS*2; x++){
        // update thresholds
        for(row=0; row<ROWS; row++){
            threshold[row]++;
            // update matrix by rows
            if ((threshold[row] > 0) && (threshold[row] <= STEPS)){
                // update row
                float ratio = threshold[row] * 1.0 / STEPS;
                uint8_t *pi = &array[row * LEDxROW * 3];
                for(p=0; p<LEDxROW; p++){
                    *pi++ = floor(fcg+ fg * ratio);
                    *pi++ = floor(fcr+ fr * ratio);
                    *pi++ = floor(fcb+ fb * ratio);
                }
            }
        }

        NEO_display();
        _delay_ms(SMOOTH);
    }
}

void NEO_init(void){
    // configure pin PC3-CS as CCL1 output to NeoPixel
    PORTC_set_pin_dir(3, PORT_DIR_OUT);
    // configure pin PC0-TX as selector CCL1-IN0
    PORTC_set_pin_level(0, 0);
    PORTC_set_pin_dir(0, 1);

    //    PORTC_set_pin_dir(0, PORT_DIR_OUT);
    // NC   MOSI  TX    bit   OUT
    // --------------------------
    //  0    0    0     0     1
    //  0    0    1     1     1
    //  0    1    0     2     1
    //  0    1    1     3     0
    //  1    0    0     4     1
    //  1    0    1     5     1
    //  1    1    0     6     1
    //  1    1    1     7     0
    CCL.TRUTH1 = 0x77;
    // configure CCL1 to AND MOSI with a selector pin PC0-TX and output on PC3-CS
    CCL.LUT1CTRLB = (0x9 << CCL_INSEL1_gp) |  0x5; // IN1 = MOSI, IN0 = CCL1_IN0
    CCL.LUT1CTRLA = CCL_ENABLE_bm | CCL_OUTEN_bm;
    CCL.CTRLA = CCL_ENABLE_bm;

    NEO_migrate(GREEN);
    NEO_migrate(0,0,0);
}

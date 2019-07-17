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

#include "../include/adc0.h"

adc_irq_cb_t ADC_0_cb = NULL; /**
* \brief Initialize ADC interface
*/
int8_t ADC_0_init()
{

		ADC0.CALIB = ADC_DUTYCYC_DUTY25_gc; /* Duty Cycle: 25% Duty cycle */

	// 	ADC0.CTRLB = ADC_SAMPNUM_ACC1_gc; /* Accumulation Samples: 1 ADC sample */

		ADC0.CTRLC = ADC_PRESC_DIV16_gc /* Clock Pre-scaler: CLK_PER divided by 16 */
					| ADC_REFSEL_VDDREF_gc /* Reference Selection: VDD */
					| 0 << ADC_SAMPCAP_bp; /* Sample Capacitance Selection: disabled */

	// 	ADC0.CTRLD = ADC_ASDV_ASVOFF_gc /* Automatic Sampling Delay Variation: The Automatic Sampling Delay Variation is disabled */
	// 				| ADC_INITDLY_DLY0_gc /* Initial Delay Selection: Delay 0 CLK_ADC cycles */
	// 				| 0 << ADC_SAMPDLY_gp; /* Sampling Delay Selection: 0 */

	// 	ADC0.CTRLE = ADC_WINCM_NONE_gc; /* Window Comparator Mode: No Window Comparison */

	// 	ADC0.DBGCTRL = 0 << ADC_DBGRUN_bp; /* Debug run: disabled */

	// 	ADC0.EVCTRL = 0 << ADC_STARTEI_bp; /* Start Event Input Enable: disabled */

	// 	ADC0.INTCTRL = 0 << ADC_RESRDY_bp /* Result Ready Interrupt Enable: disabled */
	// 				| 0 << ADC_WCMP_bp; /* Window Comparator Interrupt Enable: disabled */

		ADC0.MUXPOS = ADC_MUXPOS_AIN5_gc; /* Analog Channel Selection Bits: ADC input pin 5 */

	// 	ADC0.SAMPCTRL = 0 << ADC_SAMPLEN_gp; /* Sample lenght: 0 */

	// 	ADC0.WINHT = 0; /* Window comparator high threshold */

	// 	ADC0.WINLT = 0; /* Window comparator low threshold */

		ADC0.CTRLA = 1 << ADC_ENABLE_bp /* ADC Enable: enabled */
					| 0 << ADC_FREERUN_bp /* ADC Freerun mode: disabled */
					| ADC_RESSEL_10BIT_gc /* ADC Resolution: 10-bit mode */
					| 0 << ADC_RUNSTBY_bp; /* Run standby mode: disabled */

	return 0;
}

void ADC_0_enable()
{
	ADC0.CTRLA |= ADC_ENABLE_bm;
}

void ADC_0_disable()
{
	ADC0.CTRLA &= ~ADC_ENABLE_bm;
}

void ADC_0_start_conversion(adc_0_channel_t channel)
{
	ADC0.MUXPOS  = channel;
	ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC_0_is_conversion_done()
{
	return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}

adc_result_t ADC_0_get_conversion_result(void)
{
	return (ADC0.RES);
}

adc_result_t ADC_0_get_conversion(adc_0_channel_t channel)
{
	adc_result_t res;

	ADC_0_start_conversion(channel);
	while (!ADC_0_is_conversion_done());
	res = ADC_0_get_conversion_result();
	ADC0.INTFLAGS |= ADC_RESRDY_bm;
	return res;
}

uint8_t ADC_0_get_resolution()
{
	return (ADC0.CTRLA & ADC_RESSEL_bm) ? 8 : 10;
}

void ADC_0_register_callback(adc_irq_cb_t f)
{
	ADC_0_cb = f;
}

ISR(ADC0_RESRDY_vect)
{
	// Clear the interrupt flag
	ADC0.INTFLAGS |= ADC_RESRDY_bm;

	if (ADC_0_cb != NULL) {
		ADC_0_cb();
	}
}

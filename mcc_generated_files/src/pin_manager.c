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

#include "../include/pin_manager.h"

void PIN_MANAGER_Initialize()
{
	PD5_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	PD5_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PD5_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PD5_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PD5_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INPUT_DISABLE_gc);

	PA2_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	PA2_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PA2_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PA2_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PA2_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	CONF_WIFI_M2M_RESET_PIN_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	CONF_WIFI_M2M_RESET_PIN_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	CONF_WIFI_M2M_RESET_PIN_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	CONF_WIFI_M2M_RESET_PIN_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	CONF_WIFI_M2M_RESET_PIN_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	SW0_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	SW0_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	SW0_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_UP);

	SW0_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	SW0_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PA4_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	PA4_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PA4_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PA4_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PA4_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PA3_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	PA3_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PA3_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PA3_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PA3_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PA6_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	PA6_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PA6_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PA6_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PA6_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PA5_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	PA5_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PA5_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PA5_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PA5_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PA7_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	PA7_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PA7_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PA7_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PA7_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PF1_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	PF1_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PF1_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PF1_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PF1_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	PF0_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	PF0_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	PF0_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	PF0_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	PF0_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	LED_YELLOW_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	LED_YELLOW_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	LED_YELLOW_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	LED_YELLOW_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	LED_YELLOW_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	CONF_WIFI_M2M_INT_PIN_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	CONF_WIFI_M2M_INT_PIN_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	CONF_WIFI_M2M_INT_PIN_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	CONF_WIFI_M2M_INT_PIN_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	CONF_WIFI_M2M_INT_PIN_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	LED_RED_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	LED_RED_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	LED_RED_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	LED_RED_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	LED_RED_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	LED_BLUE_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	LED_BLUE_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	LED_BLUE_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	LED_BLUE_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	LED_BLUE_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	SW1_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	SW1_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	SW1_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_UP);

	SW1_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	SW1_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	CONF_WIFI_M2M_WAKE_PIN_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	CONF_WIFI_M2M_WAKE_PIN_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	CONF_WIFI_M2M_WAKE_PIN_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	CONF_WIFI_M2M_WAKE_PIN_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	CONF_WIFI_M2M_WAKE_PIN_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

	LED_GREEN_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_OUT);

	LED_GREEN_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	LED_GREEN_set_pull_mode(
	    // <y> Pull configuration
	    // <id> pad_pull_config
	    // <PORT_PULL_OFF"> Off
	    // <PORT_PULL_UP"> Pull-up
	    PORT_PULL_OFF);

	LED_GREEN_set_inverted(
	    // <y> Invert I/O on pin
	    // <id> pad_invert
	    // <false"> Not inverted
	    // <true"> Inverted
	    false);

	LED_GREEN_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_INTDISABLE_gc"> Interrupt disabled but input buffer enabled
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_INTDISABLE_gc);

    /* PORTMUX Initialization */
    PORTMUX.USARTROUTEA = 0x00;
    PORTMUX.EVSYSROUTEA = 0x00;
    PORTMUX.TCAROUTEA = 0x00;
    PORTMUX.CCLROUTEA = 0x00;
    PORTMUX.TCBROUTEA = 0x00;
    PORTMUX.TWISPIROUTEA = 0x00;
        
}


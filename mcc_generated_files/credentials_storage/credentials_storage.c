/*
    \file   credentials_storage.c

    \brief  Credential Storage source file.

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

#include <stdint.h>
#include <string.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include "credentials_storage.h"
#include "../debug_print.h"

#define EEPROM_SSID  0
#define EEPROM_PSW   EEPROM_SSID + MAX_WIFI_CREDENTIALS_LENGTH
#define EEPROM_SEC   EEPROM_PSW + MAX_WIFI_CREDENTIALS_LENGTH
#define EEPROM_DBG   EEPROM_SEC + 1

char ssid[MAX_WIFI_CREDENTIALS_LENGTH];
char pass[MAX_WIFI_CREDENTIALS_LENGTH];
char authType[2];

uint8_t CREDENTIALS_STORAGE_getDebugSeverity(void)
{
   return eeprom_read_byte((uint8_t *)EEPROM_DBG);
}

void CREDENTIALS_STORAGE_setDebugSeverity(uint8_t s)
{
   eeprom_write_byte((uint8_t *)EEPROM_DBG,s);
}

/**
 * \brief Read WiFi SSID and password from EEPROM
 *
 * \param ssidbuf     buffer for SSID
 * \param passwordbuf buffer for password
 * \param sec         pointer to security type
 */
void CREDENTIALS_STORAGE_read(char *ssidbuf, char *passwordbuf, char *sec)
{
    uint8_t i = MAX_WIFI_CREDENTIALS_LENGTH;
    uint8_t *addr = EEPROM_SSID;
    while (i--)
    {
        *ssidbuf++ = eeprom_read_byte((uint8_t *)addr++);
    }
    i = MAX_WIFI_CREDENTIALS_LENGTH;
    addr = (uint8_t *)EEPROM_PSW;
    while (i--)
    {
        *passwordbuf++ = eeprom_read_byte((uint8_t *)addr++);
    }
    *sec = eeprom_read_byte((uint8_t *)EEPROM_SEC);
}

/**
 * \brief Store WiFi SSID and password to EEPROM
 *
 * \param ssidbuf     buffer with SSID
 * \param passwordbuf buffer with password
 * \param sec         pointer to security type
 */
void CREDENTIALS_STORAGE_save(char *ssidbuf, char *passwordbuf, char *sec)
{
    uint8_t i = MAX_WIFI_CREDENTIALS_LENGTH;
    uint8_t *addr = EEPROM_SSID;

    i = strlen(ssidbuf) + 1;

    while (i--)
    {
        eeprom_write_byte((uint8_t *)addr++, *ssidbuf++);
    }

    i = MAX_WIFI_CREDENTIALS_LENGTH;
    addr = (uint8_t *)EEPROM_PSW;

    i = strlen(passwordbuf) + 1;
    while (i--)
    {
        eeprom_write_byte((uint8_t *)addr++, (uint8_t)*passwordbuf++);
    }

    eeprom_write_byte((uint8_t *)EEPROM_SEC, (uint8_t)*sec);
}


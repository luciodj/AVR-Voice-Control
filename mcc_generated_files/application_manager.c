/*
\file   application_manager.c

\brief  Application Manager source file.

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
#include <time.h>
#include <stdio.h>
#include <avr/wdt.h>

#include "utils/atomic.h"
#include "application_manager.h"
#include "mcc.h"
#include "include/pin_manager.h"
#include "config/IoT_Sensor_Node_config.h"
#include "config/conf_winc.h"
#include "cloud/cloud_service.h"
#include "cloud/wifi_service.h"
#include "cloud/crypto_client/cryptoauthlib_main.h"
#include "cloud/crypto_client/crypto_client.h"
#include "cli/cli.h"
#include "credentials_storage/credentials_storage.h"
#include "led.h"
#include "debug_print.h"

#define MAIN_DATATASK_INTERVAL 100

// This will contain the device ID, before we have it this dummy value is the init value which is non-0
char attDeviceID[20] = "BAAAAADD1DBAAADD1D";
struct shared_networking_params shared_networking_params;
ATCA_STATUS retValCryptoClientSerialNumber;

absolutetime_t MAIN_dataTask(void *payload);
timer_struct_t MAIN_dataTasksTimer = {MAIN_dataTask};

void  wifiConnectionStateChanged(uint8_t status);

void application_init(){
   wdt_disable();

   // Initialization of modules before interrupts are enabled
   SYSTEM_Initialize();

   LED_test();

   CLI_init();
   CLI_setdeviceId(attDeviceID);
   debug_init(attDeviceID);
   // Default not to EEPROM value but to NONE
   // debug_setSeverity(CREDENTIALS_STORAGE_getDebugSeverity());
   // debug_setSeverity(SEVERITY_DEBUG); // Use this to start up in debug mode always, TODO: Do this via define we can pass in without modifying the code!

   ENABLE_INTERRUPTS();

   // Initialization of modules where the init needs interrupts to be enabled
   cryptoauthlib_init();

   if (cryptoDeviceInitialized == false)
   {
      debug_printError("APP: CryptoAuthInit failed");
   }
   CREDENTIALS_STORAGE_read(ssid, pass, authType);

   // If the EEPROM is blank we use the default credentials
   if (ssid[0] ==  0xFF)
   {
      strcpy(ssid, CFG_MAIN_WLAN_SSID);
      strcpy(pass, CFG_MAIN_WLAN_PSK);
      itoa(CFG_MAIN_WLAN_AUTH, (char*)authType, 10);
   }

   // Get serial number from the ECC608 chip
   retValCryptoClientSerialNumber = CRYPTO_CLIENT_printSerialNumber(attDeviceID);
   if( retValCryptoClientSerialNumber != ATCA_SUCCESS )
   {
      switch(retValCryptoClientSerialNumber)
      {
         case ATCA_GEN_FAIL:
             debug_printError("APP: DeviceID generation failed, unspecified error");
         break;
         case ATCA_BAD_PARAM:
             debug_printError("APP: DeviceID generation failed, bad argument");
         default:
             debug_printError("APP: DeviceID generation failed");
         break;
      }

   }
   CLI_setdeviceId(attDeviceID);
   debug_setPrefix(attDeviceID);

   uint8_t mode = SW0_get_level();

   wifi_init(wifiConnectionStateChanged, mode);

   if (mode >0) {
      CLOUD_init(attDeviceID);
      scheduler_timeout_create(&MAIN_dataTasksTimer, MAIN_DATATASK_INTERVAL);
   }

   LED_test();
}


// React to the WIFI state change here. Status of 1 means connected, Status of 0 means disconnected
void  wifiConnectionStateChanged(uint8_t status)
{
   // If we have no AP access we want to retry
   if (status != 1)
   {
      // Restart the WIFI module if we get disconnected from the WiFi Access Point (AP)
      CLOUD_reset();
   }
}


// This scheduler will check all tasks and timers that are due and service them
void runScheduler(void)
{
    scheduler_timeout_call_next_callback();
}


// This could be better done with a function pointer (DI) but in the interest of simplicity
//     we avoided that. This is being called from MAIN_dataTask below
void sendToCloud(void);

// This gets called by the scheduler approximately every 100ms
absolutetime_t MAIN_dataTask(void *payload)
{
   static time_t previousTransmissionTime = 0;

   // Get the current time. This uses the C standard library time functions
   time_t timeNow = time(NULL);

   // Example of how to send data when MQTT is connected every 1 second based on the system clock
   if (CLOUD_isConnected())
   {
      // How many seconds since the last time this loop ran?
      int32_t delta = difftime(timeNow,previousTransmissionTime);

      if (delta >= CFG_SEND_INTERVAL)
      {
         previousTransmissionTime = timeNow;

         // Call the data task in main.c
         sendToCloud();
      }
   }

   // Example of how to read the SW0 and SW1 buttons
   // Check if the disconnect button was pressed
   if (SW0_get_level() == 0)
   {
      CLOUD_disconnect();
   }

   if (SW1_get_level() == 0)
   {
      shared_networking_params.haveAPConnection = 1;
   }


   LED_BLUE_set_level(!shared_networking_params.haveAPConnection);
//   LED_RED_set_level(!shared_networking_params.haveERROR);
   LED_GREEN_set_level(!CLOUD_isConnected());


   // This is milliseconds managed by the RTC and the scheduler, this return makes the
   //      timer run another time, returning 0 will make it stop
   return MAIN_DATATASK_INTERVAL;
}

/*
\file   wifi_service.c

\brief  Wifi service source file.

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

#include <time.h>
#include <string.h>
#include <avr/wdt.h>

#include "wifi_service.h"
#include "../winc/driver/include/m2m_wifi.h"
#include "../include/rtc.h"
#include "../application_manager.h"
#include "../debug_print.h"
#include "../config/IoT_Sensor_Node_config.h"
#include "../config/conf_winc.h"
#include "../config/mqtt_config.h"
#include "../winc/socket/include/socket.h"
#include "../credentials_storage/credentials_storage.h"

#define CLOUD_WIFI_TASK_INTERVAL    50L
#define CLOUD_NTP_TASK_INTERVAL     500L

// Scheduler
absolutetime_t ntpTimeFetchTask(void *payload);
absolutetime_t wifiHandlerTask(void * param);

timer_struct_t ntpTimeFetchTimer  = {ntpTimeFetchTask};
timer_struct_t wifiHandlerTimer  = {wifiHandlerTask};
	
absolutetime_t checkBackTask(void * param);
timer_struct_t checkBackTimer  = {checkBackTask};	
       
void (*callback_funcPtr)(uint8_t);
       
void enable_provision_ap(void);
       
// Callback function pointer for indicating status updates upwards
void  (*wifiConnectionStateChangedCallback)(uint8_t  status) = NULL;

// Function to be called by WifiModule on status updates from below
static void wifiCallback(uint8_t msgType, void *pMsg);

// This is a workaround to wifi_deinit being broken in the winc, so we can de-init without hanging up
int8_t hif_deinit(void * arg);


void wifi_reinit()
{
     tstrWifiInitParam param ;
     
     /* Initialize Wi-Fi parameters structure. */
     memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam));
     
     param.pfAppWifiCb = wifiCallback;
     socketDeinit();
     hif_deinit(NULL);
     nm_bsp_deinit();

     wifiConnectionStateChangedCallback = callback_funcPtr;

     nm_bsp_init();
     m2m_wifi_init(&param);
     socketInit();
}

// funcPtr passed in here will be called indicating AP state changes with the following values
// Wi-Fi state is disconnected   == 0
// Wi-Fi state is connected      == 1
// Wi-Fi state is undefined      == 0xff
void wifi_init(void (*funcPtr)(uint8_t), uint8_t mode) {
    callback_funcPtr = funcPtr;
    
    // This uses the global ptr set above
    wifi_reinit();

   // Mode == 0 means AP configuration mode
   if(mode == 0)
   {
      enable_provision_ap();
      debug_printInfo("ACCESS POINT MODE for provisioning");
   }
   else
   {
      scheduler_timeout_create(&ntpTimeFetchTimer,CLOUD_NTP_TASK_INTERVAL);
   }
   
   
   scheduler_timeout_create(&wifiHandlerTimer, CLOUD_WIFI_TASK_INTERVAL);
}




// Update the system time every CLOUD_NTP_TASK_INTERVAL milliseconds
absolutetime_t ntpTimeFetchTask(void *payload)
{
    m2m_wifi_get_sytem_time();
    return CLOUD_NTP_TASK_INTERVAL;
}


absolutetime_t wifiHandlerTask(void * param)
{
   m2m_wifi_handle_events(NULL);
   return CLOUD_WIFI_TASK_INTERVAL;
}

absolutetime_t checkBackTask(void * param)
{
	debug_printError("wifi_cb: M2M_WIFI_RESP_CON_STATE_CHANGED: DISCONNECTED");
	shared_networking_params.haveAPConnection = 0;
	shared_networking_params.haveERROR = 1;
	shared_networking_params.amDisconnecting = false;
	return 0;
}


static void wifiCallback(uint8_t msgType, void *pMsg)
{
    switch (msgType) {
        case M2M_WIFI_RESP_CON_STATE_CHANGED:
        {
            tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pMsg;
            if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) 
            {
                debug_printGOOD("wifi_cb: M2M_WIFI_RESP_CON_STATE_CHANGED: CONNECTED");
                // We need more than AP to have an APConnection, we also need a DHCP IP address!
            } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) 
			{
                scheduler_timeout_create(&checkBackTimer,CLOUD_WIFI_TASK_INTERVAL);
				shared_networking_params.amDisconnecting = true;
            }
            
            if ((wifiConnectionStateChangedCallback != NULL) && (!shared_networking_params.amDisconnecting))
            {
                wifiConnectionStateChangedCallback(pstrWifiState->u8CurrState);
            }            
            break;
        }
        
        case M2M_WIFI_REQ_DHCP_CONF:
        {
            // Now we are really connected, we have AP and we have DHCP, start off the MQTT host lookup now, response in dnsHandler
            if (gethostbyname((uint8_t*)CFG_MQTT_HOST) == M2M_SUCCESS)
            {
				if (shared_networking_params.amDisconnecting)
				{
					scheduler_timeout_delete(&checkBackTimer);
					shared_networking_params.amDisconnecting = false;
				}
				shared_networking_params.haveAPConnection = 1;
				shared_networking_params.haveERROR = 0;
                debug_printGOOD("CLOUD: DHCP CONF");
            }
            break;
        }

        case M2M_WIFI_RESP_GET_SYS_TIME:
        {
            tstrSystemTime* WINCTime = (tstrSystemTime*)pMsg;
            struct tm theTime;

            // Convert to UNIX_EPOCH, this mktime uses years since 1900 and months are 0 based so we
            //    are doing a couple of adjustments here.
            if(WINCTime->u16Year)
            {
                theTime.tm_hour = WINCTime->u8Hour;
                theTime.tm_min = WINCTime->u8Minute;
                theTime.tm_sec = WINCTime->u8Second;
                theTime.tm_year = WINCTime->u16Year-1900;
                theTime.tm_mon = WINCTime->u8Month-1;
                theTime.tm_mday = WINCTime->u8Day;
                theTime.tm_isdst = 0;

                set_system_time(mktime(&theTime));
            }
            break;
        }
       
        
         case M2M_WIFI_RESP_PROVISION_INFO:
         {
            tstrM2MProvisionInfo *pstrProvInfo = (tstrM2MProvisionInfo*)pMsg;
            if(pstrProvInfo->u8Status == M2M_SUCCESS)
            {
               char authType[10];
               itoa(pstrProvInfo->u8SecType,authType,10);
               debug_printInfo("%s",pstrProvInfo->au8SSID);
               CREDENTIALS_STORAGE_save((char *)pstrProvInfo->au8SSID, (char *)pstrProvInfo->au8Password, authType);
               wdt_enable(WDTO_30MS);
               while(1) {};
            }
            break;
         }

        

        default:
        {
            break;
        }
    }
}


void enable_provision_ap(void)
{
   tstrM2MAPConfig apConfig = {
      CFG_MAIN_WLAN_SSID, // Access Point Name.
      1, // Channel to use.
      0, // Wep key index.
      WEP_40_KEY_STRING_SIZE, // Wep key size.
      "1234567890", // Wep key.
      M2M_WIFI_SEC_OPEN, // Security mode.
      SSID_MODE_VISIBLE, // SSID visible.
      CFG_WLAN_AP_IP_ADDRESS
   };
   static char gacHttpProvDomainName[] = CFG_WLAN_AP_NAME;
   m2m_wifi_start_provision_mode(&apConfig, gacHttpProvDomainName, 1);
}



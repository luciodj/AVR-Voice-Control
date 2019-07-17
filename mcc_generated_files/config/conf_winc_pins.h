#ifndef CONF_WIFI_M2M_PINS_H_INCLUDED
#define CONF_WIFI_M2M_PINS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/** Module Pin Settings */
#define CONF_WIFI_M2M_SPI_CS_PIN_set_dir            PA7_set_dir
#define CONF_WIFI_M2M_SPI_CS_PIN_set_level          PA7_set_level
#define CONF_WIFI_M2M_SPI_CS_PIN_set_isc            PA7_set_isc
#define CONF_WIFI_M2M_SPI_CS_PIN_set_pull_mode      PA7_set_pull_mode
#define CONF_WIFI_M2M_SPI_CS_PIN_set_inverted       PA7_set_inverted
#define CONF_WIFI_M2M_SPI_CS_PIN_get_level          PA7_get_level
#define CONF_WIFI_M2M_SPI_CS_PIN_toggle_level       PA7_toggle_level
    
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_dir          PA4_set_dir
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_level        PA4_set_level
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_isc          PA4_set_isc
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_pull_mode    PA4_set_pull_mode
#define CONF_WIFI_M2M_SPI_MOSI_PIN_set_inverted     PA4_set_inverted
#define CONF_WIFI_M2M_SPI_MOSI_PIN_get_level        PA4_get_level
#define CONF_WIFI_M2M_SPI_MOSI_PIN_toggle_level     PA4_toggle_level
    
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_dir          PA5_set_dir
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_level        PA5_set_level
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_isc          PA5_set_isc
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_pull_mode    PA5_set_pull_mode
#define CONF_WIFI_M2M_SPI_MISO_PIN_set_inverted     PA5_set_inverted
#define CONF_WIFI_M2M_SPI_MISO_PIN_get_level        PA5_get_level
#define CONF_WIFI_M2M_SPI_MISO_PIN_toggle_level     PA5_toggle_level
    
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_dir           PA6_set_dir
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_level         PA6_set_level
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_isc           PA6_set_isc
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_pull_mode     PA6_set_pull_mode
#define CONF_WIFI_M2M_SPI_SCK_PIN_set_inverted      PA6_set_inverted
#define CONF_WIFI_M2M_SPI_SCK_PIN_get_level         PA6_get_level
#define CONF_WIFI_M2M_SPI_SCK_PIN_toggle_level      PA6_toggle_level

#ifdef __cplusplus
}
#endif

#endif /* CONF_WIFI_M2M_PINS_H_INCLUDED */

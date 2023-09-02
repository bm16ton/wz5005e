#ifndef __SETTINGS__
#define __SETTINGS__

#define htons(x) ( ((x)<< 8 & 0xFF00) | ((x)>> 8 & 0x00FF) )
#define MIN_VOLTAGE 0
#define MAX_VOLTAGE 5000
#define MIN_CURRENT 0
#define MAX_CURRENT 4999

#define MDSN_NAME "wz5005"

#define WIFI_SSID "maddocks"
#define WIFI_PASSWORD "maddocks"

#endif

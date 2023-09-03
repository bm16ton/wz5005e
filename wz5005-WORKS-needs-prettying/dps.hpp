
#ifndef __DPS__
#define __DPS__

#include <stdint.h>

#define MIN_VOLTAGE 0
#define MAX_VOLTAGE 5000
#define MIN_CURRENT 0
#define MAX_CURRENT 4999

#define BADTXCHKSM  0x90            // 144 bad checksum sent
#define BADCMNDOROVRFLW 0xA0        // 160 bad command parameter or overflow
#define BADCMNDCNTEXEC  0xB0        // 176 command cannot be executed
#define INVALIDCMD    0xC0          // 192 invalid command
#define UNKNOWNCMD    0xD0          // 208 unknown command
#define ELSEHRM       0x80          // 128 if it aint one of the ones listed above then it gets this

#define htons2(x) ( ((x)<< 8 & 0xFF00) | ((x)>> 8 & 0x00FF) )

extern volatile int doupdateon;
extern volatile int doupdateout;

struct dps_status {
  uint16_t uset;
  uint16_t iset;
  uint16_t uout;
  uint16_t iout;
  uint16_t temp;
  uint16_t uin;
  uint16_t lock;
  uint16_t protect;
  uint16_t cvcc;
  uint16_t onoff;
  uint16_t offon;
};

bool dps_read_status(dps_status *dest);
bool dps_set_voltage(const uint16_t voltage);
bool dps_set_current(const uint16_t current);
bool dps_set_voltage_current(const uint16_t voltage, const uint16_t current);

#endif

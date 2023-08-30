#include "modbus.hpp"
#include "settings.h"
#include <stdint.h>
#include "Arduino.h"

uint8_t modbus_crc16(uint8_t *buf, int len)
{
  uint8_t crc = 0x0000;
  for (int pos = 0; pos < len; pos++)
  {
crc = (crc + buf[pos]);
    /*
  crc ^= (uint16_t)buf[pos];    // XOR byte into least sig. byte of crc

  for (int i = 8; i != 0; i--) {    // Loop over each bit
    if ((crc & 0x0001) != 0) {      // If the LSB is set
      crc >>= 1;                    // Shift right and XOR 0xA001
      crc ^= 0xA001;
    }
    else                            // Else LSB is not set
      crc >>= 1;                    // Just shift right
    }
    */
  }
  crc = (crc / 0x100);
  return crc;
}

bool modbus_read_regs(uint16_t reg_addr, uint8_t n, void *dest) {
  uint8_t request[] = {0xAA, 0x01, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD6};

//  uint16_t crc16 = modbus_crc16(request, 6);
//  request[7] = crc16 & 0xff;
//  request[8] = crc16 >> 8;

  while (Serial.available()) Serial.read(); // read buffer to devnull
  Serial.write(request, 20);

  uint8_t response[20];
  memset(response, 0, 20);
  int timer = 0;
  int cntr = 0;
  while (timer < MODBUS_TIMEOUT_MS) {
    if (Serial.available()) {
      response[cntr++] = Serial.read();
      if (cntr >= 20) {
        memcpy(dest, response, 20);
        return true;
      }
    } else {
      timer += 10;
      delay(10);
    }
  }
  return false;
}

bool modbus_write_reg(uint16_t reg_addr, const uint16_t value) {
  uint8_t request[] = {0xAA, SLAVE_UNIT_ID, WRITE_SINGLE_REG, reg_addr >> 8, reg_addr & 0xff, value >> 8, value & 0xff, 0x00, 0x00};
  uint16_t crc16 = modbus_crc16(request, 6);
  request[7] = crc16 & 0xff;
  request[8] = crc16 >> 8;

  while (Serial.available()) Serial.read(); // read buffer to devnull
  Serial.write(request, 9);

  int timer = 0;
  int cntr = 0;
  while (timer < MODBUS_TIMEOUT_MS) {
    if (Serial.available()) {
      Serial.read(); // read to devnull
      if (++cntr >= 8) {
        return true;
      }
    } else {
      timer += 10;
      delay(10);
    }
  }
  return false;
}

bool modbus_write_regs(uint16_t start_reg_addr, uint8_t n, const uint16_t values[]) {
  uint8_t request[9+2*n];
  const uint8_t header[] = {0xAA, SLAVE_UNIT_ID, WRITE_MULTIPLE_REGS, start_reg_addr >> 8, start_reg_addr & 0xff, 0x00, n, 2*n};
  memcpy(request, header, 7);
  for (int i=0; i<n; ++i) {
    request[8+2*i] = values[i] >> 8;
    request[8+2*i+1] = values[i] & 0xff;
  }
  uint16_t crc16 = modbus_crc16(request, 8+2*n);
  request[8+2*n] = crc16 & 0xff;
  request[8+2*n+1] = crc16 >> 8;

  while (Serial.available()) Serial.read(); // read buffer to devnull
  Serial.write(request, 10+2*n);

  int timer = 0;
  int cntr = 0;
  while (timer < MODBUS_TIMEOUT_MS) {
    if (Serial.available()) {
      Serial.read(); // read to devnull
      if (++cntr >= 8) {
        return true;
      }
    } else {
      timer += 10;
      delay(10);
    }
  }
  return false;
}

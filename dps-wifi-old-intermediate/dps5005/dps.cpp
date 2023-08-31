#include "dps.hpp"
#include "modbus.hpp"
#include "settings.h"
#include <stdint.h>
#include "Arduino.h"

static uint8_t recvd[20] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static uint8_t recstart[20] = {0xAA,0x01,0x2B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6};


bool dps_read_status(dps_status *dest) {
  Serial.write(recstart, 20);
Serial.println("");
if (Serial.available()) {
  for (int i = 0; i <= 20; i++) {
  recvd[i] = Serial.read();
  }
}
  for (int i = 0; i <= 20; i++) {
  Serial.print(recvd[i]);
  Serial.print(" ");
  }
  Serial.println("");
  
  for (int i = 0; i <= 20; i++) {
  Serial.print(recvd[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  

//  if (modbus_read_regs(DPS_REG_VOLTAGE, DPS_REG_ONOFF + 1, dest)) {
    dest->uset = (uint16_t)((recvd[7] << 8) + (recvd[8] & 0x00ff)); 
    dest->iset = (uint16_t)((recvd[9] << 8) + (recvd[10] & 0x00ff));
    dest->uout = (uint16_t)((recvd[7] << 8) + (recvd[8] & 0x00ff));;
    dest->iout = (uint16_t)((recvd[9] << 8) + (recvd[10] & 0x00ff));;
    dest->power = htons(dest->power);
    dest->uin = htons(dest->uin);
    dest->lock = htons(dest->lock);
    dest->protect = htons(dest->protect);
    dest->cvcc = htons(dest->cvcc);
    dest->onoff = htons(dest->onoff);
    return true;
//  } else {
//    return false;
//  }
}

bool dps_set_voltage(const uint16_t voltage) {
  return modbus_write_reg(DPS_REG_VOLTAGE, voltage);
}

bool dps_set_current(const uint16_t current) {
  return modbus_write_reg(DPS_REG_CURRENT, current);
}

bool dps_set_voltage_current(const uint16_t voltage, const uint16_t current) {
  const uint16_t data[] = {voltage, current};
  return modbus_write_regs(DPS_REG_VOLTAGE, 2, data);
}

bool dps_set_onoff(bool value) {
  modbus_write_reg(DPS_REG_ONOFF, value?1:0);
}

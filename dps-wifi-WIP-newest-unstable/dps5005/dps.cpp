#include "dps.hpp"
#include "modbus.hpp"
#include "settings.h"
#include <stdint.h>
#include "Arduino.h"


unsigned long prevtime = 0;
const long timeouttime = 2000;
uint8_t voltsnamp[4] = {0x00,0x00,0x00,0x00};
static uint8_t recvd[20] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static uint8_t recvd2[20] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static uint8_t recstart[20] = {0xAA,0x01,0x2B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6};
static uint8_t readon[20] = {0xAA,0x01,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD1};
static uint8_t onread[20] = {0xAA,0x01,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD4};
static uint8_t pwrset[20] = {0xAA,0x01,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCD};
static uint8_t poop[20] = {0xAA,0x01,0x2C,0x13,0x88,0x12,0xAB,0x01,0xF4,0x00,0x04,0x00,0x00,0x00,0x42,0x00,0x00,0x00,0x00,0x6A};
uint16_t ispwron = 0;
uint8_t addedup = (0xAA+0x01+0x2C+0x13+0x88+0x12+0xAB+0x04+0x42);

uint8_t sumsum(void) {
  uint8_t result = 0;
  for (int i = 0; i <= 18; i++) {
  result = (result + poop[i]);
}
return result;
}

bool dps_read_status(dps_status *dest) {

for (int i = 0; i <= 20; i++) {
  recvd2[i] = 0;
  recvd[i] = 0;
}

Serial.write(recstart, 20);
Serial.println("");
delay(40);
if (Serial.available()) {
  for (int i = 0; i <= 20; i++) {
  recvd[i] = Serial.read();
  }




  Serial.swap();
  delay(10);

Serial.println(recvd[7]);
Serial.println(recvd[8]);

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

  Serial.swap();
  delay(10);
}
/*
poop[7] = recvd[7];
poop[8] = recvd[8];
poop[9] = recvd[9];
poop[10] = recvd[10];
poop[19] = sumsum();
Serial.write(poop, 20);
*/
Serial.write(readon, 20);
Serial.println("");
delay(40);
if (Serial.available()) {
  for (int i = 0; i <= 20; i++) {
  recvd2[i] = Serial.read();
  }
    Serial.swap();
  delay(10);
  for (int i = 0; i <= 20; i++) {
  Serial.print(recvd2[i], HEX);
  Serial.print(" ");
  }
  Serial.println("");

  Serial.swap();
  delay(10);
}


/*
//Serial.write(onread, 20);
Serial.write(readon, 20);
Serial.println("");

if (Serial.available()) {
  for (int i = 0; i <= 20; i++) {
  recvd2[i] = Serial.read();
//  Serial.print(recvd2[i]);
//  Serial.print(" ");
  }
}

  for (int i = 0; i <= 20; i++) {
  Serial.print(recvd2[i]);
  Serial.print(" ");
  }
    
  Serial.println("");
*/

//  if (modbus_read_regs(DPS_REG_VOLTAGE, DPS_REG_ONOFF + 1, dest)) {
    dest->uset = (uint16_t)((poop[7] << 8) + (poop[8] & 0x00ff)); 
    dest->iset = (uint16_t)((poop[9] << 8) + (poop[10] & 0x00ff));
    dest->uout = (uint16_t)((poop[7] << 8) + (poop[8] & 0x00ff));;
    dest->iout = (uint16_t)((poop[9] << 8) + (poop[10] & 0x00ff));;
    dest->power = htons(dest->power);
    dest->uin = htons(dest->uin);
    dest->lock = htons(dest->lock);
    dest->protect = htons(dest->protect);
    dest->cvcc = htons(dest->cvcc);
    dest->onoff = ispwron;
    return true;
//  } else {
//    return false;
//  }
}

bool dps_set_voltage(const uint16_t voltage) {
    dps_status dps;
    delay(100);
    
//  return modbus_write_reg(DPS_REG_VOLTAGE, voltage);
//recvd[2] = 0x2C;
  poop[7] = ((uint8_t)(voltage >> 8));
  poop[8] = ((uint8_t)voltage);
//  uint8_t chks = (addedup + poop[7] + poop[8]);
//  poop[19] = chks;
poop[19] = sumsum();
poop[19] = poop[7];  /* voltsnamp[0] = poop[7]; */
poop[19] = poop[8];         /* voltsnamp[1] = poop[8]; */
voltsnamp[0] = poop[7];
voltsnamp[1] = poop[8];
//  recvd[9] = ((uint8_t)(voltage >> 8));
//  recvd[10] = ((uint8_t)voltage);
  Serial.write(poop, 20);
  Serial.println("");
//  delay(200);
//  dps.iset = (uint16_t)((recvd[7] << 8) + (recvd[8] & 0x00ff));
//  dps.iout = (uint16_t)((recvd[7] << 8) + (recvd[8] & 0x00ff));;
  if (dps_read_status(&dps)) {
    ;
  }

//  Serial.write(recvd, 20);
//  Serial.println("");
//Serial.write(recstart, 20);
//Serial.println("");
//  for (int i = 0; i <= 20; i++) {
//  Serial.print(recvd[i]);
//  Serial.print(" ");
//  }
//Serial.println("");
/*
  while (Serial.available()) {
      Serial.read(); // read to devnull
  }
  */
  return true;
}

bool dps_set_current(const uint16_t current) {
  poop[9] = ((uint8_t)(current >> 8));
  poop[10] = ((uint8_t)current);
  poop[19] = sumsum();
//  voltsnamp[2] = poop[9];
//  voltsnamp[3] = poop[10];
  Serial.write(poop, 20);
  Serial.println("");
  delay(20);

//Serial.write(recstart, 20);
//Serial.println("");
//  for (int i = 0; i <= 20; i++) {
//  Serial.print(recvd[i]);
//  Serial.print(" ");
//  }
//Serial.println("");

return true;
}

bool dps_set_voltage_current(const uint16_t voltage, const uint16_t current) {
dps_set_current(current);
dps_set_voltage(voltage);
return 0;
}

bool dps_set_onoff(bool value) {
//  modbus_write_reg(DPS_REG_ONOFF, value?1:0);
unsigned long currenttime = millis();
static uint8_t pwroff[20] = {0xAA,0x01,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCD};
static uint8_t pwron[20] = {0xAA,0x01,0x22,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCE};
/*
Serial.print("millis = ");
Serial.println(millis());
Serial.print("currenttime = ");
Serial.println(currenttime);
Serial.print("prevtime = ");
Serial.println(prevtime);
//  for (int i = 0; i <= 20; i++) {
//  recvd2[i] = 0x00;
//  }
*/
if (value == 0) {
  /*
  if ((currenttime - prevtime) < 2000) {
    prevtime = currenttime;
    return true;
  }
*/
  Serial.write(pwroff, 20);
  delay(30);
    int timer = 0;
  int cntr = 0;
  ispwron = 0;
  while (timer < MODBUS_TIMEOUT_MS) {
  if (Serial.available()) {
      Serial.read(); // read to devnull
      if (++cntr >= 8) {
        prevtime = currenttime;
        return true;
      }
    } else {
      timer += 10;
      delay(10);
    }
  }
  ispwron = 0;
    Serial.swap();
  
  delay(500);
  Serial.println("poweroff");
    Serial.swap();
  delay(10);
//  prevtime = currenttime;
  return true;
}
/*
if (value == 1) {
    if ((currenttime - prevtime) < 2000) {
    prevtime = currenttime;
    return false;
  }
*/
  Serial.write(pwron, 20);
  delay(30);
  int timer = 0;
  int cntr = 0;
  ispwron = 1;
    while (timer < MODBUS_TIMEOUT_MS) {
  if (Serial.available()) {
      Serial.read(); // read to devnull
      if (++cntr >= 8) {
//        prevtime = currenttime;
        return true;
      }
    } else {
      timer += 10;
      delay(10);
    }
 // }
    Serial.swap();
  delay(500);
  Serial.println("poweron");
    Serial.swap();
  delay(10);
//  prevtime = currenttime;
  return true;
  }
//  prevtime = currenttime;
  return false;
}

#ifndef _MODBUSRTU_H
#define _MODBUSRTU_H
#include "ModbusServerRTU.h" 

#define OP_LED 33
#define TX2_PIN 12   // Serial2 RX 핀 (GPIO12)
#define RX2_PIN 14   // Serial2 TX 핀 (GPIO14)
ModbusMessage FC03(ModbusMessage request) ;
#endif
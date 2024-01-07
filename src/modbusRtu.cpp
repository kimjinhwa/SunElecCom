#include <Arduino.h>
#include <EEPROM.h>
#include "modbusRtu.h"

struct _cell_value{
  float voltage;
  float impendance;
  int16_t temperature;
};
_cell_value cellvalue[40];

ModbusMessage FC03(ModbusMessage request) {
  uint16_t address;           // requested register address
  uint16_t words;             // requested number of registers
  ModbusMessage response;     // response message to be sent back
  uint16_t value;
  uint16_t sendValue[256];
  memset(sendValue,0x00,256);
  // get request values
  request.get(2, address);
  request.get(4, words);

  if( (address & 0x00ff) + words > 40){
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  } 
  
  response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
  if(address <= 0xff){
    for (int i = 0; i < words; i++)
    {
      value = (uint16_t)(cellvalue[i].voltage*100);
      response.add(value);
    }
  }
  else if(address <= 0x1ff){
    for (int i = 0; i < words; i++)
    {
      value = cellvalue[i].temperature;
      response.add(value);
    }
  }
  else if(address <= 0x2ff){
    for (int i = 0; i < words; i++)
    {
      value = (u_int16_t)(cellvalue[i].impendance*100);
      response.add(value);
    }
  }
  else if(address <= 0x3ff){
    for (int i = 0; i < words; i++)
    {
      value = sendValue[i];
      response.add(value);
    }
  }
  else if(address <= 0x400){
    sendValue[0]=2;
    sendValue[1]=20;
    sendValue[2]=150;
    sendValue[3]=1450;
    sendValue[4]=850;
    sendValue[7]=10000;
    for (int i = 0; i < words; i++)
    {
      value = sendValue[i];
      response.add(value);
    }
  }
  else if(address <= 0x700){
    for (int i = 0; i < words; i++)
    {
      uint8_t _modBusID = EEPROM.readByte(1);
      value = _modBusID;
      response.add(value);
    }
  }
  return response;
};
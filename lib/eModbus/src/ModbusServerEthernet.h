// =================================================================================================
// eModbus: Copyright 2020 by Michael Harwerth, Bert Melis and the contributors to eModbus
//               MIT license - see license.md for details
// =================================================================================================
#ifndef _MODBUS_SERVER_ETHERNET_H
#define _MODBUS_SERVER_ETHERNET_H
#include "options.h"
#if HAS_ETHERNET == 1
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <SPI.h>

#undef SERVER_END
#define SERVER_END server.end();

#include "ModbusServerTCPtemp.h"
using ModbusServerEthernet = ModbusServerTCP<WiFiServer , WiFiClient >;
#endif

#endif

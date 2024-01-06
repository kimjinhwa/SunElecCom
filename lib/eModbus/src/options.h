// =================================================================================================
// eModbus: Copyright 2020 by Michael Harwerth, Bert Melis and the contributors to eModbus
//               MIT license - see license.md for details
// =================================================================================================
#ifndef _EMODBUS_OPTIONS_H
#define _EMODBUS_OPTIONS_H

/* === ESP32 DEFINITIONS AND MACROS === */
#if defined(ESP32) 
#include <Arduino.h>
#define USE_MUTEX 1
#define HAS_FREERTOS 1
#define HAS_ETHERNET 1
#define IS_LINUX 0
#define NEED_UART_PATCH 1
#endif

/* === COMMON MACROS === */
#if USE_MUTEX
#define LOCK_GUARD(x,y) std::lock_guard<std::mutex> x(y);
#else
#define LOCK_GUARD(x,y)
#endif

#endif // _EMODBUS_OPTIONS_H

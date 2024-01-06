#include <Arduino.h>
#include <stdio.h>
#include <ETH.h>
#include <Ethernet.h>
#include <Wifi.h>
#include <WiFiClient.h>
#include "mainGrobaldef.h"

uint16_t port = 502;
WiFiUDP udp;

static char TAG[] = "snmpMain";

extern WiFiClient Client;
int log_printf_telnet(const char *fmt, ...);

void snmpSetup();
void snmpLoop(int job);

void snmpRequest(void *parameter)
{
    snmpSetup();
    while (1)
    {
        snmpLoop(0);
    }
}
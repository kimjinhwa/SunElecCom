#ifndef _NARADAV13_H
#define _NARADAV13_H

#include <functional>
#include <map>
#include "options.h"
#include "ModbusMessage.h"

#if HAS_FREERTOS
extern "C" {
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
}
#endif
#define MAX_PACK_NUMBER     8

typedef struct {
    uint16_t voltageNumber;//설치된 배터리의 수 
    uint16_t voltage[16]; // 상위 3비트는 균등화 플레그, 과전압플레그, 배터리전전압 플레그    
    uint16_t ampere;   // ofset 30000, (30000 - (data0*256 + data1) )/100
    uint16_t soc;    // 0.01
    uint16_t Capacity;    //0.01
    uint16_t TempreatureNumber; //설치된 온도계의 수
    uint16_t Tempreature[6];   // offset -50 
    uint16_t packStatus[10];    
    uint16_t readCycleCount;    
    uint16_t totalVoltage; //0.01   
    uint16_t SOH; //0.01   % SOH = data0*256 + data1
    uint16_t BMS_PROTECT_STATUS; //0.01   % SOH = data0*256 + data1
}batteryInofo_t; 
class McMessage {
    public:
        McMessage(){};
    private:
};

typedef std::function<void(McMessage msg, uint32_t token)> NCOnData;
class NaradaClient232{
public:
    NaradaClient232();
    bool onDataHandler(NCOnData handler); // Accept onData handler
    void begin();
    void getPackData(int packNumber);
    Error readAnswerData(int packNumber);
    int dataParse(int packNumge,const uint8_t *revData);
    int dataParseExt(int packNumber,const uint8_t *revData);
    uint8_t checksum(unsigned char* buf, size_t len) ;
    void makeInt(uint16_t *dest,const uint8_t *src,byte len);
    void copyBatInfoData(int packNumber,void* dest );
    SemaphoreHandle_t revDataMutex;
private:
    //virtual void isInstance() = 0; // make class abstract
    batteryInofo_t batInfo[8];
protected:
    NCOnData onData;    //Data response handler
};

extern NaradaClient232 naradaClient232;
void h_pxNaradaV13Request(void *parameter);
// #ifdef __cplusplus
// }
// #endif

#endif
#include <Arduino.h>
#include <naradav13.h>
#include <bitset>
#include <Logging.h>
//#include "ModbusClient.h"
#define SET_TIMEOUT 300
#define OPLED_ON false
#define OPLED_OFF true
//이제 나라다 전용의 프로토콜 클레스를 하나 만들자

static const unsigned long UPTIME_UPDATE_INTERVAL = 1000; // ms = 1 second
static unsigned long lastUptimeUpdateTime = 0;

NaradaClient232::NaradaClient232()
{
    onData=nullptr;
    SemaphoreHandle_t revDataMutex;
    //onData((NCOnData)nullptr,0);
    //onData(nullptr);
}
bool NaradaClient232::onDataHandler(NCOnData handler) // Accept onData handler
{
    if(onData){
        LOG_W("onData handler was already claimed\n");
    }
    else{
        LOG_I("onData handler was claimed\n");
        onData = handler;
    }
    return true;
}
uint8_t NaradaClient232::checksum(unsigned char* buf, size_t len) {
    unsigned char i, chk = 0;
    int sum = 0;
    for (i = 0; i < len; i++)
    {
        chk ^= buf[i];
        sum += buf[i];
    }
    unsigned char bChecksum = (chk ^ sum) & 0xFF;
    return (bChecksum);
}
void NaradaClient232::begin(){
};


void NaradaClient232::makeInt(uint16_t *dest,const uint8_t *src,byte len){
    memset(dest,0x00,len*2);
    for(int i=0;i<len;i++)dest[i]= src[i*2]<<8 | src[i*2+1];    
}

void NaradaClient232::copyBatInfoData(void* dest, void* src){
    xSemaphoreTake(revDataMutex, portMAX_DELAY);
    memcpy(dest,src,sizeof(batteryInofo_t));
    xSemaphoreGive(revDataMutex);
}
int NaradaClient232::dataParse(int packNumber,const uint8_t *revData){
    revData = revData+4;
    xSemaphoreTake(revDataMutex, portMAX_DELAY);
    for(int i=0 ;i<11;i++){
        dataParseExt(packNumber, revData);
        revData = revData + revData[1]*2 +2;
    }
    xSemaphoreGive(revDataMutex);
    return 0;
};
int NaradaClient232::dataParseExt(int packNumber,const uint8_t *revData){
    uint8_t dataLen = revData[1];
    uint8_t command = revData[0];
    revData = revData +2;
    switch(command){
        case 1:// 전압
            makeInt(batInfo[packNumber].voltage,revData,dataLen);  //0xF가 올것이다 
            //  01 0F 
            //      0     1     2     3     4     4     6     7     8     9     A     B     C     D     E
            //  0C C9 0C DD 0C BC 0C DC 0C DC 0C C2 0C C7 0C BF 0C B5 0C C4 0C C3 0A E3 0C E1 0C E1 0C D6 
        break;
        case 2:// 전류
            makeInt(&batInfo[packNumber].ampere,revData,dataLen);  //0xF가 올것이다 
            // 02 01 75 30 
        break;
        case 3:// SOC 
            makeInt(&batInfo[packNumber].soc,revData,dataLen);  //0xF가 올것이다 
            //  03 01 13 88 
        break;
        case 4:// Battery Capacity 
            makeInt(&batInfo[packNumber].Capacity,revData,dataLen);  //0xF가 올것이다 
            //  04 01 27 10 
        break;
        case 5:// Temperature
            makeInt(batInfo[packNumber].Tempreature,revData,dataLen);  //0xF가 올것이다 
            //  05 06 00 50 00 4F 00 51 00 50 40 52 20 51 
        break;
        case 6:// Battery Pack Status
            makeInt(batInfo[packNumber].packStatus,revData,dataLen);  //0xF가 올것이다 
            //  06 05 00 00 10 00 00 00 00 00 00 02 
        break;
        case 7:// 읽기 사이클 카운트  
            makeInt(&batInfo[packNumber].readCycleCount,revData,dataLen);  //0xF가 올것이다 
        break;
        case 8:// 총 전압 읽기 
            makeInt(&batInfo[packNumber].totalVoltage,revData,dataLen);  //0xF가 올것이다 
            //  08 01 13 02 
        break;
        case 9:// SOH 읽기 
            makeInt(&batInfo[packNumber].SOH,revData,dataLen);  //0xF가 올것이다 
            //  09 01 27 10 
        break;
        case 10:// BMS보호 상태 비트 읽기; 보호 상태 비트 = data0 * 256 + dual 
            makeInt(&batInfo[packNumber].BMS_PROTECT_STATUS,revData,dataLen);  //0xF가 올것이다 
            //  0A 01 00 00 
        break;
        default:
        break;
    }
    return SUCCESS;
}
Error NaradaClient232::readAnswerData(int packNumber){
    int readCount =0;
    int loopCount=0;
    int timeOut = 100;
    uint8_t revData[255];
    memset(revData,0x00,255);
    if(!Serial.available()){
        loopCount++;
        vTaskDelay(10);
        if(loopCount >= timeOut) return TIMEOUT;
    }
    readCount =  Serial.readBytes(revData,4);
    if(readCount<4) return TIMEOUT;
    readCount +=  Serial.readBytes(&revData[4],revData[3]+2);// 마지막  checksum Flag 0x0D
    if(readCount < (revData[3]+4) ) return TIMEOUT;
    //명령어 4개 + data 0x56(86) + checksum + endflag = 92
    dataParse(packNumber, revData);
    checksum(revData,4+revData[4]);
    if(revData[4+revData[3]] !=  checksum(revData,4+revData[3]))return CRC_ERROR;
    return SUCCESS;
} 
void NaradaClient232::getPackData(int packNumber){
    byte sendData[7]; memset(sendData,0x00,7);
    byte index=0;
    //7E 01 01 00 FE 0D`
    sendData[index++] = 0x7E;
    sendData[index++] = packNumber;//x01;
    sendData[index++] = 01;//x01;
    sendData[index++] = 00;//x01;
    sendData[index++] =checksum(sendData,index);
    sendData[index++] = 00;//checksum
    sendData[index++] = 0x0D;//x01;
    Serial.write(sendData,index);
    readAnswerData(packNumber);
};

NaradaClient232 naradaClient232;
static int interval = 1000;
static unsigned long previousmills = 0;
static int everySecondInterval = 2000;
static unsigned long now;
void h_pxNaradaV13Request(void *parameter)
{
    naradaClient232.begin();
    int packNumber =0;
    for (;;)
    {
        now = millis();
        if ((now - previousmills > everySecondInterval))
        {
            naradaClient232.getPackData(packNumber);
            previousmills = now;
            packNumber++;
            if(packNumber >= MAX_PACK_NUMBER) packNumber =0;     
        }
        vTaskDelay(1000);
        delay(10);
    }
}
// // 함수를 이용하여 숫자로 변환하는 유틸리티 함수
// float extractFloatValue(const String &str)
// {
//     return str.toFloat();
// }


// int extractIntValue(const String &str)
// {
//     return str.toInt();
// }
// char cr = 0x0d;


// void selectPrintf(uint sel, const char *format, ...);

// extern char *kepChargeMode;

// void setKepSnmpValue()
// {
// }

    //printf("\nChecksum is %2x , %02x",revData[4+revData[3]],checksum(revData,4+revData[3]));
    // while(readCount < revData.dataLength-1){
    //     readCount += dataParse();
    //}


// 데이타 길이는 +1 을 해 주어야 한다( 0D 때문에)
//  7E 01 01 56 
//  01 0F 
//      0     1     2     3     4     4     6     7     8     9     A     B     C     D     E
//  0C C9 0C DD 0C BC 0C DC 0C DC 0C C2 0C C7 0C BF 0C B5 0C C4 0C C3 0A E3 0C E1 0C E1 0C D6 
//  02 01 75 30 
//  03 01 13 88 
//  04 01 27 10 
//  05 06 00 50 00 4F 00 51 00 50 40 52 20 51 
//  06 05 00 00 10 00 00 00 00 00 00 02 
//  07 01 00 00 
//  08 01 13 02 
//  09 01 27 10 
//  0A 01 00 00 
//  00 
//  0D  (92 BYTE)
//  read Data
//  7E 01 01 56 01 0F 0C FA 0C FA 0C FA 0C F8 0C F6 0C FA 0C FA 0C F8 0C F9 0C F9 0C FA 0C F9 0C F8 0C FA 0C FC 02 01 75 30 03 01 1D 4C 04 01 27 28 05 06 00 44 00 44 00 44 00 44 40 47 20 46 06 05 00 00 10 00 00 00 00 00 00 00 07 01 00 01 08 01 13 75 09 01 27 10 0A 01 40 00 4E 0D 
//  7E 01 01 56 01 0F 0C FA 0C FA 
//  0C FA 0C F8 0C F6 0C FA 0C FA 0C F8 0C F9 0C F9 0C FA 0C F9 0C F8 0C FA 0C FC 02 01 75 30 03 01 1D 4C 04 01 27 28 05 06 00 44 00 44 00 44 00 44 40 47 20 46 06 05 00 00 10 00 00 00 00 00 00 00 07 01 00 01 08 01 13 75 09 01 27 10 0A 01 40 00 4E 0D 
// typedef struct {
//    byte Head; 
//    byte address; 
//    byte CID; 
//    byte dataLength; 
// }naradav13_headingData_t;
// naradav13_headingData_t revData;
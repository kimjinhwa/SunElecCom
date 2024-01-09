#ifndef RFC1213_H
#define RFC1213_H
#include <ArduinoJson.h>
// RFC1213-MIB (System)
//************************************
//* SNMP Configuration               *
//************************************
const char rocommunity[] = "public";          // Read only community string
const char rwcommunity[] = "private";         // Read Write community string for set commands
char oidSysDescr[] = ".1.3.6.1.2.1.1.1.0";    // OctetString SysDescr
char oidSysObjectID[] = ".1.3.6.1.2.1.1.2.0"; // OctetString SysObjectID
char oidSysUptime[] = ".1.3.6.1.2.1.1.3.0";   // TimeTicks sysUptime (hundredths of seconds)
char oidSysContact[] = ".1.3.6.1.2.1.1.4.0";  // OctetString SysContact
char oidSysName[] = ".1.3.6.1.2.1.1.5.0";     // OctetString SysName
char oidSysLocation[] = ".1.3.6.1.2.1.1.6.0"; // OctetString SysLocation
char oidSysServices[] = ".1.3.6.1.2.1.1.7.0"; // Integer sysServices

// ENTITY-MIB .1.3.6.1.2.1.47 - Needs to be implemented to support ENTITY-SENSOR-MIB
// An entry would be required per sensor. This is index 1.

// entityPhysicalTable
char oidentPhysicalIndex_1[] = ".1.3.6.1.2.1.47.1.1.1.1.1.1";
char oidentPhysicalDescr_1[] = ".1.3.6.1.2.1.47.1.1.1.1.2.1";
char oidentPhysicalVendorType_1[] = ".1.3.6.1.2.1.47.1.1.1.1.3.1";
char oidentPhysicalContainedIn_1[] = ".1.3.6.1.2.1.47.1.1.1.1.4.1";
char oidentPhysicalClass_1[] = ".1.3.6.1.2.1.47.1.1.1.1.5.1";
char oidentPhysicalParentRelPos_1[] = ".1.3.6.1.2.1.47.1.1.1.1.6.1";
char oidentPhysicalName_1[] = ".1.3.6.1.2.1.47.1.1.1.1.7.1";
char oidentPhysicalHardwareRev_1[] = ".1.3.6.1.2.1.47.1.1.1.1.8.1";
char oidentPhysicalFirmwareRev_1[] = ".1.3.6.1.2.1.47.1.1.1.1.9.1";
char oidentPhysicalSoftwareRev_1[] = ".1.3.6.1.2.1.47.1.1.1.1.10.1";
char oidentPhysicalSerialNum_1[] = ".1.3.6.1.2.1.47.1.1.1.1.11.1";
char oidentPhysicalMfgName_1[] = ".1.3.6.1.2.1.47.1.1.1.1.12.1";
char oidentPhysicalModelName_1[] = ".1.3.6.1.2.1.47.1.1.1.1.13.1";
char oidentPhysicalAlias_1[] = ".1.3.6.1.2.1.47.1.1.1.1.14.1";
char oidentPhysicalAssetID_1[] = ".1.3.6.1.2.1.47.1.1.1.1.15.1";
char oidentPhysicalIsFRU_1[] = ".1.3.6.1.2.1.47.1.1.1.1.16.1";
char oidentPhysicalMfgDate_1[] = ".1.3.6.1.2.1.47.1.1.1.1.17.1";
char oidentPhysicalUris_1[] = ".1.3.6.1.2.1.47.1.1.1.1.18.1";

int entPhysicalIndex_1 = 1;
std::string entPhysicalDescr_1 = "Fake Temperature Sensor";
std::string entPhysicalVendorType_1 = "";
int entPhysicalContainedIn_1 = 0;
int entPhysicalClass_1 = 8; // Sensor
int entPhysicalParentRelPos_1 = -1;
std::string entPhysicalName_1 = "";
std::string entPhysicalHardwareRev_1 = "";
std::string entPhysicalFirmwareRev_1 = "";
std::string entPhysicalSoftwareRev_1 = "";
std::string entPhysicalSerialNum_1 = "";
std::string entPhysicalMfgName_1 = "";
std::string entPhysicalModelName_11 = "";
std::string entPhysicalAlias_1 = "";
std::string entPhysicalAssetID_1 = "";
int entPhysicalIsFRU_1 = 0;
std::string entPhysicalMfgDate_1 = "'0000000000000000'H"; // Special value, not sure it's correct. Or meant to be a Hex string?
std::string entPhysicalUris_1 = "";

// EntityPhysicalGroup

// ENTITY-SENSOR-MIB .1.3.6.1.2.1.99
// An entry would be required per sensor. This is index 1.
// Must match index in ENTITY-MIB
char oidentPhySensorType_1[] = ".1.3.6.1.2.1.99.1.1.1.1.1";
char oidentPhySensorScale_1[] = ".1.3.6.1.2.1.99.1.1.1.2.1";
char oidentPhySensorPrecision_1[] = ".1.3.6.1.2.1.99.1.1.1.3.1";
char oidentPhySensorValue_1[] = ".1.3.6.1.2.1.99.1.1.1.4.1";
char oidentPhySensorOperStatus_1[] = ".1.3.6.1.2.1.99.1.1.1.5.1";
char oidentPhySensorUnitsDisplay_1[] = ".1.3.6.1.2.1.99.1.1.1.6.1";
char oidentPhySensorValueTimeStamp_1[] = ".1.3.6.1.2.1.99.1.1.1.7.1";
char oidentPhySensorValueUpdateRate_1[] = ".1.3.6.1.2.1.99.1.1.1.8.1";
// char oidRfc1213_endof1361[] = ".1.3.6.1.2.1.99.1.1.1.9.0";

int entPhySensorType_1 = 8;  // Celsius
int entPhySensorScale_1 = 9; // Units
int entPhySensorPrecision_1 = 0;
int entPhySensorValue_1 = 0;      // Value to be updated
int entPhySensorOperStatus_1 = 1; // OK
std::string entPhySensorUnitsDisplay_1 = "Celsius";
uint32_t entPhySensorValueTimeStamp_1 = 0;
int entPhySensorValueUpdateRate_1 = 0; // Unknown at declaration, set later.
//************************************

//**********************
//*  KEP MIB *
//**********************
char oidKEPinputVoltage_R[] = ".1.3.6.1.4.1.12236.2.12082.1.0";
char oidKEPinputVoltage_S[] = ".1.3.6.1.4.1.12236.2.12082.2.0";
char oidKEPinputVoltage_T[] = ".1.3.6.1.4.1.12236.2.12082.3.0";
char oidKEPoutputVoltage[] = ".1.3.6.1.4.1.12236.2.12082.4.0";
char oidKEPoutputCurrent[] = ".1.3.6.1.4.1.12236.2.12082.5.0";
char oidKEPbatteryVoltage[] = ".1.3.6.1.4.1.12236.2.12082.6.0";
char oidKEPchargeCurrent[] = ".1.3.6.1.4.1.12236.2.12082.7.0";
char oidKEPchargeMode[] = ".1.3.6.1.4.1.12236.2.12082.8.0";
char oidKEPoutputFrequency[] = ".1.3.6.1.4.1.12236.2.12082.9.0";
char oidKEPpowerFail[] = ".1.3.6.1.4.1.12236.2.12082.200.0";
char oidKEPpowerFailRecovery[] = ".1.3.6.1.4.1.12236.2.12082.201.0";
char oidKEPinputpowerFail[] = ".1.3.6.1.4.1.12236.2.12082.202.0";
char oidKEPinputpowerFailRecovery[] = ".1.3.6.1.4.1.12236.2.12082.203.0";

char oidKEPmodule1[] = ".1.3.6.1.4.1.12236.2.12082.210.0";
char oidKEPmodule1Recover[] = ".1.3.6.1.4.1.12236.2.12082.211.0";
char oidKEPmodule2[] = ".1.3.6.1.4.1.12236.2.12082.212.0";
char oidKEPmodule2Recover[] = ".1.3.6.1.4.1.12236.2.12082.213.0";
char oidKEPmodule3[] = ".1.3.6.1.4.1.12236.2.12082.214.0";
char oidKEPmodule3Recover[] = ".1.3.6.1.4.1.12236.2.12082.215.0";
char oidKEPmodule4[] = ".1.3.6.1.4.1.12236.2.12082.216.0";
char oidKEPmodule4Recover[] = ".1.3.6.1.4.1.12236.2.12082.217.0";
char oidKEPmoduleEND[] = ".1.3.6.1.4.1.12236.3";

// 나라다 전압
const char *oidIFTBatNumber[] = {
    ".1.3.6.1.2.1.32.1.1.0", 
    ".1.3.6.1.2.1.32.2.1.0", 
    ".1.3.6.1.2.1.32.3.1.0", 
    ".1.3.6.1.2.1.32.4.1.0", 
    ".1.3.6.1.2.1.32.5.1.0", 
    ".1.3.6.1.2.1.32.6.1.0", 
    ".1.3.6.1.2.1.32.7.1.0", 
    ".1.3.6.1.2.1.32.8.1.0", 
};
const char *oidIFTBatVoltageR1[] = {
    ".1.3.6.1.2.1.32.1.1.1.0",
    ".1.3.6.1.2.1.32.1.1.2.0",
    ".1.3.6.1.2.1.32.1.1.3.0",
    ".1.3.6.1.2.1.32.1.1.4.0",
    ".1.3.6.1.2.1.32.1.1.5.0",
    ".1.3.6.1.2.1.32.1.1.6.0",
    ".1.3.6.1.2.1.32.1.1.7.0",
    ".1.3.6.1.2.1.32.1.1.8.0",
    ".1.3.6.1.2.1.32.1.1.9.0",
    ".1.3.6.1.2.1.32.1.1.10.0",
    ".1.3.6.1.2.1.32.1.1.11.0",
    ".1.3.6.1.2.1.32.1.1.12.0",
    ".1.3.6.1.2.1.32.1.1.13.0",
    ".1.3.6.1.2.1.32.1.1.14.0",
    ".1.3.6.1.2.1.32.1.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR2[] = {
    ".1.3.6.1.2.1.32.2.1.1.0",
    ".1.3.6.1.2.1.32.2.1.2.0",
    ".1.3.6.1.2.1.32.2.1.3.0",
    ".1.3.6.1.2.1.32.2.1.4.0",
    ".1.3.6.1.2.1.32.2.1.5.0",
    ".1.3.6.1.2.1.32.2.1.6.0",
    ".1.3.6.1.2.1.32.2.1.7.0",
    ".1.3.6.1.2.1.32.2.1.8.0",
    ".1.3.6.1.2.1.32.2.1.9.0",
    ".1.3.6.1.2.1.32.2.1.10.0",
    ".1.3.6.1.2.1.32.2.1.11.0",
    ".1.3.6.1.2.1.32.2.1.12.0",
    ".1.3.6.1.2.1.32.2.1.13.0",
    ".1.3.6.1.2.1.32.2.1.14.0",
    ".1.3.6.1.2.1.32.2.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR3[] = {
    ".1.3.6.1.2.1.32.3.1.1.0",
    ".1.3.6.1.2.1.32.3.1.2.0",
    ".1.3.6.1.2.1.32.3.1.3.0",
    ".1.3.6.1.2.1.32.3.1.4.0",
    ".1.3.6.1.2.1.32.3.1.5.0",
    ".1.3.6.1.2.1.32.3.1.6.0",
    ".1.3.6.1.2.1.32.3.1.7.0",
    ".1.3.6.1.2.1.32.3.1.8.0",
    ".1.3.6.1.2.1.32.3.1.9.0",
    ".1.3.6.1.2.1.32.3.1.10.0",
    ".1.3.6.1.2.1.32.3.1.11.0",
    ".1.3.6.1.2.1.32.3.1.12.0",
    ".1.3.6.1.2.1.32.3.1.13.0",
    ".1.3.6.1.2.1.32.3.1.14.0",
    ".1.3.6.1.2.1.32.3.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR4[] = {
    ".1.3.6.1.2.1.32.4.1.1.0",
    ".1.3.6.1.2.1.32.4.1.2.0",
    ".1.3.6.1.2.1.32.4.1.3.0",
    ".1.3.6.1.2.1.32.4.1.4.0",
    ".1.3.6.1.2.1.32.4.1.5.0",
    ".1.3.6.1.2.1.32.4.1.6.0",
    ".1.3.6.1.2.1.32.4.1.7.0",
    ".1.3.6.1.2.1.32.4.1.8.0",
    ".1.3.6.1.2.1.32.4.1.9.0",
    ".1.3.6.1.2.1.32.4.1.10.0",
    ".1.3.6.1.2.1.32.4.1.11.0",
    ".1.3.6.1.2.1.32.4.1.12.0",
    ".1.3.6.1.2.1.32.4.1.13.0",
    ".1.3.6.1.2.1.32.4.1.14.0",
    ".1.3.6.1.2.1.32.4.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR5[] = {
    ".1.3.6.1.2.1.32.5.1.1.0",
    ".1.3.6.1.2.1.32.5.1.2.0",
    ".1.3.6.1.2.1.32.5.1.3.0",
    ".1.3.6.1.2.1.32.5.1.4.0",
    ".1.3.6.1.2.1.32.5.1.5.0",
    ".1.3.6.1.2.1.32.5.1.6.0",
    ".1.3.6.1.2.1.32.5.1.7.0",
    ".1.3.6.1.2.1.32.5.1.8.0",
    ".1.3.6.1.2.1.32.5.1.9.0",
    ".1.3.6.1.2.1.32.5.1.10.0",
    ".1.3.6.1.2.1.32.5.1.11.0",
    ".1.3.6.1.2.1.32.5.1.12.0",
    ".1.3.6.1.2.1.32.5.1.13.0",
    ".1.3.6.1.2.1.32.5.1.14.0",
    ".1.3.6.1.2.1.32.5.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR6[] = {
    ".1.3.6.1.2.1.32.6.1.1.0",
    ".1.3.6.1.2.1.32.6.1.2.0",
    ".1.3.6.1.2.1.32.6.1.3.0",
    ".1.3.6.1.2.1.32.6.1.4.0",
    ".1.3.6.1.2.1.32.6.1.5.0",
    ".1.3.6.1.2.1.32.6.1.6.0",
    ".1.3.6.1.2.1.32.6.1.7.0",
    ".1.3.6.1.2.1.32.6.1.8.0",
    ".1.3.6.1.2.1.32.6.1.9.0",
    ".1.3.6.1.2.1.32.6.1.10.0",
    ".1.3.6.1.2.1.32.6.1.11.0",
    ".1.3.6.1.2.1.32.6.1.12.0",
    ".1.3.6.1.2.1.32.6.1.13.0",
    ".1.3.6.1.2.1.32.6.1.14.0",
    ".1.3.6.1.2.1.32.6.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR7[] = {
    ".1.3.6.1.2.1.32.7.1.1.0",
    ".1.3.6.1.2.1.32.7.1.2.0",
    ".1.3.6.1.2.1.32.7.1.3.0",
    ".1.3.6.1.2.1.32.7.1.4.0",
    ".1.3.6.1.2.1.32.7.1.5.0",
    ".1.3.6.1.2.1.32.7.1.6.0",
    ".1.3.6.1.2.1.32.7.1.7.0",
    ".1.3.6.1.2.1.32.7.1.8.0",
    ".1.3.6.1.2.1.32.7.1.9.0",
    ".1.3.6.1.2.1.32.7.1.10.0",
    ".1.3.6.1.2.1.32.7.1.11.0",
    ".1.3.6.1.2.1.32.7.1.12.0",
    ".1.3.6.1.2.1.32.7.1.13.0",
    ".1.3.6.1.2.1.32.7.1.14.0",
    ".1.3.6.1.2.1.32.7.1.15.0"
};                     // 0:w
const char *oidIFTBatVoltageR8[] = {
    ".1.3.6.1.2.1.32.8.1.1.0",
    ".1.3.6.1.2.1.32.8.1.2.0",
    ".1.3.6.1.2.1.32.8.1.3.0",
    ".1.3.6.1.2.1.32.8.1.4.0",
    ".1.3.6.1.2.1.32.8.1.5.0",
    ".1.3.6.1.2.1.32.8.1.6.0",
    ".1.3.6.1.2.1.32.8.1.7.0",
    ".1.3.6.1.2.1.32.8.1.8.0",
    ".1.3.6.1.2.1.32.8.1.9.0",
    ".1.3.6.1.2.1.32.8.1.10.0",
    ".1.3.6.1.2.1.32.8.1.11.0",
    ".1.3.6.1.2.1.32.8.1.12.0",
    ".1.3.6.1.2.1.32.8.1.13.0",
    ".1.3.6.1.2.1.32.8.1.14.0",
    ".1.3.6.1.2.1.32.8.1.15.0"
};                     // 0:w

// 나라다 전압
const char *oidIFTBatCurrent[] = {
    ".1.3.6.1.2.1.32.1.2.0", 
    ".1.3.6.1.2.1.32.2.2.0", 
    ".1.3.6.1.2.1.32.3.2.0", 
    ".1.3.6.1.2.1.32.4.2.0", 
    ".1.3.6.1.2.1.32.5.2.0", 
    ".1.3.6.1.2.1.32.6.2.0", 
    ".1.3.6.1.2.1.32.7.2.0", 
    ".1.3.6.1.2.1.32.8.2.0", 
};
// 나라다 전압
const char *oidIFTBatSOC[] = {
    ".1.3.6.1.2.1.32.1.3.0", 
    ".1.3.6.1.2.1.32.2.3.0", 
    ".1.3.6.1.2.1.32.3.3.0", 
    ".1.3.6.1.2.1.32.4.3.0", 
    ".1.3.6.1.2.1.32.5.3.0", 
    ".1.3.6.1.2.1.32.6.3.0", 
    ".1.3.6.1.2.1.32.7.3.0", 
    ".1.3.6.1.2.1.32.8.3.0", 
};

const char *oidIFTBatCapacity[] = {
    ".1.3.6.1.2.1.32.1.4.0", 
    ".1.3.6.1.2.1.32.2.4.0", 
    ".1.3.6.1.2.1.32.3.4.0", 
    ".1.3.6.1.2.1.32.4.4.0", 
    ".1.3.6.1.2.1.32.5.4.0", 
    ".1.3.6.1.2.1.32.6.4.0", 
    ".1.3.6.1.2.1.32.7.4.0", 
    ".1.3.6.1.2.1.32.8.4.0", 
};

//온도센서 숫자
const char *oidTempNumber[] = {
    ".1.3.6.1.2.1.32.1.5.0", 
    ".1.3.6.1.2.1.32.2.5.0", 
    ".1.3.6.1.2.1.32.3.5.0", 
    ".1.3.6.1.2.1.32.4.5.0", 
    ".1.3.6.1.2.1.32.5.5.0", 
    ".1.3.6.1.2.1.32.6.5.0", 
    ".1.3.6.1.2.1.32.7.5.0", 
    ".1.3.6.1.2.1.32.8.5.0", 
};
const char *oidTempNumberR1[] = {
    ".1.3.6.1.2.1.32.1.5.1", 
    ".1.3.6.1.2.1.32.1.5.2", 
    ".1.3.6.1.2.1.32.1.5.3", 
    ".1.3.6.1.2.1.32.1.5.4"
};
const char *oidTempNumberR2[] = {
    ".1.3.6.1.2.1.32.2.5.1", 
    ".1.3.6.1.2.1.32.2.5.2", 
    ".1.3.6.1.2.1.32.2.5.3", 
    ".1.3.6.1.2.1.32.2.5.4" 
};
const char *oidTempNumberR3[] = {
    ".1.3.6.1.2.1.32.3.5.1", 
    ".1.3.6.1.2.1.32.3.5.2", 
    ".1.3.6.1.2.1.32.3.5.3", 
    ".1.3.6.1.2.1.32.3.5.4" 
};
const char *oidTempNumberR4[] = {
    ".1.3.6.1.2.1.32.4.5.1", 
    ".1.3.6.1.2.1.32.4.5.2", 
    ".1.3.6.1.2.1.32.4.5.3", 
    ".1.3.6.1.2.1.32.4.5.4" 
};
const char *oidTempNumberR5[] = {
    ".1.3.6.1.2.1.32.5.5.1", 
    ".1.3.6.1.2.1.32.5.5.2", 
    ".1.3.6.1.2.1.32.5.5.3", 
    ".1.3.6.1.2.1.32.5.5.4" 
};
const char *oidTempNumberR6[] = {
    ".1.3.6.1.2.1.32.6.5.1", 
    ".1.3.6.1.2.1.32.6.5.2", 
    ".1.3.6.1.2.1.32.6.5.3", 
    ".1.3.6.1.2.1.32.6.5.4" 
};
const char *oidTempNumberR7[] = {
    ".1.3.6.1.2.1.32.7.5.1", 
    ".1.3.6.1.2.1.32.7.5.2", 
    ".1.3.6.1.2.1.32.7.5.3", 
    ".1.3.6.1.2.1.32.7.5.4" 
};
const char *oidTempNumberR8[] = {
    ".1.3.6.1.2.1.32.8.5.1", 
    ".1.3.6.1.2.1.32.8.5.2", 
    ".1.3.6.1.2.1.32.8.5.3", 
    ".1.3.6.1.2.1.32.8.5.4" 
};
const char *oidAlarmNumber[] = {
    ".1.3.6.1.2.1.32.1.6.0", 
    ".1.3.6.1.2.1.32.2.6.0", 
    ".1.3.6.1.2.1.32.3.6.0", 
    ".1.3.6.1.2.1.32.4.6.0", 
    ".1.3.6.1.2.1.32.5.6.0", 
    ".1.3.6.1.2.1.32.6.6.0", 
    ".1.3.6.1.2.1.32.7.6.0", 
    ".1.3.6.1.2.1.32.8.6.0", 
};
const char *oidAlarmNumberR1[] = {
    ".1.3.6.1.2.1.32.1.6.1.0", 
    ".1.3.6.1.2.1.32.1.6.2.0", 
    ".1.3.6.1.2.1.32.1.6.3.0", 
    ".1.3.6.1.2.1.32.1.6.4.0", 
    ".1.3.6.1.2.1.32.1.6.5.0", 
};
const char *oidAlarmNumberR2[] = {
    ".1.3.6.1.2.1.32.2.6.1.0", 
    ".1.3.6.1.2.1.32.2.6.2.0", 
    ".1.3.6.1.2.1.32.2.6.3.0", 
    ".1.3.6.1.2.1.32.2.6.4.0", 
    ".1.3.6.1.2.1.32.2.6.5.0", 
};
const char *oidAlarmNumberR3[] = {
    ".1.3.6.1.2.1.32.3.6.1.0", 
    ".1.3.6.1.2.1.32.3.6.2.0", 
    ".1.3.6.1.2.1.32.3.6.3.0", 
    ".1.3.6.1.2.1.32.3.6.4.0", 
    ".1.3.6.1.2.1.32.3.6.5.0", 
};
const char *oidAlarmNumberR4[] = {
    ".1.3.6.1.2.1.32.4.6.1.0", 
    ".1.3.6.1.2.1.32.4.6.2.0", 
    ".1.3.6.1.2.1.32.4.6.3.0", 
    ".1.3.6.1.2.1.32.4.6.4.0", 
    ".1.3.6.1.2.1.32.4.6.5.0", 
};
const char *oidAlarmNumberR5[] = {
    ".1.3.6.1.2.1.32.5.6.1.0", 
    ".1.3.6.1.2.1.32.5.6.2.0", 
    ".1.3.6.1.2.1.32.5.6.3.0", 
    ".1.3.6.1.2.1.32.5.6.4.0", 
    ".1.3.6.1.2.1.32.5.6.5.0", 
};
const char *oidAlarmNumberR6[] = {
    ".1.3.6.1.2.1.32.6.6.1.0", 
    ".1.3.6.1.2.1.32.6.6.2.0", 
    ".1.3.6.1.2.1.32.6.6.3.0", 
    ".1.3.6.1.2.1.32.6.6.4.0", 
    ".1.3.6.1.2.1.32.6.6.5.0", 
};
const char *oidAlarmNumberR7[] = {
    ".1.3.6.1.2.1.32.7.6.1.0", 
    ".1.3.6.1.2.1.32.7.6.2.0", 
    ".1.3.6.1.2.1.32.7.6.3.0", 
    ".1.3.6.1.2.1.32.7.6.4.0", 
    ".1.3.6.1.2.1.32.7.6.5.0", 
};
const char *oidAlarmNumberR8[] = {
    ".1.3.6.1.2.1.32.8.6.1.0", 
    ".1.3.6.1.2.1.32.8.6.2.0", 
    ".1.3.6.1.2.1.32.8.6.3.0", 
    ".1.3.6.1.2.1.32.8.6.4.0", 
    ".1.3.6.1.2.1.32.8.6.5.0", 
};

const char *oidPeriodCounter[] = {
    ".1.3.6.1.2.1.32.1.7.0", 
    ".1.3.6.1.2.1.32.2.7.0", 
    ".1.3.6.1.2.1.32.3.7.0", 
    ".1.3.6.1.2.1.32.4.7.0", 
    ".1.3.6.1.2.1.32.5.7.0", 
    ".1.3.6.1.2.1.32.6.7.0", 
    ".1.3.6.1.2.1.32.7.7.0", 
    ".1.3.6.1.2.1.32.8.7.0", 
};
const char *oidPackVoltage[] = {
    ".1.3.6.1.2.1.32.1.8.0", 
    ".1.3.6.1.2.1.32.2.8.0", 
    ".1.3.6.1.2.1.32.3.8.0", 
    ".1.3.6.1.2.1.32.4.8.0", 
    ".1.3.6.1.2.1.32.5.8.0", 
    ".1.3.6.1.2.1.32.6.8.0", 
    ".1.3.6.1.2.1.32.7.8.0", 
    ".1.3.6.1.2.1.32.8.8.0", 
};
const char *oidPackSOH[] = {
    ".1.3.6.1.2.1.32.1.9.0", 
    ".1.3.6.1.2.1.32.2.9.0", 
    ".1.3.6.1.2.1.32.3.9.0", 
    ".1.3.6.1.2.1.32.4.9.0", 
    ".1.3.6.1.2.1.32.5.9.0", 
    ".1.3.6.1.2.1.32.6.9.0", 
    ".1.3.6.1.2.1.32.7.9.0", 
    ".1.3.6.1.2.1.32.8.9.0", 
};
const char *oidPackProtected[] = {
    ".1.3.6.1.2.1.32.1.10.0", 
    ".1.3.6.1.2.1.32.2.10.0", 
    ".1.3.6.1.2.1.32.3.10.0", 
    ".1.3.6.1.2.1.32.4.10.0", 
    ".1.3.6.1.2.1.32.5.10.0", 
    ".1.3.6.1.2.1.32.6.10.0", 
    ".1.3.6.1.2.1.32.7.10.0", 
    ".1.3.6.1.2.1.32.8.10.0", 
};




//************************************
//* Function declarations            *
//************************************
void addRFC1213MIBHandler();
void addENTITYMIBHandler();
void addENTITYSENSORMIBHandler();
int getUptime();
bool loadSNMPValues();
bool saveSNMPValues();
int readFakeSensor();
uint64_t uptimeMillis();
void printFile(const char *filename);
//************************************

std::string sysDescr = "SNMP Agent";
std::string sysObjectID = "";
uint32_t sysUptime = 0;
char sysContactValue[255];
char *sysContact = sysContactValue;
char sysNameValue[255];
char *sysName = sysNameValue;
char sysLocationValue[255];
char *sysLocation = sysLocationValue;
int sysServices = 65; // Physical and Application
char kepChargeModeValue[255];
char *kepChargeMode = kepChargeModeValue;

void snmpRequest(void *parameter);
#endif




// const char oidIFTBatVoltageR1_1[] =       ".1.3.6.1.2.1.32.1.1.1.0";                     // 0
// const char oidIFTBatVoltageR1_2[] =       ".1.3.6.1.2.1.32.1.1.2.0";                     // 0
// const char oidIFTBatVoltageR1_3[] =       ".1.3.6.1.2.1.32.1.1.3.0";                     // 0
// const char oidIFTBatVoltageR1_4[] =       ".1.3.6.1.2.1.32.1.1.4.0";                     // 0
// const char oidIFTBatVoltageR1_5[] =       ".1.3.6.1.2.1.32.1.1.5.0";                     // 0
// const char oidIFTBatVoltageR1_6[] =       ".1.3.6.1.2.1.32.1.1.6.0";                     // 0
// const char oidIFTBatVoltageR1_7[] =       ".1.3.6.1.2.1.32.1.1.7.0";                     // 0
// const char oidIFTBatVoltageR1_8[] =       ".1.3.6.1.2.1.32.1.1.8.0";                     // 0
// const char oidIFTBatVoltageR1_9[] =       ".1.3.6.1.2.1.32.1.1.9.0";                     // 0
// const char oidIFTBatVoltageR1_10[] =       ".1.3.6.1.2.1.32.1.1.10.0";                     // 0
// const char oidIFTBatVoltageR1_11[] =       ".1.3.6.1.2.1.32.1.1.11.0";                     // 0
// const char oidIFTBatVoltageR1_12[] =       ".1.3.6.1.2.1.32.1.1.12.0";                     // 0
// const char oidIFTBatVoltageR1_13[] =       ".1.3.6.1.2.1.32.1.1.13.0";                     // 0
// const char oidIFTBatVoltageR1_14[] =       ".1.3.6.1.2.1.32.1.1.14.0";                     // 0
// const char oidIFTBatVoltageR1_15[] =       ".1.3.6.1.2.1.32.1.1.15.0";

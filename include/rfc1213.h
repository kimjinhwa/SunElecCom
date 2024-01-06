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

// IFTECH UPS MIB
char oidIFTYear_made[] = ".1.3.6.1.2.1.32.1.1.1.0";                     // 0
char oidIFTMonth_made[] = ".1.3.6.1.2.1.32.1.1.2.0";                    // 1
char oidIFTDate_made[] = ".1.3.6.1.2.1.32.1.1.3.0";                     // 2
char oidIFTUps_Capacity[] = ".1.3.6.1.2.1.32.1.1.4.0";                  // 3
char oidIFTInput_Phase[] = ".1.3.6.1.2.1.32.1.1.5.0";                   // 4
char oidIFTInput_Voltage[] = ".1.3.6.1.2.1.32.1.1.6.0";                 // 5
char oidIFTOutput_Phase[] = ".1.3.6.1.2.1.32.1.1.7.0";                  // 6
char oidIFTOutput_Voltage[] = ".1.3.6.1.2.1.32.1.1.8.0";                // 7
char oidIFTCompany_code_And_upstype[] = ".1.3.6.1.2.1.32.1.1.9.0";      // 8
char oidIFTInstalled_Battery_Cells[] = ".1.3.6.1.2.1.32.1.1.10.0";      // 9
char oidIFTreserved_2[] = ".1.3.6.1.2.1.32.1.1.11.0";                   // 10
char oidIFTBMS_1_2_STATE[] = ".1.3.6.1.2.1.32.1.1.12.0";                // 11
char oidIFTConverter_State[] = ".1.3.6.1.2.1.32.1.1.13.0";              // 12
char oidIFTInverter_State[] = ".1.3.6.1.2.1.32.1.1.14.0";               // 13
char oidIFTConverter_Operation_Fault[] = ".1.3.6.1.2.1.32.1.1.15.0";    // 14
char oidIFTInverter_Operation_Fault[] = ".1.3.6.1.2.1.32.1.1.16.0";     // 15
char oidIFTInput_r_volt_rms[] = ".1.3.6.1.2.1.32.1.1.17.0";             // 16  trnms->1
char oidIFTInput_s_volt_rms[] = ".1.3.6.1.2.1.32.1.1.18.0";             // 17  trnms->2
char oidIFTInput_t_volt_rms[] = ".1.3.6.1.2.1.32.1.1.19.0";             // 18  trnms->3
char oidIFTInput_r_current_rms[] = ".1.3.6.1.2.1.32.1.1.20.0";          // 19
char oidIFTInput_s_current_rms[] = ".1.3.6.1.2.1.32.1.1.21.0";          // 20
char oidIFTInput_t_current_rms[] = ".1.3.6.1.2.1.32.1.1.22.0";          // 21
char oidIFTInput_frequency[] = ".1.3.6.1.2.1.32.1.1.23.0";              // 22
char oidIFTBypass_r_volt_rms[] = ".1.3.6.1.2.1.32.1.1.24.0";            // 23
char oidIFTBypass_s_volt_rms[] = ".1.3.6.1.2.1.32.1.1.25.0";            // 24
char oidIFTBypass_t_volt_rms[] = ".1.3.6.1.2.1.32.1.1.26.0";            // 25
char oidIFTBypass_r_current_rms[] = ".1.3.6.1.2.1.32.1.1.27.0";         // 26
char oidIFTBypass_s_current_rms[] = ".1.3.6.1.2.1.32.1.1.28.0";         // 27
char oidIFTBypass_t_current_rms[] = ".1.3.6.1.2.1.32.1.1.29.0";         // 28
char oidIFTBypass_Frequency[] = ".1.3.6.1.2.1.32.1.1.30.0";             // 29
char oidIFTInverter_u_volt_rms[] = ".1.3.6.1.2.1.32.1.1.31.0";          // 30
char oidIFTInverter_v_volt_rms[] = ".1.3.6.1.2.1.32.1.1.32.0";          // 31
char oidIFTInverter_w_volt_rms[] = ".1.3.6.1.2.1.32.1.1.33.0";          // 32
char oidIFTInverter_U_curr_rms[] = ".1.3.6.1.2.1.32.1.1.34.0";          // 33
char oidIFTInverter_V_curr_rms[] = ".1.3.6.1.2.1.32.1.1.35.0";          // 34
char oidIFTInverter_W_curr_rms[] = ".1.3.6.1.2.1.32.1.1.36.0";          // 35
char oidIFTInverter_Frequency[] = ".1.3.6.1.2.1.32.1.1.37.0";           // 36
char oidIFTBat_volt_rms[] = ".1.3.6.1.2.1.32.1.1.38.0";                 // 37
char oidIFTBat_current_rms[] = ".1.3.6.1.2.1.32.1.1.39.0";              // 38
char oidIFTInput_kva_address_KVA[] = ".1.3.6.1.2.1.32.1.1.40.0";        // 39
char oidIFTInput_kw_KW[] = ".1.3.6.1.2.1.32.1.1.41.0";                  // 40
char oidIFTInput_kvar_KVAR[] = ".1.3.6.1.2.1.32.1.1.42.0";              // 41
char oidIFTInput_power_factor_Pf[] = ".1.3.6.1.2.1.32.1.1.43.0";        // 42
char oidIFTOutput_r_volt_rms[] = ".1.3.6.1.2.1.32.1.1.44.0";            // 43
char oidIFTOutput_s_volt_rms[] = ".1.3.6.1.2.1.32.1.1.45.0";            // 44
char oidIFTOutput_t_volt_rms[] = ".1.3.6.1.2.1.32.1.1.46.0";            // 45
char oidIFTOutput_u_current_rms[] = ".1.3.6.1.2.1.32.1.1.47.0";         // 46
char oidIFTOutput_v_current_rms[] = ".1.3.6.1.2.1.32.1.1.48.0";         // 47
char oidIFTOutput_w_current_rms[] = ".1.3.6.1.2.1.32.1.1.49.0";         // 48
char oidIFTOutput_frequency[] = ".1.3.6.1.2.1.32.1.1.50.0";             // 49
char oidIFTOutput_kva_KVA[] = ".1.3.6.1.2.1.32.1.1.51.0";               // 50
char oidIFTOutput_kw_KW[] = ".1.3.6.1.2.1.32.1.1.52.0";                 // 51
char oidIFTOutput_kvar_KVAR[] = ".1.3.6.1.2.1.32.1.1.53.0";             // 52
char oidIFTOutput_Power_factor_Pf[] = ".1.3.6.1.2.1.32.1.1.54.0";       // 53
char oidIFTOutput_R_Load[] = ".1.3.6.1.2.1.32.1.1.55.0";                // 54
char oidIFTOutput_S_Load[] = ".1.3.6.1.2.1.32.1.1.56.0";                // 55
char oidIFTOutput_T_Load[] = ".1.3.6.1.2.1.32.1.1.57.0";                // 56
char oidIFTBMS_Bat_Voltage[] = ".1.3.6.1.2.1.32.1.1.58.0";              // 57
char oidIFTBattery_Room_Temper[] = ".1.3.6.1.2.1.32.1.1.59.0";          // 58
char oidIFTreserved_3_batteryTimeRemain[] = ".1.3.6.1.2.1.32.1.1.60.0"; // 59 
//char oidIFTreboot[] = ".1.3.6.1.2.1.32.1.1.61.0";                       // 61 

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
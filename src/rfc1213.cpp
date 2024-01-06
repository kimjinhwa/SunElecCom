#include <Arduino.h>
#include <stdio.h>
#include <ETH.h>
#include <Ethernet.h>
#include <Wifi.h>
#include <WiFiClient.h>
#include "SNMP_Agent.h"
#include "mainGrobaldef.h"
#include "rfc1213.h"
#include "ModbusBridgeWiFi.h"
#include "ModbusClientRTU.h"
#include "upstype.h"
// extern SNMPAgent snmp;
//************************************
//* Initialise                       *
//************************************
//  Global Variables
ups_modbus_data_t ups_modbus_data;

static const unsigned long SENSOR_UPDATE_INTERVAL = 5000; // ms = 5 Seconds
//************************************
SNMPAgent snmp = SNMPAgent(rocommunity, rwcommunity); // Creates an SMMPAgent instance
const char *savedValuesFile = "/spiffs/snmp.json";

static const unsigned long UPTIME_UPDATE_INTERVAL = 1000; // ms = 1 second
// static unsigned long lastUptimeUpdateTime = 0;
LED_BLINK LED_STATUS;
void setLedStatus(LED_BLINK status)
{
    LED_STATUS = status;
}
void snmpLoop(int job)
{
    unsigned long now;
    unsigned long lastLedTime = 0;
    uint8_t ledStatus = 0;
    int onoffTime_1[] = {1000, 1000};
    int onoffTime_2[] = {1000 / 4, 1000 / 4, 1000 / 4, 1000 / 4, 1000, 1000};
    int onoffTime_3[] = {1000 / 4, 1000 / 4};
    if (ETH.linkUp())
        setLedStatus(LINKED);
    int pos = 0;
    int ledOnOffTime = 1000;
    LED_STATUS = APMODE;
    while (1)
    {
        snmp.loop();
        if (snmp.setOccurred)
        {
            Serial.println("A Set event has occured.");
            saveSNMPValues(); // Store the values
            snmp.resetSetOccurred();
        }
        if (millis() - now >= UPTIME_UPDATE_INTERVAL)
        {
            now = millis();
            sysUptime = getUptime();
        }
        vTaskDelay(5);
    }
    // if ((millis() - lastLedTime > ledOnOffTime))
    // {
    //     if (LED_STATUS == LINKED)
    //     {
    //         ledOnOffTime = onoffTime_1[pos];
    //         pos++;
    //         if (pos >= sizeof(onoffTime_1) / sizeof(onoffTime_1[0]))
    //             pos = 0;
    //     }
    //     if (LED_STATUS == APMODE)
    //     {
    //         ledOnOffTime = onoffTime_2[pos];
    //         pos++;
    //         if (pos >= sizeof(onoffTime_2) / sizeof(onoffTime_2[0]))
    //             pos = 0;
    //     }
    //     digitalWrite(33, ledStatus = !ledStatus);
    //     lastLedTime = now;
    // }
}
SNMPAgent *getSnmpPointer()
{
    return &snmp;
}
void addRFC1213MIBHandler()
{
    // Add SNMP Handlers of correct type to each OID
    snmp.addReadOnlyStaticStringHandler(oidSysDescr, sysDescr);
    snmp.addReadOnlyStaticStringHandler(oidSysObjectID, sysObjectID);
    snmp.addIntegerHandler(oidSysServices, &sysServices);
    snmp.addTimestampHandler(oidSysUptime, &sysUptime);
    // Add Settable Handlers
    snmp.addReadWriteStringHandler(oidSysContact, &sysContact, 25, true);
    snmp.addReadWriteStringHandler(oidSysName, &sysName, 25, true);
    snmp.addReadWriteStringHandler(oidSysLocation, &sysLocation, 25, true);
}
void addENTITYMIBHandler()
{
    snmp.addIntegerHandler(oidentPhysicalIndex_1, &entPhysicalIndex_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalDescr_1, entPhysicalDescr_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalVendorType_1, entPhysicalVendorType_1);
    snmp.addIntegerHandler(oidentPhysicalContainedIn_1, &entPhysicalContainedIn_1);
    snmp.addIntegerHandler(oidentPhysicalClass_1, &entPhysicalClass_1);
    snmp.addIntegerHandler(oidentPhysicalParentRelPos_1, &entPhysicalParentRelPos_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalName_1, entPhysicalName_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalHardwareRev_1, entPhysicalHardwareRev_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalFirmwareRev_1, entPhysicalFirmwareRev_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalSoftwareRev_1, entPhysicalSoftwareRev_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalSerialNum_1, entPhysicalSerialNum_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalMfgName_1, entPhysicalMfgName_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalModelName_1, entPhysicalModelName_11);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalAlias_1, entPhysicalAlias_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalAssetID_1, entPhysicalAssetID_1);
    snmp.addIntegerHandler(oidentPhysicalIsFRU_1, &entPhysicalIsFRU_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalMfgDate_1, entPhysicalMfgDate_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhysicalUris_1, entPhysicalUris_1);
    // snmp.addReadOnlyStaticStringHandler(oidRfc1213_endof1361, entPhysicalUris_1);
}

void addENTITYIFTUPSHandler()
{
    snmp.addIntegerHandler(oidIFTYear_made, (int *)&ups_modbus_data.Year_made);
    snmp.addIntegerHandler(oidIFTMonth_made, (int *)&ups_modbus_data.Month_made);
    snmp.addIntegerHandler(oidIFTDate_made, (int *)&ups_modbus_data.Date_made);
    snmp.addIntegerHandler(oidIFTUps_Capacity, (int *)&ups_modbus_data.Ups_Capacity, true);
    snmp.addIntegerHandler(oidIFTInput_Phase, (int *)&ups_modbus_data.Input_Phase);
    snmp.addIntegerHandler(oidIFTInput_Voltage, (int *)&ups_modbus_data.Input_Voltage);
    snmp.addIntegerHandler(oidIFTOutput_Phase, (int *)&ups_modbus_data.Output_Phase);
    snmp.addIntegerHandler(oidIFTOutput_Voltage, (int *)&ups_modbus_data.Output_Voltage);
    snmp.addIntegerHandler(oidIFTCompany_code_And_upstype, (int *)&ups_modbus_data.Company_code_And_upstype);
    snmp.addIntegerHandler(oidIFTInstalled_Battery_Cells, (int *)&ups_modbus_data.Installed_Battery_Cells, true);
    snmp.addIntegerHandler(oidIFTreserved_2, (int *)&ups_modbus_data.reserved_2);
    snmp.addIntegerHandler(oidIFTBMS_1_2_STATE, (int *)&ups_modbus_data.BMS_1_2_STATE);
    snmp.addIntegerHandler(oidIFTConverter_State, (int *)&ups_modbus_data.Converter_State);
    snmp.addIntegerHandler(oidIFTInverter_State, (int *)&ups_modbus_data.Inverter_State);
    snmp.addIntegerHandler(oidIFTConverter_Operation_Fault, (int *)&ups_modbus_data.Inverter_Operation_Fault);
    snmp.addIntegerHandler(oidIFTInverter_Operation_Fault, (int *)&ups_modbus_data.Inverter_Operation_Fault);
    snmp.addIntegerHandler(oidIFTInput_r_volt_rms, (int *)&ups_modbus_data.Input_r_volt_rms);
    snmp.addIntegerHandler(oidIFTInput_s_volt_rms, (int *)&ups_modbus_data.Input_s_volt_rms);
    snmp.addIntegerHandler(oidIFTInput_t_volt_rms, (int *)&ups_modbus_data.Input_t_volt_rms);
    snmp.addIntegerHandler(oidIFTInput_r_current_rms, (int *)&ups_modbus_data.Input_r_current_rms);
    snmp.addIntegerHandler(oidIFTInput_s_current_rms, (int *)&ups_modbus_data.Input_s_current_rms);
    snmp.addIntegerHandler(oidIFTInput_t_current_rms, (int *)&ups_modbus_data.Input_t_current_rms);
    snmp.addIntegerHandler(oidIFTInput_frequency, (int *)&ups_modbus_data.Input_frequency);
    snmp.addIntegerHandler(oidIFTBypass_r_volt_rms, (int *)&ups_modbus_data.Bypass_r_volt_rms);
    snmp.addIntegerHandler(oidIFTBypass_s_volt_rms, (int *)&ups_modbus_data.Bypass_s_volt_rms);
    snmp.addIntegerHandler(oidIFTBypass_t_volt_rms, (int *)&ups_modbus_data.Bypass_t_volt_rms);
    snmp.addIntegerHandler(oidIFTBypass_r_current_rms, (int *)&ups_modbus_data.Bypass_r_current_rms);
    snmp.addIntegerHandler(oidIFTBypass_s_current_rms, (int *)&ups_modbus_data.Bypass_s_current_rms);
    snmp.addIntegerHandler(oidIFTBypass_t_current_rms, (int *)&ups_modbus_data.Bypass_t_current_rms);
    snmp.addIntegerHandler(oidIFTBypass_Frequency, (int *)&ups_modbus_data.Bypass_Frequency);
    snmp.addIntegerHandler(oidIFTInverter_u_volt_rms, (int *)&ups_modbus_data.Inverter_u_volt_rms);
    snmp.addIntegerHandler(oidIFTInverter_v_volt_rms, (int *)&ups_modbus_data.Inverter_v_volt_rms);
    snmp.addIntegerHandler(oidIFTInverter_w_volt_rms, (int *)&ups_modbus_data.Inverter_w_volt_rms);
    snmp.addIntegerHandler(oidIFTInverter_U_curr_rms, (int *)&ups_modbus_data.Inverter_U_curr_rms);
    snmp.addIntegerHandler(oidIFTInverter_V_curr_rms, (int *)&ups_modbus_data.Inverter_V_curr_rms);
    snmp.addIntegerHandler(oidIFTInverter_W_curr_rms, (int *)&ups_modbus_data.Inverter_W_curr_rms);
    snmp.addIntegerHandler(oidIFTInverter_Frequency, (int *)&ups_modbus_data.Inverter_Frequency);
    snmp.addIntegerHandler(oidIFTBat_volt_rms, (int *)&ups_modbus_data.Bat_volt_rms);
    snmp.addIntegerHandler(oidIFTBat_current_rms, (int *)&ups_modbus_data.Bat_current_rms);
    snmp.addIntegerHandler(oidIFTInput_kva_address_KVA, (int *)&ups_modbus_data.Input_kva_address_KVA);
    snmp.addIntegerHandler(oidIFTInput_kw_KW, (int *)&ups_modbus_data.Input_kw_KW);
    snmp.addIntegerHandler(oidIFTInput_kvar_KVAR, (int *)&ups_modbus_data.Input_kvar_KVAR);
    snmp.addIntegerHandler(oidIFTInput_power_factor_Pf, (int *)&ups_modbus_data.Input_power_factor_Pf);
    snmp.addIntegerHandler(oidIFTOutput_r_volt_rms, (int *)&ups_modbus_data.Output_r_volt_rms);
    snmp.addIntegerHandler(oidIFTOutput_s_volt_rms, (int *)&ups_modbus_data.Output_s_volt_rms);
    snmp.addIntegerHandler(oidIFTOutput_t_volt_rms, (int *)&ups_modbus_data.Output_t_volt_rms);
    snmp.addIntegerHandler(oidIFTOutput_u_current_rms, (int *)&ups_modbus_data.Output_u_current_rms);
    snmp.addIntegerHandler(oidIFTOutput_v_current_rms, (int *)&ups_modbus_data.Output_v_current_rms);
    snmp.addIntegerHandler(oidIFTOutput_w_current_rms, (int *)&ups_modbus_data.Output_w_current_rms);
    snmp.addIntegerHandler(oidIFTOutput_frequency, (int *)&ups_modbus_data.Output_frequency);
    snmp.addIntegerHandler(oidIFTOutput_kva_KVA, (int *)&ups_modbus_data.Output_kva_KVA);
    snmp.addIntegerHandler(oidIFTOutput_kw_KW, (int *)&ups_modbus_data.Output_kw_KW);
    snmp.addIntegerHandler(oidIFTOutput_kvar_KVAR, (int *)&ups_modbus_data.Output_kvar_KVAR);
    snmp.addIntegerHandler(oidIFTOutput_Power_factor_Pf, (int *)&ups_modbus_data.Output_Power_factor_Pf);
    snmp.addIntegerHandler(oidIFTOutput_R_Load, (int *)&ups_modbus_data.Output_R_Load);
    snmp.addIntegerHandler(oidIFTOutput_S_Load, (int *)&ups_modbus_data.Output_S_Load);
    snmp.addIntegerHandler(oidIFTOutput_T_Load, (int *)&ups_modbus_data.Output_T_Load);
    snmp.addIntegerHandler(oidIFTBMS_Bat_Voltage, (int *)&ups_modbus_data.BMS_Bat_Voltage);
    snmp.addIntegerHandler(oidIFTBattery_Room_Temper, (int *)&ups_modbus_data.Battery_Room_Temper);
    snmp.addIntegerHandler(oidIFTreserved_3_batteryTimeRemain, (int *)&ups_modbus_data.reserved_3_batteryTimeRemain);
}
kep_value_st kep_value;
void addENTITYKEPHandler()
{
    snmp.addIntegerHandler(oidKEPinputVoltage_R, &kep_value.inputVoltage_R);
    snmp.addIntegerHandler(oidKEPinputVoltage_S, &kep_value.inputVoltage_S);
    snmp.addIntegerHandler(oidKEPinputVoltage_T, &kep_value.inputVoltage_T);
    snmp.addIntegerHandler(oidKEPoutputVoltage, &kep_value.outputVoltage);
    snmp.addIntegerHandler(oidKEPoutputCurrent, &kep_value.outputCurrent);
    snmp.addIntegerHandler(oidKEPbatteryVoltage, &kep_value.batteryVoltage);
    snmp.addIntegerHandler(oidKEPchargeCurrent, &kep_value.chargeCurrent);
    // snmp.addIntegerHandler(oidKEPchargeMode, &kep_value.chargeMode);
    //&kep_value.chargeMode
    snmp.addReadWriteStringHandler(oidKEPchargeMode, &kepChargeMode, 10, true);

    snmp.addIntegerHandler(oidKEPoutputFrequency, &kep_value.outputFrequency);

    snmp.addIntegerHandler(oidKEPpowerFail, &kep_value.powerFail);
    snmp.addIntegerHandler(oidKEPpowerFailRecovery, &kep_value.powerFailFailRecovery);
    snmp.addIntegerHandler(oidKEPinputpowerFail, &kep_value.inputpowerFail);
    snmp.addIntegerHandler(oidKEPinputpowerFailRecovery, &kep_value.inputpowerFailRecovery);

    snmp.addIntegerHandler(oidKEPmodule1, &kep_value.module1);
    snmp.addIntegerHandler(oidKEPmodule1Recover, &kep_value.module1Recover);
    snmp.addIntegerHandler(oidKEPmodule2, &kep_value.module2);
    snmp.addIntegerHandler(oidKEPmodule2Recover, &kep_value.module2Recover);
    snmp.addIntegerHandler(oidKEPmodule3, &kep_value.module3);
    snmp.addIntegerHandler(oidKEPmodule3Recover, &kep_value.module3Recover);
    snmp.addIntegerHandler(oidKEPmodule4, &kep_value.module4);
    snmp.addIntegerHandler(oidKEPmodule4Recover, &kep_value.module4Recover);
    snmp.addIntegerHandler(oidKEPmoduleEND, &kep_value.module4Recover);
}
void addENTITYSENSORMIBHandler()
{
    entPhySensorValueUpdateRate_1 = SENSOR_UPDATE_INTERVAL;

    snmp.addIntegerHandler(oidentPhySensorType_1, &entPhySensorType_1);
    snmp.addIntegerHandler(oidentPhySensorScale_1, &entPhySensorScale_1);
    snmp.addIntegerHandler(oidentPhySensorPrecision_1, &entPhySensorPrecision_1);
    snmp.addIntegerHandler(oidentPhySensorValue_1, &entPhySensorValue_1);
    snmp.addIntegerHandler(oidentPhySensorOperStatus_1, &entPhySensorOperStatus_1);
    snmp.addReadOnlyStaticStringHandler(oidentPhySensorUnitsDisplay_1, entPhySensorUnitsDisplay_1);
    snmp.addTimestampHandler(oidentPhySensorValueTimeStamp_1, &entPhySensorValueTimeStamp_1);
    snmp.addIntegerHandler(oidentPhySensorValueUpdateRate_1, &entPhySensorValueUpdateRate_1);
}
void printFile(const char *filename)
{
    // Open file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        Serial.println(F("Failed to read file"));
        return;
    }
    Serial.println("SNMP saved values file: ");
    // Extract each characters by one by one
    while (!feof(file))
    {
        Serial.print((char)fgetc(file));
    }
    Serial.println();
    fclose(file);
}
bool loadSNMPValues()
{
    struct stat sb;
    int statok;
    statok = stat(savedValuesFile, &sb);
    FILE *file = fopen(savedValuesFile, "r");
    if (file == NULL)
    {
        Serial.println(F("Failed to read saved values file"));
        return false;
    }
    if (sb.st_size > 1024)
    {
        Serial.print(F("Stored SNMP values file too large"));
        fclose(file);
        return false;
    }
    StaticJsonDocument<1024> doc;
    // Deserialize the JSON document
    char rData[1024];
    int nRead = fread((byte *)rData, sb.st_size, 1, file);
    DeserializationError error = deserializeJson(doc, rData);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        fclose(file);
        return false;
    }
    // Fetch values
    strlcpy(sysContact, doc["sysContact"], sizeof(sysContactValue));
    strlcpy(sysName, doc["sysName"], sizeof(sysNameValue));
    strlcpy(sysLocation, doc["sysLocation"], sizeof(sysLocationValue));
    ups_modbus_data.Ups_Capacity = doc["upsCapacity"];
    ups_modbus_data.Installed_Battery_Cells = doc["BatteryCells"]; // BatteryCells 값을 읽어옵니다.
    ups_modbus_data.Input_Phase = doc["upstype"];
    ups_modbus_data.Input_Phase = (int)(ups_modbus_data.Input_Phase / 10);
    ups_modbus_data.Output_Phase = ups_modbus_data.Input_Phase % 10;
    fclose(file);
    return true;
}
bool saveSNMPValues()
{

    FILE *file = fopen(savedValuesFile, "w");
    if (file == NULL)
    {
        Serial.println(F("Failed to open saved values file for writing"));
        return false;
    }
    StaticJsonDocument<1024> doc;
    // Store the values in the JSON document
    if (sysContact == NULL || strlen(sysContact) <= 0)
        strcpy(sysContact, "IFTECH");
    if (sysName == NULL || strlen(sysName) <= 0)
        strcpy(sysName, "UPS 3P1P");
    if (sysLocation == NULL || strlen(sysLocation) <= 0)
        strcpy(sysLocation, "FACTORY");
    if (ups_modbus_data.Ups_Capacity == 0)
        ups_modbus_data.Ups_Capacity = 30; // default
    if (ups_modbus_data.Installed_Battery_Cells == 0)
        ups_modbus_data.Installed_Battery_Cells = 16; // default
    if (ups_modbus_data.Input_Phase == 0)
        ups_modbus_data.Input_Phase = 3;
    if (ups_modbus_data.Output_Phase == 0)
        ups_modbus_data.Output_Phase = 1;
    doc["sysContact"] = sysContact;
    doc["sysName"] = sysName;
    doc["sysLocation"] = sysLocation;
    doc["upsCapacity"] = ups_modbus_data.Ups_Capacity;
    doc["BatteryCells"] = ups_modbus_data.Installed_Battery_Cells; // BatteryCells를 JSON에 추가합니다.
    doc["upstype"] = ups_modbus_data.Input_Phase * 10 + ups_modbus_data.Output_Phase;
    // Serialize JSON to file
    char buf[1024];
    if (serializeJson(doc, (char *)&buf, 1024) == 0)
    {
        Serial.println(F("Failed to save values to file"));
        fclose(file);
        return false;
    }
    Serial.println(buf);
    fwrite(buf, 1, strlen(buf), file);
    fclose(file);
    printFile(savedValuesFile);
    return true;
}
uint64_t uptimeMillis()
{
    return (esp_timer_get_time() / 1000);
}
int getUptime()
{
    return (int)(uptimeMillis() / 10); // Convert milliseconds to timeticks (hundredths of a second)
}
int readFakeSensor()
{
    int min = -50;
    int max = 100;
    return min + esp_random() % ((max + 1) - min);
}
extern WiFiUDP udp;
void snmpSetup()
{
    snmp.setUDP(&udp);
    snmp.begin();
    addRFC1213MIBHandler();      // RFC1213-MIB (System)
    addENTITYMIBHandler();       // ENTITY-MIB
    addENTITYSENSORMIBHandler(); // ENTITY-SENSOR-MIB
    addENTITYKEPHandler();       // Korea Electric Power
    addENTITYIFTUPSHandler();
    snmp.sortHandlers();

    // Read previously stored values, if any.
    if (loadSNMPValues())
    {
        Serial.println(F("Loaded stored values"));
        printFile(savedValuesFile);
    }
    else
    {
        Serial.println(F("File was created\n"));
        saveSNMPValues(); // Store the values
        printFile(savedValuesFile);
    }
}

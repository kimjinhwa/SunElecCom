#include <Arduino.h>
#include "upstype.h"
#include <bitset>
/*1P and 3P */
/*
 1  	  Q1     	  Status Inquiry
 2  	  T     	  10 Seconds Test
 3  	  TL    	  Test until Battery Low
 4  	  T<n>  	  Test for Specified Time Period
 5  	  Q     	  Turn On/Off beep
 6  	  S<n>  	  Shut Down Command
 7  	S<n>R<m>	  Shut Down and Restore Command
 8  	  C     	  Cancel Shut Down Command
 9  	  CT    	  Cancel Test Command
10	   I	  UPS Information Command
11	   F	  UPS Rating Information
*/
/*3P Command */
/*
2 G1 UPS real time data
3 G2 UPS status inquiry
4 G3 UPS real time data for 3 phase
6 GF UPS Rating Information
*/
#define SET_TIMEOUT 300
static const unsigned long UPTIME_UPDATE_INTERVAL = 1000; // ms = 1 second

static unsigned long lastUptimeUpdateTime = 0;

GF_UpsRatingInfo_t GF_UpsRatingInfo;
megatechUpsInfo_t megatechUpsInfo;
F_status_t F_status;
Q1_status_t Q1_status;
// 함수를 이용하여 숫자로 변환하는 유틸리티 함수
float extractFloatValue(const String &str)
{
    return str.toFloat();
}

G1_realtimedata_t G1_realtimedata;

G2_UpsStatus_t G2_UpsStatus;

G3_3P_realtimeData_t G3_3P_realtimeData;

int extractIntValue(const String &str)
{
    return str.toInt();
}
char cr = 0x0d;

extern ups_modbus_data_t ups_modbus_data;

void selectPrintf(uint sel, const char *format, ...);
//        3   3    4     5     5    4    4    4
// UPS :!SSS PPP NNNN RRR.R +TT.T FF.F EE.E QQ.Q <cr>
//     !240 094 0123 025.0 +35.0 60.1 62.0 60.0<cr>
int G1_Command()
{
    //selectPrintf(2, "%s", "G1\n\r");
    if (ups_modbus_data.Input_Phase != 3)
        return 0;
    Serial.print("G1\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    if (readString.length() == 0)
        return 0;
    //selectPrintf(2, "%s", readString.c_str());
    strlcpy(G1_realtimedata.receiveData, readString.c_str(), sizeof(G1_realtimedata.receiveData));
    // 문자열 분리 및 숫자 값 추출
    int pos1 = 1, pos2 = 0; // # 다음의 인덱스부터 시작
    // 3
    pos1 = pos2 + 1;
    pos2 = pos1 + 3;
    String S_b = readString.substring(pos1, pos2);
    G1_realtimedata.batteryVoltage = extractFloatValue(S_b);

    // 3   3
    pos1 = pos2 + 1;
    pos2 = pos1 + 3;
    String S_c = readString.substring(pos1, pos2);
    G1_realtimedata.batteryCapacityPercentage = S_c.toInt();

    // 3   3    4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_d = readString.substring(pos1, pos2);
    G1_realtimedata.batteryTimeRemaining = S_d.toInt();

    // 3   3    4     5
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    String S_e = readString.substring(pos1, pos2);
    G1_realtimedata.batteryCurrent = extractFloatValue(S_e);

    // 3   3    4     5     5
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    String S_f = readString.substring(pos1, pos2);
    G1_realtimedata.temperature = extractFloatValue(S_f);

    // 3   3    4     5     5    4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_g = readString.substring(pos1, pos2);
    G1_realtimedata.inputFrequency = extractFloatValue(S_g);

    // 3   3    4     5     5    4    4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_h = readString.substring(pos1, pos2);
    G1_realtimedata.frequencyOfBypass = extractFloatValue(S_h);

    // 3   3    4     5     5    4    4    4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_i = readString.substring(pos1, pos2);
    G1_realtimedata.outputFrequency = extractFloatValue(S_i);

    return 1;
}
// UPS :!a7a6a5a4a3a2a1a0 b7b6b5b4b3b2b1b0 c7c6c5c4c3c2c1c0<cr>
int G2_Command()
{
    //selectPrintf(2, "%s", "G2\n\r");
    if (ups_modbus_data.Input_Phase != 3)
        return 0;
    Serial.print("G2\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    if (readString.length() == 0)
        return 0;

    //selectPrintf(2, "\n\rG2 %s", readString.c_str());
    strlcpy(G2_UpsStatus.receiveData, readString.c_str(), sizeof(G2_UpsStatus.receiveData));
    int pos1, pos2 = 0; // ! 다음의 인덱스부터 시작

    pos1 = pos2 + 1;
    pos2 = pos1 + 8;
    String S_a = readString.substring(pos1, pos2);

    pos1 = pos2 + 1;
    pos2 = pos1 + 8;
    String S_b = readString.substring(pos1, pos2);

    pos1 = pos2 + 1;
    pos2 = pos1 + 8;
    String S_c = readString.substring(pos1, pos2);

    std::string s = S_a.c_str(); // s = S_i.c_str();
    std::bitset<8> bitset_data(s);
    G2_UpsStatus.RectifierStatus = bitset_data.to_ulong();
    s = S_b.c_str();
    bitset_data = std::bitset<8>(s);
    G2_UpsStatus.upsStatus = bitset_data.to_ulong();
    s = S_c.c_str();
    bitset_data = std::bitset<8>(s);
    G2_UpsStatus.InverterFaultCondition = bitset_data.to_ulong();
    return 1;
}
//          5     5     5     5     5     5     5     5     5     5     5     5
// UPS :!NNN.N/NNN.N/NNN.N PPP.P/PPP.P/PPP.P QQQ.Q/QQQ.Q/QQQ.Q SSS.S/SSS.S/SSS.S<cr>
// UPS :!222.0/222.0/222.0 221.0/221.0/221.0 220.0/220.0/220.0 014.0/015.0/014.0<cr>

extern kep_value_st kep_value;
extern char *kepChargeMode;
int G3_Command()
{
    //selectPrintf(2, "%s", "G3\n\r");
    if (ups_modbus_data.Input_Phase != 3)
        return 0;
    Serial.print("G3\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    if (readString.length() == 0)
        return 0;
    //selectPrintf(2, "\n\rG3 %s", readString.c_str());
    strlcpy(G3_3P_realtimeData.receiveData, readString.c_str(), sizeof(G3_3P_realtimeData.receiveData));
    // 문자열 분리 및 값 추출
    int pos1, pos2 = 0; // ! 다음의 인덱스부터 시작

    // I/P voltage of R/S/T 3 phases
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.inputVolatege_R = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.inputVolatege_S = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.inputVolatege_T = extractFloatValue(readString.substring(pos1, pos2));
    // Bypass AC source voltage of R/S/T 3 phases
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.bypassVolatege_R = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.bypassVolatege_S = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.bypassVolatege_T = extractFloatValue(readString.substring(pos1, pos2));
    // O/P voltage of R/S/T 3 phases
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.outputVolatege_R = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.outputVolatege_S = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.outputVolatege_T = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    // Load percentage of R/S/T 3 phases
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.lordPercentage_R = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.lordPercentage_S = extractFloatValue(readString.substring(pos1, pos2));
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    G3_3P_realtimeData.lordPercentage_T = extractFloatValue(readString.substring(pos1, pos2));

    return 1;
}
// UPS : !220V/380V^3P4W 060 220V/380V^3P4W 061 220V/3P3W^^^^^ 060 396 150KVA^^^^<cr>
int GF_Command()
{
    //selectPrintf(2, "%s", "GF\n\r");
    if (ups_modbus_data.Input_Phase != 3)
        return 0;
    Serial.print("GF\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    if (readString.length() == 0)
        return 0;
    //
    //selectPrintf(2, "\n\rGF %s", readString.c_str());
    strlcpy(GF_UpsRatingInfo.receiveData, readString.c_str(), sizeof(GF_UpsRatingInfo.receiveData));
    int startpos = 1; // # 다음의 인덱스부터 시작
    int endpos;
    String sub;

    /*a.14 For example : 220V 3P3W, 220V/380V 3P4W*/
    startpos = 1;
    endpos = startpos + 14;
    sub = readString.substring(startpos, endpos);
    strlcpy(GF_UpsRatingInfo.Rect_Volt, sub.c_str(), 15);
    /*b.3 For example : 060 */
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 3;
    sub = readString.substring(startpos, endpos);
    GF_UpsRatingInfo.Rectifier_Frequency = sub.toInt();
    /*c 3 For example : 220V 3P3W, 220V/380V 3P4W*/
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 14;
    sub = readString.substring(startpos, endpos);
    if (sub.length() >= 14)
    {
        strlcpy(GF_UpsRatingInfo.Bpss_Volt, sub.c_str(), 15);
    }
    /*d 3 For example : 060 */
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 3;
    sub = readString.substring(startpos, endpos);
    if (sub.length() >= 3)
    {
        GF_UpsRatingInfo.Bypass_Frequency = sub.toInt();
    }
    /*e 14 For example : 220V 3P3W, 220V/380V 3P4W*/
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 14;
    sub = readString.substring(startpos, endpos);
    if (sub.length() >= 14)
    {
        strlcpy(GF_UpsRatingInfo.OP_Volt, sub.c_str(), 15);
    }
    /*f 3 For example : 061 */
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 3;
    sub = readString.substring(startpos, endpos);
    if (sub.length() >= 3)
    {
        GF_UpsRatingInfo.output_Frequency = sub.toInt();
    }
    /*g 3 For example :  */
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 3;
    sub = readString.substring(startpos, endpos);
    if (sub.length() >= 3)
    {
        GF_UpsRatingInfo.battery_voltage = sub.toInt();
    }
    /*h 10 For example : 220V 3P3W, 220V/380V 3P4W*/
    startpos = endpos + 1; // 공백을 제거하기위하여.
    endpos = startpos + 10;
    sub = readString.substring(startpos, endpos);
    strlcpy(GF_UpsRatingInfo.Power_Rating, sub.c_str(), 11);
    return 1;
}
//      5     5     5   3    4    4    4 8
// (MMM.M NNN.N PPP.P QQQ RR.R S.SS TT.T b7b6b5b4b3b2b1b0<cr>
int Q1_Command()
{
    //selectPrintf(2, "%s", "Q1\n\r");
    Serial.print("Q1\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    //selectPrintf(0, "Q1:%s",readString.c_str() );
    if (readString.length() == 0)
        return 0;
    //selectPrintf(2, "\n\rQ1 %s", readString.c_str());
    //selectPrintf(2, "\n\r");
    // 문자열 분리 및 숫자 값 추출
    strlcpy(Q1_status.receiveData, readString.c_str(), sizeof(Q1_status.receiveData));
    //      5
    int pos1 = 1;        // # 다음의 인덱스부터 시작
    int pos2 = pos1 + 5; // # 다음의 인덱스부터 시작
    String S_b = readString.substring(pos1, pos2);
    //            5
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    String S_c = readString.substring(pos1, pos2);
    //                  5
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;
    String S_d = readString.substring(pos1, pos2);
    //                      3
    pos1 = pos2 + 1;
    pos2 = pos1 + 3;
    String S_e = readString.substring(pos1, pos2);
    //                           4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_f = readString.substring(pos1, pos2);
    //                                4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_g = readString.substring(pos1, pos2);
    //                                     4
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;
    String S_h = readString.substring(pos1, pos2);
    //                                          8
    pos1 = pos2 + 1;
    pos2 = pos1 + 8;
    String S_i = readString.substring(pos1, pos2);
    // 구조체에 값을 할당
    Q1_status.inputVoltage = extractFloatValue(S_b);
    Q1_status.faultVoltage = extractIntValue(S_c);
    Q1_status.outputVoltage = extractFloatValue(S_d);
    Q1_status.outputCurrent = extractFloatValue(S_e);
    Q1_status.inputFrequency = extractFloatValue(S_f);
    Q1_status.batteryVoltage = extractFloatValue(S_g);
    Q1_status.temperature = extractFloatValue(S_h);


    std::string s = S_i.c_str(); // s = S_i.c_str();
    std::bitset<8> bitset_data(s);
    Q1_status.upsStatus = bitset_data.to_ulong();

    G3_3P_realtimeData.inputVolatege_R = Q1_status.inputVoltage;
    G3_3P_realtimeData.inputVolatege_S = Q1_status.inputVoltage;
    G3_3P_realtimeData.inputVolatege_T = Q1_status.inputVoltage;
    G3_3P_realtimeData.outputVolatege_R = Q1_status.outputVoltage;
    G1_realtimedata.batteryVoltage = Q1_status.batteryVoltage;
    G1_realtimedata.outputFrequency = Q1_status.inputFrequency;

    ups_modbus_data.Input_r_volt_rms = Q1_status.inputVoltage;
    ups_modbus_data.Input_s_volt_rms = Q1_status.inputVoltage;
    ups_modbus_data.Input_t_volt_rms = Q1_status.inputVoltage;
    ups_modbus_data.Output_r_volt_rms = Q1_status.outputVoltage ;
    ups_modbus_data.Output_s_volt_rms = Q1_status.outputVoltage ;
    ups_modbus_data.Output_t_volt_rms = Q1_status.outputVoltage ;
    ups_modbus_data.Output_u_current_rms = Q1_status.outputCurrent ;
    ups_modbus_data.Output_v_current_rms = Q1_status.outputCurrent ;
    ups_modbus_data.Output_w_current_rms = Q1_status.outputCurrent ;
    ups_modbus_data.Input_frequency= Q1_status.inputFrequency;
    ups_modbus_data.Bat_volt_rms = Q1_status.batteryVoltage;
    ups_modbus_data.Battery_Room_Temper = Q1_status.temperature ;
    ups_modbus_data.Inverter_State = Q1_status.upsStatus;
    return 1;
}
// Test for specified time period :
// Computer : T<n><cr>
// UPS : Test for <n> minutes.
int T_Command(int minute){
    String readString;
    selectPrintf(2, "T\r");
    Serial.printf("T%02d\r",minute);
    Serial.flush();
    delay(500);
    Serial.setTimeout(SET_TIMEOUT );
    Serial.printf("T%02d\r",minute);
    Serial.flush();
    delay(100);
    readString = Serial.readStringUntil(0x0d);

    if (readString.length() == 0)
        return 0;
    return 1;
}
int I_Command()
{
    //: #Company_Name UPS_Model Version<cr>
    // ups_modbus_data
    selectPrintf(2, "I\r");
    Serial.print("I\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    if (readString.length() == 0)
        return 0;
    selectPrintf(2, "\n\rI %s", readString.c_str());
    selectPrintf(0, "\n\rI %s", readString.c_str());
    strlcpy(megatechUpsInfo.receiveData, readString.c_str(), readString.length());
    // "Company_Name" 추출
    selectPrintf(2, "%s", readString.c_str());
    int pos1 = 1;                                 // readString.indexOf('#');
    int pos2 = pos1 + 15;                         // readString.indexOf(" ", pos1 + 1);
    String S1 = readString.substring(pos1, pos2); // Company Name
    pos1 = pos2 + 1;
    pos2 = pos1 + 10;
    // "UPS_Model" 추출
    String S2 = readString.substring(pos1, pos2);

    pos1 = pos2 + 1;
    pos2 = pos1 + 10;
    // "Version" 추출
    String S3 = readString.substring(pos1, pos2);
    strlcpy(megatechUpsInfo.Company_name, S1.c_str(), sizeof(megatechUpsInfo.Company_name));
    strlcpy(megatechUpsInfo.UPS_Model, S2.c_str(), sizeof(megatechUpsInfo.UPS_Model));
    strlcpy(megatechUpsInfo.Version, S3.c_str(), sizeof(megatechUpsInfo.Version));
    return 1;
}
int F_Command()
{
    // #MMM.M QQQ SS.SS RR.R
    selectPrintf(2, "%s", "F\n\r");
    Serial.print("F\r");
    Serial.flush();
    Serial.setTimeout(SET_TIMEOUT );
    String readString = Serial.readStringUntil(0x0d);
    if (readString.length() == 0)
        return 0;
    selectPrintf(0, "Q1:%s",readString.c_str() );
    selectPrintf(2, "\n\rF %s", readString.c_str());
    // 문자열 분리 및 숫자 값 추출
    int pos1 = 1; // # 다음의 인덱스부터 시작
    int pos2 = pos1 + 5;
    String S1 = readString.substring(pos1, pos2);

    pos1 = pos2 + 1;
    pos2 = pos1 + 3;

    String S2 = readString.substring(pos1, pos2);
    pos1 = pos2 + 1;
    pos2 = pos1 + 5;

    String S3 = readString.substring(pos1, pos2);
    pos1 = pos2 + 1;
    pos2 = pos1 + 4;

    String S4 = readString.substring(pos1, pos2);

    // 구조체에 값을 할당
    strlcpy(F_status.receiveData, readString.c_str(), sizeof(F_status.receiveData));
    F_status.RatingVoltage = extractFloatValue(S1);
    F_status.RatingCurrent = extractIntValue(S2);
    F_status.BatteryVoltage = extractFloatValue(S3);
    F_status.Frequency = extractFloatValue(S4);
    return 1;
}

void setKepSnmpValue()
{
    if (ups_modbus_data.Input_Phase == 3)
    {
        kep_value.inputVoltage_R = G3_3P_realtimeData.inputVolatege_R;
        kep_value.inputVoltage_S = G3_3P_realtimeData.inputVolatege_S;
        kep_value.inputVoltage_T = G3_3P_realtimeData.inputVolatege_T;
        kep_value.outputVoltage = G3_3P_realtimeData.outputVolatege_R;
        // change to current percentage
        //  kep_value.outputCurrent = F_status.RatingCurrent;
        kep_value.outputCurrent = Q1_status.outputCurrent * F_status.RatingCurrent / 100;
        kep_value.batteryVoltage = G1_realtimedata.batteryVoltage;
        kep_value.chargeCurrent = G1_realtimedata.batteryCurrent;
        kep_value.chargeMode = G2_UpsStatus.RectifierStatus & 0x02;
        if (kep_value.chargeMode)
            strlcpy(kepChargeMode, "Equalizing", 11);
        else
            strlcpy(kepChargeMode, "Floating  ", 11);
        kep_value.outputFrequency = G1_realtimedata.outputFrequency;
    }
    else // 단상이면
    {

        kep_value.inputVoltage_R = Q1_status.inputVoltage;
        kep_value.inputVoltage_S = Q1_status.inputVoltage;
        kep_value.inputVoltage_T = Q1_status.inputVoltage;
        kep_value.outputVoltage = Q1_status.outputVoltage;
        kep_value.outputCurrent = Q1_status.outputCurrent * F_status.RatingCurrent / 100;
        // kep_value.batteryVoltage = Q1_status.batteryVoltage * ups_modbus_data.Installed_Battery_Cells;
        // kep_value.batteryVoltage = (int)(Q1_status.batteryVoltage * 13.0);
        kep_value.batteryVoltage = (int)(Q1_status.batteryVoltage * Q1_status.temperature);
        kep_value.chargeCurrent = Q1_status.faultVoltage; // G1_realtimedata.batteryCurrent;
        strlcpy(kepChargeMode, "Floating  ", 11);
        kep_value.outputFrequency = Q1_status.inputFrequency;

        G1_realtimedata.batteryCurrent = Q1_status.outputCurrent * F_status.RatingCurrent / 100;
        G1_realtimedata.batteryVoltage = (int)(Q1_status.batteryVoltage * Q1_status.temperature);
        G1_realtimedata.frequencyOfBypass = Q1_status.inputFrequency;
        G1_realtimedata.inputFrequency = Q1_status.inputFrequency;
        G1_realtimedata.outputFrequency = Q1_status.inputFrequency;

        G3_3P_realtimeData.bypassVolatege_R = Q1_status.inputVoltage;
        G3_3P_realtimeData.bypassVolatege_S = Q1_status.inputVoltage;
        G3_3P_realtimeData.bypassVolatege_T = Q1_status.inputVoltage;
        G3_3P_realtimeData.inputVolatege_R = Q1_status.inputVoltage;
        G3_3P_realtimeData.inputVolatege_S = Q1_status.inputVoltage;
        G3_3P_realtimeData.inputVolatege_T = Q1_status.inputVoltage;
        G3_3P_realtimeData.outputVolatege_R = Q1_status.outputVoltage;
        G3_3P_realtimeData.outputVolatege_S = Q1_status.outputVoltage;
        G3_3P_realtimeData.outputVolatege_T = Q1_status.outputVoltage;
        G3_3P_realtimeData.lordPercentage_R = Q1_status.outputCurrent;
        G3_3P_realtimeData.lordPercentage_S =Q1_status.outputCurrent;
        G3_3P_realtimeData.lordPercentage_T =Q1_status.outputCurrent;
    }
}
#define OPLED_ON false
#define OPLED_OFF true
void megatechRequest(void *parameter)
{
    unsigned long now = 0;
    u_int8_t ledStatus = 0;
    uint8_t upsKind = 3;
    ups_modbus_data.Input_Phase = 1;
    ups_modbus_data.Output_Phase =1;
    
    //digitalWrite(33, OPLED_OFF);
    while ((ledStatus = I_Command()) == 0)
    {
        selectPrintf(0, "Can not read I command\n\r");
        ledStatus = Q1_Command();
        //digitalWrite(33, ledStatus ? OPLED_ON : OPLED_OFF);
        vTaskDelay(500);
    }

    vTaskDelay(200);
    F_Command();
    vTaskDelay(200);
    if (GF_Command() == 0) // 단상인지 삼상인지를 구분한다.
    {
        ups_modbus_data.Input_Phase = 1;
        ups_modbus_data.Output_Phase =1;
    }
    //    upsKind = 1;
    vTaskDelay(200);
    //이상하다 강제로 단상으로 우선은 맞추어 준다. 
    // 교하 제품의 특색인가?
    ups_modbus_data.Input_Phase = 1;
    ups_modbus_data.Output_Phase =1;
    while (1)
    {
        if (millis() - now >= UPTIME_UPDATE_INTERVAL)
        {
            vTaskDelay(100);
            ledStatus = Q1_Command();
            //selectPrintf(0, "readOk Q1_Command\n\r");
            //digitalWrite(33, ledStatus ? OPLED_ON : OPLED_OFF);
            // if (ups_modbus_data.Input_Phase == 3)
            // {
            //     vTaskDelay(100);
            //     G1_Command();
            //     vTaskDelay(100);
            //     G2_Command();
            //     vTaskDelay(100);
            //     G3_Command();
            //     vTaskDelay(100);
            //     selectPrintf(0, "system is 3PxP\n\r");
            // }
            // else 
                //selectPrintf(0, "system is 1P1P\n\r");
            setKepSnmpValue();
            now = millis();
            //digitalWrite(33, OPLED_OFF);
        }
        vTaskDelay(1);
    }
}
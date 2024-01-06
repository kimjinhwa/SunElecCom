#ifndef UPSTYPE_H
#define UPSTYPE_H
typedef struct
{
    uint16_t Year_made;                    // 0
    uint16_t Month_made;                   // 1
    uint16_t Date_made;                    // 2
    uint16_t Ups_Capacity;                 // 3
    uint16_t Input_Phase;                  // 4
    uint16_t Input_Voltage;                // 5
    uint16_t Output_Phase;                 // 6
    uint16_t Output_Voltage;               // 7
    uint16_t Company_code_And_upstype;     // 8
    uint16_t Installed_Battery_Cells;      // 9
    uint16_t reserved_2;                   // 10
    uint16_t BMS_1_2_STATE;                // 11
    uint16_t Converter_State;              // 12
    uint16_t Inverter_State;               // 13
    uint16_t Converter_Operation_Fault;    // 14
    uint16_t Inverter_Operation_Fault;     // 15
    uint16_t Input_r_volt_rms;             // 16  trnms->1
    uint16_t Input_s_volt_rms;             // 17  trnms->2
    uint16_t Input_t_volt_rms;             // 18  trnms->3
    uint16_t Input_r_current_rms;          // 19
    uint16_t Input_s_current_rms;          // 20
    uint16_t Input_t_current_rms;          // 21
    uint16_t Input_frequency;              // 22
    uint16_t Bypass_r_volt_rms;            // 23
    uint16_t Bypass_s_volt_rms;            // 24
    uint16_t Bypass_t_volt_rms;            // 25
    uint16_t Bypass_r_current_rms;         // 26
    uint16_t Bypass_s_current_rms;         // 27
    uint16_t Bypass_t_current_rms;         // 28
    uint16_t Bypass_Frequency;             // 29
    uint16_t Inverter_u_volt_rms;          // 30
    uint16_t Inverter_v_volt_rms;          // 31
    uint16_t Inverter_w_volt_rms;          // 32
    uint16_t Inverter_U_curr_rms;          // 33
    uint16_t Inverter_V_curr_rms;          // 34
    uint16_t Inverter_W_curr_rms;          // 35
    uint16_t Inverter_Frequency;           // 36
    uint16_t Bat_volt_rms;                 // 37
    uint16_t Bat_current_rms;              // 38
    uint16_t Input_kva_address_KVA;        // 39
    uint16_t Input_kw_KW;                  // 40
    uint16_t Input_kvar_KVAR;              // 41
    uint16_t Input_power_factor_Pf;        // 42
    uint16_t Output_r_volt_rms;            // 43
    uint16_t Output_s_volt_rms;            // 44
    uint16_t Output_t_volt_rms;            // 45
    uint16_t Output_u_current_rms;         // 46
    uint16_t Output_v_current_rms;         // 47
    uint16_t Output_w_current_rms;         // 48
    uint16_t Output_frequency;             // 49
    uint16_t Output_kva_KVA;               // 50
    uint16_t Output_kw_KW;                 // 51
    uint16_t Output_kvar_KVAR;             // 52
    uint16_t Output_Power_factor_Pf;       // 53
    uint16_t Output_R_Load;                // 54
    uint16_t Output_S_Load;                // 55
    uint16_t Output_T_Load;                // 56
    uint16_t BMS_Bat_Voltage;              // 57
    uint16_t Battery_Room_Temper;          // 58
    uint16_t reserved_3_batteryTimeRemain; // 59 charging Method Bit(0)
} ups_modbus_data_t;

/*
UPS Information Command:
1234567890123456789012345678901234567890
#MegaTec^^^^^^^^ M1000K^^^^ V001203.12
*/
typedef struct
{
    char receiveData[1+15+1+10+1+10+1+1];
    char Company_name[15+1];
    char UPS_Model[10+1];
    char Version[10+1];
} megatechUpsInfo_t;

/*
123456789012345678901234567890
#MMM.M QQQ SS.SS RR.R
*/
typedef struct
{
    char receiveData[21+1+1];
    float RatingVoltage;
    int RatingCurrent;
    float BatteryVoltage;
    float Frequency;
} F_status_t;
/*
1234567890123456789012345678901234567890123456789
(208.4 140.0 208.4 034 59.9 2.05 35.0 00110000
*/
typedef struct
{
    char receiveData[1+5+1+5+1+5+1+3+1+4+1+4+1+4+1+8+1+1];// 48
    float inputVoltage;
    float faultVoltage;
    float outputVoltage;
    float outputCurrent;
    float inputFrequency;
    float batteryVoltage;
    float temperature;
    byte upsStatus;
} Q1_status_t;
/*
123456789012345678901234567890
!00000010 00000100 00000000
*/
typedef struct
{
    char receiveData[29];
    byte RectifierStatus;
    byte upsStatus;
    byte InverterFaultCondition;
} G2_UpsStatus_t;
/*
12345678901234567890123456789012345678901234567890123456789012345678901234567890
!222.0/222.0/222.0 221.0/221.0/221.0 220.0/220.0/220.0 014.0/015.0/014.0
*/
typedef struct
{
    char receiveData[74];
    float inputVolatege_R;
    float inputVolatege_S;
    float inputVolatege_T;
    float bypassVolatege_R;
    float bypassVolatege_S;
    float bypassVolatege_T;
    float outputVolatege_R;
    float outputVolatege_S;
    float outputVolatege_T;
    float lordPercentage_R;
    float lordPercentage_S;
    float lordPercentage_T;

} G3_3P_realtimeData_t;

/*
123456789012345678901234567890123456789012
!240 094 0123 025.0 +35.0 60.1 62.0 60.0
*/
typedef struct
{
    char receiveData[40+1+1];
    int batteryVoltage;
    int batteryCapacityPercentage;
    int batteryTimeRemaining;
    float batteryCurrent;
    float temperature;
    float inputFrequency;
    float frequencyOfBypass;
    float outputFrequency;
} G1_realtimedata_t;
/*
1234567890123456789012345678901234567890123456789012345678901234567890123456789
!220V/380V^3P4W 060 220V/380V^3P4W 061 220V/3P3W^^^^^ 060 396 150KVA^^^^
*/
typedef struct
{
    char receiveData[72+1+1];//73
    char Rect_Volt[14+1];
    int Rectifier_Frequency;
    char Bpss_Volt[14+1];
    int Bypass_Frequency;
    char OP_Volt[14+1];
    int output_Frequency;
    int battery_voltage;
    char Power_Rating[10+1];
} GF_UpsRatingInfo_t;

typedef struct 
{
    int inputVoltage_R ;
    int inputVoltage_S ;
    int inputVoltage_T ;
    int outputVoltage ;
    int outputCurrent ;
    int batteryVoltage;
    int chargeCurrent ;
    int chargeMode  ;
    int outputFrequency ;
    int powerFail  ;
    int powerFailFailRecovery ;
    int inputpowerFail ;
    int inputpowerFailRecovery ;
    int module1 ;
    int module1Recover ;
    int module2 ;
    int module2Recover ;
    int module3 ;
    int module3Recover ;
    int module4 ;
    int module4Recover ;
}kep_value_st;
#endif
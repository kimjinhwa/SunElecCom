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

#endif
#ifndef MAINGROBALDEF_H
#define MAINGROBALDEF_H

#ifdef ESP_LOGE
#undef ESP_LOGE
#endif
#ifdef ESP_LOGW
#undef ESP_LOGW
#endif
#ifdef ESP_LOGI
#undef ESP_LOGI
#endif
#ifdef ESP_LOGD
#undef ESP_LOGD
#endif
#ifdef ESP_LOGV
#undef ESP_LOGV
#endif

#ifdef log_e
#undef log_e
#endif
#define log_e(format, ...) log_printf_telnet(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__)

#ifdef log_w
#undef log_w
#endif
#define log_w(format, ...) log_printf_telnet(ARDUHAL_LOG_FORMAT(W, format), ##__VA_ARGS__)

#ifdef log_i
#undef log_i
#endif
#define log_i(format, ...) log_printf_telnet(ARDUHAL_LOG_FORMAT(I, format), ##__VA_ARGS__)

#ifdef log_d
#undef log_d
#endif
#define log_d(format, ...) log_printf_telnet(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__)

#ifdef log_v
#undef log_v
#endif
#define log_v(format, ...) log_printf_telnet(ARDUHAL_LOG_FORMAT(V, format), ##__VA_ARGS__)

#define ESP_LOGE(tag, format, ...) log_e("[%s] " format, tag, ##__VA_ARGS__)
#define ESP_LOGW(tag, format, ...) log_w("[%s] " format, tag, ##__VA_ARGS__)
#define ESP_LOGI(tag, format, ...) log_i("[%s] " format, tag, ##__VA_ARGS__)
#define ESP_LOGD(tag, format, ...) log_d("[%s] " format, tag, ##__VA_ARGS__)
#define ESP_LOGV(tag, format, ...) log_v("[%s] " format, tag, ##__VA_ARGS__)

typedef struct
{
  uint32_t IPADDRESS;
  uint32_t GATEWAY;
  uint32_t SUBNETMASK;
  uint32_t WEBSOCKETSERVER;
  uint32_t DNS1;
  uint32_t DNS2;
  uint32_t NTP_1;
  uint32_t NTP_2;
  uint16_t WEBSERVERPORT;
  bool ntpuse;
  uint32_t BAUDRATE;
  uint16_t Q_INTERVAL;

} nvsSystemSet;

enum LED_BLINK
{
  LINKED = 0,
  APMODE = 1,
  NOTLINKED = 2
};
#endif
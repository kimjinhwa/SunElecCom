#include <Arduino.h>
#include <stdio.h>
#include <ETH.h>
#include <Ethernet.h>
#include <Wifi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>
#include <EEPROM.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <esp_heap_caps.h>
#include <WiFiUDP.h>

#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <BluetoothSerial.h>

// for file system
#include <esp_spiffs.h>
#include <dirent.h>
#include <sys/stat.h>

#include <SimpleCLI.h>
#include <mainGrobaldef.h>

#include "../Version.h"
#include "main.h"
#include "megatec.h"
#include "upstype.h"
#include "modbusRtu.h"

// PCA9557PW, 118
#define ETH_PHY_TYPE ETH_PHY_LAN8720
//
// #define ETH_CLK_MODE ETH_CLOCK_GPIO0_IN
#define ETH_POWER_PIN 4
#define ETH_TYPE ETH_PHY_LAN8720
#define ETH_ADDR 1
#define ETH_MDC_PIN 23
#define ETH_MDIO_PIN 18

#define BLUETOOTH 0x02
#define NONE 0x00
// #define MAX_SOCK_NUM 12 cunstom ethernet.h ->8 to 12
#define OP_LED 33

#define USE_SERIAL Serial
#define WEB_PORT 80
#define WEBSOCKET_PORT 81
#define TELNET_PORT 23

modbusRtu rtu485;
int8_t debugFlag = 0;
BluetoothSerial SerialBT;
uint8_t setOutputDirection = 0;

bool isBTAuthenticated = false;

struct UserInfo_s
{
  char userid[20];
  char passwd[20];
};
struct UserInfo_s sUserInfo;
const char *host = "esp32";
// const char *ssid = "iftech";
// const char *password = "iftechadmin";
const long gmtOffset_sec = 9 * 3600;
const int daylightOffset_sec = 3600;

nvsSystemSet ipAddress_struct;

extern ups_modbus_data_t ups_modbus_data;
static char TAG[] = "Main";
StaticJsonDocument<2000> doc_tx;
// StaticJsonDocument<2000> doc_rx;
TaskHandle_t *h_pxSnmp;
TaskHandle_t *h_pxMegatech;
TaskHandle_t *h_pxTelnetTask;
/* setup function */
const char *soft_ap_ssid = "CHAGER_";
const char *soft_ap_password = "87654321";
static int webRequestNo = -1;

SimpleCLI cli;
Command cmd_ls_config;

ThreeWire myWire(15, 32, OP_LED); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
WebSocketsServer webSocket = WebSocketsServer(WEBSOCKET_PORT);
WebServer webServer(WEB_PORT );
// void onBTConnect(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
bool saveSNMPValues();
bool check_useridpass(String userid, String passwd);
bool check_pass(String userid, String passwd);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void readFromFile(String filename);
void readnWriteEEProm();
void printDateTime(const RtcDateTime &dt);
void printLocalTime();
void logfileRead(int32_t iStart, int32_t iEnd);
void sendBufferDataToSocket(uint8_t num);

void snmpRequest(void *parameter);
// void megatechRequest(void *parameter);
int telnetLoginUser(String userId);
int telnetLoginPass(String passwd);
// void logwrite_event();
// void WritHoldeRegister(int address, int len);
int logCount();
int clientReadTimeout(int timeout);
void readFileToWeb(const char *content_type, const char *filename);
void setRtc();
void selectPrintf(uint sel, const char *format, ...);
int log_printf_telnet(const char *fmt, ...);
// typedef int (*vprintf_like_t)(const char *, va_list);
//  EthernetClient Client;
WiFiClient Client;
WiFiServer telnetServer(TELNET_PORT );

IPAddress ipaddress(192, 168, 0, 57);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress dns1(164, 124, 101, 2);
IPAddress dns2(8, 8, 8, 8);
IPAddress websocketserver(192, 168, 0, 57);
IPAddress ntp_1(203, 248, 240, 140);
IPAddress ntp_2(13, 209, 84, 50);

uint16_t webSocketPort = 81;
QueueHandle_t h_queue;
QueueHandle_t h_sendSocketQueue;

int EthLan8720Start();
void readInputSerialBT();
void writeHellowTofile();
void littleFsInit(int bformat);
void SimpleCLISetUp();

String input = "";
String sendString = "";
int isUserLogin = 0;
int isUserPasswd = 0;

// fnmatch defines
#define FNM_NOMATCH 1        // Match failed.
#define FNM_NOESCAPE 0x01    // Disable backslash escaping.
#define FNM_PATHNAME 0x02    // Slash must be matched by slash.
#define FNM_PERIOD 0x04      // Period must be matched by period.
#define FNM_LEADING_DIR 0x08 // Ignore /<tail> after Imatch.
#define FNM_CASEFOLD 0x10    // Case insensitive search.
#define FNM_PREFIX_DIRS 0x20 // Directory prefixes of pattern match too.
#define EOS '\0'
//-----------------------------------------------------------------------
static const char *rangematch(const char *pattern, char test, int flags)
{
  int negate, ok;
  char c, c2;

  /*
   * A bracket expression starting with an unquoted circumflex
   * character produces unspecified results (IEEE 1003.2-1992,
   * 3.13.2).  This implementation treats it like '!', for
   * consistency with the regular expression syntax.
   * J.T. Conklin (conklin@ngai.kaleida.com)
   */
  if ((negate = (*pattern == '!' || *pattern == '^')))
    ++pattern;

  if (flags & FNM_CASEFOLD)
    test = tolower((unsigned char)test);

  for (ok = 0; (c = *pattern++) != ']';)
  {
    if (c == '\\' && !(flags & FNM_NOESCAPE))
      c = *pattern++;
    if (c == EOS)
      return (NULL);

    if (flags & FNM_CASEFOLD)
      c = tolower((unsigned char)c);

    if (*pattern == '-' && (c2 = *(pattern + 1)) != EOS && c2 != ']')
    {
      pattern += 2;
      if (c2 == '\\' && !(flags & FNM_NOESCAPE))
        c2 = *pattern++;
      if (c2 == EOS)
        return (NULL);

      if (flags & FNM_CASEFOLD)
        c2 = tolower((unsigned char)c2);

      if ((unsigned char)c <= (unsigned char)test &&
          (unsigned char)test <= (unsigned char)c2)
        ok = 1;
    }
    else if (c == test)
      ok = 1;
  }
  return (ok == negate ? NULL : pattern);
}
//--------------------------------------------------------------------
static int fnmatch(const char *pattern, const char *string, int flags)
{
  const char *stringstart;
  char c, test;

  for (stringstart = string;;)
    switch (c = *pattern++)
    {
    case EOS:
      if ((flags & FNM_LEADING_DIR) && *string == '/')
        return (0);
      return (*string == EOS ? 0 : FNM_NOMATCH);
    case '?':
      if (*string == EOS)
        return (FNM_NOMATCH);
      if (*string == '/' && (flags & FNM_PATHNAME))
        return (FNM_NOMATCH);
      if (*string == '.' && (flags & FNM_PERIOD) &&
          (string == stringstart ||
           ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
        return (FNM_NOMATCH);
      ++string;
      break;
    case '*':
      c = *pattern;
      // Collapse multiple stars.
      while (c == '*')
        c = *++pattern;

      if (*string == '.' && (flags & FNM_PERIOD) &&
          (string == stringstart ||
           ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
        return (FNM_NOMATCH);

      // Optimize for pattern with * at end or before /.
      if (c == EOS)
        if (flags & FNM_PATHNAME)
          return ((flags & FNM_LEADING_DIR) ||
                          strchr(string, '/') == NULL
                      ? 0
                      : FNM_NOMATCH);
        else
          return (0);
      else if ((c == '/') && (flags & FNM_PATHNAME))
      {
        if ((string = strchr(string, '/')) == NULL)
          return (FNM_NOMATCH);
        break;
      }

      // General case, use recursion.
      while ((test = *string) != EOS)
      {
        if (!fnmatch(pattern, string, flags & ~FNM_PERIOD))
          return (0);
        if ((test == '/') && (flags & FNM_PATHNAME))
          break;
        ++string;
      }
      return (FNM_NOMATCH);
    case '[':
      if (*string == EOS)
        return (FNM_NOMATCH);
      if ((*string == '/') && (flags & FNM_PATHNAME))
        return (FNM_NOMATCH);
      if ((pattern = rangematch(pattern, *string, flags)) == NULL)
        return (FNM_NOMATCH);
      ++string;
      break;
    case '\\':
      if (!(flags & FNM_NOESCAPE))
      {
        if ((c = *pattern++) == EOS)
        {
          c = '\\';
          --pattern;
        }
      }
      break;
      // FALLTHROUGH
    default:
      if (c == *string)
      {
      }
      else if ((flags & FNM_CASEFOLD) && (tolower((unsigned char)c) == tolower((unsigned char)*string)))
      {
      }
      else if ((flags & FNM_PREFIX_DIRS) && *string == EOS && ((c == '/' && string != stringstart) || (string == stringstart + 1 && *stringstart == '/')))
        return (0);
      else
        return (FNM_NOMATCH);
      string++;
      break;
    }
  // NOTREACHED
  return 0;
}
void listDir(const char *path, char *match)
{
  DIR *dir = NULL;
  struct dirent *ent;
  char type;
  char size[9];
  char tpath[255];
  char tbuffer[80];
  struct stat sb;
  struct tm *tm_info;
  char *lpath = NULL;
  int statok;
  selectPrintf(0, "\r\nList of Directory [%s]\r\n", path);
  selectPrintf(0, "-----------------------------------\r\n");
  // Open directory
  dir = opendir(path);
  if (!dir)
  {
    selectPrintf(0, "Error opening directory\r\n");
    return;
  }

  // Read directory entries
  uint64_t total = 0;
  int nfiles = 0;
  selectPrintf(0, "T  Size      Date/Time         Name\r\n");
  selectPrintf(0, "-----------------------------------\r\n");
  while ((ent = readdir(dir)) != NULL)
  {
    sprintf(tpath, path);
    if (path[strlen(path) - 1] != '/')
      strcat(tpath, "/");
    strcat(tpath, ent->d_name);
    tbuffer[0] = '\0';

    if ((match == NULL) || (fnmatch(match, tpath, (FNM_PERIOD)) == 0))
    {
      // Get file stat
      statok = stat(tpath, &sb);

      if (statok == 0)
      {
        tm_info = localtime(&sb.st_mtime);
        strftime(tbuffer, 80, "%d/%m/%Y %R", tm_info);
      }
      else
        sprintf(tbuffer, "                ");

      if (ent->d_type == DT_REG)
      {
        type = 'f';
        nfiles++;
        if (statok)
          strcpy(size, "       ?");
        else
        {
          total += sb.st_size;
          if (sb.st_size < (1024 * 1024))
            sprintf(size, "%8d", (int)sb.st_size);
          else if ((sb.st_size / 1024) < (1024 * 1024))
            sprintf(size, "%6dKB", (int)(sb.st_size / 1024));
          else
            sprintf(size, "%6dMB", (int)(sb.st_size / (1024 * 1024)));
        }
      }
      else
      {
        type = 'd';
        strcpy(size, "       -");
      }

      selectPrintf(0, "%c  %s  %s  %s\r\n",
                   type,
                   size,
                   tbuffer,
                   ent->d_name);
    }
  }
  if (total)
  {
    selectPrintf(0, "-----------------------------------\r\n");
    if (total < (1024 * 1024))
      selectPrintf(0, "   %8d", (int)total);
    else if ((total / 1024) < (1024 * 1024))
      selectPrintf(0, "   %6dKB", (int)(total / 1024));
    else
      selectPrintf(0, "   %6dMB", (int)(total / (1024 * 1024)));
    selectPrintf(0, " in %d file(s)\r\n", nfiles);
  }
  selectPrintf(0, "-----------------------------------\r\n");

  closedir(dir);

  free(lpath);

  uint32_t tot = 0, used = 0;
  esp_spiffs_info(NULL, &tot, &used);
  selectPrintf(0, "SPIFFS: free %d KB of %d KB\r\n", (tot - used) / 1024, tot / 1024);
  selectPrintf(0, "-----------------------------------\r\n");
}
//----------------------------------------------------
static int file_copy(const char *to, const char *from)
{
  FILE *fd_to;
  FILE *fd_from;
  char buf[1024];
  ssize_t nread;
  int saved_errno;

  fd_from = fopen(from, "rb");
  // fd_from = open(from, O_RDONLY);
  if (fd_from == NULL)
    return -1;

  fd_to = fopen(to, "wb");
  if (fd_to == NULL)
    goto out_error;

  while (nread = fread(buf, sizeof(buf), 1, fd_from), nread > 0)
  {
    char *out_ptr = buf;
    ssize_t nwritten;

    do
    {
      nwritten = fwrite(out_ptr, nread, 1, fd_to);

      if (nwritten >= 0)
      {
        nread -= nwritten;
        out_ptr += nwritten;
      }
      else if (errno != EINTR)
        goto out_error;
    } while (nread > 0);
  }

  if (nread == 0)
  {
    if (fclose(fd_to) < 0)
    {
      fd_to = NULL;
      goto out_error;
    }
    fclose(fd_from);

    // Success!
    return 0;
  }

out_error:
  saved_errno = errno;

  fclose(fd_from);
  if (fd_to)
    fclose(fd_to);

  errno = saved_errno;
  return -1;
}
int clientReadTimeout(int timeout)
{ // second
  unsigned long prv_mills = millis();
  unsigned long now = prv_mills;
  while (1)
  {
    now = millis();
    if (now > prv_mills + timeout)
      return -1;
    int c = Client.read();
    if (c == 'Y')
      return 'Y';
    else if (c == 'n')
    {
      selectPrintf(0, "\r\nCanceled...");
      return 'n';
    }
  }
}
void init_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);

  EEPROM.write(0, 0x00);
  EEPROM.commit();
  readnWriteEEProm();
  selectPrintf(0, "\r\nipaddress %s\r\n", IPAddress(ipAddress_struct.IPADDRESS).toString().c_str());
  selectPrintf(0, "gateway %s\r\n", IPAddress(ipAddress_struct.GATEWAY).toString().c_str());
  selectPrintf(0, "subnetmask %s\r\n", IPAddress(ipAddress_struct.SUBNETMASK).toString().c_str());
  selectPrintf(0, "dns1 %s\r\n", IPAddress(ipAddress_struct.DNS1).toString().c_str());
  selectPrintf(0, "dns2 %s\r\n", IPAddress(ipAddress_struct.DNS2).toString().c_str());
  selectPrintf(0, "websocketserver %s\r\n", IPAddress(ipAddress_struct.WEBSOCKETSERVER).toString().c_str());
  selectPrintf(0, "webSocketPort %d\r\n", ipAddress_struct.WEBSERVERPORT);
  selectPrintf(0, "NTP_1 %s\r\n", IPAddress(ipAddress_struct.NTP_1).toString().c_str());
  selectPrintf(0, "NTP_2 %s\r\n", IPAddress(ipAddress_struct.NTP_2).toString().c_str());
  selectPrintf(0, "NTP USE %d\r\n", ipAddress_struct.ntpuse);
  selectPrintf(0, "\r\nbaudrate %d\r\n", ipAddress_struct.BAUDRATE);
  selectPrintf(0, "\r\ninterval %d\r\n", ipAddress_struct.Q_INTERVAL);
  selectPrintf(0, "\r\nInit complete");
}
void quit_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  if (!Client.connected())
    return;
  if (Client.connected())
  {
    Client.stop();
    isUserLogin = 0;
    isUserPasswd = 0;
  }
}
void printLocalTime()
{
  struct tm timeinfo;
  getLocalTime(&timeinfo, 1);
  char strftime_buf[64];
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  selectPrintf(0, "\r\nThe current date/time in is: %s ", strftime_buf);
  selectPrintf(0, "\r\n");
}
void getNtpTime()
{

  // DS1302에서 시간값을 읽어온다.
  RtcDateTime now_rtc = Rtc.GetDateTime();
  // Ds1302 RTC Time is xx.xx.xxxx
  printDateTime(now_rtc);
  // Ntp에서 시간을 읽어온다
  configTime(gmtOffset_sec, daylightOffset_sec, ntp_1.toString().c_str(), ntp_2.toString().c_str());
  time_t now = time(nullptr);
  if (now > 1100000000)
  {
    struct timeval tmv;
    tmv.tv_sec = now;
    tmv.tv_usec = 0;
    // settimeofday(&tmv, NULL); // 웹에서 PC시간으로 설정을 한다.
    selectPrintf(0, "\nNtp Time Get succeed %ld\r\n", now);
    printLocalTime();
  }
  else
  {
    setRtc();
    selectPrintf(0, "Ntp Time Get succeed failed.\r\n");
    printLocalTime();
  }
}
void time_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  bool if_modified = false;
  getNtpTime();

  EEPROM.readBytes(1, (byte *)&ipAddress_struct, sizeof(nvsSystemSet));
  String strValue;
  Argument strArg = cmd.getArgument("ntpuse");
  strValue = strArg.getValue();
  if (strValue.toInt() != 255 && (strValue.toInt() == 0 || strValue.toInt() == 1))
  {
    selectPrintf(0, "ntp use status is %d\r\n", strValue.toInt());
    ipAddress_struct.ntpuse = strValue.toInt();
    if_modified = true;
  }

  strArg = cmd.getArgument("ntp1");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong NTP1 Ip address %s", strValue);
      doc_tx["error"] = "Error Wrong Ip ntp1";
      return;
    }
    else
    {
      ntp_1.fromString(strValue);
      ipAddress_struct.NTP_1 = (uint32_t)ntp_1;
      if_modified = true;
    }
  }
  strArg = cmd.getArgument("ntp2");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong NTP2 Ip address %s", strValue);
      doc_tx["error"] = "Error Wrong Ip ntp2";
      return;
    }
    else
    {
      ntp_2.fromString(strValue);
      ipAddress_struct.NTP_2 = (uint32_t)ntp_2;
      if_modified = true;
    }
  }

  if (ipAddress_struct.ntpuse)
    getNtpTime();

  if (if_modified)
  {

    EEPROM.writeBytes(1, (const byte *)&ipAddress_struct, sizeof(nvsSystemSet));
    EEPROM.commit();
  }

  doc_tx["ntpuse"] = ipAddress_struct.ntpuse;
  doc_tx["ntp1"] = IPAddress(ipAddress_struct.NTP_1).toString();
  doc_tx["ntp2"] = IPAddress(ipAddress_struct.NTP_2).toString();
}
void ntptime_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  readnWriteEEProm();
  selectPrintf(0, "IPADDRESS %s\r\n", IPAddress(ipAddress_struct.IPADDRESS).toString());
  selectPrintf(0, "GATEWAY %s\r\n", IPAddress(ipAddress_struct.GATEWAY).toString());
  selectPrintf(0, "SUBNETMASK %s\r\n", IPAddress(ipAddress_struct.SUBNETMASK).toString());
  selectPrintf(0, "WEBSOCKETSERVER %s\r\n", IPAddress(ipAddress_struct.WEBSOCKETSERVER).toString());
  selectPrintf(0, "DNS1 %s\r\n", IPAddress(ipAddress_struct.DNS1).toString());
  selectPrintf(0, "DNS2 %s\r\n", IPAddress(ipAddress_struct.DNS2).toString());
  selectPrintf(0, "NTP_1 %s\r\n", ntp_1.toString().c_str());
  selectPrintf(0, "NTP_2 %s\r\n", ntp_2.toString().c_str());
  selectPrintf(0, "WEBSERVERPORT %d\r\n", ipAddress_struct.WEBSERVERPORT);
  selectPrintf(0, "ntpuse %d\r\n", ipAddress_struct.ntpuse);
  configTime(0, 0, ntp_1.toString().c_str(), ntp_2.toString().c_str());
  // Wait for time to be set, or use RTC time if NTP server is not available
  time_t now = time(nullptr);
  // while (now < 1100000000)
  {
    delay(1000);
    now = time(nullptr);
    selectPrintf(0, "\n\rRTC time set to system time %ld", now);
    if (now < 1100000000)
    {
      // Use RTC time if NTP server is not available and RTC is write-protected
      struct timeval tv;
      gettimeofday(&tv, nullptr);
      // rtc.SetDateTime(tv.tv_sec);
      selectPrintf(0, "\n\rRTC time set to system time");
      // break;
    }
  }
  if (now >= 1100000000)
  {
    // Set the RTC time to the system time
    // rtc.SetDateTime(now);
    doc_tx["message"] = "RTC time was syncronized with NTP time"; // + String(chp);
    selectPrintf(0, "\n\rRTC time was syncronized with NTP time");
  }
  else
  {
    doc_tx["message"] = "NTP server not available, check ntp server ipaddress time"; // + String(chp);
    selectPrintf(0, "\n\rNTP server not available, check ntp server ipaddress time");
  }
}
bool check_pass(String passwd)
{
  FILE *fp;
  fp = fopen("/spiffs/user.dat", "r");
  if (fp == NULL)
  {
    fp = fopen("/spiffs/user.dat", "w");
    if (fp)
    {
      strcpy(sUserInfo.userid, "admin");
      strcpy(sUserInfo.passwd, "admin");
      fwrite((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
      fclose(fp);
    }
    fp = fopen("/spiffs/user.dat", "r");
    if (fp == NULL)
    {
      selectPrintf(0, "User infor File create fail!\r\n");
      return 0;
    }
  }
  // Now we got file pointer ;
  int nRead = fread((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
  bool ret;
  if (passwd.equals(sUserInfo.passwd))
    ret = true;
  else
    ret = false;
  fclose(fp);
  return ret;
}
bool check_useridpass(String userid, String passwd)
{
  FILE *fp;
  fp = fopen("/spiffs/user.dat", "r");
  if (fp == NULL)
  {
    fp = fopen("/spiffs/user.dat", "w");
    if (fp)
    {
      strcpy(sUserInfo.userid, "admin");
      strcpy(sUserInfo.passwd, "admin");
      fwrite((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
      fclose(fp);
    }
    fp = fopen("/spiffs/user.dat", "r");
    if (fp == NULL)
    {
      selectPrintf(0, "User infor File create fail!\r\n");
      return 0;
    }
  }
  // Now we got file pointer ;
  int nRead = fread((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
  bool ret;
  if (userid.equals(sUserInfo.userid) && passwd.equals(sUserInfo.passwd))
    ret = true;
  else
    ret = false;
  fclose(fp);
  return ret;
}

void common_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String strName[12];
  String strValue[12];
  Argument strArg;
  int ArgCount = cmd.countArgs();
  if (ArgCount > 12)
  {
    doc_tx["Error"] = "Too many Argument(max 11)";
  }
  for (int i = 0; i < ArgCount; i++)
  {
    Argument strArg = cmd.getArgument("userid");
    strName[i] = strArg.getName();
    strValue[i] = strArg.getValue();
  }

  // doc_tx["userid"] = sUserInfo.userid;
  // doc_tx["passwd"] = sUserInfo.passwd;
  return;
}
extern megatechUpsInfo_t megatechUpsInfo;
void I_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = megatechUpsInfo.receiveData;
  retString += "\r\n";
  retString += megatechUpsInfo.Company_name;
  retString += "\r\n";
  retString += megatechUpsInfo.UPS_Model;
  retString += "\r\n";
  retString += megatechUpsInfo.Version;
  retString += "\r\n";
  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}

// typedef struct
// {
//     char receiveData[21+1+1];
//     float RatingVoltage;
//     int RatingCurrent;
//     float BatteryVoltage;
//     float Frequency;
// } F_status_t;
extern F_status_t F_status;
void F_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = F_status.receiveData;
  retString += "\r\n";
  retString += F_status.RatingVoltage;
  retString += "\r\n";
  retString += F_status.RatingCurrent;
  retString += "\r\n";
  retString += F_status.BatteryVoltage;
  retString += "\r\n";
  retString += F_status.Frequency;
  retString += "\r\n";
  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}

extern GF_UpsRatingInfo_t GF_UpsRatingInfo;
void GF_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = GF_UpsRatingInfo.receiveData;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.Rect_Volt;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.Rectifier_Frequency;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.Bpss_Volt;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.Bypass_Frequency;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.OP_Volt;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.output_Frequency;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.battery_voltage;
  retString += "\r\n";
  retString += GF_UpsRatingInfo.Power_Rating;

  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}

extern Q1_status_t Q1_status;
void Q1_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = Q1_status.receiveData;
  retString += "\r\n";
  retString += Q1_status.inputVoltage;
  retString += "\r\n";
  retString += Q1_status.faultVoltage;
  retString += "\r\n";
  retString += Q1_status.outputVoltage;
  retString += "\r\n";
  retString += Q1_status.outputCurrent;
  retString += "\r\n";
  retString += Q1_status.inputFrequency;
  retString += "\r\n";
  retString += Q1_status.batteryVoltage;
  retString += "\r\n";
  retString += Q1_status.temperature;
  retString += "\r\n";
  retString += Q1_status.upsStatus;
  retString += "\r\n";

  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}

extern G1_realtimedata_t G1_realtimedata;
void G1_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = G1_realtimedata.receiveData;
  retString += "\r\n";
  retString += G1_realtimedata.batteryVoltage;
  retString += "\r\n";
  retString += G1_realtimedata.batteryCapacityPercentage;
  retString += "\r\n";
  retString += G1_realtimedata.batteryTimeRemaining;
  retString += "\r\n";
  retString += G1_realtimedata.batteryCurrent;
  retString += "\r\n";
  retString += G1_realtimedata.temperature;
  retString += "\r\n";
  retString += G1_realtimedata.inputFrequency;
  retString += "\r\n";
  retString += G1_realtimedata.frequencyOfBypass;
  retString += "\r\n";
  retString += G1_realtimedata.outputFrequency;
  retString += "\r\n";
  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}

extern G2_UpsStatus_t G2_UpsStatus;
void G2_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = G2_UpsStatus.receiveData;
  retString += "\r\n";
  retString += G2_UpsStatus.RectifierStatus;
  retString += "\r\n";
  retString += G2_UpsStatus.upsStatus;
  retString += "\r\n";
  retString += G2_UpsStatus.InverterFaultCondition;
  retString += "\r\n";

  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}
extern G3_3P_realtimeData_t G3_3P_realtimeData;
void G3_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  String retString = G3_3P_realtimeData.receiveData;
  retString += "\r\n";
  retString += G3_3P_realtimeData.inputVolatege_R;
  retString += "\r\n";
  retString += G3_3P_realtimeData.inputVolatege_S;
  retString += "\r\n";
  retString += G3_3P_realtimeData.inputVolatege_T;
  retString += "\r\n";
  retString += G3_3P_realtimeData.bypassVolatege_R;
  retString += "\r\n";
  retString += G3_3P_realtimeData.bypassVolatege_S;
  retString += "\r\n";
  retString += G3_3P_realtimeData.bypassVolatege_T;
  retString += "\r\n";
  retString += G3_3P_realtimeData.outputVolatege_R;
  retString += "\r\n";
  retString += G3_3P_realtimeData.outputVolatege_S;
  retString += "\r\n";
  retString += G3_3P_realtimeData.outputVolatege_T;
  retString += "\r\n";
  retString += G3_3P_realtimeData.lordPercentage_R;
  retString += "\r\n";
  retString += G3_3P_realtimeData.lordPercentage_S;
  retString += "\r\n";
  retString += G3_3P_realtimeData.lordPercentage_T;
  retString += "\r\n";

  selectPrintf(0, retString.c_str());
  doc_tx["response"] = retString;
  return;
}

void user_configCallback(cmd *cmdPtr)
{
  FILE *fp;
  Command cmd(cmdPtr);
  String userid, passwd;
  Argument strArg = cmd.getArgument("userid");
  userid = strArg.getValue();
  strArg = cmd.getArgument("passwd");
  passwd = strArg.getValue();
  doc_tx["command_type"] = cmd.getName(); // + String(chp);
  fp = fopen("/spiffs/user.dat", "r");
  if (fp == NULL)
  {
    fp = fopen("/spiffs/user.dat", "w");
    if (fp)
    {
      strcpy(sUserInfo.userid, "admin");
      strcpy(sUserInfo.passwd, "admin");
      fwrite((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
      fclose(fp);
      selectPrintf(0, "\r\ndefault user and file created try again: %s %s\r\n", sUserInfo.userid, sUserInfo.passwd);
      return;
    }
    else
    {
      selectPrintf(0, "File point fail!\r\n");
      return;
    }
    fp = fopen("/spiffs/user.dat", "r");
  };
  int nRead = fread((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
  selectPrintf(0, "\r\nRead  : %d\r\n", nRead);
  if (userid.length() == 0 && passwd.length() == 0)
  {
    selectPrintf(0, "\r\nexgist user : %s %s\r\n", sUserInfo.userid, sUserInfo.passwd);
    doc_tx["userid"] = sUserInfo.userid;
    doc_tx["passwd"] = sUserInfo.passwd;
    fclose(fp);
    return;
  }
  fclose(fp);
  fp = fopen("/spiffs/user.dat", "w");
  if (userid.length() > 0)
    strcpy(sUserInfo.userid, userid.c_str());
  if (passwd.length() > 0)
    strcpy(sUserInfo.passwd, passwd.c_str());
  if (fp)
  {
    fwrite((byte *)&sUserInfo, sizeof(sUserInfo), 1, fp);
    selectPrintf(0, "\r\nChanged user : %s %s\r\n", sUserInfo.userid, sUserInfo.passwd);
    doc_tx["userid"] = sUserInfo.userid;
    doc_tx["passwd"] = sUserInfo.passwd;
    fclose(fp);
  }
  return;
}
void ls_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  if (webRequestNo == 1)
  {
    doc_tx["command_type"] = cmd.getName(); // + String(chp);
    doc_tx["reply"] = " Command Succeed";
  }
  listDir("/spiffs/", NULL);
}

void rm_configCallback(cmd *cmdPtr)
{
  DIR *dir = NULL;
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  selectPrintf(0, "\r\n");

  if (argVal.length() == 0)
    return;

  if (!argVal.startsWith("*"))
  {
    argVal = String("/spiffs/") + argVal;
    if (unlink(argVal.c_str()) == -1)
      selectPrintf(0, "Faild to delete %s\r\n", argVal.c_str());
    else
      selectPrintf(0, "File deleted %s\r\n", argVal.c_str());
    return;
  }

  // argVal = String("/spiffs/") + argVal;
  //
  dir = opendir("/spiffs/");
  if (!dir)
  {
    selectPrintf(0, "Error opening directory\r\n");
    return;
  }
  struct dirent *entry;
  argVal.replace("*", ".");
  argVal.replace("..", ".");
  if (!argVal.startsWith("."))
  {
    argVal = "." + argVal;
  }
  size_t ext_len = argVal.length();

  while ((entry = readdir(dir)) != NULL)
  {
    if (entry->d_type == DT_REG && strlen(entry->d_name) > ext_len &&
        strcmp(entry->d_name + strlen(entry->d_name) - ext_len, argVal.c_str()) == 0)
    {
      String filePath = "/spiffs/" + String(entry->d_name);
      if (unlink(filePath.c_str()) != 0)
      {
      }
      selectPrintf(0, "deleted file %s\r\n", filePath.c_str());
    }
  }
}

//
int log_printf_telnet(const char *fmt, ...)
{
  if (!(Client && Client.connected()))
    return 0;
  va_list args;
  va_start(args, fmt);
  size_t length = vsnprintf(NULL, 0, fmt, args) + 1;
  length = length > 1024 ? length : 1024;
  char *buffer = (char *)malloc(length);
  if (buffer == NULL)
  {
    Serial.println("Error: Failed to allocate memory for buffer");
    va_end(args); // Clean up the variable arguments
    return 0;
  }
  vsnprintf(buffer, length, fmt, args);
  // if (Client && Client.connected())
  Client.printf(buffer);
  free(buffer);
  va_end(args);
  return 0;
}
void selectPrintf(uint sel, const char *format, ...)
{
  va_list args;
  va_start(args, format);
  size_t length = vsnprintf(NULL, 0, format, args) + 1;
  length = length > 1024 ? length : 1024;
  char *buffer = (char *)malloc(length);
  if (buffer == NULL)
  {
    if (SerialBT.hasClient())
      SerialBT.print(buffer);
    va_end(args); // Clean up the variable arguments
    return;
  }
  vsnprintf(buffer, length, format, args);

  if (sel == 2)
  {
    if (debugFlag)
    {
      if (Client && Client.connected())
        Client.printf(buffer);
      if (SerialBT.hasClient())
        SerialBT.print(buffer);
    }
  }
  else
  {
    if (Client && Client.connected())
      Client.printf(buffer);
    if (SerialBT.hasClient())
      SerialBT.print(buffer);
  }
  free(buffer);
  va_end(args);
}
void readFromFile(String filename)
{
  FILE *f;
  f = fopen(filename.c_str(), "r");
  if (f == NULL)
  {
    selectPrintf(0, "Failed to open file for reading\r\n");
    return;
  }
  char line[64];
  while (fgets(line, sizeof(line), f))
  {
    selectPrintf(0, "%s", line);
  }
  selectPrintf(0, "\r\n");

  fclose(f);
}
void ver_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  selectPrintf(0, "\r\nFirmWareVersion is %s \r\n", version);
}
void reboot_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  selectPrintf(0, "\r\nNow System Rebooting...\r\n");
  esp_restart();
}

void upstype_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  //

  if (argVal.length() == 0)
  {
    selectPrintf(0, "\nUPS TYPE : %d\r\n", ups_modbus_data.Input_Phase * 10 + ups_modbus_data.Output_Phase);
    return;
  }
  int utype = argVal.toInt();
  switch (utype)
  {
  case 11:
    ups_modbus_data.Input_Phase = 1;
    ups_modbus_data.Output_Phase = 1;
    break;
  case 31:
    ups_modbus_data.Input_Phase = 3;
    ups_modbus_data.Output_Phase = 1;
    break;
  case 33:
    ups_modbus_data.Input_Phase = 3;
    ups_modbus_data.Output_Phase = 3;
    break;
  default:
    selectPrintf(0, "\nWrong UPS TYPE , Set to 1P1P");
    ups_modbus_data.Input_Phase = 1;
    ups_modbus_data.Output_Phase = 1;
    break;
  }
  saveSNMPValues();
  return;
}

void debug_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();

  if (argVal.length() == 0)
    return;
  if (argVal.equals("1"))
    debugFlag = 1;
  else
    debugFlag = 0;
  selectPrintf(0, "\r\nDebug Mode Set to %s \r\n", debugFlag ? "ON" : "OFF");
}
void format_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  selectPrintf(0, "\r\nWould you system formating(Y/n)...\r\n");
  while (1)
  {
    int c = Client.read();
    if (c == 'Y')
    {
      littleFsInit(1);
      selectPrintf(0, "\r\nSystem format completed\r\n");
      return;
    }
    if (c == 'n')
      return;
  }
}
void cat_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  selectPrintf(0, "\r\n");

  if (argVal.length() == 0)
    return;
  argVal = String("/spiffs/") + argVal;

  readFromFile(argVal);
}
// void del_configCallback(cmd *cmdPtr)
// {
//   Command cmd(cmdPtr);
//   selectPrintf(cmd.getName().c_str());
//   selectPrintf(" command done\r\n");
// }
void mv_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  selectPrintf(0, cmd.getName().c_str());
  selectPrintf(0, " command done\r\n");
}

void ip_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  doc_tx["command_type"] = cmd.getName(); // + String(chp);

  String strValue;

  readnWriteEEProm();

  Argument strArg = cmd.getArgument("set");

  if (!strArg.isSet())
  {
    selectPrintf(0, "\r\nipaddress %s", ipaddress.toString().c_str());
    selectPrintf(0, "\r\ngateway %s", gateway.toString().c_str());
    selectPrintf(0, "\r\nsubnetmask %s", subnetmask.toString().c_str());
    selectPrintf(0, "\r\ndns1 %s", dns1.toString().c_str());
    selectPrintf(0, "\r\ndns2 %s", dns2.toString().c_str());
    selectPrintf(0, "\r\nNTP_1 %s", ntp_1.toString().c_str());
    selectPrintf(0, "\r\nNTP_2 %s", ntp_2.toString().c_str());
    selectPrintf(0, "\r\nwebsocketserver %s", websocketserver.toString().c_str());
    selectPrintf(0, "\r\nwebSocketPort %d\r\n", webSocketPort);
    selectPrintf(0, "\r\nbaudrate %d\r\n", ipAddress_struct.BAUDRATE);
    selectPrintf(0, "\r\ninterval %d\r\n", ipAddress_struct.Q_INTERVAL);
    selectPrintf(0, "\r\nver %s", version);

    doc_tx["ipaddress"] = ipaddress.toString();
    doc_tx["gateway"] = gateway.toString();
    doc_tx["subnetmask"] = subnetmask.toString();
    doc_tx["dns1"] = dns1.toString();
    doc_tx["dns2"] = dns2.toString();
    doc_tx["ntp1"] = ntp_1.toString();
    doc_tx["ntp2"] = ntp_2.toString();
    doc_tx["websocketserver"] = websocketserver.toString();
    doc_tx["webSocketPort"] = webSocketPort;
    doc_tx["ntpuse"] = ipAddress_struct.ntpuse;
    doc_tx["baudrate"] = ipAddress_struct.BAUDRATE;
    doc_tx["interval"] = ipAddress_struct.Q_INTERVAL;
    doc_tx["ver"] = version;

    return;
  }
  // Now
  bool if_modified = false;
  strArg = cmd.getArgument("ipaddr");
  strValue = strArg.getValue();
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong Ip address %s", strValue);
      doc_tx["error"] = "Error Wrong Ipaddress ";
      return;
    }
    else
    {
      ipaddress.fromString(strValue);
      ipAddress_struct.IPADDRESS = (uint32_t)ipaddress;
      if_modified = true;
    }
  }

  strArg = cmd.getArgument("gateway");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong gateway %s", strValue);
      doc_tx["error"] = "Error Wrong gateway";
      return;
    }
    else
    {
      gateway.fromString(strValue);
      ipAddress_struct.GATEWAY = (uint32_t)gateway;
      if_modified = true;
    }
  }
  strArg = cmd.getArgument("subnetmask");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong subnetmask %s", strValue);
      doc_tx["error"] = "Error Wrong subnetmask";
      return;
    }
    else
    {
      subnetmask.fromString(strValue);
      ipAddress_struct.SUBNETMASK = (uint32_t)subnetmask;
      if_modified = true;
    }
  }
  strArg = cmd.getArgument("websocket");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong websocketserver %s", strValue);
      doc_tx["error"] = "Error Wrong websocketserver";
      return;
    }
    else
    {
      websocketserver.fromString(strValue);
      ipAddress_struct.WEBSOCKETSERVER = (uint32_t)websocketserver;
      if_modified = true;
    }
  }

  strArg = cmd.getArgument("dns1");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong dns1 %s", strValue);
      doc_tx["error"] = "Error Wrong dns1";
      return;
    }
    else
    {
      dns1.fromString(strValue);
      ipAddress_struct.DNS1 = (uint32_t)dns1;
      if_modified = true;
    }
  }

  strArg = cmd.getArgument("dns2");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    if (IPAddress().fromString(strValue) == false)
    {
      selectPrintf(0, "\r\nError Wrong dns2 %s", strValue);
      doc_tx["error"] = "Error Wrong dns2";
      return;
    }
    else
    {
      dns2.fromString(strValue);
      ipAddress_struct.DNS2 = (uint32_t)dns2;
      if_modified = true;
    }
  }
  strArg = cmd.getArgument("socketport");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    uint16_t port = strValue.toInt();
    webSocketPort = port;
    ipAddress_struct.WEBSERVERPORT = port;
    if_modified = true;
  }
  strArg = cmd.getArgument("baudrate");
  // selectPrintf(0, "\r\nstrArg %s",strArg);
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    uint32_t baudrate = strValue.toInt();
    if (baudrate == 1200 || baudrate == 2400 || baudrate == 4800 || baudrate == 9600 || baudrate == 57600 || baudrate == 115200)
    {
      ipAddress_struct.BAUDRATE = baudrate;
      Serial.begin(baudrate);
    }
    if_modified = true;
  }
  strArg = cmd.getArgument("interval");
  if (strArg.isSet())
  {
    strValue = strArg.getValue();
    uint16_t interval = strValue.toInt();
    ipAddress_struct.Q_INTERVAL = interval;
    if_modified = true;
  }

  ipAddress_struct.IPADDRESS = (uint32_t)ipaddress;
  ipAddress_struct.GATEWAY = (uint32_t)gateway;
  ipAddress_struct.SUBNETMASK = (uint32_t)subnetmask;
  ipAddress_struct.WEBSOCKETSERVER = (uint32_t)websocketserver;
  ipAddress_struct.DNS1 = (uint32_t)dns1;
  ipAddress_struct.DNS2 = (uint32_t)dns2;
  ipAddress_struct.WEBSERVERPORT = webSocketPort;

  selectPrintf(0, "\r\nipaddress %s", IPAddress(ipAddress_struct.IPADDRESS).toString());
  selectPrintf(0, "\r\nwateway %s", IPAddress(ipAddress_struct.GATEWAY).toString());
  selectPrintf(0, "\r\nsubnetmask %s", IPAddress(ipAddress_struct.SUBNETMASK).toString());
  selectPrintf(0, "\r\nwebsocket %s", IPAddress(ipAddress_struct.WEBSOCKETSERVER).toString());
  selectPrintf(0, "\r\ndns1 %s", IPAddress(ipAddress_struct.DNS1).toString());
  selectPrintf(0, "\r\ndns2 %s", IPAddress(ipAddress_struct.DNS2).toString());
  selectPrintf(0, "\r\nwebserverport %d", ipAddress_struct.WEBSERVERPORT);
  selectPrintf(0, "\r\nbaudrate %d", ipAddress_struct.BAUDRATE);
  selectPrintf(0, "\r\nmodbus interval %d", ipAddress_struct.Q_INTERVAL);
  selectPrintf(0, "\r\nver %s", version);
  // selectPrintf(0, "\r\nWould you like to change IpAddress? \r\n I will be affect after reboot.(Y/n) ");

  // int c = clientReadTimeout(10000);

  // if (c == 'Y')
  // {
  // }
  // else if (c == 'n' || c == -1)
  // {
  //   selectPrintf("\r\nCanceled...");
  //   return;
  // }
  EEPROM.writeBytes(1, (const byte *)&ipAddress_struct, sizeof(nvsSystemSet));
  EEPROM.commit();
  FILE *fp;
  fp = fopen("/spiffs/ipaddress.txt", "w+");
  if (fp)
  {
    fwrite((nvsSystemSet *)&ipAddress_struct, sizeof(nvsSystemSet), 1, fp);
  }
  fclose(fp);
  fp = fopen("/spiffs/ipaddress.txt", "r");
  fread((nvsSystemSet *)&ipAddress_struct, sizeof(nvsSystemSet), 1, fp);
  fclose(fp);

  doc_tx["ipaddress"] = ipaddress.toString();
  doc_tx["gateway"] = gateway.toString();
  doc_tx["subnetmask"] = subnetmask.toString();
  doc_tx["dns1"] = dns1.toString();
  doc_tx["dns2"] = dns2.toString();
  doc_tx["ntp1"] = ntp_1.toString();
  doc_tx["ntp2"] = ntp_2.toString();
  doc_tx["websocketserver"] = websocketserver.toString();
  doc_tx["webSocketPort"] = webSocketPort;
  doc_tx["ntpuse"] = ipAddress_struct.ntpuse;
  doc_tx["baudrate"] = ipAddress_struct.BAUDRATE;
  doc_tx["interval"] = ipAddress_struct.Q_INTERVAL;
  doc_tx["ver"] = version;

  selectPrintf(0, "\r\nSucceed.. You can use reboot command\r\n");
}
void date_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  selectPrintf(0, " command done\r\n");
}
void df_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  selectPrintf(0, "\r\nESP32 Partition table:\r\n");
  selectPrintf(0, "| Type | Sub |  Offset  |   Size   |       Label      |\r\n");
  selectPrintf(0, "| ---- | --- | -------- | -------- | ---------------- |\r\n");

  esp_partition_iterator_t pi = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  if (pi != NULL)
  {
    do
    {
      const esp_partition_t *p = esp_partition_get(pi);
      selectPrintf(0, "|  %02x  | %02x  | 0x%06X | 0x%06X | %-16s |\r\n",
                   p->type, p->subtype, p->address, p->size, p->label);
    } while (pi = (esp_partition_next(pi)));
  }
  selectPrintf(0, "\r\n|  HEAP   |       |          |   %d | ESP.getHeapSize |\r\n", ESP.getHeapSize());
  selectPrintf(0, "|Free heap|       |          |   %d | ESP.getFreeHeap |\r\n", ESP.getFreeHeap());
  selectPrintf(0, "|Psram    |       |          |   %d | ESP.PsramSize   |\r\n", ESP.getPsramSize());
  selectPrintf(0, "|Free Psrm|       |          |   %d | ESP.FreePsram   |\r\n", ESP.getFreePsram());
  selectPrintf(0, "|UsedPsram|       |          |   %d | Psram - FreeRam |\r\n", ESP.getPsramSize() - ESP.getFreePsram());
  selectPrintf(0, "|SNMP Threed heap free     |   %d | Psram - FreeRam |\r\n", uxTaskGetStackHighWaterMark(h_pxSnmp));
  selectPrintf(0, "|megatech Threed heap free |   %d | Psram - FreeRam |\r\n", uxTaskGetStackHighWaterMark(h_pxMegatech));
  selectPrintf(0, "|Telnet Threed heap free     |   %d | Psram - FreeRam |\r\n", uxTaskGetStackHighWaterMark(h_pxTelnetTask));
}

void errorCallback(cmd_error *errorPtr)
{
  CommandError e(errorPtr);

  selectPrintf(0, (String("ERROR: ") + e.toString()).c_str());

  if (e.hasCommand())
  {
    selectPrintf(0, (String("Did you mean? ") + e.getCommand().toString()).c_str());
  }
  else
  {
    selectPrintf(0, cli.toString().c_str());
  }
}
void SimpleCLISetUp()
{
  cmd_ls_config = cli.addCommand("I", I_configCallback);
  cmd_ls_config = cli.addCommand("F", F_configCallback);
  cmd_ls_config = cli.addCommand("GF", GF_configCallback);
  cmd_ls_config = cli.addCommand("Q1", Q1_configCallback);
  cmd_ls_config = cli.addCommand("G1", G1_configCallback);
  cmd_ls_config = cli.addCommand("G2", G2_configCallback);
  cmd_ls_config = cli.addCommand("G3", G3_configCallback);
  cmd_ls_config = cli.addCommand("ls", ls_configCallback);
  cmd_ls_config = cli.addCommand("user", user_configCallback);
  cmd_ls_config.addArgument("u/serid", "");
  cmd_ls_config.addArgument("p/asswd", "");

  cmd_ls_config = cli.addCommand("quit", quit_configCallback); // escape telnet

  cmd_ls_config = cli.addCommand("init", init_configCallback);

  cmd_ls_config = cli.addCommand("time", time_configCallback);
  cmd_ls_config.addFlagArgument("s/et");
  cmd_ls_config.addArgument("n/tpuse", "255");
  cmd_ls_config.addArgument("ntp1", "<ip address>"); // time.bora.net
  cmd_ls_config.addArgument("ntp2", "<ip address>"); // kr.pool.ntp.org

  cmd_ls_config = cli.addCommand("ntptime", ntptime_configCallback);

  cmd_ls_config = cli.addSingleArgCmd("rm", rm_configCallback);

  cmd_ls_config = cli.addSingleArgCmd("cat", cat_configCallback);
  cmd_ls_config = cli.addSingleArgCmd("ver", ver_configCallback);

  cmd_ls_config = cli.addSingleArgCmd("reboot", reboot_configCallback);
  cmd_ls_config = cli.addSingleArgCmd("debug", debug_configCallback);
  cmd_ls_config = cli.addSingleArgCmd("format", format_configCallback);

  // cmd_ls_config = cli.addSingleArgCmd("mkdir", mkdir_configCallback);
  //  cmd_ls_config.addArgument("filename","");
  // cmd_ls_config = cli.addCommand("del", del_configCallback);
  cmd_ls_config = cli.addCommand("mv", mv_configCallback);

  cmd_ls_config = cli.addCommand("ipaddress", ip_configCallback);
  cmd_ls_config.addFlagArgument("s/et");
  cmd_ls_config.addArgument("i/paddr", "<ip address>");
  cmd_ls_config.addArgument("s/ubnetmask", "<ip address>");
  cmd_ls_config.addArgument("g/ateway", "<ip address>");
  cmd_ls_config.addArgument("websocket", "<ip address>");
  cmd_ls_config.addArgument("socketport", "81");
  cmd_ls_config.addArgument("dns1", "8.8.8.8");
  cmd_ls_config.addArgument("dns2", "164.124.101.2");
  cmd_ls_config.addArgument("baudrate", "0");
  cmd_ls_config.addArgument("interval", "0");
  cmd_ls_config.addArgument("ver", version);

  cmd_ls_config = cli.addCommand("date", date_configCallback);

  cmd_ls_config = cli.addCommand("df", df_configCallback);

  cmd_ls_config = cli.addSingleArgCmd("format", format_configCallback);

  cmd_ls_config = cli.addSingleArgCmd("upstype", upstype_configCallback);

  cli.setOnError(errorCallback);
}

void setIpaddressToEthernet()
{
  FILE *fp = fopen("/spiffs/ipaddress.txt", "r");
  if (fp)
  { // 존재하면....
    printf("\r\nIpaddress.txt file exist");
    char *line;
    size_t len = 0;
    ssize_t readLength;
    readLength = __getline(&line, &len, fp);
    if (readLength)
    {
      if (ipaddress.fromString(line) == false)
      {
        printf("\r\nWrong Ip %s", line);
        return;
      }
      printf("\r\nIpaddress %s", ipaddress.toString().c_str());
    }
    readLength = __getline(&line, &len, fp);
    if (readLength)
    {
      if (gateway.fromString(line) == false)
      {
        printf("\r\nWrong Gateway %s", line);
        return;
      }
      printf("\r\nGateway %s", gateway.toString().c_str());
    }
    readLength = __getline(&line, &len, fp);
    if (readLength)
    {
      if (subnetmask.fromString(line) == false)
      {
        printf("\r\nWrong subnet mask %s", line);
        return;
      }
      printf("\r\nsubnet mask %s", subnetmask.toString().c_str());
    }

    fclose(fp);
  }
  else
  {
    printf("\r\nipaddress.txt file not found. Use default Ipaddress");
  }
  fclose(fp);
  if (ETH.config(ipaddress, gateway, subnetmask, dns1, dns2) == false)
    printf("Eth config failed...\r\n");
  else
    printf("Eth config succeed...\r\n");
}
int EthLan8720Start()
{
  // WiFi.onEvent(WiFiEvent);
  pinMode(ETH_POWER_PIN, OUTPUT);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLOCK_GPIO0_IN /*ETH_CLK_MODE*/);
  int retrycount = 0;
  // digitalWrite(ETH_POWER_PIN,LOW);

  if (ETH.config(ipaddress, gateway, subnetmask, dns1, dns2) == false)
    printf("Eth config failed...\r\n");
  else
    printf("Eth config succeed...\r\n");
  while (!ETH.linkUp())
  {
    printf("\r\nconnecting...");
    delay(1000);
    if (retrycount++ >= 10)
    {
      return -1;
    }
  }
  printf("\r\nConnected\r\n");
  telnetServer.begin();
  // server.setNoDelay(true);
  printf("\r\nReady! Use 'telnet ");
  printf(ETH.localIP().toString().c_str());
  printf(" 23' to connect");
  return 0;
}
void littleFsInit(int bformat)
{
  esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true};
  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (bformat)
  {
    esp_spiffs_format(conf.partition_label);
  }
  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      printf("Failed to mount or format filesystem\r\n");
    }
    else if (ret == ESP_ERR_NOT_FOUND)
    {
      printf("Failed to find SPIFFS partition\r\n");
    }
    else
    {
      printf("Failed to initialize SPIFFS (%s)\r\n", esp_err_to_name(ret));
    }
    return;
  }
  printf("\r\nPerforming SPIFFS_check().");
  ret = esp_spiffs_check(conf.partition_label);
  if (ret != ESP_OK)
  {
    printf("\r\nSPIFFS_check() failed (%s)", esp_err_to_name(ret));
    return;
  }
  else
  {
    printf("\r\nSPIFFS_check() successful");
  }
  size_t total = 0, used = 0;
  ret = esp_spiffs_info(conf.partition_label, &total, &used);
  if (ret != ESP_OK)
  {
    printf("\r\nFailed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
    esp_spiffs_format(conf.partition_label);
    return;
  }
  else
  {
    printf("\r\nPartition size: total: %d, used: %d", total, used);
  }
}

void sendBufferDataToSocket(uint8_t num)
{

  webRequestNo = -1;
  sendString = "";
  serializeJson(doc_tx, sendString);
  webSocket.sendTXT(num, sendString);
}
// static bool webRequestNo = false;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  DeserializationError de_err;
  uint16_t readc = 1;

  switch (type)
  {
  case WStype_DISCONNECTED:
    ESP_LOGI(TAG, "[%u] Disconnected!\r\n", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    selectPrintf(0,"[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);

    // send message to client
    // object = doc_tx.to<JsonObject>();
    doc_tx["status"] = "connected";

    sendString = "";
    serializeJson(doc_tx, sendString);
    webSocket.sendTXT(num, sendString);
  }
  break;
  case WStype_TEXT:
    // JSON이 아닌 일반 커맨드를 먼저 확인한다.
    de_err = deserializeJson(doc_tx, payload);
    if (de_err)
    {
      printf("");
      selectPrintf(0, "requset Type is not JSON Type \r\n");
      break;
    }
    else
    {
      const char *req_type = doc_tx["command_type"].as<const char *>();

      if (!String(req_type).compareTo("timeSet"))
      {
        int reg = doc_tx["reg"].as<int>();
        unsigned long set = doc_tx["set"].as<unsigned long>();
        struct timeval tmv;
        tmv.tv_sec = set + gmtOffset_sec;
        tmv.tv_usec = 0;
        settimeofday(&tmv, NULL); // 웹에서 PC시간으로 설정을 한다.

        struct tm timeinfo;
        getLocalTime(&timeinfo, 1);
        RtcDateTime dt(
            timeinfo.tm_year - 100, /* 1900 부터 2023년은 123 Rtc는 2000부터 따라서 */
            timeinfo.tm_mon,
            timeinfo.tm_mday,
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec);
        Rtc.SetDateTime(dt);
        printLocalTime();

        doc_tx.clear();
        doc_tx["command_type"] = "timeSet";
        doc_tx["year"] = timeinfo.tm_year - 100;
        doc_tx["mon"] = timeinfo.tm_mon + 1;
        doc_tx["day"] = timeinfo.tm_mday;
        doc_tx["hour"] = timeinfo.tm_hour;
        doc_tx["min"] = timeinfo.tm_min;
        doc_tx["sec"] = timeinfo.tm_sec;

        sendString = "";
        serializeJson(doc_tx, sendString);
        webSocket.sendTXT(num, sendString);
        break;
        /// ESP_LOGD(TAG, "\nreq_type=%s reg=%d set=%d", req_type, reg, set);
      }
      else // 일반 CLI명령을 수행한다.
      {
        doc_tx.clear();

        selectPrintf(0, "requset Type is %s\r\n", req_type);
        webRequestNo = 1;
        cli.parse(req_type);
        sendString = "";
        serializeJson(doc_tx, sendString);
        // selectPrintf(0,"%s\r\n", sendString);
        webSocket.sendTXT(num, sendString);
      }
    }
    break;
  case WStype_BIN:
    printf("[%u] get binary length: %u\r\n", num, length);

    break;
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void readInputFromTelnetClient()
{
  char readBuf[2];
  char readCount = 0;

  if (Client.readBytes(readBuf, 1))
  {
    readBuf[1] = 0x00;
    if (readBuf[0] == 8)
    {
      input.remove(input.length() - 1);
    }
    else
    {
      // printf("%c", readBuf[0]);
      input += String(readBuf[0]);
    }
  }
  if (readBuf[0] == '\n' || readBuf[0] == '\r')
  {
    // if (!isUserLogin)
    //   telnetLoginUser(input);
    // if (!isUserPasswd )
    //   telnetLoginPass(input);
    // if( isUserLogin && isUserPasswd )
    // if (isUserLogin)
    {
      cli.parse(input);
      Client.readBytes(readBuf, 1);
      input = "";
      selectPrintf(0, "\r\n#");
    }
  }
}
FILE *fUpdate;
int UpdateSize;
void serverOnset()
{
  webServer.on("/style.css", HTTP_GET, []()
               { readFileToWeb("text/css", "/spiffs/style.css"); });

  webServer.on("/svg.min.js.map", HTTP_GET, []()
               { readFileToWeb("text/javascript", "/spiffs/svg.min.js.map"); });

  webServer.on("/svg.min.js", HTTP_GET, []()
               { readFileToWeb("text/javascript", "/spiffs/svg.min.js"); });

  webServer.on("/index.css", HTTP_GET, []()
               { readFileToWeb("text/css", "/spiffs/index.css"); });

  webServer.on("/index.js", HTTP_GET, []()
               { readFileToWeb("text/javascript", "/spiffs/index.js"); });

  webServer.on("/", HTTP_GET, []()
               { readFileToWeb("text/html", "/spiffs/index.html"); });

  webServer.on("/jquery.min.js", HTTP_GET, []()
               {
    webServer.sendHeader("Connection", "close");
    webServer.send(200, "text/javascript", jquery_min_js); });
  webServer.on("/login", HTTP_GET, []()
               {
    webServer.sendHeader("Connection", "close");
    webServer.send(200, "text/html", loginIndex); });

  webServer.on("/fileUpload", HTTP_GET, []()
               {
    webServer.sendHeader("Connection", "close");
    webServer.send(200, "text/html", fileUpload); });
  webServer.on("/fileUpload.html", HTTP_GET, []()
               { readFileToWeb("text/html", "/spiffs/fileUpload.html"); });

  webServer.on("/serverIndex", HTTP_GET, []()
               {
    webServer.sendHeader("Connection", "close");
    webServer.send(200, "text/html", serverIndex); });
  /*handling uploading firmware file */
  webServer.on(
      "/upload", HTTP_POST, []()
      {
    webServer.sendHeader("Connection", "close");
    webServer.send(200, "text/plain", /*(update.haserror()) ? "fail" :*/ "OK");
    selectPrintf(0,"Finish"); },
      []()
      {
        HTTPUpload &upload = webServer.upload();
        if (upload.status == UPLOAD_FILE_START)
        {
          upload.filename = String("/spiffs/") + upload.filename;
          fUpdate = fopen(upload.filename.c_str(), "w+");
          UpdateSize = 0;
        }
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
          fwrite((char *)upload.buf, upload.currentSize, 1, fUpdate);
          UpdateSize += upload.currentSize;
          selectPrintf(0, "%d\r\n", upload.currentSize);
        }
        else if (upload.status == UPLOAD_FILE_END)
        {
          fclose(fUpdate);
          selectPrintf(0, "Upload END....File name : %s\r\n", upload.filename.c_str());
          selectPrintf(0, "name : %s\r\n", upload.name.c_str());
          selectPrintf(0, "type: %s\r\n", upload.type.c_str());
          selectPrintf(0, "size: %d\r\n", upload.totalSize);
          // Update.end(false);
        }
      });
  webServer.on(
      "/update", HTTP_POST, []()
      {
    webServer.sendHeader("Connection", "close");
    webServer.send(200, "text/plain", (Update.hasError()) ? "UPDATE FAIL" : "OK");
    ESP.restart(); },
      []()
      {
        HTTPUpload &upload = webServer.upload();
        if (upload.status == UPLOAD_FILE_START)
        {
          printf("Update: %s\r\n", upload.filename.c_str());
          if (!Update.begin(UPDATE_SIZE_UNKNOWN))
          { // start with max available size
            Update.printError(Serial);
          }
        }
        else if (upload.status == UPLOAD_FILE_WRITE)
        {
          /* flashing firmware to ESP*/
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
          {
            Update.printError(Serial);
            webServer.send(200, "text/plain", "Update.write FAIL");
          }
        }
        else if (upload.status == UPLOAD_FILE_END)
        {
          if (Update.end(true))
          { // true to set the size to the current progress
            webServer.send(200, "text/plain", "Update Success ");
            printf("Update Success: %u\r\nRebooting...\r\n", upload.totalSize);
          }
          else
          {
            Update.printError(Serial);
            webServer.send(200, "text/plain", "Update FAIL");
          }
        }
      });
  webServer.onNotFound([]()
                       {
    // Handle 404 Not Found errors here

    String filename;
    filename = webServer.uri();
    if (filename.endsWith(".css"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("text/css", filename.c_str());
    }
    else if (filename.endsWith(".js"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("text/javascript", filename.c_str());
    }
    else if (filename.endsWith(".map"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("text/javascript", filename.c_str());
    }
    else if (filename.endsWith(".html"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("text/html", filename.c_str());
      //webServer.sendHeader("Connection", "close");
      //webServer.send(200, "text/html", "");
    }
    else if (filename.endsWith(".gif"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("image/gif", filename.c_str());
    }
    else if (filename.endsWith(".png"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("image/png", filename.c_str());
    }
    else if (filename.endsWith(".bmp"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("image/bmp", filename.c_str());
    }
    else if (filename.endsWith(".ico"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("image/x-icon", filename.c_str());
    }
    else if (filename.endsWith(".svg"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("image/svg+xml", filename.c_str());
    }
    else if (filename.endsWith(".mp3"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("audio/mpeg", filename.c_str());
    }
    else if (filename.endsWith(".wav"))
    {
      filename = "/spiffs";
      filename += webServer.uri();
      readFileToWeb("audio/wav", filename.c_str());
    }
    else {
      filename ="File not found ";
      filename += webServer.uri();
      webServer.send(404, "text/plain", filename );
      selectPrintf(0,"file name %s\r\n",webServer.uri().c_str() );
    } });
}
void timeSet(int year, int mon, int day, int hour, int min, int sec)
{
  time_t now;
  struct tm timeinfo;
  struct timeval tmv;

  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = mon - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = hour;
  timeinfo.tm_min = min;
  timeinfo.tm_sec = sec;
  tmv.tv_sec = mktime(&timeinfo);
  tmv.tv_usec = 0;
  settimeofday(&tmv, NULL);
}

/* LOG redirection */
int telnet_write(const char *format, va_list ap)
{
  if (!(Client && Client.connected()))
    return 0;
  char *buf = (char *)malloc(1024);
  if (buf == NULL)
  {
    // failed to allocate memory from PSRAM, log an error message
    ESP_LOGE("telnet_write", "Failed to allocate memory from PSRAM");
    return -1;
  }
  vsnprintf(buf, 1024, format, ap);
  int len = strlen(buf);

  free(buf);
  return len;
}
int telnetLoginPass(String passwd)
{
  Client.printf("\n\rpasswd : ");
  if (strncmp(sUserInfo.userid, passwd.c_str(), strlen(sUserInfo.userid)) == 0)
  {
    Client.printf("\n\rPasswd OK\r\n>");
    isUserLogin = 1;
    isUserPasswd = 1;
    return 1;
  }
  else
  {
    isUserLogin = 0;
    isUserPasswd = 0;
  }
  return 0;
}

int telnetLoginUser(String userId)
{
  Client.printf("\n\rlogin : ");
  if (strncmp(sUserInfo.userid, userId.c_str(), strlen(sUserInfo.userid)) == 0)
  {
    Client.printf("\n\rlogin OK\r\n#");
    isUserLogin = 1;
    isUserPasswd = 1;
    return 1;
  }
  else
  {
    Client.printf("\n\rlogin : %s %d ", sUserInfo.userid, strncmp(sUserInfo.userid, userId.c_str(), strlen(sUserInfo.userid)));
    isUserLogin = 0;
    isUserPasswd = 0;
  }
  return 0;
}
void telnetServerCheckClient()
{

  if (telnetServer.hasClient())
  { // check if there are any new clients
    Client = telnetServer.available();
    isUserLogin = 0;
    isUserPasswd = 0;
    if (!Client)
      printf("available broken");

    Client.printf("\n\rWelcome to system ");
    Client.printf("\n\rYou are come from ");
    Client.printf(Client.remoteIP().toString().c_str());
    Client.printf("\n\rPlease login\r\n");
  }

  if (Client && Client.connected())
  {
    if (Client.available())
    {
      readInputFromTelnetClient();
    }
  }
  else if (Client)
  {
    isUserLogin = 0;
    isUserPasswd = 0;
    Client.stop();
  }
}
void readnWriteEEProm()
{
  uint8_t ipaddr1;
  if (EEPROM.read(0) != 0x55)
  {
    selectPrintf(0, "\n\rInitialize....Ipset memory....to default..");
    EEPROM.writeByte(0, 0x55);
    ipAddress_struct.IPADDRESS = (uint32_t)IPAddress(192, 168, 0, 57);
    ipAddress_struct.GATEWAY = (uint32_t)IPAddress(192, 168, 0, 1);
    ipAddress_struct.SUBNETMASK = (uint32_t)IPAddress(255, 255, 255, 0);
    ipAddress_struct.WEBSOCKETSERVER = (uint32_t)IPAddress(192, 168, 0, 57);
    ipAddress_struct.DNS1 = (uint32_t)IPAddress(8, 8, 8, 8);
    ipAddress_struct.DNS2 = (uint32_t)IPAddress(164, 124, 101, 2);
    ipAddress_struct.WEBSERVERPORT = 81;
    ipAddress_struct.NTP_1 = (uint32_t)IPAddress(203, 248, 240, 140); //(203, 248, 240, 140);
    ipAddress_struct.NTP_2 = (uint32_t)IPAddress(13, 209, 84, 50);
    ipAddress_struct.ntpuse = false;
#ifdef S115200
    ipAddress_struct.BAUDRATE = 115200;
#else
    ipAddress_struct.BAUDRATE = 2400;
#endif
    ipAddress_struct.Q_INTERVAL = 1000;

    EEPROM.writeBytes(1, (const byte *)&ipAddress_struct, sizeof(nvsSystemSet));
    EEPROM.commit();
    selectPrintf(0, "\n\rInitialize....complete");
  }
  EEPROM.readBytes(1, (byte *)&ipAddress_struct, sizeof(nvsSystemSet));
  if (ipAddress_struct.Q_INTERVAL < 300 || ipAddress_struct.Q_INTERVAL > 60000)
  {
    ipAddress_struct.Q_INTERVAL = 2000;
    EEPROM.writeBytes(1, (const byte *)&ipAddress_struct, sizeof(nvsSystemSet));
    EEPROM.commit();
    EEPROM.readBytes(1, (byte *)&ipAddress_struct, sizeof(nvsSystemSet));
  }
  if (ipAddress_struct.BAUDRATE < 2400 || ipAddress_struct.BAUDRATE > 115200)
  {
#ifdef S115200
    ipAddress_struct.BAUDRATE = 115200;
#else
    ipAddress_struct.BAUDRATE = 2400;
#endif
    EEPROM.writeBytes(1, (const byte *)&ipAddress_struct, sizeof(nvsSystemSet));
    EEPROM.commit();
    EEPROM.readBytes(1, (byte *)&ipAddress_struct, sizeof(nvsSystemSet));
  }

  ipaddress = IPAddress(ipAddress_struct.IPADDRESS);
  gateway = IPAddress(ipAddress_struct.GATEWAY);
  subnetmask = IPAddress(ipAddress_struct.SUBNETMASK);
  dns1 = IPAddress(ipAddress_struct.DNS1);
  dns2 = IPAddress(ipAddress_struct.DNS2);
  websocketserver = IPAddress(ipAddress_struct.WEBSOCKETSERVER);
  webSocketPort = ipAddress_struct.WEBSERVERPORT;
  ntp_1 = IPAddress(ipAddress_struct.NTP_1);
  ntp_2 = IPAddress(ipAddress_struct.NTP_2);
}

// /*     여기 까지    */

#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime &dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  selectPrintf(0, "\r\nDs1302 RTC Time is ");
  selectPrintf(0, datestring);
}
void setRtc()
{
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  printf("\r\n");
  if (!Rtc.IsDateTimeValid())
  {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    printf("RTC lost confidence in the DateTime!\r\n");
    // Rtc.SetDateTime(compiled);
  }
  if (Rtc.GetIsWriteProtected())
  {
    printf("RTC was write protected, enabling writing now\r\n");
    Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning())
  {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    printf("RTC is older than compile time!  (Updating DateTime)\r\n");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    printf("RTC is newer than compile time. (this is expected)\r\n");
  }
  else if (now == compiled)
  {
    printf("RTC is the same as compile time! (not expected but all is fine)\r\n");
  }
  printDateTime(now);
  printf("\r\n");
  struct timeval tmv;
  // tmv.tv_sec = now.Epoch32Time(); //deprecated
  tmv.tv_sec = now.Unix32Time();

  tmv.tv_usec = 0;
  settimeofday(&tmv, NULL);
}
void readInputSerialBT()
{
  char readBuf[2];
  char readCount = 0;
  if (!isBTAuthenticated)
  {
    SerialBT.println("Please Input passwd: ");
    if (!isBTAuthenticated)
    {
      String incomingData = SerialBT.readStringUntil('\n');
      incomingData.trim();
      if (check_pass(incomingData))
      {
        isBTAuthenticated = true;
        SerialBT.println("Authentication successful!");
      }
      else
      {
        SerialBT.println("Authentication failed!");
      }
    }
    // You can put any initialization or setup code here that you want
    // to execute when a device is connected.
  }
  if (isBTAuthenticated)
  {
    if (SerialBT.available())
    {
      if (SerialBT.readBytes(readBuf, 1))
      {
        readBuf[1] = 0x00;
        if (readBuf[0] == 8)
        {
          input.remove(input.length() - 1);
        }
        else
        {
          printf("%c", readBuf[0]);
          input += String(readBuf);
        }
      }
      if (readBuf[0] == '\n' || readBuf[0] == '\r')
      {
        setOutputDirection = BLUETOOTH;
        cli.parse(input);
        setOutputDirection = NONE;
        SerialBT.readBytes(readBuf, 1);
        input = "";
        SerialBT.printf("\n# ");
        // break;
      }
    }
  }
}
void readFileToWeb(const char *content_type, const char *filename)
{
  struct stat st;
  st.st_size = 0;

  int isExist = stat(filename, &st);
  isExist = st.st_size;
  selectPrintf(0, "file name %s \r\n ", filename);
  selectPrintf(0, "file size  %d\r\n ", st.st_size);
  if (!isExist)
  {
    String readString = "file not found ";
    readString += filename;
    webServer.send(404, "text/plain", readString);
    selectPrintf(0, "file not exist %s %d\r\n ", filename, st.st_size);
    return;
  }

  // Send the "Connection: keep-alive" header to keep the WebSocket connection open
  webServer.sendHeader("Connection", "keep-alive");

  // Send the headers
  webServer.sendHeader("Content-Type", content_type);
  webServer.sendHeader("Content-Length", String(st.st_size));
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");

  // Send the file data in chunks
  char *chp = (char *)malloc(1024);
  if (chp == NULL)
  {
    selectPrintf(0, "memory error \r\n");
    String readString = "malloc allocate Error";
    readString += filename;
    webServer.send(404, "text/plain", readString);
    return;
  }
  selectPrintf(0, "memory allocated succeed\r\n");
  fUpdate = fopen(filename, "r");
  if (fUpdate == NULL)
  {
    String readString = "Please upload ";
    readString += filename;
    webServer.send(404, "text/plain", readString);
    free(chp);
    return;
  }
  while (!feof(fUpdate))
  {
    int nRead = fread(chp, 1, 1024, fUpdate);
    webServer.sendContent(chp, nRead);
  }
  fclose(fUpdate);
  free(chp);
  selectPrintf(0, "finished send file\r\n");
}
void telnetTask(void *parameter)
{
  while (1)
  {
    telnetServerCheckClient();
    vTaskDelay(5);
  }
}
void onBTConnect(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("Bluetooth device connected!");
    SerialBT.println("Please Input passwd: ");
  }
}
esp_spp_cb_t callback = NULL;
#define LED_1 2
#define LED_2 3
void setup()
{
  int isEthernetConnect = false;
  pinMode(OP_LED, OUTPUT);
  rtu485.modbusInit();
  readnWriteEEProm();
#ifdef S115200
  Serial.begin(115200);
#elif S2400
  Serial.begin(2400);
#else
  Serial.begin(ipAddress_struct.BAUDRATE);
#endif
  String macAddress = WiFi.macAddress();
  macAddress.replace(":", "");
  SerialBT.begin("IFTECH_" + macAddress);
  callback = onBTConnect;
  SerialBT.register_callback(&callback);

  EEPROM.begin(100);
  setRtc();

  int16_t qSocketSendRequest[5];
  int16_t qRequest[5];
  memset(qRequest, 0x00, 5);
  memset(qSocketSendRequest, 0x00, 5);
  h_queue = xQueueCreate(5, sizeof(qRequest));
  h_sendSocketQueue = xQueueCreate(5, sizeof(qSocketSendRequest));
  if (h_queue == 0 || h_sendSocketQueue == 0)
  {
    printf("\r\nFailed to create queue= %p\n", h_queue);
  }

  printf("\r\nlittleFsInit");
  littleFsInit(0);
  printf("\r\nreadnWriteEEProm");
  readnWriteEEProm();
  printf("\r\nEthLan8720Start");
  if (EthLan8720Start())
  {
    printf("\r\nWiFi.softAPConfig");
    //WiFi.softAPConfig(IPAddress(192, 168, 11, 1), IPAddress(192, 168, 11, 1), IPAddress(255, 255, 255, 0));
    printf("\r\nWiFi.mode(WIFI_MODE_AP)");
    //WiFi.mode(WIFI_MODE_AP);
    //macAddress = String(soft_ap_ssid) + macAddress;
    printf("\r\nWiFi.softAP(soft_ap_ssid, soft_ap_password)");
    //WiFi.softAP(macAddress.c_str(), soft_ap_password);
  }
  else
  {
    printf("\r\nEthernet connection succeed");
  }
  serverOnset();
  printf("\r\nWebServer Begin");
  webServer.begin();
  setIpaddressToEthernet();
  SimpleCLISetUp();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  xTaskCreate(snmpRequest, "snmptech", 10240, NULL, 1, h_pxSnmp);
  xTaskCreate(megatechRequest, "megatech", 10240, NULL, 1, h_pxMegatech);
  xTaskCreate(telnetTask, "telnetTask", 10240, NULL, 1, h_pxTelnetTask);
//   // 7168 8192 10240
  cli.parse("user");
//   // esp_log_level_set("*", ESP_LOG_VERBOSE);
//  esp_log_level_set("*", ESP_LOG_ERROR);
//   // esp_log_set_vprintf(telnet_write);
}

int interval = 1000;
unsigned long previousmills = 0;
int everySecondInterval = 1000;
unsigned long now;

void loop()
{
  webServer.handleClient();
  webSocket.loop();
  if(Serial2.available()) rtu485.receiveData();
  if (SerialBT.available()) readInputSerialBT();
  now = millis();
  if ((now - previousmills > everySecondInterval))
  {
    previousmills = now;
  }
  vTaskDelay(10);
}

// C:\Users\STELLA\.platformio\packages\framework-arduinoespressif32@3.20006.221224\tools\sdk\esp32\qio_qspi\include\sdkconfig.h
// int16_t rRequest[5]
//
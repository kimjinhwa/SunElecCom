# SNMP FOR LITHUME Batteyr , NARADA PROTOCAL with V1.3
```
C:\Users\ServerManager\.platformio\penv\Scripts\platformio.exe run --target upload --environment SNMP-RELEASE
```

썬일렉콤에 납품될 리튬 축전지 48V 모듈 8개의 병렬연결에 사용될 프로그램이다. 
- telnet debug monitering이 가능하다. 
- bluetooth monitering이 가능하다. 
  개발시에만 사용하고 납품시에는 .....?
- web서버 기능이 있다. 
- snmp기능을 제공한다. 
- Narada 배터리와 485통신을 한다.
- Display 보드와 9600으로 시리얼 통신을 한다.
- 프로그램은 웹을 통한 UPGRADE 를 지원한다. 



1. 60초마다 커넥션은 다시 해야 한다. 
2. 최대 동시사용은 4이다.

1. include https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js file into code.  
   not anymore upload into esp32 flash memory.
2. Upload html list.
- index.html
-

3. For Debugging
- Megatec장비와 통신하는 데이타를 볼 수 있다

- nc -ul -p 162 or
- nc -ul -p 162 | hexdump -C
- 
4. Time Setting command

- time -[s]et -ntpuse <0 || 1> -ntp1 <ipaddress> -ntp2 <ipaddress>

5. HTML문서에서 Winsock으로 보내는 명령어 및 Feedback

- type :[command]  
  arg :["reg"]  
  arg :["set"]
- type :[timeSet]  
  arg :["reg"]  ; This Value is ignored. 
  arg :["set"]  ; Time sec value,자바에서 넘겨줄때는 1000으로 나누어서 넘긴다.
- 모든 
# reference site

- calute json size : https://arduinojson.org/v6/assistant/#/step1
  {
  "time": 1351824120,
  "data": [
  48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608,
   48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608,
   48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608,
   48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608,
   48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608,
   48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608, 48.75608
   ]
  }

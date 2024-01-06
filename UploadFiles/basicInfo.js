"use strict"
var socketCommand = "";
var sendString = "";
var socketCommand = "";
var retryCount = 0;

var connectUrl = document.location.hostname;//"192.168.0.57"
if (document.location.hostname.includes("127.0.0")) {
    connectUrl = "192.168.0.57";// 
}

function ValidateIPaddress(inputText) {
    let ipformat = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
    if (inputText.match(ipformat)) {
        return true;
    }
    else {
        alert("입력값의 형식이 잘못 되었습니다.!");
        return false;
    }
}

function getCookie(name) {
    var nameEQ = name + '=';
    var cookies = document.cookie.split(';');
    for (var i = 0; i < cookies.length; i++) {
        var cookie = cookies[i];
        while (cookie.charAt(0) == ' ') {
            cookie = cookie.substring(1, cookie.length);
        }
        if (cookie.indexOf(nameEQ) == 0) {
            return cookie.substring(nameEQ.length, cookie.length);
        }
    }
    return null;
}

function setCookie(name, value, expires) {
    var cookie =
        name +
        '=' +
        value +
        '; expires=' +
        expires.toUTCString() +
        '; path=/';
    document.cookie = cookie;
}

function setWebSocketOnEvent(webSocket) {
    webSocket.addEventListener('open', (event) => {
        setWebSocketOnEvent(webSocket)
        console.log("WebSocket connection opened");
        statusBar.style = "color: blue;";
        statusBar.innerHTML = "네트웍 접속 완료";
        retryCount = 0
    })
    webSocket.addEventListener('error', (event) => {
        console.log("WebSocket error:", event);
    })

    webSocket.addEventListener('message', async (event) => {
        try {
            //console.log(event)
            let data = JSON.parse(event.data);
            console.log(data)
            if (data.command_type == "I") {
                //console.log(data.response)
                var strValue = data.response.split('\r');
                CompanyNametxt.value = strValue[1]
                upsModeltxt.value = strValue[2]
                upsVersionTxt.value = strValue[3]
                //console.log(strValue);
                // 0.5초 딜레이 후 "F" 요청 보내기
                setTimeout(() => {
                    requestServerData("F");
                }, 500);
            }
            else if (data.command_type == 'F') {
                var strValue = data.response.split('\r');
                //console.log(strValue);
                Rating_VoltageTxt.value = strValue[1]
                Rating_CurrentTxt.value = strValue[2]
                Rating_Battery_VoltageTxt.value = strValue[3]
                Rating_FrequencyTxt.value = strValue[4]
                setTimeout(() => {
                    requestServerData("GF");
                }, 500);
            }
            else if (data.command_type == 'GF') {
                var strValue = data.response.split('\r');
                console.log(strValue);
                RectifierVoltageTxt.value = strValue[1]
                RectifierFrequencyTxt.value = strValue[2]
                BypassSourceVoltageTxt.value = strValue[3]
                BypassSourceFrequencTxt.value = strValue[4]
                O_PVoltagTxt.value = strValue[5]
                O_PFrequencyTxt.value = strValue[6]
                BatteryVoltageTxt.value = strValue[7]
                Power_RatinTxt.value = strValue[8]
                console.log(data);
                // setTimeout(() => {
                //     requestServerData("GF");
                // }, 500);
            }
            else {

            }
            statusBar.innerHTML = `request command='${data.command_type}' answer received`
            statusBar.style = "color: blue;"
        }
        catch (error) {
            //webSocket
            console.log("eroro----->" + error);
            if (error instanceof ReferenceError) {
                console.log("Error is a ReferenceError. 이 에러는 무시하고 진행한다.");
                return;
            }
            webSocket.close();
            webSocket = null;
            setTimeout(() => {
                const newWebSocket = new myWebSocket('ws://' + connectUrl + ':81/');
                webSocket = newWebSocket; // assign the new WebSocket instance to the original variable
            }, 1000);
            console.error(error);
        }
    })
}
class myWebSocket extends WebSocket {
    constructor(url) {
        super(url); // call super() first
        setWebSocketOnEvent(this);
    }
}
var webSocket = new myWebSocket('ws://' + connectUrl + ':81/');

class winsocksubClass {
    constructor() {
    }
}
let winsocksubData = new winsocksubClass()

document.getElementById('basicInfoView').addEventListener('click', (e) => {
    console.log("viewLog click")
    document.getElementById('HtmlMainView').style.display = 'none';
    document.getElementById('sectionNetworkInfo').style.display = 'grid';

    let commandData = `I`;
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        webSocket.send(data);
    }
    else console.log("socket was closed");

    commandData = `user `
    data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        webSocket.send(data);
    }
});
//logout
document.getElementById('logout').addEventListener('click', (e) => {
    setCookie("login", "", new Date(0));
    window.location.href = "login.html";
});

function requestServerData(reqCommand) {
    let commandData = reqCommand;
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        //sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
};

document.getElementById('getServerInfo').addEventListener('click', (e) => {
    let commandData = `I`;
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        //sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
//
document.getElementById('firmWareUpload').addEventListener('click', (e) => {
    document.location = "serverIndex";
})
document.getElementById('fileUpload').addEventListener('click', (e) => {
    document.location = "fileUpload";
})



window.onload = function () {

    console.log("onLoad");

    let loginCookie = getCookie("login");
    // If the cookie exists and is not expired, the user is logged in
    if (loginCookie && new Date() < new Date(loginCookie)) {
        console.log("login ok")
        document.getElementById('HtmlMainView').style.display = 'none';
        document.getElementById('sectionNetworkInfo').style.display = 'grid';// 'grid';//
    }
    else {
        window.location.href = "login.html";
        return false;
    }
};
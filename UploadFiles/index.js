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
            let data = JSON.parse(event.data);
            if (sendString.length > 0 && sendString.includes(data.command_type)) {
                alert(`설정완료 ${sendString} ${data.command_type}`)
                sendString = "";
            }
            //console.log(`data.command_type ${typeof (data)} == ${data.command_type}`)
            if (data.command_type == 'nowtime') {
                winsocksubData.showTime(data);
            }
            else if (data.command_type == 'ipaddress') {
                statusBar.innerHTML = `request command='${data.command_type}' answer received`
                statusBar.style = "color: blue;"
                IpAddresstxt.value = data.ipaddress
                subnetMaskTxt.value = data.subnetmask
                gatewayTxt.value = data.gateway
                webSocketServerTxt.value = data.websocketserver
                webSocketPortTxt.value = data.webSocketPort
                ntp1Txt.value = data.ntp1
                ntp2Txt.value = data.ntp2
                ntpuseChkbox.checked = data.ntpuse
                versionLbl.value = data.ver
                baudRatetxt.value = data.baudrate;
                intervaltxt.value = data.interval;
                console.log(data);
            }
            else if (data.command_type == 'user') {
                //console.log(data)
                useridtxt.value = data.userid;
                passwdtxt.value = data.passwd;
            }

            else {
                statusBar.innerHTML = `request command='${data.command_type}' answer received`
                statusBar.style = "color: blue;"
                //console.log(`ret value ${ret}`)
                console.log(event.data)
            }

        }
        catch (error) {
            //webSocket
            console.log("eroro----->" + error) ;
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

document.getElementById('viewN').addEventListener('click', (e) => {
    console.log("viewLog click")
    document.getElementById('HtmlMainView').style.display = 'none';
    document.getElementById('sectionNetworkInfo').style.display = 'grid';

    let commandData = `ipaddress`;
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

document.getElementById('useridbtn').addEventListener('click', (e) => {
    let result = window.confirm("사용자 ID를  변경합니다");
    if (result) {
        let text = document.getElementById('useridtxt')
        let ipaddresss = text.value;
        let commandData = `user -u  ${ipaddresss}`
        let data = JSON.stringify({ 'command_type': commandData });
        if (webSocket.readyState === webSocket.OPEN) {
            sendString = data;
            webSocket.send(data);
        }
        else console.log("socket was closed");
    }
});

document.getElementById('passwdbtn').addEventListener('click', (e) => {
    let result = window.confirm("비밀번호를 변경합니다");
    if (result) {
        let text = document.getElementById('passwdtxt')
        let ipaddresss = text.value;
        let commandData = `user -p  ${ipaddresss}`
        let data = JSON.stringify({ 'command_type': commandData });
        if (webSocket.readyState === webSocket.OPEN) {
            sendString = data;
            webSocket.send(data);
        }
        else console.log("socket was closed");
    }
});

document.getElementById('rebootBtn').addEventListener('click', (e) => {
    let result = window.confirm("시스템을 재 부팅합니다. 1분후에 재 접속해 주세요.");
    if (result) {
        let commandData = `reboot `
        let data = JSON.stringify({ 'command_type': commandData });
        if (webSocket.readyState === webSocket.OPEN) {
            sendString = data;
            webSocket.send(data);
        }
        else console.log("socket was closed");
    }
});

document.getElementById('timesettxt').value = new Date().toLocaleString()


document.getElementById('getServerIpAddress').addEventListener('click', (e) => {
    let commandData = `ipaddress`;
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        //sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('IpAddressbtn').addEventListener('click', (e) => {
    let text = document.getElementById('IpAddresstxt')
    if (!ValidateIPaddress(text.value)) {
        text.value = "0.0.0.0" + text.value;
        return
    };
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -ipaddr ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
//
document.getElementById('subnetBtn').addEventListener('click', (e) => {
    let text = document.getElementById('subnetMaskTxt')
    if (!ValidateIPaddress(text.value)) {
        return
    };
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -subnetmask ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('gatewayBtn').addEventListener('click', (e) => {
    let text = document.getElementById('gatewayTxt')
    if (!ValidateIPaddress(text.value)) {
        return
    };
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -gateway ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});

document.getElementById('ntp1Btn').addEventListener('click', (e) => {
    let ntpAddress = ntp1Txt.value;
    let commandData = `time -s -ntp1 ${ntpAddress}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('ntp2Btn').addEventListener('click', (e) => {
    let ntpAddress = ntp2Txt.value;
    let commandData = `time -s -ntp2 ${ntpAddress}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('socketServerBtn').addEventListener('click', (e) => {
    let text = document.getElementById('webSocketServerTxt')
    if (!ValidateIPaddress(text.value)) {
        return
    };
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -websocket ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('socketportBtn').addEventListener('click', (e) => {
    let text = document.getElementById('webSocketPortTxt')
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -socketport ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('baudRateBtn').addEventListener('click', (e) => {
    let text = document.getElementById('baudRatetxt')
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -baudrate ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});
document.getElementById('intervalBtn').addEventListener('click', (e) => {
    let text = document.getElementById('intervaltxt')
    let ipaddresss = text.value;
    let commandData = `ipaddress -s -interval ${ipaddresss}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});

document.getElementById('ntpuseBtn').addEventListener('click', (e) => {
    let text = document.getElementById('ntpuseChkbox')
    let checked = text.checked ? 1 : 0;
    let commandData = `time -s -ntpuse ${checked}`
    let data = JSON.stringify({ 'command_type': commandData });
    if (webSocket.readyState === webSocket.OPEN) {
        sendString = data;
        webSocket.send(data);
    }
    else console.log("socket was closed");
});


document.getElementById('timesetbtn').addEventListener('click', (e) => {
    let nowTime = new Date();
    const data = JSON.stringify({ 'command_type': 'timeSet', 'reg': 0, 'set': nowTime.getTime() / 1000 }); /* BIT 12 =0 열림*/
    console.log(data);
    sendString = data;
    webSocket.send(data);
})
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
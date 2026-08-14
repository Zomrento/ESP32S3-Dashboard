const char GUIHtml[] = R"rawLiteral(
<!DOCTYPE html>
<html>
<head>
    <script>
        let targetIP = null;
        let text = null;
        let CMDBYTE = null;
        let CMDLENGTH = 0x01;
        let data = null;
        let packet = null;
        const encoder = new TextEncoder();

        function setTargetIP (){
            targetIP = document.getElementById("IPTarget").value;
        }

        function setText (){
            text = document.getElementById("DataText").value;
        }

        function setCMDBYTE (_CMDBYTE){
            CMDBYTE = _CMDBYTE;
        }
        function sendPacket(){
            fetch(targetIP,{
            method: "POST",
            headers:{
            "Content-Type": "application/x-shrimp",
            },
            body: packet
            }
            )
        }

        function buildPacket(hasData = false){
            if(hasData){
                data = encoder.encode(text);
                CMDLENGTH = data.length + 1;
                packet = new Uint8Array(CMDLENGTH + 1);
                packet[0] = CMDLENGTH;
                packet[1] = CMDBYTE;
                packet.set(data, 2);

            }
            else{
                packet = new Uint8Array(CMDLENGTH + 1);
                packet[0] = CMDLENGTH;
                packet[1] = CMDBYTE;
            }
        }

        function sendShrimpCMD(){
            if(targetIP && CMDBYTE){
                switch (CMDBYTE){
                    case 0x07:
                        buildPacket();
                        sendPacket();
                        break;
                    case 0x08:
                        buildPacket(true);
                        sendPacket();
                        break;
                    case 0x09:
                        buildPacket();
                        sendPacket();
                        break;
                    case 0x12:
                        buildPacket(true);
                        sendPacket();
                        break;
                    default:
                        break;
                }
            }
        }
    </script>
    <title>DEBUG GUI</title>
</head>
<body>
    <h1>SHRIMP COMMANDS</h1>
    <p>The following buttons will send binary Shrimp Commands to the ESP</p>
    <input type="text" id="IPTarget">
    <button onclick="setTargetIP()">setTargetIP</button>
    <button onclick="setCMDBYTE(0x07)" >0x07</button>
    <button onclick="setCMDBYTE(0x08)" >0x08</button>
    <button onclick="setCMDBYTE(0x09)" >0x09</button>
    <button onclick="setCMDBYTE(0x12)" >0x12</button>
    <input type="text" id="DataText">
    <button onclick="setText()">SetText</button>
    <button onclick="sendShrimpCMD()">SENTPACKET</button>
</body>
</html>
)rawLiteral";
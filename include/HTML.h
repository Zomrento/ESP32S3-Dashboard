const char GUIHtml[] = R"rawLiteral( 
<!DOCTYPE html> 
<html> 
<head> 
    <style>
        .greybox{
            background-color : #2f2f2f;
            border-radius: 5%;
            border-color: #151515;
        }
    </style>
    <script> 
        let targetIP = null; 
        let text = ""; 
        let num1 = ""; 
        let num2 = ""; 
        let CMDBYTE = ""; 
        let CMDLENGTH = 0x01; 
        let data = new Uint8Array(); 
        let textdata = new Uint8Array(); 
        let combined = null;
        let packet = null; 
        let offset = 0; 
        let hasText = false; 
        let hasNum1 = false; 
        let hasNum2 = false; 
        let BYTEARRAY = [];
        const encoder = new TextEncoder(); 
 
        function setTargetIP (){ 
            targetIP = document.getElementById("IPTarget").value; 
        } 
 
        function setVisible (id, state){ 
            if (state){ 
                document.getElementById(id).style.display = "block"; 
            } 
            else{ 
                document.getElementById(id).style.display = "none"; 
            } 
             
        }

        function ADDBYTE(){
            BYTEARRAY.push(Number(document.getElementById("BYTEINPUT").value));
        }

        function SETTEXT(){
            text = (document.getElementById("STRINGINPUT").value);
        }
 
        function setType (_hasText, _hasNum1, _hasNum2){ 
            hasText = _hasText; 
            hasNum1 = _hasNum1; 
            hasNum2 = _hasNum2; 
        } 
 
        function displayElements(){ 
                setVisible("Num1TypeMsg", hasNum1);
                setVisible("Num1", hasNum1);
                setVisible("Num2TypeMsg", hasNum2); 
                setVisible("Num2", hasNum2); 
                setVisible("TextTypeMsg", hasText); 
                setVisible("DataText", hasText); 
        } 
 
        function setCMDBYTE (_CMDBYTE){ 
            text = ""; 
            num1 = ""; 
            num2 = ""; 
            CMDBYTE = _CMDBYTE; 
            switch (CMDBYTE){ 
                    case 0x06: 
                        setVisible("CUSTOM",false);
                        setVisible("databox", true); 
                        setType(true, false, false); 
                        break;
                    case 0x07: 
                        setVisible("CUSTOM",false);
                        setVisible("databox",false); 
                        setType(false, false, false); 
                        break; 
                    case 0x08: 
                        setVisible("CUSTOM",false);
                        setVisible("databox",true); 
                        setType(true, false, false); 
                        displayElements(); 
                        break; 
                    case 0x09: 
                        setVisible("CUSTOM",false);
                        setVisible("databox",false); 
                        setType(false, false, false); 
                        break;
                    case 0x10: 
                        setVisible("CUSTOM",false);
                        setVisible("databox",true); 
                        setType(false, true, false); 
                        displayElements();
                        break; 
                    case 0x11: 
                        setVisible("CUSTOM",false);
                        setVisible("databox",true); 
                        setType(true, true, false); 
                        displayElements();
                        break; 
                    case 0x12:
                        setVisible("CUSTOM",false); 
                        setVisible("databox",true); 
                        setType(true, false, false); 
                        displayElements(); 
                        break;
                    case 0xFF:
                        setVisible("CUSTOM",true);
                        setVisible("databox",false); 
                        setType(false, false, false);
                        break; 
                    default: 
                        break; 
                } 
            setVisible("sendingbox",true); 
            setVisible("successmessage", false); 
            setVisible("errormessage", false); 
            setVisible("sendingButton",true); 
        } 
 
        function sendPacket(){ 
            if(buildPacket()){ 
                fetch(targetIP,{ 
                method: "POST", 
                headers:{ 
                "Content-Type": "application/x-shrimp", 
                }, 
                body: packet 
                } 
                ) 
                setVisible("successmessage",true); 
                setVisible("sendingButton",false); 
            } 
            else{ 
                setVisible("errormessage",true); 
            } 
        } 
 
        function buildPacket(){ 
            CMDLENGTH = 0x01; 
            if(CMDBYTE!=0xFF){
                textdata = new Uint8Array(); 
                if(hasText){ 
                    text = document.getElementById("DataText").value; 
                    textdata = encoder.encode(text) 
                    CMDLENGTH += textdata.length; 
                } 
                if(hasNum1){ 
                    num1 = document.getElementById("Num1").value; 
                    if(num1 != ""){ 
                            CMDLENGTH += 1; 
                    } 
                    else{ 
                        return false; 
                    } 
                } 
                if(hasNum2){ 
                    num2 = document.getElementById("Num2").value; 
                    if(num2 != ""){ 
                            CMDLENGTH += 1; 
                    } 
                    else{ 
                        return false; 
                    } 
                } 
                data = new Uint8Array(CMDLENGTH - 1); 
                offset = 0; 
                if(hasNum1){ 
                    data[offset] = num1;  
                    offset++; 
                } 
                if(hasNum2){ 
                    data[offset] = num2;  
                    offset++; 
                } 
                if(hasText){ 
                    data.set(textdata, offset); 
                }
            }
            else{
                data = new Uint8Array(BYTEARRAY);
                CMDLENGTH += data.length;
                textdata = encoder.encode(text);
                CMDLENGTH += textdata.length;
                combined = new Uint8Array(data.length + textdata.length);
                combined.set(data, 0);
                combined.set(textdata, data.length);
                packet = new Uint8Array(CMDLENGTH); 
                packet[0] = CMDLENGTH; 
                packet.set(combined,1);
                return true; 
            } 
        packet = new Uint8Array(CMDLENGTH + 1); 
        packet[0] = CMDLENGTH; 
        packet[1] = CMDBYTE; 
        packet.set(data, 2); 
        return true; 
        } 
 
        function sendShrimpCMD(){ 
            if(targetIP && CMDBYTE){ 
                sendPacket(); 
            } 
        } 
    </script> 
    <title>DEBUG GUI</title> 
</head> 
<body> 
    <h1>SHRIMP COMMANDS</h1> 
    <p>The following buttons will send binary Shrimp Commands to the ESP</p>
    <p>0x06 SET text as Luomi quote</p>
    <p>0x07 Luomi Standard Prompt query + auto 0x09 in a few min</p>
    <p>0x08 Luomi Custom Prompt query + auto 0x09 in a few min</p>     
    <p>0x09 GET Luomi Prompt Response and update on ESP32</p>
    <p>0x10 CLEARS last X Todolist Task</p>
    <p>0x11 SET Todolist Task at given Index</p>
    <p>0x12 ADD Todolist Task</p>
    <div class ="greybox"><input type="text" id="IPTarget"> 
        <button onclick="setTargetIP()">setTargetIP</button> 
    </div> 
    <div class ="greybox"> 
        <button onclick="setCMDBYTE(0x06)">0x06</button> 
        <button onclick="setCMDBYTE(0x07)">0x07</button> 
        <button onclick="setCMDBYTE(0x08)">0x08</button> 
        <button onclick="setCMDBYTE(0x09)">0x09</button>
        <button onclick="setCMDBYTE(0x10)">0x10</button> 
        <button onclick="setCMDBYTE(0x11)">0x11</button>  
        <button onclick="setCMDBYTE(0x12)">0x12</button> 
        <button onclick="setCMDBYTE(0xFF)">FREESTYLE</button> 
    </div> 
    <div class ="greybox" id="CUSTOM">
        <p>BYTE</p> 
        <input type="text" id="BYTEINPUT">
        <button onclick="ADDBYTE()">ADDBYTE</button>
        <p>STRING</p> 
        <input type="text" id="STRINGINPUT">
        <button onclick="SETTEXT()">SETTEXT</button> 
    </div> 
    <div class ="greybox" id= "databox">
        <p id="Num1TypeMsg">Integer</p> 
        <input type="number" id="Num1">
        <p id="Num2TypeMsg">Integer</p> 
        <input type="number" id="Num2">
        <p id="TextTypeMsg">String</p> 
        <input type="text" id="DataText"> 
    </div> 
    <div class ="greybox" id = "sendingbox"> 
        <p id="successmessage">Success</p> 
        <p id="errormessage">Error. MessagePacket could not be built</p> 
        <button onclick="sendShrimpCMD()" id="sendingButton">SENTPACKET</button> 
    </div> 
         
</body> 
</html> 
)rawLiteral";
#include <WiFi.h>
#include <WebServer.h>
#include <DMD32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
//----------------------------------------

//----------------------------------------Defining the key.
// "Key" functions like a password. In order to change the text on the P10, the user must know the "key".
// You can change it to another word.
#define key_Txt "p10esp32wb"
#define PIN 2
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 255, 255),matrix.Color(214, 103, 93), matrix.Color(255, 0, 255) };

//----------------------------------------
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <style>
    h1 { font-size: 2.0rem; color:#2980b9;}

    input[type=text], select {
      width: 100%;
      padding: 8px 8px;
      margin: 8px 0;
      display: inline-block;
      border: 1px solid #ccc;
      border-radius: 4px;
      box-sizing: border-box;
    }

    .div_Form {
      margin: auto;
      width: 70%;
      border-radius: 5px;
      background-color: #f2f2f2;
      padding: 10px 10px;
    }

    .buttonON {
      display: inline-block;
      padding: 5px 25px;
      font-size: 13px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 8px;
      box-shadow: 0 3px #999;
    }
    .buttonON:hover {background-color: #3e8e41}
    .buttonON:active {
      background-color: #3e8e41;
      box-shadow: 0 1px #666;
      transform: translateY(2px);
    }
    .buttonON:disabled {
      background-color: #666;
      box-shadow: 0 1px #666;
      transform: translateY(2px);
    }
  </style>
  
  <body>
    <div style="text-align: center;">
      <h1>ESP32 P10 Module Web Server</h1>
    </div>

    <div class="div_Form">
      <form>
        <label for="Keys_TXT">Key :</label>
        <input type="password" id="Keys_TXT" name="Keys_TXT" placeholder="Enter key here...">
        
        <br>
        <hr style="border: 1px solid #e6e6e6;">
        
        <input type="checkbox" id="cb_SingleRow" name="cb_SingleRow" value="" onclick="CB_SR_Click()">
        <label for="cb_SingleRow"> Single Row</label><br><br>
        
        <label for="SingleRow_TXT">Text :</label>
        <input type="text" id="SingleRow_TXT" name="SingleRow_TXT" placeholder="Enter text here...">
        
        
        <button type="button" class="buttonON" id="BTN_Submit" onclick="BTN_Submit_Click()">Submit</button>
        <label style="padding: 15px; color: black;" id="LBL_Info"></label>
      </form>
    </div>

    <script>
      function BTN_Submit_Click() {
        var checkBox_SR = document.getElementById("cb_SingleRow");
        var key_TXT = document.getElementById("Keys_TXT").value;
        if (key_TXT == "") {
          alert("Error ! \rThe key cannot be empty.");
          return;
        }

        document.getElementById("BTN_Submit").disabled = true;
        document.getElementById("LBL_Info").style.color = "black";
        document.getElementById("LBL_Info").innerHTML = "Please wait...";
          var SR_TXT = document.getElementById("SingleRow_TXT").value;
          var msg = key_TXT + ",SR," + SR_TXT;
          Send(msg);
      }
      
      function Send(x) {
        //alert(x);
        if (window.XMLHttpRequest) {
          // code for IE7+, Firefox, Chrome, Opera, Safari
          xmlhttp = new XMLHttpRequest();
        } else {
          // code for IE6, IE5
          xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            if (this.responseText == "+OK") {
              document.getElementById("BTN_Submit").disabled = false;
              document.getElementById("LBL_Info").innerHTML = "";
            }
            if (this.responseText == "+ERR") {
              document.getElementById("BTN_Submit").disabled = false;
              document.getElementById("LBL_Info").innerHTML = "Keys are wrong !";
              document.getElementById("LBL_Info").style.color = "red";
            }
          }
        }
        xmlhttp.open("GET","setText?Settings="+x,true);
        xmlhttp.send();
      }
    </script>
  </body>
</html>
)=====";

// Timer setup.
// create a hardware timer  of ESP32
hw_timer_t * timer = NULL;

//----------------------------------------SSID and PASSWORD of your WiFi network.
const char* ssid = "MTN_4G_E0C1AA";
const char* password = "6D862X8635";
//----------------------------------------

String display_Modes = "";
String single_Row_Txt = "";

// Server on port 80.
WebServer server(80);  

//________________________________________________________________________________IRAM_ATTR triggerScan()
// Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, 
// this gets called at the period set in Timer1.initialize();
//________________________________________________________________________________

//________________________________________________________________________________handleRoot()
// This routine is executed when you open ESP32 IP Address in browser.
void handleRoot() {
  server.send(200, "text/html", MAIN_page); //Send web page
}
//________________________________________________________________________________

//________________________________________________________________________________handleSettings().
// Subroutine to handle settings. The displayed text and others are set here.
void handleSettings() {
  String incoming_Settings = server.arg("Settings");
  Serial.println();
  Serial.println("Incoming settings : ");
  Serial.println(incoming_Settings);
  
  // Example of incoming data from a client in double row display mode : 
  // "p10esp32wb,DR,ESP32,1,ESP32 P10 LED Display"
  // - p10esp32wb             = key.
  // - DR                     = Double Row (Two row display mode).
  // - ESP32                  = Text for the first row.
  // - 1                      = Position of text for first row.
  // - ESP32 P10 LED Display  = Text for the second row.
  //
  // - When using the "getValue" string function, the sequence is:
  //   "p10esp32wb,DR,ESP32,1,ESP32 P10 LED Display"
  //        |       |   |   |           |
  //        0       1   2   3           4
  //
  //   > p10esp32wb = getValue(incoming_Settings, ',', 0);
  //   > DR         = getValue(incoming_Settings, ',', 1);
  //   > and so on.

  if (getValue(incoming_Settings, ',', 0) == key_Txt) {
      single_Row_Txt = getValue(incoming_Settings, ',', 2);
      Serial.println(single_Row_Txt);
      delay(500);

    server.send(200, "text/plane", "+OK");  //--> Sending replies to the client.
    delay(500);
  } else {
    server.send(200, "text/plane", "+ERR"); //--> Sending replies to the client.
    delay(500);
  }
  delay(500);
}
//________________________________________________________________________________

//________________________________________________________________________________getValue()
// String function to split strings based on certain characters.
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//________________________________________________________________________________ 

//________________________________________________________________________________Single_Row_Display_Mode()
// Subroutine for displaying "running text" on P10 in Single Row mode.

//________________________________________________________________________________ 

//________________________________________________________________________________Double_Row_Display_Mode()
// Subroutine to display text in the first row and display "running text" in the second row in Double Row mode.
//________________________________________________________________________________ 

//________________________________________________________________________________VOID SETUP()
// const char* IP_ssid = "ESP32_WS";  //--> access point name
// const char* IP_password = "helloesp32WS"; //--> access point password
void setup(void){
//   // put your setup code here, to run once:

// IPAddress local_ip(192,168,1,1);
// IPAddress gateway(192,168,1,1);
// IPAddress subnet(255,255,255,0);
// //----------------------------------------

// //----------------------------------------Create ESP32 as Access Point.
// Serial.println();
// Serial.println("WIFI mode : AP");
// WiFi.mode(WIFI_AP);
// Serial.println("Setting up ESP32 to be an Access Point.");
// WiFi.softAP(IP_ssid, IP_password); //--> Creating Access Points
// delay(1000);
// Serial.println("Setting up ESP32 softAPConfig.");
// WiFi.softAPConfig(local_ip, gateway, subnet);
// //----------------------------------------

// Serial.println();
// Serial.println("SSID name : ");
// Serial.println(ssid);
// Serial.println("IP address : ");
// Serial.println(WiFi.softAPIP());
// Serial.println();
// Serial.println("Connect your computer or mobile Wifi to the SSID above.");
// Serial.println("Visit the IP Address above in your browser to open the main page.");
// Serial.println();
// delay(500);
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  single_Row_Txt.reserve(50);
  delay(500);

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(255);
  matrix.setTextColor(colors[0]);
  delay(500);

  //----------------------------------------Wait for connection
  Serial.println();
  Serial.println("Connecting to : ");
  
  WiFi.mode(WIFI_STA);
  Serial.println(ssid);

  // Time Out for the process of connecting to the WiFi network. 20 = 20 seconds.
  // If after 20 seconds, it fails to connect to the WiFi network, then ESP32 restarts.
  // I created this condition because there are times when ESP32 cannot connect to a WiFi network and needs to be restarted in order to connect to a WiFi network.
  int time_out = 20; 
  time_out = time_out * 2;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");

    if (time_out > 0) {
      time_out--;
    } else {
      ESP.restart();
    }
    delay(500);
  }
  //----------------------------------------
  
  //----------------------------------------If the ESP32 is successfully connected to the WiFi network, the IP address will be displayed on the serial monitor.
  // Open the IP address in your browser to open the interface page.
  // Make sure that your computer / mobile device and ESP32 are connected to the same WiFi network.
  Serial.println();
  Serial.println("Successfully connected to ");
  Serial.println(ssid);
  Serial.println("IP address : ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------
 
  server.on("/", handleRoot); 
  server.on("/setText", handleSettings);

  // Start server.
  server.begin(); 
  Serial.println();
  Serial.println("HTTP server started");
  delay(500);
}
//________________________________________________________________________________


//________________________________________________________________________________VOID LOOP()
int x = matrix.width();
int pass = 0;
void loop(void){
  // put your main code here, to run repeatedly:

  // Handle client requests.
  server.handleClient();  
  matrix.fillScreen(1);
  matrix.setCursor(x, 0);
  Serial.println(single_Row_Txt);
  matrix.print(F(single_Row_Txt.c_str()));
  if(--x < -60) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(50);
}
//________________________________________________________________________________
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 04_ESP32_P10_Set_Text_Via_Web_Server_-_APM (Access Point Mode)
//----------------------------------------Access Point Declaration and Configuration.

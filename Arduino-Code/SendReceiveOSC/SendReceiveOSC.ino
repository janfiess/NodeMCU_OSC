/**
 * Send and receive OSC messages between NodeMCU and another OSC speaking device.
 * Send Case: Press a physical button (connected to NodeMCU) and get informed about it on your smartphone screen
 * Receive Case: Switch an LED (connected to NodeMCU) on or off via Smartphone
 * Created by Fabian Fiess in November 2016
 * Inspired by Oscuino Library Examples, Make Magazine 12/2015
 */
 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>                // for sending OSC messages
#include <OSCBundle.h>                 // for receiving OSC messages

char ssid[] = "difix";                 // your network SSID (name)
char pass[] = "88288828";              // your network password

// Button Input + LED Output
const int btnPin = 12;                 // D6 pin at NodeMCU
const int ledPin = 14;                 // D5 pin at NodeMCU
const int boardLed = LED_BUILTIN;      // Builtin LED

boolean btnChanged = false;
int btnVal = 1;  

WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
const IPAddress destIp(192,168,0,3);   // remote IP of the target device
const unsigned int destPort = 9000;    // remote port of the target device where the NodeMCU sends OSC to
const unsigned int localPort = 8000;   // local port to listen for UDP packets at the NodeMCU (another device must send OSC messages to this port)

unsigned int ledState = 1;             // LOW means led is *on*

void setup() {
    Serial.begin(115200);

    // Specify a static IP address for NodeMCU - only needeed for receiving messages)
    // If you erase this line, your ESP8266 will get a dynamic IP address
    WiFi.config(IPAddress(192,168,0,123),IPAddress(192,168,0,1), IPAddress(255,255,255,0)); 
  
    // Connect to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());

    // btnInput + LED Output
    pinMode(btnPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(boardLed, OUTPUT); 
}

void loop() {
    receiveOSC();
    sendOSC();
}

void sendOSC(){
    // read btnInput and send OSC
    OSCMessage msgOut("/1/buttonListener");

    if(digitalRead(btnPin) != btnVal) {
        btnChanged = true;
        btnVal = digitalRead(btnPin);
    }

    if(btnChanged == true){
        btnChanged = false;
        digitalWrite(ledPin, btnVal);
        digitalWrite(boardLed, (btnVal + 1) % 2);    // strange, but for the builtin LED 0 means on, 1 means off
        Serial.print("Button: ");
        Serial.println(btnVal);
        msgOut.add(btnVal);
    }

    Udp.beginPacket(destIp, destPort);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();
    delay(100);
}

void receiveOSC(){
    OSCMessage msgIN;
    int size;
    if((size = Udp.parsePacket())>0){
        while(size--)
            msgIN.fill(Udp.read());
        if(!msgIN.hasError()){
            msgIN.route("/1/toggleLED",toggleOnOff);
        }
    }
}

void toggleOnOff(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getFloat(0);

  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  digitalWrite(ledPin, ledState);               // External LED
  
  if (ledState) {
    Serial.println("LED on");
  }
  else {
    Serial.println("LED off");
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}

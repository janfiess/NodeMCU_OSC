/**
 * Receive OSC messages at NodeMCU from any OSC speaking sender.
 * Case: Switch an LED (connected to NodeMCU) on or off via Smartphone
 * Created by Fabian Fiess in November 2016
 * Inspired by Oscuino Library Examples, Make Magazine 12/2015, https://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>                 // for receiving OSC messages

char ssid[] = "difix";                 // your network SSID (name)
char pass[] = "88288828";              // your network password

// Button Input + LED Output
const int ledPin = 14;                 // D5 pin at NodeMCU
const int boardLed = LED_BUILTIN;      // Builtin LED

WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
const unsigned int localPort = 8000;   // local port to listen for UDP packets at the NodeMCU (another device must send OSC messages to this port)
const unsigned int destPort = 9000;    // remote port of the target device where the NodeMCU sends OSC to

OSCErrorCode error;
unsigned int ledState = 1;            // LOW means led is *on*

void setup() {
    Serial.begin(115200);

     // Specify a static IP address for NodeMCU
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

    // LED Output
    pinMode(boardLed, OUTPUT); 
    pinMode(ledPin, OUTPUT);
    digitalWrite(boardLed, ledState);   // turn *off* led
}


void loop() {
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
  OSCMessage msgOUT("/1/toggleLED");

  digitalWrite(boardLed, (ledState + 1) % 2);
  digitalWrite(ledPin, ledState);
  
  msgOUT.add(ledState);
  if (ledState) {
    Serial.println("LED on");
  }
  else {
    Serial.println("LED off");
  }

  ledState = !ledState;     // toggle the state from HIGH to LOW to HIGH to LOW ...

  //send osc message back to control object in TouchOSC
  //Local feedback is turned off in the TouchOSC interface.
  //The button is turned on in TouchOSC interface whe the conrol receives this message.
  Udp.beginPacket(Udp.remoteIP(), destPort);
  msgOUT.send(Udp); // send the bytes
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOUT.empty(); // free space occupied by message
}


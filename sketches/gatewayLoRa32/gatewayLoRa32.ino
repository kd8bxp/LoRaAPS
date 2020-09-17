//This sketch is written for a ESP32 LoRa board, either the LoRa32, or T-BEAM, the oled is not used. It should be used as a GATEWAY digipeater
//It can also be used as a serial interface if connected to a computer. 
//This sketch should be able to be adapted for other boards 
// September 2020, base version code 0.0.5 
// Sept 14, 2020 added serial print messages v0.0.6
// Sept 15, 2020 0.0.6(a) fixed char converted to String error (opps)
// September 16, 2020 version 0.0.7 base removed "F" from transmit, changed checks to check "P",0 for callsign (This should save a little more space in the json string)

/* Copyright (c) 2020 LeRoy Miller, KD8BXP
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

 */
 
#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <ArduinoJson.h> //version 5.13.5 (needs updated)
#include <TimedAction.h> //used to ID the digipeater ever 10 minutes. 
// Original Library by Alexander Brevig https://playground.arduino.cc/Code/TimedAction/
// A fork of the project was found on Github https://github.com/Glumgad/TimedAction
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_MAX_PACKET_SIZE 500 //Need to set this in PubSubClient.h MQTT Packet Size

//These must match for your specific board, these should work for the LoRa32u4 boards, but if it fails, check here.
#define SS      18    
#define RST     14
#define DI0     26
#define BAND    432300000 // local frequencys, must match!

#define LED 13 //LED Pin

#define CQMSG "LoRaAPS net gateway"
String CALLSIGN="KD8BXP-50"; //this will be appended to the message when a packet is digipeated. This is also the callsign to Beacon an ID 

const char* ssid = "homeinternet"; //set ssid of your wifi router
const char* password = "trixie4me"; //set password of your router
const char* mqtt_server = "broker.mqtt-dashboard.com"; //public MQTT Broker
char* topic = "/hamradio/loraradio/message/pager/loraham/v2/"; //This broker and topic are able to be seen by the public do not send anything sensitive, effectly these message is sent in the clear

//For this to work on a local level these parameters need to match
int       loraSpreadingFactor = 9;
int       loraCodingRate      = 5;
int       loraTxPower         = 17;
uint32_t  loraBandwidth       = 62500; //125E6;
//This results in about 879bps if you believe this site https://unsigned.io/understanding-lora-parameters/
//or maybe 488bps from this website https://www.rfwireless-world.com/calculators/LoRa-Data-Rate-Calculator.html

String inputString;
bool stringComplete = false;
String call, msg;
String radiopacket;
bool callPassCheck = false;
bool msgPassCheck = false;
bool pass1 = false;
bool pass2 = false;
bool pass3 = false;
//bool gatewayMessage = false;

void beacon();
TimedAction beaconAction 	= 	TimedAction(600000,beacon); //send an ID about every 10 minutes
WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{

  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
  Serial.begin(9600);
  Serial.setTimeout(10);
  //while (!Serial); //if just the the basic function, must connect to a computer
  delay(1000);
  inputString.reserve(200);
  SPI.begin(5,19,27,18);
  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  radioon();

}

void loop(){
  
  serialEvent(); //The Leonardo (ATMEGA32u4 doesn't support serialEvent(), so we can just call it from the loop)
  rx();
  beaconAction.check(); //check if it is time to send a beacon (ID the digipeater)
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
}

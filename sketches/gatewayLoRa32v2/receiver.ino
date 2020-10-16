void rx() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
        digitalWrite(LED, LOW);
        String buf1;
        char temp;
        digitalWrite(LED, HIGH); //Packet Received
    // read packet
     while (LoRa.available()) {
    temp = (char)LoRa.read();
    buf1 += temp;
  }
  char buf[buf1.length()+1];
  buf1.toCharArray(buf, buf1.length()+1);
  checkPacket(buf);     
           }  
}

void checkPacket(String pkt) {

if (pkt.indexOf(CALLSIGN) > 0) {
  serialPrt("RX: ");
      serialPrt(pkt);
 // For other devices this is were the message can be displayed for a Relay & Gateway, this is where the message stops if addressed to the relay call sign (no one will probably ever see this message)
 // also check to see if this device retransmitted the message, if it retransmitted, then it should NOT display the message
} else {
   //This is where we decode the message to get retransmit count, and to add our call if needed, and then to retransmit the message
   StaticJsonBuffer<500> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(pkt);
   int rtcount = root["R"];
   int count = rtcount;
   
   if (count > 0) {
   root["P"][4-count]=CALLSIGN;
   count--; // = rtcount - 1; 
   root["R"] = count;
      if (root.containsKey("C")) {
        //do some aprs stuff before sending to mqtt and digipeat
        //Get From Call, set status to "FromCall LoRaAPS net"
        //set latitude/longitude symbol try \0
        //send message to aprs
        String remoteLocation = convertToDMM(root["C"][0], root["C"][1]);
        msg = root["P"][0].as<String>() + " LoRaAPS net";
        aprsSend(remoteLocation, "0");
        
      }
   String radiopacket;
   root.printTo(radiopacket);
   char buf[radiopacket.length()+1];
   radiopacket.toCharArray(buf, radiopacket.length()+1);
   client.publish(topic, buf); //Publish to MQTT Broker
   digipeat(radiopacket);  //Re-Transmit OTA
  }
 }
}

void fromMQTT(String pkt) {
  //Check if a message from MQTT is a valid JSON string, check to see if it is a message the gateway has already re-transmitted.
  if (pkt.indexOf(CALLSIGN) > 0) {
  } else {
    StaticJsonBuffer<500> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(pkt);
   if (!root.success()) {
    serialPrt("Invaild Packet from MQTT!"); //reject if not a vaild JSON string
    return;
  }
  //Reset re-transmit count, add our callsign, and transmitt message from MQTT broker
   root["P"][1]=CALLSIGN; root["P"][2]="NOCALL2"; root["P"][3]="NOCALL3";
    root["R"]=2;
   String radiopacket;
  root.printTo(radiopacket);
  digipeat(radiopacket); //Transmit OTA
  }
}

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
  
     StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(pkt);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //Our Callsign was found, now let's see if we sent the message. Could probably also check the "P" or path of the json string
  if (root["F"] != CALLSIGN) {
  String f = root["F"];
  String m = root["M"];
  displayMsg(f,m); //Display message
  }
  
} else {
   //This is where we decode the message to get retransmit count, and to add our call if needed, and then to retransmit the message
   StaticJsonBuffer<500> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(pkt);

   //Check for BLT, BEACON, CQ, WX special callsigns
   String t = root["T"];
   if (t == "BEACON" || t == "CQ" || t == "WX" || t == "BLN") {
    String f = root["F"];
    String temp = f + "\n" + t;
    String m = root["M"];
    displayMsg(temp,m);
   }
   int rtcount = root["R"];
   int count = rtcount;
   
   if (count > 0) {
   root["P"][4-count]=CALLSIGN;
   count--; // = rtcount - 1; 
   root["R"] = count;
   
   String radiopacket;
  root.printTo(radiopacket);
  digipeat(radiopacket);
  }
 }
}
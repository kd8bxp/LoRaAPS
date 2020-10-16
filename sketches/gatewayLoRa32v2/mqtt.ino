

void callback(char* topic, byte* payload, unsigned int length) {
  
  String messageTemp;
  for (int i = 0; i < length; i++) {
    
    messageTemp += (char)payload[i];
  }
  
  fromMQTT(messageTemp);
}

void reconnect() {
  // Loop until we're reconnected
  //while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = CALLSIGN+"LoRaAPS-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      serialPrt("connected");
      // Once connected, publish an announcement...
      String tempHello = CALLSIGN + " Hello World."; //announce gateway is online
      char buf[tempHello.length()+1];
      tempHello.toCharArray(buf, tempHello.length()+1);
      client.publish(topic, buf);
      // ... and resubscribe
      client.subscribe(topic);
    } else {
      serialPrt("failed, rc=");
      serialPrt((String)client.state());
      serialPrt(" try again in 1 second");
      delay(1000); //our reconnect timer is faster 
    }
  //}
}

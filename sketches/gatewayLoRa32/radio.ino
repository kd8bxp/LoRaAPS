void radioon(){
 LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    serialPrt("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc(); //encable CRC checking - off by default
  LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
 
}

void radiooff(){
  //arduino-LoRa library provides a sleep, and a idle mode (Sleep is what I beleive we need here) 
  //I'm not sure this is needed once deep sleep was turned on. More
  //Information is needed.
  LoRa.sleep();
  delay(10);
}

void setupWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
serialPrt("");
  serialPrt("WiFi connected");
  serialPrt("IP address: ");
  serialPrt(WiFi.localIP());
  randomSeed(micros());
}

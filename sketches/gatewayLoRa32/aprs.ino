#define SVR_VERIFIED "verified"
#define TO_LINE  10000
int failCount = 0;


void aprsSend(String locationX, String sta_symbolX) {
  delay(500);
  serialPrt("Send to APRS....");
  if ( espClient.connect(SVR_NAME, SVR_PORT) )
  {
    serialPrt("Server connected");
    espClient.print("user ");
    espClient.print(callsign);
    espClient.print(" pass ");
    espClient.print(passcode);
    espClient.print(" vers LoRaAPS ");
    espClient.println("1.0.0");
    if ( wait4content(&espClient, SVR_VERIFIED, 8) )
    { // PP5MGT-1>APE001,TCPIP*,qAC,BRASIL:!2735.42S/04831.17WZArduino APRS-IS - pp5mgt@qsl.net
      // If connected, send APRS data
      serialPrt("Login OK");
      espClient.print(callsign);
      espClient.print(">APRS,TCPIP*:=");
      espClient.print(locationX);
      espClient.print(sta_symbolX);
      espClient.println(msg);
      serialPrt("Data sent OK");
      delay(2000);
      espClient.stop();
      serialPrt("Server disconnected\n");
      failCount = 0;
    } else {
      serialPrt("Connection Failed. ");
      if (failCount <= 4) {
        failCount++; aprsSend(locationX, sta_symbolX);
      }
    }
  }
}

boolean wait4content(Stream* stream, char *target, int targetLen)
{
  size_t index = 0;  // maximum target string length is 64k bytes!
  int c;
  boolean ret = false;
  unsigned long timeBegin;
  delay(50);
  timeBegin = millis();

  while ( true )
  {
    //  wait and read one byte
    while ( !stream->available() )
    {
      if ( millis() - timeBegin > TO_LINE )
      {
        break;
      }
      delay(2);
    }
    if ( stream->available() ) {
      c = stream->read();
      //  judge the byte
      if ( c == target[index] )
      {
        index ++;
        if ( !target[index] )
          // return true if all chars in the target match
        {
          ret = true;
          break;
        }
      }
      else if ( c >= 0 )
      {
        index = 0;  // reset index if any char does not match
      } else //  timed-out for one byte
      {
        break;
      }
    }
    else  //  timed-out
    {
      break;
    }
  }
  return ret;
}

#define SVR_VERIFIED "verified"
#define TO_LINE  10000
int failCount = 0;


void aprsSend(String locationX, String sta_symbolX) {
  Serial.println(locationX); Serial.println(sta_symbolX); Serial.println(msg);
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

/*
 * To Get to something APRS can use
 * 1st convert from decimal degrees to dms
 * Next convert to Degrees Minutes.m
 * 
 */
 
String convertToDMM(float lat, float lon) {
  int latDegrees = lat;
  int lonDegrees = lon;
  int latM = abs(((lat - latDegrees)*60));
  int lonM = abs(((lon - lonDegrees)*60));
  float latS = abs((lat - latDegrees - latM/60.0F)) * 3600;
  float lonS = abs((abs(lon - lonDegrees) - lonM/60.0F)) * 3600;
  float latMM = (latM +(latS/60.0F));
  float lonMM = (lonM +(lonS/60.0F));
  String aprsLat, aprsLon, latDirection, lonDirection;
  latDegrees = abs(latDegrees);
  lonDegrees = abs(lonDegrees);

  if ((int)lat > 0) { latDirection = "N"; } else { latDirection = "S"; }
  if ((int)lon > 0) { lonDirection = "E"; } else { lonDirection = "W"; }

  aprsLat = (String)latDegrees+(String)latMM;
  if (numdigits(lonDegrees) < 3) {aprsLon = "0"+(String)lonDegrees+(String)lonMM;} else {aprsLon = (String)lonDegrees+(String)lonMM;}
  aprsLat = aprsLat + latDirection;
  aprsLon = aprsLon + lonDirection;
  String returnString = aprsLat + "\\" + aprsLon;
 return returnString;
}

int numdigits(int i)
{
       int digits;

       i = abs(i);
       if (i < 10)
         digits = 1;
       else
         digits = (int)(log10((double)i)) + 1;
       return digits;
}

/*
 * used: https://www.directionsmag.com/site/latlong-converter/
 * and
 * https://www.rapidtables.com/convert/number/degrees-to-degrees-minutes-seconds.html
 * 
 * Other Latitude/Longitude resources
 * https://www.latlong.net/degrees-minutes-seconds-to-decimal-degrees
 * https://www.fcc.gov/media/radio/dms-decimal
 * 
 * APRS online converter
 * http://digined.pe1mew.nl/?How_to:Convert_coordinates
 */


void displaysetup() {
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();
}


void wordWrap(String sr) {
  //This works with the Adafruit5x7 font, it should work with any font that is 5x7
 oled.println("");
 oled.print("Message: ");
  int charCount = 9;
  int sizeWord = sr.length()+1; //sizeof(sr);
  char buf[sizeWord]; //sizeof(sr)];
  sr.toCharArray(buf, sizeof(buf));
  char *p = buf;
  char *str;
    while ((str = strtok_r(p, " ", &p)) != NULL) { // delimiter is the space
      charCount += strlen(str) + 1;
    if (charCount >= 20) {  charCount = strlen(str) + 1; oled.print("\n"); } 
      
        oled.print(str); oled.print(" "); 
      }
 }

 void displayMsg(String ft, String mt) {
  oled.clear();
  oled.println("\nFrom: " + ft);
  serialPrt("From: " +ft);
  serialPrt(mt);
  wordWrap(mt);
  
 }

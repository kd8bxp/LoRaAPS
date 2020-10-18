void setupButtons() {
   button.setClickHandler(click);
  button.setLongClickHandler(longClick);
  button.setDoubleClickHandler(doubleClick);
  button.setTripleClickHandler(tripleClick);
}


void click(Button2& btn) {
  displayChange++;
  switch(displayChange) {
    case 1:
    //once one function is done this will change
    #ifdef TBeam
    batteryCheck();
    #elif LoRa32
    displayCall();
    #endif
    break;
    case 2:
    displayMsg(from, holdMsg);
    break;
    case 3:
    displayPath();
    break;
    case 4:
    #ifdef TBeam
    displayLocation(); //display latitude and longitude using TBeam GPS
    #elif LoRa32
    oled.clear(); 
    #endif
    break;
    #ifdef TBeam
    case 5:
    oled.clear();
    break;
    #endif
    default:
    //once one function is done this will change
    #ifdef TBeam
    batteryCheck();
    #elif LoRa32
    displayCall();
    #endif
    break;
  }
  #ifdef TBeam
  if (displayChange>5) {displayChange=1;}
  #elif LoRa32
  if (displayChange>4) {displayChange=1;}
  #endif
}

void longClick(Button2& btn) {
    //Nothing here yet
}

void doubleClick(Button2& btn) {
    //Nothing here yet
}

void tripleClick(Button2& btn) {
    //Nothing here yet
}

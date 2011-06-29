char data[100];
uint16_t pktCount;
float voltage;


void setPacketData(){
  Utils.blinkLEDs(1);
  voltage = PWR.getBatteryVolts();
  int d1 = voltage;
  float f2 = voltage - d1;
  int d2 = trunc(f2 * 10000);
  sprintf(data,"#%u,%u,%u,%d.%04d,\r\n", pktCount, PWR.getBatteryLevel(), xbee802.powerLevel, d1, d2);
}


void sendPacketNoAPI(){
  xbee802.send("000000000000FFFF", data);
  XBee.flush();
}


void flood(uint8_t pwrLvl, uint16_t maxPackets, uint16_t delayVal){
  xbee802.setPowerLevel(pwrLvl);
  for(pktCount = 1; pktCount <= maxPackets; pktCount++){
    setPacketData();
    sendPacketNoAPI();
    delay(delayVal);
  }
}


void setup(){
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
}


void loop(){
  for(uint16_t counter = 1; counter <= 1; counter++){
    flood(0,1000,1);
  }
  //delay(120000);
}

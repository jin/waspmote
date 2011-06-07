#define addrBroadcast64 "000000000000FFFF"

packetXBee* paq_sent;
char data[100];
uint16_t pktCount;
int16_t accX, accY, accZ;
uint8_t powerLevel, temperature, batteryLevel;


void setPacketParams(){
  paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  //  paq_sent->mode=UNICAST;
  paq_sent->mode=1;
  paq_sent->packetID=0x52;
  paq_sent->opt=1; 
  xbee868.hops=0;
  xbee868.setOriginParams(paq_sent, "2222", MY_TYPE);
  //    xbee868.setDestinationParams(paq_sent, "0013A2004066A39F", data, MAC_TYPE, DATA_ABSOLUTE); //unicast to gateway
  //    xbee868.setDestinationParams(paq_sent, "0013A2004066A3A5", data, MAC_TYPE, DATA_ABSOLUTE); //unicast to receiver waspmote
  xbee868.setDestinationParams(paq_sent, addrBroadcast64 , data, MAC_TYPE, DATA_ABSOLUTE); //broadcast
}


void setPacketData(){
  Utils.blinkLEDs(1);
//  temperature = RTC.getTemperature();
//  batteryLevel = PWR.getBatteryLevel();
  accX=ACC.getX();
  accY=ACC.getY();
  accZ=ACC.getZ();

  sprintf(data,"%lu,%u,%u, X:%d, Y:%d, Z:%d\n\r", millis(), pktCount,xbee868.powerLevel,accX,accY,accZ);
}


void sendPacket(){
  xbee868.sendXBee(paq_sent);
  free(paq_sent);
  paq_sent=NULL;
}


void flood(uint8_t pwrLvl, uint16_t maxPackets){
    xbee868.setPowerLevel(pwrLvl);
    for(pktCount = 1; pktCount <= maxPackets; pktCount++){
      setPacketData();
      setPacketParams();
      sendPacket();
    }
    XBee.flush();
}
 

void setup(){
  xbee868.init(XBEE_868,FREQ868M,NORMAL);  
  xbee868.ON();
  RTC.begin();
  ACC.ON();
  ACC.setADCmode(1);
}

void loop(){
  for(uint16_t counter = 1; counter <= 1; counter++){
      flood(0,100);
      //flood(1,100);
      //flood(2,100);
      //flood(3,100);
      //flood(4,100);
  }  
  //delay(300000);
}



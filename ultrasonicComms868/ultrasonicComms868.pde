#define addrBroadcast64 "000000000000FFFF"

#define node1_16 "0001"
#define node2_16 "0002"
#define node3_16 "0003"
#define node4_16 "0004"
#define node5_16 "0005"
#define node6_16 "0006"

#define LOT_OCCUPIED 1
#define LOT_VACANT 0

packetXBee* paq_sent;
char data[100];
uint16_t pktCount, reading = 0;
uint8_t nodeID = 5, batteryLevel; 


void setPacketParams(){
    paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
    paq_sent->mode=BROADCAST;
    paq_sent->packetID=0x52;
    paq_sent->opt=1; 
    xbee868.hops=0;
    xbee868.setOriginParams(paq_sent, node5_16, MY_TYPE);
    xbee868.setDestinationParams(paq_sent, addrBroadcast64 , data, MAC_TYPE, DATA_ABSOLUTE); //broadcast
}


void setPacketData(){
    Utils.blinkLEDs(1);
    reading = pulseIn(DIGITAL1, HIGH) / 58;
    sprintf(data,"%u,%u,%u,%u,\r\n", nodeID, pktCount, PWR.getBatteryLevel(), reading);
}


void sendPacket(){
    xbee868.sendXBee(paq_sent);
    free(paq_sent);
    paq_sent=NULL;
}


void flood(uint8_t pwrLvl, uint16_t maxPackets, uint8_t delayVal){
    xbee868.setPowerLevel(pwrLvl);
    for(pktCount = 1; pktCount <= maxPackets; pktCount++){
      setPacketData();
      setPacketParams();
      sendPacket();
      delay(delayVal);
    }
    XBee.flush();
}
 

void setup(){
    xbee868.init(XBEE_868,FREQ868M,PRO);  
    xbee868.ON();
    PWR.setSensorPower(SENS_5V, SENS_ON);
}

void loop(){
    for(uint16_t counter = 1; counter <= 1; counter++){
        flood(0,100,1000);
    }  
    //delay(300000);
}

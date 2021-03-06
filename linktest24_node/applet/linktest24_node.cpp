#include "WProgram.h"
void setPacketParams();
void setPacketData();
void sendPacketAPI();
void flood(uint8_t pwrLvl, uint16_t maxPackets, uint16_t delayVal);
void setup();
void loop();
packetXBee* pkt;
char data[100];
uint16_t pktCount;
uint8_t nodeID = 1;

/* Begin packet with API header */
void setPacketParams(){
  pkt=(packetXBee*) calloc(1,sizeof(packetXBee));
  pkt->mode=BROADCAST;
  pkt->packetID=0x52;
  pkt->opt=1;
  xbee802.hops=0;
  xbee802.setOriginParams(pkt, "0001", MY_TYPE);
  xbee802.setDestinationParams(pkt, "000000000000FFFF", data, MAC_TYPE, DATA_ABSOLUTE);
}

void setPacketData(){
  Utils.blinkLEDs(1);
  sprintf(data,"%u,%u,%u,\r\n", nodeID, pktCount, xbee802.powerLevel);
}

void sendPacketAPI(){
  xbee802.sendXBee(pkt);
  free(pkt);
  pkt=NULL;
}
/* End packet with API header */

void flood(uint8_t pwrLvl, uint16_t maxPackets, uint16_t delayVal){
  xbee802.setPowerLevel(pwrLvl);
  for(pktCount = 1; pktCount <= maxPackets; pktCount++){
    setPacketData();
    setPacketParams();
    sendPacketAPI();
    //delay(delayVal);
  }
  XBee.flush();
}

void setup(){
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
}


void loop(){
  for(uint16_t counter = 1; counter <= 1; counter++){
    flood(0,100,0);
    flood(1,100,0);
    flood(2,100,0);
    flood(3,100,0);
    flood(4,100,0);
  }
  delay(120000);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


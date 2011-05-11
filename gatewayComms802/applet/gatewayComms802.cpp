#include "WProgram.h"
void setPacketParams();
void setPacketData();
void sendPacket();
void setup();
void loop();
packetXBee* paq_sent;
int8_t state=0;
char data[80]; //set to 130
uint16_t counter, counter2;
int16_t accX, accY, accZ;
uint8_t powerLevel, temperature, batteryLevel, adcMode=1;


void setPacketParams()
{
  paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  //  paq_sent->mode=UNICAST;
  paq_sent->mode=1;
  paq_sent->packetID=0x52;
  paq_sent->opt=1; 
  xbee802.hops=0;
  xbee802.setOriginParams(paq_sent, "2222", MY_TYPE);
  //    xbee802.setDestinationParams(paq_sent, "0013A2004066A39F", data, MAC_TYPE, DATA_ABSOLUTE); //unicast to gateway
  //    xbee802.setDestinationParams(paq_sent, "0013A2004066A3A5", data, MAC_TYPE, DATA_ABSOLUTE); //unicast to receiver waspmote
  xbee802.setDestinationParams(paq_sent, "000000000000FFFF", data, MAC_TYPE, DATA_ABSOLUTE); //broadcast
}


void setPacketData()
{
  Utils.blinkLEDs(1);
//  temperature = RTC.getTemperature();
//  batteryLevel = PWR.getBatteryLevel();
//  accX=ACC.getX();
//  accY=ACC.getY();
//  accZ=ACC.getZ();
  sprintf(data,"%lu,%u,%u\n\r", millis(), counter,xbee802.powerLevel);
}


void sendPacket()
{
  xbee802.sendXBee(paq_sent);
  free(paq_sent);
  paq_sent=NULL;
}
 
 
void setup()
{
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);  
  xbee802.ON();
  XBee.flush();
  //Set power level. 0 = 0dBm, 1 = 13dBm, 2 = 20dBm, 3 = 22dBm, 4 = 25dBm
  RTC.begin();
  ACC.ON();
  ACC.setADCmode(1);
}

void loop()
{
  for(counter2 = 1; counter2 <= 1; counter2++)
  {
    for(counter = 1; counter <= 100; counter++)
    {
      xbee802.setPowerLevel(0);
      setPacketData();
      setPacketParams();
      sendPacket();
    }
    XBee.flush();    
    for(counter = 1; counter <= 100; counter++)
    {
      xbee802.setPowerLevel(1);
      setPacketData();
      setPacketParams();
      sendPacket();
    }
    XBee.flush();    
    for(counter = 1; counter <= 100; counter++)
    {
      xbee802.setPowerLevel(2);
      setPacketData();
      setPacketParams();
      sendPacket();
    }
    XBee.flush();    
    for(counter = 1; counter <= 100; counter++)
    {
      xbee802.setPowerLevel(3);
      setPacketData();
      setPacketParams();
      sendPacket();
    }
    XBee.flush();    
    for(counter = 1; counter <= 100; counter++)
    {
      xbee802.setPowerLevel(4);
      setPacketData();
      setPacketParams();
      sendPacket();
    }
    XBee.flush();
  }  
  delay(300000);
}



int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


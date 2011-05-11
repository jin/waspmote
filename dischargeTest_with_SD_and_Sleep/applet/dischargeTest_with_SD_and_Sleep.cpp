#include "WProgram.h"
void setPacketParams();
void setPacketData();
void sendPacket();
void setup();
void loop();
packetXBee* paq_sent;
char data[120]; //set to 130
uint16_t counter;
int16_t accX, accY, accZ, i;
uint8_t powerLevel, temperature, batteryLevel;



void setPacketParams()
{
  paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  //  paq_sent->mode=UNICAST;
  paq_sent->mode=1;
  paq_sent->packetID=0x52;
  paq_sent->opt=1; 
  xbee868.hops=0;
  xbee868.setOriginParams(paq_sent, "2222", MY_TYPE);
  //    xbee868.setDestinationParams(paq_sent, "0013A2004066A39F", data, MAC_TYPE, DATA_ABSOLUTE); //unicast to gateway
  //    xbee868.setDestinationParams(paq_sent, "0013A2004066A3A5", data, MAC_TYPE, DATA_ABSOLUTE); //unicast to receiver waspmote
  xbee868.setDestinationParams(paq_sent, "000000000000FFFF", data, MAC_TYPE, DATA_ABSOLUTE); //broadcast
}


void setPacketData()
{
  counter++;
  Utils.blinkLEDs(1);
//  temperature = RTC.getTemperature();
  batteryLevel = PWR.getBatteryLevel();
//  accX=ACC.getX();
//  accY=ACC.getY();
//  accZ=ACC.getZ();
  sprintf(data,"%lu,%u,%u\n\r", millis(), counter, batteryLevel);
}


void sendPacket()
{
  xbee868.sendXBee(paq_sent);
  free(paq_sent);
  paq_sent=NULL;
}


void setup()
{
  xbee868.init(XBEE_868,FREQ868M,NORMAL);  
  xbee868.ON();
  xbee868.setPowerLevel(4);
  //ACC.ON();
  //ACC.setADCmode(1);
  RTC.begin();
  SD.ON();
  SD.create("logfile.txt");
  SD.appendln("logfile.txt","Sleep test");
  SD.appendln("logfile.txt","Delay: 855ms - Interval: 1000ms - Power Level: 4 - XBee Sleep Off");
  SD.appendln("logfile.txt","Timestamp in ms, Sequence number, Battery level");
}


void loop()
{  
//  counter++;
//  temperature = RTC.getTemperature();
//  batteryLevel = PWR.getBatteryLevel();
//  accX=ACC.getX();
//  accY=ACC.getY();
//  accZ=ACC.getZ();
//  sprintf(data,"%lu Seq: %u, Pwr: %u, Batt: %u, Tmp: %u C, X: %d, Y: %d, Z: %d \n\r",// 
//    millis(), counter, xbee868.powerLevel, batteryLevel, temperature, accX, accY, accZ);
//  setPacketParams();
//  sendPacket();
//  SD.append("logfile.txt",data);
//  XBee.flush();
//  delay(5000);
  setPacketData();
  setPacketParams();
  sendPacket();
  //if (counter % 25 == 0)
  //{
    SD.append("logfile.txt",data);
  //}
  //PWR.sleep(WTD_8S, ALL_OFF);
  //xbee868.ON();
  //XBee.setMode(XBEE_SLEEP);
  delay(855);
  //XBee.setMode(XBEE_ON);

}




int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


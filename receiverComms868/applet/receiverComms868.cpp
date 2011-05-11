#include "WProgram.h"
void analyzeData();
void xbeePrint();
void setup();
void loop();
uint8_t state=0;
uint16_t rcvCounter=1;
//struct fat_file_struct* outputFile;
char* time_date;

void analyzeData()
{
  xbee868.treatData();  
  if( !xbee868.error_RX )
  {
    //xbee868.getRSSI();
    Utils.blinkLEDs(25);    
    while(xbee868.pos>0)
    {       
      xbeePrint();
      if ((rcvCounter - 1) % 100 == 0)
      {
        XBee.println(RTC.getTime());
      }       
      free(xbee868.packet_finished[xbee868.pos-1]);   
      xbee868.packet_finished[xbee868.pos-1]=NULL;
      xbee868.pos--;
    }
  }
}

void xbeePrint()
{
  XBee.print("Network Origin: ");          
  XBee.print(xbee868.packet_finished[xbee868.pos-1]->naO[0],HEX);
  XBee.print(xbee868.packet_finished[xbee868.pos-1]->naO[1],HEX);       
  XBee.print(", MAC Source: ");          
  for(int b=0;b<4;b++)
  {
    XBee.print(xbee868.packet_finished[xbee868.pos-1]->macSH[b],HEX);
  }
  for(int c=0;c<4;c++)
  {
    XBee.print(xbee868.packet_finished[xbee868.pos-1]->macSL[c],HEX);
  }
  XBee.print(", Pkts received: ");
  XBee.print(rcvCounter++);
//  XBee.print(", RSSI : -");
//  XBee.print(xbee868.packet_finished[xbee868.pos-1]->RSSI,DEC); 
//  XBee.print(xbee868.valueRSSI[0],DEC);
//  XBee.print("dBm");      
  XBee.print(", Data: ");                    
  for(int f=0;f<xbee868.packet_finished[xbee868.pos-1]->data_length;f++)
  {
    XBee.print(xbee868.packet_finished[xbee868.pos-1]->data[f],BYTE);
  }      
}

void setup()
{
  xbee868.init(XBEE_868,FREQ868M,NORMAL);
  xbee868.ON();
  RTC.begin();
  RTC.setTime("01:01:01:01:00:00:00");
  XBee.flush();
//  SD.ON();
//  if ( SD.isFile("output.txt") )
//  {
//    outputFile=SD.openFile("output.txt");
//  }
//  else
//  {
//    SD.create("output.txt");
//    outputFile=SD.openFile("output.txt");
//  }
//  XBee.print(SD.ls());
}

void loop()
{
  if( XBee.available() )
  {
    analyzeData(); 
  }  
}



int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


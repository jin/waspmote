#include "WProgram.h"
void analyzeData();
void xbeePrint();
void setup();
void loop();
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
      free(xbee868.packet_finished[xbee868.pos-1]);   
      xbee868.packet_finished[xbee868.pos-1]=NULL;
      xbee868.pos--;
    }
  }
}

void xbeePrint()
{
  for(int f=0;f<xbee868.packet_finished[xbee868.pos-1]->data_length;f++)
  {
    XBee.print(xbee868.packet_finished[xbee868.pos-1]->data[f],BYTE);
  }      
}

void setup()
{
  xbee868.init(XBEE_868,FREQ868M,NORMAL);
  xbee868.ON();
  XBee.flush();
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


#include "WProgram.h"
void setup();
void loop();
void setup()
{
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
}

void loop()
{
  if( XBee.available() )
  {
      xbee802.treatData();  
      if( !xbee802.error_RX )
      {
      Utils.blinkLEDs(25);  
      //while(xbee802.pos>0)
      //{       
        //free(xbee802.packet_finished[xbee802.pos-1]);   
        //xbee802.packet_finished[xbee802.pos-1]=NULL;
        //xbee802.pos--;
      //}
      }
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


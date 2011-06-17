void analyzeData()
{
  xbee802.treatData();  
  if( !xbee802.error_RX )
  {
    Utils.blinkLEDs(25);    
    while(xbee802.pos>0)
    {       
      xbeePrint();
      free(xbee802.packet_finished[xbee802.pos-1]);   
      xbee802.packet_finished[xbee802.pos-1]=NULL;
      xbee802.pos--;
    }
  }
}

void xbeePrint()
{
  for(int f=0;f<xbee802.packet_finished[xbee802.pos-1]->data_length;f++)
  {
    XBee.print(xbee802.packet_finished[xbee802.pos-1]->data[f],BYTE);
  }      
}

void setup()
{
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
  XBee.flush();
}

void loop()
{
  if( XBee.available() )
  {
    analyzeData(); 
  }  
}

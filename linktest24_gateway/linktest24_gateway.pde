char counterstr[200];
uint16_t counter = 0;

void analyzeData()
{
  xbee802.treatData();  
  if( !xbee802.error_RX )
  {
    Utils.blinkLEDs(1);    
    while(xbee802.pos>0)
    {       
      counter++;
      sprintf(counterstr, "%u, ", counter);
      XBee.print(xbee802.packet_finished[xbee802.pos-1]->data);
      SD.append("log.txt", counterstr);
      SD.append("log.txt", xbee802.packet_finished[xbee802.pos-1]->data);
      free(xbee802.packet_finished[xbee802.pos-1]);   
      xbee802.packet_finished[xbee802.pos-1]=NULL;
      xbee802.pos--;
    }
  }
}


void setup()
{
  xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
  xbee802.ON();
  SD.ON();
  SD.create("log.txt");
  SD.appendln("log.txt", "\nBegin log..\n===========\n");
}

void loop()
{
  if( XBee.available() )
  {
    analyzeData(); 
  }  
}

char* data;

void forgePacket(){
    sprintf(data,"Hello world\n\r");
}

void setup(){
    xbee802.init(XBEE_802_15_4,FREQ2_4G,NORMAL);
    xbee802.ON();
}

void loop(){
    forgePacket();
    xbee802.send("000000000000FFFF",data);
    Utils.blinkLEDs(25);
    delay(1000);
}

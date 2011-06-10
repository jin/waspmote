#define addrBroadcast64 "000000000000FFFF"

#define node1_16 "0001"
#define node2_16 "0002"
#define node3_16 "0003"
#define node4_16 "0004"
#define node5_16 "0005"
#define node6_16 "0006"

#define LOT_OCCUPIED 1
#define LOT_VACANT 0


#include <limits.h>

// Constant to convert pulse width to range reading, in Âµs/cm
#define PULSE_WIDTH_CONST  58
// Number of readings to sample from the ultrasonic sensor
#define SAMPLE_SIZE  20
// Maximum measureable range.  All readings greater than will be truncated to this value
#define MAX_RANGE  512
// Width of each bin
#define BIN_WIDTH  64
// Number of bins
#define NUM_BINS  (MAX_RANGE / BIN_WIDTH)+1
// Time between pulse width reads
#define READ_INTERVAL_MS  100

// Constraint: Max range Ã— Sample size ? 65535 so that summation of all range values will
// not overflow uint16_t
#if (((MAX_RANGE) * (SAMPLE_SIZE)) > (USHRT_MAX))
#error "Max range Ã— Sample size > 65535!"
#endif




packetXBee* paq_sent;
char data[100];
uint16_t pktCount, reading = 0;
uint8_t nodeID = 5, batteryLevel; 





/**
 * Returns the range reading after passing through a mode filter.
 *
 * 1) Read SAMPLE_SIZE readings from the ultrasonic sensor, at READ_INTERVAL_MS intervals
 * 2) Place each reading into its own "bin", and sum up all readings for that bin.
 *    A reading R is placed in bin B if R?[B Ã— BIN_WIDTH, (B+1) Ã— BIN_WIDTH), i.e. B = R / BIN_WIDTH
 * 3) Find the "modal bin", i.e. the bin with the most number of readings
 * 4) Return the average value of all readings from the modal bin
 */
uint16_t getRange()
{
  uint16_t freqs[NUM_BINS] = {0}; // number of values in each bin
  uint16_t sums[NUM_BINS] = {0};  // sum of value in each bin
  uint8_t i;
  uint16_t range, bin, modalBin = 0;
  
  for (i = 0; i < SAMPLE_SIZE; i++) {
    // Get reading in Âµs and convert to cm
    range = pulseIn(DIGITAL1, HIGH) / PULSE_WIDTH_CONST;
    if (range > MAX_RANGE)
      range = MAX_RANGE;

    // Place readings into bins
    bin = range / BIN_WIDTH;
    sums[bin] += range;
    freqs[bin]++;

    // Update the modal bin
    if (freqs[bin] > freqs[modalBin])
      modalBin = bin;

    delay(READ_INTERVAL_MS);
  }

  // Calculate final range: the average of all readings in the modal bin
  if (freqs[modalBin] > 0)
    range = sums[modalBin] / freqs[modalBin];
  else
    range = 0;

  // Limit range
  if (range > MAX_RANGE)
    range = MAX_RANGE + 1;

  return range;
}



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
  //reading = pulseIn(DIGITAL1, HIGH) / 58;
  sprintf(data,"%u,%u,%u,%u,\r\n", nodeID, pktCount, PWR.getBatteryLevel(), getRange());
}


void sendPacket(){
  xbee868.sendXBee(paq_sent);
  free(paq_sent);
  paq_sent=NULL;
}


void flood(uint8_t pwrLvl, uint16_t maxPackets, uint16_t delayVal){
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
    flood(0,65535,5000);
  }  
}













































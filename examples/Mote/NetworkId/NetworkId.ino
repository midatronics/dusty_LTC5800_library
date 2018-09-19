/*
 *      SmeIoT Library - NetworkID Mote
 *
 *  This example shows how to change the NetworkId.
 *   
 *  Different NetworkId are required when more than one dust network is on the same area
 *
 *  created 08 07 2016
 *     by Mik (mik@ioteam.it)
 *     
 * edit 09 19 2018
 *     by Amanu95 (emanuele.mancuso@midatronics.com)
 *
 *  This example is in the public domain
 *      https://github.com/midatronics/dusty_LTC5800_library
 */

#include <Arduino.h>
#include <DustMote.h>

int newId = 1229; // write here your new NetworkId
boolean readCmd=true;

IpMtDataModel *dataModel;
char *hello = "Hello Dust Manager";
uint8_t dataStream[50];
//=========================== data generator ==================================

uint8_t* generateData(DataModel *model) {

  memcpy(dataStream, hello, 18);
  model->setDatasize(18);

  return (uint8_t*) dataStream;
}
//=========================== "main" ==========================================

void setup() {
  SerialUSB.begin(115200);
  while (!SerialUSB) {
    ;
  }
  SerialUSB.println("start Mote...\n");

  dataModel = new IpMtDataModel(generateData);

  dustMote.begin(
    60000,                         // srcPort
    61020,                         // destPort
    500,                           // dataPeriod (ms)
    dataModel,                     // dataGenerator
    false,                          // polling mode
    NULL            // get Mote status change notification
  );

  delay(100);
}

void loop() {
    dn_ipmt_getParameter_networkId_rpt* msg;

  char data = 0;
  // continuously send the message in polling and
  //      read every down link message
  DustCbStatusE status = dustMote.readData();
  switch (status) {
    
    case Completed:
        msg = (dn_ipmt_getParameter_networkId_rpt*) dustMote.getLastCommand();
        SerialUSB.print("\nNetwork Id = ");
        SerialUSB.println(msg->networkId);
        readCmd = false;
    break;
    
    default :
        if (readCmd)
            dustMote.retrieveNetworkId();
    break;
  }

  if (SerialUSB.available()) {
    data = SerialUSB.read();
    if (('C'==data) || ('c'==data)){
        readCmd = true;
        dustMote.setNetworkId(newId);
    }

    if (('R'==data) || ('r'==data))
        dustMote.retrieveNetworkId();
  }
}

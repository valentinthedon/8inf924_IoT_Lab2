

#include <MKRWAN.h>

#define PACKET_INTERVAL 20000


LoRaModem modem;

String appEui = "0000000000000000";
String appKey = "9A70EAF54E37D4B7BD4F0F2C77037484";

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Serial Started");
  
  if(!modem.begin(US915)) {
    Serial.println("Failed to start module");
    exit(1);
  };
  
  int connected;
  appKey.trim();
  appEui.trim();

  connected = modem.joinOTAA(appEui, appKey);

  if (!connected) {
    Serial.println("An error occur while try to connect");
    exit(1);
  } else {
    Serial.println("Connected to the gateway");
  }
}


int sendPacket(int val){
  modem.beginPacket();
  modem.print(val);
  int err = modem.endPacket(true);
  if(err > 0){
    Serial.print("<-- Sent Packet with value : "); 
    Serial.println(val);   
  } else {
    Serial.println("/!\\ An error occur, a packet WASN'T sent");
  }
  return err;
}


void loop() {
  int i = 0;
  while(true){
    sendPacket(i);
    i++;
    // delay(PACKET_INTERVAL);
  }
}

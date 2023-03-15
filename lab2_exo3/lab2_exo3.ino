

#include <MKRWAN.h>
#include <CayenneLPP.h>

#define PACKET_INTERVAL 20000
#define SENSOR_INTERVAL 900

LoRaModem modem;
CayenneLPP lpp(51);

String appEui = "0000000000000000";
String appKey = "9A70EAF54E37D4B7BD4F0F2C77037484";

const int gasSensorPin = A1;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Serial Started");
  
  if(!modem.begin(US915)) {
    Serial.println("/!\\ Failed to start module");
    exit(1);
  };
  
  int connected;
  appKey.trim();
  appEui.trim();

  connected = modem.joinOTAA(appEui, appKey);

  if (!connected) {
    Serial.println("/!\\ An error occur while try to connect");
    exit(1);
  } else {
    Serial.println("(i) Connected to the gateway");
  }

  lpp.reset();
  lpp.addDigitalOutput(2, 0);
  sendPacket();
  pinMode(LED_BUILTIN, OUTPUT);
}

void buildPacket(float val){
  lpp.reset();
  lpp.addAnalogInput(1, val);  
}

int sendPacket(){
  modem.beginPacket();
  modem.write(lpp.getBuffer(), lpp.getSize());
  int err = modem.endPacket(true);
  if(err > 0){
    Serial.println("<-- Packet sent...");
  } else {
    Serial.println("/!\\ An error occur, a packet WASN'T sent");
  }
  return err;
}

void receivePacket(){
  if (!modem.available()) {
    Serial.println("(i) No downlink message received at this time.");
    return;
  }
  Serial.println("--> ...Received Packet");
  char rcv[4];
  unsigned int i = 0;
  while (modem.available() || i < 4) {
    rcv[i++] = (char)modem.read();
  }
  
  if(rcv[2] == 0x64){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("(i) Starting the LED");
  } else{
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("(i) Stopping the LED");
  }
}

void loop() {
  int time = millis();
  int gasSensorDataCount = 0;
  int gasSensorSum = 0;
  int gasSensorVal = 0;
  while(time + PACKET_INTERVAL > millis()){
    gasSensorVal = analogRead(gasSensorPin);
    gasSensorSum += gasSensorVal;
    Serial.print("(i) Read Data : ");
    Serial.println(gasSensorVal);    
    gasSensorDataCount++;
    delay(SENSOR_INTERVAL);    
  }

  float gasSensorAvgVal = ((float)gasSensorSum) / ((gasSensorDataCount) * 100.0);

  buildPacket(gasSensorAvgVal);
  sendPacket();
  receivePacket();    
}

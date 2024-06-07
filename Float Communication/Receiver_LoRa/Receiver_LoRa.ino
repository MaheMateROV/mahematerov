/***
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
***/

#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 18
#define rst 23
#define dio0 26
#define SCK 5
#define MISO 19
#define MOSI 27
int counter=0;
String uptimeValue;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  SPI.begin(SCK,MISO,MOSI,ss);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(823E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  
  
}

void loop() {
  counter=0;
  digitalWrite(2,LOW);
  
  if (Serial.available()) {
    
    String data = Serial.readString();

    if (data == "Expand") {

      button();
      }
    else if (data == "Start"){
      button1();
    }
    else if (data.startsWith("uptime")){
      uptimeValue = data.substring(6).toInt();
      button2();
    }
    else if (data == "Barelyup"){
      button3();
    }
    else if (data=="Barelydown"){
      button4();
    }
  }
  // try to parse packet
  

delay(1000);

}
void button()
{
  send_ack();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("he");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("he");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("he");
  LoRa.endPacket();
  delay(100);
  
  while(counter==0){
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet ");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      
      // Check if received message is equal to "poda"
      if (LoRaData == "poda") {
        // Wait for 3 seconds
        counter++;
        
        // Send "he" three times again
        // Exit the loop and continue receiving
        ;
      }
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  }

}
void button1()
{
  send_ack();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("ha");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("ha");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("ha");
  LoRa.endPacket();
  delay(100);
  while(counter==0){
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet ");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      
      // Check if received message is equal to "poda"
      if (LoRaData == "poda") {
        // Wait for 3 seconds
        counter++;
        
        // Send "he" three times again
        // Exit the loop and continue receiving
        ;
      }
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  }

}
void button2()
{
  send_ack();
  LoRa.beginPacket();
  LoRa.print("hi"+uptimeValue);
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("hi"+uptimeValue);
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("hi"+uptimeValue);
  LoRa.endPacket();
  delay(100);
}
void button3()
{
  LoRa.beginPacket();
  LoRa.print("ho");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("ho");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("ho");
  LoRa.endPacket();
  delay(100);
}
void button4()
{
  LoRa.beginPacket();
  LoRa.print("hoe");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("hoe");
  LoRa.endPacket();
  delay(100);
  LoRa.beginPacket();
  LoRa.print("hoe");
  LoRa.endPacket();
  delay(100);
}
void send_ack() {
  digitalWrite(2,HIGH);
  Serial.print("ACK\n");
  delay(100);
}

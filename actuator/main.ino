#include <SPI.h>
#include <UIPEthernet.h>
#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // each arduino should have different MAC address. you can set it to whatever you want if it is an arduino NANO
IPAddress client_ip(192, 168, 2, 100);   // IP address of Arduino
IPAddress server_ip(192, 168, 2, 22);   // IP address of the laptop
unsigned int localPort = 12345;   // Local port number to listen on
char packetBuffer[100];  // Buffer to store incoming packets
unsigned int packetSize;                              // Size of incoming packet
EthernetUDP udp;  // Create an instance of the EthernetUDP class
void write_pin(int pin, int state);
void send_temp_data();

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  if (! sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }
  Serial.println("Found SHT4x sensor");
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);  
  Serial.println("UDP client started");
  Ethernet.begin(mac,client_ip);
  Serial.print("my ip is : ");
  Serial.println(Ethernet.localIP());
  udp.begin(localPort);
  delay(1000);
}

void loop() {
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);
  float val = temp.temperature;
  send_temp_data(val);
  packetSize = udp.parsePacket();
  if (packetSize > 0) {
    udp.read(packetBuffer, packetSize);
    Serial.print("Received packet: ");
    Serial.println(packetBuffer);
  }
  for(int ct=0;ct<4;++ct)
  {
    if(packetBuffer[ct] == '1') write_pin(ct*2+2,HIGH);
    else if(packetBuffer[ct] == '2') write_pin(ct*2+3,HIGH);
    else
    {
      write_pin(ct*2+2,LOW);
      write_pin(ct*2+3,LOW);
    }
  }
  if(packetBuffer[4] == '1') write_pin(A1,HIGH);
  else if(packetBuffer[4] == '2') write_pin(A2,HIGH);
  else
  {
    write_pin(A1,LOW);
    write_pin(A2,LOW);
  }
}

void write_pin(int pin, int state)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,state);
}

void send_temp_data(float value)
{ 
  // Convert the float to a string
  char floatString[10];
  dtostrf(value, 6, 2, floatString);  // Converts float to string with 2 decimal places
  udp.beginPacket(server_ip, localPort);
  udp.write(floatString);
  udp.endPacket();
  //Serial.println("data sent . ");
  // Wait before sending the next packet
  delay(60);
}
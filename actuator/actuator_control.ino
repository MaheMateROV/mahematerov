#include <SPI.h>
#include <UIPEthernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // each arduino should have different MAC address. you can set it to whatever you want if it is an arduino NANO
IPAddress ip(192, 168, 2, 100);   // IP address of Arduino
unsigned int localPort = 12345;   // Local port number to listen on
char packetBuffer[100];  // Buffer to store incoming packets
unsigned int packetSize;                              // Size of incoming packet
EthernetUDP udp;  // Create an instance of the EthernetUDP class

void write_pin(int pin, int state);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  Serial.print("my ip is : ");
  Serial.println(Ethernet.localIP());
  udp.begin(localPort);
  Serial.println("UDP client started");
}

void loop() {
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

#include <SPI.h>
#include <UIPEthernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // each arduino should have different MAC address. you can set it to whatever you want if it is an arduino NANO
IPAddress arduino_ip(192, 168, 2, 100);   // IP address of Arduino
IPAddress server_ip(192, 168, 2, 20);   // IP address of Arduino
unsigned int localPort = 12345;   // Local port number to listen on
EthernetUDP udp;  // Create an instance of the EthernetUDP class

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,arduino_ip);
  Serial.print("my ip is : ");
  Serial.println(Ethernet.localIP());
  udp.begin(localPort);
  Serial.println("UDP client started");
}

void loop() {
  udp.beginPacket(server_ip,localPort);
  udp.write("hello");
  udp.endPacket();
  Serial.println("Message sent to server");
  delay(1000);
}

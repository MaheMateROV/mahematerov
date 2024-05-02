#include <SPI.h>
#include <UIPEthernet.h>

int LPWM_list[5] = {0,0,0,0,0};
int RPWM_list[5] = {0,0,0,0,0};

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // each arduino should have different MAC address. you can set it to whatever you want if it is an arduino NANO
IPAddress arduino_ip(192, 168, 2, 100);       // IP address of Arduino
unsigned int localPort = 12345;       // Local port number to listen on. best to keep it same as the port on the server.
char packetBuffer[6];                 // Buffer to store incoming packets. ive kept it 1 more than actual packet size
unsigned int packetSize;              // Size of incoming packet
EthernetUDP udp;  // Create an instance of the EthernetUDP class

void stop_all_motors();

void setup() {
  for(int ct=0;ct<5;++ct) pinMode(LPWM_list[ct],OUTPUT);
  for(int ct=0;ct<5;++ct) pinMode(RPWM_list[ct],OUTPUT);
  stop_all_motors();
  Serial.begin(9600);
  Ethernet.begin(mac,arduino_ip);
  Serial.print("my ip is : ");
  Serial.println(Ethernet.localIP());
  udp.begin(localPort);
  Serial.println("UDP client started");
}

void loop() {
  packetSize = udp.parsePacket();
  if (packetSize == 5) {
    udp.read(packetBuffer, 5);
    Serial.print("Received packet: ");
    Serial.println(packetBuffer);
    for(int ct=0;ct<5;++ct)
    {
      if(packetBuffer[ct] == '1')
      {
        analogWrite(LPWM_list[ct], 255);
        analogWrite(RPWM_list[ct], 0);
      }
      else if(packetBuffer[ct] == '2')
      {
        analogWrite(LPWM_list[ct], 0);
        analogWrite(RPWM_list[ct], 255);
      }
      else stop_all_motors();
    }
  }
  else stop_all_motors();
}

void stop_all_motors()
{
  for(int ct=0;ct<5;++ct)
  {
    analogWrite(LPWM_list[ct], 0);
    analogWrite(RPWM_list[ct], 0);
  }
}

#include <SPI.h>
#include <UIPEthernet.h>

int LPWM_list[] = {4,7,A0,A2};
int RPWM_list[] = {6,8,A1,A3};

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // each arduino should have different MAC address. you can set it to whatever you want if it is an arduino NANO
IPAddress arduino_ip(192, 168, 2, 100);       // IP address of Arduino
unsigned int localPort = 12345;       // Local port number to listen on. best to keep it same as the port on the server.
char packetBuffer[6];                 // Buffer to store incoming packets. ive kept it 1 more than actual packet size
unsigned int packetSize;              // Size of incoming packet
EthernetUDP udp;  // Create an instance of the EthernetUDP class

void stop_all_motors()
{
  for(int ct=0;ct<4;++ct)
  {
    digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
  }
};

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
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
  if (packetSize == 4) {
    udp.read(packetBuffer, 4);
    Serial.print("Received packet: ");
    Serial.println(packetBuffer);
    for(int ct=0;ct<4;++ct)
    {
      if(packetBuffer[0] == '1')
      {
        
        digitalWrite(2,HIGH);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[0] == '2')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[1] == '1')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[1] == '2')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[2] == '1')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[2] == '2')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    analogWrite(A0, 0);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[3] == '1')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 255);
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      else if(packetBuffer[3] == '2')
      {
        digitalWrite(2,LOW);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(A0, 0);
    analogWrite(A1, 255);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
      }
      
      else stop_all_motors();
    }
  }
  else stop_all_motors();
}

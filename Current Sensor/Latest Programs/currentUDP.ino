#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

const float sensitivity = 0.066; // Sensitivity of ACS712 sensor in volts per ampere

//CHANGE MAC
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress arduino_ip(192, 168, 2, 100);   // IP address of Arduino
IPAddress outIp(192, 168, 1, 100); // replace with your destination's IP
unsigned int outPort = 8888; // replace with your destination's port

EthernetUDP Udp;

int portArray[] = {A0,A1,A2,A3,A4,A5,A6,A7};


String getReading(){
  String text = "";
  int adc = analogRead(portArray[0]); 
  float voltage = adc * (5.0 / 1023.0); 
  float current = (voltage -2.5)/ sensitivity; // Convert voltage to current
  text+=String(current);
  for(int i=1;i<sizeof(portArray)/sizeof(portArray[0]);i++){
    int adc = analogRead(portArray[i]); // Read analog input
    float voltage = adc * (5.0 / 1023.0); // Convert ADC value to voltage (assuming 5V Arduino)
    float current = (voltage -2.5)/ sensitivity; // Convert voltage to current
    text+=","+String(current);
  }
  return text;
}

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,arduino_ip);
  Serial.print("My IP is: ");
  Serial.println(Ethernet.localIP());
  Udp.begin(outPort);
  Serial.println("UDP client started");
}

void loop() {
  String data = getReading();
  Udp.beginPacket(outIp, outPort);
  Udp.write(data.c_str());
  Udp.endPacket();
  Serial.println(data+" sent");
  delay(1000);
}

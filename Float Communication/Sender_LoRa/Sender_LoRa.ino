#include <SPI.h>
#include <SD.h>
#include <LoRa.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "MS5837.h"
#include <TimeLib.h>

MS5837 sensor;
unsigned long lastTime = 0; // Variable to store the last time the file was written
unsigned long currentTime;

int relay1=12;
int relay2=4;
int upbutton=3;
int downbutton = 30;
const char* ssid = "dlink-M960-2.4G-347a";
const char* password = "jcvji36474";
int counter1 = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String data1;
String array[30];
String d;


// LoRa pins
#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK 5
#define CONFIG_NSS 18
#define CONFIG_RST 23
#define CONFIG_DIO0 26

// SD card pins
#define SDCARD_MOSI 15
#define SDCARD_MISO 2
#define SDCARD_SCLK 14
#define SDCARD_CS 13

File myFile;
unsigned long lastWifiConnectAttempt = 0;
unsigned long wifiConnectTimeout = 5000; // 5 seconds timeout
unsigned long startTime = 0;
float naye = 0;
float naye1 = 0;
float naye2 = 0;
int counter = 0;

void setup() {
  pinMode(25,OUTPUT);
  digitalWrite(25,HIGH);
  pinMode(relay1, OUTPUT);  // set pin as output for relay 1
  pinMode(relay2, OUTPUT);  // set pin as output for relay 2
  pinMode(upbutton, INPUT);
  pinMode(downbutton, INPUT);
  pinMode(12, OUTPUT);

  // keep the motor off by keeping both HIGH
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(12, HIGH);

  Serial.begin(115200);
  Wire.begin();
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }

  // .init sets the sensor model for us but we can override it if required.
  // Uncomment the next line to force the sensor model to the MS5837_30BA.
  sensor.setModel(MS5837::MS5837_30BA);

  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  startTime = millis(); // Get start time for timeout calculation

  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < wifiConnectTimeout) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");

    // Initialize NTP client
    timeClient.begin();

    // Wait for synchronization
    while (!timeClient.update()) {
      timeClient.forceUpdate();
      delay(100);
    }

    // Add timezone offset
    timeClient.setTimeOffset(4 * 3600);  // GMT+4 in seconds

    // Print current time adjusted for timezone
    Serial.println(timeClient.getFormattedTime());

    WiFi.disconnect(true);  // Disconnect from WiFi after getting time
  } else {
    Serial.println("Failed to connect to WiFi. Setting default time to 00:00:00");
    setTime(0); // Set default time to 00:00:00 if WiFi connection fails
  }
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);

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
  array[30]="";
  Serial.print("h");
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      if (LoRaData == "he") {
        executeCode();
        break;
      }
      else if (LoRaData == "ha"){
        executeCode2();
        break;
      }
      else if (LoRaData.startsWith("hi")){
        upbutton=LoRaData.substring(2).toInt();
        downbutton=LoRaData.substring(2).toInt();
        break;
      }
      else if (LoRaData=="ho"){
        executeCode3();
      }
      else if (LoRaData=="hoe"){
        executeCode4();
      }
    }// print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  delay(100); // Add a small delay to prevent high CPU usage
}

void executeCode() {
  int naye=0;
  int naye1=0;
  int naye2=0;
  int total=6;
  counter=0;
  SPI.end();
  SPIClass sdSPI(VSPI);
  sdSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
  if (!SD.begin(SDCARD_CS, sdSPI)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  if (SD.remove("/test.txt")) {
    Serial.println("File deleted successfully!");
  } else {
    Serial.println("Error deleting file!");
  }

  myFile = SD.open("/test.txt", FILE_WRITE);
  myFile.close(); 
  myFile = SD.open("/test.txt", FILE_APPEND);
   // turn relay 2 OFF
  // if the file opened okay, write to it:
  if (myFile) {
    while (naye<upbutton) {
      currentTime=millis();
      Serial.print(digitalRead(upbutton));
      Serial.print("Writing to test.txt...");
      if (currentTime - lastTime >= 5000) {
        Wire.begin();
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }

  // .init sets the sensor model for us but we can override it if required.
  // Uncomment the next line to force the sensor model to the MS5837_30BA.
  sensor.setModel(MS5837::MS5837_30BA);

  sensor.setFluidDensity(997);
        sensor.read();
        total++;
        lastTime = currentTime;
        Serial.print("reah");
        myFile.println("Jalpari," + timeClient.getFormattedTime() + "," + (sensor.pressure() * 100) + "," + sensor.depth());
        digitalWrite(relay1, LOW);   // turn relay 1 ON
        digitalWrite(relay2, HIGH);
    } else if ((currentTime - lastTime) >= 4000 && (currentTime - lastTime) < 5000) {
        Serial.print("ree");
        digitalWrite(relay1, HIGH);   // turn relay 1 OFF
        digitalWrite(relay2, HIGH);   // turn relay 2 ON
    } else if (currentTime - lastTime >= 5000) {
        digitalWrite(relay2, LOW);   // turn relay 2 OFF
}

      // close the file:
      Serial.println("done.");
      naye++;
      delay(1000);
    }
    Serial.print("finished");
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  Serial.print("here");
  digitalWrite(relay1, HIGH);  // turn relay 1 ON
  digitalWrite(relay2, HIGH);  // turn relay 2 OFF
  myFile = SD.open("/test.txt", FILE_APPEND);
    // turn relay 2 OFF
  if (myFile) {
    while (naye2<downbutton) {
      Serial.print(digitalRead(downbutton));
      Serial.print("Writing to test.txt...");
      currentTime=millis();
      if (currentTime - lastTime >= 5000) {
        Wire.begin();
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }

  // .init sets the sensor model for us but we can override it if required.
  // Uncomment the next line to force the sensor model to the MS5837_30BA.
  sensor.setModel(MS5837::MS5837_30BA);

  sensor.setFluidDensity(997);
        sensor.read();
        total++;
        lastTime = currentTime;
        Serial.print("reah");
        myFile.println("Jalpari," + timeClient.getFormattedTime() + "," + (sensor.pressure() * 100) + "," + sensor.depth());
        digitalWrite(relay1, HIGH);   // turn relay 1 ON
        digitalWrite(relay2, LOW);
    } else if ((currentTime - lastTime) >= 4000 && (currentTime - lastTime) < 5000) {
        Serial.print("ree");
        digitalWrite(relay1, HIGH);   // turn relay 1 OFF
        digitalWrite(relay2, HIGH);   // turn relay 2 ON
    } else if (currentTime - lastTime >= 5000) {
        digitalWrite(relay1, LOW);   // turn relay 2 OFF
}

      // close the file:
      Serial.println("done.");
      naye2++;
      
      delay(1000);
    }
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  digitalWrite(relay1, HIGH);  // turn relay 1 ON
  digitalWrite(relay2, HIGH);  // turn relay 2 OFF
  delay(1000);

  if (SDCARD_CS > 0) {
    if (!SD.begin(SDCARD_CS, sdSPI)) {
      Serial.print("SD Card Fail");
    } else {
      Serial.print("SD Card works");
      myFile = SD.open("/test.txt");
      while (myFile.available()) {
        char c = myFile.read();
        d = String(c);
        if (d == "\n") {
          array[counter] = data1;
          data1 = "";
          counter++;
          delay(200);
        } else {
          data1 += c;
        }
      }
      myFile.close();
    }
  }
  sdSPI.end();
  Serial.print("done");
  for (int i = 0; i < 30; i++) {
    Serial.print(array[i]);
  }
  SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
  LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
  while (!LoRa.begin(823E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  Serial.print(total);
  for (int i = 0; i <= total; i++) {
    if (array[i] != "") {
      LoRa.beginPacket();
      Serial.print(array[i]);
      LoRa.print(array[i]);
      LoRa.print("\n");
      LoRa.endPacket();
    }
   
    delay(200);
    
  }
  LoRa.beginPacket();
  LoRa.print("poda");
  LoRa.endPacket();
  delay(1000);
  naye1=0;
}

void executeCode2()
{
  LoRa.beginPacket();
  LoRa.print("\n");
  LoRa.endPacket();
  for (int k=0;k<3;k++){
    sensor.read();
    LoRa.beginPacket();
    LoRa.print("Jalpari,"+timeClient.getFormattedTime() + ","+(sensor.pressure()*100)+","+sensor.depth());
    LoRa.print("\n");
    Serial.print("Jalpari,"+timeClient.getFormattedTime() + ","+(sensor.pressure()*100)+","+sensor.depth());
    LoRa.endPacket();
    delay(1000);

  }
  LoRa.beginPacket();
  LoRa.print("poda");
  LoRa.endPacket();
  delay(1000);
}
void executeCode3(){
  digitalWrite(relay1, HIGH);   // turn relay 1 ON
  digitalWrite(relay2, LOW);
  delay(2000);
  digitalWrite(relay1, HIGH);   // turn relay 1 ON
  digitalWrite(relay2, HIGH);

}
void executeCode4(){
  digitalWrite(relay1, LOW);   // turn relay 1 ON
  digitalWrite(relay2, HIGH);
  delay(2000);
  digitalWrite(relay1, HIGH);   // turn relay 1 ON
  digitalWrite(relay2, HIGH);
}

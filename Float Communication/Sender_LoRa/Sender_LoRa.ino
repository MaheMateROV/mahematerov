#include <SPI.h>
#include <SD.h>
#include <LoRa.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;

int relay1 = 21;
int relay2 = 22;
int upbutton = 34;
int downbutton = 35;
const char* ssid = "MU-STUDENT-1";
const char* password = "mahedubai";
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
int naye = 0;
int naye1 = 0;
int naye2 = 0;
int counter = 0;

void setup() {
  pinMode(relay1, OUTPUT);  // set pin as output for relay 1
  pinMode(relay2, OUTPUT);  // set pin as output for relay 2
  pinMode(upbutton, INPUT);
  pinMode(downbutton, INPUT);
  pinMode(12, OUTPUT);

  // keep the motor off by keeping both HIGH
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
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
  //sensor.setModel(MS5837::MS5837_30BA);

  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
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

  // Disconnect from WiFi
  WiFi.disconnect(true);
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
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  delay(100); // Add a small delay to prevent high CPU usage
}

void executeCode() {
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
  digitalWrite(relay1, LOW);   // turn relay 1 ON
  digitalWrite(relay2, HIGH);  // turn relay 2 OFF
  // if the file opened okay, write to it:
  if (myFile) {
    while (digitalRead(upbutton) == LOW) {
      Serial.print(digitalRead(upbutton));
      Serial.print("Writing to test.txt...");
      if (naye % 5 == 0) {
        sensor.read();
        myFile.println("Jalpari,"+timeClient.getFormattedTime() + ","+sensor.pressure()+","+sensor.depth());
      }
      // close the file:
      Serial.println("done.");
      naye++;
      total++;
      delay(1000);
    }
    Serial.print("finished");
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  Serial.print("here");
  myFile = SD.open("/test.txt", FILE_APPEND);
  digitalWrite(relay1, LOW);  // turn relay 1 ON
  digitalWrite(relay2, LOW);  // turn relay 2 OFF
  if (myFile) {
    while (naye1 <= 5) {
      Serial.print("Writing to test.txt...");
      if (naye1 % 5== 0) {
        sensor.read();
        myFile.println("Jalpari,"+timeClient.getFormattedTime() + ","+sensor.pressure()+","+sensor.depth());
      }
      // close the file:
      Serial.println("done.");
      naye1++;
      delay(1000);
    }
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  myFile = SD.open("/test.txt", FILE_APPEND);
  digitalWrite(relay1, HIGH);  // turn relay 1 ON
  digitalWrite(relay2, LOW);   // turn relay 2 OFF
  if (myFile) {
    while (digitalRead(downbutton) == LOW) {
      Serial.print(digitalRead(downbutton));
      Serial.print("Writing to test.txt...");
      if (naye2 % 5 == 0) {
        sensor.read();
        myFile.println("Jalpari,"+timeClient.getFormattedTime() + ","+sensor.pressure()+","+sensor.depth());
      }
      // close the file:
      Serial.println("done.");
      naye2++;
      total++;
      delay(1000);
    }
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  digitalWrite(relay1, LOW);  // turn relay 1 ON
  digitalWrite(relay2, LOW);  // turn relay 2 OFF
  delay(5000);

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
          delay(1000);
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
   
    delay(100);
    
  }
  LoRa.beginPacket();
  LoRa.print("poda");
  LoRa.endPacket();
  delay(1000);
  naye1=0;
  

}

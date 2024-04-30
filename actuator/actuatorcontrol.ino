int RPWMEX = 9;
int LPWMCT = 10;
void setup() {
  // Start the serial communication
  Serial.begin(9600);
  pinMode(RPWMEX, OUTPUT);
  pinMode(LPWMCT, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readString();

    if (data == "Expand") {
      analogWrite(RPWMEX, 254);analogWrite(LPWMCT, 0);
    } else if (data == "Contract") {
      analogWrite(LPWMCT, 254);analogWrite(RPWMEX, 0);
    }
     else if (data == "Stop") {
      analogWrite(LPWMCT, 0);analogWrite(RPWMEX, 0);
    }
  }
}

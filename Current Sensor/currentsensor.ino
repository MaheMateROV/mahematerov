const float sensitivity = 0.066; // Sensitivity of ACS712 sensor in volts per ampere

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void readcurrent(int port, int i){
  int adc = analogRead(port); // Read analog input
  float voltage = adc * (5.0 / 1023.0); // Convert ADC value to voltage (assuming 5V Arduino)
  float current = (voltage -2.5)/ sensitivity; // Convert voltage to current
  //if (current<0) current=0;
  if (i==1) {Serial.print(current, 4); Serial.print(',');}// Print current with 2 decimal places
  else Serial.println(current, 4); // Print current with 2 decimal places
}

void loop() {
  // put your main code here, to run repeatedly:
  readcurrent(A0,1);
  readcurrent(A1,1);
  readcurrent(A2,1);
  readcurrent(A3,0);
  
  delay(100); // Optional delay for readability
}
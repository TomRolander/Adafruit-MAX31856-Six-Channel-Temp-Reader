#include <Adafruit_MAX31856.h>

#define VERSION "Ver 1.0 Six Channel Temp Reader"

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 max[6] = {
  Adafruit_MAX31856(10, 11, 12, 13),
  Adafruit_MAX31856( 9, 11, 12, 13),
  Adafruit_MAX31856( 8, 11, 12, 13),
  Adafruit_MAX31856( 7, 11, 12, 13),
  Adafruit_MAX31856( 6, 11, 12, 13),
  Adafruit_MAX31856( 5, 11, 12, 13)
};

// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 max = Adafruit_MAX31856(10);

void setup() {
  Serial.begin(9600);
  Serial.println("MAX31856 thermocouple test");
  Serial.println("Thermocouple type: ");
  
  for (int i=0; i<6; i++) {
    max[i].begin();
    max[i].setThermocoupleType(MAX31856_TCTYPE_K);

    Serial.print(i+1);
    switch ( max[i].getThermocoupleType() ){
      case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
      case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
      case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
      case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
      case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
      case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
      case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
      case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
      case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
      case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
      default: Serial.println("Unknown"); break;
    }
    max[i].setTempFaultThreshholds(0.0, 200.0);    
  }
}


void loop() {
//  Serial.print("Cold Junction Temp: "); Serial.println(max.readCJTemperature());

  String data;
  while (Serial.available() == 0);
  data = Serial.readString();
  data.toUpperCase();

  if (data[0] == 'V') {
    Serial.println(VERSION);
    return;
  }
  if (data[0] != "T")
    return;

  if ((data[1] >= '1' && data[1] <= '6') || data[1] == '*') {
    for (int i=0; i<6; i++) {
      char buf[120];
      float temp;
      if (data[1] == '*' || data[1] == i+'1') {
        temp = max[i].readThermocoupleTemperature();
        
        // Check and print any faults
        uint8_t fault = max[i].readFault();
        if (fault) {
          if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
          if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
          if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
          if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
          if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
          if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
          if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
          if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
        }  
       
        sprintf("T%d %f", i+1, temp);
        Serial.println(buf);
      }
    }
  }  
}

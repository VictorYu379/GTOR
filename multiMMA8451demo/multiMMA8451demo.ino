#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define TCAADDR 0x70

Adafruit_MMA8451 mma[4] = Adafruit_MMA8451();

int sensornum = 0;
int loopnum = 0;

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Adafruit MMA8451 test!");

  for(int i = 0; i < 4; i++) {
    tcaselect(i);
    if (! mma[i].begin()) {
      Serial.println("Couldnt start");
      while (1);
    }
    Serial.println("MMA8451 found!");
  
    mma[i].setRange(MMA8451_RANGE_2_G);
  
    Serial.print("Range = "); Serial.print(2 << mma[i].getRange());  
    Serial.println("G");  
  }

}

void loop() {
  sensornum = loopnum % 4;
  Serial.print("Sensor Number: ");
  Serial.println(sensornum);
  // Read the 'raw' data in 14-bit counts
  mma[sensornum].read();
  Serial.print("X:\t"); Serial.print(mma[sensornum].x); 
  Serial.print("\tY:\t"); Serial.print(mma[sensornum].y); 
  Serial.print("\tZ:\t"); Serial.print(mma[sensornum].z); 
  Serial.println();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma[sensornum].getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  
  /* Get the orientation of the sensor */
  uint8_t o = mma[sensornum].getOrientation();
  
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.println("Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      Serial.println("Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.println("Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.println("Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.println("Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.println("Landscape Right Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.println("Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.println("Landscape Left Back");
      break;
    }
  Serial.println(millis());
  if (sensornum == 3) {
    delay(10);
  } else {
    delay(10);
  }
  
  loopnum++;
}

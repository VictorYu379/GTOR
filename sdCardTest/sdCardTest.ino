#include "SD.h"
#include <Wire.h>
#include "RTClib.h"

// the digital pins that connect to the LEDs
#define redLEDpin 3
#define greenLEDpin 4

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

//line counter
int line;

// the logging file
File logfile;

void error(char *str) 
{
  Serial.print("error: ");
  Serial.println(str);

  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while (1);
}

RTC_DS1307 RTC;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);

  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }

  if (!logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);

  logfile.println("input,someNumber");
  Serial.println("input,someNumber");

//  if (logfile.writeError || !logfile.sync()) {
//    Serial.println("write header error");
//    while (1);
//  }

 
  line = 0;

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    String input = Serial.readString();
    logfile.print(input);
    logfile.print(",");
    logfile.print(line);
    logfile.println();
   
    
    Serial.print(input);
    Serial.print(",");
    Serial.print(line);
    Serial.println();

    logfile.flush();
    
    
//    if (logfile.writeError || !logfile.sync()) {
//      Serial.println("write header error");
//      while (1);
//    }
    line++;
  }



}

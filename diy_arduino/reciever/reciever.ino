
/*
* Reciever
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(40,53);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

int dataToWrite;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Init srv"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  // Start the radio listening for data
  radio.startListening();

  //Init all the ports (refer to port-mapping.txt)
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, INPUT);
    pinMode(8, INPUT);

    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
}

void loop() {
    int data;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &data, sizeof(int) );                           // Get the payload
      }

      dataToWrite = decypherAction(data);
      
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &dataToWrite, sizeof(int) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
   }
}


int decypherAction (int dataInt){   //Write data
  String data = String(dataInt);
  
  int portCode = data.substring(0,3).toInt();
  int arduinoPort = portCode - 100;
    
  bool valueToWrite = data.substring(3,4) == "1"; //force :)
  bool shouldRead = data.length() == 3;
  bool readValue = digitalRead(arduinoPort);

  printStatus(portCode, arduinoPort, valueToWrite, shouldRead, readValue);

  if(shouldRead)
    return readValue;
  else {
    digitalWrite(arduinoPort, valueToWrite);
    return dataInt;
  }
}

void printStatus(int portCode, int arduinoPort, bool valueToWrite, bool shouldRead, bool readValue){
  Serial.println("");
  Serial.print("---------------------");
  Serial.println("");
  Serial.print(F(" Should Read: "));
  Serial.print(shouldRead);
  Serial.println("");
  
  Serial.print(F(" Data To Write: "));
  Serial.print(valueToWrite);
  Serial.println("");

  Serial.print(F(" Arduino Port: "));
  Serial.print(arduinoPort);
  Serial.println("");
  
  Serial.print(F(" Port Code: "));
  Serial.print(portCode);
  Serial.println("");
  Serial.print("---------------------");

  if(shouldRead) {
    Serial.println("");
    Serial.print(F(" Digital Read: "));
    Serial.print(readValue);
    Serial.println("");
    Serial.print("---------------------");
  }
  else {
    Serial.println("");
    Serial.print(F(" Write: "));
    Serial.print(valueToWrite);
    Serial.print(F(", to port: "));
    Serial.print(arduinoPort);
    Serial.println("");
    Serial.print("---------------------");
  }

}

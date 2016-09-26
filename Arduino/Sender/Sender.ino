
/*
* Sender
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;
bool sendingData = false;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  Serial.println(F("Init"));  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);

  // Start the radio listening for data
  radio.startListening();
}

void loop() {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    unsigned long start_time = millis();               // Set up a timeout period, get the current microseconds
    
    checkSerial();
 
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = millis();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (millis() - started_waiting_at > 500 ){            // If waited longer than 500ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }

    if(!sendingData){
    }
    else if ( timeout && sendingData ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
        sendingData = false;
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = millis();
        
        // Spew it
        Serial.print(F("Sent Data"));
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" milliseconds"));
    }

    // Try again 1s later
    delay(500);
}

void sendData(unsigned long data) {
    Serial.println(F("Now sending"));
    sendingData = true;
    if (!radio.write( &data, sizeof(int) )){ // Take the time, and send it.  This will block until complete
       Serial.println(F("failed"));
     }
}

void checkSerial() {
if ( Serial.available() )
  {
    int d = Serial.parseInt();
    Serial.print(F("Serial.Read: "));
    Serial.println(d);   
    sendData(d);
  }
}


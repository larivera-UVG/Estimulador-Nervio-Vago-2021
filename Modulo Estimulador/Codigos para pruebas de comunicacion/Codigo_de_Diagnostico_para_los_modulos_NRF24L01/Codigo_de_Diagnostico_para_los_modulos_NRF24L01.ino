#include <SPI.h> 
#include <nRF24L01.h>   //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <RF24.h>          //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include "printf.h" 

const int pinCE = 0; 
const int pinCSN = 2;

RF24 wirelessSPI(pinCE, pinCSN);
 
const uint64_t pAddress = 0xB00B1E5000LL;            



void setup()  
{
  Serial.begin(9600);   
  printf_begin();       
  wirelessSPI.begin();          
  wirelessSPI.setAutoAck(1);          
  wirelessSPI.enableAckPayload();             
  wirelessSPI.setRetries(5,15);                 
  wirelessSPI.openWritingPipe(pAddress);        
  wirelessSPI.stopListening();
  wirelessSPI.printDetails();                  

}


void loop()  
{  
  
 
}

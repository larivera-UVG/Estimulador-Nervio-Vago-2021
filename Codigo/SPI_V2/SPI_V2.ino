//Programa para pruebas de Multiplexado del puerto SPI utilizando el Trinket MO/SAMD21E18
//Probado para comunicacion inalambrica utilizando el nRF24I01 y un arduino
// que toma como referencia las instrucciones de https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-spi
//https://create.arduino.cc/projecthub/muhammad-aqib/nrf24l01-interfacing-with-arduino-wireless-communication-0c13d4
#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(0, 1); // CE, CSN   
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//int button_pin = 2;  
//SPIClass mySPI (&sercom2, 3, 5, 4, SPI_PAD_0_SCK_3, SERCOM_RX_PAD_1);

void setup() {
  //PORT->Group[PORTA].DIRSET.reg= PORT_PA05;
  //PORT->Group[PORTA].OUTSET.reg= PORT_PA05;
  //PORT->Group[PORTA].DIRSET.reg= PORT_PA06;
  //PORT->Group[PORTA].OUTSET.reg= PORT_PA06;
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
//  // Assign pins 3, 4, 5 to SERCOM & SERCOM_ALT
//  pinPeripheral(3, PIO_SERCOM_ALT);
//  pinPeripheral(4, PIO_SERCOM_ALT);
//  pinPeripheral(5, PIO_SERCOM);
}

uint8_t i=0;
void loop() {
  Serial.println(i);
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(i++);
  SPI.endTransaction();
  radio.write(&i,sizeof(i));

 //  delay(100);
}
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
//RF24 radio(0, 1); // CE, CSN         
//const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
////int button_pin = 2;
//boolean button_state = 0;
//void setup() {
////pinMode(button_pin, INPUT);
//radio.begin();                  //Starting the Wireless communication
//radio.openWritingPipe(address); //Setting the address where we will send the data
//radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
//radio.stopListening();          //This sets the module as transmitter
//}
//void loop()
//{
////button_state = digitalRead(button_pin);
//if(button_state == HIGH)
//{
//const char text[] = "Your Button State is HIGH";
//radio.write(&text, sizeof(text));                  //Sending the message to receiver
//}
//else
//{
//const char text[] = "Your Button State is LOW";
//radio.write(&text, sizeof(text));                  //Sending the message to receiver 
//}
//radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver 
//delay(1000);
//}

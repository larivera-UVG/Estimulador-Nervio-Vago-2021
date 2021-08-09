 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(0, 1); // CE, CSN // Usando la libreria de Radio Frecuencia, Indicamos cual va a ser nuestro pin de CE y CSN

const byte address[6] = "00001"; //A donde vamos a mandar la informacion
uint8_t i=0;
const char text[] = "Your Button State is HIGH";
void setup() {
  // put your setup code here, to run once:
  //PORT->Group[PORTA].DIRSET.reg= PORT_PA14; //Inicializamos el Pin 14 como CSN (CHip Select NOT), En salida
  //PORT->Group[PORTA].DIRSET.reg= PORT_PA15;// Inicializamos el Pin 15 como CE(Control data transmission), En salida
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();  
  
// PORT->Group[PORTA].OUTSET.reg = PORT_PA14;                        // Set CSN Pin to HIGH
 
}

void loop() {
  PORT->Group[PORTA].OUTCLR.reg = PORT_PA14;                        // Set CSN Pin to LOW
  long converted_T= atol(text);
  //Serial.println(i);
  Serial.println(converted_T);
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  //SPI.transfer(i++);
  SPI.transfer(converted_T);
  SPI.endTransaction();
  //radio.write(&text,sizeof(text));
  PORT->Group[PORTA].OUTSET.reg = PORT_PA14;  

}

//ESP Mandando datos y esperando mensaje de confirmacion
/************************************************************************
   Titulo: Prueba de Emision y Recepcion de ESP8266 y SAMD21 con nRF4L01
   Receptor SAMD21
   Fecha: 29/06/21
   Autor: Rodrigo Figueroa
   Version: 1.0
 ***********************************************************************/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/**********************************************************
   Inicializamos las variables
 *******************************************************/
//RF24 radio(0, 1); // CE, CSN //Trinket Receptor, ESP Emisor
//RF24 radio(9, 10); // CE, CSN Arduino
RF24 radio(2, 0); // CE= D3, CSN= D4 //Trinket Receptor, ESP8266 Emisor
//RF24 radio(4, 5); // CE= D3, CSN= D4 //Trinket Receptor, ESP8266 Emisor
bool leido = false;
int prueba = 1;
bool confirmacion1 = false;
bool confirmacion2 = false;
bool confirmacion3 = false;
int textint = 0;
char dataBus[7]; // Bus de parámetros
/**************************************************************************************************
   Inicializamos tanto la comunicacion Serial como la comunicacion SPI usando la libreria RadioHead
 **************************************************************************************************/
const byte address[6] = "00001";
const byte address2[6] = "00002";
void setup() {
  Serial.begin(115200);
  radio.begin();
    radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
    radio.openWritingPipe(address2); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.setChannel(108); 
  //radio.setDataRate( RF24_250KBPS );
  radio.startListening();              //This sets the module as receiver
  confirmacion1 = false;
  confirmacion2 = false;
  confirmacion3 = false;
}
/**********************************************************
   Iniciamos con el Loop
 *******************************************************/
void loop()
{
  /**********************************************************
     Le Decimos que mande un mensaje inicial para comenzar
     la comunicacion, luego que espere hasta recibir el mensaje
     de confirmacion de parametros para poder enviar los parametros
     correspondientes
   *******************************************************/
  switch (prueba) {
    /////////////////////////////////////////////////////////////////////////////////////
    case 1: {
        if (radio.available())              //Looking for the data.
        {
          char text[4] = "";                 //Saving the incoming data
          radio.read(&text, sizeof(text));    //Reading the data
          Serial.println("El Dice:");
          Serial.println(text);
          leido = true;
          delay(20);
        }
        //if(leido == true){
        radio.stopListening();
        Serial.println("Yo digo:");
        const char text2[] = "abcdefg";
        Serial.println(text2);
        radio.write(&text2, sizeof(text2));
        delay(20);
        radio.startListening();
        break;
      }
    //////////////////////////////////////////////////////////////////////////////////////
    case 2: {
        while (confirmacion1 == false) {
          radio.startListening();
          if (radio.available())              //Looking for the data.
          {
            char text[2] = "";                 //Saving the incoming data
            radio.read(&text, sizeof(text));    //Reading the data
            Serial.println("Tu Dices:");
            Serial.println(text);
            if (strcmp(text, "1") == 0) {
              confirmacion1 = true;
              Serial.println("Confirmacion de Dato 1 Aceptada");
            }
            delay(20);
          }
          radio.stopListening();
          const char text2[] = "2";
          Serial.println("Yo digo:");
          Serial.println(text2);
          radio.write(&text2, sizeof(text2));
          delay(20);
        }
        while (confirmacion1 == true && confirmacion2 == false) {
          radio.startListening();
          char text3[1] = "";                 //Saving the incoming data
          radio.read(&text3, sizeof(text3));    //Reading the data
          Serial.println("Tu Dices:");
          Serial.println(text3);
          if (strcmp(text3, "3") == 0) {
            confirmacion2 = true;
            Serial.println("Confirmacion de Dato 2 Aceptada");
          }
          delay(20);
        }
        radio.startListening();
      }
    //////////////////////////////////////////////
    case 3: {
        radio.stopListening();
        const char text3[] = "abc";
        Serial.println(text3);
        radio.write(&text3, sizeof(text3));                  //Sending the message to receiver
        delay(20);
        break;
        //        const char text2[] = "abc";
        //        Serial.println(text2);
        //        radio.write(&text2, sizeof(text2));
        //        delay(20);
        //        radio.startListening();
        //        if (radio.available())              //Looking for the data.
        //        {
        //          char text[1] = "";                 //Saving the incoming data
        //          radio.read(&text, sizeof(text));    //Reading the data
        //          //textint = int(float(text));
        //          Serial.println(text);
        //        }
        //        delay(5);

      }
    ////////////////////////////////////////////////////////
    case 4: {
        if (radio.available())              //Looking for the data.
        {
          char text[2] = "";                 //Saving the incoming data
          radio.read(&text, sizeof(text));    //Reading the data
          //textint = int(float(text));
          Serial.println(text);
        }
        delay(5);
        break;
      }
    ///////////////////////////////////////////////////////////////////
    case 5: {
        radio.startListening(); // Módulo RF como receptor
        if (radio.available()) // Si hay datos en el pipe
        {
          Serial.println("Si hay datos");
          radio.read(&dataBus, sizeof(dataBus)); // Leer y almacenar en dataBus
          Serial.println(dataBus[0]);
          Serial.println(dataBus[1]);
          Serial.println(dataBus[2]);
          Serial.println(dataBus[3]);
          Serial.println(dataBus[4]);
          Serial.println(dataBus[5]);
          //          if (dataBus[5] == 1) { // Si el bit 5 de dataBus es 1 (recepción confirmada)
          //            dataControl = 1; // Ya no se quiere leer
          //            contSec++;
          //          }
        }
        delay(5);
        int modo = dataBus[0];
        int ancho = dataBus[1];
        int tiempo = dataBus[2];
        int freq = dataBus[3];
        int sleep = dataBus[4];

        // Se muestran en el monitor serial las opciones seleccionadas de los parámetros
        //Serial.print("Reception Number: ");
        //Serial.println(contSec);
        Serial.print("Modo de Operación: ");
        //Serial.print("Operating Mode: ");
        if (modo == 1)
        {
          Serial.println("Estimulación");
          //Serial.println("Stimulation");
        }
        else if (modo == 2)
        {
          Serial.println("Programación");
          //Serial.println("Programming");
        }
        else if (modo == 3)
        {
          Serial.println("Reposo");
          //Serial.println("Rest");
        }

        Serial.print("Ancho de Pulso: ");
        //Serial.print("Pulse width: ");
        if (ancho == 1)
        {
          Serial.println("250 us");
        }
        else if (ancho == 2)
        {
          Serial.println("500 us");
        }
        else if (ancho == 3)
        {
          Serial.println("25%");
        }
        else if (ancho == 4)
        {
          Serial.println("50%");
        }
        else if (ancho == 5)
        {
          Serial.println("75%");
        }

        Serial.print("Tiempo de Estimulación: ");
        //Serial.print("ON TIME: ");
        if (tiempo == 1)
        {
          Serial.println("30 s");
        }
        else if (tiempo == 2)
        {
          Serial.println("60 s");
        }
        else if (tiempo == 3)
        {
          Serial.println("Tiempo 3");
        }
        else if (tiempo == 4)
        {
          Serial.println("Tiempo 4");
        }
        else if (tiempo == 5)
        {
          Serial.println("Tiempo 5");
        }

        Serial.print("Frequencia: ");
        //Serial.print("Frequency: ");
        if (freq == 1)
        {
          Serial.println("490.20 Hz (Default)");
        }
        else if (freq == 2)
        {
          Serial.println("30.64");
        }
        else if (freq == 3)
        {
          Serial.println("122.50 Hz");
        }
        else if (freq == 4)
        {
          Serial.println("245.10 Hz");
        }
        else if (freq == 5)
        {
          Serial.println("980.39 Hz");
        }
        else if (freq == 6)
        {
          Serial.println("3921.16 Hz");
        }
        else if (freq == 7)
        {
          Serial.println("31372.55 Hz");
        }
        else if (freq == 8)
        {
          Serial.println("Frecuencia 8");
        }

        Serial.print("Tiempo de apagado: ");
        //Serial.print("OFF Time: ");
        if (sleep == 1)
        {
          Serial.println("5 min");
        }
        else if (sleep == 2)
        {
          Serial.println("NO");
        }

      }
  }
}

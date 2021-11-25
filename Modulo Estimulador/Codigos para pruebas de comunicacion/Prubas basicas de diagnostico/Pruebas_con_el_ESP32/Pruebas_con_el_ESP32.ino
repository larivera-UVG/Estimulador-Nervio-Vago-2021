#include <WiFi.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
String apiKey = "C25ICK6FHOR7PST4";
 
const char* ssid = "Familia Juarez";
const char* password = "max2017$";
 
const char* server = "api.thingspeak.com";
 
RF24 radio(4, 5); 
const uint64_t address = 0xF0F0F0F0E1LL;
 
struct MyData 
{
  int counter;
  float temperature;
  float humidity;
  float altitude;
  float pressure;
};
MyData data;
 
WiFiClient client;
 
void setup() 
{
  Serial.begin(115200);
  radio.begin();
  
 
  Serial.println("Receiver Started....");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}
 
int recvData()
{
  if ( radio.available() ) 
  {
    radio.read(&data, sizeof(MyData));
    return 1;
    }
    return 0;
}
 
 
void loop()
{
  if(recvData())
  {
 
  Serial.println("Data Received:");
  Serial.print("Packet No. = ");
  Serial.println(data.counter);
  
  Serial.print("Temperature = ");
  Serial.print(data.temperature);
  Serial.println("*C");
 
  Serial.print("Pressure = ");
  Serial.print(data.pressure);
  Serial.println("hPa");
 
  Serial.print("Approx. Altitude = ");
  Serial.print(data.altitude);
  Serial.println("m");
 
  Serial.print("Humidity = ");
  Serial.print(data.humidity);
  Serial.println("%");
 
  Serial.println();
 
  if (client.connect(server, 80)) 
  {
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(data.temperature);
        postStr += "&field2=";
        postStr += String(data.pressure);
        postStr += "&field3=";
        postStr += String(data.altitude);
        postStr += "&field4=";
        postStr += String(data.humidity);
        postStr += "\r\n\r\n\r\n\r\n";
        
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        delay(1000);
        Serial.println("Data Sent to Server");
      }
        client.stop();
  }
}

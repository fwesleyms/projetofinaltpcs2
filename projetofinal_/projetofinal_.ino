#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <SimpleTimer.h>


String apiKey = "544RO609IAMS6H32"; //  seu Write API key do site ThingSpeak
const char *ssid = "lixo"; //  substitua com o ssid e senha da rede Wifi
const char *pass = "wesley123";
const char* server = "api.thingspeak.com";
char auth[] = "416b8707426d4402846008b896aab2f4";
WiFiClient client;

// defines pins numbers

//Pinos sensor de distância
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

//Sensor de humi/temp

#define DHTPIN 13
DHT dht(DHTPIN, DHT11);
SimpleTimer timer;

// defines variables
long duration; //sensor de distância
int distance; //sensor de distância

void setup() {
  //Definições da conexão sem fio
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass);

  //Definição dos pinos do sensor de disTância
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output //Sensor de distância
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input //Sensor de distância
  Serial.begin(9600); // Starts the serial communication //Sensor de distância
  dht.begin();

}

void loop() {

  //Sensor de distância 

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê o pino echo, retorna o tempo de viagem da onda de som em microssegundos
  duration = pulseIn(echoPin, HIGH);

  // Calculando a distancia
  distance = duration * 0.034 / 2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(2000);
  Blynk.virtualWrite(8,distance);// virtualpin 8 distance
  
  // ||||||||||||||||||||||||||||||||||||||||||||

  // Aqui começa a parte do sensor LDR

  int sensorValue = analogRead(A0);   // read the input on analog pin 0

  float voltage = sensorValue * (5.0 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)

  Serial.println(voltage);   // print out the value you read
  int lumi;
  lumi = voltage * 20;
  Serial.print("Luminosidade em: ");
  Serial.print(lumi);
  Serial.println(" %");

  //Aqui termina a parte do sensor LDR

  // Começa a parte do humidade e Temperatura
    
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
 if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else 
  {  

  Serial.print("Umidade: ");
    Serial.print(h);
    Serial.println(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
    Serial.println("");
    Serial.println("******************");
    Serial.println("");
    Serial.println("");
    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V6, t);

  }
  // Termina a parte do humidade temperatura

  // Começa a parte que transmite pro thingspeak
      if (client.connect(server,80)) // "184.106.153.149" or api.thingspeak.com
    {
        String postStr = apiKey;
        postStr +="&field1="; // atenção, esse é o campo 1 que você escolheu no canal do ThingSpeak Temperatura
        postStr += String(t);
        postStr +="&field2=";
        postStr += String(h);
        postStr +="&field3=";
        postStr += String(voltage);
        postStr +="&field4=";
        postStr += String(distance);
        postStr += "\r\n\r\n";
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print(" degrees Celcius, Humidity: ");
        Serial.print(h);
        Serial.println("%. Send to Thingspeak.");
        }
        client.stop();
        // thingspeak needs minimum 15 sec delay between updates, i've set it to 20 seconds
        delay(16000);
        Blynk.run(); // Initiates Blynk
        timer.run(); // Initiates SimpleTimer
  }



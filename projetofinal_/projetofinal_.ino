#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <SimpleTimer.h>


String apiKey = "544RO609IAMS6H32"; //  seu Write API key do site ThingSpeak
const char *ssid = "Wesley"; //  substitua com o ssid e senha da rede Wifi
const char *pass = "99102637";
const char* server = "api.thingspeak.com";
char auth[] = "416b8707426d4402846008b896aab2f4";
WiFiClient client;



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



  Serial.begin(9600); // Inicia a comunicação Serial.

  //Definições da conexão sem fio
  Serial.println("Conectando-se a rede Wi-fi ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Blynk.begin(auth, ssid, pass);



  //Definição dos pinos do sensor de disTância
  pinMode(trigPin, OUTPUT); // Define o trigPin como uma saída
  pinMode(echoPin, INPUT); // Define o echoPin como uma entrada

  dht.begin();



}


//Recebendo valores dos botoes do Blynk

bool liga_temp, liga_humi, liga_dist, liga_lumi;

BLYNK_WRITE(V1) {
   int i=param.asInt();
   if ( i==1 ) {
      Serial.println("Temperatura Ligado");
      liga_temp = true;
    } else {
      liga_temp = false;
      Serial.println("Temperatura Desligado");
    }
}

BLYNK_WRITE(V3) {
   int i=param.asInt();
   if ( i==1 ) {
      Serial.println("Distancia Ligado");
      liga_dist = true;
    } else {
      liga_dist = false;
      Serial.println("Distancia Desligado");
    }
}

BLYNK_WRITE(V4) {
   int i=param.asInt();
   if ( i==1 ) {
      Serial.println("luminosidade Ligado");
      liga_lumi = true;
    } else {
      liga_lumi = false;
      Serial.println("luminosidade Desligado");
    }
                }

int cont = 1;

void loop() {



 Serial.print("Leitura ");
 Serial.println(cont);
 
 Blynk.run(); // Inicializa o Blynk
 timer.run(); // Inicializa o SimpleTimer 

  if (liga_dist == true){
 
  //Sensor de distância

  // Limpa o Pino Trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  //Define o trigPin no estado HIGH por 10 microssegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê o pino echo, retorna o tempo de viagem da onda de som em microssegundos
  duration = pulseIn(echoPin, HIGH);

  

  // Calculando a distancia
  distance = duration * 0.034 / 2; 
  delay(2000);

   Serial.println("*************************");
   Serial.print("Distancia: ");
   Serial.print(distance);
   Serial.println(" cm");
   Blynk.virtualWrite(8, distance); // virtualpin 8 - distancia
   Serial.println("*************************");   
  
  }  else if
    (liga_dist == false) {
      distance = -1;
      }


  

  float lumi;
  if (liga_lumi == true){

  int sensorValue = analogRead(A0);   // Lê a entrada do pino analógico
  float voltagem = sensorValue * (5.0 / 1023.0);   // Converte a leitura analógica (de 0 - 1023) para uma voltagem (0 - 5V)

  lumi = voltagem * 20;

  Serial.println("*************************");
  Serial.print("Luminosidade em: ");
  Serial.print(lumi);
  Blynk.virtualWrite(7,lumi);
  Serial.println(" %");
  Serial.println("*************************");
  
  } else 
         if (liga_lumi == false) {
          lumi = -1;
                                  }
  
  
  
  // Começa a parte da humidade e Temperatura

  float h,t;


  
  if (liga_temp == true){
     h = dht.readHumidity();
     t = dht.readTemperature();  
          if (isnan(t) || isnan(h))
          {
                  Serial.println("Falha ao ler valores do sensor");
          }
                else
          { 
          
                  Blynk.virtualWrite(V5, h);
                  Blynk.virtualWrite(V6, t);
            
          }
  
  } else if (liga_temp == false){
        h = -1;
        t = -1;
  }

 
  
  // Começa a parte que transmite pro thingspeak
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1="; 
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "&field3=";
    postStr += String(lumi);
    postStr += "&field4=";
    postStr += String(distance);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("**************************");
    Serial.println("ENVIADO PARA O THINKSPEAK");
    Serial.print("Humidade ");
    Serial.println(h);
    Serial.print("Temperatura ");
    Serial.println(t);
    Serial.print("Luminosidade ");
    Serial.println(lumi);
    Serial.print("Diatancia ");
    Serial.println(distance);    ;
    Serial.println("**************************");
  }
  
  client.stop();
  
  delay(20000); //delay devido o thingspeak
  cont = cont+1;
}

  
    



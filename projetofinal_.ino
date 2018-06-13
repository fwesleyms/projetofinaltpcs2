#include <DHT.h>

// defines pins numbers

//Pinos sensor de distância
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

//Sensor de humi/temp

#define DHTPIN 13
DHT dht(DHTPIN, DHT11);

// defines variables
long duration; //sensor de distância
int distance; //sensor de distância

void setup() {

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

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(2000);
  
  // ||||||||||||||||||||||||||||||||||||||||||||

  // Aqui começa a parte do sensor LDR

  int sensorValue = analogRead(A0);   // read the input on analog pin 0

  float voltage = sensorValue * (5.0 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)

  Serial.print("Váriável de luminosidade ");
  Serial.println(voltage);   // print out the value you read

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

  }
  // Termina a parte do humidade temperatura


}

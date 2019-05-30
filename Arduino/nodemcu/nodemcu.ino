/**
 * 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳
 * 🐬                                                                                                     🐳
 * 🐬                                Intelligent Aquarium - Arduino IoT project                           🐳
 * 🐬                                                                                                     🐳
 * 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳
 * 
 * Github repository: (https://github.com/mvelezg99/intelligent-aquarium-arduino-iot)
 * -----------------------------------------------------------------------------------------------------
 * NodeMCU V3 Configuration
 * 
 * Purpose: Configuration of NodeMCU V3 ESP8266;
 * 
 * This code is the main logic incorporated in the NodeMCU V3,
 * responsible for managing and configuring the connection to
 * WiFi, connection to Firebase, and execute the control routines
 * of the aquarium.
 * 
 * @file nodemcu.ino
 * @author Juan Jose Correa <https://github.com/Juanjoc12>
 * @author Miguel Angel Velez <https://github.com/mvelezg99>
 * @version 1.0 19/05/2019
 */

//--------------------------------------- IMPORTING LIBRARIES ----------------------------------------//
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//----------------------------------------- IMPORTING HEADERS ----------------------------------------//

#include "config.h"
#include "routines.h"

//----------------------------------------- PINS ASSIGNATION -----------------------------------------//

// Constant pins assignation to control the sensors inputs.
const byte temperatureDQ = D2;
const byte proximityTrigger = D4;
const byte proximityEcho = D3;

// Constant pins assignation to control the digital outputs (relays).
const byte temperatureRelay = D1;
const byte waterLevelRelay = D5;
const byte feederRelay = D6;
const byte waterChangeRelay = D7;

//---------------------------------------- INITIALIZATIONS ------------------------------------------//

#define FIREBASE_HOST "xxxxxx.firebaseio.com"
#define FIREBASE_AUTH "tJUm9XpxxxxxxxxxxxxxxxxxpzCbKD7b"
#define WIFI_SSID "xxxxxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxx123"

OneWire oneWireObject(temperatureDQ);
DallasTemperature DS18B20(&oneWireObject);

long echoTime; 
long echoDistance;

String feed = "";
String waterChange = "";
float temperature = 0.0;
float distance = 0.0;


//----------------------------------------- SETTING UP ----------------------------------------------//
/**
 * Setting up instances and initial features to run correctly the loop.
 */
void setup() {
  Serial.begin(115200);

  WiFiInitialize();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  relaysInitialize();
  
  proximityInitialize();

  DS18B20.begin();
}

//----------------------------------------- LOOP ----------------------------------------------------//
/**
 * Looping the code to execute the control routines every time.
 */
void loop() {

  setFirebaseTemperature();
  
  setFirebaseDistance();
  
  getFirebaseFeed();
  
  getFirebaseWaterChange();
  
  controlTemperature();

  controlWaterLevel();

  controlWaterChange();
  
  controlFeeding();
}

//-------------------------------------- CONFIGURATION ----------------------------------------------//
/**
 * 
 */
void relaysInitialize(){
  pinMode(temperatureRelay, OUTPUT);
  pinMode(waterLevelRelay, OUTPUT);
  pinMode(feederRelay, OUTPUT);
  pinMode(waterChangeRelay, OUTPUT);
}

/**
 * 
 */
void WiFiInitialize(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

/**
 * 
 */
void proximityInitialize(){
  pinMode(proximityTrigger, OUTPUT);
  pinMode(proximityEcho, INPUT);
  digitalWrite(proximityTrigger, LOW);
}


//----------------------------------------- FIREBASE ROUTINES ----------------------------------------//

/**
 * 
 */
void setFirebaseTemperature(){
  DS18B20.requestTemperatures();
  
  Serial.println("-----------------");

  temperature = DS18B20.getTempCByIndex(0);
  
  Serial.print("TEMPERATURE: ");
  Serial.println(temperature);
  
  Firebase.setFloat("/IntelligentAquarium/temperature", temperature);
  if (Firebase.failed()) {
      Serial.print("setting /temperature failed:");
      Serial.println(Firebase.error());  
      ESP.restart();
      return;
  }
  delay(300); 
}

/**
 * 
 */
void setFirebaseDistance(){
  Serial.println("-----------------");
  
  digitalWrite(proximityTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(proximityTrigger, LOW);
  
  echoTime = pulseIn(proximityEcho, HIGH);
  distance = echoTime/59;
  
  Serial.print("DISTANCE: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);

  Firebase.setFloat("/IntelligentAquarium/distance", distance);
  if (Firebase.failed()) {
      Serial.print("setting /distance failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(300);
}

/**
 * 
 */
void getFirebaseFeed(){
  Serial.println("-----------------");
  feed = Firebase.getString("/IntelligentAquarium/feed");
  Serial.print("FEED: ");
  Serial.println(Firebase.getString("/IntelligentAquarium/feed"));
  
  if (Firebase.failed()) {
      Serial.print("setting /feed failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(300);
}

/**
 * 
 */
void getFirebaseWaterChange(){
  Serial.println("-----------------");
  waterChange = Firebase.getString("/IntelligentAquarium/waterChange");
  Serial.print("WATER CHANGE: ");
  Serial.println(Firebase.getString("/IntelligentAquarium/waterChange"));
  
  if (Firebase.failed()) {
      Serial.print("setting /waterChange failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(300);
}

//----------------------------------------- CONTROL ROUTINES ----------------------------------------//
/**
 * 
 */
void controlTemperature(){
  if(temperature <= 25){
    digitalWrite(temperatureRelay, HIGH);
  } else {
    digitalWrite(temperatureRelay, LOW);
  }
  delay(100);
}

/**
 * 
 */
void controlWaterLevel(){
  if(distance >= 4 && waterChange == "0"){
    digitalWrite(waterLevelRelay, HIGH);
  } else {
    digitalWrite(waterLevelRelay, LOW);
  }
  delay(100);
}

/*
 * 
 */
void controlWaterChange(){
  if(waterChange == "1"){
    digitalWrite(waterChangeRelay, HIGH);
  } else {
    digitalWrite(waterChangeRelay, LOW);
  }

  delay(100);
}

/*
 * 
 */
void controlFeeding(){
  if(feed == "1"){
    digitalWrite(feederRelay, HIGH);
    delay(3000);
    digitalWrite(feederRelay, LOW);
    Firebase.setString("/IntelligentAquarium/feed", "0");
  }
  delay(100);
}

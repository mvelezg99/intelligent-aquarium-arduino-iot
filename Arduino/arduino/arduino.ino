/**
 * 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳
 * 🐬                                                                                                     🐳
 * 🐬                                Intelligent Aquarium - Arduino IoT project                           🐳
 * 🐬                                                                                                     🐳
 * 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳 🐋 🐡 🦈 🐙 🐚 🐬 🐠 🐟 🐳
 * 
 * Github repository: (https://github.com/mvelezg99/intelligent-aquarium-arduino-iot)
 * -----------------------------------------------------------------------------------------------------
 * Arduino UNO Configuration
 * 
 * Purpose: Configuration of Arduino UNO;
 * 
 * This code is the main logic incorporated in the Arduino UNO,
 * responsible for showing in a LCD the current temperature in
 * the aquarium.
 * 
 * 
 * @file arduino.ino
 * @author Juan Jose Correa <https://github.com/Juanjoc12>
 * @author Miguel Angel Velez <https://github.com/mvelezg99>
 * @version 1.0 19/05/2019
 */

//--------------------------------------- IMPORTING LIBRARIES ----------------------------------------//

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "max6675.h"

//----------------------------------------- PINS ASSIGNATION -----------------------------------------//

int ktcSO = 8;
int ktcCS = 9;
int ktcCLK = 10;

//---------------------------------------- INITIALIZATIONS -------------------------------------------//

LiquidCrystal_I2C lcd(0x27, 16, 2);
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

//----------------------------------------- SETTING UP ----------------------------------------------//

void setup() {
  Serial.begin(9600);
  // Inicializamos el LCD.
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Welcome");
  lcd.setCursor(0,1);
  lcd.print("Aquarium");
  delay(2000);
  lcd.clear();
}

//----------------------------------------- LOOP ----------------------------------------------------//

void loop() {
  Serial.println("Deg C = "); 
  Serial.println(ktc.readCelsius());

  float temperature = ktc.readCelsius() - 1;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  delay(2000);

  if(temperature <= 25){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TEMPERATURE");
    lcd.setCursor(0,1);
    lcd.print("CONTROL");
    lcd.noBacklight();
    delay(200);
    lcd.backlight();
    delay(200);
    lcd.noBacklight();
    delay(200);
    lcd.backlight();
    delay(200);
    lcd.noBacklight();
    delay(200);
    lcd.backlight();
    delay(200);
    lcd.noBacklight();
    delay(200);
    lcd.backlight();
    delay(200);
    delay(2500);
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature: "); 
    lcd.print(temperature);
    delay(2000);
  }

}

// maravilhosa estacao de baixo custo :3

//LowCostAirQualityStation
//
//Datafile header:
//
//Date;Time;WeekDay;RH;P;T;CO;CO2;MP

// SD init
// pin MOSI 11
// pin MISO 12
// pin CLK 13 SCK
// pin CS 10
#include <SPI.h>
#include <SD.h>
File myFile;
const int chipSelect = 10;

// Clock init
// pin CLK 6
// pin DAT 7
// pin RST 8
// fonte https://playground.arduino.cc/Main/DS1302/
//#define DS1302_SCLK_PIN   6    // Arduino pin for the Serial Clock
//#define DS1302_IO_PIN     7    // Arduino pin for the Data I/O
//#define DS1302_CE_PIN     8    // Arduino pin for the Chip Enable
#include <virtuabotixRTC.h>
virtuabotixRTC myRTC(6, 7, 8);

// DHT init
// pin 1 5V
// pin 2 4
// pin 3 GND
// pin 4 GND
#include "Arduino.h"
#include <DHT12.h>
DHT12 dht12(4, true);

// MQ7 init
// pin A0 A2
// pin D0 9
const int MQ7Apin = 2;
const int MQ7Dpin = 9;
int COlimit;
int CO;

// MQ135 init
// pin A0 A1
// pin D0 5
const int MQ135Apin = 1;
const int MQ135Dpin = 5;
int CO2limit;
int CO2;

// DSM501A init
// pin 2 2
// pin 3 5V
// pin 5 GND
#include<string.h>
byte buff[2];
int DSMpin = 2;//DSM501A input D2
unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 30000; // intervalo de medida do dsm
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

// DS18B20 init
// pin 1 GND
// pin 2 3
// pin 3 5V
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// BMP108 init
// pin VIN 3.3V (NÃO É 5V)
// pin GND GND
// pin SCL A5
// pin SDA A4
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

// STATE LED pin
#define LED 3
int MODE = LOW;
double DangerousCO = 130, DangerousCO2 = 130, DangerousConcentration = 3000;

// Setup of the reset function to reset every 24 h
void(* resetFunc) (void) = 0;

void setup() {
  //(segundos, minutos, hora, dia da semana, dia do mes, mes, ano)
 // myRTC.setDS1302Time(0, 32, 15, 6, 31, 12, 2020); //set datetime, comment line after


  pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  do {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  } while(!SD.begin());

  dht12.begin();
  bmp.begin();
  sensors.begin(); //ds18b20
  pinMode(MQ7Dpin, INPUT);
  pinMode(MQ135Dpin, INPUT);
  pinMode(DSMpin,INPUT);
  starttime = millis(); 
}

void loop() {
    digitalWrite(LED, HIGH);
    delay(4000);
    digitalWrite(LED, LOW);
    delay(4000);
//  digitalWrite(LED, MODE);
  myFile = SD.open("data.csv", FILE_WRITE);

  // Refresh data reading
  float h = dht12.readHumidity();
  CO = analogRead(MQ7Apin);
  COlimit = digitalRead(MQ7Dpin);
  CO2 = analogRead(MQ135Apin);
  CO2limit = digitalRead(MQ135Dpin);
  sensors.requestTemperatures(); // one wire ds18b20

  
  // DSM header
  duration = pulseIn(DSMpin, LOW);
  lowpulseoccupancy += duration;
  endtime = millis();
  if ((endtime-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy / (sampletime_ms*10.0);
    //ratio = (lowpulseoccupancy-endtime+starttime + sampletime_ms)/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    lowpulseoccupancy = 0;
    starttime = millis();
  }

  
  if(myFile){
    myRTC.updateTime();
    myFile.print(myRTC.dayofmonth);
    myFile.print("/");
    myFile.print(myRTC.month);
    myFile.print("/");
    myFile.print(myRTC.year);
    myFile.print(";");
    //Adiciona um 0 caso o valor da hora seja <10
    if (myRTC.hours < 10)
    {
      myFile.print("0");
    }
    myFile.print(myRTC.hours);
    myFile.print(":");
    //Adiciona um 0 caso o valor dos minutos seja <10
    if (myRTC.minutes < 10)
    {
      myFile.print("0");
    }
    myFile.print(myRTC.minutes);
    myFile.print(":");
    //Adiciona um 0 caso o valor dos segundos seja <10
    if (myRTC.seconds < 10)
    {
      myFile.print("0");
    }
    myFile.print(myRTC.seconds);
    myFile.print(";");
    myFile.print(myRTC.dayofweek);
    myFile.print(";");
    
    myFile.print(h);
    myFile.print(";");
    myFile.print(bmp.readPressure());
    myFile.print(";");
    myFile.print(sensors.getTempCByIndex(0));
    myFile.print(";");
    
    if(COlimit == HIGH){
      myFile.print("9999");
    } else{
      myFile.print(CO);
    }
    myFile.print(";");
    if(CO2limit == HIGH){
      myFile.print("9999");
    } else{
      myFile.print(CO2);
    }
    myFile.print(";");
    myFile.print(concentration);
      myFile.println(";");

    myFile.close();
  }
  
  // turns the led on if the concentration reach one of the trholds
//  if(CO >= DangerousCO || CO2 >= DangerousCO2 ||  concentration >= DangerousConcentration){
//    MODE = HIGH;
//    }
    
//  else {
//    MODE = LOW;
//    }
  
 delay(2000);
  if (millis() > 864000000){
    resetFunc();
  }
}

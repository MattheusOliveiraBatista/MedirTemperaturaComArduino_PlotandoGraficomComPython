#include <SPI.h> //for the SD Card module
#include <SD.h> // for the SD card
#include <RTClib.h> // for the RTC
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>


const int chipSelect = 4;
float tempMin = 999;
float tempMax = 0;
int estadoBotao = 0;
int Botao = 10;

File myFile;
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DS18B20 7 //DEFINE O PINO DIGITAL UTILIZADO PELO SENSOR

OneWire ourWire(DS18B20); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire

void setup() {

  Serial.begin(9600);
  lcd.setBacklight(HIGH);
  sensors.begin();
  pinMode(10, INPUT);
  lcd.init();

  if (! rtc.begin()) {
    Serial.println("Unable to find RTC");
    while (1);
  }
  else rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (! rtc.isrunning()) Serial.println("RTC is NOT running!");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  myFile = SD.open("DATA.txt", FILE_WRITE);

  if (myFile)Serial.println("Open file ok");
  myFile.close();
}

void RegistrandoTempo() {
  DateTime now = rtc.now();
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("Data: ");
    myFile.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    myFile.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    myFile.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    myFile.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    myFile.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    myFile.print(" - Hora: "); //IMPRIME O TEXTO NA SERIAL
    myFile.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    myFile.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    myFile.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    myFile.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    myFile.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    myFile.print(" - ");
  }
  
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(now.day(), DEC);
  lcd.setCursor(5, 0);
  lcd.print('/');
  lcd.setCursor(6, 0);
  lcd.print(now.month(), DEC);
  lcd.setCursor(7, 0);
  lcd.print('/');
  lcd.setCursor(8, 0);
  lcd.print(now.year(), DEC);
  lcd.setCursor(13, 0);

  lcd.setCursor(4, 1);
  lcd.print(now.hour(), DEC);
  lcd.setCursor(6, 1);
  lcd.print(':');
  lcd.setCursor(7, 1);
  lcd.print(now.minute(), DEC);
  lcd.setCursor(9, 1);
  lcd.print(':');
  lcd.setCursor(10, 1);
  lcd.print(now.second(), DEC);
  myFile.close();
}

void RegistraTemperaturas() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC < tempMin)tempMin = tempC;
  if (tempC > tempMax) tempMax = tempC;
  
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp.:");
  lcd.setCursor(7, 0);
  lcd.print(tempC);
  lcd.setCursor(12, 0);
  lcd.write(223);
  lcd.setCursor(13, 0);
  lcd.print("C");

  lcd.setCursor(1, 1);
  lcd.print("L:");
  lcd.setCursor(3, 1);
  lcd.print(tempMin, 1);
  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.setCursor(11, 1);
  lcd.print(tempMax, 1);
  
  myFile = SD.open("DATA.txt", FILE_WRITE);

  if (myFile) {
    myFile.print(tempC);
    myFile.println(" ");
  }
  myFile.close();
}

void loop() {

  estadoBotao = digitalRead(Botao);
  
  if (estadoBotao == HIGH) {
    RegistrandoTempo();
    RegistraTemperaturas();
    delay(4000);
  }
  else{
    RegistrandoTempo();
    RegistraTemperaturas();
    delay(4000);
  }
}

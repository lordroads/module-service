#include <Ticker.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266FtpServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/* Для Wifi */
const char* ssid = "nbn";
const char* password = "p@ncreatite";

/* PIN */
const byte pinRelay = 5;      //relay pin 4 on board
//Btn
const byte pinBtn = 14; 
boolean buttonWasUp = true;
//LED
const byte pinLedGreen = 12; 
const byte pinLedRed = 13; 

/* Time and tick */
Ticker blinker;                                                   //Создание таймера
int countBlink = 0;                                               //Подсчет тиков
const long utcOffsetInSeconds = 10800;                            //Для коррекции чесового пояса
WiFiUDP ntpUDP;                                                   //ДЛя запроса времени
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds); //запрос времени

/* Web and FTP*/
ESP8266WebServer HTTP(80);
FtpServer ftpSrv;

void setup(void){
  initPin();
  connectToWifi(ssid, password);
  allBegin();
}

void loop(void){
  
}

void initPin(){
  Serial.begin(9600);
  
  pinMode(pinRelay, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT);

  digitalWrite(pinRelay, LOW);
  digitalWrite(pinLedGreen, LOW);
  digitalWrite(pinLedRed, LOW);

  Serial.println("Init - OK!");
  
  Serial.print("Init status wifw - ");
  Serial.println(WiFi.status());
}

void allBegin(){
  Serial.print("AllBegin status wifw - ");
  Serial.println(WiFi.status());
  if(WiFi.status() != WL_DISCONNECTED){ //statusWifi != WL_CONNECT_FAILED | statusWifi != WL_CONNECT_WRONG_PASSWORD | 
    SPIFFS.begin();
    Serial.println("SPIFFS - OK!");
    
    HTTP.begin();
    Serial.println("HTTP - OK!");
    
    ftpSrv.begin("relay","realy");
    Serial.println("FTP Server - OK!");
    Serial.println("--------------------");
    Serial.println("LOGIN - relay");
    Serial.println("PASSWORD - realy");
    Serial.println("--------------------");
    
    timeClient.begin();
    Serial.println("Time Client - OK!");
  }
}

void connectToWifi(String _ssid, String _password){
  WiFi.mode(WIFI_STA);          
  WiFi.begin(_ssid, _password);
  
  Serial.print("ConnectToWifi.begin status wifw - ");
  Serial.println(WiFi.status());
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.print("Fatal status wifw - ");
    Serial.println(WiFi.status());
    
    digitalWrite(pinLedRed, HIGH);
    Serial.println("Connection Failed! Rebooting...");
    //TODO: сделать запуск как точку вай фай, чтобы зайти и задать новый ssid и pass
  }
  else{
    Serial.print("OK status wifw - ");
    Serial.println(WiFi.status());
    
    digitalWrite(pinLedGreen, HIGH);
    digitalWrite(pinLedRed, LOW);
    
    Serial.print("Modul IP - ");
    Serial.println(WiFi.localIP());
  }
}

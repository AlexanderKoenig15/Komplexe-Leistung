#include <Arduino.h>
#include <Wire.h>
#include <cmath>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <iostream>
#include <ArduinoOTA.h>
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define FAN_Pin D7
#define PWM_frequenz 25000
#define PWM_max 1023
#define Esp32_Name "Kelleranlage"
#define Wifi_Name "OHS56"
#define Wifi_password "ks$djAz7830nFbsf42ld91kdjsaGMs"
#define MQTT_Broker "192.168.178.23"
#define Esp32_password "F238(GHJSAN§$klm124nKJN4m,.321"
const char* indexhtml = "<!doctype html><html lang='de'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Konfiguration</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><form action='/' method='post'><h1 class=''>Konfiguration</h1><br /><div class='form-floating'><label>SSID Wlan</label><input class='form-control' name='SSID-Wlan' type='text' maxlength='25'/></div><div class='form-floating'><br /><label>Passwort Wlan</label><input class='form-control' name='Passwort-Wlan' type='password' maxlength='30'/></div><div class='form-floating'><br /><label>SSID AP</label><input class='form-control' name='SSID-AP' type='text' maxlength='25'/></div><div class='form-floating'><br /><label>Passwort AP</label><input class='form-control' name='Passwort-AP' type='password' maxlength='30'/></div><div class='form-floating'><br /><label>Toplevel Topic</label><input class='form-control' name='Topic' type='text' maxlength='25'/></div><div class='form-floating'><br /><label>MQTT Server IP</label><input class='form-control' name='MQTT-IP' type='text' maxlength='15'/></div><div class='form-floating'><br /><label>MQTT Server Port</label><input class='form-control' name='MQTT-Port' type='number' min='100' max='30000'/></div><div class='form-floating'><br /><label>Zielfeuchte in %</label><input class='form-control' name='Zielfeuchte' type='number' min='50' max='85'/></div><div class='form-floating'><br /><br /><br /><button type='submit'>Speichern</button></form></body></html> ";
struct configuration 
{
    char Wifi_name[26];
    char Wifi_passwort[31];
    char Esp32_name[26];
    char Esp32_passwort[31];
    char toplevel_topic[26];
    char MQTT_IP[16];
    char MQTT_Port[6];
    char Zielfeuchte[3];
} config_esp8266 = {};
WiFiClient nodemcu;
PubSubClient mqttclient(nodemcu);
ESP8266WebServer server(80);
Adafruit_SSD1306 Display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);
Adafruit_BME280 bmpinnen;
Adafruit_BME280 bmpaußen;
int Zähler_mqtt = 0;
unsigned char WiFi_logo[]
{
0b01111110,
0b10000001, 
0b00000000, 
0b00111100, 
0b01000010, 
0b00000000, 
0b00011000, 
0b00100100
};
unsigned char NoWiFi_logo[]
{
0b01111111,
0b10000011, 
0b00000100, 
0b00111100, 
0b01010010, 
0b00100000, 
0b01011000, 
0b10100100
};
unsigned char MQTT_logo[]
{
0b11100011, 
0b00111001, 
0b00001100, 
0b11100110, 
0b01110010, 
0b00011011, 
0b11011001, 
0b11001001
};
//*****************************************************************************************************************
void Werte(String bez, float wert,String ein)
{
    String str = (String) wert;str +=ein;
    int16_t x,y;
    uint16_t x1,y1;
    Display.print(bez);
    Display.getTextBounds(str, Display.getCursorX(), Display.getCursorY(), &x,&y,&x1,&y1);
    Display.setCursor(SCREEN_WIDTH - x1, y);
    Display.print(wert);
    Display.println(ein);
}
//*****************************************************************************************************************
void center(String buf)
{
    int16_t x, y;
    uint16_t w,h;   
Display.getTextBounds(buf, Display.getCursorX(), Display.getCursorY(), &x,&y,&w,&h);
Display.setCursor((SCREEN_WIDTH - w)/2,Display.getCursorY());
Display.println(buf);
}
//*****************************************************************************************************************
void WiFi_connect()
{
while(WiFi.isConnected() == false && WiFi.status() != WL_NO_SSID_AVAIL && WiFi.status() != WL_CONNECT_FAILED && WiFi.status() != WL_WRONG_PASSWORD)
{
Display.clearDisplay();
Display.setCursor(0,0);
center("verbinden.");
Display.display();
delay(500);
Display.clearDisplay();
Display.setCursor(0,0);
center("verbinden..");
Display.display();
delay(500);
Display.clearDisplay();
Display.setCursor(0,0);
center("verbinden...");
Display.display();
delay(500);
}
if(WiFi.status() != WL_CONNECTED)
{
Display.clearDisplay();
Display.setCursor(0,0);
center("Keine Verbindung");
center("Konfiguration startet");
Display.display();
delay(2500);
}else{
mqttclient.setServer(config_esp8266.MQTT_IP,  std::stoi(config_esp8266.MQTT_Port));
mqttclient.connect(config_esp8266.Esp32_name);
ArduinoOTA.setHostname(config_esp8266.Esp32_name);
ArduinoOTA.begin();
}
}
//*****************************************************************************************************************
void config()
{
if(server.method() == HTTP_POST)
{
if(server.arg("SSID-Wlan") != ""){strncpy(config_esp8266.Wifi_name, server.arg("SSID-Wlan").c_str(), sizeof(config_esp8266.Wifi_name));}
if(server.arg("Passwort-Wlan") != ""){strncpy(config_esp8266.Wifi_passwort, server.arg("Passwort-Wlan").c_str(), sizeof(config_esp8266.Wifi_passwort));}
if(server.arg("SSID-AP") != ""){strncpy(config_esp8266.Esp32_name, server.arg("SSID-AP").c_str(), sizeof(config_esp8266.Esp32_name));}
if(server.arg("Passwort-AP") != ""){strncpy(config_esp8266.Esp32_passwort, server.arg("Passwort-AP").c_str(), sizeof(config_esp8266.Esp32_passwort));}
if(server.arg("Topic") != ""){strncpy(config_esp8266.toplevel_topic, server.arg("Topic").c_str(), sizeof(config_esp8266.toplevel_topic));}
if(server.arg("MQTT-IP") != ""){strncpy(config_esp8266.MQTT_IP, server.arg("MQTT-IP").c_str(), sizeof(config_esp8266.MQTT_IP));}
if(server.arg("MQTT-Port") != ""){strncpy(config_esp8266.MQTT_Port, server.arg("MQTT-Port").c_str(), sizeof(config_esp8266.MQTT_Port));}
if(server.arg("Zielfeuchte") != ""){strncpy(config_esp8266.Zielfeuchte, server.arg("Zielfeuchte").c_str(), sizeof(config_esp8266.Zielfeuchte));}
config_esp8266.Wifi_name[25] = config_esp8266.Wifi_passwort[30] =config_esp8266.Esp32_name[25] =config_esp8266.Esp32_passwort[30] =config_esp8266.toplevel_topic[25] =config_esp8266.MQTT_IP[15] =config_esp8266.MQTT_Port[5] =config_esp8266.Zielfeuchte[2] = 0;
EEPROM.put(0, config_esp8266);
EEPROM.commit();
server.send(200, "text/html", "<html lang='de'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Konfiguration beendet!</title> </head><body><p style='text-align: center;'><strong>Die Anlage wurde erfolgreich konfiguriert!</strong></p><p style='text-align: center;'>Neustart in 5s.</p> </body>");
delay(5000);
ESP.restart();
}else
{
server.send(200, "text/html", indexhtml);
}
}
//*****************************************************************************************************************
void SelectBUS(uint8_t bus)
{
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    Wire.endTransmission();
}
//*****************************************************************************************************************
float taupunkt(float temp, float humid)
{
    float tau;
    humid = humid/100;
    tau = 243.13*((17.62*temp)/(243.12+temp)+log(humid))/((17.62*243.12)/(243.12+temp)-log(humid));
    return tau;
}
//*****************************************************************************************************************
uint8_t Lüfter(float tin, float tou,float relin, float relou)
{
    uint8_t speed;
    float ain=(4.75081*pow(1.06468,tin))*(relin/100);
    float aou=(4.75081*pow(1.06468,tou))*(relou/100);
    speed = (ain-aou) > 1.0f;
    if(relin < std::stoi(config_esp8266.Zielfeuchte))
    {
      speed = 0;
    }
    analogWrite(FAN_Pin, speed*PWM_max);
    return speed*100;
}
//*****************************************************************************************************************
void refresh(float tin, float fin,float tout ,float fout,float taup,uint8_t speed, float presin, float presout)
{
Display.clearDisplay();
Display.setCursor(0,0);
if(WiFi.status() == WL_CONNECTED){
Display.drawBitmap(0,0, WiFi_logo,8,8,WHITE);
if(mqttclient.connected()){
Display.drawBitmap(SCREEN_WIDTH -8,0, MQTT_logo,8,8,WHITE);
if(Zähler_mqtt == 30)
{
Zähler_mqtt = 0;
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/InPressure")).c_str(),String(presin).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/OutPressure")).c_str(),String(presout).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/Fan2Pwm")).c_str(),String(speed).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/Fan1Pwm")).c_str(),String(speed).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/OutHumidity")).c_str(),String(fout).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/InHumidity")).c_str(),String(fin).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/OutTemp")).c_str(),String(tout).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("home/InTemp")).c_str(),String(tin).c_str());
}
Zähler_mqtt++;
}
}else{
Display.drawBitmap(0,0, NoWiFi_logo,8,8,WHITE);
}
center(config_esp8266.Esp32_name);
if(WiFi.softAPIP().isSet()){center(WiFi.softAPIP().toString());}else{center(WiFi.localIP().toString());}
Display.writeLine(0,SCREEN_HEIGHT/4-1,SCREEN_WIDTH,SCREEN_HEIGHT/4-1, WHITE);
Display.setCursor(0, SCREEN_HEIGHT/4);
Werte("Temp Innen:",tin," C");
Werte("Temp Ausen:",tout," C");
Werte("Luft Innen:",fin," %");
Werte("Luft Ausen:",fout," %");
Werte("Taup Innen:",taup," C");
Werte("Auslastung:",speed," %");
Display.display();
}
//*****************************************************************************************************************
/*+++++++++++++++++++++++++++++++++++++++++++++++++++
setup
+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup() {
Serial.begin(9600);
EEPROM.begin(sizeof(struct configuration));
EEPROM.get(0, config_esp8266);
Serial.println("Konfiguration:");
Serial.println(config_esp8266.Wifi_name);
Serial.println(config_esp8266.Wifi_passwort);
Serial.println(config_esp8266.Esp32_name);
Serial.println(config_esp8266.Esp32_passwort);
Serial.println(config_esp8266.toplevel_topic);
Serial.println(config_esp8266.MQTT_IP);
Serial.println(config_esp8266.MQTT_Port);
Serial.println(config_esp8266.Zielfeuchte);
Wire.begin();
SelectBUS(0);
Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
Display.setTextColor(WHITE);
Display.setTextSize(1);
SelectBUS(1);
bmpinnen.begin(BME280_ADDRESS_ALTERNATE, &Wire);
SelectBUS(2);
bmpaußen.begin(BME280_ADDRESS_ALTERNATE,&Wire);
SelectBUS(0);
pinMode(FAN_Pin, OUTPUT);
analogWriteFreq(PWM_frequenz);
analogWrite(FAN_Pin, PWM_max/2);
WiFi.softAP(config_esp8266.Esp32_name, config_esp8266.Esp32_passwort, 1, 0, 2);
WiFi.enableAP(false);
WiFi.begin(config_esp8266.Wifi_name, config_esp8266.Wifi_passwort);
WiFi.hostname(config_esp8266.Esp32_name);
WiFi_connect();
server.on("/", config);
server.begin();
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++
loop
+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void loop() {
SelectBUS(1);
float tin = bmpinnen.readTemperature();
float fin = bmpinnen.readHumidity();
SelectBUS(2);
float tou = bmpaußen.readTemperature();
float fou = bmpaußen.readHumidity();
SelectBUS(0);
if(WiFi.isConnected() == false)
{
WiFi.enableAP(true);
}else
{
    WiFi.enableAP(false);
    if(mqttclient.connected() == false)
    {
        mqttclient.connect(config_esp8266.Esp32_name);
    }
}
mqttclient.loop();
refresh(tin,fin,tou,fou,taupunkt(tin,fin),Lüfter(tin,tou,round(fin),round(fou)),bmpinnen.readPressure()/100.0f,bmpaußen.readPressure()/100.0f);
server.handleClient();
ArduinoOTA.handle();
delay(1000);
}

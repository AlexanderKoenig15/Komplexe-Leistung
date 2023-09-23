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
#include <M2M_LM75A.h>
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define Bus_notused 1
#define LM75Offset -3.0f
#define clockspeed 10000
#define bmeinnen 3
#define tempoff 35.0f
#define bmeaußen 2
#define lm75 4
#define bitmapsize 8
#define workingdelay 1000
#define debug_mode false
uint8_t FAN_Pin = D5;
uint8_t FAN_Pin2 = D6;
uint8_t FAN_Pin3 = D8;
uint8_t Button_Pin = D7;
uint8_t ExtPowerOutPin = 10;
uint8_t CirculationPumpPin = D3;
float wandbackup = 0;
#define PWM_frequenz 25000
#define PWM_max 1023
const char* indexhtml = "<!doctype html><html lang='de'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Konfiguration</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><form action='/' method='post'><h1>Konfiguration</h1><br /><div class='form-floating'><br /><label>Password</label><input class='form-control' name='Password-config' type='text' pattern='.{1,25}' required value = ''/></div><button type='submit' value ='Check'>weiter</button></form></body></html>";
//original html = "<!doctype html><html lang='de'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Konfiguration</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><form action='/' method='post'><h1 class=''>Konfiguration</h1><br /><div class='form-floating'><br /><label>Name der Anlage</label><input class='form-control' name='SSID-AP' type='text' maxlength='25'/></div><div class='form-floating'><br /><label>Passwort AP</label><input class='form-control' name='Passwort-AP' type='password' maxlength='40'/></div><br /><div class='form-floating'><label>SSID Wlan</label><input class='form-control' name='SSID-Wlan' type='text' maxlength='25'/></div><div class='form-floating'><br /><label>Passwort Wlan</label><input class='form-control' name='Passwort-Wlan' type='password' maxlength='40'/></div><div class='form-floating'><br /><label>Toplevel Topic</label><input class='form-control' name='Topic' type='text' maxlength='25'/></div><div class='form-floating'><br /><label>MQTT Server IP</label><input class='form-control' name='MQTT-IP' type='text' maxlength='15'/></div><div class='form-floating'><br /><label>MQTT Server Port</label><input class='form-control' name='MQTT-Port' type='number' min='100' max='30000'/></div><div class='form-floating'><br /><label>Zielfeuchte in %</label><input class='form-control' name='Zielfeuchte' type='number' min='50' max='85'/></div><div class='form-floating'><br /><br /><br /><button type='submit'>Speichern</button></form></body></html>"
struct configuration 
{
    char Wifi_name[26];
    char Wifi_passwort[41];
    char Esp32_name[26];
    char Esp32_passwort[41];
    char toplevel_topic[26];
    char MQTT_IP[16];
    char MQTT_Port[6];
    char Zielfeuchte[3];
    char mode[4];
    char pwm[4];
} config_esp8266 = {};
WiFiClient nodemcu;
M2M_LM75A walltemp;
PubSubClient mqttclient(nodemcu);
ESP8266WebServer server(80);
Adafruit_SSD1306 Display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);
Adafruit_BME280 bmpinnen;
Adafruit_BME280 bmpaußen;
int z_mqtt = 0;
unsigned char WiFi_good[]
{
0b11100000, 
0b00111000, 
0b00001100, 
0b11100110, 
0b01110010, 
0b00011011, 
0b11011001, 
0b11001001
};
unsigned char WiFi_middle[]
{
0b00000000, 
0b00000000, 
0b00000000, 
0b11100000, 
0b01110000, 
0b00011000, 
0b11011000, 
0b11001000
};
unsigned char WiFi_bad[]
{
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b00000000, 
0b11000000, 
0b11000000
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
unsigned char NoMQTT_logo[]
{
0b00000001,
0b00000010,
0b00000101,
0b00001001,
0b00010101,
0b10100101,
0b11010101,
0b11110101
};
unsigned char Fan_enabled[]
{
0b00011110,
0b10011100,
0b11011000,
0b11100111,
0b11100111,
0b00011011,
0b00111001,
0b01111000
};
unsigned char dehum_enabled[]
{
0b00011000,
0b00011000,
0b00111100,
0b00111100,
0b01111110,
0b01111110,
0b01111110,
0b00111100
};
unsigned char nothingenabled[]
{
0b10000001,
0b01000010,
0b00100100,
0b00011000,
0b00011000,
0b00100100,
0b01000010,
0b10000001
};
unsigned char modeeco[]
{
0b00000000,
0b00111100,
0b00100000,
0b00111000,
0b00100000,
0b00111100,
0b00000000,
0b00000000
};
unsigned char MQTT_logo[]
{
0b00000111,
0b00000011,
0b00000101,
0b00001010,
0b00010010,
0b00101010,
0b01001010,
0b10101010
};
unsigned char modeper[]
{
0b00000000,
0b00111100,
0b00100100,
0b00111100,
0b00100000,
0b00100000,
0b00000000,
0b00000000
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
while(WiFi.status() == WL_DISCONNECTED)
{
delay(500);
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
ArduinoOTA.begin();
ArduinoOTA.setHostname(config_esp8266.Esp32_name);
}
}
//*****************************************************************************************************************
void saveineeprom()
{
config_esp8266.Wifi_name[25] = config_esp8266.Wifi_passwort[40] =config_esp8266.Esp32_name[25] =config_esp8266.Esp32_passwort[40] =config_esp8266.toplevel_topic[25] =config_esp8266.MQTT_IP[15] =config_esp8266.MQTT_Port[5] =config_esp8266.Zielfeuchte[2] = config_esp8266.mode[3]=config_esp8266.pwm[3]=0;
EEPROM.put(0, config_esp8266);
EEPROM.commit();
}
//*****************************************************************************************************************
void configreset(bool State)
{
if(!State){
strncpy(config_esp8266.Esp32_name, "NodeMCU 1.0", sizeof(config_esp8266.Esp32_name));
strncpy(config_esp8266.Esp32_passwort, "Password123", sizeof(config_esp8266.Esp32_passwort));
strncpy(config_esp8266.Wifi_name, "Wlan123", sizeof(config_esp8266.Wifi_name));
strncpy(config_esp8266.Wifi_passwort, "Wlanpass1", sizeof(config_esp8266.Wifi_passwort));
strncpy(config_esp8266.MQTT_IP, "1.1.1.1", sizeof(config_esp8266.MQTT_IP));
strncpy(config_esp8266.MQTT_Port, "1883", sizeof(config_esp8266.MQTT_Port));
strncpy(config_esp8266.toplevel_topic, "home", sizeof(config_esp8266.toplevel_topic));
strncpy(config_esp8266.Zielfeuchte, "75", sizeof(config_esp8266.Zielfeuchte));
strncpy(config_esp8266.mode, "eco", sizeof(config_esp8266.mode));
strncpy(config_esp8266.pwm, "100", sizeof(config_esp8266.pwm));
saveineeprom();
ESP.restart();
}
}
//*****************************************************************************************************************
std::string sternchen(int length)
{
    int z;
    std::string str= "*";
for(z = 1;z < length;z++)
{
str = str + "*";
}
return str;
}
//*****************************************************************************************************************
std::string optionen()
{
std::string str = "";
int z, networks = WiFi.scanNetworks();;

for(z = 0; z < networks; z++)
{
    std::string name = std::string(WiFi.SSID(z).c_str()), background = "good";
if(WiFi.RSSI(z) <= -70)
background = "middle";
if(WiFi.RSSI(z) <= -80)
background = "bad";
    if(WiFi.SSID(z) != config_esp8266.Wifi_name)
    str = str + " <option value='"+ name + "'>"+ name +" ("+ background +")</option>";
    else
    str = str + " <option value='"+ name + "' selected >"+ name +" ("+ background +")</option>";
}
return str;
}
//*****************************************************************************************************************
std::string checked()
{
std::string str = "";
if(std::string(config_esp8266.mode) == std::string("per"))
{
    str = " Checked ";
}
return str;
}
//*****************************************************************************************************************
std::string converthtml()
{
    std::string str = "<!DOCTYPE html><html lang='de'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Konfiguration</title><style>:root{background-color: whitesmoke;}body{font-size: large;}div{background-color: transparent;}.formunterfeld{border: 2px solid;border-radius: 5px;padding: 5px;margin: 15px;}input{border-radius: 5px;border: 1px solid;font-size: large;margin: auto;width: 100px;}select{border-radius: 5px;border: 1px solid;font-size: large;margin: auto;width: 100px;}.formfeld{padding: 5px;border: hidden;}input[type='submit']{background-color: lightskyblue;width: 100%;margin: auto;padding: 5px;transition-duration: 50ms ;}input[type='submit']:hover{position: relative;top: -2px;background-color: aqua;}</style></head><body><form method='post' name='configform'><h1>%s</h1><div class='formunterfeld'><h4>Allgemeines</h4><div class='formfeld'><label >Name der Anlage</label><br><input type='text' name='SSID-AP' placeholder='%s'></div><div class='formfeld'><label >Passwort</label><br><input type='password' name='Passwort-AP' ></div><div class='formfeld'><label >Wlan</label><br><select name='SSID-Wlan'>%s</select></div><div class='formfeld'><label >Wlan Passwort</label><br><input type='password' name='Passwort-Wlan'></div></div><div class='formunterfeld'><h4>MQTT</h4><div class='formfeld'><label >Toplevel Topic</label><br><input type='text' name='Topic' placeholder='%s'></div><div class='formfeld'><label >Server IP</label><br><input type='text' name='MQTT-IP' placeholder='%s'></div><div class='formfeld'><label >Server Port</label><br><input type='text' name='MQTT-Port' placeholder='%s'></div></div><div class='formunterfeld'><h4>Performance Einstellungen</h4><div class='formfeld'><label >Zielfeuchte</label><br><input type='range' name='Zielfeuchte' id='Zielfeuchte' min='0' max='100' value=%s><label id='rangezielfeuchte'>%s%</label></div><div class='formfeld'><label >Modus</label><br><select form='configform'><option>per</option><option selected>eco</option></select><label>Derzeit: %s</label></div><div class='formfeld'><label >Lüfterdrehzahl</label><br><input type='range'name='PWM' id='PWM' min='0' max='100' value=%s><label id='rangepwm'>%s%</label></div><input type='submit' value='Speichern'></div></form><script>var rangezielfeuchte = document.getElementById('Zielfeuchte');var rangepwm = document.getElementById('PWM');var zielfeuchte = document.getElementsById('rangezielfeuchte');var pwm = document.getElementsById('rangepwm');pwm.addEventListener('change', ()=>{rangepwm.value = pwm.innerHTML + '%';})zielfeuchte.addEventListener('change', ()=>{rangezielfeuchte.value = zielfeuchte.innerHTML + '%';})</script></body></html>";
    int i = 0, count = 11; // Anzahl der zu ersetzenden %s
    std::string arguments[count] = 
    {
    config_esp8266.Esp32_name, //erstenes %s
    config_esp8266.Esp32_name, //zweites %s etc.
    optionen().c_str(),
    config_esp8266.toplevel_topic,
    config_esp8266.MQTT_IP,
    config_esp8266.MQTT_Port,
    config_esp8266.Zielfeuchte,
    config_esp8266.Zielfeuchte,
    config_esp8266.mode,
    config_esp8266.pwm,
    config_esp8266.pwm
    };
    for (int z = 0; z < count; z++)
    {
        i = str.find("%s", i);
        str.replace(i, 2, arguments[z]);
    }
    return str;
}
//*****************************************************************************************************************
void config()
{
if(server.method() == HTTP_POST)
{
if(server.arg("Password-config") == NULL){
if(server.arg("SSID-Wlan") != ""){strncpy(config_esp8266.Wifi_name, server.arg("SSID-Wlan").c_str(), sizeof(config_esp8266.Wifi_name));}
if(server.arg("Passwort-Wlan") != ""){strncpy(config_esp8266.Wifi_passwort, server.arg("Passwort-Wlan").c_str(), sizeof(config_esp8266.Wifi_passwort));}
if(server.arg("SSID-AP") != ""){strncpy(config_esp8266.Esp32_name, server.arg("SSID-AP").c_str(), sizeof(config_esp8266.Esp32_name));}
if(server.arg("Passwort-AP") != ""){strncpy(config_esp8266.Esp32_passwort, server.arg("Passwort-AP").c_str(), sizeof(config_esp8266.Esp32_passwort));}
if(server.arg("Topic") != ""){strncpy(config_esp8266.toplevel_topic, server.arg("Topic").c_str(), sizeof(config_esp8266.toplevel_topic));}
if(server.arg("MQTT-IP") != ""){strncpy(config_esp8266.MQTT_IP, server.arg("MQTT-IP").c_str(), sizeof(config_esp8266.MQTT_IP));}
if(server.arg("MQTT-Port") != ""){strncpy(config_esp8266.MQTT_Port, server.arg("MQTT-Port").c_str(), sizeof(config_esp8266.MQTT_Port));}
if(server.arg("Zielfeuchte") != ""){strncpy(config_esp8266.Zielfeuchte, server.arg("Zielfeuchte").c_str(), sizeof(config_esp8266.Zielfeuchte));}
if(server.arg("mode") != ""){strncpy(config_esp8266.mode, server.arg("mode").c_str(), sizeof(config_esp8266.mode));}
if(server.arg("PWM") != ""){strncpy(config_esp8266.pwm, server.arg("PWM").c_str(), sizeof(config_esp8266.pwm));}
saveineeprom();
server.send(200, "text/html", "<html lang='de'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Konfiguration beendet!</title> </head><body><p style='text-align: center;'><strong>Die Anlage wurde erfolgreich konfiguriert!</strong></p><p style='text-align: center;'>Neustart in 5s.</p> </body>");
delay(5000);
ESP.restart();
}else
{
if(server.arg("Password-config") == config_esp8266.Esp32_passwort)
{
/*std::string str = std::string("<!doctype html><html lang='de'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Konfiguration</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><form action='/' method='post'><h1 class=''>Konfiguration</h1><br /><div class='form-floating'><br /><label>Name der Anlage</label><input class='form-control' name='SSID-AP' type='text' placeholder='")+ std::string(config_esp8266.Esp32_name) +std::string("' maxlength='25'/></div><div class='form-floating'><br /><label>Passwort AP</label><input class='form-control' name='Passwort-AP' type='password' placeholder='")+ sternchen(strlen(config_esp8266.Esp32_passwort)) +std::string("'maxlength='40'/></div><br /><div class='form-floating'><label>SSID Wlan</label><select class='form-control' name='SSID-Wlan'>")+ optionen() +std::string("</select></div><div class='form-floating'><br /><label>Passwort Wlan</label><input class='form-control' name='Passwort-Wlan' type='password' placeholder='")+ sternchen(strlen(config_esp8266.Wifi_passwort)) +std::string("'maxlength='40'/></div><div class='form-floating'><br /><label>Toplevel Topic</label><input class='form-control' name='Topic' type='text' placeholder='")+ std::string(config_esp8266.toplevel_topic) +std::string("'maxlength='25'/></div><div class='form-floating'><br /><label>MQTT Server IP</label><input class='form-control' name='MQTT-IP' type='text' placeholder='")+ std::string(config_esp8266.MQTT_IP) +std::string("'maxlength='15'/></div><div class='form-floating'><br /><label>MQTT Server Port</label><input class='form-control' name='MQTT-Port' type='number' placeholder='")+ std::string(config_esp8266.MQTT_Port) +std::string("'min='100' max='30000'/></div><div class='form-floating'><br /><label>Zielfeuchte in %</label><input class='form-control' name='Zielfeuchte' type='number' placeholder='")+ std::string(config_esp8266.Zielfeuchte) +std::string("'min='50' max='85'/></div><br/><input type='checkbox' value = 'on' name='mode'")+ checked() +std::string("></input><label>Performance Mode</label><div class='form-floating'><br /><br /><br /><button type='submit'>Speichern</button></form></body></html>");
const char* html = str.c_str();*/
/*int size = strlen(content) + 256;
char html[size] = "";
sprintf(html, content, config_esp8266.Esp32_name,"<option>Test</option>");//, config_esp8266.toplevel_topic, config_esp8266.MQTT_IP, config_esp8266.MQTT_Port, config_esp8266.Zielfeuchte, config_esp8266.mode, config_esp8266.Zielfeuchte);
*/
server.send(200, "text/html", converthtml().c_str());
}else{server.send(404, "text/html", "Wrong Password");}    
}
}else
{
server.send(200, "text/html", indexhtml);
}
}
//*****************************************************************************************************************
void SelectBUS(uint8_t bus)
{
    Wire.beginTransmission(0x71);
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
float abs(float temp, float humid)
{
float result;
result = (4.75081*pow(1.06468,temp))*(humid/100);
return result;
}
//*****************************************************************************************************************
void Fan(float tin, float tou,float relin, float relou, float wtemp)
{
    uint8_t speed = 0;
    float ain= abs(tin, relin);
    float aou= abs(tou, relou);
    float aop= abs(tin, std::stoi(config_esp8266.Zielfeuchte));
    int statebefore = analogRead(FAN_Pin);
    int feuchterstate = digitalRead(ExtPowerOutPin);
    float tau = taupunkt(tin,relin);
    int mode = LOW;
    //Eco Mode*****************************************************************************************************************
    if(std::string(config_esp8266.mode) == std::string("eco")){
    if(relin > std::stoi(config_esp8266.Zielfeuchte) && tin >= 5)
    {
        speed = ain-aou > 0.5f;
    }
    if(speed != 1)
    {
        if(tau >= (wtemp -1.5f)||( feuchterstate == HIGH && wtemp <= (tau +3.5f)))
        {
        mode = HIGH;
        }else{mode = LOW;}
    }
    }
    //Performance Mode*****************************************************************************************************************   
    if(std::string(config_esp8266.mode) == std::string("per")){ 
        if((aou+ 0.5f) <= aop && tin >= 5)
        {
            if(statebefore == 0 && (aou+1.5f) <= aop)
            {
                mode = HIGH;
                speed = 0;
            }else{
        speed = 1;}
    }else{
        mode = HIGH;
        speed = 0;
    }}
    analogWrite(FAN_Pin, speed*(std::stof(config_esp8266.pwm)/100)*PWM_max);
    analogWrite(FAN_Pin2, speed*(std::stof(config_esp8266.pwm)/100)*PWM_max);
    analogWrite(FAN_Pin3, speed*(std::stof(config_esp8266.pwm)/100)*PWM_max);
    digitalWrite(ExtPowerOutPin, mode);
}
////////////////////////////////////////////////////////////////////////////////////////////
void checkforeeprom()
{
/*if(std::string(config_esp8266.Esp32_name) == "" || std::string(config_esp8266.Wifi_name) == "" || std::string(config_esp8266.Wifi_passwort) == "")
configreset(false);   */ 
}
//*****************************************************************************************************************
unsigned char* wifiquality()
{
    if(WiFi.RSSI() >= -70)
    return WiFi_good;
    if(WiFi.RSSI() >= -80)
    return WiFi_middle;
    if(WiFi.RSSI() >= -90)
    return WiFi_bad;
    return NoWiFi_logo;
}
//*****************************************************************************************************************
void refresh(float tin, float fin,float tout ,float fout,float taup, float presin, float presout, float wt)
{
Display.clearDisplay();
Display.setCursor(0,0);
uint8_t speed = analogRead(FAN_Pin)/PWM_max*100;
int state = digitalRead(ExtPowerOutPin);
if(speed != 0)
Display.drawBitmap(SCREEN_WIDTH - (2*bitmapsize+2), 0, Fan_enabled, bitmapsize, bitmapsize, WHITE);
if(state == HIGH)
Display.drawBitmap(SCREEN_WIDTH - (2*bitmapsize+2), 0, dehum_enabled, bitmapsize, bitmapsize, WHITE);
if(state != HIGH && speed == 0)
Display.drawBitmap(SCREEN_WIDTH - (2*bitmapsize+2), 0, nothingenabled, bitmapsize, bitmapsize, WHITE);
if(std::string(config_esp8266.mode) == std::string("eco"))
Display.drawBitmap(SCREEN_WIDTH - bitmapsize, 0, modeeco, bitmapsize, bitmapsize, WHITE);
if(std::string(config_esp8266.mode) == std::string("per"))
Display.drawBitmap(SCREEN_WIDTH - bitmapsize, 0, modeper, bitmapsize, bitmapsize, WHITE);
if(WiFi.status() == WL_CONNECTED)
Display.drawBitmap(0,0, wifiquality(),bitmapsize,bitmapsize,WHITE);
else
Display.drawBitmap(0,0, NoWiFi_logo,8,8,WHITE);
if(mqttclient.connected())
Display.drawBitmap(bitmapsize + 2 ,0, MQTT_logo,bitmapsize,bitmapsize,WHITE);
else
Display.drawBitmap(bitmapsize + 2 ,0, NoMQTT_logo,bitmapsize,bitmapsize,WHITE);
if(z_mqtt == 30)
{
z_mqtt = 0;
Fan(tin,tout,fin,fout, wt);
if(WiFi.status() == WL_CONNECTED && mqttclient.connected()){
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/InPressure")).c_str(),String(presin).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/OutPressure")).c_str(),String(presout).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/Fan2Pwm")).c_str(),String(speed).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/Fan1Pwm")).c_str(),String(speed).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/OutHumidity")).c_str(),String(fout).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/InHumidity")).c_str(),String(fin).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/OutTemp")).c_str(),String(tout).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/InTemp")).c_str(),String(tin).c_str());
mqttclient.publish(std::string(std::string(config_esp8266.toplevel_topic) + std::string("/GroundTemp")).c_str(),String(wt).c_str());
}}
z_mqtt++;
if(WiFi.softAPIP().isSet()){center(WiFi.softAPIP().toString());}else{center(WiFi.localIP().toString());}
center(config_esp8266.Esp32_name);
Display.writeLine(0,SCREEN_HEIGHT/4-1,SCREEN_WIDTH,SCREEN_HEIGHT/4-1, WHITE);
Display.setCursor(0, SCREEN_HEIGHT/4);
#if debug_mode == false
Werte("Temp In:",tin," C");
Werte("Temp Au:",tout," C");
Werte("Luft In:",abs(tin, fin),String("-" + String(fin) + "%"));
Werte("Luft Au:",abs(tout, fout),String("-" + String(fout) + "%"));
Werte("Taup In:",taup," C");
//Werte("Wandtmp:",wt," C");
Werte("PWM:", std::stof(config_esp8266.pwm)/100*PWM_max, "%");
#else
float absin = abs(tin, fin);
float absout = abs(tout, fout);
Werte("Luft In:",absin,"");
Werte("Luft Au:",absout,"");
bool lueften = absin-absout > 0.5f;
Werte("Lueften :",absin-absout, "/" + String(lueften));
#endif
Display.display();
}
//*****************************************************************************************************************
void wifi()
{
if(!WiFi.isConnected() && WiFi.getMode() == WIFI_STA){
WiFi.softAP(config_esp8266.Esp32_name, config_esp8266.Esp32_passwort, 1, 0, 3);
}
}
//*****************************************************************************************************************
/*+++++++++++++++++++++++++++++++++++++++++++++++++++
setup
+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void setup() {
pinMode(CirculationPumpPin, OUTPUT);
pinMode(ExtPowerOutPin, OUTPUT);
digitalWrite(ExtPowerOutPin, LOW);      
digitalWrite(CirculationPumpPin, LOW); 
Serial.begin(115200);
EEPROM.begin(sizeof(struct configuration));
EEPROM.get(0, config_esp8266);
checkforeeprom();
Serial.println("Konfiguration:");
Serial.println(config_esp8266.Wifi_name);
Serial.println(config_esp8266.Wifi_passwort);
Serial.println(config_esp8266.Esp32_name);
Serial.println(config_esp8266.Esp32_passwort);
Serial.println(config_esp8266.toplevel_topic);
Serial.println(config_esp8266.MQTT_IP);
Serial.println(config_esp8266.MQTT_Port);
Serial.println(config_esp8266.Zielfeuchte);
Serial.println(config_esp8266.mode);
Serial.println(config_esp8266.pwm);
Wire.begin();
Wire.setClock(clockspeed);
Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
Display.clearDisplay();
Display.setTextColor(WHITE);
Display.setTextSize(1);
SelectBUS(bmeinnen);
bmpinnen.begin(BME280_ADDRESS_ALTERNATE, &Wire);
SelectBUS(bmeaußen);
bmpaußen.begin(BME280_ADDRESS_ALTERNATE,&Wire);
SelectBUS(lm75);
walltemp.begin();
SelectBUS(Bus_notused);
pinMode(FAN_Pin, OUTPUT);
pinMode(FAN_Pin2, OUTPUT);
pinMode(FAN_Pin3, OUTPUT);
pinMode(Button_Pin, INPUT);
analogWriteFreq(PWM_frequenz);
analogWrite(FAN_Pin, PWM_max);
analogWrite(FAN_Pin2, PWM_max);
analogWrite(FAN_Pin3, PWM_max);
WiFi.begin(config_esp8266.Wifi_name, config_esp8266.Wifi_passwort);
WiFi.hostname(config_esp8266.Esp32_name);
WiFi_connect();
server.on("/", config);
server.begin();
Wire.setClock(clockspeed);
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++
loop
+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void loop() {
ArduinoOTA.handle();
SelectBUS(bmeinnen);
float tin = bmpinnen.readTemperature();
float fin = bmpinnen.readHumidity();
float pin = bmpinnen.readPressure()/100.0f;
SelectBUS(bmeaußen);
float tou = bmpaußen.readTemperature();
float fou = bmpaußen.readHumidity();
float pou = bmpaußen.readPressure()/100.0f;
SelectBUS(lm75);
float wt = (walltemp.getTemperature() + LM75Offset);
if(wt <= -tempoff || wt >= tempoff)
    wt = wandbackup;
else
    wandbackup = wt;
SelectBUS(Bus_notused);
 if(mqttclient.connected() == false)
    {
        mqttclient.connect(config_esp8266.Esp32_name);
    }
wifi();
mqttclient.loop();
refresh(tin,fin,tou,fou,taupunkt(tin,fin),pin,pou, wt);
server.handleClient();
configreset(digitalRead(Button_Pin));
delay(workingdelay);
}
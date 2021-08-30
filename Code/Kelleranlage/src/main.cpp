#include <Arduino.h>
#include <Wire.h>
#include <cmath>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Fonts/Org_01.h>
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define FAN_Pin D7
#define PWM_frequenz 25000
#define PWM_max 1023
Adafruit_SSD1306 Display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);
Adafruit_BME280 bmpinnen;
Adafruit_BME280 bmpaußen;
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
void center(String buf)
{
    int16_t x, y;
    uint16_t w,h;   
Display.getTextBounds(buf, Display.getCursorX(), Display.getCursorY(), &x,&y,&w,&h);
Display.setCursor((SCREEN_WIDTH - w)/2,Display.getCursorY());
Display.println(buf);
}
void SelectBUS(uint8_t bus)
{
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    Wire.endTransmission();
}
float taupunkt(float temp, float humid)
{
    float tau;
    humid = humid/100;
    tau = 243.13*((17.62*temp)/(243.12+temp)+log(humid))/((17.62*243.12)/(243.12+temp)-log(humid));
    return tau;
}
uint8_t Lüfter(float tin, float tou,float relin, float relou)
{
    uint8_t speed;
    float ain=(4.75081*pow(1.06468,tin))*(relin/100);
    float aou=(4.75081*pow(1.06468,tou))*(relou/100);
    speed = (ain-aou) > 1.0f;
    analogWrite(FAN_Pin, speed*PWM_max);
    return speed*100;
}
void refresh(float tin, float fin,float tout ,float fout,float taup,uint8_t speed)
{
Display.clearDisplay();
Display.setCursor(0,0);
Display.setTextSize(1);
center("Kelleranlage");
Display.setCursor(0, SCREEN_HEIGHT/4);
Display.setTextSize(1);
Display.writeLine(0,SCREEN_HEIGHT/4-1,SCREEN_WIDTH,SCREEN_HEIGHT/4-1, WHITE);
Werte("Temp Innen:",tin," C");
Werte("Temp Ausen:",tout," C");
Werte("Luft Innen:",fin," %");
Werte("Luft Ausen:",fout," %");
Werte("Taup Innen:",taup," C");
Werte("Auslastung:",speed," %");
Display.display();
}
void setup() {
Serial.begin(9600);
pinMode(FAN_Pin, OUTPUT);
analogWriteFreq(PWM_frequenz);
analogWrite(FAN_Pin, PWM_max/2);
Wire.begin();
SelectBUS(0);
Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
Display.setTextColor(WHITE);
SelectBUS(1);
bmpinnen.begin(BME280_ADDRESS_ALTERNATE, &Wire);
SelectBUS(2);
bmpaußen.begin(BME280_ADDRESS_ALTERNATE,&Wire);
}
void loop() {
delay(5000);
SelectBUS(1);
float tin = bmpinnen.readTemperature();
float fin = bmpinnen.readHumidity();
SelectBUS(2);
float tou = bmpaußen.readTemperature();
float fou = bmpaußen.readHumidity();
SelectBUS(0);
refresh(tin,fin,tou,fou,taupunkt(tin,fin),Lüfter(tin,tou,round(fin),round(fou)));
}

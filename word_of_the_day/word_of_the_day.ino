#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DEBUG

#define LIGHTPIN 10

#define SSID "babbel-hackday"
#define PASSWORD "hackday_2015"

#define DOMAIN "freegeoip.net"
#define PATH "/json/"
#define COUNTRY_CODE "CH"
#ifdef DEBUG
  SoftwareSerial debugPort(2, 3); // RX, TX
#endif

#define I2C_ADDR      0x27 // I2C address of PCF8574A
#define BACKLIGHT_PIN 3
#define En_pin        2
#define Rw_pin        1
#define Rs_pin        0
#define D4_pin        4
#define D5_pin        5
#define D6_pin        6
#define D7_pin        7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);

#ifdef DEBUG
ESP esp(&Serial, &debugPort, 4);
#else
ESP esp(&Serial, 4);
#endif

REST rest(&esp);

boolean wifiConnected = false;

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if (res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if (status == STATION_GOT_IP) {
      #ifdef DEBUG
        debugPort.println("WIFI CONNECTED");
      #endif
      wifiConnected = true;
    } else {
      wifiConnected = false;
    }

  }
}


void setup() {
  Serial.begin(19200);
 
  #ifdef DEBUG
    debugPort.begin(19200);
    debugPort.println("INIT");
  #endif
  
  lcd.begin(16, 2);
  lcd.home();
  lcd.print("Hello");

  pinMode(LIGHTPIN, OUTPUT);
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while (!esp.ready());

  delay(500);
  if (!rest.begin(DOMAIN)) {
    #ifdef DEBUG
      debugPort.println("fail");
    #endif
    while (1);
  }

  delay(500);
  esp.wifiCb.attach(&wifiCb);
  esp.wifiConnect(SSID, PASSWORD);
  #ifdef DEBUG
    debugPort.println("started");
  #endif
}

char response[50] = "";
void loop() {
  esp.process();
  if (wifiConnected) {
    rest.get(PATH);
    if (rest.getResponse(response, 50) == HTTP_STATUS_OK) {
      #ifdef DEBUG
        lcd.print(response);
        debugPort.println("get");
      #endif
    }
    delay(10000);
  }
}

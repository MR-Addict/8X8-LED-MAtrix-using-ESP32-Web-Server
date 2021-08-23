#include <WiFi.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"
#include "unities.h"

const char* ssid = "ESP32";
const char* password = "123456789";

const uint8_t LED = 2;
const uint8_t LED_PIN = 32;
const uint8_t NUM_LEDS = 64;
const uint8_t Button1 = 18;
const uint8_t Button2 = 19;
const uint8_t Button3 = 21;
const uint8_t Button4 = 23;

boolean isDisplay;
uint8_t Mode;
uint8_t BRIGHTNESS = 127;
uint8_t Palette_Mode;
uint8_t R = 0, G = 0, B = 255;
unsigned long Last_Time;

String SoftAPIP = "192.168.4.1";
String TEXT = "Welcome Visitor";

CRGBPalette16 ColorPalette[] = {RainbowColors_p, CloudColors_p, OceanColors_p, LavaColors_p, ForestColors_p};

CRGB leds[NUM_LEDS];

AsyncWebServer server(80);
WebSocketsServer websocket(81);

#include "WS_Function.h"
#include "LED_Function.h"

void setup() {
  delay(3000);
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  WiFi_Init();
}

void loop() {
  Get_Button();
  Display();
  websocket.loop();
}

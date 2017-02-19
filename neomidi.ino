#include <Adafruit_NeoPixel.h>
#include <Color.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define BTN 10
#define NUL 0
#define SLIDER0 A0
#define SLIDER1 A1
#define SLIDER2 A2


byte type;
byte val;
byte pres;
int current_val;
int current_pres;
const int list_length = 150;
uint32_t list[list_length];
float mod_i = 0;

Color c = Color();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  current_val = NUL;
  for (int i = 0; i < list_length; i++) {
    list[i] = NUL;
  }
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  stripShift();
  if (Serial.available() > 0) {
    type = Serial.read();
    delay(1);
    val = Serial.read();
//    Serial.print(type);
//    Serial.print(" ");
//    Serial.println(val);
    if (type == 49) {
      current_val = val;
      current_pres = pres;
    } else {
      if (val == current_val) {
        current_val = NUL;
        current_pres = 0;
      }
    }
    delay(1);
    Serial.read();
  }
  if (current_val == NUL) {
    list[0] = NUL;
  } else {
    int b = getSlider(SLIDER1, 0, 70) + (int)(sin(mod_i) * getSlider(SLIDER2, 0, 20));
    if (b < 0) b = 0;
    if (b > 70) b = 70;
    list[0] = getRGB(map((current_val - 21) % (88 / getSlider(SLIDER0, 1, 7)), 0, 88 / getSlider(SLIDER0, 1, 7), 0, 255), b);
  }
  mod_i += 0.1;
  if (mod_i >= 6.2) {
    mod_i = 0;
  }
  showList();
  strip.show();
}

void showList() {
  for (int i = 0; i < list_length; i++) {
    if (list[i] == NUL) {
      strip.setPixelColor(list_length - i, 0, 0, 0);
      strip.setPixelColor(list_length + i, 0, 0, 0);
    } else {
//      strip.setPixelColor(list_length - i, Wheel(map((list[i] - 21) % 12, 0, 12, 0, 255)));
//      strip.setPixelColor(list_length + i, Wheel(map((list[i] - 21) % 12, 0, 12, 0, 255)));
      strip.setPixelColor(list_length - i, list[i]);
      strip.setPixelColor(list_length + i, list[i]);
    }
  }
}

void stripShift() {
  for (int i = list_length - 1; i > 0; i--) {
    list[i] = list[i - 1];
  }
}

void setStrip(int r, int g, int b) {
  for (int i = 0; i < 300; i++) {
    strip.setPixelColor(i, r, g, b);
  }
}

void setStrip(uint32_t c) {
  for (int i = 0; i < 300; i++) {
    strip.setPixelColor(i, c);
  }
}

uint32_t getRGB(int hue, int bri) {
//  Serial.println(bri);
  c.convert_hcl_to_rgb(hue / 255.0, 1, bri / 100.0);
  return strip.Color(c.red, c.green, c.blue);
}

uint32_t getRGB(int hue, double bri) {
//  Serial.println(bri);
  c.convert_hcl_to_rgb(hue / 255.0, 1, bri);
  return strip.Color(c.red, c.green, c.blue);
}


int getSlider(int pin, int low, int high) {
  int v = analogRead(pin);
  return map(v, 0, 1023, low, high);
}



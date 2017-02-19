#include <RGBConverter.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
#define SLIDER0 A0
#define SLIDER1 A1
#define SLIDER2 A2
#define BUTTON1 10
#define BUTTON2 11
#define BUTTON3 12

#define LIGHT A3

int j;
int i;
int state;

const int n = 50;
int balls[n];
int dirs[n];
int cols[n];


Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  randomSeed(analogRead(LIGHT));
  Serial.begin(9600);
  state = 0;
  j = 0;
  i = 0;
  strip.begin();
  strip.show();
  pinMode(SLIDER0, INPUT);
  pinMode(SLIDER1, INPUT);
  pinMode(SLIDER2, INPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  randomise();
}

void loop() {
  if (digitalRead(BUTTON1) ==  LOW) {state = 0;}
  if (digitalRead(BUTTON2) ==  LOW) {state = 1;}
  if (digitalRead(BUTTON3) ==  LOW) {state = 2;}
  switch (state) {
    case 0:
      setStrip(getSlider(SLIDER0, 0, 255), getSlider(SLIDER1, 0, 255), getSlider(SLIDER2, 0, 255));
      break;
    case 1:
      j++;
      if (j > 255) j = 0;
      rainbow(getSlider(SLIDER0, 0, 20), j);   
      break;
    case 2:
      switch (getSlider(SLIDER2, 0, 3)) {
        case 0:
          i++;
          if (i > 300) i = 0;
          setStrip(0, 0, 0);
          strip.setPixelColor(i, Wheel(getSlider(SLIDER1, 0, 255)));
          delay(getSlider(SLIDER0, 0, 200));
          break;
        case 1:
          setStrip(0, 0, 0);
          for (int k = 0; k < n; k++) {
            strip.setPixelColor(balls[k], Wheel(cols[k]));
            if (!random(5) == 0) continue;
            balls[k] += dirs[k];
        //    if (balls[i] > 299) dirs[i] = -dirs[i];
        //    if (balls[i] < 0) dirs[i] = -dirs[i];
            if (balls[k] > 299) balls[k] = 0;
            if (balls[k] < 0) balls[k] = 300;
            strip.setPixelColor(balls[k], Wheel(cols[k]));
          }
          break;
        case 2:
          setStrip(0, 0, 0);
          strip.setPixelColor(getSlider(SLIDER0, 0, 299), Wheel(getSlider(SLIDER1, 0, 255)));
          break;
      }
  }
  strip.show();
}

void setStrip(int r, int g, int b) {
  for (int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
}

int getSlider(int pin, int low, int high) {
  int v = analogRead(pin);
  return map(v, 0, 1023, low, high);
}

void rainbow(uint8_t wait, int j) {
  uint16_t i;
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((i+j) & 255));
  }
  strip.show();
  delay(wait);
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void randomise() {
  for (int i = 0; i < n; i++) {
    balls[i] = random(300);
    dirs[i] = (random(2) == 0) ? -1 : 1;
    cols[i] = random(256);
  }
}


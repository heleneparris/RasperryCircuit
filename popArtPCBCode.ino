#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(64, 0, NEO_GRB + NEO_KHZ800);

int numGrid = 30;                     //represents number of neopixels
int numFrames = 2;                    //number of frames in the animation
int state = 0;                        //controls switch case
int butPin = 1;                       //debounce
boolean butState = false;             //debounce
boolean prevButState = false;         //debounce
int whichFrame = 0;                   //controls which animation frame is currently being displayed
int startTime;                        //timer
int endTime;                          //timer
int interval = 2000;                  //timer for animation
int rbInterval = 1;                   //timer for rbCycle()
boolean isRBIncreasing;               //boolean controlling hue colors of rbCycle()
unsigned long rbtone = 43690;         //hue vars for third mode (rbCycle)

char index[30] {                      //creating an array for the positions of the neopixels
  0, 1, 2,
  3, 4, 5, 6, 7,
  8, 9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21,
  22, 23, 24, 25, 26,
  27, 28, 29,
};
char colors[4][3] {                   //creating an array for storing animation color vars
  {0, 255, 0},    //0 - green
  {215, 0, 75},   //1 - rasperry pink
  {255, 0, 0},    //2 - red
};
char frames[2][30] {                  //creating frames for the animation
  //2 animation frames
  {
    0, 0, 0,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1,
  },
  {
    0, 0, 0,
    1, 0, 0, 0, 1,
    1, 2, 2, 2, 2, 2, 1,
    1, 2, 2, 2, 2, 2, 1,
    1, 2, 2, 2, 1,
    1, 1, 1,
  },
};
void setup() {
  state = 0;
  startTime = millis();
  isRBIncreasing = true;
  strip.begin(); strip.show(); strip.clear();
}
void loop() {
  endTime = millis();
  butState = digitalRead(butPin);
  butState = debounce(butPin, prevButState);
  
  if (butState == true && prevButState == false) {
    state = state + 1;
    if (state % 3 == 0) {
      state = 0;
    }
  }
  switch (state) {
    case 0: strip.clear(); strip.show(); break;
    case 1: animation(); break;
    case 2: rbCycle(); break;
  }
  prevButState = butState;
}
void animation() {
  if (whichFrame > 1) {
    whichFrame = 0;
  }
  if (endTime - startTime >= interval) {
    for (int i = 0; i < numGrid; i++) {
      int whichLed = i;
      int whichIndex = index[whichLed];
      int whichColor = frames[whichFrame][whichLed];
      int r = colors[whichColor][0];
      int g = colors[whichColor][1];
      int b = colors[whichColor][2];
      strip.setPixelColor(whichIndex, r, g, b);
      strip.show();
      startTime = millis();
    }
    whichFrame = whichFrame + 1;
  }
}
void rbCycle() {
  if (endTime - startTime >= rbInterval) {
    if (isRBIncreasing == true) {
      rbtone = rbtone + 300;
    }
    else if (isRBIncreasing == false) {
      rbtone = rbtone - 300;
    }
    if (rbtone >= 65535) {
      isRBIncreasing = false;
    }
    if (rbtone <= 43690) {
      isRBIncreasing = true;
    }
    startTime = millis();
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.ColorHSV(rbtone));
    strip.show();
  }
}
boolean debounce (int aButPin, boolean aPrevState) {
  boolean aButtonState = digitalRead(aButPin);
  if (aButtonState != aPrevState) {
    delay(15);
  }
  return aButtonState;
}

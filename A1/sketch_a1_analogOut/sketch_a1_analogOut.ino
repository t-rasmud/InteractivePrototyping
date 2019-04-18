/*
 * Assignment 1 : Interactive night light
 * Takes input from 2 sources - a photo resistive cell and a pressure sensor.
 * Change in ambient light controls the brightness of the RGB LED.
 * Change in pressure causes a change in hue. 
 * The LED switches among the 7 colors of a rainbow (mostly).
 * Every time all the 7 colors have been displayed, prints
 * "Rainbow" on the serial monitor which is used to increment
 * a rainbow count on a webpage.
 * 
 * Fragments of code taken from https://github.com/jonfroehlich/CSE599Sp2019/blob/master/
 */

const int RGB_RED_PIN = 6;        // Output pin for Red.
const int RGB_GREEN_PIN  = 5;     // Output pin for Green.
const int RGB_BLUE_PIN  = 3;      // Output pin for Blue.

const int PHOTOCELL_PIN = A0;     // the cell and 10K pulldown are connected to A0
int photocellReading;             // the analog reading from the sensor divider (photocell)
int LEDbrightness;                // input values of the photocell mapped to 0 - 255.

const int PRESSURE_PIN = A5;      // Input pin for pressure sensor (DIY lo-fi)
int pressureCellReading;          // input values of the pressure sensor mapped to 0 - 255.
int oldPressure = 0;              // these two values are used to maintain the pressure difference.
int newPressure = 0;

// Variables to store the RGB color values mapped from HSL colorspace.
int r = 0;        
int b = 0;
int g = 0;
int rgb[3];

// The seven Rainbow (VIBGYOR) colors.
float vibgyor[7] = {276.0, 180.0, 240.0, 120.0, 33.0, 347.0, 0.0};
float vibgyor_s[7] = {1.0, 0.6, 1.0, 1.0, 1.0, 1.0, 1.0};
float vibgyor_l[7] = {0.69, 0.65, 0.5, 0.25, 0.5, 0.85, 0.5};
int colorIndex = 0;
float hue;
float saturation;
float lightness;

const int DELAY = 10; // delay in ms between changing colors

const int MIN_PHOTOCELL_VAL = 100; // Photocell reading in dark
const int MAX_PHOTOCELL_VAL = 990; // Photocell reading in ambient light

const int MIN_PRESSURE_VAL = 370; // Minimum value read from pressure sensor
const int MAX_PRESSURE_VAL = 630; // Maximum value read from pressure sensor

void setup() {
  // Set the RGB pins to output
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  // Turn on Serial so we can verify input values via Serial Monitor.
  Serial.begin(9600); 
}

void loop() {
  // Read input photocell value.
  photocellReading = analogRead(PHOTOCELL_PIN); 
  Serial.print("Photocell reading = ");
  Serial.println(photocellReading);     // the raw analog reading
  delay(DELAY);

  // Read input pressure sensor value.
  pressureCellReading = analogRead(PRESSURE_PIN);
  Serial.print("Pressure reading = ");
  Serial.println(pressureCellReading);

  // Calculate the brightness to be displayed based on photocell value.
  // LED gets brighter the darker it is at the sensor
  // that means we have to -invert- the reading from 0-1023 back to 1023-0
  photocellReading = 1023 - photocellReading;
  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, MIN_PHOTOCELL_VAL, MAX_PHOTOCELL_VAL, 0, 255);
  LEDbrightness = constrain(LEDbrightness, 0, 255);

  // Calculate the difference in pressure between the previous reading and the current one.
  // Switch LED hue if the pressure has increased by at least 50.
  newPressure = map(pressureCellReading, MIN_PRESSURE_VAL, MIN_PRESSURE_VAL, 0, 255);
  newPressure = constrain(newPressure, 0, 255);
  hue = vibgyor[colorIndex];
  saturation = vibgyor_s[colorIndex];
  lightness = vibgyor_l[colorIndex];
  if ((newPressure - oldPressure) > 50) {
    colorIndex = (colorIndex + 1) % 7;
    if(colorIndex == 0) {
      // All 7 colors have been displayed.
      Serial.println("Rainbow");
    }
  }
  oldPressure = newPressure;

  // Convert HSL to RGB. 
  hsl2rgb(hue/360.0, saturation, lightness * (LEDbrightness/255.0), rgb);

  // Output to RGB LED.
  analogWrite(RGB_RED_PIN, 255 - rgb[0]);     // turn on the red LED
  analogWrite(RGB_GREEN_PIN, 255 - rgb[1]);  // turn off the green LED
  analogWrite(RGB_BLUE_PIN, 255 - rgb[2]);    // turn on the blue LED
  delay(DELAY);
}


/*
 * Function to convert HSL to RGB colorpace. 
 * Code taken from: http://courses.ischool.berkeley.edu/i262/f13/content/clemensmeyer/lab-3-potentiometers-hsl-rgb.html 
 * param h: hue ranges from 0 - 1.
 * param s: saturation ranges from 0 - 1.
 * param l: lightness ranges from 0 - 1.
 * param rgb: array to store the output rgb values.
 */
void hsl2rgb (float h, float s, float l, int rgb[]) {
  // Variables for conversion
  float q = 0;
  float t = 0;
  
  if (s == 0) {
    r = g = b = l;  //achromatic
  } else {
    if (l < 0.5) {
      q = l * (1 + s);
    } else {
      q = l + s - l * s;
    }
    float p = 2 * l - q;
    r = hue2rgb(p, q, h + 0.33) * 255;
    g = hue2rgb(p, q, h) * 255;
    b = hue2rgb(p, q, h - 0.33) * 255;
  }
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
}

/*
 * Also taken from http://courses.ischool.berkeley.edu/i262/f13/content/clemensmeyer/lab-3-potentiometers-hsl-rgb.html
 */
float hue2rgb (float p, float q, float t) {
  if(t < 0) {t += 1;}
  if(t > 1) {t -= 1;}
  if(t < 0.17) {return p + (q - p) * 6 * t;}
  if(t < 0.5) {return q;}
  if(t < 0.67) {return p + (q - p) * (0.67 - t) * 6;}
  return p;
}

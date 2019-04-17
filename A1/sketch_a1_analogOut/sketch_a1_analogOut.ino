
const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN  = 5;
const int RGB_BLUE_PIN  = 3;

const int PHOTOCELL_PIN = A0;    // the cell and 10K pulldown are connected to A0
int photocellReading;     // the analog reading from the sensor divider
int LEDbrightness;        // 
int LEDbrightness_hsl;
int oldPressure = 0;
int newPressure = 0;

const int PRESSURE_PIN = A5;
int pressureCellReading;
double temp = 0.0;

// RGB Values
int r = 0;        // Variables to store the RGB color values
int b = 0;
int g = 0;
int rgb[3];

float vibgyor[7] = {276.0, 209.0, 240.0, 120.0, 60.0, 30.0, 0.0};
float vibgyor_s[7] = {1.0, 0.83, 1.0, 1.0, 1.0, 1.0, 1.0};
float vibgyor_l[7] = {0.69, 0.3, 0.5, 0.25, 0.5, 0.5, 0.5};
int colorIndex = 2;
float hue = 276.0;
float saturation = 1.0;
float lightness = 0.69;

const int DELAY = 1000; // delay in ms between changing colors

void setup() {
  // Set the RGB pins to output
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  // Turn on Serial so we can verify expected colors via Serial Monitor
  Serial.begin(9600); 
}

void loop() {

  // Because the RGB LED we purchased for the class is a 'common anode'
  // RGB, the way we turn on each light is counter to our intuition
  // We set a pin to LOW to turn on the corresponding LED to its maximum brightness
  // and HIGH to turn it off (the opposite of what you might think)

  photocellReading = analogRead(PHOTOCELL_PIN); 
  Serial.print("Photocell reading = ");
  Serial.println(photocellReading);     // the raw analog reading
  delay(10);
  // LED gets brighter the darker it is at the sensor
  // that means we have to -invert- the reading from 0-1023 back to 1023-0
  photocellReading = 1023 - photocellReading;
  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 100, 990, 0, 255);
  LEDbrightness = constrain(LEDbrightness, 0, 255);
  // http://eduardofv.com/2011/01/15/arduino-rgb-led-hsv-color-wheel/

  pressureCellReading = analogRead(PRESSURE_PIN);
  newPressure = map(pressureCellReading, 70, 140, 0, 255);
  newPressure = constrain(newPressure, 0, 255);
  Serial.print("Pressure reading = ");
  Serial.println(newPressure);

  if ((newPressure - oldPressure) > 50) {
    colorIndex = (colorIndex + 1) % 7;
    hue = vibgyor[colorIndex];
    saturation = vibgyor_s[colorIndex];
    lightness = vibgyor_l[colorIndex];
    Serial.print("Pressure diff ====== ");
    Serial.println(hue);
    Serial.println(lightness);
  }
  oldPressure = newPressure;

  hsl2rgb(hue/360.0, saturation, lightness * (LEDbrightness/255.0), rgb);

  Serial.print("::: ");
  Serial.print(rgb[0]);
  Serial.print(rgb[1]);
  Serial.println(rgb[2]);
  analogWrite(RGB_RED_PIN, 255 - rgb[0]);     // turn on the red LED
  analogWrite(RGB_GREEN_PIN, 255 - rgb[1]);  // turn off the green LED
  analogWrite(RGB_BLUE_PIN, 255 - rgb[2]);    // turn on the blue LED
}


// http://courses.ischool.berkeley.edu/i262/f13/content/clemensmeyer/lab-3-potentiometers-hsl-rgb.html

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

 

float hue2rgb (float p, float q, float t) {

  if(t < 0) {t += 1;}

  if(t > 1) {t -= 1;}

  if(t < 0.17) {return p + (q - p) * 6 * t;}

  if(t < 0.5) {return q;}

  if(t < 0.67) {return p + (q - p) * (0.67 - t) * 6;}

  return p;
 

}

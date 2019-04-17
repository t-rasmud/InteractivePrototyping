
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
byte rgb[3];
int vibgyor[3] = {240, 120, 0};
double vibgyor_l[3] = {0.5, 0.5, 0.5};
int colorIndex = 2;
int hue = 0;
double lightness = 0.5;

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
    hue = vibgyor[colorIndex];
    lightness = vibgyor_l[colorIndex];
    colorIndex = (colorIndex + 1) % 3;
    Serial.print("Pressure diff ====== ");
    Serial.print(hue);
  }
  oldPressure = newPressure;

//  hslToRgb(hue,1,(LEDbrightness/255.0), rgb);
  hslToRgb(hue,1,lightness, rgb);
  
  analogWrite(RGB_RED_PIN, 255 - rgb[0]);     // turn on the red LED
  analogWrite(RGB_GREEN_PIN, 255 - rgb[1]);  // turn off the green LED
  analogWrite(RGB_BLUE_PIN, 255 - rgb[2]);    // turn on the blue LED
  delay(DELAY);
}

// https://github.com/ratkins/RGBConverter/blob/master/RGBConverter.cpp
/**
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   Number  h       The hue
 * @param   Number  s       The saturation
 * @param   Number  l       The lightness
 * @return  Array           The RGB representation
 */
void hslToRgb(double h, double s, double l, byte rgb[]) {
    double r, g, b;

    if (s == 0) {
        r = g = b = l; // achromatic
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3.0);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3.0);
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}

double hue2rgb(double p, double q, double t) {
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6.0) return p + (q - p) * 6 * t;
    if(t < 1/2.0) return q;
    if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
    return p;
}

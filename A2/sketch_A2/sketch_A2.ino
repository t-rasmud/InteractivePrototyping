#define MAX_ANALOG_INPUT_VAL 1023

const int RGB_RED_PIN = 6;        // Output pin for Red.
const int RGB_GREEN_PIN  = 5;     // Output pin for Green.
const int RGB_BLUE_PIN  = 3;      // Output pin for Blue.

const int POT_INPUT_PIN = A0;     // Trimpot input

const int FSR_INPUT_PIN = A5;     // Force sensitive resistor input

void setup() {
  // Set the RGB pins to output
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // read the potentiometer value
  int potVal = analogRead(POT_INPUT_PIN);
  delay(10);
  int fsrVal = analogRead(FSR_INPUT_PIN);
  Serial.println(fsrVal);
  
  // the analogRead on the Arduino Uno goes from 0 to 1023. We need to remap
  // this value to the smaller range (0-255) since the analogWrite function can 
  // only write out 0-255 (a byte--2^8). The map function provides a linear
  // mapping to do this (however, a better way would likely be some sort of
  // non-linear mapping given that perceived LED brightness is not linear with current,
  // perhaps logarithmic)
  int ledVal = map(potVal, 0, MAX_ANALOG_INPUT_VAL, 0, 255);
//  int ledVal = 0;
  
  // If fsrVal is not 0, turn on the LED.
  if (fsrVal == 0) {
   analogWrite(RGB_RED_PIN, ledVal);     // turn on the red LED
   analogWrite(RGB_GREEN_PIN, ledVal);   // turn on the green LED
   analogWrite(RGB_BLUE_PIN, ledVal);    // turn on the blue LED 
  }
  else {
    analogWrite(RGB_RED_PIN, 255);     // turn on the red LED
    analogWrite(RGB_GREEN_PIN, 255);   // turn on the green LED
    analogWrite(RGB_BLUE_PIN, 255);    // turn on the blue LED 
  }
  delay(10);
}

#include <SevSegShift.h>

unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long currentMillis;
const long interval = 1000;

const int SHIFT_PIN_SHCP = 4;
const int SHIFT_PIN_STCP = 3;
const int SHIFT_PIN_DS  = 2;

const int trigPin = 5;
const int echoPin = 6;
long duration;
int distance;
int sensorValA;
int sensorValB;

int timer;
const int goTimer = 60;
const int stopTimer = 60;
const int readyTimer = 30;
bool timerMode = false; //false = incrementing, true = decrementing


const int buzzer = A3;
bool buzzerMode = false;

const int greenLight = 11;
const int yellowLight = 12;
const int redLight = 13;

const int blueButton = A5;
const int redButton = A4;


SevSegShift sevseg(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP, 1, true);


void setup() {
  //Setup for Buttons and LEDs
  Serial.begin(9600); //start serial connection
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(blueButton, INPUT_PULLUP);
  pinMode(redButton, INPUT_PULLUP);
  pinMode(greenLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(redLight, OUTPUT);

  //Setup for Buzzer
  pinMode (buzzer, OUTPUT);
  
  //Setup for Ultrasonic Sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //Setup for Shift Registers and Display
  byte numDigits = 4;
  byte digitPins[] = {7, 8, 9, 10}; // These are the PINS of the ** Arduino **
  byte segmentPins[] = {0, 1, 2, 3, 4, 5, 6, 7}; // these are the PINs of the ** Shift register **
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(100);
  digitalWrite(redLight,HIGH);

}

void loop() {
  currentMillis = millis();
  for (timer = 1; timer < 60; timer += 0) {
    if (millis() - previousMillis >= interval) {
      buttonPress(0);
      ledLight(0);
      distanceSensor();
      timer++;
      previousMillis = millis();
    }
    displayTimer();
  }

  for (timer = 60; timer > 30; timer -= 0) {
    if (millis() - previousMillis >= interval) {
      buttonPress(1);
      ledLight(2);
      timer--;
      previousMillis = millis();
    }
    displayTimer();
  }
  for (timer = 30; timer > 0; timer -= 0) {
    if (millis() - previousMillis >= interval) {
      buttonPress(1);
      ledLight(1);
      timer--;
      previousMillis = millis();
    }
    displayTimer();
  }
}

void buttonPress(int x) {
  
  switch(x) {
    case 0:
    sensorValA = digitalRead(A5); //read the pushbutton value into a variable
    if (sensorValA == LOW) {
      timer += 15;
    }
    break;
    case 1:
    sensorValB = digitalRead(A4);//read the pushbutton value into a variable
    if (sensorValB == LOW) {
      timer += 30;
    }
    break;
  }
  
  

  
  Serial.println(sensorValA);//print out the value of the pushbutton
  Serial.println(sensorValB);//print out the value of the pushbutton


  

  if (sensorValB == LOW) {

  }
}

void buzzerBeep(int z) {
  switch(z) {
    case 0:
    digitalWrite(A3, HIGH);
    break;
    case 1:
    digitalWrite(A3, LOW);
    break;
  }
}

void displayTimer() {
  sevseg.setNumber(timer);
  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
}

void distanceSensor() {
  digitalWrite(trigPin, LOW); // Clears the trigPin
  //delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
  //delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;  // Calculating the distance
  Serial.print("Distance: ");   // Prints the distance on the Serial Monitor
  Serial.println(distance);

  if (distance <= 10) {
    buzzerBeep(0);
  }
  else {
    buzzerBeep(1);
  }
}

void ledLight(int y) {
  switch(y) {
  case 0:
    digitalWrite(greenLight,LOW);
    digitalWrite(yellowLight,LOW);
    digitalWrite(redLight,HIGH);
    break;
  case 1:
    digitalWrite(greenLight,LOW);
    digitalWrite(yellowLight,HIGH);
    digitalWrite(redLight,LOW);
    break;
  case 2:
    digitalWrite(greenLight,HIGH);
    digitalWrite(yellowLight,LOW);
    digitalWrite(redLight,LOW);
    break;
  }
}
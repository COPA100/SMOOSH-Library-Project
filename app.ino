#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int GREEN_LED = 12;
const int YELLOW_LED = 11;
const int RED_LED = 10;

const int TRIG_1 = 7;
const int ECHO_1 = 6;

const int TRIG_2 = 5;
const int ECHO_2 = 4;

int occupancyCount = 0;

// FIND MAXIMUM CAPACITY OF LIBRARY AND PUT HERE
const int MAX_CAPACITY = 250;

// adjust as necessary but these ratios are probably fine
const int LOW_THRESHOLD = 33;
const int MEDIUM_THRESHOLD = 66;

const int DETECTION_DISTANCE = 50;

bool sensor1Triggered = false;
bool sensor2Triggered = false;
unsigned long sensor1TriggerTime = 0;
unsigned long sensor2TriggerTime = 0;
const unsigned long TRIGGER_TIMEOUT = 2000;

unsigned long lastUpdateTime = 0;
const unsigned long UPDATE_DELAY = 500;

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("Library Counter");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  
  updateDisplay();
  updateLEDs();
}

void loop() {
  long distance1 = getDistance(TRIG_1, ECHO_1);
  long distance2 = getDistance(TRIG_2, ECHO_2);
  
  bool person1 = (distance1 > 0 && distance1 < DETECTION_DISTANCE);
  bool person2 = (distance2 > 0 && distance2 < DETECTION_DISTANCE);
  
  unsigned long currentTime = millis();
  
  if (sensor1Triggered && (currentTime - sensor1TriggerTime > TRIGGER_TIMEOUT)) {
    sensor1Triggered = false;
  }
  if (sensor2Triggered && (currentTime - sensor2TriggerTime > TRIGGER_TIMEOUT)) {
    sensor2Triggered = false;
  }
  
  if (person1 && !sensor1Triggered && !sensor2Triggered) {
    sensor1Triggered = true;
    sensor1TriggerTime = currentTime;
  }
  
  if (person2 && sensor1Triggered && !sensor2Triggered) {
    if (currentTime - lastUpdateTime > UPDATE_DELAY) {
      occupancyCount++;
      if (occupancyCount > MAX_CAPACITY) occupancyCount = MAX_CAPACITY;
      updateDisplay();
      updateLEDs();
      lastUpdateTime = currentTime;
    }
    sensor1Triggered = false;
    sensor2Triggered = false;
  }
  
  if (person2 && !sensor2Triggered && !sensor1Triggered) {
    sensor2Triggered = true;
    sensor2TriggerTime = currentTime;
  }
  
  if (person1 && sensor2Triggered && !sensor1Triggered) {
    if (currentTime - lastUpdateTime > UPDATE_DELAY) {
      occupancyCount--;
      if (occupancyCount < 0) occupancyCount = 0;
      updateDisplay();
      updateLEDs();
      lastUpdateTime = currentTime;
    }
    sensor1Triggered = false;
    sensor2Triggered = false;
  }
  
  delay(50);
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  
  return distance;
}

void updateDisplay() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Library Counter:");
  
  lcd.setCursor(0, 1);
  lcd.print("Count: ");
  lcd.print(occupancyCount);
  lcd.print("/");
  lcd.print(MAX_CAPACITY);
  
  lcd.setCursor(14, 1);
  int percentage = (occupancyCount * 100) / MAX_CAPACITY;
  if (percentage <= LOW_THRESHOLD) {
    lcd.print("Lo");
  } else if (percentage <= MEDIUM_THRESHOLD) {
    lcd.print("Me");
  } else {
    lcd.print("Hi");
  }
}

void updateLEDs() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  
  int percentage = (occupancyCount * 100) / MAX_CAPACITY;
  
  if (percentage <= LOW_THRESHOLD) {
    digitalWrite(GREEN_LED, HIGH);
  } else if (percentage <= MEDIUM_THRESHOLD) {
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    digitalWrite(RED_LED, HIGH);
  }
}

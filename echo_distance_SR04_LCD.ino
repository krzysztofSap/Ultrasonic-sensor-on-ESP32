#include <LiquidCrystal.h>

hw_timer_t *Timer0_Cfg = NULL; // Timer0 handler
const int trigPin = 32;
const int echoPin = 35;
const int greenLed = 25;
const int alarm_red = 33;
LiquidCrystal M_LCD(4, 0, 26, 27, 14, 13);

#define SOUND_SPEED 0.034 // in cm/us
#define SAFE_DISTANCE_BORDER 20

volatile bool measure_flag = false;
long duration;
float distanceCm;

void IRAM_ATTR Timer0_ISR() {
  measure_flag = true; // Start of measurement
}

void setup() {
  // setup of 16x2 LCD
  Serial.begin(115200);
  M_LCD.begin(16, 2);
  M_LCD.clear();
  // Configure GPIOs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(alarm_red, OUTPUT);
  
  // Welcome message
  M_LCD.print("START");
  delay(1000);
  M_LCD.clear();
  M_LCD.setCursor(2, 0);
  M_LCD.print("Distance[cm]:");
  
  // Setup of Timer0
  Timer0_Cfg= timerBegin(1000000);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR);
  timerAlarm(Timer0_Cfg, 200000, true, 50000);
  timerStart(Timer0_Cfg);
}

void loop() {
  if (measure_flag) {
    measure_flag = false; // Flag reset

    // Measure distance
    // 1. Triggering
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 2. Measuring duration of the pulse
    duration = pulseIn(echoPin, HIGH, 100000);
    // 3. Calculating distance
    distanceCm = (duration * SOUND_SPEED) / 2;

    // Send distance to serial port
    Serial.print("Distance[cm]: ");
    Serial.println(distanceCm);

    // Display distance on LCD
    M_LCD.setCursor(5, 1);
    M_LCD.print("      "); // Clearing of the old value
    M_LCD.setCursor(5, 1);
    M_LCD.print(distanceCm);

    
    // Check if the distance is within a safe range
    if(distanceCm>SAFE_DISTANCE_BORDER){
    digitalWrite(greenLed, HIGH);
    analogWrite(alarm_red, 0);
    }
    else{
    digitalWrite(greenLed, LOW);
    analogWrite(alarm_red, 140);
    }
  }

}


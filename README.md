# Ultrasonic-sensor-on-ESP32
A small program for an ESP32 that uses the HC-SR04 ultrasonic sensor to measure distance and display it on an LCD. The LCD is connected in 4-bit mode to the board.
It also provides a functionality that checks if the measured distance does not exceed the defined safe distance threshold.
When the distance is greater than the defined threshold, a green LED turns on. Otherwise, we use PWM to power a parallel-connected LED and a small buzzer.

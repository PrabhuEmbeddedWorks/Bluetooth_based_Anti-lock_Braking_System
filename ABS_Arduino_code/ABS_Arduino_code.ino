#include <SoftwareSerial.h>  // Include library to use software-based serial communication

SoftwareSerial BTSerial(2, 3); // Create Bluetooth serial on pins 2 (RX) and 3 (TX)

// Define motor control pins for Motor A (left)
#define IN1 8    // Direction control pin 1
#define IN2 9    // Direction control pin 2
#define ENA 10   // PWM speed control

// Define motor control pins for Motor B (right)
#define IN3 6    // Direction control pin 1
#define IN4 7    // Direction control pin 2
#define ENB 5    // PWM speed control

void setup() {
  // Set all motor pins as OUTPUT
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);      // Initialize Serial Monitor
  BTSerial.begin(9600);    // Initialize HC-05 Bluetooth with default baud rate
}

void loop() {
  // Check if data is available from Bluetooth module
  if (BTSerial.available()) {
    char command = BTSerial.read();  // Read command from Bluetooth
    Serial.print("Command: "); 
    Serial.println(command);         // Print received command to Serial Monitor

    // Execute command based on character received
    switch (command) {
      case 'F':       // 'F' for forward movement
        forward();
        break;
      case 'S':       // 'S' for stop
        stopMotors();
        break;
      case 'A':       // 'A' to activate anti-lock braking simulation
        absControl();
        break;
    }
  }
}

// Move both motors forward at medium speed
void forward() {
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  analogWrite(ENA, 200);    // Motor A at ~78% speed

  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
  analogWrite(ENB, 200);    // Motor B at ~78% speed
}

// Stop both motors by disabling PWM and setting direction LOW
void stopMotors() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  analogWrite(ENA, 0);      // Disable Motor A

  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW); 
  analogWrite(ENB, 0);      // Disable Motor B
}

// Simulate Anti-lock Braking System (ABS) by pulsing stop and release
void absControl() {
  Serial.println("ABS Active");
  for (int i = 0; i < 5; i++) {          // Repeat ABS cycle 5 times
    stopMotors();                        // Apply brake
    delay(100);                          // Hold brake for 100ms

    // Release briefly to simulate ABS pulsation
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW); 
    analogWrite(ENA, 120);               // Motor A at ~47% speed

    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW); 
    analogWrite(ENB, 120);               // Motor B at ~47% speed

    delay(70);                           // Short release delay

    stopMotors();                        // Brake again
    delay(100);                          // Wait before next cycle
  }
  Serial.println("ABS Complete");        // Indicate ABS routine finished
}
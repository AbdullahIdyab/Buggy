#include <Servo.h>
Servo myServo;

// Left wheel
int Ena = 3;
int In1 = 5;
int In2 = 6;

// Right wheel
int In3 = 7;
int In4 = 8;
int Enb = 9;

// Ultrasonic sensor
int TRG = 11;
int ECHO = 12;

void setup() {
  Serial.begin(9600);
  
  // Left wheel
  pinMode(Ena, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  
  // Right wheel
  pinMode(Enb, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  
  // Sensor
  pinMode(TRG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  myServo.attach(10);
  myServo.write(90); // Start centered
}

long checkAhead() {
  // Send ultrasonic pulse
  digitalWrite(TRG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRG, HIGH);  // Fixed: was LOW
  delayMicroseconds(10);
  digitalWrite(TRG, LOW);
  
  // Measure echo time
  long time = pulseIn(ECHO, HIGH);
  long dis = (time / 2) * 0.0343;
  
  Serial.print("Distance: ");
  Serial.print(dis);
  Serial.println(" cm");
  
  return dis;
}

long checkStraight() {
  myServo.write(90);  // Center
  delay(500);
  return checkAhead();
}

long checkLeft() {
  myServo.write(180);  // Look left
  delay(500);
  long dist = checkAhead();
  myServo.write(90);  // Return to center
  delay(300);
  return dist;
}

long checkRight() {
  myServo.write(0);  // Look right
  delay(500);
  long dist = checkAhead();
  myServo.write(90);  // Return to center
  delay(300);
  return dist;
}

void stopMotors() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
  analogWrite(Ena, 0);
  analogWrite(Enb, 0);
}

void moveAhead() {
  Serial.println("Forward...");
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  analogWrite(Ena, 200);  // Slightly reduced for better control
  analogWrite(Enb, 200);
}

void moveBack() {
  Serial.println("Backwards...");
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  analogWrite(Ena, 200);
  analogWrite(Enb, 200);
}

void turnRight() {
  Serial.println("Turning right...");
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  analogWrite(Ena, 200);
  analogWrite(Enb, 200);
}

void turnLeft() {
  Serial.println("Turning left...");
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  analogWrite(Ena, 200);
  analogWrite(Enb, 200);
}

void loop() {
  long distance = checkStraight();
  
  if (distance > 20) {
    // Path is clear, move forward
    moveAhead();
    delay(200);
  } else {
    // Obstacle detected, stop and assess
    stopMotors();
    delay(200);
    
    // Check both directions
    long rightDist = checkRight();
    long leftDist = checkLeft();
    
    Serial.print("Right: ");
    Serial.print(rightDist);
    Serial.print(" cm | Left: ");
    Serial.print(leftDist);
    Serial.println(" cm");
    
    // Choose the direction with more space
    if (rightDist > leftDist && rightDist > 20) {
      // Turn right
      turnRight();
      delay(600);  // Adjust timing for ~90° turn
      stopMotors();
    } else if (leftDist > 20) {
      // Turn left
      turnLeft();
      delay(600);  // Adjust timing for ~90° turn
      stopMotors();
    } else {
      // Both blocked, back up and try again
      moveBack();
      delay(800);
      turnRight();
      delay(800);
      stopMotors();
    }
    
    delay(200);
  }
}
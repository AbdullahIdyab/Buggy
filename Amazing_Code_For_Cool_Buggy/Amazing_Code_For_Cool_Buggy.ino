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
  myServo.write(90); 
}

long checkAhead() {
 
  digitalWrite(TRG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRG, HIGH);  
  delayMicroseconds(10);
  digitalWrite(TRG, LOW);
  

  long time = pulseIn(ECHO, HIGH, 30000);
  if (time == 0) {
    Serial.println("No echo");
    return 999;
  }
  long dis = (time / 2) * 0.0343;
  
  Serial.print("Distance: ");
  Serial.print(dis);
  Serial.println(" cm");
  
  return dis;
}

long checkStraight() {
  myServo.write(90); 
  delay(500);
  return checkAhead();
}

long checkLeft() {
  myServo.write(180); 
  delay(500);
  long dist = checkAhead();
  myServo.write(90);  
  delay(300);
  return dist;
}

long checkRight() {
  myServo.write(0);  
  delay(500);
  long dist = checkAhead();
  myServo.write(90); 
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
  analogWrite(Ena, 200);
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
  long funny = checkAhead();
  if (funny < 30){
    moveAhead();
    delay(2000);
    moveBack();
  }

}
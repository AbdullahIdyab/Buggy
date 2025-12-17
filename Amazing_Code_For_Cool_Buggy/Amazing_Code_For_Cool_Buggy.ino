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

// other variables
int threshold = 12; //safe distance from wall
int emergency_distance = 5;
int right_speed = 200;
int left_speed = 200;
int turn_delay = 500;
int push_delay = 500;
int forward_delay = 500; //time between left checks
int servo_turn_delay = 500;

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
  

  long time = pulseIn(ECHO, HIGH); //revmoved the "30000" number parameter
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
  myServo.write(0); 
  delay(servo_turn_delay);
  return checkAhead();
}

long checkLeft() {
  myServo.write(60); 
  delay(servo_turn_delay);
  long dist = checkAhead();
  return dist;
}

// long checkRight() {
//   myServo.write(0);  
//   delay(500);
//   long dist = checkAhead();
//   myServo.write(90); 
//   delay(300);
//   return dist;
// }

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
  analogWrite(Ena, right_speed);
  analogWrite(Enb, left_speed);
}

void moveBack() {
  Serial.println("Backwards...");
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  analogWrite(Ena, right_speed);
  analogWrite(Enb, left_speed);
}

void turnRight() {
  Serial.println("Turning right...");
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  analogWrite(Ena, right_speed);
  analogWrite(Enb, left_speed);
}

void turnLeft() {
  Serial.println("Turning left...");
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  analogWrite(Ena, right_speed);
  analogWrite(Enb, left_speed);
}

void pushForward() {
  moveAhead();
  delay(push_delay);
  stopMotors();
}

void loop() {
  //distance checks
  long straight_dis = checkStraight();
  long left_dis = checkLeft();


  //emergency case check
  if (straight_dis < emergency_distance) {
    if (left_dis < emergency_distance) {
      moveBack();
      delay(250);    // Give it enough room to move away
      stopMotors();
      turnRight();   // Pivot away from the current heading
      delay(250);
      stopMotors();
    } else {
      moveBack();
      delay(250);    // Give it enough room to move away
      stopMotors();
    }
    return; //restart loop
  }

  //Algorithm
  if (left_dis > threshold){
    pushForward();
    turnLeft();
    delay(turn_delay);
    stopMotors();
    pushForward();
  } else {
      if (straight_dis > threshold){
      moveAhead();
      delay(forward_delay);
      stopMotors();
      } else {
        turnRight();
        delay(turn_delay);
        stopMotors();
      }
  }
}
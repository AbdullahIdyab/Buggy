//library for servo to be defined and all that stuff
#include <Servo.h>
Servo myServo;

//wheel on left
int Ena = 3;
int In1 = 5;
int In2 = 6;

//wheel on right
int In3 = 7;
int In4 = 8;
int Enb = 9;

//the eyes

int TRG = 11;
int ECHO = 12;


void setup() {
  //9600 bits sent per sec
  Serial.begin(9600);
  //left wheel
  pinMode(Ena, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  //right wheel
  pinMode(Enb, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  //sensor
  pinMode(TRG, OUTPUT);
  pinMode(ECHO, INPUT);

  myServo.attach(10);

}

long checkAhead(){
  //send ultrasonic waves for 10 microseconds, before it just stops
  digitalWrite(TRG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRG, LOW);
  delayMicroseconds(10);
  digitalWrite(TRG, LOW);
  //define the distance and duration for light to get back to the echopin
  long time = pulseIn(ECHO, HIGH);
  long dis = (time / 2) * 0.0343;
  //send the chip data we collected
  Serial.print("Distance: ");
  Serial.print(dis);
  Serial.println(" cm");

  return dis;
}

long checkStright(){

  myServo.write(0);
  delay(1000);
  return checkAhead();
}

long CheckLeft(){

  myServo.write(90);
  delay(1000);
  return checkAhead();

}

long CheckRight(){

myServo.write(180);
  delay(1000);
  return checkAhead();

}



void moveAhead(){
  Serial.println("Forward...");

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  //setting the speed, max is 255
  analogWrite(Ena, 255);
  analogWrite(Enb, 255);
}

void moveBack(){
  Serial.println("Backwards...");

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  //setting the speed, max is 255
  analogWrite(Ena, 255);
  analogWrite(Enb, 255);

}

void turnRight(){

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);

  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);

  analogWrite(Ena, 255);
  analogWrite(Enb, 255);

}

void turnLeft(){

  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);

  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);

  analogWrite(Ena, 255);
  analogWrite(Enb, 255);

}

void loop() {

  moveAhead();
  delay(1000);
  long distance = checkAhead();
  delay(100);
  if (distance < 10){
    moveBack();
    delay(1000);

    CheckRight();
    long right = CheckRight();

  }

    if (right > 10){
      
      turnRight();
      delay(500);

    CheckLeft();
    long left = CheckLeft();
    }

    if (left > 10){

      turnLeft();
      delay(500);

    }
  }
}

//hi
#include <Stepper.h>
const int stepsPerRevolution = 2048;  // here go the 2048 steps

// initialize the stepper library on pins 8 through 11 -> IN1, IN2, IN3, IN4
// as shown above, we need to exchange wire 2&3, which we do in the constructor
Stepper leftStepper(stepsPerRevolution, 10, 12, 11, 13);
Stepper rightStepper(stepsPerRevolution, 6, 8, 7, 9);
int stepsToDo;  //variable to be flexible in the loop (e.g. depending on the distance to obstacles)

// HC-SR04 Sensor
int trigger = 2;
int echo = 3;
float duration = 0;
float distance = 0;
float realDistance = 0;
int steps_per_try = 500;
int steps_per_creep = 100;
//line sensor
int line_sensor_left=5;
int line_state_left=0;

int line_sensor_right=4;
int line_state_right=0;

void setup()
{
  // set the speed at 15 rpm:
  rightStepper.setSpeed(15);
  leftStepper.setSpeed(15);

  // set pinmodes for HC-04
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(line_sensor_left,INPUT);
  pinMode(line_sensor_right,INPUT);

  
  Serial.begin(9600); // Starts the serial communication
}

void loop()
{
  //get line state
  line_state_left=digitalRead(line_sensor_left);
  line_state_right=digitalRead(line_sensor_right);
  Serial.print("Line Sensor Left Reading:");
  Serial.println(line_state_left);
  Serial.print("Line Sensor Right Reading:");
  Serial.println(line_state_right);

  
  //get obstacle state
  realDistance = whatDistance();
  if (realDistance <= 0) {
    realDistance = whatDistance();
  }
  Serial.print(realDistance);
    Serial.println("cm");
  while (realDistance <= 12) {
    stop();
    Serial.println("I'm stuck! Move obstacle please!");
    Serial.print("Object detected at: ");
    Serial.print(realDistance);
    Serial.println("cm");
    delay(500);
    realDistance = whatDistance(); //get new distance
  }

  rightStepper.setSpeed(12);
  leftStepper.setSpeed(12);
  //LOW is off line
  //High is on line

  //straddling line - go forward
  if(line_state_left == LOW && line_state_right == LOW){
    goForward(steps_per_try);
    //Serial.println("Can't find line. Stopping.");
    //stop();
    //creepLeft(steps_per_creep);
    //goForward(steps_per_try);
  }

  
  if(line_state_left == HIGH && line_state_right == HIGH){
    stop();
  }else{
    //rightStepper.setSpeed(12);
    //leftStepper.setSpeed(12);
  }

  if(line_state_right == HIGH && line_state_left == LOW){
    creepRight(steps_per_creep);  
  }

  if(line_state_left == HIGH && line_state_right == LOW){
    //probe for angle/position...are we pointing left or right?
    creepLeft(steps_per_creep);
  }
  
 /* //obstacle is gone continue moving
  stepsToDo = 2048;  // steps for a complete revolution
  goForward(stepsToDo);*/

}

void goForward(int x) {
  // set the speed at 15 rpm:
  rightStepper.setSpeed(15);
  leftStepper.setSpeed(15);

  Serial.println("goForward()");
  for (int s = 0; s < x; s++) {
    leftStepper.step(1);
    rightStepper.step(-1);
  }

}

void stop(){
  rightStepper.setSpeed(0);
  leftStepper.setSpeed(0);

}

void goBack(int x) {
  // set the speed at 15 rpm:
  rightStepper.setSpeed(10);
  leftStepper.setSpeed(10);
  
  Serial.println("goBack()");
  for (int s = 0; s < x; s++) {
    leftStepper.step(-1);
    rightStepper.step(1);
  }

}


//turning left and right standing at the same place - with both motors
void turnLeft(int x) {
  // set the speed at 15 rpm:
  rightStepper.setSpeed(15);
  leftStepper.setSpeed(15);
  
  Serial.println("turnLeft()");
  for (int s = 0; s < x; s++) {
    leftStepper.step(-1);
    rightStepper.step(-1);
  }

}


void creepRight(int x) {
  // set the speed at 15 rpm:
  
  leftStepper.setSpeed(12);
  rightStepper.setSpeed(12);
  
  Serial.println("creepRight()");
  for (int s = 0; s < x; s++) {
    leftStepper.step(1);
    rightStepper.step(1);
  }

}


void creepLeft(int x) {
  // set the speed at 15 rpm:
  leftStepper.setSpeed(12);
  rightStepper.setSpeed(12);
  
  Serial.println("creepLeft()");
  for (int s = 0; s < x; s++) {
    leftStepper.step(-1);
    rightStepper.step(-1);
    
  }
}



void turnRight(int x) {
  // set the speed at 15 rpm:
  rightStepper.setSpeed(12);
  leftStepper.setSpeed(12);
  
  Serial.println("turnRight()");
  for (int s = 0; s < x; s++) {
    leftStepper.step(-1);
    rightStepper.step(-1);
  }
}


int whatDistance() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) * 0.03432;
 // Serial.println(distance);
  return distance;
}

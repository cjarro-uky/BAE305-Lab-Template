/*
  SparkFun Inventor’s Kit
  Circuit 5B - Remote Control Robot

  Control a two wheeled robot by sending direction commands through the serial monitor.
  This sketch was adapted from one of the activities in the SparkFun Guide to Arduino.
  Check out the rest of the book at
  https://www.sparkfun.com/products/14326

  This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
  This code is completely free for any use.

  View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
  Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code

  Modified by:
  Carlos Jarro for University of Kentucky's BAE305 Lab 6
  02/28/2024
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // HC-05 Tx connected to Arduino #2 & HC-05 Rx to Arduino #3

const byte numChars = 16;       
char receivedChars[numChars];  // an array to store the received data
char tempChars[numChars];

char botDir[numChars] = {0};         // char type variable for the direction of the robot
int botSpeed = 0;           //stores the speed of the whole robot
boolean newData = false;

//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 13;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 12;            //control pin 2 on the motor driver for the right motor
const int PWMA = 11;            //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 10;           //speed control pin on the motor driver for the left motor
const int BIN2 = 9;           //control pin 2 on the motor driver for the left motor
const int BIN1 = 8;           //control pin 1 on the motor driver for the left motor

const int trigPin = 6;        //trigger pin for distance snesor
const int echoPin = 7;        //echo pin for distance sensor

String botDirection;           //the direction that the robot will drive in (this change which direction the two motors spin in)
String motorSpeedStr;

int motorSpeed;               //speed integer for the motors
float duration, distance;     //duration and distance for the distance sensor

int counter = 0;

/********************************************************************************/
void setup()
{
  mySerial.begin(9600);       //Default Baud Rate for software serial communications

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  //set the distance sensor trigger pin as output and the echo pin as input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);           //begin serial communication with the computer
  //prompt the user to enter a command
  Serial.println("Enter a direction followed by speed.");
  Serial.println("f = forward, b = backward, r = turn right, l = turn left, s = stop");
  Serial.println("Example command: f 50 or s 0");
}

/********************************************************************************/
void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*340/10000)/2; // Units are cm
  counter = counter + 1;
  if (counter%200 == 0) 
  {
    mySerial.print("Distance: ");
    mySerial.println(distance);
    Serial.print("Distance: ");
    Serial.println(distance);
    delayMicroseconds(100);
  }

  if (Serial.available() > 0)                         //if the user has sent a command to the RedBoard
  {
    botDirection = Serial.readStringUntil(' ');       //read the characters in the command until you reach the first space
    motorSpeedStr = Serial.readStringUntil('\n');           //read the characters in the command until you reach the second space
    motorSpeed = motorSpeedStr.toInt();
    Serial.print(botDirection);
    Serial.print(" ");
    Serial.println(motorSpeedStr);
  }

  recvWithEndMarker();
  if (newData == true)
  {
    strcpy(tempChars, receivedChars);
    parseData();
    botDirection = botDir;
    motorSpeed = botSpeed;
    newData = false;
    Serial.println(botDirection);
    Serial.println(motorSpeed);
  }

  if (distance > 10)
  {                                                     //if the switch is in the ON position
    if (botDirection == "f")                         //if the entered direction is forward
    {
      rightMotor(-motorSpeed);                                //drive the right wheel forward
      leftMotor(motorSpeed);                                 //drive the left wheel forward
    }
    else if (botDirection == "b")                    //if the entered direction is backward
    {
      rightMotor(motorSpeed);                               //drive the right wheel forward
      leftMotor(-motorSpeed);                                //drive the left wheel forward
    }
    else if (botDirection == "r")                     //if the entered direction is right
    {
      rightMotor(motorSpeed);                               //drive the right wheel forward
      leftMotor(motorSpeed);                                 //drive the left wheel forward
    }
    else if (botDirection == "l")                   //if the entered direction is left
    {
      rightMotor(-motorSpeed);                                //drive the right wheel forward
      leftMotor(-motorSpeed);                                //drive the left wheel forward
    }
    else if (botDirection == "s")
    {
      rightMotor(0);
      leftMotor(0);
    }
  }
  else if (distance < 10)
  {
    if (botDirection == "b")
    {
      rightMotor(motorSpeed);                               //drive the right wheel forward
      leftMotor(-motorSpeed);                                //drive the left wheel forward
    }
    else
    {
      Serial.print("Object Detected at ");
      Serial.print(distance);
      Serial.println(" cm");
      rightMotor(0);                                  //turn the right motor off
      leftMotor(0);                                   //turn the left motor off
    }
  }
}
/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void recvWithEndMarker() {
/*Taken from https://forum.arduino.cc/t/serial-input-basics-updated/38200. Posted by Robin2*/
  
  static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (mySerial.available() > 0 && newData == false){
      rc = mySerial.read();
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars){
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        ndx = 0;
        newData = true;
      }
    }
}
/*****************************************************************************************/
void parseData() {      // split the data into its parts
/*Taken from https://forum.arduino.cc/t/serial-input-basics-updated/38200. Posted by Robin2*/
  
    char * strtokIndx; // this is used by strtok() as an index
    strtokIndx = strtok(tempChars," ");      // get the first part - the string
    strcpy(botDir, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, " "); // this continues where the previous call left off
    //strcpy(botSpeed, strtokIndx); // Use this line for sending speed as text
    botSpeed = atoi(strtokIndx);     // Use this line for sending sepeed as an integer

}


#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"


#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3

// Motor speed and direction constants
#define speed_Max 255
#define direction_just true // Assuming true is forward
#define direction_back false // Assuming false is backward

void setup()
{

   pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_STBY, OUTPUT);

  // Disable motors during setup
  digitalWrite(PIN_Motor_STBY, LOW);
  Serial.begin(9600);
  Serial.setTimeout(500);

  Application_FunctionSet.ApplicationFunctionSet_Init();
  wdt_enable(WDTO_2S);


}

void handleMoveForward(int speed) {


digitalWrite(PIN_Motor_STBY, HIGH); // Enable the motor controller
  digitalWrite(PIN_Motor_AIN_1, direction_just);
  digitalWrite(PIN_Motor_BIN_1, direction_just);
  analogWrite(PIN_Motor_PWMA, speed);
  analogWrite(PIN_Motor_PWMB, speed);

}

void handleMoveRight(int speed) {

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, direction_just); // Right motor forward
  digitalWrite(PIN_Motor_BIN_1, direction_back); // Left motor backward
  analogWrite(PIN_Motor_PWMA, speed);
  analogWrite(PIN_Motor_PWMB, speed);


}

void handleMoveLeft(int speed) {


 digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, direction_back); // Right motor backward
  digitalWrite(PIN_Motor_BIN_1, direction_just); // Left motor forward
  analogWrite(PIN_Motor_PWMA, speed);
  analogWrite(PIN_Motor_PWMB, speed);

}


void handleMoveBackward(int speed) {

  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, direction_back);
  digitalWrite(PIN_Motor_BIN_1, direction_back);
  analogWrite(PIN_Motor_PWMA, speed);
  analogWrite(PIN_Motor_PWMB, speed);


}

void handleMoveStop(){

   digitalWrite(PIN_Motor_STBY, LOW); // Disable the motor controller
  analogWrite(PIN_Motor_PWMA, 0);
  analogWrite(PIN_Motor_PWMB, 0);


}



void loop()
{

     if (Serial.available() > 0){

    // String command = Serial.readStringUntil('\n');
    String command = Serial.readString();
    String command1 = Serial.readString();
    command.trim();
    command1.trim();
    Serial.print(command);

    if(command1 == command){
      Serial.print("conditional 1 worked");

    }

    if(command == "FORWARD"){
      //  if(command == "842018941"){

      handleMoveForward(250);
      Serial.print(" after forward");

      }
                    else if(command == "HALFFORWARD"){

      handleMoveForward(125);
      }
        else if(command == "BACKWARD"){

      handleMoveBackward(250);
         Serial.print(" after backward");
      }
              else if(command == "HALFBACKWARD"){

      handleMoveBackward(125);
         Serial.print(" after halfbackward");
      }
              else if(command == "RIGHT"){

      handleMoveRight(250);
      }
              else if(command == "HALFRIGHT"){

      handleMoveRight(125);
      }

              else if(command == "LEFT"){

      handleMoveLeft(250);
      }

                    else if(command == "HALFLEFT"){

      handleMoveLeft(125);
      }
                    else if(command == "STOP"){

      handleMoveStop();
      }}





    }



#include <PS2X_lib.h>
#include <Stepper.h>
#include <Servo.h>

#define DATA 5
#define COMMAND 6
#define CLOCK 8
#define ATTENTION 7
#define STEPPER_PIN_1 49
#define STEPPER_PIN_2 48
#define STEPPER_PIN_3 50
#define STEPPER_PIN_4 51
#define M370_PIN_1 12
#define M370_PIN_2 11
#define M370_PIN_3 10
#define M370_PIN_4 9
#define LSPEED 3
#define RSPEED 4
#define SERVO_ARM_LEFT 52
#define SERVO_ARM_RIGHT 53
#define LEFT ( arm )
#define RIGHT ( 180 - arm )

PS2X ps2x;
Stepper Stp(500,STEPPER_PIN_1,STEPPER_PIN_2,STEPPER_PIN_3,STEPPER_PIN_4);
Servo Left;
Servo Right;

int error = 0; 
byte vibrate = 0;
const int Step = 10;
int arm = 20;

void setup(){
  Serial.begin(57600);
  
  pinMode(M370_PIN_1,OUTPUT);
  pinMode(M370_PIN_2,OUTPUT);
  pinMode(M370_PIN_3,OUTPUT);
  pinMode(M370_PIN_4,OUTPUT);  
  pinMode(LSPEED,OUTPUT);
  pinMode(RSPEED,OUTPUT);
  analogWrite(LSPEED,255);
  analogWrite(RSPEED,255);
  error = ps2x.config_gamepad(CLOCK,COMMAND,ATTENTION,DATA, true, true);
  Stp.setSpeed(60);

  Left.attach(SERVO_ARM_LEFT);
  Right.attach(SERVO_ARM_RIGHT);
}

void loop(){
  if(error != 0){ error = ps2x.config_gamepad(CLOCK,COMMAND,ATTENTION,DATA, true, true);return;}
  ps2x.read_gamepad(false, vibrate);

  if(ps2x.Button(PSB_L1)){//stepper
    if(ps2x.Analog(PSS_LY)<100){
      Stp.step(Step);
    }
    if(ps2x.Analog(PSS_LY)>155){
      Stp.step(-Step);
    }
    if(ps2x.Analog(PSS_RY)<100){
      if(arm < 180){
        arm = arm + 1;
        Left.write(LEFT);
        Right.write(RIGHT);
        //delay(15);
      }
    }
    if(ps2x.Analog(PSS_RY)>155){
      if(arm > 0){
        arm = arm - 1;
        Left.write(LEFT);
        Right.write(RIGHT);
        //delay(15);
      }
    }
  }
  
  if(ps2x.ButtonPressed(PSB_TRIANGLE)){//arm 10cm
    arm = 0;
    Left.write(LEFT);
    Right.write(RIGHT);
   }
  if(ps2x.ButtonPressed(PSB_CIRCLE)){//arm 0cm
    arm = 23;
    Left.write(LEFT);
    Right.write(RIGHT);
  }
  
  if(ps2x.ButtonPressed(PSB_SQUARE)){//arm 5cm
    arm = 26;
    Left.write(LEFT);
    Right.write(RIGHT);
  }
  
  if(ps2x.ButtonPressed(PSB_PAD_DOWN)){
    digitalWrite(M370_PIN_1,HIGH);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,HIGH);
    analogWrite(LSPEED,160);
    analogWrite(RSPEED,160);
    delay(150);
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,LOW);
  }
  if(ps2x.ButtonPressed(PSB_PAD_UP)){
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,HIGH);
    digitalWrite(M370_PIN_3,HIGH);
    digitalWrite(M370_PIN_4,LOW);
    analogWrite(LSPEED,160);
    analogWrite(RSPEED,160);
    delay(150);
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,LOW);
  }
  if(ps2x.Button(PSB_PAD_RIGHT)){//direction
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,HIGH);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,HIGH);
    analogWrite(LSPEED,255);
    analogWrite(RSPEED,160);
    delay(150);
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,LOW);
  }
  if(ps2x.Button(PSB_PAD_LEFT)){//direction
    digitalWrite(M370_PIN_1,HIGH);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,HIGH);
    digitalWrite(M370_PIN_4,LOW);
    analogWrite(LSPEED,160);
    analogWrite(RSPEED,255);
    delay(150);
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,LOW);
  }
  if(ps2x.ButtonReleased(PSB_CROSS)){
    digitalWrite(M370_PIN_1,LOW);
    digitalWrite(M370_PIN_2,LOW);
    digitalWrite(M370_PIN_3,LOW);
    digitalWrite(M370_PIN_4,LOW);
    Serial.println(arm);
    arm = 148;
    Left.write(LEFT);
    Right.write(RIGHT);
  }
  if(ps2x.Button(PSB_R1)){
    arm = 30;
    Left.write(LEFT);
    Right.write(RIGHT); 
  }
  if(ps2x.Button(PSB_L2)){
    analogWrite(LSPEED,255);
  }
  else{
    analogWrite(LSPEED,160);
  }
  if(ps2x.Button(PSB_R2)){
    analogWrite(RSPEED,255);
  }
  else{
    analogWrite(RSPEED,160);
  }
  if(!((ps2x.Button(PSB_L1))||(ps2x.Button(PSB_R1)))){
    if(ps2x.Analog(PSS_LY)<100){
      digitalWrite(M370_PIN_1,LOW);
      digitalWrite(M370_PIN_2,HIGH);
    }
    else if(ps2x.Analog(PSS_LY)>155){
        digitalWrite(M370_PIN_1,HIGH);
        digitalWrite(M370_PIN_2,LOW);
    }
    else{
      digitalWrite(M370_PIN_1,LOW);
      digitalWrite(M370_PIN_2,LOW);
    }
    if(ps2x.Analog(PSS_RY)<100){
      digitalWrite(M370_PIN_3,HIGH);
      digitalWrite(M370_PIN_4,LOW);
    }
    else if(ps2x.Analog(PSS_RY)>155){
        digitalWrite(M370_PIN_3,LOW);
        digitalWrite(M370_PIN_4,HIGH);
    }
    else{
      digitalWrite(M370_PIN_3,LOW);
      digitalWrite(M370_PIN_4,LOW);
    }
  }
  delay(10);
}


/*
* Name: övningsprojekt
* Author: Isak Grönlund
* Date: 2026-02-06
* Description:
* This project uses a solenoid, relays, IR remote, IR sensor, battery,
* two motors and wheels.
* The car can drive and is meant to play football with another car.
* The solenoid is used to shoot the ball.
*/

#include <IRremote.hpp>

/*
* IR receiver pin definition
*/
#define IR_PIN 3

/*
* Output pins for solenoid and motors
*/
int solenoidPin = 6;
int rightWheel = 10;
int leftWheel = 11;

/*
* IR remote button HEX codes
*/
unsigned long IR_FORWARD  = 0xB946FF00;
unsigned long IR_RIGHT    = 0xBC43FF00;
unsigned long IR_LEFT     = 0xBB44FF00;
unsigned long IR_STOP     = 0xE619FF00;
unsigned long IR_SOLENOID = 0xBF40FF00;

/*
* Enum for car movement states
*/
enum State {
  FRAM,
  HOGER,
  VANSTER,
  STANNA
};

/*
* Current active state of the car
*/
State aktiv = STANNA;

/*
* Setup function runs once when Arduino starts
*/
void setup() {

  /*
  * Start IR receiver
  */
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  /*
  * Set pins as outputs
  */
  pinMode(solenoidPin, OUTPUT);
  pinMode(rightWheel, OUTPUT);
  pinMode(leftWheel, OUTPUT);

  /*
  * Start serial communication
  */
  Serial.begin(9600);
}

/*
* Drive forward
*/
void fram(){
  digitalWrite(leftWheel, HIGH);
  digitalWrite(rightWheel, HIGH);
}

/*
* Turn right
*/
void hoger(){
  digitalWrite(leftWheel, HIGH);
  digitalWrite(rightWheel, LOW);
  delay(1000);

  digitalWrite(leftWheel, LOW);
  digitalWrite(rightWheel, LOW);
  delay(500);

  aktiv = STANNA;
}

/*
* Turn left
*/
void vanster(){
  digitalWrite(rightWheel, HIGH);
  digitalWrite(leftWheel, LOW);
  delay(1000);

  digitalWrite(rightWheel, LOW);
  digitalWrite(leftWheel, LOW);
  delay(500);

  aktiv = STANNA;
}

/*
* Stop both motors
*/
void stanna(){
  digitalWrite(rightWheel, LOW);
  digitalWrite(leftWheel, LOW);
}

/*
* Activate solenoid to shoot the ball
*/
void aktiverasolenoid(){
  digitalWrite(solenoidPin, HIGH);
  delay(100);

  digitalWrite(solenoidPin, LOW);
  delay(100);
}

/*
* Main loop
*/
void loop() {

  /*
  * Check if IR signal is received
  */
  if (IrReceiver.decode()) {

    /*
    * Read received IR code
    */
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(code, HEX);

    /*
    * Change state depending on button pressed
    */
    if (code == IR_FORWARD) aktiv = FRAM;
    else if (code == IR_RIGHT) aktiv = HOGER;
    else if (code == IR_LEFT) aktiv = VANSTER;
    else if (code == IR_STOP) aktiv = STANNA;
    else if (code == IR_SOLENOID) aktiverasolenoid();

    /*
    * Prepare receiver for next signal
    */
    IrReceiver.resume();
  }

  /*
  * Do movement based on current state
  */
  switch (aktiv) {

    case FRAM:
      fram();
      break;

    case HOGER:
      hoger();
      break;

    case VANSTER:
      vanster();
      break;

    case STANNA:
      stanna();
      break;
  }
}

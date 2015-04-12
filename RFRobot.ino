#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

#define FORWARD_BUTTON TK0
#define BACKWARD_BUTTON TK1

#define LEFT_BUTTON TK2
#define RIGHT_BUTTON TK3

#define MIN_TURTLE_POWER 800

void setup() {
  // put your setup code here, to run once:
  Robot.begin();
  Robot.beginTFT();
}

void loop() {
  // I'm over here.
  int turtlePower = getTurtlePower();

  if (isDebugTime()) {
    Robot.debugPrint(turtlePower);
    Robot.digitalWrite(LED1, LOW);
    delay(10);
    Robot.digitalWrite(LED1, HIGH);
  }

  if (turtlePower < MIN_TURTLE_POWER) {
    // Help I've fallen and I can't get up!
    Robot.motorsStop();
  } else {
    // Alright!! Let's do this!!
    updateMotors();
  }
}

int getTurtlePower() {
  Robot.updateIR();
  int turtleFactor = 0;

  for(int i = 0; i <= 4; i++) {
    turtleFactor += Robot.IRarray[i];
  }

  turtleFactor /= 5;

  return turtleFactor;
}

void updateMotors() {

  boolean forward;
  boolean backward;
  boolean left;
  boolean right;
  boolean allstop;

  forward = Robot.digitalRead(FORWARD_BUTTON) == HIGH;
  backward = Robot.digitalRead(BACKWARD_BUTTON);
  right = Robot.digitalRead(RIGHT_BUTTON);
  left = Robot.digitalRead(LEFT_BUTTON);
  allstop = !(forward || backward || right || left);

  int lmotor = 0;
  int rmotor = 0;

  if (allstop) {
    Robot.motorsStop();
  }
  else {
    if (forward) {
      lmotor = 127;
      rmotor = 127;
    } else if (backward) {
      lmotor = -127;
      rmotor = -127;
    } else {
      lmotor = 0;
      rmotor = 0;
    }

    if (left) {
      lmotor = lmotor - 127;
      rmotor = rmotor + 127;
    } else if (right) {
      lmotor = lmotor + 127;
      rmotor = rmotor - 127;
    } else {
    }

    lmotor = constrain(lmotor, -255, 255);
    rmotor = constrain(rmotor, -255, 255);

    Robot.motorsWrite(lmotor, rmotor);
  }
}

boolean isDebugTime() {
  static unsigned int beep_start = 0;
  static unsigned int beep_end = 0;

  boolean beepTime;
  beep_end = millis();

  beepTime = (beep_end - beep_start) > 1000;

  if (beepTime) {
    beep_start = millis();
  }

  return beepTime;
}


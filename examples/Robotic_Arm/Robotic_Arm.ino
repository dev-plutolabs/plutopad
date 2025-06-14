/**
 * Example sketch for using PlutoPAD.h controller library with a wireless ESP32 to control a 3-axis (servo) robotic arm.
 * 
 * This sketch demonstrates how to use the PlutoPAD.h library to connect to a PlutoPAD controller,
 * respond to user button presses moving specific servo motors (open/close, up/down) to control the robotic arm.
 * 
 */
 
#include <ESP32Servo.h>
#include "PlutoPAD.h"

// Declare PlutoPAD controller object
PlutoPAD plutoPAD;

// Declare 3-axis servo motors
Servo BASE, BOTTOM, MIDDLE, GRIPPER;

// Initialise their start position
int basePos = 90;
int bottomPos = 90;
int middlePos = 90;
int gripperPos = 90;

// Store the previous button states (initialised in setup)
uint16_t previousButtonState;

// Adjust this delay to control speed (lower = faster, higher = slower)
int servoSpeedDelay = 10;

void setup() 
{
  Serial.begin(115200);
  
  // Attach servos to pins (use GPIOs with PWM capability)
  BASE.setPeriodHertz(50);    BASE.attach(26);
  BOTTOM.setPeriodHertz(50);  BOTTOM.attach(27);
  MIDDLE.setPeriodHertz(50);  MIDDLE.attach(32);
  GRIPPER.setPeriodHertz(50); GRIPPER.attach(33);

  // Initialise servo motors to GPIOs
  BASE.write(basePos);
  BOTTOM.write(bottomPos);
  MIDDLE.write(middlePos);
  GRIPPER.write(gripperPos);

  // Start PlutoPAD and register event callbacks
  plutoPAD.begin("RoboticArm", false);
  plutoPAD.linkOnChange(onButtonChange);
  plutoPAD.linkOnConnect(onConnect);
  plutoPAD.linkOnDisconnect(onDisconnect);

  // Initialise previousButtonState with actual current state
  previousButtonState = plutoPAD.button.raw;

  delay(500);
}


void loop() 
{
  plutoPAD.run(); // Run PlutoPAD...

  //***************
  // GRIPPER CODE
  //***************
  // Close gripper toward 0°
  if (plutoPAD.button.right_b_pressed() && gripperPos > 55) 
  {
    gripperPos--;
    GRIPPER.write(gripperPos);
    delay(servoSpeedDelay);
  }

  // Open gripper toward 180°
  if (plutoPAD.button.left_b_pressed() && gripperPos < 155) 
  {
    gripperPos++;
    GRIPPER.write(gripperPos);
    delay(servoSpeedDelay);
  }

  //***************
  // MIDDLE CODE
  //***************
  // Close middle toward 0°
  if (plutoPAD.button.up_b_pressed() && middlePos > 0) 
  {
    middlePos--;
    MIDDLE.write(middlePos);
    delay(servoSpeedDelay);
  }

  // Open middle toward 180°
  if (plutoPAD.button.down_b_pressed() && middlePos < 180) 
  {
    middlePos++;
    MIDDLE.write(middlePos);
    delay(servoSpeedDelay);
  }

  //***************
  // BOTTOM CODE
  //***************
  // Close bottom toward 0°
  if (plutoPAD.button.down_a_pressed() && bottomPos > 0) 
  {
    bottomPos--;
    BOTTOM.write(bottomPos);
    delay(servoSpeedDelay);
  }

  // Open bottom toward 180°
  if (plutoPAD.button.up_a_pressed() && bottomPos < 180) 
  {
    bottomPos++;
    BOTTOM.write(bottomPos);
    delay(servoSpeedDelay);
  }
  
  //***************
  // BASE CODE
  //***************
  // Close base toward 0°
  if (plutoPAD.button.right_a_pressed() && basePos > 0) 
  {
    basePos--;
    BASE.write(basePos);
    delay(servoSpeedDelay);
  }

  // Open base toward 180°
  if (plutoPAD.button.left_a_pressed() && basePos < 180) 
  {
    basePos++;
    BASE.write(basePos);
    delay(servoSpeedDelay);
  }

  // If no button is held, do nothing
}

// Callback for button state changes
void onButtonChange(void) 
{
    uint16_t currentState = plutoPAD.button.raw; // Get current button states
    uint16_t previousState = previousButtonState; // Store previous button states
    previousButtonState = currentState; // Update previous button states

    for (int i = 0; i < 16; i++) 
    {
        bool wasPressed = (previousState >> i) & 1;  // Previous button state
        bool isPressed = (currentState >> i) & 1; // Current button state

        if (wasPressed != isPressed) // Only print if the state has changed
        {
            Serial.print("@RoboticArm: ");
            Serial.print(plutoPAD.getButtonName(i)); // Fetch button name from the library
            Serial.println(isPressed ? " pressed!" : " released!");
        }
    }
}

// Callback for connection event
void onConnect(void) 
{
    Serial.println("@RoboticArm: Successfully connected to PlutoPAD!");
}

// Callback for disconnection event
void onDisconnect(void) 
{
    Serial.println("@RoboticArm: Lost connection to PlutoPAD!");
}

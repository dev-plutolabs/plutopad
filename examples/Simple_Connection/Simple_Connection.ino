/**
 * Example sketch for using PlutoPAD.h controller library with a wireless ESP32 or Arduino device.
 * 
 * This sketch demonstrates how to use the PlutoPAD.h library to connect to a PlutoPAD controller,
 * manage button states, and respond to user input.
 * 
 */

#include "PlutoPAD.h"

// Declare PlutoPAD controller object
PlutoPAD plutoPAD;

// Declare function signatures
void onButtonChange(void);
void onConnect(void);
void onDisconnect(void);

// Store the previous button states (initialised in setup)
uint16_t previousButtonState;

void setup() 
{
    Serial.begin(115200);

    // Start PlutoPAD and register event callbacks
    plutoPAD.begin("MyWirelessDevice", false);
    plutoPAD.linkOnChange(onButtonChange);
    plutoPAD.linkOnConnect(onConnect);
    plutoPAD.linkOnDisconnect(onDisconnect);

    // Initialise previousButtonState with actual current state
    previousButtonState = plutoPAD.button.raw;
}

void loop() 
{
    // Run PlutoPAD system
    plutoPAD.run();

    // Example of checking a specific button state
    if (plutoPAD.button.centre_b_pressed()) // Check if centre_b button is pressed
    {
        Serial.println("@MyWirelessDevice: Centre B button pressed!");
    }
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
            Serial.print("@MyWirelessDevice: ");
            Serial.print(plutoPAD.getButtonName(i)); // Fetch button name from the library
            Serial.println(isPressed ? " pressed!" : " released!");
        }
    }
}

// Callback for connection event
void onConnect(void) 
{
    Serial.println("@MyWirelessDevice: Successfully connected to PlutoPAD!");
}

// Callback for disconnection event
void onDisconnect(void) 
{
    Serial.println("@MyWirelessDevice: Lost connection to PlutoPAD!");
}

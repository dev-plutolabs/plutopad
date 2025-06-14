/**
 * Example framework sketch for using PlutoPAD.h controller library.
 * 
 * This sketch demonstrates gives the user an empty sketch framework of the typical use of the PlutoPAD controller library.
 * 
 */

#include "PlutoPAD.h"

// Declare PlutoPAD controller object
PlutoPAD plutoPAD;

// Declare function signatures
void onButtonChange(void);
void onConnect(void);
void onDisconnect(void);

void setup() 
{
    Serial.begin(115200);
    
    // Start PlutoPAD and register event callbacks
    plutoPAD.begin("Device");
    plutoPAD.linkOnChange(onButtonChange);
    plutoPAD.linkOnConnect(onConnect);
    plutoPAD.linkOnDisconnect(onDisconnect);
}

void loop() 
{
  // Run PlutoPAD system
  plutoPAD.run();
}

// Callback for button state changes
void onButtonChange(void) 
{
  Serial.println("@Device: A button has changed!");
}

// Callback for connection event
void onConnect(void) 
{
    Serial.println("@Device: Successfully connected to PlutoPAD!");
}

// Callback for disconnection event
void onDisconnect(void) 
{
    Serial.println("@Device: Lost connection to PlutoPAD!");
}

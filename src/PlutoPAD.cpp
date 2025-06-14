/**
 * @file PlutoPAD.cpp
 * @brief Implementation of the PlutoPAD Bluetooth code controller code interface.
 *
 * This file contains the implementation of the PlutoPAD class, which handles:
 *   - Bluetooth Classic communication with a paired receiver (e.g., PlutoBUG)
 *   - 16-button state management packed into a 2-byte packet
 *   - Connection monitoring and user-defined callbacks for button, connect, and disconnect events
 *   - Utility functions including button name lookup and bitfield access
 *
 * PlutoPAD is designed to work as a Bluetooth HID-style interface for microcontroller-based robots
 * and applications, enabling wireless control through simple packet transmission.
 * *
 * @author  
 * Peter Kyriakides
 *
 * @version 0.1.1
 * @date    15-06-2025
 * 
 * @note Subject to the GNU General Public License v3.0 (GPL-3.0).
 */

#include "PlutoPAD.h"

PlutoPAD::PlutoPAD() : client_isConnected(false) 
{
    resetButtons();
}

// Function to initialise own Bluetooth connection and connect to PlutoPAD
// (will always/only look for PlutoPAD device)
// - const char* deviceName: Name of own Bluetooth device
// - bool debugMode: Enable debug mode for serial prints | DEFAULT: false
void PlutoPAD::begin(const char* deviceName, bool debugMode) 
{
    DEBUG_MODE = debugMode;
    if (!serialBluetooth.begin(deviceName, true)) 
    { // Start Bluetooth in client mode
        if(DEBUG_MODE) Serial.println("@>: *ERROR* Bluetooth Unable to Initialised!");
        while (1);
    }
    if(DEBUG_MODE) Serial.print("@>: Bluetooth started!");
    connectController();
}

// Function to attempt connection to PlutoPAD
void PlutoPAD::connectController(void) 
{
    if(DEBUG_MODE) Serial.println("@>: Attempting to connect to PlutoPAD...");
    if (serialBluetooth.connect("PlutoPAD")) 
    {  // Attempt to connect
        if(DEBUG_MODE) Serial.println("@>: Connected to PlutoPAD!");
        client_isConnected = true;
        if (connectCallback) connectCallback();
    } else 
    {
        if(DEBUG_MODE) Serial.println("@>: Connection failed! Retrying...");
        client_isConnected = false;
    }
}

// Function to end own Bluetooth connection
void PlutoPAD::end() 
{
    serialBluetooth.end();
    if(DEBUG_MODE) Serial.println("@>: Bluetooth stopped!");
}

// Function to check if connected to PlutoPAD
// - Returns true if connected, false otherwise
bool PlutoPAD::isConnected() 
{
    return serialBluetooth.hasClient();
}

// Function to link user-defined callback to button state change
void PlutoPAD::linkOnChange(void (*callback)()) 
{
    buttonCallback = callback;
}

// Function to link user-defined callback to connection event
void PlutoPAD::linkOnConnect(void (*callback)()) 
{
    connectCallback = callback;
}

// Function to link user-defined callback to disconnection event
void PlutoPAD::linkOnDisconnect(void (*callback)()) 
{
    disconnectCallback = callback;
}

// Function to reset all button states to false
void PlutoPAD::resetButtons() 
{
    memset(&button, 0, sizeof(button)); // Reset all button states to false
}


// Function to run PlutoPAD system
// - Checks connection status
// - Handles input packet from PlutoPAD
void PlutoPAD::run(void) 
{
    checkConnection();
    handleInput();
}

// Function to check connection status to PlutoPAD
// - If disconnected, attempt to reconnect
void PlutoPAD::checkConnection(void) 
{
    if (!serialBluetooth.connected()) 
    {
        if (client_isConnected)  // Only trigger disconnect event once
        {
            if(DEBUG_MODE) Serial.println("@>: Connection lost! Reconnecting...");
            client_isConnected = false;
            resetButtons(); // Reset all button states
            if (disconnectCallback) disconnectCallback(); // Call user-defined disconnect function
        }
        connectController(); // Attempt to reconnect
    }
}

// Function to handle input from PlutoPAD
// - If input is available, read and update button states
void PlutoPAD::handleInput() 
{
    // Exit function if no client or new bluetooth available
    if (!client_isConnected || serialBluetooth.available() < 2)  
        return;

    // 1) read the 2‑byte packet once
    uint16_t newRaw = 0;
    serialBluetooth.readBytes((uint8_t*)&newRaw, sizeof(newRaw));

    // 2) stash the old value
    uint16_t oldRaw = button.raw;

    // 3) update the raw bits once
    button.raw = newRaw;

    // 4) for each bit that changed, update the bool and fire the callback
    for (int i = 0; i < 16; i++) 
    {
        bool prev = (oldRaw >> i) & 1;
        bool curr = (newRaw >> i) & 1;
        if (prev != curr) 
        {
            updateButtonState(i, curr);
            if (buttonCallback) 
                buttonCallback();
        }
    }
}

// Function to get the raw packet received via bluetooth
// - Returns the raw packet as a 16-bit value
uint16_t PlutoPAD::getRawPacket() const
{
    return button.raw;
}

// Define button names in the correct order
const char* PlutoPAD::buttonNames[16] = 
{
    "Centre B", "Down B", "Left B", "Middle B", "Right B", "Up B", 
    "R1", "R2", "L2", "L1", 
    "Up A", "Right A", "Middle A", "Left A", "Down A", "Centre A"
};

// Function to get button name by index
const char* PlutoPAD::getButtonName(int index) 
{
    if (index >= 0 && index < 16) 
    {
        return buttonNames[index];
    }
    return "Unknown Button"; // Fallback in case of out-of-range index
}
/**
 * @file PlutoPAD.h
 * @brief Header file for the PlutoPAD Bluetooth controller code interface.
 *
 * This file declares the PlutoPAD class, which manages a 16-button Bluetooth Classic
 * controller for embedded systems. It includes:
 *   - Bluetooth client initialisation and reconnection
 *   - Button state tracking via a 2-byte bitfield
 *   - User-defined callback support for button changes, connections, and disconnections
 *   - Accessor for readable button names by index
 *
 * The button layout and bit order align with a logical left-to-right, bottom-to-top
 * arrangement, compatible with PlutoBUG and similar robotics applications.
 *
 * @author
 * Peter Kyriakides
 *
 * @version 1.0
 * @date 14-06-2025
 * 
 * @note Subject to the GNU General Public License v3.0 (GPL-3.0).
 */

#ifndef PLUTOPAD_H
#define PLUTOPAD_H

#include "BluetoothSerial.h"

// Class to handle PlutoPAD controller
class PlutoPAD 
{
public: // Public methods
    PlutoPAD();

    void begin(const char* deviceName, bool debugMode = false); //
    void end(void);
    void run(void);
    bool isConnected(void);
    void linkOnChange(void (*callback)());
    void linkOnConnect(void (*callback)());
    void linkOnDisconnect(void (*callback)());
    const char* getButtonName(int index); // Function to return button names
    uint16_t getRawPacket() const; // Function to return the raw packet received

    // Struct to store button states
    struct ButtonStates 
    {
        union {
            struct 
            {
                bool centre_b, down_b, left_b, middle_b, right_b, up_b;
                bool r1, r2, l2, l1;
                bool up_a, right_a, middle_a, left_a, down_a, centre_a;
            };
            uint16_t raw; // Store entire button state as a 16-bit value
        };

        // Macro to generate pressed and released functions
        // - Example: <name>.<button>_pressed() or <name>.<button>_released()
        #define BUTTON_STATE_FUNCTIONS(button) \
            bool button##_pressed() const { return button; } \
            bool button##_released() const { return !button; }

        BUTTON_STATE_FUNCTIONS(centre_b)
        BUTTON_STATE_FUNCTIONS(down_b)
        BUTTON_STATE_FUNCTIONS(left_b)
        BUTTON_STATE_FUNCTIONS(middle_b)
        BUTTON_STATE_FUNCTIONS(right_b)
        BUTTON_STATE_FUNCTIONS(up_b)
        BUTTON_STATE_FUNCTIONS(r1)
        BUTTON_STATE_FUNCTIONS(r2)
        BUTTON_STATE_FUNCTIONS(l2)
        BUTTON_STATE_FUNCTIONS(l1)
        BUTTON_STATE_FUNCTIONS(up_a)
        BUTTON_STATE_FUNCTIONS(right_a)
        BUTTON_STATE_FUNCTIONS(middle_a)
        BUTTON_STATE_FUNCTIONS(left_a)
        BUTTON_STATE_FUNCTIONS(down_a)
        BUTTON_STATE_FUNCTIONS(centre_a)

        #undef BUTTON_STATE_FUNCTIONS
    } button;

private: // Private members
    BluetoothSerial serialBluetooth;
    bool client_isConnected;
    bool DEBUG_MODE;
    void (*buttonCallback)() = nullptr;
    void (*connectCallback)() = nullptr;
    void (*disconnectCallback)() = nullptr;

    void checkConnection(void);
    void handleInput(void);
    void connectController(void);
    void resetButtons(void);
    void updateButtonState(int buttonIndex, bool isPressed);

    static const char* buttonNames[16]; // Button names array
};

#endif

/**
 * @file PlutoPAD.h
 * @brief Header file for the PlutoPAD Bluetooth controller code interface.
 *
 * This file declares the PlutoPAD class, which manages the PlutoPAD 16-button Bluetooth
 * controller for embedded systems. It includes:
 *   - Bluetooth client initialisation and reconnection
 *   - Button state tracking via a 2-byte bitfield
 *   - User-defined callback support for button changes, connections, and disconnections
 *   - Accessor for readable button names by index
 *
 * The button layout and bit order align with a logical left-to-right, bottom-to-top
 * arrangement, compatible with PlutoBUG and similar wireless robotics applications.
 *
 * @author
 * Peter Kyriakides
 *
 * @version 0.1.1
 * @date 15-06-2025
 * 
 * @note Subject to the GNU General Public License v3.0 (GPL-3.0).
 */

#ifndef PLUTOPAD_H
#define PLUTOPAD_H

#include "BluetoothSerial.h"

// Class to handle PlutoPAD controller
class PlutoPAD
{
public:
    PlutoPAD();

    void begin(const char* deviceName, bool debugMode = false);
    void end(void);
    void run(void);
    bool isConnected(void);
    void linkOnChange(void (*callback)());
    void linkOnConnect(void (*callback)());
    void linkOnDisconnect(void (*callback)());
    const char* getButtonName(int index);
    uint16_t getRawPacket() const;

    // Struct to store button states
    struct ButtonStates 
    {
        union {
            struct {
                uint16_t centre_b  : 1;
                uint16_t down_b    : 1;
                uint16_t left_b    : 1;
                uint16_t middle_b  : 1;
                uint16_t right_b   : 1;
                uint16_t up_b      : 1;
                uint16_t r1        : 1;
                uint16_t r2        : 1;
                uint16_t l2        : 1;
                uint16_t l1        : 1;
                uint16_t up_a      : 1;
                uint16_t right_a   : 1;
                uint16_t middle_a  : 1;
                uint16_t left_a    : 1;
                uint16_t down_a    : 1;
                uint16_t centre_a  : 1;
            } bits;
            uint16_t raw;
        };

        #define BUTTON_STATE_FUNCTIONS(button) \
            bool button##_pressed() const { return bits.button; } \
            bool button##_released() const { return !bits.button; }

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

private:
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

    // Update a specific button field based on its index
    //  - Safe conduct of checking bits
    inline void updateButtonState(int buttonIndex, bool isPressed)
    {
    switch (buttonIndex) 
    {
        case 0:  button.bits.centre_b = isPressed; break;
        case 1:  button.bits.down_b   = isPressed; break;
        case 2:  button.bits.left_b   = isPressed; break;
        case 3:  button.bits.middle_b = isPressed; break;
        case 4:  button.bits.right_b  = isPressed; break;
        case 5:  button.bits.up_b     = isPressed; break;
        case 6:  button.bits.r1       = isPressed; break;
        case 7:  button.bits.r2       = isPressed; break;
        case 8:  button.bits.l2       = isPressed; break;
        case 9:  button.bits.l1       = isPressed; break;
        case 10: button.bits.up_a     = isPressed; break;
        case 11: button.bits.right_a  = isPressed; break;
        case 12: button.bits.middle_a = isPressed; break;
        case 13: button.bits.left_a   = isPressed; break;
        case 14: button.bits.down_a   = isPressed; break;
        case 15: button.bits.centre_a = isPressed; break;
        default: break;
    }
    }

    static const char* buttonNames[16];
};

#endif // PLUTOPAD_H

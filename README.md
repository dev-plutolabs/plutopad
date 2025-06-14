# PlutoPAD.h Library

**ESP32/Arduino library for communicating with a PlutoPAD, allowing users to easily integrate the wireless controller into their own projects.** 

PlutoPAD library manages all PlutoPAD states using a compact packet structure and provides user-defined callbacks for button events, connections, and disconnections. Ideal for robotics and embedded projects (like PlutoBUG), it offers a simple and reliable wireless control interface.

---

## 🔧 Features

- Supports all PlutoPAD functionality: 16 digital buttons packed into a 2-byte data packet  
- Automatic reconnection if the PlutoPAD controller disconnects  
- User-defined callbacks for:
  - Button state changes
  - Connection events
  - Disconnection events  
- Human-readable button name lookup  
- Designed for projects with ESP32 or Arduino boards using Bluetooth
- Optimised for microcontroller-based robotics projects

---

## 🚀 Getting Started

### Requirements

- A PlutoPAD (obviously...)
- ESP32 or Arduino board with Bluetooth
- [BluetoothSerial](https://www.arduino.cc/reference/en/libraries/bluetoothserial/) (included with the ESP32 core)

### Installation

1. Clone or download this repository.
2. Copy `PlutoPAD.h` and `PlutoPAD.cpp` into your Arduino project.
3. Include the library in your sketch:

```cpp
#include "PlutoPAD.h"
```

---

## 📚 Example Usage

### Basic PlutoPAD Connection
```cpp
#include "PlutoPAD.h"

PlutoPAD PlutoPad;

// Function callback when any button state changes on the PlutoPAD
void onButtonChange()
{
  if (PlutoPad.button.centre_b_pressed())
  {
    Serial.println("Centre B pressed!");
  }
}

// Function callback when the PlutoPAD connects
void onConnect()
{
  Serial.println("PlutoPAD connected.");
}

// Function callback when the PlutoPAD disconnects
void onDisconnect()
{
  Serial.println("PlutoPAD disconnected.");
}

void setup()
{
  Serial.begin(115200);
  PlutoPad.begin("MyBluetoothProject");
  PlutoPad.linkOnChange(onButtonChange);
  PlutoPad.linkOnConnect(onConnect);
  PlutoPad.linkOnDisconnect(onDisconnect);
}

void loop()
{
  PlutoPad.run(); // Run PlutoPAD...
}

```
---

## 🎮 Button Layout
**PlutoPAD button states are stored in a 16-bit packet in the following order:**
```cpp
["Centre B", "Down B", "Left B", "Middle B", "Right B", "Up B", 
 "R1", "R2", "L2", "L1", 
 "Up A", "Right A", "Middle A", "Left A", "Down A", "Centre A"]
```

**You can get button names by index:**
```cpp
Serial.println(PlutoPad.getButtonName(0)); // Prints "Centre B"
```

**Or use the built-in accessors:**
```cpp
if (PlutoPad.button.centre_b_pressed())
{
  // React to Centre B press
  // do things....
}
```

---

## 🧪 Debug Mode
**Enable serial debug output by passing `true` as the second argument to `begin()`:**
```cpp
PlutoPad.begin("MyBluetoothProject", true); // Enables debugging mode
```
---

## 📄 License
**This library is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).**
You are free to use, modify, and distribute this library under the terms of the GPL.

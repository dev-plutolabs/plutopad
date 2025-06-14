# PlutoPAD.h Library

**ESP32/Arduino library for communicating with a PlutoPAD, allowing users to easily integrate the bluetooth controller into their projects.** 

PlutoPAD library manages all PlutoPAD states using a compact packet structure and provides user-defined callbacks for button events, connections, and disconnections. Ideal for robotics and embedded projects (like PlutoBUG), it offers a simple and reliable wireless control interface.

---

## 🔧 Features

- Supports 16 digital buttons packed into a 2-byte data packet  
- Automatic reconnection if the PlutoPAD disconnects  
- User-defined callbacks for:
  - Button state changes
  - Connection events
  - Disconnection events  
- Human-readable button name lookup  
- Designed for ESP32 or Arduino boards using Bluetooth
- Optimised for microcontroller-based robotics projects

---

## 🚀 Getting Started

### Requirements

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

void onButtonChange()
{
  if (PlutoPad.button.centre_b_pressed())
  {
    Serial.println("Centre B pressed!");
  }
}

void onConnect()
{
  Serial.println("PlutoPAD connected.");
}

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
  PlutoPad.run();
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
}
```

---

## 🧪 Debug Mode
**Enable serial debug output by passing `true` as the second argument to `begin()`:**
```cpp
PlutoPad.begin("MyBluetoothProject", true);
```

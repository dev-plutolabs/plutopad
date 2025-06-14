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

PlutoPAD controller;

void onButtonChange() {
  if (controller.button.centre_b_pressed()) {
    Serial.println("Centre B pressed!");
  }
}

void onConnect() {
  Serial.println("PlutoPAD connected.");
}

void onDisconnect() {
  Serial.println("PlutoPAD disconnected.");
}

void setup() {
  Serial.begin(115200);
  controller.begin("MyESP32Device", true); // Enable debug prints
  controller.linkOnChange(onButtonChange);
  controller.linkOnConnect(onConnect);
  controller.linkOnDisconnect(onDisconnect);
}

void loop() {
  controller.run();
}
```


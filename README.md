Конечно! Ниже — готовый `README.md` на английском языке для вашего GitHub-репозитория, описывающий библиотеку **Control Library v1.0**, включающую классы `Button`, `Pot`, и `Joystick`.

---

# Control Library v1.0

A lightweight Arduino C++ library for reading common input devices: buttons, potentiometers, and analog joysticks. Designed for simplicity and efficiency, this library provides easy-to-use abstractions for reliable input handling in embedded projects.

Perfect for DIY controllers, robotics, UI prototypes, and interactive art.

---

## 📦 Features

- ✅ **Debounced button input** with minimal delay
- ✅ **High-resolution potentiometer readings** (10-bit)
- ✅ **Smart joystick support** with:
  - Dead zone calibration
  - Auto-trim (centering)
  - Adjustable margins and output range
- ✅ No external dependencies — only uses `Arduino.h` and `millis()`
- ✅ Lightweight and efficient for real-time applications

---

## 🛠️ Installation

1. Download this repository as a `.zip` file.
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded file.
4. Include the header in your sketch:

```cpp
#include "Control.h"
```

> ⚠️ Rename the main header file to `Control.h` if needed, or include individual components.

---

## 🔧 Components

### 1. `Button`

Reads a push button state with basic debouncing.

#### Usage

```cpp
Button btn(2); // Button on pin 2 (with internal pull-up)

void loop() {
  if (btn.ifclick()) {
    Serial.println("Button pressed!");
  }
}
```

#### Constructor

```cpp
Button(uint8_t pin)
```

- Uses internal `INPUT_PULLUP`, so button should connect pin to GND when pressed.

#### Method

| Method | Description |
|-------|-------------|
| `ifclick()` | Returns `true` on falling edge (button press) |

---

### 2. `Pot`

Reads analog value from a potentiometer or variable resistor.

#### Usage

```cpp
Pot pot(A0);

void loop() {
  uint16_t value = pot.getval(); // 0–1023
  Serial.println(value);
  delay(100);
}
```

#### Constructor

```cpp
Pot(uint8_t pin)
```

#### Method

| Method | Description |
|-------|-------------|
| `getval()` | Returns 10-bit analog reading (0–1023) |

---

### 3. `Joystick`

Advanced analog joystick handler with calibration, dead zones, and mapped output.

#### Usage

```cpp
Joystick joy(A0, A1, 3); // X, Y, Button pins

void setup() {
  // Optional: auto-calibrate center position
  delay(1000);
  joy.autotrim(-255, 255, -255, 255); // Map output to -255..255
}

void loop() {
  JData pos = joy.getval();
  Serial.print("X: "); Serial.print(pos.xvalue);
  Serial.print(" Y: "); Serial.println(pos.yvalue);

  if (joy.ifclick()) {
    Serial.println("Joystick pressed!");
  }

  delay(50);
}
```

#### Constructor

```cpp
Joystick(uint8_t x_pin, uint8_t y_pin, uint8_t button_pin, 
         uint8_t margin_x = 0, uint8_t margin_y = 0)
```

- `margin_x/y`: Dead zone radius around center (512)

#### Methods

| Method | Description |
|-------|-------------|
| `autotrim(lb_x, ub_x, lb_y, ub_y)` | Calibrates joystick center and sets output range |
| `getval()` | Returns `JData` struct with `xvalue` and `yvalue` in calibrated range |
| `ifclick()` | Detects button press on joystick |

#### Calibration Logic

- Automatically detects joystick offset from center (512).
- Applies left/right or up/down dead zones.
- Maps input to a custom range (e.g. `-255` to `255`).

---

## 📐 How Calibration Works

When you call `autotrim(...)`, the joystick:
1. Reads current X/Y values.
2. Calculates offset from ideal center (512).
3. Sets left/right or up/down "soft zones" to ignore drift.
4. Scales future readings into your desired output range.

This helps eliminate jitter and center drift on low-quality joysticks.

---

## 📊 Example Output

```
X: 0 Y: 0
X: 45 Y: -30
X: 120 Y: -200
Joystick pressed!
```

---

## 📎 Wiring Guide

| Component     | Arduino Connection        |
|---------------|----------------------------|
| Button        | Pin → GND, Other → Pin with `INPUT_PULLUP` |
| Potentiometer | Wiper → A0, Ends → 5V/GND |
| Joystick      | X → A0, Y → A1, SW → D3 (with pull-up) |

---

## 📄 License

MIT License — Free for personal and commercial use.  
See [LICENSE](LICENSE) for details.

---

> ✅ Tip: Use `Serial.begin(9600)` in `setup()` to debug input values.

---

Let me know if you'd like to add:
- Diagrams
- Video demo link
- PlatformIO support
- More examples (e.g. motor control, servo, etc.)

Happy coding! 🎮🕹️

# ESP32 Web-Based LED Matrix Drawing using MAX7219

## Project Overview

This project creates a **web-based drawing interface** for an **8×8 MAX7219 LED Matrix** using an **ESP32**. After connecting to Wi-Fi, the ESP32 hosts a web server that allows users to draw patterns directly from their smartphone or computer. Every pixel drawn on the web interface is transmitted to the ESP32, which updates the LED matrix in real time.

The project demonstrates the integration of **ESP32 Wi-Fi**, **embedded web servers**, **HTML/CSS/JavaScript**, and **MAX7219 LED Matrix control**.

---

## Circuit Diagram

> **Add your circuit diagram or wiring image here.**

### ESP32 to MAX7219 Connections

| ESP32 Pin | MAX7219 Pin |
|-----------|-------------|
| GPIO 47 | DIN |
| GPIO 21 | CLK |
| GPIO 20 | CS |
| 5V | VCC |
| GND | GND |

---

## Components Used

- ESP32 Development Board
- MAX7219 8×8 LED Matrix Module
- Jumper Wires
- USB Cable
- Mobile Phone or Computer
- Wi-Fi Network (Cirkit Designer Virtual Wi-Fi)

---

## Working Principle

1. The ESP32 connects to the Wi-Fi network.

2. After connecting, it starts an HTTP Web Server on port **80**.

3. When a user enters the ESP32 IP address in a web browser, a drawing interface is displayed.

4. The interface contains an **8×8 interactive grid**.

5. Users can draw by clicking or dragging across the grid.

6. When the **SHOW** button is pressed:

   - JavaScript converts each row into an 8-bit binary value.
   - The data is sent to the ESP32 using an HTTP POST request.

7. The ESP32 receives the row data and sends it to the MAX7219 using the SPI protocol.

8. The LED matrix immediately displays the drawn pattern.

9. Pressing the **CLEAR** button clears the drawing area.

---

## Pin Connections

```cpp
#define DIN_PIN 47
#define CS_PIN  20
#define CLK_PIN 21
```

---

## Features

- ESP32 Web Server
- Interactive 8×8 Drawing Canvas
- Mobile-Friendly Interface
- Real-Time LED Matrix Updates
- Click and Drag Drawing
- Touch Screen Support
- HTTP Communication
- SPI Communication with MAX7219
- Simple and Responsive UI
- No Mobile Application Required

---

## Code Logic

### 1. Wi-Fi Initialization

The ESP32 connects to the configured Wi-Fi network.

```cpp
WiFi.begin(ssid, password);
```

Once connected, the ESP32 obtains an IP address and starts the web server.

---

### 2. MAX7219 Initialization

The MAX7219 is configured by setting:

- Normal Operation Mode
- Scan Limit
- Brightness
- Matrix Mode
- Display Clear

---

### 3. Web Server

The ESP32 creates a web server using:

```cpp
WebServer server(80);
```

Two routes are available:

| URL | Function |
|------|----------|
| / | Opens the drawing interface |
| /draw | Receives LED matrix data |

---

### 4. HTML Drawing Interface

The web page includes:

- HTML
- CSS
- JavaScript

The interface generates an **8×8 clickable grid**.

Each square represents one LED.

Users can draw using:

- Mouse
- Touch Screen

---

### 5. Data Encoding

JavaScript converts every row into an 8-bit value.

Example:

```
1 1 0 0 0 1 0 0
```

becomes

```
11000100
```

which is transmitted as an integer.

---

### 6. ESP32 Data Processing

The ESP32 receives values such as:

```
r0=196
r1=130
...
r7=0
```

Each row is written to the MAX7219 register.

---

### 7. Matrix Update

The function

```cpp
writeMax7219()
```

sends register and row data through SPI using:

- DIN
- CLK
- CS

The LED matrix immediately updates to display the new pattern.

---

## Algorithm

```
Start

↓

Initialize MAX7219

↓

Connect ESP32 to Wi-Fi

↓

Start Web Server

↓

User Opens Web Page

↓

User Draws Pattern

↓

Press SHOW

↓

JavaScript Converts Grid to Binary Rows

↓

HTTP POST Request

↓

ESP32 Receives Row Data

↓

Write Data to MAX7219

↓

Display Pattern

↓

Wait for Next Drawing

↓

Repeat
```

---

## Installation Steps

1. Install Arduino IDE.
2. Install the ESP32 Board Package.
3. Install the required libraries.
4. Connect the MAX7219 module according to the circuit diagram.
5. Upload the code to the ESP32.
6. Open the Serial Monitor (115200 baud).
7. Wait for the ESP32 to connect to Wi-Fi.
8. Copy the displayed IP address.
9. Open the IP address in a web browser.
10. Draw on the grid.
11. Press **SHOW** to display the drawing on the LED matrix.

---

## Required Libraries

Install the following libraries using the Arduino Library Manager:

- WiFi (ESP32)
- WebServer

---

## Future Improvements

- Support multiple MAX7219 modules
- Add text scrolling mode
- Save custom patterns
- Pattern gallery
- Animation support
- Undo/Redo functionality
- Brightness control from the web page
- Real-time drawing without pressing SHOW
- Mobile App integration
- MQTT and IoT support
- Drawing history
- User authentication

---

## Applications

- LED Pixel Art
- Smart Notice Boards
- Educational Projects
- IoT Demonstrations
- Embedded Systems Learning
- Remote LED Display Control
- Digital Signboards
- Home Automation Displays
- Interactive Display Systems

---

## Author

**Muhammad Zulqarnain Khandapa**

**Robotics & Intelligent Systems Engineer**

### Expertise

- Embedded Systems
- ESP32 Development
- Arduino
- STM32
- Internet of Things (IoT)
- Robotics & Automation

**GitHub:** https://github.com/your-username

---

## License

This project is licensed under the **MIT License**.

You are free to use, modify, and distribute this project for educational and personal purposes.

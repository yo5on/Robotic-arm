<div align="center">

<img src="https://raw.githubusercontent.com/yo5on/yo5on/main/hd-projects.svg" width="620" alt="projects"/>

<samp><b>ESP32 ROBOTIC ARM</b></samp>

<samp>robotics · esp32 · c/c++ · embedded systems</samp>

</div>

---

An ESP32-based 4-DOF robotic arm controlled wirelessly using an RC transmitter and receiver. The project demonstrates embedded control, servo motor manipulation, wireless communication, and coordinated robotic movement for pick-and-place applications.

---

<div align="center">
<samp><b>Project Overview</b></samp>
</div>

This project implements a four-degree-of-freedom robotic arm using an ESP32 microcontroller and a FlySky RC transmitter and receiver.

The system converts wireless control inputs into servo motor movements, allowing independent control of the arm's base, shoulder, elbow, and gripper.

The project provides practical experience with:

- Embedded systems
- Wireless control
- Servo motor control
- Microcontroller programming
- Robotic motion
- Hardware interfacing
- Pick-and-place automation

---

<div align="center">
<samp><b>Project Images</b></samp>
</div>

| Robotic Arm | Wiring Diagram |
|-------------|----------------|
| ![](pic3.jpeg) | ![](wiring.png) |

---

<div align="center">
<samp><b>Features</b></samp>
</div>

- Wireless RC control
- Four degrees of freedom
- Independent servo motor control
- Responsive robotic arm movement
- LED-based system status indication
- Push-button controls
- Pick-and-place functionality
- Modular hardware and software design
- Expandable architecture for future development

---

<div align="center">
<samp><b>Hardware Components</b></samp>
</div>

| Component | Quantity |
|-----------|----------|
| ESP32 Development Board | 1 |
| Servo Motors | 4 |
| FlySky Receiver | 1 |
| Push Buttons | 5 |
| LEDs | 3 |
| Robotic Arm Chassis | 1 |
| 5V Power Supply | 1 |
| Connecting Wires | As required |
| Breadboard / PCB | Optional |

---

<div align="center">
<samp><b>Wiring</b></samp>
</div>

The complete wiring configuration is provided below.

![Wiring Diagram](wiring.png)

---

<div align="center">
<samp><b>Repository Structure</b></samp>
</div>

```text
Robotic-arm/
│
├── code.ino
├── README.md
├── wiring.png
├── pic1.jpeg
├── pic2.jpeg
├── pic3.jpeg
└── 3dparts/
```

---

<div align="center">
<samp><b>Getting Started</b></samp>
</div>

<samp><b>Prerequisites</b></samp>

Before setting up the project, ensure that the following are available:

- Arduino IDE
- ESP32 board support package
- ESP32Servo library
- ESP32 development board
- FlySky transmitter and receiver
- Required servo motors
- Suitable 5V power supply

<samp><b>Clone the Repository</b></samp>

```bash
git clone https://github.com/yo5on/Robotic-arm.git
cd Robotic-arm
```

<samp><b>Open the Project</b></samp>

Open `code.ino` in the Arduino IDE.

<samp><b>Install Required Libraries</b></samp>

Install the following components through the Arduino IDE:

- ESP32 Board Package
- ESP32Servo Library

<samp><b>Configure the ESP32</b></samp>

1. Connect the ESP32 to the computer.
2. Select the appropriate ESP32 board from the Arduino IDE board menu.
3. Select the correct COM port.
4. Verify the wiring connections.
5. Open `code.ino`.

<samp><b>Upload the Code</b></samp>

Click **Upload** in the Arduino IDE and wait for the upload process to complete.

Once the code has been uploaded, power the robotic arm and transmitter/receiver system.

---

<div align="center">
<samp><b>Controls</b></samp>
</div>

The robotic arm is controlled through four RC channels:

| Channel | Function |
|---------|----------|
| CH1 | Base Rotation |
| CH2 | Shoulder Movement |
| CH3 | Elbow Movement |
| CH4 | Gripper Control |

Servo limits and control ranges can be adjusted in the Arduino source code according to the mechanical configuration of the arm.

---

<div align="center">
<samp><b>System Architecture</b></samp>
</div>

```text
RC Transmitter
      |
      v
FlySky Receiver
      |
      v
     ESP32
      |
      +---- Base Servo
      |
      +---- Shoulder Servo
      |
      +---- Elbow Servo
      |
      +---- Gripper Servo
```

---

<div align="center">
<samp><b>Working Principle</b></samp>
</div>

1. The RC transmitter generates control signals based on user input.
2. The FlySky receiver receives the wireless signals.
3. The ESP32 reads the corresponding receiver channels.
4. The input values are mapped to appropriate servo positions.
5. The four servo motors move the robotic arm according to the received commands.
6. LEDs provide system status information.
7. Push buttons provide additional functionality such as mode selection, reset, or calibration.

---

<div align="center">
<samp><b>Applications</b></samp>
</div>

This project provides a foundation for experimenting with:

- Pick-and-place systems
- Embedded robotics
- Wireless robotic control
- Servo motor coordination
- Robotic manipulation
- Industrial automation concepts
- Autonomous robotic systems

---

<div align="center">
<samp><b>Future Improvements</b></samp>
</div>

Potential future developments include:

- Inverse kinematics
- Preset position memory
- Mobile application control
- Wi-Fi and Bluetooth control
- Camera integration
- AI-based object detection
- Autonomous pick-and-place operations
- Motion planning and trajectory optimization
- Object tracking and classification

---

<div align="center">
<samp><b>Technologies</b></samp>
</div>

| Category | Technology |
|----------|------------|
| Microcontroller | ESP32 |
| Programming | C/C++ |
| Development Environment | Arduino IDE |
| Wireless Control | FlySky RC Transmitter/Receiver |
| Actuators | Servo Motors |
| Communication | RC Receiver Channels |

---

<div align="center">
<samp><b>Author</b></samp>
</div>

**Yoson**

Computer Science student interested in AI/ML, robotics, embedded systems, and automation.

GitHub: https://github.com/yo5on

---

<div align="center">
<samp><b>License</b></samp>
</div>

This project is intended for educational and personal use. You are free to explore, modify, and extend the project for your own robotics and embedded systems experiments.

If you find this project useful, consider giving the repository a star.

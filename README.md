# Robotic-arm
# ESP32 Robotic Arm with Record and Replay

## Features

* Manual RC control of 4 servos
* Learn mode to record movements
* Replay recorded movements once
* Continuous replay loop
* Slow mode for smooth playback
* Emergency stop mode
* LED status indicators

## Hardware Required

* ESP32 Development Board
* 4-Channel RC Receiver and Transmitter
* 4 Servo Motors
* 5 Push Buttons
* 3 LEDs with 220Ω resistors
* External 5V power supply for servos

## Pin Configuration

### Receiver Inputs

CH1 → GPIO34
CH2 → GPIO35
CH3 → GPIO32
CH4 → GPIO33

### Servo Outputs

Base Servo → GPIO14
Shoulder Servo → GPIO27
Elbow Servo → GPIO26
Gripper Servo → GPIO25

### Buttons

Reset → GPIO5
Stop → GPIO18
Slow → GPIO19
Learn → GPIO17
Replay → GPIO16

### LEDs

Blue → GPIO21
Green → GPIO22
Red → GPIO23

## Operating Modes

### Normal Mode

Control all servos directly using the RC transmitter.

### Learn Mode

Press the Learn button once to start recording movements.
Press it again to stop recording.

Green LED ON = Learn mode active.

### Replay Once

Single press the Replay button to play the recorded sequence one time.

Blue LED ON = Replay once active.

### Replay Loop

Double press the Replay button to continuously replay the sequence.

Blue LED double blink = Replay loop active.

### Slow Mode

Press the Slow button to enable slow playback.

Red LED double blink = Slow mode active.

### Stop Mode

Press the Stop button to freeze all operations.

Red LED ON = Stop mode active.

### Reset

Press the Reset button to:

* Exit Stop mode
* Return all servos to their home positions
* Show a confirmation blink on all LEDs.

## Default Servo Positions

Base: 90°
Shoulder: 90°
Elbow: 90°
Gripper: 45°

## Memory Capacity

Maximum recorded steps: 200.

## Power Recommendations

Use an external 5V supply capable of providing at least 3A for the servos. Connect the external supply ground to the ESP32 ground.

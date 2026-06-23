#include <Arduino.h>
#include <ESP32Servo.h>

// ===== Servos =====
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// ===== Receiver pins =====
const int ch1Pin = 34;
const int ch2Pin = 35;
const int ch3Pin = 32;
const int ch4Pin = 33;

// ===== Servo pins =====
const int basePin     = 14;
const int shoulderPin = 27;
const int elbowPin    = 26;
const int gripperPin  = 25;

// ===== Button pins =====
#define BTN_RESET   5
#define BTN_STOP    18
#define BTN_PAUSE   19
#define BTN_LEARN   17
#define BTN_REPLAY  16

// ===== Button timing =====
const unsigned long SHORT_PRESS = 800;
const unsigned long LONG_PRESS  = 2500;

// ===== Button structure =====
struct Button {
  int pin;
  bool pressed;
  unsigned long pressTime;
};

// ===== Button objects =====
Button btnReset  = {BTN_RESET,  false, 0};
Button btnStop   = {BTN_STOP,   false, 0};
Button btnPause  = {BTN_PAUSE,  false, 0};
Button btnLearn  = {BTN_LEARN,  false, 0};
Button btnReplay = {BTN_REPLAY, false, 0};

// ===== States =====
bool stopMode  = false;
bool pauseMode = false;
bool slowMode  = false;

// ===== Learn / Replay =====
#define MAX_STEPS 200

int baseMem[MAX_STEPS];
int shoulderMem[MAX_STEPS];
int elbowMem[MAX_STEPS];
int gripperMem[MAX_STEPS];

int stepCount = 0;

bool learnMode  = false;
bool replayOnce = false;
bool replayLoop = false;

// ===== Button handler =====
void handleButton(Button &b, void (*shortAct)(), void (*longAct)()) {
  if (digitalRead(b.pin) == LOW && !b.pressed) {
    b.pressed = true;
    b.pressTime = millis();
  }

  if (digitalRead(b.pin) == HIGH && b.pressed) {
    unsigned long duration = millis() - b.pressTime;
    b.pressed = false;

    if (duration < SHORT_PRESS) shortAct();
    else if (duration >= LONG_PRESS) longAct();
  }
}

// ===== Button actions =====

// STOP
void stopShort() { stopMode = true; }
void stopLong()  { stopMode = false; }

// RESET
void resetShort() {
  stopMode  = false;
  pauseMode = false;

  baseServo.write(90);
  shoulderServo.write(90);
  elbowServo.write(90);
  gripperServo.write(45);
}
void resetLong() { ESP.restart(); }

// PAUSE
void pauseShort() { pauseMode = !pauseMode; }
void pauseLong()  { slowMode = !slowMode; }

// LEARN
void learnShort() {
  learnMode = !learnMode;
  if (learnMode) stepCount = 0;   // fresh record
}

void learnLong() {
  stepCount = 0;                  // clear memory
  learnMode = false;
}

// REPLAY
void replayShort() {
  replayOnce = true;
  replayLoop = false;
}

void replayLong() {
  replayLoop = !replayLoop;       // toggle loop
  replayOnce = false;
}

void setup() {
  pinMode(ch1Pin, INPUT);
  pinMode(ch2Pin, INPUT);
  pinMode(ch3Pin, INPUT);
  pinMode(ch4Pin, INPUT);

  pinMode(BTN_RESET,  INPUT_PULLUP);
  pinMode(BTN_STOP,   INPUT_PULLUP);
  pinMode(BTN_PAUSE,  INPUT_PULLUP);
  pinMode(BTN_LEARN,  INPUT_PULLUP);
  pinMode(BTN_REPLAY, INPUT_PULLUP);

  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  gripperServo.attach(gripperPin);

  // Home position
  baseServo.write(90);
  shoulderServo.write(90);
  elbowServo.write(90);
  gripperServo.write(45);
}

void loop() {

  // ===== Handle buttons =====
  handleButton(btnStop,   stopShort,   stopLong);
  handleButton(btnReset,  resetShort,  resetLong);
  handleButton(btnPause,  pauseShort,  pauseLong);
  handleButton(btnLearn,  learnShort,  learnLong);
  handleButton(btnReplay, replayShort, replayLong);

  // ===== Stop / Pause =====
  if (stopMode || pauseMode) return;

  // ===== Replay ONCE =====
  if (replayOnce && stepCount > 0) {
    for (int i = 0; i < stepCount; i++) {
      baseServo.write(baseMem[i]);
      shoulderServo.write(shoulderMem[i]);
      elbowServo.write(elbowMem[i]);
      gripperServo.write(gripperMem[i]);
      delay(40);
    }
    replayOnce = false;
  }

  // ===== Replay LOOP =====
  if (replayLoop && stepCount > 0) {
    for (int i = 0; i < stepCount; i++) {
      baseServo.write(baseMem[i]);
      shoulderServo.write(shoulderMem[i]);
      elbowServo.write(elbowMem[i]);
      gripperServo.write(gripperMem[i]);
      delay(40);
    }
  }

  // ===== RC input =====
  int pwm1 = pulseIn(ch1Pin, HIGH, 25000);
  int pwm2 = pulseIn(ch2Pin, HIGH, 25000);
  int pwm3 = pulseIn(ch3Pin, HIGH, 25000);
  int pwm4 = pulseIn(ch4Pin, HIGH, 25000);

  if (pwm1 >= 1100 && pwm1 <= 1800)
    baseServo.write(map(pwm1, 1100, 1800, 0, 180));

  if (pwm2 >= 1200 && pwm2 <= 1800)
    shoulderServo.write(map(pwm2, 1200, 1800, 20, 160));

  if (pwm3 >= 1200 && pwm3 <= 1900)
    elbowServo.write(map(pwm3, 1200, 1900, 30, 150));

  if (pwm4 >= 1000 && pwm4 <= 2000)
    gripperServo.write(map(pwm4, 1000, 2000, 0, 90));

  // ===== Record =====
  if (learnMode && stepCount < MAX_STEPS) {
    baseMem[stepCount]     = baseServo.read();
    shoulderMem[stepCount] = shoulderServo.read();
    elbowMem[stepCount]    = elbowServo.read();
    gripperMem[stepCount]  = gripperServo.read();
    stepCount++;
    delay(40);
  }
}
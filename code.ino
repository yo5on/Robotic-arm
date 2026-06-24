#include <Arduino.h>
#include <ESP32Servo.h>

// ===== Servos =====
Servo baseServo, shoulderServo, elbowServo, gripperServo;

// ===== Receiver pins =====
const int ch1Pin = 34;
const int ch2Pin = 35;
const int ch3Pin = 32;
const int ch4Pin = 33;

// ===== Servo pins =====
const int basePin = 14;
const int shoulderPin = 27;
const int elbowPin = 26;
const int gripperPin = 25;

// ===== Button pins =====
#define BTN_RESET   5
#define BTN_STOP    18
#define BTN_SLOW    19
#define BTN_LEARN   17
#define BTN_REPLAY  16

// ===== LED pins =====
#define LED_BLUE   21
#define LED_GREEN  22
#define LED_RED    23

// ===== Timing =====
const unsigned long SHORT_PRESS = 800;
const unsigned long DOUBLE_CLICK_GAP = 400;

// ===== Speed =====
bool slowMode = false;
unsigned long moveDelay = 2;
const unsigned long SLOW_DELAY = 25;

// ===== Button struct =====
struct Button {
  int pin;
  bool pressed;
  unsigned long t;
};

Button btnReset  = {BTN_RESET, false, 0};
Button btnStop   = {BTN_STOP, false, 0};
Button btnSlow   = {BTN_SLOW, false, 0};
Button btnLearn  = {BTN_LEARN, false, 0};
Button btnReplay = {BTN_REPLAY, false, 0};

// ===== States =====
bool stopMode = false;
bool learnMode = false;
bool replayOnce = false;
bool replayLoop = false;

// ===== Replay double click =====
bool replayPending = false;
unsigned long replayClickTime = 0;

// ===== Memory =====
#define MAX_STEPS 200
int baseMem[MAX_STEPS], shoulderMem[MAX_STEPS], elbowMem[MAX_STEPS], gripperMem[MAX_STEPS];
int stepCount = 0;

// ===== Reset confirmation =====
bool resetConfirm = false;
unsigned long resetTimer = 0;
int resetStep = 0;
bool resetState = false;

// ===== Blink timers =====
unsigned long redTimer = 0, blueTimer = 0;
int redStep = 0, blueStep = 0;
bool redState = false, blueState = false;

// ===== Button handler =====
void handleButton(Button &b, void (*action)()) {
  if (digitalRead(b.pin) == LOW && !b.pressed) {
    b.pressed = true;
    b.t = millis();
  }
  if (digitalRead(b.pin) == HIGH && b.pressed) {
    b.pressed = false;
    if (millis() - b.t < SHORT_PRESS) action();
  }
}

// ===== Button actions =====
void resetShort() {
  stopMode = false;
  baseServo.write(90);
  shoulderServo.write(90);
  elbowServo.write(90);
  gripperServo.write(45);

  resetConfirm = true;
  resetStep = 0;
  resetTimer = millis();
}

void stopShort() { stopMode = !stopMode; }

void slowShort() {
  slowMode = !slowMode;
  moveDelay = slowMode ? SLOW_DELAY : 2;
}

void learnShort() {
  learnMode = !learnMode;
  if (learnMode) stepCount = 0;
}

void replayShort() {
  unsigned long now = millis();
  if (replayPending && (now - replayClickTime <= DOUBLE_CLICK_GAP)) {
    replayLoop = !replayLoop;
    replayOnce = false;
    replayPending = false;
  } else {
    replayPending = true;
    replayClickTime = now;
  }
}

// ===== Blink helpers =====
bool doubleBlink(unsigned long &t, int &s, bool &st) {
  const unsigned long p[4] = {120,120,120,600};
  if (millis() - t >= p[s]) {
    t = millis();
    s = (s + 1) % 4;
    st = (s == 0 || s == 2);
  }
  return st;
}

bool resetBlink() {
  const unsigned long p[4] = {120,120,120,600};
  if (millis() - resetTimer >= p[resetStep]) {
    resetTimer = millis();
    resetStep++;
    resetState = (resetStep == 1 || resetStep == 3);
    if (resetStep >= 4) {
      resetConfirm = false;
      resetState = false;
    }
  }
  return resetState;
}

void setup() {
  pinMode(ch1Pin, INPUT);
  pinMode(ch2Pin, INPUT);
  pinMode(ch3Pin, INPUT);
  pinMode(ch4Pin, INPUT);

  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
  pinMode(BTN_SLOW, INPUT_PULLUP);
  pinMode(BTN_LEARN, INPUT_PULLUP);
  pinMode(BTN_REPLAY, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  baseServo.attach(basePin);
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  gripperServo.attach(gripperPin);

  baseServo.write(90);
  shoulderServo.write(90);
  elbowServo.write(90);
  gripperServo.write(45);
}

void loop() {

  handleButton(btnReset, resetShort);
  handleButton(btnStop, stopShort);
  handleButton(btnSlow, slowShort);
  handleButton(btnLearn, learnShort);
  handleButton(btnReplay, replayShort);

  if (replayPending && millis() - replayClickTime > DOUBLE_CLICK_GAP) {
    if (!replayLoop) replayOnce = true;
    replayPending = false;
  }

  // ===== RESET CONFIRM =====
  if (resetConfirm) {
    bool s = resetBlink();
    digitalWrite(LED_RED, s);
    digitalWrite(LED_GREEN, s);
    digitalWrite(LED_BLUE, s);
    return;
  }

  // ===== CLEAR LEDs =====
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);

  // ===== STOP (highest priority) =====
  if (stopMode) {
    digitalWrite(LED_RED, HIGH);
    return;
  }

  // ===== STATE LEDs (can coexist) =====
  if (learnMode) {
    digitalWrite(LED_GREEN, HIGH);
  }

  if (replayLoop) {
    digitalWrite(LED_BLUE, doubleBlink(blueTimer, blueStep, blueState));
  }
  else if (replayOnce) {
    digitalWrite(LED_BLUE, HIGH);
  }

  // ===== SPEED MODIFIER (overlay) =====
  if (slowMode) {
    digitalWrite(LED_RED, doubleBlink(redTimer, redStep, redState));
  }

  // ===== Replay once =====
  if (replayOnce && stepCount > 0) {
    for (int i = 0; i < stepCount; i++) {
      baseServo.write(baseMem[i]);
      shoulderServo.write(shoulderMem[i]);
      elbowServo.write(elbowMem[i]);
      gripperServo.write(gripperMem[i]);
      delay(moveDelay);
    }
    replayOnce = false;
  }

  // ===== Replay loop (non-blocking) =====
  if (replayLoop && stepCount > 0) {
    static int i = 0;
    static unsigned long lastMove = 0;
    if (millis() - lastMove >= moveDelay) {
      lastMove = millis();
      baseServo.write(baseMem[i]);
      shoulderServo.write(shoulderMem[i]);
      elbowServo.write(elbowMem[i]);
      gripperServo.write(gripperMem[i]);
      i = (i + 1) % stepCount;
    }
  }

  // ===== Live RC control =====
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
    baseMem[stepCount] = baseServo.read();
    shoulderMem[stepCount] = shoulderServo.read();
    elbowMem[stepCount] = elbowServo.read();
    gripperMem[stepCount] = gripperServo.read();
    stepCount++;
    delay(moveDelay);
  }
}

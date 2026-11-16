#include <EEPROM.h>

// LED HEART ARDUINO NANO
// 30 MODES + EEPROM MEMORY
// Button: D2 to GND

const int buttonPin = 2;
const int topLed = A5;
const int bottomLed = A2;

const int leftSide[] = {11, 7, 8, 9, 6, 5, 12};
const int rightSide[] = {13, A4, A3, 10, 4, A1, A0};

const int clockwisePath[] = {
  A5, 13, A4, A3, 10, 4, A1, A0,
  A2, 12, 5, 6, 9, 8, 7, 11
};

const int numTotalLeds = 16;

const int allLeds[] = {
  A2, 4, 5, 6, 7, 8, 9, 10,
  11, 12, 13, A0, A1, A3, A4, A5
};

int stage = 1;
bool stageChanged = false;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 35;

bool lastReading = HIGH;
bool stableButtonState = HIGH;

// Function declarations
void checkButton();
void smartDelay(unsigned long ms);
void turnAllOff();
void turnAllOn();
void lightLevel(int level, int state);

void stage1_AllOff();
void stage2_Waterfall();
void stage3_FillUp();
void stage4_Clockwise();
void stage5_CounterClockwise();
void stage6_Heartbeat();
void stage7_BinaryPattern();
void stage8_ExpandContract();
void stage9_FastBlink();
void stage10_AllOn();
void stage11_PingPong();
void stage12_DoubleChaser();
void stage13_RandomSparkle();
void stage14_StrobeFlash();
void stage15_SideAlternating();
void stage16_WaveSweep();
void stage17_BreathingEffect();
void stage18_CenterPulse();
void stage19_SnakeLoop();
void stage20_DiscoMode();
void stage21_RealHeartGlow();
void stage22_AlternatingPairs();
void stage23_LovePulse();
void stage24_RandomGroups();
void stage25_CenterOut();
void stage26_EdgeIn();
void stage27_MirrorWave();
void stage28_MeteorTrail();
void stage29_TwinHeartbeat();
void stage30_RunningHeart();

void setup() {
  byte savedStage = EEPROM.read(0);

  if (savedStage >= 1 && savedStage <= 30) {
    stage = savedStage;
  } else {
    stage = 1;
    EEPROM.update(0, stage);
  }

  for (int i = 0; i < numTotalLeds; i++) {
    pinMode(allLeds[i], OUTPUT);
    digitalWrite(allLeds[i], LOW);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(micros());
}

void loop() {
  stageChanged = false;

  switch (stage) {
    case 1:  stage1_AllOff(); break;
    case 2:  stage2_Waterfall(); break;
    case 3:  stage3_FillUp(); break;
    case 4:  stage4_Clockwise(); break;
    case 5:  stage5_CounterClockwise(); break;
    case 6:  stage6_Heartbeat(); break;
    case 7:  stage7_BinaryPattern(); break;
    case 8:  stage8_ExpandContract(); break;
    case 9:  stage9_FastBlink(); break;
    case 10: stage10_AllOn(); break;
    case 11: stage11_PingPong(); break;
    case 12: stage12_DoubleChaser(); break;
    case 13: stage13_RandomSparkle(); break;
    case 14: stage14_StrobeFlash(); break;
    case 15: stage15_SideAlternating(); break;
    case 16: stage16_WaveSweep(); break;
    case 17: stage17_BreathingEffect(); break;
    case 18: stage18_CenterPulse(); break;
    case 19: stage19_SnakeLoop(); break;
    case 20: stage20_DiscoMode(); break;
    case 21: stage21_RealHeartGlow(); break;
    case 22: stage22_AlternatingPairs(); break;
    case 23: stage23_LovePulse(); break;
    case 24: stage24_RandomGroups(); break;
    case 25: stage25_CenterOut(); break;
    case 26: stage26_EdgeIn(); break;
    case 27: stage27_MirrorWave(); break;
    case 28: stage28_MeteorTrail(); break;
    case 29: stage29_TwinHeartbeat(); break;
    case 30: stage30_RunningHeart(); break;

    default:
      stage = 1;
      EEPROM.update(0, stage);
      turnAllOff();
      break;
  }
}

void checkButton() {
  bool reading = digitalRead(buttonPin);

  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }

  if (millis() - lastDebounceTime >= debounceDelay) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      if (stableButtonState == LOW) {
        stage++;

        if (stage > 30) {
          stage = 1;
        }

        EEPROM.update(0, (byte)stage);
        stageChanged = true;
        turnAllOff();
      }
    }
  }
}

void smartDelay(unsigned long ms) {
  unsigned long start = millis();

  while (millis() - start < ms) {
    checkButton();
    if (stageChanged) return;
    delay(2);
  }
}

void turnAllOff() {
  for (int i = 0; i < numTotalLeds; i++) {
    digitalWrite(allLeds[i], LOW);
  }
}

void turnAllOn() {
  for (int i = 0; i < numTotalLeds; i++) {
    digitalWrite(allLeds[i], HIGH);
  }
}

void lightLevel(int level, int state) {
  if (level == 0) {
    digitalWrite(topLed, state);
  } else if (level >= 1 && level <= 7) {
    digitalWrite(leftSide[level - 1], state);
    digitalWrite(rightSide[level - 1], state);
  } else if (level == 8) {
    digitalWrite(bottomLed, state);
  }
}

// MODE 1: ALL OFF
void stage1_AllOff() {
  turnAllOff();
  while (!stageChanged) {
    checkButton();
    delay(2);
  }
}

// MODE 2: WATERFALL
void stage2_Waterfall() {
  while (!stageChanged) {
    for (int i = 0; i <= 8; i++) {
      turnAllOff();
      lightLevel(i, HIGH);
      smartDelay(110);
      if (stageChanged) return;
    }
  }
}

// MODE 3: FILL UP
void stage3_FillUp() {
  while (!stageChanged) {
    turnAllOff();
    for (int i = 8; i >= 0; i--) {
      lightLevel(i, HIGH);
      smartDelay(120);
      if (stageChanged) return;
    }
    smartDelay(200);
  }
}

// MODE 4: CLOCKWISE
void stage4_Clockwise() {
  while (!stageChanged) {
    for (int i = 0; i < numTotalLeds; i++) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      smartDelay(80);
      if (stageChanged) return;
    }
  }
}

// MODE 5: COUNTER-CLOCKWISE
void stage5_CounterClockwise() {
  while (!stageChanged) {
    for (int i = numTotalLeds - 1; i >= 0; i--) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      smartDelay(80);
      if (stageChanged) return;
    }
  }
}

// MODE 6: HEARTBEAT
void stage6_Heartbeat() {
  while (!stageChanged) {
    turnAllOn();
    smartDelay(100);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(100);
    if (stageChanged) return;
    turnAllOn();
    smartDelay(120);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(450);
    if (stageChanged) return;
  }
}

// MODE 7: ALTERNATING LEVELS
void stage7_BinaryPattern() {
  while (!stageChanged) {
    turnAllOff();
    for (int i = 0; i <= 8; i += 2) lightLevel(i, HIGH);
    smartDelay(220);
    if (stageChanged) return;

    turnAllOff();
    for (int i = 1; i <= 8; i += 2) lightLevel(i, HIGH);
    smartDelay(220);
    if (stageChanged) return;
  }
}

// MODE 8: EXPAND AND CONTRACT
void stage8_ExpandContract() {
  while (!stageChanged) {
    for (int i = 0; i <= 4; i++) {
      turnAllOff();
      lightLevel(4 - i, HIGH);
      lightLevel(4 + i, HIGH);
      smartDelay(120);
      if (stageChanged) return;
    }
  }
}

// MODE 9: FAST BLINK
void stage9_FastBlink() {
  while (!stageChanged) {
    turnAllOn();
    smartDelay(150);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(150);
    if (stageChanged) return;
  }
}

// MODE 10: ALL ON
void stage10_AllOn() {
  turnAllOn();
  while (!stageChanged) {
    checkButton();
    delay(2);
  }
}

// MODE 11: PING PONG
void stage11_PingPong() {
  while (!stageChanged) {
    for (int i = 0; i < numTotalLeds; i++) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      smartDelay(50);
      if (stageChanged) return;
    }
    for (int i = numTotalLeds - 2; i > 0; i--) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      smartDelay(50);
      if (stageChanged) return;
    }
  }
}

// MODE 12: DOUBLE CHASER
void stage12_DoubleChaser() {
  while (!stageChanged) {
    for (int i = 0; i < numTotalLeds; i++) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      digitalWrite(clockwisePath[(i + 8) % numTotalLeds], HIGH);
      smartDelay(70);
      if (stageChanged) return;
    }
  }
}

// MODE 13: RANDOM SPARKLE
void stage13_RandomSparkle() {
  while (!stageChanged) {
    turnAllOff();
    digitalWrite(allLeds[random(0, numTotalLeds)], HIGH);
    smartDelay(80);
    if (stageChanged) return;
  }
}

// MODE 14: STROBE
void stage14_StrobeFlash() {
  while (!stageChanged) {
    turnAllOn();
    smartDelay(40);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(40);
    if (stageChanged) return;
  }
}

// MODE 15: LEFT AND RIGHT ALTERNATING
void stage15_SideAlternating() {
  while (!stageChanged) {
    turnAllOff();
    for (int i = 0; i < 7; i++) digitalWrite(leftSide[i], HIGH);
    smartDelay(200);
    if (stageChanged) return;

    turnAllOff();
    for (int i = 0; i < 7; i++) digitalWrite(rightSide[i], HIGH);
    smartDelay(200);
    if (stageChanged) return;
  }
}

// MODE 16: WAVE SWEEP
void stage16_WaveSweep() {
  while (!stageChanged) {
    for (int i = 0; i <= 8; i++) {
      lightLevel(i, HIGH);
      smartDelay(60);
      if (stageChanged) return;
    }
    for (int i = 0; i <= 8; i++) {
      lightLevel(i, LOW);
      smartDelay(60);
      if (stageChanged) return;
    }
  }
}

// MODE 17: BREATHING-STYLE BLINK
void stage17_BreathingEffect() {
  while (!stageChanged) {
    for (int i = 0; i < 5; i++) {
      turnAllOn();
      smartDelay(i * 3 + 2);
      if (stageChanged) return;
      turnAllOff();
      smartDelay(20 - i * 3);
      if (stageChanged) return;
    }
    for (int i = 5; i > 0; i--) {
      turnAllOn();
      smartDelay(i * 3 + 2);
      if (stageChanged) return;
      turnAllOff();
      smartDelay(20 - i * 3);
      if (stageChanged) return;
    }
  }
}

// MODE 18: CENTER PULSE
void stage18_CenterPulse() {
  while (!stageChanged) {
    turnAllOff();
    digitalWrite(topLed, HIGH);
    digitalWrite(bottomLed, HIGH);
    smartDelay(150);
    if (stageChanged) return;

    turnAllOff();
    lightLevel(3, HIGH);
    lightLevel(4, HIGH);
    lightLevel(5, HIGH);
    smartDelay(150);
    if (stageChanged) return;
  }
}

// MODE 19: SNAKE LOOP
void stage19_SnakeLoop() {
  while (!stageChanged) {
    for (int i = 0; i < numTotalLeds; i++) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      digitalWrite(clockwisePath[(i + 1) % numTotalLeds], HIGH);
      digitalWrite(clockwisePath[(i + 2) % numTotalLeds], HIGH);
      smartDelay(70);
      if (stageChanged) return;
    }
  }
}

// MODE 20: DISCO
void stage20_DiscoMode() {
  while (!stageChanged) {
    turnAllOff();
    for (int i = 0; i < 5; i++) {
      digitalWrite(allLeds[random(0, numTotalLeds)], HIGH);
    }
    smartDelay(100);
    if (stageChanged) return;
  }
}

// MODE 21: REAL HEART GLOW
void stage21_RealHeartGlow() {
  while (!stageChanged) {
    turnAllOff();
    lightLevel(4, HIGH);
    smartDelay(80);
    if (stageChanged) return;

    lightLevel(3, HIGH);
    lightLevel(5, HIGH);
    smartDelay(80);
    if (stageChanged) return;

    turnAllOn();
    smartDelay(140);
    if (stageChanged) return;

    turnAllOff();
    smartDelay(80);
    if (stageChanged) return;

    turnAllOn();
    smartDelay(180);
    if (stageChanged) return;

    turnAllOff();
    smartDelay(500);
    if (stageChanged) return;
  }
}


// MODE 22: ALTERNATING PAIRS
void stage22_AlternatingPairs() {
  while (!stageChanged) {
    turnAllOff();
    for (int i = 0; i < numTotalLeds; i += 2) digitalWrite(clockwisePath[i], HIGH);
    smartDelay(180);
    if (stageChanged) return;

    turnAllOff();
    for (int i = 1; i < numTotalLeds; i += 2) digitalWrite(clockwisePath[i], HIGH);
    smartDelay(180);
    if (stageChanged) return;
  }
}

// MODE 23: LOVE PULSE
void stage23_LovePulse() {
  while (!stageChanged) {
    turnAllOn();
    smartDelay(100);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(100);
    if (stageChanged) return;
    turnAllOn();
    smartDelay(250);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(350);
    if (stageChanged) return;
  }
}

// MODE 24: RANDOM GROUPS
void stage24_RandomGroups() {
  while (!stageChanged) {
    turnAllOff();
    for (int i = 0; i < 4; i++) {
      digitalWrite(allLeds[random(0, numTotalLeds)], HIGH);
    }
    smartDelay(160);
    if (stageChanged) return;
  }
}

// MODE 25: CENTER OUT
void stage25_CenterOut() {
  while (!stageChanged) {
    for (int level = 4; level >= 0; level--) {
      turnAllOff();
      lightLevel(level, HIGH);
      if (level > 0) lightLevel(8 - level, HIGH);
      smartDelay(100);
      if (stageChanged) return;
    }
    for (int level = 0; level <= 4; level++) {
      turnAllOff();
      lightLevel(level, HIGH);
      if (level > 0) lightLevel(8 - level, HIGH);
      smartDelay(100);
      if (stageChanged) return;
    }
  }
}

// MODE 26: EDGE IN
void stage26_EdgeIn() {
  while (!stageChanged) {
    for (int level = 0; level <= 4; level++) {
      turnAllOff();
      lightLevel(level, HIGH);
      lightLevel(8 - level, HIGH);
      smartDelay(110);
      if (stageChanged) return;
    }
    for (int level = 4; level >= 0; level--) {
      turnAllOff();
      lightLevel(level, HIGH);
      lightLevel(8 - level, HIGH);
      smartDelay(110);
      if (stageChanged) return;
    }
  }
}

// MODE 27: MIRROR WAVE
void stage27_MirrorWave() {
  while (!stageChanged) {
    for (int level = 0; level <= 8; level++) {
      turnAllOff();
      lightLevel(level, HIGH);
      smartDelay(75);
      if (stageChanged) return;
    }
    for (int level = 8; level >= 0; level--) {
      turnAllOff();
      lightLevel(level, HIGH);
      smartDelay(75);
      if (stageChanged) return;
    }
  }
}

// MODE 28: METEOR TRAIL
void stage28_MeteorTrail() {
  while (!stageChanged) {
    for (int i = 0; i < numTotalLeds; i++) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      digitalWrite(clockwisePath[(i + 1) % numTotalLeds], HIGH);
      digitalWrite(clockwisePath[(i + 2) % numTotalLeds], HIGH);
      smartDelay(65);
      if (stageChanged) return;
    }
  }
}

// MODE 29: TWIN HEARTBEAT
void stage29_TwinHeartbeat() {
  while (!stageChanged) {
    for (int pulse = 0; pulse < 2; pulse++) {
      turnAllOn();
      smartDelay(90);
      if (stageChanged) return;
      turnAllOff();
      smartDelay(90);
      if (stageChanged) return;
    }
    smartDelay(300);
    if (stageChanged) return;
  }
}

// MODE 30: RUNNING HEART
void stage30_RunningHeart() {
  while (!stageChanged) {
    for (int i = 0; i < numTotalLeds; i++) {
      turnAllOff();
      digitalWrite(clockwisePath[i], HIGH);
      digitalWrite(clockwisePath[(i + 1) % numTotalLeds], HIGH);
      smartDelay(55);
      if (stageChanged) return;
    }
    turnAllOn();
    smartDelay(160);
    if (stageChanged) return;
    turnAllOff();
    smartDelay(100);
    if (stageChanged) return;
  }
}

#include <FastLED.h>

// ============================================
// PIN CONFIGURATION
// ============================================

// ---------- LDR ----------
#define LDR_AO 34
#define LDR_DO 27

// ---------- SOUND SENSOR ----------
#define SOUND_AO 35
#define SOUND_DO 26

// ---------- PIR ----------
#define PIR_PIN 33

// ---------- WS2812B ----------
#define LED_PIN 25
#define NUM_LEDS 100

CRGB leds[NUM_LEDS];


// ============================================
// SETTINGS
// ============================================

#define NUM_READINGS 10
#define READING_INTERVAL 1000

// Your LDR:
// DARK  -> higher ADC
// BRIGHT -> lower ADC

#define DARK_THRESHOLD 900


// ============================================
// PARTY LEVEL
// ============================================

int partyLevel = 1;


// ============================================
// SETUP
// ============================================

void setup() {

  Serial.begin(115200);

  analogReadResolution(12);

  pinMode(LDR_DO, INPUT);
  pinMode(SOUND_DO, INPUT);
  pinMode(PIR_PIN, INPUT);

  // WS2812B
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(
    leds,
    NUM_LEDS
  );

  FastLED.setBrightness(100);

  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(LDR_AO));

  Serial.println();
  Serial.println("======================================");
  Serial.println("       PARTY METER ONLINE");
  Serial.println("======================================");
  Serial.println("Collecting 10-second sensor windows...");
  Serial.println();
}


// ============================================
// MAIN LOOP
// ============================================

void loop() {

  // ========================================
  // COLLECT 10 READINGS
  // ========================================

  Serial.println("WINDOW_START");

  for (int i = 0; i < NUM_READINGS; i++) {

    int ldrAnalog = analogRead(LDR_AO);
    int ldrDigital = digitalRead(LDR_DO);

    int soundAnalog = analogRead(SOUND_AO);
    int soundDigital = digitalRead(SOUND_DO);

    int pir = digitalRead(PIR_PIN);


    // ----------------------------------------
    // Send structured reading
    // ----------------------------------------

    Serial.print("DATA,");

    Serial.print(i + 1);

    Serial.print(",");
    Serial.print(ldrAnalog);

    Serial.print(",");
    Serial.print(ldrDigital);

    Serial.print(",");
    Serial.print(soundAnalog);

    Serial.print(",");
    Serial.print(soundDigital);

    Serial.print(",");
    Serial.println(pir);


    // ----------------------------------------
    // LED continues showing current level
    // ----------------------------------------

    updateLEDs();


    delay(READING_INTERVAL);
  }


  // ========================================
  // TELL PYTHON WINDOW IS COMPLETE
  // ========================================

  Serial.println("WINDOW_END");

  Serial.println("WAITING_FOR_AI");


  // ========================================
  // WAIT FOR GEMMA RESULT
  // ========================================

  unsigned long waitStart = millis();

  while (millis() - waitStart < 30000) {

    if (Serial.available()) {

      String command = Serial.readStringUntil('\n');

      command.trim();


      // --------------------------------------
      // Expect:
      // LEVEL:1
      // LEVEL:2
      // ...
      // LEVEL:5
      // --------------------------------------

      if (command.startsWith("LEVEL:")) {

        int newLevel =
          command.substring(6).toInt();

        if (newLevel >= 1 && newLevel <= 5) {

          partyLevel = newLevel;

          Serial.print("AI_LEVEL:");
          Serial.println(partyLevel);

          Serial.println("NEW_PARTY_LEVEL");

          // Immediately show new level
          for (int i = 0; i < 10; i++) {
            updateLEDs();
            delay(30);
          }

          break;
        }
      }
    }

    updateLEDs();
    delay(20);
  }
}


// ============================================
// LED CONTROLLER
// ============================================

void updateLEDs() {

  static uint8_t hue = 0;

  hue += 2;


  // ========================================
  // LEVEL 1 — CALM
  // ========================================

  if (partyLevel == 1) {

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t brightness =
        beatsin8(
          5,
          20,
          60,
          0,
          i * 2
        );

      leds[i] = CRGB(
        brightness,
        brightness * 0.45,
        brightness * 0.08
      );
    }
  }


  // ========================================
  // LEVEL 2 — CHILL
  // ========================================

  else if (partyLevel == 2) {

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t brightness =
        beatsin8(
          8,
          40,
          100,
          0,
          i * 3
        );

      leds[i] = CHSV(
        150,
        180,
        brightness
      );
    }
  }


  // ========================================
  // LEVEL 3 — LIVELY
  // ========================================

  else if (partyLevel == 3) {

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t brightness =
        beatsin8(
          12,
          60,
          170,
          0,
          i * 5
        );

      leds[i] = CHSV(
        hue + i * 3,
        220,
        brightness
      );
    }
  }


  // ========================================
  // LEVEL 4 — PARTY
  // ========================================

  else if (partyLevel == 4) {

    for (int i = 0; i < NUM_LEDS; i++) {

      uint8_t brightness =
        beatsin8(
          20,
          100,
          255,
          0,
          i * 8
        );

      leds[i] = CHSV(
        hue + i * 8,
        255,
        brightness
      );
    }
  }


  // ========================================
  // LEVEL 5 — ABSOLUTE CHAOS
  // ========================================

  else if (partyLevel == 5) {

    for (int i = 0; i < NUM_LEDS; i++) {

      leds[i] = CHSV(
        hue + random8(100),
        255,
        random8(150, 255)
      );
    }
  }


  FastLED.show();
}

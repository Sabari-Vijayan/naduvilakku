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

// ---------- WS281B ----------
#define LED_PIN 25
#define NUM_LEDS 100

CRGB leds[NUM_LEDS];


// ============================================
// LDR SETTINGS
// ============================================

// Your LDR behaves inversely:
// DARK  → higher ADC value
// BRIGHT → lower ADC value

#define DARK_THRESHOLD 900


// ============================================
// PIR SETTINGS
// ============================================

// Keep people detected for a few seconds after
// the PIR stops detecting movement.

#define PIR_TIMEOUT 5000

unsigned long lastMotionTime = 0;


// ============================================
// SETUP
// ============================================

void setup() {

  Serial.begin(115200);

  // ADC
  analogReadResolution(12);

  // Digital sensor inputs
  pinMode(LDR_DO, INPUT);
  pinMode(SOUND_DO, INPUT);
  pinMode(PIR_PIN, INPUT);

  // WS281B
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(
    leds,
    NUM_LEDS
  );

  FastLED.setBrightness(80);

  FastLED.clear();
  FastLED.show();


  Serial.println();
  Serial.println("======================================");
  Serial.println("   SENSOR + WS281B SYSTEM ONLINE");
  Serial.println("======================================");
  Serial.println("LDR + SOUND + PIR + 100 LEDS");
  Serial.println();
}


// ============================================
// MAIN LOOP
// ============================================

void loop() {

  // ========================================
  // READ SENSORS
  // ========================================

  int ldrAnalog = analogRead(LDR_AO);
  int ldrDigital = digitalRead(LDR_DO);

  int soundAnalog = analogRead(SOUND_AO);
  int soundDigital = digitalRead(SOUND_DO);

  int pir = digitalRead(PIR_PIN);


  // ========================================
  // PIR MEMORY
  // ========================================

  if (pir == HIGH) {
    lastMotionTime = millis();
  }

  bool personPresent =
    (millis() - lastMotionTime < PIR_TIMEOUT);


  // ========================================
  // DETERMINE LIGHT CONDITION
  // ========================================

  bool isDark = ldrAnalog > DARK_THRESHOLD;


  // ========================================
  // DETERMINE LIGHT MODE
  // ========================================

  if (personPresent && soundDigital == HIGH) {

    // 🎉 PEOPLE + SOUND
    danceMode(soundAnalog);

  }

  else if (isDark) {

    // 🌙 DARK
    lampMode();

  }

  else {

    // ☀️ BRIGHT
    freshnessMode();

  }


  // ========================================
  // SERIAL MONITOR
  // ========================================

  static unsigned long lastPrint = 0;

  if (millis() - lastPrint >= 1000) {

    lastPrint = millis();

    Serial.print("LDR:");
    Serial.print(ldrAnalog);

    Serial.print(" | LDR_DO:");
    Serial.print(ldrDigital);

    Serial.print(" | SOUND:");
    Serial.print(soundAnalog);

    Serial.print(" | SOUND_DO:");
    Serial.print(soundDigital);

    Serial.print(" | PIR:");
    Serial.print(pir);

    Serial.print(" | MODE:");

    if (personPresent && soundDigital == HIGH) {
      Serial.println("DANCE");
    }
    else if (isDark) {
      Serial.println("NIGHT LAMP");
    }
    else {
      Serial.println("DAY FRESH");
    }
  }
}


// ============================================
// 🌙 NIGHT LAMP MODE
// ============================================

void lampMode() {

  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate < 80)
    return;

  lastUpdate = millis();


  for (int i = 0; i < NUM_LEDS; i++) {

    // Random flame-like brightness
    int brightness = random(70, 160);

    leds[i] = CRGB(
      brightness,
      brightness * 0.40,
      brightness * 0.08
    );
  }

  FastLED.show();
}


// ============================================
// ☀️ DAY / FRESHNESS MODE
// ============================================

void freshnessMode() {

  static uint8_t hue = 25;

  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate < 40)
    return;

  lastUpdate = millis();

  hue++;


  for (int i = 0; i < NUM_LEDS; i++) {

    // Smooth breathing effect
    uint8_t brightness =
      beatsin8(
        8,
        70,
        180,
        0,
        i * 5
      );

    leds[i] = CHSV(
      hue,
      120,
      brightness
    );
  }

  FastLED.show();
}


// ============================================
// 🎉 DANCE MODE
// ============================================

void danceMode(int soundLevel) {

  static uint8_t hue = 0;

  static unsigned long lastUpdate = 0;


  // Sound controls speed

  int speed = map(
    soundLevel,
    1800,
    4095,
    40,
    5
  );

  speed = constrain(speed, 5, 40);


  if (millis() - lastUpdate < speed)
    return;

  lastUpdate = millis();


  hue += 5;


  for (int i = 0; i < NUM_LEDS; i++) {

    uint8_t brightness =
      beatsin8(
        20,
        100,
        255,
        0,
        i * 10
      );

    leds[i] = CHSV(
      hue + i * 8,
      255,
      brightness
    );
  }

  FastLED.show();
}

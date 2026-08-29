# Thumba AI 🪔

An AI-powered Kerala oil lamp (vilakku) that senses its surroundings and responds through light, flicker, and sound. Built for the **Google Physical AI Hackathon — Onam Edition 2026**, TinkerSpace, Kochi.

## Overview

Thumba AI reads presence, ambient light, and sound in the room, and asks a Gemini model to decide which of five Onam-inspired moods fits the moment. That decision is expressed physically — LED color, brightness, and flicker, plus a short tone through a speaker. Remove the hardware and there's nothing left: the AI's decision only exists as the lamp's physical behavior.

## Moods

| Mood | Trigger | Behavior |
|---|---|---|
| Varavelpu (Welcome) | someone approaches alone, dim, quiet | warm amber glow, slow flicker |
| Onam Ashamsakal (Celebration) | festive activity, moderate light/motion | bright marigold-gold, gentle sparkle |
| Thiruvathira Thaalam (Song) | rhythmic audio detected | color pulses synced to the beat |
| Nombu Nerav (Calm/Prayer) | quiet, still, night | soft warm white, very slow flicker |
| Onakalikal Vibe (Group) | multiple people / clapping | fast multi-hue pulses |

## Hardware

- ESP32
- PIR motion sensor
- LDR (ambient light)
- Sound sensor module
- Addressable pixel LED (WS2812)
- OLED display (SSD1306, I2C)
- GF1002 (PAM8403) audio amplifier + speaker
- Breadboard + jumper wires

## Architecture

```
[PIR | LDR | Sound sensor] → ESP32 → WiFi → Flask + Gemini (laptop) → mood JSON
                                                                          ↓
                                              ESP32 ← [Pixel LED | OLED | Speaker]
```

## Setup

1. Flash `thumba_ai.ino` to the ESP32 — Arduino IDE, with the ESP32 board package plus the Adafruit NeoPixel, Adafruit GFX, Adafruit SSD1306, and ArduinoJson libraries.
2. Set your WiFi credentials and laptop IP in the sketch.
3. On the laptop: `pip install flask google-genai`, set the `GEMINI_API_KEY` environment variable, then run `python decision_service.py`.
4. Power on the ESP32 — the lamp should react to motion, light, or sound within about 2 seconds.

## Project structure

```
thumba-ai/
├── thumba_ai.ino        # ESP32 firmware — sensors, LED, OLED, audio
├── decision_service.py  # Flask + Gemini mood decision service
└── README.md
```

## Built at

Google Physical AI Hackathon — Onam Edition 2026, TinkerSpace, Kalamassery, Kochi.

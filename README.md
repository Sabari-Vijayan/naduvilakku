# 🎉 Party Meter — AI-Powered Onam Celebration Monitor

An AI-powered **Party Meter** that senses the energy of a celebration through light, sound, and human movement, uses a **local Gemma model** to interpret the combined environmental data, and responds through a 100-pixel WS2812 LED strip.

Built for the **Google Physical AI Hackathon — Onam Edition 2026**, TinkerSpace, Kochi.

## Overview

**How energetic was your Onam celebration?**

Party Meter is a small physical device designed to be placed at a party, celebration, or Onam gathering. It continuously observes its surroundings using three different types of sensors:

* 💡 **LDR** — measures ambient light and changes in the environment
* 🔊 **Sound sensor** — measures sound intensity and detects bursts of activity such as clapping, cheering, and music
* 🚶 **PIR sensor** — detects human presence and movement

The ESP32 collects these signals and sends the sensor data to a laptop, where a **locally running Gemma model** interprets the combined context.

Rather than treating each sensor independently, the system looks at the overall activity:

> **How loud is it? How active is the space? How long has the activity continued? How is the environment changing?**

The resulting interpretation is reflected physically through the WS2812 LED strip.

The LEDs become a **visual Party Meter** — calm environments produce subtle lighting, while increasingly energetic activity produces increasingly dynamic and colourful patterns.

---

## 🧠 How It Works

```text
              PHYSICAL ENVIRONMENT
                       │
          ┌────────────┼────────────┐
          ↓            ↓            ↓
         LDR          SOUND         PIR
      Light level    Activity     Movement
          │            │            │
          └────────────┼────────────┘
                       ↓
                     ESP32
                       │
                  USB / WiFi
                       ↓
                 Local Gemma
                    (Ollama)
                       │
                       ↓
              Context / Energy
                 Interpretation
                       │
                       ↓
                    ESP32
                       │
                       ↓
                WS2812 × 100
                       │
                       ↓
              🎉 PARTY METER 🎉
```

The ESP32 handles real-time sensing and LED control, while Gemma provides the higher-level interpretation of what is happening around the device.

This creates a simple **Physical AI loop**:

**Sense → Interpret → Respond**

---

## 🎉 Party Energy

The system combines the three sensor inputs to estimate the current energy of the environment.

For example:

| Environment               | Interpretation       | LED Behaviour             |
| ------------------------- | -------------------- | ------------------------- |
| Quiet + still             | Calm                 | Soft, slow glow           |
| People present            | Gathering            | Gentle flowing light      |
| Moderate sound + movement | Getting lively       | Brighter dynamic patterns |
| Loud + active             | Party                | Fast colourful animation  |
| Sustained high activity   | 🔥 High-energy party | Maximum dynamic lighting  |

The goal is not simply to detect whether someone is present.

The system attempts to understand **the dynamics of the celebration over time**.

A short clap should not be equivalent to several hours of continuous celebration.

---

## 🤖 Local AI with Gemma

Party Meter uses a **local Gemma model through Ollama** rather than relying on a cloud AI API.

The ESP32 provides raw sensor observations, while the laptop processes and summarizes the data before presenting it to Gemma.

For example:

```text
Light level: 842
Sound activity: HIGH
Motion: DETECTED
High-energy events: 17
Activity duration: 2h 34m
```

Gemma can then interpret the context:

```text
"High sustained activity with frequent sound
events and significant human movement.
The environment appears to be a high-energy
celebration."
```

The AI interpretation can then influence the physical lighting behaviour.

This allows the project to demonstrate **local AI + physical sensing + physical response** without requiring an internet connection for the intelligence layer.

---

## 💡 Physical Response

The WS2812 LED strip acts as the physical output of the system.

Instead of displaying text on a screen, the AI's interpretation is translated into **light, colour, brightness, movement, and animation**.

```text
LOW ENERGY
    ↓
🟠 Soft warm glow

MEDIUM ENERGY
    ↓
🟡 Flowing colourful patterns

HIGH ENERGY
    ↓
🌈 Fast dynamic animation

EXTREME ENERGY
    ↓
🔥 Full Party Mode
```

The current prototype uses a **100-pixel addressable LED strip**.

---

# 🏆 Party Leaderboard

Party Meter isn't only about measuring a single celebration.

We are also building a small **leaderboard web application** where different parties can submit their Party Meter results and compete against each other.

Each recorded celebration can have metrics such as:

* 🎉 Overall Party Score
* 🔊 Sound Energy
* 🚶 Activity / Movement
* ⏱️ Duration
* 📈 Peak Activity
* 🌪️ Overall Activity Variation

These measurements can be combined into a final **Party Score**.

Example:

```text
🏆 ONAM PARTY LEADERBOARD

1. Hostel Onam       94 🔥
2. Batch Celebration 89
3. Friends' Onam     84
4. Department Party  79
5. Freshers Onam     73
```

This turns the project into more than a physical gadget — it becomes a **participatory Onam competition**, where different celebrations can measure their energy and see how they rank against others.

---

## 🌼 Why Onam?

Party Meter is designed around the social and celebratory nature of **Onam**.

Onam brings together people, food, music, games, dancing, and gatherings. These naturally produce changes in:

**sound + movement + environment**

Party Meter turns those physical signals into a playful measurement of **how alive the celebration is**.

Instead of trying to digitally recreate Onam, the project uses technology to **sense the celebration itself**.

> **Your celebration happens in the real world.
> Party Meter measures the energy.
> AI interprets it.
> Light brings that energy back into the physical world.**

---

## 🔧 Hardware

* ESP32
* PIR motion sensor
* LDR (ambient light)
* Sound sensor module
* WS2812 addressable LED strip — 100 pixels
* Breadboard
* Jumper wires
* External 5V power supply for the LED strip

### Current ESP32 Pin Configuration

| Component    | Signal | ESP32 GPIO |
| ------------ | ------ | ---------: |
| LDR          | AO     |    GPIO 34 |
| LDR          | DO     |    GPIO 27 |
| Sound Sensor | AO     |    GPIO 35 |
| Sound Sensor | DO     |    GPIO 26 |
| PIR          | OUT    |    GPIO 33 |
| WS2812       | DIN    |    GPIO 25 |

The ESP32 and external LED power supply share a **common ground**.

---

## 🛠️ Software Architecture

```text
ESP32
 │
 ├── LDR
 ├── Sound Sensor
 ├── PIR
 │
 └── WS2812 LED Strip
        │
        │ Sensor data
        ↓
     Laptop
        │
        ├── Python
        ├── Data processing
        └── Ollama + Gemma
               │
               ↓
        AI interpretation
               │
               ↓
            ESP32
               │
               ↓
        LED response

        ┌─────────────────┐
        │  Leaderboard    │
        │      Site       │
        └─────────────────┘
               ↑
          Party results
```

---

## 📁 Project Structure

```text
party-meter/
├── party_meter.ino       # ESP32 firmware
├── gemma.py              # Local Gemma / Ollama interface
├── analytics.py          # Party energy & scoring
├── leaderboard/          # Leaderboard web application
└── README.md
```

---

## 🚀 Current Status

### ✅ Working

* ESP32 sensor acquisition
* LDR light sensing
* Sound sensing
* PIR motion detection
* WS2812 LED control
* Real-time sensor data transmission
* Local Gemma inference through Ollama
* Sensor interpretation through AI
* Dynamic LED response

### 🔨 In Development

* More robust Party Energy scoring
* Long-term party data collection
* Party session recording
* Leaderboard backend
* Leaderboard web interface
* AI-generated party summaries
* Improved sensor fusion and calibration

---

## 🎯 The Vision

Party Meter aims to turn a traditional celebration into something that can be **sensed, interpreted, visualized, and compared**.

It combines:

**Physical Computing + Edge AI + Sensor Fusion + Data Analytics + Interactive Lighting**

into a playful Onam experience.

### **Measure the madness. Rank the celebration. 🎉**


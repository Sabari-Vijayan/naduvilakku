import serial
import requests
import time

# ==============================
# SETTINGS
# ==============================

SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 115200

OLLAMA_URL = "http://localhost:11434/api/generate"

MODEL = "gemma:2b"   # CHANGE THIS to your model


# ==============================
# CONNECT TO ESP32
# ==============================

esp32 = serial.Serial(
    SERIAL_PORT,
    BAUD_RATE,
    timeout=1
)

print("================================")
print(" ESP32 + GEMMA SENSOR TEST")
print("================================")
print("ESP32 connected!")
print("Waiting for sensor data...\n")


# ==============================
# READ SENSOR DATA
# ==============================

last_gemma_call = 0

while True:

    line = esp32.readline().decode(
        "utf-8",
        errors="ignore"
    ).strip()

    if not line:
        continue

    print("SENSOR:", line)

    # Don't bombard Gemma
    if time.time() - last_gemma_call < 5:
        continue

    last_gemma_call = time.time()


    # ==============================
    # SEND TO GEMMA
    # ==============================

    prompt = f"""
You are interpreting a small environmental sensor system.

The ESP32 currently reports:

{line}

The sensors are:
- LDR = light sensor
- SOUND = microphone/sound sensor
- PIR = motion sensor

Give a short, natural-language interpretation
of what might be happening in the environment.

Do not invent information.
Keep the response to 1-2 sentences.
"""


    try:

        response = requests.post(
            OLLAMA_URL,
            json={
                "model": MODEL,
                "prompt": prompt,
                "stream": False
            },
            timeout=60
        )

        result = response.json()

        print("\n🤖 GEMMA:")
        print(result["response"])
        print()

    except Exception as e:

        print("\n❌ Ollama error:")
        print(e)

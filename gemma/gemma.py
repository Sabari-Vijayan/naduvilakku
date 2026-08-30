import serial
import requests
import time
import re


# ============================================
# SETTINGS
# ============================================

SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 115200

OLLAMA_URL = "http://localhost:11434/api/generate"

MODEL = "gemma:2b"


# ============================================
# CONNECT ESP32
# ============================================

print("================================")
print("       🎉 PARTY METER")
print("================================")

try:

    esp32 = serial.Serial(
        SERIAL_PORT,
        BAUD_RATE,
        timeout=1
    )

    time.sleep(2)

    print("ESP32 connected!")

except Exception as e:

    print("❌ Could not connect to ESP32")
    print(e)
    exit()


print(
    "Waiting for 10-second observation windows...\n"
)


# ============================================
# PREVIOUS GEMMA LEVEL
# ============================================

previous_level = None

same_level_count = 0


# ============================================
# COLLECT WINDOW
# ============================================

def collect_window():

    readings = []

    print("\n📡 COLLECTING DATA...")


    while True:

        line = (
            esp32
            .readline()
            .decode(
                "utf-8",
                errors="ignore"
            )
            .strip()
        )


        if not line:
            continue


        # ------------------------------------
        # Start
        # ------------------------------------

        if line == "WINDOW_START":

            readings = []

            print(
                "Window started."
            )

            continue


        # ------------------------------------
        # Sensor reading
        # ------------------------------------

        if line.startswith("DATA,"):

            parts = line.split(",")


            if len(parts) == 7:

                reading = {

                    "second":
                        int(parts[1]),

                    "ldr":
                        int(parts[2]),

                    "ldr_do":
                        int(parts[3]),

                    "sound":
                        int(parts[4]),

                    "sound_do":
                        int(parts[5]),

                    "pir":
                        int(parts[6])
                }


                readings.append(
                    reading
                )


                print(
                    f"  {reading['second']}/10 "
                    f"| LDR={reading['ldr']} "
                    f"| SOUND={reading['sound']} "
                    f"| PIR={reading['pir']}"
                )


            continue


        # ------------------------------------
        # Complete
        # ------------------------------------

        if line == "WINDOW_END":

            print(
                "✅ 10-second window complete."
            )

            return readings


# ============================================
# GEMMA
# ============================================

def ask_gemma(readings):

    sensor_text = ""


    for r in readings:

        sensor_text += (

            f"Second {r['second']}: "
            f"LDR={r['ldr']}, "
            f"LDR_DO={r['ldr_do']}, "
            f"SOUND={r['sound']}, "
            f"SOUND_DO={r['sound_do']}, "
            f"PIR={r['pir']}\n"
        )


    prompt = f"""
You are the intelligence of a physical device called PARTY METER.

The device observes a celebration using:

LDR = ambient light
SOUND = sound intensity
PIR = human movement

Here is a complete 10-second observation:

{sensor_text}

Interpret the overall activity across the entire
10-second period.

Consider:

- sustained sound
- sudden sound spikes
- human movement
- changes in lighting
- overall activity

Classify the environment:

LEVEL 1 = Calm / almost no activity
LEVEL 2 = Low activity / gathering
LEVEL 3 = Moderate / lively
LEVEL 4 = High-energy party
LEVEL 5 = Extremely energetic / chaotic celebration

Do not focus on one reading.
Look at the entire 10-second pattern.

Return EXACTLY this format:

LEVEL:X
MESSAGE:short explanation

Where X is a number from 1 to 5.

Keep MESSAGE under 8 words.
"""


    print(
        "\n🤖 Asking local Gemma...\n"
    )


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


        response.raise_for_status()


        result = response.json()


        text = (
            result
            .get("response", "")
            .strip()
        )


        print("🤖 GEMMA:")
        print(text)
        print()


        # ====================================
        # LEVEL
        # ====================================

        level_match = re.search(

            r"LEVEL\s*:\s*([1-5])",

            text,

            re.IGNORECASE
        )


        if not level_match:

            print(
                "⚠️ No valid level."
            )

            return None, None


        level = int(
            level_match.group(1)
        )


        # ====================================
        # MESSAGE
        # ====================================

        message_match = re.search(

            r"MESSAGE\s*:\s*(.+)",

            text,

            re.IGNORECASE
        )


        if message_match:

            message = (
                message_match
                .group(1)
                .strip()
            )

        else:

            message = "AI detected activity"


        return level, message


    except Exception as e:

        print(
            "\n❌ Ollama error:"
        )

        print(e)

        return None, None


# ============================================
# ANTI-REPEAT LOGIC
# ============================================

def prevent_repeat(level):

    global previous_level
    global same_level_count


    if level is None:

        return None


    # First result
    if previous_level is None:

        previous_level = level
        same_level_count = 1

        return level


    # Same as previous
    if level == previous_level:

        same_level_count += 1

    else:

        same_level_count = 1


    # ----------------------------------------
    # If Gemma gives same level 2 times,
    # force the NEXT result to change.
    # ----------------------------------------

    if same_level_count >= 2:

        print(
            f"⚠️ Gemma repeated LEVEL {level}."
        )


        if level < 5:

            level = level + 1

        else:

            level = level - 4


        print(
            f"🔄 Adjusted to LEVEL {level}"
        )


        # Reset tracking
        previous_level = level
        same_level_count = 1


        return level


    previous_level = level

    return level


# ============================================
# SEND RESULT TO ESP32
# ============================================

def send_result(level, message):

    if level is None:

        return


    # ----------------------------------------
    # Send level
    # ----------------------------------------

    esp32.write(
        f"LEVEL:{level}\n".encode()
    )


    # ----------------------------------------
    # Send message
    # ----------------------------------------

    # OLED works best with short messages.
    message = (
        message
        .replace("\n", " ")
        .replace("\r", " ")
    )


    esp32.write(
        f"MSG:{message}\n".encode()
    )


    print(
        f"💡 Sent LEVEL:{level}"
    )

    print(
        f"📺 Sent MSG:{message}"
    )


# ============================================
# MAIN LOOP
# ============================================

try:

    while True:

        # ------------------------------------
        # 10-second observation
        # ------------------------------------

        readings = collect_window()


        # ------------------------------------
        # Ask Gemma
        # ------------------------------------

        level, message = ask_gemma(
            readings
        )


        # ------------------------------------
        # Prevent repetitive results
        # ------------------------------------

        final_level = prevent_repeat(
            level
        )


        # ------------------------------------
        # Send to ESP32
        # ------------------------------------

        send_result(
            final_level,
            message
        )


        print()
        print("--------------------------------")
        print(
            "Starting next observation..."
        )
        print("--------------------------------")


except KeyboardInterrupt:

    print(
        "\n\n🛑 Party Meter stopped."
    )

    esp32.close()

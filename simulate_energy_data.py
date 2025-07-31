import requests
import random
import time

API_KEY = "583FG9T68JWR8A5G"  # <-- paste your ThingSpeak Write API Key here

try:
    while True:
        # Simulate current value
        current = round(random.uniform(0.0, 15.0), 2)
        power = round(current * 120, 2)

        url = f"https://api.thingspeak.com/update?api_key={API_KEY}&field1={current}&field2={power}"

        response = requests.get(url)

        if response.status_code == 200:
            print(f"✅ Sent: Current={current} A | Power={power} W")
        else:
            print("❌ Failed to send data. Status code:", response.status_code)

        time.sleep(15)  # ThingSpeak limit: 15 sec minimum

except KeyboardInterrupt:
    print("\n🛑 User stopped sending data to ThingSpeak.")
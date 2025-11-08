
# from arduino.app_utils import *

import requests
 

def get_temperature():
    api_key = "033e4af2c403c176f2a262ad63af4565 "
    url = "https://api.openweathermap.org/data/2.5/weather"
    params = {"q": "Neratovice", "appid": api_key}

    try:
        resp = requests.get(url, params=params, timeout=10)
        resp.raise_for_status()
        data = resp.json()
    except Exception as exc:
        raise RuntimeError(f"Error fetching weather data: {exc}") from exc

    # Extract temperature in Kelvin and convert to Celsius
    main = data.get("main") or {}
    temp_k = main.get("temp")
    if temp_k is None:
        raise RuntimeError("Temperature not found in API response")

    temp_c = round(temp_k - 273.15)
    
    # Log the temperature
    print(f"Current temperature: {temp_c}°C")
  
    return temp_c

if __name__ == "__main__":
    temperature = get_temperature()
    print(f"Temperature in Neratovice: {temperature}°C")

# Bridge.provide("get_temperature", get_temperature)

# App.run()

# ESP32_Altimeter
Digital Altimeter with Lolin D32 (ESP32) and TFT-Display

Simple straight forward digital Altimeter with

- Lolin D32 Pro
- LOLIN TFT - 2.4
- HP303BP - I2C-Sensor

Shows:
- Pressure in hPa and in inHg (convenient for Pilots with old American instruments)
- Temperature in °Celsius
- Altitude in Meter and in Feed calculated from pressure. 
        
Interaction:
        Touchscreen supports to adjust the Altimeter-Correction-Factor
        Since the atmospherically pressure changes the offset for the calculated of the hight changes as well.
                This offset (Correction-Factor can be changed by pressing in the touchscreen.)
                
                Left side - 10 Meters         right side + 10 Meters

![Setup](/Lolin32_Altimeter.jpeg)

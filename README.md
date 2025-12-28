# ESP32 Randomized Soundboard

## HW used

- ESP32 DevKitC
- 5V Speaker
- MAX98357A I2S mono amp
- Button

### MAX98357A connection to ESP32

MAX98357A ESP32
              
LRC		         D22

BLCK	         D26

DIN 	         D25

GAIN	         not connected

SD		         3.3V

GND		         GND

Vcc		         5V 


## Load mp3 files

1. Choose the correct "upload_port" and "monitor_baud" in platformio.ini
2. Place .mp3 files in plataformIO's /data folder.
3. Load all data files into esp32 ("PlatformIO: new terminal" in vscode):
```
pio run --target uploadfs
```
4. Compile and upload code into esp32
```
pio run --target upload
```

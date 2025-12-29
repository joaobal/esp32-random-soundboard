#include <Arduino.h>
#include <AudioFileSourceLittleFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <LittleFS.h>

// Button pin
const int buttonPin = 15;

// Audio objects
AudioGeneratorMP3 *mp3;
AudioFileSourceLittleFS *file;
AudioOutputI2S *out;

// State tracking
bool isPlaying = false;
bool lastButtonState = HIGH;
bool firstButtonPress = true;
bool startupSoundPlayed = false;

// Random sound tracking
int lastButtonSound = -1;  // Track last played button sound
int lastRandomSound = -1;  // Track last played random sound

// Timing for random playback (in milliseconds)
const unsigned long MIN_WAIT_TIME = 60000*30;  // 30 minute
const unsigned long MAX_WAIT_TIME = 60000*120; // 2 h
unsigned long nextRandomTime = 0;

void playSound(const char* filename) {
    Serial.print("Playing: ");
    Serial.println(filename);
    
    // Stop any currently playing audio
    if (isPlaying && mp3 && mp3->isRunning()) {
        mp3->stop();
        delete mp3;
        delete file;
    }
    
    // Start playing the MP3
    file = new AudioFileSourceLittleFS(filename);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(file, out);
    isPlaying = true;
}

int getRandomSound(int min, int max, int exclude) {
    // Get a random number that's different from the last one
    int sound;
    do {
        sound = random(min, max + 1);
    } while (sound == exclude && (max - min) > 0);
    return sound;
}

void scheduleNextRandomSound() {
    unsigned long waitTime = random(MIN_WAIT_TIME, MAX_WAIT_TIME + 1);
    nextRandomTime = millis() + waitTime;
    Serial.print("Next random sound in ");
    Serial.print(waitTime / 1000);
    Serial.println(" seconds");
}

void setup() {
    Serial.begin(115200);

    // Setup button with pull-up
    pinMode(buttonPin, INPUT_PULLUP);

    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }
    Serial.println("LittleFS mounted");

    // I2S output setup
    out = new AudioOutputI2S();
    out->SetPinout(26, 22, 25);  // BCLK, LRC, DIN pins

    // Seed random number generator
    randomSeed(analogRead(0));

    // Schedule first random sound
    scheduleNextRandomSound();
}

void loop() {
    // Play startup sound once
    if (!startupSoundPlayed && !isPlaying) {
        playSound("/meme-okay-lets-go-louder.mp3");
        startupSoundPlayed = true;
    }

    // Read button state
    bool buttonPressed = digitalRead(buttonPin) == LOW;

    // Detect button press (transition from not pressed to pressed)
    if (buttonPressed && !lastButtonState && !isPlaying) {
        if (firstButtonPress) {
            // First button press plays start.mp3
            playSound("/start.mp3");
            firstButtonPress = false;
        } else {
            // Subsequent presses play random sounds from 0-6
            int soundNum = getRandomSound(0, 6, lastButtonSound);
            lastButtonSound = soundNum;
            
            char filename[20];
            sprintf(filename, "/%d.mp3", soundNum);
            playSound(filename);
        }
    }

    lastButtonState = buttonPressed;

    // Check if it's time to play a random sound
    if (millis() >= nextRandomTime && !isPlaying) {
        int soundNum = getRandomSound(0, 5, lastRandomSound);
        lastRandomSound = soundNum;
        
        char filename[20];
        sprintf(filename, "/rand%d.mp3", soundNum);
        playSound(filename);
        
        scheduleNextRandomSound();
    }

    // Keep the MP3 playing if it's running
    if (isPlaying && mp3->isRunning()) {
        if (!mp3->loop()) {
            mp3->stop();
            delete mp3;
            delete file;
            Serial.println("Playback finished.");
            isPlaying = false;
        }
    }

    delay(10);  // Small delay for debouncing
}
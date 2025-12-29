// #include <Arduino.h>
// #include <AudioFileSourceLittleFS.h>
// #include <AudioGeneratorMP3.h>
// #include <AudioOutputI2S.h>
// #include <LittleFS.h>

// // Button pin
// const int buttonPin = 15;

// // Audio objects
// AudioGeneratorMP3 *mp3;
// AudioFileSourceLittleFS *file;
// AudioOutputI2S *out;

// // State tracking
// bool isPlaying = false;
// bool lastButtonState = HIGH;  // Not pressed initially

// void setup() {
//     Serial.begin(115200);

//     // Setup button with pull-up
//     pinMode(buttonPin, INPUT_PULLUP);

//     if (!LittleFS.begin()) {
//         Serial.println("LittleFS mount failed");
//         return;
//     }
//     Serial.println("LittleFS mounted");

//     // I2S output setup
//     out = new AudioOutputI2S();
//     out->SetPinout(26, 22, 25);  // BCLK, LRC, DIN pins
// }

// void loop() {
//     // Read button state
//     bool buttonPressed = digitalRead(buttonPin) == LOW;

//     // Detect button press (transition from not pressed to pressed)
//     if (buttonPressed && !lastButtonState) {
//         Serial.println("Button pressed! Playing sound...");
        
//         // Stop any currently playing audio
//         if (isPlaying && mp3 && mp3->isRunning()) {
//             mp3->stop();
//         }
        
//         // Start playing the MP3
//         file = new AudioFileSourceLittleFS("/start.mp3");
//         mp3 = new AudioGeneratorMP3();
//         mp3->begin(file, out);
//         isPlaying = true;
//     }

//     lastButtonState = buttonPressed;

//     // Keep the MP3 playing if it's running
//     if (isPlaying && mp3->isRunning()) {
//         if (!mp3->loop()) {
//             mp3->stop();
//             Serial.println("Playback finished.");
//             isPlaying = false;
//         }
//     }

//     delay(10);  // Small delay for debouncing
// }
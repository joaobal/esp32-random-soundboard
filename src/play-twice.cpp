// #include <Arduino.h>
// #include <AudioFileSourceLittleFS.h>
// #include <AudioGeneratorMP3.h>
// #include <AudioOutputI2S.h>
// #include <LittleFS.h>

// // Objects for the first file
// AudioGeneratorMP3 *mp3_1;
// AudioFileSourceLittleFS *file_1;

// // Objects for the second file
// AudioGeneratorMP3 *mp3_2;
// AudioFileSourceLittleFS *file_2;

// // I2S output (Shared by both files)
// AudioOutputI2S *out;

// // State tracking flags
// bool file1_playing = false;
// bool file2_started = false;

// void setup() {
//     Serial.begin(115200);

//     if (!LittleFS.begin()) {
//         Serial.println("LittleFS mount failed");
//         return;
//     }
//     Serial.println("LittleFS mounted");

//     // I2S output setup
//     out = new AudioOutputI2S();
//     // BCLK, LRC, DIN pins
//     out->SetPinout(26, 22, 25);

//     // --- Start File 1 Setup ---
//     Serial.println("Preparing first file: /meme-okay-lets-go.mp3");
//     file_1 = new AudioFileSourceLittleFS("/meme-okay-lets-go.mp3");
//     mp3_1 = new AudioGeneratorMP3();
//     mp3_1->begin(file_1, out);
//     file1_playing = true;
    
//     // Note: The delay(1000) is removed from setup().
//     // The 1-second delay will now be handled *after* file 1 finishes.
// }

// void loop() {
//     // --- State 1: Playback for File 1 ---
//     if (file1_playing) {
//         if (mp3_1->isRunning()) {
//             if (!mp3_1->loop()) {
//                 // File 1 has just finished playback
//                 mp3_1->stop();
//                 Serial.println("Playback 1 finished.");
//                 file1_playing = false;
                
//                 // Now that File 1 is done, start the 1-second pause
//                 delay(1000); 
//             }
//         }
//     }

//     // --- State 2: Start and Playback for File 2 ---
//     if (!file1_playing && !file2_started) {
//         // File 1 is finished, and we haven't started File 2 yet
//         Serial.println("Starting second file: /meme-okay-lets-go-louder.mp3");
        
//         // --- Start File 2 Setup ---
//         // Need to ensure the I2S output is ready for the new file
//         if (out) {
//             delete out;
//             out = new AudioOutputI2S();
//             out->SetPinout(26, 22, 25);
//         }

//         file_2 = new AudioFileSourceLittleFS("/meme-okay-lets-go-louder.mp3");
//         mp3_2 = new AudioGeneratorMP3();
//         mp3_2->begin(file_2, out);

//         file2_started = true;
//     }

//     if (file2_started && mp3_2->isRunning()) {
//         if (!mp3_2->loop()) {
//             mp3_2->stop();
//             Serial.println("Playback 2 finished.");
//         }
//     }
// }
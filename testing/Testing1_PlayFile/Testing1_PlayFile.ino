/*
  Play an MP3 over software serial using the MY1690X MP3 IC
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 10th, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  The MY1690 has a large number of features. This example presents the user
  with a serial menu to control the various aspects of the IC.

  Feel like supporting our work? Buy a board from SparkFun!
  MY1690X Serial MP3 Player Shield: https://www.sparkfun.com/sparkfun-serial-mp3-player-shield-my1690x.html
  MY1690X Audio Player Breakout: https://www.sparkfun.com/sparkfun-audio-player-breakout-my1690x-16s.html

  Hardware Connections:
  MY1690 Pin -> Arduino Pin
  -------------------------------------
  TXO -> 8
  RXI -> 9
  VIN -> 5V
  GND -> GND

  Don't forget to load some MP3s on your sdCard and plug it in too!
  Note: Track must be named 0001.mp3 to myMP3.playTrackNumber(1)
*/

// Note: A testing version of the play all sketch - just to validate compiles

#include "SparkFun_MY1690_MP3_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_MY1690

#if defined(ESP32)
// For boards that have multiple hardware serial ports
HardwareSerial serialMP3(2); // Create serial port on ESP32: TX on 17, RX on 16

#else
// For boards that support software serial
#include "SoftwareSerial.h"
// RX on Arduino connected to TX on MY1690's, TX on Arduino connected to the MY1690's RX pin
SoftwareSerial serialMP3(8, 9);
#endif

SparkFunMY1690 myMP3;

void setup()
{
    Serial.begin(115200);
    Serial.println(F("MY1690 MP3 Example"));

    serialMP3.begin(9600); // The MY1690 expects serial communication at 9600bps

    if (myMP3.begin(serialMP3) ==
        false) // Beginning the MP3 player requires a serial port (either hardware or software)
    {
        Serial.println(F("Device not detected. Check wiring. Freezing."));
        while (1)
            ;
    }

    int songCount = myMP3.getSongCount();
    if (songCount == 0)
    {
        Serial.println(
            F("Oh no! No songs found. Make sure the SD card is inserted and there are MP3s on it. Freezing."));
        while (1)
            ;
    }

    Serial.print(F("Number of tracks on SD card: "));
    Serial.println(songCount);

    Serial.print(F("MY1690 Version: "));
    Serial.println(myMP3.getVersion());

    myMP3.play(); // Will play the lowest numbered song in the folder

    // It takes ~30ms for a track to start playing. If we check immediately, the track has not yet started.
    delay(50);

    int playStatus = myMP3.getPlayStatus();
    // 0 = stop, 1 = play, 2 = pause, 3 = fast forward, 4 = rewind

    Serial.print(F("playStatus: "));
    Serial.print(playStatus);
    if (playStatus == 1)
        Serial.println(F(" (playing)"));
    else if (playStatus == 0)
        Serial.println(F(" (stopped)"));

    myMP3.setVolume(5); // 30 is loudest. 5 is comfortable with headphones. 0 is mute.

    Serial.print(F("Volume: "));
    Serial.println(myMP3.getVolume());

    myMP3.setPlayModeNoLoop();

    mainMenu();
}

void loop()
{
    if (Serial.available())
    {
        byte incoming = Serial.read();
        if (incoming == 's')
        {
            if (myMP3.stopPlaying() == true)
                Serial.println("Stop success");
            else
                Serial.println("Stop command failed");
        }
        else if (incoming == 'x')
        {
            if (myMP3.reset() == true)
                Serial.println("Reset success");
            else
                Serial.println("Reset command failed");
        }
        else if (incoming == 'a')
        {
            myMP3.volumeUp();
            Serial.print("Volume: ");
            Serial.println(myMP3.getVolume());
        }
        else if (incoming == 'z')
        {
            myMP3.volumeDown();
            Serial.print("Volume: ");
            Serial.println(myMP3.getVolume());
        }
        else if (incoming == 'f')
        {
            myMP3.fastForward();
        }
        else if (incoming == 'r')
        {
            myMP3.rewind();
        }
        else if (incoming == 'p')
        {
            myMP3.playPause();
        }
        else if (incoming == 'e')
        {
            int currentEQ = myMP3.getEQ();
            currentEQ++; // Go to next EQ. Device automatically wraps.

            myMP3.setEQ(currentEQ);

            currentEQ = myMP3.getEQ();
            Serial.print(F("Current EQ: "));
            Serial.println(currentEQ);
        }
        else if (incoming == 'm')
        {
            int currentMode = myMP3.getPlayMode();
            currentMode++; // Go to next mode.

            if (currentMode > 4)
                currentMode = 0;

            myMP3.setPlayMode(currentMode);

            currentMode = myMP3.getPlayMode();
            Serial.print(F("Current Mode: "));
            Serial.println(currentMode);
        }
        else if (incoming == '<')
        {
            myMP3.playPrevious();
        }
        else if (incoming == '>')
        {
            myMP3.playNext();
        }
        else if (incoming == '#')
        {
            delay(20);
            while (Serial.available())
                Serial.read();

            Serial.println(F("Track number to play: "));
            while (Serial.available() == 0)
                delay(1);
            int value = Serial.parseInt();

            // Note: Track must be named 0001.mp3 to myMP3.playTrackNumber(1)
            myMP3.playTrackNumber(value);
        }
        else if (incoming == 'c')
        {
            Serial.print(F("Current track: "));
            Serial.println(myMP3.getTrackNumber());
        }
        else if (incoming == 't')
        {
            Serial.print(F("Current track elapsed time (s): "));
            Serial.println(myMP3.getTrackElapsedTime());
        }
        else if (incoming == 'T')
        {
            Serial.print(F("Current track length (s): "));
            Serial.println(myMP3.getTrackTotalTime());
        }
        else if (incoming == '\r' || incoming == '\n')
        {
            // Ignore these
        }
        else
        {
            Serial.print(F("Unknown command: "));
            Serial.write(incoming);
            Serial.println();
            mainMenu();
        }
    }
}

void mainMenu()
{
    Serial.println();
    Serial.println(F("SparkFun MY1690 Menu:"));

    Serial.println(F("s) Stop play"));
    Serial.println(F("x) Reset IC"));
    Serial.println(F("a) Volume up"));
    Serial.println(F("z) Volume down"));
    Serial.println(F("f) Fast forward"));
    Serial.println(F("r) Reverse"));
    Serial.println(F("p) Play/Pause toggle"));
    Serial.println(F("e) Set EQ"));
    Serial.println(F("m) Set play mode"));
    Serial.println(F("<) Play previous"));
    Serial.println(F(">) Play next"));
    Serial.println(F("#) Play track number"));
    Serial.println(F("c) Current track number"));
    Serial.println(F("t) Track elapsed time"));
    Serial.println(F("T) Track total time"));
    Serial.println(F("Enter command:"));
}

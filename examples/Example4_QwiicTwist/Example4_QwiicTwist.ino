/******************************************************************************
  Example4_QwiicTwist.ino

  This example shows how to use the Audio Player Breakout with the Qwiic Twist.
  Load your uSD card up with some sound files (MP3 or WAV).
  Wire up the Qwiic Twist and the Mono Audio Amp.
  Press the Qwiic Twist to play the selected track number.
  Rotate the Qwiic Twist to change the track number.
  Watch the serial monitor for feedback.

  Development environment specifics:
	IDE: Arduino 2.3.6
	Hardware Platform: SparkFun Redboard Qwiic

  By: Pete Lewis, Nathan Seidle
  SparkFun Electronics
  Date: May 5th, 2025
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  Hardware Connections:
  Use a qwiic cable to connect from the Redboard Qwiic to the Qwiic Twist.
  Connect audio-in, speakers, and power to the Mono Audio Amp.

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
******************************************************************************/

#include <Wire.h>
#include "SparkFun_MY1690_MP3_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_MY1690

//For boards that support software serial
#include "SoftwareSerial.h"
SoftwareSerial serialMP3(8, 9); //RX on Arduino connected to TX on MY1690's, TX on Arduino connected to the MY1690's RX pin

//For boards that have multiple hardware serial ports
//HardwareSerial serialMP3(2); //Create serial port on ESP32: TX on 17, RX on 16

SparkFunMY1690 myMP3;

int songCount; //Number of songs on the SD card

#include "SparkFun_Qwiic_Twist_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_Twist
TWIST twist;                                      //Create instance of this object

int user_track_number = 1;
int user_track_number_previous = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println("Example 4 - Control sound playback with a Qwiic Twist");

  Wire.begin();

  if (twist.begin() == false)
  {
    Serial.println("Twist does not appear to be connected. Please check wiring. Freezing...");
  }

  // reset twist count to 1
  twist.setCount(1);

  serialMP3.begin(9600); //The MY1690 expects serial communication at 9600bps

  if (myMP3.begin(serialMP3) == false) // Beginning the MP3 player requires a serial port (either hardware or software)
  {
    Serial.println(F("MY1690 Device not detected. Check wiring. Freezing."));
    while (1);
  }

  songCount = myMP3.getSongCount();
  if (songCount == 0)
  {
    Serial.println(F("Oh no! No songs found. Make sure the SD card is inserted and there are MP3s on it. Freezing."));
    while (1);
  }

  Serial.print(F("Number of tracks on SD card: "));
  Serial.println(songCount);

  Serial.print(F("MY1690 Version: "));
  Serial.println(myMP3.getVersion());

  myMP3.play(); //Will play the lowest numbered song in the folder

  myMP3.setVolume(30); //30 is loudest. 5 is comfortable with headphones. 0 is mute.

  Serial.print(F("Volume: "));
  Serial.println(myMP3.getVolume());

  myMP3.setPlayModeNoLoop();
}

void loop()
{
    if (twist.isPressed())
    {
      Serial.print(" Pressed! Playing track number: ");
      Serial.println(user_track_number);
      myMP3.playTrackNumber(user_track_number);
      delay(500);
    }

    user_track_number = twist.getCount();

    if (user_track_number > songCount) // limit max track number to song count
    {
      user_track_number = songCount;
      twist.setCount(user_track_number); // reset twist count to track number
    }
    else if (user_track_number < 1) // limit min track number to 1
    {
      user_track_number = 1;
      twist.setCount(user_track_number); // reset twist count to track number
    }

    // check if the user has changed the track number
    if (user_track_number != user_track_number_previous)
    {
      user_track_number_previous = user_track_number;
      Serial.print("user_track_number: ");
      Serial.println(user_track_number);
    }
    delay(10);
}

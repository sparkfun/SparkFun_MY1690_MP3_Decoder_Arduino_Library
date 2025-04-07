/*
  Play an MP3 over software serial using the MY1690X MP3 IC
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 10th, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

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

#include "SparkFun_MY1690_MP3_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_MY1690

//For boards that support software serial
#include "SoftwareSerial.h"
SoftwareSerial serialMP3(8, 9); //RX on Arduino connected to TX on MY1690's, TX on Arduino connected to the MY1690's RX pin

//For boards that have multiple hardware serial ports
//HardwareSerial serialMP3(2); //Create serial port on ESP32: TX on 17, RX on 16

SparkFunMY1690 myMP3;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("MY1690 MP3 Example 1 - Play File"));

  serialMP3.begin(9600); //The MY1690 expects serial communication at 9600bps

  if (myMP3.begin(serialMP3) == false) // Beginning the MP3 player requires a serial port (either hardware or software)
  {
    Serial.println(F("Device not detected. Check wiring. Freezing."));
    while (1);
  }

  int songCount = myMP3.getSongCount();
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

  //It takes ~30ms for a track to start playing. If we check immediately, the track has not yet started.
  delay(50);

  int playStatus = myMP3.getPlayStatus();
  // 0 = stop, 1 = play, 2 = pause, 3 = fast forward, 4 = rewind

  Serial.print(F("playStatus: "));
  Serial.print(playStatus);
  if (playStatus == 1)
    Serial.println(F(" (playing)"));
  else if (playStatus == 0)
    Serial.println(F(" (stopped)"));

  myMP3.setVolume(15); //30 is loudest. 15 is comfortable with headphones. 0 is mute.

  Serial.print(F("Volume: "));
  Serial.println(myMP3.getVolume());

  myMP3.setPlayModeNoLoop();

  Serial.print(F("Example Complete."));
}

void loop()
{

}
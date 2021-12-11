/*
  Play an MP3 over software serial using the MY1690X MP3 IC
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 10th, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/18642

  Hardware Connections:
  MY1690 Pin -> Arduino Pin
  -------------------------------------
  TXO -> 6
  RXI -> 4
  3.3V -> 3.3V
  GND -> GND

  Don't forget to load some MP3s on your sdCard and plug it in too!
*/

#include "SparkFun_MY1690_MP3_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_MY1690

//For boards that support software serial
#include "SoftwareSerial.h"
SoftwareSerial serialMP3(6, 4); //RX on Arduino connected to TX on MY1690's, TX on Arduino connected to the MY1690's RX pin

//For boards that have multiple hardware serial ports
//HardwareSerial serialMP3(2); //Create serial port on ESP32: TX on 17, RX on 16

MY1690 myMP3;

void setup()
{
  Serial.begin(115200);
  Serial.println("MY1690 MP3 Example");

  serialMP3.begin(9600); //The MY1690 expects serial communication at 9600bps
  
  if (myMP3.begin(serialMP3) == false) // Beginning the MP3 player requires a serial port (either hardware or software)
  {
    Serial.println("Device not detected. Check wiring. Freezing.");
    while (1);
  }

  int songCount = myMP3.getSongCount();
  if (songCount == 0)
  {
    Serial.println("Oh no! No songs found. Try adding songs or plugging in the sd card. Freezing.");
    while (1);
  }

  Serial.print("Number of tracks on SD card: ");
  Serial.println(songCount);

  myMP3.setVolume(25); //Max of 30

  myMP3.play(); //Will play the lowest numbered song in the folder
}

void loop()
{

}

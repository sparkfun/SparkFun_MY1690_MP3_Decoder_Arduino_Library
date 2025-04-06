/*!
 * @file SparkFun_MY1690_MP3_Library.h
 * @brief  This is a library written for the MY1690 Serial MP3 player
 *
 * SparkFun sells these at its website: www.sparkfun.com
 *
 * Do you like this library? Help support SparkFun. Buy a board!
 * https://www.sparkfun.com/products/15050
 *
 * https://github.com/sparkfun/SparkFun_MY1690_MP3_Decoder_Arduino_Library
 *
 * @author SparkFun Electronics
 * @date 2024
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef SPARKFUN_MY1690_MP3_LIBRARY_H
#define SPARKFUN_MY1690_MP3_LIBRARY_H

#include "Arduino.h"

#define MP3_NUM_CMD_BYTES 7 // Longest command is Play Select Track

// These are the commands that are sent over serial to the MY1690

// MY1690
#define MP3_COMMAND_PLAY 0x11
#define MP3_COMMAND_PAUSE 0x12
#define MP3_COMMAND_NEXT 0x13
#define MP3_COMMAND_PREVIOUS 0x14
#define MP3_COMMAND_VOLUME_UP 0x15
#define MP3_COMMAND_VOLUME_DOWN 0x16
#define MP3_COMMAND_RESET 0x19
#define MP3_COMMAND_FASTFOWARD 0x1A
#define MP3_COMMAND_REWIND 0x1B
#define MP3_COMMAND_PLAY_PAUSE 0x1C
#define MP3_COMMAND_STOP 0x1E

#define MP3_COMMAND_SET_VOLUME 0x31
#define MP3_COMMAND_SET_EQ_MODE 0x32
#define MP3_COMMAND_SET_LOOP_MODE 0x33
#define MP3_COMMAND_SET_BUSY_LEVEL 0x38

#define MP3_COMMAND_SELECT_TRACK_PLAY 0x41

#define MP3_COMMAND_GET_STATUS 0x20
#define MP3_COMMAND_GET_VOLUME 0x21
#define MP3_COMMAND_GET_EQ 0x22
#define MP3_COMMAND_GET_LOOP_MODE 0x23
#define MP3_COMMAND_GET_VERSION_NUMBER 0x24
#define MP3_COMMAND_GET_SONG_COUNT 0x25
#define MP3_COMMAND_GET_CURRENT_TRACK 0x29
#define MP3_COMMAND_GET_CURRENT_TRACK_TIME 0x2C
#define MP3_COMMAND_GET_CURRENT_TRACK_TIME_TOTAL 0x2D
#define MP3_COMMAND_GET_CURRENT_TRACK_NAME 0x2E
#define MP3_COMMAND_GET_SONGS_IN_FOLDER_COUNT 0x2F

#define MP3_LOOP_MODE_FULL 0x00
#define MP3_LOOP_MODE_FOLDER 0x01
#define MP3_LOOP_MODE_SINGLE 0x02
#define MP3_LOOP_MODE_RANDOM 0x03
#define MP3_LOOP_MODE_NO_LOOP 0x04

#define MP3_EQ_MODE_NORMAL 0x00
#define MP3_EQ_MODE_POP 0x01
#define MP3_EQ_MODE_ROCK 0x02
#define MP3_EQ_MODE_JAZZ 0x03
#define MP3_EQ_MODE_CLASSIC 0x04
#define MP3_EQ_MODE_BASS 0X05

#define MP3_START_CODE 0x7E
#define MP3_END_CODE 0xEF

/*!
 * @class SparkFunMY1690
 * @brief  A library for controlling the MY1690 Serial MP3 player module.
 *
 * This class provides an interface to control the MY1690 MP3 player module
 * via serial communication. It supports playback control, volume adjustment,
 * equalizer settings, and querying the module's status.
 *
 * @note The MY1690 module requires a serial connection and optionally a busy pin.
 */
class SparkFunMY1690
{

  protected:
    Stream *_serialPort;
    uint8_t _busyPin;

  public:
    uint8_t commandBytes[MP3_NUM_CMD_BYTES];

    SparkFunMY1690();

    /**
     * @brief Initializes the MY1690 MP3 player module.
     *
     * @param serialPort The serial port to communicate with the MY1690 module.
     *                   Defaults to `Serial`.
     * @param pin The busy pin used to monitor the module's status.
     *            Pass 255 if no busy pin is used. Defaults to 255.
     *
     * @return Returns `true` if the initialization was successful, `false` otherwise.
     *
     * @note Call this method before using any other functions in the class.
     */
    bool begin(Stream &serialPort = Serial, uint8_t pin = 255);

    // Control functions
    /**
     * @brief Starts playback of the current audio track.
     *
     * This function initiates the playback of the audio track currently loaded
     * in the MP3 decoder. Ensure that the decoder is properly initialized and
     * a track is loaded before calling this function.
     */
    void play(void);
    /**
     * @brief Pauses the playback of the MP3 decoder.
     *
     * This function pauses the current audio playback. Playback can be resumed
     * using the appropriate resume function provided by the library.
     *
     * @return true if the pause operation was successful, false otherwise.
     */
    bool pause(void);
    /**
     * @brief Plays the next track on the SD card.
     *
     * This function advances to the next track and starts playback.
     * It is typically used to skip to the next audio file in a sequence.
     *
     * @return true if the next track was successfully started, false otherwise.
     */
    bool playNext(void);
    /**
     * @brief Plays the previous track on the SD card.
     *
     * This function attempts to switch to and play the previous track.
     * The behavior may depend on the current state of the playback and
     * the implementation of the playlist or queue.
     *
     * @return true if the operation was successful, false otherwise.
     */
    bool playPrevious(void);
    /**
     * @brief Increases the volume of the MP3 decoder by one step.
     *
     * This function adjusts the volume level of the MP3 decoder to a higher setting.
     *
     * @return true if the volume was successfully increased, false if the volume is already at the maximum level.
     */
    bool volumeUp(void);
    /**
     * @brief Decreases the volume of the MP3 decoder by one step.
     *
     * This function adjusts the volume level of the MP3 decoder to a lower setting.
     *
     * @return true if the volume was successfully decreased, false if the volume is already at the minimum level.
     */
    bool volumeDown(void);
    /**
     * @brief Resets the MY1690 MP3 decoder module to its default state.
     *
     * It can be used to recover from errors or reinitialize the module.
     *
     * @return true if the reset operation was successful, false otherwise.
     */
    bool reset(void);
    /**
     * @brief Skips forward in the currently playing audio track.
     *
     * This function advances the playback position of the current audio track.
     *
     * @return true if the operation was successful, false otherwise.
     */
    bool fastForward(void);
    /**
     * @brief Skips backward in the currently playing audio track.
     *
     * This function rewinds the playback position of the current audio track.
     *
     * @return true if the operation was successful, false otherwise.
     */
    bool rewind(void);
    /**
     * @brief Toggles between play and pause states for the MP3 decoder.
     *
     * This function switches the MP3 decoder between playing and paused states.
     * If the decoder is currently playing, it will pause. If it is paused, it will
     * resume playing.
     *
     * @return true if the operation was successful, false otherwise.
     */
    bool playPause(void);
    /**
     * @brief Stops the currently playing audio track.
     *
     * This function halts playback of the audio track that is currently being played.
     * It ensures that the MP3 decoder stops processing the audio stream.
     *
     * @return true if the playback was successfully stopped, false otherwise.
     */
    bool stopPlaying(void);

    /**
     * @brief Sets the volume level of the MP3 decoder.
     *
     * @param volumeLevel The desired volume level (0-30), where 0 is mute and 30 is the maximum volume.
     * @return true if the volume was successfully set, false otherwise.
     */
    bool setVolume(uint8_t volumeLevel);
    /**
     * @brief Sets the equalizer (EQ) type for audio playback.
     *
     * @param eqType The EQ type to set. 0: None, 1: Pop, 2:Rock, 3:Jazz, 4:Classic, 5:Bass
     *
     * @return true if the EQ type was successfully set, false otherwise.
     */
    bool setEQ(uint8_t eqType);
    /**
     * @brief Sets the play mode for the MP3 decoder.
     *
     * @param playMode The desired play mode to set. 0‑4((Full/Folder/Single/Random/No Loop)
     *
     * @return true if the play mode was successfully set, false otherwise.
     */
    bool setPlayMode(uint8_t playMode);
    /**
     * @brief Plays a specific track by its track number.
     *
     * @param trackNumber The track number to play (1-based index).
     * @return true if the command to play the track was successfully sent,
     *         false otherwise.
     */
    bool playTrackNumber(uint16_t trackNumber);

    // Query commands
    /**
     * @brief Retrieves the current playback status of the MP3 decoder.
     *
     * @return uint8_t The playback status, 0(Stop) 1(Play) 2(Pause) 3(Fast forward) 4(Rewind)
     */
    uint8_t getPlayStatus(void);
    /**
     * @brief Retrieves the current volume level of the MP3 decoder.
     *
     * @return uint8_t The current volume level (0-30).
     */
    uint8_t getVolume(void);
    /**
     * @brief Retrieves the current equalizer (EQ) setting of the MP3 decoder.
     *
     * @return uint8_t The current EQ setting, a value of  0 to 5 (None\POP\ROCK\JAZZ\CLASSIC\BASS)
     */
    uint8_t getEQ(void);
    /**
     * @brief Retrieves the current play mode of the MP3 decoder.
     *
     * @return uint8_t The current play mode. 0‑4(Full/Folder/Single/Random/No Loop)
     */
    uint8_t getPlayMode(void);
    /**
     * @brief Retrieves the version of the MY1690 MP3 decoder firmware.
     *
     * @return uint16_t The version number of the firmware.
     */
    uint16_t getVersion(void);
    /**
     * @brief Retrieves the total number of songs available on the SD Card.
     *
     * @return uint16_t The total count of songs. 1‑65535
     */
    uint16_t getSongCount(void);

    /**
     * @brief Retrieves the current track number being played.
     *
     * @return uint16_t The track number currently being played. (1‑65536)
     */
    uint16_t getTrackNumber(void);
    /**
     * @brief Retrieves the elapsed time of the current track being played.
     *
     * @return uint16_t The elapsed time in seconds since the track started playing.
     */
    uint16_t getTrackElapsedTime(void);
    /**
     * @brief Retrieves the total duration of the current track.
     *
     * @return uint16_t The total time of the track in seconds.
     */
    uint16_t getTrackTotalTime(void);

    // Helper functions
    /**
     * @brief Checks if the MP3 decoder module is connected and responsive.
     *
     * @return true if the module is connected and operational, false otherwise.
     */
    bool isConnected(void);
    /**
     * @brief Checks if the MP3 decoder is currently playing audio.
     *
     * @return true if audio is playing, false otherwise.
     */
    bool isPlaying(void);
    /**
     * @brief Sets the playback mode to "Full Play Mode".
     *
     * In this mode, the device will play all songs on the SD card sequentially
     * and then loop back to the beginning once all songs have been played.
     *
     * @return true if the playback mode was successfully set, false otherwise.
     */
    bool setPlayModeFull(void); // Play all songs on the SD card, then loop
    /**
     * @brief Sets the playback mode to play all songs in the current folder and loop continuously.
     *
     * This function configures the MP3 decoder to sequentially play all audio files
     * within the current folder. Once all files have been played, playback will loop
     * back to the first file in the folder and continue indefinitely.
     *
     * @return true if the playback mode was successfully set, false otherwise.
     */
    bool setPlayModeFolder(void); // Play all songs in the folder, then loop
    /**
     * @brief Sets the MP3 decoder to single play mode.
     *
     * In single play mode, the MP3 decoder will play the current song
     * and then loop it continuously.
     *
     * @return true if the play mode was successfully set, false otherwise.
     */
    bool setPlayModeSingle(void); // Play song, then loop
    /**
     * @brief Sets the MP3 decoder to play songs in random order continuously.
     *
     * This function enables a play mode where the MP3 decoder selects a random
     * song to play, and upon completion, selects another random song. This
     * process continues indefinitely without stopping.
     *
     * @return true if the play mode was successfully set to random, false otherwise.
     */
    bool setPlayModeRandom(void); // Play random song, then play another random song, with no end
    /**
     * @brief Sets the play mode to "No Loop".
     *
     * This function configures the MP3 decoder to play a song once and then stop.
     * It disables any looping behavior, ensuring that the playback halts after
     * the current track finishes.
     *
     * @return true if the play mode was successfully set, false otherwise.
     */
    bool setPlayModeNoLoop(void); // Play a song, then stop

    void sendCommand(uint8_t commandLength);

    uint16_t getNumberResponse(void);
    bool getOKResponse(void);
    bool getSTOPResponse(void);
    bool getStringResponse(const char *expectedResponse);

    bool responseAvailable(uint8_t maxTimeout = 100);

    void clearBuffer(void);
};

#endif
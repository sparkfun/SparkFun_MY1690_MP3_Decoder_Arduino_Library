/*!
 * @file SparkFun_SparkFunMY1690_MP3_Library.cpp
 * @brief  This is a library written for the SparkFunMY1690 Serial MP3 player
 *
 * SparkFun sells these at its website: www.sparkfun.com
 *
 * Do you like this library? Help support SparkFun. Buy a board!
 * https://www.sparkfun.com/products/15050
 *
 * https://github.com/sparkfun/SparkFun_SparkFunMY1690_MP3_Decoder_Arduino_Library
 *
 * @author SparkFun Electronics
 * @date 2024
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */
#include "SparkFun_MY1690_MP3_Library.h"

SparkFunMY1690::SparkFunMY1690()
{
}

bool SparkFunMY1690::begin(Stream &serialPort, uint8_t pin)
{
    _serialPort = &serialPort;
    _busyPin = pin;
    if (_busyPin < 255)
        pinMode(_busyPin, INPUT);

    // Datasheet says MY1690 needs 1.5s after power-on before first communication
    uint8_t x = 0;
    while (isConnected() == false)
    {
        delay(100);
        if (x++ == 15)
            return (false);
    }

    stopPlaying(); // Stop any playing tracks. Stop doesn't always return 'STOP' so don't return it

    return (true);
}

// Try to get the version number from the device
uint16_t SparkFunMY1690::getVersion(void)
{
    commandBytes[0] = MP3_COMMAND_GET_VERSION_NUMBER;

    // Sometimes it responds with 'OK1.1\r\n'
    sendCommand(1);
    if (getStringResponse("OK1.1\r\n") == true)
        return (101);

    // Sometimes it responds with '1.1\r\n'
    sendCommand(1);
    if (getStringResponse("1.1\r\n") == true)
        return (101);

    sendCommand(1);
    if (getStringResponse("OK1.0\r\n") == true)
        return (100);

    sendCommand(1);
    if (getStringResponse("1.0\r\n") == true)
        return (100);

    sendCommand(1);
    int version = getNumberResponse();
    return (version);
}

// Verify the device responds correctly with a version number
bool SparkFunMY1690::isConnected(void)
{
    int version = getVersion();
    if (version == 100 || version == 101)
        return (true);

    return (false);
}

// Play all songs on the SD card, then loop
bool SparkFunMY1690::setPlayModeFull(void)
{
    commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
    commandBytes[1] = MP3_LOOP_MODE_FULL;
    sendCommand(2);
    return (getOKResponse());
}

// Play all songs in the folder, then loop
bool SparkFunMY1690::setPlayModeFolder(void)
{
    commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
    commandBytes[1] = MP3_LOOP_MODE_FOLDER;
    sendCommand(2);
    return (getOKResponse());
}

// Play song, then loop
bool SparkFunMY1690::setPlayModeSingle(void)
{
    commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
    commandBytes[1] = MP3_LOOP_MODE_SINGLE;
    sendCommand(2);
    return (getOKResponse());
}

// Play random song, then play another random song, with no end
bool SparkFunMY1690::setPlayModeRandom(void)
{
    commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
    commandBytes[1] = MP3_LOOP_MODE_RANDOM;
    sendCommand(2);
    return (getOKResponse());
}

// Play a song, then stop
bool SparkFunMY1690::setPlayModeNoLoop(void)
{
    commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
    commandBytes[1] = MP3_LOOP_MODE_NO_LOOP;
    sendCommand(2);
    return (getOKResponse());
}

uint16_t SparkFunMY1690::getSongCount(void)
{
    commandBytes[0] = MP3_COMMAND_GET_SONG_COUNT;
    sendCommand(1);

    return (getNumberResponse());
}

uint16_t SparkFunMY1690::getTrackNumber(void)
{
    commandBytes[0] = MP3_COMMAND_GET_CURRENT_TRACK;
    sendCommand(1);

    return (getNumberResponse());
}

uint16_t SparkFunMY1690::getTrackElapsedTime(void)
{
    commandBytes[0] = MP3_COMMAND_GET_CURRENT_TRACK_TIME;
    sendCommand(1);

    return (getNumberResponse());
}

uint16_t SparkFunMY1690::getTrackTotalTime(void)
{
    commandBytes[0] = MP3_COMMAND_GET_CURRENT_TRACK_TIME_TOTAL;
    sendCommand(1);

    return (getNumberResponse());
}

bool SparkFunMY1690::playTrackNumber(uint16_t trackNumber)
{
    commandBytes[0] = MP3_COMMAND_SELECT_TRACK_PLAY;
    commandBytes[1] = trackNumber >> 8;   // MSB
    commandBytes[2] = trackNumber & 0xFF; // LSB
    sendCommand(3);
    return (getOKResponse());
}

bool SparkFunMY1690::setVolume(uint8_t volumeLevel)
{
    // Any number above 30 will be automatically set to 30 by SparkFunMY1690
    // Trim value so return is true
    if (volumeLevel > 30)
        volumeLevel = 30;

    commandBytes[0] = MP3_COMMAND_SET_VOLUME;
    commandBytes[1] = volumeLevel;
    sendCommand(2);

    // In v1.1, setVolume no longer responds with an OK. We must query it
    if (getVolume() == volumeLevel)
        return (true);
    return (false);
}

uint8_t SparkFunMY1690::getVolume(void)
{
    commandBytes[0] = MP3_COMMAND_GET_VOLUME;
    sendCommand(1);
    uint8_t volLevel = getNumberResponse() & 0xFF;
    return (volLevel);
}

bool SparkFunMY1690::volumeUp(void)
{
    commandBytes[0] = MP3_COMMAND_VOLUME_UP;
    sendCommand(1);
    return (getOKResponse());
}
bool SparkFunMY1690::volumeDown(void)
{
    commandBytes[0] = MP3_COMMAND_VOLUME_DOWN;
    sendCommand(1);
    return (getOKResponse());
}

uint8_t SparkFunMY1690::getEQ(void)
{
    commandBytes[0] = MP3_COMMAND_GET_EQ;
    sendCommand(1);
    return (getNumberResponse());
}

bool SparkFunMY1690::setEQ(uint8_t eqType)
{
    commandBytes[0] = MP3_COMMAND_SET_EQ_MODE;
    commandBytes[1] = eqType;
    sendCommand(2);
    return (getOKResponse());
}

bool SparkFunMY1690::setPlayMode(uint8_t playMode)
{
    commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
    commandBytes[1] = playMode;
    sendCommand(2);
    return (getOKResponse());
}

uint8_t SparkFunMY1690::getPlayMode(void)
{
    commandBytes[0] = MP3_COMMAND_GET_LOOP_MODE;
    sendCommand(1);
    return (getNumberResponse());
}

bool SparkFunMY1690::isPlaying(void)
{
    if (_busyPin == 255)
    {
        if (getPlayStatus() == 1)
            return true;
        return (false);
    }

    if (digitalRead(_busyPin) == HIGH)
        return (true); // Song is playing
    return (false);
}

// Responds with '0000 \r\n' (note the space), '0001 \r\n', etc
uint8_t SparkFunMY1690::getPlayStatus(void)
{
    commandBytes[0] = MP3_COMMAND_GET_STATUS;
    sendCommand(1);
    return (getNumberResponse());
}

void SparkFunMY1690::play(void)
{
    commandBytes[0] = MP3_COMMAND_PLAY;
    sendCommand(1);

    // In v1.1 there are no OK responses. Busy goes high after ~30ms.
    // User can also use the isPlaying() after 30ms to see if song has started
}

bool SparkFunMY1690::pause(void)
{
    commandBytes[0] = MP3_COMMAND_PAUSE;
    sendCommand(1);
    return (getOKResponse());
}

bool SparkFunMY1690::playNext(void)
{
    commandBytes[0] = MP3_COMMAND_NEXT;
    sendCommand(1);
    return (getOKResponse());
}

bool SparkFunMY1690::playPrevious(void)
{
    commandBytes[0] = MP3_COMMAND_PREVIOUS;
    sendCommand(1);
    return (getOKResponse());
}

// Device responds with 'OK'
// If a song is playing, then ~14ms later 'STOP' is reported
bool SparkFunMY1690::stopPlaying(void)
{
    // Use hardware pins or software command
    if (isPlaying() == false)
        return (true);

    commandBytes[0] = MP3_COMMAND_STOP;
    sendCommand(1);

    // v1.1 doesn't respond with OK or STOP, instead the isPlaying can be used
    // getOKResponse();

    delay(10); // IC takes 5ms to stop a track

    if (isPlaying() == false)
        return (true);

    return (false);
}

bool SparkFunMY1690::reset(void)
{
    // Device responds with 'OK'
    // Then 'STOPMP3' ~18ms later
    // Then 'OK' ~67ms later
    commandBytes[0] = MP3_COMMAND_RESET;
    sendCommand(1);
    return (getOKResponse());
}

// Advance track ~1s
bool SparkFunMY1690::fastForward(void)
{
    commandBytes[0] = MP3_COMMAND_FASTFOWARD;
    sendCommand(1);
    return (getOKResponse());
}

// Rewind track ~1s
bool SparkFunMY1690::rewind(void)
{
    commandBytes[0] = MP3_COMMAND_REWIND;
    sendCommand(1);
    return (getOKResponse());
}

// Toggle play/pause on this track
bool SparkFunMY1690::playPause(void)
{
    commandBytes[0] = MP3_COMMAND_PLAY_PAUSE;
    sendCommand(1);
    return (getOKResponse());
}

// In version 1.1, sometimes SparkFunMY1690 responds with '0000 \r\n' to a get command. No OK, and a space.
// Sometimes 'OK0001 \r\n'. Ok, and a space. Yay!
// In version 1.0 it was lower case letters. In v1.1, it's upper case HEX.
// Convert the four letters to a decimal value
uint16_t SparkFunMY1690::getNumberResponse(void)
{
    const uint8_t maxLength = 9;
    uint8_t okResponseOffset = 0;

    if (responseAvailable() == false)
        return (0); // Timeout

    // Get four byte response
    uint16_t responseValue = 0;
    uint8_t i = 0;

    while (_serialPort->available() && i <= maxLength)
    {
        uint8_t incoming = _serialPort->read();
        if ((i == 0 && incoming == 'O') || (i == 1 && incoming == 'K'))
        {
            // Throw away chars

            okResponseOffset = 2; // Increase scanning i by 2 more
        }
        else if (i <= (3 + okResponseOffset))
        {
            // Added because getVersion response is three characters long
            if (incoming != '\r' && incoming != '\n')
            {
                // Convert ASCII HEX values to decimal
                responseValue <<= 4;
                if (incoming >= '0' && incoming <= '9')
                    responseValue += (incoming - '0');
                else if (incoming >= 'A' && incoming <= 'Z')
                    responseValue += (incoming - 'A') + 10;
                else if (incoming >= 'a' && incoming <= 'z')
                    responseValue += (incoming - 'a') + 10;
            }
        }
        else if (incoming == '\n')
            break; // End of response
        else
        {
            ; // Throw out \r and ' ' characters
        }

        i++;

        // The device can take a few ms between response chars
        uint8_t escapeCounter = 0;
        while (_serialPort->available() == 0 && i <= maxLength)
        {
            if (escapeCounter++ > 10)
                return (responseValue); // Give up

            delay(1); // At 9600bps 1 byte takes 0.8ms
        }
    }

    return (responseValue);
}

// MY1690 responds with OK (no \n \r) in ASCII to a control command
bool SparkFunMY1690::getOKResponse(void)
{
    return (getStringResponse("OK"));
}

// Returns true if MY1690 responds with a given string
bool SparkFunMY1690::getStringResponse(const char *expectedResponse)
{
    uint8_t expectedLength = strlen(expectedResponse);

    if (responseAvailable() == false)
        return (false); // Timeout

    // Get response
    uint8_t i = 0;
    bool responseOK = true;

    while (_serialPort->available() && i < expectedLength)
    {
        uint8_t incoming = _serialPort->read();
        if (incoming != expectedResponse[i])
            responseOK = false;
        i++;

        // The device can take a few ms between response chars
        uint8_t escapeCounter = 0;
        while (_serialPort->available() == 0 && i < expectedLength)
        {
            if (escapeCounter++ > 10)
                return (false); // Give up

            delay(1); // At 9600bps 1 byte takes 0.8ms
        }
    }

    if (i != expectedLength)
        responseOK = false;

    return (responseOK);
}

// Returns false if no serial data is seen after maxTimeout
bool SparkFunMY1690::responseAvailable(uint8_t maxTimeout)
{
    uint8_t counter = 0;

    while (_serialPort->available() == false)
    {
        delay(1);

        if (counter++ > maxTimeout)
            return (false); // Timeout
    }
    return (true);
}

void SparkFunMY1690::clearBuffer(void)
{
    while (_serialPort->available())
    {
        _serialPort->read();
        delay(1); // 1 byte at 9600bps should take 1ms
    }
    return;
}

void SparkFunMY1690::sendCommand(uint8_t commandLength)
{
    clearBuffer(); // Clear anything in the buffer

    _serialPort->write(MP3_START_CODE);
    _serialPort->write(commandLength + 2); // Add one byte for 'length', one for CRC

    // Begin sending command bytes while calc'ing CRC
    byte crc = commandLength + 2;
    for (byte x = 0; x < commandLength; x++) // Length + command code + parameter
    {
        _serialPort->write(commandBytes[x]); // Send this byte
        crc ^= commandBytes[x];              // XOR this byte to the CRC
    }

    _serialPort->write(crc); // Send CRC
    _serialPort->write(MP3_END_CODE);
}
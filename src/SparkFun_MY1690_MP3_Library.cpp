/*
  This is a library written for the MY1690 Serial MP3 player
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/15050

  https://github.com/sparkfun/SparkFun_MY1690_MP3_Decoder_Arduino_Library

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SparkFun_MY1690_MP3_Library.h"

MY1690::MY1690()
{
}

bool MY1690::begin(Stream &serialPort, uint8_t pin)
{
	_serialPort = &serialPort;
	_busyPin = pin;
	if (_busyPin < 255)
		pinMode(_busyPin, INPUT);

	//Datasheet says MY1690 needs 1.5s after power-on before first communication
	uint8_t x = 0;
	while (isConnected() == false)
	{
		delay(100);
		if (x++ == 15)
			return (false);
	}

	return (stopPlaying()); // Stop any playing tracks
}

uint16_t MY1690::getVersion(void)
{
	commandBytes[0] = MP3_COMMAND_GET_VERSION_NUMBER;
	sendCommand(1);

	return (getNumberResponse());
}

// Verify the device responds correctly with a version number
bool MY1690::isConnected(void)
{
	uint16_t version = getVersion();
	if (version > 0 && version < 10)
		return (true);
	return (false);
}

// Play all songs on the SD card, then loop
bool MY1690::setPlayModeFull(void)
{
	commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
	commandBytes[1] = MP3_LOOP_MODE_FULL;
	sendCommand(2);
	return (getOKResponse());
}

// Play all songs in the folder, then loop
bool MY1690::setPlayModeFolder(void)
{
	commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
	commandBytes[1] = MP3_LOOP_MODE_FOLDER;
	sendCommand(2);
	return (getOKResponse());
}

// Play song, then loop
bool MY1690::setPlayModeSingle(void)
{
	commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
	commandBytes[1] = MP3_LOOP_MODE_SINGLE;
	sendCommand(2);
	return (getOKResponse());
}

// Play random song, then play another random song, with no end
bool MY1690::setPlayModeRandom(void)
{
	commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
	commandBytes[1] = MP3_LOOP_MODE_RANDOM;
	sendCommand(2);
	return (getOKResponse());
}

// Play a song, then stop
bool MY1690::setPlayModeNoLoop(void)
{
	commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
	commandBytes[1] = MP3_LOOP_MODE_NO_LOOP;
	sendCommand(2);
	return (getOKResponse());
}

uint16_t MY1690::getSongCount(void)
{
	commandBytes[0] = MP3_COMMAND_GET_SONG_COUNT;
	sendCommand(1);

	return (getNumberResponse());
}

uint16_t MY1690::getTrackNumber(void)
{
	commandBytes[0] = MP3_COMMAND_GET_CURRENT_TRACK;
	sendCommand(1);

	return (getNumberResponse());
}

uint16_t MY1690::getTrackElapsedTime(void)
{
	commandBytes[0] = MP3_COMMAND_GET_CURRENT_TRACK_TIME;
	sendCommand(1);

	return (getNumberResponse());
}

uint16_t MY1690::getTrackTotalTime(void)
{
	commandBytes[0] = MP3_COMMAND_GET_CURRENT_TRACK_TIME_TOTAL;
	sendCommand(1);

	return (getNumberResponse());
}

bool MY1690::playTrackNumber(uint16_t trackNumber)
{
	commandBytes[0] = MP3_COMMAND_SELECT_TRACK_PLAY;
	commandBytes[1] = trackNumber >> 8;	  // MSB
	commandBytes[2] = trackNumber & 0xFF; // LSB
	sendCommand(3);
	return (getOKResponse());
}

bool MY1690::setVolume(uint8_t volumeLevel)
{
	// Any number above 30 will be automatically set to 30 by MY1690
	commandBytes[0] = MP3_COMMAND_SET_VOLUME;
	commandBytes[1] = volumeLevel;
	sendCommand(2);
	return (getOKResponse());
}

uint8_t MY1690::getVolume(void)
{
	commandBytes[0] = MP3_COMMAND_GET_VOLUME;
	sendCommand(1);
	uint8_t volLevel = getNumberResponse() & 0xFF;
	return (volLevel);
}

bool MY1690::volumeUp(void)
{
	commandBytes[0] = MP3_COMMAND_VOLUME_UP;
	sendCommand(1);
	return (getOKResponse());
}
bool MY1690::volumeDown(void)
{
	commandBytes[0] = MP3_COMMAND_VOLUME_DOWN;
	sendCommand(1);
	return (getOKResponse());
}

uint8_t MY1690::getEQ(void)
{
	commandBytes[0] = MP3_COMMAND_GET_EQ;
	sendCommand(1);
	return (getNumberResponse());
}

bool MY1690::setEQ(uint8_t eqType)
{
	commandBytes[0] = MP3_COMMAND_SET_EQ_MODE;
	commandBytes[1] = eqType;
	sendCommand(2);
	return (getOKResponse());
}

bool MY1690::setPlayMode(uint8_t playMode)
{
	commandBytes[0] = MP3_COMMAND_SET_LOOP_MODE;
	commandBytes[1] = playMode;
	sendCommand(2);
	return (getOKResponse());
}

uint8_t MY1690::getPlayMode(void)
{
	commandBytes[0] = MP3_COMMAND_GET_LOOP_MODE;
	sendCommand(1);
	return (getNumberResponse());
}

bool MY1690::isPlaying(void)
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

uint8_t MY1690::getPlayStatus(void)
{
	commandBytes[0] = MP3_COMMAND_GET_STATUS;
	sendCommand(1);
	return (getNumberResponse());
}

bool MY1690::play(void)
{
	commandBytes[0] = MP3_COMMAND_PLAY;
	sendCommand(1);
	return (getOKResponse());
}

bool MY1690::pause(void)
{
	commandBytes[0] = MP3_COMMAND_PAUSE;
	sendCommand(1);
	return (getOKResponse());
}

bool MY1690::playNext(void)
{
	commandBytes[0] = MP3_COMMAND_NEXT;
	sendCommand(1);
	return (getOKResponse());
}

bool MY1690::playPrevious(void)
{
	commandBytes[0] = MP3_COMMAND_PREVIOUS;
	sendCommand(1);
	return (getOKResponse());
}

// Device responds with 'OK'
// If a song is playing, then ~14ms later 'STOP' is reported
bool MY1690::stopPlaying(void)
{
	bool currentlyPlaying = isPlaying();

	commandBytes[0] = MP3_COMMAND_STOP;
	sendCommand(1);

	if (currentlyPlaying == false)
		return (getOKResponse());

	getOKResponse();

	// Wait 15ms for 'STOP' to come through
	return (getSTOPResponse());
}

bool MY1690::reset(void)
{
	// Device responds with 'OK'
	// Then 'STOPMP3' ~18ms later
	// Then 'OK' ~67ms later
	commandBytes[0] = MP3_COMMAND_RESET;
	sendCommand(1);
	return (getOKResponse());
}

// Advance track ~1s
bool MY1690::fastForward(void)
{
	commandBytes[0] = MP3_COMMAND_FASTFOWARD;
	sendCommand(1);
	return (getOKResponse());
}

// Rewind track ~1s
bool MY1690::rewind(void)
{
	commandBytes[0] = MP3_COMMAND_REWIND;
	sendCommand(1);
	return (getOKResponse());
}

// Toggle play/pause on this track
bool MY1690::playPause(void)
{
	commandBytes[0] = MP3_COMMAND_PLAY_PAUSE;
	sendCommand(1);
	return (getOKResponse());
}

// MY1690 responds with OK0004\n\r in ASCII to a get command
// The ASCII response is HEX so OK000d is 14.
// Convert the four letters to a decimal value
uint16_t MY1690::getNumberResponse(void)
{
#define EXPECTED_LENGTH 8
	if (responseAvailable() == false)
		return (0); // Timeout

	// Get four byte response
	uint16_t responseValue = 0;
	uint8_t i = 0;

	while (_serialPort->available() && i < EXPECTED_LENGTH)
	{
		uint8_t incoming = _serialPort->read();
		if (i == 0 || i == 1)
			; // This is throw away value 'O' and 'K'
		else if (i >= 2 && i <= 5)
		{
			// Convert ASCII HEX values to decimal
			responseValue *= 16;
			if (incoming >= '0' && incoming <= '9')
				responseValue += (incoming - '0');
			else if (incoming >= 'a' && incoming <= 'z')
				responseValue += (incoming - 'a') + 10;
		}
		else
		{
			; // These are the \n \r characters
		}

		i++;

		// The device can take a few ms between response chars
		uint8_t escapeCounter = 0;
		while (_serialPort->available() == 0 && i < EXPECTED_LENGTH)
		{
			if (escapeCounter++ > 10)
				return (responseValue); // Give up

			delay(1); // At 9600bps 1 byte takes 0.8ms
		}
	}

	return (responseValue);
}

// MY1690 responds with OK (no \n \r) in ASCII to a control command
bool MY1690::getOKResponse(void)
{
	return (getStringResponse("OK", 2));
}

// MY1690 responds with STOP (no \n \r) in ASCII to stop command
// if a song is currently playing
bool MY1690::getSTOPResponse(void)
{
	return (getStringResponse("STOP", 4));
}

// Returns true if MY1690 reponds with a given string
bool MY1690::getStringResponse(const char *expectedResponse, uint8_t expectedLength)
{
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
bool MY1690::responseAvailable(uint8_t maxTimeout)
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

void MY1690::clearBuffer(void)
{
	while (_serialPort->available())
	{
		_serialPort->read();
		delay(1); // 1 byte at 9600bps should take 1ms
	}
	return;
}

void MY1690::sendCommand(uint8_t commandLength)
{
	clearBuffer(); // Clear anything in the buffer

	_serialPort->write(MP3_START_CODE);
	_serialPort->write(commandLength + 2); // Add one byte for 'length', one for CRC

	// Begin sending command bytes while calc'ing CRC
	byte crc = commandLength + 2;
	for (byte x = 0; x < commandLength; x++) // Length + command code + parameter
	{
		_serialPort->write(commandBytes[x]); // Send this byte
		crc ^= commandBytes[x];				 // XOR this byte to the CRC
	}

	_serialPort->write(crc); // Send CRC
	_serialPort->write(MP3_END_CODE);
}
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

#ifndef SPARKFUN_MY1690_MP3_LIBRARY_H
#define SPARKFUN_MY1690_MP3_LIBRARY_H

#include "Arduino.h"

#define MP3_NUM_CMD_BYTES 7 //Longest command is Play Select Track

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

class MY1690
{

protected:
	Stream *_serialPort;
	uint8_t _busyPin;

public:
	uint8_t commandBytes[MP3_NUM_CMD_BYTES];

	MY1690();

	bool begin(Stream &serialPort = Serial, uint8_t pin = 255);

	// Control functions
	void play(void);
	bool pause(void);
	bool playNext(void);
	bool playPrevious(void);
	bool volumeUp(void);
	bool volumeDown(void);
	bool reset(void);
	bool fastForward(void);
	bool rewind(void);
	bool playPause(void);
	bool stopPlaying(void);

	bool setVolume(uint8_t volumeLevel);
	bool setEQ(uint8_t eqType);
	bool setPlayMode(uint8_t playMode);
	bool playTrackNumber(uint16_t trackNumber);

	// Query commands
	uint8_t getPlayStatus(void);
	uint8_t getVolume(void);
	uint8_t getEQ(void);
	uint8_t getPlayMode(void);
	uint16_t getVersion(void);
	uint16_t getSongCount(void);
	uint16_t getTrackNumber(void);
	uint16_t getTrackElapsedTime(void);
	uint16_t getTrackTotalTime(void);

	// Helper functions
	bool isConnected(void);
	bool isPlaying(void);
	bool setPlayModeFull(void);	  // Play all songs on the SD card, then loop
	bool setPlayModeFolder(void); // Play all songs in the folder, then loop
	bool setPlayModeSingle(void); // Play song, then loop
	bool setPlayModeRandom(void); // Play random song, then play another random song, with no end
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
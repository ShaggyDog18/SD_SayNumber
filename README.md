# SD_SayNumber Library for DFPlayer

by **ShaggyDog18@gmail.com**, DEC 2020

github: https://github.com/ShaggyDog18/SD_SayNumber

License: [GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)

The library allows to pronounce an integer number from the range of **-999,999** to **999,999** in English or Ukrainian, male or female voices by a [DFPlayer MP3 audio Player](https://www.dfrobot.com/index.php?route=product/product&product_id=1121) in a **synchronous** (blocking) or **asynchronous** (non-blocking) modes.

Any prefix/postfix messages can be easily added. So far, meters, kilometers and miles are included to the set of the MP3 files provided in the sdcard folder.

The library is compact and efficient, can run on uControllers with limited resources.

## Resources

The library uses:
- [DFRobot library for DFPlayer](https://github.com/DFRobot/DFRobotDFPlayerMini) - you need to download and set it up.
- a slightly modified Queue application by [Steven de Salas](https://github.com/sdesalas/Arduino-Queue.h) - provided as part of the **SD_SayNumber** library.
- Sets of MP3 voice files for *English* and *Ukrainian*, both *male* and *female* voices. Any language may be added by writing a corresponding set of audio files to the MicroSD card (please, refer to the *sdcard* folder structure and naming convention: 001.mp3 ... 255.mp3).

## Library Methods

- `SayNumber(uint8_t busyPin, uint8_t language, bool mode)` - Class Constructor, defines *Busy* pin, *language* and *play mode*.

- `bool sayNumber(int32_t number, DFRobotDFPlayerMini& myDFPlayer)` - pronounces an integer number from the range of *-999,999* to *999,999* (6 digits lenghts). Returns *true* if a message was played OK (in sync mode) or queue is not overflowed (in asynchronous mode). Returns *false* if playing issues or the queue is full in async mode.

- `bool sayAny(uint8_t say, DFRobotDFPlayerMini& myDFPlayer)` - pronounces any MP3 file/message by its number; mp3 files should be named as 001.mp3-255.mp3 according to DFPlayer message naming convention (please, refer to the player [datasheet](https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/doc/FN-M16P%2BEmbedded%2BMP3%2BAudio%2BModule%2BDatasheet.pdf)). Usually **sayAny** method is used to pronounce prefix/postfix for the number like meters/kilometers/miles, etc. Returns *true* if a message was played OK (in synch mode), or the queue is not overflowed (in the async mode). Returns *false* if issues or the queue is full.

- `bool sayAsyncMode(DFRobotDFPlayerMini& myDFPlayer)` - pronounces a pre-constructed message that may consist of several MP3 files in an asynchronous non-blocking mode. The message construction is performed by **sayNumber()** and **sayAny()** methods. Returns *true* if player has started playing MP3 file getting it from the queue. Returns *false* if the player is still busy playing a message, or the queue is empty and there is nothing to play.

- `void setLanguage(uint8_t language)` - sets the language.

- `uint8_t getLanguage(void)` - returns the current language.

- `void setMode(bool mode)` - sets the mode, either synch or async.

- `bool getMode(void)`- returns the current mode.

- `void clearQueue(void)` - clears the queue in case of issues.

## Synchronous and Asynchronous Modes

### Asynchronous Mode

The main differentiator of the **SD_SayNumber** library is its ability to pronounce numbers and audio messages in an **Asynchronous** non-blocking mode: the audio message is constructed of several MP3 files and pushed to the queue by **sayNumber()** and/or **sayAny()** methods. Then, MP3 files are pronounced in asynchronous non-blocking mode by **sayAsyncMode()** method called each cycle at the end of the *loop()* function. The main application cycle runs unblocked while MP3 files comprising the message are being played one by one taken from the queue. A consequent MP3 file is played as soon as playing of the current MP3 file is completed.

It is important to maintain enough time for the whole message to be played in the async mode before the next message pops up to prevent the queue overflow. Otherwise, the message may be jammed. If so, call a **clearQueue()** method to resolve the issue and start over (refer to the library example).

You may also extend the length of the queue (in the SD_SayNumber.h file), but better to manage the right timing between messages.

### Synchronous Mode

Playing messages in **Synchronous** mode means the main application cycle is blocked for the time while the entire message is being played.

Both modes can be used in one application. Refer to the library example in which a synch mode is used in *setup()* function, while the asynch mode is used in the *loop()* function.

## DFPlayer Connection

Module is connected to UART interface (RX-TX) according to [datasheet](https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/doc/FN-M16P%2BEmbedded%2BMP3%2BAudio%2BModule%2BDatasheet.pdf). 
A **BUSY** pin of the DFPlyer is also used to monitor the Player status: the pin signals if the player is idle and ready to play the next mp3 file (pin is high), or it is currently busy playing a file (pin is low).

For more details on the library use, please, refer to the example that utilizes all library methods.

## Compatibility

The library developed for Arduino UNO/NANO/Pro Mini, ESP8266, ESP32, STM32, etc.

## Gratitude

**If you use and like this library, please, consider making a small "cup of coffee" donation using [PayPal](https://paypal.me/shaggyDog18/3USD)**

Enjoy!

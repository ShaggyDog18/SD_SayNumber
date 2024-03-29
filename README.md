# SD_SayNumber Library for DFPlayer

by **ShaggyDog18@gmail.com**, FEB-2021. Updated on MAY-2022
**Version 2.0**

github: https://github.com/ShaggyDog18/SD_SayNumber

License: [GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)

The library allows to pronounce both integer numbers in the range of **-999,999** to **999,999** and float numbers in range of **-999,999.99** to **999,999.99** in English or Ukrainian, male or female voices by a [DFPlayer MP3 audio Player](https://www.dfrobot.com/index.php?route=product/product&product_id=1121) in a **synchronous** (blocking) or **asynchronous** (non-blocking) modes.

Any prefix/postfix voice messages can be easily added. So far, meters, kilometers and miles are included to the set of MP3 files provided in the [sdcard folder](https://github.com/ShaggyDog18/SD_SayNumber/tree/main/sdcard).

The library is compact and efficient, can run on uControllers with limited resources.

Inhale life to your applications :-) If you have sets of voice MP3 files for other languages, please, share. I would be glad to include them to the library! Thank you!

## Releases

- FEB-2021 - initial release **version 1.0**
- MAY-2022 - **version 2.0** major update: simplified and optimized the library by inheriting all methods of **DFPlayer** class instead of having it as a separate object and passing its reference to all library functions. 
So, if you use an initial release, you need to update all **Say** methods by simply removing references to **DFPlayer**. Also, all functions of DFPlayer are inherited by the library. Please, refer to Examples on how to update your code.

## Resources

The library uses:
- [DFRobot library for DFPlayer](https://github.com/DFRobot/DFRobotDFPlayerMini) - you need to download and set it up.
- a slightly modified **Queue** application by [Steven de Salas](https://github.com/sdesalas/Arduino-Queue.h) - provided as part of the **SD_SayNumber** library.
- Sets of MP3 audio files for *English* and *Ukrainian*, both *male* and *female* voices. Any language may be added by writing a corresponding set of audio files to the MicroSD card (please, refer to the [*sdcard* folder](https://github.com/ShaggyDog18/SD_SayNumber/tree/main/sdcard) structure and naming convention: 001.mp3 ... 255.mp3).

## Library Methods

- `SayNumber(uint8_t busyPin, uint8_t language, bool mode)` - Class Constructor, defines *Busy* pin, *language* and *play mode*.

- `bool sayInteger(int32_t number)` - pronounces integer numbers in the range of *-999,999* to *999,999* (6 digits lenghts). Returns *true* if a message was played OK (in sync mode) or queue is not overflowed (in asynchronous mode). Returns *false* if playing issues or the queue is full in async mode.

- `bool sayFloat(float number, uint8_t floatDecimalPrecision)` - pronounces float numbers in the range of *-999,999.99* to *999,999.99* (6 digits integer part and either one or two digits decimal part). Use a float decimal precision constant *FLOAT_POINT_ONE* for one digit after the dot, or *FLOAT_POINT_TWO* for two digits. Returns *true* if a message was played OK (in sync mode) or queue is not overflowed (in asynchronous mode). Returns *false* if playing issues or the queue is full in async mode. 
**Note:** Be aware, that adding *float* variables to your application will result in immediate increase of the sketch size by ~2.5kBytes. Try to avoid *float* data type if uController's resources are scarce.  

- `bool sayAny(uint8_t say)` - pronounces any MP3 file/message by its number; mp3 files should be named as 001.mp3-255.mp3 according to DFPlayer message naming convention (please, refer to the player [datasheet](https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/doc/FN-M16P%2BEmbedded%2BMP3%2BAudio%2BModule%2BDatasheet.pdf)). Usually **sayAny** method is used to pronounce prefix/postfix for the number like meters/kilometers/miles, etc. Returns *true* if a message was played OK (in synch mode), or the queue is not overflowed (in the async mode). Returns *false* if issues or the queue is full.

- `bool sayAsyncMode(void)` - pronounces a pre-constructed message that may consist of several MP3 files in an asynchronous non-blocking mode. The message construction is performed by **sayNumber()** and **sayAny()** methods. Returns *true* if player has started playing MP3 file getting it from the queue. Returns *false* if the player is still busy playing a message, or the queue is empty and there is nothing to play.

- `void setLanguage(uint8_t language)` - sets the language. Refer to **SayNumber.h** for the supported list of languages. 

- `uint8_t getLanguage(void)` - returns the current language.

- `void setMode(bool mode)` - sets the mode, either synch or async. Use predefined constants *MODE_SYNC* or *MODE_ASYNC* accordingly.

- `bool getMode(void)`- returns the current mode.

- `void clearQueue(void)` - clears the queue in case of issues.

## Synchronous and Asynchronous Modes

### Asynchronous Mode

The main differentiator of the **SD_SayNumber** library is its ability to pronounce numbers and audio messages in an **Asynchronous** non-blocking mode: the audio message is constructed of several MP3 files and pushed to the queue by **sayInteger()**, **sayFloat()** and/or **sayAny()** methods. Then, MP3 files are pronounced in asynchronous non-blocking mode by **sayAsyncMode()** method called each cycle at the end of the *loop()* function. The main application cycle runs unblocked while MP3 files comprising the message are being played one by one taken from the queue. A consequent MP3 file in the queue is played as soon as playing of the current MP3 file is completed.

It is important to maintain enough time for the whole audio message to be played in the async mode before the next message pops up to prevent the queue overflow. Otherwise, the message may be jammed. If so, call a **clearQueue()** method to resolve the issue and start over (refer to the [library example](https://github.com/ShaggyDog18/SD_SayNumber/blob/main/example/SayNumberSD/SayNumberSD.ino)).

You may extend the length of the queue (in the [SD_SayNumber.h](https://github.com/ShaggyDog18/SD_SayNumber/blob/main/SD_SayNumber.h) file), but it's better to maintain the right timing between messages. Currently, the length of the queue is 18 bytes which is double of what is required to pronounce the lartgest integer number 999,999: *"nine hundred ninety nine thousand nine hundred ninety nine"* that engages nine MP3 files.

### Synchronous Mode

Playing messages in **Synchronous** mode means the main application cycle is blocked for the time while the entire message is being played.

Both modes can be used in one application. Refer to the [library example](https://github.com/ShaggyDog18/SD_SayNumber/blob/main/example/SayNumberSD/SayNumberSD.ino) in which a synch mode is used in *setup()* function, while the asynch mode is used in the *loop()* function.

## DFPlayer Connection

Module is connected to UART interface (RX-TX) according to [datasheet](https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/doc/FN-M16P%2BEmbedded%2BMP3%2BAudio%2BModule%2BDatasheet.pdf). 
A **Busy** pin of the DFPlyer is used to monitor the Player status: the **Busy** pin signals the player is idle and ready to play the next mp3 file (pin is high), or it is currently busy playing a file (pin is low).

For more details on the library use, please, refer to the example that utilizes all library methods.

## Compatibility

The library was developed for Arduino UNO/NANO/Pro Mini, ESP8266, ESP32, STM32, etc.

## Please, contribute! 

If you have sets of voice MP3 files for other languages, please, share. I would be glad to include them to the library! Thank you!

## Gratitude

**If you use and like this library, please, consider making a small "cup of coffee" donation using [PayPal](https://paypal.me/shaggyDog18/3USD)**

Enjoy!
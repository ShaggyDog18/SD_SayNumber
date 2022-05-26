#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#define N_DIGITS     6  // max number 999,999
#define MAX_NUMBER   999999
#define QUEUE_LENGTH 18 // digits

#include "Queue.h"

// modes
#define MODE_SYNC   true
#define MODE_ASYNC  false

// define float precision: one digit or two digits after the dot
#define FLOAT_POINT_ONE 10
#define FLOAT_POINT_TWO 100

// languages- names of folders on sdcard
#define ENGLISH_MALE    10
#define ENGLISH_FEMALE  11
#define UKRAINIAN_MALE   20
#define UKRAINIAN_FEMALE 21

// below languages are not included to the library; provided as an example only
#define POLISH_MALE     30
#define POLISH_FEMALE   31
#define GERMAN_MALE     40
#define GERMAN_FEMALE   41
#define ITALIAN_MALE    50
#define ITALIAN_FEMALE  51
#define FRENCH_MALE     60
#define FRENCH_FEMALE   61
#define SPANISH_MALE    70
#define SPANISH_FEMALE  71
#define PORTUGUESE_MALE    80
#define PORTUGUESE_FEMALE  81
#define RUSSIAN_MALE    90
#define RUSSIAN_FEMALE  91
//--------------------------

// extra audio files to pronounce numbers  
#define SAY_HUNDRED  100
#define SAY_THOUSAND 101
#define SAY_ZERO  120
#define SAY_MINUS 121
#define SAY_POINT 122

// for speed and distance
#define SAY_KM    123
#define SAY_METER 124
#define SAY_MILE  125
#define SAY_KMH   125  // not in the library
#define SAY_MILEH 126  // not in the library
#define SAY_PERCENT  127  // not in the library
#define SAY_DEGREES  128  // not in the library

// for audio clock - not in the library
#define SAY_HOUR     129
#define SAY_MINUTES  130
#define SAY_SECONDS  131
#define SAY_JANUARY  132
#define SAY_FEBRUARY 133
// .... up to 255.mp3


class SayNumber : public DFRobotDFPlayerMini {
  public:
    SayNumber( uint8_t busyPin = 0, uint8_t language = ENGLISH_MALE, bool mode = MODE_SYNC );

    bool sayInteger( int32_t number );

    bool sayFloat( float number, uint8_t floatDecimalPrecision);

    bool sayAny( uint8_t say );

    bool sayAsyncMode(void);

    inline void setLanguage( uint8_t language ){ _language = language; }

    inline uint8_t getLanguage ( void ){ return _language; }

    inline void setMode( bool mode ){ _mode = mode; }

    inline bool getMode( void ){ return _mode; }

    inline void clearQueue( void ){ playQueue.clear(); }

  private:
    bool say1000( uint8_t digits[] );

    bool say100( uint8_t digits[] );

    bool say10( uint8_t digits[] );

    bool say1( uint8_t digits[] );

    void playerDelayWhilePlaying( void ); // hold on untill complete playing

    uint8_t _busyPin;

    uint8_t _language;

    bool _mode;

    Queue<uint8_t, QUEUE_LENGTH> playQueue;// = Queue<uint8_t>( QUEUE_LENGTH );
};
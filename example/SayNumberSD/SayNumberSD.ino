#include "SD_SayNumber.h"
#include <SoftwareSerial.h>

//define serial port for DFPlayer
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// define Say object
#define BUSY_PIN 4
SayNumber Say( BUSY_PIN, ENGLISH_MALE, MODE_SYNC ); // sync "blocking" mode

void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);

  delay(500);

  Serial.println();
  Serial.println(F("Say a Number Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  // DFPlayer settings (copy-pasted from DFPlayer example)
  myDFPlayer.begin(mySoftwareSerial);
  delay(500);
  Serial.println(F("DFPlayer Mini ready."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(26);  //Set volume value (0~30).
  //myDFPlayer.volumeUp(); //Volume Up
  //myDFPlayer.volumeDown(); //Volume Down

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  Serial.println(F("---Sync mode: block the cycle while is talking---"));
  
  bool errorStatus;
  Serial.println(F("---Say Float numbers---"));
  float floatNumber = 175.35F;
  Serial.print( "Say:" ); Serial.print( floatNumber ); Serial.print( ":" ); 
  errorStatus = Say.sayFloat( floatNumber, FLOAT_POINT_TWO, myDFPlayer );
  Serial.println( errorStatus ? "~OK~" : "~Error!~" );
  delay(500);
  
  //goto ASYNCH;  // jump to asynch example
  
  floatNumber = -6.241F;
  Serial.print( "Say:" ); Serial.print( floatNumber ); Serial.print( ":" );
  errorStatus = Say.sayFloat( floatNumber, FLOAT_POINT_TWO, myDFPlayer );
  Serial.println( errorStatus ? "~OK~" : "~Error!~" );
  delay(500);
  
  floatNumber = 11.56F;
  Serial.print( "Say:" ); Serial.print( floatNumber ); Serial.print( ":" );
  errorStatus = Say.sayFloat( floatNumber, FLOAT_POINT_ONE, myDFPlayer );
  Serial.println( errorStatus ? "~OK~" : "~Error!~" );
  delay(500);

  Serial.println(F("---Say Integer numbers---"));
  
  for ( int32_t i = -3; i < 1023; i++ ) {
    Serial.print( "Say:" ); Serial.print(i); Serial.print( ":" );
    errorStatus = Say.sayInteger( i, myDFPlayer );
    if ( i > 1 && i <= 3 ) errorStatus *= Say.sayAny( SAY_KM, myDFPlayer );
    else if ( i > 3 && i <= 6 ) errorStatus *= Say.sayAny( SAY_METER, myDFPlayer );
    else if ( i > 6 && i <= 9 ) errorStatus *= Say.sayAny( SAY_MILE, myDFPlayer );
    Serial.println( errorStatus ? "~OK~" : "~Error!~" );
    if ( i == 105) i = 197; // quick jump
    if ( i == 205) i = 297;
    if ( i == 305) i = 397;
    if ( i == 405) i = 497;
    if ( i == 505) i = 597;
    if ( i == 605) i = 697;
    if ( i == 705) i = 797;
    if ( i == 805) i = 897;
    if ( i == 905) i = 997;
    delay(100);
  }
  Serial.println("-----");

  // test different number ranges:
  /*
    for ( int32_t i = 998; i < 1125; i++ ) {
    Serial.print( "Say:" ); Serial.print(i); Serial.print( ":" );
    bool status = Say.sayInteger( i, myDFPlayer );
    Serial.println();
    }
    Serial.println("-----");
  */

  /*
    for ( int32_t i = 9998; i < 11125; i++ ) {
    Serial.print( "Say:" ); Serial.print(i); Serial.print( ":" );
    bool status = Say.sayInteger(i, myDFPlayer );
    Serial.println();
    }
    Serial.println("-----");
  */
  /*
    for ( int32_t i = 19998; i < 22125; i++ ) {
    Serial.print( "Say:" ); Serial.print(i); Serial.print( ":" );
    bool status = Say.sayInteger( i, myDFPlayer );
    Serial.println();
    }
    Serial.println("-----");
  */
  /*
    for ( int32_t i = 99998; i < 101135; i++ ) {
    Serial.print( "Say:" ); Serial.print(i); Serial.print( ":" );
    bool status = Say.sayInteger( i, myDFPlayer );
    Serial.println();
    }
    Serial.println("-----");
  */
  /*
    for ( int32_t i = 999998; i < 1000000; i++ ) {
      Serial.print( "Say:" ); Serial.print(i); Serial.print( ":" );
      bool status = Say.sayInteger( i, myDFPlayer );
      Serial.println();
    }
    Serial.println("-----");
  */
  
ASYNCH:
  Serial.println(F("---Set Asynchronous non-blocking mode---")); // before getting into the loop
  Say.setMode( MODE_ASYNC );   //set asynchronous "non-blocking" mode
}  //  end of setup()


unsigned long prevTime, currTime;
const unsigned long timeSpan = 8000UL; // say a number every 8 seconds
int32_t num2say = 199959; // a start number to say in the async mode
uint8_t countFalse = 0;

// say numner in async mode in the loop: non-blocking!
void loop() {

  delay(50);
  // do something useful/important
  delay(50);


  // say a number: essentially, the entire voice message is constructed here without being
  // pronounced by putting a sequesnce of voice files to the play queue
  // It is important to maintain enough time for the whole audio message to be played in the async mode before 
  // the next message pops up to prevent the queue overflow. Otherwise, the message may be jammed. 
  // If so, calling a clearQueue() method to resolve the issue and start over
  currTime = millis();
  if ( currTime - prevTime >= timeSpan ) {  // say a number every 10 seconds
    Serial.print( "\nSay async:" ); Serial.print(num2say); Serial.println( ":" );
    if ( Say.sayInteger(  num2say, myDFPlayer ) ) {
      // returns *true* if the queue is not overflowed. So, gets to the next number to pronounce
      // Returns *false* if the queue is full; So, skip till the next time span
      num2say++;
    } else {
      // count false cases
      countFalse++;
      Serial.println( "--Queue overflow--" );
      if ( countFalse >= 5) { // if count 5 errors, then clear the queue to fix the issue. Some parts of the previous message may be lost
        Serial.println( "--Clearing the Queue--" );
        Say.clearQueue();  // resolve the issue and start over
        countFalse = 0;
      }
    }
    prevTime = currTime;
  }


  delay(50);
  // do something useful/important before talking
  delay(50);


  // at the end of application loop:
  // Pronounce the message constructed above by getting MP3 files one by one from the play queue.
  // Returns *true* if player has started playing the next MP3 file from the queue.
  // Returns *false* if the player is still busy playing a message, or the queue is empty and there is nothing to play.
  bool playerStatus = Say.sayAsyncMode( myDFPlayer );
  Serial.print( playerStatus );   if ( playerStatus ) Serial.println();
  // 1 shows loop cycles at which the player has started playing the next MP3 file from the queue.
  // zero values shows loop cycles at which the player was busy or there was nothing to say...
 }

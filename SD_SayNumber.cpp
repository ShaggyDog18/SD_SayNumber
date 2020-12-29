#include "SD_SayNumber.h"

//#define DEBUG

#ifdef DEBUG
  char temp[9];
#endif


SayNumber::SayNumber( uint8_t busyPin, uint8_t language, bool mode ): _busyPin(busyPin), _language(language), _mode(mode) {
  pinMode( _busyPin, INPUT );
}


bool SayNumber::sayAny( uint8_t say, DFRobotDFPlayerMini& myDFPlayer ) {
  bool errorState = true;
  if( _mode ) {
    myDFPlayer.playFolder( _language, say ); //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
    playerDelayWhilePlaying();
  } else {
    errorState = playQueue.push( say );
  }
  return errorState;
}


bool SayNumber::sayInteger( int32_t x, DFRobotDFPlayerMini& myDFPlayer ) {
  uint8_t digits[N_DIGITS] = {0, 0, 0, 0, 0, 0};  // max number 999,999; 
  bool errorState = true; // true - means OK, no error

  if( x > MAX_NUMBER || x < -MAX_NUMBER ) 
    return false;  // the number is out of range
  else if( x == 0 )
    return sayAny( SAY_ZERO, myDFPlayer );  // say Zero
  else if( x < 0 ) {
    errorState = sayAny( SAY_MINUS, myDFPlayer );  // say Minus
    x = - x;
  }

  // split a number to digits
  uint8_t decade = 0;
  do {
    digits[N_DIGITS - decade - 1] = x % 10;
    decade++;
    x /= 10;
  } while ( x > 0 );
  decade--;
  /*
        Serial.print("~");
        for ( uint8_t i = 0; i < N_DIGITS; i++) Serial.print(digits[i]);
        Serial.print("::");
  */
  switch( decade ) { // !!!no breaks!!!
    case 5: errorState *= say100( &digits[0], myDFPlayer );

    case 4:
    case 3: errorState *= say1000( &digits[1], myDFPlayer );

    case 2: errorState *= say100( &digits[3], myDFPlayer );

    case 1:
    case 0: errorState *= say10( &digits[4], myDFPlayer );
  }

  #ifdef DEBUG
    if( !_mode ) {
      Serial.print("Queue length: "); Serial.println( playQueue.count() );
    }
  #endif
  return errorState;
}


bool SayNumber::sayFloat( float number, uint8_t floatDecimalPrecision, DFRobotDFPlayerMini& myDFPlayer ) {
	bool errorState = true;
	
	if( number < 0.0f ) {
		errorState = sayAny( SAY_MINUS, myDFPlayer );
		number = - number;
	}
	int32_t intPart = (int) number;
	int32_t decPart = (int) ((number - intPart) * floatDecimalPrecision );
	
	#ifdef DEBUG
		Serial.print( "Float Integer part: " ); Serial.println( intPart );
		Serial.print( "Float Decimal part: " ); Serial.println( decPart );
	#endif

	errorState *= sayInteger( intPart, myDFPlayer );
	errorState *= sayAny( SAY_POINT, myDFPlayer );
	errorState *= sayInteger( decPart, myDFPlayer );
	return errorState;
}


bool SayNumber::say1000( uint8_t digits[], DFRobotDFPlayerMini& myDFPlayer ) {
  bool errorState;

  errorState = say10( &digits[0], myDFPlayer );
  #ifdef DEBUG
    Serial.print( " thousand " );
  #endif
  errorState *= sayAny( SAY_THOUSAND, myDFPlayer );
  return errorState;
}


bool SayNumber::say100( uint8_t digits[], DFRobotDFPlayerMini& myDFPlayer ) {
  bool errorState;
  
  switch( _language ) {
    case UKRAINIAN_MALE:
    case UKRAINIAN_FEMALE:
    case POLISH_MALE:
    case POLISH_FEMALE:
    case RUSSIAN_MALE:
    case RUSSIAN_FEMALE:
      errorState = sayAny( 200+digits[0], myDFPlayer );  // 100, 200, 300, 400,...900 recorded to files 201, 201, 203, 204, 209 correspondigly
      break;
      
    default:  // ENGLISH, SPANISH, ITALIAN....
      errorState = say1( &digits[0], myDFPlayer );
      if( digits[0] ) {
		#ifdef DEBUG
			Serial.print( " hundred " );
		#endif
        errorState *= sayAny( SAY_HUNDRED, myDFPlayer );
      }
  }
  return errorState;
}


bool SayNumber::say10( uint8_t digits[], DFRobotDFPlayerMini& myDFPlayer ) {
  bool errorState;

  if( digits[0] == 0 )  // 1...9
    errorState = say1( &digits[1], myDFPlayer ); 
  else if( digits[0] == 1 ) { //11-19
    #ifdef DEBUG
      sprintf(temp, "%03d", 10 + digits[1] );
      Serial.print( temp ); //10-19
      Serial.print( "." );
    #endif
    errorState = sayAny(  10+digits[1], myDFPlayer );
  } else {  //20,30,40...90
    #ifdef DEBUG
      sprintf(temp, "%03d", 10*digits[0] );
      Serial.print( temp ); //20, 30...90
      Serial.print( "^" );
    #endif
    errorState = sayAny( 10*digits[0], myDFPlayer );
    errorState *= say1( &digits[1], myDFPlayer );  // 1...9
  }
  return errorState;
}


bool SayNumber::say1( uint8_t digits[], DFRobotDFPlayerMini& myDFPlayer ) {
  bool errorState = true;
  if( digits[0] ) {
    #ifdef DEBUG
      sprintf(temp, "%03d", digits[0] );
      Serial.print(temp);
      Serial.print( "-" );
    #endif
    errorState = sayAny( digits[0], myDFPlayer );
  } 
  return errorState;
}


void SayNumber::playerDelayWhilePlaying( void ) {
  delay(100);
  if( _busyPin ) {
    while( !digitalRead( _busyPin ) ){  // low means busy
      delay(10);
    }
  }
}


bool SayNumber::sayAsyncMode( DFRobotDFPlayerMini& myDFPlayer ) {
  // if query is not empty && digitalRead( _busyPin )  
  if( playQueue.count() && digitalRead(_busyPin) ) {
    myDFPlayer.playFolder( _language, playQueue.pop() );
    return true;
  }
  return false;
}

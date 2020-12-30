/*
 * Queue.h
 * 
 * By Steven de Salas
 * 
 * Defines a templated (generic) class for a queue of things.
 * Used for Arduino projects, just #include "Queue.h" and add this file via the IDE.
 * 
 * Modified and optimized by ShaggyDog18@gmail.com
 *
 */

#pragma once 

#include <Arduino.h>

template<class T>
class Queue {
  private:
    uint8_t _front, _back, _count;
    uint8_t _maxitems;
    //T *_data;		// ShaggyDog: commented
	T _data[ QUEUE_LENGTH ];  // ShaggyDog: added


  public:
    Queue( int maxitems ) { 
      _front = _back = _count = 0;
	  _maxitems = maxitems;
//    _data = new T[maxitems];   // ShaggyDog: commented
    }

    ~Queue() {
//      delete[] _data;  // ShaggyDog: commented
    }

    inline uint8_t count( void );
    inline uint8_t front( void );
    inline uint8_t back( void );
    bool push(const T &item);
    T peek( void );
    T pop( void );
    void clear( void );
};

template<class T>
inline uint8_t Queue<T>::count( void ) {
  return _count;
}

template<class T>
inline uint8_t Queue<T>::front( void ) {
  return _front;
}

template<class T>
inline uint8_t Queue<T>::back( void ) {
  return _back;
}

template<class T>
bool Queue<T>::push(const T &item) {  // ShaggyDog: changed return value to bool
  if(_count < _maxitems) { // Drops out when full
    _data[_back++]=item;
    _count++;
    // Check wrap around
    if( _back >= _maxitems ) _back = 0;
/*
// DEBUG queue
		Serial.print(" Queue length: "); Serial.print( _count );
		Serial.print(" front: "); Serial.print( _front );
		Serial.print(" back: "); Serial.println( _back );
*/
    return true; // ShaggyDog: added
  } else 
    return false; // ShaggyDog: added
}

template<class T>
T Queue<T>::pop( void ) {
  if( !_count ) return T(); // Returns empty
  else {
    T result = _data[_front++];
    _count--;
    // Check wrap around
    if( _front >= _maxitems ) _front = 0;
    return result; 
  }
}

template<class T>
T Queue<T>::peek( void ) {
  if( !_count ) return T(); // Returns empty
  else return _data[_front];
}

template<class T>
void Queue<T>::clear( void ) {
  _front = _back = _count = 0;
}

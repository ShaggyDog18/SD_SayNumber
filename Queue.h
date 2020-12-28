/*
 * Queue.h
 * 
 * By Steven de Salas
 * 
 * Defines a templated (generic) class for a queue of things.
 * Used for Arduino projects, just #include "Queue.h" and add this file via the IDE.
 * 
 */

#pragma once 

#include <Arduino.h>

template<class T>
class Queue {
  private:
    int _front, _back, _count;
    T *_data;
    int _maxitems;
  public:
    Queue(int maxitems = 256) { 
      _front = 0;
      _back = 0;
      _count = 0;
      _maxitems = maxitems;
      _data = new T[maxitems + 1];   
    }
    ~Queue() {
      delete[] _data;  
    }
    inline int count( void );
    inline int front( void );
    inline int back( void );
    bool push(const T &item);
    T peek( void );
    T pop( void );
    void clear( void );
};

template<class T>
inline int Queue<T>::count( void ) {
  return _count;
}

template<class T>
inline int Queue<T>::front( void ) {
  return _front;
}

template<class T>
inline int Queue<T>::back( void ) {
  return _back;
}

template<class T>
bool Queue<T>::push(const T &item) {
  if(_count < _maxitems) { // Drops out when full
    _data[_back++]=item;
    ++_count;
    // Check wrap around
    if (_back > _maxitems)
      _back -= (_maxitems + 1);
    return true;
  } else 
    return false;
}

template<class T>
T Queue<T>::pop( void ) {
  if(_count <= 0) return T(); // Returns empty
  else {
    T result = _data[_front];
    _front++;
    --_count;
    // Check wrap around
    if (_front > _maxitems) 
      _front -= (_maxitems + 1);
    return result; 
  }
}

template<class T>
T Queue<T>::peek( void ) {
  if(_count <= 0) return T(); // Returns empty
  else return _data[_front];
}

template<class T>
void Queue<T>::clear( void ) {
  _front = _back;
  _count = 0;
}

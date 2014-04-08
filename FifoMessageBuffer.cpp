/*************************************************** 
  Using https://github.com/adafruit/Adafruit-BMP085-Library as template for an Arduino library.

  This is a library for the Adafruit BMP085/BMP180 Barometric Pressure + Temp sensor

  Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout 
  ----> http://www.adafruit.com/products/391
  ----> http://www.adafruit.com/products/1603

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <stdlib.h>

#include "FifoMessageBuffer.h"

// Use bufPtr instead of malloc'ing. m_bufSize will be bufSize-1 to allow for a null sentinel
FifoMessageBuffer::FifoMessageBuffer(char * bufPtr, short bufSize) {
  m_freeBufferOnDestructor = false;
  initForBuffer(bufPtr, bufSize);
}

FifoMessageBuffer::FifoMessageBuffer(short bufSize) {
  char * bufPtr = (char *)malloc(bufSize + 1);
  m_freeBufferOnDestructor = true;
  initForBuffer(bufPtr, (bufSize + 1));
}

FifoMessageBuffer::~FifoMessageBuffer() {
  if (m_freeBufferOnDestructor == true) {
    free(m_buffer);
    m_buffer = NULL;
    m_bufSize = 0;
    m_freeBufferOnDestructor = false;
    m_length = 0;
  }
}

// Add a char. return 1 if added. return 0 if buffer is already full.
short FifoMessageBuffer::add(char aChar) {
  if (m_length >= m_bufSize) { // already full
    return 0;
  }
  else if (aChar == '\0') {
    return 0; // no NULL characters will be added.
  }
  else if (m_firstCharacter == NULL) {
    m_firstCharacter = m_buffer;
    *m_firstCharacter = aChar;
    m_length = 1;
    *(m_firstCharacter+1) = '\0';
    return 1;
  }
  else { // room for another character
    char * nextPtr = m_firstCharacter + m_length;
    if (nextPtr >= m_pastRawBuffer) {
      nextPtr = m_buffer; // wrapped to start
    }
    *nextPtr = aChar;

    ++nextPtr; // add sentinel NULL at end of message
    if (nextPtr >= m_pastRawBuffer) {
      nextPtr = m_buffer; // wrapped to start
    }
    ++m_length;
    *nextPtr = '\0';
    return 1;
  }
}

// Add a null terminated char string. return the number of characters added.
short FifoMessageBuffer::add(char * srcString) {
  short add_count = 0;
  int srcPos = 0;
  for ( ; (srcString[srcPos] != '\0'); ++srcPos ) {
    add_count = add(srcString[srcPos]);
    if (add_count == 0) {
      return srcPos;
    }
  }
  return srcPos;
}

// Add length characters from src.  Terminates if null in src or buffer fills.
short FifoMessageBuffer::add(char * src, short length) {
  short add_count = 0;
  int srcPos = 0;
  for ( ; srcPos<length; ++srcPos ) {
    add_count = add(src[srcPos]);
    if (add_count == 0) {
      return srcPos;
    }
  }
  return srcPos;
}

short FifoMessageBuffer::available() { // Number of character slots available.
  return (m_bufSize - m_length);
}

// Clear the buffer. Return the number of characters removed.
short FifoMessageBuffer::clear() {
  short clear_count = m_length;
  m_length = 0;
  m_firstCharacter = NULL;
  *m_buffer = '\0';
  return clear_count;
}

// Clear count characters from the start of the buffer. Return the number of characters removed.
short FifoMessageBuffer::clear(short count) {
  if (m_length <= 0) {
    return 0;
  }
  else if (count >= m_length) {
    return clear();
  }
  else { // count < m_length
    m_firstCharacter += count;
    if (m_firstCharacter >= m_pastRawBuffer) {
      m_firstCharacter = m_buffer  + (m_firstCharacter - m_pastRawBuffer);
    }
    return count;
  }
}

// return the position of the character, -1 if not found.
short FifoMessageBuffer::find(char aChar) {
  if (m_length <= 0) {
    return -1;
  }
  char * nextChar = m_firstCharacter;
  int foundPos = 0;
  for (; foundPos<m_length; ++foundPos, ++nextChar) {
    if (nextChar >= m_pastRawBuffer) {
      nextChar = m_buffer; // wrap to beginning
    }
    if (*nextChar == aChar) {
      return foundPos;
    }
  }
  return -1;
}

void FifoMessageBuffer::initForBuffer(char * bufPtr, short bufSize) {
  m_bufSize = bufSize - 1;
  m_buffer = bufPtr;
  m_pastRawBuffer = m_buffer + (m_bufSize + 1);
  m_firstCharacter = NULL;
  m_length = 0;
}

char * FifoMessageBuffer::insertPtr() {
  if (m_length >= m_bufSize) {
    return NULL;
  } else {
    char * nextPtr = m_firstCharacter + m_length;

    ++m_length;
    if (nextPtr >= m_pastRawBuffer) {
      nextPtr = m_buffer  + (nextPtr - m_pastRawBuffer);
    }
    return nextPtr;
  }
}

short FifoMessageBuffer::length() {
  return m_length;
}

// pop all available characters if possible. dst is NULL terminated. Return the number of characters copied and removed.
short FifoMessageBuffer::pop(char *dst, short dst_length) {
  if (dst_length < 1) {
    return 0;
  }
  else if (m_length <= 0) {
    *dst = '\0';
    return 0;
  }
  else {
    short nextPos = 0;
    short transfer_count = (m_length < dst_length) ? m_length : (dst_length-1);
    for ( ; nextPos<transfer_count; ++nextPos ) {
      dst[nextPos] = *m_firstCharacter;
      ++m_firstCharacter;
      if (m_firstCharacter >= m_pastRawBuffer) {
        m_firstCharacter = m_buffer; // wrap to start of ring buffer
      }
    }
    dst[nextPos] = '\0';
    return transfer_count;
  }
}

// pop dst_length characters if possible. dst is NULL terminated. Return the number of characters copied and removed.
short FifoMessageBuffer::pop(short length, char *dst, short dst_length) {
  if (dst_length < 1) {
    return 0;
  }
  else if (m_length <= 0) {
    *dst = '\0';
    return 0;
  }
  else {
    short nextPos = 0;
    short transfer_count = (length < dst_length) ? length : (dst_length-1);
    if (transfer_count > m_length) {
      transfer_count = m_length;
    }
    for ( ; nextPos<transfer_count; ++nextPos ) {
      dst[nextPos] = *m_firstCharacter;
      ++m_firstCharacter;
      if (m_firstCharacter >= m_pastRawBuffer) {
        m_firstCharacter = m_buffer; // wrap to start of ring buffer
      }
    }
    dst[nextPos] = '\0';
    return transfer_count;
  }
}


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

#ifndef FIFOMESSAGEBUFFER_H
#define FIFOMESSAGEBUFFER_H

class FifoMessageBuffer {
  private:
    static const short defaultBufSize = 255;
  public:
    FifoMessageBuffer() : FifoMessageBuffer(defaultBufSize) { };
    FifoMessageBuffer(char * bufPtr, short bufSize); // Use this buffer instead of malloc'ing. length with be bufSize-1 to allow for a null sentinel
    FifoMessageBuffer(short bufSize);
    ~FifoMessageBuffer();

    short add(char aChar); // Add a char. return 1 if added. return 0 if buffer is already full.
    short add(char * srcString); // Add a null terminated char string. return the number of characters added.
    short add(char * src, short length); // Add length characters from src.  Terminates if null in src or buffer fills.
    short bufSize() { return m_bufSize; } // maximum message length. One less than actual buffer size.
    short clear(); // Clear the buffer. Return the number of characters removed.
    short clear(short count); // Clear count characters from the start of the buffer. Return the number of characters removed.
    short find(char src); // return the position of the character, -1 if not found.
    short length();
    short pop(char *dst, short dst_length); // pop all available characters if possible. dst is left null terminated. Return the number of characters copied and removed.
    short pop(short length, char *dst, short dst_length); // pop length characters if possible. dst is left null terminated. Return the number of characters copied and removed.

  private:
    char * m_buffer; // 1 more than m_bufSize so there is a null sentinel character
    short m_bufSize; // Maximum message length. 1 less than actual buffer length.
    char * m_firstCharacter; // NULL with m_length==0 when no characters available.
    bool m_freeBufferOnDestructor;
    short m_length;
    char * m_pastRawBuffer;

    void initForBuffer(char * bufPtr, bufSize);
    char * insertPtr();
};

#endif // FIFOMESSAGEBUFFER_H


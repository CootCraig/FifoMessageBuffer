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

Class FifoMessageBuffer {
  private:
    static const short defaultBufSize = 256;
  public:
    FifoMessageBuffer() : FifoMessageBuffer(defaultBufSize) { };
    FifoMessageBuffer(unsigned short bufSize);
    ~FifoMessageBuffer();

    short bufSize() { return m_bufSize };
  private:
    short m_bufSize;
};

#endif // FIFOMESSAGEBUFFER_H


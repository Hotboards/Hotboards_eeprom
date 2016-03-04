/*
  Hotboards_eeprom.cpp - Driver to control serial (spi) eeprom memories, The memories are
  compatibles amount the manufactures Microchip, Atmel and ST, and of course you can control
  Hotboards eeprom board (http://hotboards.org)
  Created by Diego Perez, January 16, 2016.
  Released into the public domain.

  Density:   1Kbit   | 2Kbit   | 4Kbit   | 8Kbit   | 16Kbit  | 32Kbit  | 64Kbit  | 128Kbit | 256Kbit | 512Kbit | 1 Mbit
  Part:      25xx010 | 25xx020 | 25xx040 | 25xx080 | 25xx160 | 25xx320 | 25xx640 | 25xx128 | 25xx256 | 25xx512 | 25xx1024
  Page/Byte: 16      | 16      | 16      | 16(32)  | 16(32)  | 32      | 32      | 64      | 64      | 128     | 256
  Addr/Bits: 7       | 8       | 9       | 16      | 16      | 16      | 16      | 16      | 16      | 16
*/

#ifndef Hotboards_eeprom_h
#define Hotboards_eeprom_h

#include <Arduino.h>

//EEPROM size in kilobits. EEPROM part numbers are usually designated in k-bits.
typedef enum
{
    HT_EEPROM25xx_1Kb = 0,
    HT_EEPROM25xx_2Kb,
    HT_EEPROM25xx_4Kb,
    HT_EEPROM25xx_8Kb,
    HT_EEPROM25xx_16Kb,
    HT_EEPROM25xx_32Kb,
    HT_EEPROM25xx_64Kb,
    HT_EEPROM25xx_128Kb,
    HT_EEPROM25xx_256Kb,
    HT_EEPROM25xx_512Kb,
    HT_EEPROM25xx_1Mb
}_eEEPROM;

/** Hotboards_eeprom class.
 *  Used to read and write Microchip 25xxx spi eeproms
 *
 * Example:
 * @code
 * #include "Hotboards_eeprom.h"
 *
 * Hotboards_eeprom mem( 7, HT_EEPROM25xx_32Kb );
 *
 * int setup( void )
 * {
 *   SPI.begin( );
 *   mem.begin( );
 *   mem.write( 0, 0x12 );
 *   uint8_t var = mem.read( 0 );
 * }
 * @endcode
 */
class Hotboards_eeprom
{
    public :
        /** Create Hotboards_eeprom instance
          * @param cs pin where to control the chip select
          * @param type eeprom type (use values from _eEEPROM enumeration)
          *
          * Example:
          * @code
          *   // instance 32Kbits memory with CS signal on pin 7
          *   Hotboards_eeprom mem( 7, HT_EEPROM25xx_32Kb );
          * @endcode
          */
        Hotboards_eeprom( uint8_t cs, uint8_t type );

        /** Just init CS pin
          *
          * Example:
          * @code
          *   // instance 32Kbits memory with CS on pin 7
          *   Hotboards_eeprom mem( 7, HT_EEPROM25xx_32Kb );
          *   mem.begin( ); // we are now ready to write and read
          * @endcode
          */
        void begin( void );

        /** write a single byte in a given eeprom address, this operation will take 5ms
          * @param address eeprom address where tha byte will be written
          * @param data the byte that will be written at the given address
          *
          * Example:
          * @code
          *   // Write character A on address 0x00
          *   mem.write( 0, 'A' );
          * @endcode
          */
        void write( uint32_t address, uint8_t data );

        /** write a given number of bytes into the eeprom atarting at a given address
          * @param address start eeprom address where tha byte array will be written
          * @param data pointer to array of bytes that need to be written
          * @param size number of bytes to write
          *
          * Example:
          * @code
          *   uint8_t array[] = {0,1,2,3,4,5};
          *   // Write array "array" starting on address 0x00
          *   mem.write( 0, array, 6 );
          * @endcode
          */
        void write( uint32_t address, uint8_t *data, uint16_t size );

        /** read a single byte from a given eeprom address
          * @param address eeprom address where the byte will be read
          *
          * Example:
          * @code
          *   // Read a character from address 0x00
          *   uint8_t var = mem.read( 0 );
          * @endcode
          */
        uint8_t read( uint32_t address );

        /** read a given number of bytes from the eeprom starting at a given address
          * @param address eeprom address where the bytes will be read it
          * @param data pointer to array where data will be stored
          * @param size the number of bytes to read
          *
          * Example:
          * @code
          *   uint8_t array[6];
          *   // read 6 bytes from address 0x00 and store in array "array"
          *   mem.read( 0, array, 6 );
          * @endcode
          */
        void read( uint32_t address, uint8_t *data, uint16_t size );

    private :
        void sendAddress( uint8_t cmd, uint32_t address );
        void writePage( uint32_t address, uint8_t *data, uint16_t size );

        uint8_t _cs_pin;
        uint8_t _type;
        uint16_t _page;
        uint32_t _density;
};

#endif

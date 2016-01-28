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

#include <inttypes.h>

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

class Hotboards_eeprom
{
    public :
        Hotboards_eeprom( uint8_t cs, uint8_t type );
        void begin( void );
        void write( uint32_t address, uint8_t data );
        void write( uint32_t address, uint8_t *data, uint16_t size );
        uint8_t read( uint32_t address );
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

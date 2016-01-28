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
#include "Hotboards_eeprom.h"
#include <SPI.h>
#include <inttypes.h>

#define READ     0x03
#define WRITE    0x02
#define WRDI     0x04
#define WREN     0x06
#define RDSR     0x05
#define WRSR     0x01

const uint16_t PageSize[ ] = { 16, 16, 16, 16, 32, 32, 64, 64, 128, 256 };
const uint32_t EepromSize[ ] = { 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072 };

Hotboards_eeprom::Hotboards_eeprom( uint8_t cs, uint8_t type )
{
    _cs_pin = cs;
    _type = type;
    _page = PageSize[ type ];
    _density = EepromSize[ type ];
}

/*
 * just make sure the Cs pin is not clear
 */
void Hotboards_eeprom::begin( void )
{
   pinMode( _cs_pin, OUTPUT );
   digitalWrite( _cs_pin, HIGH ); 
}

/*
 * write a single byte in a given eeprom address, this operation will take 5ms
 * address: eeprom address where tha byte will be written
 * data: the byte that will be written at the given address 
 */
void Hotboards_eeprom::write( uint32_t address, uint8_t data )
{
    write( address, &data, 1 );
}

/*
 * write a given number of bytes into the eeprom atarting at a given address
 * address: eeprom address where tha byte will be written
 * data: pointer to array of bytes that need to be written
 * size: the number of bytes to write 
 */
void Hotboards_eeprom::write( uint32_t address, uint8_t *data, uint16_t size )
{
    uint16_t temp;
    /* just to cover your ass, check if the address is a valid direction */
    if( ( address < _density ) && ( size != 0 ) )
    {
        /* check if the amount of bytes to be read from the requested address do not exced the eeprom
        memory map if so, then only read the bytes left*/
        if( ( address + size ) > _density )
        {
            size = _density - address;
        }

        /* Check how many bytes are left to be written within a single page */
        temp = _page - ( address % _page );
        if( temp >= size  )
        {
            /* Data lenght can be written witihn the current page */
            writePage( address, data, size );
        }
        else
        {
            /* Data can not be written within the current page, so write only the bytes left */
            writePage( address, data, temp );
            /* update pointer to data, lenght and addres */
            data = data + temp;
            size = size - temp;
            address = address + temp;
                
            /* Check if the remaining data is bigger than a page size */
            while( size > _page )
            {
                /* Write an entire page into eeprom */
                writePage( address, data, _page );
                size = size - _page;
                data = data + _page;
                address = address + _page;
            }
            /* Write one last time if there is still data to be written */
            if( size != 0 )
            {
                writePage( address, data, size );
            }
        }
    } 
}

/*
 * read a single byte from a given eeprom address
 * address: eeprom address where the byte will be read
 */
uint8_t Hotboards_eeprom::read( uint32_t address )
{
    uint8_t data;
    read( address, &data, 1 );
    return data;  
}

/*
 * read a given number of bytes from the eeprom starting at a given address
 * address: eeprom address where the bytes will be read it
 * data: pointer to array where data will be stored
 * size: the number of bytes to read 
 */
void Hotboards_eeprom::read( uint32_t address, uint8_t *data, uint16_t size )
{
    uint16_t i;
    /* just to cover your ass, check if the address is a valid direction */
    if( ( address < _density ) && ( size != 0 ) )
    {
        /* check if the amount of bytes to be read from the requested address do not exced the eeprom
        memory map if so, then only read the bytes left*/
        if( ( address + size ) > _density )
        {
            size = _density - address;
        }
        
        /* operation begins, select memory */
        digitalWrite( _cs_pin, LOW );
        
        /* Send command Read and address, depend on device is the number of address bytes neccesary */
        sendAddress( READ, address );

        for( i = 0 ; i<size ; i++ )
        {
            /* Read data from memory */
            *data = SPI.transfer( 0xAA );
            data++;
        };
        /* operation ended, unselect memory */
        digitalWrite( _cs_pin, HIGH );
    }
}

void Hotboards_eeprom::sendAddress( uint8_t cmd, uint32_t address )
{
    if( _type < HT_EEPROM25xx_4Kb )
    {
        /* Send the command and one byte address */
        SPI.transfer( cmd );
        SPI.transfer( address );
    }
    else if( _type == HT_EEPROM25xx_4Kb )
    {
        /* Send the command coded in the MSB of the address and one LSB address */
        SPI.transfer( cmd | ( uint8_t )( address >> 8 ) );
        SPI.transfer( ( uint8_t )address );
    }
    else if( ( _type > HT_EEPROM25xx_4Kb ) && ( _type < HT_EEPROM25xx_1Mb ) )
    {
        /* Send command plus two byte address */
        SPI.transfer( cmd );
        SPI.transfer( ( uint8_t )( address >> 8 ) );
        SPI.transfer( ( uint8_t )address );
    }
    else if( _type == HT_EEPROM25xx_1Mb )
    {
        /* Send command plus three byte address */
        SPI.transfer( cmd );
        SPI.transfer( ( uint8_t )( address >> 16 ) );
        SPI.transfer( ( uint8_t )( address >> 8 ) );
        SPI.transfer( ( uint8_t )address );
    }
}

void Hotboards_eeprom::writePage( uint32_t address, uint8_t *data, uint16_t size )
{
    uint16_t i;

    /* Select memory */
    digitalWrite( _cs_pin, LOW );  
    /* write eneable latch */
    SPI.transfer( WREN );
    /* Unselect eeprom */
    digitalWrite( _cs_pin, HIGH );    
    
    /* Select memory */
    digitalWrite( _cs_pin, LOW );    
    /* Send comand write and address, depend on device is the number of address bytes neccesary */
    sendAddress( WRITE, address );
    /* Send data to eeprom */
    for( i = 0 ; i<size ; i++ )
    {
        /* Read data from memory */
        SPI.transfer( *data );
        data++;
    };
    /* Unselect eeprom */
    digitalWrite( _cs_pin, HIGH );    
    /* wait until data has been recorded */
    delay( 6 );  
}



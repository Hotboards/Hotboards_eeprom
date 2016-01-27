/*
  Hotboards eeprom Library - Byte array

 Demonstrates the use a spi eeprom. The Hotboards_eeprom
 library works with all Microchip 25xxx serial memory series 
 like the one presented eeprom board (http://www.hotboards.org).

 This sketch write a single byte into memory, then read it back
 and display the value on the serial port.

 The circuit:
 *  VDD   -->  3.3v
 *  SO    -->  MISO (ICSPs conector)
 *  SI    -->  MOSI (ICSPs conector)
 *  SCLK  -->  CLK (ICSP conector)
 *  CS    -->  D7
 *  GND   -->  GND
 
 Library and example created by Diego from Hotboards
 
 This example code is in the public domain.

 */
#include <Hotboards_eeprom.h>
#include <SPI.h>


/* lets create an eeprom instance, the frist paramter is the CS pin and th second one 
   is the memory density (32Kb),  */
Hotboards_eeprom eeprom( 7, HT_EEPROM25xx_32Kb );

void setup( void ) 
{
    /* variable to hold the read it value from the memory */
    uint8_t var;
 
    /*open serial port and send welcome message*/
    Serial.begin( 9600 );
    /* since the driver needs the spi, we need to configure it before */
    SPI.begin( );
    /* don't forget the eeprom only works up to 10MHz */
    SPI.beginTransaction( SPISettings( 10000000, MSBFIRST, SPI_MODE0 ) );

    /* initialize eeprom driver */
    eeprom.begin( );
    /* write a byte with 0x12 value at address 0x00 */
    Serial.println( "Writing an single byte on addres 0x00" );
    eeprom.write( 0, 0x12 );
    /* read the same value already store it at eeprom */
    Serial.println( "Reading an single byte from address 0x00" );
    var = eeprom.read( 0 );
    
    /* this for loop is only to display the array using the serial port */
    Serial.print( "var = " );
    Serial.println( var, HEX );
}

void loop( void ) 
{
    /* since this example only requires one time excution, is not necesary 
       to put anything here */
}


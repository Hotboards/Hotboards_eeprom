Hotboards_eeprom
=================

Genial libreria de proposito general para leer teclados matriciales con pull-ups externas en las columnas, se puede obtener la ultima tecla presionada, cambiar el tiempo en el que una tecla es poleada, incluso permite presionar mas de una tecla al mismo tiempo Descarga la libreria en formato  [**zip**](https://github.com/Hotboards/Hotboards_eeprom/archive/master.zip), renombra el archivo ( _solo quita el **-master**_ ) e importala a tu Arduino IDE com dice [aqui](http://developer.mbed.org/cookbook/Working-with-Libraries).

API
---

**Hotboards_eeprom( uint8_t cs, uint8_t type )**

Constructor para nuestra memoria eeprom. Se puede especificar la densidad de la memoria asi como el pin que controlara la señal de chip select ( _CS_ ).

``` cpp
// Creamos una memoria de 32Kbits con la señal de CS en el pin 7
Hotboards_eeprom mem( 7, HT_EEPROM25xx_32Kb );
```

**void begin( void )**

inicializa la memoria eeprom. En realidad solo el pin de CS ya que la memoria no ocupa rutina de inicializacion.
``` cpp
// creamos la memoria
Hotboards_eeprom mem( 7, HT_EEPROM25xx_32Kb );
// inicializamos
mem.begin( );
```

**void write( uint32_t address, uint8_t data )**

Escribimos un solo byte en la memoria. El parametro indica la direccion en la cual escribiremos el byte. Debes tomar en cuenta que esta operacion puede tomar hasta 5ms.

``` cpp
// escribimos el caracter A en la direccion 0x00
mem.write( 0, 'A' );
```

**void write( uint32_t address, uint8_t *data, uint16_t size )**

Escribimos un cierto numero de bytes a partir de la direccion indicada. Cada que la direccion de la memoria llegue a un multiplo de 32 la operacion se detendra por 5ms. La funcion controla este proceso internamente permitiendo escribir el numero de bytes que querramos.

``` cpp
// declaramos un arreglo para escrbirlo en la memoria
uint8_t array[] = {0,1,2,3,4,5};
// Escribimos 6 bytes de "array" en la memoria empezando en la direccion 0x00
mem.write( 0, array, 6 );
```

**uint8_t read( uint32_t address )**

Lee un solo byte de la memoria eeprom. El parametro especifica la direccion de la cual se lee el byte.

``` cpp
// Leemos un byte de la direccion 0x00
uint8_t var = mem.read( 0 );
```

**void read( uint32_t address, uint8_t *data, uint16_t size )**

Leemos un cierto numero de bytes de la memoria.

``` cpp
// en este arreglo almacenaremos los datos leidos
uint8_t array[6];
// leemos 6 bytes a partir de la direccion 0x00 y los guardamos en el arreglo "array"
mem.read( 0, array, 6 );
```

Ejemplos
--------

- [Escribiendo y leyendo un solo byte en la memoria](https://github.com/Hotboards/Hotboards_eeprom/blob/master/examples/write_byte/write_byte.ino)
- [Escribiendo y leyendo un arreglo en la memoria](https://github.com/Hotboards/Hotboards_eeprom/blob/master/examples/write_array/write_array.ino)

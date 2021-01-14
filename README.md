# Proyecto II Sistemas Operativos

## Participantes

- Alfredo Manjón Canela
- Jorge Caro Poza

## Introducción

Desarrollo realizado en la rama development.

Este proyecto consiste en la elaboración de un simulador de un sistema de ficheros tipo Linux ext muy simplificado. En lugar de trabajar sobre una partición del disco, utilizaremos un fichero binario llamado particion.bin que contiene el sistema completo. Los bloques de este sistema concreto tienen un tamaño de 512 bytes, la partición ocupa 100 bloques, y su esquema es el que se describe a continuación. No hay bloque de arranque, como en un sistema ext2, en el que el bloque 0 se reserva para tal fin.

## Desarrollo
Para el desarrollo se ha utilizado C sobre Linux (Ubuntu) para la creación de un proceso llamado **simul_ext** que lee el contenido del fichero binario particion.bin en un array de bytes en memoria y entra en un bucle infinito de procesamiento de comandos.
El bloque 0 de particion.bin lo ocupa el superbloque y tiene la siguiente forma: 
```c
typedef struct {
 short int ETQ;
 short int Datos[8];
} T_LINEA_CACHE;/* Estructura del superbloque */
typedef struct {
 unsigned int s_inodes_count; /* inodos de la partición */
 unsigned int s_blocks_count; /* bloques de la partición */
 unsigned int s_free_blocks_count; /* bloques libres */
 unsigned int s_free_inodes_count; /* inodos libres */
 unsigned int s_first_data_block; /* primer bloque de datos */
 unsigned int s_block_size; /* tamaño del bloque en bytes */
 unsigned char s_relleno[SIZE_BLOQUE-6*sizeof(unsigned int)]; /* relleno a
0's*/
} EXT_SIMPLE_SUPERBLOCK;
```

Para simplificar el problema, en lugar de bitmaps, vamos a usar bytemaps o arrays de
bytes para bloques e inodos. En el bytemap inodos, si el byte M vale 1, significa que
ese inodo está ocupado, si vale 0 está libre. Lo mismo para el bytemap de bloques.

```c
/* Bytemaps, caben en un bloque */
typedef struct {
 unsigned char bmap_bloques[MAX_BLOQUES_PARTICION];
 unsigned char bmap_inodos[MAX_INODOS]; /* inodos 0 y 1 reservados, inodo 2
directorio */
} EXT_BYTE_MAPS;
```
El bytemap de bloques tiene 100 elementos. Los 4 primeros están ocupados por el
superbloque, los bytemaps, la lista de inodos y el directorio. Los 96 restantes son los
bloque de datos. El bytemap de inodos tiene 28 elementos. Los inodos 0 y 1 están
reservados aunque no se usan, y el 3 corresponde al directorio.
En el bloque 3 está la lista de inodos. El inodo simplificado tiene la siguiente
estructura:

```c
/* inodo */
typedef struct {
 unsigned int size_fichero;
 unsigned short int i_nbloque[MAX_NUMS_BLOQUE_INODO];
} EXT_SIMPLE_INODE;
```

El inodo guarda el tamaño en bytes de información útil del fichero, si no está ocupado
el valor de ese campo es 0. Además hay una lista de hasta 7 números de bloques de
datos en los que se encuentra dicha carga útil. Si un fichero ocupa los bloques 5, 11 y
7, los elementos 0, 1 y 2 del array i_nbloque toman esos valores y el resto valen
FFFFH. Para recuperar el contenido hay que concatenar los bloques 5, 11 y 7 en ese
orden. La lista de inodos tiene 24 elementos.
En el bloque 3 está el directorio. Este sistema puede guardar hasta 20 ficheros. Para
cada fichero hay una entrada en la que figuran el nombre del fichero y el número de
inodo. La entrada 0 del directorio, contiene el nombre ‘.’ y el número de inodo 2. El
resto están vacías, y para ellas el valor de inodo es FFFFH.

```c
typedef struct {
 char dir_nfich[LEN_NFICH];
 unsigned short int dir_inodo;
} EXT_ENTRADA_DIR;
```

## Comandos soportados:

| Comando | Descripción |
| ------------- | ------------- |
| info  | Muestra la información del superbloque  |
| bytemaps  | Muestra el contenido del bytemap de inodos y los 25 primeros elementos del bytemap  |
| dir  | Lista todos los ficheros  |
| rename  | Cambia el nombre de un fichero en la entrada correspondiente  |
| imprimir  | Muestra el contenido del fichero especificado  |
| remove  | Elimina el fichero especificado  |
| copy  | Copia el fichero especificado en el primer parámetro y lo guarda en el fichero especificado en el segundo parámetro  |

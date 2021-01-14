#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre)
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
	if (strcmp(orden,"info\n")!=0) 
        if (strcmp(orden,"bytemaps\n")!=0) 
        if (strcmp(orden,"dir\n")!=0) 
        if (strcmp(orden,"rename")!=0)
        if (strcmp(orden,"imprimir")!=0)
        if (strcmp(orden,"remove")!=0)
        if (strcmp(orden,"copy")!=0)
        if (strcmp(orden,"salir\0")!=0)
	 printf("camando no valido\n");

	return 0;
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
	printf("Bloque %d Bytes\n", psup->s_block_size);
	printf("inodos particion = %d\n", psup->s_inodes_count);
	printf("inodos libres = %d\n", psup->s_free_inodes_count);
	printf("Bloques particion = %d\n", psup->s_blocks_count);
	printf("Bloques libres = %d\n", psup->s_free_blocks_count);
	printf("Primer bloque de datos = %d\n", psup->s_first_data_block);
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
	printf("Inodos : ");
	for(int i=0; i < 24; i++){
		printf("%d ",ext_bytemaps->bmap_inodos[i]);
	}
	printf("\nBloques [0-25] : ");
	for(int i=0; i < 25; i++){
		printf("%d ",ext_bytemaps->bmap_bloques[i]);
	}
	printf("\n");
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
	if(directorio->dir_nfich != ""){
		if(directorio->dir_nfich != "" && directorio->dir_inodo != 65535){
		 printf("%-25stamaño:%-20dinodo:%-10d", directorio->dir_nfich , inodos->blq_inodos[directorio->dir_inodo].size_fichero, directorio->dir_inodo);
		 printf("bloques: ");
		 for(int j= 0; j < sizeof(inodos->blq_inodos[directorio->dir_inodo].i_nbloque); j++){
		 	int bloque = inodos->blq_inodos[directorio->dir_inodo].i_nbloque[j];
		 	if(bloque != 65535 && bloque != 0){
		  		printf("%d ",inodos->blq_inodos[directorio->dir_inodo].i_nbloque[j]);
		  	}
		 }
		 printf("\n");
		}
	}
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *nombreantiguo, char *nombrenuevo){
	nombrenuevo[strlen(nombrenuevo)-1] = '\0';
	for(int i=1; i < sizeof(&directorio); i++){
	  if(strcmp(directorio[i].dir_nfich,nombrenuevo)==0){
	  	printf("ERROR: El fichero %s ya existe prueba con otro nombre \n",nombrenuevo);
	  	break;
	  }
	  printf("NOMBRE FICHERO: %s\n",directorio[i].dir_nfich);
	  printf("LOGITUD NOMBRE FIHWEO: %i\n",sizeof(directorio[i].dir_nfich));
	  printf("NOMBRE ANTIGUO: %s\n",nombreantiguo);
	  printf("LONGITUD NOMBRE ANTIGUO: %i\n",sizeof(nombreantiguo));
	  printf("NOMBRE NUEVO: %s\n",nombrenuevo);
	  if(strcmp(directorio[i].dir_nfich,nombreantiguo)==0){
	  	printf("ENTRA DENTRO DEL IF");
	  	strcpy(directorio[i].dir_nfich,nombrenuevo);
	  	break;
	  }else{
	  	printf("ERROR: el fichro %s no encontrado \n",nombreantiguo);
	  }
	}
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_DATOS *memdatos, char *nombre){
	nombre[strlen(nombre)-1] = '\0';
	int successfull = 0;
	for(int i=1; i < sizeof(&directorio); i++){
	 if(strcmp(directorio[i].dir_nfich,nombre)==0){
	 successfull = 1;
	 char insideTexto [2300];
	    for(int j= 0; j < sizeof(inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque); j++){
	        int bloque = inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j];
	        if(bloque != 65535 && bloque != 0){
	  		//printf("%d",inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]);
	  		printf("%s",memdatos[bloque].dato);
	        }
	    }
	    printf("\n");
	  }
	}
	if(successfull == 0){
	  	printf("Error: Fichero %s no encontrado\n",nombre);
	}
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,char *nombre,  FILE *fich){
	nombre[strlen(nombre)-1] = '\0';
	int successfull = 0;
	for(int i=1; i < sizeof(&directorio); i++){
	 if(strcmp(directorio[i].dir_nfich,nombre)==0){
	 	successfull = 1;
	 	strcpy(directorio[i].dir_nfich,"");
	 	for(int j= 0; j < sizeof(inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque); j++){
	        int bloque = inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j];
	        if(bloque != 65535 && bloque != 0){
	  		ext_bytemaps->bmap_bloques[bloque] = 0;
	  		inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j] = 65535;
	        	}
	    	}
	    	ext_bytemaps->bmap_inodos[directorio[i].dir_inodo] = 0;
	    	directorio[i].dir_inodo=65535;
	    
	 }
	}
}

int main()
{
	 char *comando[LONGITUD_COMANDO];
	 char *orden[LONGITUD_COMANDO];
	 char *argumento1[LONGITUD_COMANDO];
	 char *argumento2[LONGITUD_COMANDO];
	 
	 int i,j;
	 unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
     EXT_BYTE_MAPS ext_bytemaps;
     EXT_BLQ_INODOS ext_blq_inodos;
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     // Lectura del fichero completo de una sola vez
     ...
     
     fent = fopen("particion.bin","r+b");
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     
     // Buce de tratamiento de comandos
     for (;;){
	do {
	 printf (">> ");
	 fflush(stdin);
	 fgets(comando, LONGITUD_COMANDO, stdin);
	 char * separateData;
	 separateData = strtok(comando," ");
	 int contador = 0;
	 char * ordenArray [2] = {"empty","empty","empty"};
	 while(separateData != NULL){
	 	ordenArray[contador] = separateData;
	 	contador++;
	 	separateData = strtok(NULL," ");
	 }
	 orden = ordenArray[0];
	 argumento1 = ordenArray[1];
	 argumento2 = ordenArray[2];
	} while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
		
	if (strcmp(orden,"dir")==0 || strcmp(orden,"dir\n")==0) {
	 for(int i=1; i < sizeof(&directorio); i++){
	  Directorio(&directorio[i],&ext_blq_inodos);
	 }
	 continue;
	}
	
	if (strcmp(orden,"info")==0 || strcmp(orden,"info\n")==0) {
	 LeeSuperBloque(&ext_superblock);
	 continue;
	}
	
	if (strcmp(orden,"bytemaps")==0 || strcmp(orden,"bytemaps\n")==0) {
	 Printbytemaps(&ext_bytemaps);
	 continue;
	}
	
	if (strcmp(orden,"rename")==0) {
	 Renombrar(&directorio[i],&ext_blq_inodos,argumento1,argumento2);
	 continue;
	}
	
	if (strcmp(orden,"imprimir")==0) {
	 Imprimir(&directorio, &ext_blq_inodos,&datosfich,argumento1);
	 continue;
	}
	
	if (strcmp(orden,"remove")==0) {
	 Borrar(&directorio, &ext_blq_inodos,&ext_bytemaps,&ext_superblock,argumento1,&fent);
	 continue;
	}
	
	if (strcmp(orden,"copy")==0) {
	 Copiar(&directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,&datosfich,argumento1,argumento2,&fent);
	 continue;
	}
	if (strcmp(orden,"salir\n")==0){
         fclose(fent);
         return 0;
        }

     }
}

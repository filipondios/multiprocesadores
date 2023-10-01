/******************************************************************************
			getmem.h  - description
			----------------------
	begin		: Jan 2002
	copywirght	: (C) 2002 by L.G.C.
	
     ********************************************************************	
     Purpose: Rutines to get and free memory.
     
     Routines:
     	GetMem():   Get memory for a one dimensional vector. "Where" is a string
		which usualy contains the caller routine name to know where was
		the error.
			Use free((void *) p) for p=(type) GetMem (....)

		GetMem2D(): Get memory for a 2D array using GetMem.
					Allocated memory is not continous.

		Free2D(): Free memory allocated with GetMem2D.

		To Get 2D continous allocation we can use GetMem with N*M elements: 
			for (i=0; i < N; i++) {
  				for (j=0; j < M; j++) {
	   				2d_array[i*M +j]=...

		GetMem3D(): continous memory allocation of a 3D array.
					Allocated memory is not continous.
		Free3D(): Free memory allocated with GetMem3D.


*******************************************************************************/
#pragma once
#include <stddef.h>

/*  @brief Inicializa un array de punteros genericos (void*). 
 *  
 *  @Num numero de elementos del array.
 *  @Tam tamano en bytes del array. Calculado con sizeof().
 *  @Where string que describe para que se invoco la funcion.
 *
 *  @return puntero al array. 
 *  */
void* GetMem(size_t Num, size_t Tam, const char* Where);

/*  @brief Inicializa una matriz de punteros genericos (void*). 
 *  
 *  @rows numero de filas de la matriz.
 *  @columns numero de columnas de la matriz
 *  @sizeofTipo tamano en bytes de la matriz. Calculado con sizeof().
 *  @Where string que describe para que se invoco la funcion.
 *
 *  @return puntero al array. 
 *  */
void** GetMem2D(int rows, int columns, int sizeofTipo, char const* Where);

/*  @brief Libera la memoria alojada para una matriz de punteros
 *  genericos (void*). 
 *  
 *  @h matriz de punteros
 *  @rows numero de filas de la matriz.
 *  */
void Free2D(void** h, int rows);

/*  @brief Inicializa un cubo de punteros genericos (void*). 
 *  
 *  @nelem numero de elementos del cubo (profundidad)
 *  @rows numero de filas de la matriz.
 *  @columns numero de columnas de la matriz
 *  @sizeofTipo tamano en bytes de la matriz. Calculado con sizeof().
 *  @Where string que describe para que se invoco la funcion.
 *
 *  @return puntero al cubo. 
 *  */
void*** GetMem3D(int nelem, int rows, int columns, int sizeofTipo, char const * Where);

/*  @brief Libera la memoria alojada para un cubo de punteros
 *  genericos (void*). 
 *  
 *  @h cubo de punteros
 *  @nelem numero de elementos del cubo (profundidad)
 *  @rows numero de filas de la matriz. 
 *  */
void Free3D(void***  h, int nelem, int rows);

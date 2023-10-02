# Actividad: Bandera serie.

# A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : ``Daniel Pérez Valverde``
 * Nombre y apellidos alumno 2   : ``Anass El Jabiry Kaddir``
 * Mayoría en GTA1, GTA2 o GTA3  : ``GT3 y GT1 respectivamente``
 * Nombre de grupo de actividades: ``Grupo 58``

# Descripción de la arquitectura utilizada:
Ver la información del micro en Internet. Otra información se puede ver con los comandos:
  * $ free -h}
  * $ lscpu  (versión reducida de $less /proc/cpuinfo)
  * $ lstopo

A la BIOS se accede justo después de arrancar pulsando ESC, F1 o F2.

## Arquitectura: 
  * Microprocesador: ``AMD Ryzen 7 5800X 8-Core Processor``
  * Número de núcleos: ``8``
  * Cantidad de subprocesos por nucleo: ``2``
  * Tiene hyperthreading (SMT) activado en BIOS: ``No``
  * HDD/SDD: ``931Gb``
  * RAM: ``15Gb``
  * Se usa máquina virtual: ``No``

## Instrucciones:
 * El ejemplo muestra como generar una imagen RGB de la bandera de España.
 * Los parámetros de anchura y altura se introducen por la línea de comandos.
 * La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida, se usan tres matrices (R, G y B) para generar los colores RGB de cada pixel.

## Librerias
Se necesita tener instalados los siguientes paquetes:
 * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
 * eog para visualizar la imagen.
 * valgrind para ver las posibles perdidas de memoria.
 * perf para medir el rendimiento del programa.

## Objetivos
 * Practicar los conocimientos de C.
 * Familiarizar al alumno con 
    - El uso de varios archivos fuente. 
    - El uso de makefile y el comando make.
  * Uso de memoria dinámica. Ver getmem.h y getmem.c
  * Gestión de parámetros de entrada. Ver argshand.h y argshand.c
  * Uso de script de ejemplo para compilación y ejecución. Ver Run.sh
  * Ejemplo secuencial donde el trabajo computacional se sabe de antemano.
  * Observar que el visor de imágenes eog actualiza la salida si se cambia la imagen.
  * Chequear memory leaks.
  * Mejorar el rendimiento del programa

## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Bandera -h
```

## Ejemplo de compilacion y ejecución
 * En el script Run.sh

 * El script tiene dos ejecuciones. Para ejecutar solo una, hay que copiarla en el línea de comandos del shell. Normalmente se usa la línea de comandos, no el Run.sh. El Run.sh es para recordar los comandos a ejecutar y sus parámetros.

- - - 

# Entrega: 

## Parte 1: Verificar el Software.

Ejecutar en la consola:
```console 
$ make 
$ ./Bandera -r 300 -c 400 -o España
$ ./eog España.png
```
* Se debería ver la bandera de España.

Hay que evaluar si hay alguna perdida de memoria. 
* Ejecutar en la consola:
```console 
$ valgrind ./Bandera -r 300 -c 400
```
1. **Muestra aquí la salida de pantalla:**

<img src="https://github.com/dpv927/multiprocesadores/blob/main/Bandera/resources/memloss.png?raw=true">

2. **¿Hay perdida de memoria?**

Si, en concreto se han perdido 122.400 bytes por no liberar correctamente la memoria usada en el prgrama. 

4. **¿Qué has tenido que corregir en Bandera.c para que no haya perdida de memoria**. 
Se han corregido las ultimas lineas de Bandera.c, donde se liberaba la memoria de las matrices ppRed y ppGreen pero no se liberaba ppBlue:

```c
//Free allocated memory
Free2D((void**) ppGreen,Rows);
Free2D((void**) ppRed,  Rows);

// Liberar memoria de ppBlue:
Free2D((void**) ppBlue, Rows);
```

4. **Muestra aquí la salida de pantalla del comando valgrind sobre Bandera sin perdida de memoria.**
<img src="https://github.com/dpv927/multiprocesadores/blob/main/Bandera/resources/nomemloss.png?raw=true">

- - - 
## Parte 2: Mejorar el rendimiento.

**Copia Bandera.c en Bandera2.c y en vez de usar tres bucles dobles: dos para el rojo y uno para el amarillo; hay que usar un solo bucle doble para establecer el color de cada pixel.** 

**Nota**: El makefile está ya preparado para poder compilar  Bandera2 con `$ make Bandera2`. Hay que verificar que Bandera2 funciona correctamente viendo si genera bien la imagen. 

**Nota**: El amarillo se compone de rojo y verde. Todos los pixeles tienen rojo. Solo los de la banda central tienen verde.

### Comparar el rendimiento de Bandera y de Bandera2:

**Nota**: Hay que elegir un número de filas y columnas (Rows=Cols) múltiplo de 1024 que haga que el programa tarde varios segundos y que no consuma toda la RAM.

**Nota**: cada pixel necesita tres bytes, uno para cada color.

**Nota**: 1 kB =1024 bytes.

6. **¿Que valor de Rows=Cols has elegido? ¿Cuanta memoria ocupa la imagen?**

    Dimensión: 4096 x 4096
    
    Tamaño: 5985 KB

7. **Mostrar el menor tiempo real de varias ejecuciones de la salida de:**
```console 
& time ./Bandera -r Rows -c Cols
```

8. **¿Que algoritmo tiene menor tiempo de ejecución, Bandera o Bandera2?**

    Bandera2

**Ejecutar el comando:**
```console 
& perf stat -r 3 -d ./Bandera2 -r Rows -c Cols
```

9. **Mostrar los valores:** 
 * page-faults:u = 12.424      
 * L1-dcache-load-misses:u = 1.388
 * LLC-load-misses:u = 1.207.254

**Nota**: perf stat -r 3 -ddd permite obtener más detalles.

**Nota**: La memoria se ha cogido por filas, por lo que es más eficiente visitar las matrices por filas ya que los datos a modificar estarán más tiempo en las caches. 

10. **En Bandera2 cambia el orden del doble bucle, primero las columnas (j) y luego las filas (i). Muestra para esta versión los resultados solicitados en los puntos 7 y 9.**

    El tiempo es 7,606 segundos

  * page-faults:u = 12.425     
  * L1-dcache-load-misses:u = 18.222.200      
  * LLC-load-misses:u = 23.302.318

11. **¿Cuando se tienen más fallos de cache, visitando la matriz primero por filas o primero por columnas? ¿Porqué?** 

    Visitando primero por filas, por el resultado que hemos obtenido anteriormente da más fallos en caso de acceder primero por fila ([j][i]) que por columnas ([i][j]).

12. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

    Si, lo hemos hecho.

- - - 
### Como ver este .md en el navegador
En linux instalar grip:

```console 
$ pip install grip 
```

y ejecutar
```console
$ grip -b README.md
```

### Markdown cheat sheet

Para añadir información a este README.md:

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.

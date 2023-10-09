# Actividad Wa-tor serie:

## A rellenar por el alumno
 * Nombre y apellidos alumno 1   : ``Daniel Pérez Valverde``
 * Nombre y apellidos alumno 2   : ``Anass El Jabiry Kaddir``
 * Mayoría en GTA1, GTA2 o GTA3  : ``GT3 y GT1 respectivamente``
 * Nombre de grupo de actividades: ``Grupo 58``

## Descripción de la arquitectura utilizada: 
* Microprocesador: ``AMD Ryzen 7 5800X 8-Core Processor``
* Número de núcleos: ``8``
* Cantidad de subprocesos por nucleo: ``2``
* Tiene hyperthreading (SMT) activado en BIOS: ``No``
* HDD/SDD: ``931Gb``
* RAM: ``15Gb``
* Se usa máquina virtual: ``No``

## Instrucciones:
* El alumno debe realizar la rutina de iteración de un pez.
* Se puede basar en la función de iteración de un tiburón, que ya está realizada.
* Ambas funciones están en la librería ocean.h, ocean.c.
Buscar "**//TODO**" en los ficheros:
```console 
$ grep -n TODO *
```
* Además para la versión de producción hay que entregar dos ficheros:
    - **Profiling.txt** con la salida de **gprof**. 
    - **MemProf.txt** con la salida del uso de **valgrind**.
* Para el MemProf.txt la compilación en modo producción debe ser sin -g ni -pg.
* Para obtener datos razonables del % de tiempo de cada rutina en el profiling, el algoritmo debe tardar.  Se puede necesitar incrementar el número de iteraciones 
```console 
$ ./Wa-tor -ni 10000
```
* Las reglas de como funciona Wa-tor se encuentran más abajo.

## Librerias
 * El paquete **netpbm-progs** (o **netpbm**) para el comando del sistema rawtoppm.
 * El paquete **eog** para visualizar los ficheros .ppm.
 * El paquete **gnuplot** para generar las gráficas de evolución de peces y tiburones.
 * El paquete **gdb** para posibles debuggings.
 * El paquete **binutils** para el comando **gprof**.
 * El paquete **valgrind** para chequear posibles memoria asignada y no liberada.
 * El paquete **ffmpeg** o **ffmpeg-free** para generar video raw que se visualiza con ffplay

## Objetivos
 * Ejemplo secuencial donde el trabajo computacional no se sabe de antemano.
 * Además, existen conflictos:
    - Una celda (pixel) no puede contener dos animales.
    -  Dos tiburones no pueden comerse el mismo pez.
  * Es un código secuencial candidato a necesitar balanceo dinámico de la carga en su paralelización y resolución de conflictos o dependencia de datos.

## Compilación
```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Wa-tor -h
```

## Ejemplo de compilación y ejecuciones
* En el script Run.sh
* A Wa-tor, si se le indica salida gráfica **-o**, realiza una llamada a system para ejecutar **eog** (image viewer) de los movimientos de los animales en el océano. En preferencias de eog, quitar el "antialiasing" al escalar la imagen.
* A Wa-tor, si se le indica **-d**, usa la aplicación **gnuplot** que hay que instalar para visualizar la evolución de los peces y los tiburones que se graba en el fichero "datos.txt" por defecto.
* Wa-tor, si se le indica **-ffmpeg**, usa la aplicación **ffplay** que hay que instalar para visualizar la evolución de los peces en formato video raw.
* Antes de ejecutar Wa-tor hay que ver que no hay otra instancia corriendo:
```console
$ps aux  | grep Wa-tor
$ps aux  | grep eog
$ps aux  | grep gnouplot
$ps aux  | grep ffplay

```
de **Wa-tor** o en su caso **eog**, **gnuplot**, y/o **ffplay**. En tal caso hay que matar el proceso necesario con:
```command 
$ ps aux   (para ver los procesos y sus pid).
$ kill -9 <pid>
```

- - -

# Entrega :

1. **Indica los comandos que has realizado para hacer el profiling**.

Primero debemos activar el flag de profiling en el archivo makefile:
```bash
PROF=-pg
```
Despues solo tenemos que compilar el programa y generar el archivo de profiling Profiling.txt:
```bash
# compilar
make clean && make

# Ejecutar para obtener gmon.out
./Wa-tor -ni 10000

# Generar archivo de profiling
gprof ./Wa-tor gmon.out > Profiling.txt
```

3. **En el fichero Profiling.txt con la salida del gprof del programa, ¿Que rutinas consumen más tiempo?**
   
Los resultados obtenidos son los siguientes:
```
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  us/call  us/call  name    
 29.35      0.39     0.39    10000    39.03   132.60  IterateOcean
 23.33      0.70     0.31 26011238     0.01     0.02  IterateFish
 18.44      0.95     0.25 29006343     0.01     0.01  FreeNeighbours
 15.05      1.15     0.20 12037975     0.02     0.03  IterateShark
 10.53      1.29     0.14  7897346     0.02     0.02  FishNeighbours
  1.50      1.31     0.02  4387302     0.00     0.00  SwapInt
  1.50      1.33     0.02  2347149     0.01     0.01  NewAnimal
  0.38      1.33     0.01                             OceanToRGBFile
  0.00      1.33     0.00  2347250     0.00     0.00  GetMem
  0.00      1.33     0.00       15     0.00     0.00  ExistArg
  0.00      1.33     0.00        1     0.00     0.00  Free2D
  0.00      1.33     0.00        1     0.00     0.00  FreeOcean
  0.00      1.33     0.00        1     0.00     0.00  GetArg
  0.00      1.33     0.00        1     0.00     0.00  GetMem2D
  0.00      1.33     0.00        1     0.00    25.58  InitOcean
```
Con ello, podemos confirmar que las rutinaas que consumen maas tiempo del programa son IterateOcean, IterateFish y FreeNeighbours con bastante diferencia respecto de las demas. 

4. **Indica los comandos que has realizado para hacer el chequeo de perdida de memoria.**
   
De igual manera que hemos activado el flag -pg en el makefile, ahora debemos desactivarlo;
```bash
#PROF=-pg
```

Ahora debemos compilar de nuevo el programa y ejecutar valgrind para obtener el archivo MemProf.txt:
```bash
# compilar
make clean && make

# generar el archivo
valgrind --log-file="MemProf.txt" ./Wa-tor -ni 10000
```

6. **En el fichero Memprof.txt que has generado con la salida de valgrind, ¿Existen perdidas de memoria?**

Mo, no existen perdidas de memoria, tal y como se puede observar:
```
==2753== Memcheck, a memory error detector
==2753== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==2753== Using Valgrind-3.21.0 and LibVEX; rerun with -h for copyright info
==2753== Command: ./Wa-tor -ni 10000
==2753== Parent PID: 1028
==2753==
==2753==
==2753== HEAP SUMMARY:
==2753==     in use at exit: 0 bytes in 0 blocks
==2753==   total heap usage: 2,464,856 allocs, 2,464,856 frees, 39,517,888 bytes allocated
==2753==
==2753== All heap blocks were freed -- no leaks are possible
==2753==
==2753== For lists of detected and suppressed errors, rerun with: -s
==2753== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

8. **¿Existen ejecuciones en las que no se llegan al número establecido de iteraciones? ¿Porqué?**

Por ejemplo, si observamos una simulación que ha realizado 100,023 iteraciones máximas, solo se han conseguido 25,032. Esto puede ocurrir cuando quedan pocos peces y muchos tiburones, lo que provoca que los peces no tengan tiempo para reproducirse antes de que un tiburón se los coma.
```
~/Desktop/multiprocesadores/Wa-tor # ./Wa-tor -ni 100023
Wa-tor ends. Niter=25032, NFishes= 0, NSharks=641.
```

10.  **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

Si, todos los archivos innecesarios han sido borrados.

- - -

# Actividad Wa-tor: reglas.
* Tomadas de [wiki](https://en.wikipedia.org/wiki/Wa-Tor)

## Movimento
* En cada iteración un pez o tiburón puede moverse norte sur este u oeste a un pixel adyacente, si no está ocupado por un miembro de su misma especie.

### Peces
* Si los 4 pixeles adyacentes están ocupados (por peces o tiburones), no se mueve.
* Se selecciona el pixel a moverse aleatoriamente entre los libres.

### Tiburón
* Se elige entre los adyacentes ocupados por peces, uno aleatoriamente y se come el pez.
* Si no hay peces adyacentes, se mueve como el pez, evitando otros tiburones.

## Vida y reproducción:

### Pez
* Si el pez ha sobrevivido un número de iteraciones se reproduce. Esto ocurre cuando además hay un pixel adyacente libre en el cual se crea un nuevo pez. Ambos, padre e hijo ponen sus contadores de iteraciones a 0.

### Tiburon
* En cada iteración se le decrementa en 1 la energía.
* Al alcanzar la energía 0 el tiburón muere.
* Al comerse un pescado, el tiburón gana una cantidad de energía.
* Si el tiburón alcanza un número de iteraciones, se reproduce como el pez. El hijo coje la energia de creación de tiburón. El padre mantiene la suya.

- - - 
### Como ver este .md 
En linux instalar grip:
```console 
$ pip install grip 
```
y ejecutar
```console
$ grip -b README.md
```
- - -
### Markdown cheat sheet
Para añadir información a este README.md:
[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -
&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.

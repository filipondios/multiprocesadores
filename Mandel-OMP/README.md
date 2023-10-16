# Ejercicio: Mandel OMP

# A rellenar por el alumno/grupo
## Nombre y apellidos alumno 1   : < .... >
## Nombre y apellidos alumno 2   : < .... >
## Nombre y apellidos alumno 3   : < .... >
## Mayoría en GTA1, GTA2 o GTA3  : < .... >
## Nombre de grupo de actividades: < .... >

# Descripción de la arquitectura utilizada:
## Arquitectura: 
  * Microprocesador:
  * Número de núcleos:
  * Cantidad de subprocesos por núcleo:
  * Tiene hyperthreading (SMT) activado en BIOS:
  * HDD/SDD: 
  * RAM:
  * Se usa máquina virtual:
    - Número de cores:
    - RAM: 
    - Capacidad HDD: 

# Instrucciones

El ejemplo muestra como generar una imagen en color del fractal de Mandelbrot.

> Información del fractal de Mandelbrot en la [wiki](https://es.wikipedia.org/wiki/Conjunto_de_Mandelbrot). 
>
> Mandelbrot set [Rosetta code](https://rosettacode.org/wiki/Mandelbrot_set#C)
>
> Mandelbrot set  [Techniques for computer generated pictures](https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set) 

El fractal de Mandelbrot es en blanco y negro aunque se pueden generar tonos de gris o colores dependiendo del número de iteraciones "NIter" que se realicen sobre cada punto (pixel). El número de iteraciones en cada pixel es normalmente diferente porlo que se necesita balanceo dinámico e la carga para obtener buenos speed-ups.

Se proporciona el código que genera el fractal de Mandelbrot en color usando la rutina NIterToRGB(). 

Se pedirá al alumno que *decore* el programa con directivas OpenMP para obtener una versión paralela (gcc -fopenmp). Debe poder compilarse también en serie (gcc) sin warnnings. 

Los parámetros se introducen por la línea de comandos.

La memoria se asigna de forma dinámica. 
Aunque se podría escribir directamente en el fichero de salida, se usan:
  *  Tres matrices (R, G y B) para generar los colores RGB de cada pixel.


### A tener en cuenta:
  * En el Run.sh se encuentran las distintas ejecuciones paralelas con -mi 1e4 y -mi 1e5, y con distinto número de hebras, no la secuencial.
  * Cuando se miden tiempos hay que quitar el PRINT=1 y poner PRINT=0 ya que la salida por pantalla consume mucho tiempo.
  * Si se usa portátil, medir tiempos con el portátil enchufado a la corriente ya que si no os cores reducen su rendimiento.
  * Si se miden tiempos hay que compilar sin el -g ni el -pg.
  * No se realizan dos ficheros con el código secuencial y el paralelo. Es el mismo fichero para la versión secuencial y paralela, compilando o no con -fopenmp y haciendo uso de #ifdef _OPENMP y/o #ifndef _OPENMP.
  * Al compilar no debe haber warnings.
  * Hay que responder a las preguntas y argumentar los resultados.
  * Para el calculo de la ganancia de velocidad real (speed-up) Sp(p) se usará el tiempo real devuelto por $time Mandel-OMP .....
  * En el programa secuencial ( compilado sin -fopenmp) ya se mide el tiempo del código secualcial aparalelizar. El tiempo total del programa, serie o paralelo, se obtiene con la parte read de ```$ time .Mandel-OMP ...```. 
  * El parámetro -mi indica el número máximo de iteraciones a realizar en un pixel. Habrá pixeles en los que se llegue a ese máximo y otros en los que no. Por eso el trabajo en cada pixel no es estático, es variable. Esto requerirá de algún tipo de balanceo de la carga entre hebras. 

## Librerias
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.
  * Los comandos OpenMP no están en las man. Hay que instalarlos. Ver como en [OpenMP-man-pages](https://github.com/Shashank-Shet/OpenMP-man-pages).

## Objetivos
 * El uso de OpenMP sobre bucles for y el uso de distintos *schedule* y como establecer el *chunk*.
 * Cómo medir el tiempo consumido de CPU y wall clock time.
 * Optimizar algoritmos paralelos en OMP para problemas donde la cantidad de trabajo computacional a realizar por cada tarea no es el mismo.

## Compilación

```console 
$ make 
```

## Ayuda parámetros 
```console
$ ./Mandel -h
```

## Ejemplo de compilacion y ejecución
	En el script Run.sh

- - -

# Entrega:

## Speed-up Teórico:

1. **Rellena la siguiente tabla para la versión secuencial.**
 * Memoria: Es la memoria consumida por ppRed, ppGreen y ppBlue y otros datos usados.
 * T.Sec: El wall-clock time del programa secuencial. Parte real del ```$ time Mandel-OMP ...```.
 * T.CsPar: El tiempo del código secuencial que será paralelizado. 
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.
 * Se adjunta una hoja de cálculo para calcular SpA() y Sp(). Hay que rellenar los campos T.Sec, T.CsPar, T(p) y p. Se visualiza ejecutando:

```console 
$ oocalc Speed-up.ods
```

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
| Memoria(Gb) |                 |                 |
|T.Sec        |                 |                 |
|T.CsPar      |                 |                 |
|SpA(2)       |                 |                 |
|SpA(4)       |                 |                 |


2. **¿Porqué ahora la salida gráfica no es tan influyente en SpA() como en la actividad Bandera-OMP?** 
 * Contesta teniendo en cuenta los porcentajes de código paralelizado y no paralelizado.


## Speed-up real Sp(p): 

La opción **-o** genera la imagen, que debe ser correcta al visualizarse con el comando *eog* al usar hebras.

3. **Describe que realiza el schedule(static) y qué chunk usa por defecto.**

4. **Rellena la siguiente tabla. Para la parte paralela se usará schedule(static) sin establecer el chunk.** 

 * T(p): Wall clock time del programa paralelo con p hebras.
 * Sp(p): ganancia en velocidad con p hebras.

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 | 

5. **¿Hay diferencias entre T.Sec y T(1)?**
 * Ver el número de hebras se usan realmente en T(1). Hay más recursos? 

6. **¿Porqué no mejora sustancialmente el Sp(4) respecto al Sp(2)?**
 * Ver la imagen generada. A cuantas hebras le ha tocado casi todo el trabajo para p=2 y p=4? 

7. **Describe que realiza el schedule(dynamic) y que chunk usa por defecto.**

8. **Rellena la siguiente tabla. Para la parte paralela se usará schedule(dynamic) sin establecer el chunk.**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 | 


9. **Describe que realiza el schedule(guided) y que chunk usa por defecto.**


10. **Rellena la siguiente tabla. Para la parte paralela se usará schedule(guided) sin establecer el chunk.**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 | 


11. **¿Qué scheduler se comporta mejor para -mi 1e5? ¿Porqué?**

12. **¿Cual es el mejor chunk para el scheduler dynamic y -mi 1e5?**
* Hay que hacer una búsqueda dicotómica del mejor chunk.
* nIter= número de iteraciones totales a paralelizar.
* Se obtiene T(p).max con el chunk max = máximo chunk = nIter/p
* Se obtiene T(p).min con el chunk min = mínimo chunk = 1.
* Repetir
    - Se obtiene T(p).med con el chunk med = (max - min)/2
    - Si T(p).min < T(p).max
       + max=med
    - Si no min=med
* hasta que T(p).min sea similar a T(p).max

13. **Rellena la siguiente tabla para el scheduler y chunk del punto anterior.**

| Ejecución   | -mi 1e5         |
| ----------- | --------------- |
|Chuck p=2    |                 |
|Chunk p=4    |                 | 
|T(2)         |                 |
|T(4)         |                 | 
|Sp(2)        |                 |
|Sp(4)        |                 |

14. **Explica el porqué de los resultados usando chunk y sin usarlo.**

15. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**



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

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto. Informática, UAL.

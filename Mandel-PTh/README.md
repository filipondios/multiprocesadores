# Ejercicio: Mandel PTh.

## A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : Daniel Pérez Valverde
 * Nombre y apellidos alumno 2   : Anass El Jabiry Kaddir
 * Mayoría en GTA1, GTA2 o GTA3  : GT3 y GT1 respectivamente
 * Nombre de grupo de actividades: Grupo 58

 ## Arquitectura: 
  * Microprocesador: AMD Ryzen 7 5800X 8-Core Processor
  * Número de núcleos: 8
  * Cantidad de subprocesos por nucleo: 2
  * Tiene hyperthreading (SMT) activado en BIOS: No
  * HDD/SDD: 931Gb
  * RAM: 15Gb
  * Se usa máquina virtual: No

## Instrucciones:

* El Mandel.c muestra como generar una imagen en color del fractal de Mandelbrot.
> Información del fractal de Mandelbrot en la [wiki](https://es.wikipedia.org/wiki/Conjunto_de_Mandelbrot). 
>
> Mandelbrot set [Rosetta code](https://rosettacode.org/wiki/Mandelbrot_set#C)
>
> Mandelbrot set  [Techniques for computer generated pictures](https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set) 
* El fractal de Mandelbrot es en blanco y negro aunque se pueden generar tonos de gris o colores dependiendo del número de iteraciones "NIter" que se realicen sobre cada punto (pixel).
* Se proporciona el código que genera el fractal de mandelbrot en color usando la rutina NIterToRGB(). 
* Los parámetros se introducen por la línea de comandos.
* La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida. Se usan:
    +  Tres matrices (R, G y B) para generar los colores RGB de cada pixel.
 * En el Run.sh se tienen los comandos a ejecutar para -mi 1e4 y -mi 1e5 para el algoritmo secuencial (Mandel) y el paralelo (Mandel-PTh) con distinto número de hebras.
    - La versión secuencial se usará para obtener T.Sec y T.CsPar.
    - La versión paralela con p hebras se usará para obtener T(p).
 * En la versión paralela se ha añadido las opciones:
    + -cs < chunck size > por defecto 1.
    + -nt < número de hebras > 
 * La opción **-o** genera la imagen. La imagen se debe usar para verificar que el código es correcto, al visualizarse la imagen con el comando *eog*.
 * Se requiere completar el Mandel-PTh. Ver **//TODO** en el código.
 * Las hebras en Mandel-PTh trabajan por filas completas. Cada hebra trabajará en un número ChunkSize de filas indicado en la opción -cs (chunk size), que por defecto es de una fila cada vez.
 * De Mandel OMP sabemos que una distribución estática, donde cada hebra realiza Rows/NThreads filas, no es lo más eficiente debido a la distinta carga computacional por pixel y por tanto también por filas. Por otro lado, con -cs 1, donde cada hebra realiza una fila cada vez, las hebras tienen más probabilidad de esperar en el mutex para saber en qué fila comienzan la nueva ejecución, que si se usaran valores mayores de ChunkSize.
 * Cuando se midan tiempos PRINT=0 y DEGUD=0.

## Librerías
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.

## Objetivos
 * Familiarizar al alumno con 
	- El uso de PThreads.
 * Usar balanceo de la carga variando tamaño de chunck para problemas con diferente carga computacional por iteración.
 * Familiarizarse con el uso de exclusión mutua al modificar variables compartidas entre hebras (siguiente fila a computar).
  * Cómo medir el tiempo consumido de CPU, Wall-clock-time y el speed-up.

## Compilación

```console 
$ make -j
$ make -j Mandel
$ make -j Mandel-PTh
$ make -j all
```

## Ayuda parámetros 
```console
$ ./Mandel -h
$ ./Mandel-PTh -h
```

## Ejemplo de compilacion y ejecución
	En el script Run.sh

- - -

# Entrega:

## Speed-up teórico

1. **Rellena la siguiente tabla para la versión secuencial, Mandel.c.**


| Ejecución   | -mi 1e4         | -mi 1e5           |
| ----------- | --------------- | ----------------- |
|T.Sec        |                 |                   |
|T.CsPar      |                 |                   |
|SpA(2)       |                 |                   |
|SpA(4)       |                 |                   |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Mandel ... 
 * T.CsPar: El tiempo de la parte del código secuencial que será paralelizado: doble bucle.
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.



## Speed-up real Sp(p): 
 * Para las siguientes tablas, todo se mide para el algoritmo paralelo Mandel-PTh.c. T.Sec es el obtenido en el punto 1.
 * T(p): Wall clock time del programa paralelo con p hebras.
 * Sp(p): ganancia en velocidad con p hebras.

2. **Con -cs 1. Es el valor por defecto.**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |                 |                 | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 |

3. **Con -cs Rows/NThreads. Distribución estática.**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |                 |                 | 
|Chuck p=2    |                 |                 |
|Chunk p=4    |                 |                 |
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 | 


4. **Con el mejor valor encontrado para -cs.**
* Hay que hacer una búsqueda dicotómica del mejor chunk.
* Rows= número de filas totales a paralelizar.
* Se obtiene T(p).max con el chunk max = máximo chunk = Rows/p
* Se obtiene T(p).min con el chunk min = mínimo chunk = 1.
* Repetir
    - Se obtiene T(p).med con el chunk med = (max - min)/2
    - Si T(p).min < T(p).max
       + max=med
    - Si no min=med
* hasta que T(p).min sea similar a T(p).max

| Ejecución   | -mi 1e5         |
| ----------- | --------------- | 
|T.Sec        |                 | 
|Chuck p=2    |                 |
|Chunk p=4    |                 |
|T(2)         |                 | 
|T(4)         |                 | 
|Sp(2)        |                 | 
|Sp(4)        |                 | 


5. **Compara el mejor Sp(4) para -mi 1e5 de Mandel-OMP con schedule dynamic y el mejor chunk encontrado con Mandel-PTh del punto 4.**
 + **¿Cual es mejor?**
 + **¿Son los chunks OMP y PTh distintos (indica sus valores)?**

6. **Indica al número de filas que realiza cada hebra para una ejecución con p=4 de la tabla en el punto 4.**
    - **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Porqué?**
    - **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Porqué?**


7. **En vez de solo por filas, se podría haber paralelizado por pixeles. Teóricamente, ¿sería más efectivo? ¿Cuando y porqué?** 
* Para ello, el pixel donde empezar se numera de 0 a (Rows * Cols)-1. Para pasar de número de pixel a posición [i][[j] se puede usar la rutina VectorToMatrixInd() que se encuentra en el fichero Indexes-Vector-Matrix.c, y este en Rutines-PTh.tgz, que está en el aula virtual de la asignatura. Solo habría que usar VectorToMatrixInd() para el primer pixel del chunk, ya que los demás se pueden saber teniendo en cuenta Rows, Cols y el ChunkSize.


8. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**


- - -
### Cómo ver este .md 
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

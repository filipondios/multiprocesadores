# Ejercicio: Mandelbrot MPI.

## A rellenar por el alumno/grupo
 * Nombre y apellidos alumno 1   : 
 * Nombre y apellidos alumno 2   : 
 * Nombre y apellidos alumno 3   : 
 * Mayoría en GTA1, GTA2 o GTA3  : 
 * Nombre de grupo de actividades: 

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

## Instrucciones:

* En el Run.sh se tienen los comandos a ejecutar para -mi 1e4 y -mi 1e5 para el algoritmo secuencial (Mandel) y el paralelo (Mandel-MPI) con distinto número de tareas.
* La opción **-o** genera la imagen. La imagen se puede usar para verificar que el código es correcto, al visualizarse con el comando *eog*.

## Librerías
Se necesita tener instalados los siguientes paquetes:
  * netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
  * eog para visualizar la imagen.

## Objetivos
 * Familiarizar al alumno con 
	- El uso de la librería MPI.
 	- Uso de una estrategia maestro-trabajador donde la tarea maestra (0) reparte trabajo entre las tareas trabajadoras (>0). Este esquema se usa para realizar balanceo dinámico de la carga, cuando la carga computacional por unidad a calcular no es homogénea. Usar Scatter y Gather no sería lo más eficiente para este tipo de cómputo.
 	- En la Opción 2: cómo chequear sin bloqueo si hay o no mensajes para la tarea maestra, de forma que si no los hay la tarea maestra también haga trabajo. 

## Compilación

```console 
$ make -j
$ make -j Mandel
$ make -j Mandel-MPI
$ make -j all
```

## Ayuda parámetros 
```console
$ ./Mandel -h
$ ./Mandel-MPI -h
```

## Ejemplo de compilacion y ejecución
	En el script Run.sh

- - -

# Entrega:


## Speed-up teórico

1. **Rellena la siguiente tabla para la versión secuencial, Mandel.c.**


| Ejecución   | -mi 1e4         | -mi 1e5           |
| ----------- | --------------- | ----------------- |
|T.Sec        | 4,995s          | 50,226s           |
|T.CsPar      | 4.98883s        | 50,2186s          |
|SpA(2)       | 1,99753         | 1,999705          |
|SpA(4)       | 3,98523         | 3,99823           |

donde
 * T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Mandel ... 
 * T.CsPar: El tiempo de la parte del código secuencial que será paralelizado: doble bucle.
 * SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.



**Se tiene que entregar una de las siguientes opciones**:

## Opción 1: La tarea maestra (0) no realiza cómputo.
 * Es la opción que para la que está preparado el código. Es la más fácil, pero no se obtiene la máxima nota.
 * Se requiere completar el Mandel-MPI. Ver **//TODO** en el código. Solo se requieren modificaciones en las comunicaciones de las tareas trabajadoras (>0) que deben casar con las que realiza la tarea maestra (0).
 ```console
 $ grep -n TODO Mandel-MPI.c
 ```
* Las tareas trabajadoras en Mandel-MPI trabajan por filas completas y en una fila cada vez. Cada fila tiene tres vectores de `char` para RGB.
* Las tareas trabajadoras terminan cuando reciben un número de fila a procesar igual al número de filas. Las filas  van de 0 a Rows-1.


2. **Rellene la siguiente tabla para Mandel-MPI.c:**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        | 4,995s          | 49,914s         | 
|T(1)         | -               | -               | 
|T(2)         | 5,390s          | 50,535s         | 
|T(4)         | 2,027s          | 17,568s         | 
|Sp(1)        | -               | -               | 
|Sp(2)        | 0,92671614100185| 0,98771148708815| 
|Sp(4)        | 2,46423285643809| 2,84118852459016|

3. **Indica al número de filas que realiza cada hebra para una ejecución con p=4 de la tabla anterior.**
    - **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Por qué?**
    - **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Por qué?**
| Tarea   	| -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|Tarea 0        | 0 filas          | 0 flas         |
|Tarea 1        | 341 filas          | 328 filas         |
|Tarea 2        | 343 filas          | 373 filas         |
|Tarea 3        | 340 filas          | 323 filas         |

 Si difieren, porque la tarea 0 solo reparte el trabajo y lo unifica, mientras que las demás son las que hacen el trabajo. Estas filas difieren entre ellas debido a que hay mas iteraciones y el trabajo por pixel sera mas costoso en algunos casos. 

4. **¿Porqué el tiempo paralelo con dos tareas (-np 2) es siempre peor que el secuencial?**
   Debido a que la tarea principal reparte el trabajo, mientras que la otra tarea hace los calculos. Ademas, hay que sumar el coste de enviar y recibir los datos a procesar y procesados.
 
6. **¿Tiene ventajas su paralelización?**
Si, solamente si el numero de tareas es mayor que dos, por la razon explicada en el apartado anterior.

8. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**
Si, los archivos innecesarios han sido borrados.

## Opción 2: La tarea maestra también realiza cómputo.
* Se necesita cambiar más el código, pero se opta a la máxima nota.
* La tarea maestra (0) debe chequear de forma no bloqueante si le ha llegado un mensaje de una tarea trabajadora. 
    + Si le ha llegado un mensaje, debe recoger los resultados y enviarle la nueva fila a procesar, tal como está actualmente el código. 
    + **Si no le ha llegado un mensaje** (MPI_IProbe) la hebra maestra realiza el cómputo de la fila actual en esta iteración del bucle.

7. **Rellene la siguiente tabla para Mandel-MPI.c:**

| Ejecución   | -mi 1e4         | -mi 1e5         |
| ----------- | --------------- | --------------- | 
|T.Sec        |                 |                 | 
|T(1)         |                 |                 | 
|T(2)         |                 |                 | 
|T(4)         |                 |                 | 
|Sp(1)        |                 |                 | 
|Sp(2)        |                 |                 | 
|Sp(4)        |                 |                 |

8. **Indica al número de filas que realiza cada hebra para una ejecución con p=2,4 de la tabla anterior.**
    - **¿Difieren los números de filas realizadas por cada hebra de una ejecución a otra? ¿Porqué?**
    - **¿Es el número de filas realizado por las hebras de una ejecución similar? ¿Porqué?**


9. **¿Tiene ventajas su paralelización?**

10. **¿Cuando sería mejor que la hebra maestra haga trabajo?**
    - Cuando el número de tareas trabajadoras es bajo, o
    - Cuando el número de tareas trabajadoras el alto.

11. **¿Has hecho un *make clean* y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**

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

### Markdown cheat sheet

Para añadir información a este README.md:

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

- - -

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.

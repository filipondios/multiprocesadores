# Ejercicio: Bandera OMP.

# A rellenar por el alumno/grupo

## Nombre y apellidos alumno 1 : < .... >

## Nombre y apellidos alumno 2 : < .... >

## Nombre y apellidos alumno 3 : < .... >

## Mayoría en GTA1, GTA2 o GTA3 : < .... >

## Nombre de grupo de actividades: < .... >

# Descripción de la arquitectura utilizada:

## Arquitectura:

- Microprocesador:
- Número de núcleos:
- Cantidad de subprocesos por nucleo:
- Tiene hyperthreading (SMT) activado en BIOS:
- HDD/SDD:
- RAM:
- Se usa máquina virtual:
  - Número de cores:
  - RAM:
  - Capacidad HDD:

## Instrucciones:

El ejemplo muestra como generar una imagen RGB de la bandera de España.

Los parámetros de anchura y altura se introducen por la línea de comandos.

La memoria se asigna de forma dinámica. Aunque se podría escribir directamente en el fichero de salida, se usan tres matrices (R, G y B) para generar los colores RGB de cada pixel.

Se pedirá al alumno que _decore_ el programa con directivas OpenMP de forma que los colores ppRed, ppGren y ppBlue de cada pixel puedan calcularse en paralelo con varias hebras.

El programa podrá ejecutarse en

- secuencial al compilarse con gcc y
- en paralelo, al compilarse con gcc -fopenmp y distinto número de hebras.

En el makefile ya se ha puesto el flag OMP.

Como tamaño de la bandera, se usarán los indicados en las tablas de la entrega. Se puede cambiar el Run.sh para todas las versiones paralelas.

Hay que comparar los tiempos obtenidos con distinto número de hebras y con la versión secuencial.

Según los [consejos/trucos/gotchas](https://www.archer.ac.uk/training/course-material/2017/08/openmp-ox/) de EPCC:

- En las regiones paralelas
  - usad siempre **default(none)** y usad **private**, **share**, ... para las variables.
  - Es mejor que las variables sean locales a la región paralela.

### A tener en cuenta:

- En el Run.sh hay que poner todas las ejecuciones con y sin salida gráfica.
- Cuando se miden tiempos hay que quitar el PRINT=1 y poner PRINT=0, ya que la salida por pantalla consume mucho tiempo.
- Si se usa portátil, hay medir tiempos con el portátil enchufado a la corriente ya que si no los cores reducen su rendimiento.
- Si se miden tiempos hay que compilar sin el -g ni el -pg.
- Hay que tener en cuanta la opción -o España. Es decir con y sin salida gráfica.
  - $Bandera-OMP -r 200 -c 300 -o España (genera España.png)
  - $Bandera-OMP -r 200 -c 300 (no genera España.png)
- No se realizan dos ficheros fuentes con el código secuencial y el paralelo. Es el mismo fichero para la versión secuencial y paralela haciendo uso de #ifdef \_OPENMP e #ifndef \_OPENMP.
- Al compilar no debe haber warnings.
- La memoria consumida dependerá del tamaño de la imagen y el tipo de datos a almacenar por pixel. Hay que calcularla.
- Hay que responder a las preguntas y argumentar los resultados.

## Librerias

Se necesita tener instalados los siguientes paquetes:

- netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
- eog para visualizar la imagen.
- Normalmente, los comandos OpenMP no están en las man. Hay que instalarlos. Ver como en [OpenMP-man-pages](https://github.com/Shashank-Shet/OpenMP-man-pages).

## Objetivos

- Familiarizar al alumno con
  - El uso de OpenMP sobre bucles for.
- Como medir el tiempo consumido.
  - Para el tiempo total se usará la parte real del comando `$ time programa < parámetros > ` en la consola.
  - Para el tiempo consumido de alguna parte del programa:
    - En secuencial: gettimeoftheday().
    - En paralelo omp_get_wtime(). Hay un ejemplo en OpenMP/solution/pi/pi2.c al descomprimir OpenMPsingle.tar.gz.

---

## Compilación

```console
$ make
```

## Ayuda parámetros

```console
$ ./Bandera-OMP -h
```

## Ejemplo de compilacion, establecer parámetros y ejecución

- En el script Run.sh
- Ver las variables de entorno para OpenMP.

---

# Entrega:

## Speed-up Teórico:

1. **Describe la formula del speed-up o ganancia de velocidad teórica usando la ley de Amdahl (SpA), en términos del porcentaje del código secuencial a paralelizar y del porcentaje del código secuencial que no se va ha paralelizar.**

- Para calcular SpA(p) solo se miden tiempos del programa secuencial.
- SpA(p)= 1/(%T.CsPar/p+%T.CsnPar)
- p = número de elementos de proceso.
- Se miden en secuencial dentro del código (con gettimeofday()):
  - T.CsPar: Tiempo del código secuencial a paralelizar.
  - T.CsnPar: Tiempo del código secuencial que no se va ha paralelizar.
- Se miden con la parte real de `$ time programa < parámetros >  `
  - T.Sec: Tiempo total del programa secuencial.
- T.CsnPar = T.Sec - T.CsPar.
- %T.CsPar = T.CsPar/T.Sec.
- %T.CsnPar = T.CsnPar/T.Sec.

```
T.Sec = 1.160s
T.CsPar = 0.72663s

T.CsnPar = T.Sec - T.CsPar = 1.160 - 0.72663 = 0.43337
%T.CsPar = T.CsPar/T.Sec = 0.72663 / 1.160 = 0.626405172
%T.CsnPar = T.CsnPar/T.Sec = 0.43337 / 1.160 = 0.373594828

SpA(p)= 1/(%T.CsPar/p+%T.CsnPar) = 1/(0.626405172/p+0.373594828)
```

2. **¿Cuanto es el valor de SpA(p) si p=1?**

```
   SpA(p)= 1/(%T.CsPar/p+%T.CsnPar) = 1/(0.626405172/1+0.373594828) = 1
```

3. **¿Cuanto es el valor de SpA(p) si todo el código no es paralelizable?**

```
   SpA(p)= 1/(%T.CsPar/p+%T.CsnPar) = 1/(0/p+1) = 1
```

4. **¿Cuanto es el valor de SpA(p) si se puede paralelizar todo el código?**

- Hay que elegir un número de filas y columnas (Rows=Col) múltiplo de 1024 (n\*1204, n>1) que haga que el programa secuencial tarde varios segundos y que no consuma toda la RAM.
- Cada pixel necesita tres bytes, uno para cada color.
- 1 kB =1024 bytes.

```
    SpA(p)= 1/(%T.CsPar/p+%T.CsnPar) = 1/(1/p+0) = p
```

5. **¿Que valor de Rows=Col has elegido? ¿Cuanta memoria (Mem.) ocupa la imagen?**

```
   El valor que hemos elegido es 11264. La imagen ocupa 26KB. Como utilizamos el color rojo solo para dibujar la mitad de la imagen en total y luego combinamos el rojo y el verde para formar el amarillo para dibujar en la mitad. La formula para saber la memoria que ocupa es:
   Rojo: 11264 x (11264 / 2) x 2 = 12687769 bytes = 123904KB
   Amarillo: (11264 x (11264 / 2)) x 2 = 123904KB
   En total seria de 247808KB
```

6. **Rellena la siguiente tabla para la versión secuencial sin salida gráfica.**

- Hay que compilar sin -fopenmp (en el makefile). \_OPENMP no estará definido.
- Ejemplo de ejecución

```console
$ time Bandera-OMP -r Rows -c Cols
```

T.CsPar:
El tiempo del código secuencial que será paralelizado: relleno de las matrices ppRed, ppGreen y ppBlue. Ya está en el código la medición del tiempo secuencial que se tarda en la parte del código a paralelizar (bucles que establecen los colores de la bandera) de forma interna en el programa secuencial.

- Se adjunta una hoja de cálculo para calcular SpA() y Sp(). Hay que rellenar los campos T.Sec, T.CsPar, T(p) y p. Se visualiza ejecutando:

```console
$ oocalc Speed-up.ods
```

| Ejecución | -r 1024 -r 1024 | -r Rows -c Cols |
| --------- | --------------- | --------------- |
| Mem.      | 2048KB          | 354,52 MiB      |
| T.Sec     | 0.013s          | 1,16s           |
| T.CsPar   | 0.005556s       | 0,72663s        |
| SpA(2)    | 1,2717667775    | 1,4560334386    |
| SpA(4)    | 1,4717536511    | 1,8860945242    |

## Speed-up real Sp(p):

7. **Describe la formula de la ganancia en velocidad o speed-up real, describiendo los términos utilizados.**

   La fórmula para la ganancia en velocidad (Speedup Real) se utiliza para cuantificar el rendimiento de un programa o proceso cuando se ejecuta en un sistema paralelo en comparación con su ejecución en un sistema secuencial. La fórmula se expresa de la siguiente manera:
   $$SR = \frac{T.Sec}{T(p)} $$

- Donde:
  - T(p) : Tiempo total del programa paralelo con p elementos de proceso.

8. **Describe qué realiza el schedule(static) y qué chunk usa por defecto.**

   Esta cláusula se utiliza en bucles paralelos para especificar cómo se deben asignar las iteraciones del bucle a los hilos (threads) en un equipo paralelo.

   Por defecto, cuando se utiliza schedule(static), el "chunk size" o tamaño del fragmento se calcula automáticamente por OpenMP para lograr una distribución equitativa de las iteraciones entre los hilos.

   Por ejemplo, si tienes un bucle con 100 iteraciones y estás utilizando 4 hilos con schedule(static), OpenMP dividirá las iteraciones de manera equitativa, de modo que cada hilo se encargará de 25 iteraciones.

9. **Rellena la siguiente tabla para la versión paralela. Se usará schedule(static) sin establecer el chunk.**

- Incluir el código OMP (decorar el código). Ver **//TODO**.
- Hay que compilar con -fopenmp (en el makefile). \_OPENMP estará definido.
- Hay que establecer la variable de entorno con el número de hebras. Por ejemplo, para p=4 sería:

```console
$ export OMP_NUM_THREADS=4
```

- Al compilarse con -fopenmp, no se mide internamente el tiempo del código secuencial a paralelizar. Solo se mide si \_OPENMP no está definido.

| Ejecución | -r 1024 -r 1024 | -r Rows -c Cols |
| --------- | --------------- | --------------- |
| T(2)      | 0.011s          | 0.761s          |
| T(4)      | 0.010s          | 0.661s          |
| Sp(2)     | 1.4             | 1.9531          |
| Sp(4)     | 1.2727          | 1.6964          |

10. **¿Es el SpA(p) distinto del SP(p)? ¿Por qué?**

```
La fórmula de la ganancia en velocidad (Speed-Up Real) se utiliza para evaluar el rendimiento real de un programa o sistema paralelo después de su implementación. Mientras que la Ley de Amdahl se utiliza para establecer límites teóricos, el Speed-Up Real permite calcular el rendimiento real en un entorno específico.
Por lo que si son distintos.
```

11. **Teóricamente, ¿Mejoraría el Sp() si se establece el tamaño del chunk en el schedule(static,chunk)? ¿Y si se usa otro scheduler?**

    Tamaño del Chunk en `schedule(static, chunk)`:

- _Chunk Pequeño_: Si el tamaño del chunk es demasiado pequeño, puede generar overhead en la asignación de tareas a los hilos, lo que afecta negativamente el rendimiento.

- _Chunk Grande_: Un tamaño de chunk demasiado grande puede resultar en una utilización desigual de recursos, ya que algunos hilos pueden terminar antes que otros. Esto también afecta negativamente el rendimiento.

  El tamaño óptimo del chunk depende de la naturaleza de las tareas y la arquitectura del hardware. Se recomienda realizar pruebas con diferentes tamaños de chunk para determinar la configuración más eficiente.

  Otro Scheduler:

  En OpenMP, tienes opciones adicionales a `schedule(static, chunk`, como `schedule(dynamic)`, `schedule(guided)`, y `schedule(auto)`. La elección del scheduler depende de cómo se generen y distribuyan las tareas paralelas.

  - `schedule(dynamic)`: Útil si las tareas no tienen una carga de trabajo uniforme y se asignan dinámicamente a los hilos.

  - `schedule(guided)`: Una opción intermedia efectiva cuando algunas tareas requieren más tiempo que otras.

  - `schedule(auto)`: Permite al compilador determinar el mejor plan de programación.

  La elección del scheduler y el tamaño del chunk puede afectar el rendimiento. Es importante realizar pruebas y mediciones específicas para determinar la configuración óptima en tu aplicación y hardware. Ajustar estos parámetros es esencial para obtener el mejor rendimiento.

12. **¿Qué hace el collapse(2) en la directiva OMP?**  
    La directiva `collapse` en OpenMP se utiliza para combinar múltiples bucles anidados en uno solo, lo que puede ser beneficioso para la paralelización de código. El número entre paréntesis, como `collapse(2)`, indica cuántos niveles de bucles anidados se deben colapsar o combinar en un solo bucle. En el caso de `collapse(2)`, se están combinando dos niveles de bucles anidados en un solo bucle.

13. **Rellena la siguiente tabla para la versión paralela con schedule(static) y collapse(2)**

| Ejecución | -r 1024 -r 1024 | -r Rows -c Cols |
| --------- | --------------- | --------------- |
| T(2)      | 0.010s          | 0.784s          |
| T(4)      | 0.008s          | 0.646s          |
| Sp(2)     | 1,2             | 1,4744          |
| Sp(4)     | 1,5             | 1,7894          |

**14. ¿Mejora el Sp(p)? ¿Porqué?**

- Incluye y compara la salida de los rendimientos ( `$ perf stat -r 3 -ddd programa < parámetros >` usando o no collapse (2).

## Experimentos con salida gráfica (-o España):

15. **Rellena la siguiente tabla para la ejecución secuencial con salida gráfica.**

- Hay que compilar sin -fopenmp (en el makefile). \_OPENMP no estará definido.
- Ejemplo de ejecución (Rows=Cols se establecieron en el punto 5):

```console
$ time Bandera-OMP -r Rows -c Cols -o España
```

| Ejecución | -r 1024 -r 1024 | -r Rows -c Cols |
| --------- | --------------- | --------------- |
| Mem.      | 2048KB          | 247808KB        |
| T.Sec     | 0.299s          | 30.679s         |
| T.CsPar   | 0.005442s       | 0.777256s       |
| SpA(2)    | 1,0091839111    | 1,0128300834    |
| SpA(4)    | 1,0138394167    | 1,0193693805    |

16. **Rellena la siguiente tabla para la ejecución paralela con salida gráfica, con schedure(static) y sin collapse().**

| Ejecución | -r 1024 -r 1024 | -r Rows -c Cols |
| --------- | --------------- | --------------- |
| T(2)      | 0.294s          | 29.838s         |
| T(4)      | 0.285s          | 29.318s         |
| Sp(2)     | 1,0170068027    | 1,0281855352    |
| Sp(4)     | 1,049122807     | 1,0464219933    |

16. **¿Porqué ahora el SpA() y Sp(p) son peores que en la tablas 6 y 9?**

- Responde usando como argumentos los porcentajes de código paralelizable y no paralelizable.

  El rendimiento de SpA() y Sp(p) puede verse afectado por los porcentajes de código paralelizable y no paralelizable, y esto podría explicar por qué son peores en comparación con las tablas 6 y 9. Aquí está la explicación:

  SpA() (Speedup Potencial):

  - Porcentaje de Código Paralelizable (P): Si el porcentaje de código que se puede paralelizar (P) es menor en comparación con las situaciones de las tablas 6 y 9, la Ley de Amdahl sugiere que el speedup potencial (SpA()) será limitado por esta fracción no paralelizable. En otras palabras, si hay menos código que se puede ejecutar en paralelo, SpA() será más bajo.

  - Porcentaje de Código No Paralelizable (1 - P): Si el porcentaje de código no paralelizable (1 - P) es alto, esto reducirá el speedup potencial, ya que la Ley de Amdahl establece que SpA() se ve limitado por la fracción no paralelizable. En comparación con las tablas 6 y 9, un mayor porcentaje de código no paralelizable podría reducir SpA().

  Sp(p) (Speedup Real)

  - Overheads de Paralelización: En la práctica, la ejecución paralela a menudo conlleva overheads asociados con la creación, administración y sincronización de hilos. Si estos overheads son significativos en la implementación real, pueden reducir Sp(p) en comparación con el SpA() teórico.

  - División de Carga: Si la carga de trabajo no está equilibrada de manera uniforme entre los hilos o procesadores, algunos procesadores podrían completar su trabajo antes que otros, lo que resulta en una menor eficiencia y un Sp(p) más bajo.

  En resumen, la diferencia entre las situaciones actuales y las tablas 6 y 9 podría deberse a una combinación de un menor porcentaje de código paralelizable (P) y un mayor porcentaje de código no paralelizable (1 - P) en el programa, así como a los efectos prácticos de overheads de paralelización y posibles desequilibrios en la carga de trabajo entre los hilos o procesadores. Estos factores podrían llevar a un SpA() y Sp(p) más bajos en las condiciones actuales en comparación con las tablas 6 y 9.

17. **¿Porque SpA(p) y Sp(p) no mejoran sustancialmente al aumentar el tamaño de la imagen?**

El hecho de que SpA(p) y Sp(p) no mejoren sustancialmente al aumentar el tamaño de la imagen puede deberse a varias razones:

1. Límite de Paralelización: Es posible que el algoritmo que estás paralelizando tenga un límite en la cantidad de trabajo que se puede paralelizar de manera efectiva. Si la gran mayoría del tiempo se gasta en cálculos que no pueden ser paralelizados, el aumento del tamaño de la imagen no cambiará este hecho fundamental.

2. Carga de Trabajo Desigual: Si la carga de trabajo no está equilibrada de manera uniforme entre los hilos o procesadores, algunos procesadores pueden terminar antes que otros, lo que resulta en una utilización ineficiente de los recursos paralelos. Esto puede limitar la mejora del rendimiento a medida que aumenta el tamaño de la imagen.

3. Overheads de Paralelización: La creación y sincronización de hilos en paralelización conlleva overheads que pueden afectar el rendimiento. A medida que aumenta el tamaño de la imagen, estos overheads pueden convertirse en una proporción más pequeña del tiempo total, lo que limita la mejora proporcional del rendimiento.

4. **¿Has hecho un _make clean_ y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**  
   Se hizo.

---

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

---

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.

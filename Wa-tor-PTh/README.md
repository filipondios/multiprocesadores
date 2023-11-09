# Ejercicio: Wa-tor PTh.

## A rellenar por el alumno/grupo

- Nombre y apellidos alumno 1 : Daniel Pérez Valverde
- Nombre y apellidos alumno 2 : Anass El Jabiry Kaddir
- Mayoría en GTA1, GTA2 o GTA3 : GT3 y GT1 respectivamente
- Nombre de grupo de actividades: Grupo 58

## Arquitectura:

- Microprocesador: AMD Ryzen 7 5800X 8-Core Processor
- Número de núcleos: 8
- Cantidad de subprocesos por nucleo: 2
- Tiene hyperthreading (SMT) activado en BIOS: No
- HDD/SDD: 931Gb
- RAM: 15Gb
- Se usa máquina virtual: No

# Instrucciones:

- Se pide realizar los **//TODO** del código.

```console
$ grep -n TODO *
```

- En el Run.sh se tienen los comandos a ejecutar, (-r 201 -c 201) y (-r 300 -c 300) con -ni 10000 y distinto número de hebras.
  **El número de iteraciones al final de la ejecución debe ser 10000**, es decir, deben quedar peces y tiburones después de la ejecución para obtener tiempos.
- No se usaran las salidas gráficas o de datos de gnuplot para medir los tiempos. Solo se usarán para chequear que el programa paralelo funciona como el secuencial.
- Para evitar algunas secciones criticas, se ha forzado que el tamaño del océano sea múltiplo de 3 en filas y columnas y se ha optado por la opción de tener tres bucles con i=0,1,2; i < rows; i=i+3 (y su respectivo bucle j para las columnas completo). **Cada hebra realiza una fila a la vez**.
- Las hebras tienen un barrier antes de empezar cada doble bucle (i,j) para que antes se ponga la _RowToProcess_ a 0,1,o 2. Para 0 lo hace el main, y para 1 y 2 lo hace la hebra 0.
- Se ejecutan el main y -nt hebras.
- Nótese que las variables locales de la función de cada hebra son independientes en cada hebra.
- Se usa srand(0) y lrand48() para inicializar el océano por una sola hebra.
- Se usa srand48_r(i,&pRandData[i]) y lrand48_r(&pRandData[i],&Res) para el movimiento de peces y tiburones, así como la comida de tiburones.
- Se entrega la versión secuencial (Wa-tor.c) y la paralela a modificar (Wa-tor-PTh.c).

## Librerias

- El paquete **netpbm-progs** (o **netpbm**) para el comando del sistema rawtoppm.
- El paquete **eog** para visualizar los ficheros .ppm.
- El paquete **gnuplot** para generar las gráficas de evolución de peces y tiburones.
- El paquete **valgrind** para chequear posibles memoria asignada y no liberada.
- El paquete **ffmpeg** para generar video raw que se visualiza con ffplay

## Objetivos

- Familiarizar al alumno con
  - El uso de PThreads.
  - Trabajar con problemas dinámicos donde hay dependencia de datos.
  - Evitar secciones críticas con el desenrollado de bucles.
  - Usar mutexes y barriers.
  - Uso de secuencias de números aleatorios independientes por hebra.

## Compilación

```console
$ make -j
$ make -j all
$ make -j Wa-tor
$ make -j wa-tor-PTh
```

## Ayuda parámetros

```console
$ ./Wa-tor -h
$ ./Wa-tor-PTh -h
```

## Ejemplo de compilacion y ejecución

    En el script Run.sh

Wa-tor realiza una llamada a system para ejecutar **eog** (image viewer) si se le indica salida gráfica **-o** de los movimientos de los animales en el océano.

Wa-tor tambien usa el paquete **gnuplot** que hay que instalar si se le indica **-d** para general un fichero con número de peces y tiburones por iteración y mostrarlos como líneas en gratifico.

Wa-tor tambien usa el paquete **ffmpeg** que hay que instalar si se le indica el parámetro -ffmpeg para que la salida por el pipe generado la coja el comando ffmplay, generando un vídeo sin interrupciones del movimiento en el océano.

Antes de ejecutar Wa-tor hay que ver que no hay otra instancia corriendo:

```console
$ps aux  | grep Wa-tor
$ps aux  | grep eog
$ps aux  | grep gnouplot
$ps aux  | grep ffmplay

```

de **Wa-tor** o en su caso **eog**, **gnuplot**, y/o **ffmplay**. En tal caso matad el proceso necesario con:

```command
$ kill -9 <pid>
```

---

# Entrega :

1. **¿Tiene el algoritmo paralelo finalizado perdidas de memoria (memory leaks)?**

- Mostrar la salida de

```console
$ valgrind -s ./Wa-tor-PTh -ni 102 -nt 4
```

Si hay perdida de memoria es que dos animales se han movido a la misma celda y no está bien el código. Muestra aquí el ERROR SUMMARY.

<img src="./resources/img1.png">

No hay perdida de memoria. En "still reacheable" sale distinto de 0 porque se utilizan bibliotecas compartidas que quedan guardadas en memoria.

2. **Rellena la siguiente tabla:**

- T.Sec es el tiempo de ejecución del algoritmo secencial.
- T(p) es el tiempo de ejecución del algoritmo paralelo con -nt p hebras.

| Ejecución | 201x201 | 300x300 |
| --------- | ------- | ------- |
| T.Sec     | 6,145s  | 14,241s |
| T(1)      | 6,647s  | 15,103s |
| T(2)      | 4,247s  | 8,962s  |
| T(4)      | 2,490s  | 5,217s  |

3. **¿Tiene ventajas su paralelización?**  
   Si, como se puede observar en la tabla anterior, incluso la version con un unico hilo de Wa-tor-PTh
   es mas rapida que la version secuencial Wa-tor.

4. **¿Se podrían usar menos número de mutexes o barriers? ¿Por qué?**  
   Si se podria, por ejemplo, podriamos eliminar los barriers "Loop_end", de forma que los hilos solo esperarian
   en los barriers "Loop_start". No obstante, los resultados no son mejores que los de la version original.

<img src="./resources/img2.png">

6. **¿Se te ocurre un método más eficiente de paralelizar Wa-tor?**  
   Como se ha comentado en el apartado anterior, se nos ocurre una manera de eliminar mutexes o barriers
   pero los resultados obtenidos no son optimos.

7. **¿Has hecho un _make clean_ y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**  
   Si, todos los archivos innecesarios han sido eliminados.

---

# Actividad Wa-tor: reglas.

Tomadas de [wiki](https://en.wikipedia.org/wiki/Wa-Tor)

## Movimento

- En cada iteración un pez o tiburón puede moverse norte sur este u oeste a un pixel adyacente, si no está ocupado por un miembro de su misma especie.

### Peces

- Si los 4 pixeles adyacentes están ocupados (por peces o tiburones), no se mueve.
- Se selecciona el pixel a moverse aleatoriamente entre los libres.

### Tiburón

- Se elige entre los adyacentes ocupados por peces, uno aleatoriamente y se come el pez.
- Si no hay peces adyacentes, se mueve como el pez, evitando otros tiburones.

## Vida y reproducción:

### Pez

Si el pez ha sobrevivido un número de iteraciones se reproduce. Esto ocurre cuando además hay un pixel adyacente libre en el cual se crea un nuevo pez. Ambos, padre e hijo ponen sus contadores de iteraciones a 0.

### Tiburón

- En cada iteración se le decrementa en 1 la energía.
- Al alcanzar la energía 0 el tiburón muere.
- Al comerse un pescado, el tiburón gana una cantidad de energía.
- Si el tiburón alcanza un número de iteraciones, se reproduce como el pez. El hijo coge la energía de creación de tiburón. El padre mantiene la suya.

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

[Markdown cheat sheet](https://www.markdownguide.org/cheat-sheet/)

---

&copy; [Leocadio González Casado](https://sites.google.com/ual.es/leo). Dpto, Informática, UAL.

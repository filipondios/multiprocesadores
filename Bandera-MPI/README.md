# Ejercicio: Bandera MPI.

## A rellenar por el alumno/grupo

- Nombre y apellidos alumno 1 :
- Nombre y apellidos alumno 2 :
- Nombre y apellidos alumno 3 :
- Mayoría en GTA1, GTA2 o GTA3 :
- Nombre de grupo de actividades:

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

# Instrucciones:

- Ya se asignan pixeles a las tareas, de forma que:
  - Hacen un trabajo similar
  - Ningún pixel debe quedarse sin hacer.
  - Que un pixel no se haga más de una vez.
- El makefile y el Run.sh ya se encuentran preparados para las ejecuciones secuencial y paralelas.
  - El valor **2048** hay que cambiarlo para que el secuencial dure al menos varios segundos sin consumir la RAM.
- La salida gráfica se usará solo para verificar que se está haciendo correctamente el proceso. No se usará para medir tiempos.
- La memoria consumida dependerá del tamaño de la imagen y el tipo de datos a almacenar. Hay que calcularlo. La memoria consumida no es la misma en la tarea 0 que en las demás tareas.
- Debido a que MPI envía secuencias de datos contiguas y la imagen es 2D, se han cogido vectores de tamaño ROWs\*COLs para los colores RGB.
- Se hace una distribución estática, donde cada tarea (incluyendo la 0) calcula la posición y el número de pixeles que debe procesar.
- Cada tarea calcula los valores R y G de los pixeles que le han tocado y los manda a la tarea 0 que los recoge y opcionalmente los escribe en un fichero para que se pueda visualizar la bandera.
- Busca que hacer en los TODO. Hay que realizarlos **todos**:

```console
$ grep TODO *
```

## Librerias

Se necesita tener instalados los siguientes paquetes:

- netpbm-progs (o netpbm) para los comandos del sistema rawtoppm y rawtopng.
- eog para visualizar la imagen.

## Objetivos

- Familiarizar al alumno con
  - El uso de la librería MPI.
- Usar MPI_Probe cuando no se sabe de quien se recibe el mensaje.
- Usar MPI_Get_count cuando no se sabe el tamaño que hay que poner en el receive.
- Entender la memoria consumida por las tareas.

### A tener en cuenta:

- Para saber el funcionamiento de las funciones de la librería, una vez cargada con (en Ubuntu no hay que cargarla):

```console
$ module load < libreria mpi >
```

hágase

```console
$ man <rutina>
```

Por ejemplo,

```console
$ man MPI_Probe
$ man MPI_Scatterv
```

## Compilación

```console
$ make Bandera
$ make Bandera-MPI
$ make all
```

## Ayuda parámetros

```console
$ ,/Bandera -h
$ ./Bandera-MPI -h
```

## Ejemplo de compilacion, establecer parámetros y ejecución

- En el script Run.sh
- El valor 2048 habrá que cambiarlo.

# Entrega

**Se tiene que entregar una de las siguientes opciones**:

### Opcion 1: Realizar los //TODO:

- En el ejemplo hay que realizar lo que se encuentra en **//TODO**
- Es la opción que para la que está preparado el código. Es la más fácil, pero no se obtiene la máxima nota.

### Opcion 2: Usar MPI_Scatterv y MPI_Gatherv

- Se usan las versiones "v" porque a cada tarea le pueden corresponder un número de pixeles distinto ya que el número de pixeles puede no ser divisible por el número de tareas. Puede forzarse a que el uso de las versiones v (Scatterv) o (Gatherv) no sea necesario, en tal caso razonar la solución propuesta y se tiene menos nota.

## Speed-up teórico

- Al igual que en ejercicios anteriores, hay que elegir un número de filas y columnas (Rows=Col) múltiplo de 1024 (n\*1204, n>1) que haga que el programa secuencial (Bandera.c) tarde varios segundos y que no consuma toda la RAM. Se puede usar el mismo valor que se usó en los ejercicios anteriores, si no se ha cambiado la arquitectura.
- Para la **Opción 2** hay que usar un valor que no sea divisible entre el número de hebras. Habrá que sumarle algo a Rows=Cols del punto anterior para que no sea múltiplo de 2.

1. **¿Que valor de Rows=Col has elegido? ¿Cuanta memoria (Mem.) ocupa la imagen?**  
   Hemos elegido 12288 y la memoria que ocupa la imagen es de 0.15099494 GB.

2. **Usa la versión secuencial _Bandera.c_ para rellenar la siguiente tabla.**

- Ejemplo de ejecución sin salida gráfica:

```console
$ time ./Bandera -r <Rows> -c <Cols>
```

| Ejecución | -r 1024 -c 1024  | -r Rows -c Cols  |
| --------- | ---------------- | ---------------- |
| T.Sec     | 0.013s           | 1.235s           |
| T.CsPar   | 0.006412s        | 0.814239s        |
| SpA(2)    | 1.32734327139065 | 1.49176119017177 |
| SpA(4)    | 1.58710780124527 | 1.97814985326052 |

donde

- T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del $time Bandera ...
- T.CsPar: El tiempo del código secuencial que será paralelizado: relleno de las matrices ppRed, ppGreen y ppBlue.
- SpA(p): El spedd-up **teórico** según la ley de Amhdal para p hebras paralelas.

## Resultados del algoritmo paralelo

3. **¿Qué opción se ha seleccionado?**  
   La opción 1.
4. **Usa la versión paralela _Bandera-MPI.c_ para rellenar la siguiente tabla.**

- Ejemplo de ejecución sin salida gráfica:

```console
$ time mpirun -np <p> ./Bandera-MPI -r <Rows> -c <Cols>
```

| Ejecución       | -r 768 -c 1024     | -r Rows -c Cols  |
| --------------- | ------------------ | ---------------- |
| T.Sec           | 0.010s             | 1.235s           |
| T(1)            | 0.464s             | 1.758s           |
| Mem. Task=0(Gb) | 0.00209716         | 0.30198988       |
| --------------- | --------------     | ---------------  |
| T(2)            | 0.371s             | 1.161s           |
| Mem. Task>0(Gb) | 0.00104858         | 0.15099494       |
| --------------- | --------------     | ---------------  |
| T(4)            | 0.390s             | 0.853s           |
| Mem. Task>0(Gb) | 0.00052428         | 0.07549748       |
| --------------- | --------------     | ---------------  |
| Sp(1)           | 0.021551724137931  | 1                |
| Sp(2)           | 0.0269541778975741 | 1.06373815676141 |
| Sp(4)           | 0.0256410256410256 | 1.44783118405627 |s

Donde

- T.Sec: El wall-clock time (tiempo total) del programa secuencial. Parte real del `$ time ./Bandera ...`
- T(p): El tiempo total (parte real de la salida `$ time ./Bandera-MPI ...`) del algoritmo paralelo con p tareas.
- Mem. Task=0(Gb): Memoria consumida por la tarea cero.
- Mem. Task>0(Gb): Memoria consumida por las tareas distintas de la cero.
- Sp(p): Speed-up real con p tareas paralelas.

5. **¿Es Sp(p)> SpA(p)? ¿Cuales pueden ser los motivos?**  
   No es el caso, los motivos podrían ser estos:

- Overhead de Comunicación y Sincronización: En programas paralelos, especialmente aquellos que utilizan MPI, la comunicación y la sincronización entre los procesos pueden introducir overhead. Si este overhead es significativo, puede afectar negativamente el speedup obtenido.
- Carga Desbalanceada: Si la carga de trabajo no está equilibrada entre los diferentes procesos, algunos pueden terminar antes y deben esperar a los demás, lo que afecta el speedup.
- Dimensiones del Problema: El tamaño del problema también puede influir. Para tamaños pequeños de entrada, el overhead de la paralelización puede superar los beneficios.

6. **Explica como has calculado las cantidades de memoria consumidas por la tarea 0 y las otras tareas.**  
   Se calculo sabiendo la division de pixeles que fue asignada a cada tarea y conociendo tambien que cada pixel es 1 byte.
7. **¿Has hecho un _make clean_ y borrado todas los ficheros innecesarios (imágenes, etc) para la entrega antes de comprimir?**  
   Hecho.

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

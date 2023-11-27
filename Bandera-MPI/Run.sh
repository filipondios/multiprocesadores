#!/bin/bash

# echo "Cambiar 2048 por valor para que el secuencial dure varios segundos"  

make Bandera

echo "-----------------------------------------"
echo "Secuencial"
echo "-----------------------------------------"
time ./Bandera -r 1024  -c 1024
time ./Bandera -r 12288 -c 12288 


make

echo "-----------------------------------------"
echo "MPI -r 768 -c 1024 "
echo "-----------------------------------------"
time mpirun -np 1 ./Bandera-MPI -r 1024 -c 1024 
time mpirun -np 2 ./Bandera-MPI -r 1024 -c 1024 
time mpirun -np 4 ./Bandera-MPI -r 1024 -c 1024 

echo "-----------------------------------------"
echo "MPI -r 12288 -c 12288"
echo "-----------------------------------------"
time mpirun -np 1 ./Bandera-MPI -r 12288 -c 12288 
time mpirun -np 2 ./Bandera-MPI -r 12288 -c 12288 
time mpirun -np 4 ./Bandera-MPI -r 12288 -c 12288 


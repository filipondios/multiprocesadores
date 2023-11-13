make MultiVector
make MultiVector-OMP
make MultiVector-PTh

echo "-----------------------------------------"
echo "MultiVector -r 10000"
echo "-----------------------------------------"
time ./MultiVector -r 10000

echo "-----------------------------------------"
echo "MultiVector-OMP -r 10000 1 Thread"
echo "-----------------------------------------"
export OMP_NUM_THREADS=1
time ./MultiVector-OMP -r 10000

echo "-----------------------------------------"
echo "MultiVector-OMP -r 10000 2 Threads"
echo "-----------------------------------------"
export OMP_NUM_THREADS=2
time ./MultiVector-OMP -r 10000

echo "-----------------------------------------"
echo "MultiVector-OMP -r 10000 4 Threads"
echo "-----------------------------------------"
export OMP_NUM_THREADS=4
time ./MultiVector-OMP -r 10000

echo "-----------------------------------------"
echo "MultiVector-PTh -r 10000 1 Thread"
echo "-----------------------------------------"
time ./MultiVector-PTh -r 10000 -nt 1

echo "-----------------------------------------"
echo "MultiVector-PTh -r 10000 2 Threads"
echo "-----------------------------------------"
time ./MultiVector-PTh -r 10000 -nt 2

echo "-----------------------------------------"
echo "MultiVector-PTh -r 10000 4 Threads"
echo "-----------------------------------------"
time ./MultiVector-PTh -r 10000 -nt 4
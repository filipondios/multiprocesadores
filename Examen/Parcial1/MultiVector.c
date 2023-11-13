#include <stdio.h>
#include <stdlib.h>
#include "argshand.h"
#include "getmem.h"
#include "utils.h"
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define PRINT 0

void *InicializarVector(int *pVector1, int *pVector2, int Rows, struct drand48_data *pRandData)
{
    for (int i = 0; i < Rows; i++)
    {
#ifdef _OPENMP
        int idthread = omp_get_thread_num();
        long rand = 0;
        lrand48_r(&pRandData[idthread], &rand);
        pVector1[i] = rand % 10;
        lrand48_r(&pRandData[idthread], &rand);
        pVector2[i] = rand % 10;
#endif

#ifndef _OPENMP
        pVector1[i] = lrand48() % 10;
        pVector2[i] = lrand48() % 10;
#endif
    }

    return NULL;
}

void *MultiplicarVector(int pVector1[], int pVector2[], int *pVector3, int Rows)
{
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(pVector1, pVector2, pVector3, Rows) schedule(static)
#endif
    for (int i = 0; i < Rows; i++)
    {
        pVector3[i] = pVector1[i] * pVector2[i];
    }

    return NULL;
}

void ParametersError()
{
    puts("Options are:");
    puts("\t[ -h To show this help ]");
    puts("\t -r <n rows>");

    exit(0);
}

int main(int argc, char **argv)
{
    int Rows;
    int *pVector1, *pVector2, *pVector3;
    // Vector of data for drand48_r for each thread
    struct drand48_data *pRandData = NULL;

    // Inicializamos numeros aleatorios
    srand48(time(NULL));

    if (ExistArg("-h", argc, argv))
        ParametersError();

    if (!ExistArg("-r", argc, argv))
    {
        fputs("Parameter -r is neccesary.\n", stderr);
        ParametersError();
    }
    else
    {
        Rows = atoi(GetArg("-r", argc, argv));
        if (Rows < 1)
        {
            puts("Rows < 1");
            exit(1);
        }
    }

    if (PRINT == 1)
    {
        printf("Rows=%d\n", Rows);
    }

#ifdef _OPENMP
#pragma omp parallel default(none)
    {
#pragma omp single
        {
            printf("------------\n");
            printf("OpenMP-parallel with %1d threads\n", omp_get_num_threads());
            printf("------------\n");
        }
    } /* end omp parallel */
#pragma omp barrier
#endif

    // Getmem to arrays
    pVector1 = (int *)GetMem(Rows, sizeof(int), "Main:PVector1");
    pVector2 = (int *)GetMem(Rows, sizeof(int), "Main:pVector2");
    pVector3 = (int *)GetMem(Rows, sizeof(int), "Main:pVector3");

    // Init pRandData if OPENMP define
#ifdef _OPENMP
    // TODO Get the number of threads (nthreads)
    int nthreads = omp_get_max_threads();
    // TODO Get memory for pRandData, with size nthreads. Possible False Sharing.
    pRandData = (struct drand48_data *)GetMem(nthreads, sizeof(struct drand48_data), "Main: pRandData");
    // TODO: init seed for each  drand48_data using srand48_r(i,...), i=0..nthreads-1. See man pages.
    for (int i = 0; i < nthreads; i++)
    {
        srand48_r(i, &pRandData[i]);
    }
#endif

    // Init arrays
    InicializarVector(pVector1, pVector2, Rows, pRandData);

    // Multiply arrays
    MultiplicarVector(pVector1, pVector2, pVector3, Rows);

#if (PRINT == 1)
    // Show arrays
    printf("pVector1:\t");
    for (int i = 0; i < Rows; i++)
    {
        printf("%d\t", pVector1[i]);
    }

    printf("\n");

    printf("pVector2:\t");
    for (int i = 0; i < Rows; i++)
    {
        printf("%d\t", pVector2[i]);
    }

    printf("\nResult:\t\t");

    for (int i = 0; i < Rows; i++)
    {
        printf("%d\t", pVector3[i]);
    }

    printf("\n");
#endif

    free((void *)pVector1);
    free((void *)pVector2);
    free((void *)pVector3);

#ifdef _OPENMP
    free((void *)pRandData);
#endif

    return 0;
}

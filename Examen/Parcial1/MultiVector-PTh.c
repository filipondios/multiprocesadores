#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "argshand.h"
#include "getmem.h"
#include "utils.h"
#include "01-DivideVector.h"

#define PRINT 0

struct Th_Args
{
    int ThreadId;
    int inicio;
    int fin;
    int Rows;
    int *pVector1;
    int *pVector2;
    int *pVector3;
    STDrand48Data *pRandData;
};

void *InicializarVector(int *pVector1, int *pVector2, int Rows, STDrand48Data *pRandData)
{
    long RandRes;
    for (int i = 0; i < Rows; i++)
    {
        lrand48_r(pRandData, &RandRes);
        pVector1[i] = RandRes % 10;
        lrand48_r(pRandData, &RandRes);
        pVector2[i] = RandRes % 10;
    }

    return NULL;
}

void *MultiplicarVector(void *pTh_Args)
{
    struct Th_Args *myData = (struct Th_Args *)pTh_Args;
    for (int i = myData->inicio; i <= myData->fin; i++)
    {
        myData->pVector3[i] = myData->pVector1[i] * myData->pVector2[i];
    }

    pthread_exit((void *)&(myData->ThreadId));
}

void ParametersError()
{
    puts("Options are:");
    puts("\t[ -h To show this help ]");
    puts("\t -r <n rows>");
    puts("\t  -nt <Numbre of Threads>");

    exit(0);
}

int main(int argc, char **argv)
{
    int Rows;
    int *pVector1, *pVector2, *pVector3;
    int NThreads;
    int rc;
    struct Th_Args *pTh_Args = NULL;
    pthread_t *pThreads = NULL;
    STDrand48Data *pRandData = NULL;
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

    if (!ExistArg("-nt", argc, argv))
    {
        fputs("Parameter -nt is neccesary.\n", stderr);
        ParametersError();
    }
    else
    {
        NThreads = atoi(GetArg("-nt", argc, argv));
        if (NThreads <= 0 || NThreads > Rows / 3)
        {
            puts("NThreads must be > 0 and <= Rows/3.");
            exit(1);
        }
    }

    if (PRINT == 1)
    {
        printf("Rows=%d\n", Rows);
    }

    // Getmem to arrays
    pVector1 = (int *)GetMem(Rows, sizeof(int), "Main:PVector1");
    pVector2 = (int *)GetMem(Rows, sizeof(int), "Main:pVector2");
    pVector3 = (int *)GetMem(Rows, sizeof(int), "Main:pVector3");

    // Get mem for pRandData
    pRandData = (STDrand48Data *)GetMem(NThreads, sizeof(STDrand48Data), "Main: pRandomData");

    // Init seeds
    for (int i = 0; i < NThreads; i++)
        srand48_r(i, &pRandData[i]);

    // Getmem to struct
    pTh_Args = (struct Th_Args *)GetMem(NThreads, sizeof(struct Th_Args), "Main: pTh_Args");

    // Getmem threads
    pThreads = (pthread_t *)GetMem(NThreads, sizeof(pthread_t), "Main: pThreads");

    // Init arrays
    InicializarVector(pVector1, pVector2, Rows, pRandData);

    int row_start;
    int n_sub_rows;

    for (int t = 0; t < NThreads; t++)
    {
        pTh_Args[t].ThreadId = t;
        HowtoShareVector(t, NThreads, Rows, &row_start, &n_sub_rows);
        pTh_Args[t].inicio = row_start;
        pTh_Args[t].fin = row_start + n_sub_rows - 1;
        pTh_Args[t].pVector1 = pVector1;
        pTh_Args[t].pVector2 = pVector2;
        pTh_Args[t].pVector3 = pVector3;
        pTh_Args[t].Rows = Rows;
        pTh_Args[t].pRandData = &pRandData[t];

        rc = pthread_create(&pThreads[t], NULL, MultiplicarVector, (void *)&pTh_Args[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (int t = 0; t < NThreads; t++)
    {
        rc = pthread_join(pThreads[t], NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

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

    free((void *)pRandData);

    free((void *)pTh_Args);

    free((void *)pThreads);

    return 0;
}

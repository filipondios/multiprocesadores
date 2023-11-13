#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "01-DivideVector.h"

#define NThreads 4
#define Rows 10

//-----------------------------------------------------------------------------
void HowtoShareVector(int ThisTh, int NTh, int NRows, int *start, int *n)
{
   int n_sub_rows = NRows / NTh;                    // How many sub rows.
   int n_larger_threads = NRows - NTh * n_sub_rows; // Threads with more rows
   int row_start;                                   // In which row a thread starts.

   if (ThisTh < n_larger_threads)
   {
      n_sub_rows = n_sub_rows + 1;
      row_start = 0 + ThisTh * n_sub_rows;
   }
   else if (n_sub_rows > 0)
   {
      row_start = 0 + n_larger_threads + ThisTh * n_sub_rows;
   }
   else
   { // this process has only zero elements
      row_start = -1;
      n_sub_rows = 0;
   }

   *n = n_sub_rows;
   *start = row_start;
}

//-----------------------------------------------------------------------------
// int main(int argc, char *argv[])
// {
//    int row_start;  // In which row a thread starts.
//    int n_sub_rows; // How many rows to ptocess.

//    for (int t = 0; t < NThreads; t++)
//    {

//       HowtoShareVector(t, NThreads, Rows, &row_start, &n_sub_rows);

//       printf("Thread %d out of %d, responsible of %d rows: [%d,%d].\n",
//              t, NThreads, n_sub_rows, row_start, row_start + n_sub_rows - 1);
//    }
//    return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "getmem.h"
#include "argshand.h"
#include "utils.h"

/* Show info? */
#define INFO 0

void ParametersError() {
    puts("Options are:");
    puts("\t[ -h To show this help ]");
    puts("\t  -r <n rows>			");
    puts("\t  -c <n columns>		");
    puts("\t  -o <file> [Image]>	");
    exit(0);
}

int main(int argc, char **argv) {
    int Rows, Cols;
    char **ppRed, **ppGreen, **ppBlue;  //Matrices de RGB (0..255)
    char *FileName;
    FILE * FOut;
    char Command[256];
    bool GenImage = false;
 
    if (ExistArg("-h",argc,argv))
        ParametersError();  

    if (!ExistArg("-r",argc,argv)) {
        fputs("Parameter -r is neccesary.\n",stderr);
        ParametersError();
    }
    else {
        Rows = atoi(GetArg("-r", argc, argv));

        if (Rows <=3) {
         puts("Rows<=3");
         exit(1);
        }
    } 

    if (!ExistArg("-c",argc,argv)) {
        fputs("Parameter -c is neccesary.\n",stderr);
        ParametersError();
    }
    else {
        Cols = atoi(GetArg("-c",argc,argv)); 
     
        if (Cols <=3) {
         puts("Col<=3");
         exit(1);
        }
    }  
    if (ExistArg("-o",argc,argv)) {
        GenImage=true;
        FileName = GetArg("-o",argc,argv);
    } 

 #if (INFO==1)
 printf("Rows=%d, Cols=%d, Output=%s.\n", Rows, Cols, FileName);
 #endif

    /*Calloc de Getmen put data to zeroes*/
    ppRed   = (char**) GetMem2D (Rows, Cols,sizeof(char), "Main:ppRed");
    ppGreen = (char**) GetMem2D (Rows, Cols,sizeof(char), "Main:ppGreen");
    ppBlue  = (char**) GetMem2D (Rows, Cols,sizeof(char), "Main:ppBlue");

 #if (INFO==1)
 printf("Rows/4=%d, Rows*3/4=%d\n", Rows/4, Rows*3/4);
 #endif

    int mid = Rows>>2;
    int send = Rows*3/4;
        
    for(int i=0; i<Cols; i++){
        for (int j=0; j<mid; j++)
            ppRed[j][i] = (char)225;

        for (int j=mid; j<send; j++){
            ppRed[j][i] = (char)225;
            ppGreen[j][i] =(char)255;
        }

        for (int j=send; j<Rows; j++)
            ppRed[j][i] = (char)225;
    }

    /*Red*/
    /*
    for (int i=0; i<Rows/4; i++)
        for (int j=0; j<Cols; j++)
            ppRed[i][j]=(char)255;
    */

    /*Yellow*/
    /*
    for (int i=Rows/4; i<Rows*3/4; i++){
        for (int j=0; j<Cols; j++) {
            ppRed  [i][j]=(char)255;
            ppGreen[i][j]=(char)255;
        }
    }
    */
    /*Red*/
    /*
    for (int i=Rows*3/4; i<Rows; i++)
        for (int j=0; j<Cols; j++)
          ppRed[i][j]=(char)255;
    */

    if (GenImage) {
        //Print to file
	    FOut = OpenFile(FileName,"wb");    

        for (int i=0;i<Rows;i++) {
	        for (int j=0;j<Cols;j++) {
		        fwrite(&ppRed  [i][j], sizeof(char), 1, FOut);
			    fwrite(&ppGreen[i][j], sizeof(char), 1, FOut);
			    fwrite(&ppBlue [i][j], sizeof(char), 1, FOut);
		    }
        }
    
        CloseFile(FOut);

 	    //Convert raw file to png
	    /*rawtoppm and pnmtopng are in netpbm package*/
	    sprintf(Command,"rawtoppm %d %d %s | pnmtopng > %s.png", Cols,Rows, FileName, FileName); 
	    system(Command);
    }

    //Free allocated memory
    Free2D((void**) ppGreen,Rows);
    Free2D((void**) ppRed,  Rows);
    Free2D((void**) ppBlue, Rows);
    return 0;
}

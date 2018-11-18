#include <zephyr.h>
#include <math.h>

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#include <stdlib.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

#define M_PI (3.14159)
#define MAX_ITERATION (10)

#define COEFFS(Cu,Cv,u,v) { \
        if (u == 0) Cu = 1.0 / sqrt(2.0); else Cu = 1.0; \
        if (v == 0) Cv = 1.0 / sqrt(2.0); else Cv = 1.0; \
        }


void dct(double macroblock[8][8], double dct2[8][8])
{
   int u,v,x,y;
   //FILE * fp = fopen("dct2.csv", "w");

    for (v=0; v<8; v++)
    {
        for (u=0; u<8; u++)
        {
            double Cu, Cv, z = 0.0;

            COEFFS(Cu,Cv,u,v);

            for (y=0; y<8; y++)
            for (x=0; x<8; x++)
            {
                double s, q;

                s = macroblock[x][y];

                q = s * cos((double)(2*x+1) * (double)u * M_PI/16.0) *
                        cos((double)(2*y+1) * (double)v * M_PI/16.0);
                z += q;
            }

            dct2[v][u] = 0.25 * Cu * Cv * z;
            //fprintf(fp, "\n %lf", dct2[v][u]);
        }
        //fprintf(fp, "\n");

    }
}

void idct(double dct2[8][8], double idct2[8][8])
{
    int u,v,x,y;
    //FILE * fp = fopen("idct2.csv", "w");

    for (y=0; y<8; y++)
    {
        for (x=0; x<8; x++)
        {
            double z = 0.0;

            for (v=0; v<8; v++)
            for (u=0; u<8; u++)
            {
                double S, q;
                double Cu, Cv;

                COEFFS(Cu,Cv,u,v);
                S = dct2[v][u];

                q = Cu * Cv * S *
                    cos((double)(2*x+1) * (double)u * M_PI/16.0) *
                    cos((double)(2*y+1) * (double)v * M_PI/16.0);

                    z += q;
            }
            z /= 4.0;
            idct2[x][y] = z;
            //fprintf(fp, "\n %lf", idct2[x][y]);
        }
        //fprintf(fp, "\n");
    }
}


void main(void)
{
    int idx=0;
    double Macroblock[8][8] = { {101, 100,  94, 102,  97,  91,  88,  83},
                                {101,  99,  98, 103,  93,  93, 107, 110},
                                { 98,  97,  97,  97, 103, 101,  94, 100},
                                { 97,  98,  99, 100, 103, 105, 101,  96},
                                { 99, 100, 104, 104, 100, 107, 109,  89},
                                { 99, 101, 105, 105, 116, 113,  87,  58},
                                { 94,  69,  66,  66,  79,  70,  40,  26},
                                { 59,  30,  33,  33,  32,  37,  45,  41} };
    double dct2[8][8];
    double idct2[8][8];


    PRINT("Starting DCT test for %d iterations\n", MAX_ITERATION);
    for(idx=0; idx < MAX_ITERATION; idx++)
    {
        dct(Macroblock, dct2);
        PRINT("Completed DCT test %d\n", idx);
        idct(dct2, idct2);
        PRINT("Completed INVERSE DCT test %d\n", idx);
    }
    PRINT("Completed DCT test for %d iterations\n", idx);

}

#include <zephyr.h>

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#include <stdlib.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

#define MAX (300000)
#define CODE_LENGTH ((sizeof(unsigned char))*8)

unsigned char isprime[(MAX/(CODE_LENGTH))+1];

int chk_isprime(unsigned int i)
{
    unsigned int idx;
    unsigned int bitpos;

    idx = i/(CODE_LENGTH);
    bitpos = i % (CODE_LENGTH);

    //PRINT("i=%u, idx=%u, bitpos=%u\n", i, idx, bitpos);

    return(((isprime[idx]) & (1<<bitpos))>0);
}

int set_isprime(unsigned int i, unsigned char val)
{
    unsigned int idx;
    unsigned int bitpos;

    idx = i/(CODE_LENGTH);
    bitpos = i % (CODE_LENGTH);

    //PRINT("i=%u, idx=%u, bitpos=%u\n", i, idx, bitpos);

    if(val > 0)
    {
        isprime[idx] = isprime[idx] | (1<<bitpos);
    }
    else
    {
        isprime[idx] = isprime[idx] & (~(1<<bitpos));
    }

        return bitpos;
}

void print_isprime(void)
{
    int idx=0;

    //PRINT("idx=%d\n", (MAX/(CODE_LENGTH)));

    for(idx=(MAX/(CODE_LENGTH)); idx >= 0; idx--)
    {
        //PRINT("idx=%d, %02X\n", idx, isprime[idx]);
    }
    //PRINT("\n");

}


void main(void)
{
        unsigned int i, j;
        unsigned int p=2;
        unsigned int cnt=0;

        PRINT("max uint = %u\n", (0xFFFFFFFF));

        // Not prime by definition
        // 0 & 1 not prime, 2 is prime, 3 is prime, assume others prime to start
        isprime[0]=0xFC;
        for(i=2; i<MAX; i++) { set_isprime(i, 1); }

        //for(i=0; i<MAX; i++) { PRINT("isprime=%d\n", chk_isprime(i)); }
        print_isprime();

        while( (p*p) <=  MAX)
        {
            //PRINT("p=%u\n", p);

            // invalidate all multiples of lowest prime so far
            //
            // simple to compose into a grid of invalidations
            //

            for(j=2*p; j<MAX+1; j+=p)
            {
                //PRINT("j=%u\n", j);
                set_isprime(j,0);
            }

            // find next lowest prime - sequential process
            for(j=p+1; j<MAX+1; j++)
            {
                if(chk_isprime(j)) { p=j; break; }
            }

        }


        for(i=0; i<MAX+1; i++)
        {
            if(chk_isprime(i))
            {
                cnt++;
                //PRINT("i=%u\n", i);
            }
        }
        PRINT("\nNumber of primes [0..%u]=%u\n\n", MAX, cnt);

}

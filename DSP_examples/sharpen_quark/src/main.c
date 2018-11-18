#include <zephyr.h>
#include <microkernel.h>

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#include <stdlib.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

// https://www.zephyrproject.org/doc/kernel/common/common_float.html

#include "image.h"

UINT8 convGrayVal[IMG_HEIGHT*IMG_WIDTH];

#define K 4.0
#define FRAME_RATE (60)
typedef double FLOAT;


void transform(void);

void main(void)
{
    PRINT("START: Transforming an 80x60 image buffer %d times\n", FRAME_RATE);
    task_start(TRANS_TASK);
    PRINT("DONE: Transforming an 80x60 image buffer %d times\n", FRAME_RATE);
}

void transform(void)
{
    int row, col, frameIdx=0;
    int64_t time_stamp, ticks_spent;
    FLOAT temp=0.0;
    FLOAT PSF[9] = {(-K/8.0), (-K/8.0), (-K/8.0), (-K/8.0), (K+1.0), (-K/8.0), (-K/8.0), (-K/8.0), (-K/8.0)};

    PRINT("TRANSFORM START\n");

    for(frameIdx=0; frameIdx < FRAME_RATE; frameIdx++)
    {
        time_stamp = sys_tick_get();
        PRINT("Frame %d @ %lu\n", frameIdx, time_stamp);

        // Skip first and last row, no neighbors to convolve with
        for(row=1; row<((IMG_HEIGHT)-1); row++)
        {

            // Skip first and last column, no neighbors to convolve with
            for(col=1; col<((IMG_WIDTH)-1); col++)
            {
                temp=0.0;
                temp += ((PSF[0]) * ((FLOAT)GrayVal[((row-1)*IMG_WIDTH)+col-1]));
                temp += (PSF[1] * (FLOAT)GrayVal[((row-1)*IMG_WIDTH)+col]);
                temp += (PSF[2] * (FLOAT)GrayVal[((row-1)*IMG_WIDTH)+col+1]);
                temp += (PSF[3] * (FLOAT)GrayVal[((row)*IMG_WIDTH)+col-1]);
                temp += (PSF[4] * (FLOAT)GrayVal[((row)*IMG_WIDTH)+col]);
                temp += (PSF[5] * (FLOAT)GrayVal[((row)*IMG_WIDTH)+col+1]);
                temp += (PSF[6] * (FLOAT)GrayVal[((row+1)*IMG_WIDTH)+col-1]);
                temp += (PSF[7] * (FLOAT)GrayVal[((row+1)*IMG_WIDTH)+col]);
                temp += (PSF[8] * (FLOAT)GrayVal[((row+1)*IMG_WIDTH)+col+1]);

                if(temp < 0.0) temp=0.0;
                if(temp > 255.0) temp=255.0;

                convGrayVal[(row*IMG_WIDTH)+col]=(UINT8)temp;
            }
        }
        ticks_spent = sys_tick_delta(&time_stamp);
        PRINT("Frame %d TRANSFORMED in %lu microsec\n", frameIdx, (ticks_spent*sys_clock_us_per_tick));
    }

    PRINT("TRANSFORM END\n");

    PRINT("HEX IMAGE:\n");
    for(row=0; row<IMG_HEIGHT; row++)
    {
        for(col=0; col<IMG_WIDTH; col++)
        {
           PRINT("%02X ", convGrayVal[(row*IMG_WIDTH)+col]);
        }
        PRINT("\n");
    }
}


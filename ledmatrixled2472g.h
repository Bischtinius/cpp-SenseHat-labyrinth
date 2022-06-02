#pragma once
// #ifndef LEDMATRIXLED2472G_H
// #define LEDMATRIXLED2472G_H
/*
 *  library for 8x8 RGB565 Matrix using frame buffer method
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/times.h>

#define FILEPATH "/dev/fb0"
#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

#define MAX_X  8       // 8x8 LED Matrix - [x=0,y=0] entspricht *map
#define MAX_Y  8

int fbfd;
uint16_t *mapmem;
uint16_t *p;


void delay_led(int t)
{

    clock_t time_end;
    time_end = clock() + t * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }

}

void open_LED_dev_LED2472G()
{

    fbfd = open(FILEPATH, O_RDWR);

}

void init_LED_LED2472G()

{
    struct fb_fix_screeninfo fix_info;
    //open_LED_dev_LED2472G();
    fbfd = open(FILEPATH, O_RDWR);
    //printf("fbfd = %d \n",fbfd);

    if (fbfd == -1)
    {
        perror("Error (call to 'open')");
        exit(EXIT_FAILURE);
    }

    /* read fixed screen info for the open device */
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fix_info) == -1)
    {
        perror("Error (call to 'ioctl')");
        close(fbfd);
        exit(EXIT_FAILURE);
    }

    /* now check the correct device has been found */
    if (strcmp(fix_info.id, "RPi-Sense FB") != 0)
    {
        printf("%s\n", "Error: RPi-Sense FB not found");
        close(fbfd);
        exit(EXIT_FAILURE);
    }

    /* map the led frame buffer device into memory */
	int temp = fbfd;
    mapmem = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, temp, 0);

    if (mapmem == MAP_FAILED)
    {
        close(fbfd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    p = mapmem;
    //printf("Init success p=%d\n",p);
}

void close_LED_LED2472G()
{
    /* un-map and close */
    if (munmap(mapmem, FILESIZE) == -1)
    {
        perror("Error un-mmapping the file");
    }
    close(fbfd);
}

void clear_allLED_LED2472G()
{
    /* clear the led matrix */
    memset(mapmem, 0, FILESIZE);
}

void set_singleLED_LED2472G(int x, int y, int r, int g, int b)
{
    int color = (r<<12)+(g<<8)+b;

    *(p+(y*MAX_Y)+x) = color;
}

void clear_singleLED_LED2472G(int x, int y)
{
    set_singleLED_LED2472G(x, y, 0, 0, 0);
}


void set_allLED_LED2472G(int r, int g, int b)
{

    /* flash all LEDs */
    int color = (r<<12)+(g<<8)+b;
    //printf("color = %x\n", color);
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {

            set_singleLED_LED2472G(x, y, r, g, b);

        }
    }
}
// #endif // LEDMATRIXLED2472G_H


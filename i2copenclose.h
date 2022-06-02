#ifndef I2COPENCLOSE_H
#define I2COPENCLOSE_H

#include <unistd.h>
#include "smbus.h"
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>

#define DEV_PATH "/dev/i2c-1"

int fd=0;

int open_i2c_dev()
{
    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0)
    {
        perror("Unable to open i2c device");
        exit(1);
    }

}

void close_i2c_dev()
{

    close(fd);
}

void delay(int t)
{

    clock_t time_end;
    time_end = clock() + t * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }

    //)usleep(t * 1000);
}
#endif

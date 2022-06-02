#ifndef PRESSTEMPLPS25H_H
#define PRESSTEMPLPS25H_H
/*
 *  C code to read pressure and temperature from the
 *  Raspberry Pi Sense HAT add-on board (LPS25H sensor)
 *  per i2c Interface
 *
 */

#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>

#define DEV_ID_LPS25H 0x5c                 // Values see datasheet of LPS25H
#define WHO_AM_I_LPS25H 0x0F

#define CTRL_REG1_LPS25H 0x20
#define CTRL_REG2_LPS25H 0x21
#define PRESS_OUT_XL_LPS25H 0x28
#define PRESS_OUT_L_LPS25H 0x29
#define PRESS_OUT_H_LPS25H 0x2A
#define TEMP_OUT_L_LPS25H 0x2B
#define TEMP_OUT_H_LPS25H 0x2C



void init_PressTemp_LPS25H()
{

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, DEV_ID_LPS25H) < 0)
    {
	perror("Unable to configure i2c slave device");
	close(fd);
	exit(1);
    }

    /* check we are who we should be */
    if (i2c_smbus_read_byte_data(fd, WHO_AM_I_LPS25H) != 0xBD)
    {
        printf("%s\n", "who_am_i error");
        close(fd);
        exit(1);
    }

    /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd, CTRL_REG1_LPS25H, 0x00);

    /* Turn on the pressure sensor analog front end in single shot mode  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1_LPS25H, 0x84);

    /* Run one-shot measurement (temperature and pressure), the set bit will be reset by the
     * sensor itself after execution (self-clearing bit) */
    i2c_smbus_write_byte_data(fd, CTRL_REG2_LPS25H, 0x01);
}

double read_Temp_LPS25H()
{

    uint8_t temp_out_l = 0, temp_out_h = 0;
    int16_t temp_out = 0;
    double t_c = 0.0;
    uint8_t status = 0;

    /* Wait until the measurement is complete */
    do
    {
        delay(25);		/* 25 milliseconds */
        status = i2c_smbus_read_byte_data(fd, CTRL_REG2_LPS25H);
    }

    while (status != 0);

    /* Read the temperature measurement (2 bytes to read) */
    temp_out_l = i2c_smbus_read_byte_data(fd, TEMP_OUT_L_LPS25H);
    temp_out_h = i2c_smbus_read_byte_data(fd, TEMP_OUT_H_LPS25H);

    /* make 16 and 24 bit values (using bit shift) */
    temp_out = temp_out_h << 8 | temp_out_l;

    /* calculate output values */
    t_c = 42.5 + (temp_out / 480.0);  // convert from °F into °C

    return t_c;

}

double read_Press_LPS25H()
{
    uint8_t press_out_xl = 0;
    uint8_t press_out_l = 0;
    uint8_t press_out_h = 0;

    int32_t press_out = 0;
    double pressure = 0.0;

    uint8_t status = 0;

    /* Wait until the measurement is complete */
    do
    {
        delay(25);		/* 25 milliseconds */
        status = i2c_smbus_read_byte_data(fd, CTRL_REG2_LPS25H);
    }

    while (status != 0);

        /* Read the pressure measurement (3 bytes to read)*/
    press_out_xl = i2c_smbus_read_byte_data(fd, PRESS_OUT_XL_LPS25H);
    press_out_l = i2c_smbus_read_byte_data(fd, PRESS_OUT_L_LPS25H);
    press_out_h = i2c_smbus_read_byte_data(fd, PRESS_OUT_H_LPS25H);
        /* make 16 and 24 bit values (using bit shift)*/

    press_out = press_out_h << 16 | press_out_l << 8 | press_out_xl;
    pressure = press_out / 4096.0;      // convert into [hPa]
    return pressure;

}


void close_PressTemp_LPS25H()
{
    /* Power down the device */
    i2c_smbus_write_byte_data(fd, CTRL_REG1_LPS25H, 0x00);
    close(fd);
}
#endif

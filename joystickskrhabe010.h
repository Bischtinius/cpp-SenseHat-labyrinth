#pragma once
// #ifndef JOYSTICKSKRHABE10_H
// #define JOYSTICKSKRHABE10_H

/* Testprogramm for joystick of the SenseHAT using the input subsystem */

#define _GNU_SOURCE
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/input.h>

int fd_joy;


/***********************************************************/
static int is_event_device(const struct dirent *dir)
{
	return strncmp(EVENT_DEV_NAME, dir->d_name,strlen(EVENT_DEV_NAME)-1) == 0;
}

/***********************************************************/
static int open_evdev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;

	ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);

	if (ndev <= 0)
	{
		return ndev;
    }
	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256];

		snprintf(fname, sizeof(fname),"%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
		fd = open(fname, O_RDONLY);

		if (fd < 0)
		{
			continue;
		}
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

		//printf("%s \n",name);
		//printf("%s",dev_name);

		if (strcmp(dev_name, name) == 0)
		{
			break;
            close(fd);
        }
	}

	for (i = 0; i < ndev; i++)
	{
		free(namelist[i]);
    }
	return fd;
}

/*****************************************************************/
int read_Joystick_SKRHABE010()
{
	struct input_event ev[64];    // using the input subsystem
	int i, rd;

	rd = read(fd_joy, ev, sizeof(struct input_event) * 64);
	//printf("rd = %d \n", rd);

	if (rd < (int) sizeof(struct input_event))              // error handling
	{
		fprintf(stderr, "expected %d bytes, got %d\n", (int) sizeof(struct input_event), rd);
		return 0;
	}

    //printf("ev->type = %d  ev->value = %d ev->code = %d\n", ev->type,ev->value,ev->code);

    if(ev->value == 0)
    {
        return 0;       // Taste losgelassen
    }

    return(ev->code);
}
/*****************************************************************/

void init_JoyStick_SKRHABE010()
{
	struct pollfd evpoll = {.events = POLLIN,};     // in poll.h

	evpoll.fd = open_evdev("Raspberry Pi Sense HAT Joystick");

	fd_joy = evpoll.fd;
	//printf(" Filedeskriptor = %d\n",fd_joy);

	if (evpoll.fd < 0)                              // error handling
	{
		fprintf(stderr, "Event device not found.\n");
	}


}
/*****************************************************************/
int poll_Joystick_SKRHABE010()
{
    struct pollfd evpoll = {.events = POLLIN,};
    evpoll.fd = fd_joy;

    return poll(&evpoll, 1, 0);
}
// #endif

/*-*- C -*-*******************************************************************
 *
 *  Copyright (c) 2011, Freescale Semiconductor. All Rights Reserved.
 *  Freescale Confidential Proprietary
 *
 ****************************************************************************/

#include "ilm_client.h"
#include "ilm_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <linux/input.h>
#include <linux/joystick.h>


/** **/
#ifdef EVENT
/**/
#define JS_DEV "/dev/input/event1"
#define STRUCT_EVENT struct input_event
#define CODE_FIELD code

#else
/**/
#define JS_DEV "/dev/input/js0"
#define STRUCT_EVENT struct js_event
#define CODE_FIELD number

#endif /* #ifdef EVENT */
/** **/

#define TRACE if (debug) printf

#define SURFACE_ID 96
#define CODE_BACK 0x2c2

static const char short_options [] = "j:s:b:v:h";
static const struct option
long_options [] = {
        { "joystick",    required_argument,      NULL,           'j' },
        { "surface",     required_argument,      NULL,           's' },
        { "backcode",    optional_argument,      NULL,           'b' },
        { "verbose",     no_argument,            NULL,           'v' },
        { "help",        no_argument,            NULL,           'h' },
        { 0, 0, 0, 0 }
};

int main(int argc, char *argv[]) {
    int fd = -1;
    char* dev_name = JS_DEV;
    uint32_t surfaceid = SURFACE_ID;
    uint32_t backcode = CODE_BACK;
    uint32_t debug = 0;

    for (;;) {
        int index;
        int c;
    
        c = getopt_long (argc, argv, short_options, long_options, &index);

        if (-1 == c) {
            break;
        }

        switch (c) {
        case 'j':
            dev_name = optarg;
            break;
        case 's':
            surfaceid = atoi(optarg);
            break;
        case 'b':
            backcode = atoi(optarg);
            break;
        case 'v':
            debug = 1;
            break;
        case 'h':
        default:
            fprintf (stdout,
                 "Usage: %s [options]\n\n"
                 "Options:\n"
                 "-j | --joystick device name   JoyStick device name [/dev/input/js]\n"
                 "-s | --surface  surface id    surface id\n"
                 "-b | --backcode back code     back code\n"
                 "-v | --verbose                output debug message\n"
                 "-h | --help                   Print this message\n"
                 "",
                 argv[0]);
             exit (EXIT_SUCCESS);
        }
    }
 
    fd = open(dev_name, O_RDONLY, 0);
    if (-1 == fd) {
        fprintf (stderr, "Cannot open '%s': %d, %s\n", dev_name, errno, strerror(errno));
        exit (EXIT_FAILURE);
    }

    ilm_init();
    fprintf (stdout, "surface id:%d, backcode:%u\n", surfaceid, backcode);
    TRACE("DEBUG MODE \n");
    usleep(1000);

    TRACE("sizeof(STRUCT_EVENT) = %u\n", sizeof(STRUCT_EVENT));

    while(1) {
	STRUCT_EVENT ev_js;

        read(fd, &ev_js, sizeof(ev_js));

        TRACE("type       = 0x%X\n", ev_js.type);
        TRACE("CODE_FIELD = 0x%X\n", ev_js.CODE_FIELD);
        TRACE("value      = 0x%X\n", ev_js.value);
        if (ev_js.type != 1) {
            continue;
        }

        if (ev_js.CODE_FIELD != backcode) {
            continue;
        }

        if (ev_js.value == 1) {
            usleep(1000);
            ilm_surfaceSetVisibility(surfaceid, ILM_TRUE);
            ilm_commitChanges();
        } else {
            ilm_surfaceSetVisibility(surfaceid, ILM_FALSE);
            ilm_commitChanges();
            usleep(1000);

        }
    }

    ilm_destroy();
    close(fd);
    return 0;
}


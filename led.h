/*
 *  ser2net - A program for allowing telnet connection to serial ports
 *  Copyright (C) 2016-2020  Corey Minyard <minyard@acm.org>
 *  Copyright (C) 2016 Michael Heimpold <mhei@heimpold.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef LED_H
#define LED_H

struct led_driver_s;

struct led_s
{
    struct led_s *next;
    char *name;

    struct led_driver_s *driver;
    void *drv_data;
};

struct led_driver_s {
    struct led_driver_s *next;
    const char *name;

    /* required: parse the parameters from config file */
    int (*init)(struct led_s *led, const char * const *options, int lineno);

    /* optional, but required when drv_data is malloced in init */
    int (*free)(struct led_s *led);

    /* optional: called once during initialization, prepares the LED */
    int (*configure)(void *drv_data, int lineno);

    /* required: called when data transfer should be signaled */
    int (*flash)(void *drv_data);

    /* optional: called during deinitialization, could switch the LED off */
    int (*deconfigure)(void *drv_data);
};

/* Initializes and registers all LED drivers */
int led_driver_init(void);

/* Callback to register a given LED driver in the system */
int led_driver_register(struct led_driver_s *led_driver);

/* Handle an LED config line */
int add_led(const char *name, const char *driverstr,
	    const char * const *options, int lineno);

/* Search for a LED by name */
struct led_s *find_led(const char *name);

/* Free all registered LEDs in the system */
void free_leds(void);

/* Flash the given LED */
int led_flash(struct led_s *led);

#endif /* LED_H */

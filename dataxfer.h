/*
 *  ser2net - A program for allowing telnet connection to serial ports
 *  Copyright (C) 2001  Corey Minyard <minyard@acm.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef DATAXFER
#define DATAXFER

#include "controller.h"

#ifdef linux

#include <linux/serial.h>

/* Check, if the toolchain provides SER_RS485_RX_DURING_TX macro
 * (introduced in kernel 3.2) */
#if HAVE_DECL_TIOCSRS485
#ifndef SER_RS485_RX_DURING_TX
#define SER_RS485_RX_DURING_TX          (1 << 4)
#endif /* SER_RS485_RX_DURING_TX */
#endif /* HAVE_DECL_TIOCSRS485 */

#endif /* linux */

/* Create a port given the criteria. */
int portconfig(struct absout *eout,
	       const char *name,
	       const char *accstr,
	       const char *state,
	       unsigned int timeout,
	       const char *devname,
	       const char * const *devcfg);
void apply_new_ports(struct absout *eout);

/* Shut down all the ports, and provide a way to check when done. */
void shutdown_ports(void);
int check_ports_shutdown(void);

/* Initialize the data transfer code. */
void dataxfer_init(void);

/* Show information about a port (or all ports if portspec is NULL).
   The parameters are all strings that the routine will convert to
   integers.  Error output will be generated on invalid data. */
void showports(struct controller_info *cntlr, const char *portspec, bool yaml);

/* Show information about a port (as above) but in a one-line format. */
void showshortports(struct controller_info *cntlr, const char *portspec);

/* Set the port's timeout.  The parameters are all strings that the
   routine will convert to integers.  Error output will be generated
   on invalid data. */
void setporttimeout(struct controller_info *cntlr,
		    const char *portspec,
		    const char *timeout);

/* Modify the DTR and RTS lines for the port. */
void setportcontrol(struct controller_info *cntlr,
		    const char *portspec,
		    char * const controls[]);

/* Set the enable state of a port (off, raw, telnet).  The parameters
   are all strings that the routine will convert to integers.  Error
   output will be generated on invalid data. */
void setportenable(struct controller_info *cntlr,
		   const char *portspec,
		   const char *enable);

/* Start data monitoring on the given port, type may be either "tcp" or
   "term" and only one direction may be monitored.  This return NULL if
   the monitor fails.  The monitor output will go to the controller
   via the controller_write() call. */
void *data_monitor_start(struct controller_info *cntlr,
			 const char *type,
			 const char *portspec);

/* Stop monitoring the given id. */
void data_monitor_stop(struct controller_info *cntlr,
		       void   *monitor_id);

/* Shut down the port, if it is connected. */
void disconnect_port(struct controller_info *cntlr,
		     const char *portspec);

struct devio;

/* Initialization function for device I/O */
int devcfg_init(struct devio *io, struct absout *eout, const char *instr,
		int (*otherconfig)(void *data, struct absout *eout,
				   const char *item),
		void *data);

int add_rotator(const char *name, const char *accstr,
		int portc, const char **ports,
		const char **options, int lineno);

void free_rotators(void);

#endif /* DATAXFER */

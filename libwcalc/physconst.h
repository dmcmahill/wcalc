
/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2006 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

/*
 * physical constants and also conversion constants
 */

#ifndef __PHYSCONST_H__
#define __PHYSCONST_H__

/* free space speed of light, meters/second */
#define LIGHTSPEED 2.99792458e8

/* free space permitivitty (Henries/meter) */
#define FREESPACE_MU0  (4.0 * M_PI * 1.0e-7)

/* free space permitivitty (Farads/meter) */
#define FREESPACE_E0  (1.0 / (LIGHTSPEED * LIGHTSPEED * FREESPACE_MU0))

/* free space impedance, Ohms */
#define FREESPACEZ0 (FREESPACE_MU0 * LIGHTSPEED)

#define MIL2MICRON(x)  (x*25.4)
#define MICRON2MIL(x)  (x/25.4)

#define MIL2UM(x)  (x*25.4)
#define UM2MIL(x)  (x/25.4)

#define MIL2MM(x)  (x*25.4e-3)
#define MM2MIL(x)  (x/25.4e-3)

#define MIL2CM(x)  (x*25.4e-4)
#define CM2MIL(x)  (x/25.4e-4)

#define MIL2M(x)  (x*25.4e-6)
#define M2MIL(x)  (x/25.4e-6)

#define INCH2M(x)  (x*25.4e-3)
#define M2INCH(x)  (x/25.4e-3)

#define MILSTR   "mil"
#define UMSTR    "um "
#define MMSTR    "mm "
#define CMSTR    "cm "
#define MSTR     "m  "

#endif /*__PHYSCONST_H__*/





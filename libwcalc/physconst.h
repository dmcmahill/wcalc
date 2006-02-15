/* $Id: physconst.h,v 1.5 2004/08/06 01:49:16 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2006 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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





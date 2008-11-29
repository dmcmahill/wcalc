/* $Id: misc.h,v 1.10 2007/11/29 21:04:06 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004, 2007 Dan McMahill
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

#ifndef __MISC_H__
#define __MISC_H__

double dia2awg(double dia);
double awg2dia(double AWG);

#ifndef __GNUC__
#define __FUNCTION1(a,b) a ":" #b
#define __FUNCTION2(a,b) __FUNCTION1(a,b)
#define __FUNCTION__ __FUNCTION2(__FILE__,__LINE__)
#endif

#endif /*__MISC_H__*/

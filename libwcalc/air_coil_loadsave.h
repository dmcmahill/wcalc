/* $Id: air_coil_loadsave.h,v 1.7 2004/08/31 21:38:17 dan Exp $ */

/*
 * Copyright (C) 2001, 2002 Dan McMahill
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

#ifndef __AIR_COIL_LOADSAVE_H__
#define __AIR_COIL_LOADSAVE_H__

/* writes the data from 'coil' to fp */
void air_coil_save(air_coil_coil *coil, FILE *fp, char *fname);

/*
 * returns 0 on success
 */
int air_coil_load(air_coil_coil *coil, FILE *fp);

int air_coil_load_string(air_coil_coil *coil, const char *str);
char * air_coil_save_string(air_coil_coil *coil);

#endif /*__AIR_COIL_LOADSAVE_H__*/

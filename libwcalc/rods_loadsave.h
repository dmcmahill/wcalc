/*
 * Copyright (C) 2020 Dan McMahill
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

#ifndef __RODS_LOADSAVE_H__
#define __RODS_LOADSAVE_H__

/* writes the data from 'b' to fp */
void rods_save(rods *b, FILE *fp, char *fname);

/*
 * returns 0 on success
 */
int rods_load(rods *bb, FILE *fp);

int rods_load_string(rods *b, const char *str);
char * rods_save_string(rods *b);

#endif /*__RODS_LOADSAVE_H__*/

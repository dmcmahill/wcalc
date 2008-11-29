/* $Id: coax_loadsave.h,v 1.6 2004/08/31 21:38:18 dan Exp $ */

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

#ifndef __COAX_LOADSAVE_H__
#define __COAX_LOADSAVE_H__

/* writes the data from 'line' to fp */
void coax_save(coax_line *line, FILE *fp, char *fname);

/*
 * returns 0 on success
 */
int coax_load(coax_line *line, FILE *fp);

int coax_load_string(coax_line *line, const char *str);
char * coax_save_string(coax_line *line);

#endif /*__COAX_LOADSAVE_H__*/

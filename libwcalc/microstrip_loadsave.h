/* $Id: microstrip_loadsave.h,v 1.6 2004/08/31 21:38:23 dan Exp $ */

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

#ifndef __MICROSTRIP_LOADSAVE_H__
#define __MICROSTRIP_LOADSAVE_H__

void microstrip_save(microstrip_line *line, FILE *fp, char *fname);
int microstrip_load(microstrip_line *line, FILE *fp);

int microstrip_load_string(microstrip_line *line, const char *str);
char * microstrip_save_string(microstrip_line *line);

#endif /*__MICROSTRIP_LOADSAVE_H__*/


/*
 * Copyright (C) 2001, 2002, 2004 Dan McMahill
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

#ifndef __IC_MICROSTRIP_LOADSAVE_H__
#define __IC_MICROSTRIP_LOADSAVE_H__

void ic_microstrip_save(ic_microstrip_line *line, FILE *fp, char *fname);
int ic_microstrip_load(ic_microstrip_line *line, FILE *fp);

int ic_microstrip_load_string(ic_microstrip_line *line, const char *str);
char * ic_microstrip_save_string(ic_microstrip_line *line);

#endif /*__IC_MICROSTRIP_LOADSAVE_H__*/

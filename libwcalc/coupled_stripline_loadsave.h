
/*
 * Copyright (C) 2001, 2002, 2006 Dan McMahill
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

#ifndef __COUPLED_STRIPLINE_LOADSAVE_H__
#define __COUPLED_STRIPLINE_LOADSAVE_H__

void coupled_stripline_save(coupled_stripline_line *line, FILE *fp, 
			     char *fname);
int coupled_stripline_load(coupled_stripline_line *line, FILE *fp);

int coupled_stripline_load_string(coupled_stripline_line *line,
				   const char *str);
char * coupled_stripline_save_string(coupled_stripline_line *line);

#endif /*__COUPLED_STRIPLINE_LOADSAVE_H__*/

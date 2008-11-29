/* $Id: coplanar_loadsave.h,v 1.2 2006/01/08 06:35:29 dan Exp $ */

/*
 * Copyright (C) 2006 Dan McMahill
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

#ifndef __COPLANAR_LOADSAVE_H__
#define __COPLANAR_LOADSAVE_H__

void coplanar_save(coplanar_line *line, FILE *fp, char *fname);
int coplanar_load(coplanar_line *line, FILE *fp);

int coplanar_load_string(coplanar_line *line, const char *str);
char * coplanar_save_string(coplanar_line *line);

#endif /*__COPLANAR_LOADSAVE_H__*/

/* $Id: bars_loadsave.h,v 1.2 2008/11/29 20:42:03 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
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

#ifndef __PARALLEL_RC_LOADSAVE_H__
#define __PARALLEL_RC_LOADSAVE_H__

/* writes the data from 'b' to fp */
void parallel_rc_save(parallel_rc *b, FILE *fp, char *fname);

/*
 * returns 0 on success
 */
int parallel_rc_load(parallel_rc *bb, FILE *fp);

int parallel_rc_load_string(parallel_rc *b, const char *str);
char * parallel_rc_save_string(parallel_rc *b);

#endif /*__PARALLEL_RC_LOADSAVE_H__*/

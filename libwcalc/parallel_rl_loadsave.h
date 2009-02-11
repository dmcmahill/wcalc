/* $Id: parallel_lr_loadsave.h,v 1.1 2009/02/10 05:00:21 dan Exp $ */

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

#ifndef __PARALLEL_RL_LOADSAVE_H__
#define __PARALLEL_RL_LOADSAVE_H__

/* writes the data from 'b' to fp */
void parallel_rl_save(parallel_rl *b, FILE *fp, char *fname);

/*
 * returns 0 on success
 */
int parallel_rl_load(parallel_rl *bb, FILE *fp);

int parallel_rl_load_string(parallel_rl *b, const char *str);
char * parallel_rl_save_string(parallel_rl *b);

#endif /*__PARALLEL_RL_LOADSAVE_H__*/

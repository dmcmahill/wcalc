
/*
 * Copyright (C) 2002, 2005 Dan McMahill
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

#ifndef __CGI_COMMON_H__
#define __CGI_COMMON_H__

void printFormError(const char *fmt,...);
void inputErr(int *input_err);
void fixInputMsg(void);
void bugMsg(void);

/* desired cookie age for all cgi programs here (seconds) */
#define COOKIE_AGE (60*60*24*180)

/* XXX find a better way... */
/* max cookie length */
#define COOKIE_MAX 512

#endif /* __CGI_COMMON_H__ */


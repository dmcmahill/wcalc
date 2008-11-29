/* $Id: mex-alert.c,v 1.2 2002/06/12 11:30:38 dan Exp $ */

/*
 * Copyright (C) 2001 Dan McMahill
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

#include "config.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "alert.h"
#include "mex.h"

#define MSGMAX 500
void alert(const char *fmt,...)
{
  va_list ap;
  /* XXX buffer overflow.... */
  char msg[MSGMAX+1]; 
  char msg2[MSGMAX+2]; 
  char *i,*o;

  /* extract the warning string */
  va_start(ap,fmt);
  vsprintf(msg,fmt,ap);
  va_end(ap);

  i=msg;
  o=msg2;
  while( (*i != '\0') && (o < (msg2+MSGMAX)) ){
    if(*i == '\n'){
      *o = '\r'; 
      o++;
    }
    *o = *i;
    o++; i++;
  }
  *o = '\0';

  /* XXX is there a way to raise a warning? */
  mexPrintf(msg2);
}





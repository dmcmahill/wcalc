/* $Id: cgi-common.c,v 1.6 2005/10/13 22:39:46 dan Exp $ */

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

/*
 * some routines that are common to the various cgi programs
 */


#include "config.h"

#include <stdarg.h>
#include <stdio.h>


#include "cgic.h"
#include "cgi-common.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

void bugMsg()
{
  fprintf(cgiOut,"<BR><EM>If you think this is a bug:</EM>&nbsp&nbsp\n");
  fprintf(cgiOut,"<BR>Please report it.  See <a href=\"http://wcalc.sf.net/bugs.html\">the\n"
	  "bug reporting section</a> of the main <a href=\"http://wcalc.sf.net\">\n"
	  "wcalc home page</a>.\n");
  fprintf(cgiOut,"<BR>Please include the exact URL, and exact error message.\n");
  fprintf(cgiOut,"<BR>\n");
}

void printFormError(const char *fmt,...)
{
  va_list ap;

  fprintf(cgiOut,"<BR><EM>INPUT ERROR:</EM>&nbsp&nbsp\n");

  va_start(ap,fmt);
  vfprintf(cgiOut,fmt,ap);
  va_end(ap);

  fprintf(cgiOut,"<BR>\n");
}

void inputErr(int *input_err)
{

  if (!*input_err) {
    cgiHeaderContentType("text/html");
    
    fprintf(cgiOut,"<DIV ALIGN=\"CENTER\"><P>"
	    "<EM>*** WARNING ***</EM></P>"
	    "<P>There are errors in your form entries.  "
	    "Please fix them and try again.</P></DIV>\n");
    bugMsg();
    *input_err=1;
  }
}

void fixInputMsg(void)
{
  fprintf(cgiOut,"<P>There were errors in your input values.  "
	  "Please hit the \"back\" button on your browser, "
	  "correct the errors, and resubmit the form."
	  "</P><HR>\n");
  
}

/* $Id: cgi-common.c,v 1.4 2002/05/10 22:52:26 dan Exp $ */

/*
 * Copyright (c) 2002 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
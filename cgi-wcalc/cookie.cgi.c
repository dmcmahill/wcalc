/* $Id: cookie.cgi.c,v 1.1 2002/01/18 14:21:12 dan Exp $ */

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
 *        This product includes software developed Dan McMahill
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
 * a simple example of cookies and cgi
 */

/* #define DEBUG */

#include "config.h"

#include <stdio.h>

/* CGI specific */
#include "cgic.h"
#include "cookie.h"

#define ACTION_LEN 20
#define VAL_LEN 20

static const char *name_string="cookie.cgi";

int cgiMain(void){

  /* CGI variables */
  int input_err = 0;
  char str_action[ACTION_LEN];
  char val[VAL_LEN];
  char nm[VAL_LEN];
  cgiCookieType *cookie;

  /* 
   * uncomment to be able to run in the debugger. 
   * access the CGI URL that gives the problem, then change foo.cgi to
   * capture.cgi and reload.  That dumps the env to /tmp/capcgi.dat.
   */
  /*cgiReadEnvironment("/tmp/capcgi.dat"); */
  
  /* create and set a fixed value cookie */
  cookie = cgiCookie_new("visited","yes i have");
  cgiCookie_MaxAge_set(cookie,60*60*24);
  cgiHeaderSetCookie(cookie);

  if(cgiFormStringNoNewlines("send",str_action,ACTION_LEN) ==
     cgiFormSuccess){
    /*
     * the "ENTER" button was pressed so get the value from the entry
     * and send it out as a cookie.
     */
    cgiFormStringNoNewlines("a",val,VAL_LEN);
    cgiFormStringNoNewlines("nm",nm,VAL_LEN);

    /* create a cookie from our form data */
    cookie = cgiCookie_new(nm,val);

    /* send out the cookie header */
    cgiCookie_MaxAge_max(cookie);
    cgiHeaderSetCookie(cookie);
  }
  /* Put out the rest of the header */
  cgiHeaderContentType("text/html");  

  /* and the HTML */
  fprintf(cgiOut,"<HTML>\n");
  fprintf(cgiOut,"<P>Your cookies (if any) are:<br>\n%s\n</P>",
	  cgiCookie);


  fprintf(cgiOut,"<FORM METHOD=GET ACTION=\"cookie.cgi\">\n"
	  "<P>Enter a name:\n"
	  "<INPUT TYPE=\"TEXT\" SIZE=%d NAME=\"nm\">\n"
	  "<P>Enter a value:\n"
	  "<INPUT TYPE=\"TEXT\" SIZE=%d NAME=\"a\">\n"
	  "<INPUT TYPE=\"SUBMIT\" NAME=\"send\" VALUE=\"ENTER\">\n"
	  "</P>\n",VAL_LEN,VAL_LEN);

  fprintf(cgiOut,"</HTML>\n");
  return 0;
}


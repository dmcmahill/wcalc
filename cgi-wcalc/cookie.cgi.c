/* $Id: cookie.cgi.c,v 1.5 2004/08/05 21:42:51 dan Exp $ */

/*
 * Copyright (C) 2002, 2004 Dan McMahill
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
 * a simple example of cookies and cgi
 */

/* #define DEBUG */

#include "config.h"

#include <stdio.h>

/* CGI specific */
#include "cgic.h"
#include "cookie.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ACTION_LEN 20
#define VAL_LEN 20

int cgiMain(void){

  /* CGI variables */
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
  cookie = cgiCookie_new("visited", "yes i have");
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


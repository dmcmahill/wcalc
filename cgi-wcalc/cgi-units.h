/* $Id: cgi-units.h,v 1.4 2004/07/22 13:06:23 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004 Dan McMahill
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

#ifndef __CGI_UNITS_H__
#define __CGI_UNITS_H__

/* for units stuff */
#include "misc.h"
#include "units.h"

typedef struct _ENTRY_LIST
{
  /* the name of the HTML entry */
  char *name;

  /* pointers to prev/next node in list */
  struct _ENTRY_LIST *next, *prev;
  
} entry_list;

typedef struct _CGI_MENU_LIST
{
  /* the menu */
  struct _CGI_UNITS_MENU *menu;

  /* pointers to prev/next node in list */
  struct _CGI_MENU_LIST *next, *prev;
  
} cgi_menu_list;

typedef struct _CGI_UNITS_MENU
{
  wc_units *units;

  /* the name of the HTML select */
  char name[80];

  /* list of HTML text entries which are attached to this menu */
  struct _ENTRY_LIST *ul;

} cgi_units_menu;

char * cgi_units_menu_show(const units_data *u, char *name, int ind);
char ** cgi_units_inductance(void);
char * cgi_units_menu_display(cgi_units_menu *menu);

cgi_units_menu * cgi_units_menu_new(wc_units *units);
void cgi_units_attach_entry(cgi_units_menu *menu, char *name);
char * cgi_units_menu_init();

#endif /* __CGI_UNITS_H__ */

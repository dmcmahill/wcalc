/* $Id: cgi-units.h,v 1.7 2004/08/02 22:11:55 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004 Dan McMahill
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

char * cgi_units_menu_show(const wc_units_data *u, char *name, int ind);
char ** cgi_units_inductance(void);
char * cgi_units_menu_display(cgi_units_menu *menu);

cgi_units_menu * cgi_units_menu_new(wc_units *units);
void cgi_units_attach_entry(cgi_units_menu *menu, char *name);
char * cgi_units_menu_init();

void cgi_units_menu_read(void);

#endif /* __CGI_UNITS_H__ */

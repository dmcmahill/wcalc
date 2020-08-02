
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <stdio.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* CGI specific headers */
#include "cgic.h"

/* libwcalc headers */
#include "alert.h"
#include "misc.h"
#include "units.h"

#include "cgi-common.h"
#include "cgi-units.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif


static cgi_menu_list * all_menus=NULL;

char * cgi_units_menu_show(const wc_units_data *units, char *name, int ind)
{
  int i;

  assert(units != NULL);

  if (units[0].name == NULL)
    return "";


  fprintf(cgiOut,"<SELECT NAME=\"%s\">\n",name);
  i = 0;
  while (units[i].name != NULL){
    fprintf(cgiOut,"<OPTION VALUE=\"%s\"",
	    units[i].name);

    if (i == ind)
      fprintf(cgiOut," SELECTED");

    fprintf(cgiOut,">%s\n",units[i].name);
    i++;
  }  
  fprintf(cgiOut,"</SELECT>\n");

  return "";
}

static void cgi_units_submenu(const wc_units_data *units, 
			      char * name, int index, int sub)
{
  int i;

  fprintf(cgiOut, "<SELECT NAME=\"%s_%d\" "
	  "onChange=\"changed_%s(); valsChanged();\">\n", name, sub, name);

  /* populate the menu */
  i = 0;
  
  while( units[i].name != NULL ) {
    fprintf(cgiOut,"    <OPTION VALUE=\"%s\"", units[i].name);

    if (i == index)
      fprintf(cgiOut," SELECTED ");

    fprintf(cgiOut,">%s\n", units[i].name);
    
    i++;
  }
  fprintf(cgiOut, "</SELECT>\n");

}

char * cgi_units_menu_display(cgi_units_menu *menu)
{
  int i, j;
  wc_units * units;
  char * name;

  assert(menu != NULL);

  units = menu->units;
  name = menu->name;

  assert(units != NULL);

  j = 0;
  /* Create the numerator menus */
  if (units->nnum == 0) {
    fprintf(cgiOut, "1");
  } else {
    for ( i = 0; i < units->nnum; i++) {

      cgi_units_submenu(units->num[i], name, units->numi[i], j++);

      if (i < (units->nnum - 1)) 
	fprintf(cgiOut, "-\n");
      
    }  
      
  }

  if (units->nden > 0) {
    fprintf(cgiOut, "/\n");
      
      for (i=0; i<units->nden; i++) {
	cgi_units_submenu(units->den[i], name, units->deni[i], j++);
	
	if (i < (units->nden - 1)) 
	  fprintf(cgiOut, "-\n");
      }
  }

  
  return "";
}


cgi_units_menu * cgi_units_menu_new(wc_units *units)
{
  static int mid=0;
  cgi_units_menu *um;
  cgi_menu_list *el;

  /* create the menu */
  if( (um = (cgi_units_menu *) malloc(sizeof(cgi_units_menu))) == NULL ) {
    fprintf(stderr,"cgi_units_menu_new():  malloc() failed\n");
    exit(1);
  }
  
  sprintf(um->name, "wc_unit_menu_%d", mid++);
  um->units = units;
  um->ul = NULL;

  /* add to the master list */
  if( all_menus == NULL ) {
    if( (all_menus = (cgi_menu_list *) malloc(sizeof(cgi_menu_list))) == NULL ) {
      fprintf(stderr,"cgi_units_menu_new():  malloc() failed\n");
      exit(1);
    }
    all_menus->prev = NULL;
    all_menus->next = NULL;
    all_menus->menu = um;
  } else {
    el = all_menus;
    while(el->next != NULL)
      el = el->next;

    if( (el->next = (cgi_menu_list *) malloc(sizeof(cgi_menu_list))) == NULL ) {
      fprintf(stderr,"cgi_units_menu_new():  malloc() failed\n");
      exit(1);
    }
    el->next->prev = el;
    el->next->next = NULL;
    el->next->menu = um;
  }

  return um;
}

void cgi_units_attach_entry(cgi_units_menu *menu, char *name)
{
  entry_list *el;

  if( menu->ul == NULL ) {
    if( (menu->ul = (entry_list *) malloc(sizeof(entry_list))) == NULL ) {
      fprintf(stderr,"cgi_units_attach_entry():  malloc() failed\n");
      exit(1);
    }
    menu->ul->prev = NULL;
    menu->ul->next = NULL;
    menu->ul->name = name;
  } else {
    el = menu->ul;
    while(el->next != NULL)
      el = el->next;

    if( (el->next = (entry_list *) malloc(sizeof(entry_list))) == NULL ) {
      fprintf(stderr,"cgi_units_attach_entry():  malloc() failed\n");
      exit(1);
    }
    el->next->prev = el;
    el->next->next = NULL;
    el->next->name = name;
  }

}

/* 
 * cgi_units_menu_init()
 *
 * This function spits out JavaScript code in the HTML document head
 * to define menu changed callbacks.
 */
char * cgi_units_menu_init()
{
  cgi_menu_list *ml=all_menus;
  entry_list *el;
  wc_units * units;
  int i,j;
  
  fprintf(cgiOut, "<SCRIPT LANGUAGE=\"LiveScript\">\n");
  fprintf(cgiOut, "<!-- hide this script tag's contents from old browsers\n\n");

  fprintf(cgiOut, "\nfunction valsChanged()\n{\n");
  fprintf(cgiOut, "\tdocument.wcalc.status.value = 'Values Out of Sync';\n");
  fprintf(cgiOut, "}\n\n");

  fprintf(cgiOut, "\nfunction register(cgi, cookie) {\n");
  fprintf(cgiOut, "\tvar today = new Date();\n");
  fprintf(cgiOut, "\tvar expires = new Date();\n");
  fprintf(cgiOut, "\texpires.setTime(today.getTime() + 1000*60*60*24*365);\n");
  fprintf(cgiOut, "\tsetCookie(cgi, cookie, expires);\n");
  fprintf(cgiOut, "}\n\n");


  while( ml != NULL ) {
    fprintf(cgiOut, "function changed_%s()\n", ml->menu->name);
    fprintf(cgiOut, "{\n");
    el = ml->menu->ul;
    units = ml->menu->units;
    
    fprintf(cgiOut, "\tvar i;\n");
    fprintf(cgiOut, "\tvar tmps;\n");

    fprintf(cgiOut, "\ttmps = '';\n");

    j = 0;
    /* Emit the numerator terms */
    if (units->nnum == 0) {
      fprintf(cgiOut, "\ttmps = tmps + '1';\n");
    } else {
      for ( i = 0; i < units->nnum; i++) {
	
	fprintf(cgiOut, "\ti = document.wcalc.%s_%d.selectedIndex;\n", 
		ml->menu->name, j);
	fprintf(cgiOut, "\ttmps = tmps + document.wcalc.%s_%d[i].value;\n", 
		ml->menu->name, j);
	j++;

	if (i < (units->nnum - 1)) 
	  fprintf(cgiOut, "\ttmps = tmps + '-';\n");
	
      }  
      
    }
    
    /* Emit the denominator terms */
    if (units->nden > 0) {
      fprintf(cgiOut, "\ttmps = tmps + '/';\n");
      
      for (i=0; i<units->nden; i++) {
	fprintf(cgiOut, "\ti = document.wcalc.%s_%d.selectedIndex;\n",
		ml->menu->name, j);
	fprintf(cgiOut, "\ttmps = tmps + document.wcalc.%s_%d[i].value;\n",
		ml->menu->name, j);
	j++;
	
	if (i < (units->nden - 1)) 
	  fprintf(cgiOut, "\ttmps = tmps + '-';\n");
      }
    }

    while( el != NULL ) {
      fprintf(cgiOut, "\tdocument.wcalc.%s.value = tmps;\n", el->name);
      
      el = el->next;
    }
    fprintf(cgiOut, "}\n\n");

    ml = ml->next;
  }

  /*
   * Emit a function which will tickle all the SELECT changed
   * functions when the form is loaded.  This will initialize the
   * entries which track various menus.
   */

  fprintf(cgiOut, "function units_init()\n{\n");
  fprintf(cgiOut, "\tdocument.wcalc.status.value = 'Values In Sync';\n");
  
  ml = all_menus;
  while( ml != NULL ) {
    fprintf(cgiOut, "\tchanged_%s();\n", ml->menu->name);
    ml = ml->next;
  }
  fprintf(cgiOut, "}\n\n");

  fprintf(cgiOut, "-->\n</SCRIPT>\n");

  return "";
}

/* 
 * cgi_units_menu_read()
 *
 * Read in the units menus on the form all at once.
 *
 * This should be used at the beginning of the cgi program so units
 * are available for loading in the numeric values
 */
void cgi_units_menu_read(void)
{
  cgi_menu_list *ml=all_menus;
  wc_units * units;
  int i,j;
  /* XXX */
  char tmps[80];
  int input_err = 0;
  
  while( ml != NULL ) {
    units = ml->menu->units;

    j = 0;
    /* Read the numerator terms */
    for ( i = 0; i < units->nnum; i++) {
      sprintf(tmps, "%s_%d", ml->menu->name, j++);

      /* read in the cgi form */
      if (cgiFormSelectSingle(tmps, 
			      wc_units_strings_get(units->num[i]),
			      wc_units_size(units->num[i]),
			      &units->numi[i], 0) != cgiFormSuccess){
	inputErr(&input_err);
	printFormError("Had problems reading the %s numerator\n", tmps);
      }  
    }
      
    
    /* Read the denominator terms */
    for ( i = 0; i < units->nden; i++) {
      sprintf(tmps, "%s_%d", ml->menu->name, j++);

      /* read in the cgi form */
      if (cgiFormSelectSingle(tmps, 
			      wc_units_strings_get(units->den[i]),
			      wc_units_size(units->den[i]),
			      &units->deni[i], 0) != cgiFormSuccess){
	inputErr(&input_err);
	printFormError("Had problems reading the %s denominator\n", tmps);
      }  
    }

    /* and sync the string and scale factor */
    if( units->name != NULL )
      free(units->name);
    units->name = wc_units_to_str(units);
    units->sf = wc_units_to_sf(units);
    
    ml = ml->next;
  }

}

/* $Id: gtk-units.c,v 1.2 2002/02/21 02:09:48 dan Exp $ */

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

#define DEBUG

#include "config.h"

#include <gtk/gtk.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "alert.h"

/* libwcalc related */
#include "misc.h"
#include "physconst.h"

/* GTK-wcalc related */
#include "gtk-units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

GtkWidget *wc_composite_units_menu_new(const composite_units_data *units, 
				       Wcalc *gui,
				       void (*callback)(GtkWidget *, gpointer))
{
  GtkWidget *hbox;
  GtkWidget *item;
  int i;
  wc_units_gui *ug;


  if ( (ug = malloc(sizeof(wc_units_gui))) == NULL ) {
    fprintf(stderr,"wc_composite_units_menu_new():  malloc() failed\n");
    exit(1);
  }

  ug->units=units;
  ug->menu_num=NULL;
  ug->menu_den=NULL;

  hbox = gtk_hbox_new(FALSE,0);

  /* the numerator */
  if (units->nnum == 0) {
      item = gtk_label_new("1");
      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
  }
  else {
    for (i=0; i<units->nnum; i++) {
      item = units_menu_new(units->num[i],0,gui,callback);

      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);

      /* add to our list of numerator menus */
      g_list_append(ug->menu_num,item);
      
      if (i < (units->nnum - 1)) {
	item = gtk_label_new("-");
	gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
      }
    }
  }

  if (units->nden > 0) {
      item = gtk_label_new("/");
      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);

      /* add to our list of denominator menus */
      g_list_append(ug->menu_den,item);
      
      for (i=0; i<units->nden; i++) {
	item = units_menu_new(units->den[i],0,gui,callback);
	gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
	
	if (i < (units->nden - 1)) {
	  item = gtk_label_new("-");
	  gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
	}
      }   
  }

  gtk_widget_show_all(hbox);
  
  return hbox;
}


static void wc_composite_units_menu_changed( GtkWidget *w, gpointer data)
{
  int which;
  //  coax_gui *gui;

  // gui = WC_COAX_GUI(data);
  //g_list_nth_data(ug->menu_num,action);

  which = GPOINTER_TO_INT(gtk_object_get_user_data(GTK_OBJECT(w)));
#ifdef DEBUG
  g_print("wc_composite_units_menu_changed():  \n");
#endif

}

#ifdef notdef
static void wc_composite_units_menu_update( coax_gui *gui,int which)
{
  gtk_option_menu_set_history(GTK_OPTION_MENU(gui->menu_abct_units), which);

  gtk_label_set_text(GTK_LABEL(gui->units_b),length_units[which].name);
  gtk_label_set_text(GTK_LABEL(gui->units_c),length_units[which].name);
  gtk_label_set_text(GTK_LABEL(gui->units_t),length_units[which].name);
  gui->line->a_sf = length_units[which].sf;
  gui->line->b_sf = length_units[which].sf;
  gui->line->c_sf = length_units[which].sf;
  gui->line->tshield_sf = length_units[which].sf;

  gui->line->a_units = length_units[which].name;
  gui->line->b_units = length_units[which].name;
  gui->line->c_units = length_units[which].name;
  gui->line->tshield_units = length_units[which].name;

}
#endif

GtkWidget *units_menu_new(const units_data *units, 
			  int initial,
			  Wcalc *gui,
			  void (*callback)(GtkWidget *, gpointer))
{
  GtkWidget *opt_menu;
  GtkWidget *menu;
  GtkWidget *item;
  int i;

  opt_menu = gtk_option_menu_new();
  menu = gtk_menu_new();

  i=0;
  while (units[i].name != NULL) {
    item = gtk_menu_item_new_with_label(units[i].name);
    gtk_signal_connect(GTK_OBJECT(item), "activate",
		       GTK_SIGNAL_FUNC(callback), 
		       (gpointer) gui);
    gtk_object_set_user_data(GTK_OBJECT(item),GINT_TO_POINTER(i));
    gtk_menu_append(GTK_MENU(menu), item);
    gtk_widget_show(item);

    i++;
  }

  /* attach our new menu to the options menu */
  gtk_option_menu_set_menu(GTK_OPTION_MENU(opt_menu), menu);

  /* pick the default (initial) selection */
  gtk_option_menu_set_history(GTK_OPTION_MENU(opt_menu), initial);

  gtk_widget_show_all(opt_menu);
  
  return opt_menu;
}

void  set_sf_menu(GtkWidget *menu, const units_data units[],double sf)
{
  int i;
  int found_sf=0;

  i=0;
  while (units[i].name != NULL){
    if (units[i].sf == sf) {
      found_sf=1;
      break;
    }
    i++;
  }  

  if (!found_sf) {
    fprintf(stderr,"set_sf_menu():  error.  could not locate sf=%g\n",sf);
    exit(1);
  }

  /* set the menu */
#ifdef DEBUG
  g_print("set_sf_menu():  set menu (%p) to option %d (sf=%g)\n",
	  (void *) menu,i,sf);
#endif

  gtk_option_menu_set_history(GTK_OPTION_MENU(menu), i);
  
}

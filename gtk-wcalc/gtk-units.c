/* $Id: gtk-units.c,v 1.7 2002/06/28 22:59:59 dan Exp $ */

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

#define DEBUG

/* #define DEBUG2 */

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


static wc_units_menu_data *units_menu_data_new(int ind);

GtkWidget *wc_composite_units_menu_new(const composite_units_data *units, 
				       Wcalc *gui,
				       wc_units_gui **ug,
				       void (*callback)(GtkWidget *,gpointer)
				       )
{
  GtkWidget *hbox;
  GtkWidget *item;
  int i;


  if ( ((*ug) = malloc(sizeof(wc_units_gui))) == NULL ) {
    fprintf(stderr,"wc_composite_units_menu_new():  malloc() failed\n");
    exit(1);
  }

  (*ug)->units=units;
  (*ug)->menu_num=NULL;
  (*ug)->menu_den=NULL;
  (*ug)->update_list=NULL;

  hbox = gtk_hbox_new(FALSE,0);
#ifdef DEBUG
      g_print("wc_composite_units_menu_new():  created new wc_units_gui = %p\n",(*ug));
      g_print("wc_composite_units_menu_new():      units = %p\n",(*ug)->units);
      g_print("wc_composite_units_menu_new():  created hbox = %p\n",hbox);
#endif

  /* the numerator */
  if (units->nnum == 0) {
      item = gtk_label_new("1");
      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
  }
  else {
    for (i=0; i<units->nnum; i++) {
      /* item = units_menu_new(units->num[i],0,gui,callback); */
      item = units_menu_new(units->num[i],0,(*ug),callback);

      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);

      /* add to our list of numerator menus */
      (*ug)->menu_num = g_list_append((*ug)->menu_num,item);
#ifdef DEBUG2
      g_print("wc_composite_units_menu_new():  added numerator item[%d] = %p to ug %p\n",i,item,*ug);
      g_print("wc_composite_units_menu_new():        numerator list     = %p\n",(*ug)->menu_num);
#endif
      
      if (i < (units->nnum - 1)) {
	item = gtk_label_new("-");
	gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
      }
    }
  }

  if (units->nden > 0) {
      item = gtk_label_new("/");
      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
      
      for (i=0; i<units->nden; i++) {
	/* item = units_menu_new(units->den[i],0,gui,callback); */
	item = units_menu_new(units->den[i],0,(*ug),callback);
	gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
	/* add to our list of denominator menus */
	(*ug)->menu_den = g_list_append((*ug)->menu_den,item);
#ifdef DEBUG
	g_print("wc_composite_units_menu_new():  added denominator item[%d] = %p to ug %p\n",i,item,*ug);
	g_print("wc_composite_units_menu_new():        denominator list     = %p\n",(*ug)->menu_den);
#endif
	if (i < (units->nden - 1)) {
	  item = gtk_label_new("-");
	  gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
	}
      }   
  }

  gtk_widget_show_all(hbox);
  
  return hbox;
}


void wc_composite_units_menu_changed( GtkWidget *w, gpointer data)
{
  int which,ind;
  wc_units_gui *ug;
  wc_units_menu_data *menu_data;
  guint i;
  wc_units_update_item *up_item;

  /* XXX there must be a better way than fixing this size... */
  char str[80];

#ifdef DEBUG
  char *ustr;
  double sf;
#endif

  ug = (wc_units_gui *) data;
  /* gui = WC_COAX_GUI(data);
     g_list_nth_data(ug->menu_num,action); */
  menu_data = WC_UNITS_MENU_DATA(gtk_object_get_user_data(GTK_OBJECT(w)));
  which = menu_data->ind;
  
#ifdef DEBUG
    g_print("wc_composite_units_menu_changed():  ug = %p\n", ug);
    g_print("wc_composite_units_menu_changed():  ug->units = %p\n", ug->units);
#endif
  
  if ( (ind = g_list_index(ug->menu_num,menu_data->opt_menu)) != -1) {
#ifdef DEBUG
    g_print("wc_composite_units_menu_changed():  ug %p changed numerator[%d] units to %d\n", ug, ind,which);
    g_print("wc_composite_units_menu_changed():  ug->units->numi[%d] = %d\n", ind, ug->units->numi[ind]);
#endif
    ug->units->numi[ind] = which;
  }
  else if ( (ind = g_list_index(ug->menu_den,menu_data->opt_menu)) != -1) {
#ifdef DEBUG
    g_print("wc_composite_units_menu_changed():  ug %p changed denominator[%d] units\n", ug, ind);
#endif
    ug->units->deni[ind] = which;
  }
  else {
    fprintf(stderr,"wc_composite_units_menu_changed():  ug %p could not locate menu\n",ug);
    exit(1);
  }

#ifdef DEBUG
  g_print("wc_composite_units_menu_changed():      calling units_update(%p,%p,%p)\n",
	  ug->units,&sf,&ustr);
  units_update(ug->units,&sf,&ustr);
  g_print("wc_composite_units_menu_changed():      units string = \"%s\", scale factor = %g\n",
	  ustr,sf);
#endif

  /*
   * Now update all the labels and text entries which may be tied to
   * these units 
   */

  for (i=0; i<g_list_length(ug->update_list); i++) {
    up_item = (wc_units_update_item *) g_list_nth_data(ug->update_list,i);

    /* update the scale factor and units string for this item */
    units_update(ug->units,up_item->sf,&(up_item->units_str));
#ifdef DEBUG
    g_print("wc_composite_units_menu_changed():      updated item %u units string = \"%s\", scale factor = %g\n",
	    i,up_item->units_str,*(up_item->sf));
#endif
    
    /* only update the displayed widget if the update flag is set */
    if( (up_item->update) && (up_item->widget != NULL) ) {

      if(up_item->fmt_string != NULL) {
	sprintf(str,up_item->fmt_string,(*(up_item->mks_val))/(*(up_item->sf)));
      }
      else {
	sprintf(str,"--ERR--");
      }

      switch( up_item->type ) {
      case LABEL:
	gtk_label_set_text(GTK_LABEL(up_item->widget),str);
	break ;
	
      case ENTRY:
	gtk_entry_set_text(GTK_ENTRY(up_item->widget),str);
      break ;
      
      case UNITS_LABEL:
	gtk_label_set_text(GTK_LABEL(up_item->widget),up_item->units_str);
      break ;
      
      default:
	fprintf(stderr,"wc_composite_units_menu_changed():  invalid up_item->type = %d\n",
		up_item->type);
	exit(1);
      }
    }

  }

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

static wc_units_menu_data *wc_units_menu_data_new(int ind)
{
  wc_units_menu_data *data;

  if ( (data = (wc_units_menu_data *) malloc(sizeof(wc_units_menu_data))) == NULL) {
    fprintf(stderr,"wc_units_menu_data():  malloc() failed\n");
    exit(1);
  }

  data->ind = ind;

  return data;
}

GtkWidget *units_menu_new(const units_data *units, 
			  int initial,
			  gpointer gui,
			  void (*callback)(GtkWidget *, gpointer))
{
  GtkWidget *opt_menu;
  GtkWidget *menu;
  GtkWidget *item;
  wc_units_menu_data *data;

  int i;

  opt_menu = gtk_option_menu_new();
  menu = gtk_menu_new();

#ifdef DEBUG
  g_print("units_menu_new():  opt_menu = %p\n",opt_menu);
  g_print("units_menu_new():  menu     = %p\n",menu);
#endif

  i=0;
  while (units[i].name != NULL) {
    item = gtk_menu_item_new_with_label(units[i].name);
#ifdef DEBUG
    /*    g_print("units_menu_new():  item[%d] = %p\n",i,item); */
#endif
    gtk_signal_connect(GTK_OBJECT(item), "activate",
		       GTK_SIGNAL_FUNC(callback), 
		       (gpointer) gui);
    data = wc_units_menu_data_new(i);
    data->opt_menu = opt_menu;
    gtk_object_set_user_data(GTK_OBJECT(item),(gpointer *) data);
    /* `menu' becomes the parent of each `item' */
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



void  wc_composite_units_attach(wc_units_gui *ug, 
				GtkWidget *widget, 
				double *mks_val, 
				double *sf,
				char *units_str, 
				const char *fmt_string,
				int update,
				int type)
{
  wc_units_update_item *item;

#ifdef DEBUG
  g_print("wc_composite_units_attach():\n");
#endif

  item = g_malloc(sizeof(wc_units_update_item));

  /* fill in the struct */
  item->widget = widget;
  item->mks_val = mks_val;
  item->sf = sf;
  item->units_str = units_str;
  item->fmt_string = strdup(fmt_string);
  item->update = update;
  item->type = type;

  /* add to the list in the wc_units_gui */
  ug->update_list = g_list_append(ug->update_list, item);
  
#ifdef DEBUG
  g_print("wc_composite_units_attach():  Added %p to ug=%p, list=%p\n",item,ug,ug->update_list);
#endif
  
  return ;
}

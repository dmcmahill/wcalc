/* $Id: gtk-units.c,v 1.18 2004/07/25 04:15:09 dan Exp $ */

/*
 * Copyright (c) 2002, 2003, 2004 Dan McMahill
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

/* #define DEBUG */

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

static GtkWidget *wc_units_submenu_new(Wcalc *wcgui,
				       const wc_units_data *units, 
				       int initial,
				       gpointer gui,
				       void (*callback)(GtkWidget *, gpointer));

static wc_units_menu_data *wc_units_menu_data_new(int ind);

static void wc_units_menu_init1(void * item, void * data);
static void wc_units_menu_init2(void * item, void * data);

void wc_units_menu_init(Wcalc *wc)
{

  g_list_foreach( wc->units_menu_list,
		  (GFunc) wc_units_menu_init1,
		  NULL );
}

void wc_units_menu_init1(void * item, void * data)
{
  wc_units_gui *ug;

  ug = (wc_units_gui *) item;

#ifdef DEBUG
  printf("wc_units_menu_init1():  ug = %p\n", ug);
#endif

  /*
   * iterate over each of the options menus for the various terms in
   * the units menu.
  */
  g_list_foreach( ug->menu_num,
		  (GFunc) wc_units_menu_init2,
		  item );

  g_list_foreach( ug->menu_den,
		  (GFunc) wc_units_menu_init2,
		  item );
}

void wc_units_menu_init2(void * item, void * data)
{
  GtkWidget *w;
  GtkWidget *m;
  GtkWidget *menu_item;
  wc_units_gui *ug;

  assert( item != NULL );
  assert( data != NULL );

  ug = (wc_units_gui *) data;
#ifdef DEBUG
  printf("wc_units_menu_init2():  ug = %p\n", ug);
  printf("wc_units_menu_init2():  item = %p\n", item);
  printf("wc_units_menu_init2():  data = %p\n", data);
#endif

  w = GTK_WIDGET( item );
#ifdef DEBUG
  g_print("wc_units_menu_init2():  w = %p\n", w);
#endif

  /* pick out the menu */
  m = gtk_option_menu_get_menu(GTK_OPTION_MENU(w));
#ifdef DEBUG
  g_print("wc_units_menu_init2():  m = %p\n", m);
#endif

  /* figure out which one is active */
  menu_item = gtk_menu_get_active(GTK_MENU( m ));
#ifdef DEBUG
  g_print("wc_units_menu_init2():  menu_item = %p\n", menu_item);
#endif

#ifdef DEBUG
  g_print("wc_units_menu_init2():  Running calback.  "
	  "item = %p, data = %p, m = %p, "
	  "menu_item = %p\n",
	  item, data, m, menu_item);
#endif

  /* run the callback */
  wc_units_menu_changed( menu_item, (gpointer) ug);

}

/*
 * wc_units_menu_new()
 *
 */
GtkWidget *wc_units_menu_new(const wc_units *units, 
			     Wcalc *gui,
			     wc_units_gui **ug
			     )
{
  GtkWidget *hbox;
  GtkWidget *item;
  int i;
  

  if ( ((*ug) = malloc(sizeof(wc_units_gui))) == NULL ) {
    fprintf(stderr,"wc_units_menu_new():  malloc() failed\n");
    exit(1);
  }

  (*ug)->units = units;
  (*ug)->menu_num = NULL;
  (*ug)->menu_den = NULL;
  (*ug)->update_list = NULL;

  /* append to our list of units GUI's */
  gui->units_menu_list =  g_list_append(gui->units_menu_list, *ug);
  
  /* create the container for the complete units menu */
  hbox = gtk_hbox_new(FALSE,0);
#ifdef DEBUG
  g_print("wc_units_menu_new():  created new wc_units_gui = %p\n", (*ug));
  g_print("wc_units_menu_new():  units                    = %p\n", (*ug)->units);
  g_print("wc_units_menu_new():  created hbox             = %p\n", hbox);
#endif
  
  /* Create the numerator menus */
  if (units->nnum == 0) {
      item = gtk_label_new("1");
      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
  }
  else {
    for ( i = 0; i < units->nnum; i++) {
      item = wc_units_submenu_new(gui, units->num[i], units->numi[i], (*ug), 
				  wc_units_menu_changed);

      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);

      /* add to our list of numerator menus */
      (*ug)->menu_num = g_list_append((*ug)->menu_num, item);
#ifdef DEBUG2
      g_print("wc_units_menu_new():  added numerator item[%d] = %p to ug %p\n",
	      i, item, *ug);
      g_print("wc_units_menu_new():        numerator list     = %p\n",
	      (*ug)->menu_num);
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
	item = wc_units_submenu_new(gui, units->den[i], units->deni[i], (*ug), 
				    wc_units_menu_changed);
	gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);

	/* add to our list of denominator menus */
	(*ug)->menu_den = g_list_append((*ug)->menu_den, item);
#ifdef DEBUG
	g_print("wc_units_menu_new():  added denominator item[%d] = %p to ug %p\n",i,item,*ug);
	g_print("wc_units_menu_new():        denominator list     = %p\n",(*ug)->menu_den);
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


void wc_units_menu_changed( GtkWidget *w, gpointer data)
{
  int which, ind;
  wc_units_gui *ug;
  wc_units_menu_data *menu_data;
  guint i;
  wc_units_update_item *up_item;

  /* XXX there must be a better way than fixing this size... */
  char str[80];
  double sf;
  static char *units_str = NULL;

  ug = (wc_units_gui *) data;
  /* gui = WC_COAX_GUI(data);
     g_list_nth_data(ug->menu_num,action); */
  menu_data = WC_UNITS_MENU_DATA(gtk_object_get_user_data(GTK_OBJECT(w)));
  which = menu_data->ind;
  
#ifdef DEBUG
    g_print("wc_units_menu_changed():  ug = %p\n", ug);
    g_print("wc_units_menu_changed():  ug->units = %p\n", ug->units);
#endif
  
  if ( (ind = g_list_index(ug->menu_num,menu_data->opt_menu)) != -1) {
#ifdef DEBUG
    g_print("wc_units_menu_changed():  ug %p changed numerator[%d] units to %d\n", 
	    ug, ind,which);
    g_print("wc_units_menu_changed():  ug->units->numi[%d] = %d\n",
	    ind, ug->units->numi[ind]);
#endif
    ug->units->numi[ind] = which;
  }
  else if ( (ind = g_list_index(ug->menu_den,menu_data->opt_menu)) != -1) {
#ifdef DEBUG
    g_print("wc_units_menu_changed():  ug %p changed denominator[%d] units\n", 
	    ug, ind);
#endif
    ug->units->deni[ind] = which;
  }
  else {
    fprintf(stderr,"wc_units_menu_changed():  ug %p could not locate menu\n",
	    ug);
    exit(1);
  }

  sf = wc_units_to_sf(ug->units);
  if(units_str != NULL)
    free(units_str);
  units_str = wc_units_to_str(ug->units);

  /* update scale factor and name in the units */
  ug->units->sf = sf;
  if(ug->units->name != NULL)
    free(ug->units->name);
  ug->units->name = wc_units_to_str(ug->units);

#ifdef DEBUG
  g_print("wc_units_menu_changed():  Units string = \"%s\", scale factor = %g\n",
	  units_str, sf);
#endif

  /*
   * Now update all the labels and text entries which may be tied to
   * these units 
   */

  for (i=0; i<g_list_length(ug->update_list); i++) {
    up_item = (wc_units_update_item *) g_list_nth_data(ug->update_list,i);

#ifdef DEBUG
    g_print("wc_units_menu_changed():      processing item %u, type %d\n",
	    i,up_item->type);
#endif

    /* update the scale factor and units string for this item */
    if (up_item->sf != NULL)
      *(up_item->sf)=sf;
    if (up_item->units_str != NULL)
      *(up_item->units_str) = strdup(units_str);

    /* only update the displayed widget if the update flag is set */
    if( (up_item->update) && (up_item->widget != NULL) ) {
#ifdef DEBUG
      g_print("wc_units_menu_changed():    Updating widget\n");
#endif
      if(up_item->fmt_string != NULL) {
	sprintf(str, up_item->fmt_string, (*(up_item->mks_val))/sf);
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
	gtk_label_set_text(GTK_LABEL(up_item->widget),units_str);
      break ;
      
      default:
	fprintf(stderr,"wc_units_menu_changed():  invalid up_item->type = %d\n",
		up_item->type);
	exit(1);
      }
    }
    else {
#ifdef DEBUG
      g_print("wc_units_menu_changed():    not updating widget\n");
#endif
    }

  }

}

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

static GtkWidget *wc_units_submenu_new(Wcalc *wcgui,
				       const wc_units_data *units, 
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
  g_print("wc_units_submenu_new():  opt_menu = %p\n", opt_menu);
  g_print("wc_units_submenu_new():  menu     = %p\n", menu);
  g_print("wc_units_submenu_new():  gui      = %p\n", gui);
#endif

  i=0;
  while (units[i].name != NULL) {
    item = gtk_menu_item_new_with_label(units[i].name);
#ifdef DEBUG
    /*    g_print("wc_units_submenu_new():  item[%d] = %p\n",i,item); */
#endif
    /*
     * XXX would be nice to only have the wcalc_save_needed get
     * tickled if the menu actually changes instead of selecting the
     * same choice again
     */
    gtk_signal_connect (GTK_OBJECT(item),
			"activate",
			GTK_SIGNAL_FUNC (wcalc_save_needed), 
			(gpointer) wcgui);
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

/*
 * wc_units_attach()
 * 
 * This function attaches various gtk widgets to a 
 * wc_units_gui.  This lets the wc_units_gui know what actions may
 * need to be taken in its callbacks.  For example, you can attach a
 * label and specify that it will get updated to reflect a new choice
 * of units.  Ie, if you change a wc_units_gui unit from 'cm' to 'mm'
 * you can specify that several attached labels should be converted to
 * 'mm' and updated.  
 *
 * The supported attachements are (see gtk-units.h for the most up to
 * date list):
 *
 * LABEL        =  a gtk_label which should hold a string showing a
 *                 value which is in the units specified.
 *
 * ENTRY        =  a gtk_entry which should hold a string showing a
 *                 value which is in the units specified.
 *
 * UNITS_LABEL  =  a gtk_label which should hold a string reporting
 *                 the current units.
 *
 * NONE         =
 *
 * For these attachments, the 'update' flag when set specifies that
 * the callback for the units menu will update the attached object.
 * When the 'update' flag is not set, no update is performed.  This
 * allows you to type in "4.7" in an entry and then realize you needed
 * to set the units to "mil" instead of "cm".  With update=0, you can
 * make the change and the "4.7" stays.  With update=1, the 4.7 cm
 * will be converted to 4.7 mil.
 * 
 * The other arguments are:
 *
 * double *mks_val;
 *    This is a pointer to the numerical value in
 *    MKS units.  When a LABEL or ENTRY is being updated to reflect
 *    new units, *mks_val is consulted.
 *
 * double *sf;
 *    This is a pointer to the scale factor in units/mks_units.
 *    When the units are changed, *sf is updated to reflect the new
 *    units.
 *
 * char **units_str;
 *    This is a pointer to a string containing the currently selected
 *    units.  When the units are changed, **units_str is updated to
 *    reflect the new units.
 *
 * const char *fmt_string;
 *    This is used as the format specifier for converting the
 *    numerical value to the string used by the LABEL and ENTRY.
 *
 * int update;
 *    When set, the attached object will be updated when the units
 *    change.
 *
 * int type;
 *    One of {LABEL, ENTRY, UNITS_LABEL, NONE}
 */

void  wc_units_attach(wc_units_gui *ug, 
				GtkWidget *widget, 
				double *mks_val, 
				double *sf,
				char **units_str, 
				const char *fmt_string,
				int update,
				int type)
{
  wc_units_update_item *item;

#ifdef DEBUG
  g_print("wc_units_attach():  attaching a ");
  switch( type ) {
  case LABEL:
    g_print("LABEL");
    break ;
    
  case ENTRY:
    g_print("ENTRY");
    break ;
    
  case UNITS_LABEL:
    g_print("UNITS_LABEL");
    break ;
    
  case NONE:
    g_print("NONE");
    break ;
    
  default:
    g_print("unknown type (%d)",type);
  }
  g_print("\n");
#endif

  item = g_malloc(sizeof(wc_units_update_item));

  /* fill in the struct */
  item->widget = widget;
  item->mks_val = mks_val;
  item->sf = sf;
  item->units_str = units_str;
  if (fmt_string != NULL) {
    item->fmt_string = strdup(fmt_string);
  }
  else {
    item->fmt_string = NULL;
  }
  item->update = update;
  item->type = type;

  /* add to the list in the wc_units_gui */
  ug->update_list = g_list_append(ug->update_list, item);
  
#ifdef DEBUG
  g_print("wc_units_attach():  Added %p to ug=%p, list=%p\n",item,ug,ug->update_list);
#endif
  
  return ;
}

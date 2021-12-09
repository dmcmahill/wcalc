/*
 * Copyright (C) 2002, 2003, 2004, 2021 Dan McMahill
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

static void wc_print_glist(void * item, void * data)
{
  fprintf(stderr, "       item (combo) = %p, data (ug) = %p\n", item, data);
}

/*
 * this is called at the end of creating and initializing a gui.
 * At this point we have already created all of the wc_units_gui's
 */
void wc_units_menu_init(Wcalc *wc)
{

  /*
   * Iterate over *all* units menus in the GUI.  This is a list of
   *    pointers to wc_units_gui's
   */
  g_list_foreach( wc->units_menu_list,
		  (GFunc) wc_units_menu_init1,
		  NULL );
}


/*
 * This processes a wc_units_gui.
 * item - pointer to a wc_units_gui
 * data - NULL
 */
static void wc_units_menu_init1(void * item, void * data)
{
  wc_units_gui *ug;

  ug = WC_UNITS_GUI (item);

#ifdef DEBUG
  printf("wc_units_menu_init1():  ug = %p\n", ug);
#endif

  /*
   * iterate over each of the options menus for the various terms in
   * the units menu.  In this case we are letting each element in the
   * units gui know who the wc_units_gui is by passing in item.
   */

  /* terms in the numerator */
  g_list_foreach( ug->menu_num,
		  (GFunc) wc_units_menu_init2,
		  item );

  /* terms in the denominator */
  g_list_foreach( ug->menu_den,
		  (GFunc) wc_units_menu_init2,
		  item );
}

static void wc_units_menu_init2(void * item, void * data)
{
  GtkWidget *w;
  wc_units_gui *ug;

  assert( item != NULL );
  assert( data != NULL );

  ug = WC_UNITS_GUI (data);
  w = GTK_WIDGET( item );

#ifdef DEBUG
  g_print("%s():  ug = %p\n", __FUNCTION__, ug);
  g_print("%s():  item = %p\n", __FUNCTION__, item);
  g_print("%s():  data = %p\n", __FUNCTION__, data);
  g_print("%s():  w = %p\n", __FUNCTION__, w);
#endif

  /* run the callback */
  wc_units_menu_changed( w, (gpointer) ug);

  return;

}

/*
 * wc_units_menu_new()
 *
 * This creates a new units menu for the GUI and adds to the
 * list of menus that the GUI maintains.
 *
 * units - is a libwcalc wc_units pointer.  This has the info about
 *         what the units are.
 *
 * gui - the GUI we are adding a units menu to.
 *
 * ug - pointer to a pointer to a units gui.  *ug will be modified
 *      to point to our new wc_units_gui.
 *
 * Returns the GtkWidget that should be added to the gui in some
 * spot.
 *
 */
GtkWidget *wc_units_menu_new(wc_units *units,
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
    /*
     * nothing in the numerator so this should be something like
     * 1/Ohms  when we are done.  That is why we are adding a "1"
     * in the numerator.
     */
#ifdef DEBUG
    g_print("%s():  Adding 1 to the numerator\n", __FUNCTION__);
#endif
    item = gtk_label_new("1");
    gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);
  } else {
    /*
     * there is something in the numerator so build up each part
     * for things like uOhm-mm
     */
    for ( i = 0; i < units->nnum; i++) {
#ifdef DEBUG
      g_print("%s():  Adding %p index %d to the numerator\n", __FUNCTION__, units->num[i], units->numi[i]);
#endif
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

  /* Create the denominator menus */
  if (units->nden > 0) {
      item = gtk_label_new("/");
      gtk_box_pack_start (GTK_BOX (hbox), item, 0, 0, 0);

      for (i=0; i<units->nden; i++) {
#ifdef DEBUG
      g_print("%s():  Adding %p index %d to the denominator\n", __FUNCTION__, units->den[i], units->deni[i]);
#endif
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


/*
 * w - the GtkComboBoxText that corresponds to the individual
 *     component (like "Henry") of a units GUI
 *
 * data - the units gui
 */

void wc_units_menu_changed( GtkWidget *w, gpointer data)
{
  int which, ind;
  wc_units_gui *ug;
  wc_units_menu_data *menu_data;
  guint i;
  wc_units_update_item *up_item;

  /*
   * FIXME
   * there must be a better way than fixing this size.  At a
   * minimum need to use a limited prinf
   */
  char str[80];
  double sf;
  static char *units_str = NULL;

  assert(w != NULL);
  assert(data != NULL);

  ug = WC_UNITS_GUI (data);
  which = gtk_combo_box_get_active( GTK_COMBO_BOX(w));

#ifdef DEBUG
  g_print("\n\n%s(widget for the component = %p, units_gui = %p) which selected = %d\n", __FUNCTION__, w, data, which);
  g_print("%s():  ug = %p\n", __FUNCTION__, ug);
  g_print("%s():  ug->units = %p\n", __FUNCTION__, ug->units);
#endif

    /*
     * here we try to figure out if the combo box (w) is part of the
     * numerator and if so, which position in the numerator
     */
    if ( (ind = g_list_index(ug->menu_num, w)) != -1) {
#ifdef DEBUG
      g_print("%s():  ug %p changed numerator[%d] units to %d\n",
              __FUNCTION__, ug, ind, which);
      g_print("%s():  Previously: ug->units->numi[%d] = %d\n",
              __FUNCTION__, ind, ug->units->numi[ind]);
#endif
      ug->units->numi[ind] = which;
    } else if ( (ind = g_list_index(ug->menu_den, w)) != -1) {
#ifdef DEBUG
    g_print("%s():  ug %p changed denominator[%d] units to %d\n",
	    __FUNCTION__, ug, ind, which);
    g_print("%s():  Previously: ug->units->deni[%d] = %d\n",
            __FUNCTION__, ind, ug->units->deni[ind]);
#endif
    ug->units->deni[ind] = which;
  } else {
      /*
       * this seems to happen right at startup, ignore until I figure
       * out why
       */

      /*
        fprintf(stderr,"%s():  ug %p could not locate combo %p\n",
        __FUNCTION__, (void *) ug, w);
        exit(1);
      */

    return;
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
  g_print("%s():  Units string = \"%s\", scale factor = %g\n",
	  __FUNCTION__, units_str, sf);
#endif

  /*
   * Now update all the labels and text entries which may be tied to
   * these units
   */

  for (i=0; i<g_list_length(ug->update_list); i++) {
    up_item = (wc_units_update_item *) g_list_nth_data(ug->update_list,i);

#ifdef DEBUG
    g_print("%s():      processing item %u, type %d\n",
	    __FUNCTION__, i, up_item->type);
#endif

    /* update the scale factor and units string for this item */
    if (up_item->sf != NULL)
      *(up_item->sf)=sf;

    if (up_item->units_str != NULL)
      *(up_item->units_str) = strdup(units_str);

    /* only update the displayed widget if the update flag is set */
    if( (up_item->update) && (up_item->widget != NULL) ) {
#ifdef DEBUG
      g_print("%s():    Updating widget\n", __FUNCTION__);
#endif
      if(up_item->fmt_string != NULL) {
	sprintf(str, up_item->fmt_string, (*(up_item->mks_val))/sf);
      } else {
	sprintf(str, "--ERR--");
      }

      switch( up_item->type ) {
      case LABEL:
	gtk_label_set_text(GTK_LABEL(up_item->widget), str);
	break ;

      case ENTRY:
	gtk_entry_set_text(GTK_ENTRY(up_item->widget), str);
      break ;

      case UNITS_LABEL:
	gtk_label_set_text(GTK_LABEL(up_item->widget), units_str);
      break ;

      default:
	fprintf(stderr,"%s():  invalid up_item->type = %d\n",
		__FUNCTION__, up_item->type);
	exit(1);
      }
    } else {
#ifdef DEBUG
      g_print("%s():    not updating widget\n", __FUNCTION__);
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

/*
 * This function is used to create a single component of a units gui.
 * For example if we had units of Henry/meter, this function would
 * be called once for the "Henry" part and once for the "meter" part.
 *
 * wcgui - the main dialog gui (e.g. all of a microstrip gui)
 *
 * units - our units structure
 *
 * initial - index into which item in units is currently to be
 * selected
 *
 * gui - the units gui
 *
 * callback - the callback function for updating whatever the units
 * gui needed
 *
 */
static GtkWidget *wc_units_submenu_new(Wcalc *wcgui,
				       const wc_units_data *units,
				       int initial,
				       gpointer gui,
				       void (*callback)(GtkWidget *, gpointer))
{
  GtkWidget *combo;
  int i;


  //  GtkWidget *opt_menu;
  //  GtkWidget *menu;
  //  GtkWidget *item;
  //  wc_units_menu_data *data;


#ifdef DEBUG
  g_print("%s():  Create combo box\n", __FUNCTION__);
#endif

  combo = gtk_combo_box_text_new();

#ifdef DEBUG
  g_print("%s():    Connect combo box signals\n", __FUNCTION__);
#endif

  g_signal_connect (G_OBJECT(combo), "changed",
                    G_CALLBACK (wcalc_save_needed),
                    wcgui);

  g_signal_connect(G_OBJECT(combo), "changed",
                   G_CALLBACK(callback),
                   gui);

  g_signal_connect (GTK_OBJECT (combo), "changed",
                    G_CALLBACK (vals_changedCB),
                    wcgui);


  i=0;
  while (units[i].name != NULL) {
#ifdef DEBUG
    g_print("%s():    Add %s to the combo\n", __FUNCTION__, units[i].name);
#endif

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo), units[i].name);

    i++;
  }

  /* pick the default (initial) selection */
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo), initial);

  return combo;
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

/* $Id: menus.c,v 1.16 2009/01/13 14:36:08 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2009 Dan McMahill
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

#include "config.h"

#include <gtk/gtk.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "about.h"
#include "files.h"
#include "newprint.h"
#include "print.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/*static void window_close(GtkWidget *widget,
			 GdkEventAny *event,
			 gpointer data);
*/
static void window_close (gpointer data,
			  guint action,
			  GtkWidget *widget);


static gchar *wc_menu_translate(const gchar *path, gpointer data);

/* This is the GtkItemFactoryEntry structure used to generate new menus.
   Item 1: The menu path. The letter after the underscore indicates an
           accelerator key once the menu is open.
   Item 2: The accelerator key for the entry
   Item 3: The callback function.
   Item 4: The callback action.  This changes the parameters with
           which the function is called.  The default is 0.
   Item 5: The item type, used to define what kind of an item it is.
           Here are the possible values:

           NULL               -> "<Item>"
           ""                 -> "<Item>"
           "<Title>"          -> create a title item
           "<Item>"           -> create a simple item
           "<CheckItem>"      -> create a check item
           "<ToggleItem>"     -> create a toggle item
           "<RadioItem>"      -> create a radio item
           <path>             -> path of a radio item to link against
           "<Separator>"      -> create a separator
           "<Branch>"         -> create an item to hold sub items (optional)
           "<LastBranch>"     -> create a right justified branch 
*/

static GtkItemFactoryEntry static_menu_items[] = {
  { N_("/_File"),           NULL,          NULL,             0, "<Branch>" },
  { N_("/File/_New"),       NULL,          NULL,             0, "<Branch>" },
  { N_("/File/sep1"),       NULL,          NULL,             0, "<Separator>" },
  { N_("/File/_Open"),      "<control>O",  wcalc_open,       0, NULL },
  { N_("/File/_Save"),      "<control>S",  wcalc_save,       0, NULL },
  { N_("/File/Save _As"),   NULL,          wcalc_save_as,    0, NULL },
  { N_("/File/sep2"),       NULL,          NULL,             0, "<Separator>" },
#if GTK_CHECK_VERSION(2,10,0)
  { N_("/File/_Print"),     "<control>P",  newprint_popup,      0, NULL },
#else
  { N_("/File/_Print"),     "<control>P",  print_popup,      0, NULL },
#endif
  { N_("/File/sep3"),       NULL,          NULL,             0, "<Separator>" },
  { N_("/File/Close"),      "<control>W",  window_close,     0, NULL },
  { N_("/File/Quit"),       "<control>Q",  gtk_main_quit,    0, NULL },
  { N_("/_Options"),        NULL,          NULL,             0, "<Branch>" },
  { N_("/Options/tearoff"), NULL,          0,                0, "<Tearoff>"},
  { N_("/_Window"),         NULL,          NULL,             0, "<Branch>" },
  { N_("/Window/tearoff"),  NULL,          0,                0, "<Tearoff>"},
  { N_("/_Help"),           NULL,          NULL,             0, "<LastBranch>" },
  { N_("/_Help/About"),     NULL,          about_popup,      0, NULL },
  { N_("/_Help/Copyright"), NULL,          copyright_popup,  0, NULL },
  { N_("/_Help/_Material Properties"), NULL,NULL,            0, "<Branch>" },
  { N_("/_Help/_Material Properties/_Permeabilities"), 
                        NULL,          permeability_popup,0, NULL },
  { N_("/_Help/_Material Properties/_Permitivities"), 
                        NULL,          permitivity_popup,0, NULL },
  { N_("/_Help/_Material Properties/_Resistivities"), 
                        NULL,          resistivity_popup,0, NULL },
  { N_("/_Help/_Material Properties/_Units"), 
                        NULL,          units_popup,0, NULL },
  { N_("/_Help/_Material Properties/_Wire Sizes"), 
                        NULL,          AWG_popup,0, NULL },
};

static gchar *wc_menu_translate(const gchar *path, gpointer data)
{
  gchar *str;
  
  str = _(path);
  return str;
}

void get_main_menu( Wcalc *wcalc,
		    GtkWidget  *window,
                    GtkWidget **menubar )
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  static GtkItemFactoryEntry *menu_items=NULL;
  gint nmenu_items = sizeof (static_menu_items) / sizeof (static_menu_items[0]);
  static guint nmodels;
  int i;

  if (!menu_items){
    nmodels = g_list_length(global_model_names);
    menu_items = (GtkItemFactoryEntry *) 
      g_malloc((nmenu_items+nmodels)*sizeof(GtkItemFactoryEntry));
    
    /* copy over the static menu items */
    for (i=0; i<nmenu_items; i++){
      menu_items[i] = static_menu_items[i];
    }
    
    /* add in the models */
    for (i=0; i<nmodels; i++){
      menu_items[nmenu_items+i] = (GtkItemFactoryEntry)
	{g_list_nth_data(global_model_menus,i), NULL, wcalc_setup_cb, i, NULL};
    }
  }

  nmenu_items += nmodels;

  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.
     Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
              or GTK_TYPE_OPTION_MENU.
     Param 2: The path of the menu.
     Param 3: A pointer to a gtk_accel_group.  The item factory sets up
              the accelerator table while generating menus.
  */

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
				       accel_group);

  gtk_item_factory_set_translate_func(item_factory,
				      wc_menu_translate,
				      NULL,
				      NULL);

  /* This function generates the menu items. Pass the item factory,
     the number of items in the array, the array itself, and any
     callback data for the the menu items. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, wcalc);

  /* Attach the new accelerator group to the window. */
  /* XXX gtk-2.2 */
#if GTK_CHECK_VERSION(2,0,0)
  /* _gtk_accel_group_attach (accel_group, GTK_OBJECT (window)); */
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
#else
  gtk_accel_group_attach (accel_group, GTK_OBJECT (window));
#endif

  if (menubar){
    /* Finally, return the actual menu bar created by the item factory. */ 
    *menubar = gtk_item_factory_get_widget (item_factory, "<main>");
  }
}


static void  window_close (gpointer data,
			   guint action,
			   GtkWidget *widget)
{
  GtkWidget *window;

  window = ( (Wcalc *) data)->window;

#ifdef DEBUG
  printf(_("window_close():  Called with input pointer %p\n"),(void *) data);
  printf(_("window_close():  I will destroy window %p\n"),window);
#endif

  /* blow away the window */
  gtk_widget_destroy(window);

}


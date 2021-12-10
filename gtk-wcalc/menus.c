/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2009, 2021 Dan McMahill
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
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void  window_close (GtkAction *action, gpointer data);

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
#ifdef notdef

static GtkItemFactoryEntry static_menu_items[] = {
  { N_("/_File"),           NULL,          NULL,             0, "<Branch>" },
  { N_("/File/_New"),       NULL,          NULL,             0, "<Branch>" },
  { N_("/File/sep1"),       NULL,          NULL,             0, "<Separator>" },
  { N_("/File/_Open"),      "<control>O",  wcalc_open,       0, NULL },
  { N_("/File/_Save"),      "<control>S",  wcalc_save,       0, NULL },
  { N_("/File/Save _As"),   NULL,          wcalc_save_as,    0, NULL },
  { N_("/File/sep2"),       NULL,          NULL,             0, "<Separator>" },
  { N_("/File/_Print"),     "<control>P",  newprint_popup,   0, NULL },
  { N_("/File/_Export to PDF"),"<control>E",newprint_pdf_popup,  0, NULL },
  { N_("/File/Page Set_up"),NULL,          page_setup_popup,      0, NULL },
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
  /* _gtk_accel_group_attach (accel_group, GTK_OBJECT (window)); */
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

  if (menubar){
    /* Finally, return the actual menu bar created by the item factory. */
    *menubar = gtk_item_factory_get_widget (item_factory, "<main>");
  }
}

#endif

static void  window_close (GtkAction *action, gpointer data)
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



static const GtkActionEntry file_entries[]={
  { "FileMenu" , NULL, "_File"} ,
  { "NewMenu" , NULL, "_New"} ,
  { "Open" , GTK_STOCK_OPEN,
    "_Open" , "<control>O",
    "Open a file" , G_CALLBACK (wcalc_open)},
  { "Save" , GTK_STOCK_SAVE,
    "_Save" , "<control>S",
    "Save current file" , G_CALLBACK (wcalc_save)},
  { "SaveAs" , GTK_STOCK_SAVE_AS,
    "Save _As" , NULL,
    "Save to a new file" , G_CALLBACK (wcalc_save_as)},
  { "Print" , GTK_STOCK_PRINT,
    "Print" , "<control>P",
    "Save to a new file" , G_CALLBACK (newprint_popup)},
  { "ExportToPDF" , GTK_STOCK_PRINT,
    "Export to PDF" , "<control>E",
    "Export to a PDF file" , G_CALLBACK (newprint_pdf_popup)},
  { "PageSetup" , NULL,
    "Page Setup" , NULL,
    "Open the page setup dialog" , G_CALLBACK (page_setup_popup)},
  { "Close" , GTK_STOCK_CLOSE,
    "_Close" , "<control>W" ,
    "Close a file", G_CALLBACK (window_close)},
  { "Exit" , GTK_STOCK_QUIT,
    "E_xit", "<control>Q",
    "Exit the program", G_CALLBACK (gtk_main_quit)},
  { "HelpMenu" , NULL, "_Help"} ,
  { "About" , NULL,
    "About", NULL,
    "Show the about info", G_CALLBACK (about_popup)},
  { "Copyright" , NULL,
    "Copyright", NULL,
    "Show the copyright info", G_CALLBACK (copyright_popup)},
  { "MaterialsMenu" , NULL, "_Material Properties"} ,
  { "Permeability", NULL,
    "Per_meabilities", NULL,
    "Show a table of permeabilities of common materials.", G_CALLBACK (permeability_popup)},
  { "Permitivity", NULL,
    "P_ermitivities", NULL,
    "Show a table of permitivities of common materials.", G_CALLBACK (permitivity_popup)},
  { "Resistivity", NULL,
    "_Resistivities", NULL,
    "Show a table of resistivities of common materials.", G_CALLBACK (resistivity_popup)},
  { "Units", NULL,
    "_Units", NULL,
    "Show a units conversion table.", G_CALLBACK (units_popup)},
  { "WireSize", NULL,
    "_Wire Sizes", NULL,
    "Show a table of wire diameter vs AWG.", G_CALLBACK (AWG_popup)},
};

/* FIXME
 * - get the new menu insertion to work
 * - what about translations??
 * - should I put the XML in a string?
 */
void get_main_menu( Wcalc *wcalc,
		    GtkWidget  *window,
                    GtkWidget **menubar )
{
  GtkActionGroup *action_group;
  GError *error = NULL;
  GtkUIManager *ui_manager;
  GtkAccelGroup *accel_group;
  guint merge_id;

  *menubar = NULL;
  
  /* create actions and actions group */
  action_group = gtk_action_group_new ("Common_Actions");
  gtk_action_group_add_actions (action_group,
                                file_entries,
                                G_N_ELEMENTS (file_entries),
                                wcalc);
  //g_print("%s():  Created action group and added actions.  wcalc=%p\n", __FUNCTION__, wcalc);
  
  /* create UI manger */
  ui_manager = gtk_ui_manager_new();
  //g_print("%s():  Created UI manager: %p\n", __FUNCTION__, ui_manager);

  /* add the action group(s) to the UI manager */
  gtk_ui_manager_insert_action_group ( ui_manager,
                                       action_group,
                                       0);
  //g_print("%s():  Inserted action group\n", __FUNCTION__);

  /* extract accelerators from the UI manage and add to the top-level
     window */
  accel_group = gtk_ui_manager_get_accel_group (ui_manager);
  //g_print("%s():  Extracted accel_group = %p\n", __FUNCTION__, accel_group);

  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  //g_print("%s():  Added accel_group to window\n", __FUNCTION__);
  
  /* Load the UI */
  merge_id = gtk_ui_manager_add_ui_from_file(ui_manager, "menus.xml", &error);
  //g_print("%s():  Loaded UI from file, merge_id = %u, error = %p\n", __FUNCTION__, merge_id, error);
  
  /* 
     merge_id = gtk_ui_manager_add_ui_from_string(ui_manager,
     ui_string, -1, &error);
  */
  if( error != NULL) {
    g_critical("%s():  Failed to load UI: %s\n", __FUNCTION__, error->message);
    g_clear_error( &error );
    error = NULL;
    //exit(1);
    return;
  }  else if ( merge_id == 0) {
    g_critical("%s():  Failed to merge UI: %s\n", __FUNCTION__, error->message);
    g_clear_error( &error );
    error = NULL;
    //exit(1);
    return;
  }


  /* retrieve the widgets */
  *menubar = gtk_ui_manager_get_widget (ui_manager, "/MainMenu");
}
